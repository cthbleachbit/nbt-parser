//
// Created by cth451 on 2020/05/09.
//

#include <istream>
#include "tags/ByteTag.h"
#include "constants.h"
#include "Logging.h"

namespace NBTP {

	std::ostream &ByteTag::nbtOutput(std::ostream &ostream) const {
		ostream.write(reinterpret_cast<const char *>(&this->payload), sizeof(V));
		return ostream;
	}

	ByteTag::ByteTag(std::istream &input, ssize_t &counter, IOFormat format) {
		switch (format) {
			case BIN:
				V buffer;
				input.read(reinterpret_cast<char *>(&buffer), sizeof(V));
				if (input.fail()) {
					Logging::error(fmt::format(IO_UNEXPECTED_EOF, sizeof(V)), counter);
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