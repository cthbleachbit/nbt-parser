//
// Created by cth451 on 2020/05/09.
//

#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(__WINDOWS__)
#define __WINDOWS__
#endif

#include "Conversion.h"

// This dirty hack comes from https://gist.github.com/panzi/6856583#file-portable_endian-h

#if defined(__WINDOWS__)
#include <winsock2.h>

// This is a very adhoc solution to build on msys2, where ntohll and htonll do not exist.
#ifndef flipll
#define flipll(x) ((((uint64_t)htonl(x)) << 32) + htonl((x) >> 32))
#endif

#ifndef htonll
#define htonll(x) flipll(x)
#endif

#ifndef ntohll
#define ntohll(x) flipll(x)
#endif

#define htobe16(x) htons(x)
#define htole16(x) (x)
#define be16toh(x) ntohs(x)
#define le16toh(x) (x)

#define htobe32(x) htonl(x)
#define htole32(x) (x)
#define be32toh(x) ntohl(x)
#define le32toh(x) (x)

#define htobe64(x) htonll(x)
#define htole64(x) (x)
#define be64toh(x) ntohll(x)
#define le64toh(x) (x)
#else
#include <endian.h>
#endif


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