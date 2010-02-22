#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <OpenCL/opencl.h>

#define DEVICE_TYPE CL_DEVICE_TYPE_GPU

const char *KernelSource = "\n" \
"__kernel void square(                                                  \n" \
"   __global float* input,                                              \n" \
"   __global float* output,                                             \n" \
"   const unsigned int count)                                           \n" \
"{                                                                      \n" \
"   int i = get_global_id(0);                                           \n" \
"   if(i < count)                                                       \n" \
"       output[i] = input[i] * input[i];                                \n" \
"}                                                                      \n" \
"\n";

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

void throwError(const char * errorMessage)
{
    printf("Error: %s\n", errorMessage);
}

void raiseOpenCLError(const char * errinfo, const void * private_info,
                      size_t cb, void * user_data)
{
    throwError(errinfo);
}

void showBuildLog(SimulatorContext * sim, SimulatorProgram * prog)
{
    size_t len;
    char buf[2048];
    
    clGetProgramBuildInfo(prog->program, sim->devs, CL_PROGRAM_BUILD_LOG,
                          sizeof(buf), buf, &len);
    
    printf("%s\n", buf);
}

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
    
    printf("Running kernel on %zd elements, %zd at a time.\n",
           prog->globalCount, prog->localCount);
    
    clEnqueueNDRangeKernel(sim->cmds, prog->kernel, 1, NULL, &prog->globalCount,
                           &prog->localCount, 0, NULL, NULL);
}

void waitForPrograms(SimulatorContext * sim)
{
    clFinish(sim->cmds);
}

int main(int argc, const char * argv[])
{
    SimulatorContext * sim = initializeSimulator();
    SimulatorProgram * prog = compileProgram(sim, "square", KernelSource);
    showBuildLog(sim, prog);
    
    prog->globalCount = 16;
    
    float * data = (float *)calloc(prog->globalCount, sizeof(float));
    float * results = (float *)calloc(prog->globalCount, sizeof(float));
    
    for(int i = 0; i < prog->globalCount; i++)
        data[i] = i;
    
    cl_mem input, output;
    input = clCreateBuffer(sim->ctx, CL_MEM_READ_ONLY,
                           sizeof(float) * prog->globalCount, NULL, NULL);
    output = clCreateBuffer(sim->ctx, CL_MEM_WRITE_ONLY,
                            sizeof(float) * prog->globalCount, NULL, NULL);
    
    clEnqueueWriteBuffer(sim->cmds, input, CL_TRUE, 0,
                         sizeof(float) * prog->globalCount, data, 0,
                         NULL, NULL);
    
    unsigned int ct = prog->globalCount;
    clSetKernelArg(prog->kernel, 0, sizeof(cl_mem), &input);
    clSetKernelArg(prog->kernel, 1, sizeof(cl_mem), &output);
    clSetKernelArg(prog->kernel, 2, sizeof(unsigned int), &ct);
    
    executeProgram(sim, prog);
    waitForPrograms(sim);
    
    clEnqueueReadBuffer(sim->cmds, output, CL_TRUE, 0,
                        sizeof(float) * prog->globalCount, results, 0,
                        NULL, NULL);
    
    for(int i = 0; i < prog->globalCount; i++)
        printf("%f ", results[i]);
}
