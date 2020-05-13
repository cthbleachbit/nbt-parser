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
		typedef double (*EndianConv)(double);

		static EndianConv toH;
		static EndianConv toJ;
	public:
		TagType typeCode() noexcept override;

		std::ostream &output(std::ostream &ostream, IOFormat format) const override;

		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) const override;

		explicit DoubleTag(double value);

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
		static std::ostream &nbtOutput(std::ostream &ostream, double value);
	};
}

#endif //NBTP_FLOATTAG_H
