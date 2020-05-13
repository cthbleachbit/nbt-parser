//
// Created by cth451 on 2020/05/10.
//

#ifndef NBTP_DOUBLETAG_H
#define NBTP_DOUBLETAG_H

#include "Tag.h"

namespace NBTP {
	class DoubleTag : public Tag {
	public:
		typedef double V;

		typedef V (*EndianConv)(V);

		static EndianConv toH;
		static EndianConv toJ;
	private:
		V payload;
	public:
		TagType typeCode() noexcept override;

		std::ostream &output(std::ostream &ostream, IOFormat format) override;

		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) override;

		V getPayload() const;

		void setPayload(V value);

		explicit DoubleTag(V value);

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
		static std::ostream &nbtOutput(std::ostream &ostream, V value);

		bool equal(Tag &rhs) override;
	};
}

#endif //NBTP_FLOATTAG_H
