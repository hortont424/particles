/* particles - libsimulator - SMContext.c
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

#include "libsimulator.h"

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
 * Allocate the space required for an SMContext, create an OpenCL context
 * and command queue associated with it, parse command line options, print
 * the make and model of the OpenCL computation device, and return the newly
 * created SMContext.
 *
 * Set the compile-time options to the default, which adds the "kernels"
 * subdirectory of the current directory to the include path.
 *
 * @return The newly allocated simulation context.
 */
SMContext * SMContextNew()
{
    SMContext * sim;
    int deviceType;

    char deviceName[2048], vendorName[2048];

    sim = calloc(1, sizeof(SMContext));
    deviceType = simulatorUsesCPU ? CL_DEVICE_TYPE_CPU : CL_DEVICE_TYPE_GPU;

    clGetDeviceIDs(NULL, deviceType, 1, &sim->devs, NULL);
    clGetDeviceInfo(sim->devs, CL_DEVICE_VENDOR, 2048, &vendorName, NULL);
    clGetDeviceInfo(sim->devs, CL_DEVICE_NAME, 2048, &deviceName, NULL);

    sim->ctx = clCreateContext(0, 1, &sim->devs, &raiseOpenCLError, NULL, NULL);
    sim->cmds = clCreateCommandQueue(sim->ctx, sim->devs, 0, NULL);

    sim->buildOptions = cwdSprintf("-I %s/../Libraries/libparticles");

    if(sim->buildOptions && sim->ctx && sim->cmds)
        printf("Created simulator on '%s %s'\n", vendorName, deviceName);
    else
        throwError("failed to create simulator");

    return sim;
}

/**
 * Free the memory used by an SMContext, destroying the OpenCL context and
 * command queue in the process.
 *
 * @param sim The simulation context to free.
 */
void SMContextFree(SMContext * sim)
{
    clReleaseCommandQueue(sim->cmds);
    clReleaseContext(sim->ctx);
    free(sim->buildOptions);
    free(sim);
}

/**
 * @param sim Context for which to modify compile-time options.
 * @param buildOptions New compile-time option string.
 */
void SMContextSetBuildOptions(SMContext * sim, char * buildOptions)
{
    free((void *)sim->buildOptions);

    sim->buildOptions = buildOptions;
}

/**
 * @param sim Context to inspect.
 * @return The current compile-time option string for the given context.
 */
const char * SMContextGetBuildOptions(SMContext * sim)
{
    return sim->buildOptions;
}

/**
 * Wait for all commands in the given context's command queue to complete, then
 * return.
 *
 * @param sim The simulation context to wait for.
 */
void SMContextWait(SMContext * sim)
{
    clFinish(sim->cmds);
}
