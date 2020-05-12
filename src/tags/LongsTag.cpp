//
// Created by cth451 on 5/10/20.
//

#include <boost/format.hpp>
#include "tags/IntTag.h"
#include "tags/LongTag.h"
#include "tags/LongsTag.h"
#include "TagIO.h"

namespace NBTP {
	TagType LongsTag::typeCode() noexcept {
		return TagType::LONGS;
	}

	void LongsTag::insert(int64_t v) {
		ListTag::insert(std::make_shared<LongTag>(v));
	}

	TagType LongsTag::getContentType() const noexcept {
		return TagType::LONG;
	}

	void LongsTag::insert(const std::shared_ptr<Tag> &v) {
		ListTag::insert(v);
	}

	LongsTag::LongsTag(std::istream &input, ssize_t &counter, IOFormat format) {
		int32_t size;
		switch (format) {
			case BIN:
				size = IntTag::parseInt(input, counter);
				if (size < 0) {
					TagIO::error(CONTENT_LEN_NEG, counter);
				}
				for (int i = 0; i < size; i++) {
					this->payload.push_back(std::shared_ptr<Tag>(new LongTag(input, counter)));
				}
				break;
			case PRETTY_PRINT:
				TagIO::error(PARSE_PRETTY, counter);
		}
	}
}