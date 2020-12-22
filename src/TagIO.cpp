//
// Created by cth451 on 2020/05/10.
//

#include <iostream>
#include <utility>
#include <climits>
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

	TagParseException::TagParseException(ssize_t offset, std::string reason) noexcept
		: runtime_error(reason), offset(offset), reason(std::move(reason)) {
		message = static_cast<char *>(malloc(LINE_MAX));
	}

	const char *TagParseException::what() const noexcept {
		snprintf(message, LINE_MAX - 1, "Error at byte offset %li: %s", offset, reason.c_str());
		return this -> message;
	}

	TagParseException::~TagParseException() {
		free(message);
	}

	TagParseException::TagParseException(const TagParseException &arg) noexcept
			: std::runtime_error(arg.reason), offset(arg.offset), reason(arg.reason) {
		message = static_cast<char *>(malloc(LINE_MAX));
	}

	ListTypeUnmatchException::ListTypeUnmatchException(TagType expected, TagType got) noexcept
			:
			std::runtime_error("List Type Unmatch"), expected(expected), got(got) {
		message = static_cast<char *>(malloc(LINE_MAX));
	}

	const char *ListTypeUnmatchException::what() const noexcept {
		snprintf(message, LINE_MAX - 1, LIST_ADD_UNMATCH,
		         TypeNames[got].c_str(), TypeNames[expected].c_str());
		return message;
	}

	ListTypeUnmatchException::~ListTypeUnmatchException() {
		free(message);
	};
}

