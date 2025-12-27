/* -*- c++ -*- */
/*
 * Copyright 2025 gr-ofdm_testbed author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "ber_impl.h"
#include <gnuradio/io_signature.h>
#include <iostream>

namespace gr {
namespace ofdm_testbed {

ber::sptr ber::make(int avg_len, bool enable_output)
{
    return gnuradio::make_block_sptr<ber_impl>(avg_len, enable_output);
}

/*
 * Constructor
 */
ber_impl::ber_impl(int avg_len, bool enable_output)
    : gr::sync_block("ber",
          // Input: 2 byte ports (Reference, Received)
          gr::io_signature::make(2, 2, sizeof(uint8_t)),
          // Output: 1 float port (BER) if enabled, otherwise 0
          gr::io_signature::make(enable_output ? 1 : 0, enable_output ? 1 : 0, sizeof(float))),
      d_avg_len(avg_len),
      d_enable_output(enable_output),
      d_bits_index(0),
      d_error_count(0),
      d_current_ber(0.0f)
{
}

ber_impl::~ber_impl() {}

/*
 * Callback to update average length at runtime
 */
void ber_impl::set_avg_len(int avg_len)
{
    d_avg_len = avg_len;
    // Reset counters to apply new setting immediately
    d_bits_index = 0;
    d_error_count = 0;
}

/*
 * Work Function
 */
int ber_impl::work(int noutput_items,
                   gr_vector_const_void_star& input_items,
                   gr_vector_void_star& output_items)
{
    const uint8_t* in_ref = static_cast<const uint8_t*>(input_items[0]);
    const uint8_t* in_rx = static_cast<const uint8_t*>(input_items[1]);
    
    // Output pointer (only assigned if output is enabled)
    float* out = d_enable_output ? static_cast<float*>(output_items[0]) : nullptr;

    for (int i = 0; i < noutput_items; i++) {
        
        if (in_ref[i] ^ in_rx[i]) {
            d_error_count++;
        }
        
        d_bits_index++;

        
        if (d_bits_index >= (uint64_t)d_avg_len) {
            d_current_ber = (float)d_error_count / (float)d_avg_len;
            
           
            if (!d_enable_output) {
               std::cout << "BER: " << d_current_ber 
                         << " (" << d_error_count << "/" << d_avg_len << ")" << std::endl;
            }


            d_error_count = 0;
            d_bits_index = 0;
        }

        
        if (d_enable_output) {
            out[i] = d_current_ber;
        }
    }

    return noutput_items;
}

} /* namespace ofdm_testbed */
} /* namespace gr */