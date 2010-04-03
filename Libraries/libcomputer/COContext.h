/* particles - libcomputer - COContext.h
 *
 * Copyright 2010 Tim Horton. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY TIM HORTON "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL TIM HORTON OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _CO_CONTEXT_H_
#define _CO_CONTEXT_H_

#include <OpenCL/opencl.h>

#include "COOptions.h"

/**
 * @defgroup COContext COContext
 * @{
 */

typedef struct _SMContext
{
    cl_device_id devs;          /**< OpenCL device ID context was created on */
    cl_context ctx;             /**< OpenCL context */
    cl_command_queue cmds;      /**< OpenCL command queue */

    char * buildOptions;        /**< Options to pass to OpenCL compiler */
} COContext;

COContext * COContextNew();
void COContextFree(COContext * ctx);

void COContextSetBuildOptions(COContext * ctx, char * buildOptions);
const char * COContextGetBuildOptions(COContext * ctx);

void COContextWait(COContext * ctx);

/** @} */

#endif
