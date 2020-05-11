//
// Created by cth451 on 2020/05/10.
//

#ifndef NBTP_TAGIO_H
#define NBTP_TAGIO_H

#include "tags/Tag.h"

namespace NBTP {

	class TagIO {
	public:
		/**
		 * Parse a root tag, this will initialize this tag's type and stuff.
		 * @param input
		 * @param counter   the number of bytes parsed
		 * @return
		 */
		static std::shared_ptr<Tag> parseRoot(std::istream &input, ssize_t &counter);

		/**
		 * Output the specified tag as the root tag to the ostream NBT-encoded
		 * @param ostream
		 * @param tag
		 */
		static void dumpRoot(std::ostream &ostream, Tag& tag);

		/**
		 * Print warning to stderr
		 * @param msg      warning message
		 * @param counter  the location in the stream this warning occurs
		 */
		static void warn(const std::string &msg, ssize_t counter);

		/**
		 * Throws a runtime error with custom message
		 * @param msg
		 * @param counter  the location in the stream this error occurs
		 */
		static void error(const std::string &msg, ssize_t counter);
	};
}

#endif //NBTP_TAGIO_H
