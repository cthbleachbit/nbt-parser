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
			case PRETTY_PRINT:
				this->textOutput(ostream, 0);
				break;
			case BIN:
				// Do size sanity checking
				if (this->size() > INT32_MAX) {
					throw std::runtime_error(LIST_TOO_LONG);
				}
				IntTag::nbtOutput(ostream, this->size());
				outputPayloadOnly(ostream, BIN, 0);
				break;
		}
		return ostream;
	}

	std::ostream &LongsTag::textOutput(std::ostream &ostream, unsigned int indent) {
		ostream << boost::format("%s with %i elements:") % TypeNames[this->typeCode()] % this->size() << std::endl;
		outputPayloadOnly(ostream, PRETTY_PRINT, indent);
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