//
// Created by cth451 on 2020/05/09.
//

#include "conv.h"
#include <endian.h>

namespace NBTP {
	namespace Conversion {
		int16_t conv_16_beh(int16_t v) {
			return be16toh(v);
		}

		int32_t conv_32_beh(int32_t v) {
			return be32toh(v);
		}

		int64_t conv_64_beh(int64_t v) {
			return be64toh(v);
		}

		int16_t conv_16_hbe(int16_t v) {
			return htobe16(v);
		}

		int32_t conv_32_hbe(int32_t v) {
			return htobe32(v);
		}

		int64_t conv_64_hbe(int64_t v) {
			return htobe64(v);
		}
	}
}