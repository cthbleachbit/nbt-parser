set(NBTP_SRC
	parser.c
	)

add_library(nbtp SHARED ${NBTP_SRC})

pkg_check_modules(ZLIB REQUIRED zlib)
target_link_libraries(nbtp ${ZLIB_LIBRARIES})
target_include_directories(nbtp PUBLIC ${ZLIB_INCLUDE_DIRS})
target_compile_options(nbtp PUBLIC ${ZLIB_CFLAGS_OTHER})

install (TARGETS nbtp DESTINATION lib)
