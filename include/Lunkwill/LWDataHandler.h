/*
 * LWDataHandler.h
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

#ifndef __LUNKWILL_DATAHANDLER_H__
#define __LUNKWILL_DATAHANDLER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <Lunkwill/LunkwillDefines.h>
#include <Lunkwill/LunkwillTypes.h>
#include <Lunkwill/LWMessage.h>
#include <Lunkwill/LWValidator.h>

#pragma mark Creating Data Handlers

LW_EXPORT
LWDataHandler *LWDataHandlerCreate(void *aUserInfo);

#pragma mark -
#pragma mark Deleting Data Handlers

LW_EXPORT
void LWDataHandlerDelete(LWDataHandler *aDataHandler);

#pragma mark -
#pragma mark Setting Callbacks

LW_EXPORT
void LWDataHandlerSetUnrecognisedMessageCallback(LWDataHandler *aDataHandler, LWDataHandlerCallback aCallback);

LW_EXPORT
void LWDataHandlerSetInvalidMessageCallback(LWDataHandler *aDataHandler, LWDataHandlerCallback aCallback);

LW_EXPORT
void LWDataHandlerSetMessageCallback(LWDataHandler *aDataHandler, uint8_t aMessageID, LWDataHandlerCallback aCallback);

LW_EXPORT
void LWDataHandlerClearMessageCallbacks(LWDataHandler *aDataHandler);

#pragma mark -
#pragma mark Setting Validators

LW_EXPORT
void LWDataHandlerSetValidator(LWDataHandler *aDataHandler, LWValidator *aValidator);

#pragma mark -
#pragma mark Handling Data

LW_EXPORT
bool LWDataHandlerHandleData(LWDataHandler *aDataHandler, void *aData, size_t aDataLength);

#ifdef __cplusplus
}
#endif

#endif
