//
// Created by cth451 on 5/10/20.
//

#ifndef NBTP_STRINGTAG_H
#define NBTP_STRINGTAG_H

#include "Tag.h"

namespace NBTP {
	class StringTag : public ContainerTag {
	private:
		std::string payload;
	public:
		/**
		 * @return length of the string
		 */
		ssize_t size() override;

		/**
		 * @return NBT type code string
		 */
		TagType typeCode() noexcept override;

		std::ostream &output(std::ostream &ostream, IOFormat format) override;
		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) override;

		/**
		 * @return an immutable reference to the string
		 */
		const std::string& getPayload() const;

		/**
		 * Set the string content of this tag
		 * @param s
		 */
		void setPayload(std::string s);

		/**
		 * Construct a string tag from a c++ string
		 * @param s
		 */
		explicit StringTag(std::string s);

		/**
		 * Default constructor
		 */
		StringTag();

		bool equal(Tag &rhs) override;
	};
}


#endif //NBTP_STRINGTAG_H
