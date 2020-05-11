//
// Created by cth451 on 2020/05/09.
//

#include "libnbtp.h"
#include <boost/format.hpp>
#include <memory>

namespace NBTP {

	TagType CompoundTag::typeCode() noexcept {
		return TagType::COMPOUND;
	}

	ssize_t CompoundTag::size() {
		return this->payload.size();
	}

	std::ostream &CompoundTag::output(std::ostream &ostream, IOFormat format) {
		switch (format) {
			case PRETTY_PRINT:
				this->textOutput(ostream, 0);
				break;
			case BIN:
				// For each tag in there
				//    - dump its type
				//    - dump its name
				//    - dump itself
				// Add END
				for (CompoundElem itr : this->payload) {
					char typeByte = static_cast<char>(itr.second->typeCode());
					ostream.write(&typeByte, 1);
					StringTag::nbtOutput(ostream, itr.first);
					itr.second->output(ostream, BIN);
				}
				char endByte = static_cast<char>(TagType::END);
				ostream.write(&endByte, 1);
				break;
		}
		return ostream;
	}

	std::ostream &CompoundTag::textOutput(std::ostream &ostream, unsigned int indent) {
		ostream << boost::format("Compound with %i elements:") % this->size() << std::endl;
		for (const auto &elemItr : this->payload) {
			Tag::indent(ostream, indent + 1);
			ostream << elemItr.first << ": ";
			elemItr.second->textOutput(ostream, indent + 1);
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

	void CompoundTag::insert(const std::string &k, const std::shared_ptr<Tag> &v) noexcept {
		this->payload[k] = v;
	}

	void CompoundTag::remove(const std::string &k) noexcept {
		this->payload.erase(k);
	}

	bool CompoundTag::equal(Tag &rhs) {
		if (rhs.typeCode() != TagType::COMPOUND) {
			return false;
		}
		return this->payload == ((CompoundTag &) rhs).payload;
	}

	const CompoundTag::Compound &CompoundTag::getPayload() const {
		return this->payload;
	}

	CompoundTag::CompoundTag(std::istream &input, ssize_t &counter) {
		// Read in type
		TagType typeCode;
		while ((typeCode = readType(input, counter)) != END) {
			// Read in name, this can be an empty string
			std::string name = StringTag::parseString(input, counter);
			// Construct and add tag
			this->payload[name] = Tag::parseTag(input, typeCode, counter);
		}
		// Parse tag payload
	}
}
