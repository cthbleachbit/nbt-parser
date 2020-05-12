//
// Created by cth451 on 2020/05/10.
//

#include <tags/ByteTag.h>
#include <boost/format.hpp>
#include "TagIO.h"
#include "tags/BytesTag.h"
#include "tags/IntTag.h"

namespace NBTP {
	TagType BytesTag::typeCode() noexcept {
		return TagType::BYTES;
	}

	void BytesTag::insert(int8_t v) {
		ListTag::insert(std::make_shared<ByteTag>(v));
	}

	TagType BytesTag::getContentType() const noexcept {
		return TagType::BYTE;
	}

	void BytesTag::insert(const std::shared_ptr<Tag> &v) {
		ListTag::insert(v);
	}

	BytesTag::BytesTag(std::istream &input, ssize_t &counter, IOFormat format) {
		int32_t size;
		switch (format) {
			case BIN:
				size = IntTag::parseInt(input, counter);
				if (size < 0) {
					TagIO::error(CONTENT_LEN_NEG, counter);
				}
				for (int i = 0; i < size; i++) {
					this->payload.push_back(std::shared_ptr<Tag>(new ByteTag(input, counter)));
				}
				break;
			case PRETTY_PRINT:
				TagIO::error(PARSE_PRETTY, counter);
				break;
		}
	}
}

