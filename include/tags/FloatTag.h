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
		/**
		 * @return NBT type code compound 1
		 */
		TagType typeCode() noexcept override;
		std::ostream &output(std::ostream &ostream, IOFormat format) override;
		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) override;
		FloatTag(V value);
		/**
		 * Deserializer constructor
		 * @param input stream to read a single byte
		 */
		FloatTag(std::istream& input);
	};
}

#endif //NBTP_FLOATTAG_H
