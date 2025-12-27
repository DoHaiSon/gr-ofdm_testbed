#include "ber_pattern_generator_impl.h"
#include <gnuradio/io_signature.h>
#include <cstdlib> // Cho std::rand
#include <ctime>   // Cho std::time
#include <iostream>

namespace gr {
namespace ofdm_testbed {

ber_pattern_generator::sptr
ber_pattern_generator::make(double error_rate, int pattern_len)
{
    return gnuradio::make_block_sptr<ber_pattern_generator_impl>(error_rate, pattern_len);
}

// CONSTRUCTOR
ber_pattern_generator_impl::ber_pattern_generator_impl(double error_rate, int pattern_len)
    : gr::sync_block("ber_pattern_generator",
          gr::io_signature::make(0, 0, 0), // Không có Input
          gr::io_signature::make(2, 2, sizeof(uint8_t))), // 2 Output (Ref, Rx)
      d_error_rate(error_rate),
      d_pattern_len(pattern_len),
      d_index(0),
      d_need_regen(true)
{
    
    std::srand(std::time(nullptr));
    
    
    d_ref_bits.resize(d_pattern_len);
    d_rx_bits.resize(d_pattern_len);

    
    regenerate_pattern();
}

ber_pattern_generator_impl::~ber_pattern_generator_impl() {}
void ber_pattern_generator_impl::regenerate_pattern()
{
    for(int i = 0; i < d_pattern_len; i++) {
        d_ref_bits[i] = std::rand() % 2;
        
        double random_val = static_cast<double>(std::rand()) / RAND_MAX;
        
        if (random_val < d_error_rate) {
            
            d_rx_bits[i] = 1 - d_ref_bits[i];
        } else {
            
            d_rx_bits[i] = d_ref_bits[i];
        }
    }
    
    d_index = 0;
    d_need_regen = false;
    // std::cout << "Generated " << d_pattern_len << " bits with error rate " << d_error_rate << std::endl;
}


void ber_pattern_generator_impl::set_error_rate(double error_rate)
{
    
    if (std::abs(d_error_rate - error_rate) > 1e-5) {
        d_error_rate = error_rate;
        d_need_regen = true; // Bật cờ để work() biết cần sinh lại
    }
}

// WORK FUNCTION
int ber_pattern_generator_impl::work(int noutput_items,
                                     gr_vector_const_void_star& input_items,
                                     gr_vector_void_star& output_items)
{
    uint8_t *out_ref = (uint8_t *) output_items[0];
    uint8_t *out_rx  = (uint8_t *) output_items[1];

  
    if (d_need_regen) {
        regenerate_pattern();
    }


    for (int i = 0; i < noutput_items; i++) {
        out_ref[i] = d_ref_bits[d_index];
        out_rx[i]  = d_rx_bits[d_index];
        
        
        d_index = (d_index + 1) % d_pattern_len;
    }

    return noutput_items;
}

} /* namespace ofdm_testbed */
} /* namespace gr */
