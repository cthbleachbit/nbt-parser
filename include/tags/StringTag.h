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

		/**
		 * Deserializer constructor
		 * @param input
		 */
		explicit StringTag(std::istream &input);

		/**
		 * Static helper function for getting a string from NBT
		 * This will consume two bytes, to read in length of the string X, then the following X bytes
		 * for the string itself.
		 *
		 * @param input
		 * @return
		 */
		static std::string parseString(std::istream &input);

		bool equal(Tag &rhs) override;
	};
}


#endif //NBTP_STRINGTAG_H
