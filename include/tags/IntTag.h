//
// Created by cth451 on 2020/05/09.
//

#ifndef NBTP_INTTAG_H
#define NBTP_INTTAG_H

#include "Tag.h"
#include "SingleValuedTag.h"

namespace NBTP {
	class IntTag : public SingleValuedTag<int32_t> {
	public:
		typedef int32_t V;
	public:
		constexpr TagType typeCode() const noexcept override { return INT; }

		explicit IntTag(V value) : SingleValuedTag<int32_t>(value) {};

		/**
		 * Deserializer constructor for decompressed NBT input
		 * @param input        stream to read a int32_t in big endian from
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 */
		IntTag(std::istream &input, ssize_t &counter) : IntTag(input, counter, BIN) {};

		/**
		 * Deserializer constructor with specified format
		 * @param input
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 * @param format       specifies the format of incoming data
		 */
		IntTag(std::istream &input, ssize_t &counter, IOFormat format)
				: SingleValuedTag<int32_t>(input, counter, format) {};

		/**
		 * Read an NBT encoded 32 bit integer from istream
		 * @param input
		 * @param counter current offset into parsed stream
		 * @return parsed integer
		 *
		 * @throw std::ios_base::failure  if I/O error has occurred
		 */
		static V parseInt(std::istream &input, ssize_t &counter) {
			return SingleValuedTag<V>::parseBinaryNumeric(input, counter);
		}
	};
}

#endif //NBTP_INTTAG_H
