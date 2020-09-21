//
// Created by cth451 on 2020/05/12.
//

#include "TagIO.h"
#include <boost/format.hpp>
#include <iostream>
#include "Logging.h"

void NBTP::Logging::error(const std::string &msg, ssize_t counter) {
	throw TagParseException(counter, msg);
}

namespace NBTP {
	void Logging::warn(const std::string &msg, ssize_t counter) {
		std::cerr << boost::format("Warning at byte offset %i: %s") % counter % msg << std::endl;
	}
}
