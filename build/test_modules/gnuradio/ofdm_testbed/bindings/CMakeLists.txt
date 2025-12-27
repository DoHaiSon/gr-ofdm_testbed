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
    ber_pattern_generator_python.cc python_bindings.cc)

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
