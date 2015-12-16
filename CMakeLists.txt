cmake_minimum_required(VERSION 3.1)
project(gst_pipeapp)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")

include(FindPkgConfig)

pkg_check_modules(GLIB2 REQUIRED glib-2.0)
pkg_check_modules(GSTRM REQUIRED gstreamer-0.10)
pkg_check_modules(GSTRMAPP REQUIRED gstreamer-app-0.10)

include_directories(
        ${GLIB2_INCLUDE_DIRS}
        ${GSTRM_INCLUDE_DIRS}
        ${GSTRMAPP_INCLUDE_DIRS})
link_directories(
        ${GLIB2_LIBRARY_DIRS}
        ${GSTRM_LIBRARY_DIRS}
        ${GSTRMAPP_LIBRARY_DIRS})

# message(STATUS ${GSTRM_INCLUDE_DIRS})

add_subdirectory(src)
# add_subdirectory(tests)
