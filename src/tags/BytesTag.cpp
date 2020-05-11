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

	void BytesTag::setContentType(TagType type) noexcept {}

	TagType BytesTag::getContentType() const noexcept {
		return TagType::BYTE;
	}

	std::ostream &BytesTag::output(std::ostream &ostream, IOFormat format) {
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

	std::ostream &BytesTag::textOutput(std::ostream &ostream, unsigned int indent) {
		ostream << boost::format("%s with %i elements:") % TypeNames[this->typeCode()] % this->size() << std::endl;
		outputPayloadOnly(ostream, PRETTY_PRINT, indent);
		return ostream;
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

