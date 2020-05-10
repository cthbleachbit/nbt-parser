set(NBTP_PUBLIC_HEADERS
		libnbtp.h

		Tag.h
		ByteTag.h
		CompoundTag.h

		constants.h
		)

install (FILES ${NBTP_PUBLIC_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR}/nbtp)