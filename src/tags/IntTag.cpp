//
// Created by cth451 on 2020/05/09.
//

#include "tags/IntTag.h"
#include "Conversion.h"
#include "constants.h"
#include "TagIO.h"
#include "Logging.h"
#include <boost/format.hpp>
#include <istream>

namespace NBTP {
	IntTag::EndianConv IntTag::toH = Conversion::conv_32_beh;
	IntTag::EndianConv IntTag::toJ = Conversion::conv_32_hbe;

	TagType IntTag::typeCode() const noexcept {
		return TagType::INT;
	}

	std::ostream &IntTag::output(std::ostream &ostream, IOFormat format) const {
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

	std::ostream &IntTag::textOutput(std::ostream &ostream, unsigned int indent) const {
		ostream << boost::format("(%s) %d") % TypeNames[this->typeCode()] % (int64_t) this->payload;
		return ostream << std::endl;
	}

	IntTag::V IntTag::parseInt(std::istream &input, ssize_t &counter) {
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

	std::ostream &IntTag::nbtOutput(std::ostream &ostream, IntTag::V value) {
		// Perform host to java big endian conversion
		V big = toJ(value);
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