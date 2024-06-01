find_package(GTest REQUIRED)
include_directories(${GTest_INCLUDE_DIR})

set(NBTP_TESTS
		BasicParseTest
		FormatterTest
		TagTest
		FloatBigEndianTest
		ContainerTagTest
		RealParseTest
		UnparseTest
		SingleValuedTagTest
		)

foreach(TEST_FILE ${NBTP_TESTS})
	add_executable(${TEST_FILE} ${TEST_FILE}.cpp memstream.h)
	target_compile_definitions(${TEST_FILE} PRIVATE TEST_BINARY_DATA_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/test_data\")
	target_link_libraries(${TEST_FILE} PUBLIC ${Boost_LIBRARIES})
	target_link_libraries(${TEST_FILE} PUBLIC nbtp)
	# Probably not the right way to link but eh
	target_link_libraries(${TEST_FILE} PUBLIC gtest)
	gtest_discover_tests(${TEST_FILE})
endforeach()