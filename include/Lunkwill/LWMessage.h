/*
 * LWMessage.h
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

#ifndef __LUNKWILL_MESSAGE_H__
#define __LUNKWILL_MESSAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <sys/types.h>

#include <Lunkwill/LunkwillDefines.h>
#include <Lunkwill/LunkwillTypes.h>
#include <Lunkwill/LWArgument.h>

#pragma mark Creating Messages

LW_EXPORT
LWMessage *LWMessageCreate(uint8_t aMessageID, LWArgument *aArguments, ...);

LW_EXPORT
LWMessage *LWMessageCreate2(uint8_t aMessageID, size_t aArgumentCount, LWArgument **aArguments);

#pragma mark -
#pragma mark Deleting Messages

LW_EXPORT
void LWMessageDelete(LWMessage *aMessage);

#pragma mark -
#pragma mark Adding Arguments

LW_EXPORT
void LWMessageAddArgument(LWMessage *aMessage, LWArgument *aArgument);

#pragma mark -
#pragma mark Serializing And Deserializing Messages

LW_EXPORT
bool LWMessageSerialize(LWMessage *aMessage, size_t *aLength, void **aSerializedMessage);

LW_EXPORT
LWMessage *LWMessageDeserialize(void *aData, size_t aLength, size_t *aBytesUsed);

#pragma mark -
#pragma mark Validating Messages

LW_EXPORT
bool LWMessageIsValid(LWMessage *aMessage, int32_t aInteger, ...);

#pragma mark -
#pragma mark Querying Messages

LW_EXPORT
uint8_t LWMessageGetMessageID(LWMessage *aMessage);

LW_EXPORT
size_t LWMessageGetArgumentCount(LWMessage *aMessage);

LW_EXPORT
LWArgument *LWMessageGetArgumentAtIndex(LWMessage *aMessage, size_t aIndex);

#pragma mark -
#pragma mark Debugging

LW_EXPORT
void LWMessagePrint(LWMessage *aMessage);

LW_EXPORT
void LWMessageGetStringRepresentation(LWMessage *aMessage, char *aString, size_t aLength);

#ifdef __cplusplus
}
#endif

#endif
