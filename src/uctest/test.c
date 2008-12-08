/*
 * test.c
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uctest/uctest.h>
#include <uctest/private.h>

uc_test_t *g_uc_test_current = NULL;

#pragma mark -

uc_test_t *uc_test_create(char *a_name, uc_callback_t a_callback)
{
	return uc_test_create_with_callbacks(a_name, a_callback, NULL, NULL);
}

uc_test_t *uc_test_create_with_callbacks(char *a_name, uc_callback_t a_callback, uc_callback_t a_up_callback, uc_callback_t a_down_callback)
{
	/* create test */
	uc_test_t *test = malloc(sizeof(uc_test_t));
	if(!test)
		return NULL;
	
	/* set name */
	test->name = uc_strdup(a_name);
	if(!test->name)
	{
		free(test);
		return NULL;
	}
	
	/* set failures */
	test->failures_count	= 0;
	test->failures_capacity	= 5;
	test->failures			= malloc(test->failures_capacity*sizeof(uc_failure_t *));
	
	/* set callbacks */
	test->callback		= a_callback;
	test->up_callback	= a_up_callback;
	test->down_callback	= a_down_callback;
	
	return test;
}

void uc_test_destroy(uc_test_t *a_test)
{
	/* free name */
	free(a_test->name);
	
	/* free failures */
	for(size_t i = 0; i < a_test->failures_count; ++i)
		free(a_test->failures[i]);
	free(a_test->failures);
	
	/* free test */
	free(a_test);
}

#pragma mark -

inline void uc_test_set_up_callback(uc_test_t *a_test, uc_callback_t a_up_callback)
{
	a_test->up_callback = a_up_callback;
}

inline void uc_test_set_down_callback(uc_test_t *a_test, uc_callback_t a_down_callback)
{
	a_test->down_callback = a_down_callback;
}

#pragma mark -

void uc_test_add_failure(uc_test_t *a_test, uc_failure_t *a_failure)
{
	/* resize array if necessary */
	if(a_test->failures_capacity == a_test->failures_count)
	{
		uc_failure_t **failures = realloc(a_test->failures, a_test->failures_capacity*2*sizeof(uc_failure_t *));
		if(!failures)
			return;
		a_test->failures = failures;
		a_test->failures_capacity *= 2;
	}
	
	/* add failure */
	a_test->failures[a_test->failures_count] = a_failure;
	++a_test->failures_count;
}

#pragma mark -

void uc_test_run(uc_test_t *a_test)
{
	/* set current test */
	g_uc_test_current = a_test;
	
	/* set up */
	if(a_test->up_callback)
		a_test->up_callback();
	
	/* test */
	a_test->callback();
	
	/* print */
	if(a_test->failures_count > 0)
		putchar('F');
	else
		putchar('.');
	
	/* tear down */
	if(a_test->down_callback)
		a_test->down_callback();
	
	/* unset current test */
	g_uc_test_current = NULL;
}
