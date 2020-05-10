//
// Created by cth451 on 2020/05/09.
//

#include "LongTag.h"
#include <boost/format.hpp>
#include <istream>

namespace NBTP {
	TagType LongTag::typeCode() noexcept {
		return TagType::LONG;
	}

	std::ostream &LongTag::output(std::ostream &ostream, IOFormat format) {
		switch (format) {
			case TEXT:
				textOutput(ostream, 0);
				break;
			case BIN:
				// Perform host to java big endian conversion
				V big = htobe64(this->payload);
				ostream.write(reinterpret_cast<const char *>(&big), sizeof(V));
		}
		return ostream;
	}

	std::ostream &LongTag::textOutput(std::ostream &ostream, unsigned int indent) {
		ostream << boost::format("(%s) %d") % TypeNames[this->typeCode()] % (int64_t) this->payload;
		return ostream;
	}

	LongTag::LongTag(V value) {
		this->payload = value;
	}

	LongTag::LongTag(std::istream &input) {
		V buffer;
		input.read(reinterpret_cast<char *>(&buffer), sizeof(V));
		// Perform java big-endian to host conversion
		buffer = be64toh(buffer);
		if (input.fail()) {
			throw std::ios_base::failure(IO_UNEXPECTED_EOF);
		}
		this->payload = buffer;
	}
}