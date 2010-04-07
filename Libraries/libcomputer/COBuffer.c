/* particles - libcomputer - COBuffer.c
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
#include <sys/mman.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#include <liblog/liblog.h>

#include "libcomputer.h"

/**
 * Create an COBuffer backed by an OpenCL memory object. Depending on whether
 * the context it's created in is on the CPU or GPU, reads and writes to this
 * buffer may involve transferring data between main and video memory, which is
 * much slower than working solely within either.
 *
 * @param ctx The context in which to create the new buffer.
 * @param elementCount The number of elements in the buffer.
 * @param elementSize The size of an element in the buffer.
 * @param doubleBuffered If we should double-buffer or not.
 * @return The newly allocated buffer.
 */
COBuffer * COBufferNew(COContext * ctx, long elementCount,
                       size_t elementSize, bool doubleBuffered)
{
    COBuffer * buf = (COBuffer *)calloc(1, sizeof(COBuffer));

    buf->context = ctx;
    buf->type = CO_OPENCL_BUFFER;
    buf->doubleBuffered = doubleBuffered;

    buf->elementCount = elementCount;
    buf->elementSize = elementSize;

    /// \todo Use host pointers? (OpenCL page 49)
    buf->gpuBuffer = clCreateBuffer(ctx->ctx, CL_MEM_READ_WRITE,
                                    COBufferGetSize(buf), NULL, NULL);

    if(buf->doubleBuffered)
        buf->gpuBackBuffer = clCreateBuffer(ctx->ctx, CL_MEM_READ_WRITE,
                                            COBufferGetSize(buf), NULL, NULL);

    return buf;
}

/**
 * Create an COBuffer backed by an mmapped file. If the file exists, it is
 * overwritten; if it doesn't, it is created.
 *
 * @param ctx The context in which to create the new buffer.
 * @param elementCount The number of elements in the buffer.
 * @param elementSize The size of an element in the buffer.
 * @param filename The filename to map into the buffer's memory.
 * @return The newly allocated buffer.
 */
COBuffer * COBufferNewWithFile(COContext * ctx, long elementCount,
                               size_t elementSize, const char * filename)
{
    COBuffer * buf = (COBuffer *)calloc(1, sizeof(COBuffer));

    buf->context = ctx;
    buf->type = CO_FILE_BUFFER;

    buf->elementCount = elementCount;
    buf->elementSize = elementSize;

    buf->file = open(filename, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);

    if(buf->file == -1)
    {
        LOError("couldn't create buffer file '%s'", filename);
        return NULL;
    }

    lseek(buf->file, COBufferGetSize(buf), SEEK_SET);
    write(buf->file, "", 1);

    buf->fileBuffer = (void *)mmap(NULL, COBufferGetSize(buf),
                                   PROT_READ | PROT_WRITE, MAP_SHARED,
                                   buf->file, 0);

    return buf;
}

/**
 * Free the memory used by the given COBuffer. This will also unmap and close
 * the associated file if it's a file-backed buffer.
 *
 * @param buf The buffer to free.
 */
void COBufferFree(COBuffer * buf)
{
    switch(buf->type)
    {
        case CO_OPENCL_BUFFER:
            clReleaseMemObject(buf->gpuBuffer);

            if(buf->doubleBuffered)
                clReleaseMemObject(buf->gpuBackBuffer);

            break;
        case CO_FILE_BUFFER:
            munmap(buf->fileBuffer, COBufferGetSize(buf));
            close(buf->file);

            break;
        default:
            LOError("tried to free unknown buffer type");
    }

    free(buf);
}

/**
 * @param buf The COBuffer to inspect.
 * @return The number of elements in the buffer.
 */
long COBufferGetElementCount(COBuffer * buf)
{
    return buf->elementCount;
}

/**
 * @param buf The COBuffer to inspect.
 * @return The size of each element in the buffer.
 */
size_t COBufferGetElementSize(COBuffer * buf)
{
    return buf->elementSize;
}

/**
 * @param buf The COBuffer to inspect.
 * @return The total size of the buffer.
 */
size_t COBufferGetSize(COBuffer * buf)
{
    return COBufferGetElementSize(buf) * COBufferGetElementCount(buf);
}

/**
 * Return the cl_mem object that currently backs the buffer. If the back
 * buffer is requested and the buffer isn't double-buffered, return the sole
 * front buffer.
 *
 * @param buf The COBuffer to inspect.
 * @param backBuffer If true and we're double-buffered, return secondary buffer.
 * @return The cl_mem object which backs OpenCL buffers.
 */
cl_mem COBufferGetCLBuffer(COBuffer * buf, bool backBuffer)
{
    if(buf->type != CO_OPENCL_BUFFER)
        LOError("tried to get cl_mem from non-OpenCL buffer");

    return ((backBuffer && buf->doubleBuffered) ?
        buf->gpuBackBuffer : buf->gpuBuffer);
}

/**
 * @param buf The COBuffer to inspect.
 * @return A pointer to the native buffer in memory, for file-backed buffers.
 */
void * COBufferGetNativeBuffer(COBuffer * buf)
{
    if(buf->type != CO_FILE_BUFFER)
        LOError("tried to get native buffer from non-file buffer");

    return buf->fileBuffer;
}

/**
 * Copy the contents of the given COBuffer into the given native buffer. If
 * space hasn't been allocated for the native buffer, allocate just enough
 * space to fit the COBuffer.
 *
 * Keep in mind that if the COBuffer is OpenCL-backed, this is an expensive
 * operation, as it involves copying a chunk of data from video to main memory.
 *
 * @param buf The source COBuffer.
 * @param data A pointer to the destination native buffer.
 */
void COBufferGet(COBuffer * buf, void ** data)
{
    /// \todo I'm not totally sure this makes sense.
    if(*data == NULL)
    {
        (*data) = (void *)calloc(1, COBufferGetSize(buf));
    }

    switch(buf->type)
    {
        case CO_OPENCL_BUFFER:
            clEnqueueReadBuffer(buf->context->cmds, buf->gpuBuffer, CL_TRUE,
                                0, COBufferGetSize(buf), *data, 0, NULL, NULL);
            break;
        case CO_FILE_BUFFER:
            break;
        default:
            LOError("tried to copy from unknown buffer type");
    }
}

/**
 * Copy the contents of the given native buffer into the given COBuffer.
 *
 * Keep in mind that if the COBuffer is OpenCL-backed, this is an expensive
 * operation, as it involves copying a chunk of data from main to video memory.
 *
 * @param buf The destination COBuffer.
 * @param data The source native buffer.
 */
void COBufferSet(COBuffer * buf, void * data)
{
    switch(buf->type)
    {
        case CO_OPENCL_BUFFER:
            clEnqueueWriteBuffer(buf->context->cmds, buf->gpuBuffer, CL_TRUE,
                                 0, COBufferGetSize(buf), data, 0, NULL, NULL);
            break;
        case CO_FILE_BUFFER:
            break;
        default:
            LOError("tried to copy to unknown buffer type");
    }
}

/**
 * Swap front and back buffers (if this is a double-buffered buffer).
 *
 * @param buf The buffer to swap.
 */
void COBufferSwap(COBuffer * buf)
{
    cl_mem temp;
    temp = buf->gpuBuffer;
    buf->gpuBuffer = buf->gpuBackBuffer;
    buf->gpuBackBuffer = temp;
}
