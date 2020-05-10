//
// Created by cth451 on 2020/05/09.
//

#include "tags/ByteTag.h"
#include "constants.h"
#include <boost/format.hpp>
#include <istream>

namespace NBTP {
	TagType ByteTag::typeCode() noexcept {
		return TagType::BYTE;
	}

	std::ostream &ByteTag::output(std::ostream &ostream, IOFormat format) {
		switch (format) {
			case TEXT:
				textOutput(ostream, 0);
				break;
			case BIN:
				ostream.write(reinterpret_cast<const char *>(&this->payload), sizeof(V));
		}
		return ostream;
	}

	std::ostream &ByteTag::textOutput(std::ostream &ostream, unsigned int indent) {
		ostream << boost::format("(%s) %d") % TypeNames[this->typeCode()] % (int64_t) this->payload;
		return ostream << std::endl;
	}

	ByteTag::ByteTag(V value) {
		this->payload = value;
	}

	ByteTag::ByteTag(std::istream &input) {
		V buffer;
		input.read(reinterpret_cast<char *>(&buffer), sizeof(V));
		if (input.fail()) {
			throw std::ios_base::failure(IO_UNEXPECTED_EOF);
		}
		this->payload = buffer;
	}

	bool ByteTag::equal(Tag &rhs) {
		if (rhs.typeCode() != TagType::BYTE) {
			return false;
		}
		return this->payload == ((ByteTag &) rhs).payload;
	}

	ByteTag::V ByteTag::getPayload() const {
		return this->payload;
	}
}