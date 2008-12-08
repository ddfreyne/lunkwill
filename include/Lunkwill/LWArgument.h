/*
 * LWArgument.h
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

#ifndef __LUNKWILL_ARGUMENT_H__
#define __LUNKWILL_ARGUMENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

#include <Lunkwill/LunkwillDefines.h>
#include <Lunkwill/LunkwillTypes.h>

#pragma mark Creating Arguments

LW_EXPORT
LWArgument *LWArgumentCreate(void *aData, size_t aLength);

LW_EXPORT
LWArgument *LWArgumentCreateWithoutCopying(void *aData, size_t aLength);

LW_EXPORT
LWArgument *LWArgumentCreateFromString(char *aString);

LW_EXPORT
LWArgument *LWArgumentCreateFrom8BitInteger(int8_t aInteger);

LW_EXPORT
LWArgument *LWArgumentCreateFrom8BitUnsignedInteger(uint8_t aInteger);

LW_EXPORT
LWArgument *LWArgumentCreateFrom16BitInteger(int16_t aInteger);

LW_EXPORT
LWArgument *LWArgumentCreateFrom16BitUnsignedInteger(uint16_t aInteger);

LW_EXPORT
LWArgument *LWArgumentCreateFrom32BitInteger(int32_t aInteger);

LW_EXPORT
LWArgument *LWArgumentCreateFrom32BitUnsignedInteger(uint32_t aInteger);

#pragma mark -
#pragma mark Altering Arguments

LW_EXPORT
void LWArgumentSetRetainable(LWArgument *aArgument, bool aIsRetainable);

#pragma mark -
#pragma mark Querying Arguments

LW_EXPORT
void *LWArgumentGetData(LWArgument *aArgument);

LW_EXPORT
size_t LWArgumentGetLength(LWArgument *aArgument);

LW_EXPORT
char *LWArgumentGetStringValue(LWArgument *aArgument);

LW_EXPORT
int8_t LWArgumentGet8BitIntegerValue(LWArgument *aArgument);

LW_EXPORT
uint8_t LWArgumentGet8BitUnsignedIntegerValue(LWArgument *aArgument);

LW_EXPORT
int16_t LWArgumentGet16BitIntegerValue(LWArgument *aArgument);

LW_EXPORT
uint16_t LWArgumentGet16BitUnsignedIntegerValue(LWArgument *aArgument);

LW_EXPORT
int32_t LWArgumentGet32BitIntegerValue(LWArgument *aArgument);

LW_EXPORT
uint32_t LWArgumentGet32BitUnsignedIntegerValue(LWArgument *aArgument);

#pragma mark -
#pragma mark Deleting Arguments

LW_EXPORT
void LWArgumentDelete(LWArgument *aArgument);

#pragma mark -
#pragma mark Debugging

LW_EXPORT
void LWArgumentPrint(LWArgument *aArgument);

LW_EXPORT
void LWArgumentGetStringRepresentation(LWArgument *aArgument, char *aString, size_t aLength);

#ifdef __cplusplus
}
#endif

#endif
