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

		typedef V (*EndianConv)(V);

		static EndianConv toH;
		static EndianConv toJ;
	public:
		TagType typeCode() const noexcept override;

		std::ostream &output(std::ostream &ostream, IOFormat format) const override;

		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) const override;

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
		IntTag(std::istream &input, ssize_t &counter, IOFormat format);

		/**
		 * Read an NBT encoded 32 bit integer from istream
		 * @param input
		 * @return
		 */
		static V parseInt(std::istream &input, ssize_t &counter);

		/**
		 * Helper function to write a 32-bit integer NBT encoded to ostream
		 * @param ostream
		 * @param value
		 * @return
		 */
		static std::ostream &nbtOutput(std::ostream &ostream, V value);
	};
}

#endif //NBTP_INTTAG_H
