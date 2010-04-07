/* particles - libcomputer - COContext.c
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#include <liblog/liblog.h>

#include "libcomputer.h"

char * cwdSprintf(const char * fmt)
{
    char * workingDir, * defaultIncludePath;
    size_t maxPathLength, formatLength, totalLength;

    maxPathLength = pathconf(".", _PC_PATH_MAX);
    formatLength = strlen(fmt);
    totalLength = maxPathLength + formatLength;

    workingDir = (char *)calloc(maxPathLength, sizeof(char));
    defaultIncludePath = (char *)calloc(totalLength, sizeof(char));

    workingDir = getcwd(workingDir, maxPathLength);

    /** \todo It seems that the OpenCL compiler simply *cannot* handle arguments
              with spaces in them; so, please don't pass them... */

    /// \todo This should all be magical and formatty and varargs and stuff
    snprintf(defaultIncludePath, totalLength, fmt, workingDir);

    free(workingDir);

    return defaultIncludePath;
}

/**
 * Allocate the space required for an COContext, create an OpenCL context
 * and command queue associated with it, parse command line options, print
 * the make and model of the OpenCL computation device, and return the newly
 * created COContext.
 *
 * Set the compile-time options to the default, which adds the "kernels"
 * subdirectory of the current directory to the include path.
 *
 * @return The newly allocated computation context.
 */
COContext * COContextNew()
{
    COContext * ctx;
    int deviceType;

    char deviceName[2048], vendorName[2048];

    ctx = calloc(1, sizeof(COContext));
    deviceType = computerUsesCPU ? CL_DEVICE_TYPE_CPU : CL_DEVICE_TYPE_GPU;

    clGetDeviceIDs(NULL, deviceType, 1, &ctx->devs, NULL);
    clGetDeviceInfo(ctx->devs, CL_DEVICE_VENDOR, 2048, &vendorName, NULL);
    clGetDeviceInfo(ctx->devs, CL_DEVICE_NAME, 2048, &deviceName, NULL);

    ctx->ctx = clCreateContext(0, 1, &ctx->devs, &raiseOpenCLError, NULL, NULL);
    ctx->cmds = clCreateCommandQueue(ctx->ctx, ctx->devs, 0, NULL);

    ctx->buildOptions = cwdSprintf("-I %s/../Libraries/libparticles");

    if(ctx->buildOptions && ctx->ctx && ctx->cmds)
        printf("Created simulator on '%s %s'\n", vendorName, deviceName);
    else
        LOError("failed to create simulator");

    return ctx;
}

/**
 * Free the memory used by an COContext, destroying the OpenCL context and
 * command queue in the process.
 *
 * @param ctx The simulation context to free.
 */
void COContextFree(COContext * ctx)
{
    clReleaseCommandQueue(ctx->cmds);
    clReleaseContext(ctx->ctx);
    free(ctx->buildOptions);
    free(ctx);
}

/**
 * @param ctx Context for which to modify compile-time options.
 * @param buildOptions New compile-time option string.
 */
void COContextSetBuildOptions(COContext * ctx, char * buildOptions)
{
    free((void *)ctx->buildOptions);

    ctx->buildOptions = buildOptions;
}

/**
 * @param ctx Context to inspect.
 * @return The current compile-time option string for the given context.
 */
const char * COContextGetBuildOptions(COContext * ctx)
{
    return ctx->buildOptions;
}

/**
 * Wait for all commands in the given context's command queue to complete, then
 * return.
 *
 * @param ctx The simulation context to wait for.
 */
void COContextWait(COContext * ctx)
{
    clFinish(ctx->cmds);
}
