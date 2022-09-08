/* memfun: memory management fun
 *
 * refs:
 * - [1] https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef MEMFUN_H
#define MEMFUN_H 1

#include <string.h>
#include <unistd.h>

#include "popcnt.h"

#define _MEMFUN_PAGE_DEFAULT           4096
#define _MEMFUN_BLOCK_DEFAULT          8192
#define _MEMFUN_GROWTH_FACTOR_DEFAULT  10

// #define MEMFUN_BLOCK          _MEMFUN_BLOCK_DEFAULT
// #define MEMFUN_GROWTH_FACTOR  _MEMFUN_GROWTH_FACTOR_DEFAULT

#ifndef MEMFUN_FREE_PROC
#define MEMFUN_FREE_PROC free
#endif

static size_t memfun_page_size(void)
{
	static size_t x = 0;
	if (x) return x;

#ifdef _SC_PAGESIZE
	long len = sysconf(_SC_PAGESIZE);
	if (len > 0) return x = len;
#endif

	// good old standard
	return x = _MEMFUN_PAGE_DEFAULT;
}

static size_t memfun_block_size(void)
{
	static size_t x = 0;
	if (x) return x;

#ifdef MEMFUN_BLOCK
	static const size_t _default
	= ((MEMFUN_BLOCK) < _MEMFUN_PAGE_DEFAULT)
	? _MEMFUN_BLOCK_DEFAULT
	: ((POPCNT_MACRO64(MEMFUN_BLOCK) == 1) ? (MEMFUN_BLOCK) : _MEMFUN_BLOCK_DEFAULT);
#else
	static const size_t _default
	= _MEMFUN_BLOCK_DEFAULT;
#endif

	size_t page = memfun_page_size();

	return x = (_default > page) ? _default : page;
}

#ifdef MEMFUN_GROWTH_FACTOR
static const unsigned int memfun_growth_factor = (((MEMFUN_GROWTH_FACTOR) > 1) && ((MEMFUN_GROWTH_FACTOR) < (__INTPTR_WIDTH__ / 2)))
	                                           ? (MEMFUN_GROWTH_FACTOR) : _MEMFUN_GROWTH_FACTOR_DEFAULT;
#else /* ! MEMFUN_GROWTH_FACTOR */
static const unsigned int memfun_growth_factor = _MEMFUN_GROWTH_FACTOR_DEFAULT;
#endif /* MEMFUN_GROWTH_FACTOR */

#define MEMFUN_MACRO_ALIGN_EX(length, align) \
	(((align) < 1) ? (length) : ( \
		(POPCNT_MACRO32(align) == 1) \
		? ( \
			((length) & ~((align) - 1)) + ((((length) & ((align) - 1)) != 0) ? (align) : 0) \
		  ) \
		: ( \
			(length) + ((((length) % (align)) != 0) ? ((align) - ((length) % (align))) : 0) \
		  ) \
	))

static size_t memfun_align_ex(size_t length, size_t align)
{
	if (!align) return length;

	if (popcntl(align) == 1) {
		size_t mask = align - 1;
		return (length & ~mask) + (((length & mask) != 0) ? align : 0);
	}

	size_t rem = length % align;
	return length + ((rem != 0) ? (align - rem) : 0);
}

#define MEMFUN_MACRO_ND2_DUMB(x) \
	(((x)<<1) & ~((x)|((x)>>1)|((x)>>2)|((x)>>3)|((x)>>4)))

static size_t memfun_nd2_dumb(size_t x)
{
	return (x<<1) & ~(x|(x>>1)|(x>>2)|(x>>3)|(x>>4));
}

#define MEMFUN_MACRO_ALIGN(length) \
	((length) < 1) ? 0 : ( \
		(POPCNT_MACRO64(length) == 1) ? (length) : ( \
			((length) > sizeof(size_t)) \
			? MEMFUN_MACRO_ALIGN_EX(length, sizeof(size_t)) \
			: MEMFUN_MACRO_ND2_DUMB(length) \
		) \
	)

static size_t memfun_align(size_t length)
{
	if (!length) return 0;

	if (popcntl(length) == 1)
		return length;

	if (length > sizeof(size_t))
		return memfun_align_ex(length, sizeof(size_t));

	return memfun_nd2_dumb(length);
}

static size_t memfun_calc_growth(size_t item_size)
{
	// waterfall
	static size_t x = 0;
	if (!x) x = memfun_block_size() >> memfun_growth_factor;

	if (item_size > x)
		return memfun_align_ex(item_size << memfun_growth_factor, memfun_block_size());

	return memfun_block_size();
}

static int memfun_want_realloc_raw(size_t length, size_t extend, size_t * new_length)
{
	if (new_length) *new_length = 0;

	size_t b = length + extend;
	if (b < length) return 0; // got overflow?..

	if (new_length) *new_length = b;
	return b > length;
}

static int memfun_want_realloc(size_t length, size_t extend, size_t * new_length)
{
	if (new_length) *new_length = 0;

	size_t a, b;
	a = memfun_align_ex(length, memfun_block_size());
	b = length + extend;
	if (b < a) return 0; // got overflow?..

	b = memfun_align_ex(b, memfun_block_size());
	if (new_length) *new_length = b;
	return b > a;
}

static void memfun_free(void * ptr, size_t len)
{
	if (!ptr) return;
	if (len) memset(ptr, 0, len);
	MEMFUN_FREE_PROC(ptr);
}

#endif
