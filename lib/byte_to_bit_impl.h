/* -*- c++ -*- */
/*
 * Copyright 2026 minh.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef INCLUDED_OFDM_TESTBED_BYTE_TO_BIT_IMPL_H
#define INCLUDED_OFDM_TESTBED_BYTE_TO_BIT_IMPL_H

#include <gnuradio/ofdm_testbed/byte_to_bit.h>

namespace gr {
namespace ofdm_testbed {

    class byte_to_bit_impl : public byte_to_bit
    {
    public:
        byte_to_bit_impl();
        ~byte_to_bit_impl();

        
        void forecast(int noutput_items, gr_vector_int &ninput_items_required) override;

        int general_work(int noutput_items,
                         gr_vector_int &ninput_items_v,
                         gr_vector_const_void_star &input_items,
                         gr_vector_void_star &output_items) override;
    };

} // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_BYTE_TO_BIT_IMPL_H */
