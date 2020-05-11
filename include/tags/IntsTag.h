//
// Created by cth451 on 2020/05/10.
//

#ifndef NBTP_INTSTAG_H
#define NBTP_INTSTAG_H

#include "ListTag.h"

namespace NBTP {
	class IntsTag : public TypedListTag {
	public:
		/**
		 * @return NBT type int array 11
		 */
		TagType typeCode() noexcept override;

		/**
		 * @return NBT type int 3
		 */
		TagType getContentType() const noexcept override;

		/**
		 * Insert a literal integer into the byte array
		 * @param v the integer to insert
		 */
		void insert(int32_t v);

		/**
		 * Insert a pre-formed tag
		 * @param v
		 */
		void insert(const std::shared_ptr<Tag> &v) override;

		/**
		 * Deserializing constructor
		 * @param input
		 */
		explicit IntsTag(std::istream &input, ssize_t &counter);
	};
}


#endif //NBTP_INTSTAG_H
