//
// Created by cth451 on 2020/05/09.
//

#include "tags/ByteTag.h"
#include "constants.h"
#include <boost/format.hpp>
#include <istream>
#include "TagIO.h"
#include "Logging.h"

namespace NBTP {
	TagType ByteTag::typeCode() noexcept {
		return TagType::BYTE;
	}

	std::ostream &ByteTag::output(std::ostream &ostream, IOFormat format) {
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

	std::ostream &ByteTag::textOutput(std::ostream &ostream, unsigned int indent) {
		ostream << boost::format("(%s) %d") % TypeNames[this->typeCode()] % (int64_t) this->payload;
		return ostream << std::endl;
	}

	ByteTag::ByteTag(V value) {
		this->payload = value;
	}

	std::ostream &ByteTag::nbtOutput(std::ostream &ostream, ByteTag::V value) {
		ostream.write(reinterpret_cast<const char *>(&value), sizeof(V));
		return ostream;
	}

	ByteTag::ByteTag(std::istream &input, ssize_t &counter, IOFormat format) {
		switch (format) {
			case BIN:
				V buffer;
				input.read(reinterpret_cast<char *>(&buffer), sizeof(V));
				if (input.fail()) {
					Logging::error(IO_UNEXPECTED_EOF, counter);
				}
				this->payload = buffer;
				counter += sizeof(V);
				break;
			case PRETTY_PRINT:
				Logging::error(PARSE_PRETTY, counter);
				break;
		}
	}
}