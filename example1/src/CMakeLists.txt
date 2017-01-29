cmake_minimum_required(VERSION 2.6)
project(Example1)

add_executable(example1 main.c)

INSTALL(TARGETS example1 RUNTIME DESTINATION /usr/bin/)
