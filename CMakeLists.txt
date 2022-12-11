# ASIPGM: CMakeLists.txt

# Copyright (C) 2022  Johnathan K Burchill

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.


PROJECT(ASIPGM)

CMAKE_MINIMUM_REQUIRED(VERSION 3.3)

ADD_DEFINITIONS(-DHAVE_GETOPT_H)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    message( "-- Debugging enabled")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg -std=gnu99 ")
else (CMAKE_BUILD_TYPE STREQUAL Debug)
    message( "-- Optimization enabled")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -std=gnu99 ")
endif(CMAKE_BUILD_TYPE STREQUAL Debug)

INCLUDE_DIRECTORIES(include)

FIND_LIBRARY(CDF libcdf.a)
FIND_LIBRARY(MATH libm.a)

SET(LIBS ${LIBS})

INCLUDE_DIRECTORIES(${INCLUDE_DIRS})

ADD_LIBRARY(asipgm asipgm.c)

ADD_EXECUTABLE(readpgm main.c)
TARGET_LINK_LIBRARIES(readpgm -static asipgm ${CDF} ${MATH})

install(TARGETS readpgm DESTINATION $ENV{HOME}/bin)
