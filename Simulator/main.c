#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SMSimulator.h"

#define ELEMENT_COUNT   (2048)
#define FRAME_SIZE      (ELEMENT_COUNT * sizeof(SMPhysicsParticle))
#define FRAME_COUNT     200
#define TOTAL_SIZE      (FRAME_SIZE * FRAME_COUNT)

int main(int argc, char * const * argv)
{
    SMContext * sim;
    SMProgram ** programs;
    SMPhysicsParticle * data, * partialResults;
    SMPhysicsNewtonian * newton;
    SMBuffer * parts, * newts, * fileBuf;
    int programCount = 2;

    srand((int)time(NULL));

    SMOptionsParse(argc, argv);

    sim = SMContextNew();

    programs = (SMProgram **)calloc(programCount, sizeof(SMProgram *));
    programs[0] = SMProgramNew(sim, "./kernels/gravity.cl");
    programs[1] = SMProgramNew(sim, "./kernels/verlet.cl");

    for(int program = 0; program < programCount; program++)
    {
        SMProgramSetGlobalCount(programs[program], ELEMENT_COUNT);
        showBuildLog(sim, programs[program]);
    }

    data = (SMPhysicsParticle *)calloc(ELEMENT_COUNT,
                                       sizeof(SMPhysicsParticle));

    newton = (SMPhysicsNewtonian *)calloc(ELEMENT_COUNT,
                                          sizeof(SMPhysicsNewtonian));

    for(unsigned int i = 0; i < ELEMENT_COUNT; i++)
    {
        data[i].enabled = 1.0;
        newton[i].ox = data[i].x = (SMFloat)rand()/(SMFloat)RAND_MAX;
        newton[i].oy = data[i].y = (SMFloat)rand()/(SMFloat)RAND_MAX;
        newton[i].oz = data[i].z = (SMFloat)rand()/(SMFloat)RAND_MAX;

        newton[i].mass = 100000000.0 * (SMFloat)rand()/(SMFloat)RAND_MAX;
    }

    parts = SMBufferNew(sim, ELEMENT_COUNT, sizeof(SMPhysicsParticle), true);
    newts = SMBufferNew(sim, ELEMENT_COUNT, sizeof(SMPhysicsNewtonian), true);

    SMBufferSet(parts, data);
    SMBufferSet(newts, newton);
    free(data);
    free(newton);

    fileBuf = SMBufferNewWithFile(sim, ELEMENT_COUNT * FRAME_COUNT,
                                  sizeof(SMPhysicsParticle), "test.out");
    printf("Created output file (%ld KB)\n", TOTAL_SIZE / 1024);

    partialResults = (SMPhysicsParticle *)SMBufferGetNativeBuffer(fileBuf);

    for(int program = 0; program < programCount; program++)
    {
        SMProgramSetArgument(programs[program], 0,
                             SMArgumentNewWithBuffer(parts, 0));
        SMProgramSetArgument(programs[program], 1,
                             SMArgumentNewWithBuffer(parts, 1));
        SMProgramSetArgument(programs[program], 2,
                             SMArgumentNewWithBuffer(newts, 0));
        SMProgramSetArgument(programs[program], 3,
                             SMArgumentNewWithBuffer(newts, 1));
        SMProgramSetArgument(programs[program], 4,
                             SMArgumentNewWithInt(ELEMENT_COUNT));
    }

    for(int step = 0; step < FRAME_COUNT; step++, partialResults += ELEMENT_COUNT)
    {
        printf("Computing frame %d/%d (%d%%)...\n", step + 1, FRAME_COUNT,
               (int)((float)(step + 1) / FRAME_COUNT * 100));

        for(int program = 0; program < programCount; program++)
        {
            SMProgramExecute(programs[program]);
            SMContextWait(sim);
        }

        SMBufferSwap(parts);
        SMBufferSwap(newts);

        SMBufferGet(parts, (void**)&partialResults);
    }

    SMBufferFree(parts);
    SMBufferFree(newts);
    SMBufferFree(fileBuf);

    for(int program = 0; program < programCount; program++)
        SMProgramFree(programs[program]);

    SMContextFree(sim);

    return EXIT_SUCCESS;
}
