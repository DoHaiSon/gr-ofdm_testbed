/* -*- c++ -*- */
/*
 * Copyright 2025 gr-ofdm_testbed author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "byte_to_bit_impl.h"
#include <gnuradio/io_signature.h>
#include <iostream>

namespace gr {
namespace ofdm_testbed {

    byte_to_bit::sptr byte_to_bit::make()
    {
        return gnuradio::make_block_sptr<byte_to_bit_impl>();
    }

    /*
     * Constructor
     */
    byte_to_bit_impl::byte_to_bit_impl()
        : gr::block("byte_to_bit",
                    gr::io_signature::make(1, 1, sizeof(unsigned char)), // Input: Byte
                    gr::io_signature::make(1, 1, sizeof(unsigned char))) // Output: Byte (0 or 1)
    {
        
        set_output_multiple(8);
    }

    byte_to_bit_impl::~byte_to_bit_impl() {}

    void byte_to_bit_impl::forecast(int noutput_items, gr_vector_int &ninput_items_required)
    {
        
        int ninput = (noutput_items - (noutput_items % 8)) / 8;
        ninput_items_required[0] = ninput;
    }

    int byte_to_bit_impl::general_work(int noutput_items,
                                       gr_vector_int &ninput_items_v,
                                       gr_vector_const_void_star &input_items,
                                       gr_vector_void_star &output_items)
    {
        int ninput_items = ninput_items_v[0];
        const unsigned char *in = (const unsigned char *) input_items[0];
        unsigned char *out = (unsigned char *) output_items[0];

        int ni = 0; // index input
        int no = 0; // index output
        while (ni < ninput_items && no + 8 <= noutput_items)
        {
          
            for (int i = 0; i < 8; i++)
            {
                out[no] = ((in[ni] >> i) & 0x1);
                no++;
            }
            ni++;
        }

        consume_each(ni);
        return no;
    }

} /* namespace ofdm_testbed */
} /* namespace gr */