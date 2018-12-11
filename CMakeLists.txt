cmake_minimum_required (VERSION 3.0)
project (NBTP)
set (NBTP_VERSION_MAJOR 0)
set (NBTP_VERSION_MINOR 0)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZLIB REQUIRED zlib)

set(warnings "-g -Wall -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${warnings}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

# Source files
add_subdirectory(src)

# Resources

# Testing
enable_testing()
