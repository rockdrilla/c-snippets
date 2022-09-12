/* numfun: numbers fun
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef HEADER_INCLUDED_NUMFUN
#define HEADER_INCLUDED_NUMFUN 1

#include "dumb-recurse.h"
#include "popcnt.h"

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
		for (int i = 0; i < ((sizeof(t) * CHAR_BIT) - 3); i++) { \
			a = (a | b) >> 1; \
		} \
		return a; \
	}
_NUMFUN_DEFINE_SLB_FUNC(,   unsigned int)
_NUMFUN_DEFINE_SLB_FUNC(l,  unsigned long)
_NUMFUN_DEFINE_SLB_FUNC(ll, unsigned long long)

#define NUMFUN_NEXT2DEGREE32(v)  ( 1 + NUMFUN_MACRO_SET_LOWER_BITS32(v) )
#define NUMFUN_NEXT2DEGREE64(v)  ( 1 + NUMFUN_MACRO_SET_LOWER_BITS64(v) )

#define _NUMFUN_DEFINE_N2D_FUNC(n, t) \
	static t numfun_next2degree ## n (t v) { \
		if (v == 0) return 1; \
		return 1 + numfun_set_lower_bits ## n (v); \
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

	if (popcntl(align) == 1) {
		size_t mask = align - 1;
		return (value & ~mask) + (((value & mask) != 0) ? align : 0);
	}

	size_t rem = value % align;
	return value + ((rem != 0) ? (align - rem) : 0);
}

//

#endif
