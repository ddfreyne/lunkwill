/*
 * LWMessage.c
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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Lunkwill/LunkwillDefines.h>
#include <Lunkwill/LunkwillTypes.h>
#include <Lunkwill/LunkwillPrivate.h>
#include <Lunkwill/LWMessage.h>

#pragma mark Creating Messages

LWMessage *LWMessageCreate(uint8_t aMessageID, LWArgument *aArguments, ...)
{
	va_list ap;
	LWArgument *argument;

	// allocate message
	LWMessage *message = malloc(sizeof(LWMessage));
	if(!message)
		return NULL;

	// set message id
	message->messageID = aMessageID;

	// count arguments
	message->argumentCount = 0;
	va_start(ap, aArguments);
	argument = aArguments;
	while(argument)
	{
		message->argumentCount++;
		argument = va_arg(ap, LWArgument *);
	}
	va_end(ap);

	// allocate arguments
	message->arguments = malloc(message->argumentCount*sizeof(LWArgument *));
	message->argumentCapacity = message->argumentCount;
	if(!message->arguments)
	{
		free(message);
		return NULL;
	}

	// add arguments to message
	va_start(ap, aArguments);
	argument = aArguments;
	for(size_t i = 0; i < message->argumentCount; ++i)
	{
		message->arguments[i] = argument;
		argument = va_arg(ap, LWArgument *);
	}
	va_end(ap);

	return message;
}

LWMessage *LWMessageCreate2(uint8_t aMessageID, size_t aArgumentCount, LWArgument **aArguments)
{
	// allocate message
	LWMessage *message = malloc(sizeof(LWMessage));
	if(!message)
		return NULL;

	// set arguments count
	message->argumentCount = aArgumentCount;

	// set message id
	message->messageID = aMessageID;

	// allocate arguments
	message->arguments = malloc(aArgumentCount*sizeof(LWArgument *));
	message->argumentCapacity = aArgumentCount;
	if(!message->arguments)
	{
		free(message);
		return NULL;
	}

	// add arguments to message
	for(size_t i = 0; i < aArgumentCount; ++i)
		message->arguments[i] = aArguments[i];

	return message;
}

#pragma mark -
#pragma mark Deleting Messages

void LWMessageDelete(LWMessage *aMessage)
{
	for(size_t i = 0; i < aMessage->argumentCount; ++i)
	{
		if(aMessage->arguments[i]->isRetainable)
			LWArgumentDelete(aMessage->arguments[i]);
	}
	if(aMessage->arguments)
		free(aMessage->arguments);
	free(aMessage);
}

#pragma mark -
#pragma mark Adding Arguments

void LWMessageAddArgument(LWMessage *aMessage, LWArgument *aArgument)
{
	// Allocate some space for adding arguments if necessary
	if(0 == aMessage->argumentCapacity)
	{
		aMessage->argumentCapacity = 5;
		aMessage->argumentCount = 0;
		aMessage->arguments = malloc(aMessage->argumentCapacity*sizeof(LWArgument *));
	}
	// Double array capacity if necessary
	else if(aMessage->argumentCapacity == aMessage->argumentCount)
	{
		LWArgument **newArguments = realloc(aMessage->arguments, sizeof(LWArgument *)*aMessage->argumentCapacity*2);
		if(!newArguments)
			return;

		aMessage->argumentCapacity *= 2;
		aMessage->arguments = newArguments;
	}

	// add argument
	aMessage->arguments[aMessage->argumentCount] = aArgument;
	++aMessage->argumentCount;
}

#pragma mark -
#pragma mark Serializing And Deserializing Messages

bool LWMessageSerialize(LWMessage *aMessage, size_t *aLength, void **aSerializedMessage)
{
	// calculate serialized message length
	size_t length = 1 + 1;
	for(size_t i = 0; i < aMessage->argumentCount; ++i)
	{
		LWArgument *argument = aMessage->arguments[i];
		length += argument->length/255 + argument->length + 1;
	}

	// allocate buffer
	*aSerializedMessage = malloc(length*sizeof(uint8_t));
	if(!*aSerializedMessage)
		return false;

	// serialize message
	((uint8_t *)*aSerializedMessage)[0] = aMessage->messageID;
	size_t nextArgumentIndex = 1;
	for(size_t i = 0; i < aMessage->argumentCount; ++i)
	{
		LWArgument	*argument			= aMessage->arguments[i];

		// serialize argument
		ssize_t		remainingLength		= argument->length;
		uint8_t		*remainingData		= argument->data;
		uint8_t		*serializedArgument	= *aSerializedMessage + nextArgumentIndex;
		size_t		position = 0;
		while(remainingLength >= 0)
		{
			size_t subLength = (remainingLength > 255 ? 255 : remainingLength);
			serializedArgument[position] = subLength;
			if(0 != subLength)
				memcpy(serializedArgument + position + 1, remainingData, subLength);
			position		+= 1 + subLength;
			remainingData	+= subLength;
			remainingLength	-= 255;
		}

		// move to next argument
		size_t serializedArgumentLength = argument->length/255 + argument->length + 1;
		nextArgumentIndex += serializedArgumentLength;
	}
	((uint8_t *)*aSerializedMessage)[length-1] = 0;

	// set length
	*aLength = length;

	return true;
}

LWMessage *LWMessageDeserialize(void *aData, size_t aLength, size_t *aBytesUsed)
{
	size_t	pos;
	uint8_t	*data = (uint8_t *)aData;

	// initialize number of bytes used
	*aBytesUsed = 0;

	// ignore small messages
	if(aLength < 2)
		return NULL;

	// check message-wellformedness and count arguments
	pos										= 1;
	bool	previousArgumentWasIncomplete	= false;
	size_t	argumentCount					= 0;
	while(true)
	{
		// check bounds
		if(pos >= aLength)
			return NULL;

		// at end of message
		if(0 == data[pos] && !previousArgumentWasIncomplete)
			break;

		// at end of argument
		if(255 != data[pos])
		{
			++argumentCount;
			previousArgumentWasIncomplete = false;
		}
		else
			previousArgumentWasIncomplete = true;

		// move to  next argument index
		pos += 1ul + data[pos];
	}

	// allocate arguments
	LWArgument **arguments = malloc(argumentCount*sizeof(LWArgument *));
	if(!arguments)
		return NULL;

	// copy arguments
	pos						= 1;
	size_t	currentArgument	= 0;
	while(pos < aLength)
	{
		// calculate argument length
		size_t	pos2						= pos;
		size_t	argumentLength				= 0;
		size_t	serializedArgumentLength	= 0;
		while(pos2 < aLength)
		{
			argumentLength				+= data[pos2];
			serializedArgumentLength	+= 1ul + data[pos2];

			if(255 != data[pos2])
				break;

			pos2						+= 1ul + data[pos2];
		}

		// don't copy terminator argument
		if(0 == argumentLength)
			break;

		// calculate number of sub-arguments
		size_t	fullSubArgumentCount	= argumentLength / 255;
		uint8_t	remainingBytes			= argumentLength % 255;

		// create data buffer
		void *argumentData = malloc((argumentLength+1)*sizeof(uint8_t));
		if(!argumentData)
		{
			free(arguments);
			return NULL;
		}

		// copy argument data
		for(size_t i = 0; i < fullSubArgumentCount; ++i)
		{
			memcpy(
				argumentData + i*255,
				data + pos + 1 + i*256,
				255
			);
		}
		memcpy(
			argumentData + fullSubArgumentCount*255,
			data + pos + 1 + fullSubArgumentCount*256,
			remainingBytes
		);
		((uint8_t *)argumentData)[argumentLength] = 0;

		// create argument
		arguments[currentArgument] = LWArgumentCreateWithoutCopying(argumentData, argumentLength);
		LWArgumentSetOwnsData(arguments[currentArgument], true);
		++currentArgument;

		// move to next argument index
		pos += serializedArgumentLength;
	}

	// create message
	LWMessage *message = LWMessageCreate2(
		data[0],
		argumentCount,
		arguments
	);

	// set bytes used
	*aBytesUsed = pos + 1;

	return message;
}

#pragma mark -
#pragma mark Validating Messages

bool LWMessageIsValid(LWMessage *aMessage, int32_t aInteger, ...)
{
	// messages with variable number of arguments are valid
	if(aInteger < 0)
		return true;

	// get number of arguments
	int32_t argumentCount = aInteger;

	// check whether argument counts match
	if(aMessage->argumentCount != (uint32_t)argumentCount)
		return false;

	va_list ap;
	bool matches = true;
	va_start(ap, aInteger);
	for(int32_t i = 0; i < argumentCount && matches; ++i)
	{
		int32_t integer = va_arg(ap, int32_t);
		if(integer >= 0 && (uint32_t)integer != aMessage->arguments[i]->length)
			matches = false;
	}
	va_end(ap);

	return matches;
}

#pragma mark -
#pragma mark Querying Messages

uint8_t LWMessageGetMessageID(LWMessage *aMessage)
{
	return aMessage->messageID;
}

size_t LWMessageGetArgumentCount(LWMessage *aMessage)
{
	return aMessage->argumentCount;
}

LWArgument *LWMessageGetArgumentAtIndex(LWMessage *aMessage, size_t aIndex)
{
	return aMessage->arguments[aIndex];
}

#pragma mark -
#pragma mark Debugging

void LWMessagePrint(LWMessage *aMessage)
{
	// write begin of string representation
	fprintf(stdout, "<LWMessage: 0x%08x>{message id = %u", (unsigned)aMessage, aMessage->messageID);

	if(aMessage->argumentCount > 0)
	{
		// write arguments
		fputs(", arguments = (\n", stdout);

		for(size_t i = 0; i < aMessage->argumentCount; ++i)
		{
			// write argument index
			fprintf(stdout, "\t%lu: ", i);

			// write argument
			LWArgumentPrint(aMessage->arguments[i]);
		}

		// write end of arguments string representation
		fputs(")", stdout);
	}

	// write end of string representation
	fputs("}\n", stdout);
}

void LWMessageGetStringRepresentation(LWMessage *aMessage, char *aString, size_t aLength)
{
	size_t usedLength;

	// append begin of string representation
	snprintf(aString, aLength, "<LWMessage: 0x%08x>{message id = %u", (unsigned)aMessage, aMessage->messageID);
	usedLength = strlen(aString);
	if(usedLength + 1 >= aLength)
		return;

	if(aMessage->argumentCount > 0)
	{
		// append arguments
		snprintf(aString + usedLength, aLength - usedLength, ", arguments = (\n");
		usedLength = strlen(aString);
		if(usedLength + 1 >= aLength)
			return;

		for(size_t i = 0; i < aMessage->argumentCount; ++i)
		{
			// append argument index
			snprintf(aString + usedLength, aLength - usedLength, "\t%lu: ", i);
			usedLength = strlen(aString);
			if(usedLength + 1 >= aLength)
				return;

			// append argument representation
			LWArgumentGetStringRepresentation(aMessage->arguments[i], aString + usedLength, aLength - usedLength);
			usedLength = strlen(aString);
			if(usedLength + 1 >= aLength)
				return;
		}

		// write end of arguments string representation
		snprintf(aString + usedLength, aLength - usedLength, ")");
		usedLength = strlen(aString);
		if(usedLength + 1 >= aLength)
			return;
	}

	// append end of string representation
	strncat(aString + usedLength, "}\n", aLength - usedLength);
}
