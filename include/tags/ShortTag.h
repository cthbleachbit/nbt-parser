//
// Created by cth451 on 2020/05/09.
//

#ifndef NBTP_SHORTTAG_H
#define NBTP_SHORTTAG_H

#include "Tag.h"

namespace NBTP {
	class ShortTag : public Tag {
	public:
		typedef int16_t V;

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

		explicit ShortTag(V value);

		/**
		 * Deserializer constructor
		 * @param input stream to read a single byte
		 */
		explicit ShortTag(std::istream &input, ssize_t &counter);

		/**
		 * Helper function to read in a single short from input binary stream
		 * @param input
		 * @return short
		 */
		static V parseShort(std::istream &input, ssize_t &counter);

		/**
		 * Write a literal short to ostream in NBT format
		 * @param ostream
		 * @return ostream
		 */
		static std::ostream &nbtOutput(std::ostream &ostream, V value);

		bool equal(Tag &rhs) override;
	};
}

#endif //NBTP_BYTETAG_H
