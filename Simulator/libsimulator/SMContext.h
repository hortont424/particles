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
    cl_device_id devs;      /**< OpenCL device ID context was created on */
    cl_context ctx;         /**< OpenCL context */
    cl_command_queue cmds;  /**< OpenCL command queue */
} SMContext;

SMContext * SMContextNew(int argc, char * const * argv);
void SMContextWait(SMContext * sim);
void SMContextFree(SMContext * sim);

/** @} */

#endif
