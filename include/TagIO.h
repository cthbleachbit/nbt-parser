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
		 * @return
		 */
		static std::shared_ptr<Tag> parseRoot(std::istream &input);

		/**
		 * Print warning to stderr
		 * @param msg      warning message
		 * @param offset   the location in the stream this warning occurs
		 */
		static void warn(const std::string &msg);
	};
}

#endif //NBTP_TAGIO_H
