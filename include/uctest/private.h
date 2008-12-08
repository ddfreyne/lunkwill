/*
 * private.h
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

#ifndef __UCTEST_PRIVATE_H__
#define __UCTEST_PRIVATE_H__

#include <uctest/uctest.h>

typedef struct uc_failure_s uc_failure_t;

struct uc_suite_s {
	char			*name;
	
	uc_test_t		**tests;
	size_t			tests_count;
	size_t			tests_capacity;
	
	uc_callback_t	up_callback;
	uc_callback_t	down_callback;
	
	size_t			asserts_count_total;
	size_t			asserts_count_failed;
};

struct uc_failure_s {
	char			*file;
	unsigned		line;
	char			*statement;
};

struct uc_test_s {
	char			*name;
	
	uc_failure_t	**failures;
	size_t			failures_count;
	size_t			failures_capacity;
	
	uc_callback_t	callback;
	
	uc_callback_t	up_callback;
	uc_callback_t	down_callback;
};

#pragma mark -

extern uc_suite_t	*g_uc_suite_current;
extern uc_test_t	*g_uc_test_current;

#pragma mark -

extern uc_failure_t *uc_failure_create(char *a_file, unsigned a_line, char *a_statement);
extern void uc_test_add_failure(uc_test_t *a_test, uc_failure_t *a_failure);
extern char *uc_strdup(char *string);

#endif
