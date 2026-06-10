/* -*- c++ -*- */
/*
 * Copyright 2026 minh.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_OFDM_TESTBED_ZERO_ELIMINATION_H
#define INCLUDED_OFDM_TESTBED_ZERO_ELIMINATION_H

#include <gnuradio/ofdm_testbed/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace ofdm_testbed {

    /*!
     * \brief <+description of block+>
     * \ingroup ofdm_testbed
     *
     */
    class OFDM_TESTBED_API zero_elimination : virtual public gr::block
    {
     public:
      typedef std::shared_ptr<zero_elimination> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ofdm_testbed::zero_elimination.
       *
       */
      static sptr make(int node_id, int data_len);
    };

  } // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_ZERO_ELIMINATION_H */
