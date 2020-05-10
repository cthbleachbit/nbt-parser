//
// Created by cth451 on 5/10/20.
//

#include <boost/format.hpp>
#include <istream>
#include "tags/StringTag.h"
#include "tags/ShortTag.h"

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

	std::ostream &StringTag::output(std::ostream &ostream, IOFormat format) {
		switch (format) {
			case TEXT:
				this->textOutput(ostream, 0);
				break;
			case BIN:
				// TODO
				break;
		}
		return ostream;
	}

	std::ostream &StringTag::textOutput(std::ostream &ostream, unsigned int indent) {
		std::string print = this->payload.length() ? this->payload : "(empty string)";
		ostream << boost::format("(%s) %s") % TypeNames[this->typeCode()] % print;
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

	StringTag::StringTag(std::istream &input) {
		this->payload = parseString(input);
	}

	std::string StringTag::parseString(std::istream &input) {
		int16_t length = ShortTag::parseShort(input);
		char *copy_buf = new char[length + 1];
		input.read(copy_buf, length);
		copy_buf[length] = 0x00;
		if (input.fail()) {
			delete[] copy_buf;
			throw std::ios_base::failure(IO_UNEXPECTED_EOF);
		}
		std::string ret = std::string(copy_buf);
		delete[] copy_buf;
		return ret;
	}


	StringTag::StringTag() = default;
}