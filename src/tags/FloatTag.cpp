//
// Created by cth451 on 2020/05/10.
//

#include <tags/FloatTag.h>
#include <conv.h>
#include <boost/format.hpp>
#include <istream>

namespace NBTP {
	FloatTag::EndianConv FloatTag::toH = Conversion::conv_f_beh;
	FloatTag::EndianConv FloatTag::toJ = Conversion::conv_f_hbe;

	TagType FloatTag::typeCode() noexcept {
		return TagType::FLOAT;
	}

	std::ostream &FloatTag::output(std::ostream &ostream, IOFormat format) {
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

	std::ostream &FloatTag::textOutput(std::ostream &ostream, unsigned int indent) {
		ostream << boost::format("(%s) %f") % TypeNames[this->typeCode()] % (float) this->payload;
		return ostream << std::endl;
	}

	FloatTag::FloatTag(V value) {
		this->payload = value;
	}

	FloatTag::FloatTag(std::istream &input) {
		V buffer;
		input.read(reinterpret_cast<char *>(&buffer), sizeof(V));
		// Perform java big-endian to host conversion
		buffer = toH(buffer);
		if (input.fail()) {
			throw std::ios_base::failure(IO_UNEXPECTED_EOF);
		}
		this->payload = buffer;
	}

	bool FloatTag::equal(Tag &rhs) {
		if (rhs.typeCode() != TagType::FLOAT) {
			return false;
		}
		return this->payload == ((FloatTag &) rhs).payload;
	}

	FloatTag::V FloatTag::getPayload() const {
		return this->payload;
	}

	std::ostream &FloatTag::nbtOutput(std::ostream &ostream, FloatTag::V value) {
		// Perform host to java big endian conversion
		V big = toJ(value);
		ostream.write(reinterpret_cast<const char *>(&big), sizeof(V));
		return ostream;
	}
}