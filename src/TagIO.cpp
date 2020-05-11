//
// Created by cth451 on 2020/05/10.
//

#include <iostream>
#include <boost/format.hpp>
#include "TagIO.h"
#include "tags/StringTag.h"
#include "tags/ShortTag.h"

namespace NBTP {
	std::shared_ptr<Tag> TagIO::parseRoot(std::istream &input, ssize_t &counter) {
		TagType typeCode = readType(input);
		if (typeCode != COMPOUND) {
			warn(ROOT_NOT_COMPOUND, counter);
		}
		counter++;
		std::string name = StringTag::parseString(input);
		if (name.length() != 0) {
			warn(ROOT_HAS_NAME, counter);
		}
		counter += 2 + name.length();
		return Tag::parseTag(input, typeCode, <#initializer#>);
	}

	void TagIO::warn(const std::string &msg, ssize_t counter) {
		std::cerr << boost::format("Warning at %i: %s") % counter % msg << std::endl;
	}

	void TagIO::dumpRoot(std::ostream &ostream, Tag &tag) {
		if (tag.typeCode() != COMPOUND) {
			warn(ROOT_NOT_COMPOUND, 0);
		}
		char typeByte = static_cast<char>(tag.typeCode());
		ostream.write(&typeByte, 1);
		ShortTag::nbtOutput(ostream, 0);
		tag.output(ostream, BIN);
	}

	void TagIO::error(const std::string &msg, ssize_t counter) {
		auto errmsg = boost::format("Error at %i: %s") % counter % msg;
		throw std::runtime_error(errmsg.str());
	}
}

