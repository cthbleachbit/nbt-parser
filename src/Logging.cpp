//
// Created by cth451 on 2020/05/12.
//

#include "tags/ShortTag.h"
#include "tags/StringTag.h"
#include "TagIO.h"
#include <boost/format.hpp>
#include <iostream>
#include "Logging.h"

void NBTP::Logging::error(const std::string &msg, ssize_t counter) {
	auto errmsg = boost::format("Error at %i: %s") % counter % msg;
	throw std::runtime_error(errmsg.str());
}

namespace NBTP {
	void Logging::warn(const std::string &msg, ssize_t counter) {
		std::cerr << boost::format("Warning at %i: %s") % counter % msg << std::endl;
	}
}
