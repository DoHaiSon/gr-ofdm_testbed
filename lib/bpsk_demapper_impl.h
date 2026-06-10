/* -*- c++ -*- */
/*
 * Copyright 2026 minh.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_OFDM_TESTBED_BPSK_DEMAPPER_IMPL_H
#define INCLUDED_OFDM_TESTBED_BPSK_DEMAPPER_IMPL_H

#include <gnuradio/ofdm_testbed/bpsk_demapper.h>

namespace gr {
namespace ofdm_testbed {

    class bpsk_demapper_impl : public bpsk_demapper
    {
    private:
        int d_i;        
        char d_byte;    
        
        int d_one;
        int d_zero;

    public:
        bpsk_demapper_impl();
        ~bpsk_demapper_impl();

        
        void forecast(int noutput_items, gr_vector_int &ninput_items_required) override;

        int general_work(int noutput_items, 
                         gr_vector_int &ninput_items_v,
                         gr_vector_const_void_star &input_items,
                         gr_vector_void_star &output_items) override;
    };

} // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_BPSK_DEMAPPER_IMPL_H */
