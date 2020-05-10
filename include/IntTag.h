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
	private:
		V payload;
	public:
		/**
		 * @return NBT type code compound 1
		 */
		TagType typeCode() noexcept override;
		std::ostream &output(std::ostream &ostream, IOFormat format) override;
		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) override;
		IntTag(V value);
		/**
		 * Deserializer constructor
		 * @param input stream to read a single byte
		 */
		IntTag(std::istream& input);
	};
}

#endif //NBTP_INTTAG_H
