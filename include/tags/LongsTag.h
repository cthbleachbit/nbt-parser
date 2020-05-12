//
// Created by cth451 on 5/10/20.
//

#ifndef NBTP_LONGSTAG_H
#define NBTP_LONGSTAG_H

#include "ListTag.h"

namespace NBTP {
	class LongsTag : public ListTag {
	public:
		/**
		 * @return NBT type long array 12
		 */
		TagType typeCode() noexcept override;

		/**
		 * @return NBT type long 4
		 */
		TagType getContentType() const noexcept override;

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
		 */
		LongsTag(std::istream &input, ssize_t &counter, IOFormat format);

		/**
		 * Default constructor
		 */
		LongsTag() = default;
	};
}

#endif //NBTP_LONGSTAG_H
