//
// Created by cth451 on 2020/05/09.
//

#include "libnbtp.h"
#include "Logging.h"
#include "tags/CompoundTag.h"

#include <memory>
#include <iostream>

namespace NBTP {

	ssize_t CompoundTag::size() const {
		return this->payload.size();
	}

	std::ostream &CompoundTag::nbtOutput(std::ostream &ostream) const {
		for (const auto &itr: this->payload) {
			char typeByte = static_cast<char>(itr.second->typeCode());
			ostream.write(&typeByte, 1);
			StringTag tmp(itr.first);
			tmp.nbtOutput(ostream);
			itr.second->output(ostream, BIN);
		}
		char endByte = static_cast<char>(TagType::END);
		ostream.write(&endByte, 1);
		return ostream;
	}

	std::ostream &CompoundTag::textOutput(std::ostream &ostream, unsigned int indent) const {
		ostream << fmt::format(REPR_COMPOUND, this->size()) << std::endl;
		for (const auto &elemItr: this->payload) {
			Tag::indent(ostream, indent + 1);
			ostream << elemItr.first << ": ";
			elemItr.second->textOutput(ostream, indent + 1);
		}
		return ostream;
	}

	std::shared_ptr<Tag> CompoundTag::lookup(const std::string &k) noexcept {
		auto itr = this->payload.find(k);
		if (itr == this->payload.end()) {
			return nullptr;
		} else {
			return itr->second;
		}
	}

	void CompoundTag::insert(const std::string &k, const std::shared_ptr<Tag> &v) noexcept {
		this->payload[k] = v;
	}

	void CompoundTag::remove(const std::string &k) noexcept {
		this->payload.erase(k);
	}

	bool CompoundTag::equal(const Tag &rhs) const {
		if (rhs.typeCode() != TagType::COMPOUND) {
			return false;
		}
		return equal(this->payload, ((CompoundTag &) rhs).getPayload());
	}

	CompoundTag::Compound &CompoundTag::getPayload() {
		return this->payload;
	}

	bool CompoundTag::equal(const CompoundTag::Compound &lhs, const CompoundTag::Compound &rhs) {
		if (lhs.size() != rhs.size()) {
			return false;
		}
		for (const auto &l: lhs) {
			auto r = rhs.find(l.first);
			if (r == rhs.end()) {
				return false;
			}
			if (*(l.second) != *(r->second)) {
				return false;
			}
		}
		for (const auto &r: rhs) {
			auto l = lhs.find(r.first);
			if (l == lhs.end()) {
				return false;
			}
			if (*(r.second) != *(l->second)) {
				return false;
			}
		}
		return true;
	}

	CompoundTag::CompoundTag(std::istream &input, ssize_t &counter, IOFormat format) {
		switch (format) {
			case BIN:
				// Read in type
				TagType typeCode;
				while ((typeCode = readType(input, counter)) != END) {
					// Read in name, this can be an empty string
					std::string name = StringTag::parseString(input, counter);
					// Construct and add tag
					this->payload[name] = Tag::parseTag(input, typeCode, counter);
				}
				break;
			case PRETTY_PRINT:
				Logging::error(PARSE_PRETTY, counter);
				break;
		}
	}

	// COPY
	CompoundTag::CompoundTag(const CompoundTag &tag) noexcept {
		this->payload.clear();
		for (const auto &elemItr: tag.payload) {
			std::shared_ptr<Tag> from = Tag::deepCopy(elemItr.second);
			this->payload[elemItr.first] = from;
		}
	}

	// COPY
	CompoundTag &CompoundTag::operator=(const CompoundTag &tag) noexcept {
		if (&tag != this) {
			this->payload.clear();
			for (const auto &elemItr: tag.payload) {
				std::shared_ptr<Tag> from = Tag::deepCopy(elemItr.second);
				this->payload[elemItr.first] = from;
			}
		}
		return *this;
	}

	// MOVE
	CompoundTag::CompoundTag(CompoundTag &&tag) noexcept {
		this->payload = tag.payload;
	}

	// MOVE
	CompoundTag &CompoundTag::operator=(CompoundTag &&tag) noexcept {
		if (&tag != this) {
			this->payload = tag.payload;
		}
		return *this;
	}
}
