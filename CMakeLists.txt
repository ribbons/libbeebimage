# This file is part of libbeebimage.
# Copyright © 2016-2021 by the authors - see the AUTHORS file for details.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
