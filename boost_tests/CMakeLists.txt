set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

set(NBTP_TESTS
		TagTest
		PtrTest
		MultiTagTest
		BasicParseTest
		UnparseTest
		RealParseTest
		)

foreach(TEST_FILE ${NBTP_TESTS})
	add_executable(${TEST_FILE} ${TEST_FILE}.cpp memstream.h)
	target_link_libraries(${TEST_FILE} PUBLIC ${Boost_LIBRARIES})
if(WIN32)
	target_link_libraries(${TEST_FILE} PRIVATE nbtp-static)
else()
	target_link_libraries(${TEST_FILE} PUBLIC nbtp)
endif()
	add_test(NAME ${TEST_FILE} COMMAND ${TEST_FILE})
endforeach()

add_subdirectory(test_data)