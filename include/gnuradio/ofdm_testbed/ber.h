/* -*- c++ -*- */
/*
 * Copyright 2025 gr-ofdm_testbed author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_OFDM_TESTBED_BER_H
#define INCLUDED_OFDM_TESTBED_BER_H

#include <gnuradio/block.h>
#include <gnuradio/ofdm_testbed/api.h>

namespace gr {
namespace ofdm_testbed {

/*!
 * \brief <+description of block+>
 * \ingroup ofdm_testbed
 *
 */
class OFDM_TESTBED_API ber : virtual public gr::block
{
public:
    typedef std::shared_ptr<ber> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of ofdm_testbed::ber.
     *
     * To avoid accidental use of raw pointers, ofdm_testbed::ber's
     * constructor is in a private implementation
     * class. ofdm_testbed::ber::make is the public interface for
     * creating new instances.
     *
     * \param avg_len Number of bits to calculate average BER (default: 1000000)
     * \param enable_output Enable output stream (default: false)
     */
    static sptr make(int avg_len = 1000000, bool enable_output = false);
};

} // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_BER_H */
