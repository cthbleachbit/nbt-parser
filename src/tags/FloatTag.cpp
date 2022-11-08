//
// Created by cth451 on 2020/05/10.
//

#include <istream>

#include "tags/FloatTag.h"
#include "Conversion.h"

namespace NBTP {
	FloatTag::EndianConv FloatTag::toH = Conversion::conv_f_beh;
	FloatTag::EndianConv FloatTag::toJ = Conversion::conv_f_hbe;

	std::ostream &FloatTag::nbtOutput(std::ostream &ostream) const {
		// Perform host to java big endian conversion
		float big = toJ(this->payload);
		ostream.write(reinterpret_cast<const char *>(&big), sizeof(float));
		return ostream;
	}

	FloatTag::FloatTag(std::istream &input, ssize_t &counter, IOFormat format) {
		switch (format) {
			case BIN:
				V buffer;
				input.read(reinterpret_cast<char *>(&buffer), sizeof(V));
				input.exceptions(std::istream::failbit);
				// Perform java big-endian to host conversion
				buffer = toH(buffer);
				this->payload = buffer;
				counter += sizeof(V);
				break;
			case PRETTY_PRINT:
				throw std::invalid_argument(PARSE_PRETTY);
		}
	}
}