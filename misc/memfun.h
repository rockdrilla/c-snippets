/* memfun: memory management fun
 *
 * refs:
 * - [1] https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef MEMFUN_H
#define MEMFUN_H

#include <string.h>
#include <unistd.h>

#include "popcnt.h"

#define _MEMFUN_BLOCK_DEFAULT          8192
#define _MEMFUN_GROWTH_FACTOR_DEFAULT  10

// #define MEMFUN_BLOCK          _MEMFUN_BLOCK_DEFAULT
// #define MEMFUN_GROWTH_FACTOR  _MEMFUN_GROWTH_FACTOR_DEFAULT

static size_t memfun_page_size(void)
{
	static size_t x = 0;
	if (x) return x;

#ifdef _SC_PAGESIZE
	long len = sysconf(_SC_PAGESIZE);
	if (len > 0) return x = len;
#endif

	// good old standard
	return x = 4096;
}

static size_t memfun_block_size(void)
{
	static size_t x = 0;
	if (x) return x;

#ifdef MEMFUN_BLOCK
	static const size_t _default = (POPCNT_MACRO64(MEMFUN_BLOCK) == 1)
	                             ? (MEMFUN_BLOCK) : _MEMFUN_BLOCK_DEFAULT;
#else
	static const size_t _default = _MEMFUN_BLOCK_DEFAULT;
#endif

	size_t page = memfun_page_size();

	return x = (_default > page) ? _default : page;
}

#ifdef MEMFUN_GROWTH_FACTOR
static const unsigned int memfun_growth_factor = (((MEMFUN_GROWTH_FACTOR) > 1) && ((MEMFUN_GROWTH_FACTOR) < (__INTPTR_WIDTH__ / 2)))
	                                           ? (MEMFUN_GROWTH_FACTOR) : _MEMFUN_GROWTH_FACTOR_DEFAULT;
#else
static const unsigned int memfun_growth_factor = _MEMFUN_GROWTH_FACTOR_DEFAULT;
#endif

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

static size_t _memfun_nd2(size_t x)
{
#if __has_builtin(__builtin_clzl)
	return 1 << (__INTPTR_WIDTH__ + 1 - __builtin_clzl(x));
#else
	return (x<<1) & ~(x|(x>>1)|(x>>2)|(x>>3));
#endif
}

static size_t memfun_align(size_t length)
{
	if (!length) return 0;

	if (popcntl(length) == 1)
		return length;

	if (length > sizeof(size_t))
		return memfun_align_ex(length, sizeof(size_t));

	return _memfun_nd2(length);
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
	free(ptr);
}

#endif
