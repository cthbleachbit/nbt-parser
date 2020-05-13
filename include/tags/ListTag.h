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
		ssize_t size() const override;

		/**
		 * @return NBT type code list 9
		 */
		TagType typeCode() const noexcept override;

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
		const List &getPayload() const;

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

		/**
		 * Deserializing constructor that reads uncompressed NBT data
		 * @param input
		 * @param counter   progress tracking
		 */
		ListTag(std::istream &input, ssize_t &counter) : ListTag(input, counter, BIN) {};

		/**
		 * Deserialize constructor with a format specified
		 * @param input
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 * @param format       specifies the format of incoming data
		 */
		ListTag(std::istream &input, ssize_t &counter, IOFormat format);

		std::ostream &output(std::ostream &ostream, IOFormat format) const override;

		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) const override;

		bool equal(Tag &rhs) const override;
	protected:
		/**
		 * This output function only handles the payload vector itself w/o writing the type and length.
		 *
		 * All the fixed-type lists, i.e. ints, bytes and longs will use this function when serializing.
		 * @param ostream
		 * @param indent
		 * @return
		 */
		std::ostream &outputPayloadOnly(std::ostream &ostream, IOFormat, unsigned int indent) const;
	};

	/**
	 * A prototype class for fixed type lists, namely Int, Byte, and Float array
	 */
	class TypedListTag : public ListTag {
	public:
		/**
		 * For a fixed size list, this function does nothing as the type cannot be changed
		 * @param type
		 */
		void setContentType(TagType type) noexcept override;

		std::ostream &output(std::ostream &ostream, IOFormat format) const override;

		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) const override;
	};
}


#endif //NBTP_LISTTAG_H
