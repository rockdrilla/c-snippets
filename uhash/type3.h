/* uhash: simple grow-only hash
 *
 * - uhash "type 3": both key and value are "aggregate"
 *
 * refs:
 * - [1] https://github.com/etherealvisage/avl
 * - [2] https://github.com/DanielGibson/Snippets
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef HEADER_INCLUDED_UHASH_TYPE3
#define HEADER_INCLUDED_UHASH_TYPE3 1

#include "type2.h"

#define _UHASH_NAMEPROC_NODE_VISITOR__TYPE3(user_t, key_t, value_t) \
	typedef int (* UHASH_NAME(user_t, node_proc) ) (const key_t * key, const value_t * value);

#define _UHASH_NAME_NODE__TYPE3(user_t) \
	typedef struct UHASH_NAME(user_t, node) { \
		uhash_idx_t  left, right; \
		int          depth; \
		uhash_idx_t  key, value; \
	} UHASH_NAME(user_t, node);

/*
#define _UHASH_NAMEIMPL__TYPE3(user_t) \
	_UHASH_NAMEIMPL__TYPE2(user_t) \
	ulist_t  keys;
*/
#define _UHASH_NAMEIMPL__TYPE3(user_t) \
	ulist_t                      nodes; \
	uhash_idx_t                  tree_root; \
	ulist_t                      keys; \
	UHASH_NAME(user_t, key_cmp)     key_comparator; \
	ulist_t                      values; \
	UHASH_NAME(user_t, value_proc)  value_constructor; \
	UHASH_NAME(user_t, value_proc)  value_destructor;


#define _UHASH_PROC_KEY__TYPE3(user_t, key_t) \
	static inline const key_t * \
	UHASH_PROC_INT(user_t, raw_key) (const user_t * hash, uhash_idx_t index) { \
		return ulist_get(&hash->keys, _uhash_idx_int(index)); \
	} \
	\
	static inline const key_t * \
	UHASH_PROC_INT(user_t, key) (const user_t * hash, const UHASH_NAME(user_t, node) * node) { \
		return (node->key) ? UHASH_CALL_INT(user_t, raw_key, hash, node->key) : NULL; \
	} \
	\
	static const key_t * \
	UHASH_PROC(user_t, key) (const user_t * hash, uhash_idx_t node_index) { \
		const UHASH_NAME(user_t, node) * node = UHASH_CALL(user_t, cnode, hash, node_index); \
		if (!node) return NULL; \
		return UHASH_CALL_INT(user_t, key, hash, node); \
	} \
	\
	static void \
	UHASH_PROC_INT(user_t, set_key) (user_t * hash, UHASH_NAME(user_t, node) * node, key_t * key) { \
		uhash_idx_t i; \
		switch (node->key) { \
		case 0: \
			if (!key) break; \
			i = ulist_append(&(hash->keys), key); \
			if (i == ulist_invalid_idx_t) break; \
			node->key = _uhash_idx_pub(i); \
			break; \
		default: \
			i = _uhash_idx_int(node->key); \
			ulist_set(&(hash->keys), i, key); \
			if (!key) \
				node->key = 0; \
		} \
	} \
	\
	static void \
	UHASH_PROC(user_t, set_key) (user_t * hash, uhash_idx_t node_index, key_t * key) { \
		UHASH_NAME(user_t, node) * node = UHASH_CALL(user_t, node, hash, node_index); \
		if (!node) return; \
		UHASH_CALL_INT(user_t, set_key, hash, node, key); \
	}


#define _UHASH_PROC_VALUE__TYPE3(user_t, value_t) \
	_UHASH_PROC_VALUE__TYPE2(user_t, value_t)


#define _UHASH_PROC__INIT_NODE__TYPE3(user_t, key_t, value_t) \
	static inline void \
	UHASH_PROC_INT(user_t, init_node) (user_t * hash, UHASH_NAME(user_t, node) * node, key_t * key, value_t * value) { \
		node->depth = 1; \
		UHASH_CALL_INT(user_t, set_key, hash, node, key); \
		UHASH_CALL_INT(user_t, set_value, hash, node, value); \
	}

#define _UHASH_PROC__NODE_VISITOR__TYPE3(user_t) \
	static inline int \
	UHASH_PROC_INT(user_t, node_visitor) (const user_t * hash, const UHASH_NAME(user_t, node) * node, UHASH_NAME(user_t, node_proc) visitor) { \
		return visitor( \
			UHASH_CALL_INT(user_t, key, hash, node), \
			UHASH_CALL_INT(user_t, value, hash, node) \
		); \
	}

#define _UHASH_PROCIMPL_INIT__TYPE3(user_t, key_t, value_t) \
	{ \
	_UHASH_PROCIMPL_INIT__TYPE2(user_t, value_t) \
	ulist_init(&hash->keys, sizeof(key_t)); \
	}

#define _UHASH_PROC_INIT__TYPE3(user_t, key_t, value_t) \
	static void \
	UHASH_PROC(user_t, init) (user_t * hash) \
		_UHASH_PROCIMPL_INIT__TYPE3(user_t, key_t, value_t)

#define _UHASH_PROCIMPL_FREE__TYPE3(user_t) \
	{ \
	ulist_free(&hash->keys); \
	_UHASH_PROCIMPL_FREE__TYPE2(user_t) \
	}

#define _UHASH_PROC_FREE__TYPE3(user_t) \
	static void \
	UHASH_PROC(user_t, free) (user_t * hash) \
		_UHASH_PROCIMPL_FREE__TYPE3(user_t)


#define _UHASH_PROC_SEARCH__TYPE3(user_t, key_t) \
	static uhash_idx_t \
	UHASH_PROC(user_t, search) (user_t * hash, key_t * key) \
		_UHASH_PROCIMPL_SEARCH(user_t)

#define _UHASH_PROC_INSERT__TYPE3(user_t, key_t, value_t) \
	static uhash_idx_t \
	UHASH_PROC(user_t, insert) (user_t * hash, key_t * key, value_t * value) \
		_UHASH_PROCIMPL_INSERT(user_t)


#define UHASH_DEFINE_TYPE3(user_t, key_t, value_t) \
	_UHASH_NAMEPROC_NODE_VISITOR__TYPE3(user_t, key_t, value_t) \
	_UHASH_NAMEPROC_KEY_VISITOR(user_t, key_t) \
	_UHASH_NAMEPROC_VALUE_VISITOR(user_t, value_t) \
	_UHASH_NAMEPROC_CMP_KEY_PTR(user_t, key_t) \
	\
	_UHASH_NAME_NODE__TYPE3(user_t) \
	typedef struct { \
		_UHASH_NAMEIMPL__TYPE3(user_t) \
	} user_t; \
	\
	_UHASH_PROC_NODE(user_t) \
	_UHASH_PROC_KEY__TYPE3(user_t, key_t) \
	_UHASH_PROC_VALUE__TYPE3(user_t, value_t) \
	\
	_UHASH_PROC__INIT_NODE__TYPE3(user_t, key_t, value_t) \
	_UHASH_PROC__NODE_VISITOR__TYPE3(user_t) \
	_UHASH_PROC_INIT__TYPE3(user_t, key_t, value_t) \
	_UHASH_PROC_FREE__TYPE3(user_t) \
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
	_UHASH_PROC_SEARCH__TYPE3(user_t, key_t) \
	_UHASH_PROC_INSERT__TYPE3(user_t, key_t, value_t) \


#endif /* HEADER_INCLUDED_UHASH_TYPE3 */