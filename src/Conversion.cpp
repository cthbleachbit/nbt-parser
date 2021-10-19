//
// Created by cth451 on 2020/05/09.
//

#include "Conversion.h"

// This dirty hack comes from https://gist.github.com/panzi/6856583#file-portable_endian-h
#if defined(__WINDOWS__)
#include <winsock2.h>

// This is a very adhoc solution to build on msys2, where ntohll and htonll do not exist.
// Also this applies to windows only, which for now implies a little-endian processor.
#ifndef htonll
int64_t htonll(int64_t x) {
	uint64_t high = htonl((uint32_t) x);
	uint64_t low = htonl((uint32_t) (x >> 32));
	return (int64_t) ((high << 32) + low);
}
#endif
#endif

// Apple puts this in a different place and uses the same naming convention as M$ does.
#if defined(__APPLE__)
#include <machine/endian.h>
#endif

#if defined(__WINDOWS__) || defined(__APPLE__)
#ifndef ntohll
#define ntohll(x) htonll(x)
#endif

#define htobe16(x) htons(x)
#define htole16(x) (x)
#define be16toh(x) ntohs(x)
#define le16toh(x) (x)

#define htobe32(x) htonl(x)
#define htole32(x) (x)
#define be32toh(x) ntohl(x)
#define le32toh(x) (x)

#ifdef MSC_VER
#define htobe64(x) htonll(x)
#define htole64(x) (x)
#define be64toh(x) ntohll(x)
#define le64toh(x) (x)
#else
#define htobe64(x) ((1==htonl(1)) ? (x) : (((uint64_t)htonl((x) & 0xFFFFFFFFUL)) << 32) | htonl((uint32_t)((x) >> 32)))
#define be64toh(x) ((1==ntohl(1)) ? (x) : (((uint64_t)ntohl((x) & 0xFFFFFFFFUL)) << 32) | ntohl((uint32_t)((x) >> 32)))
#endif

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
