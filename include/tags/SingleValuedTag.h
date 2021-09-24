//
// Created by cth451 on 2020/05/12.
//

#ifndef NBTP_SINGLEVALUEDTAG_H
#define NBTP_SINGLEVALUEDTAG_H

#include <fmt/format.h>
#include "Tag.h"

namespace NBTP {
	template<typename V>
	class SingleValuedTag : public Tag {
	protected:
		V payload;
	public:
		virtual void setPayload(V value) {
			this->payload = value;
		}

		virtual V getPayload() const {
			return this->payload;
		}

		bool equal(const Tag &rhs) const override {
			if (rhs.typeCode() != this->typeCode()) {
				return false;
			}
			return this->payload == ((const SingleValuedTag<V> &) rhs).payload;
		}

		/**
		 * Format this tag
		 * @param ostream   output stream to write formatted string info
		 * @param indent    indentation ununsed for single valued tags
		 * @return          output stream
		 */
		std::ostream &textOutput(std::ostream &ostream, unsigned int indent) const override {
			ostream << fmt::format(REPR_SINGLE_VALUED, TypeNames[this->typeCode()], this->payload) << std::endl;
			return ostream;
		}

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
			if (&tag == this) {
				return *this;
			}

			this->payload = tag.payload;
			return *this;
		}

		SingleValuedTag() = default;

		explicit SingleValuedTag(V value) : payload(value) {};

		~SingleValuedTag() override = default;
	};
}

#endif //NBTP_SINGLEVALUEDTAG_H
