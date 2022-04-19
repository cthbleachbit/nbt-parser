set(NBTP_PC ${CMAKE_CURRENT_BINARY_DIR}/nbtp.pc)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/nbtp.pc.in ${NBTP_PC} @ONLY)
install(FILES ${NBTP_PC} DESTINATION ${PKGCONFIG_INSTALL_DIR})

include(CMakePackageConfigHelpers)
configure_package_config_file(nbtp-config.cmake.in
		${CMAKE_CURRENT_BINARY_DIR}/nbtp-config.cmake
		INSTALL_DESTINATION ${LIB_INSTALL_DIR}/cmake/nbtp
		PATH_VARS INSTALL_INCLUDE_DIR)
write_basic_package_version_file(
		${CMAKE_CURRENT_BINARY_DIR}/nbtp-config-version.cmake
		VERSION ${VERSION}
		COMPATIBILITY SameMajorVersion )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/nbtp-config.cmake
		${CMAKE_CURRENT_BINARY_DIR}/nbtp-config-version.cmake
		DESTINATION ${LIB_INSTALL_DIR}/cmake/nbtp )