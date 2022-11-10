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

		template<typename V>
		V toH(V v) = delete;

		template<typename V>
		V toJ(V v) = delete;

		template<> int8_t toH(int8_t);
		template<> int16_t toH(int16_t);
		template<> int32_t toH(int32_t);
		template<> int64_t toH(int64_t);
		template<> float toH(float);
		template<> double toH(double);
		template<> int8_t toJ(int8_t);
		template<> int16_t toJ(int16_t);
		template<> int32_t toJ(int32_t);
		template<> int64_t toJ(int64_t);
		template<> float toJ(float);
		template<> double toJ(double);
	}
}

#endif //NBTP_CONVERSION_H
