#include <stdio.h>
#include <stdlib.h>

#include "SMProgram.h"
#include "SMError.h"

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

void executeProgram(SMContext * sim, SMProgram * prog)
{
    clGetKernelWorkGroupInfo(prog->kernel, sim->devs, CL_KERNEL_WORK_GROUP_SIZE,
                             sizeof(prog->localCount), &prog->localCount, NULL);
    
    if(prog->globalCount < prog->localCount)
        prog->localCount = prog->globalCount;
    
    printf("Running '%s' on %zd elements, %zd at a time\n",
           prog->name, prog->globalCount, prog->localCount);
    
    clEnqueueNDRangeKernel(sim->cmds, prog->kernel, 1, NULL, &prog->globalCount,
                           &prog->localCount, 0, NULL, NULL);
}
