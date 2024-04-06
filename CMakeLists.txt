cmake_minimum_required(VERSION 3.26)
project(signaling_utilities)

set(CMAKE_CXX_STANDARD 17)

add_executable(signaling_utilities main.cpp sigutil/envelope_detector.cpp sigutil/envelope_detector.h
        sigutil/datapoint.cpp
        sigutil/datapoint.h
        sigutil/finite_impuse_response_filter.cpp
        sigutil/finite_impuse_response_filter.h
        sigutil/generator.cpp
        sigutil/generator.h)
