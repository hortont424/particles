/* particles - libcomputer - COBuffer.h
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

#ifndef _CO_BUFFER_H_
#define _CO_BUFFER_H_

#include <sys/types.h>
#include <OpenCL/opencl.h>

#include "COContext.h"

/**
 * @defgroup COBuffer COBuffer
 * @{
 */

typedef enum _SMBufferType
{
    CO_FILE_BUFFER,         /**< mmapped-file-backed buffer */
    CO_OPENCL_BUFFER        /**< OpenCL-memory-object-backed buffer */
} COBufferType;

typedef struct _SMBuffer
{
    COBufferType type;      /**< Type of buffer */
    bool doubleBuffered;    /**< If buffer is double-buffered */

    cl_mem gpuBuffer;       /**< OpenCL buffer (for CO_OPENCL_BUFFER) */
    cl_mem gpuBackBuffer;   /**< OpenCL back buffer (for double-buffered
                                 CO_OPENCL_BUFFER) */
    void * fileBuffer;      /**< Native buffer (for CO_FILE_BUFFER) */
    int file;               /**< File descriptor (for CO_FILE_BUFFER) */

    size_t elementSize;     /**< Size of each element in the buffer */
    long elementCount;      /**< Number of elements in the buffer */

    COContext * context;    /**< The context that owns the buffer */
} COBuffer;

COBuffer * COBufferNew(COContext * ctx, long elementCount,
                       size_t elementSize, bool doubleBuffered);
COBuffer * COBufferNewWithFile(COContext * ctx, long elementCount,
                               size_t elementSize, const char * filename);
void COBufferFree(COBuffer * buf);

size_t COBufferGetSize(COBuffer * buf);
long COBufferGetElementCount(COBuffer * buf);
size_t COBufferGetElementSize(COBuffer * buf);
cl_mem COBufferGetCLBuffer(COBuffer * buf, bool backBuffer);
void * COBufferGetNativeBuffer(COBuffer * buf);

void COBufferGet(COBuffer * buf, void ** data);
void COBufferSet(COBuffer * buf, void * data);

void COBufferSwap(COBuffer * buf);

/** @} */

#endif
