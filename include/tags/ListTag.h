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
		typedef std::vector<std::shared_ptr<Tag>>::iterator ListIterator;
	private:
		/**
		 * The type of tags in this list. Defaults to END for an empty list.
		 */
		TagType contentType;
	protected:
		List payload;
	public:
		/**
		 * @return size of this compound map
		 */
		ssize_t size() override;

		/**
		 * @return NBT type code list 9
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
		virtual void insert(const std::shared_ptr<Tag> &v);

		/**
		 * Get a reference to internals. Note that will return a immutable reference
		 * @return a reference to the vector
		 */
		const List &getPayload();

		/**
		 * Set content type of this list.
		 * If the list has previously no content
		 *     this method will always succeed.
		 * If the list already has sub-tags
		 *     setting a different type than existing one will cause an exception.
		 *
		 * This function will be overridden in one of its subclasses, where it does nothing:
		 *    - Bytes
		 *    - Ints
		 *    - Longs
		 * @param type   the type code to be set
		 */
		virtual void setContentType(TagType type);

		/**
		 * Get content type for this list
		 *
		 * This function will be overridden in one of its subclasses, where it returns a predefined type
		 *    - Bytes
		 *    - Ints
		 *    - Longs
		 * @return the type code
		 */
		virtual TagType getContentType() const noexcept;

		/**
		 * Default constructor
		 */
		ListTag();

		/**
		 * Construct a list with specific type of contents
		 * @param type
		 */
		explicit ListTag(TagType type);

		std::ostream &output(std::ostream &ostream, IOFormat format) override;

		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) override;

		bool equal(Tag &rhs) override;
	};
}


#endif //NBTP_LISTTAG_H
