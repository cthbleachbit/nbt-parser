//
// Created by cth451 on 2020/05/10.
//

#ifndef NBTP_TAGIO_H
#define NBTP_TAGIO_H

#include "tags/Tag.h"

namespace NBTP {

	class TagIO {
	protected:
		/**
		 * Parse a root tag, this will initialize this tag's type and stuff.
		 * @param input     the decompressed NBT binary stream
		 * @param counter   the number of bytes parsed
		 * @return
		 */
		static std::shared_ptr<Tag> parseRoot(std::istream &input, ssize_t &counter);

	public:
		/**
		 * Parse a root tag, this will initialize this tag's type and stuff.
		 * @param input     the input stream
		 * @param counter   the number of bytes parsed
		 * @param format    specifies the format of incoming data
		 * @return
		 */
		static std::shared_ptr<Tag> parseRoot(std::istream &input, ssize_t &counter, IOFormat format);

		/**
		 * Output the specified tag as the root tag to the ostream NBT-encoded
		 * @param ostream
		 * @param tag
		 */
		static void dumpRoot(std::ostream &ostream, Tag &tag);
	};
}

#endif //NBTP_TAGIO_H
