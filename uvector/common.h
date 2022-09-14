/* uvector: dynamic array
 *
 * - common definitions
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef HEADER_INCLUDED_UVECTOR_COMMON
#define HEADER_INCLUDED_UVECTOR_COMMON 1

// #define UVECTOR_LEGACY

#ifdef GCC_VERSION
  #if GCC_VERSION < 80000
    #define _UVECTOR_LEGACY 1
  #endif
#endif /* GCC_VERSION */

#ifndef UVECTOR_LEGACY
  #ifndef _UVECTOR_LEGACY
  #define _UVECTOR_LEGACY 0
  #endif
#else /* UVECTOR_LEGACY */
  #ifdef _UVECTOR_LEGACY
  #undef _UVECTOR_LEGACY
  #endif
  #define _UVECTOR_LEGACY 1
#endif /* UVECTOR_LEGACY */

#include "../misc/memfun.h"
#include "../misc/kustom.h"

#define UVECTOR_NAME(t, k)  KUSTOM_NAME(uvector, t, k)
#define UVECTOR_PROC        KUSTOM_PROC
#define UVECTOR_CALL        KUSTOM_CALL
#define UVECTOR_PROC_INT    KUSTOM_PROC_INT
#define UVECTOR_CALL_INT    KUSTOM_CALL_INT

#if ! _UVECTOR_LEGACY

#define _UVECTOR_DEFINE_CONSTANT(user_t, index_t, value_t) \
	static const size_t UVECTOR_NAME(user_t, item_size)  = sizeof(value_t); \
	static const size_t UVECTOR_NAME(user_t, align_size) = MEMFUN_MACRO_ALIGN(UVECTOR_NAME(user_t, item_size)); \
	static const size_t UVECTOR_NAME(user_t, pad_size)   = UVECTOR_NAME(user_t, align_size) - UVECTOR_NAME(user_t, item_size); \
	static const size_t UVECTOR_NAME(user_t, growth)     = MEMFUN_MACRO_CALC_GROWTH(UVECTOR_NAME(user_t, align_size)); \
	static const int    UVECTOR_NAME(user_t, align_bits) = GETMSB_MACRO32(UVECTOR_NAME(user_t, item_size)); \
	\
	static const int     UVECTOR_NAME(user_t, idx_bits_r) = sizeof(index_t) * CHAR_BIT; \
	static const index_t UVECTOR_NAME(user_t, idx_max_r)  = ~((index_t) 0); \
	\
	static const int     UVECTOR_NAME(user_t, idx_bits)     = UVECTOR_NAME(user_t, idx_bits_r) - UVECTOR_NAME(user_t, align_bits); \
	static const index_t UVECTOR_NAME(user_t, idx_max)      = UVECTOR_NAME(user_t, idx_max_r) >> UVECTOR_NAME(user_t, align_bits); \
	static const index_t UVECTOR_NAME(user_t, idx_inv)      = UVECTOR_NAME(user_t, idx_max_r); \
	static const index_t UVECTOR_NAME(user_t, idx_max_safe) = UVECTOR_NAME(user_t, idx_max) >> 1; \
	\
	static const index_t UVECTOR_NAME(user_t, idx_mask_inv)   = UVECTOR_NAME(user_t, idx_max_r) & ~(UVECTOR_NAME(user_t, idx_max)); \
	static const index_t UVECTOR_NAME(user_t, idx_mask_wfall) = UVECTOR_NAME(user_t, idx_max)   & ~(UVECTOR_NAME(user_t, idx_max_safe)); \

#else /* _UVECTOR_LEGACY */

#define _UVECTOR_ALIGN_SIZE(value_t)  (MEMFUN_MACRO_ALIGN(sizeof(value_t)))
#define _UVECTOR_GROWTH(value_t)      (MEMFUN_MACRO_CALC_GROWTH(_UVECTOR_ALIGN_SIZE(value_t)))
#define _UVECTOR_ALIGN_BITS(value_t)  (GETMSB_MACRO32(sizeof(value_t)))

