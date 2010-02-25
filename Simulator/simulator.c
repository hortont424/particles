#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"
#include "error.h"

SimulatorContext * initializeSimulator()
{
    SimulatorContext * sim = calloc(1, sizeof(SimulatorContext));
    
    clGetDeviceIDs(NULL, DEVICE_TYPE, 1, &sim->devs, NULL);
    sim->ctx = clCreateContext(0, 1, &sim->devs, &raiseOpenCLError, NULL, NULL);
    sim->cmds = clCreateCommandQueue(sim->ctx, sim->devs, 0, NULL);
    
    return sim;
}

SimulatorProgram * compileProgram(SimulatorContext * sim, const char * name,
                                  const char * source)
{
    SimulatorProgram * prog = calloc(1, sizeof(SimulatorProgram));
    
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

void executeProgram(SimulatorContext * sim, SimulatorProgram * prog)
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

void waitForPrograms(SimulatorContext * sim)
{
    clFinish(sim->cmds);
}