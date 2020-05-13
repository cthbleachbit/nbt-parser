#ifndef NBTP_TAG_H
#define NBTP_TAG_H

#include "constants.h"

#include <cstdint>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <memory>

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
		LONGS = 12
	};

	TagType readType(std::istream &input, ssize_t &counter);

	extern std::string TypeNames[];

	/**
	 * A Tag is defined as either a single value or a container of key-tag pairs
	 *
	 * A tag by itself carries no name. The name attribute of a tag is contained in its patent tag as the key.
	 * A compound tag is a key-tag hash map.
	 * A list tag is a vector of tags
	 */
	class Tag {
	public:
		virtual TagType typeCode() const noexcept;

		/**
		 * A helper function to print 4n spaces
		 * @param ostream   printing output
		 * @param n         number of spaces / 4
		 * @return ostream itself
		 */
		static std::ostream &indent(std::ostream &ostream, unsigned int n);

		virtual std::ostream &output(std::ostream &ostream, IOFormat format) const;

		virtual std::ostream &textOutput(std::ostream &ostream, unsigned int indent) const;

		std::string toString();

		virtual bool equal(Tag &rhs) const = 0;

		bool equal(std::shared_ptr<Tag> &rhs) const;

		friend std::ostream &operator<<(std::ostream &ostream, Tag &tag);

		bool operator==(Tag &rhs) const;

		bool operator!=(Tag &rhs) const;

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
	};
}

#endif