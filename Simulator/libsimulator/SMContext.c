#include <stdlib.h>
#include <stdio.h>

#include "SMContext.h"
#include "SMError.h"

SMContext * SMContextNew(int argc, char * const * argv)
{
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

void SMContextWait(SMContext * sim)
{
    clFinish(sim->cmds);
}
