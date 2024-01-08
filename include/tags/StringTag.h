//
// Created by cth451 on 5/10/20.
//

#ifndef NBTP_STRINGTAG_H
#define NBTP_STRINGTAG_H

#include "Tag.h"

namespace NBTP {
	class StringTag : public ContainerTag {
	private:
		std::string payload;
	public:
		/**
		 * @return length of the string
		 */
		ssize_t size() const override;

		/**
		 * @return NBT type code string
		 */
		constexpr TagType typeCode() const noexcept override { return STRING; }

		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) const override;

		/**
		 * @return an immutable reference to the string
		 */
		const std::string &getPayload() const;

		/**
		 * Set the string content of this tag
		 * @param s
		 */
		void setPayload(std::string s);

		/**
		 * Compare whether two String tags are equal
		 * @param rhs Tag - doesn't have to be StringTag
		 * @return whether rhs is a StringTag and has identical content as this
		 */
		bool equal(const Tag &rhs) const override;

	public: /* C-tor, D-tor, copy/move assignments */
		/**
		 * Construct a string tag from a c++ string
		 * @param s
		 */
		explicit StringTag(std::string s);

		/**
		 * Default constructor
		 */
		StringTag();

		/**
		 * Copy constructor
		 * @param tag
		 */
		StringTag(const StringTag &tag) noexcept;

		/**
		 * Copy assignment operator
		 * @param tag
		 * @return self
		 */
		StringTag &operator=(const StringTag &tag) noexcept;

		/**
		 * Move constructor
		 * @param tag
		 */
		StringTag(StringTag &&tag) noexcept;

		/**
		 * Move assignment operator
		 * @param tag
		 * @return self
		 */
		StringTag &operator=(StringTag &&tag) noexcept;

	public: /* I/O */

		/**
		 * Constructs a string tag from uncompressed NBT data stream
		 * This will consume two bytes, to read in length of the string X, then the following X bytes
		 * for the string itself.
		 *
		 * @param input    stream of uncompressed NBT data
		 * @param counter  updated to reflect the number of bytes read from the input stream
		 */
		StringTag(std::istream &input, ssize_t &counter) : StringTag(input, counter, BIN) {};

		/**
		 * Constructs a string tag from input stream with specified format
		 *
		 * @param input    stream of uncompressed NBT data
		 * @param counter  updated to reflect the number of bytes read from the input stream
		 * @param format   input format specifier
		 */
		StringTag(std::istream &input, ssize_t &counter, IOFormat format);

		/**
		 * Writes a string in NBT encoded format, i.e. length + non-termininated string
		 * @param ostream
		 * @return
		 */
		std::ostream &nbtOutput(std::ostream &ostream) const override;

		/**
		 * Static helper function for getting a string from NBT
		 * This will consume two bytes, to read in length of the string X, then the following X bytes
		 * for the string itself.
		 *
		 * @param input    stream of uncompressed NBT data
		 * @param counter  updated to reflect the number of bytes read from the input stream
		 *
		 * @return parsed string
		 *
		 * @throw NBTP::TagParseException if string length is negative
		 * @throw std::ios_base::failure  if I/O error has occurred
		 */
		static std::string parseString(std::istream &input, ssize_t &counter);
	};
}


#endif //NBTP_STRINGTAG_H
