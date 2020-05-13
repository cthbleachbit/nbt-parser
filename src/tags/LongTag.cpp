//
// Created by cth451 on 2020/05/09.
//

#include "tags/LongTag.h"
#include "conv.h"
#include "TagIO.h"
#include "Logging.h"
#include <boost/format.hpp>
#include <istream>

namespace NBTP {
	LongTag::EndianConv LongTag::toH = Conversion::conv_64_beh;
	LongTag::EndianConv LongTag::toJ = Conversion::conv_64_hbe;

	TagType LongTag::typeCode() noexcept {
		return TagType::LONG;
	}

	std::ostream &LongTag::output(std::ostream &ostream, IOFormat format) {
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

	std::ostream &LongTag::textOutput(std::ostream &ostream, unsigned int indent) {
		ostream << boost::format("(%s) %d") % TypeNames[this->typeCode()] % (int64_t) this->payload;
		return ostream << std::endl;
	}

	LongTag::LongTag(V value) {
		this->payload = value;
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

	bool LongTag::equal(Tag &rhs) {
		if (rhs.typeCode() != TagType::LONG) {
			return false;
		}
		return this->payload == ((LongTag &) rhs).payload;
	}

	LongTag::V LongTag::getPayload() const {
		return this->payload;
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

	void LongTag::setPayload(LongTag::V value) {
		this->payload = value;
	}
}