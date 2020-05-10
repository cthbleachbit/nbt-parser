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

#ifndef SYSTEM_IS_BIG_ENDIAN

		inline float swap_f(float v) {
			float ret;
			auto dest = (uint8_t *) &ret;
			auto orig = (uint8_t *) &v;
			for (unsigned long i = 0; i < sizeof(float); i++) {
				dest[i] = orig[sizeof(float) - 1 - i];
			}
			return ret;
		}

		inline double swap_d(double v) {
			double ret;
			auto dest = (uint8_t *) &ret;
			auto orig = (uint8_t *) &v;
			for (unsigned long i = 0; i < sizeof(double); i++) {
				dest[i] = orig[sizeof(double) - 1 - i];
			}
			return ret;
		}

#endif

		float conv_f_beh(float v) {
#ifdef SYSTEM_IS_BIG_ENDIAN
			return v;
#else
			return swap_f(v);
#endif
		}

		float conv_f_hbe(float v) {
#ifdef SYSTEM_IS_BIG_ENDIAN
			return v;
#else
			return swap_f(v);
#endif
		}

		double conv_d_beh(double v) {
#ifdef SYSTEM_IS_BIG_ENDIAN
			return v;
#else
			return swap_d(v);
#endif
		}

		double conv_d_hbe(double v) {
#ifdef SYSTEM_IS_BIG_ENDIAN
			return v;
#else
			return swap_d(v);
#endif
		}
	}
}