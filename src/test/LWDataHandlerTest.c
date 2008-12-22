/*
 * LWDataHandlerTest.c
 * Lunkwill
 *
 * Copyright (c) 2003-2008 Denis Defreyne, Sam Rushing
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
#include <Lunkwill/LWDataHandler.h>
#include <Lunkwill/LWValidator.h>

uint8_t gTestNumber;
uint8_t gCount;

enum {
	kTestNumberIncompleteMessage,
	kTestNumberCompleteMessage,
	kTestNumberMoreThanCompleteMessage,
	kTestNumberTwoMessages,
	kTestNumberUnrecognisedMessage,
	kTestNumberValidMessage,
	kTestNumberInvalidMessage
};

#pragma mark -

static bool validate_valid_message(LWMessage *aMessage)
{
#pragma unused (aMessage)

	return true;
}

static bool validate_invalid_message(LWMessage *aMessage)
{
#pragma unused (aMessage)

	return false;
}

#pragma mark -

static void unrecognised_message_callback(LWDataHandler *aDataHandler, LWMessage *aMessage, void *aUserInfo)
{
#pragma unused (aDataHandler, aMessage, aUserInfo)

	switch(gTestNumber)
	{
		case kTestNumberIncompleteMessage:
		case kTestNumberCompleteMessage:
		case kTestNumberMoreThanCompleteMessage:
		case kTestNumberTwoMessages:
		case kTestNumberInvalidMessage:
		case kTestNumberValidMessage:
			UC_ASSERT(false);
			break;

		case kTestNumberUnrecognisedMessage:
			UC_ASSERT(true);
			break;
	}
}

static void invalid_message_callback(LWDataHandler *aDataHandler, LWMessage *aMessage, void *aUserInfo)
{
#pragma unused (aDataHandler, aMessage, aUserInfo)

	switch(gTestNumber)
	{
		case kTestNumberIncompleteMessage:
		case kTestNumberCompleteMessage:
		case kTestNumberMoreThanCompleteMessage:
		case kTestNumberTwoMessages:
		case kTestNumberUnrecognisedMessage:
		case kTestNumberValidMessage:
			UC_ASSERT(false);
			break;

		case kTestNumberInvalidMessage:
			UC_ASSERT(true);
			break;
	}
}

static void message_callback(LWDataHandler *aDataHandler, LWMessage *aMessage, void *aUserInfo)
{
#pragma unused (aDataHandler, aMessage, aUserInfo)

	switch(gTestNumber)
	{
		case kTestNumberIncompleteMessage:
		case kTestNumberInvalidMessage:
		case kTestNumberUnrecognisedMessage:
			UC_ASSERT(false);
			break;

		case kTestNumberCompleteMessage:
			++gCount;
			if(gCount > 1)
				UC_ASSERT(false);
			else
				UC_ASSERT(true);
			break;

		case kTestNumberMoreThanCompleteMessage:
			++gCount;
			if(gCount > 1)
				UC_ASSERT(false);
			else
				UC_ASSERT(true);
			break;

		case kTestNumberTwoMessages:
			++gCount;
			if(gCount > 2)
				UC_ASSERT(false);
			else
				UC_ASSERT(true);
			break;

		case kTestNumberValidMessage:
			UC_ASSERT(true);
			break;
	}
}

#pragma mark -

static void test_create(void)
{
	LWDataHandler *dataHandler = LWDataHandlerCreate(NULL);
	UC_ASSERT_NOT_NULL(dataHandler);
	UC_ASSERT_NULL(dataHandler->userInfo);
	LWDataHandlerDelete(dataHandler);
}

static void test_create_with_user_info(void)
{
	LWDataHandler *dataHandler = LWDataHandlerCreate(&gTestNumber);
	UC_ASSERT_NOT_NULL(dataHandler);
	UC_ASSERT_EQUAL(&gTestNumber, dataHandler->userInfo);
	LWDataHandlerDelete(dataHandler);
}

static void test_set_unrecognised_message_callback(void)
{
	LWDataHandler *dataHandler = LWDataHandlerCreate(NULL);
	UC_ASSERT_NULL(dataHandler->unrecognisedMessageCallback);
	LWDataHandlerSetUnrecognisedMessageCallback(dataHandler, &unrecognised_message_callback);
	UC_ASSERT_EQUAL(&unrecognised_message_callback, dataHandler->unrecognisedMessageCallback);
	LWDataHandlerDelete(dataHandler);
}

static void test_set_invalid_message_callback(void)
{
	LWDataHandler *dataHandler = LWDataHandlerCreate(NULL);
	UC_ASSERT_NULL(dataHandler->invalidMessageCallback);
	LWDataHandlerSetInvalidMessageCallback(dataHandler, &invalid_message_callback);
	UC_ASSERT_EQUAL(&invalid_message_callback, dataHandler->invalidMessageCallback);
	LWDataHandlerDelete(dataHandler);
}

static void test_set_message_callback(void)
{
	LWDataHandler *dataHandler = LWDataHandlerCreate(NULL);
	UC_ASSERT_NULL(dataHandler->messageCallbacks[0]);
	LWDataHandlerSetMessageCallback(dataHandler, 0, &message_callback);
	UC_ASSERT_EQUAL(&message_callback, dataHandler->messageCallbacks[0]);
	LWDataHandlerDelete(dataHandler);
}

static void test_set_validator(void)
{
	LWValidator *validator = LWValidatorCreate();
	LWDataHandler *dataHandler = LWDataHandlerCreate(NULL);
	UC_ASSERT_NULL(dataHandler->validator);
	LWDataHandlerSetValidator(dataHandler, validator);
	UC_ASSERT_EQUAL(validator, dataHandler->validator);
	LWDataHandlerDelete(dataHandler);
	LWValidatorDelete(validator);
}

static void test_append_incomplete_message(void)
{
	uint8_t data[] = { 123, 2, 1 };

	gTestNumber = kTestNumberIncompleteMessage;

	LWDataHandler *dataHandler = LWDataHandlerCreate(NULL);
	LWDataHandlerSetUnrecognisedMessageCallback(dataHandler, &unrecognised_message_callback);
	LWDataHandlerSetInvalidMessageCallback(dataHandler, &invalid_message_callback);
	LWDataHandlerSetMessageCallback(dataHandler, 123, &message_callback);
	UC_ASSERT(LWDataHandlerHandleData(dataHandler, data, 3));
}

static void test_append_complete_message(void)
{
	uint8_t data[] = { 123, 2, 1, 2, 0 };

	gTestNumber = kTestNumberCompleteMessage;
	gCount = 0;

	LWDataHandler *dataHandler = LWDataHandlerCreate(NULL);
	LWDataHandlerSetUnrecognisedMessageCallback(dataHandler, &unrecognised_message_callback);
	LWDataHandlerSetInvalidMessageCallback(dataHandler, &invalid_message_callback);
	LWDataHandlerSetMessageCallback(dataHandler, 123, &message_callback);
	UC_ASSERT(LWDataHandlerHandleData(dataHandler, data, 5));
	UC_ASSERT_EQUAL(1, gCount);
}

static void test_append_more_than_complete_message(void)
{
	uint8_t data[] = { 123, 2, 1, 2, 0, 8, 2, 4 };

	gTestNumber = kTestNumberMoreThanCompleteMessage;
	gCount = 0;

	LWDataHandler *dataHandler = LWDataHandlerCreate(NULL);
	LWDataHandlerSetUnrecognisedMessageCallback(dataHandler, &unrecognised_message_callback);
	LWDataHandlerSetInvalidMessageCallback(dataHandler, &invalid_message_callback);
	LWDataHandlerSetMessageCallback(dataHandler, 123, &message_callback);
	UC_ASSERT(LWDataHandlerHandleData(dataHandler, data, 8));
	UC_ASSERT_EQUAL(1, gCount);
}

static void test_append_two_messages(void)
{
	uint8_t data[] = { 123, 2, 1, 2, 0, 123, 2, 4, 3, 0 };

	gTestNumber = kTestNumberTwoMessages;
	gCount = 0;

	LWDataHandler *dataHandler = LWDataHandlerCreate(NULL);
	LWDataHandlerSetUnrecognisedMessageCallback(dataHandler, &unrecognised_message_callback);
	LWDataHandlerSetInvalidMessageCallback(dataHandler, &invalid_message_callback);
	LWDataHandlerSetMessageCallback(dataHandler, 123, &message_callback);
	UC_ASSERT(LWDataHandlerHandleData(dataHandler, data, 10));
	UC_ASSERT_EQUAL(2, gCount);
}

static void test_append_unrecognised_message(void)
{
	uint8_t data[] = { 200, 2, 1, 2, 0 };

	gTestNumber = kTestNumberUnrecognisedMessage;

	LWDataHandler *dataHandler = LWDataHandlerCreate(NULL);
	LWDataHandlerSetUnrecognisedMessageCallback(dataHandler, &unrecognised_message_callback);
	LWDataHandlerSetInvalidMessageCallback(dataHandler, &invalid_message_callback);
	LWDataHandlerSetMessageCallback(dataHandler, 123, &message_callback);
	UC_ASSERT(LWDataHandlerHandleData(dataHandler, data, 5));
}

static void test_append_valid_message(void)
{
	uint8_t data[] = { 123, 2, 1, 2, 0 };

	gTestNumber = kTestNumberValidMessage;

	LWDataHandler *dataHandler = LWDataHandlerCreate(NULL);
	LWDataHandlerSetUnrecognisedMessageCallback(dataHandler, &unrecognised_message_callback);
	LWDataHandlerSetInvalidMessageCallback(dataHandler, &invalid_message_callback);
	LWDataHandlerSetMessageCallback(dataHandler, 123, &message_callback);
	LWValidator *validator = LWValidatorCreate();
	LWValidatorSetMessageValidationCallback(validator, 123, &validate_valid_message);
	LWDataHandlerSetValidator(dataHandler, validator);
	UC_ASSERT(LWDataHandlerHandleData(dataHandler, data, 5));
}

static void test_append_invalid_message(void)
{
	uint8_t data[] = { 123, 2, 1, 2, 0 };

	gTestNumber = kTestNumberInvalidMessage;

	LWDataHandler *dataHandler = LWDataHandlerCreate(NULL);
	LWDataHandlerSetUnrecognisedMessageCallback(dataHandler, &unrecognised_message_callback);
	LWDataHandlerSetInvalidMessageCallback(dataHandler, &invalid_message_callback);
	LWDataHandlerSetMessageCallback(dataHandler, 123, &message_callback);
	LWValidator *validator = LWValidatorCreate();
	LWValidatorSetMessageValidationCallback(validator, 123, &validate_invalid_message);
	LWDataHandlerSetValidator(dataHandler, validator);
	UC_ASSERT(LWDataHandlerHandleData(dataHandler, data, 5));
}

#pragma mark -

void test_data_handler(void)
{
	/* create suite */
	uc_suite_t *suite = uc_suite_create("data handler");

	/* add tests to suite */
	uc_suite_add_test(suite, uc_test_create("create",								&test_create));
	uc_suite_add_test(suite, uc_test_create("create with user info",				&test_create_with_user_info));
	uc_suite_add_test(suite, uc_test_create("set unrecognised message callback",	&test_set_unrecognised_message_callback));
	uc_suite_add_test(suite, uc_test_create("set invalid message callback",			&test_set_invalid_message_callback));
	uc_suite_add_test(suite, uc_test_create("set message callback",					&test_set_message_callback));
	uc_suite_add_test(suite, uc_test_create("set validator",						&test_set_validator));
	uc_suite_add_test(suite, uc_test_create("append incomplete message",			&test_append_incomplete_message));
	uc_suite_add_test(suite, uc_test_create("append complete message",				&test_append_complete_message));
	uc_suite_add_test(suite, uc_test_create("append more than complete message",	&test_append_more_than_complete_message));
	uc_suite_add_test(suite, uc_test_create("append two messages",					&test_append_two_messages));
	uc_suite_add_test(suite, uc_test_create("append unrecognised message",			&test_append_unrecognised_message));
	uc_suite_add_test(suite, uc_test_create("append valid message",					&test_append_valid_message));
	uc_suite_add_test(suite, uc_test_create("append invalid message",				&test_append_invalid_message));

	/* run suite */
	uc_suite_run(suite);

	/* destroy suite */
	uc_suite_destroy(suite);
}
