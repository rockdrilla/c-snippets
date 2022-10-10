/* minmax: min/max
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef HEADER_INCLUDED_NUM_MINMAX
#define HEADER_INCLUDED_NUM_MINMAX 1

#include "../misc/ext-c-begin.h"

#ifndef min
#define min(a,b) (((a) > (b)) ? (b) : (a))
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

static
int min_positive(int a, int b)
{
	if (a < 1) return max(b, 0);
	if (b < 1) return max(a, 0);
	return min(a, b);
}

#include "../misc/ext-c-end.h"

#endif /* HEADER_INCLUDED_NUM_MINMAX */
