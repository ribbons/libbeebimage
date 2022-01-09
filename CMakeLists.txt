# Copyright © 2016-2021 Matt Robinson
#
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.2)
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

add_custom_target(cppcheck COMMAND cppcheck --quiet --error-exitcode=2
                  --enable=all --suppress=missingIncludeSystem
                  --suppress=unusedFunction:*C.cpp --std=c++11
                  --inline-suppr ${CPP_FILES}
                  DEPENDS beebimage VERBATIM)
