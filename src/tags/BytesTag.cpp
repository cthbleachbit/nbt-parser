//
// Created by cth451 on 2020/05/10.
//

#include <tags/ByteTag.h>
#include <boost/format.hpp>
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

	BytesTag::BytesTag(std::istream &input) {
		int32_t size = IntTag::parseInt(input);
		if (size < 0) {
			throw std::runtime_error(CONTENT_LEN_NEG);
		}
		for (int i = 0; i < size; i++) {
			this->payload.push_back(std::shared_ptr<Tag>(new ByteTag(input)));
		}
	}
}

