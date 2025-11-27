/* -*- c++ -*- */
/*
 * Copyright 2025 gr-ofdm_testbed author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_OFDM_TESTBED_BER_IMPL_H
#define INCLUDED_OFDM_TESTBED_BER_IMPL_H

#include <gnuradio/ofdm_testbed/ber.h>

namespace gr {
namespace ofdm_testbed {

class ber_impl : public ber
{
private:
    int d_avg_len;              // Number of bits for averaging
    bool d_enable_output;       // Enable output stream
    
    std::vector<uint8_t> d_ref_buffer;  // Buffer for reference bits
    std::vector<uint8_t> d_rx_buffer;   // Buffer for received bits
    
    int d_total_bits;           // Total bits processed
    int d_error_bits;           // Error bits in current window
    float d_ber;                // Current BER value
    
    // For periodic console logging when output is disabled
    std::chrono::steady_clock::time_point d_last_log_time;
    const double d_log_interval_sec;    // Log interval in seconds (0.1s)

public:
    ber_impl(int avg_len, bool enable_output);
    ~ber_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_BER_IMPL_H */
