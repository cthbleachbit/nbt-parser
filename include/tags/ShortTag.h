//
// Created by cth451 on 2020/05/09.
//

#ifndef NBTP_SHORTTAG_H
#define NBTP_SHORTTAG_H

#include "Tag.h"
#include "SingleValuedTag.h"

namespace NBTP {
	class ShortTag : public SingleValuedTag<int16_t> {
	public:
		typedef int16_t V;

		typedef V (*EndianConv)(V);

		static EndianConv toH;
		static EndianConv toJ;
	public:
		constexpr TagType typeCode() const noexcept override { return SHORT; }

		explicit ShortTag(V value) : SingleValuedTag<int16_t>(value) {};

		/**
		 * Deserializer constructor for decompressed NBT input
		 * @param input        stream to read a float in Big Endian from
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 */
		ShortTag(std::istream &input, ssize_t &counter) : ShortTag(input, counter, BIN) {};

		/**
		 * Deserializer constructor with specified format
		 * @param input
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 * @param format       specifies the format of incoming data
		 */
		ShortTag(std::istream &input, ssize_t &counter, IOFormat format);

		/**
		 * Helper function to read in a single short from input binary stream
		 * @param input
		 * @param counter current offset into parsed stream
		 * @return parsed 16b integer
		 *
		 * @throw std::ios_base::failure  if I/O error has occurred
		 */
		static V parseShort(std::istream &input, ssize_t &counter);

		/**
		 * Helper function to write a single byte to ostream
		 * @param ostream
		 * @return
		 */
		std::ostream &nbtOutput(std::ostream &ostream) const override;
	};
}

#endif //NBTP_BYTETAG_H
