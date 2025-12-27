/* -*- c++ -*- */
/*
 * Copyright 2025 gr-ofdm_testbed author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_OFDM_TESTBED_BER_IMPL_H
#define INCLUDED_OFDM_TESTBED_BER_IMPL_H

#include <gnuradio/ofdm_testbed/ber.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace ofdm_testbed {


class ber_impl : public ber
{
private:
    int d_avg_len;              
    bool d_enable_output;       
    
    
    uint64_t d_bits_index;      
    uint64_t d_error_count;     
    
    float d_current_ber;       

public:
    ber_impl(int avg_len, bool enable_output);
    ~ber_impl();

    void set_avg_len(int avg_len) override;
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_BER_IMPL_H */
