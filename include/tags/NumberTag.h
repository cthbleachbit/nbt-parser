//
// Created by cth451 on 2020/05/12.
//

#ifndef NBTP_NUMBERTAG_H
#define NBTP_NUMBERTAG_H

namespace NBTP {
	template<typename V>
	class NumberTag {
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

#endif //NBTP_NUMBERTAG_H
