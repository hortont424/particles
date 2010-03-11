#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SMSimulator.h"

#define ELEMENT_COUNT   (1024*2)
#define FRAME_SIZE      (ELEMENT_COUNT * sizeof(SMPhysicsParticle))
#define FRAME_COUNT     100
#define TOTAL_SIZE      (FRAME_SIZE * FRAME_COUNT)

int main(int argc, char * const * argv)
{
    SMContext * sim;
    SMProgram * prog;
    SMPhysicsParticle * data, * partialResults;
    SMBuffer * abuf, * bbuf, * fileBuf;
    SMArgument * abufarg, * bbufarg, * countarg;

    srand((int)time(NULL));

    SMOptionsParse(argc, argv);

    sim = SMContextNew();
    prog = SMProgramNew(sim, "./kernels/passthrough.cl");
    SMProgramSetGlobalCount(prog, ELEMENT_COUNT);
    showBuildLog(sim, prog);

    data = (SMPhysicsParticle *)calloc(ELEMENT_COUNT,
                                       sizeof(SMPhysicsParticle));

    for(unsigned int i = 0; i < ELEMENT_COUNT; i++)
    {
        data[i].enabled = 1.0;
        data[i].x = (SMFloat)rand()/(SMFloat)RAND_MAX;
        data[i].y = (SMFloat)rand()/(SMFloat)RAND_MAX;
        data[i].z = (SMFloat)rand()/(SMFloat)RAND_MAX;
    }

    abuf = SMBufferNew(sim, ELEMENT_COUNT, sizeof(SMPhysicsParticle));
    bbuf = SMBufferNew(sim, ELEMENT_COUNT, sizeof(SMPhysicsParticle));

    SMBufferSet(abuf, data);
    free(data);

    fileBuf = SMBufferNewWithFile(sim, ELEMENT_COUNT * FRAME_COUNT,
                                  sizeof(SMPhysicsParticle), "test.out");
    printf("Created output file (%ld KB)\n", TOTAL_SIZE / 1024);

    abufarg = SMArgumentNewWithBuffer(abuf);
    bbufarg = SMArgumentNewWithBuffer(bbuf);
    countarg = SMArgumentNewWithInt(ELEMENT_COUNT);

    partialResults = (SMPhysicsParticle *)SMBufferGetNativeBuffer(fileBuf);

    for(int step = 0; step < FRAME_COUNT; step++, partialResults += ELEMENT_COUNT)
    {
        printf("Computing frame %d/%d (%d%%)...\n", step + 1, FRAME_COUNT,
               (int)((float)(step + 1) / FRAME_COUNT * 100));
        SMProgramSetArgument(prog, 0, (step % 2 ? bbufarg : abufarg));
        SMProgramSetArgument(prog, 1, (step % 2 ? abufarg : bbufarg));
        SMProgramSetArgument(prog, 2, countarg);

        SMProgramExecute(prog);
        SMContextWait(sim);

        SMBufferGet((step % 2 ? abuf : bbuf), (void**)&partialResults);
    }

    SMBufferFree(abuf);
    SMBufferFree(bbuf);
    SMBufferFree(fileBuf);
    SMArgumentFree(abufarg);
    SMArgumentFree(bbufarg);
    SMArgumentFree(countarg);
    SMProgramFree(prog);
    SMContextFree(sim);

    return EXIT_SUCCESS;
}
