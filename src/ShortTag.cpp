//
// Created by cth451 on 2020/05/09.
//

#include "ShortTag.h"
#include "constants.h"
#include <boost/format.hpp>
#include <istream>

namespace NBTP {
	int8_t ShortTag::typeCode() noexcept {
		return TagType::SHORT;
	}

	std::ostream &ShortTag::output(std::ostream &ostream, IOFormat format) {
		switch (format) {
			case TEXT:
				textOutput(ostream, 0);
				break;
			case BIN:
				ostream.write(reinterpret_cast<const char *>(&this->payload), sizeof(V));
		}
		return ostream;
	}

	std::ostream &ShortTag::textOutput(std::ostream &ostream, unsigned int indent) {
		ostream << boost::format("(Short) %d") % (int64_t) this->payload;
		return ostream;
	}

	ShortTag::ShortTag(V value) {
		this->payload = value;
	}

	ShortTag::ShortTag(std::istream &input) {
		V buffer;
		input.read(reinterpret_cast<char *>(&buffer), sizeof(V));
		if (input.fail()) {
			throw std::ios_base::failure(IO_UNEXPECTED_EOF);
		}
		this->payload = buffer;
	}
}