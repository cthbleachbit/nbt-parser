#include "tags/Tag.h"

namespace NBTP {

	std::string TypeNames[] = {
			"End of Tag",
			"Byte",
			"Short",
			"Int",
			"Long",
			"Float",
			"Double",
			"Bytes",
			"String",
			"List",
			"Compound",
			"Ints",
			"Longs"
	};

	std::ostream &Tag::textOutput(std::ostream &ostream, unsigned int indent) {
		return ostream;
	}

	TagType Tag::typeCode() noexcept {
		return TagType::INVALID;
	}

	std::ostream &Tag::indent(std::ostream &ostream, unsigned int n) {
		for (unsigned int i = 0; i < n; i++) {
			ostream << std::string("    ");
		}
		return ostream;
	}

}