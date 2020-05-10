//
// Created by cth451 on 2020/05/09.
//

#include "ByteTag.h"
#include "constants.h"
#include <boost/format.hpp>
#include <istream>

namespace NBTP {
	int8_t ByteTag::typeCode() noexcept {
		return TagType::BYTE;
	}

	std::ostream &ByteTag::output(std::ostream &ostream, IOFormat format) {
		switch (format) {
			case TEXT:
				textOutput(ostream, 0);
				break;
			case BIN:
				ostream.write(reinterpret_cast<const char *>(&this->payload), 1);
		}
		return ostream;
	}

	std::ostream &ByteTag::textOutput(std::ostream &ostream, unsigned int indent) {
		ostream << boost::format("(Byte) %d") % (int64_t) this->payload;
		return ostream;
	}

	ByteTag::ByteTag(int8_t value) {
		this->payload = value;
	}

	ByteTag::ByteTag(std::istream &input) {
		int8_t buffer;
		input.read(reinterpret_cast<char *>(&buffer), 1);
		if (input.fail()) {
			throw std::ios_base::failure(IO_UNEXPECTED_EOF);
		}
		this->payload = buffer;
	}
}