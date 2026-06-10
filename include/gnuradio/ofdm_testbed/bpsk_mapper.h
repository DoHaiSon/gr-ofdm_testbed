/* -*- c++ -*- */
/*
 * Copyright 2026 minh.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_OFDM_TESTBED_BPSK_MAPPER_H
#define INCLUDED_OFDM_TESTBED_BPSK_MAPPER_H

#include <gnuradio/ofdm_testbed/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace ofdm_testbed {

    /*!
     * \brief BPSK Mapper: Maps input bits (0/1) to BPSK symbols (-1/+1)
     * \ingroup ofdm_testbed
     */
    class OFDM_TESTBED_API bpsk_mapper : virtual public gr::block
    {
    public:
        typedef std::shared_ptr<bpsk_mapper> sptr;

        /*!
         * \brief Return a shared_ptr to a new instance of ofdm_testbed::bpsk_mapper.
         */
        static sptr make();
    };

} // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_BPSK_MAPPER_H */
