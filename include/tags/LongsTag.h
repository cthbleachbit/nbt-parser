//
// Created by cth451 on 5/10/20.
//

#ifndef NBTP_LONGSTAG_H
#define NBTP_LONGSTAG_H

#include "ListTag.h"

namespace NBTP {
	class LongsTag : public TypedListTag {
	public:
		/**
		 * @return NBT type long array 12
		 */
		constexpr TagType typeCode() const noexcept override { return LONGS; }

		/**
		 * @return NBT type long 4
		 */
		constexpr TagType getContentType() const noexcept override { return LONG; }

		/**
		 * Insert a literal long into the byte array
		 * @param v the long to insert
		 */
		void insert(int64_t v);

		/**
		 * Insert a pre-formed tag
		 * @param v
		 */
		void insert(const std::shared_ptr<Tag> &v) override;

		/**
		 * Deserialize constructor for decompressed NBT input
		 * @param input
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 */
		LongsTag(std::istream &input, ssize_t &counter) : LongsTag(input, counter, BIN) {};

		/**
		 * Deserialize constructor with a format specified
		 * @param input
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 * @param format       specifies the format of incoming data
		 *
		 * @throw NBTP::TagParseException if array length is negative
		 * @throw std::ios_base::failure  if I/O error has occurred
		 */
		LongsTag(std::istream &input, ssize_t &counter, IOFormat format);

		/**
		 * Default constructor
		 */
		LongsTag() = default;
	};
}

#endif //NBTP_LONGSTAG_H
