#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>

#include "SMSimulator.h"

SMProgram * compileProgram(SMContext * sim, char * name,
                           const char * source)
{
    SMProgram * prog = calloc(1, sizeof(SMProgram));

    prog->program = clCreateProgramWithSource(sim->ctx, 1, &source, NULL, NULL);
    prog->name = name;

    if(clBuildProgram(prog->program, 0, NULL, SMContextGetBuildOptions(sim),
                      NULL, NULL) != CL_SUCCESS)
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

/**
 * Allocate the space required for an SMProgram, load an OpenCL kernel from the
 * given file, compile it, and allocate space for its list of arguments.
 *
 * Since we use fstat to determine the size of the file to be loaded, it can't
 * be a symlink (the size of the symlink would be reported instead!).
 *
 * @param sim The simulation context to compile the kernel within.
 * @param filename The filename of the OpenCL kernel to load and compile.
 * @return The newly allocated program.
 */
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

    free(fileContent);

    if(prog)
        printf("Successfully loaded kernel '%s'\n", kernelName);

    prog->arguments = (SMArgument **)calloc(SMProgramGetArgumentCount(prog),
                                            sizeof(SMArgument *));

    return prog;
}

/**
 * Free the memory used by the given SMProgram and its contents.
 *
 * @param prog The program to be freed.
 */
void SMProgramFree(SMProgram * prog)
{
    clReleaseKernel(prog->kernel);
    clReleaseProgram(prog->program);
    free(prog->arguments);
    free(prog->name);
    free(prog);
}

/**
 * Execute the given SMProgram in its context, after setting all of the
 * arguments on the kernel from the SMProgram's list of arguments.
 *
 * @param prog The program to execute.
 */
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

    printf("Running '%s' on %zd elements, %zd at a time\n",
           prog->name, prog->globalCount, prog->localCount);

    clEnqueueNDRangeKernel(prog->context->cmds, prog->kernel, 1, NULL,
                           &prog->globalCount, &prog->localCount, 0, NULL,
                           NULL);
}

/**
 * @param prog The SMProgram to modify.
 * @param globalCount The total number of kernel instances to be spawned.
 */
void SMProgramSetGlobalCount(SMProgram * prog, size_t globalCount)
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
}

/**
 * Set an argument to be passed into the kernel when it is executed;
 * arguments are owned by the program, so don't try to reuse them.
 *
 * @todo We could reference-count arguments... and everything, really!
 *
 * @param prog The SMProgram to modify.
 * @param i The index of the argument to set.
 * @param arg A pointer to the SMArgument object representing the value of
 * the kernel argument.
 */
void SMProgramSetArgument(SMProgram * prog, unsigned int i, SMArgument * arg)
{
    SMArgument * oldArg;

    if(i > SMProgramGetArgumentCount(prog))
    {
        throwError("trying to set nonexistent argument #%d", i);
        return;
    }

    oldArg = SMProgramGetArgument(prog, i);
    if(oldArg)
        SMArgumentFree(oldArg);

    prog->arguments[i] = arg;
}

/**
 * @param prog The SMProgram to inspect.
 * @param i The index of the argument to return.
 */
SMArgument * SMProgramGetArgument(SMProgram * prog, unsigned int i)
{
    if(i > SMProgramGetArgumentCount(prog))
    {
        throwError("trying to access nonexistent argument #%d", i);
        return NULL;
    }

    return prog->arguments[i];
}

/**
 * @param prog The SMProgram to inspect.
 * @return The number of arguments that the compiled kernel expects to receive.
 */
unsigned int SMProgramGetArgumentCount(SMProgram * prog)
{
    unsigned int argc;

    clGetKernelInfo(prog->kernel, CL_KERNEL_NUM_ARGS, sizeof(unsigned int),
                    &argc, NULL);

    return argc;
}
