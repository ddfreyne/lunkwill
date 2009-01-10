/*
 * LWDataHandler.c
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
#include <string.h>

#include <Lunkwill/LunkwillDefines.h>
#include <Lunkwill/LunkwillTypes.h>
#include <Lunkwill/LunkwillPrivate.h>
#include <Lunkwill/LWDataHandler.h>

#define kLWDataHandlerInitialBufferCapacity	(256)
#define kLWDataHandlerMaxBufferCapacity		(10240)

#pragma mark Creating Data Handlers

LWDataHandler *LWDataHandlerCreate(void *aUserInfo)
{
	// allocate data handler
	LWDataHandler *dataHandler = malloc(sizeof(LWDataHandler));
	if(!dataHandler)
		return NULL;

	// initialize data handler
	dataHandler->validator				= NULL;
	dataHandler->isHandlingData			= false;
	dataHandler->isScheduledForDeletion	= false;

	// allocate buffer
	dataHandler->buffer = malloc(kLWDataHandlerInitialBufferCapacity*sizeof(uint8_t));
	if(!dataHandler->buffer)
	{
		free(dataHandler);
		return NULL;
	}
	dataHandler->bufferCapacity			= kLWDataHandlerInitialBufferCapacity;
	dataHandler->availableDataLength	= 0;

	// clear callbacks
	LWDataHandlerClearMessageCallbacks(dataHandler);

	// set user info
	dataHandler->userInfo = aUserInfo;

	return dataHandler;
}

#pragma mark -
#pragma mark Deleting Data Handlers

void LWDataHandlerDelete(LWDataHandler *aDataHandler)
{
	if(aDataHandler->isHandlingData)
	{
		// schedule data handler for deletion
		aDataHandler->isScheduledForDeletion = true;
	}
	else
	{
		// delete data handler
		free(aDataHandler->buffer);
		free(aDataHandler);
	}
}

#pragma mark -
#pragma mark Setting Callbacks

void LWDataHandlerSetUnrecognisedMessageCallback(LWDataHandler *aDataHandler, LWDataHandlerCallback aCallback)
{
	// set callback
	aDataHandler->unrecognisedMessageCallback = aCallback;
}

void LWDataHandlerSetInvalidMessageCallback(LWDataHandler *aDataHandler, LWDataHandlerCallback aCallback)
{
	// set callback
	aDataHandler->invalidMessageCallback = aCallback;
}

void LWDataHandlerSetMessageCallback(LWDataHandler *aDataHandler, uint8_t aMessageID, LWDataHandlerCallback aCallback)
{
	// set callback
	aDataHandler->messageCallbacks[aMessageID] = aCallback;
}

void LWDataHandlerClearMessageCallbacks(LWDataHandler *aDataHandler)
{
	// clear unrecognised/invalid message callbacks
	aDataHandler->unrecognisedMessageCallback	= NULL;
	aDataHandler->invalidMessageCallback		= NULL;

	// clear message callbacks
	for(uint16_t i = 0; i < 256; ++i)
		aDataHandler->messageCallbacks[i] = NULL;
}

#pragma mark -
#pragma mark Setting Validators

void LWDataHandlerSetValidator(LWDataHandler *aDataHandler, LWValidator *aValidator)
{
	// set validator
	aDataHandler->validator = aValidator;
}

#pragma mark -
#pragma mark Handling Data

bool LWDataHandlerHandleData(LWDataHandler *aDataHandler, void *aData, size_t aDataLength)
{
	// make sure we don't exceed the 10k buffer limit
	if(aDataHandler->availableDataLength + aDataLength > kLWDataHandlerMaxBufferCapacity)
		return false;

	// resize buffer if necessary
	if(aDataHandler->availableDataLength + aDataLength > aDataHandler->bufferCapacity)
	{
		// determine new buffer size
		size_t newBufferCapacity = aDataHandler->availableDataLength + aDataLength;
		newBufferCapacity = (newBufferCapacity/1024 + 1)*1024;
		if(newBufferCapacity > kLWDataHandlerMaxBufferCapacity)
			return false;

		// reallocate buffer
		void *newBuffer = realloc(aDataHandler->buffer, newBufferCapacity);
		if(!newBuffer)
			return false;
		aDataHandler->buffer = newBuffer;
		aDataHandler->bufferCapacity = newBufferCapacity;
	}

	// append data
	memcpy(aDataHandler->buffer + aDataHandler->availableDataLength, aData, aDataLength);
	aDataHandler->availableDataLength += aDataLength;

	// set handling data
	aDataHandler->isHandlingData = true;

	// look for messages in the buffer
	size_t totalBytesUsed = 0;
	while(true)
	{
		// get next message
		size_t		bytesUsed;
		LWMessage	*message;
		message = LWMessageDeserialize(aDataHandler->buffer + totalBytesUsed, aDataHandler->availableDataLength - totalBytesUsed, &bytesUsed);
		if(!message)
			break;

		// validate message
		if(aDataHandler->validator && !LWValidatorMessageIsValid(aDataHandler->validator, message))
		{
			// message is invalid
			if(aDataHandler->invalidMessageCallback)
				aDataHandler->invalidMessageCallback(aDataHandler, message, aDataHandler->userInfo);

			// skip message
			totalBytesUsed += bytesUsed;
			continue;
		}

		// get appropriate callback and call it
		LWDataHandlerCallback callback = aDataHandler->messageCallbacks[message->messageID];
		if(callback)
			callback(aDataHandler, message, aDataHandler->userInfo);
		else if(aDataHandler->unrecognisedMessageCallback)
			aDataHandler->unrecognisedMessageCallback(aDataHandler, message, aDataHandler->userInfo);

		// delete message
		LWMessageDelete(message);

		// check whether data handler is scheduled for deletion
		if(aDataHandler->isScheduledForDeletion)
		{
			free(aDataHandler->buffer);
			free(aDataHandler);
			return true;
		}

		// move to next message
		totalBytesUsed += bytesUsed;
	}

	// remove used bytes from buffer
	memmove(aDataHandler->buffer, aDataHandler->buffer + totalBytesUsed, aDataHandler->availableDataLength - totalBytesUsed);
	aDataHandler->availableDataLength -= totalBytesUsed;

	// set not handling data
	aDataHandler->isHandlingData = false;

	return true;
}
