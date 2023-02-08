/**
 * @file libnbtp tag input/output routines
 */

#ifndef NBTP_TAGIO_H
#define NBTP_TAGIO_H

#include <stdexcept>
#include "tags/Tag.h"

namespace NBTP {
	namespace TagIO {
		/**
		 * Parse a root tag, this will initialize this tag's type and stuff.
		 * @param input     the decompressed NBT binary stream
		 * @param counter   the number of bytes parsed
		 * @param logging   warning output stream, defaults to nullptr
		 *
		 * @return successfully parsed tag on successful parse
		 *
		 * @throw std::ios_base::failure for any I/O error occurred
		 * @throw std::TagParseException for errors occurred due to malformed tag
		 */
		std::shared_ptr<Tag> parseRoot(
				std::istream &input,
				ssize_t &counter,
				std::ostream *logging = nullptr
		);

		/**
		 * Parse a root tag, this will initialize this tag's type and stuff.
		 * @param input     the input stream
		 * @param counter   the number of bytes parsed
		 * @param format    specifies the format of incoming data
		 * @param logging   warning output stream, defaults to nullptr
		 *
		 * @return successfully parsed tag on successful parse
		 *
		 * @throw std::ios_base::failure    for any I/O error occurred
		 * @throw NBTP::TagParseException   for errors due to malformed tag
		 * @throw std::invalid_argument     if incorrect IOFormat were specified
		 */
		std::shared_ptr<Tag> parseRoot(
				std::istream &input,
				ssize_t &counter,
				IOFormat format,
				std::ostream *logging = nullptr
		);

		/**
		 * Output the specified tag as the root tag to the ostream NBT-encoded
		 * @param ostream   data output stream
		 * @param tag       tag to output
		 * @param logging   warning output stream, defaults to nullptr
		 */
		void writeRoot(std::ostream &ostream, const Tag &tag, std::ostream *logging = nullptr);
	}

	/**
	 * @class Exception thrown when the parser encounters an irrecoverable error
	 */
	class TagParseException : std::runtime_error {
	private:
		const ssize_t offset;
		std::string message;

	public:
		/**
		 * @param offset byte-offset into the input stream
		 * @param reason human readable error string describing what went wrong
		 */
		TagParseException(ssize_t offset, std::string reason) noexcept;
		const char *what() const noexcept override;
	};

	/**
	 * @class Exception thrown when a tag of one type is inserted into a list with tags of another type
	 */
	class ListTypeMismatchException : std::runtime_error {
	private:
		const TagType expected;
		const TagType got;
		std::string message;
	public:
		/**
		 * @param expected type expected under this context
		 * @param got      actual tag type received
		 */
		ListTypeMismatchException(TagType expected, TagType got) noexcept;
		const char *what() const noexcept override;
	};
}

#endif //NBTP_TAGIO_H
