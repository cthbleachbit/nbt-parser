//
// Created by cth451 on 2020/05/10.
//

#include "tags/BytesTag.h"
#include "tags/SingleValuedTag.h"
#include "TagIO.h"

namespace NBTP {
	void BytesTag::insert(int8_t v) {
		ListTag::insert(std::make_shared<ByteTag>(v));
	}

	void BytesTag::insert(const std::shared_ptr<Tag> &v) {
		ListTag::insert(v);
	}

	BytesTag::BytesTag(std::istream &input, ssize_t &counter, IOFormat format) {
		int32_t size;
		switch (format) {
			case BIN:
				size = IntTag::parseBinaryNumeric(input, counter);
				if (size < 0) {
					throw TagParseException(counter, fmt::format(CONTENT_LEN_NEG, size));
				}
				for (int i = 0; i < size; i++) {
					this->payload.push_back(std::shared_ptr<Tag>(new ByteTag(input, counter)));
				}
				break;
			case PRETTY_PRINT:
				throw std::invalid_argument(PARSE_PRETTY);
				break;
		}
		this->contentType = NBTP::BYTE;
	}
}

