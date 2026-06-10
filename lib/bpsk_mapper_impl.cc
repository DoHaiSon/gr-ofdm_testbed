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
#include "bpsk_mapper_impl.h"
#include <iostream>

namespace gr {
namespace ofdm_testbed {

    bpsk_mapper::sptr
    bpsk_mapper::make()
    {
        
        return gnuradio::make_block_sptr<bpsk_mapper_impl>();
    }

    bpsk_mapper_impl::bpsk_mapper_impl()
      : gr::block("bpsk_mapper",
              gr::io_signature::make(1, 1, sizeof(unsigned char)), // Input: Byte (0 or 1)
              gr::io_signature::make(1, 1, sizeof(gr_complex))),   // Output: Complex
        d_symbols(2, gr_complex(0, 0))
    {
        // std::cout << "init::bpsk_mapper (OFDM Testbed)" << std::endl;
        
        
        // Bit 0 -> -1 + 0j
        // Bit 1 -> +1 + 0j
        d_symbols[0] = gr_complex(-1, 0); 
        d_symbols[1] = gr_complex(+1, 0);
    }

    bpsk_mapper_impl::~bpsk_mapper_impl()
    {
    }

    void
    bpsk_mapper_impl::forecast(int noutput_items, gr_vector_int &ninput_items_required)
    {
        
        unsigned ninputs = ninput_items_required.size();
        for (unsigned i = 0; i < ninputs; i++)
            ninput_items_required[i] = noutput_items;
    }

    int
    bpsk_mapper_impl::general_work(int noutput_items,
                                   gr_vector_int &ninput_items_v,
                                   gr_vector_const_void_star &input_items,
                                   gr_vector_void_star &output_items)
    {
        int ninput_items = ninput_items_v[0];
        const unsigned char *in = (const unsigned char *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        int ni = 0;
        int no = 0;
        unsigned char bit = 0;

        
        while (ni < ninput_items && no < noutput_items)
        {
            
            bit = in[ni] & 0x1;
            
            
            out[no] = d_symbols[bit];
            
            ni++;
            no++;
        }

        consume_each(ni);
        return no;
    }

} /* namespace ofdm_testbed */
} /* namespace gr */

