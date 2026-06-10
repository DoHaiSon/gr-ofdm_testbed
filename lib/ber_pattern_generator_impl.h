#ifndef INCLUDED_OFDM_TESTBED_BER_PATTERN_GENERATOR_IMPL_H
#define INCLUDED_OFDM_TESTBED_BER_PATTERN_GENERATOR_IMPL_H

#include <gnuradio/ofdm_testbed/ber_pattern_generator.h>
#include <vector>

namespace gr {
namespace ofdm_testbed {

class ber_pattern_generator_impl : public ber_pattern_generator
{
private:
    double d_error_rate;
    int d_pattern_len;
    
    
    std::vector<uint8_t> d_ref_bits;
    std::vector<uint8_t> d_rx_bits;
    
    int d_index;        
    bool d_need_regen;  

  
    void regenerate_pattern();

public:
    ber_pattern_generator_impl(double error_rate, int pattern_len);
    ~ber_pattern_generator_impl();

    // Override setter function.
    void set_error_rate(double error_rate) override;

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_BER_PATTERN_GENERATOR_IMPL_H */
