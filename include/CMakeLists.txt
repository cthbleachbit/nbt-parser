set(NBTP_PUBLIC_HEADERS
		libnbtp.h

		tags/Tag.h
		tags/ByteTag.h
		tags/ShortTag.h
		tags/IntTag.h
		tags/LongTag.h
		tags/FloatTag.h
		tags/DoubleTag.h
		tags/BytesTag.h
		tags/IntsTag.h
		tags/CompoundTag.h
		tags/ListTag.h

		constants.h
		conv.h
		)

install (FILES ${NBTP_PUBLIC_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR}/nbtp)