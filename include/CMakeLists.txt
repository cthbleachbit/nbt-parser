set(NBTP_PUBLIC_HEADERS
		libnbtp.h

		Tag.h
		ByteTag.h
		ShortTag.h
		IntTag.h
		CompoundTag.h
		ListTag.h

		constants.h
		)

install (FILES ${NBTP_PUBLIC_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR}/nbtp)