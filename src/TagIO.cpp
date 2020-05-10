//
// Created by cth451 on 2020/05/10.
//

#include <iostream>
#include <boost/format.hpp>
#include "TagIO.h"
#include "tags/StringTag.h"

namespace NBTP {
	std::shared_ptr<Tag> TagIO::parseRoot(std::istream &input) {
		TagType typeCode = readType(input);
		if (typeCode != COMPOUND) {
			warn(ROOT_NOT_COMPOUND);
		}
		std::string name = StringTag::parseString(input);
		if (name.length() != 0) {
			warn(ROOT_HAS_NAME);
		}
		return Tag::parseTag(input, typeCode);
	}

	void TagIO::warn(const std::string &msg) {
		std::cerr << boost::format("[WARN] %s") % msg << std::endl;
	}
}

