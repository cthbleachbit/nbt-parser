//
// Created by cth451 on 2020/05/09.
//

#include "tags/ShortTag.h"
#include "constants.h"
#include "conv.h"
#include "TagIO.h"
#include "Logging.h"
#include <boost/format.hpp>
#include <istream>

namespace NBTP {
	ShortTag::EndianConv ShortTag::toH = Conversion::conv_16_beh;
	ShortTag::EndianConv ShortTag::toJ = Conversion::conv_16_hbe;

	TagType ShortTag::typeCode() noexcept {
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
		ostream << boost::format("(%s) %d") % TypeNames[this->typeCode()] % (int64_t) this->payload;
		return ostream << std::endl;
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