#ifndef INCLUDED_OFDM_TESTBED_RECEIVER_CONTROL_P2P_IMPL_H
#define INCLUDED_OFDM_TESTBED_RECEIVER_CONTROL_P2P_IMPL_H

// S?A 1: Include dúng file header c?a module hi?n t?i
#include <gnuradio/ofdm_testbed/receiver_control_p2p.h>
#include <vector>

namespace gr {
  namespace ofdm_testbed { // S?A 2: Ð?i Namespace t? DNC -> ofdm_testbed

    class receiver_control_p2p_impl : public receiver_control_p2p
    {
     private:
        enum state_rx
        {
            ST_IDLE,
            ST_CHECK_TRANSMITTER_ID,
            ST_CHECK_PACKET_NO,
            ST_GET_DATA,
            ST_CHECK_END_PACKET
        };
        enum state_tx
        {
            ST_HEADER_TRANS,
            ST_SESSION_REQUEST_TRANS,
            ST_CHECK_PACKET,
            ST_CONFIRM_TRANS,
            ST_CHECK_DATA_OUT,
            ST_RECVED_DATA_OUT
        };
        
        // S?A 3: S?p x?p l?i bi?n cho kh?p v?i Constructor (Kh? Warning Reorder)
        state_tx d_tx_state;
        state_rx d_rx_state;

        int d_packet_size;
        unsigned char d_transmitter_id;
        unsigned char d_receiver_id;

        int d_buffer_size;
        int d_curr_part; // Bi?n này trong code cu không dùng? Nhung c? gi? l?i
        
        int d_check_transmitter_id_index;
        int d_check_transmitter_id_count;
        int d_check_recv_packet_number_index;
        
        unsigned char d_rx_pkt_no;
        int d_data_index;
        int d_check_end_packet_count;
        int d_check_end_packet_index;
        
        unsigned char d_next_out_packet_no;
        int d_confirm_index;
        int d_pkt_no_confirm_index;
        int d_count_recvd_packet; // Ð?o v? trí cho kh?p .cc
        
        int d_data_out_index;
        bool d_recv_packet;
        int d_header_index;
        int d_session_request_index;
        
        unsigned char d_session_no;
        bool d_session_request_trans;
        bool d_data_out;
        bool d_data_out_full;
        
        std::vector<unsigned char> d_received_data_buffer;
        std::vector<unsigned char> d_received_packet_number_buffer;
        std::vector<unsigned char> d_rx_packet;
        unsigned char d_rx_packet_number[3];

     public:
      receiver_control_p2p_impl(int packet_size, int transmitter_id, int receiver_id);
      ~receiver_control_p2p_impl();

      void forecast (int noutput_items, gr_vector_int &ninput_items_required) override;
    
      int general_work(int noutput_items,
               gr_vector_int &ninput_items,
               gr_vector_const_void_star &input_items,
               gr_vector_void_star &output_items) override;
    };

  } // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_RECEIVER_CONTROL_P2P_IMPL_H */
