//
// Created by cth451 on 2020/05/10.
//

#include "tags/FloatTag.h"
#include "Logging.h"
#include <Conversion.h>
#include <istream>
#include <climits>

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
				float buffer;
				input.read(reinterpret_cast<char *>(&buffer), sizeof(float));
				// Perform java big-endian to host conversion
				buffer = toH(buffer);
				if (input.fail()) {
					Logging::error(IO_UNEXPECTED_EOF, counter);
				}
				this->payload = buffer;
				counter += sizeof(float);
				break;
			case PRETTY_PRINT:
				Logging::error(PARSE_PRETTY, counter);
		}
	}
}