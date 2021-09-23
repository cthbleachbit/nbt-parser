//
// Created by cth451 on 2020/05/10.
//

#include <iostream>
#include <fmt/format.h>
#include "TagIO.h"
#include "tags/StringTag.h"
#include "tags/ShortTag.h"
#include "Logging.h"

namespace NBTP {
	std::shared_ptr<Tag> TagIO::parseRoot(std::istream &input, ssize_t &counter) {
		counter = 0;
		TagType typeCode = readType(input, counter);
		if (typeCode != COMPOUND) {
			Logging::warn(ROOT_NOT_COMPOUND, counter);
		}
		std::string name = StringTag::parseString(input, counter);
		if (name.length() != 0) {
			Logging::warn(ROOT_HAS_NAME, counter);
		}
		return Tag::parseTag(input, typeCode, counter);
	}

	void TagIO::writeRoot(std::ostream &ostream, Tag &tag) {
		if (tag.typeCode() != COMPOUND) {
			Logging::warn(ROOT_NOT_COMPOUND, 0);
		}
		char typeByte = static_cast<char>(tag.typeCode());
		ostream.write(&typeByte, 1);
		ShortTag::nbtOutput(ostream, 0);
		tag.output(ostream, BIN);
	}

	std::shared_ptr<Tag> TagIO::parseRoot(std::istream &input, ssize_t &counter, IOFormat format) {
		switch (format) {
			case BIN:
				return parseRoot(input, counter);
			case PRETTY_PRINT:
				Logging::error(PARSE_PRETTY, counter);
				break;
			default:
				return std::shared_ptr<Tag>(nullptr);
		}
		return nullptr;
	}

	// TagParseException
	TagParseException::TagParseException(ssize_t offset, std::string reason) noexcept
		: runtime_error(reason), offset(offset) {
		message = fmt::format("Error at byte offset {}: {}", offset, reason);
	}

	const char *TagParseException::what() const noexcept {
		return message.c_str();
	}

	// ListTypeUnmatchException
	ListTypeMismatchException::ListTypeMismatchException(TagType expected, TagType got) noexcept
			:std::runtime_error("List Type Unmatch"), expected(expected), got(got) {
		message = fmt::format(LIST_ADD_UNMATCH, TypeNames[got], TypeNames[expected]);
	}

	const char *ListTypeMismatchException::what() const noexcept {
		return message.c_str();
	}
}

