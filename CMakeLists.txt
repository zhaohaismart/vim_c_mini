cmake_minimum_required(VERSION 3.16)
project (myvim)

set (myvim_VERSION_MAJOR 1)
set (myvim_VERSION_MINOR 0)

set(ENV{PKG_CONFIG_PATH} /usr/loca/lib/pkgconfig)
find_package(PkgConfig)
message(STATUS "-----  PKG_CONIG_FOUND: ${PKG_CONIG_FOUND}")
message(STATUS "--- PKG_CONFIG_VERSION_STRING: ${PKG_CONFIG_VERSION_STRING}")

pkg_search_module(uv REQUIRED libuv)
add_executable(myvim main.c)
target_link_libraries(myvim uv)
