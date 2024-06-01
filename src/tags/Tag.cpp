#include <istream>
#include <sstream>
#include <tags/Tag.h>
#include <array>
#include <type_traits>

#include "libnbtp.h"
#include "TagIO.h"

namespace NBTP {

	const std::array<const char *, TagType::END_OF_TAG_TYPE> TypeNames = {
			"End of Compound",
			"Byte",
			"Short",
			"Int",
			"Long",
			"Float",
			"Double",
			"Array of Bytes",
			"String",
			"List",
			"Compound",
			"Array of Ints",
			"Array of Longs"
	};
	static_assert(TypeNames.size() == TagType::END_OF_TAG_TYPE);

	std::ostream &Tag::indent(std::ostream &ostream, unsigned int n) {
		for (unsigned int i = 0; i < n; i++) {
			ostream << std::string("    ");
		}
		return ostream;
	}

	std::ostream &operator<<(std::ostream &ostream, Tag &tag) {
		return tag.textOutput(ostream, 0);
	}

	TagType readType(std::istream &input, ssize_t &counter) {
		int8_t buf;
		input.read(reinterpret_cast<char *>(&buf), 1);
		input.exceptions(std::istream::failbit);
		counter++;
		return static_cast<TagType>(buf);
	}

	std::shared_ptr<Tag> Tag::parseTag(std::istream &input, TagType typeCode, ssize_t &counter) {
		std::shared_ptr<Tag> ptr;
		switch (typeCode) {
			case BYTE:
				ptr = std::make_shared<ByteTag>(input, counter);
				break;
			case SHORT:
				ptr = std::make_shared<ShortTag>(input, counter);
				break;
			case INT:
				ptr = std::make_shared<IntTag>(input, counter);
				break;
			case LONG:
				ptr = std::make_shared<LongTag>(input, counter);
				break;
			case FLOAT:
				ptr = std::make_shared<FloatTag>(input, counter);
				break;
			case DOUBLE:
				ptr = std::make_shared<DoubleTag>(input, counter);
				break;
			case BYTES:
				ptr = std::make_shared<BytesTag>(input, counter);
				break;
			case STRING:
				ptr = std::make_shared<StringTag>(input, counter);
				break;
			case LIST:
				ptr = std::make_shared<ListTag>(input, counter);
				break;
			case COMPOUND:
				ptr = std::make_shared<CompoundTag>(input, counter);
				break;
			case INTS:
				ptr = std::make_shared<IntsTag>(input, counter);
				break;
			case LONGS:
				ptr = std::make_shared<LongsTag>(input, counter);
				break;
			default:
				throw TagParseException(counter, fmt::format(INVALID_TYPE, std::underlying_type_t<TagType>(typeCode)));
		}
		return ptr;
	}

	std::shared_ptr<Tag> Tag::parseTag(std::istream &input, TagType typeCode, ssize_t &counter, IOFormat format) {
		switch (format) {
			case BIN:
				return parseTag(input, typeCode, counter);
			case PRETTY_PRINT:
				throw std::invalid_argument(PARSE_PRETTY);
			default:
				throw std::invalid_argument(PARSE_UNKNOWN_FMT);
		}
	}

	std::shared_ptr<Tag> Tag::deepCopy(const std::shared_ptr<Tag> &from) noexcept {
		std::shared_ptr<Tag> to;
		switch (from->typeCode()) {
			case BYTE:
				to = std::make_shared<ByteTag>(*(ByteTag *) from.get());
				break;
			case SHORT:
				to = std::make_shared<ShortTag>(*(ShortTag *) from.get());
				break;
			case INT:
				to = std::make_shared<IntTag>(*(IntTag *) from.get());
				break;
			case LONG:
				to = std::make_shared<LongTag>(*(LongTag *) from.get());
				break;
			case FLOAT:
				to = std::make_shared<FloatTag>(*(FloatTag *) from.get());
				break;
			case DOUBLE:
				to = std::make_shared<DoubleTag>(*(DoubleTag *) from.get());
				break;
			case BYTES:
				to = std::make_shared<BytesTag>(*(BytesTag *) from.get());
				break;
			case STRING:
				to = std::make_shared<StringTag>(*(StringTag *) from.get());
				break;
			case LIST:
				to = std::make_shared<ListTag>(*(ListTag *) from.get());
				break;
			case COMPOUND:
				to = std::make_shared<CompoundTag>(*(CompoundTag *) from.get());
				break;
			case INTS:
				to = std::make_shared<IntsTag>(*(IntsTag *) from.get());
				break;
			case LONGS:
				to = std::make_shared<LongsTag>(*(LongsTag *) from.get());
				break;
			default:
				to = nullptr;
		}
		return to;
	}

	std::string Tag::toString() const {
		std::stringstream ss;
		textOutput(ss, 0);
		return ss.str();
	}
}
