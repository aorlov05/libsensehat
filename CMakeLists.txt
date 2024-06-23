cmake_minimum_required(VERSION 3.25.1)

if (NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX "$ENV{HOME}" CACHE STRING " " FORCE)
endif()

project(sensehat
        LANGUAGES C
        DESCRIPTION "Raspberry Pi Sense Hat C API"
        VERSION 1.0)

set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES_LIST src/framebuffer.c src/joystick.c)

add_library(sensehat STATIC ${SOURCE_FILES_LIST})

target_include_directories(sensehat PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/sensehat>)

# Installs the library into ~/lib/sensehat
install(TARGETS sensehat ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/sensehat)

# Copies header files into ~/include/sensehat
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/sensehat)

# Test using static library
set(APP_EXE libsensehat_test)
add_executable(${APP_EXE} main.c)
target_link_libraries(${APP_EXE} sensehat)
