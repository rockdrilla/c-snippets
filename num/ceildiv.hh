/* ceildiv: (safe) "ceil-div" variant
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef HEADER_INCLUDED_NUM_CEILDIV_HH
#define HEADER_INCLUDED_NUM_CEILDIV_HH 1

#include "ceildiv.h"

template<typename T1, typename T2 = T1>
static
CC_FORCE_INLINE T1 ceildiv_t(T1 a, T2 b);

template<>
CC_FORCE_INLINE int ceildiv_t<int>(int a, int b)
{
	return ceildiv(a, b);
}

#define _CEILDIV_T_DEFINE_FUNC(n, T) \
	template<> \
	CC_FORCE_INLINE T ceildiv_t<T>(T a, int b) \
	{ \
		return ceildiv ## n (a, b); \
	} \
	\
	template<> \
	CC_FORCE_INLINE T ceildiv_t<T>(T a, T b) \
	{ \
		return ceildiv ## n (a, b); \
	}
_CEILDIV_T_DEFINE_FUNC(l,  long)
_CEILDIV_T_DEFINE_FUNC(ll, long long)

#endif /* HEADER_INCLUDED_NUM_CEILDIV_HH */
