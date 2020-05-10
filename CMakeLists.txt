cmake_minimum_required (VERSION 3.0)
project (NBTP)
set(VERSION "0.0.1")

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZLIB REQUIRED zlib)
find_package(Boost REQUIRED)
pkg_check_modules(YAML REQUIRED yaml-0.1)

set(VERSION_STRING ${VERSION} CACHE STRING "VERSION_STRING")
add_definitions(-DVERSION=\"${VERSION_STRING}\")

set (CMAKE_CXX_STANDARD 11)

execute_process(COMMAND git rev-parse HEAD
		OUTPUT_VARIABLE GIT_SHA1
		RESULT_VARIABLE GIT_RET_VAL)

if(${GIT_RET_VAL} EQUAL 0)
	string(SUBSTRING ${GIT_SHA1} 0 8 GIT_SHORT_REV)
	add_definitions(-DGIT_SHA1=\"${GIT_SHORT_REV}\")
endif(${GIT_RET_VAL} EQUAL 0)

set(warnings "-g -Wall -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${warnings}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")
set(INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include CACHE PATH "Installation directory for headers")
set(INSTALL_PKGCONFIG_DIR ${CMAKE_INSTALL_PREFIX}/share/pkgconfig CACHE PATH "Installation directory for pkgconfig (.pc) files")

add_definitions(-DBEGIN_NBTP_NS=namespace\ NBTP\ {)
add_definitions(-DEND_NBTP_NS=})

# Source files
include_directories(src)
add_subdirectory(src)
add_subdirectory(pkgconfig)
add_subdirectory(boost_tests)

# Resources

# Testing
enable_testing()
