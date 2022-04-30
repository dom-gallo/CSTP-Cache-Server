cmake_minimum_required(VERSION 3.22)
project(cstp_server)

set(CMAKE_CXX_STANDARD 20)

add_executable(cstp_server main.cpp Message.cpp Message.h RequestHandler.cpp RequestHandler.h OpCodes.cpp OpCodes.h)
