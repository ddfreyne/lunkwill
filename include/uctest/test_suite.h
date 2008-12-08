/*
 * test_suite.h
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

#ifndef __UCTEST_TEST_SUITE_H__
#define __UCTEST_TEST_SUITE_H__

#include <uctest/test.h>

/* test suite structure */
typedef struct uc_suite_s uc_suite_t;

#pragma mark -

/* creates test suite */
uc_suite_t *uc_suite_create(char *a_name);

/* creates test suite with callbacks */
uc_suite_t *uc_suite_create_with_callbacks(char *a_name, uc_callback_t a_up_callback, uc_callback_t a_down_callback);

/* deletes test suite */
void uc_suite_destroy(uc_suite_t *a_test_suite);

#pragma mark -

/* sets up callback */
void uc_suite_set_up_callback(uc_suite_t *a_test_suite, uc_callback_t a_up_callback);

/* sets down callback */
void uc_suite_set_down_callback(uc_suite_t *a_test_suite, uc_callback_t a_down_callback);

#pragma mark -

/* adds test to test suite */
int uc_suite_add_test(uc_suite_t *a_test_suite, uc_test_t *a_test);

#pragma mark -

/* runs all tests in test suite */
void uc_suite_run(uc_suite_t *a_test_suite);

#endif
