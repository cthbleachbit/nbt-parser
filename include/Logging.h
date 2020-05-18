//
// Created by cth451 on 2020/05/12.
//

#ifndef NBTP_LOGGING_H
#define NBTP_LOGGING_H

#include "WindowsHacks.h"
#include "tags/Tag.h"

namespace NBTP {
	namespace Logging {
		/**
		 * Print warning to stderr
		 * @param msg      warning message
		 * @param counter  the location in the stream this warning occurs
		 */
		void warn(const std::string &msg, ssize_t counter);
		/**
			 * Throws a runtime error with custom message
			 * @param msg
			 * @param counter  the location in the stream this error occurs
			 */
		void error(const std::string &msg, ssize_t counter);
	};
}


#endif //NBTP_LOGGING_H
