/* uhash: simple grow-only hash
 *
 * - uhash "type 2": key is "scalar", value is "aggregate"
 *
 * refs:
 * - [1] https://github.com/etherealvisage/avl
 * - [2] https://github.com/DanielGibson/Snippets
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef UHASH_TYPE2_H
#define UHASH_TYPE2_H

#include "uhash-type1.h"

#define _UHASH_TYPEPROC_NODE_VISITOR__TYPE2(user_t, key_t, value_t) \
	typedef int (* UHASH_T(user_t, node_proc) ) (key_t key, const value_t * value);

#define _UHASH_TYPE_NODE__TYPE2(user_t, key_t, value_t) \
	typedef struct UHASH_T(user_t, node) { \
		uhash_idx_t  left, right; \
		int          depth; \
		key_t        key; \
		uhash_idx_t  value; \
	} UHASH_T(user_t, node);

#define _UHASH_TYPEIMPL__TYPE2(user_t) \
	_UHASH_TYPEIMPL__TYPE1(user_t) \
	ulist_t                      values; \
	UHASH_T(user_t, value_proc)  value_constructor; \
	UHASH_T(user_t, value_proc)  value_destructor;


#define _UHASH_PROC_KEY__TYPE2(user_t, key_t) \
	_UHASH_PROC_KEY__TYPE1(user_t, key_t)


#define _UHASH_PROC_VALUE__TYPE2(user_t, value_t) \
	static inline const value_t * \
	UHASH_P(user_t, _raw_value) (const user_t * hash, uhash_idx_t index) { \
		return ulist_get(&hash->values, _uhash_idx_int(index)); \
	} \
	\
	static inline const value_t * \
	UHASH_P(user_t, _value) (const user_t * hash, const UHASH_T(user_t, node) * node) { \
		return (node->value == 0) ? NULL : UHASH_C(user_t, _raw_value, hash, node->value); \
	} \
	\
	static const value_t * \
	UHASH_P(user_t, value) (const user_t * hash, uhash_idx_t node_index) { \
		const UHASH_T(user_t, node) * node = UHASH_C(user_t, cnode, hash, node_index); \
		if (!node) return NULL; \
		return UHASH_C(user_t, _value, hash, node); \
	} \
	\
	static void \
	UHASH_P(user_t, _set_value) (user_t * hash, UHASH_T(user_t, node) * node, value_t * value) { \
		uhash_idx_t i; \
		void * v ; \
		switch (node->value) { \
		case 0: \
			if (!value) break; \
			i = ulist_append(&(hash->values), value); \
			if (i == ulist_invalid_idx_t) break; \
			v = ulist_get(&(hash->values), i); \
			if (hash->value_constructor) \
				hash->value_constructor(v); \
			node->value = _uhash_idx_pub(i); \
			break; \
		default: \
			i = _uhash_idx_int(node->value); \
			v = ulist_get(&(hash->values), i); \
			if (hash->value_destructor) \
				hash->value_destructor(v); \
			ulist_set(&(hash->values), i, value); \
			if (!value) \
				node->value = 0; \
			else \
			if (hash->value_constructor) \
				hash->value_constructor(v); \
		} \
	} \
	\
	static void \
	UHASH_P(user_t, set_value) (user_t * hash, uhash_idx_t node_index, value_t * value) { \
		UHASH_T(user_t, node) * node = UHASH_C(user_t, node, hash, node_index); \
		if (!node) return; \
		UHASH_C(user_t, _set_value, hash, node, value); \
	}


#define _UHASH_PROC__INIT_NODE__TYPE2(user_t, key_t, value_t) \
	static inline void \
	UHASH_P(user_t, _init_node) (user_t * hash, UHASH_T(user_t, node) * node, key_t key, value_t * value) { \
		node->depth = 1; \
		UHASH_C(user_t, _set_key, hash, node, key); \
		UHASH_C(user_t, _set_value, hash, node, value); \
	}

#define _UHASH_PROC__NODE_VISITOR__TYPE2(user_t) \
	static inline int \
	UHASH_P(user_t, _node_visitor) (const user_t * hash, const UHASH_T(user_t, node) * node, UHASH_T(user_t, node_proc) visitor) { \
		return visitor( \
			UHASH_C(user_t, _key, hash, node), \
			UHASH_C(user_t, _value, hash, node) \
		); \
	}

#define _UHASH_PROCIMPL_INIT__TYPE2(user_t, value_t) \
	{ \
	_UHASH_PROCIMPL_INIT__TYPE0(user_t) \
	ulist_init(&hash->values, sizeof(value_t)); \
	}

#define _UHASH_PROC_INIT__TYPE2(user_t, value_t) \
	static void \
	UHASH_P(user_t, init) (user_t * hash) \
		_UHASH_PROCIMPL_INIT__TYPE2(user_t, value_t)

#define _UHASH_PROCIMPL_FREE__TYPE2(user_t) \
	{ \
	if (hash->value_destructor) \
		for (uint32_t i = 0; i < hash->values.used; i++) \
			hash->value_destructor(ulist_get(&hash->values, i)); \
	ulist_free(&hash->values); \
	_UHASH_PROCIMPL_FREE__TYPE0(user_t) \
	}

#define _UHASH_PROC_FREE__TYPE2(user_t) \
	static void \
	UHASH_P(user_t, free) (user_t * hash) \
		_UHASH_PROCIMPL_FREE__TYPE2(user_t)


#define _UHASH_PROC_SEARCH__TYPE2(user_t, key_t) \
	_UHASH_PROC_SEARCH__TYPE1(user_t, key_t)

#define _UHASH_PROC_INSERT__TYPE2(user_t, key_t, value_t) \
	static uhash_idx_t \
	UHASH_P(user_t, insert) (user_t * hash, key_t key, value_t * value) \
		_UHASH_PROCIMPL_INSERT(user_t)


#define UHASH_DEFINE__TYPE2(user_t, key_t, value_t) \
	_UHASH_TYPEPROC_NODE_VISITOR__TYPE2(user_t, key_t, value_t) \
	_UHASH_TYPEPROC_KEY_VISITOR(user_t, key_t) \
	_UHASH_TYPEPROC_VALUE_VISITOR(user_t, value_t) \
	_UHASH_TYPEPROC_CMP_KEY_PLAIN(user_t, key_t) \
	\
	_UHASH_TYPE_NODE__TYPE2(user_t, key_t, value_t) \
	typedef struct { \
		_UHASH_TYPEIMPL__TYPE2(user_t) \
	} user_t; \
	\
	_UHASH_PROC_NODE(user_t) \
	_UHASH_PROC_KEY__TYPE2(user_t, key_t) \
	_UHASH_PROC_VALUE__TYPE2(user_t, value_t) \
	\
	_UHASH_PROC__INIT_NODE__TYPE2(user_t, key_t, value_t) \
	_UHASH_PROC__NODE_VISITOR__TYPE2(user_t) \
	_UHASH_PROC_INIT__TYPE2(user_t, value_t) \
	_UHASH_PROC_FREE__TYPE2(user_t) \
	\
	_UHASH_PROC_RELA_INDEX(user_t) \
	_UHASH_PROC_RELA_NODE(user_t) \
	_UHASH_PROC_LEFT(user_t) \
	_UHASH_PROC_RIGHT(user_t) \
	_UHASH_PROC_DEPTH(user_t) \
	_UHASH_PROC_TREE_DEPTH(user_t) \
	_UHASH_PROC_BALANCE_FACTOR(user_t) \
	_UHASH_PROC_UPDATE_DEPTH(user_t) \
	_UHASH_PROC_ROTATE_LEFT(user_t) \
	_UHASH_PROC_ROTATE_RIGHT(user_t) \
	_UHASH_PROC_ROTATE(user_t) \
	_UHASH_PROC_REBALANCE(user_t) \
	_UHASH_PROC_WALK(user_t) \
	\
	_UHASH_PROC_SEARCH__TYPE2(user_t, key_t) \
	_UHASH_PROC_INSERT__TYPE2(user_t, key_t, value_t) \


#endif /* UHASH_TYPE2_H */
