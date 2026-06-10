#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "zero_elimination_impl.h"

namespace gr {
  namespace ofdm_testbed {

    zero_elimination::sptr
    zero_elimination::make(int node_id, int data_len)
    {
      return gnuradio::get_initial_sptr
        (new zero_elimination_impl(node_id, data_len));
    }

   zero_elimination_impl::zero_elimination_impl(int node_id, int data_len)
      : gr::block("zero_elimination",
              gr::io_signature::make(1, 1, sizeof(unsigned char)), 
              gr::io_signature::make(1, 1, sizeof(unsigned char))), 
        d_node_id(node_id),
        d_data_len(data_len),
        d_data_index(0),
        d_data(data_len * data_len, 0x00), 
        d_state(ST_ZERO_ELIMINATE)         
    {}
    zero_elimination_impl::~zero_elimination_impl()
    {
    }

    void
    zero_elimination_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        
        ninput_items_required[0] = noutput_items;
    }

    int
    zero_elimination_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items_v,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        int ninput_items = ninput_items_v[0];
        const unsigned char *in = (const unsigned char *) input_items[0];
        unsigned char *out = (unsigned char *) output_items[0];

        int ni = 0, no = 0;

        while (ni < ninput_items && no < noutput_items) 
        {
            switch(d_state)
            {
                case ST_ZERO_ELIMINATE:
                {
                    // find marked byte (node_id)
                    if (in[ni] == (unsigned char) d_node_id)
                    {
                        d_state = ST_DATA_READ;
                    }
                    ni++; 
                    break;
                }
                case ST_DATA_READ:
                {   
                    
                    out[no] = in[ni];
                    ni++; 
                    no++;
                    d_data_index++;
                    
                    if(d_data_index == d_data_len)
                    {
                        d_data_index = 0;
                        d_state = ST_ZERO_ELIMINATE;
                    }
                    break;
                }
            }
        }
        
        
        consume_each (ni);

        
        return no;
    }

  } /* namespace ofdm_testbed */
} /* namespace gr */