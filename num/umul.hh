/* umul: (safe) unsigned multiply
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef HEADER_INCLUDED_NUM_UMUL_HH
#define HEADER_INCLUDED_NUM_UMUL_HH 1

#include "../misc/cc-inline.h"
#include "umul.h"

template<typename T1, typename T2 = T1>
static
CC_FORCE_INLINE int umul_t(T1 a, T2 b, T1 * r);

template<>
CC_FORCE_INLINE int umul_t<unsigned int>(unsigned int a, unsigned int b, unsigned int * r)
{
	return umul(a, b, r);
}

#define _UMUL_T_DEFINE_FUNC(n, T) \
	template<> \
	CC_FORCE_INLINE int umul_t<T>(T a, unsigned int b, T * r) \
	{ \
		return umul ## n (a, b, r); \
	} \
	\
	template<> \
	CC_FORCE_INLINE int umul_t<T>(T a, T b, T * r) \
	{ \
		return umul ## n (a, b, r); \
	}
_UMUL_T_DEFINE_FUNC(l,  unsigned long)
_UMUL_T_DEFINE_FUNC(ll, unsigned long long)

#endif /* HEADER_INCLUDED_NUM_UMUL_HH */
