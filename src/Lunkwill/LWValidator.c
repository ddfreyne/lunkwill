/*
 * LWValidator.c
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

#include <stdlib.h>

#include <Lunkwill/LunkwillDefines.h>
#include <Lunkwill/LunkwillTypes.h>
#include <Lunkwill/LunkwillPrivate.h>
#include <Lunkwill/LWValidator.h>

#pragma mark Creating Validators

LWValidator *LWValidatorCreate()
{
	// allocate validator
	LWValidator *validator = malloc(sizeof(LWValidator));
	if(!validator)
		return NULL;

	// clear callbacks
	for(uint16_t i = 0; i < 256; ++i)
		validator->messageValidationCallbacks[i] = NULL;

	return validator;
}

#pragma mark -
#pragma mark Deleting Validators

void LWValidatorDelete(LWValidator *aValidator)
{
	// delete validator
	free(aValidator);
}

#pragma mark -
#pragma mark Setting Validation Rules

void LWValidatorSetMessageValidationCallback(LWValidator *aValidator, uint8_t aMessageID, LWValidatorMessageValidationCallback aCallback)
{
	// set callback
	aValidator->messageValidationCallbacks[aMessageID] = aCallback;
}

void LWValidatorClearMessageValidationCallbacks(LWValidator *aValidator)
{
	// clear message validation callbacks
	for(uint16_t i = 0; i < 256; ++i)
		aValidator->messageValidationCallbacks[i] = NULL;
}

#pragma mark -
#pragma mark Validating Messages

bool LWValidatorMessageIsValid(LWValidator *aValidator, LWMessage *aMessage)
{
	// get message validation rule
	LWValidatorMessageValidationCallback callback = aValidator->messageValidationCallbacks[aMessage->messageID];
	if(!callback)
		return true;

	// validate message
	return callback(aMessage);
}
