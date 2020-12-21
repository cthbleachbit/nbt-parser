//
// Created by cth451 on 2020/05/09.
//

#include "tags/ByteTag.h"
#include "constants.h"
#include <istream>
#include <climits>
#include "Logging.h"

namespace NBTP {
	TagType ByteTag::typeCode() const noexcept {
		return TagType::BYTE;
	}

	std::ostream &ByteTag::output(std::ostream &ostream, IOFormat format) const {
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

	std::ostream &ByteTag::textOutput(std::ostream &ostream, unsigned int indent) const {
		char *message = new char[LINE_MAX];
		snprintf(message, LINE_MAX - 1, "(%s) %d", TypeNames[this->typeCode()].c_str(), this->payload);
		ostream << message << std::endl;
		delete[] message;
		return ostream;
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