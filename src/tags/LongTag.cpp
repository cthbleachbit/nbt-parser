//
// Created by cth451 on 2020/05/09.
//

#include "tags/LongTag.h"
#include "conv.h"
#include <boost/format.hpp>
#include <istream>

namespace NBTP {
	LongTag::EndianConv LongTag::toH = Conversion::conv_64_beh;
	LongTag::EndianConv LongTag::toJ = Conversion::conv_64_hbe;

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
				V big = toJ(this->payload);
				ostream.write(reinterpret_cast<const char *>(&big), sizeof(V));
		}
		return ostream;
	}

	std::ostream &LongTag::textOutput(std::ostream &ostream, unsigned int indent) {
		ostream << boost::format("(%s) %d") % TypeNames[this->typeCode()] % (int64_t) this->payload;
		return ostream << std::endl;
	}

	LongTag::LongTag(V value) {
		this->payload = value;
	}

	LongTag::LongTag(std::istream &input) {
		V buffer;
		input.read(reinterpret_cast<char *>(&buffer), sizeof(V));
		// Perform java big-endian to host conversion
		buffer = toH(buffer);
		if (input.fail()) {
			throw std::ios_base::failure(IO_UNEXPECTED_EOF);
		}
		this->payload = buffer;
	}

	bool LongTag::equal(Tag &rhs) {
		if (rhs.typeCode() != TagType::LONG) {
			return false;
		}
		return this->payload == ((LongTag &) rhs).payload;
	}

	LongTag::V LongTag::getPayload() const {
		return this->payload;
	}
}