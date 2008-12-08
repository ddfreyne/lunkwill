/*
 * test_suite.c
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

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <uctest/uctest.h>
#include <uctest/private.h>

uc_suite_t *g_uc_suite_current = NULL;

#pragma mark -

uc_suite_t *uc_suite_create(char *a_name)
{
	return uc_suite_create_with_callbacks(a_name, NULL, NULL);
}

uc_suite_t *uc_suite_create_with_callbacks(char *a_name, uc_callback_t a_up_callback, uc_callback_t a_down_callback)
{
	/* create test suite */
	uc_suite_t *test_suite = malloc(sizeof(uc_suite_t));
	if(!test_suite)
		return NULL;
	
	/* set name */
	test_suite->name = uc_strdup(a_name);
	if(!test_suite->name)
	{
		free(test_suite);
		return NULL;
	}
	
	/* set tests */
	test_suite->tests_count		= 0;
	test_suite->tests_capacity	= 5;
	test_suite->tests			= malloc(test_suite->tests_capacity*sizeof(uc_test_t *));
	
	/* set counts */
	test_suite->asserts_count_total		= 0;
	test_suite->asserts_count_failed	= 0;
	
	/* set callbacks */
	test_suite->up_callback		= a_up_callback;
	test_suite->down_callback	= a_down_callback;
	
	return test_suite;
}

void uc_suite_destroy(uc_suite_t *a_test_suite)
{
	/* free tests */
	for(size_t i = 0; i < a_test_suite->tests_count; ++i)
		uc_test_destroy(a_test_suite->tests[i]);
	free(a_test_suite->tests);
	
	/* free name */
	free(a_test_suite->name);
	
	/* free test suite */
	free(a_test_suite);
}

#pragma mark -

inline void uc_suite_set_up_callback(uc_suite_t *a_test_suite, uc_callback_t a_up_callback)
{
	a_test_suite->up_callback = a_up_callback;
}

inline void uc_suite_set_down_callback(uc_suite_t *a_test_suite, uc_callback_t a_down_callback)
{
	a_test_suite->down_callback = a_down_callback;
}

#pragma mark -

int uc_suite_add_test(uc_suite_t *a_test_suite, uc_test_t *a_test)
{
	/* resize array if necessary */
	if(a_test_suite->tests_capacity == a_test_suite->tests_count)
	{
		uc_test_t **tests = realloc(a_test_suite->tests, a_test_suite->tests_capacity*2*sizeof(uc_test_t *));
		if(!tests)
			return -1;
		a_test_suite->tests = tests;
		a_test_suite->tests_capacity *= 2;
	}
	
	/* add test */
	a_test_suite->tests[a_test_suite->tests_count] = a_test;
	++a_test_suite->tests_count;
	
	return 0;
}

#pragma mark -

void uc_suite_run(uc_suite_t *a_test_suite)
{
	/* set current test suite */
	g_uc_suite_current = a_test_suite;
	
	/* set up */
	if(a_test_suite->up_callback)
		a_test_suite->up_callback();
	
	/* run tests */
	for(size_t i = 0; i < a_test_suite->tests_count; ++i)
		uc_test_run(a_test_suite->tests[i]);
	
	/* tear down */
	if(a_test_suite->down_callback)
		a_test_suite->down_callback();
	
	/* print failures */
	for(size_t i = 0; i < a_test_suite->tests_count; ++i)
	{
		uc_test_t *test = a_test_suite->tests[i];
		for(size_t j = 0; j < test->failures_count; ++j)
		{
			uc_failure_t *failure = test->failures[j];
			printf("\nFailure [%s:%u %s]: %s", failure->file, failure->line, test->name, failure->statement);
		}
	}
	
	/* print result */
	printf(
		"\n%s: %lu tests, %lu assertions, %lu failures\n\n",
		a_test_suite->name,
		a_test_suite->tests_count,
		a_test_suite->asserts_count_total,
		a_test_suite->asserts_count_failed
	);
	
	/* unset current test suite */
	g_uc_suite_current = NULL;
}
