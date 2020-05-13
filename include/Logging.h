//
// Created by cth451 on 2020/05/12.
//

#ifndef NBTP_LOGGING_H
#define NBTP_LOGGING_H

#include "tags/Tag.h"

namespace NBTP {
	class Logging {

	public:
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


#endif //NBTP_LOGGING_H
