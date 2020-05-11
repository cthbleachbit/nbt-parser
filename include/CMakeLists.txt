set(NBTP_PUBLIC_HEADERS
		libnbtp.h
		constants.h
		conv.h
		)

install(FILES ${NBTP_PUBLIC_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR}/nbtp)
add_subdirectory(tags)