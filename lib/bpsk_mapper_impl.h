/* -*- c++ -*- */
/*
 * Copyright 2026 minh.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_OFDM_TESTBED_BPSK_MAPPER_IMPL_H
#define INCLUDED_OFDM_TESTBED_BPSK_MAPPER_IMPL_H

#include <gnuradio/ofdm_testbed/bpsk_mapper.h>
#include <vector>

namespace gr {
namespace ofdm_testbed {

    class bpsk_mapper_impl : public bpsk_mapper
    {
    private:
        std::vector<gr_complex> d_symbols; 

    public:
        bpsk_mapper_impl();
        ~bpsk_mapper_impl();

        void forecast(int noutput_items, gr_vector_int &ninput_items_required) override;

        int general_work(int noutput_items,
                         gr_vector_int &ninput_items_v,
                         gr_vector_const_void_star &input_items,
                         gr_vector_void_star &output_items) override;
    };

} // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_BPSK_MAPPER_IMPL_H */
