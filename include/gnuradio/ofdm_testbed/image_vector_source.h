/* -*- c++ -*- */
/*
 * Copyright 2025 gr-ofdm_testbed author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_OFDM_TESTBED_IMAGE_VECTOR_SOURCE_H
#define INCLUDED_OFDM_TESTBED_IMAGE_VECTOR_SOURCE_H

#include <gnuradio/ofdm_testbed/api.h>
#include <gnuradio/sync_block.h>
#include <vector>
#include <string>
#include <gnuradio/tags.h>

namespace gr {
namespace ofdm_testbed {

    /*!
     * \brief Image Vector Source Block (Reads file and sends data in packets)
     * \ingroup ofdm_testbed
     */
    class OFDM_TESTBED_API image_vector_source : virtual public gr::sync_block
    {
    public:
        typedef std::shared_ptr<image_vector_source> sptr;
        static sptr make(const std::string &filename, int image_size, int packet_size,
                 bool repeat, int channel_coding, int vlen,
                 const std::vector<gr::tag_t> &tags = std::vector<gr::tag_t>());
    };

} // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_IMAGE_VECTOR_SOURCE_H */
