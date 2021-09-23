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
		typedef V (*EndianConv)(double);

		static EndianConv toH;
		static EndianConv toJ;
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
		 */
		DoubleTag(std::istream &input, ssize_t &counter, IOFormat format);

		/**
		 * Helper function to write a double NBT encoded to ostream
		 * @param ostream
		 * @param value
		 * @return
		 */
		std::ostream &nbtOutput(std::ostream &ostream) const override;
	};
}

#endif //NBTP_FLOATTAG_H
