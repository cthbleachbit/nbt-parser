//
// Created by cth451 on 2020/05/10.
//

#ifndef NBTP_INTSTAG_H
#define NBTP_INTSTAG_H

#include "ListTag.h"

namespace NBTP {
	class IntsTag : public ListTag {
	public:
		/**
		 * @return NBT type byte array 7
		 */
		TagType typeCode() noexcept override;

		/**
		 * This function does nothing
		 * @param type
		 */
		void setContentType(TagType type) noexcept override;

		/**
		 * @return NBT type byte 1
		 */
		TagType getContentType() const noexcept override;

		/**
		 * Insert a literal byte into the byte array
		 * @param v the byte to insert
		 */
		void insert(int8_t v);

		void insert(const std::shared_ptr<Tag> &v) override;

		std::ostream &output(std::ostream &ostream, IOFormat format) override;
		std::ostream &textOutput(std::ostream &ostream, unsigned int indent);
	};
}


#endif //NBTP_INTSTAG_H
