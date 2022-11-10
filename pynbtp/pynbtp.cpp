/**
 * @file pynbtp bindings - main module definitions
 */

#include <pybind11/pybind11.h>
#include <pybind11/iostream.h>
#include <pybind11/stl.h>
#include <sstream>
#include "libnbtp.h"
#include "PyBytesBuf.h"

namespace py = pybind11;

namespace pyNBTP {
	using namespace NBTP;

	/**
	 * write tag with specified format
	 * @param tag     tag to write
	 * @param io      python output stream
	 * @param format  format specifier
	 */
	void pyOutputTag(Tag *tag, pybind11::object &io, IOFormat format) {
		if (format == PRETTY_PRINT) {
			pybind11::detail::pythonbuf buffer(io);
			std::ostream out(&buffer);
			tag->output(out, format);
		} else if (format == BIN) {
			PyOBytesBuf buffer(io);
			std::ostream out(&buffer);
			tag->output(out, format);
		}
	}

	/**
	 * Parse a tag that is read from a python binary buffer
	 * @param io   python binary input stream
	 * @return a Tag object or None if parsing fails
	 */
	std::shared_ptr<Tag> pyParseRoot(pybind11::object &io) {
		ssize_t counter;
		PyIBytesBuf buffer(io);
		std::istream in(&buffer);
		return TagIO::parseRoot(in, counter);
	}

	/**
	 * Parse a tag in specified format
	 * Only binary is supported at this time. Other formats are not implemented.
	 *
	 * @param io              python input stream
	 * @param format          format specifier
	 * @return a Tag object or None if parsing fails
	 */
	std::shared_ptr<Tag> pyParseRootFormat(pybind11::object &io, IOFormat format) {
		ssize_t counter;
		PyIBytesBuf buffer(io);
		std::istream in(&buffer);
		return TagIO::parseRoot(in, counter, format);
	}

	/**
	 * Write out an in-memory tag in binary format
	 * @param io      python binary output stream
	 * @param tag     tag to write
	 */
	void pyWriteRoot(pybind11::object &io, Tag &tag) {
		PyOBytesBuf buffer(io);
		std::ostream out(&buffer);
		tag.output(out, IOFormat::BIN);
	}
}

/**
 * pynbtp Module definition
 */
