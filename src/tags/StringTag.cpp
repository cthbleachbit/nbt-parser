//
// Created by cth451 on 5/10/20.
//

#include <boost/format.hpp>
#include <istream>
#include "tags/StringTag.h"
#include "tags/ShortTag.h"
#include "conv.h"
#include "TagIO.h"
#include "Logging.h"

namespace NBTP {
	ssize_t StringTag::size() {
		return this->payload.length();
	}

	TagType StringTag::typeCode() noexcept {
		return TagType::STRING;
	}

	bool StringTag::equal(Tag &rhs) {
		if (rhs.typeCode() != TagType::STRING) {
			return false;
		}
		return this->payload == ((StringTag &) rhs).payload;
	}

	std::ostream &StringTag::output(std::ostream &ostream, IOFormat format) const {
		switch (format) {
			case PRETTY_PRINT:
				this->textOutput(ostream, 0);
				break;
			case BIN:
				nbtOutput(ostream, this->payload);
				break;
		}
		return ostream;
	}

	std::ostream &StringTag::textOutput(std::ostream &ostream, unsigned int indent) const {
		std::string print = this->payload.length() ? this->payload : "(empty string)";
		ostream << boost::format("(%s) %s") % TypeNames[this->typeCode()] % print;
		return ostream << std::endl;
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
			Logging::error(CONTENT_LEN_NEG, counter);
		}
		char *copy_buf = new char[length + 1];
		input.read(copy_buf, length);
		copy_buf[length] = 0x00;
		if (input.fail()) {
			delete[] copy_buf;
			Logging::error(IO_UNEXPECTED_EOF, counter);
		}
		std::string ret = std::string(copy_buf);
		delete[] copy_buf;
		counter += length;
		return ret;
	}

	std::ostream &StringTag::nbtOutput(std::ostream &ostream, const std::string &value) {
		// Length sanity checking
		if (value.length() > INT16_MAX) {
			throw std::runtime_error(STRING_TOO_LONG);
		}
		ShortTag::nbtOutput(ostream, value.length());
		// Write string itself w/o null terminator
		ostream.write(value.c_str(), value.length());
		return ostream;
	}


	StringTag::StringTag() = default;
}