#ifndef INCLUDED_OFDM_TESTBED_RECEIVER_CONTROL_P2P_H
#define INCLUDED_OFDM_TESTBED_RECEIVER_CONTROL_P2P_H

#include <gnuradio/ofdm_testbed/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace ofdm_testbed { 

    /*!
     * \brief Network Coding P2P Receiver Control Block
     * \ingroup ofdm_testbed
     */
    class OFDM_TESTBED_API receiver_control_p2p : virtual public gr::block 
    {
     public:
      // GNU Radio hi?n d?i dùng std::shared_ptr
      typedef std::shared_ptr<receiver_control_p2p> sptr;

    
      static sptr make(int packet_size, int transmitter_id, int receiver_id);
    };

  } // namespace ofdm_testbed
} // namespace gr

#endif /* INCLUDED_OFDM_TESTBED_RECEIVER_CONTROL_P2P_H */
