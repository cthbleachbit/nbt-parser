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
		typedef float (*EndianConv)(float);

		static EndianConv toH;
		static EndianConv toJ;
	public:
		TagType typeCode() noexcept override;

		std::ostream &output(std::ostream &ostream, IOFormat format) const override;

		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) const override;

		explicit FloatTag(float value) : SingleValuedTag<float>(value) {};

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
		 */
		FloatTag(std::istream &input, ssize_t &counter, IOFormat format);

		/**
		 * Helper function to write a float NBT-encoded to ostream
		 * @param ostream
		 * @param value
		 * @return
		 */
		static std::ostream &nbtOutput(std::ostream &ostream, float value);
	};
}

#endif //NBTP_FLOATTAG_H
