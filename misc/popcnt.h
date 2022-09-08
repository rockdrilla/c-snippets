/* popcnt: simple wrapper
 *
 * - if using "builtins" is desired (POPCNT_NO_BUILTIN is not defined):
 *   - if compiler can't optimize current call in build time:
 *     - if "popcnt" instruction is supported by CPU in runtime [determined only once]:
 * 		 - then instruction is to be "called".
 *     - if "popcnt" instruction isn't supported by CPU in runtime:
 *       - "bithacks" function is called (inspired by [1]).
 *   - if compiler can optimize current call in build time:
 *     - nothing is called (you're getting intermediate value somewhere).
 * - if using "builtins" isn't desired (POPCNT_NO_BUILTIN is defined):
 *   - "bithacks" function is called (inspired by [1]).
 *
 * Nota bene: consider NOT using POPCNT_NO_BUILTIN because compiler does greater work
 * than you're expecting (sometimes).
 *
 * refs:
 * - [1] https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
 * - [2] https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
 * - [3] https://gcc.gnu.org/onlinedocs/gcc/x86-Built-in-Functions.html
 *
 * Nota bene: code snippets from [1] are in public domain.
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef HEADER_INCLUDED_POPCNT
#define HEADER_INCLUDED_POPCNT 1

#include <limits.h>
#include <stdlib.h>

#include "kustom.h"

#ifdef __has_builtin
  #if __has_builtin(__builtin_cpu_init) && __has_builtin(__builtin_cpu_supports)
    #ifndef _POPCNT_HAVE_BUILTIN
      #define _POPCNT_HAVE_BUILTIN 1
    #endif
  #endif /* __has_builtin(__builtin_cpu_init) && __has_builtin(__builtin_cpu_supports) */
#endif /* defined __has_builtin */
#ifndef _POPCNT_HAVE_BUILTIN
  #define _POPCNT_HAVE_BUILTIN 0
#endif
#ifndef POPCNT_NO_BUILTIN
  #if _POPCNT_HAVE_BUILTIN
    #define _POPCNT_USE_BUILTIN 1
  #endif
#endif /* ! POPCNT_NO_BUILTIN */

static int popcnt(unsigned int x);
static int popcntl(unsigned long x);
static int popcntll(unsigned long long x);

#define _POPCNT_DECLARE_BITHACKS(n, t)  static inline int KUSTOM_PROC(popcnt_bithacks, n) (t x)

_POPCNT_DECLARE_BITHACKS(ui, unsigned int);
_POPCNT_DECLARE_BITHACKS(ul, unsigned long);
_POPCNT_DECLARE_BITHACKS(ull, unsigned long long);

#if _POPCNT_USE_BUILTIN

#define _POPCNT_BUILTIN_NONE  0
#define _POPCNT_BUILTIN_OK    1
#define _POPCNT_BUILTIN_NACK  2

static int _popcnt_builtin = _POPCNT_BUILTIN_NONE;

static int _popcnt_cpu_supports(void)
{
	/* ref:
	 * - https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
	 * - https://gcc.gnu.org/onlinedocs/gcc/x86-Built-in-Functions.html
	 */
	__builtin_cpu_init();
	return (__builtin_cpu_supports("popcnt"))
		? _POPCNT_BUILTIN_OK
		: _POPCNT_BUILTIN_NACK;
}

#define _POPCNT_TRY_BUILTIN(f) \
	{ while (1) { \
		if (_popcnt_builtin == _POPCNT_BUILTIN_OK) \
			return f(x); \
		if (_popcnt_builtin == _POPCNT_BUILTIN_NACK) \
			break; \
		_popcnt_builtin = _popcnt_cpu_supports(); \
	} }

#else /* ! _POPCNT_USE_BUILTIN */

#define _POPCNT_TRY_BUILTIN(f) {}

#endif /* _POPCNT_USE_BUILTIN */

static int popcnt(unsigned int x)
{
	_POPCNT_TRY_BUILTIN(__builtin_popcount)
	return KUSTOM_CALL(popcnt_bithacks, ui, x);
}

static int popcntl(unsigned long x)
{
	_POPCNT_TRY_BUILTIN(__builtin_popcountl)
	return KUSTOM_CALL(popcnt_bithacks, ul, x);
}

static int popcntll(unsigned long long x)
{
	_POPCNT_TRY_BUILTIN(__builtin_popcountll)
	return KUSTOM_CALL(popcnt_bithacks, ull, x);
}

/* ref:
 * - https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
 */

#define _POPCNT_T_m(t)   ((t) ~0)
#define _POPCNT_T_c0(t)  (_POPCNT_T_m(t) / (t) 3)
#define _POPCNT_T_c1(t)  (_POPCNT_T_m(t) / (t) 15 * (t) 3)
#define _POPCNT_T_c2(t)  (_POPCNT_T_m(t) / (t) 255 * (t) 15)
#define _POPCNT_T_c3(t)  (_POPCNT_T_m(t) / (t) 255)
#define _POPCNT_T_c4(t)  ((sizeof(t) - 1) * CHAR_BIT)

#define _POPCNT_T_R0(v,t)  ((t) (v) - (((t) (v) >> 1) & _POPCNT_T_c0(t)))
#define _POPCNT_T_R1(v,t)  ((_POPCNT_T_R0(v,t) & _POPCNT_T_c1(t)) + ((_POPCNT_T_R0(v,t) >> 2) & _POPCNT_T_c1(t)))
#define _POPCNT_T_R2(v,t)  ((_POPCNT_T_R1(v,t) + (_POPCNT_T_R1(v,t) >> 4)) & _POPCNT_T_c2(t))
#define _POPCNT_T_R3(v,t)  ((_POPCNT_T_R2(v,t) * _POPCNT_T_c3(t)) >> _POPCNT_T_c4(t))

#define POPCNT_MACRO(v, t)  _POPCNT_T_R3(v, t)

#define POPCNT_MACRO32(v)   POPCNT_MACRO(v, unsigned int)
#define POPCNT_MACRO64(v)   POPCNT_MACRO(v, unsigned long long)

#define _POPCNT_DEFINE_BITHACKS(n, t) \
	_POPCNT_DECLARE_BITHACKS(n, t) \
	{ \
		static const t c0 = _POPCNT_T_c0(t); \
		static const t c1 = _POPCNT_T_c1(t); \
		static const t c2 = _POPCNT_T_c2(t); \
		static const t c3 = _POPCNT_T_c3(t); \
		static const t c4 = _POPCNT_T_c4(t); \
		x = x - ((x >> 1) & c0); \
		x = (x & c1) + ((x >> 2) & c1); \
		x = (x + (x >> 4)) & c2; \
		x = (x * c3) >> c4; \
		return x; \
	}

_POPCNT_DEFINE_BITHACKS(ui, unsigned int)
_POPCNT_DEFINE_BITHACKS(ul, unsigned long)
_POPCNT_DEFINE_BITHACKS(ull, unsigned long long)

#endif /* HEADER_INCLUDED_POPCNT */
