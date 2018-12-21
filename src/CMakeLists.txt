
set(LIBNBTP_SRC
	parser.c
	compound.c
	tag.c
	util.c
	)

set(NBTP_SRC
	nbtp.c
	)

set(NBTP_HEADER
	compound.h
	nbtp.h
	parser.h
	tag.h
	util.h
	)


add_library(nbtp SHARED ${LIBNBTP_SRC})
add_executable(nbtp-cli ${NBTP_SRC})

pkg_check_modules(ZLIB REQUIRED zlib)
target_link_libraries(nbtp ${ZLIB_LIBRARIES})
target_include_directories(nbtp PUBLIC ${ZLIB_INCLUDE_DIRS})
target_compile_options(nbtp PUBLIC ${ZLIB_CFLAGS_OTHER})
pkg_check_modules(YAML REQUIRED yaml-0.1)
target_link_libraries(nbtp ${YAML_LIBRARIES})
target_include_directories(nbtp PUBLIC ${YAML_INCLUDE_DIRS})
target_compile_options(nbtp PUBLIC ${YAML_CFLAGS_OTHER})

target_link_libraries(nbtp-cli nbtp)

install (TARGETS nbtp DESTINATION ${INSTALL_LIB_DIR})
install (TARGETS nbtp-cli DESTINATION ${INSTALL_BIN_DIR})

install (FILES ${NBTP_HEADER} DESTINATION ${INSTALL_INCLUDE_DIR}/nbtp)
