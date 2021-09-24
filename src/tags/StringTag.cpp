//
// Created by cth451 on 5/10/20.
//

#include <istream>
#include <climits>
#include "tags/StringTag.h"
#include "tags/ShortTag.h"
#include "Conversion.h"
#include "Logging.h"

namespace NBTP {
	ssize_t StringTag::size() const {
		return this->payload.length();
	}

	bool StringTag::equal(const Tag &rhs) const {
		if (rhs.typeCode() != TagType::STRING) {
			return false;
		}
		return this->payload == ((const StringTag &) rhs).payload;
	}

	std::ostream &StringTag::textOutput(std::ostream &ostream, unsigned int indent) const {
		const std::string &to_print = this->payload == "" ? "(Empty String)" : this->payload;
		ostream << fmt::format(REPR_SINGLE_VALUED, TypeNames[this->typeCode()], to_print) << std::endl;
		return ostream;
	}

	const std::string &StringTag::getPayload() const {
		return this->payload;
	}

	void StringTag::setPayload(std::string s) {
		this->payload = s;
	}

	StringTag::StringTag(std::string s) {
		this->payload = s;
	}

	StringTag::StringTag(std::istream &input, ssize_t &counter, IOFormat format) {
		switch (format) {
			case BIN:
				this->payload = parseString(input, counter);
				break;
			case PRETTY_PRINT:
				Logging::error(PARSE_PRETTY, counter);
				break;
		}
	}

	std::string StringTag::parseString(std::istream &input, ssize_t &counter) {
		int16_t length = ShortTag::parseShort(input, counter);
		if (length < 0) {
			Logging::error(fmt::format(CONTENT_LEN_NEG, length), counter);
		}
		char *copy_buf = new char[length + 1];
		input.read(copy_buf, length);
		copy_buf[length] = 0x00;
		if (input.fail()) {
			delete[] copy_buf;
			Logging::error(fmt::format(IO_UNEXPECTED_EOF, length), counter);
			return "";
		}
		std::string ret = std::string(copy_buf);
		delete[] copy_buf;
		counter += length;
		return ret;
	}

	std::ostream &StringTag::nbtOutput(std::ostream &ostream) const {
		// Length sanity checking
		if (this->payload.length() > INT16_MAX) {
			throw std::runtime_error(STRING_TOO_LONG);
		}
		ShortTag tmp(this->payload.length());
		tmp.nbtOutput(ostream);
		// Write string itself w/o null terminator
		ostream.write(this->payload.c_str(), this->payload.length());
		return ostream;
	}


	StringTag::StringTag() = default;
}