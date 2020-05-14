//
// Created by cth451 on 2020/05/10.
//

#include "tags/FloatTag.h"
#include "TagIO.h"
#include "Logging.h"
#include <Conversion.h>
#include <boost/format.hpp>
#include <istream>

namespace NBTP {
	FloatTag::EndianConv FloatTag::toH = Conversion::conv_f_beh;
	FloatTag::EndianConv FloatTag::toJ = Conversion::conv_f_hbe;

	TagType FloatTag::typeCode() const noexcept {
		return TagType::FLOAT;
	}

	std::ostream &FloatTag::output(std::ostream &ostream, IOFormat format) const {
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

	std::ostream &FloatTag::textOutput(std::ostream &ostream, unsigned int indent) const {
		ostream << boost::format("(%s) %f") % TypeNames[this->typeCode()] % (float) this->payload;
		return ostream << std::endl;
	}

	std::ostream &FloatTag::nbtOutput(std::ostream &ostream, float value) {
		// Perform host to java big endian conversion
		float big = toJ(value);
		ostream.write(reinterpret_cast<const char *>(&big), sizeof(float));
		return ostream;
	}

	FloatTag::FloatTag(std::istream &input, ssize_t &counter, IOFormat format) {
		switch (format) {
			case BIN:
				float buffer;
				input.read(reinterpret_cast<char *>(&buffer), sizeof(float));
				// Perform java big-endian to host conversion
				buffer = toH(buffer);
				if (input.fail()) {
					Logging::error(IO_UNEXPECTED_EOF, counter);
				}
				this->payload = buffer;
				counter += sizeof(float);
				break;
			case PRETTY_PRINT:
				Logging::error(PARSE_PRETTY, counter);
		}
	}
}