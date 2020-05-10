//
// Created by cth451 on 5/10/20.
//

#ifndef NBTP_LONGSTAG_H
#define NBTP_LONGSTAG_H

#include "ListTag.h"

namespace NBTP {
	class LongsTag : public ListTag {
	public:
		/**
		 * @return NBT type long array 12
		 */
		TagType typeCode() noexcept override;

		/**
		 * This function does nothing
		 * @param type
		 */
		void setContentType(TagType type) noexcept override;

		/**
		 * @return NBT type long 4
		 */
		TagType getContentType() const noexcept override;

		/**
		 * Insert a literal long into the byte array
		 * @param v the long to insert
		 */
		void insert(int64_t v);

		/**
		 * Insert a pre-formed tag
		 * @param v
		 */
		void insert(const std::shared_ptr<Tag> &v) override;

		/**
		 * Deserializing constructor
		 * @param input
		 */
		explicit LongsTag::LongsTag(std::istream &input);

		std::ostream &output(std::ostream &ostream, IOFormat format) override;

		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) override;
	};
}

#endif //NBTP_LONGSTAG_H
