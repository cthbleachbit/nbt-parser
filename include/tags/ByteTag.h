//
// Created by cth451 on 2020/05/09.
//

#ifndef NBTP_BYTETAG_H
#define NBTP_BYTETAG_H

#include "Tag.h"

namespace NBTP {
	class ByteTag : public Tag {
	public:
		typedef int8_t V;
	private:
		V payload;
	public:
		/**
		 * @return NBT type code compound 1
		 */
		TagType typeCode() noexcept override;

		std::ostream &output(std::ostream &ostream, IOFormat format) override;

		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) override;

		V getPayload() const;

		explicit ByteTag(V value);

		/**
		 * Deserializer constructor
		 * @param input stream to read a single byte
		 */
		explicit ByteTag(std::istream &input, ssize_t &counter);

		/**
		 * Helper function to write a single byte to ostream
		 * @param ostream
		 * @param value
		 * @return
		 */
		static std::ostream &nbtOutput(std::ostream &ostream, V value);

		bool equal(Tag &rhs) override;
	};
}

#endif //NBTP_BYTETAG_H
