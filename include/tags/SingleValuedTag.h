//
// Created by cth451 on 2020/05/12.
//

#ifndef NBTP_SINGLEVALUEDTAG_H
#define NBTP_SINGLEVALUEDTAG_H

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
	};
}

#endif //NBTP_SINGLEVALUEDTAG_H