//
// Created by cth451 on 2020/05/10.
//

#include <boost/format.hpp>
#include <ostream>
#include "tags/IntTag.h"
#include "tags/IntsTag.h"

namespace NBTP {
	TagType IntsTag::typeCode() noexcept {
		return TagType::INTS;
	}

	void IntsTag::insert(int32_t v) {
		ListTag::insert(std::make_shared<IntTag>(v));
	}

	void IntsTag::setContentType(TagType type) noexcept {}

	TagType IntsTag::getContentType() const noexcept {
		return TagType::BYTE;
	}

	std::ostream &IntsTag::output(std::ostream &ostream, IOFormat format) {
		switch (format) {
			case TEXT:
				this->textOutput(ostream, 0);
				break;
			case BIN:
				ListTag::output(ostream, BIN);
				break;
		}
		return ostream;
	}

	std::ostream &IntsTag::textOutput(std::ostream &ostream, unsigned int indent) {
		Tag::indent(ostream, indent);
		ostream << boost::format("Integer array with %i elements:") % this->size() << std::endl;
		for (const auto &elemItr : this->payload) {
			Tag::indent(ostream, indent + 1);
			elemItr->textOutput(ostream, indent + 1) << std::endl;
		}
		return ostream;
	}

	void IntsTag::insert(const std::shared_ptr<Tag> &v) {
		ListTag::insert(v);
	}

	IntsTag::IntsTag(std::istream &input) {
		int32_t size = IntTag::parseInt(input);
		if (size < 0) {
			throw std::runtime_error(CONTENT_LEN_NEG);
		}
		for (int i = 0; i < size; i++) {
			this->payload.push_back(std::shared_ptr<Tag>(new IntTag(input)));
		}
	}
}