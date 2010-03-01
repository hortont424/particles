#ifndef _SM_BUFFER_H_
#define _SM_BUFFER_H_

#include <sys/types.h>

typedef struct _SMBuffer
{
    void * buffer;
    
    size_t elementSize;
    long elementCount;
} SMBuffer;

SMBuffer * SMBufferNew(long elementCount);
SMBuffer * SMBufferNewWithSize(long elementCount, size_t elementSize);

SMBuffer * SMBufferFree(SMBuffer * buf);

#endif