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
		 * Deserialize constructor
		 * @param input
		 */
		BytesTag(std::istream &input, ssize_t &counter);
	};
}

#endif //NBTP_BYTESTAG_H
