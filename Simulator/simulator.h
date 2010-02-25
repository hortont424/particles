#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_

#include <OpenCL/opencl.h>

#define DEVICE_TYPE CL_DEVICE_TYPE_GPU

typedef struct _SimulatorContext
{
    cl_device_id devs;
    cl_context ctx;
    cl_command_queue cmds;
} SimulatorContext;

typedef struct _SimulatorProgram
{
    size_t globalCount, localCount;
    
    cl_program program;
    cl_kernel kernel;
} SimulatorProgram;

SimulatorContext * initializeSimulator();
SimulatorProgram * compileProgram(SimulatorContext * sim, const char * name,
                                  const char * source);
void executeProgram(SimulatorContext * sim, SimulatorProgram * prog);
void waitForPrograms(SimulatorContext * sim);

#endif