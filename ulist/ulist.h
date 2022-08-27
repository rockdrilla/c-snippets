/* ulist: simple grow-only dynamic array
 *
 * refs:
 * - [1] https://github.com/DanielGibson/Snippets
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef ULIST_H
#define ULIST_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../misc/memfun.h"

#ifndef ULIST_IDX_T
  #define ULIST_IDX_T  uint32_t
#endif
typedef ULIST_IDX_T ulist_idx_t;

typedef struct {
	void         * ptr;
	size_t         igrowth;
	uint           isize;
	ulist_idx_t    used, allocated;
} ulist_t;
static const size_t __ulist_size_debug = sizeof(ulist_t);

typedef void (*ulist_item_visitor) (const void * value, ulist_idx_t index);

static inline size_t _ulist_item_mul(const ulist_t * list, size_t value)
{
	return (size_t) list->isize * value;
}

static inline void * _ulist_item_ptr(const ulist_t * list, ulist_idx_t index)
{
	return (void *)((size_t) list->ptr + _ulist_item_mul(list, index));
}

static void _ulist_grow_by_size(ulist_t * list, size_t length)
{
	if (!length) return;

	size_t new_alloc = memfun_want_realloc(_ulist_item_mul(list, list->allocated), length);
	if (!new_alloc) return;

	void * new_ptr = realloc(list->ptr, new_alloc);
	if (!new_ptr) return;

	list->ptr = new_ptr;
	list->allocated = new_alloc / (size_t) list->isize;
	memset(_ulist_item_ptr(list, list->used), 0, _ulist_item_mul(list, list->allocated - list->used));
}

static inline void _ulist_grow_by_item_count(ulist_t * list, ulist_idx_t count)
{
	if (!count) return;
	_ulist_grow_by_size(list, _ulist_item_mul(list, count));
}

static inline void _ulist_autogrow(ulist_t * list)
{
	_ulist_grow_by_size(list, list->igrowth);
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
	list->isize = item_size;
	if (!item_size) return;
	list->igrowth = memfun_calc_growth(item_size);
}

static void ulist_free(ulist_t * list)
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
	if (list->used == list->allocated)
		_ulist_autogrow(list);
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

#endif /* ULIST_H */
