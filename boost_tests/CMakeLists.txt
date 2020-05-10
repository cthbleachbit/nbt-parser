set (Boost_USE_STATIC_LIBS OFF)
find_package (Boost REQUIRED COMPONENTS unit_test_framework)
include_directories (${Boost_INCLUDE_DIRS})

add_executable(boost_tests_run TagTest.cpp)
target_link_libraries(boost_tests_run ${Boost_LIBRARIES})
target_link_libraries(boost_tests_run nbtp)