//
// Created by cth451 on 2020/05/09.
//

#include "tags/ShortTag.h"
#include "constants.h"
#include "conv.h"
#include <boost/format.hpp>
#include <istream>

namespace NBTP {
	ShortTag::EndianConv ShortTag::toH = Conversion::conv_16_beh;
	ShortTag::EndianConv ShortTag::toJ = Conversion::conv_16_hbe;

	TagType ShortTag::typeCode() noexcept {
		return TagType::SHORT;
	}

	std::ostream &ShortTag::output(std::ostream &ostream, IOFormat format) {
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

	std::ostream &ShortTag::textOutput(std::ostream &ostream, unsigned int indent) {
		ostream << boost::format("(%s) %d") % TypeNames[this->typeCode()] % (int64_t) this->payload;
		return ostream;
	}

	ShortTag::ShortTag(V value) {
		this->payload = value;
	}

	ShortTag::ShortTag(std::istream &input) {
		V buffer;
		input.read(reinterpret_cast<char *>(&buffer), sizeof(V));
		// Perform java big-endian to host conversion
		buffer = toH(buffer);
		if (input.fail()) {
			throw std::ios_base::failure(IO_UNEXPECTED_EOF);
		}
		this->payload = buffer;
	}
}