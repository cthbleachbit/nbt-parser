/**
 * @file Conversion functions - taking care of big endian integers used by JVM and minecraft
 */

#ifndef NBTP_CONVERSION_H
#define NBTP_CONVERSION_H

#include <cstdint>

#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(__WINDOWS__)
#define __WINDOWS__
#endif

namespace NBTP {
	namespace Conversion {
		int16_t conv_16_beh(int16_t v);

		int32_t conv_32_beh(int32_t v);

		int64_t conv_64_beh(int64_t v);

		int16_t conv_16_hbe(int16_t v);

		int32_t conv_32_hbe(int32_t v);

		int64_t conv_64_hbe(int64_t v);

		float conv_f_beh(float v);

		float conv_f_hbe(float v);

		double conv_d_beh(double v);

		double conv_d_hbe(double v);
	}
}

#endif //NBTP_CONVERSION_H
