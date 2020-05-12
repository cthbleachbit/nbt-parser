//
// Created by cth451 on 2020/05/10.
//

#ifndef NBTP_INTSTAG_H
#define NBTP_INTSTAG_H

#include "ListTag.h"

namespace NBTP {
	class IntsTag : public TypedListTag {
	public:
		/**
		 * @return NBT type int array 11
		 */
		TagType typeCode() noexcept override;

		/**
		 * @return NBT type int 3
		 */
		TagType getContentType() const noexcept override;

		/**
		 * Insert a literal integer into the byte array
		 * @param v the integer to insert
		 */
		void insert(int32_t v);

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
		IntsTag(std::istream &input, ssize_t &counter) : IntsTag(input, counter, BIN) {};

		/**
		 * Deserialize constructor with a format specified
		 * @param input
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 * @param format       specifies the format of incoming data
		 */
		IntsTag(std::istream &input, ssize_t &counter, IOFormat format);

		/**
		 * Default constructor
		 */
		IntsTag() = default;
	};
}


#endif //NBTP_INTSTAG_H
