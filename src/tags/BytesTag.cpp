//
// Created by cth451 on 2020/05/10.
//

#include <tags/ByteTag.h>
#include <boost/format.hpp>
#include "tags/BytesTag.h"

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
			case TEXT:
				this->textOutput(ostream, 0);
				break;
			case BIN:
				// TODO
				break;
		}
		return ostream;
	}

	std::ostream &BytesTag::textOutput(std::ostream &ostream, unsigned int indent) {
		Tag::indent(ostream, indent);
		ostream << boost::format("Byte array with %i elements:") % this->size() << std::endl;
		for (const auto &elemItr : this->payload) {
			Tag::indent(ostream, indent + 1);
			elemItr->textOutput(ostream, indent + 1) << std::endl;
		}
		return ostream;
	}

	void BytesTag::insert(const std::shared_ptr<Tag> &v) {
		ListTag::insert(v);
	}
}

