/* fgets: wrapper around fgets() with trim leading newline
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef HEADER_INCLUDED_IO_FGETS
#define HEADER_INCLUDED_IO_FGETS 1

#include <stdio.h>
#include <string.h>

static
char * fgets_trim(char * s, int n, FILE * stream)
{
	s = fgets(s, n, stream);
	if (!s) return NULL;
	s[strcspn(s, "\r\n")] = 0;
	return s;
}

#endif /* HEADER_INCLUDED_IO_FGETS */