#define _UVECTOR_IDX_BITS_R(index_t)    (sizeof(index_t) * CHAR_BIT)
#define _UVECTOR_IDX_MAX_R(index_t)     (~((index_t) 0))

#define _UVECTOR_IDX_MAX(index_t, value_t)       (_UVECTOR_IDX_MAX_R(index_t) >> _UVECTOR_ALIGN_BITS(value_t))
#define _UVECTOR_IDX_MAX_SAFE(index_t, value_t)  (_UVECTOR_IDX_MAX(index_t, value_t) >> 1)

#define _UVECTOR_DEFINE_CONSTANT(user_t, index_t, value_t) \
	static const size_t UVECTOR_NAME(user_t, item_size)  = sizeof(value_t); \
	static const size_t UVECTOR_NAME(user_t, align_size) = _UVECTOR_ALIGN_SIZE(value_t); \
	static const size_t UVECTOR_NAME(user_t, pad_size)   = _UVECTOR_ALIGN_SIZE(value_t) - sizeof(value_t); \
	static const size_t UVECTOR_NAME(user_t, growth)     = _UVECTOR_GROWTH(value_t); \
	static const int    UVECTOR_NAME(user_t, align_bits) = _UVECTOR_ALIGN_BITS(value_t); \
	\
	static const int     UVECTOR_NAME(user_t, idx_bits_r) = _UVECTOR_IDX_BITS_R(index_t); \
	static const index_t UVECTOR_NAME(user_t, idx_max_r)  = _UVECTOR_IDX_MAX_R(index_t); \
	\
	static const int     UVECTOR_NAME(user_t, idx_bits)     = _UVECTOR_IDX_BITS_R(index_t) - _UVECTOR_ALIGN_BITS(value_t); \
	static const index_t UVECTOR_NAME(user_t, idx_max)      = _UVECTOR_IDX_MAX(index_t, value_t); \
	static const index_t UVECTOR_NAME(user_t, idx_inv)      = _UVECTOR_IDX_MAX_R(index_t); \
	static const index_t UVECTOR_NAME(user_t, idx_max_safe) = _UVECTOR_IDX_MAX_SAFE(index_t, value_t); \
	\
	static const index_t UVECTOR_NAME(user_t, idx_mask_inv)   = _UVECTOR_IDX_MAX_R(index_t) & ~(_UVECTOR_IDX_MAX(index_t, value_t)); \
	static const index_t UVECTOR_NAME(user_t, idx_mask_wfall) = _UVECTOR_IDX_MAX(index_t, value_t) & ~(_UVECTOR_IDX_MAX_SAFE(index_t, value_t)); \

#endif /* _UVECTOR_LEGACY */

#define _UVECTOR_DEFINE_TYPE(user_t, index_t, value_t) \
	typedef struct { \
		union { \
		char     bytes[ /* UVECTOR_NAME(user_t, align_size) */ MEMFUN_MACRO_ALIGN(sizeof(value_t)) ]; \
		value_t  dummy; \
		} _;\
	} UVECTOR_NAME(user_t, val_t); \
	\
	typedef struct { \
	  UVECTOR_NAME(user_t, val_t) * ptr; \
	  index_t                       used, allocated; \
	} user_t;


#define _UVECTOR_DEFINE_VISITOR_PROC(user_t, index_t, value_t) \
	typedef void (* UVECTOR_NAME(user_t,  visitor))    (index_t index,       value_t * value); \
	typedef void (* UVECTOR_NAME(user_t, cvisitor))    (index_t index, const value_t * value); \
	typedef void (* UVECTOR_NAME(user_t,  visitor_ex)) (index_t index,       value_t * value, void * state); \
	typedef void (* UVECTOR_NAME(user_t, cvisitor_ex)) (index_t index, const value_t * value, void * state); \


#define _UVECTOR_PROC_INIT(user_t) \
	static void \
	UVECTOR_PROC(user_t, init) (user_t * vector) { \
		(void) memset(vector, 0, sizeof(user_t)); \
	}


