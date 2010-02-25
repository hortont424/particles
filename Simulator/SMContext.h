#ifndef _SM_CONTEXT_H_
#define _SM_CONTEXT_H_

#include <OpenCL/opencl.h>

#define DEVICE_TYPE CL_DEVICE_TYPE_GPU

typedef struct _SMContext
{
    cl_device_id devs;
    cl_context ctx;
    cl_command_queue cmds;
} SMContext;

SMContext * initializeSimulator();

void waitForCompletion(SMContext * sim);

#endif