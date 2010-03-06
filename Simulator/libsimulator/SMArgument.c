#include <stdlib.h>

#include "SMSimulator.h"

SMArgument * SMArgumentNew()
{
    return (SMArgument *)calloc(1, sizeof(SMArgument));
}

SMArgument * SMArgumentNewWithBuffer(SMBuffer * buf)
{
    SMArgument * arg = SMArgumentNew();

    cl_mem * nativeBuffer = (cl_mem *)calloc(1, sizeof(cl_mem));
    (*nativeBuffer) = SMBufferGetCLBuffer(buf);

    arg->size = sizeof(cl_mem);
    arg->pointer = (void *)nativeBuffer;
    arg->owned = false;

    return arg;
}

SMArgument * SMArgumentNewWithFloat(float f)
{
    SMArgument * arg = SMArgumentNew();

    arg->size = sizeof(float);
    arg->pointer = (float *)calloc(1, sizeof(float));
    *((float *)arg->pointer) = f;
    arg->owned = true;

    return arg;
}

SMArgument * SMArgumentNewWithInt(int f)
{
    SMArgument * arg = SMArgumentNew();

    arg->size = sizeof(int);
    arg->pointer = (float *)calloc(1, sizeof(int));
    *((int *)arg->pointer) = f;
    arg->owned = true;

    return arg;
}


void SMArgumentFree(SMArgument * arg)
{
    if(arg->owned)
        free(arg->pointer);

    free(arg);
}

size_t SMArgumentGetSize(SMArgument * arg)
{
    return arg->size;
}

void * SMArgumentGetPointer(SMArgument * arg)
{
    return arg->pointer;
}
