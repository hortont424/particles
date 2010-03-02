#ifndef _SM_CONTEXT_H_
#define _SM_CONTEXT_H_

#include <OpenCL/opencl.h>

#include "SMOptions.h"

typedef struct _SMContext
{
    cl_device_id devs;
    cl_context ctx;
    cl_command_queue cmds;
} SMContext;

SMContext * SMContextNew(int argc, char * const * argv);
void SMContextWait(SMContext * sim);

#endif
