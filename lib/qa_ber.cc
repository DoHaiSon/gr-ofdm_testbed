/* -*- c++ -*- */
/*
 * Copyright 2025 gr-ofdm_testbed author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/attributes.h>
#include <gnuradio/ofdm_testbed/ber.h>
#include <gnuradio/top_block.h>
#include <gnuradio/blocks/vector_source.h>
#include <gnuradio/blocks/vector_sink.h>
#include <boost/test/unit_test.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

namespace gr {
namespace ofdm_testbed {

// Test case 1: Perfect match (BER = 0) - Large data stream
BOOST_AUTO_TEST_CASE(test_ber_zero_errors)
{
    int avg_len = 100000;  // 100K bits
    bool enable_output = true;

    // Create test data - 5 million bits (50x larger than avg_len)
    // This tests buffer management with large data streams
    std::vector<uint8_t> ref_data;
    std::vector<uint8_t> rx_data;

    int total_bits = 5000000;  // 5 million bits
    for (int i = 0; i < total_bits; i++) {
        ref_data.push_back(i % 2);
        rx_data.push_back(i % 2);  // Identical - no errors
    }

    // Create blocks
    auto tb = gr::make_top_block("test_ber_zero_errors");
    auto src_ref = gr::blocks::vector_source<uint8_t>::make(ref_data);
    auto src_rx = gr::blocks::vector_source<uint8_t>::make(rx_data);
    auto ber_block = ber::make(avg_len, enable_output);
    auto sink = gr::blocks::vector_sink<float>::make();

    // Connect blocks
    tb->connect(src_ref, 0, ber_block, 0);
    tb->connect(src_rx, 0, ber_block, 1);
    tb->connect(ber_block, 0, sink, 0);

    // Run flowgraph
    std::cout << "Running test_ber_zero_errors with " << total_bits << " bits..." << std::endl;
    tb->run();

    // Get results
    std::vector<float> result = sink->data();

    // Check BER should be 0 throughout
    BOOST_CHECK(!result.empty());
    BOOST_CHECK_EQUAL(result.size(), total_bits);

    // Check final BER (after all bits processed)
    float final_ber = result[result.size() - 1];
    BOOST_CHECK_SMALL(final_ber, 1e-6f);
    std::cout << "Final BER: " << final_ber << std::endl;
}

// Test case 2: Known error rate - Large data stream
BOOST_AUTO_TEST_CASE(test_ber_known_errors)
{
    int avg_len = 1000000;  // 1 million bits
    bool enable_output = true;

    // Create test data - 10 million bits with 10% error rate
    std::vector<uint8_t> ref_data;
    std::vector<uint8_t> rx_data;

    int total_bits = 10000000;  // 10 million bits
    int error_count = 0;

    for (int i = 0; i < total_bits; i++) {
        uint8_t bit = i % 2;
        ref_data.push_back(bit);

        // Introduce error every 10 bits (10% error rate)
        if (i % 10 == 0) {
            rx_data.push_back(1 - bit); // Flip bit
            error_count++;
        } else {
            rx_data.push_back(bit);
        }
    }

    float expected_ber = 0.1f;  // 10% error rate

    // Create blocks
    auto tb = gr::make_top_block("test_ber_known_errors");
    auto src_ref = gr::blocks::vector_source<uint8_t>::make(ref_data);
    auto src_rx = gr::blocks::vector_source<uint8_t>::make(rx_data);
    auto ber_block = ber::make(avg_len, enable_output);
    auto sink = gr::blocks::vector_sink<float>::make();

    // Connect blocks
    tb->connect(src_ref, 0, ber_block, 0);
    tb->connect(src_rx, 0, ber_block, 1);
    tb->connect(ber_block, 0, sink, 0);

    // Run flowgraph
    std::cout << "Running test_ber_known_errors with " << total_bits << " bits..." << std::endl;
    tb->run();

    // Get results
    std::vector<float> result = sink->data();

    // Check BER matches expected value
    BOOST_CHECK(!result.empty());
    BOOST_CHECK_EQUAL(result.size(), total_bits);

    // The last values should converge to expected BER
    float measured_ber = result[result.size() - 1];
    BOOST_CHECK_CLOSE(measured_ber, expected_ber, 1.0); // Within 1% tolerance
    std::cout << "Expected BER: " << expected_ber << ", Measured BER: " << measured_ber << std::endl;
}

// Test case 3: Sliding window behavior - Tests buffer add/remove mechanism
BOOST_AUTO_TEST_CASE(test_ber_sliding_window)
{
    int avg_len = 1000000;  // 1 million bits window
    bool enable_output = true;

    // Create test data - 3 phases to test sliding window
    std::vector<uint8_t> ref_data;
    std::vector<uint8_t> rx_data;

    // Phase 1: First 1M bits - no errors (BER = 0)
    std::cout << "Generating Phase 1: 1M bits with 0% error..." << std::endl;
    for (int i = 0; i < avg_len; i++) {
        ref_data.push_back(1);
        rx_data.push_back(1);
    }

    // Phase 2: Next 2M bits - 50% errors (alternating correct/wrong)
    std::cout << "Generating Phase 2: 2M bits with 50% error..." << std::endl;
    for (int i = 0; i < 2 * avg_len; i++) {
        ref_data.push_back(0);
        if (i % 2 == 0) {
            rx_data.push_back(0);  // Correct
        } else {
            rx_data.push_back(1);  // Wrong - 50% error rate
        }
    }

    // Phase 3: Final 1M bits - 100% errors
    std::cout << "Generating Phase 3: 1M bits with 100% error..." << std::endl;
    for (int i = 0; i < avg_len; i++) {
        ref_data.push_back(1);
        rx_data.push_back(0); // All flipped
    }    int total_bits = ref_data.size();  // 4 million bits total
    std::cout << "Total bits: " << total_bits << std::endl;

    // Create blocks
    auto tb = gr::make_top_block("test_ber_sliding_window");
    auto src_ref = gr::blocks::vector_source<uint8_t>::make(ref_data);
    auto src_rx = gr::blocks::vector_source<uint8_t>::make(rx_data);
    auto ber_block = ber::make(avg_len, enable_output);
    auto sink = gr::blocks::vector_sink<float>::make();

    // Connect blocks
    tb->connect(src_ref, 0, ber_block, 0);
    tb->connect(src_rx, 0, ber_block, 1);
    tb->connect(ber_block, 0, sink, 0);

    // Run flowgraph
    std::cout << "Running test_ber_sliding_window with " << total_bits << " bits..." << std::endl;
    tb->run();

    // Get results
    std::vector<float> result = sink->data();

    // Check that BER changes as window slides
    BOOST_CHECK(!result.empty());
    BOOST_CHECK_EQUAL(result.size(), total_bits);

    // After processing all 4M bits, sliding window contains only last 1M bits (100% error)
    // Because we remove old data to keep exactly avg_len bits
    float final_ber = result[result.size() - 1];
    std::cout << "Final BER (should be ~1.0 from Phase 3): " << final_ber << std::endl;
    BOOST_CHECK_CLOSE(final_ber, 1.0f, 1.0); // Within 1% tolerance

    // Check BER at different points to verify sliding window
    // After 1M bits (end of Phase 1): BER should be ~0
    if (result.size() > avg_len) {
        float ber_phase1 = result[avg_len - 1];
        std::cout << "BER at end of Phase 1 (1M bits, should be ~0.0): " << ber_phase1 << std::endl;
        BOOST_CHECK_SMALL(ber_phase1, 0.01f);
    }

    // After 3M bits (end of Phase 2): BER should be ~0.5 (window contains Phase 2 data)
    if (result.size() > 3 * avg_len) {
        float ber_phase2 = result[3 * avg_len - 1];
        std::cout << "BER at end of Phase 2 (3M bits, should be ~0.5): " << ber_phase2 << std::endl;
        BOOST_CHECK_CLOSE(ber_phase2, 0.5f, 2.0); // Within 2% tolerance
    }
}// Test case 4: No output mode - Large data stream (console output only)
BOOST_AUTO_TEST_CASE(test_ber_no_output)
{
    int avg_len = 500000;  // 500K bits
    bool enable_output = false;  // No output stream, only console

    // Create test data - 2 million bits
    std::vector<uint8_t> ref_data;
    std::vector<uint8_t> rx_data;

    int total_bits = 2000000;
    for (int i = 0; i < total_bits; i++) {
        ref_data.push_back(i % 2);
        rx_data.push_back(i % 2);
    }

    // Create blocks
    auto tb = gr::make_top_block("test_ber_no_output");
    auto src_ref = gr::blocks::vector_source<uint8_t>::make(ref_data);
    auto src_rx = gr::blocks::vector_source<uint8_t>::make(rx_data);
    auto ber_block = ber::make(avg_len, enable_output);

    // Connect blocks (no output sink needed)
    tb->connect(src_ref, 0, ber_block, 0);
    tb->connect(src_rx, 0, ber_block, 1);

    // Run flowgraph - should complete without errors and print to console
    std::cout << "Running test_ber_no_output with " << total_bits << " bits (console only)..." << std::endl;
    tb->run();

    // Just check that it runs successfully
    BOOST_CHECK(true);
}

// Test case 5: Extreme buffer stress test - Very large data exceeding avg_len many times
BOOST_AUTO_TEST_CASE(test_ber_buffer_stress)
{
    int avg_len = 100000;  // 100K bits window
    bool enable_output = true;

    // Create test data - 20 million bits (200x larger than avg_len)
    // This heavily tests the buffer add/remove mechanism
    std::vector<uint8_t> ref_data;
    std::vector<uint8_t> rx_data;

    int total_bits = 20000000;  // 20 million bits
    std::cout << "Generating " << total_bits << " bits for stress test..." << std::endl;

    // Varying error pattern throughout the stream
    // First 10M bits: no errors, Next 10M bits: 50% errors
    for (int i = 0; i < total_bits; i++) {
        ref_data.push_back(0);

        if (i < 10000000) {
            rx_data.push_back(0);  // No errors in first 10M
        } else {
            rx_data.push_back(i % 2);  // 50% errors in last 10M
        }
    }

    // Create blocks
    auto tb = gr::make_top_block("test_ber_buffer_stress");
    auto src_ref = gr::blocks::vector_source<uint8_t>::make(ref_data);
    auto src_rx = gr::blocks::vector_source<uint8_t>::make(rx_data);
    auto ber_block = ber::make(avg_len, enable_output);
    auto sink = gr::blocks::vector_sink<float>::make();

    // Connect blocks
    tb->connect(src_ref, 0, ber_block, 0);
    tb->connect(src_rx, 0, ber_block, 1);
    tb->connect(ber_block, 0, sink, 0);

    // Run flowgraph
    std::cout << "Running test_ber_buffer_stress with " << total_bits << " bits..." << std::endl;
    tb->run();

    // Get results
    std::vector<float> result = sink->data();

    // Check BER is calculated correctly
    BOOST_CHECK(!result.empty());
    BOOST_CHECK_EQUAL(result.size(), total_bits);

    float final_ber = result[result.size() - 1];
    std::cout << "Final BER after processing " << total_bits << " bits: " << final_ber << std::endl;

    // Final window (last 100K bits) should reflect 50% error rate from last 10M segment
    BOOST_CHECK_CLOSE(final_ber, 0.5f, 2.0);  // Within 2% tolerance    std::cout << "Buffer stress test completed successfully!" << std::endl;
}

} /* namespace ofdm_testbed */
} /* namespace gr */
