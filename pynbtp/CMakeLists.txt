pybind11_add_module(pynbtp MODULE SYSTEM THIN_LTO pynbtp.cpp)
target_link_libraries(pynbtp PUBLIC nbtp)