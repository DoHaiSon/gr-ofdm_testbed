#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Not titled yet
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
import osmosdr
import time



from gnuradio import qtgui

class p2p(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Not titled yet", catch_exceptions=True)
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Not titled yet")
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

        self.settings = Qt.QSettings("GNU Radio", "p2p")

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
        self.samp_rate = samp_rate = 2.2e6
        self.packet_len = packet_len = 50
        self.occupied_tones = occupied_tones = 48
        self.len_tag_key = len_tag_key = "packet_len"
        self.fft_len = fft_len = 64
        self.data_size = data_size = 1024
        self.cp_length = cp_length = 16
        self.band_width = band_width = 2.5e6

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
            2.5e6, #bw
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
        self.osmosdr_source_0 = osmosdr.source(
            args="numchan=" + str(1) + " " + 'bladerf=2a40,buffers=256,buflen=65536'
        )
        self.osmosdr_source_0.set_time_unknown_pps(osmosdr.time_spec_t())
        self.osmosdr_source_0.set_sample_rate(samp_rate)
        self.osmosdr_source_0.set_center_freq(2.415e9, 0)
        self.osmosdr_source_0.set_freq_corr(0, 0)
        self.osmosdr_source_0.set_dc_offset_mode(1, 0)
        self.osmosdr_source_0.set_iq_balance_mode(1, 0)
        self.osmosdr_source_0.set_gain_mode(False, 0)
        self.osmosdr_source_0.set_gain(20, 0)
        self.osmosdr_source_0.set_if_gain(20, 0)
        self.osmosdr_source_0.set_bb_gain(20, 0)
        self.osmosdr_source_0.set_antenna('', 0)
        self.osmosdr_source_0.set_bandwidth(band_width, 0)
        self.osmosdr_sink_0_0 = osmosdr.sink(
            args="numchan=" + str(1) + " " + 'bladerf=2a40,buffers=256,buflen=65536'
        )
        self.osmosdr_sink_0_0.set_time_unknown_pps(osmosdr.time_spec_t())
        self.osmosdr_sink_0_0.set_sample_rate(samp_rate)
        self.osmosdr_sink_0_0.set_center_freq(2.4e9, 0)
        self.osmosdr_sink_0_0.set_freq_corr(0, 0)
        self.osmosdr_sink_0_0.set_gain(60, 0)
        self.osmosdr_sink_0_0.set_if_gain(20, 0)
        self.osmosdr_sink_0_0.set_bb_gain(20, 0)
        self.osmosdr_sink_0_0.set_antenna('', 0)
        self.osmosdr_sink_0_0.set_bandwidth(band_width, 0)
        self.ofdm_testbed_zero_elimination_0 = ofdm_testbed.zero_elimination(170, 1024)
        self.ofdm_testbed_receiver_control_p2p_0 = ofdm_testbed.receiver_control_p2p(1024, 187, 170)
        self.digital_ofdm_tx_0_0_0 = digital.ofdm_tx(
            fft_len=fft_len,
            cp_len=fft_len//4,
            packet_length_tag_key=len_tag_key,
            occupied_carriers=((-4,-3,-2,-1,1,2,3,4),),
            pilot_carriers=((-6,-5,5,6),),
            pilot_symbols=((-1,1,-1,1),),
            sync_word1=None,
            sync_word2=None,
            bps_header=1,
            bps_payload=2,
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
            bps_payload=2,
            debug_log=False,
            scramble_bits=False)
        self.dc_blocker_xx_0_0 = filter.dc_blocker_cc(128, True)
        self.blocks_tag_debug_0 = blocks.tag_debug(gr.sizeof_char*1, 'Rx Packets', "")
        self.blocks_tag_debug_0.set_display(False)
        self.blocks_stream_to_tagged_stream_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, packet_len, len_tag_key)
        self.blocks_multiply_const_vxx_0_0 = blocks.multiply_const_cc(0.05)


        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_multiply_const_vxx_0_0, 0), (self.osmosdr_sink_0_0, 0))
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.digital_ofdm_tx_0_0_0, 0))
        self.connect((self.dc_blocker_xx_0_0, 0), (self.digital_ofdm_rx_0_0, 0))
        self.connect((self.dc_blocker_xx_0_0, 0), (self.qtgui_sink_x_0, 0))
        self.connect((self.digital_ofdm_rx_0_0, 0), (self.blocks_tag_debug_0, 0))
        self.connect((self.digital_ofdm_rx_0_0, 0), (self.ofdm_testbed_receiver_control_p2p_0, 0))
        self.connect((self.digital_ofdm_tx_0_0_0, 0), (self.blocks_multiply_const_vxx_0_0, 0))
        self.connect((self.ofdm_testbed_receiver_control_p2p_0, 0), (self.blocks_stream_to_tagged_stream_0, 0))
        self.connect((self.ofdm_testbed_receiver_control_p2p_0, 1), (self.ofdm_testbed_zero_elimination_0, 0))
        self.connect((self.ofdm_testbed_zero_elimination_0, 0), (self.show_image_0, 1))
        self.connect((self.ofdm_testbed_zero_elimination_0, 0), (self.show_image_0, 0))
        self.connect((self.osmosdr_source_0, 0), (self.dc_blocker_xx_0_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "p2p")
        self.settings.setValue("geometry", self.saveGeometry())
        self.stop()
        self.wait()

        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.osmosdr_sink_0_0.set_sample_rate(self.samp_rate)
        self.osmosdr_source_0.set_sample_rate(self.samp_rate)

    def get_packet_len(self):
        return self.packet_len

    def set_packet_len(self, packet_len):
        self.packet_len = packet_len
        self.blocks_stream_to_tagged_stream_0.set_packet_len(self.packet_len)
        self.blocks_stream_to_tagged_stream_0.set_packet_len_pmt(self.packet_len)

    def get_occupied_tones(self):
        return self.occupied_tones

    def set_occupied_tones(self, occupied_tones):
        self.occupied_tones = occupied_tones

    def get_len_tag_key(self):
        return self.len_tag_key

    def set_len_tag_key(self, len_tag_key):
        self.len_tag_key = len_tag_key

    def get_fft_len(self):
        return self.fft_len

    def set_fft_len(self, fft_len):
        self.fft_len = fft_len

    def get_data_size(self):
        return self.data_size

    def set_data_size(self, data_size):
        self.data_size = data_size

    def get_cp_length(self):
        return self.cp_length

    def set_cp_length(self, cp_length):
        self.cp_length = cp_length

    def get_band_width(self):
        return self.band_width

    def set_band_width(self, band_width):
        self.band_width = band_width
        self.osmosdr_sink_0_0.set_bandwidth(self.band_width, 0)
        self.osmosdr_source_0.set_bandwidth(self.band_width, 0)




def main(top_block_cls=p2p, options=None):

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
