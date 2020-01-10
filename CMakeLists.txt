cmake_minimum_required(VERSION 3.15)
project(systatus C)

set(CMAKE_C_STANDARD 99)

add_executable(systatus main.c)

target_link_libraries(systatus m)

if(DEFINED ENV{INSTALL_DIR})
    install(TARGETS systatus RUNTIME DESTINATION $ENV{INSTALL_DIR})
else()
    install(TARGETS systatus RUNTIME DESTINATION ~/.local/bin)
endif()
