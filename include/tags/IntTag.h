//
// Created by cth451 on 2020/05/09.
//

#ifndef NBTP_INTTAG_H
#define NBTP_INTTAG_H

#include "Tag.h"

namespace NBTP {
	class IntTag : public Tag {
	public:
		typedef int32_t V;

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

		explicit IntTag(V value);

		/**
		 * Deserializer constructor
		 * @param input stream to read a single byte
		 */
		explicit IntTag(std::istream &input);

		/**
		 * Read an NBT encoded 32 bit integer from istream
		 * @param input
		 * @return
		 */
		static V parseInt(std::istream &input);

		/**
		 * Helper function to write a 32-bit integer NBT encoded to ostream
		 * @param ostream
		 * @param value
		 * @return
		 */
		static std::ostream &nbtOutput(std::ostream&ostream, V value);

		bool equal(Tag &rhs) override;
	};
}

#endif //NBTP_INTTAG_H
