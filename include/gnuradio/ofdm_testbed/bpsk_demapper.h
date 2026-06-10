/* -*- c++ -*- */
/*
 * Copyright 2026 minh.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_OFDM_TESTBED_BPSK_DEMAPPER_H
#define INCLUDED_OFDM_TESTBED_BPSK_DEMAPPER_H

#include <gnuradio/ofdm_testbed/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace ofdm_testbed {

    /*!
     * \brief BPSK Demapper: Converts BPSK symbols (complex) to bits/bytes (1 byte output per 8 complex inputs).
     * \ingroup ofdm_testbed
     */
    class OFDM_TESTBED_API bpsk_demapper : virtual public gr::block
    {
    public:
        typedef std::shared_ptr<bpsk_demapper> sptr;

        /*!
         * \brief Return a shared_ptr to a new instance of ofdm_testbed::bpsk_demapper.
         */
        static sptr make();
    };

} // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_BPSK_DEMAPPER_H */
