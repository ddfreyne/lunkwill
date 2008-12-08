/*
 * test.h
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

#ifndef __UCTEST_TEST_H__
#define __UCTEST_TEST_H__

/* test structure */
typedef struct uc_test_s uc_test_t;

#pragma mark -

/* creates test */
uc_test_t *uc_test_create(char *a_name, uc_callback_t a_callback);

/* creates test with callbacks */
uc_test_t *uc_test_create_with_callbacks(char *a_name, uc_callback_t a_callback, uc_callback_t a_up_callback, uc_callback_t a_down_callback);

/* deletes test */
void uc_test_destroy(uc_test_t *a_test);

#pragma mark -

/* sets up callback */
void uc_test_set_up_callback(uc_test_t *a_test, uc_callback_t a_up_callback);

/* sets down callback */
void uc_test_set_down_callback(uc_test_t *a_test, uc_callback_t a_down_callback);

#pragma mark -

/* runs test */
void uc_test_run(uc_test_t *a_test);

#endif