#define _UVECTOR_PROCINT_OFFSET_OF(user_t, index_t) \
	static inline size_t \
	UVECTOR_PROC_INT(user_t, offset_of) (index_t index) { \
		return (size_t) memfun_ptr_offset_ex(NULL, UVECTOR_NAME(user_t, align_size), index); \
	}


#define _UVECTOR_PROC_OFFSET_OF(user_t, index_t) \
	static size_t \
	UVECTOR_PROC(user_t, offset_of) (const user_t * vector, index_t index) { \
		if (index >= vector->allocated) \
			return UVECTOR_NAME(user_t, idx_inv); \
		return UVECTOR_CALL_INT(user_t, offset_of, index); \
	}


#define _UVECTOR_PROCINT_PTR_OF(user_t, index_t, value_t) \
	static inline value_t * \
	UVECTOR_PROC_INT(user_t, ptr_of) (const user_t * vector, index_t index) { \
		return (value_t *) memfun_ptr_offset_ex(vector->ptr, UVECTOR_NAME(user_t, align_size), index); \
	}


#define _UVECTOR_PROC_FREE(user_t) \
	static void \
	UVECTOR_PROC(user_t, free) (user_t * vector) { \
		memfun_free(vector->ptr, UVECTOR_CALL_INT(user_t, offset_of, vector->used)); \
		(void) memset(vector, 0, sizeof(user_t)); \
	}


#define _UVECTOR_PROCINT_IS_INV(user_t, index_t) \
	static inline int \
	UVECTOR_PROC(user_t, is_inv) (index_t index) { \
		return ((index & UVECTOR_NAME(user_t, idx_mask_inv)) != 0); \
	}


#define _UVECTOR_PROCINT_IS_WFALL(user_t, index_t) \
	static inline int \
	UVECTOR_PROC_INT(user_t, is_wfall) (index_t index) { \
		return ((index & UVECTOR_NAME(user_t, idx_mask_wfall)) != 0); \
	}


#define _UVECTOR_PROCINT_GROW_BY_BYTES(user_t) \
	static int \
	UVECTOR_PROC_INT(user_t, grow_by_bytes) (user_t * vector, size_t bytes) { \
		size_t _new = UVECTOR_CALL_INT(user_t, offset_of, vector->allocated); \
		void * nptr = memfun_realloc_ex(vector->ptr, &_new, bytes); \
		if ((!nptr) || (!_new)) return 0; \
		size_t _alloc = _new / UVECTOR_NAME(user_t, align_size); \
		vector->allocated = (_alloc < UVECTOR_NAME(user_t, idx_max)) ? _alloc : UVECTOR_NAME(user_t, idx_max); \
		vector->ptr = nptr; \
		return 1; \
	}


#define _UVECTOR_PROCINT_GROW_BY_COUNT(user_t, index_t) \
	static int \
	UVECTOR_PROC_INT(user_t, grow_by_count) (user_t * vector, index_t count) { \
		size_t _new = 0; \
		if (UVECTOR_CALL_INT(user_t, is_wfall, vector->allocated)) { \
			if (!uaddl(vector->allocated, count, &_new)) \
				return 0; \
		} else { \
			_new = vector->allocated + count; \
		} \
		if (UVECTOR_CALL(user_t, is_inv, _new)) \
			return 0; \
		return UVECTOR_CALL_INT(user_t, grow_by_bytes, vector, UVECTOR_CALL_INT(user_t, offset_of, count)); \
	}


#define _UVECTOR_PROC_GROW_BY_BYTES(user_t, index_t) \
	static int \
	UVECTOR_PROC(user_t, grow_by_bytes) (user_t * vector, size_t bytes) { \
		if (!bytes) return 0; \
		if (vector->allocated >= UVECTOR_NAME(user_t, idx_max)) \
			return 0; \
		return UVECTOR_CALL_INT(user_t, grow_by_bytes, vector, bytes); \
	}


