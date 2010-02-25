#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SMSimulator.h"

int main(int argc, const char * argv[])
{
    SMContext * sim = initializeSimulator();
    SMProgram * prog = loadKernel(sim, "./kernels/square.cl");
    showBuildLog(sim, prog);
    
    prog->globalCount = 32;
    
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
