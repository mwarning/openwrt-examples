cmake_minimum_required(VERSION 2.6)
project(MyApp1)

add_executable(myapp1 main.c)

INSTALL(TARGETS myapp1 RUNTIME DESTINATION /usr/bin/)
