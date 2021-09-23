//
// Created by cth451 on 2020/05/09.
//

#include "tags/LongTag.h"
#include "Conversion.h"
#include "Logging.h"
#include <istream>

namespace NBTP {
	LongTag::EndianConv LongTag::toH = Conversion::conv_64_beh;
	LongTag::EndianConv LongTag::toJ = Conversion::conv_64_hbe;

	LongTag::LongTag(std::istream &input, ssize_t &counter, IOFormat format) {
		switch (format) {
			case BIN:
				this->payload = parseLong(input, counter);
				break;
			case PRETTY_PRINT:
				Logging::error(PARSE_PRETTY, counter);
				break;
		}
	}

	LongTag::V LongTag::parseLong(std::istream &input, ssize_t &counter) {
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

	std::ostream &LongTag::nbtOutput(std::ostream &ostream) const {
		// Perform host to java big endian conversion
		V big = toJ(this->payload);
		ostream.write(reinterpret_cast<const char *>(&big), sizeof(V));
		return ostream;
	}
}