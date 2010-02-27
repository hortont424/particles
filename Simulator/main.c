#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "SMSimulator.h"

int main(int argc, char * const * argv)
{
    srand((int)time(NULL));
    
    SMContext * sim = initializeSimulator(argc, argv);
    SMProgram * prog = loadKernel(sim, "./kernels/gravity.cl");
    showBuildLog(sim, prog);

    prog->globalCount = 3;

    float * data = (float *)calloc(prog->globalCount * 7, sizeof(float));
    float * results = (float *)calloc(prog->globalCount * 7, sizeof(float));

    for(unsigned int i = 0; i < prog->globalCount * 7; i += 7)
    {
        data[i + 0] = (float)rand()/(float)RAND_MAX;
        data[i + 1] = (float)rand()/(float)RAND_MAX;
        data[i + 2] = (float)rand()/(float)RAND_MAX;
        data[i + 3] = 10000000.0;
        data[i + 4] = 0.0;
        data[i + 5] = 0.0;
        data[i + 6] = 0.0;
    }

    cl_mem input, output;
    input = clCreateBuffer(sim->ctx, CL_MEM_READ_WRITE,
                           sizeof(float) * prog->globalCount * 7, NULL, NULL);
    output = clCreateBuffer(sim->ctx, CL_MEM_READ_WRITE,
                            sizeof(float) * prog->globalCount * 7, NULL, NULL);

    clEnqueueWriteBuffer(sim->cmds, input, CL_TRUE, 0,
                         sizeof(float) * prog->globalCount * 7, data, 0,
                         NULL, NULL);

    unsigned int ct = prog->globalCount;
    
    FILE * outputFile = fopen("test.out", "w");
    
    for(int step = 0; step < 100; step++)
    {
        clSetKernelArg(prog->kernel, 0, sizeof(cl_mem), (step % 2 == 0 ? &input : &output));
        clSetKernelArg(prog->kernel, 1, sizeof(cl_mem), (step % 2 == 0 ? &output : &input));
        clSetKernelArg(prog->kernel, 2, sizeof(unsigned int), &ct);

        executeProgram(sim, prog);
        waitForCompletion(sim);

        clEnqueueReadBuffer(sim->cmds, output, CL_TRUE, 0,
                            sizeof(float) * prog->globalCount * 7, results, 0,
                            NULL, NULL);
        
        for(unsigned int i = 0; i < prog->globalCount * 7; i += 7)
            fprintf(outputFile, "%f,%f,%f\n", results[i], results[i + 1],
                    results[i + 2]);
        
        fprintf(outputFile, "\n");
    }
}
