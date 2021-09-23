//
// Created by cth451 on 2020/05/10.
//

#ifndef NBTP_TAGIO_H
#define NBTP_TAGIO_H

#include <stdexcept>
#include "WindowsHacks.h"
#include "tags/Tag.h"

namespace NBTP {
	namespace TagIO {
		/**
		 * Parse a root tag, this will initialize this tag's type and stuff.
		 * @param input     the decompressed NBT binary stream
		 * @param counter   the number of bytes parsed
		 * @return
		 */
		std::shared_ptr<Tag> parseRoot(std::istream &input, ssize_t &counter);

		/**
		 * Parse a root tag, this will initialize this tag's type and stuff.
		 * @param input     the input stream
		 * @param counter   the number of bytes parsed
		 * @param format    specifies the format of incoming data
		 * @return
		 */
		std::shared_ptr<Tag> parseRoot(std::istream &input, ssize_t &counter, IOFormat format);

		/**
		 * Output the specified tag as the root tag to the ostream NBT-encoded
		 * @param ostream
		 * @param tag
		 */
		void writeRoot(std::ostream &ostream, Tag &tag);
	}

	class TagParseException : std::runtime_error {
	private:
		const ssize_t offset;
		std::string message;

	public:
		TagParseException(ssize_t offset, std::string reason) noexcept;
		const char *what() const noexcept override;
	};

	class ListTypeMismatchException : std::runtime_error {
	private:
		const TagType expected;
		const TagType got;
		std::string message;
	public:
		ListTypeMismatchException(TagType expected, TagType got) noexcept;
		const char *what() const noexcept override;
	};
}

#endif //NBTP_TAGIO_H
