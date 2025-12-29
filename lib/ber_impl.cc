/* -*- c++ -*- */
/*
 * Copyright 2025 gr-ofdm_testbed author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "ber_impl.h"
#include <gnuradio/io_signature.h>
#include <iostream>
#include <chrono>

namespace gr {
namespace ofdm_testbed {

using input_type = uint8_t;
using output_type = float;

ber::sptr ber::make(int avg_len, bool enable_output)
{
    return gnuradio::make_block_sptr<ber_impl>(avg_len, enable_output);
}


/*
 * The private constructor
 */
ber_impl::ber_impl(int avg_len, bool enable_output)
    : gr::block("ber",
                gr::io_signature::make(
                    2 /* min inputs */, 2 /* max inputs */, sizeof(input_type)),
                gr::io_signature::make(
                    enable_output ? 1 : 0 /* min outputs */,
                    enable_output ? 1 : 0 /* max outputs */,
                    sizeof(output_type))),
      d_avg_len(avg_len),
      d_enable_output(enable_output),
      d_total_bits(0),
      d_error_bits(0),
      d_ber(0.0f),
      d_log_interval_sec(0.1)  // 0.1 second log interval
{
    // Reserve space for buffers to avoid frequent reallocations
    d_ref_buffer.reserve(avg_len);
    d_rx_buffer.reserve(avg_len);

    // Initialize last log time
    d_last_log_time = std::chrono::steady_clock::now();
}

/*
 * Our virtual destructor.
 */
ber_impl::~ber_impl() {}

void ber_impl::forecast(int noutput_items, gr_vector_int& ninput_items_required)
{
    // We need the same number of items from both inputs
    // For general_work, we process as many items as available
    unsigned ninputs = ninput_items_required.size();
    for (unsigned i = 0; i < ninputs; i++) {
        ninput_items_required[i] = noutput_items;
    }
}

int ber_impl::general_work(int noutput_items,
                           gr_vector_int& ninput_items,
                           gr_vector_const_void_star& input_items,
                           gr_vector_void_star& output_items)
{
    const uint8_t* in_ref = static_cast<const uint8_t*>(input_items[0]);
    const uint8_t* in_rx = static_cast<const uint8_t*>(input_items[1]);

    // Determine how many items to process
    int items_to_process = std::min(ninput_items[0], ninput_items[1]);

    // If output is enabled, limit by available output space
    if (d_enable_output) {
        items_to_process = std::min(items_to_process, noutput_items);
    }

    if (items_to_process == 0) {
        return 0;
    }

    // Add new bits to buffers
    for (int i = 0; i < items_to_process; i++) {
        d_ref_buffer.push_back(in_ref[i]);
        d_rx_buffer.push_back(in_rx[i]);
    }

    // If buffer exceeds avg_len, remove old bits from the front
    if (d_ref_buffer.size() > static_cast<size_t>(d_avg_len)) {
        int excess = d_ref_buffer.size() - d_avg_len;
        d_ref_buffer.erase(d_ref_buffer.begin(), d_ref_buffer.begin() + excess);
        d_rx_buffer.erase(d_rx_buffer.begin(), d_rx_buffer.begin() + excess);
    }

    // Calculate BER when we have enough bits
    if (d_ref_buffer.size() >= static_cast<size_t>(d_avg_len)) {
        d_error_bits = 0;
        for (size_t i = 0; i < d_ref_buffer.size(); i++) {
            if (d_ref_buffer[i] != d_rx_buffer[i]) {
                d_error_bits++;
            }
        }
        d_ber = static_cast<float>(d_error_bits) / d_ref_buffer.size();

        // Print BER to console only when output is disabled
        if (!d_enable_output) {
            // Print only every 0.1 seconds
            auto now = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = now - d_last_log_time;

            if (elapsed.count() >= d_log_interval_sec) {
                std::cout << "BER: " << d_ber
                          << " (Errors: " << d_error_bits
                          << " / Total: " << d_ref_buffer.size() << ")" << std::endl;
                d_last_log_time = now;
            }
        }
    }

    // If output is enabled, produce BER values
    int produced = 0;
    if (d_enable_output) {
        float* out = static_cast<float*>(output_items[0]);
        // Output current BER value for each processed bit
        for (int i = 0; i < items_to_process; i++) {
            out[i] = d_ber;
        }
        produced = items_to_process;
    }

    // Tell runtime system how many input items we consumed on each input stream
    consume_each(items_to_process);

    // Tell runtime system how many output items we produced
    return produced;
}

} /* namespace ofdm_testbed */
} /* namespace gr */
