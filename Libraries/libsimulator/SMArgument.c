/* particles - libsimulator - SMArgument.c
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

#include <stdio.h>
#include <stdlib.h>

#include "libsimulator.h"

/**
 * Allocate the space required for an empty SMArgument.
 *
 * @return The newly allocated empty argument.
 */
SMArgument * SMArgumentNew()
{
    return (SMArgument *)calloc(1, sizeof(SMArgument));
}

/**
 * Allocate the space required for an SMArgument, and fill it with a pointer
 * to the given SMBuffer (this doesn't work with file buffers, because
 * they aren't cl_mem objects and as such can't be passed into an OpenCL
 * kernel).
 *
 * The native pointer will be retrieved when requested with
 * SMArgumentGetPointer - that way, the correct front/back buffer will be
 * returned without having to create a new argument each time.
 *
 * @param buf The OpenCL-backed SMBuffer the argument should point at.
 * @param backBuffer If true, use the SMBuffer's back buffer.
 * @return The newly allocated argument, pointing at an SMBuffer.
 */
SMArgument * SMArgumentNewWithBuffer(SMBuffer * buf, bool backBuffer)
{
    SMArgument * arg = SMArgumentNew();

    arg->type = SM_BUFFER_ARGUMENT;
    arg->size = sizeof(cl_mem);
    arg->pointer = (void *)buf;
    arg->owned = false;
    arg->backBuffer = backBuffer;

    return arg;
}

/**
 * Allocate the space required for an SMArgument, and fill it with a pointer
 * to a copy of the given float.
 *
 * @param f A float for the argument to point at.
 * @return The newly allocated argument, pointing at a copy of the given float.
 */
SMArgument * SMArgumentNewWithFloat(PAFloat f)
{
    SMArgument * arg = SMArgumentNew();

    arg->type = SM_POINTER_ARGUMENT;
    arg->size = sizeof(PAFloat);
    arg->pointer = (void *)calloc(1, sizeof(PAFloat));
    *((PAFloat *)arg->pointer) = f;
    arg->owned = true;

    return arg;
}

/**
 * Allocate the space required for an SMArgument, and fill it with a pointer
 * to a copy of the given int.
 *
 * @param i A int for the argument to point at.
 * @return The newly allocated argument, pointing at a copy of the given int.
 */
SMArgument * SMArgumentNewWithInt(PAInt i)
{
    SMArgument * arg = SMArgumentNew();

    arg->type = SM_POINTER_ARGUMENT;
    arg->size = sizeof(PAInt);
    arg->pointer = (void *)calloc(1, sizeof(PAInt));
    *((PAInt *)arg->pointer) = i;
    arg->owned = true;

    return arg;
}

/**
 * Free the memory used by the given SMArgument, also freeing the object it
 * points at, if we own it.
 *
 * @param arg The SMArgument to free.
 */
void SMArgumentFree(SMArgument * arg)
{
    if(arg->owned)
        free(arg->pointer);

    if(arg->bufferCache)
        free(arg->bufferCache);

    free(arg);
}

/**
 * @param arg The SMArgument to inspect.
 * @return The size of the object the argument points at.
 */
size_t SMArgumentGetSize(SMArgument * arg)
{
    return arg->size;
}

/**
 * @param arg The SMArgument to inspect.
 * @return A pointer to the object the argument points at.
 */
void * SMArgumentGetPointer(SMArgument * arg)
{
    switch(arg->type)
    {
        case SM_POINTER_ARGUMENT:
            return arg->pointer;
            break;
        case SM_BUFFER_ARGUMENT:
            if(arg->bufferCache)
                free(arg->bufferCache);

            arg->bufferCache = (cl_mem *)calloc(1, sizeof(cl_mem));
            (*arg->bufferCache) = SMBufferGetCLBuffer((SMBuffer *)arg->pointer,
                                                      arg->backBuffer);
            return arg->bufferCache;
            break;
        default:
            throwError("tried to get pointer from uninitialized argument");
            break;
    }

    return arg->pointer;
}