PYBIND11_MODULE(pynbtp, m) {
	m.doc() = "Python bindings for libnbtp";

	/* Enums */
	{
		using namespace NBTP;
		py::enum_<TagType>(m, "TagType", "Possible payload type for NBT tags")
				.value("BYTE", TagType::BYTE, "an 8-bit signed integer")
				.value("SHORT", TagType::SHORT, "a 16-bit signed integer")
				.value("INT", TagType::INT, "a 32-bit signed integer")
				.value("LONG", TagType::LONG, "a 64-bit signed integer")
				.value("FLOAT", TagType::FLOAT, "a 32-bit IEEE-754 floating point decimal")
				.value("DOUBLE", TagType::DOUBLE, "a 64-bit IEEE-754 floating point decimal")
				.value("LIST", TagType::LIST, "a list of tags of the same type")
				.value("COMPOUND", TagType::COMPOUND, "a set of named tags")
				.value("STRING", TagType::STRING, "a string literal")
				.value("BYTES", TagType::BYTES, "a list of 8-bit signed integers")
				.value("INTS", TagType::INTS, "a list of 32-bit signed integers")
				.value("LONGS", TagType::LONGS, "a list of 64-bit signed integers")
				.value("END", TagType::END, "an empty tag - used only for type designation for empty lists");
		py::enum_<IOFormat>(m, "IOFormat", "Supported I/O format when handling tags")
				.value("BIN", IOFormat::BIN, "Uncompressed NBT in binary")
				.value("PRETTY_PRINT", IOFormat::PRETTY_PRINT, "Human readable text - as tag output only");
	}

	/* Tag Types */
	{
		using namespace NBTP;
		using namespace std;
		/* Purely virtual Classes */
		py::class_<Tag, std::shared_ptr<Tag>>(m, "Tag", "Abstract class encompassing all tag types")
				.def("__copy__", [](const shared_ptr<Tag> &self) {
					return Tag::deepCopy(self);
				})
				.def("output", pyNBTP::pyOutputTag, "Serialize the tag and write to external file")
				.def("typeCode", &Tag::typeCode, "Retrieve type code for this tag")
				.def("__repr__", &Tag::toString);
		py::class_<SingleValuedTag<int8_t>, Tag, shared_ptr<SingleValuedTag<int8_t>>>(m, "__SingleValuedInt8Tag");
		py::class_<SingleValuedTag<int16_t>, Tag, shared_ptr<SingleValuedTag<int16_t>>>(m, "__SingleValuedInt16Tag");
		py::class_<SingleValuedTag<int32_t>, Tag, shared_ptr<SingleValuedTag<int32_t>>>(m, "__SingleValuedInt32Tag");
		py::class_<SingleValuedTag<int64_t>, Tag, shared_ptr<SingleValuedTag<int64_t>>>(m, "__SingleValuedInt64Tag");
		py::class_<SingleValuedTag<float>, Tag, shared_ptr<SingleValuedTag<float>>>(m, "__SingleValuedFloatTag");
		py::class_<SingleValuedTag<double>, Tag, shared_ptr<SingleValuedTag<double>>>(m, "__SingleValuedDoubleTag");
		py::class_<ContainerTag, Tag, shared_ptr<ContainerTag>>
				(
						m,
						"ContainerTag",
						"Abstract class encompassing tags that may contain other tags"
				)
				.def(
						"size",
						&ContainerTag::size,
						"Number of elements in the container tag"
				);

		/* Base classes */
		py::class_<ListTag, ContainerTag, Tag, shared_ptr<ListTag>>
				(
						m,
						"ListTag",
						"A container tag that holds multiple tags of the same type"
				)
				.def(py::init<>())
				.def(py::init<TagType>())
				.def(
						"insert",
						&ListTag::insert,
						"Insert a tag into this list. The tag inserted must match content type for this list."
				)
				.def(
						"remove",
						&ListTag::remove,
						"Remove tag by index and return the tag removed, or return null if the index doesn't exist."
				)
				.def(
						"setContentType",
						&ListTag::setContentType,
						"Set content type for an empty list. Will throw if the list is not empty."
				)
				.def(
						"getContentType",
						&ListTag::getContentType,
						"Get content type for a list. If the list is empty, return value is undefined."
				)
				.def(
						"__getitem__",
						[](ListTag *tag, ssize_t index) {
							return tag->getPayload().at(index);
						}
				)
				.def(
						"__setitem__",
						[](ListTag *tag, ssize_t index, std::shared_ptr<Tag> newValue) {
							tag->getPayload()[index] = newValue;
						}
				);
		py::class_<CompoundTag, ContainerTag, Tag, shared_ptr<CompoundTag>>
				(
						m,
						"CompoundTag",
						"A container tag that holds a set of named tags (potentially of different types)"
				)
				.def(py::init<>())
				.def(
						"insert",
						&CompoundTag::insert,
						"Insert a name-tag pair into this compound. "
						"Existing tag will under the same key will be erased."
				)
				.def(
						"lookup",
						&CompoundTag::lookup,
						"Retrieve the tag under the key or return None if the key doesn't exist"
				)
				.def(
						"remove",
						&CompoundTag::remove,
						"Remove tag corresponding to key if it exists"
				);
		py::class_<StringTag, ContainerTag, Tag, shared_ptr<StringTag>>
				(
						m,
						"StringTag",
						"A Tag that represents a string"
				)
				.def(py::init<>())
				.def(py::init<std::string &>())
				.def("setPayload", &StringTag::setPayload)
				.def("getPayload", &StringTag::getPayload);

		/* Single Valued Tags */
		py::class_<ByteTag, SingleValuedTag<int8_t>, Tag, shared_ptr<ByteTag>>
				(
						m,
						"ByteTag",
						"A tag that represents a signed 8-bit integer"
				)
				.def(py::init<int8_t>())
				.def("setPayload", &ByteTag::setPayload)
				.def("getPayload", &ByteTag::getPayload);
		py::class_<ShortTag, SingleValuedTag<int16_t>, Tag, shared_ptr<ShortTag>>
				(
						m,
						"ShortTag",
						"A tag that represents a signed 16-bit integer"
				)
				.def(py::init<int16_t>())
				.def("setPayload", &ShortTag::setPayload)
				.def("getPayload", &ShortTag::getPayload);
		py::class_<IntTag, SingleValuedTag<int32_t>, Tag, shared_ptr<IntTag>>
				(
						m,
						"IntTag",
						"A tag that represents a signed 32-bit integer"
				)
				.def(py::init<int32_t>())
				.def("setPayload", &IntTag::setPayload)
				.def("getPayload", &IntTag::getPayload);
		py::class_<LongTag, SingleValuedTag<int64_t>, Tag, shared_ptr<LongTag>>
				(
						m,
						"LongTag",
						"A tag that represents a signed 64-bit integer"
				)
				.def(py::init<int64_t>())
				.def("setPayload", &LongTag::setPayload)
				.def("getPayload", &LongTag::getPayload);
		py::class_<FloatTag, SingleValuedTag<float>, Tag, shared_ptr<FloatTag>>
				(
						m,
						"FloatTag",
						"A tag that represents a 32-bit floating point decimal in IEEE-754 format"
				)
				.def(py::init<float>())
				.def("setPayload", &FloatTag::setPayload)
				.def("getPayload", &FloatTag::getPayload);
		py::class_<DoubleTag, SingleValuedTag<double>, Tag, shared_ptr<DoubleTag>>
				(
						m,
						"DoubleTag",
						"A tag that represents a 64-bit floating point decimal in IEEE-754 format"
				)
				.def(py::init<double>())
				.def("setPayload", &DoubleTag::setPayload)
				.def("getPayload", &DoubleTag::getPayload);

		/* Types that extends ListTag */
		py::class_<BytesTag, ListTag, ContainerTag, Tag, shared_ptr<BytesTag>>
				(
						m,
						"BytesTag",
						"A container tag that holds multiple signed 8-bit integer tags"
				);
		py::class_<IntsTag, ListTag, ContainerTag, Tag, shared_ptr<IntsTag>>
				(
						m,
						"IntsTag",
						"A container tag that holds multiple signed 32-bit integer tags"
				);
		py::class_<LongsTag, ListTag, ContainerTag, Tag, shared_ptr<LongsTag>>
				(
						m,
						"LongsTag",
						"A container tag that holds multiple signed 64-bit integer tags"
				);
	}

	/* Exception Types */
	{
		using namespace NBTP;
		py::register_local_exception<TagParseException>(m, "TagParseError", PyExc_RuntimeError);
		py::register_local_exception<ListTypeMismatchException>(m, "ListTypeMismatchError", PyExc_RuntimeError);
		py::register_local_exception_translator([](std::exception_ptr p) {
			try {
				if (p) std::rethrow_exception(p);
			} catch (const std::ios_base::failure &e) {
				auto err = e.code().value();
				PyErr_SetObject(PyExc_IOError, py::make_tuple(err, e.what()).ptr());
			}
		});
	}

	/* Parsing / binary output entry point */
	{
		m.def("parseRoot", &pyNBTP::pyParseRoot, "Parse a root tag from a binary input stream");
		// m.def("parseRoot", &pyNBTP::pyParseRootFormat, "Parse Root Tag");
		m.def("writeRoot", &pyNBTP::pyWriteRoot, "Write a root tag to a binary output stream");
	}
}