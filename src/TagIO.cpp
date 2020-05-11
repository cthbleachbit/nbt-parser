//
// Created by cth451 on 2020/05/10.
//

#include <iostream>
#include <boost/format.hpp>
#include "TagIO.h"
#include "tags/StringTag.h"
#include "tags/ShortTag.h"

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

	void TagIO::dumpRoot(std::ostream &ostream, Tag &tag) {
		if (tag.typeCode() != COMPOUND) {
			warn(ROOT_NOT_COMPOUND);
		}
		char typeByte = static_cast<char>(tag.typeCode());
		ostream.write(&typeByte, 1);
		ShortTag::nbtOutput(ostream, 0);
		tag.output(ostream, BIN);
	}
}

