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

		explicit DoubleTag(V value);

		/**
		 * Deserializer constructor
		 * @param input stream to read a single byte
		 */
		explicit DoubleTag(std::istream &input, ssize_t &counter);

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
