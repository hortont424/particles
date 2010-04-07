/* particles - libcomputer - COProgram.c
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

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>

#include <liblog/liblog.h>

#include "libcomputer.h"

COProgram * compileProgram(COContext * ctx, char * name,
                           const char * source)
{
    COProgram * prog = calloc(1, sizeof(COProgram));

    prog->program = clCreateProgramWithSource(ctx->ctx, 1, &source, NULL, NULL);
    prog->name = name;

    if(clBuildProgram(prog->program, 0, NULL, COContextGetBuildOptions(ctx),
                      NULL, NULL) != CL_SUCCESS)
    {
        showBuildLog(ctx, prog);
        exit(EXIT_FAILURE);
    }

    prog->kernel = clCreateKernel(prog->program, name, NULL);

    return prog;
}

char * kernelNameFromFilename(const char * filename)
{
    char * lastSlash, * lastDot, * kernelName;
    unsigned int kernelNameLength;

    lastSlash = strrchr(filename, '/');
    lastDot = strrchr(filename, '.');

    kernelNameLength = (unsigned int)(lastDot - lastSlash - 1);
    kernelName = (char *)calloc(kernelNameLength + 1, sizeof(char));

    return strncpy(kernelName, lastSlash + 1, kernelNameLength);
}

/**
 * Allocate the space required for an COProgram, load an OpenCL kernel from the
 * given file, compile it, and allocate space for its list of arguments.
 *
 * Since we use fstat to determine the size of the file to be loaded, it can't
 * be a symlink (the size of the symlink would be reported instead!).
 *
 * @param ctx The simulation context to compile the kernel within.
 * @param filename The filename of the OpenCL kernel to load and compile.
 * @return The newly allocated program.
 */
COProgram * COProgramNew(COContext * ctx, const char * filename)
{
    struct stat fileInfo;
    int fileHandle;
    char * fileContent;
    char * kernelName;
    COProgram * prog;

    if(!strstr(filename, ".cl"))
    {
        LOError("'%s' is not an OpenCL kernel file", filename);
        return NULL;
    }

    fileHandle = open(filename, O_RDONLY);

    if(fileHandle == -1 || fstat(fileHandle, &fileInfo) == -1)
    {
        LOError("kernel file '%s' not found", filename);
        return NULL;
    }

    // Since we use stat's size, we can't allow symlinks here
    if(!S_ISREG(fileInfo.st_mode))
    {
        LOError("'%s' is non-regular; symbolic link?", filename);
        return NULL;
    }

    kernelName = kernelNameFromFilename(filename);

    fileContent = (char *)calloc(fileInfo.st_size + 1, sizeof(char));
    read(fileHandle, fileContent, fileInfo.st_size + 1);
    close(fileHandle);

    prog = compileProgram(ctx, kernelName, fileContent);
    prog->context = ctx;

    free(fileContent);

    if(prog)
        printf("Successfully loaded kernel '%s'\n", kernelName);

    prog->arguments = (COArgument **)calloc(COProgramGetArgumentCount(prog),
                                            sizeof(COArgument *));

    return prog;
}

/**
 * Free the memory used by the given COProgram and its contents.
 *
 * @param prog The program to be freed.
 */
void COProgramFree(COProgram * prog)
{
    for(int i = 0; i < COProgramGetArgumentCount(prog); i++)
    {
        COArgumentFree(prog->arguments[i]);
    }

    clReleaseKernel(prog->kernel);
    clReleaseProgram(prog->program);
    free(prog->arguments);
    free(prog->name);
    free(prog);
}

/**
 * Execute the given COProgram in its context, after setting all of the
 * arguments on the kernel from the COProgram's list of arguments.
 *
 * @param prog The program to execute.
 */
void COProgramExecute(COProgram * prog)
{
    for(int i = 0; i < COProgramGetArgumentCount(prog); i++)
    {
        COArgument * arg = prog->arguments[i];

        if(!arg)
        {
            LOError("kernel argument #%d not set", i + 1);
            return;
        }

        clSetKernelArg(prog->kernel, i, COArgumentGetSize(arg),
                       COArgumentGetPointer(arg));
    }

    clEnqueueNDRangeKernel(prog->context->cmds, prog->kernel, 1, NULL,
                           &prog->globalCount, &prog->localCount, 0, NULL,
                           NULL);
}

/**
 * @param prog The COProgram to modify.
 * @param globalCount The total number of kernel instances to be spawned.
 */
void COProgramSetGlobalCount(COProgram * prog, size_t globalCount)
{
    int multiplier;

    prog->globalCount = globalCount;

    clGetKernelWorkGroupInfo(prog->kernel, prog->context->devs,
                             CL_KERNEL_WORK_GROUP_SIZE,
                             sizeof(prog->localCount), &prog->localCount, NULL);

    // If we don't have enough elements to fill one run, reduce the
    // number per run
    if(prog->globalCount < prog->localCount)
        prog->localCount = prog->globalCount;

    // If the number of elements we have doesn't evenly divide into the number
    // of elements per run, reduce the number of elements per run to fit
    /// \todo There is a more efficient way to do this, for sure.
    multiplier = ceil((float)prog->globalCount / prog->localCount);

    while(prog->globalCount % prog->localCount)
        prog->localCount = prog->globalCount / (multiplier++);

    printf("Will run '%s' on %zd elements, %zd at a time\n",
           prog->name, prog->globalCount, prog->localCount);
}

/**
 * Set an argument to be passed into the kernel when it is executed;
 * arguments are owned by the program, so don't try to reuse them.
 *
 * @todo We could reference-count arguments... and everything, really!
 *
 * @param prog The COProgram to modify.
 * @param i The index of the argument to set.
 * @param arg A pointer to the COArgument object representing the value of
 * the kernel argument.
 */
void COProgramSetArgument(COProgram * prog, unsigned int i, COArgument * arg)
{
    COArgument * oldArg;

    if(i > COProgramGetArgumentCount(prog))
    {
        LOError("trying to set nonexistent argument #%d", i);
        return;
    }

    oldArg = COProgramGetArgument(prog, i);
    if(oldArg)
        COArgumentFree(oldArg);

    prog->arguments[i] = arg;
}

/**
 * @param prog The COProgram to inspect.
 * @param i The index of the argument to return.
 */
COArgument * COProgramGetArgument(COProgram * prog, unsigned int i)
{
    if(i > COProgramGetArgumentCount(prog))
    {
        LOError("trying to access nonexistent argument #%d", i);
        return NULL;
    }

    return prog->arguments[i];
}

/**
 * @param prog The COProgram to inspect.
 * @return The number of arguments that the compiled kernel expects to receive.
 */
unsigned int COProgramGetArgumentCount(COProgram * prog)
{
    unsigned int argc;

    clGetKernelInfo(prog->kernel, CL_KERNEL_NUM_ARGS, sizeof(unsigned int),
                    &argc, NULL);

    return argc;
}
