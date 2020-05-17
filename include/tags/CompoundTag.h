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
		ssize_t size() const override;

		/**
		 * @return NBT type code compound 10
		 */
		TagType typeCode() const noexcept override;

		/**
		 * Get a reference to internals. Note that will return a immutable reference
		 * @return a reference to the vector
		 */
		Compound &getPayload();

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

		/**
		 * Deserializer constructor for decompressed NBT input
		 * @param input        stream to read raw bytes from
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 */
		CompoundTag(std::istream &input, ssize_t &counter) : CompoundTag(input, counter, BIN) {};

		/**
		 * Deserializer constructor with specified format
		 * @param input
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 * @param format       specifies the format of incoming data
		 */
		CompoundTag(std::istream &input, ssize_t &counter, IOFormat format);

		/**
		 * Default constructor, gives an empty Compound Tag
		 */
		CompoundTag() = default;

		std::ostream &output(std::ostream &ostream, IOFormat format) const override;

		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) const override;

		bool equal(Tag &rhs) const override;

		/**
		 * Helper function for compound equals
		 * @param lhs
		 * @param rhs
		 * @return
		 */
		static bool equal(const Compound &lhs, const Compound &rhs);

		virtual ~CompoundTag() = default;
	};
}

#endif //NBTP_COMPOUNDTAG_H
