//
// Created by cth451 on 2020/05/09.
//

#include <boost/format.hpp>
#include <iostream>
#include <tags/ListTag.h>

#include "libnbtp.h"

namespace NBTP {
	TagType ListTag::typeCode() noexcept {
		return TagType::LIST;
	}

	ssize_t ListTag::size() {
		return this->payload.size();
	}

	void ListTag::setContentType(TagType type) {
		if (this->size() == 0) {
			this->contentType = type;
		} else {
			if (this->contentType != type) {
				throw std::runtime_error(LIST_CHANGE_TYPE);
			}
		}
	}

	TagType ListTag::getContentType() const noexcept {
		return this->contentType;
	}

	std::ostream &ListTag::output(std::ostream &ostream, IOFormat format) {
		switch (format) {
			case PRETTY_PRINT:
				this->textOutput(ostream, 0);
				break;
			case BIN:
				// Grab type
				char typeByte = static_cast<char>(this->getContentType());
				// Check element numbers
				if (this->size() > INT32_MAX) {
					throw std::runtime_error(LIST_TOO_LONG);
				}
				// Otherwise we can start writing, type byte, then length, then actual contents
				ostream.write(&typeByte, 1);
				IntTag::nbtOutput(ostream, this->size());
				outputPayloadOnly(ostream, BIN, 0);
				break;
		}
		return ostream;
	}

	std::ostream &ListTag::textOutput(std::ostream &ostream, unsigned int indent) {
		std::string typeString = TypeNames[this->getContentType()];
		ostream << boost::format("List of type %s with %i elements:") % typeString % this->size() << std::endl;
		this->outputPayloadOnly(ostream, PRETTY_PRINT, indent);
		return ostream;
	}

	void ListTag::insert(const std::shared_ptr<Tag> &v) {
		if (this->size() == 0) {
			this->contentType = v->typeCode();
		} else {
			if (this->contentType != v->typeCode()) {
				throw std::runtime_error(LIST_ADD_UNMATCH);
			}
		}
		this->payload.push_back(v);
	}

	ListTag::ListTag(TagType type) {
		this->contentType = type;
	}

	ListTag::ListTag() {
		this->contentType = TagType::END;
	}

	bool ListTag::equal(Tag &rhs) {
		if (rhs.typeCode() != TagType::LIST) {
			return false;
		}
		auto &rhs_ref = (ListTag &) rhs;
		if (rhs_ref.getContentType() != this->getContentType()) {
			return false;
		}
		return this->payload == rhs_ref.payload;
	}

	const ListTag::List &ListTag::getPayload() const {
		return this->payload;
	}

	ListTag::ListTag(std::istream &input) {
		// Check content type
		TagType typeCode = readType(input);
		if (static_cast<int8_t>(typeCode) > LONGS || static_cast<int8_t>(typeCode) < END) {
			throw std::runtime_error(INVALID_TYPE);
		}
		// Read in payload length
		int32_t size = IntTag::parseInt(input);
		if (size < 0) {
			throw std::runtime_error(CONTENT_LEN_NEG);
		}

		// Check if type and length agree with each other
		if (typeCode == END && size == 0) {
			// This thing is empty
			this->contentType = END;
			return;
		} else if (typeCode == END && size != 0) {
			throw std::runtime_error(LIST_END_NZ_LEN);
		}

		// Otherwise this list has sensible contents:
		this->contentType = typeCode;
		for (int32_t i = 0; i < size; i++) {
			this->payload.push_back(Tag::parseTag(input, typeCode));
		}
	}

	std::ostream &ListTag::outputPayloadOnly(std::ostream &ostream, IOFormat format, unsigned int indent) {
		switch (format) {
			case PRETTY_PRINT:
				for (const auto &elemItr : this->payload) {
					Tag::indent(ostream, indent + 1);
					elemItr->textOutput(ostream, indent + 1);
				}
				break;
			case BIN:
				for (const auto &elemItr : this->payload) {
					elemItr->output(ostream, BIN);
				}
				break;
		}
		return ostream;
	}
}