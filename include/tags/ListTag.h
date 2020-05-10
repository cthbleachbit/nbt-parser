//
// Created by cth451 on 2020/05/09.
//

#ifndef NBTP_LISTTAG_H
#define NBTP_LISTTAG_H

#include "Tag.h"

namespace NBTP {
	/**
	 * A list tag is a vector of tags of the same type
	 */
	class ListTag : public ContainerTag {
	public:
		typedef std::vector<std::shared_ptr<Tag>> List;
	private:
		List payload;
		/**
		 * The type of tags in this list. Defaults to END for an empty list.
		 */
		int8_t contentType;
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
		 * @brief Insert a tag into the list.
		 *
		 * If the list is previously empty:
		 *    - The tag will be added to the list.
		 *    - The contentType field will be updated to match this tag.
		 * If the list has existing contents:
		 *    - If the tag to add has the same type as contentType, the tag will be added
		 *    - If the tag to add has a different type, an exception will be thrown
		 *
		 * @param v the tag to add
		 */
		void insert(const std::shared_ptr<Tag>& v);

		/**
		 * Set content type of this list.
		 * If the list has previously no content
		 *     this method will always succeed.
		 * If the list already has sub-tags
		 *     setting a different type than existing one will cause an exception.
		 *
		 * @param type   the type code to be set
		 */
		void setContentType(int8_t type);

		/**
		 * Get content type for this list
		 * @return the type code
		 */
		int8_t getContentType() const noexcept;

		/**
		 * Default constructor
		 */
		ListTag();

		/**
		 * Construct a list with specific type of contents
		 * @param type
		 */
		ListTag(TagType type);

		std::ostream &output(std::ostream &ostream, IOFormat format);
		std::ostream &textOutput(std::ostream &ostream, unsigned int indent);
	};
}



#endif //NBTP_LISTTAG_H
