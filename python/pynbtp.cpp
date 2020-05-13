//
// Created by cth451 on 2020/05/12.
//

#include <pybind11/pybind11.h>
#include "libnbtp.h"

std::shared_ptr<NBTP::CompoundTag> mksimple() {
	auto tag = std::make_shared<NBTP::CompoundTag>();
	tag->insert("tag1", std::make_shared<NBTP::IntTag>(14));
	return tag;
}

PYBIND11_MODULE(pynbtp, m) {
	m.doc() = "Python bindings for libnbtp";

	m.def("mksimple", &mksimple, "A function which adds two numbers");
}