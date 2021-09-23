//
// Created by cth451 on 2020/05/09.
//

#ifndef NBTP_BYTETAG_H
#define NBTP_BYTETAG_H

#include "SingleValuedTag.h"
#include "Tag.h"

namespace NBTP {
	class ByteTag : public SingleValuedTag<int8_t> {
	public:
		typedef int8_t V;
	public:
		/**
		 * @return NBT type code compound 1
		 */
		constexpr TagType typeCode() const noexcept override { return BYTE; }

		explicit ByteTag(V value) : SingleValuedTag<V>(value) {};

		/**
		 * Deserializer constructor for decompressed NBT input
		 * @param input stream to read a single byte
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 */
		ByteTag(std::istream &input, ssize_t &counter) : ByteTag(input, counter, BIN) {};

		/**
		 * Deserializer constructor with specified format
		 * @param input
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 * @param format       specifies the format of incoming data
		 */
		ByteTag(std::istream &input, ssize_t &counter, IOFormat format);

		/**
		 * Helper function to write a single byte to ostream
		 * @param ostream
		 * @return
		 */
		std::ostream &nbtOutput(std::ostream &ostream) const override;
	};
}

#endif //NBTP_BYTETAG_H
