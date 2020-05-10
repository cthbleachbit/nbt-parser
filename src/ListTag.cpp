//
// Created by cth451 on 2020/05/09.
//

#include <boost/format.hpp>
#include <iostream>
#include "ListTag.h"

namespace NBTP {
	TagType ListTag::typeCode() noexcept {
		return TagType::LIST;
	}

	ssize_t ListTag::size() {
		return this->payload.size();
	}

	void ListTag::setContentType(int8_t type) {
		if (this->size() == 0) {
			this->contentType = type;
		} else {
			if (this->contentType != type) {
				throw std::runtime_error(LIST_CHANGE_TYPE);
			}
		}
	}

	int8_t ListTag::getContentType() const noexcept {
		return this->contentType;
	}

	std::ostream &ListTag::output(std::ostream &ostream, IOFormat format) {
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

	std::ostream &ListTag::textOutput(std::ostream &ostream, unsigned int indent) {
		Tag::indent(ostream, indent);
		std::string typeString = TypeNames[this->getContentType()];
		ostream << boost::format("List of type %s with %i elements:") % typeString % this->size() << std::endl;
		for (const auto& elemItr : this->payload) {
			Tag::indent(ostream, indent + 1);
			elemItr->textOutput(ostream, indent + 1) << std::endl;
		}
		return ostream;
	}

	void ListTag::insert(const std::shared_ptr<Tag>& v) {
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
}