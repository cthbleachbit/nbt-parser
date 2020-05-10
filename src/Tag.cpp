#include "Tag.h"

namespace NBTP {

	std::ostream &Tag::textOutput(std::ostream &ostream, unsigned int indent) {
		return ostream;
	}

	int8_t Tag::typeCode() noexcept {
		return TagType::INVALID;
	}

	std::ostream &Tag::indent(std::ostream &ostream, unsigned int n) {
		for (unsigned int i = 0; i < n; i++) {
			ostream << std::string("    ");
		}
		return ostream;
	}

}
