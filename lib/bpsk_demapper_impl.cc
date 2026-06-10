/* -*- c++ -*- */
/*
 * Copyright 2026 minh.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "bpsk_demapper_impl.h"
#include <cstdio>
#include <iostream>

namespace gr {
namespace ofdm_testbed {

    bpsk_demapper::sptr bpsk_demapper::make()
    {
        return gnuradio::make_block_sptr<bpsk_demapper_impl>();
    }

    /*
     * The private constructor
     */
    bpsk_demapper_impl::bpsk_demapper_impl()
        : gr::block("bpsk_demapper",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),    // Input: Complex symbol
              gr::io_signature::make(1, 1, sizeof(unsigned char))), // Output: Byte
          d_i(0), 
          d_byte(0),
          d_one(0),
          d_zero(0)
    {
        
    }

    bpsk_demapper_impl::~bpsk_demapper_impl()
    {
    }

    void bpsk_demapper_impl::forecast(int noutput_items, gr_vector_int &ninput_items_required)
    {
        
        ninput_items_required[0] = 8 * noutput_items;
    }

    int bpsk_demapper_impl::general_work(int noutput_items,
                                         gr_vector_int &ninput_items_v,
                                         gr_vector_const_void_star &input_items,
                                         gr_vector_void_star &output_items)
    {
        int ninput_items = ninput_items_v[0];
        const gr_complex *in = (const gr_complex *) input_items[0];
        unsigned char *out = (unsigned char *) output_items[0];

        int ni = 0;
        int no = 0;

       
        while (ni < ninput_items && no < noutput_items)
        {
            
            if (in[ni].real() > 0)
            {
                
                d_byte |= (0x1 << d_i);
                d_one++;
            }
            else
            {
                d_zero++;
            }

            d_i++;
            ni++;

            
            if (d_i % 8 == 0)
            {
                out[no] = d_byte;
                
                
                d_i = 0;
                d_byte = 0;
                no++;
            }
        }

        
        consume_each(ni);
        
        
        return no;
    }

} /* namespace ofdm_testbed */
} /* namespace gr */