//
// Created by cth451 on 2020/05/09.
//

#ifndef NBTP_LISTTAG_H
#define NBTP_LISTTAG_H

#include <vector>
#include "Tag.h"
#include "SingleValuedTag.h"

namespace NBTP {
	/**
	 * A list tag is a vector of tags of the same type
	 */
	class ListTag : public ContainerTag {
	public:
		typedef std::vector<std::shared_ptr<Tag>> List;
		typedef std::vector<std::shared_ptr<Tag>>::iterator ListIterator;

	protected:
		/**
		 * Content stpred in this list
		 */
		List payload;

		/**
		 * The type of tags in this list. Defaults to END for an empty list.
		 */
		TagType contentType;

	public:
		/**
		 * @return size of this compound map
		 */
		ssize_t size() const override;

		/**
		 * @return NBT type code list 9
		 */
		constexpr TagType typeCode() const noexcept override { return LIST; }

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
		 * Remove a element from the list
		 * @param index   index of the tag to remove
		 * @return the tag removed or null if the index is out of bound
		 */
		virtual std::shared_ptr<Tag> remove(const ssize_t index);

		/**
		 * Get a reference to internals. Note that will return a immutable reference
		 * @return a reference to the vector
		 */
		List &getPayload();

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
		 * Default constructor, this gives you an empty list with unspecified type.
		 */
		ListTag() noexcept;

		/**
		 * Construct a list with specific type of contents
		 * @param type
		 */
		explicit ListTag(TagType type) noexcept;

		/**
		 * Copy constructor: Do a deep copy.
		 * @param tag  tag to be copied
		 */
		ListTag(const ListTag &tag) noexcept;

		/**
		 * Copy assignment
		 * @param tag  list tag to copy from
		 * @return
		 */
		ListTag &operator=(const ListTag &tag) noexcept;

		/**
		 * Move constructor
		 * @param tag tag to move from
		 */
		ListTag(ListTag &&tag) noexcept;

		/**
		 * Move assignment
		 * @param tag  list tag to move from
		 * @return  moved tag
		 */
		ListTag &operator=(ListTag &&tag) noexcept;

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
		 *
		 * @throw NBTP::TagParseException if array length is negative
		 * @throw std::ios_base::failure  if I/O error has occurred
		 * @throw std::invalid_argument   if an invalid format is specified
		 */
		ListTag(std::istream &input, ssize_t &counter, IOFormat format);

		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) const override;

		std::ostream &nbtOutput(std::ostream &ostream) const override;

		bool equal(const Tag &rhs) const override;

		~ListTag() override = default;

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
	 * A prototype class for fixed type lists.
	 * 3 types are currently in use by Minecraft: Ints, Bytes, and Longs, implemented as typedefs from this template.
	 *
	 * @tparam P    primitive type in C/C++ for tags stored here
	 * @tparam T    NBT tag type for tags stored here
	 * @tparam L    NBT tag type for the list itself
	 *
	 * Note: SingleValuedTag<P, T> should be a tag type that is typedef'd in SingleValueTags.
	 */
	template<typename P, TagType T, TagType L>
	class TypedListTag : public ListTag {
		typedef SingleValuedTag<P, T> ContentTag;
	public: /* Content Type information */
		/**
		 * For a fixed size list, this function does nothing as the type cannot be changed
		 * @param type
		 */
		void setContentType(TagType type) noexcept override {}

		/**
		 * @return NBT type for the list itself
		 */
		constexpr TagType typeCode() const noexcept override { return L; }

		/**
		 * @return NBT type for tags stored within
		 */
		constexpr TagType getContentType() const noexcept override { return T; }

	public: /* Content manipulation */
		/**
		 * Insert a literal value into the byte array. Type of argument must match fixed content for this list.
		 * @param v the byte to insert
		 */
		void insert(P v) {
			ListTag::insert(std::make_shared<ContentTag>(v));
		}

		/**
		 * Insert a pre-formed tag. Type of argument must match fixed content for this list.
		 * @param v the byte to insert
		 * @throw std::runtime_error if the tag to be inserted doesn't match type for this list.
		 */
		void insert(const std::shared_ptr<Tag> &v) override {
			ListTag::insert(v);
		}

	public: /* Constructor and I/O */
		/**
		 * Deserializer constructor with specified format
		 * @param input        data input stream
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 * @param format       specifies the format of incoming data
		 *
		 * @throw NBTP::TagParseException if array length is negative
		 * @throw std::ios_base::failure  if I/O error has occurred
		 * @throw std::invalid_argument   if an invalid format is specified
		 */
		TypedListTag(std::istream &input, ssize_t &counter, IOFormat format = BIN) {
			int32_t size;
			switch (format) {
				case BIN:
					size = IntTag::parseBinaryNumeric(input, counter);
					if (size < 0) {
						throw TagParseException(counter, fmt::format(CONTENT_LEN_NEG, size));
					}
					for (int i = 0; i < size; i++) {
						this->payload.push_back(std::make_shared<ContentTag>(input, counter));
					}
					break;
				case PRETTY_PRINT:
					throw std::invalid_argument(PARSE_PRETTY);
				default:
					throw std::invalid_argument(PARSE_UNKNOWN_FMT);
			}
			this->contentType = T;
		}

		/**
		 * Helper function to write a fix-typed list tag to ostream
		 * @param ostream        output data stream
		 * @return the ostream passed in
		 */
		std::ostream &nbtOutput(std::ostream &ostream) const override {
			/* Do size sanity checking */
			if (this->size() > INT32_MAX) {
				throw std::runtime_error(LIST_TOO_LONG);
			}
			/* Write header, i.e. length of the list */
			IntTag tmp(this->size());
			tmp.nbtOutput(ostream);
			/* Write payload */
			outputPayloadOnly(ostream, BIN, 0);
			return ostream;
		}

		/**
		 * Format this tag in human readable output
		 * @param ostream   output stream to write formatted string info
		 * @param indent    indentation - content will be indented 1 level deeper
		 * @return          output stream
		 */
		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) const override {
			ostream << fmt::format(REPR_TYPED_LIST, TypeNames[this->typeCode()], this->size()) << std::endl;
			outputPayloadOnly(ostream, PRETTY_PRINT, indent);
			return ostream;
		}

		TypedListTag() = default;

		~TypedListTag() override = default;

		/**
		 * Copy constructor for all array-like tags. Inherits from ListTag.
		 * @param from tag to copy from
		 */
		TypedListTag(const TypedListTag &from) noexcept = default;

		TypedListTag &operator=(const TypedListTag &from) noexcept = default;

		/**
		 * Move constructor for all array-like tags. Inherits from ListTag.
		 * @param from tag to move from
		 */
		TypedListTag(TypedListTag &&from) noexcept = default;

		TypedListTag &operator=(TypedListTag &&from) noexcept = default;
	};

	typedef TypedListTag<int8_t, TagType::BYTE, TagType::BYTES> BytesTag;
	typedef TypedListTag<int32_t, TagType::INT, TagType::INTS> IntsTag;
	typedef TypedListTag<int64_t, TagType::LONG, TagType::LONGS> LongsTag;
}


#endif //NBTP_LISTTAG_H
