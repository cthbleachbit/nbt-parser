set(NBTP_TAGS_HEADERS
		Tag.h
		SingleValuedTag.h
		BytesTag.h
		IntsTag.h
		LongsTag.h
		CompoundTag.h
		ListTag.h
		StringTag.h
		)

set(NBTP_TAGS_HEADERS_DEPRECATED
		ByteTag.h
		ShortTag.h
		IntTag.h
		LongTag.h
		FloatTag.h
		DoubleTag.h
		)

install(FILES ${NBTP_TAGS_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR}/nbtp/tags)
install(FILES ${NBTP_TAGS_HEADERS_DEPRECATED} DESTINATION ${INSTALL_INCLUDE_DIR}/nbtp/tags)