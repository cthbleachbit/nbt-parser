//
// Created by cth451 on 2020/05/10.
//

#ifndef NBTP_FLOATTAG_H
#define NBTP_FLOATTAG_H

#include "Tag.h"
#include "SingleValuedTag.h"

namespace NBTP {
	class FloatTag : public SingleValuedTag<float> {
	public:
		typedef float V;
	public:
		constexpr TagType typeCode() const noexcept override { return FLOAT; }

		explicit FloatTag(V value) : SingleValuedTag<V>(value) {};

		/**
		 * Deserializer constructor for decompressed NBT input
		 * @param input        stream to read a float in Big Endian from
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 */
		FloatTag(std::istream &input, ssize_t &counter) : FloatTag(input, counter, BIN) {};

		/**
		 * Deserializer constructor with specified format
		 * @param input
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 * @param format       specifies the format of incoming data
		 *
		 * @throw std::ios_base::failure  if I/O error has occurred
		 */
		FloatTag(std::istream &input, ssize_t &counter, IOFormat format)
				: SingleValuedTag<float>(input, counter, format) {};
	};
}

#endif //NBTP_FLOATTAG_H
