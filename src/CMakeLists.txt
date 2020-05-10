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
        conv.cpp
        )

set(NBTP_SRC
        nbtp.cpp
        )

add_library(nbtp SHARED ${LIBNBTP_SRC})

target_link_libraries(nbtp ${ZLIB_LIBRARIES})
target_include_directories(nbtp PUBLIC ${ZLIB_INCLUDE_DIRS})
target_compile_options(nbtp PUBLIC ${ZLIB_CFLAGS_OTHER})

target_link_libraries(nbtp ${YAML_LIBRARIES})
target_include_directories(nbtp PUBLIC ${YAML_INCLUDE_DIRS})
target_compile_options(nbtp PUBLIC ${YAML_CFLAGS_OTHER})

target_link_libraries(nbtp ${Boost_LIBRARIES})
target_include_directories(nbtp PUBLIC ${Boost_INCLUDE_DIRS})


add_executable(nbtp-cli ${NBTP_SRC})
target_link_libraries(nbtp-cli nbtp)

install(TARGETS nbtp DESTINATION ${INSTALL_LIB_DIR})
install(TARGETS nbtp-cli DESTINATION ${INSTALL_BIN_DIR})


