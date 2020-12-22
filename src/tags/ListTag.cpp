//
// Created by cth451 on 2020/05/09.
//

#include <iostream>
#include <tags/ListTag.h>
#include <climits>

#include "libnbtp.h"
#include "Logging.h"

namespace NBTP {
	TagType ListTag::typeCode() const noexcept {
		return TagType::LIST;
	}

	ssize_t ListTag::size() const {
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

	std::ostream &ListTag::output(std::ostream &ostream, IOFormat format) const {
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
				IntTag::nbtOutput(ostream, static_cast<IntTag::V>(this->size()));
				outputPayloadOnly(ostream, BIN, 0);
				break;
		}
		return ostream;
	}

	std::ostream &ListTag::textOutput(std::ostream &ostream, unsigned int indent) const {
		std::string typeString = TypeNames[this->getContentType()];
		char *message = new char[LINE_MAX];
		snprintf(message, LINE_MAX - 1, "List of type %s with %li elements:", typeString.c_str(), this->size());
		ostream << message << std::endl;
		delete[] message;
		this->outputPayloadOnly(ostream, PRETTY_PRINT, indent);
		return ostream;
	}

	void ListTag::insert(const std::shared_ptr<Tag> &v) {
		if (this->size() == 0) {
			this->contentType = v->typeCode();
		} else {
			if (this->contentType != v->typeCode()) {
				throw ListTypeUnmatchException(this->typeCode(), typeCode());
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

	bool ListTag::equal(Tag &rhs) const {
		if (rhs.typeCode() != TagType::LIST) {
			return false;
		}
		auto &rhs_ref = (ListTag &) rhs;
		if (rhs_ref.getContentType() != this->getContentType()) {
			return false;
		}
		// Note that using vector operator== won't work, as comparing shared pointers doesn't make sense
		// This thing -> return this->payload == rhs_ref.payload;
		auto l_p = this->payload;
		auto r_p = rhs_ref.payload;
		if (l_p.size() == r_p.size()) {
			return false;
		}

		auto l_itr = l_p.cbegin();
		auto r_itr = r_p.cbegin();
		for (; l_itr != l_p.end() && r_itr != r_p.end(); l_itr++, r_itr++) {
			if (*l_itr != *r_itr) {
				return false;
			}
		}
		return true;
	}

	ListTag::List &ListTag::getPayload() {
		return this->payload;
	}

	ListTag::ListTag(std::istream &input, ssize_t &counter, IOFormat format) {
		TagType typeCode;
		int32_t size;
		switch (format) {
			case BIN:
				// Check content type
				typeCode = readType(input, counter);
				if (static_cast<int8_t>(typeCode) > LONGS || static_cast<int8_t>(typeCode) < END) {
					Logging::error(INVALID_TYPE, counter);
				}

				// Read in payload length
				size = IntTag::parseInt(input, counter);
				if (size < 0) {
					Logging::error(CONTENT_LEN_NEG, counter);
				}

				// Check if type and length agree with each other
				if (typeCode == END && size == 0) {
					// This thing is empty
					this->contentType = END;
					return;
				} else if (typeCode == END && size != 0) {
					Logging::error(LIST_END_NZ_LEN, counter);
				}

				// Otherwise this list has sensible contents:
				this->contentType = typeCode;
				for (int32_t i = 0; i < size; i++) {
					this->payload.push_back(Tag::parseTag(input, typeCode, counter));
				}
				break;
			case PRETTY_PRINT:
				Logging::error(PARSE_PRETTY, counter);
				break;
		}
	}

	std::ostream &ListTag::outputPayloadOnly(std::ostream &ostream, IOFormat format, unsigned int indent) const {
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

	ListTag::ListTag(const ListTag &tag) noexcept {
		this->contentType = tag.contentType;
		this->payload.clear();
		for (const auto &elemItr : tag.payload) {
			this->payload.push_back(Tag::deepCopy(elemItr));
		}
	}

	ListTag &ListTag::operator=(const ListTag &tag) noexcept {
		if (this != &tag) {
			this->contentType = tag.contentType;
			this->payload.clear();
			for (const auto &elemItr : tag.payload) {
				this->payload.push_back(Tag::deepCopy(elemItr));
			}
		}
		return *this;
	}

	ListTag::ListTag(ListTag &&tag) noexcept {
		this->contentType = tag.contentType;
		this->payload = tag.payload;
	}

	ListTag &ListTag::operator=(ListTag &&tag) noexcept {
		if (this != &tag) {
			this->contentType = tag.contentType;
			this->payload = tag.payload;
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

	void TypedListTag::setContentType(TagType type) noexcept {}

	std::ostream &TypedListTag::output(std::ostream &ostream, IOFormat format) const {
		switch (format) {
			case PRETTY_PRINT:
				this->textOutput(ostream, 0);
				break;
			case BIN:
				// Do size sanity checking
				if (this->size() > INT32_MAX) {
					throw std::runtime_error(LIST_TOO_LONG);
				}
				IntTag::nbtOutput(ostream, static_cast<IntTag::V>(this->size()));
				outputPayloadOnly(ostream, BIN, 0);
				break;
		}
		return ostream;
	}

	std::ostream &TypedListTag::textOutput(std::ostream &ostream, unsigned int indent) const {
		char *message = new char[LINE_MAX];
		snprintf(message, LINE_MAX - 1, "%s with %li elements:", TypeNames[this->typeCode()].c_str(), this->size());
		ostream << message << std::endl;
		delete[] message;
		outputPayloadOnly(ostream, PRETTY_PRINT, indent);
		return ostream;
	}
}