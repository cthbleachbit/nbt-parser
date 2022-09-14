//
// Created by cth451 on 2020/05/10.
//

#include "tags/IntTag.h"
#include "tags/IntsTag.h"
#include "Logging.h"

namespace NBTP {
	void IntsTag::insert(int32_t v) {
		ListTag::insert(std::make_shared<IntTag>(v));
	}

	void IntsTag::insert(const std::shared_ptr<Tag> &v) {
		ListTag::insert(v);
	}

	IntsTag::IntsTag(std::istream &input, ssize_t &counter, IOFormat format) {
		int32_t size;
		switch (format) {
			case BIN:
				size = IntTag::parseInt(input, counter);
				if (size < 0) {
					Logging::error(fmt::format(CONTENT_LEN_NEG, size), counter);
				}
				for (int i = 0; i < size; i++) {
					this->payload.push_back(std::shared_ptr<Tag>(new IntTag(input, counter)));
				}
				break;
			case PRETTY_PRINT:
				Logging::error(PARSE_PRETTY, counter);
		}
		this->contentType = NBTP::INT;
	}
}