#define _UVECTOR_PROC_GROW_BY_COUNT(user_t, index_t) \
	static int \
	UVECTOR_PROC(user_t, grow_by_count) (user_t * vector, index_t count) { \
		if (!count) return 0; \
		if (count >= UVECTOR_NAME(user_t, idx_max_safe)) \
			return 0; \
		if (vector->allocated >= UVECTOR_NAME(user_t, idx_max)) \
			return 0; \
		return UVECTOR_CALL_INT(user_t, grow_by_count, vector, count); \
	}


#define _UVECTOR_PROC_GROW_AUTO(user_t) \
	static int \
	UVECTOR_PROC(user_t, grow_auto) (user_t * vector) { \
		if (vector->used < vector->allocated) \
			return 1; \
		return UVECTOR_CALL(user_t, grow_by_bytes, vector, UVECTOR_NAME(user_t, growth)); \
	}


#define _UVECTOR_PROC_GET_BY_PTR(user_t, index_t, value_t) \
	static value_t * \
	UVECTOR_PROC(user_t, get_by_ptr) (const user_t * vector, index_t index) { \
		if (index >= vector->used) \
			return NULL; \
		return UVECTOR_CALL_INT(user_t, ptr_of, vector, index); \
	}


#define _UVECTOR_PROCINT_GET_BY_VAL(user_t, index_t, value_t) \
	static value_t \
	UVECTOR_PROC_INT(user_t, get_by_val) (user_t * vector, index_t index) { \
		value_t * item = UVECTOR_CALL_INT(user_t, ptr_of, vector, index); \
		return *item; \
	}


#define _UVECTOR_PROC_GET_BY_VAL(user_t, index_t, value_t) \
	static value_t \
	UVECTOR_PROC(user_t, get_by_val) (user_t * vector, index_t index) { \
		if (index >= vector->used) { \
			value_t default_value; \
			(void) memset(&default_value, 0, UVECTOR_NAME(user_t, item_size)); \
			return default_value; \
		} \
		return UVECTOR_CALL_INT(user_t, get_by_val, vector, index); \
	}


#define _UVECTOR_PROCINT_SET_BY_PTR(user_t, index_t, value_t) \
	static void \
	UVECTOR_PROC_INT(user_t, set_by_ptr) (user_t * vector, index_t index, value_t * source) { \
		void * item = UVECTOR_CALL_INT(user_t, ptr_of, vector, index); \
		if (source) \
			(void) memcpy(item, source, UVECTOR_NAME(user_t, item_size)); \
		else \
			(void) memset(item, 0, UVECTOR_NAME(user_t, item_size)); \
	}


#define _UVECTOR_PROC_SET_BY_PTR(user_t, index_t, value_t) \
	static int \
	UVECTOR_PROC(user_t, set_by_ptr) (user_t * vector, index_t index, value_t * source) { \
		if (index >= vector->used) \
			return 0; \
		UVECTOR_CALL_INT(user_t, set_by_ptr, vector, index, source); \
		return 1; \
	}


#define _UVECTOR_PROCINT_SET_BY_VAL(user_t, index_t, value_t) \
	static void \
	UVECTOR_PROC_INT(user_t, set_by_val) (user_t * vector, index_t index, value_t value) { \
		value_t * item = UVECTOR_CALL_INT(user_t, ptr_of, vector, index); \
		*item = value; \
	}


#define _UVECTOR_PROC_SET_BY_VAL(user_t, index_t, value_t) \
	static int \
	UVECTOR_PROC(user_t, set_by_val) (user_t * vector, index_t index, value_t value) { \
		if (index >= vector->used) \
			return 0; \
		UVECTOR_CALL_INT(user_t, set_by_val, vector, index, value); \
		return 1; \
	}


#define _UVECTOR_PROC_APPEND_BY_PTR(user_t, value_t) \
	static int \
	UVECTOR_PROC(user_t, append_by_ptr) (user_t * vector, value_t * source) { \
		if (!UVECTOR_CALL(user_t, grow_auto, vector)) \
			return UVECTOR_NAME(user_t, idx_inv); \
		UVECTOR_CALL_INT(user_t, set_by_ptr, vector, vector->used, source); \
		return (vector->used++); \
	}


