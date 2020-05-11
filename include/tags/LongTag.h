//
// Created by cth451 on 2020/05/09.
//

#ifndef NBTP_LONGTAG_H
#define NBTP_LONGTAG_H

#include "Tag.h"

namespace NBTP {
	class LongTag : public Tag {
	public:
		typedef int64_t V;

		typedef V (*EndianConv)(V);

		static EndianConv toH;
		static EndianConv toJ;
	private:
		V payload;
	public:
		/**
		 * @return NBT type code compound 1
		 */
		TagType typeCode() noexcept override;

		std::ostream &output(std::ostream &ostream, IOFormat format) override;

		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) override;

		V getPayload() const;

		explicit LongTag(V value);

		/**
		 * Helper function to read in a single long from input binary stream
		 * @param input
		 * @return short
		 */
		static V parseLong(std::istream &input, ssize_t &counter);

		/**
		 * Deserializer constructor
		 * @param input stream to read a single byte
		 */
		explicit LongTag(std::istream &input, ssize_t &counter);

		/**
		 * Write a literal long to ostream in NBT format
		 * @param ostream
		 * @return ostream
		 */
		static std::ostream &nbtOutput(std::ostream &ostream, V value);

		bool equal(Tag &rhs) override;
	};
}


#endif //NBTP_LONGTAG_H
