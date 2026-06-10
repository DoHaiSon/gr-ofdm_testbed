/* -*- c++ -*- */
/*
 * Copyright 2025 gr-ofdm_testbed author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_OFDM_TESTBED_IMAGE_VECTOR_SOURCE_IMPL_H
#define INCLUDED_OFDM_TESTBED_IMAGE_VECTOR_SOURCE_IMPL_H

#include <gnuradio/ofdm_testbed/image_vector_source.h>

namespace gr {
namespace ofdm_testbed {

    class image_vector_source_impl : public image_vector_source
    {
    private:
        
        std::vector<unsigned char> d_data;
        std::string d_filename;
        std::vector<tag_t> d_tags;
        
        int d_image_size;
        int d_packet_size;
        int d_channel_coding;
        int d_vlen;
        
        int d_data_index;
        int d_end_data_idx;
        int d_pkt_no;
        
        unsigned int d_offset;
        unsigned char d_out_pkt_no;
        
        bool d_repeat;
        bool d_settags;
        bool d_check_end;

    public:
        image_vector_source_impl(const std::string &filename, int image_size, int packet_size,
                                 bool repeat, int channel_coding, int vlen, const std::vector<tag_t> &tags);
        ~image_vector_source_impl();

        int work(int noutput_items,
                 gr_vector_const_void_star &input_items,
                 gr_vector_void_star &output_items) override;
    };

} // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_IMAGE_VECTOR_SOURCE_IMPL_H */
