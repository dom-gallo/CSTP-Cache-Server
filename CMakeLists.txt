cmake_minimum_required(VERSION 3.22)
project(cstp_server)

set(CMAKE_CXX_STANDARD 20)

add_executable(cstp_server main.cpp lib/Message.cpp lib/Message.h lib/RequestHandler.cpp lib/RequestHandler.h lib/OpCodes.cpp lib/OpCodes.h)
