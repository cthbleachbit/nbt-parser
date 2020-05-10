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
		virtual TagType typeCode() noexcept;
		static std::ostream &indent(std::ostream &ostream, unsigned int n);

		virtual std::ostream &output(std::ostream &ostream, IOFormat format) = 0;
		virtual std::ostream &textOutput(std::ostream &ostream, unsigned int indent);
		friend std::ostream &operator<< (std::ostream &ostream, Tag &tag);
	};

	/**
	 * A prototype class for container tags, namely compounds, lists, bytes, chars ...
	 */
	class ContainerTag : public Tag {
	public:
		/**
		 * @return size of this container
		 */
		virtual ssize_t size() = 0;
	};
}

#endif