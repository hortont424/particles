#ifndef _SM_BUFFER_H_
#define _SM_BUFFER_H_

#include <sys/types.h>
#include <OpenCL/opencl.h>

#include "SMContext.h"

typedef enum _SMBufferType
{
    SM_FILE_BUFFER,
    SM_OPENCL_BUFFER
} SMBufferType;

typedef struct _SMBuffer
{
    SMBufferType type;
    
    cl_mem gpuBuffer;
    void * fileBuffer;
    int file;

    size_t elementSize;
    long elementCount;

    SMContext * context;
} SMBuffer;

SMBuffer * SMBufferNew(SMContext * sim, long elementCount, size_t elementSize);
SMBuffer * SMBufferNewWithFile(SMContext * sim, long elementCount,
                               size_t elementSize, const char * filename);
SMBuffer * SMBufferFree(SMBuffer * buf);

size_t SMBufferGetSize(SMBuffer * buf);
long SMBufferGetElementCount(SMBuffer * buf);
size_t SMBufferGetElementSize(SMBuffer * buf);
cl_mem SMBufferGetCLBuffer(SMBuffer * buf);
void * SMBufferGetNativeBuffer(SMBuffer * buf);

void SMBufferGet(SMBuffer * buf, void ** data);
void SMBufferSet(SMBuffer * buf, void * data);

#endif
