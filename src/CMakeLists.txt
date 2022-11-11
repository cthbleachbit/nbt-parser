set(LIBNBTP_SRC
		tags/Tag.cpp
		tags/ListTag.cpp
		tags/CompoundTag.cpp
		tags/StringTag.cpp

		Conversion.cpp
		TagIO.cpp
		)

set(NBTP_CLI_SRC
		nbtp-cli.cpp
		)

if (NOT WIN32)
	add_library(nbtp SHARED ${LIBNBTP_SRC})
	target_link_libraries(nbtp PUBLIC fmt::fmt)
	set_target_properties(nbtp PROPERTIES VERSION ${VERSION})
	set_target_properties(nbtp PROPERTIES SOVERSION ${SOVERSION})

	install(TARGETS nbtp DESTINATION ${INSTALL_LIB_DIR})
endif()

if (NBTP_BUILD_STATIC_LIB OR WIN32)
	add_library(nbtp-static STATIC ${LIBNBTP_SRC})
	target_link_libraries(nbtp-static PRIVATE fmt::fmt-header-only)
	set_target_properties(nbtp-static PROPERTIES OUTPUT_NAME nbtp)

	if (WIN32)
		target_link_libraries(nbtp-static PRIVATE ws2_32)
	endif()
	install(TARGETS nbtp-static DESTINATION ${INSTALL_LIB_DIR})
endif()

add_executable(nbtp-cli ${NBTP_CLI_SRC})
if (WIN32)
	target_link_libraries(nbtp-cli nbtp-static)
else()
	target_link_libraries(nbtp-cli nbtp)
endif()

install(TARGETS nbtp-cli DESTINATION ${INSTALL_BIN_DIR})


