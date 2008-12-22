/*
 * LWArgument.c
 * Lunkwill
 * 
 * Copyright (c) 2003-2007 Denis Defreyne, Sam Rushing
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

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#	include <winsock2.h>
#else
#	include <arpa/inet.h>
#endif

#include <Lunkwill/LunkwillDefines.h>
#include <Lunkwill/LunkwillTypes.h>
#include <Lunkwill/LunkwillPrivate.h>
#include <Lunkwill/LWArgument.h>

#pragma mark Creating Arguments

LWArgument *LWArgumentCreate(void *aData, size_t aLength)
{
	// don't create argument with length equal to zero
	if(0 == aLength)
		return NULL;

	// create argument
	LWArgument *argument = malloc(sizeof(LWArgument));
	if(!argument)
		return NULL;

	// set retainable
	argument->isRetainable = true;

	// create data
	argument->data = malloc((aLength+1)*sizeof(uint8_t));
	if(!argument->data)
	{
		free(argument);
		return NULL;
	}
	argument->ownsData = true;

	// copy data
	argument->length = aLength;
	memcpy(argument->data, aData, aLength);
	argument->data[aLength] = 0;

	return argument;
}

LWArgument *LWArgumentCreateWithoutCopying(void *aData, size_t aLength)
{
	// don't create argument with length equal to zero
	if(0 == aLength)
		return NULL;

	// create argument
	LWArgument *argument = malloc(sizeof(LWArgument));
	if(!argument)
		return NULL;

	// set retainable
	argument->isRetainable = true;

	// create data
	argument->data = aData;
	argument->length = aLength;
	argument->ownsData = false;

	return argument;
}

LWArgument *LWArgumentCreateFromString(char *aString)
{
	return LWArgumentCreate(aString, strlen(aString));
}

LWArgument *LWArgumentCreateFrom8BitInteger(int8_t aInteger)
{
	return LWArgumentCreate(&aInteger, sizeof(int8_t));
}

LWArgument *LWArgumentCreateFrom8BitUnsignedInteger(uint8_t aInteger)
{
	return LWArgumentCreate(&aInteger, sizeof(uint8_t));
}

LWArgument *LWArgumentCreateFrom16BitInteger(int16_t aInteger)
{
	int16_t newInteger = htons(aInteger);
	return LWArgumentCreate(&newInteger, sizeof(int16_t));
}

LWArgument *LWArgumentCreateFrom16BitUnsignedInteger(uint16_t aInteger)
{
	uint16_t newInteger = htons(aInteger);
	return LWArgumentCreate(&newInteger, sizeof(uint16_t));
}

LWArgument *LWArgumentCreateFrom32BitInteger(int32_t aInteger)
{
	int32_t newInteger = htonl(aInteger);
	return LWArgumentCreate(&newInteger, sizeof(int32_t));
}

LWArgument *LWArgumentCreateFrom32BitUnsignedInteger(uint32_t aInteger)
{
	uint32_t newInteger = htonl(aInteger);
	return LWArgumentCreate(&newInteger, sizeof(uint32_t));
}

#pragma mark -
#pragma mark Altering Arguments

void LWArgumentSetRetainable(LWArgument *aArgument, bool aIsRetainable)
{
	aArgument->isRetainable = aIsRetainable;
}

void LWArgumentSetOwnsData(LWArgument *aArgument, bool aOwnsData)
{
	aArgument->ownsData = aOwnsData;
}

#pragma mark -
#pragma mark Querying Arguments

void *LWArgumentGetData(LWArgument *aArgument)
{
	return aArgument->data;
}

size_t LWArgumentGetLength(LWArgument *aArgument)
{
	return aArgument->length;
}

char *LWArgumentGetStringValue(LWArgument *aArgument)
{
	char *string = malloc(sizeof(char)*(aArgument->length+1));
	memcpy(string, aArgument->data, aArgument->length);
	string[aArgument->length] = '\0';

	return string;
}

int8_t LWArgumentGet8BitIntegerValue(LWArgument *aArgument)
{
	return (int8_t)aArgument->data[0];
}

uint8_t LWArgumentGet8BitUnsignedIntegerValue(LWArgument *aArgument)
{
	return (uint8_t)aArgument->data[0];
}

int16_t LWArgumentGet16BitIntegerValue(LWArgument *aArgument)
{
	return ntohs(*((int16_t *)(aArgument->data)));
}

uint16_t LWArgumentGet16BitUnsignedIntegerValue(LWArgument *aArgument)
{
	return ntohs(*((uint16_t *)(aArgument->data)));
}

int32_t LWArgumentGet32BitIntegerValue(LWArgument *aArgument)
{
	return ntohl(*((int32_t *)(aArgument->data)));
}

uint32_t LWArgumentGet32BitUnsignedIntegerValue(LWArgument *aArgument)
{
	return ntohl(*((int32_t *)(aArgument->data)));
}

#pragma mark -
#pragma mark Deleting Arguments

void LWArgumentDelete(LWArgument *aArgument)
{
	if(aArgument->ownsData)
		free(aArgument->data);
	free(aArgument);
}

#pragma mark -
#pragma mark Debugging

void LWArgumentPrint(LWArgument *aArgument)
{
	// write begin of string representation
	fprintf(stdout, "(%03u) <", (unsigned)aArgument->length);

	// write argument data
	bool nonPrintableCharacterFound = false;
	for(size_t i = 0; i < aArgument->length; ++i)
	{
		// write a single byte
		fprintf(stdout, "%02x", (unsigned)aArgument->data[i]);

		// write space if necessary
		if(3 == i % 4 && i != aArgument->length - 1)
			fputs(" ", stdout);

		// check for non-printable characters
		if(!isprint(aArgument->data[i]))
			nonPrintableCharacterFound = true;
	}

	// write argument data as string if necessary
	if(!nonPrintableCharacterFound)
	{
		fputs("> (", stdout);

		for(size_t i = 0; i < aArgument->length; ++i)
			fputc(aArgument->data[i], stdout);
	}

	// write end of string representation
	if(!nonPrintableCharacterFound)
		fputs(")\n", stdout);
	else
		fputs(">\n", stdout);
}

void LWArgumentGetStringRepresentation(LWArgument *aArgument, char *aString, size_t aLength)
{
	size_t usedLength;

	// append begin of string representation
	snprintf(aString, aLength, "(%03u) <", (unsigned)aArgument->length);
	usedLength = strlen(aString);
	if(usedLength + 1 >= aLength)
		return;

	// append argument data
	bool nonPrintableCharacterFound = false;
	for(size_t i = 0; i < aArgument->length; ++i)
	{
		// append a single byte
		snprintf(aString + usedLength, aLength - usedLength, "%02x", (unsigned)aArgument->data[i]);
		usedLength = strlen(aString);
		if(usedLength + 1 >= aLength)
			return;

		// append space if necessary
		if(3 == i % 4 && i != aArgument->length - 1)
		{
			strncat(aString + usedLength, " ", aLength - usedLength);
			usedLength = strlen(aString);
			if(usedLength + 1 >= aLength)
				return;
		}

		// check for non-printable characters
		if(!isprint(aArgument->data[i]))
			nonPrintableCharacterFound = true;
	}

	// append argument data as string if necessary
	if(!nonPrintableCharacterFound)
	{
		strncat(aString + usedLength, "> (", aLength - usedLength);
		usedLength = strlen(aString);
		if(usedLength + 1 >= aLength)
			return;

		for(size_t i = 0; i < aArgument->length; ++i)
		{
			snprintf(aString + usedLength, aLength - usedLength, "%c", aArgument->data[i]);
			usedLength = strlen(aString);
			if(usedLength + 1 >= aLength)
				return;
		}
	}

	// append end of string representation
	if(!nonPrintableCharacterFound)
		strncat(aString + usedLength, ")\n", aLength - usedLength);
	else
		strncat(aString + usedLength, ">\n", aLength - usedLength);
}
