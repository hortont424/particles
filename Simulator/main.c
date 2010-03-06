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

    prog->globalCount = 1024;

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

    SMBuffer * abuf, * bbuf;
    abuf = SMBufferNew(sim, prog->globalCount * 7, sizeof(float));
    bbuf = SMBufferNew(sim, prog->globalCount * 7, sizeof(float));

    unsigned int ct = prog->globalCount;

    int iters = 10;
    long fileSize = sizeof(float) * prog->globalCount * 7 * iters;
    int fd = open("test.out", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    // Stretch the file
    lseek(fd, fileSize, SEEK_SET);
    write(fd, "", 1);

    float * results = (float *)mmap(NULL, fileSize, PROT_READ | PROT_WRITE,
                                    MAP_SHARED, fd, 0);

    SMBufferSet(abuf, data);

    SMArgument * abufarg, * bbufarg, * countarg;
    abufarg = SMArgumentNewWithBuffer(abuf);
    bbufarg = SMArgumentNewWithBuffer(bbuf);
    countarg = SMArgumentNewWithInt(ct);

    for(int step = 0; step < iters; step++)
    {
        SMArgument * inarg, * outarg;
        inarg = (step % 2 == 0 ? abufarg : bbufarg);
        outarg = (step % 2 == 0 ? bbufarg : abufarg);

        SMProgramSetArgument(prog, 0, inarg);
        SMProgramSetArgument(prog, 1, outarg);
        SMProgramSetArgument(prog, 2, countarg);

        SMProgramExecute(prog);
        SMContextWait(sim);

        float * partialResults = results + (prog->globalCount * 7 * step);
        SMBufferGet((step % 2 == 0 ? bbuf : abuf), (void**)&partialResults);
    }

    munmap(results, fileSize);
    close(fd);
}
