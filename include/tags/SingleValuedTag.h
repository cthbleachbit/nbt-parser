//
// Created by cth451 on 2020/05/12.
//

#ifndef NBTP_SINGLEVALUEDTAG_H
#define NBTP_SINGLEVALUEDTAG_H

#include <fmt/format.h>
#include "TagIO.h"
#include "Conversion.h"
#include "Tag.h"

namespace NBTP {
	/**
	 * @class A tag that contains a fix-sized primitive - integers or decimals.
	 *
	 * Byte / Short / Int / Long / Float / Double tags all derive from this class.
	 *
	 * @tparam P  type of the primitive value in the tag
	 * @tparam T  NBT TagType this tag should have
	 */
	template<
			typename P,
			TagType T,
			std::enable_if_t<(std::is_floating_point_v<P> || std::is_integral_v<P>) && std::is_signed_v<P>, int> = 0
	>
	class SingleValuedTag : public Tag {
	public:
		typedef P V;

	protected:
		/**
		 * Payload contained within tag
		 */
		P payload;

	public: /* Get and set payload */
		constexpr TagType typeCode() const noexcept override {
			return T;
		}

		/**
		 * Set payload
		 * @param value incoming value
		 */
		virtual void setPayload(P value) {
			this->payload = value;
		}

		/**
		 * Retrieve payload
		 * @return primitive value contained in the tag
		 */
		virtual P getPayload() const {
			return this->payload;
		}

	public: /* Comparison operators */
		/**
		 * Spaceship operator for tags that contains integer types.
		 * @param rhs   the other tag
		 * @return      std::strong_ordering of the tags
		 */
		template<typename P1 = P, std::enable_if_t<std::is_integral_v<P1>, bool> = true>
		std::strong_ordering operator<=>(const SingleValuedTag<P, T> &rhs) const {
			return this->payload <=> rhs.payload;
		}

		/**
		 * Spaceship operator for tags that contains decimal types.
		 * Note: the return type is std::partial_ordering since floating point in IEEE754 allows NaN and Inf.
		 *
		 * @param rhs   the other tag
		 * @return      std::partial_ordering of the tags
		 */
		template<typename P1 = P, std::enable_if_t<std::is_floating_point_v<P1>, bool> = true>
		std::partial_ordering operator<=>(const SingleValuedTag<P, T> &rhs) const {
			return this->payload <=> rhs.payload;
		}

		/**
		 * Equality operator for potentially other tag types
		 * @param rhs  the other tag to compare
		 * @return  whether tags are equal
		 */
		bool equal(const Tag &rhs) const override {
			if (rhs.typeCode() != this->typeCode()) {
				return false;
			}
			return std::is_eq(*this <=> ((const SingleValuedTag<P, T> &) rhs));
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
		static P parseBinaryNumeric(std::istream &input, ssize_t &counter) {
			P buffer;
			input.read(reinterpret_cast<char *>(&buffer), sizeof(P));
			try {
				input.exceptions(std::istream::failbit);
			} catch (std::ios_base::failure &e) {
				throw TagParseException(counter, fmt::format(IO_UNEXPECTED_EOF, e.what()));
			}
			/* Perform java big-endian to host conversion */
			buffer = Conversion::toH(buffer);
			counter += sizeof(P);
			return buffer;
		}

		/**
		 * Helper function to write a single valued tag to ostream
		 * @param ostream        output data stream
		 * @return the ostream passed in
		 */
		std::ostream &nbtOutput(std::ostream &ostream) const override {
			/* Perform host to java big endian conversion */
			P big = Conversion::toJ(this->payload);
			ostream.write(reinterpret_cast<const char *>(&big), sizeof(P));
			return ostream;
		}

	public: /* Constructors */
		/**
		 * Copy constructor. This constructor cannot cause an exception.
		 * Since a single valued tag only store a single number within itself, this does not
		 * involve memory alloc / dealloc for the payload.
		 * @param tag tag to copy from
		 */
		SingleValuedTag(const SingleValuedTag<P, T> &tag) noexcept {
			this->payload = tag.payload;
		}

		/**
		 * Copy assignment operator.
		 * @param tag  The right hand side to copy from
		 * @return Updated tag
		 */
		SingleValuedTag &operator=(const SingleValuedTag<P, T> &tag) noexcept {
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
		SingleValuedTag(SingleValuedTag<P, T> &&tag) noexcept {
			this->payload = tag.payload;
		}

		/**
		 * Move assignment operator
		 * @param tag   The right hand side to move from
		 * @return Updated tag
		 */
		SingleValuedTag &operator=(SingleValuedTag<P, T> &&tag) noexcept {
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
		explicit SingleValuedTag(P value) : payload(value) {};

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

	/**
	 * @typedef A tag that contains an 8-bit signed integer
	 */
	typedef SingleValuedTag<int8_t, TagType::BYTE> ByteTag;

	/**
	 * @typedef A tag that contains a 16-bit signed integer
	 */
	typedef SingleValuedTag<int16_t, TagType::SHORT> ShortTag;

	/**
	 * @typedef A tag that contains a 32-bit signed integer
	 */
	typedef SingleValuedTag<int32_t, TagType::INT> IntTag;

	/**
	 * @typedef A tag that contains a 64-bit signed integer
	 */
	typedef SingleValuedTag<int64_t, TagType::LONG> LongTag;

	/**
	 * @typedef A tag that contains a 32-bit IEEE754 decimal (i.e. float)
	 */
	typedef SingleValuedTag<float, TagType::FLOAT> FloatTag;

	/**
	 * @typedef A tag that contains a 64-bit IEEE754 decimal (i.e. double)
	 */
	typedef SingleValuedTag<double, TagType::DOUBLE> DoubleTag;
}

#endif //NBTP_SINGLEVALUEDTAG_H
