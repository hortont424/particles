#ifndef _SM_CONTEXT_H_
#define _SM_CONTEXT_H_

#include <OpenCL/opencl.h>

#include "SMOptions.h"

/**
 * @defgroup SMContext SMContext
 * @{
 */

typedef struct _SMContext
{
    cl_device_id devs;          /**< OpenCL device ID context was created on */
    cl_context ctx;             /**< OpenCL context */
    cl_command_queue cmds;      /**< OpenCL command queue */

    char * buildOptions;  /**< Options to pass to OpenCL compiler */
} SMContext;

SMContext * SMContextNew();
void SMContextFree(SMContext * sim);

void SMContextSetBuildOptions(SMContext * sim, char * buildOptions);
const char * SMContextGetBuildOptions(SMContext * sim);

void SMContextWait(SMContext * sim);

/** @} */

#endif
