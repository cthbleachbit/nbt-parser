//
// Created by cth451 on 2020/05/09.
//

#include "CompoundTag.h"
#include <boost/format.hpp>

namespace NBTP {

	int8_t CompoundTag::typeCode() noexcept {
		return TagType::COMPOUND;
	}

	ssize_t CompoundTag::size() {
		return this->payload.size();
	}

	std::ostream &CompoundTag::output(std::ostream &ostream, IOFormat format) {
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

	std::ostream &CompoundTag::textOutput(std::ostream &ostream, unsigned int indent) {
		Tag::indent(ostream, indent);
		ostream << boost::format("Compound with %i elements:") % this->size() << std::endl;
		for (const auto& elemItr : this->payload) {
			Tag::indent(ostream, indent + 1);
			ostream << elemItr.first << ": ";
			elemItr.second->textOutput(ostream, indent + 1) << std::endl;
		}
		return ostream;
	}

	std::shared_ptr<Tag> CompoundTag::lookup(const std::string &k) noexcept {
		auto itr = this->payload.find(k);
		if (itr == this->payload.end()) {
			return std::shared_ptr<Tag>(nullptr);
		} else {
			return itr->second;
		}
	}

	void CompoundTag::insert(const std::string &k, const std::shared_ptr<Tag>& v) noexcept {\
		this->payload.emplace(CompoundElem(k, v));
	}

	void CompoundTag::remove(const std::string &k) noexcept {
		this->payload.erase(k);
	}
}
