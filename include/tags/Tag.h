/**
 * @file Tag definitions - the basic for all other types of tags
 */

#ifndef NBTP_TAG_H
#define NBTP_TAG_H

#include "constants.h"

#include <string>
#include <memory>
#include <array>
#include <iostream>
#include <fmt/format.h>

namespace NBTP {

	/**
	 * Possible tag types and type code used for NBT tags
	 */
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
	auto format_as(TagType t) -> fmt::underlying_t<TagType>;

	/**
	 * Read type code at next byte and advance in the stream
	 * @param input input stream
	 * @param counter current offset into parsed stream
	 * @return type code
	 *
	 * @throw std::ios_base::failure  if I/O error has occurred
	 */
	TagType readType(std::istream &input, ssize_t &counter);

	/**
	 * Compile-time list of human readable names of every tag type
	 */
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

	public: /* Output control */
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
		virtual std::ostream &nbtOutput(std::ostream &ostream) const = 0;

		/**
		 * Helper function to write this tag in text to ostream.
 		 * Left broken deliberately as this function should not be called from outside
		 * @param ostream
		 * @param indent
		 * @return
		 */
		virtual std::ostream &textOutput(std::ostream &ostream, unsigned int indent) const = 0;

		[[nodiscard]] std::string toString() const;

		friend std::ostream &operator<<(std::ostream &ostream, Tag &tag);

	public: /* Comparison operators */

		[[nodiscard]] virtual bool equal(const Tag &rhs) const = 0;

		[[nodiscard]] bool equal(const std::shared_ptr<Tag> &rhs) const {
			return this->equal(*rhs);
		}

		bool operator==(const Tag &rhs) const {
			return this->equal(rhs);
		}

		bool operator!=(const Tag &rhs) const = default;

	public: /* Parse entry points and copy generator*/
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

	public: /* Destructor */
		virtual ~Tag() = default;
	};

	/**
	 * @class Container tags, namely compounds, lists, bytes, ints. Tags that hold other tags.
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
