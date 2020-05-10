//
// Created by cth451 on 5/10/20.
//

#include <boost/format.hpp>
#include <utility>
#include "tags/StringTag.h"

namespace NBTP {
	ssize_t StringTag::size() {
		return this->payload.length();
	}

	TagType StringTag::typeCode() noexcept {
		return TagType::STRING;
	}

	bool StringTag::equal(Tag &rhs) {
		if (rhs.typeCode() != TagType::STRING) {
			return false;
		}
		return this->payload == ((StringTag &) rhs).payload;
	}

	std::ostream &StringTag::output(std::ostream &ostream, IOFormat format) {
		switch (format) {
			case TEXT:
				this->textOutput(ostream, 0);
				break;
			case BIN:
				// TODO
				break;
		}
		return ostream;
	}

	std::ostream &StringTag::textOutput(std::ostream &ostream, unsigned int indent) {
		std::string print = this->payload.length() ? this->payload : "(empty string)";
		ostream << boost::format("(%s) %s") % TypeNames[this->typeCode()] % print;
		return ostream;
	}

	const std::string &StringTag::getPayload() const {
		return this->payload;
	}

	void StringTag::setPayload(std::string s) {
		this->payload = s;
	}

	StringTag::StringTag(std::string s) {
		this->payload = s;
	}

	StringTag::StringTag() = default;
}