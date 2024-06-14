cmake_minimum_required(VERSION 3.25.1)
project(libsensehat
        LANGUAGES C
        DESCRIPTION "Raspberry Pi Sense Hat C API"
        VERSION 0.1)

set(CMAKE_C_STANDARD 11)

add_executable(libsensehat main.c
        framebuffer/framebuffer.c
        framebuffer/framebuffer.h)
