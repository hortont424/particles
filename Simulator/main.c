#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <OpenCL/opencl.h>

#define DEVICE_TYPE CL_DEVICE_TYPE_GPU

typedef struct _SimulatorContext
{
    int err;
    
    cl_device_id device;
    cl_context context;
    cl_command_queue commands;
} SimulatorContext;

void throwError(char * errorMessage)
{
    printf("Error: %s\n", errorMessage);
    return EXIT_FAILURE;
}

void checkError(SimulatorContext * ctx, char * errorMessage)
{
    if(ctx->err != CL_SUCCESS)
    {
        throwError(errorMessage);
    }
}

SimulatorContext * initializeSimulator()
{
    SimulatorContext * ctx = calloc(1, sizeof(SimulatorContext));
    
    ctx->err = clGetDeviceIDs(NULL, DEVICE_TYPE, 1, &ctx->device, NULL);
    checkError(ctx, "failed to create a device group");
    
    ctx->context = clCreateContext(0, 1, &ctx->device, NULL, NULL, &ctx->err);
    checkError(ctx, "failed to create a compute context");
    
    ctx->commands = clCreateCommandQueue(ctx->context, ctx->device,
                                         0, &ctx->err);
    checkError(ctx, "failed to create a command queue");
}

int main(int argc, const char * argv[])
{
    SimulatorContext * ctx = initializeSimulator();
}
