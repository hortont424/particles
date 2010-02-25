#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "simulator.h"

const char *KernelSource = "\n" \
"__kernel void square(                                                  \n" \
"   __global float* input,                                              \n" \
"   __global float* output,                                             \n" \
"   const unsigned int count)                                           \n" \
"{                                                                      \n" \
"   int i = get_global_id(0);                                           \n" \
"   if(i < count){                                                      \n" \
"       for(int j = 0; j < 1024 * 1024 * 5; j++)                        \n" \
"           output[i] = sqrt(input[i]);}                                \n" \
"}                                                                      \n" \
"\n";

int main(int argc, const char * argv[])
{
    SimulatorContext * sim = initializeSimulator();
    SimulatorProgram * prog = compileProgram(sim, "square", KernelSource);
    showBuildLog(sim, prog);
    
    prog->globalCount = 2048;
    
    float * data = (float *)calloc(prog->globalCount, sizeof(float));
    float * results = (float *)calloc(prog->globalCount, sizeof(float));
    
    for(int i = 0; i < prog->globalCount; i++)
        data[i] = pow(i, 2);
    
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
