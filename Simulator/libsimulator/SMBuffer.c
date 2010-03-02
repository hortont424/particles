#include <stdlib.h>

#include "SMSimulator.h"

SMBuffer * SMBufferNew(long elementCount)
{
    return SMBufferNewWithSize(elementCount, sizeof(float));
}

SMBuffer * SMBufferNewWithSize(long elementCount, size_t elementSize)
{
    SMBuffer * buf = (SMBuffer *)calloc(1, sizeof(SMBuffer));

    buf->elementCount = elementCount;
    buf->elementSize = elementSize;
    buf->buffer = (void *)calloc(elementCount, elementSize);

    return buf;
}

SMBuffer * SMBufferFree(SMBuffer * buf)
{
    free(buf->buffer);
    free(buf);
}
