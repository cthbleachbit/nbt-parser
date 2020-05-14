pybind11_add_module(pynbtp MODULE SYSTEM THIN_LTO pynbtp.cpp PyBytesBuf.h)
target_link_libraries(pynbtp PUBLIC nbtp)
install(TARGETS pynbtp DESTINATION ${CMAKE_INSTALL_PREFIX}/${PYTHON_MODULES_DIR})