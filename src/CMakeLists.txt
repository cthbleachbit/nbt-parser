set(LIBNBTP_SRC
	parser.c
	compound.c
	tag.c
	util.c
	)

set(NBTP_SRC
	nbtp.c
	)

add_library(nbtp SHARED ${LIBNBTP_SRC})
add_executable(nbtp-cli ${NBTP_SRC})

pkg_check_modules(ZLIB REQUIRED zlib)
target_link_libraries(nbtp ${ZLIB_LIBRARIES})
target_include_directories(nbtp PUBLIC ${ZLIB_INCLUDE_DIRS})
target_compile_options(nbtp PUBLIC ${ZLIB_CFLAGS_OTHER})

target_link_libraries(nbtp-cli nbtp)

install (TARGETS nbtp DESTINATION lib)
install (TARGETS nbtp-cli DESTINATION bin)
