/* particles - libsimulator - SMBuffer.h
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

#ifndef _SM_BUFFER_H_
#define _SM_BUFFER_H_

#include <sys/types.h>
#include <OpenCL/opencl.h>

#include "SMContext.h"

/**
 * @defgroup SMBuffer SMBuffer
 * @{
 */

typedef enum _SMBufferType
{
    SM_FILE_BUFFER,         /**< mmapped-file-backed buffer */
    SM_OPENCL_BUFFER        /**< OpenCL-memory-object-backed buffer */
} SMBufferType;

typedef struct _SMBuffer
{
    SMBufferType type;      /**< Type of buffer */
    bool doubleBuffered;    /**< If buffer is double-buffered */

    cl_mem gpuBuffer;       /**< OpenCL buffer (for SM_OPENCL_BUFFER) */
    cl_mem gpuBackBuffer;   /**< OpenCL back buffer (for double-buffered
                                 SM_OPENCL_BUFFER) */
    void * fileBuffer;      /**< Native buffer (for SM_FILE_BUFFER) */
    int file;               /**< File descriptor (for SM_FILE_BUFFER) */

    size_t elementSize;     /**< Size of each element in the buffer */
    long elementCount;      /**< Number of elements in the buffer */

    SMContext * context;    /**< The context that owns the buffer */
} SMBuffer;

SMBuffer * SMBufferNew(SMContext * sim, long elementCount,
                       size_t elementSize, bool doubleBuffered);
SMBuffer * SMBufferNewWithFile(SMContext * sim, long elementCount,
                               size_t elementSize, const char * filename);
void SMBufferFree(SMBuffer * buf);

size_t SMBufferGetSize(SMBuffer * buf);
long SMBufferGetElementCount(SMBuffer * buf);
size_t SMBufferGetElementSize(SMBuffer * buf);
cl_mem SMBufferGetCLBuffer(SMBuffer * buf, bool backBuffer);
void * SMBufferGetNativeBuffer(SMBuffer * buf);

void SMBufferGet(SMBuffer * buf, void ** data);
void SMBufferSet(SMBuffer * buf, void * data);

void SMBufferSwap(SMBuffer * buf);

/** @} */

#endif
