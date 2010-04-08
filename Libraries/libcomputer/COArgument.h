/* particles - libcomputer - COArgument.h
 *
 * Copyright 2010 Tim Horton. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY TIM HORTON "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL TIM HORTON OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _CO_ARGUMENT_H_
#define _CO_ARGUMENT_H_

#include <stdbool.h>
#include <sys/types.h>
#include <OpenCL/opencl.h>

#include <libparticles/libparticles.h>

#include "COBuffer.h"

/**
 * @defgroup COArgument COArgument
 * @{
 */

typedef enum _COArgumentType
{
    CO_BUFFER_ARGUMENT,         /**< argument points to an COBuffer */
    CO_POINTER_ARGUMENT         /**< argument points to an arbitrary pointer */
} COArgumentType;

typedef struct _COArgument
{
    COArgumentType type;    /**< Type of argument */
    void * pointer;         /**< Pointer to arbitrary argument object */
    size_t size;            /**< Size of arbitrary argument object */

    bool owned;             /**< Whether we own (and should free) the pointer */
    bool backBuffer;        /**< If true (and this is a buffer argument), use
                                 the back buffer when accessing the buffer */

    cl_mem * bufferCache;   /**< For buffer args, a cached native pointer */
} COArgument;

COArgument * COArgumentNew();
COArgument * COArgumentNewWithBuffer(COBuffer * buf, bool backBuffer);
COArgument * COArgumentNewWithPointer(void * ptr, size_t size);
COArgument * COArgumentNewWithFloat(PAFloat f);
COArgument * COArgumentNewWithInt(PAInt i);
void COArgumentFree(COArgument * arg);

size_t COArgumentGetSize(COArgument * arg);
void * COArgumentGetPointer(COArgument * arg);

/** @} */

#endif
