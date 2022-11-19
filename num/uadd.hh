/* uadd: (safe) unsigned add
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef HEADER_INCLUDED_NUM_UADD_HH
#define HEADER_INCLUDED_NUM_UADD_HH 1

#include "../misc/cc-inline.h"
#include "uadd.h"

template<typename T1, typename T2 = T1>
static
CC_FORCE_INLINE int uadd_t(T1 a, T2 b, T1 * r);

template<>
CC_FORCE_INLINE int uadd_t<unsigned int>(unsigned int a, unsigned int b, unsigned int * r)
{
	return uadd(a, b, r);
}

#define _UADD_T_DEFINE_FUNC(n, T) \
	template<> \
	CC_FORCE_INLINE int uadd_t<T>(T a, unsigned int b, T * r) \
	{ \
		return uadd ## n (a, b, r); \
	} \
	\
	template<> \
	CC_FORCE_INLINE int uadd_t<T>(T a, T b, T * r) \
	{ \
		return uadd ## n (a, b, r); \
	}
_UADD_T_DEFINE_FUNC(l,  unsigned long)
_UADD_T_DEFINE_FUNC(ll, unsigned long long)

#endif /* HEADER_INCLUDED_NUM_UADD_HH */
