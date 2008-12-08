/*
 * assert.h
 * uctest
 *
 * Copyright (c) 2006-2007 Denis Defreyne
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef __UCTEST_ASSERT_H__
#define __UCTEST_ASSERT_H__

#include <stdbool.h>

extern void uc_assert(bool a_result, char *a_file, unsigned a_line, char *a_statement);

#define UC_ASSERT(x)				uc_assert((x), __FILE__, __LINE__, #x)

#define UC_ASSERT_EQUAL(x, y)		UC_ASSERT(x == y)
#define UC_ASSERT_NOT_EQUAL(x, y)	UC_ASSERT(x != y)

#define UC_ASSERT_NULL(x)			UC_ASSERT(NULL == x)
#define UC_ASSERT_NOT_NULL(x)		UC_ASSERT(NULL != x)

#endif
