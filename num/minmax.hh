/* minmax: min/max
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef HEADER_INCLUDED_NUM_MINMAX_HH
#define HEADER_INCLUDED_NUM_MINMAX_HH 1

#include "../misc/cc-inline.h"
#include "minmax.h"

template<typename T1, typename T2 = T1>
static
CC_FORCE_INLINE T1 min_positive_t(T1 a, T2 b);

template<>
CC_FORCE_INLINE int min_positive_t<int>(int a, int b)
{
	return min_positive(a, b);
}

#define _MIN_POSITIVE_T_DEFINE_FUNC(n, T) \
	template<> \
	CC_FORCE_INLINE T min_positive_t<T>(T a, int b) \
	{ \
		return min_positive ## n (a, b); \
	} \
	\
	template<> \
	CC_FORCE_INLINE T min_positive_t<T>(T a, T b) \
	{ \
		return min_positive ## n (a, b); \
	}
_MIN_POSITIVE_T_DEFINE_FUNC(l,  long)
_MIN_POSITIVE_T_DEFINE_FUNC(ll, long long)

#endif /* HEADER_INCLUDED_NUM_MINMAX_HH */
