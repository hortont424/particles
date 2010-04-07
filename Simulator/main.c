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
#include <sys/time.h>
#include <math.h>

#include <liblog/liblog.h>
#include <libparticles/libparticles.h>
#include <libcomputer/libcomputer.h>

#include <json.h>

#define ELEMENT_COUNT   4096
#define FRAME_SIZE      (ELEMENT_COUNT * sizeof(PAPhysicsParticle))
#define FRAME_COUNT     100
#define TOTAL_SIZE      (FRAME_SIZE * FRAME_COUNT)

void drawProgressBar(int width, double progress)
{
    printf(" [");

    for(int i = 0; i < width; i++)
    {
        printf(((double)i/width) < progress ? "=" : " ");
    }

    printf("]");
}

int main(int argc, char ** argv)
{
    COContext * sim;
    COProgram * gravProg, * intProg;
    COProgramLibrary * library;
    PAPhysicsParticle * data, * partialResults;
    PAPhysicsNewtonian * newton;
    PASystem * system;
    COBuffer * parts, * newts, * fileBuf;
    int step;
    struct timeval startTime, currentTime;

    srand((int)time(NULL));

    COOptionsParse(argc, argv);

    system = PASystemNewFromFile("../Systems/sample.psys");
    exit(EXIT_SUCCESS);

    sim = COContextNew();

    library = COProgramLibraryNew(sim);
    COProgramLibrarySetGlobalCount(library, ELEMENT_COUNT);

    data = (PAPhysicsParticle *)calloc(ELEMENT_COUNT,
                                       sizeof(PAPhysicsParticle));

    newton = (PAPhysicsNewtonian *)calloc(ELEMENT_COUNT,
                                          sizeof(PAPhysicsNewtonian));

    for(unsigned int i = 0; i < ELEMENT_COUNT; i++)
    {
        data[i].enabled = 1.0;
        newton[i].ox = data[i].x = (PAFloat)rand()/(PAFloat)RAND_MAX;
        newton[i].oy = data[i].y = (PAFloat)rand()/(PAFloat)RAND_MAX;
        newton[i].oz = data[i].z = (PAFloat)rand()/(PAFloat)RAND_MAX/5.0;

        newton[i].mass = 50000000.0 * (PAFloat)rand()/(PAFloat)RAND_MAX;
    }

    parts = COBufferNew(sim, ELEMENT_COUNT, sizeof(PAPhysicsParticle), true);
    newts = COBufferNew(sim, ELEMENT_COUNT, sizeof(PAPhysicsNewtonian), true);

    printf("Allocated video memory (%ld KB)\n",
           ((2 * ELEMENT_COUNT * sizeof(PAPhysicsParticle)) +
           (2 * ELEMENT_COUNT * sizeof(PAPhysicsNewtonian))) / 1024);

    COBufferSet(parts, data);
    COBufferSet(newts, newton);
    free(data);
    free(newton);

    fileBuf = COBufferNewWithFile(sim, ELEMENT_COUNT * FRAME_COUNT,
                                  sizeof(PAPhysicsParticle), "test.out");
    printf("Created output file (%ld KB)\n", TOTAL_SIZE / 1024);

    partialResults = (PAPhysicsParticle *)COBufferGetNativeBuffer(fileBuf);

    gravProg = COProgramLibraryGetProgram(library, PAPhysicsGravityType);
    intProg = COProgramLibraryGetProgram(library, PAPhysicsIntegrationType);

    COProgramSetArgument(gravProg, 0, COArgumentNewWithBuffer(parts, 0));
    COProgramSetArgument(gravProg, 1, COArgumentNewWithBuffer(parts, 1));
    COProgramSetArgument(gravProg, 2, COArgumentNewWithBuffer(newts, 0));
    COProgramSetArgument(gravProg, 3, COArgumentNewWithBuffer(newts, 1));
    COProgramSetArgument(gravProg, 4, COArgumentNewWithInt(ELEMENT_COUNT));

    COProgramSetArgument(intProg, 0, COArgumentNewWithBuffer(parts, 0));
    COProgramSetArgument(intProg, 1, COArgumentNewWithBuffer(parts, 1));
    COProgramSetArgument(intProg, 2, COArgumentNewWithBuffer(newts, 0));
    COProgramSetArgument(intProg, 3, COArgumentNewWithBuffer(newts, 1));
    COProgramSetArgument(intProg, 4, COArgumentNewWithInt(ELEMENT_COUNT));

    printf("\n");
    gettimeofday(&startTime, NULL);

    for(step = 0; step < FRAME_COUNT; step++, partialResults += ELEMENT_COUNT)
    {
        // Clear the line
        printf("\033[K");
        printf("%*d/%d", (int)ceil(log10(FRAME_COUNT)), step + 1, FRAME_COUNT);
        drawProgressBar(50, ((double)step) / FRAME_COUNT);

        // Print time estimate
        if(step > 0)
        {
            gettimeofday(&currentTime, NULL);
            double elapsed = (currentTime.tv_sec - startTime.tv_sec) +
                (double)(currentTime.tv_usec - startTime.tv_usec) / 1000000;

            printf("%*.1lf sec",
                   (int)ceil(log10(FRAME_COUNT * (elapsed / step))) + 3,
                   (FRAME_COUNT - step) * (elapsed / step));
        }

        printf("\r");
        fflush(stdout);

        COProgramExecute(gravProg);
        COContextWait(sim);
        COProgramExecute(intProg);
        COContextWait(sim);

        COBufferSwap(parts);
        COBufferSwap(newts);

        COBufferGet(parts, (void**)&partialResults);
    }

    printf("\n\n");

    COBufferFree(parts);
    COBufferFree(newts);
    COBufferFree(fileBuf);
    COProgramLibraryFree(library);
    COContextFree(sim);

    return EXIT_SUCCESS;
}
