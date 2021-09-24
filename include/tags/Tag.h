#ifndef NBTP_TAG_H
#define NBTP_TAG_H

#include "WindowsHacks.h"
#include "constants.h"

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <memory>
#include <fmt/format.h>

namespace NBTP {

	enum TagType {
		INVALID = -1,
		END = 0,
		BYTE = 1,
		SHORT = 2,
		INT = 3,
		LONG = 4,
		FLOAT = 5,
		DOUBLE = 6,
		BYTES = 7,
		STRING = 8,
		LIST = 9,
		COMPOUND = 10,
		INTS = 11,
		LONGS = 12,
		END_OF_TAG_TYPE,
	};

	TagType readType(std::istream &input, ssize_t &counter);

	extern const std::array<const char*, TagType::END_OF_TAG_TYPE> TypeNames;

	/**
	 * A Tag is defined as either a single value or a container of key-tag pairs
	 *
	 * A tag by itself carries no name. The name attribute of a tag is contained in its patent tag as the key.
	 * A compound tag is a key-tag hash map.
	 * A list tag is a vector of tags
	 */
	class Tag {
	public:
		virtual constexpr TagType typeCode() const noexcept { return INVALID; }

		/**
		 * A helper function to print 4n spaces
		 * @param ostream   printing output
		 * @param n         number of spaces / 4
		 * @return ostream itself
		 */
		static std::ostream &indent(std::ostream &ostream, unsigned int n);

		/**
		 * Write out this tag
		 * @param ostream   output stream to write
		 * @param format    format to write
		 * @return          output stream itself
		 */
		virtual std::ostream &output(std::ostream &ostream, IOFormat format) const {
			switch (format) {
				case PRETTY_PRINT:
					this->textOutput(ostream, 0);
					break;
				case BIN:
					this->nbtOutput(ostream);
					break;
			}
			return ostream;
		}

		/**
 		 * Helper function to write this tag in binary to ostream.
 		 * Left broken deliberately as this function should not be called from outside
 		 * @param ostream
 		 * @return
 		 */
		virtual std::ostream &nbtOutput(std::ostream &ostream) const {
			// FIXME: Use std::source_location::current().function_name() when we have GCC 11
			throw(std::runtime_error(fmt::format(GENERIC_METHOD, __FUNCTION__)));
		}

		/**
		 * Helper function to write this tag in text to ostream.
 		 * Left broken deliberately as this function should not be called from outside
		 * @param ostream
		 * @param indent
		 * @return
		 */
		virtual std::ostream &textOutput(std::ostream &ostream, unsigned int indent) const {
			throw(std::runtime_error(fmt::format(GENERIC_METHOD, __FUNCTION__)));
		}

		[[nodiscard]] virtual std::string toString() const;

		[[nodiscard]] virtual bool equal(const Tag &rhs) const = 0;

		[[nodiscard]] bool equal(const std::shared_ptr<Tag> &rhs) const {
			return this->equal(*rhs);
		}

		friend std::ostream &operator<<(std::ostream &ostream, Tag &tag);

		bool operator==(const Tag &rhs) const {
			return this->equal(rhs);
		}

		bool operator!=(const Tag &rhs) const = default;

		/**
		 * A wrapper to parse tags based on type
		 * @param input        input decompressed NBT binary stream
		 * @param typeCode     type code of the upcoming tag
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 * @return a pointer to the tag
		 */
		static std::shared_ptr<Tag> parseTag(std::istream &input, TagType typeCode, ssize_t &counter);

		/**
		 * A wrapper to parse tags based on type
		 * @param input        input decompressed NBT binary stream
		 * @param typeCode     type code of the upcoming tag
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 * @param format       specifies the format of incoming data
		 * @return a pointer to the tag
		 */
		static std::shared_ptr<Tag> parseTag(std::istream &input, TagType typeCode, ssize_t &counter, IOFormat format);

		/**
		 * Deep copy a tag to a new shared pointer. This creates a copy that can be updated independently.
		 * @param from   tag to copy from
		 * @return       a copy of the tag in a new memory location
		 */
		static std::shared_ptr<Tag> deepCopy(const std::shared_ptr<Tag> &from) noexcept;

		virtual ~Tag() = default;
	};

	/**
	 * A prototype class for container tags, namely compounds, lists, bytes, chars ...
	 */
	class ContainerTag : public Tag {
	public:
		/**
		 * @return size of this container
		 */
		virtual ssize_t size() const = 0;

		~ContainerTag() override = default;
	};
}

#endif