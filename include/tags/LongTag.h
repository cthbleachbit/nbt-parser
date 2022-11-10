//
// Created by cth451 on 2020/05/09.
//

#ifndef NBTP_LONGTAG_H
#define NBTP_LONGTAG_H

#include "Tag.h"
#include "SingleValuedTag.h"

namespace NBTP {
	class LongTag : public SingleValuedTag<int64_t> {
	public:
		typedef int64_t V;
	public:
		/**
		 * @return NBT type code compound 1
		 */
		constexpr TagType typeCode() const noexcept override { return LONG; }

		explicit LongTag(V value) : SingleValuedTag<int64_t>(value) {};

		/**
		 * Deserializer constructor for decompressed NBT input
		 * @param input        stream to read a int64_t in Big Endian from
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 */
		LongTag(std::istream &input, ssize_t &counter) : LongTag(input, counter, BIN) {};

		/**
		 * Deserializer constructor with specified format
		 * @param input
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 * @param format       specifies the format of incoming data
		 */
		LongTag(std::istream &input, ssize_t &counter, IOFormat format)
				: SingleValuedTag<int64_t>(input, counter, format) {}

		/**
		 * Helper function to read in a single long from input binary stream
		 * @param input
		 * @param counter current offset into parsed stream
		 * @return parsed 64b integer
		 *
		 * @throw std::ios_base::failure  if I/O error has occurred
		 */
		static V parseLong(std::istream &input, ssize_t &counter) {
			return SingleValuedTag<V>::parseBinaryNumeric(input, counter);
		}
	};
}


#endif //NBTP_LONGTAG_H
