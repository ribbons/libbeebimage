# Copyright © 2016-2024 Matt Robinson
#
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.5)
project("beebimage")

if(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_library(beebimage STATIC
            BbcScreen.h BbcScreen.cpp BbcScreenC.cpp
            BbcImageLoader.h BbcImageLoader.cpp BbcImageLoaderC.cpp)

if(NOT BEEBIMAGE_NO_INSTALL)
    install(TARGETS beebimage
            DESTINATION lib)

    install(FILES BbcScreen.h BbcImageLoader.h
            DESTINATION include)
endif()

file(GLOB CPP_FILES *.cpp)
