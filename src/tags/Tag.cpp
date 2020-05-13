#include "libnbtp.h"
#include "Logging.h"
#include <istream>
#include <tags/Tag.h>


namespace NBTP {

	std::string TypeNames[] = {
			"End of Tag",
			"Byte",
			"Short",
			"Int",
			"Long",
			"Float",
			"Double",
			"Byte array",
			"String",
			"List",
			"Compound",
			"Integet array",
			"Long array"
	};

	std::ostream &Tag::output(std::ostream &ostream, IOFormat format) const {
		throw std::runtime_error(GENERIC_METHOD);
	}

	std::ostream &Tag::textOutput(std::ostream &ostream, unsigned int indent) const {
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

	TagType readType(std::istream &input, ssize_t &counter) {
		int8_t buf;
		input.read(reinterpret_cast<char *>(&buf), 1);
		if (input.fail()) {
			Logging::error(IO_UNEXPECTED_EOF, counter);
		}
		counter++;
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
				Logging::error(INVALID_TYPE, counter);
		}
		return ptr;
	}

	std::shared_ptr<Tag> Tag::parseTag(std::istream &input, TagType typeCode, ssize_t &counter, IOFormat format) {
		switch (format) {
			case BIN:
				return parseTag(input, typeCode, counter);
			case PRETTY_PRINT:
				Logging::error(PARSE_PRETTY, counter);
				break;
			default:
				Logging::error(PARSE_UNKNOWN_FMT, counter);
				break;
		}
		// This should never happen
		return std::shared_ptr<Tag>(nullptr);
	}
}
