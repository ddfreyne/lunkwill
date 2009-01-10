/*
 * LWArgumentTest.c
 * Lunkwill
 *
 * Copyright (c) 2003-2009 Denis Defreyne, Sam Rushing
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * 
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * 
 * - Neither the name of Lunkwill nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <string.h>

#include <uctest/uctest.h>

#include <Lunkwill/LunkwillPrivate.h>
#include <Lunkwill/LunkwillTypes.h>
#include <Lunkwill/LWArgument.h>

static void test_create(void)
{
	uint8_t data[] = { 7, 2, 4 };

	LWArgument *argument = LWArgumentCreate(data, 3);
	UC_ASSERT_NOT_NULL(argument);

	UC_ASSERT(argument->isRetainable);

	UC_ASSERT(argument->length == 3);
	UC_ASSERT(argument->data[0] == 7);
	UC_ASSERT(argument->data[1] == 2);
	UC_ASSERT(argument->data[2] == 4);

	data[0] = 3;

	UC_ASSERT(argument->data[0] == 7);
}

static void test_create_without_copying(void)
{
	uint8_t data[] = { 7, 2, 4 };

	LWArgument *argument = LWArgumentCreateWithoutCopying(data, 3);
	UC_ASSERT_NOT_NULL(argument);

	UC_ASSERT(argument->isRetainable);

	UC_ASSERT(argument->length == 3);
	UC_ASSERT(argument->data[0] == 7);
	UC_ASSERT(argument->data[1] == 2);
	UC_ASSERT(argument->data[2] == 4);

	data[0] = 3;

	UC_ASSERT(argument->data[0] == 3);
}

static void test_create_zero_length(void)
{
	uint8_t data[] = { 7, 2, 4 };

	LWArgument *argument1 = LWArgumentCreate(data, 0);
	UC_ASSERT_NULL(argument1);

	LWArgument *argument2 = LWArgumentCreateWithoutCopying(data, 0);
	UC_ASSERT_NULL(argument2);
}

static void test_create_from_string(void)
{
	char *string = "hello";

	LWArgument *argument = LWArgumentCreateFromString(string);
	UC_ASSERT_NOT_NULL(argument);

	UC_ASSERT(argument->isRetainable);

	UC_ASSERT(argument->length == 5);
	UC_ASSERT(argument->data[0] == 'h');
	UC_ASSERT(argument->data[1] == 'e');
	UC_ASSERT(argument->data[2] == 'l');
	UC_ASSERT(argument->data[3] == 'l');
	UC_ASSERT(argument->data[4] == 'o');
}

static void test_create_medium(void)
{
	char *string =	"01234567890123456789012345678901234567890123456789" 
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234";

	LWArgument *argument = LWArgumentCreateFromString(string);
	UC_ASSERT_NOT_NULL(argument);

	UC_ASSERT(argument->length == 255);
	UC_ASSERT(argument->data[0] == '0');
	UC_ASSERT(argument->data[1] == '1');
	UC_ASSERT(argument->data[253] == '3');
	UC_ASSERT(argument->data[254] == '4');
}

static void test_create_large(void)
{
	char *string =	"01234567890123456789012345678901234567890123456789" 
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789";

	LWArgument *argument = LWArgumentCreateFromString(string);
	UC_ASSERT_NOT_NULL(argument);

	UC_ASSERT(argument->length == 300);
	UC_ASSERT(argument->data[0] == '0');
	UC_ASSERT(argument->data[1] == '1');
	UC_ASSERT(argument->data[298] == '8');
	UC_ASSERT(argument->data[299] == '9');
}

static void test_create_from_8_bit_integer(void)
{
	LWArgument *argument = LWArgumentCreateFrom8BitInteger(-8);
	UC_ASSERT_NOT_NULL(argument);

	UC_ASSERT(argument->length == 1);
	UC_ASSERT(argument->data[0] == (uint8_t)-8);
}

static void test_create_from_8_bit_unsigned_integer(void)
{
	LWArgument *argument = LWArgumentCreateFrom8BitUnsignedInteger(8);
	UC_ASSERT_NOT_NULL(argument);

	UC_ASSERT(argument->length == 1);
	UC_ASSERT(argument->data[0] == 8);
}

static void test_create_from_16_bit_integer(void)
{
	LWArgument *argument = LWArgumentCreateFrom16BitInteger(-300);
	UC_ASSERT_NOT_NULL(argument);

	UC_ASSERT(argument->length == 2);
	UC_ASSERT((int16_t)ntohs(*(int16_t *)argument->data) == -300);
}

static void test_create_from_16_bit_unsigned_integer(void)
{
	LWArgument *argument = LWArgumentCreateFrom16BitUnsignedInteger(300);
	UC_ASSERT_NOT_NULL(argument);

	UC_ASSERT(argument->length == 2);
	UC_ASSERT((uint16_t)ntohs(*(uint16_t *)argument->data) == 300);
}

static void test_create_from_32_bit_integer(void)
{
	LWArgument *argument = LWArgumentCreateFrom32BitInteger(-70000);
	UC_ASSERT_NOT_NULL(argument);

	UC_ASSERT(argument->length == 4);
	UC_ASSERT((int32_t)ntohl(*(int32_t *)argument->data) == -70000);
}

static void test_create_from_32_bit_unsigned_integer(void)
{
	LWArgument *argument = LWArgumentCreateFrom32BitUnsignedInteger(70000);
	UC_ASSERT_NOT_NULL(argument);

	UC_ASSERT(argument->length == 4);
	UC_ASSERT((int32_t)ntohl(*(int32_t *)argument->data) == 70000);
}

static void test_get_data(void)
{
	LWArgument *argument = LWArgumentCreateFromString("hello");

	void *data = LWArgumentGetData(argument);
	UC_ASSERT_NOT_NULL(data);
	UC_ASSERT(0 == strcmp("hello", (char *)data));
}

static void test_get_length(void)
{
	LWArgument *argument = LWArgumentCreateFromString("hello");

	UC_ASSERT_EQUAL(5, LWArgumentGetLength(argument));
}

static void test_get_string_value(void)
{
	LWArgument *argument = LWArgumentCreateFromString("hello");

	char *string = LWArgumentGetStringValue(argument);
	UC_ASSERT_NOT_NULL(string);
	UC_ASSERT_EQUAL('\0', string[5]);
	UC_ASSERT_EQUAL(5, strlen(string));
}

static void test_get_8_bit_integer_value(void)
{
	LWArgument *argument = LWArgumentCreateFrom8BitInteger(-8);

	UC_ASSERT_EQUAL(-8, LWArgumentGet8BitIntegerValue(argument));
}

static void test_get_8_bit_unsigned_integer_value(void)
{
	LWArgument *argument = LWArgumentCreateFrom8BitUnsignedInteger(8);

	UC_ASSERT_EQUAL(8, LWArgumentGet8BitUnsignedIntegerValue(argument));
}

static void test_get_16_bit_integer_value(void)
{
	LWArgument *argument = LWArgumentCreateFrom16BitInteger(-16000);

	UC_ASSERT_EQUAL(-16000, LWArgumentGet16BitIntegerValue(argument));
}

static void test_get_16_bit_unsigned_integer_value(void)
{
	LWArgument *argument = LWArgumentCreateFrom16BitUnsignedInteger(16000);

	UC_ASSERT_EQUAL(16000, LWArgumentGet16BitUnsignedIntegerValue(argument));
}

static void test_get_32_bit_integer_value(void)
{
	LWArgument *argument = LWArgumentCreateFrom32BitInteger(-32000000);

	UC_ASSERT_EQUAL(-32000000, LWArgumentGet32BitIntegerValue(argument));
}

static void test_get_32_bit_unsigned_integer_value(void)
{
	LWArgument *argument = LWArgumentCreateFrom32BitUnsignedInteger(32000000);

	UC_ASSERT_EQUAL(32000000, LWArgumentGet32BitUnsignedIntegerValue(argument));
}

#pragma mark -

void test_argument(void)
{
	/* create suite */
	uc_suite_t *suite = uc_suite_create("argument");

	/* add tests to suite */
	uc_suite_add_test(suite, uc_test_create("create",								&test_create));
	uc_suite_add_test(suite, uc_test_create("create without copying",				&test_create_without_copying));
	uc_suite_add_test(suite, uc_test_create("create zero length",					&test_create_zero_length));
	uc_suite_add_test(suite, uc_test_create("create from string",					&test_create_from_string));
	uc_suite_add_test(suite, uc_test_create("create medium",						&test_create_medium));
	uc_suite_add_test(suite, uc_test_create("create large",							&test_create_large));
	uc_suite_add_test(suite, uc_test_create("create from 8 bit integer",			&test_create_from_8_bit_integer));
	uc_suite_add_test(suite, uc_test_create("create from 8 bit unsigned integer",	&test_create_from_8_bit_unsigned_integer));
	uc_suite_add_test(suite, uc_test_create("create from 16 bit integer",			&test_create_from_16_bit_integer));
	uc_suite_add_test(suite, uc_test_create("create from 16 bit unsigned integer",	&test_create_from_16_bit_unsigned_integer));
	uc_suite_add_test(suite, uc_test_create("create from 32 bit integer",			&test_create_from_32_bit_integer));
	uc_suite_add_test(suite, uc_test_create("create from 32 bit unsigned integer",	&test_create_from_32_bit_unsigned_integer));
	uc_suite_add_test(suite, uc_test_create("get data",								&test_get_data));
	uc_suite_add_test(suite, uc_test_create("get length",							&test_get_length));
	uc_suite_add_test(suite, uc_test_create("get string value",						&test_get_string_value));
	uc_suite_add_test(suite, uc_test_create("get 8 bit integer value",				&test_get_8_bit_integer_value));
	uc_suite_add_test(suite, uc_test_create("get 8 bit unsigned integer value",		&test_get_8_bit_unsigned_integer_value));
	uc_suite_add_test(suite, uc_test_create("get 16 bit integer value",				&test_get_16_bit_integer_value));
	uc_suite_add_test(suite, uc_test_create("get 16 bit unsigned integer value",	&test_get_16_bit_unsigned_integer_value));
	uc_suite_add_test(suite, uc_test_create("get 32 bit integer value",				&test_get_32_bit_integer_value));
	uc_suite_add_test(suite, uc_test_create("get 32 bit unsigned integer value",	&test_get_32_bit_unsigned_integer_value));

	/* run suite */
	uc_suite_run(suite);

	/* destroy suite */
	uc_suite_destroy(suite);
}
