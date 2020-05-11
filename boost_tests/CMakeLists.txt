set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(boost_tag_test TagTest.cpp)
target_link_libraries(boost_tag_test ${Boost_LIBRARIES})
target_link_libraries(boost_tag_test nbtp)

add_executable(boost_ptr_test PtrTest.cpp)
target_link_libraries(boost_ptr_test ${Boost_LIBRARIES})
target_link_libraries(boost_ptr_test nbtp)

add_executable(boost_multi_tag_test MultiTagTest.cpp)
target_link_libraries(boost_multi_tag_test ${Boost_LIBRARIES})
target_link_libraries(boost_multi_tag_test nbtp)

add_executable(boost_basic_parse_test BasicParseTest.cpp memstream.h)
target_link_libraries(boost_basic_parse_test ${Boost_LIBRARIES})
target_link_libraries(boost_basic_parse_test nbtp)

add_executable(boost_unparse_test UnparseTest.cpp memstream.h)
target_link_libraries(boost_unparse_test ${Boost_LIBRARIES})
target_link_libraries(boost_unparse_test nbtp)

add_executable(boost_real_parse_test RealParseTest.cpp)
target_link_libraries(boost_real_parse_test ${Boost_LIBRARIES})
target_link_libraries(boost_real_parse_test nbtp)


add_subdirectory(test_data)