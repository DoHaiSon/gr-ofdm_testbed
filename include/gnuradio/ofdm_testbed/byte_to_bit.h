/* -*- c++ -*- */
/*
 * Copyright 2026 minh.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#ifndef INCLUDED_OFDM_TESTBED_BYTE_TO_BIT_H
#define INCLUDED_OFDM_TESTBED_BYTE_TO_BIT_H

#include <gnuradio/ofdm_testbed/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace ofdm_testbed {

    /*!
     * \brief Converts a stream of bytes into a stream of bits (1 byte -> 8 bits)
     * \ingroup ofdm_testbed
     */
    class OFDM_TESTBED_API byte_to_bit : virtual public gr::block
    {
    public:
        typedef std::shared_ptr<byte_to_bit> sptr;

        /*!
         * \brief Return a shared_ptr to a new instance of ofdm_testbed::byte_to_bit.
         */
        static sptr make();
    };

} // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_BYTE_TO_BIT_H */
