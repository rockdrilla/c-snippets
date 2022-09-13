/* numfun: numbers fun
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef HEADER_INCLUDED_NUMFUN
#define HEADER_INCLUDED_NUMFUN 1

#include "dumb-recurse.h"
#include "popcnt.h"

#if defined __has_builtin
  #if __has_builtin(__builtin_uadd_overflow)
    #ifndef _NUMFUN_HAVE_BUILTIN_UADD
    #define _NUMFUN_HAVE_BUILTIN_UADD 1
    #endif
  #endif /* __builtin_uadd_overflow */
  #if __has_builtin(__builtin_uaddl_overflow)
    #ifndef _NUMFUN_HAVE_BUILTIN_UADDL
    #define _NUMFUN_HAVE_BUILTIN_UADDL 1
    #endif
  #endif /* __builtin_uaddl_overflow */
  #if __has_builtin(__builtin_uaddll_overflow)
    #ifndef _NUMFUN_HAVE_BUILTIN_UADDLL
    #define _NUMFUN_HAVE_BUILTIN_UADDLL 1
    #endif
  #endif /* __builtin_uaddll_overflow */
#endif /* __has_builtin */

// SLB = set lower bits

#define _NUMFUN_SLB_a(v)     ( (v) | ((v) - 1) | ((v) >> 1) )
#define _NUMFUN_SLB_b(v)     ( (v) << 1 )
#define _NUMFUN_SLB_x(a, b)  ( ((a) | (b)) >> 1 )

// 32 - 3 = 29
#define _NUMFUN_SLB32(v)  DUMB_RECURSE_29(_NUMFUN_SLB_x, _NUMFUN_SLB_a(v), _NUMFUN_SLB_b(v))
// 64 - 3 = 61
#define _NUMFUN_SLB64(v)  DUMB_RECURSE_61(_NUMFUN_SLB_x, _NUMFUN_SLB_a(v), _NUMFUN_SLB_b(v))

#define NUMFUN_MACRO_SET_LOWER_BITS32(v)  ( (((v) & UINT_MAX)   == 0) ? 0 : _NUMFUN_SLB32((v) & UINT_MAX) )
#define NUMFUN_MACRO_SET_LOWER_BITS64(v)  ( (((v) & ULLONG_MAX) == 0) ? 0 : _NUMFUN_SLB64((v) & ULLONG_MAX) )

#define _NUMFUN_DEFINE_SLB_FUNC(n, t) \
	static t numfun_set_lower_bits ## n (t v) { \
		if (v == 0) return 0; \
		t a = v | (v - 1) | (v >> 1); \
		t b = v << 1; \
		for (unsigned long i = 0; i < ((sizeof(t) * CHAR_BIT) - 3); i++) { \
			a = (a | b) >> 1; \
		} \
		return a; \
	}
_NUMFUN_DEFINE_SLB_FUNC(,   unsigned int)
_NUMFUN_DEFINE_SLB_FUNC(l,  unsigned long)
_NUMFUN_DEFINE_SLB_FUNC(ll, unsigned long long)

// TODO: write fair enough version
#define _NUMFUN_DEFINE_UADD_FUNC(n, t) \
	static int numfun_uadd ## n (t a, t b, t * r) { \
		t res = a + b; \
		if (a > b) { \
			if (res < a) return 0; \
		} else { \
			if (res < b) return 0; \
		} \
		if (r) *r = res; \
		return 1; \
	}

#if _NUMFUN_HAVE_BUILTIN_UADD
#define numfun_uadd(a, b, r) __builtin_uadd_overflow(a, b, r)
#else /* ! _NUMFUN_HAVE_BUILTIN_UADD */
_NUMFUN_DEFINE_UADD_FUNC(, unsigned int)
#endif /* _NUMFUN_HAVE_BUILTIN_UADD */

#if _NUMFUN_HAVE_BUILTIN_UADDL
#define numfun_uaddl(a, b, r) __builtin_uaddl_overflow(a, b, r)
#else /* ! _NUMFUN_HAVE_BUILTIN_UADDL */
_NUMFUN_DEFINE_UADD_FUNC(l, unsigned long)
#endif /* _NUMFUN_HAVE_BUILTIN_UADDL */

#if _NUMFUN_HAVE_BUILTIN_UADDLL
#define numfun_uaddll(a, b, r) __builtin_uaddll_overflow(a, b, r)
#else /* ! _NUMFUN_HAVE_BUILTIN_UADDLL */
_NUMFUN_DEFINE_UADD_FUNC(ll, unsigned long long)
#endif /* _NUMFUN_HAVE_BUILTIN_UADDLL */

#define NUMFUN_NEXT2DEGREE32(v)  ( 1 + NUMFUN_MACRO_SET_LOWER_BITS32(v) )
#define NUMFUN_NEXT2DEGREE64(v)  ( 1 + NUMFUN_MACRO_SET_LOWER_BITS64(v) )

#define _NUMFUN_DEFINE_N2D_FUNC(n, t) \
	static t numfun_next2degree ## n (t v) { \
		if (v == 0) return 1; \
		t r, x = numfun_set_lower_bits ## n (v); \
		return (numfun_uadd ## n (x, 1, &r)) ? r : (1 + (x >> 1)); \
	}
_NUMFUN_DEFINE_N2D_FUNC(,   unsigned int)
_NUMFUN_DEFINE_N2D_FUNC(l,  unsigned long)
_NUMFUN_DEFINE_N2D_FUNC(ll, unsigned long long)

#define NUMFUN_MACRO_HIGHEST_BIT32(v)  ( (((v) & UINT_MAX)   == 0) ? 0 : POPCNT_MACRO32(NUMFUN_MACRO_SET_LOWER_BITS32(v)) )
#define NUMFUN_MACRO_HIGHEST_BIT64(v)  ( (((v) & ULLONG_MAX) == 0) ? 0 : POPCNT_MACRO64(NUMFUN_MACRO_SET_LOWER_BITS64(v)) )

#define _NUMFUN_DEFINE_HB_FUNC(n, t) \
	static int numfun_highest_bit ## n (t v) { \
		if (v == 0) return 0; \
		return popcnt ## n (numfun_set_lower_bits ## n (v)); \
	}
_NUMFUN_DEFINE_HB_FUNC(,   unsigned int)
_NUMFUN_DEFINE_HB_FUNC(l,  unsigned long)
_NUMFUN_DEFINE_HB_FUNC(ll, unsigned long long)

#define _NUMFUN_ROUND_BIN(v, a) \
	( ((v) & ~((a) - 1)) + ((((v) & ((a) - 1)) != 0) ? (a) : 0) )

#define _NUMFUN_ROUND_ANY(v, a) \
	( (v) + ((((v) % (a)) != 0) ? ((a) - ((v) % (a))) : 0) )

#define NUMFUN_MACRO_ROUND_BY(value, align) \
	( ((align) < 2) ? (value) : ( \
		(POPCNT_MACRO32(align) == 1) \
		? _NUMFUN_ROUND_BIN(value, align) \
		: _NUMFUN_ROUND_ANY(value, align) \
	) )

static size_t numfun_round_by(size_t value, size_t align)
{
	if (align < 2) return value;

	size_t r, x;

	r = (popcntl(align) == 1) ? (value & (align - 1)) : (value % align);
	if (!r) return value;

	x = value - r;
	return (numfun_uaddl(x, align, &r)) ? r : x;
}

#endif
