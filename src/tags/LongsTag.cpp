//
// Created by cth451 on 5/10/20.
//

#include <boost/format.hpp>
#include "tags/IntTag.h"
#include "tags/LongTag.h"
#include "tags/LongsTag.h"

namespace NBTP {
	TagType LongsTag::typeCode() noexcept {
		return TagType::LONGS;
	}

	void LongsTag::insert(int64_t v) {
		ListTag::insert(std::make_shared<LongTag>(v));
	}

	void LongsTag::setContentType(TagType type) noexcept {}

	TagType LongsTag::getContentType() const noexcept {
		return TagType::LONG;
	}

	std::ostream &LongsTag::output(std::ostream &ostream, IOFormat format) {
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

	std::ostream &LongsTag::textOutput(std::ostream &ostream, unsigned int indent) {
		Tag::indent(ostream, indent);
		ostream << boost::format("Long array with %i elements:") % this->size() << std::endl;
		for (const auto &elemItr : this->payload) {
			Tag::indent(ostream, indent + 1);
			elemItr->textOutput(ostream, indent + 1);
		}
		return ostream;
	}

	void LongsTag::insert(const std::shared_ptr<Tag> &v) {
		ListTag::insert(v);
	}

	LongsTag::LongsTag(std::istream &input) {
		int32_t size = IntTag::parseInt(input);
		if (size < 0) {
			throw std::runtime_error(CONTENT_LEN_NEG);
		}
		for (int i = 0; i < size; i++) {
			this->payload.push_back(std::shared_ptr<Tag>(new LongTag(input)));
		}
	}
}