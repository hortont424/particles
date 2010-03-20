/* particles - simulator - main.c
 *
 * Copyright 2010 Tim Horton. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY TIM HORTON "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL TIM HORTON OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "libsimulator/Simulator.h"

#define ELEMENT_COUNT   2048
#define FRAME_SIZE      (ELEMENT_COUNT * sizeof(SMPhysicsParticle))
#define FRAME_COUNT     200
#define TOTAL_SIZE      (FRAME_SIZE * FRAME_COUNT)

int main(int argc, char ** argv)
{
    SMContext * sim;
    SMProgram * gravProg, * intProg;
    SMProgramLibrary * library;
    SMPhysicsParticle * data, * partialResults;
    SMPhysicsNewtonian * newton;
    SMBuffer * parts, * newts, * fileBuf;
    int step;

    srand((int)time(NULL));

    SMOptionsParse(argc, argv);

    sim = SMContextNew();

    library = SMProgramLibraryNew(sim);
    SMProgramLibrarySetGlobalCount(library, ELEMENT_COUNT);

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

    gravProg = SMProgramLibraryGetProgram(library, SMPhysicsGravityType);
    intProg = SMProgramLibraryGetProgram(library, SMPhysicsIntegrationType);

    SMProgramSetArgument(gravProg, 0, SMArgumentNewWithBuffer(parts, 0));
    SMProgramSetArgument(gravProg, 1, SMArgumentNewWithBuffer(parts, 1));
    SMProgramSetArgument(gravProg, 2, SMArgumentNewWithBuffer(newts, 0));
    SMProgramSetArgument(gravProg, 3, SMArgumentNewWithBuffer(newts, 1));
    SMProgramSetArgument(gravProg, 4, SMArgumentNewWithInt(ELEMENT_COUNT));

    SMProgramSetArgument(intProg, 0, SMArgumentNewWithBuffer(parts, 0));
    SMProgramSetArgument(intProg, 1, SMArgumentNewWithBuffer(parts, 1));
    SMProgramSetArgument(intProg, 2, SMArgumentNewWithBuffer(newts, 0));
    SMProgramSetArgument(intProg, 3, SMArgumentNewWithBuffer(newts, 1));
    SMProgramSetArgument(intProg, 4, SMArgumentNewWithInt(ELEMENT_COUNT));

    for(step = 0; step < FRAME_COUNT; step++, partialResults += ELEMENT_COUNT)
    {
        printf("Computing frame %d/%d (%d%%)...\n", step + 1, FRAME_COUNT,
               (int)((float)(step + 1) / FRAME_COUNT * 100));

        SMProgramExecute(gravProg);
        SMContextWait(sim);
        SMProgramExecute(intProg);
        SMContextWait(sim);

        SMBufferSwap(parts);
        SMBufferSwap(newts);

        SMBufferGet(parts, (void**)&partialResults);
    }

    SMBufferFree(parts);
    SMBufferFree(newts);
    SMBufferFree(fileBuf);
    SMProgramLibraryFree(library);
    SMContextFree(sim);

    return EXIT_SUCCESS;
}
