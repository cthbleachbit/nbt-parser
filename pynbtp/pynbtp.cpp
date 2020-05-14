//
// Created by cth451 on 2020/05/12.
//

#include <pybind11/pybind11.h>
#include <pybind11/iostream.h>
#include <pybind11/stl.h>
#include <sstream>
#include "libnbtp.h"
#include "PyBytesBuf.h"

namespace py = pybind11;

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
			PyOBytesBuf buffer(io);
			std::ostream out(&buffer);
			tag->output(out, format);
		}
	}

	pybind11::object pyParseRoot(pybind11::object &io) {
		ssize_t counter;
		PyIBytesBuf buffer(io);
		std::istream in(&buffer);
		return pybind11::make_tuple(NBTP::TagIO::parseRoot(in, counter), counter);
	}
}

PYBIND11_MODULE(pynbtp, m) {


	// Enums
	{
		using namespace NBTP;
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
	}

	{
		using namespace NBTP;
		// Virtual Classes
		py::class_<Tag, std::shared_ptr<Tag>>(m, "Tag")
				.def("output", pyNBTP::pyOutputTag)
				.def("typeCode", &Tag::typeCode)
				.def("__repr__", &Tag::toString);
		py::class_<SingleValuedTag<int8_t>, Tag, std::shared_ptr<SingleValuedTag<int8_t>>>(m, "__SingleValuedInt8Tag");
		py::class_<SingleValuedTag<int16_t>, Tag, std::shared_ptr<SingleValuedTag<int16_t>>>(m,
		                                                                                     "__SingleValuedInt16Tag");
		py::class_<SingleValuedTag<int32_t>, Tag, std::shared_ptr<SingleValuedTag<int32_t>>>(m,
		                                                                                     "__SingleValuedInt32Tag");
		py::class_<SingleValuedTag<int64_t>, Tag, std::shared_ptr<SingleValuedTag<int64_t>>>(m,
		                                                                                     "__SingleValuedInt64Tag");
		py::class_<SingleValuedTag<float>, Tag, std::shared_ptr<SingleValuedTag<float>>>(m, "__SingleValuedFloatTag");
		py::class_<SingleValuedTag<double>, Tag, std::shared_ptr<SingleValuedTag<double>>>(m,
		                                                                                   "__SingleValuedDoubleTag");
		py::class_<ContainerTag, Tag, std::shared_ptr<ContainerTag>>(m, "ContainerTag")
				.def("size", &ContainerTag::size);

		// Base classes
		py::class_<ListTag, ContainerTag, Tag, std::shared_ptr<ListTag>>(m, "ListTag")
				.def(py::init<>())
				.def("insert", &ListTag::insert)
				.def("setContentType", &ListTag::setContentType)
				.def("getContentType", &ListTag::getContentType)
				.def("getPayload", &ListTag::getPayload);
		py::class_<CompoundTag, ContainerTag, Tag, std::shared_ptr<CompoundTag>>(m, "CompoundTag")
				.def(py::init<>())
				.def("insert", &CompoundTag::insert)
				.def("lookup", &CompoundTag::lookup)
				.def("getPayload", &CompoundTag::getPayload);
		py::class_<StringTag, ContainerTag, Tag, std::shared_ptr<StringTag>>(m, "StringTag")
				.def(py::init<>())
				.def(py::init<std::string &>())
				.def("setPayload", &StringTag::setPayload)
				.def("getPayload", &StringTag::getPayload);

		// Single Valued Tags
		py::class_<ByteTag, SingleValuedTag<int8_t>, Tag, std::shared_ptr<ByteTag>>(m, "ByteTag")
				.def(py::init<int8_t>())
				.def("setPayload", &ByteTag::setPayload)
				.def("getPayload", &ByteTag::getPayload);
		py::class_<ShortTag, SingleValuedTag<int16_t>, Tag, std::shared_ptr<ShortTag>>(m, "ShortTag")
				.def(py::init<int16_t>())
				.def("setPayload", &ShortTag::setPayload)
				.def("getPayload", &ShortTag::getPayload);
		py::class_<IntTag, SingleValuedTag<int32_t>, Tag, std::shared_ptr<IntTag>>(m, "IntTag")
				.def(py::init<int32_t>())
				.def("setPayload", &IntTag::setPayload)
				.def("getPayload", &IntTag::getPayload);
		py::class_<LongTag, SingleValuedTag<int64_t>, Tag, std::shared_ptr<LongTag>>(m, "LongTag")
				.def(py::init<int64_t>())
				.def("setPayload", &LongTag::setPayload)
				.def("getPayload", &LongTag::getPayload);
		py::class_<FloatTag, SingleValuedTag<float>, Tag, std::shared_ptr<FloatTag>>(m, "FloatTag")
				.def(py::init<float>())
				.def("setPayload", &FloatTag::setPayload)
				.def("getPayload", &FloatTag::getPayload);
		py::class_<DoubleTag, SingleValuedTag<double>, Tag, std::shared_ptr<DoubleTag>>(m, "DoubleTag")
				.def(py::init<double>())
				.def("setPayload", &DoubleTag::setPayload)
				.def("getPayload", &DoubleTag::getPayload);

		// Types that extends ListTag
		py::class_<BytesTag, ListTag, ContainerTag, Tag, std::shared_ptr<BytesTag>>(m, "BytesTag");
		py::class_<IntsTag, ListTag, ContainerTag, Tag, std::shared_ptr<IntsTag>>(m, "IntsTag");
		py::class_<LongsTag, ListTag, ContainerTag, Tag, std::shared_ptr<LongsTag>>(m, "LongsTag");
	}

	// Parsing entry point
	{
		using namespace NBTP;
		m.def("parseRoot", &pyNBTP::pyParseRoot , "Parse Root Tag");
	}

	m.def("mksimple", &pyNBTP::mksimple, "A function which adds two numbers");
}