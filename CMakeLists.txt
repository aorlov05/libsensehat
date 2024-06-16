cmake_minimum_required(VERSION 3.25.1)
project(sensehat
        LANGUAGES C
        DESCRIPTION "Raspberry Pi Sense Hat C API"
        VERSION 0.1)

set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES_LIST src/framebuffer.c src/joystick.c)

add_library(sensehat STATIC ${SOURCE_FILES_LIST})

target_include_directories(sensehat PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Sets output directory library inside /lib
set_target_properties(sensehat PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# Test using static library
set(APP_EXE libsense_test)
add_executable(${APP_EXE} main.c)
target_link_libraries(${APP_EXE} sensehat)
