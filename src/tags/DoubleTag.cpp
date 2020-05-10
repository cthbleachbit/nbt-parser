//
// Created by cth451 on 2020/05/10.
//

#include <tags/DoubleTag.h>
#include <conv.h>
#include <boost/format.hpp>
#include <istream>

namespace NBTP {
	DoubleTag::EndianConv DoubleTag::toH = Conversion::conv_d_beh;
	DoubleTag::EndianConv DoubleTag::toJ = Conversion::conv_d_hbe;

	TagType DoubleTag::typeCode() noexcept {
		return TagType::DOUBLE;
	}

	std::ostream &DoubleTag::output(std::ostream &ostream, IOFormat format) {
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

	std::ostream &DoubleTag::textOutput(std::ostream &ostream, unsigned int indent) {
		ostream << boost::format("(%s) %f") % TypeNames[this->typeCode()] % (double) this->payload;
		return ostream;
	}

	DoubleTag::DoubleTag(V value) {
		this->payload = value;
	}

	DoubleTag::DoubleTag(std::istream &input) {
		V buffer;
		input.read(reinterpret_cast<char *>(&buffer), sizeof(V));
		// Perform java big-endian to host conversion
		buffer = toH(buffer);
		if (input.fail()) {
			throw std::ios_base::failure(IO_UNEXPECTED_EOF);
		}
		this->payload = buffer;
	}

	bool DoubleTag::equal(Tag &rhs) {
		if (rhs.typeCode() != TagType::DOUBLE) {
			return false;
		}
		return this->payload == ((DoubleTag &) rhs).payload;
	}
}