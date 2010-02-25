#include <stdlib.h>

#include "SMContext.h"
#include "SMError.h"

SMContext * initializeSimulator()
{
    SMContext * sim = calloc(1, sizeof(SMContext));
    
    clGetDeviceIDs(NULL, DEVICE_TYPE, 1, &sim->devs, NULL);
    sim->ctx = clCreateContext(0, 1, &sim->devs, &raiseOpenCLError, NULL, NULL);
    sim->cmds = clCreateCommandQueue(sim->ctx, sim->devs, 0, NULL);
    
    return sim;
}

void waitForPrograms(SMContext * sim)
{
    clFinish(sim->cmds);
}