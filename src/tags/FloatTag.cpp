//
// Created by cth451 on 2020/05/10.
//

#include <istream>

#include "tags/FloatTag.h"
#include "Logging.h"
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
				// Perform java big-endian to host conversion
				buffer = toH(buffer);
				if (input.fail()) {
					Logging::error(fmt::format(IO_UNEXPECTED_EOF, sizeof(V)), counter);
				}
				this->payload = buffer;
				counter += sizeof(V);
				break;
			case PRETTY_PRINT:
				Logging::error(PARSE_PRETTY, counter);
		}
	}
}