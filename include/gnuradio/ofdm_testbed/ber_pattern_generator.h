/* -*- c++ -*- */
/*
 * Copyright 2025 gr-ofdm_testbed author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_OFDM_TESTBED_BER_PATTERN_GENERATOR_H
#define INCLUDED_OFDM_TESTBED_BER_PATTERN_GENERATOR_H

#include <gnuradio/ofdm_testbed/api.h>
#include <gnuradio/sync_block.h> 

namespace gr {
namespace ofdm_testbed {

    /*!
     * \brief <+description of block+>
     * \ingroup ofdm_testbed
     *
     */
    class OFDM_TESTBED_API ber_pattern_generator : virtual public gr::sync_block
    {
    public:
        typedef std::shared_ptr<ber_pattern_generator> sptr;  
        static sptr make(double error_rate, int pattern_len);
   
        virtual void set_error_rate(double error_rate) = 0;
    };

} // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_BER_PATTERN_GENERATOR_H */
