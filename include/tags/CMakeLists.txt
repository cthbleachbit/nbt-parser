set(NBTP_TAGS_HEADERS
		Tag.h
		SingleValuedTag.h
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
		BytesTag.h
		IntsTag.h
		LongsTag.h
		)

install(FILES ${NBTP_TAGS_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR}/nbtp/tags)
if (NBTP_INSTALL_DEPRECATED_HEADER)
	install(FILES ${NBTP_TAGS_HEADERS_DEPRECATED} DESTINATION ${INSTALL_INCLUDE_DIR}/nbtp/tags)
endif()