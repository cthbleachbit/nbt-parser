#include "libnbtp.h"
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
		if (input.fail()) {
			throw std::ios_base::failure(IO_UNEXPECTED_EOF);
		}
		return static_cast<TagType>(buf);
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

	std::shared_ptr<Tag> Tag::parseTag(std::istream &input, TagType typeCode) {
		std::shared_ptr<Tag> ptr;
		switch (typeCode) {
			case BYTE:
				ptr = std::make_shared<ByteTag>(input);
				break;
			case SHORT:
				ptr = std::make_shared<ShortTag>(input);
				break;
			case INT:
				ptr = std::make_shared<IntTag>(input);
				break;
			case LONG:
				ptr = std::make_shared<LongTag>(input);
				break;
			case FLOAT:
				ptr = std::make_shared<FloatTag>(input);
				break;
			case DOUBLE:
				ptr = std::make_shared<DoubleTag>(input);
				break;
			case BYTES:
				ptr = std::make_shared<BytesTag>(input);
				break;
			case STRING:
				ptr = std::make_shared<StringTag>(input);
				break;
			case LIST:
				ptr = std::make_shared<ListTag>(input);
				break;
			case COMPOUND:
				ptr = std::make_shared<CompoundTag>(input);
				break;
			case INTS:
				ptr = std::make_shared<IntsTag>(input);
				break;
			case LONGS:
				ptr = std::make_shared<LongsTag>(input);
				break;
			default:
				throw std::runtime_error(INVALID_TYPE);
		}
		return ptr;
	}
}
