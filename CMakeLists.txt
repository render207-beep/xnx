cmake_minimum_required(VERSION 3.15)
project(XNX LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

# Borealis como submódulo: git submodule add https://github.com/xfangfang/borealis third_party/borealis
add_subdirectory(third_party/borealis/library)

add_executable(XNX
    source/main.cpp
    source/config.cpp
    source/api/http_client.cpp
    source/api/x_client.cpp
    source/ui/timeline_view.cpp)

target_include_directories(XNX PRIVATE source)
target_link_libraries(XNX PRIVATE borealis curl)

if (PLATFORM_SWITCH)
    # Funciones de devkitPro (Platform/NintendoSwitch.cmake)
    nx_generate_nacp(XNX.nacp NAME "X NX" AUTHOR "xnx" VERSION "0.1.0")
    nx_create_nro(XNX NACP XNX.nacp ROMFS ${CMAKE_SOURCE_DIR}/resources)
endif()
