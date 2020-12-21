//
// Created by cth451 on 2020/05/09.
//

#include "tags/ShortTag.h"
#include "constants.h"
#include "Conversion.h"
#include "Logging.h"
#include <istream>
#include <climits>

namespace NBTP {
	ShortTag::EndianConv ShortTag::toH = Conversion::conv_16_beh;
	ShortTag::EndianConv ShortTag::toJ = Conversion::conv_16_hbe;

	TagType ShortTag::typeCode() const noexcept {
		return TagType::SHORT;
	}

	std::ostream &ShortTag::output(std::ostream &ostream, IOFormat format) const {
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

	std::ostream &ShortTag::textOutput(std::ostream &ostream, unsigned int indent) const {
		char *message = new char[LINE_MAX];
		snprintf(message, LINE_MAX - 1, "(%s) %d", TypeNames[this->typeCode()].c_str(), this->payload);
		ostream << message << std::endl;
		delete[] message;
		return ostream;
	}

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
			Logging::error(IO_UNEXPECTED_EOF, counter);
		}
		counter += sizeof(V);
		return buffer;
	}

	std::ostream &ShortTag::nbtOutput(std::ostream &ostream, ShortTag::V value) {
		// Perform host to java big endian conversion
		V big = toJ(value);
		ostream.write(reinterpret_cast<const char *>(&big), sizeof(V));
		return ostream;
	}
}