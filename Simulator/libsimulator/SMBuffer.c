#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "SMSimulator.h"

SMBuffer * SMBufferNew(SMContext * sim, long elementCount)
{
    return SMBufferNewWithSize(sim, elementCount, sizeof(float));
}

SMBuffer * SMBufferNewWithSize(SMContext * sim, long elementCount,
                               size_t elementSize)
{
    SMBuffer * buf = (SMBuffer *)calloc(1, sizeof(SMBuffer));

    buf->context = sim;

    buf->elementCount = elementCount;
    buf->elementSize = elementSize;

    buf->buffer = clCreateBuffer(sim->ctx, CL_MEM_READ_WRITE,
                                 SMBufferGetSize(buf), NULL, NULL);

    return buf;
}

SMBuffer * SMBufferFree(SMBuffer * buf)
{
    // TODO: free(buf->buffer);
    free(buf);
}

long SMBufferGetElementCount(SMBuffer * buf)
{
    return buf->elementCount;
}

size_t SMBufferGetElementSize(SMBuffer * buf)
{
    return buf->elementSize;
}

size_t SMBufferGetSize(SMBuffer * buf)
{
    return SMBufferGetElementSize(buf) * SMBufferGetElementCount(buf);
}

cl_mem SMBufferGetNative(SMBuffer * buf)
{
    return buf->buffer;
}

void SMBufferGet(SMBuffer * buf, void ** data)
{
    if(*data == NULL)
    {
        (*data) = (void *)calloc(1, SMBufferGetSize(buf));
    }

    clEnqueueReadBuffer(buf->context->cmds, buf->buffer, CL_TRUE,
                        0, SMBufferGetSize(buf), *data, 0, NULL, NULL);
}

void SMBufferSet(SMBuffer * buf, void * data)
{
    clEnqueueWriteBuffer(buf->context->cmds, buf->buffer, CL_TRUE,
                         0, SMBufferGetSize(buf), data, 0, NULL, NULL);
}
