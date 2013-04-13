cmake_minimum_required(VERSION 2.6)
project(MyApp)

add_executable(myapp main.c)

INSTALL(TARGETS myapp RUNTIME DESTINATION sbin)
