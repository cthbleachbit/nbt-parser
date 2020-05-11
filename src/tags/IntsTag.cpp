//
// Created by cth451 on 2020/05/10.
//

#include <boost/format.hpp>
#include <ostream>
#include "tags/IntTag.h"
#include "tags/IntsTag.h"

namespace NBTP {
	TagType IntsTag::typeCode() noexcept {
		return TagType::INTS;
	}

	void IntsTag::insert(int32_t v) {
		ListTag::insert(std::make_shared<IntTag>(v));
	}

	void IntsTag::setContentType(TagType type) noexcept {}

	TagType IntsTag::getContentType() const noexcept {
		return TagType::BYTE;
	}

	std::ostream &IntsTag::output(std::ostream &ostream, IOFormat format) {
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

	std::ostream &IntsTag::textOutput(std::ostream &ostream, unsigned int indent) {
		ostream << boost::format("%s with %i elements:") % TypeNames[this->typeCode()] % this->size() << std::endl;
		outputPayloadOnly(ostream, PRETTY_PRINT, indent);
		return ostream;
	}

	void IntsTag::insert(const std::shared_ptr<Tag> &v) {
		ListTag::insert(v);
	}

	IntsTag::IntsTag(std::istream &input) {
		int32_t size = IntTag::parseInt(input);
		if (size < 0) {
			throw std::runtime_error(CONTENT_LEN_NEG);
		}
		for (int i = 0; i < size; i++) {
			this->payload.push_back(std::shared_ptr<Tag>(new IntTag(input)));
		}
	}
}