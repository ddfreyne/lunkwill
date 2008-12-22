/*
 * LWValidatorTest.c
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
#include <Lunkwill/LWValidator.h>

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

static void test_create(void)
{
	LWValidator *validator = LWValidatorCreate();
	UC_ASSERT_NOT_NULL(validator);
	LWValidatorDelete(validator);
}

static void test_set_message_validation_callback(void)
{
	LWValidator *validator = LWValidatorCreate();
	UC_ASSERT_NULL(validator->messageValidationCallbacks[8]);
	LWValidatorSetMessageValidationCallback(validator, 8, &validate_valid_message);
	UC_ASSERT_NOT_NULL(validator->messageValidationCallbacks[8]);
	LWValidatorDelete(validator);
}

static void test_clear_message_validation_callbacks(void)
{
	LWValidator *validator = LWValidatorCreate();
	LWValidatorSetMessageValidationCallback(validator, 8, &validate_valid_message);
	LWValidatorClearMessageValidationCallbacks(validator);
	UC_ASSERT_NULL(validator->messageValidationCallbacks[8]);
	LWValidatorDelete(validator);
}

static void test_validate_valid_message(void)
{
	LWValidator *validator = LWValidatorCreate();
	LWMessage *message = LWMessageCreate(123, NULL);
	LWValidatorSetMessageValidationCallback(validator, 123, &validate_valid_message);
	UC_ASSERT(LWValidatorMessageIsValid(validator, message));
	LWMessageDelete(message);
	LWValidatorDelete(validator);
}

static void test_validate_invalid_message(void)
{
	LWValidator *validator = LWValidatorCreate();
	LWMessage *message = LWMessageCreate(123, NULL);
	LWValidatorSetMessageValidationCallback(validator, 123, &validate_invalid_message);
	UC_ASSERT(!LWValidatorMessageIsValid(validator, message));
	LWMessageDelete(message);
	LWValidatorDelete(validator);
}

static void test_validate_unknown_message(void)
{
	LWValidator *validator = LWValidatorCreate();
	LWMessage *message = LWMessageCreate(123, NULL);
	UC_ASSERT_NULL(validator->messageValidationCallbacks[123]);
	UC_ASSERT(LWValidatorMessageIsValid(validator, message));
	LWMessageDelete(message);
	LWValidatorDelete(validator);
}

#pragma mark -

void test_validator(void)
{
	/* create suite */
	uc_suite_t *suite = uc_suite_create("validator");

	/* add tests to suite */
	uc_suite_add_test(suite, uc_test_create("create",								&test_create));
	uc_suite_add_test(suite, uc_test_create("set message valiation callback",		&test_set_message_validation_callback));
	uc_suite_add_test(suite, uc_test_create("clear message validation callbacks",	&test_clear_message_validation_callbacks));
	uc_suite_add_test(suite, uc_test_create("validate valid message",				&test_validate_valid_message));
	uc_suite_add_test(suite, uc_test_create("validate invalid message",				&test_validate_invalid_message));
	uc_suite_add_test(suite, uc_test_create("validate unknown message",				&test_validate_unknown_message));

	/* run suite */
	uc_suite_run(suite);

	/* destroy suite */
	uc_suite_destroy(suite);
}
