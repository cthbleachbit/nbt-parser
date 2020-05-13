//
// Created by cth451 on 2020/05/10.
//

#include <boost/format.hpp>
#include <ostream>
#include "tags/IntTag.h"
#include "tags/IntsTag.h"
#include "TagIO.h"
#include "Logging.h"

namespace NBTP {
	TagType IntsTag::typeCode() const noexcept {
		return TagType::INTS;
	}

	void IntsTag::insert(int32_t v) {
		ListTag::insert(std::make_shared<IntTag>(v));
	}

	TagType IntsTag::getContentType() const noexcept {
		return TagType::BYTE;
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
					Logging::error(CONTENT_LEN_NEG, counter);
				}
				for (int i = 0; i < size; i++) {
					this->payload.push_back(std::shared_ptr<Tag>(new IntTag(input, counter)));
				}
				break;
			case PRETTY_PRINT:
				Logging::error(PARSE_PRETTY, counter);
		}
	}
}