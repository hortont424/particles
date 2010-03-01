#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#include "SMSimulator.h"

int main(int argc, char * const * argv)
{
    srand((int)time(NULL));

    SMContext * sim = SMContextNew(argc, argv);
    SMProgram * prog = SMProgramNew(sim, "./kernels/gravity.cl");
    showBuildLog(sim, prog);

    prog->globalCount = 16384;

    float * data = (float *)calloc(prog->globalCount * 7, sizeof(float));

    for(unsigned int i = 0; i < prog->globalCount * 7; i += 7)
    {
        data[i + 0] = (float)rand()/(float)RAND_MAX;
        data[i + 1] = (float)rand()/(float)RAND_MAX;
        data[i + 2] = (float)rand()/(float)RAND_MAX;
        data[i + 3] = 10000000.0;
        data[i + 4] = 0.0; //((float)rand()/(float)RAND_MAX) - 0.5;
        data[i + 5] = 0.0; //((float)rand()/(float)RAND_MAX) - 0.5;
        data[i + 6] = 0.0; //((float)rand()/(float)RAND_MAX) - 0.5;
    }

    cl_mem input, output;
    cl_mem * inputbuf, * outputbuf;
    input = clCreateBuffer(sim->ctx, CL_MEM_READ_WRITE,
                           sizeof(float) * prog->globalCount * 7, NULL, NULL);
    output = clCreateBuffer(sim->ctx, CL_MEM_READ_WRITE,
                            sizeof(float) * prog->globalCount * 7, NULL, NULL);

    clEnqueueWriteBuffer(sim->cmds, input, CL_TRUE, 0,
                         sizeof(float) * prog->globalCount * 7, data, 0,
                         NULL, NULL);

    unsigned int ct = prog->globalCount;

    int iters = 5000;
    long fileSize = sizeof(float) * prog->globalCount * 7 * iters;
    int fd = open("test.out", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    // Stretch the file
    lseek(fd, fileSize, SEEK_SET);
    write(fd, "", 1);

    float * results = (float *)mmap(NULL, fileSize, PROT_READ | PROT_WRITE,
                                    MAP_SHARED, fd, 0);

    for(int step = 0; step < iters; step++)
    {
        inputbuf = (step % 2 == 0 ? &input : &output);
        outputbuf = (step % 2 == 0 ? &output : &input);

        clSetKernelArg(prog->kernel, 0, sizeof(cl_mem), inputbuf);
        clSetKernelArg(prog->kernel, 1, sizeof(cl_mem), outputbuf);
        clSetKernelArg(prog->kernel, 2, sizeof(unsigned int), &ct);

        SMProgramExecute(sim, prog);
        SMContextWait(sim);

        clEnqueueReadBuffer(sim->cmds, *outputbuf, CL_TRUE, 0,
                            sizeof(float) * prog->globalCount * 7,
                            results + (prog->globalCount * 7 * step),
                            0, NULL, NULL);
    }

    munmap(results, fileSize);
    close(fd);
}
