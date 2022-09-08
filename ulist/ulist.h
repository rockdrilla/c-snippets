/* ulist: simple grow-only dynamic array
 *
 * refs:
 * - [1] https://github.com/DanielGibson/Snippets
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef HEADER_INCLUDED_ULIST
#define HEADER_INCLUDED_ULIST 1

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../misc/memfun.h"

#ifndef ULIST_IDX_T
  #define ULIST_IDX_T  uint32_t
#endif
typedef ULIST_IDX_T ulist_idx_t;

static const ulist_idx_t _ulist_idx_t_max_real = ~((ulist_idx_t) 0);
static const ulist_idx_t _ulist_idx_t_max      = _ulist_idx_t_max_real >> 1;

static const ulist_idx_t ulist_invalid_idx_t = _ulist_idx_t_max_real;

typedef struct {
	void         * ptr;
	size_t         igrowth;
	ulist_idx_t    isize, ialign;
	ulist_idx_t    used, allocated;
} ulist_t;
static const size_t __ulist_size_debug = sizeof(ulist_t);

typedef void (*ulist_item_visitor) (const void * value, ulist_idx_t index);

static inline size_t _ulist_item_mul(const ulist_t * list, size_t value)
{
	return value * (size_t) list->ialign;
}

static inline void * _ulist_item_ptr(const ulist_t * list, ulist_idx_t index)
{
	return (void *)((size_t) list->ptr + _ulist_item_mul(list, index));
}

static int ulist_grow_by_size(ulist_t * list, size_t extend)
{
	if (!extend) return 0;

	size_t old_length = _ulist_item_mul(list, list->allocated);
	size_t new_length = 0;
	size_t want_realloc = memfun_want_realloc(old_length, extend, &new_length);
	if (!new_length) {
		// TODO: report overflow somehow
		return 0;
	}

	size_t new_alloc = new_length / (size_t) list->ialign;
	if ((new_alloc >= _ulist_idx_t_max) || (new_alloc <= list->allocated)) {
		// TODO: report overflow somehow
		return 0;
	}

	if (want_realloc) {
		void * new_ptr = realloc(list->ptr, new_length);
		if (!new_ptr) return 0;

		list->ptr = new_ptr;
	}

	list->allocated = new_alloc;

	memset(_ulist_item_ptr(list, list->used), 0, new_length - old_length);

	return 1;
}

static inline int ulist_grow_by_items(ulist_t * list, ulist_idx_t count)
{
	if (!count) return 0;
	return ulist_grow_by_size(list, _ulist_item_mul(list, count));
}

static inline int _ulist_autogrow(ulist_t * list)
{
	return ulist_grow_by_size(list, list->igrowth);
}

static void _ulist_set(ulist_t * list, ulist_idx_t index, void * item)
{
	void * dst = _ulist_item_ptr(list, index);
	if (item)
		memcpy(dst, item, list->isize);
	else
		memset(dst, 0, list->isize);
}

static void ulist_init(ulist_t * list, ulist_idx_t item_size)
{
	memset(list, 0, sizeof(ulist_t));
	if (!item_size) return;
	list->isize = item_size;
	list->ialign = memfun_align(item_size);
	list->igrowth = memfun_calc_growth(list->ialign);
}

static void ulist_free(ulist_t * list)
{
	free(list->ptr);
	memset(list, 0, sizeof(ulist_t));
}

static void ulist_free_s(ulist_t * list)
{
	memfun_free(list->ptr, _ulist_item_mul(list, list->used));
	memset(list, 0, sizeof(ulist_t));
}

static void * ulist_get(const ulist_t * list, ulist_idx_t index)
{
	return (index >= list->used) ? NULL : _ulist_item_ptr(list, index);
}

static void ulist_set(ulist_t * list, ulist_idx_t index, void * item)
{
	if (index >= list->used)
		return;
	_ulist_set(list, index, item);
}

static ulist_idx_t ulist_append(ulist_t * list, void * item)
{
	if (list->used == list->allocated) {
		if (!_ulist_autogrow(list))
			return ulist_invalid_idx_t;
	}
	_ulist_set(list, list->used, item);
	return (list->used++);
}

static void ulist_walk(const ulist_t * list, ulist_item_visitor visitor)
{
	for (ulist_idx_t i = 0; i < list->used; i++) {
		visitor(_ulist_item_ptr(list, i), i);
	}
}

static void ulist_rwalk(const ulist_t * list, ulist_item_visitor visitor)
{
	for (ulist_idx_t i = list->used; (i--) != 0; ) {
		visitor(_ulist_item_ptr(list, i), i);
	}
}

#endif /* HEADER_INCLUDED_ULIST */
