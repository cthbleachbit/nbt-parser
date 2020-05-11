cmake_minimum_required (VERSION 3.0)
project (NBTP)
set(VERSION "0.0.2")

# Dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZLIB REQUIRED zlib)
pkg_check_modules(YAML REQUIRED yaml-0.1)
find_package(Boost REQUIRED)

# Version numbers
set(VERSION_STRING ${VERSION})
add_definitions(-DNBTP_VERSION=\"${VERSION_STRING}\")
execute_process(COMMAND git rev-parse HEAD
		OUTPUT_VARIABLE GIT_SHA1
		RESULT_VARIABLE GIT_RET_VAL)

if(${GIT_RET_VAL} EQUAL 0)
	string(SUBSTRING ${GIT_SHA1} 0 8 GIT_SHORT_REV)
	add_definitions(-DNBTP_GIT_SHA1=\"${GIT_SHORT_REV}\")
endif(${GIT_RET_VAL} EQUAL 0)

# Environment checks
include(TestBigEndian)
TEST_BIG_ENDIAN(SYSTEM_BIG)
if (SYSTEM_BIG)
	add_definitions(-DSYSTEM_IS_BIG_ENDIAN)
endif()

# Built setup
option(NBTP_DEBUG "Build binary with debug symbols" OFF)
if(NBTP_DEBUG OR (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	add_compile_options("-g")
endif()
option(NBTP_O2 "Build binary with optimization" ON)
if(NBTP_O2)
	add_compile_options("-O2")
endif()
option(NBTP_BUILD_TEST "Build unit tests" OFF)

# Compilation options
set (CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall -Wno-unused-variable)
set(INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include CACHE PATH "Installation directory for headers")
set(INSTALL_PKGCONFIG_DIR ${CMAKE_INSTALL_PREFIX}/share/pkgconfig CACHE PATH "Installation directory for pkgconfig (.pc) files")

add_definitions(-DBEGIN_NBTP_NS=namespace\ NBTP\ {)
add_definitions(-DEND_NBTP_NS=})

# Source files
include_directories(include)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(pkgconfig)
if(NBTP_BUILD_TEST OR (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	add_subdirectory(boost_tests)
endif()

# Resources

# Testing
enable_testing()
