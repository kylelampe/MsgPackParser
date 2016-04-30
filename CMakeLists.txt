cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(MsgPackParser)

find_package(Boost REQUIRED CONFIG)

include_directories(
    ${Boost_INCLUDE_DIR}
)

link_directories(${Boost_LIBRARIES})

add_executable (MsgPackParser MsgPackParser.cpp stdafx.cpp stdafx.h targetver.h cmp/cmp.c cmp/cmp.h)
