//
// Created by cth451 on 2020/05/12.
//

#include "TagIO.h"
#include <iostream>
#include "Logging.h"

namespace NBTP {
	void Logging::warn(const std::string &msg, ssize_t counter) {
		fprintf(stderr, "Warning at byte offset %li: %s\n", counter, msg.c_str());
	}

	void Logging::error(const std::string &msg, ssize_t counter) {
		throw TagParseException(counter, msg);
	}
}
