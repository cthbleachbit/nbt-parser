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
	using namespace NBTP;
	m.doc() = "Python bindings for libnbtp";

	py::class_<Tag, std::shared_ptr<Tag>>(m, "Tag");
	py::class_<SingleValuedTag<int32_t>, std::shared_ptr<SingleValuedTag<int32_t>>>(m, "SingleValuedInt32Tag");
	py::class_<ContainerTag, Tag, std::shared_ptr<ContainerTag>>(m, "ContainerTag");

	py::class_<IntTag, SingleValuedTag<int32_t>, Tag, std::shared_ptr<IntTag>>(m, "IntTag")
			.def(py::init<int32_t>())
			.def("setPayload", &IntTag::setPayload)
			.def("getPayload", &IntTag::getPayload)
			.def("__repr__", [](const IntTag &t) {
				std::stringstream ss;
				t.textOutput(ss, 0);
				return ss.str();
			});
	py::class_<CompoundTag, ContainerTag, Tag, std::shared_ptr<CompoundTag>>(m, "CompoundTag")
			.def(py::init<>())
			.def("insert", &CompoundTag::insert)
			.def("lookup", &CompoundTag::lookup)
			.def("__repr__", [](const CompoundTag &t) {
				std::stringstream ss;
				t.textOutput(ss, 0);
				return ss.str();
			});;
	m.def("mksimple", &mksimple, "A function which adds two numbers");
}