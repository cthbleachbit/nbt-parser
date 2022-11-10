//
// Created by cth451 on 2020/05/10.
//

#include <iostream>
#include <fmt/format.h>
#include "TagIO.h"
#include "tags/StringTag.h"
#include "tags/ShortTag.h"

namespace NBTP {

	void static _warn(const char *msg, ssize_t counter, std::ostream *logging) {
		if (logging)
			*logging << fmt::format(WARNING_PREFIX, counter, msg) << std::endl;
	}

	std::shared_ptr<Tag> TagIO::parseRoot(std::istream &input, ssize_t &counter, std::ostream *logging) {
		counter = 0;
		TagType typeCode = readType(input, counter);
		if (typeCode != COMPOUND) {
			_warn(ROOT_NOT_COMPOUND, counter, logging);
		}
		std::string name = StringTag::parseString(input, counter);
		if (name.length() != 0) {
			_warn(ROOT_HAS_NAME, counter, logging);
		}
		return Tag::parseTag(input, typeCode, counter);
	}

	void TagIO::writeRoot(std::ostream &ostream, Tag &tag, std::ostream *logging) {
		if (tag.typeCode() != COMPOUND) {
			_warn(ROOT_NOT_COMPOUND, 0, logging);
		}
		char typeByte = static_cast<char>(tag.typeCode());
		ostream.write(&typeByte, 1);
		ShortTag tmp(0);
		tmp.nbtOutput(ostream);
		tag.output(ostream, BIN);
	}

	std::shared_ptr<Tag> TagIO::parseRoot(
			std::istream &input,
			ssize_t &counter,
			IOFormat format,
			std::ostream *logging) {
		switch (format) {
			case BIN:
				return parseRoot(input, counter, logging);
			case PRETTY_PRINT:
				throw std::invalid_argument(PARSE_PRETTY);
			default:
				throw std::invalid_argument(PARSE_UNKNOWN_FMT);
		}
	}

	TagParseException::TagParseException(ssize_t offset, std::string reason) noexcept
			: runtime_error(reason), offset(offset) {
		message = fmt::format(ERROR_PREFIX, offset, reason);
	}

	const char *TagParseException::what() const noexcept {
		return message.c_str();
	}

	ListTypeMismatchException::ListTypeMismatchException(TagType expected, TagType got) noexcept
			: std::runtime_error("List Type Unmatch"), expected(expected), got(got) {
		message = fmt::format(LIST_ADD_UNMATCH, TypeNames[got], TypeNames[expected]);
	}

	const char *ListTypeMismatchException::what() const noexcept {
		return message.c_str();
	}
}

