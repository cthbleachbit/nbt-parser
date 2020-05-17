set(LIBNBTP_SRC
		tags/Tag.cpp

		tags/ByteTag.cpp
		tags/ShortTag.cpp
		tags/IntTag.cpp
		tags/LongTag.cpp
		tags/FloatTag.cpp
		tags/DoubleTag.cpp
		tags/BytesTag.cpp
		tags/IntsTag.cpp
		tags/LongsTag.cpp
		tags/ListTag.cpp
		tags/CompoundTag.cpp
		tags/StringTag.cpp

		Conversion.cpp
		Logging.cpp
		TagIO.cpp
		)

set(NBTP_SRC
		nbtp.cpp
		)

if (NOT MSVC)
	add_library(nbtp SHARED ${LIBNBTP_SRC})

	target_link_libraries(nbtp ${Boost_LIBRARIES})
	target_include_directories(nbtp PUBLIC ${Boost_INCLUDE_DIRS})

	install(TARGETS nbtp DESTINATION ${INSTALL_LIB_DIR})
endif()

if (NBTP_BUILD_STATIC_LIB OR MSVC)
	add_library(nbtp-static STATIC ${LIBNBTP_SRC})
	set_target_properties(nbtp-static PROPERTIES OUTPUT_NAME nbtp)

	if (MSVC)
		target_link_libraries(nbtp-static ws2)
	endif()
	target_link_libraries(nbtp-static ${Boost_LIBRARIES})
	target_include_directories(nbtp-static PUBLIC ${Boost_INCLUDE_DIRS})
	install(TARGETS nbtp-static DESTINATION ${INSTALL_LIB_DIR})
endif()

add_executable(nbtp-cli ${NBTP_SRC})
if (MSVC)
	target_link_libraries(nbtp-cli nbtp-static)
else()
	target_link_libraries(nbtp-cli nbtp)
endif()
target_link_libraries(nbtp-cli ${Boost_LIBRARIES})
target_include_directories(nbtp-cli PUBLIC ${Boost_INCLUDE_DIRS})

install(TARGETS nbtp-cli DESTINATION ${INSTALL_BIN_DIR})


