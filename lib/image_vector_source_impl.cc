/* -*- c++ -*- */
/*
 * Copyright 2025 gr-ofdm_testbed minh.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "image_vector_source_impl.h"
#include <gnuradio/io_signature.h>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <cstring> // Cho memcpy
#include <iostream>

namespace gr {
namespace ofdm_testbed {

    image_vector_source::sptr
    image_vector_source::make(const std::string &filename, int image_size, int packet_size,
                              bool repeat, int channel_coding, int vlen, const std::vector<tag_t> &tags)
    {
        return gnuradio::make_block_sptr<image_vector_source_impl>(filename, image_size, packet_size, repeat, channel_coding, vlen, tags);
    }

    /*
     * Constructor
     */
    image_vector_source_impl::image_vector_source_impl(const std::string &filename, int image_size, int packet_size,
                                                       bool repeat, int channel_coding, int vlen, const std::vector<tag_t> &tags)
        : gr::sync_block("image_vector_source",
                         gr::io_signature::make(0, 0, 0), // 0 Inputs
                         gr::io_signature::make(1, 1, sizeof(unsigned char) * vlen)), // 1 Output vector
          d_filename(filename),
          d_tags(tags),
          d_image_size(image_size),
          d_packet_size(packet_size),
          d_channel_coding(channel_coding),
          d_vlen(vlen),
          d_data_index(d_packet_size), 
          d_end_data_idx(0),
          d_pkt_no(0),
          d_offset(0),
          d_out_pkt_no(0x01),
          d_repeat(repeat),
          d_settags(false),
          d_check_end(false)
    {
        // Resize buffer
        d_data.resize(d_image_size, 0x00);

        /* Read data from file */
        std::ifstream f(d_filename.c_str());
        
        if (f.is_open())
        {
            int no = 0;
            int temp;
            while (no < d_image_size && f >> temp)
            {
                d_data[no] = static_cast<unsigned char>(temp);
                no++;
            }
            f.close();
        }
        else
        {
            throw std::runtime_error("Error: Can't open file: " + d_filename);
        }

        if (tags.size() == 0)
        {
            d_settags = false;
        }
        else
        {
            d_settags = true;
            set_output_multiple(d_data.size() / vlen);
        }

        if ((d_data.size() % vlen) != 0)
            throw std::invalid_argument("data length must be a multiple of vlen");
    }

    image_vector_source_impl::~image_vector_source_impl() {}

    int image_vector_source_impl::work(int noutput_items,
                                       gr_vector_const_void_star &input_items,
                                       gr_vector_void_star &output_items)
    {
        // Logic CHANNEL CODING == 0 
        if (d_channel_coding == 0)
        {
            unsigned char *optr = (unsigned char *)output_items[0];
            
            /* --- REPEAT MODE --- */
            if (d_repeat)
            {
                unsigned int size = d_data.size();
                unsigned int offset = d_offset;
                if (size == 0) return -1;

                if (d_settags)
                {
                    
                    int n_outputitems_per_vector = d_data.size() / d_vlen;
                    for (int i = 0; i < noutput_items; i += n_outputitems_per_vector)
                    {
                        if (i + n_outputitems_per_vector > noutput_items) break; 
                        
                        memcpy((void *)optr, (const void *)&d_data[0], size * sizeof(unsigned char));
                        optr += size; 
                        
                        for (unsigned t = 0; t < d_tags.size(); t++)
                        {
                            add_item_tag(0, nitems_written(0) + i + d_tags[t].offset,
                                         d_tags[t].key, d_tags[t].value);
                        }
                    }
                    return noutput_items; 
                }
                else
                {
                    
                    int i = 0;
                    while (i < noutput_items * d_vlen)
                    {
                        if (d_data_index == d_packet_size)
                        {
                            optr[i] = 0x00; 
                            d_data_index = 0;
                        }
                        else
                        {
                            optr[i] = d_data[offset];
                            offset++;
                            if (offset == size) { offset = 0; }
                            d_data_index++;
                        }
                        i++;
                    }
                    d_offset = offset;
                    return noutput_items;
                }
            }
            
            else
            {
                int no = 0;
                if (d_check_end == false)
                {
                    while (no < noutput_items)
                    {
                        if (d_offset < d_data.size())
                        {
                            if (d_data_index == d_packet_size)
                            {
                                optr[no] = 0x00; 
                                d_data_index = 0;
                            }
                            else
                            {
                                optr[no] = d_data[d_offset];
                                d_data_index++; 
                                d_offset++;    
                                
                                if (d_data_index == d_packet_size)
                                {
                                    d_pkt_no++;
                                    
                                    
                                }
                            }
                        }
                        else
                        {
                            
                            if (d_end_data_idx < d_packet_size)
                            {
                                optr[no] = 0x01; // End marker
                                d_end_data_idx++;
                            }
                            else
                            {
                                d_end_data_idx = 0;
                                d_check_end = true;
                                return no; 
                            }
                        }
                        no++;
                    }
                    return no;
                }
                else
                {
                    return -1; 
                }
            }
        }
        // Logic CHANNEL CODING != 0 
        else
        {
            unsigned char *optr = (unsigned char *)output_items[0];
            
            if (d_repeat)
            {
                unsigned int size = d_data.size();
                unsigned int offset = d_offset;
                if (size == 0) return -1;

                if (d_settags) {
                    
                }
                     
                for (int i = 0; i < noutput_items * d_vlen; i++)
                {
                    optr[i] = d_data[offset++];
                    if (offset >= size) { offset = 0; }
                }
                d_offset = offset;
                return noutput_items;
            }
            else
            {
                if (d_offset >= d_data.size()) return -1; // Done

                unsigned int n = std::min((unsigned int)d_data.size() - d_offset,
                                          (unsigned int)noutput_items * d_vlen);
                
                for (unsigned int i = 0; i < n; i++)
                {
                    optr[i] = d_data[d_offset + i];
                }

                
                for (unsigned t = 0; t < d_tags.size(); t++)
                {
                    if ((d_tags[t].offset >= d_offset) && (d_tags[t].offset < d_offset + n))
                        add_item_tag(0, d_tags[t].offset, d_tags[t].key, d_tags[t].value);
                }

                d_offset += n;
                return n / d_vlen;
            }
        }
    }

} // namespace ofdm_testbed
} // namespace gr