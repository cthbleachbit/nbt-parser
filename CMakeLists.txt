cmake_minimum_required (VERSION 3.0)
project (NBTP)
set(VERSION "0.0.1")

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZLIB REQUIRED zlib)

set(warnings "-g -Wall -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${warnings}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")
set(INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include CACHE PATH "Installation directory for headers")
set(INSTALL_PKGCONFIG_DIR ${CMAKE_INSTALL_PREFIX}/share/pkgconfig CACHE PATH "Installation directory for pkgconfig (.pc) files")

# Source files
add_subdirectory(src)
add_subdirectory(pkgconfig)

# Resources

# Testing
enable_testing()
