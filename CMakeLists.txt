cmake_minimum_required(VERSION 3.20)
project(Mehrband_TM C)

set(CMAKE_C_STANDARD 11)

add_executable(Mehrband_TM main.c program_parser.c program_parser.h helper.c helper.h tape_parser.c tape_parser.h simulator.c simulator.h)
target_link_libraries(Mehrband_TM m)