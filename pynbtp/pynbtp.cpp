//
// Created by cth451 on 2020/05/12.
//

#include <pybind11/pybind11.h>
#include <pybind11/iostream.h>
#include <pybind11/stl.h>
#include <sstream>
#include "libnbtp.h"
#include "PyBytesBuf.h"

namespace pyNBTP {
	std::shared_ptr<NBTP::CompoundTag> mksimple() {
		auto tag = std::make_shared<NBTP::CompoundTag>();
		tag->insert("tag1", std::make_shared<NBTP::IntTag>(14));
		return tag;
	}

	void pyOutputTag(NBTP::Tag *tag, pybind11::object &io, NBTP::IOFormat format) {
		if (format == NBTP::PRETTY_PRINT) {
			pybind11::detail::pythonbuf buffer(io);
			std::ostream out(&buffer);
			tag->output(out, format);
		} else if (format == NBTP::BIN) {
			PyBytesBuf buffer(io);
			std::ostream out(&buffer);
			tag->output(out, format);
		}
	}
}

namespace py = pybind11;

PYBIND11_MODULE(pynbtp, m) {
	using namespace NBTP;
	m.doc() = "Python bindings for libnbtp";

	py::enum_<TagType>(m, "TagType")
			.value("BYTE", TagType::BYTE)
			.value("SHORT", TagType::SHORT)
			.value("INT", TagType::INT)
			.value("LONG", TagType::LONG)
			.value("FLOAT", TagType::FLOAT)
			.value("DOUBLE", TagType::DOUBLE)
			.value("LIST", TagType::LIST)
			.value("COMPOUND", TagType::COMPOUND)
			.value("STRING", TagType::STRING)
			.value("BYTES", TagType::BYTES)
			.value("INTS", TagType::INTS)
			.value("LONGS", TagType::LONGS)
			.value("END", TagType::END);
	py::enum_<IOFormat>(m, "IOFormat")
			.value("BIN", IOFormat::BIN)
			.value("PRETTY_PRINT", IOFormat::PRETTY_PRINT);

	py::class_<Tag, std::shared_ptr<Tag>>(m, "Tag");
	py::class_<SingleValuedTag<int32_t>, Tag, std::shared_ptr<SingleValuedTag<int32_t>>>(m, "__SingleValuedInt32Tag");
	py::class_<SingleValuedTag<int64_t>, Tag, std::shared_ptr<SingleValuedTag<int64_t>>>(m, "__SingleValuedInt64Tag");
	py::class_<ContainerTag, Tag, std::shared_ptr<ContainerTag>>(m, "ContainerTag");

	py::class_<IntTag, SingleValuedTag<int32_t>, Tag, std::shared_ptr<IntTag>>(m, "IntTag")
			.def(py::init<int32_t>())
			.def("typeCode", &IntTag::typeCode)
			.def("setPayload", &IntTag::setPayload)
			.def("getPayload", &IntTag::getPayload)
			.def("__repr__", &IntTag::toString)
			.def("output", pyNBTP::pyOutputTag);
	py::class_<CompoundTag, ContainerTag, Tag, std::shared_ptr<CompoundTag>>(m, "CompoundTag")
			.def(py::init<>())
			.def("typeCode", &CompoundTag::typeCode)
			.def("insert", &CompoundTag::insert)
			.def("lookup", &CompoundTag::lookup)
			.def("__repr__", &CompoundTag::toString)
			.def("output", pyNBTP::pyOutputTag);
	m.def("mksimple", &pyNBTP::mksimple, "A function which adds two numbers");
}