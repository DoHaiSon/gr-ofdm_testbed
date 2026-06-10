#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Top Block
# GNU Radio version: 3.10.1.1

from packaging.version import Version as StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print("Warning: failed to XInitThreads()")

from PyQt5 import Qt
from gnuradio import qtgui
from gnuradio.filter import firdes
import sip
import display
from gnuradio import blocks
from gnuradio import digital
from gnuradio import filter
from gnuradio import gr
from gnuradio.fft import window
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
from gnuradio import ofdm_testbed
from gnuradio import soapy
import ofdm_testbed



from gnuradio import qtgui

class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block", catch_exceptions=True)
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")

        try:
            if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
                self.restoreGeometry(self.settings.value("geometry").toByteArray())
            else:
                self.restoreGeometry(self.settings.value("geometry"))
        except:
            pass

        ##################################################
        # Variables
        ##################################################
        self.silence_block_rep = silence_block_rep = 1
        self.samp_rate = samp_rate = 1.5e6
        self.preamble_block_rep = preamble_block_rep = 1
        self.pilot_block_rep = pilot_block_rep = 1
        self.occupied_tones = occupied_tones = 48
        self.fft_len = fft_len = 64
        self.data_size = data_size = 1024
        self.data_block_sz = data_block_sz = 21
        self.data_block_rep = data_block_rep = 1
        self.cp_length = cp_length = 16
        self.bpsk = bpsk = digital.constellation_calcdist([-1+1j, 1+1j], [0, 1],
        2, 1, digital.constellation.AMPLITUDE_NORMALIZATION).base()

        ##################################################
        # Blocks
        ##################################################
        self.tab_0 = Qt.QTabWidget()
        self.tab_0_widget_0 = Qt.QWidget()
        self.tab_0_layout_0 = Qt.QBoxLayout(Qt.QBoxLayout.TopToBottom, self.tab_0_widget_0)
        self.tab_0_grid_layout_0 = Qt.QGridLayout()
        self.tab_0_layout_0.addLayout(self.tab_0_grid_layout_0)
        self.tab_0.addTab(self.tab_0_widget_0, 'QT GUI')
        self.tab_0_widget_1 = Qt.QWidget()
        self.tab_0_layout_1 = Qt.QBoxLayout(Qt.QBoxLayout.TopToBottom, self.tab_0_widget_1)
        self.tab_0_grid_layout_1 = Qt.QGridLayout()
        self.tab_0_layout_1.addLayout(self.tab_0_grid_layout_1)
        self.tab_0.addTab(self.tab_0_widget_1, 'Demodulation')
        self.top_layout.addWidget(self.tab_0)
        self.soapy_bladerf_source_0_0 = None
        dev = 'driver=bladerf'
        stream_args = ''
        tune_args = ['']
        settings = ['']

        self.soapy_bladerf_source_0_0 = soapy.source(dev, "fc32", 1, 'bladerf=2a40',
                                  stream_args, tune_args, settings)
        self.soapy_bladerf_source_0_0.set_sample_rate(0, samp_rate)
        self.soapy_bladerf_source_0_0.set_bandwidth(0, 1500000)
        self.soapy_bladerf_source_0_0.set_frequency(0, 2.415e9)
        self.soapy_bladerf_source_0_0.set_frequency_correction(0, 0)
        self.soapy_bladerf_source_0_0.set_gain(0, min(max(20, -1.0), 60.0))
        self.soapy_bladerf_source_0 = None
        dev = 'driver=bladerf'
        stream_args = ''
        tune_args = ['']
        settings = ['']

        self.soapy_bladerf_source_0 = soapy.source(dev, "fc32", 1, '',
                                  stream_args, tune_args, settings)
        self.soapy_bladerf_source_0.set_sample_rate(0, samp_rate)
        self.soapy_bladerf_source_0.set_bandwidth(0, 0.0)
        self.soapy_bladerf_source_0.set_frequency(0, 2.45e9)
        self.soapy_bladerf_source_0.set_frequency_correction(0, 0)
        self.soapy_bladerf_source_0.set_gain(0, min(max(20.0, -1.0), 60.0))
        self.soapy_bladerf_sink_0_0 = None
        dev = 'driver=bladerf'
        stream_args = ''
        tune_args = ['']
        settings = ['']

        self.soapy_bladerf_sink_0_0 = soapy.sink(dev, "fc32", 1, 'bladerf=2a40',
                                  stream_args, tune_args, settings)
        self.soapy_bladerf_sink_0_0.set_sample_rate(0, samp_rate)
        self.soapy_bladerf_sink_0_0.set_bandwidth(0, 1.5e6)
        self.soapy_bladerf_sink_0_0.set_frequency(0, 2.45e9)
        self.soapy_bladerf_sink_0_0.set_frequency_correction(0, 0)
        self.soapy_bladerf_sink_0_0.set_gain(0, min(max(20.0, 17.0), 73.0))
        self.soapy_bladerf_sink_0 = None
        dev = 'driver=bladerf'
        stream_args = ''
        tune_args = ['']
        settings = ['']

        self.soapy_bladerf_sink_0 = soapy.sink(dev, "fc32", 1, '',
                                  stream_args, tune_args, settings)
        self.soapy_bladerf_sink_0.set_sample_rate(0, samp_rate)
        self.soapy_bladerf_sink_0.set_bandwidth(0, 0.0)
        self.soapy_bladerf_sink_0.set_frequency(0, 2.415e9)
        self.soapy_bladerf_sink_0.set_frequency_correction(0, 0)
        self.soapy_bladerf_sink_0.set_gain(0, min(max(20.0, 17.0), 73.0))
        self.show_image_0 = display.show_image( 512, 512  )
        self.show_image_0.displayBottomUp(False)
        self._show_image_0_win = sip.wrapinstance(self.show_image_0.qwidget(), Qt.QWidget)
        self.tab_0_grid_layout_1.addWidget(self._show_image_0_win, 0, 0, 1, 1)
        for r in range(0, 1):
            self.tab_0_grid_layout_1.setRowStretch(r, 1)
        for c in range(0, 1):
            self.tab_0_grid_layout_1.setColumnStretch(c, 1)
        self.qtgui_sink_x_0 = qtgui.sink_c(
            1024, #fftsize
            window.WIN_BLACKMAN_hARRIS, #wintype
            0, #fc
            samp_rate, #bw
            "", #name
            True, #plotfreq
            True, #plotwaterfall
            True, #plottime
            True, #plotconst
            None # parent
        )
        self.qtgui_sink_x_0.set_update_time(1.0/10)
        self._qtgui_sink_x_0_win = sip.wrapinstance(self.qtgui_sink_x_0.qwidget(), Qt.QWidget)

        self.qtgui_sink_x_0.enable_rf_freq(False)

        self.tab_0_grid_layout_0.addWidget(self._qtgui_sink_x_0_win, 0, 0, 1, 1)
        for r in range(0, 1):
            self.tab_0_grid_layout_0.setRowStretch(r, 1)
        for c in range(0, 1):
            self.tab_0_grid_layout_0.setColumnStretch(c, 1)
        self.ofdm_testbed_zero_elimination_0 = ofdm_testbed.zero_elimination(170, 1024)
        self.ofdm_testbed_receiver_control_p2p_0 = ofdm_testbed.receiver_control_p2p(1024, 187, 170)
        self.ofdm_testbed_primary_tx_control_0 = ofdm_testbed.primary_tx_control(1024, 6, 187, 170, 6)
        self.ofdm_testbed_image_vector_source_0 = ofdm_testbed.image_vector_source('/home/minh/SDR_NC/2 Nodes/2_P2P_Img/lena_gray_512.txt', 512*512, 1024, False, 0, 1)
        self.ofdm_testbed_byte_to_bit_0_0 = ofdm_testbed.byte_to_bit()
        self.ofdm_testbed_byte_to_bit_0 = ofdm_testbed.byte_to_bit()
        self.digital_ofdm_tx_0_0 = digital.ofdm_tx(
            fft_len=fft_len,
            cp_len=fft_len//4,
            packet_length_tag_key='len_tag_key',
            occupied_carriers=((-4,-3,-2,-1,1,2,3,4),),
            pilot_carriers=((-6,-5,5,6),),
            pilot_symbols=((-1,1,-1,1),),
            sync_word1=None,
            sync_word2=None,
            bps_header=1,
            bps_payload=1,
            rolloff=0,
            debug_log=False,
            scramble_bits=False)
        self.digital_ofdm_tx_0 = digital.ofdm_tx(
            fft_len=fft_len,
            cp_len=fft_len//4,
            packet_length_tag_key='len_tag_key',
            occupied_carriers=((-4,-3,-2,-1,1,2,3,4),),
            pilot_carriers=((-6,-5,5,6),),
            pilot_symbols=((-1,1,-1,1),),
            sync_word1=None,
            sync_word2=None,
            bps_header=1,
            bps_payload=1,
            rolloff=0,
            debug_log=False,
            scramble_bits=False)
        self.digital_ofdm_rx_0_0 = digital.ofdm_rx(
            fft_len=fft_len, cp_len=fft_len//4,
            frame_length_tag_key='frame_'+"rx_len",
            packet_length_tag_key="rx_len",
            occupied_carriers=((-4,-3,-2,-1,1,2,3,4),),
            pilot_carriers=((-6,-5,5,6),),
            pilot_symbols=((-1,1,-1,1),),
            sync_word1=None,
            sync_word2=None,
            bps_header=1,
            bps_payload=1,
            debug_log=False,
            scramble_bits=False)
        self.digital_ofdm_rx_0 = digital.ofdm_rx(
            fft_len=fft_len, cp_len=fft_len//4,
            frame_length_tag_key='frame_'+"rx_len",
            packet_length_tag_key="rx_len",
            occupied_carriers=((-4,-3,-2,-1,1,2,3,4),),
            pilot_carriers=((-6,-5,5,6),),
            pilot_symbols=((-1,1,-1,1),),
            sync_word1=None,
            sync_word2=None,
            bps_header=1,
            bps_payload=1,
            debug_log=False,
            scramble_bits=False)
        self.dc_blocker_xx_0_0_0 = filter.dc_blocker_cc(256, True)
        self.dc_blocker_xx_0_0 = filter.dc_blocker_cc(256, True)
        self.blocks_stream_to_vector_2 = blocks.stream_to_vector(gr.sizeof_char*1, 6)
        self.blocks_multiply_const_vxx_0_0_0_0 = blocks.multiply_const_cc(25e3)
        self.blocks_multiply_const_vxx_0_0_0 = blocks.multiply_const_cc(25e3)


        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_multiply_const_vxx_0_0_0, 0), (self.digital_ofdm_rx_0_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0_0_0_0, 0), (self.digital_ofdm_rx_0, 0))
        self.connect((self.blocks_stream_to_vector_2, 0), (self.ofdm_testbed_primary_tx_control_0, 0))
        self.connect((self.dc_blocker_xx_0_0, 0), (self.blocks_multiply_const_vxx_0_0_0, 0))
        self.connect((self.dc_blocker_xx_0_0_0, 0), (self.blocks_multiply_const_vxx_0_0_0_0, 0))
        self.connect((self.digital_ofdm_rx_0, 0), (self.blocks_stream_to_vector_2, 0))
        self.connect((self.digital_ofdm_rx_0_0, 0), (self.ofdm_testbed_receiver_control_p2p_0, 0))
        self.connect((self.digital_ofdm_tx_0, 0), (self.soapy_bladerf_sink_0, 0))
        self.connect((self.digital_ofdm_tx_0_0, 0), (self.soapy_bladerf_sink_0_0, 0))
        self.connect((self.ofdm_testbed_byte_to_bit_0, 0), (self.digital_ofdm_tx_0, 0))
        self.connect((self.ofdm_testbed_byte_to_bit_0_0, 0), (self.digital_ofdm_tx_0_0, 0))
        self.connect((self.ofdm_testbed_image_vector_source_0, 0), (self.ofdm_testbed_primary_tx_control_0, 1))
        self.connect((self.ofdm_testbed_primary_tx_control_0, 0), (self.ofdm_testbed_byte_to_bit_0_0, 0))
        self.connect((self.ofdm_testbed_receiver_control_p2p_0, 0), (self.ofdm_testbed_byte_to_bit_0, 0))
        self.connect((self.ofdm_testbed_receiver_control_p2p_0, 1), (self.ofdm_testbed_zero_elimination_0, 0))
        self.connect((self.ofdm_testbed_zero_elimination_0, 0), (self.show_image_0, 1))
        self.connect((self.ofdm_testbed_zero_elimination_0, 0), (self.show_image_0, 0))
        self.connect((self.soapy_bladerf_source_0, 0), (self.dc_blocker_xx_0_0, 0))
        self.connect((self.soapy_bladerf_source_0, 0), (self.qtgui_sink_x_0, 0))
        self.connect((self.soapy_bladerf_source_0_0, 0), (self.dc_blocker_xx_0_0_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        self.stop()
        self.wait()

        event.accept()

    def get_silence_block_rep(self):
        return self.silence_block_rep

    def set_silence_block_rep(self, silence_block_rep):
        self.silence_block_rep = silence_block_rep

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate)
        self.soapy_bladerf_sink_0.set_sample_rate(0, self.samp_rate)
        self.soapy_bladerf_sink_0_0.set_sample_rate(0, self.samp_rate)
        self.soapy_bladerf_source_0.set_sample_rate(0, self.samp_rate)
        self.soapy_bladerf_source_0_0.set_sample_rate(0, self.samp_rate)

    def get_preamble_block_rep(self):
        return self.preamble_block_rep

    def set_preamble_block_rep(self, preamble_block_rep):
        self.preamble_block_rep = preamble_block_rep

    def get_pilot_block_rep(self):
        return self.pilot_block_rep

    def set_pilot_block_rep(self, pilot_block_rep):
        self.pilot_block_rep = pilot_block_rep

    def get_occupied_tones(self):
        return self.occupied_tones

    def set_occupied_tones(self, occupied_tones):
        self.occupied_tones = occupied_tones

    def get_fft_len(self):
        return self.fft_len

    def set_fft_len(self, fft_len):
        self.fft_len = fft_len

    def get_data_size(self):
        return self.data_size

    def set_data_size(self, data_size):
        self.data_size = data_size

    def get_data_block_sz(self):
        return self.data_block_sz

    def set_data_block_sz(self, data_block_sz):
        self.data_block_sz = data_block_sz

    def get_data_block_rep(self):
        return self.data_block_rep

    def set_data_block_rep(self, data_block_rep):
        self.data_block_rep = data_block_rep

    def get_cp_length(self):
        return self.cp_length

    def set_cp_length(self, cp_length):
        self.cp_length = cp_length

    def get_bpsk(self):
        return self.bpsk

    def set_bpsk(self, bpsk):
        self.bpsk = bpsk




def main(top_block_cls=top_block, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()

    tb.start()

    tb.show()

    def sig_handler(sig=None, frame=None):
        tb.stop()
        tb.wait()

        Qt.QApplication.quit()

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    timer = Qt.QTimer()
    timer.start(500)
    timer.timeout.connect(lambda: None)

    qapp.exec_()

if __name__ == '__main__':
    main()
