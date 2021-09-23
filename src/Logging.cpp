//
// Created by cth451 on 2020/05/12.
//

#include "TagIO.h"
#include <iostream>
#include <fmt/format.h>
#include "Logging.h"

namespace NBTP {
	void Logging::warn(const std::string &msg, ssize_t counter) {
		std::cerr << fmt::format(WARNING_PREFIX, counter, msg) << std::endl;
	}

	void Logging::error(const std::string &msg, ssize_t counter) {
		throw TagParseException(counter, msg);
	}
}
