//
// Created by cth451 on 2020/05/09.
//

#include "IntTag.h"
#include "constants.h"
#include <boost/format.hpp>
#include <istream>

namespace NBTP {
	int8_t IntTag::typeCode() noexcept {
		return TagType::INT;
	}

	std::ostream &IntTag::output(std::ostream &ostream, IOFormat format) {
		switch (format) {
			case TEXT:
				textOutput(ostream, 0);
				break;
			case BIN:
				ostream.write(reinterpret_cast<const char *>(&this->payload), 1);
		}
		return ostream;
	}

	std::ostream &IntTag::textOutput(std::ostream &ostream, unsigned int indent) {
		ostream << boost::format("(Int) %d") % (int64_t) this->payload;
		return ostream;
	}

	IntTag::IntTag(int32_t value) {
		this->payload = value;
	}

	IntTag::IntTag(std::istream &input) {
		int32_t buffer;
		input.read(reinterpret_cast<char *>(&buffer), 4);
		// Perform java big-endian to host conversion
		buffer = be32toh(buffer);
		if (input.fail()) {
			throw std::ios_base::failure(IO_UNEXPECTED_EOF);
		}
		this->payload = buffer;
	}
}