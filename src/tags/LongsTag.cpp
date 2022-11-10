//
// Created by cth451 on 5/10/20.
//

#include "tags/IntTag.h"
#include "tags/LongTag.h"
#include "tags/LongsTag.h"
#include "TagIO.h"

namespace NBTP {
	void LongsTag::insert(int64_t v) {
		ListTag::insert(std::make_shared<LongTag>(v));
	}

	void LongsTag::insert(const std::shared_ptr<Tag> &v) {
		ListTag::insert(v);
	}

	LongsTag::LongsTag(std::istream &input, ssize_t &counter, IOFormat format) {
		int32_t size;
		switch (format) {
			case BIN:
				size = SingleValuedTag<int32_t>::parseBinaryNumeric(input, counter);
				if (size < 0) {
					throw TagParseException(counter, fmt::format(CONTENT_LEN_NEG, size));
				}
				for (int i = 0; i < size; i++) {
					this->payload.push_back(std::shared_ptr<Tag>(new LongTag(input, counter)));
				}
				break;
			case PRETTY_PRINT:
				throw std::invalid_argument(PARSE_PRETTY);
		}
		this->contentType = NBTP::LONG;
	}
}