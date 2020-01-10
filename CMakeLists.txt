cmake_minimum_required(VERSION 3.15)
project(dmenuinfo C)

set(CMAKE_C_STANDARD 99)

add_executable(dmenuinfo main.c)

target_link_libraries(dmenuinfo m)

if(DEFINED ENV{INSTALL_DIR})
    install(TARGETS dmenuinfo RUNTIME DESTINATION $ENV{INSTALL_DIR})
else()
    install(TARGETS dmenuinfo RUNTIME DESTINATION ~/.local/bin)
endif()
