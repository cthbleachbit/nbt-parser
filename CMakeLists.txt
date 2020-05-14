cmake_minimum_required (VERSION 3.0)
project (NBTP)
set(VERSION "0.1.1")

# Dependencies
find_package(PkgConfig REQUIRED)
#pkg_check_modules(ZLIB REQUIRED zlib)
#pkg_check_modules(YAML REQUIRED yaml-0.1)
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
	if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
		add_compile_options("-fprofile-arcs")
		add_compile_options("-ftest-coverage")
		add_link_options("-lgcov")
		add_link_options("--coverage")
	elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
		add_compile_options("-fprofile-instr-generate")
		add_compile_options("-fcoverage-mapping")
	endif()
endif()
option(NBTP_O2 "Build binary with optimization" ON)
if(NBTP_O2)
	add_compile_options("-O2")
endif()
option(NBTP_BUILD_TEST "Build unit tests" OFF)
option(NBTP_BUILD_PYTHON_MODULE "Build python module" ON)
set(USE_PYTHON_INCLUDE_DIR ON CACHE BOOL "")
set(PYTHON_MODULES_DIR "/lib/python3.8/site-packages" CACHE STRING "Directory relative to CMAKE_INSTALL_PREFIX to install pyNBTP")

# Compilation options
set (CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall -Wno-unused-variable)
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
if(NBTP_BUILD_TEST OR (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	add_subdirectory(boost_tests)
endif()
if(NBTP_BUILD_PYTHON_MODULE)
	add_subdirectory(pybind11)
	add_subdirectory(pynbtp)
endif()

# Resources

# Testing
enable_testing()
