#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <string.h>

#include "SMSimulator.h"

SMProgram * compileProgram(SMContext * sim, const char * name,
                                  const char * source)
{
    SMProgram * prog = calloc(1, sizeof(SMProgram));

    prog->program = clCreateProgramWithSource(sim->ctx, 1, &source, NULL, NULL);
    prog->name = name;

    if(clBuildProgram(prog->program, 0, NULL, NULL, NULL, NULL) != CL_SUCCESS)
    {
        showBuildLog(sim, prog);
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

SMProgram * SMProgramNew(SMContext * sim, const char * filename)
{
    struct stat fileInfo;
    int fileHandle;
    char * fileContent;
    char * kernelName;
    SMProgram * prog;

    if(!strstr(filename, ".cl"))
    {
        throwError("'%s' is not an OpenCL kernel file", filename);
        return NULL;
    }

    fileHandle = open(filename, O_RDONLY);

    if(fileHandle == -1 || fstat(fileHandle, &fileInfo) == -1)
    {
        throwError("kernel file '%s' not found", filename);
        return NULL;
    }

    // Since we use stat's size, we can't allow symlinks here
    if(!S_ISREG(fileInfo.st_mode))
    {
        throwError("'%s' is non-regular; symbolic link?", filename);
        return NULL;
    }

    kernelName = kernelNameFromFilename(filename);

    fileContent = (char *)calloc(fileInfo.st_size + 1, sizeof(char));
    read(fileHandle, fileContent, fileInfo.st_size + 1);

    prog = compileProgram(sim, kernelName, fileContent);
    prog->context = sim;

    if(prog)
        printf("Successfully loaded kernel '%s'\n", kernelName);

    prog->arguments = (SMArgument **)calloc(SMProgramGetArgumentCount(prog),
                                            sizeof(SMArgument *));

    return prog;
}

void SMProgramFree(SMProgram * prog)
{
    // TODO: cleanup OpenCL stuff
    free(prog->arguments);
    free(prog);
}

void SMProgramExecute(SMProgram * prog)
{
    for(int i = 0; i < SMProgramGetArgumentCount(prog); i++)
    {
        SMArgument * arg = prog->arguments[i];

        if(!arg)
        {
            throwError("kernel argument #%d not set", i + 1);
            return;
        }

        clSetKernelArg(prog->kernel, i, SMArgumentGetSize(arg),
                       SMArgumentGetPointer(arg));
    }

    clGetKernelWorkGroupInfo(prog->kernel, prog->context->devs,
                             CL_KERNEL_WORK_GROUP_SIZE,
                             sizeof(prog->localCount), &prog->localCount, NULL);

    if(prog->globalCount < prog->localCount)
        prog->localCount = prog->globalCount;

    printf("Running '%s' on %zd elements, %zd at a time\n",
           prog->name, prog->globalCount, prog->localCount);

    clEnqueueNDRangeKernel(prog->context->cmds, prog->kernel, 1, NULL,
                           &prog->globalCount, &prog->localCount, 0, NULL,
                           NULL);
}

void SMProgramSetGlobalCount(SMProgram * prog, size_t globalCount)
{
    prog->globalCount = globalCount;
}

void SMProgramSetArgument(SMProgram * prog, unsigned int i, SMArgument * arg)
{
    if(i > SMProgramGetArgumentCount(prog))
    {
        throwError("trying to set nonexistent argument #%d", i);
        return;
    }

    prog->arguments[i] = arg;
}

unsigned int SMProgramGetArgumentCount(SMProgram * prog)
{
    unsigned int argc;

    clGetKernelInfo(prog->kernel, CL_KERNEL_NUM_ARGS, sizeof(unsigned int),
                    &argc, NULL);

    return argc;
}
