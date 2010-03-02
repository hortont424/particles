#ifndef _SM_ARGUMENT_H_
#define _SM_ARGUMENT_H_

#include <OpenCL/opencl.h>

#include "SMBuffer.h"

typedef enum _SMArgumentType
{
    SMArgumentBuffer,
    SMArgumentFloat
} SMArgumentType;

typedef struct _SMArgument
{
    SMArgumentType type;


} SMArgument;

SMArgument * SMArgumentNew(SMArgumentType type);
SMArgument * SMArgumentFree(SMArgument * arg);

#endif
