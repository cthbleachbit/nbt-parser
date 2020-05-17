cmake_minimum_required(VERSION 3.0)
project(NBTP)
set(VERSION "0.1.1")

# Dependencies
find_package(Boost REQUIRED)

# Version numbers
set(VERSION_STRING ${VERSION})
add_compile_definitions(NBTP_VERSION=\"${VERSION_STRING}\")
execute_process(COMMAND git rev-parse HEAD
		OUTPUT_VARIABLE GIT_SHA1
		RESULT_VARIABLE GIT_RET_VAL)

if (${GIT_RET_VAL} EQUAL 0)
	string(SUBSTRING ${GIT_SHA1} 0 8 GIT_SHORT_REV)
	add_compile_definitions(NBTP_GIT_SHA1=\"${GIT_SHORT_REV}\")
endif (${GIT_RET_VAL} EQUAL 0)

# Environment checks
include(TestBigEndian)
TEST_BIG_ENDIAN(SYSTEM_BIG)
if (SYSTEM_BIG)
	add_compile_definitions(SYSTEM_IS_BIG_ENDIAN)
endif ()

# User options
option(NBTP_DEBUG "Build binary with debug symbols" OFF)
option(NBTP_O2 "Build binary with optimization" ON)
option(NBTP_BUILD_TEST "Build unit tests" OFF)
option(NBTP_BUILD_PYTHON_MODULE "Build python module" ON)
set(USE_PYTHON_INCLUDE_DIR ON CACHE BOOL "")
set(PYTHON_MODULES_DIR "/lib/python3.8/site-packages" CACHE STRING "Directory relative to CMAKE_INSTALL_PREFIX to install pyNBTP")
option(NBTP_BUILD_STATIC_LIB "Build static library as well (Unix only)" OFF)

# Windows hacks
if (NBTP_DEBUG OR (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	if (NOT MSVC)
		add_compile_options("-g")
		add_compile_options("-Wall")
		add_compile_options("-Wno-unused-variable")
	else()
		add_compile_definitions(ssize_t=uint64_t)
	endif ()
	if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
		add_compile_options("-fprofile-arcs")
		add_compile_options("-ftest-coverage")
		add_link_options("-lgcov")
		add_link_options("--coverage")
	elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
		add_compile_options("-fprofile-instr-generate")
		add_compile_options("-fcoverage-mapping")
	endif ()
endif ()
if (NBTP_O2 AND NOT MSVC)
	add_compile_options("-O2")
endif ()

# Compilation options
set(CMAKE_CXX_STANDARD 14)
set(INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include CACHE PATH "Installation directory for headers")
set(INSTALL_PKGCONFIG_DIR ${CMAKE_INSTALL_PREFIX}/share/pkgconfig CACHE PATH "Installation directory for pkgconfig (.pc) files")
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH ${INSTALL_LIB_DIR})

# Source files
include_directories(include)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(pkgconfig)
if (NBTP_BUILD_TEST OR (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	if (NOT MSVC)
		add_subdirectory(boost_tests)
	endif()
endif ()
if (NBTP_BUILD_PYTHON_MODULE)
	add_subdirectory(pybind11)
	add_subdirectory(pynbtp)
endif ()

# Resources

# Testing
enable_testing()
