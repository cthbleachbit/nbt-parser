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
	private:
		V payload;
	public:
		/**
		 * @return NBT type code compound 1
		 */
		TagType typeCode() noexcept override;
		std::ostream &output(std::ostream &ostream, IOFormat format) override;
		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) override;
		LongTag(V value);
		/**
		 * Deserializer constructor
		 * @param input stream to read a single byte
		 */
		LongTag(std::istream& input);
	};
}


#endif //NBTP_LONGTAG_H
