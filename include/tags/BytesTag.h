//
// Created by cth451 on 2020/05/10.
//

#ifndef NBTP_BYTESTAG_H
#define NBTP_BYTESTAG_H

#include "ListTag.h"

namespace NBTP {
	class BytesTag : public TypedListTag {
	public:
		/**
		 * @return NBT type byte array 7
		 */
		TagType typeCode() noexcept override;

		/**
		 * @return NBT type byte 1
		 */
		TagType getContentType() const noexcept override;

		/**
		 * Insert a literal byte into the byte array
		 * @param v the byte to insert
		 */
		void insert(int8_t v);

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
		BytesTag(std::istream &input, ssize_t &counter);

		/**
		 * Deserialize constructor with a format specified
		 * @param input
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 * @param format       specifies the format of incoming data
		 */
		BytesTag(std::istream &input, ssize_t &counter, IOFormat format);

		/**
		 * Trivial constructor
		 */
		BytesTag() = default;
	};
}

#endif //NBTP_BYTESTAG_H
