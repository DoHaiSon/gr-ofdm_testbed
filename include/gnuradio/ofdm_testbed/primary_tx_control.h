/* -*- c++ -*- */
/*
 * Copyright 2026 minh.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */



#ifndef INCLUDED_OFDM_TESTBED_PRIMARY_TX_CONTROL_H
#define INCLUDED_OFDM_TESTBED_PRIMARY_TX_CONTROL_H

#include <gnuradio/ofdm_testbed/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace ofdm_testbed {

    /*!
     * \brief Primary Transmitter Control Block
     * \ingroup ofdm_testbed
     */
    class OFDM_TESTBED_API primary_tx_control : virtual public gr::block
    {
    public:
        typedef std::shared_ptr<primary_tx_control> sptr;

        /*!
         * \brief Return a shared_ptr to a new instance of ofdm_testbed::primary_tx_control.
         */
        static sptr make(int packet_sz, int guard_interval, int source_id, int destination_id, int ack_burst_size);
    };

} // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_PRIMARY_TX_CONTROL_H */
