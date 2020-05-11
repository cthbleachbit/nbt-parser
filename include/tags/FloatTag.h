//
// Created by cth451 on 2020/05/10.
//

#ifndef NBTP_FLOATTAG_H
#define NBTP_FLOATTAG_H

#include "Tag.h"

namespace NBTP {
	class FloatTag : public Tag {
	public:
		typedef float V;

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

		explicit FloatTag(V value);

		/**
		 * Deserializer constructor
		 * @param input stream to read a single byte
		 */
		explicit FloatTag(std::istream &input, ssize_t &counter);

		/**
		 * Helper function to write a float NBT-encoded to ostream
		 * @param ostream
		 * @param value
		 * @return
		 */
		static std::ostream &nbtOutput(std::ostream &ostream, V value);

		bool equal(Tag &rhs) override;
	};
}

#endif //NBTP_FLOATTAG_H
