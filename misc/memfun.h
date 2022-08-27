/* memfun: memory management fun
 * 
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef MEMFUN_H
#define MEMFUN_H

#include <string.h>
#include <unistd.h>

#include "popcnt.h"

#define _MEMFUN_BLOCK_DEFAULT          65536
#define _MEMFUN_GROWTH_FACTOR_DEFAULT  8

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

static unsigned int memfun_growth_factor(void)
{
	static unsigned int x = 0;

	if (x) return x;

#ifdef MEMFUN_GROWTH_FACTOR
	static const unsigned int _default = (((MEMFUN_GROWTH_FACTOR) > 1) && ((MEMFUN_GROWTH_FACTOR) < (__INTPTR_WIDTH__ / 2)))
	                                   ? (MEMFUN_GROWTH_FACTOR) : _MEMFUN_GROWTH_FACTOR_DEFAULT;
#else
	static const unsigned int _default = _MEMFUN_GROWTH_FACTOR_DEFAULT;
#endif

	return x = _default;
}

static size_t memfun_align(size_t length, size_t align)
{
	if (align == 0)
		return length;

	if (popcntl(align) == 1) {
		size_t mask = align - 1;
		return (length & ~mask) + (((length & mask) != 0) ? align : 0);
	}

	size_t rem = length % align;
	return length + ((rem != 0) ? (align - rem) : 0);
}

static size_t memfun_calc_growth(size_t item_size)
{
	static size_t watermark = 0;

	if (!watermark) watermark = memfun_block_size() >> memfun_growth_factor();

	if (item_size > watermark)
		return memfun_align(item_size << memfun_growth_factor(), memfun_block_size());

	return memfun_block_size();
}

static size_t memfun_want_realloc(size_t length, size_t extend)
{
	size_t a, b;
	a = memfun_align(length, memfun_block_size());
	b = memfun_align(length + extend, memfun_block_size());
	return (b > a) ? b : 0;
}

static void memfun_free(void * ptr, size_t len)
{
	if (!ptr) return;
	if (len) memset(ptr, 0, len);
	free(ptr);
}

#endif
