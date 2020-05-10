#include "tags/Tag.h"
#include <istream>

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

	std::ostream &operator<<(std::ostream &ostream, Tag &tag) {
		return tag.textOutput(ostream, 0);
	}

	TagType readType(std::istream &input) {
		int8_t buf;
		input.read(reinterpret_cast<char *>(&buf), 1);
		return (TagType) buf;
	}

	bool Tag::equal(std::shared_ptr<Tag> &rhs) {
		return this->equal(*rhs);
	}

	bool Tag::operator==(Tag &rhs) {
		return this->equal(rhs);
	}

	bool Tag::operator!=(Tag &rhs) {
		return !(this->operator==(rhs));
	}
}
