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

#define ELEMENT_SIZE    7
#define ELEMENT_COUNT   1024
#define FRAME_SIZE      (ELEMENT_SIZE * ELEMENT_COUNT)
#define FRAME_COUNT     1000

int main(int argc, char * const * argv)
{
    srand((int)time(NULL));

    SMContext * sim = SMContextNew(argc, argv);
    SMProgram * prog = SMProgramNew(sim, "./kernels/gravity.cl");
    showBuildLog(sim, prog);

    prog->globalCount = ELEMENT_COUNT;

    float * data = (float *)calloc(FRAME_SIZE, sizeof(float));

    for(unsigned int i = 0; i < FRAME_SIZE; i += ELEMENT_SIZE)
    {
        data[i + 0] = (float)rand()/(float)RAND_MAX;
        data[i + 1] = (float)rand()/(float)RAND_MAX;
        data[i + 2] = (float)rand()/(float)RAND_MAX;
        data[i + 3] = 10000000.0;
        data[i + 4] = 0.0;
        data[i + 5] = 0.0;
        data[i + 6] = 0.0;
    }

    SMBuffer * abuf, * bbuf, * fileBuf;
    abuf = SMBufferNew(sim, FRAME_SIZE, sizeof(float));
    bbuf = SMBufferNew(sim, FRAME_SIZE, sizeof(float));

    unsigned int ct = prog->globalCount;
    
    fileBuf = SMBufferNewWithFile(sim, FRAME_SIZE * FRAME_COUNT,
                                  sizeof(float), "test.out");

    SMBufferSet(abuf, data);

    SMArgument * abufarg, * bbufarg, * countarg;
    abufarg = SMArgumentNewWithBuffer(abuf);
    bbufarg = SMArgumentNewWithBuffer(bbuf);
    countarg = SMArgumentNewWithInt(ct);

    for(int step = 0; step < FRAME_COUNT; step++)
    {
        SMArgument * inarg, * outarg;
        inarg = (step % 2 == 0 ? abufarg : bbufarg);
        outarg = (step % 2 == 0 ? bbufarg : abufarg);

        SMProgramSetArgument(prog, 0, inarg);
        SMProgramSetArgument(prog, 1, outarg);
        SMProgramSetArgument(prog, 2, countarg);

        SMProgramExecute(prog);
        SMContextWait(sim);

        float * partialResults = (float *)SMBufferGetNativeBuffer(fileBuf) +
            (FRAME_SIZE * step);
        SMBufferGet((step % 2 == 0 ? bbuf : abuf), (void**)&partialResults);
    }

    SMBufferFree(abuf);
    SMBufferFree(bbuf);
    SMBufferFree(fileBuf);
    
    return 0;
}
