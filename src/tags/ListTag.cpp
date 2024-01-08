//
// Created by cth451 on 2020/05/09.
//

#include <iostream>
#include <optional>

#include "tags/ListTag.h"
#include "libnbtp.h"

namespace NBTP {
	ssize_t ListTag::size() const {
		return this->payload.size();
	}

	void ListTag::setContentType(TagType type) {
		if (this->size() == 0) {
			this->contentType = type;
		} else {
			if (this->contentType != type) {
				throw std::runtime_error(fmt::format(LIST_CHANGE_TYPE, TypeNames[this->contentType]));
			}
		}
	}

	TagType ListTag::getContentType() const noexcept {
		return this->contentType;
	}

	std::ostream &ListTag::nbtOutput(std::ostream &ostream) const {
		// Grab type
		char typeByte = static_cast<char>(this->getContentType());
		// Check element numbers
		if (this->size() > INT32_MAX) {
			throw std::runtime_error(LIST_TOO_LONG);
		}
		// Otherwise we can start writing, type byte, then length, then actual contents
		ostream.write(&typeByte, 1);
		IntTag tmp(this->size());
		tmp.nbtOutput(ostream);
		outputPayloadOnly(ostream, BIN, 0);
		return ostream;
	}

	std::ostream &ListTag::textOutput(std::ostream &ostream, unsigned int indent) const {
		ostream << fmt::format(REPR_LIST, TypeNames[this->getContentType()], this->size()) << std::endl;
		this->outputPayloadOnly(ostream, PRETTY_PRINT, indent);
		return ostream;
	}

	void ListTag::insert(const std::shared_ptr<Tag> &v) {
		if (this->size() == 0) {
			this->contentType = v->typeCode();
		} else {
			if (this->contentType != v->typeCode()) {
				throw ListTypeMismatchException(this->typeCode(), typeCode());
			}
		}
		this->payload.push_back(v);
	}

	ListTag::ListTag(TagType type) noexcept {
		this->contentType = type;
	}

	ListTag::ListTag() noexcept {
		this->contentType = TagType::END;
	}

	bool ListTag::equal(const Tag &rhs) const {
		if (rhs.typeCode() != TagType::LIST) {
			return false;
		}
		const auto &rhs_ref = (const ListTag &) rhs;
		if (rhs_ref.getContentType() != this->getContentType()) {
			return false;
		}
		// Note that using vector operator== won't work, as comparing shared pointers doesn't make sense
		// This thing -> return this->payload == rhs_ref.payload;
		auto l_p = this->payload;
		auto r_p = rhs_ref.payload;
		if (l_p.size() != r_p.size()) {
			return false;
		}

		auto l_itr = l_p.cbegin();
		auto r_itr = r_p.cbegin();
		for (; l_itr != l_p.cend() && r_itr != r_p.cend(); l_itr++, r_itr++) {
			// Make sure we don't overrun the list
			if ((l_itr == l_p.cend()) != (r_itr == r_p.cend())) {
				return false;
			}

			// Using double * to force value comparison instead of pointer comparison
			if (**l_itr != **r_itr) {
				return false;
			}
		}
		return true;
	}

	ListTag::List &ListTag::getPayload() {
		return this->payload;
	}

	ListTag::ListTag(std::istream &input, ssize_t &counter, IOFormat format) {
		std::optional<TagType> typeCode;
		int32_t size;
		switch (format) {
			case BIN:
				// Check content type
				typeCode = readType(input, counter);
				if (!typeCode.has_value() || static_cast<int8_t>(*typeCode) > LONGS ||
				    static_cast<int8_t>(*typeCode) < END) {
					throw TagParseException(counter, fmt::format(INVALID_TYPE, *typeCode));
				}

				// Read in payload length
				size = IntTag::parseBinaryNumeric(input, counter);
				if (size < 0) {
					throw TagParseException(counter, fmt::format(CONTENT_LEN_NEG, size));
				}

				// Check if type and length agree with each other
				if (typeCode == END && size == 0) {
					// This thing is empty
					this->contentType = END;
					return;
				} else if (typeCode == END && size != 0) {
					throw TagParseException(counter, fmt::format(LIST_END_NZ_LEN, size));
				}

				// Otherwise this list has sensible contents:
				this->contentType = *typeCode;
				for (int32_t i = 0; i < size; i++) {
					this->payload.push_back(Tag::parseTag(input, *typeCode, counter));
				}
				break;
			case PRETTY_PRINT:
				throw std::invalid_argument(PARSE_PRETTY);
			default:
				throw std::invalid_argument(PARSE_UNKNOWN_FMT);
		}
	}

	std::ostream &ListTag::outputPayloadOnly(std::ostream &ostream, IOFormat format, unsigned int indent) const {
		switch (format) {
			case PRETTY_PRINT:
				for (const auto &elemItr: this->payload) {
					Tag::indent(ostream, indent + 1);
					elemItr->textOutput(ostream, indent + 1);
				}
				break;
			case BIN:
				for (const auto &elemItr: this->payload) {
					elemItr->output(ostream, BIN);
				}
				break;
		}
		return ostream;
	}

	ListTag::ListTag(const ListTag &tag) noexcept {
		this->contentType = tag.contentType;
		this->payload.clear();
		for (const auto &elemItr: tag.payload) {
			this->payload.push_back(Tag::deepCopy(elemItr));
		}
	}

	ListTag &ListTag::operator=(const ListTag &tag) noexcept {
		if (this != &tag) {
			this->contentType = tag.contentType;
			this->payload.clear();
			for (const auto &elemItr: tag.payload) {
				this->payload.push_back(Tag::deepCopy(elemItr));
			}
		}
		return *this;
	}

	ListTag::ListTag(ListTag &&tag) noexcept {
		this->contentType = tag.contentType;
		this->payload = std::move(tag.payload);
		tag.contentType = TagType::END;
	}

	ListTag &ListTag::operator=(ListTag &&tag) noexcept {
		if (this != &tag) {
			this->contentType = tag.contentType;
			this->payload = std::move(tag.payload);
			tag.contentType = TagType::END;
		}
		return *this;
	}

	std::shared_ptr<Tag> ListTag::remove(const ssize_t index) {
		if (index >= size()) {
			return nullptr;
		}
		auto ret = this->payload[index];
		this->payload.erase(this->payload.begin() + index);
		return ret;
	}
}
