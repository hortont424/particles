#ifndef _SM_ARGUMENT_H_
#define _SM_ARGUMENT_H_

#include <stdbool.h>
#include <OpenCL/opencl.h>

#include "SMBuffer.h"

typedef struct _SMArgument
{
    void * pointer;
    size_t size;

    bool owned;
} SMArgument;

SMArgument * SMArgumentNew();
SMArgument * SMArgumentNewWithBuffer(SMBuffer * buf);
SMArgument * SMArgumentNewWithFloat(float f);
SMArgument * SMArgumentNewWithInt(int f);
SMArgument * SMArgumentFree(SMArgument * arg);

size_t SMArgumentGetSize(SMArgument * arg);
void * SMArgumentGetPointer(SMArgument * arg);

#endif
