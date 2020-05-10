//
// Created by cth451 on 2020/05/09.
//

#ifndef NBTP_COMPOUNDTAG_H
#define NBTP_COMPOUNDTAG_H

#include "tags/Tag.h"

namespace NBTP {
	/**
	 * A compound tag is a name-Tag hash map
	 */
	class CompoundTag : public ContainerTag {
	public:
		typedef std::unordered_map<std::string, std::shared_ptr<Tag>> Compound;
		typedef std::pair<std::string, std::shared_ptr<Tag>> CompoundElem;
	private:
		Compound payload;
	public:
		/**
		 * @return size of this compound map
		 */
		ssize_t size() override;

		/**
		 * @return NBT type code compound 10
		 */
		TagType typeCode() noexcept override;

		/**
		 * Insert a tag into the compound, will replace existing tag if the key already exists
		 */
		void insert(const std::string &, const std::shared_ptr<Tag> &) noexcept;

		/**
		 * Remove a tag by its key, do nothing if the key doesn't exist
		 */
		void remove(const std::string &) noexcept;

		/**
		 * Check if the key is in there
		 * @return the tag, or null if it doesn't exist
		 */
		std::shared_ptr<Tag> lookup(const std::string &) noexcept;

		std::ostream &output(std::ostream &ostream, IOFormat format) override;

		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) override;

		bool equal(Tag &rhs) override;
	};
}

#endif //NBTP_COMPOUNDTAG_H
