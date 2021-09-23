//
// Created by cth451 on 2020/05/09.
//

#include "tags/LongTag.h"
#include "Conversion.h"
#include "Logging.h"
#include <istream>
#include <climits>

namespace NBTP {
	LongTag::EndianConv LongTag::toH = Conversion::conv_64_beh;
	LongTag::EndianConv LongTag::toJ = Conversion::conv_64_hbe;

	std::ostream &LongTag::output(std::ostream &ostream, IOFormat format) const {
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

	std::ostream &LongTag::textOutput(std::ostream &ostream, unsigned int indent) const {
		char *message = new char[LINE_MAX];
#ifdef __WINDOWS__
		// Under windows uint64_t and int64_t is considered long long int.
		snprintf(message, LINE_MAX - 1, "(%s) %lld", TypeNames[this->typeCode()].c_str(), this->payload);
#else
		snprintf(message, LINE_MAX - 1, "(%s) %ld", TypeNames[this->typeCode()].c_str(), this->payload);
#endif
		ostream << message << std::endl;
		delete[] message;
		return ostream;
	}

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
			Logging::error(IO_UNEXPECTED_EOF, counter);
		}
		counter += sizeof(V);
		return buffer;
	}

	std::ostream &LongTag::nbtOutput(std::ostream &ostream, LongTag::V value) {
		// Perform host to java big endian conversion
		V big = toJ(value);
		ostream.write(reinterpret_cast<const char *>(&big), sizeof(V));
		return ostream;
	}
}