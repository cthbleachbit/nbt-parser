//
// Created by cth451 on 2020/05/12.
//

#include <pybind11/pybind11.h>
#include <sstream>
#include "libnbtp.h"

std::shared_ptr<NBTP::CompoundTag> mksimple() {
	auto tag = std::make_shared<NBTP::CompoundTag>();
	tag->insert("tag1", std::make_shared<NBTP::IntTag>(14));
	return tag;
}

namespace py = pybind11;

PYBIND11_MODULE(pynbtp, m) {
	m.doc() = "Python bindings for libnbtp";

	py::class_<NBTP::IntTag>(m, "IntTag")
	        .def(py::init<int32_t>())
	        .def("setPayload", &NBTP::IntTag::setPayload)
	        .def("getPayload", &NBTP::IntTag::getPayload);
	py::class_<NBTP::CompoundTag>(m, "CompoundTag")
			.def(py::init<>())
			.def("insert", &NBTP::CompoundTag::insert)
			.def("lookup", &NBTP::CompoundTag::lookup);
	m.def("mksimple", &mksimple, "A function which adds two numbers");
}