//
// Created by cth451 on 2020/05/09.
//

#include <istream>

#include "tags/IntTag.h"
#include "Conversion.h"
#include "constants.h"
#include "Logging.h"

namespace NBTP {
	IntTag::EndianConv IntTag::toH = Conversion::conv_32_beh;
	IntTag::EndianConv IntTag::toJ = Conversion::conv_32_hbe;

	IntTag::V IntTag::parseInt(std::istream &input, ssize_t &counter) {
		V buffer;
		input.read(reinterpret_cast<char *>(&buffer), sizeof(V));
		if (input.fail()) {
			Logging::error(fmt::format(IO_UNEXPECTED_EOF, sizeof(V)), counter);
		}
		// Perform java big-endian to host conversion
		buffer = toH(buffer);
		counter += sizeof(V);
		return buffer;
	}

	std::ostream &IntTag::nbtOutput(std::ostream &ostream) const {
		// Perform host to java big endian conversion
		V big = toJ(this->payload);
		ostream.write(reinterpret_cast<const char *>(&big), sizeof(V));
		return ostream;
	}

	IntTag::IntTag(std::istream &input, ssize_t &counter, IOFormat format) {
		switch (format) {
			case BIN:
				this->payload = parseInt(input, counter);
				break;
			case PRETTY_PRINT:
				Logging::error(PARSE_PRETTY, counter);
				break;
		}
	}
}