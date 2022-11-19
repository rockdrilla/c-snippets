/* roundby: align value at certain alignment value
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef HEADER_INCLUDED_NUM_ROUNDBY_HH
#define HEADER_INCLUDED_NUM_ROUNDBY_HH 1

#include "roundby.h"

template<typename T1, typename T2 = T1>
static
CC_FORCE_INLINE T1 roundby_t(T1 a, T2 b);

template<>
CC_FORCE_INLINE unsigned int roundby_t<unsigned int>(unsigned int a, unsigned int b)
{
	return roundby(a, b);
}

#define _ROUNDBY_T_DEFINE_FUNC(n, T) \
	template<> \
	CC_FORCE_INLINE T roundby_t<T>(T a, unsigned int b) \
	{ \
		return roundby ## n (a, b); \
	} \
	\
	template<> \
	CC_FORCE_INLINE T roundby_t<T>(T a, T b) \
	{ \
		return roundby ## n (a, b); \
	}
_ROUNDBY_T_DEFINE_FUNC(l,  unsigned long)
_ROUNDBY_T_DEFINE_FUNC(ll, unsigned long long)

#endif /* HEADER_INCLUDED_NUM_ROUNDBY_HH */