#define _UVECTOR_PROC_APPEND_BY_VAL(user_t, value_t) \
	static int \
	UVECTOR_PROC(user_t, append_by_val) (user_t * vector, value_t value) { \
		if (!UVECTOR_CALL(user_t, grow_auto, vector)) \
			return UVECTOR_NAME(user_t, idx_inv); \
		UVECTOR_CALL_INT(user_t, set_by_val, vector, vector->used, value); \
		return (vector->used++); \
	}


#define _UVECTOR_PROC_WALK(user_t, index_t, value_t) \
	static void \
	UVECTOR_PROC(user_t, walk) (user_t * vector, UVECTOR_NAME(user_t, visitor) visitor) { \
		for (index_t i = 0; i < vector->used; i++) { \
			visitor(i, UVECTOR_CALL_INT(user_t, ptr_of, vector, i)); \
		} \
	}


#define _UVECTOR_PROC_WALK_EX(user_t, index_t, value_t) \
	static void \
	UVECTOR_PROC(user_t, walk_ex) (user_t * vector, UVECTOR_NAME(user_t, visitor_ex) visitor, void * state) { \
		for (index_t i = 0; i < vector->used; i++) { \
			visitor(i, UVECTOR_CALL_INT(user_t, ptr_of, vector, i), state); \
		} \
	}


#define _UVECTOR_PROC_CONST_WALK(user_t, index_t, value_t) \
	static void \
	UVECTOR_PROC(user_t, const_walk) (const user_t * vector, UVECTOR_NAME(user_t, cvisitor) visitor) { \
		for (index_t i = 0; i < vector->used; i++) { \
			visitor(i, (const value_t *) UVECTOR_CALL_INT(user_t, ptr_of, vector, i)); \
		} \
	}


#define _UVECTOR_PROC_CONST_WALK_EX(user_t, index_t, value_t) \
	static void \
	UVECTOR_PROC(user_t, const_walk_ex) (const user_t * vector, UVECTOR_NAME(user_t, cvisitor_ex) visitor, void * state) { \
		for (index_t i = 0; i < vector->used; i++) { \
			visitor(i, (const value_t *) UVECTOR_CALL_INT(user_t, ptr_of, vector, i), state); \
		} \
	}


#define _UVECTOR_PROC_RWALK(user_t, index_t, value_t) \
	static void \
	UVECTOR_PROC(user_t, rwalk) (user_t * vector, UVECTOR_NAME(user_t, visitor) visitor) { \
		for (index_t i = vector->used; (i--) != 0; ) { \
			visitor(i, UVECTOR_CALL_INT(user_t, ptr_of, vector, i)); \
		} \
	}


#define _UVECTOR_PROC_RWALK_EX(user_t, index_t, value_t) \
	static void \
	UVECTOR_PROC(user_t, rwalk_ex) (user_t * vector, UVECTOR_NAME(user_t, visitor_ex) visitor, void * state) { \
		for (index_t i = vector->used; (i--) != 0; ) { \
			visitor(i, UVECTOR_CALL_INT(user_t, ptr_of, vector, i), state); \
		} \
	}


#define _UVECTOR_PROC_CONST_RWALK(user_t, index_t, value_t) \
	static void \
	UVECTOR_PROC(user_t, const_rwalk) (const user_t * vector, UVECTOR_NAME(user_t, cvisitor) visitor) { \
		for (index_t i = vector->used; (i--) != 0; ) { \
			visitor(i, (const value_t *) UVECTOR_CALL_INT(user_t, ptr_of, vector, i)); \
		} \
	}


#define _UVECTOR_PROC_CONST_RWALK_EX(user_t, index_t, value_t) \
	static void \
	UVECTOR_PROC(user_t, const_rwalk_ex) (const user_t * vector, UVECTOR_NAME(user_t, cvisitor_ex) visitor, void * state) { \
		for (index_t i = vector->used; (i--) != 0; ) { \
			visitor(i, (const value_t *) UVECTOR_CALL_INT(user_t, ptr_of, vector, i), state); \
		} \
	}


#endif /* HEADER_INCLUDED_UVECTOR_COMMON */
