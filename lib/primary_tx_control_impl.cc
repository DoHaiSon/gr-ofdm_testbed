/* -*- c++ -*- */
/*
 * Copyright 2026 minh.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "primary_tx_control_impl.h"
#include <gnuradio/io_signature.h>
#include <iostream>
#include <stdexcept>

namespace gr {
namespace ofdm_testbed {

    primary_tx_control::sptr
    primary_tx_control::make(int packet_sz, int guard_interval, int source_id, int destination_id, int ack_burst_size)
    {
        return gnuradio::make_block_sptr<primary_tx_control_impl>(packet_sz, guard_interval, source_id, destination_id, ack_burst_size);
    }

    primary_tx_control_impl::primary_tx_control_impl(int packet_sz, int guard_interval, int source_id, int destination_id, int ack_burst_size)
        : gr::block("primary_tx_control",
                    gr::io_signature::make2(2, 2, sizeof(unsigned char) * ack_burst_size, sizeof(unsigned char)),
                    gr::io_signature::make(1, 1, sizeof(unsigned char))),
          d_packet_size(packet_sz),
          d_guard_interval(guard_interval),
          d_transmitter_id(static_cast<unsigned char>(source_id)),
          d_receiver_id(static_cast<unsigned char>(destination_id)),
          d_ack_burst_size(ack_burst_size),
          d_buffer_size(255),
          d_load_packet_index(0),
          d_load_data_index(0),
          d_load_packet_number(0x01),
          d_load_data(false),
          d_tx_buffer_index(0),
          d_header_index(0),
          d_data_index(0),
          d_guard_index(0),
          d_check_rx_id_count(0),
          d_check_rx_id_index(0),
          d_check_recv_packet_number_index(0),
          d_number_of_session_packets(0),
          d_expected_session_number(0x01),
          d_check_session_index(0),
          d_check_session_count(0),
          d_loaded_data(false),
          d_tx_state(ST_WAIT),
          d_rx_state(ST_RECEIVE)
    {
        if (source_id == 0x00)
            throw std::runtime_error("Invalid parameter! source ID must be different from 0...!!!\n");
        if (destination_id == 0x00)
            throw std::runtime_error("Invalid parameter! destination ID must be different from 0...!!!\n");
        if (ack_burst_size != 6)
            throw std::runtime_error("Invalid parameter! acknowlege burst size must be 6...!!!\n");

        
        d_confirm_packet_number.resize(d_buffer_size);
        std::fill(d_confirm_packet_number.begin(), d_confirm_packet_number.end(), 0x00);

        d_loaded_packet_number.resize(d_buffer_size);
        std::fill(d_loaded_packet_number.begin(), d_loaded_packet_number.end(), 0x00);

        d_transmitted_packet_number.resize(d_buffer_size);
        std::fill(d_transmitted_packet_number.begin(), d_transmitted_packet_number.end(), 0x00);

        d_transmitted_data_buffer.resize(d_buffer_size * packet_sz);
        std::fill(d_transmitted_data_buffer.begin(), d_transmitted_data_buffer.end(), 0x00);

        for (int i = 0; i < 3; i++) d_rx_packet_number[i] = 0x00;
    }

    primary_tx_control_impl::~primary_tx_control_impl() {}

    void primary_tx_control_impl::forecast(int noutput_items, gr_vector_int &ninput_items_required)
    {
       
        ninput_items_required[0] = 0; // ACK: không bắt buộc phải có
        ninput_items_required[1] = 0; // Data: không bắt buộc phải có
    }

    void primary_tx_control_impl::reset()
    {
        for (int k = 0; k < d_buffer_size; k++) {
            d_loaded_packet_number[k] = 0x00;
            d_transmitted_packet_number[k] = 0x00;
            d_confirm_packet_number[k] = 0x00;
        }
        d_number_of_session_packets = 0;
        d_tx_buffer_index = 0;
        d_header_index = 0;
        d_data_index = 0;
        d_guard_index = 0;
        d_tx_state = ST_WAIT;
    }

    int primary_tx_control_impl::general_work(int noutput_items,
                                              gr_vector_int &ninput_items_v,
                                              gr_vector_const_void_star &input_items,
                                              gr_vector_void_star &output_items)
    {
        int nInputItems0 = ninput_items_v[0]; // ACK
        int nInputItems1 = ninput_items_v[1]; // DATA

        const unsigned char *in_ack  = (const unsigned char *) input_items[0];
        const unsigned char *in_data = (const unsigned char *) input_items[1];

        unsigned char *out = (unsigned char *) output_items[0];

        int ni0 = 0;
        int ni1 = 0;
        int no  = 0;

        int nidx_ack = 0;
        int i = 0;
        int j = 0;

        
        while (ni0 < nInputItems0)
        {
            if (d_load_data == true)
            {
                // Đang nạp dữ liệu từ in_data vào transmit buffer
                if (ni1 < nInputItems1)
                {
                    if (d_load_data_index == 0)
                    {
                        // Byte đầu tiên của mỗi gói là flag:
                        // 0x01 = kết thúc session, ngừng nạp
                        if (in_data[ni1] == 0x01)
                        {
                            std::cout << "[DEBUG] end data" << std::endl;
                            d_number_of_session_packets--;
                            std::cout << "d_number_of_session_packets = " << d_number_of_session_packets << "\n";
                            d_load_packet_number = 0x01;
                            d_load_packet_index  = 0;
                            d_load_data_index    = 0;
                            d_load_data          = false;
                            d_loaded_data        = true;
                        }
                        d_load_data_index++;
                        ni1++;
                    }
                    else
                    {
                        int idx = d_load_packet_index * d_packet_size + d_load_data_index - 1;
                        // FIX 4 (giữ lại từ file mới): bounds check trước khi ghi
                        if (idx < (int)d_transmitted_data_buffer.size()) {
                            d_transmitted_data_buffer[idx] = in_data[ni1];
                        }
                        d_load_data_index++;
                        ni1++;

                        if (d_load_data_index == (d_packet_size + 1))
                        {
                            d_number_of_session_packets++;
                            d_loaded_packet_number[d_load_packet_index] = d_load_packet_number;
                            d_load_packet_index++;
                            d_load_packet_number++;
                            d_load_data_index = 0;
                            if (d_load_packet_index == d_buffer_size)
                            {
                                d_load_packet_number = 0x01;
                                d_load_packet_index  = 0;
                                std::cout << "d_number_of_session_packets = " << d_number_of_session_packets << "\n";
                                d_load_data   = false;
                                d_loaded_data = true;
                            }
                        }
                    }
                }
                // Nếu không có data item, bỏ qua ACK item này và thử lần sau
                else
                {
                    break;
                }
            }
            else
            {
                // Xử lý ACK burst từ receiver
                switch (d_rx_state)
                {
                case ST_RECEIVE:
                {
                    // Bước 1: Kiểm tra 3 byte đầu có khớp receiver_id không (voting 3/3)
                    for (j = 0; j < 3; j++) {
                        if (in_ack[nidx_ack + j] == (unsigned char)d_receiver_id)
                            d_check_rx_id_count++;
                    }

                    if (d_check_rx_id_count == 3)
                    {
                        // Bước 2: Lấy số gói từ 3 byte cuối
                        for (j = 3; j < 6; j++)
                            d_rx_packet_number[j - 3] = in_ack[nidx_ack + j];

                        unsigned char a = d_rx_packet_number[0];
                        unsigned char b = d_rx_packet_number[1];
                        unsigned char c = d_rx_packet_number[2];
                        unsigned char rx_pkt_no = 0x00;
                        bool recv_pkt = false;

                        // Majority voting: lấy giá trị có ít nhất 2/3 phiếu
                        if      (a == b) { rx_pkt_no = a; recv_pkt = true; }
                        else if (b == c) { rx_pkt_no = b; recv_pkt = true; }
                        else if (c == a) { rx_pkt_no = c; recv_pkt = true; }

                        if (recv_pkt)
                        {
                            if (rx_pkt_no == 0x00)
                            {
                                // PKT_NO = 0 → receiver báo hiệu bắt đầu phiên mới
                                d_rx_state = ST_CHECK_SESSION_NUMBER;
                            }
                            else
                            {
                                // Xác nhận gói đã được receiver nhận đúng
                                int pkt_idx = (int)rx_pkt_no - 1;
                                if (pkt_idx >= 0 && pkt_idx < d_buffer_size) {
                                    if (d_transmitted_packet_number[pkt_idx] == rx_pkt_no) {
                                        d_confirm_packet_number[pkt_idx] = rx_pkt_no;
                                    }
                                }
                            }
                        }
                    }
                    d_check_rx_id_count = 0;
                    nidx_ack += d_ack_burst_size;
                    ni0++;
                    break;
                }

                case ST_CHECK_SESSION_NUMBER:
                {
                    // Đếm số byte trong burst khớp với session number đang chờ
                    for (i = 0; i < 6; i++) {
                        if (in_ack[nidx_ack + i] == d_expected_session_number)
                            d_check_session_count++;
                    }

                    if (d_check_session_count > 4)
                    {
                        // Chấp nhận phiên mới, cập nhật expected session number
                        if (d_expected_session_number == 0xFF)
                            d_expected_session_number = 0x01;
                        else
                            d_expected_session_number++;

                        std::cout << "Received session request!\n";
                        d_load_data   = true;
                        d_loaded_data = false;
                        reset();
                    }
                    d_check_session_count = 0;
                    d_rx_state = ST_RECEIVE;
                    nidx_ack += d_ack_burst_size;
                    ni0++;
                    break;
                }
                } // end switch rx
            }
        } // end while RX 

        
        while (no < noutput_items)
        {
            switch (d_tx_state)
            {
            case ST_WAIT:
            {
                if (d_loaded_data == true)
                    d_tx_state = ST_BUFFER_MANAGEMENT;
                out[no] = 0x00;
                no++;
                break;
            }

            case ST_BUFFER_MANAGEMENT:
            {
                // Tìm gói tiếp theo chưa được xác nhận (confirm = 0x00)
                if (d_confirm_packet_number[d_tx_buffer_index] == 0x00)
                {
                    d_tx_state = ST_TRANSMITTER_ID_TRANS;
                }
                else
                {
                    d_tx_buffer_index++;
                    if (d_tx_buffer_index == d_number_of_session_packets)
                        d_tx_buffer_index = 0;
                }
                out[no] = 0x00;
                no++;
                break;
            }

            case ST_TRANSMITTER_ID_TRANS:
            {
                // Phát source ID 3 lần liên tiếp vào header
                out[no] = (unsigned char)d_transmitter_id;
                d_header_index++;
                no++;
                if (d_header_index == 3) {
                    d_tx_state    = ST_PACKET_NO_TRANS;
                    d_header_index = 0;
                }
                break;
            }

            case ST_PACKET_NO_TRANS:
            {
                // Phát số gói 3 lần liên tiếp vào header
                out[no] = d_loaded_packet_number[d_tx_buffer_index];
                d_transmitted_packet_number[d_tx_buffer_index] = d_loaded_packet_number[d_tx_buffer_index];
                d_header_index++;
                no++;
                if (d_header_index == 3) {
                    printf("transmitted packet no %i\n", (int)d_loaded_packet_number[d_tx_buffer_index]);
                    d_tx_state    = ST_DATA_TRANS;
                    d_header_index = 0;
                }
                break;
            }

            case ST_DATA_TRANS:
            {
                // Phát payload của gói hiện tại
                int idx = d_tx_buffer_index * d_packet_size + d_data_index;
                out[no] = d_transmitted_data_buffer[idx];
                d_data_index++;
                no++;
                if (d_data_index == d_packet_size) {
                    d_tx_state   = ST_GUARD_INTERVAL_TRANS;
                    d_data_index = 0;
                }
                break;
            }

            case ST_GUARD_INTERVAL_TRANS:
            {
                // Phát các byte 0x00 làm guard interval giữa các gói
                out[no] = 0x00;
                d_guard_index++;
                no++;
                if (d_guard_index == d_guard_interval) {
                    d_tx_state    = ST_BUFFER_MANAGEMENT;
                    d_guard_index = 0;
                    d_tx_buffer_index++;
                    if (d_tx_buffer_index == d_number_of_session_packets)
                        d_tx_buffer_index = 0;
                }
                break;
            }
            } // end switch tx
        } // end while TX

        consume(0, ni0);
        consume(1, ni1);

        return no;
    }

} /* namespace ofdm_testbed */
} /* namespace gr */