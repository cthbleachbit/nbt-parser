//
// Created by cth451 on 2020/05/09.
//

#ifndef NBTP_BYTETAG_H
#define NBTP_BYTETAG_H

#include "Tag.h"

namespace NBTP {
	class ByteTag : public Tag {
	public:
		typedef int8_t V;
	private:
		V payload;
	public:
		/**
		 * @return NBT type code compound 1
		 */
		static int8_t typeCode() noexcept;
		std::ostream &output(std::ostream &ostream, IOFormat format) override;
		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) override;
		ByteTag(V value);
		/**
		 * Deserializer constructor
		 * @param input stream to read a single byte
		 */
		ByteTag(std::istream& input);
	};
}

#endif //NBTP_BYTETAG_H
