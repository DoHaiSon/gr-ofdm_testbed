/* -*- c++ -*- */
/*
 * Copyright 2025 gr-ofdm_testbed minh.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_OFDM_TESTBED_PRIMARY_TX_CONTROL_IMPL_H
#define INCLUDED_OFDM_TESTBED_PRIMARY_TX_CONTROL_IMPL_H

#include <gnuradio/ofdm_testbed/primary_tx_control.h>
#include <vector>

namespace gr {
namespace ofdm_testbed {

    class primary_tx_control_impl : public primary_tx_control
    {
    private:
        enum state_tx {
            ST_WAIT,
            ST_BUFFER_MANAGEMENT,
            ST_TRANSMITTER_ID_TRANS,
            ST_PACKET_NO_TRANS,
            ST_DATA_TRANS,
            ST_GUARD_INTERVAL_TRANS
        };
        enum state_rx {
            ST_RECEIVE,
            ST_CHECK_SESSION_NUMBER
        };
        
        int d_packet_size;
        int d_guard_interval;
        int d_transmitter_id;
        int d_receiver_id;
        int d_ack_burst_size;
        
        int d_buffer_size;
        
        int d_load_packet_index;
        int d_load_data_index;
        unsigned char d_load_packet_number;
        bool d_load_data;
        
        int d_tx_buffer_index;
        int d_header_index;
        int d_data_index;
        int d_guard_index;
        int d_check_rx_id_count;
        int d_check_rx_id_index;
        int d_check_recv_packet_number_index;
        int d_number_of_session_packets;
        unsigned char d_expected_session_number;
        int d_check_session_index;
        int d_check_session_count;
        
        bool d_loaded_data;  
        state_tx d_tx_state;
        state_rx d_rx_state;

        std::vector<unsigned char> d_transmitted_data_buffer;
        std::vector<unsigned char> d_loaded_packet_number;
        std::vector<unsigned char> d_transmitted_packet_number;
        std::vector<unsigned char> d_confirm_packet_number;
        
        unsigned char d_rx_packet_number[3];
        
        void reset();

    public:
        
        primary_tx_control_impl(int packet_sz, int guard_interval, int source_id, int destination_id, int ack_burst_size);
        ~primary_tx_control_impl();

        void forecast(int noutput_items, gr_vector_int &ninput_items_required) override;

        int general_work(int noutput_items,
                         gr_vector_int &ninput_items_v,
                         gr_vector_const_void_star &input_items,
                         gr_vector_void_star &output_items) override;
    };

} // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_PRIMARY_TX_CONTROL_IMPL_H */
