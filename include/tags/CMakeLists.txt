set(NBTP_TAGS_HEADERS
		Tag.h
		SingleValuedTag.h
		ByteTag.h
		ShortTag.h
		IntTag.h
		LongTag.h
		FloatTag.h
		DoubleTag.h
		BytesTag.h
		IntsTag.h
		LongsTag.h
		CompoundTag.h
		ListTag.h
		StringTag.h
		)

install(FILES ${NBTP_TAGS_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR}/nbtp/tags)