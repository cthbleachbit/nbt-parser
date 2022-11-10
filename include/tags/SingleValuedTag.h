//
// Created by cth451 on 2020/05/12.
//

#ifndef NBTP_SINGLEVALUEDTAG_H
#define NBTP_SINGLEVALUEDTAG_H

#include <fmt/format.h>
#include "TagIO.h"
#include "Conversion.h"
#include "Tag.h"

/**
 * Define a new tag type based off SingleValuedTagd
 * @param Primitive     C/C++ type of primitive stored in the tag
 * @param ClassName     Name of the class
 * @param TagTypeEnum   TagType value for this type of tag
 */
#define __DEFINE_SINGLE_VALUED_TAG_TYPE(Primitive, ClassName, TagTypeEnum) \
class ClassName : public SingleValuedTag<Primitive> { \
public: \
constexpr TagType typeCode() const noexcept override { return TagTypeEnum ; } \
explicit ClassName(Primitive value) : SingleValuedTag<Primitive>(value) {}; \
ClassName(std::istream &input, ssize_t &counter) : ClassName(input, counter, BIN) {}; \
ClassName(std::istream &input, ssize_t &counter, IOFormat format) : SingleValuedTag<Primitive>(input, counter, format) {}; \
};

namespace NBTP {
	/**
	 * @class A tag that contains a fix-sized primitive - integers or decimals.
	 *
	 * Byte / Short / Int / Long / Float / Double tags all derive from this class.
	 *
	 * @tparam V  type of the primitive value in the tag
	 */
	template<typename V>
	class SingleValuedTag : public Tag {

	protected:
		/**
		 * Payload contained within tag
		 */
		V payload;

	public: /* Get and set payload */
		/**
		 * Set payload
		 * @param value incoming value
		 */
		virtual void setPayload(V value) {
			this->payload = value;
		}

		/**
		 * Retrieve payload
		 * @return primitive value contained in the tag
		 */
		virtual V getPayload() const {
			return this->payload;
		}

	public: /* Comparison operators */
		/**
		 * Equality operator
		 * @param rhs  the other tag to compare
		 * @return  whether tags are equal
		 */
		bool equal(const Tag &rhs) const override {
			if (rhs.typeCode() != this->typeCode()) {
				return false;
			}
			return this->payload == ((const SingleValuedTag<V> &) rhs).payload;
		}

	public: /* I/O */
		/**
		 * Format this tag in human readable output
		 * @param ostream   output stream to write formatted string info
		 * @param indent    indentation - unused for single valued tags
		 * @return          output stream
		 */
		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) const override {
			ostream << fmt::format(REPR_SINGLE_VALUED, TypeNames[this->typeCode()], this->payload) << std::endl;
			return ostream;
		}

		/**
		 * Parse numeric value from binary
		 * @param input        input data stream
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 * @return
		 */
		static V parseBinaryNumeric(std::istream &input, ssize_t &counter) {
			V buffer;
			input.read(reinterpret_cast<char *>(&buffer), sizeof(V));
			try {
				input.exceptions(std::istream::failbit);
			} catch (std::ios_base::failure &e) {
				throw TagParseException(counter, fmt::format(IO_UNEXPECTED_EOF, e.what()));
			}
			/* Perform java big-endian to host conversion */
			buffer = Conversion::toH(buffer);
			counter += sizeof(V);
			return buffer;
		}

		/**
		 * Helper function to write a single valued tag to ostream
		 * @param ostream        output data stream
		 * @return the ostream passed in
		 */
		std::ostream &nbtOutput(std::ostream &ostream) const override {
			/* Perform host to java big endian conversion */
			V big = Conversion::toJ(this->payload);
			ostream.write(reinterpret_cast<const char *>(&big), sizeof(V));
			return ostream;
		}

	public: /* Constructors */
		/**
		 * Copy constructor. This constructor cannot cause an exception.
		 * Since a single valued tag only store a single number within itself, this does not
		 * involve memory alloc / dealloc for the payload.
		 * @param tag tag to copy from
		 */
		SingleValuedTag(const SingleValuedTag<V> &tag) noexcept {
			this->payload = tag.payload;
		}

		/**
		 * Copy assignment operator.
		 * @param tag  The right hand side to copy from
		 * @return Updated tag
		 */
		SingleValuedTag &operator=(const SingleValuedTag<V> &tag) noexcept {
			if (&tag == this) {
				return *this;
			}

			this->payload = tag.payload;
			return *this;
		}

		/**
		 * Move constructor. This constructor cannot cause an exception.
		 * Since a single valued tag only store a single number within itself, this does not
		 * involve memory alloc / dealloc for the payload.
		 * @param tag tag to move from
		 */
		SingleValuedTag(SingleValuedTag<V> &&tag) noexcept {
			this->payload = tag.payload;
		}

		/**
		 * Move assignment operator
		 * @param tag   The right hand side to move from
		 * @return Updated tag
		 */
		SingleValuedTag &operator=(SingleValuedTag<V> &&tag) noexcept {
			this->payload = tag.payload;
			return *this;
		}

		/**
		 * Default constructor
		 */
		SingleValuedTag() = default;

		/**
		 * Direct constructor
		 * @param value    value this tag should take
		 */
		explicit SingleValuedTag(V value) : payload(value) {};

		/**
		 * Deserializer constructor for decompressed NBT input
		 * @param input stream to read a single byte
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 */
		SingleValuedTag(std::istream &input, ssize_t &counter) : SingleValuedTag(input, counter, BIN) {};

		/**
		 * Deserializer constructor with specified format
		 * @param input        data input stream
		 * @param counter      updated to reflect the number of bytes read from the input stream
		 * @param format       specifies the format of incoming data
		 *
		 * @throw std::ios_base::failure  if I/O error has occurred
		 */
		SingleValuedTag(std::istream &input, ssize_t &counter, IOFormat format) {
			switch (format) {
				case BIN:
					this->payload = parseBinaryNumeric(input, counter);
					break;
				case PRETTY_PRINT:
					throw std::invalid_argument(PARSE_PRETTY);
				default:
					throw std::invalid_argument(PARSE_UNKNOWN_FMT);
			}
		}

		/**
		 * Default destructor
		 */
		~SingleValuedTag() override = default;
	};

	__DEFINE_SINGLE_VALUED_TAG_TYPE(int8_t, ByteTag, TagType::BYTE);
	__DEFINE_SINGLE_VALUED_TAG_TYPE(int16_t, ShortTag, TagType::SHORT);
	__DEFINE_SINGLE_VALUED_TAG_TYPE(int32_t, IntTag, TagType::INT);
	__DEFINE_SINGLE_VALUED_TAG_TYPE(int64_t, LongTag, TagType::LONG);
	__DEFINE_SINGLE_VALUED_TAG_TYPE(float, FloatTag, TagType::FLOAT);
	__DEFINE_SINGLE_VALUED_TAG_TYPE(double, DoubleTag, TagType::DOUBLE);
}

#endif //NBTP_SINGLEVALUEDTAG_H
