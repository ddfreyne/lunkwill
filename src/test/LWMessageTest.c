/*
 * LWMessageTest.c
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

#include <uctest/uctest.h>

#include <Lunkwill/LunkwillPrivate.h>
#include <Lunkwill/LunkwillTypes.h>
#include <Lunkwill/LWArgument.h>
#include <Lunkwill/LWMessage.h>

static void test_create_with_no_arguments(void)
{
	LWMessage *message = LWMessageCreate(123, NULL);
	UC_ASSERT_NOT_NULL(message);
	UC_ASSERT_EQUAL(123, message->messageID);
	UC_ASSERT_EQUAL(0, message->argumentCount);
}

static void test_create_with_one_argument(void)
{
	LWArgument *argument = LWArgumentCreateFrom8BitUnsignedInteger(8);

	LWMessage *message = LWMessageCreate(123, argument, NULL);
	UC_ASSERT_NOT_NULL(message);
	UC_ASSERT_EQUAL(123, message->messageID);
	UC_ASSERT_EQUAL(1, message->argumentCount);
	UC_ASSERT_EQUAL(argument, message->arguments[0]);
}

static void test_create_with_two_arguments(void)
{
	LWArgument *argument1 = LWArgumentCreateFrom8BitUnsignedInteger(8);
	LWArgument *argument2 = LWArgumentCreateFrom16BitInteger(-40);

	LWMessage *message = LWMessageCreate(123, argument1, argument2, NULL);
	UC_ASSERT_NOT_NULL(message);
	UC_ASSERT_EQUAL(123, message->messageID);
	UC_ASSERT_EQUAL(2, message->argumentCount);
	UC_ASSERT_EQUAL(argument1, message->arguments[0]);
	UC_ASSERT_EQUAL(argument2, message->arguments[1]);
}

static void test_add_argument(void)
{
	LWArgument *argument1 = LWArgumentCreateFrom8BitUnsignedInteger(8);
	LWArgument *argument2 = LWArgumentCreateFrom16BitInteger(-40);

	LWMessage *message = LWMessageCreate(123, NULL);
	LWMessageAddArgument(message, argument1);
	UC_ASSERT_EQUAL(1, message->argumentCount);
	UC_ASSERT_EQUAL(argument1, message->arguments[0]);
	LWMessageAddArgument(message, argument2);
	UC_ASSERT_EQUAL(2, message->argumentCount);
	UC_ASSERT_EQUAL(argument1, message->arguments[0]);
	UC_ASSERT_EQUAL(argument2, message->arguments[1]);
}

static void test_validate(void)
{
	LWArgument *argument1 = LWArgumentCreateFrom8BitUnsignedInteger(8);
	LWArgument *argument2 = LWArgumentCreateFrom16BitInteger(-40);
	LWArgument *argument3 = LWArgumentCreateFromString("Moof");
	LWArgument *argument4 = LWArgumentCreateFromString("Once upon a time there was a novel embedded inside an argument.");
	LWMessage *message = LWMessageCreate(123, argument1, argument2, argument3, argument4, NULL);

	UC_ASSERT(LWMessageIsValid(message, -1));
	UC_ASSERT(LWMessageIsValid(message,  4, -1, -1, -1, -1));
	UC_ASSERT(LWMessageIsValid(message,  4,  1,  2, -1, -1));
	UC_ASSERT(!LWMessageIsValid(message, 4,  1,  4, -1, -1));
	UC_ASSERT(!LWMessageIsValid(message, 5,  1,  2, -1, -1, 5));
}

static void test_get_message_id(void)
{
	LWMessage *message = LWMessageCreate(123, NULL);

	UC_ASSERT_EQUAL(123, LWMessageGetMessageID(message));
}

static void test_get_argument_count(void)
{
	LWArgument *argument1 = LWArgumentCreateFrom8BitUnsignedInteger(8);
	LWArgument *argument2 = LWArgumentCreateFrom16BitInteger(-40);
	LWArgument *argument3 = LWArgumentCreateFromString("Moof");
	LWArgument *argument4 = LWArgumentCreateFromString("Once upon a time there was a novel embedded inside an argument.");
	LWMessage *message1 = LWMessageCreate(123, argument1, argument2, argument3, argument4, NULL);
	LWMessage *message2 = LWMessageCreate(123, NULL);

	UC_ASSERT_EQUAL(4, LWMessageGetArgumentCount(message1));
	UC_ASSERT_EQUAL(0, LWMessageGetArgumentCount(message2));
}

static void test_get_argument_at_index(void)
{
	LWArgument *argument1 = LWArgumentCreateFrom8BitUnsignedInteger(8);
	LWArgument *argument2 = LWArgumentCreateFrom16BitInteger(-40);
	LWArgument *argument3 = LWArgumentCreateFromString("Moof");
	LWArgument *argument4 = LWArgumentCreateFromString("Once upon a time there was a novel embedded inside an argument.");
	LWMessage *message = LWMessageCreate(123, argument1, argument2, argument3, argument4, NULL);

	UC_ASSERT_EQUAL(argument1, LWMessageGetArgumentAtIndex(message, 0));
	UC_ASSERT_EQUAL(argument2, LWMessageGetArgumentAtIndex(message, 1));
	UC_ASSERT_EQUAL(argument3, LWMessageGetArgumentAtIndex(message, 2));
	UC_ASSERT_EQUAL(argument4, LWMessageGetArgumentAtIndex(message, 3));
}

static void test_serialize_with_no_arguments(void)
{
	LWMessage *message = LWMessageCreate(123, NULL);

	void *serializedMessage;
	size_t serializedMessageLength;
	UC_ASSERT(LWMessageSerialize(message, &serializedMessageLength, &serializedMessage));
	UC_ASSERT_NOT_NULL(serializedMessage);
	UC_ASSERT_EQUAL(2, serializedMessageLength);
	UC_ASSERT_EQUAL(123, ((uint8_t *)serializedMessage)[0]);
	UC_ASSERT_EQUAL(0, ((uint8_t *)serializedMessage)[1]);
}

static void test_serialize_with_one_small_argument(void)
{
	LWArgument *argument = LWArgumentCreateFrom8BitUnsignedInteger(8);

	LWMessage *message = LWMessageCreate(123, argument, NULL);
	void *serializedMessage;
	size_t serializedMessageLength;
	UC_ASSERT(LWMessageSerialize(message, &serializedMessageLength, &serializedMessage));
	UC_ASSERT_NOT_NULL(serializedMessage);
	UC_ASSERT_EQUAL(4, serializedMessageLength);
	UC_ASSERT_EQUAL(123, ((uint8_t *)serializedMessage)[0]);
	UC_ASSERT_EQUAL(1, ((uint8_t *)serializedMessage)[1]);
	UC_ASSERT_EQUAL(8, ((uint8_t *)serializedMessage)[2]);
	UC_ASSERT_EQUAL(0, ((uint8_t *)serializedMessage)[3]);
}

static void test_serialize_with_one_medium_argument(void)
{
	char *string =	"01234567890123456789012345678901234567890123456789" 
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234";
	LWArgument *argument = LWArgumentCreateFromString(string);

	LWMessage *message = LWMessageCreate(123, argument, NULL);
	void *serializedMessage;
	size_t serializedMessageLength;
	UC_ASSERT(LWMessageSerialize(message, &serializedMessageLength, &serializedMessage));
	UC_ASSERT_NOT_NULL(serializedMessage);
	UC_ASSERT_EQUAL(259, serializedMessageLength);
	UC_ASSERT_EQUAL(123, ((uint8_t *)serializedMessage)[0]);
	UC_ASSERT_EQUAL(255, ((uint8_t *)serializedMessage)[1]);
	UC_ASSERT_EQUAL(0, ((uint8_t *)serializedMessage)[257]);
	UC_ASSERT_EQUAL(0, ((uint8_t *)serializedMessage)[258]);
}

static void test_serialize_with_one_large_argument(void)
{
	char *string =	"01234567890123456789012345678901234567890123456789" 
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789";
	LWArgument *argument = LWArgumentCreateFromString(string);

	LWMessage *message = LWMessageCreate(123, argument, NULL);
	void *serializedMessage;
	size_t serializedMessageLength;
	UC_ASSERT(LWMessageSerialize(message, &serializedMessageLength, &serializedMessage));
	UC_ASSERT_NOT_NULL(serializedMessage);
	UC_ASSERT_EQUAL(304, serializedMessageLength);
	UC_ASSERT_EQUAL(123, ((uint8_t *)serializedMessage)[0]);
	UC_ASSERT_EQUAL(255, ((uint8_t *)serializedMessage)[1]);
	UC_ASSERT_EQUAL(45, ((uint8_t *)serializedMessage)[257]);
	UC_ASSERT_EQUAL(0, ((uint8_t *)serializedMessage)[303]);
}

static void test_serialize_with_one_huge_argument(void)
{
	char *string =	"01234567890123456789012345678901234567890123456789" 
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789"
					"01234567890123456789012345678901234567890123456789";
	LWArgument *argument = LWArgumentCreateFromString(string);

	LWMessage *message = LWMessageCreate(123, argument, NULL);
	void *serializedMessage;
	size_t serializedMessageLength;
	UC_ASSERT(LWMessageSerialize(message, &serializedMessageLength, &serializedMessage));
	UC_ASSERT_NOT_NULL(serializedMessage);
	UC_ASSERT_EQUAL(705, serializedMessageLength);
	UC_ASSERT_EQUAL(123, ((uint8_t *)serializedMessage)[0]);
	UC_ASSERT_EQUAL(255, ((uint8_t *)serializedMessage)[1]);
	UC_ASSERT_EQUAL(255, ((uint8_t *)serializedMessage)[257]);
	UC_ASSERT_EQUAL(190, ((uint8_t *)serializedMessage)[513]);
	UC_ASSERT_EQUAL(0, ((uint8_t *)serializedMessage)[704]);
}

static void test_serialize_with_two_arguments(void)
{
	LWArgument *argument1 = LWArgumentCreateFrom8BitUnsignedInteger(8);
	LWArgument *argument2 = LWArgumentCreateFrom8BitUnsignedInteger(4);

	LWMessage *message = LWMessageCreate(123, argument1, argument2, NULL);
	void *serializedMessage;
	size_t serializedMessageLength;
	UC_ASSERT(LWMessageSerialize(message, &serializedMessageLength, &serializedMessage));
	UC_ASSERT_NOT_NULL(serializedMessage);
	UC_ASSERT_EQUAL(6, serializedMessageLength);
	UC_ASSERT_EQUAL(123, ((uint8_t *)serializedMessage)[0]);
	UC_ASSERT_EQUAL(1, ((uint8_t *)serializedMessage)[1]);
	UC_ASSERT_EQUAL(8, ((uint8_t *)serializedMessage)[2]);
	UC_ASSERT_EQUAL(1, ((uint8_t *)serializedMessage)[3]);
	UC_ASSERT_EQUAL(4, ((uint8_t *)serializedMessage)[4]);
	UC_ASSERT_EQUAL(0, ((uint8_t *)serializedMessage)[5]);
}

static void test_deserialize_with_no_arguments(void)
{
	uint8_t data[] = { 123, 0 };

	size_t bytesUsed;
	LWMessage *message = LWMessageDeserialize(data, 2, &bytesUsed);
	UC_ASSERT_NOT_NULL(message);
	UC_ASSERT_EQUAL(2, bytesUsed);
	UC_ASSERT_EQUAL(123, message->messageID);
	UC_ASSERT_EQUAL(0, message->argumentCount);
}

static void test_deserialize_with_one_small_argument(void)
{
	uint8_t data[] = { 123, 2, 1, 2, 0 };

	size_t bytesUsed;
	LWMessage *message = LWMessageDeserialize(data, 5, &bytesUsed);
	UC_ASSERT_NOT_NULL(message);
	UC_ASSERT_EQUAL(5, bytesUsed);
	UC_ASSERT_EQUAL(123, message->messageID);
	UC_ASSERT_EQUAL(1, message->argumentCount);
	LWArgument *argument = LWMessageGetArgumentAtIndex(message, 0);
	UC_ASSERT_NOT_NULL(argument);
	UC_ASSERT_EQUAL(2, argument->length);
	UC_ASSERT_EQUAL(1, argument->data[0]);
	UC_ASSERT_EQUAL(2, argument->data[1]);
}

static void test_deserialize_with_one_medium_argument(void)
{
	uint8_t data[] = {	123,
						255,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4,
						0,
						0 };

	size_t bytesUsed;
	LWMessage *message = LWMessageDeserialize(data, 259, &bytesUsed);
	UC_ASSERT_NOT_NULL(message);
	UC_ASSERT_EQUAL(259, bytesUsed);
	UC_ASSERT_EQUAL(123, message->messageID);
	UC_ASSERT_EQUAL(1, message->argumentCount);
	LWArgument *argument = LWMessageGetArgumentAtIndex(message, 0);
	UC_ASSERT_NOT_NULL(argument);
	UC_ASSERT_EQUAL(255, argument->length);
	UC_ASSERT_EQUAL(0, argument->data[0]);
	UC_ASSERT_EQUAL(1, argument->data[1]);
	UC_ASSERT_EQUAL(3, argument->data[253]);
	UC_ASSERT_EQUAL(4, argument->data[254]);
}

static void test_deserialize_with_one_large_argument(void)
{
	uint8_t data[] = {	123,
						255,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0, 1, 2, 3, 4,
						10,
						0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
						0 };

	size_t bytesUsed;
	LWMessage *message = LWMessageDeserialize(data, 269, &bytesUsed);
	UC_ASSERT_NOT_NULL(message);
	UC_ASSERT_EQUAL(269, bytesUsed);
	UC_ASSERT_EQUAL(123, message->messageID);
	UC_ASSERT_EQUAL(1, message->argumentCount);
	LWArgument *argument = LWMessageGetArgumentAtIndex(message, 0);
	UC_ASSERT_NOT_NULL(argument);
	UC_ASSERT_EQUAL(265, argument->length);
	UC_ASSERT_EQUAL(0, argument->data[0]);
	UC_ASSERT_EQUAL(1, argument->data[1]);
	UC_ASSERT_EQUAL(3, argument->data[253]);
	UC_ASSERT_EQUAL(4, argument->data[254]);
	UC_ASSERT_EQUAL(8, argument->data[263]);
	UC_ASSERT_EQUAL(9, argument->data[264]);
}

static void test_deserialize_with_two_arguments(void)
{
	uint8_t data[] = { 123, 2, 1, 2, 1, 3, 0 };

	size_t bytesUsed;
	LWMessage *message = LWMessageDeserialize(data, 7, &bytesUsed);
	UC_ASSERT_NOT_NULL(message);
	UC_ASSERT_EQUAL(7, bytesUsed);
	UC_ASSERT_EQUAL(123, message->messageID);
	UC_ASSERT_EQUAL(2, message->argumentCount);
	LWArgument *argument1 = LWMessageGetArgumentAtIndex(message, 0);
	LWArgument *argument2 = LWMessageGetArgumentAtIndex(message, 1);
	UC_ASSERT_NOT_NULL(argument1);
	UC_ASSERT_NOT_NULL(argument2);
	UC_ASSERT_EQUAL(2, argument1->length);
	UC_ASSERT_EQUAL(1, argument1->data[0]);
	UC_ASSERT_EQUAL(2, argument1->data[1]);
	UC_ASSERT_EQUAL(1, argument2->length);
	UC_ASSERT_EQUAL(3, argument2->data[0]);
}

static void test_deserialize_incomplete(void)
{
	uint8_t data[] = { 123, 2, 1, 2, 1 };

	size_t bytesUsed;
	LWMessage *message = LWMessageDeserialize(data, 5, &bytesUsed);
	UC_ASSERT_NULL(message);
}

static void test_deserialize_more_than_complete(void)
{
	uint8_t data[] = { 123, 0, 234 };

	size_t bytesUsed;
	LWMessage *message = LWMessageDeserialize(data, 3, &bytesUsed);
	UC_ASSERT_NOT_NULL(message);
	UC_ASSERT_EQUAL(2, bytesUsed);
	UC_ASSERT_EQUAL(123, message->messageID);
	UC_ASSERT_EQUAL(0, message->argumentCount);
}

#pragma mark -

void test_message(void)
{
	/* create suite */
	uc_suite_t *suite = uc_suite_create("message");

	/* add tests to suite */
	uc_suite_add_test(suite, uc_test_create("create with no arguments",				&test_create_with_no_arguments));
	uc_suite_add_test(suite, uc_test_create("create with one argument",				&test_create_with_one_argument));
	uc_suite_add_test(suite, uc_test_create("create with two arguments",			&test_create_with_two_arguments));
	uc_suite_add_test(suite, uc_test_create("add argument",							&test_add_argument));
	uc_suite_add_test(suite, uc_test_create("validate",								&test_validate));
	uc_suite_add_test(suite, uc_test_create("get message id",						&test_get_message_id));
	uc_suite_add_test(suite, uc_test_create("get argument count",					&test_get_argument_count));
	uc_suite_add_test(suite, uc_test_create("get argument at index",				&test_get_argument_at_index));
	uc_suite_add_test(suite, uc_test_create("serialize with no arguments",			&test_serialize_with_no_arguments));
	uc_suite_add_test(suite, uc_test_create("serialize with one small argument",	&test_serialize_with_one_small_argument));
	uc_suite_add_test(suite, uc_test_create("serialize with one medium argument",	&test_serialize_with_one_medium_argument));
	uc_suite_add_test(suite, uc_test_create("serialize with one large argument",	&test_serialize_with_one_large_argument));
	uc_suite_add_test(suite, uc_test_create("serialize with one huge argument",		&test_serialize_with_one_huge_argument));
	uc_suite_add_test(suite, uc_test_create("serialize with two arguments",			&test_serialize_with_two_arguments));
	uc_suite_add_test(suite, uc_test_create("deserialize with no arguments",		&test_deserialize_with_no_arguments));
	uc_suite_add_test(suite, uc_test_create("deserialize with one small argument",	&test_deserialize_with_one_small_argument));
	uc_suite_add_test(suite, uc_test_create("deserialize with one medium argument",	&test_deserialize_with_one_medium_argument));
	uc_suite_add_test(suite, uc_test_create("deserialize with one large argument",	&test_deserialize_with_one_large_argument));
	uc_suite_add_test(suite, uc_test_create("deserialize with two arguments",		&test_deserialize_with_two_arguments));
	uc_suite_add_test(suite, uc_test_create("deserialize incomplete",				&test_deserialize_incomplete));
	uc_suite_add_test(suite, uc_test_create("deserialize more than complete",		&test_deserialize_more_than_complete));

	/* run suite */
	uc_suite_run(suite);

	/* destroy suite */
	uc_suite_destroy(suite);
}
