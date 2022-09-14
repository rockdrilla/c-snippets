/* uvector: dynamic array
 *
 * - uvector "type 1": value is "aggregate"
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef HEADER_INCLUDED_UVECTOR_TYPE1
#define HEADER_INCLUDED_UVECTOR_TYPE1

#include "common.h"

#define UVECTOR_DEFINE_TYPE1(user_t, index_t, value_t) \
	_UVECTOR_DEFINE_CONSTANT(user_t, index_t, value_t) \
	_UVECTOR_DEFINE_TYPE(user_t, index_t, value_t) \
	_UVECTOR_DEFINE_VISITOR_PROC(user_t, index_t, value_t) \
	_UVECTOR_PROC_INIT(user_t) \
	_UVECTOR_PROCINT_OFFSET_OF(user_t, index_t) \
	_UVECTOR_PROC_OFFSET_OF(user_t, index_t) \
	_UVECTOR_PROCINT_PTR_OF(user_t, index_t, value_t) \
	_UVECTOR_PROC_FREE(user_t) \
	_UVECTOR_PROCINT_IS_INV(user_t, index_t) \
	_UVECTOR_PROCINT_IS_WFALL(user_t, index_t) \
	_UVECTOR_PROCINT_GROW_BY_BYTES(user_t) \
	_UVECTOR_PROCINT_GROW_BY_COUNT(user_t, index_t) \
	_UVECTOR_PROC_GROW_BY_BYTES(user_t, index_t) \
	_UVECTOR_PROC_GROW_BY_COUNT(user_t, index_t) \
	_UVECTOR_PROC_GROW_AUTO(user_t) \
	_UVECTOR_PROC_GET_BY_PTR(user_t, index_t, value_t) \
	_UVECTOR_PROCINT_SET_BY_PTR(user_t, index_t, value_t) \
	_UVECTOR_PROC_SET_BY_PTR(user_t, index_t, value_t) \
	_UVECTOR_PROC_APPEND_BY_PTR(user_t, value_t) \
	_UVECTOR_PROC_WALK(user_t, index_t, value_t) \
	_UVECTOR_PROC_WALK_EX(user_t, index_t, value_t) \
	_UVECTOR_PROC_CONST_WALK(user_t, index_t, value_t) \
	_UVECTOR_PROC_CONST_WALK_EX(user_t, index_t, value_t) \
	_UVECTOR_PROC_RWALK(user_t, index_t, value_t) \
	_UVECTOR_PROC_RWALK_EX(user_t, index_t, value_t) \
	_UVECTOR_PROC_CONST_RWALK(user_t, index_t, value_t) \
	_UVECTOR_PROC_CONST_RWALK_EX(user_t, index_t, value_t) \


#endif /* HEADER_INCLUDED_UVECTOR_TYPE1 */
