/* -*- c++ -*- */
/*
 * Copyright 2026 minh.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_OFDM_TESTBED_ZERO_ELIMINATION_IMPL_H
#define INCLUDED_OFDM_TESTBED_ZERO_ELIMINATION_IMPL_H

#include <gnuradio/ofdm_testbed/zero_elimination.h>

namespace gr {
  namespace ofdm_testbed {

    class zero_elimination_impl : public zero_elimination
    {
     private:
      int d_node_id;
      int d_data_len;
      int d_data_index;
      std::vector<unsigned char> d_data;
      
      enum state_t
      {
          ST_ZERO_ELIMINATE,
          ST_DATA_READ
      };
      state_t d_state;

     public:
      zero_elimination_impl(int node_id, int data_len);
      ~zero_elimination_impl();

      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
                       gr_vector_int &ninput_items_v,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items);
    };

  } // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_ZERO_ELIMINATION_IMPL_H */
