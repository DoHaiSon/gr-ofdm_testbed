find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_OFDM_TESTBED gnuradio-ofdm_testbed)

FIND_PATH(
    GR_OFDM_TESTBED_INCLUDE_DIRS
    NAMES gnuradio/ofdm_testbed/api.h
    HINTS $ENV{OFDM_TESTBED_DIR}/include
        ${PC_OFDM_TESTBED_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_OFDM_TESTBED_LIBRARIES
    NAMES gnuradio-ofdm_testbed
    HINTS $ENV{OFDM_TESTBED_DIR}/lib
        ${PC_OFDM_TESTBED_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-ofdm_testbedTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_OFDM_TESTBED DEFAULT_MSG GR_OFDM_TESTBED_LIBRARIES GR_OFDM_TESTBED_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_OFDM_TESTBED_LIBRARIES GR_OFDM_TESTBED_INCLUDE_DIRS)
