//
// Created by cth451 on 2020/05/10.
//

#include "tags/DoubleTag.h"
#include "conv.h"
#include <boost/format.hpp>
#include <istream>
#include "TagIO.h"

namespace NBTP {
	DoubleTag::EndianConv DoubleTag::toH = Conversion::conv_d_beh;
	DoubleTag::EndianConv DoubleTag::toJ = Conversion::conv_d_hbe;

	TagType DoubleTag::typeCode() noexcept {
		return TagType::DOUBLE;
	}

	std::ostream &DoubleTag::output(std::ostream &ostream, IOFormat format) {
		switch (format) {
			case PRETTY_PRINT:
				textOutput(ostream, 0);
				break;
			case BIN:
				nbtOutput(ostream, this->payload);
				break;
		}
		return ostream;
	}

	std::ostream &DoubleTag::textOutput(std::ostream &ostream, unsigned int indent) {
		ostream << boost::format("(%s) %f") % TypeNames[this->typeCode()] % (double) this->payload;
		return ostream << std::endl;
	}

	DoubleTag::DoubleTag(V value) {
		this->payload = value;
	}

	DoubleTag::DoubleTag(std::istream &input, ssize_t &counter) {
		V buffer;
		input.read(reinterpret_cast<char *>(&buffer), sizeof(V));
		// Perform java big-endian to host conversion
		buffer = toH(buffer);
		if (input.fail()) {
			TagIO::error(IO_UNEXPECTED_EOF, counter);
		}
		this->payload = buffer;
		counter += sizeof(V);
	}

	bool DoubleTag::equal(Tag &rhs) {
		if (rhs.typeCode() != TagType::DOUBLE) {
			return false;
		}
		return this->payload == ((DoubleTag &) rhs).payload;
	}

	DoubleTag::V DoubleTag::getPayload() const {
		return this->payload;
	}

	std::ostream &DoubleTag::nbtOutput(std::ostream &ostream, DoubleTag::V value) {
		// Perform host to java big endian conversion
		V big = toJ(value);
		ostream.write(reinterpret_cast<const char *>(&big), sizeof(V));
		return ostream;
	}
}