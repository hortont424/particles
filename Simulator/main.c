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
#include <libsimulator/libsimulator.h>
#include <libpreviewer/libpreviewer.h>

#define BENCHMARK_FRAMES 10

static SMSimulator * simulator;

SMSimulator * simulateFrame()
{
    SMSimulatorSimulate(simulator);
    SMSimulatorPullData(simulator);

    return simulator;
}

int main(int argc, const char ** argv)
{
    SMOptionsParse(argc, argv);

    simulator = SMSimulatorNewFromFile("../Systems/gravity.psys");
    SMSimulatorPushData(simulator);

    if(simulatorOutputMode == SM_PREVIEWER_OUTPUT)
    {
        PVPreviewerInit(&argc, argv);
        PVPreviewerSetFrameCallback(simulateFrame);
        PVPreviewerStart();
    }
    else if(simulatorOutputMode == SM_NO_OUTPUT)
    {
        for(unsigned int i = 0; i < BENCHMARK_FRAMES; i++)
            SMSimulatorSimulate(simulator);
    }

    return EXIT_SUCCESS;
}

/*void drawProgressBar(int width, double progress)
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
    struct timeval startTime, currentTime;
    SMSimulator * simulator;

    COOptionsParse(argc, argv);

    simulator = SMSimulatorNewFromFile("../Systems/sample.psys");
    SMSimulatorPushData(simulator);

    printf("\n");
    gettimeofday(&startTime, NULL);

    for(int step = 0; step < FRAME_COUNT; step++)
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

            printf(" %*.1lf sec",
                   (int)ceil(log10(FRAME_COUNT * (elapsed / step))) + 3,
                   (FRAME_COUNT - step) * (elapsed / step));
        }

        printf("\r");
        fflush(stdout);

        SMSimulatorSimulate(simulator);
    }

    printf("\n\n");

    return EXIT_SUCCESS;
}*/
