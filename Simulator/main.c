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
#define ELEMENT_COUNT   2048
#define FRAME_SIZE      (ELEMENT_SIZE * ELEMENT_COUNT)
#define FRAME_COUNT     100
#define TOTAL_SIZE      (FRAME_SIZE * FRAME_COUNT)

int main(int argc, char * const * argv)
{
    SMContext * sim;
    SMProgram * prog;
    float * data, * partialResults;
    SMBuffer * abuf, * bbuf, * fileBuf;
    SMArgument * abufarg, * bbufarg, * countarg;
    
    srand((int)time(NULL));

    sim = SMContextNew(argc, argv);
    prog = SMProgramNew(sim, "./kernels/gravity.cl");
    SMProgramSetGlobalCount(prog, ELEMENT_COUNT);
    showBuildLog(sim, prog);
    
    data = (float *)calloc(FRAME_SIZE, sizeof(float));

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

    abuf = SMBufferNew(sim, FRAME_SIZE, sizeof(float));
    bbuf = SMBufferNew(sim, FRAME_SIZE, sizeof(float));
    SMBufferSet(abuf, data);
    
    fileBuf = SMBufferNewWithFile(sim, TOTAL_SIZE, sizeof(float), "test.out");

    abufarg = SMArgumentNewWithBuffer(abuf);
    bbufarg = SMArgumentNewWithBuffer(bbuf);
    countarg = SMArgumentNewWithInt(ELEMENT_COUNT);
    
    partialResults = (float*)SMBufferGetNativeBuffer(fileBuf);

    for(int step = 0; step < FRAME_COUNT; step++, partialResults += FRAME_SIZE)
    {
        SMProgramSetArgument(prog, 0, (step % 2 ? bbufarg : abufarg));
        SMProgramSetArgument(prog, 1, (step % 2 ? abufarg : bbufarg));
        SMProgramSetArgument(prog, 2, countarg);

        SMProgramExecute(prog);
        SMContextWait(sim);

        SMBufferGet((step % 2 == 0 ? bbuf : abuf), (void**)&partialResults);
    }

    SMBufferFree(abuf);
    SMBufferFree(bbuf);
    SMBufferFree(fileBuf);
    
    return EXIT_SUCCESS;
}
