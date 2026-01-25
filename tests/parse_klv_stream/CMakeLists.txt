cmake_minimum_required(VERSION 3.5)

project(libmisb601_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optional: don't force this unless you really want it
# set(CMAKE_BUILD_TYPE Debug)

find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

# ---- GStreamer ----
pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_check_modules(GST REQUIRED
    gstreamer-1.0
    gstreamer-video-1.0
    gstreamer-app-1.0
    gstreamer-rtsp-server-1.0
)

# ---- OpenCV ----
find_package(OpenCV REQUIRED)

# ---- IMPORTED misb0601 static library ----
set(MISB_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/libs)

add_library(misb0601 STATIC IMPORTED)

set_target_properties(misb0601 PROPERTIES
    IMPORTED_LOCATION
        ${MISB_ROOT}/lib/libmisb0601.a
    INTERFACE_INCLUDE_DIRECTORIES
        ${MISB_ROOT}/include
)

# ---- Helper sources ----
file(GLOB HELPERS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/*.cpp
)

# ---- Test_Decode_KLV ----
add_executable(Test_Decode_KLV
    test_decode_udpsrc.cpp
    ${HELPERS_SOURCES}
)

target_link_libraries(Test_Decode_KLV
    PRIVATE
        misb0601
        ${GST_LIBRARIES}
        ${OpenCV_LIBS}
        Threads::Threads
)

target_include_directories(Test_Decode_KLV
    PRIVATE
        ${GST_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/helpers
)
