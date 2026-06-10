# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT ofdm_testbed_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT ofdm_testbed_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################
list(APPEND ofdm_testbed_python_files
    ber_python.cc
    ber_pattern_generator_python.cc
    receiver_control_p2p_python.cc
    primary_tx_control_python.cc
    byte_to_bit_python.cc
    bpsk_mapper_python.cc
    bpsk_demapper_python.cc
    image_vector_source_python.cc
    zero_elimination_python.cc
    python_bindings.cc)

GR_PYBIND_MAKE_OOT(ofdm_testbed
   ../../..
   gr::ofdm_testbed
   "${ofdm_testbed_python_files}")

# copy in bindings .so file for use in QA test module
add_custom_target(
  copy_bindings_for_tests ALL
  COMMAND
    ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/*.so"
    ${CMAKE_BINARY_DIR}/test_modules/gnuradio/ofdm_testbed/
  DEPENDS ofdm_testbed_python)

install(TARGETS ofdm_testbed_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/ofdm_testbed COMPONENT pythonapi)
