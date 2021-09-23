//
// Created by cth451 on 2020/05/09.
//

#include "tags/ShortTag.h"
#include "constants.h"
#include "Conversion.h"
#include "Logging.h"
#include <istream>

namespace NBTP {
	ShortTag::EndianConv ShortTag::toH = Conversion::conv_16_beh;
	ShortTag::EndianConv ShortTag::toJ = Conversion::conv_16_hbe;

	ShortTag::ShortTag(std::istream &input, ssize_t &counter, IOFormat format) {
		switch (format) {
			case BIN:
				this->payload = parseShort(input, counter);
				break;
			case PRETTY_PRINT:
				Logging::error(PARSE_PRETTY, counter);
				break;
		}
	}

	ShortTag::V ShortTag::parseShort(std::istream &input, ssize_t &counter) {
		V buffer;
		input.read(reinterpret_cast<char *>(&buffer), sizeof(V));
		// Perform java big-endian to host conversion
		buffer = toH(buffer);
		if (input.fail()) {
			Logging::error(fmt::format(IO_UNEXPECTED_EOF, sizeof(V)), counter);
		}
		counter += sizeof(V);
		return buffer;
	}

	std::ostream &ShortTag::nbtOutput(std::ostream &ostream) const {
		// Perform host to java big endian conversion
		V big = toJ(this->payload);
		ostream.write(reinterpret_cast<const char *>(&big), sizeof(V));
		return ostream;
	}
}