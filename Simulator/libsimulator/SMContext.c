#include <stdlib.h>
#include <stdio.h>

#include "SMSimulator.h"

/**
 * Allocates the space required for an SMContext, creates an OpenCL context
 * and command queue associated with it, parses command line options, prints
 * the make and model of the OpenCL computation device, and returns the newly
 * created SMContext.
 */
SMContext * SMContextNew(int argc, char * const * argv)
{ // TODO: does it make sense to do SMOptionsParse from here?!
    SMContext * sim;
    int deviceType;
    char deviceName[2048], vendorName[2048];

    SMOptionsParse(argc, argv);

    sim = calloc(1, sizeof(SMContext));
    deviceType = simulatorUsesCPU ? CL_DEVICE_TYPE_CPU : CL_DEVICE_TYPE_GPU;

    clGetDeviceIDs(NULL, deviceType, 1, &sim->devs, NULL);
    clGetDeviceInfo(sim->devs, CL_DEVICE_VENDOR, 2048, &vendorName, NULL);
    clGetDeviceInfo(sim->devs, CL_DEVICE_NAME, 2048, &deviceName, NULL);

    sim->ctx = clCreateContext(0, 1, &sim->devs, &raiseOpenCLError, NULL, NULL);
    sim->cmds = clCreateCommandQueue(sim->ctx, sim->devs, 0, NULL);

    if(sim->ctx && sim->cmds)
        printf("Created simulator on '%s %s'\n", vendorName, deviceName);
    else
        throwError("failed to create simulator");

    return sim;
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
    free(sim);
}
