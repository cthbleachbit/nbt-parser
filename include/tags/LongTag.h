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

		typedef V (*EndianConv)(V);

		static EndianConv toH;
		static EndianConv toJ;
	public:
		/**
		 * @return NBT type code compound 1
		 */
		TagType typeCode() noexcept override;

		std::ostream &output(std::ostream &ostream, IOFormat format) override;

		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) override;

		explicit LongTag(V value) : SingleValuedTag<int64_t>(value) {};

		/**
		 * Helper function to read in a single long from input binary stream
		 * @param input
		 * @return short
		 */
		static V parseLong(std::istream &input, ssize_t &counter);

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
		LongTag(std::istream &input, ssize_t &counter, IOFormat format);

		/**
		 * Write a literal long to ostream in NBT format
		 * @param ostream
		 * @return ostream
		 */
		static std::ostream &nbtOutput(std::ostream &ostream, V value);
	};
}


#endif //NBTP_LONGTAG_H
