//
// Created by cth451 on 2020/05/10.
//

#ifndef NBTP_DOUBLETAG_H
#define NBTP_DOUBLETAG_H

#include "Tag.h"
#include "SingleValuedTag.h"

namespace NBTP {
	class DoubleTag : public SingleValuedTag<double> {
	public:
		typedef double V;
	public:
		constexpr TagType typeCode() const noexcept override { return DOUBLE; }

		explicit DoubleTag(V value) : SingleValuedTag<V>(value) {};

		/**
		 * Deserializer constructor for decompressed NBT input
		 * @param input        stream to read a double in Big Endian from
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 */
		DoubleTag(std::istream &input, ssize_t &counter) : DoubleTag(input, counter, BIN) {};

		/**
		 * Deserializer constructor with specified format
		 * @param input
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 * @param format       specifies the format of incoming data
		 *
		 * @throw std::ios_base::failure  if I/O error has occurred
		 */
		DoubleTag(std::istream &input, ssize_t &counter, IOFormat format)
				: SingleValuedTag<double>(input, counter, format) {};
	};
}

#endif //NBTP_FLOATTAG_H
