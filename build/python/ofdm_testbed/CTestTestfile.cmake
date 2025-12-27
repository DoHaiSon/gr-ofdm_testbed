# CMake generated Testfile for 
# Source directory: /home/minh/gr-ofdm_testbed/python/ofdm_testbed
# Build directory: /home/minh/gr-ofdm_testbed/build/python/ofdm_testbed
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(qa_ber_pattern_generator "/usr/bin/sh" "qa_ber_pattern_generator_test.sh")
set_tests_properties(qa_ber_pattern_generator PROPERTIES  _BACKTRACE_TRIPLES "/usr/lib/x86_64-linux-gnu/cmake/gnuradio/GrTest.cmake;116;add_test;/home/minh/gr-ofdm_testbed/python/ofdm_testbed/CMakeLists.txt;42;GR_ADD_TEST;/home/minh/gr-ofdm_testbed/python/ofdm_testbed/CMakeLists.txt;0;")
subdirs("bindings")
