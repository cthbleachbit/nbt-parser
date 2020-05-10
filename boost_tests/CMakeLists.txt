set (Boost_USE_STATIC_LIBS OFF)
find_package (Boost REQUIRED COMPONENTS unit_test_framework)
include_directories (${Boost_INCLUDE_DIRS})

add_executable(boost_tag_test TagTest.cpp)
target_link_libraries(boost_tag_test ${Boost_LIBRARIES})
target_link_libraries(boost_tag_test nbtp)

add_executable(boost_ptr_Test PtrTest.cpp)
target_link_libraries(boost_ptr_Test ${Boost_LIBRARIES})
target_link_libraries(boost_ptr_Test nbtp)