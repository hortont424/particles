#ifndef _SM_BUFFER_H_
#define _SM_BUFFER_H_

#include <sys/types.h>
#include <OpenCL/opencl.h>

#include "SMContext.h"

typedef struct _SMBuffer
{
    cl_mem buffer;

    size_t elementSize;
    long elementCount;

    SMContext * context;
} SMBuffer;

SMBuffer * SMBufferNew(SMContext * sim, long elementCount);
SMBuffer * SMBufferNewWithSize(SMContext * sim, long elementCount,
                               size_t elementSize);
SMBuffer * SMBufferFree(SMBuffer * buf);

size_t SMBufferGetSize(SMBuffer * buf);
long SMBufferGetElementCount(SMBuffer * buf);
size_t SMBufferGetElementSize(SMBuffer * buf);
cl_mem SMBufferGetNative(SMBuffer * buf);

void SMBufferGet(SMBuffer * buf, void ** data);
void SMBufferSet(SMBuffer * buf, void * data);

#endif
