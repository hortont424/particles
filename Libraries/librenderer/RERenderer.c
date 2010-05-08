/* particles - librenderer - RERenderer.c
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
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#include <liblog/liblog.h>
#include <libsimulator/libsimulator.h>
#include <libparticles/libparticles.h>

#include "librenderer.h"

#define FRAME_COUNT 500
#define RESOLUTION 500

static RERendererFrameCallback reFrameCallback = NULL;
extern const char * SMKernelSource_render;

void drawProgressBar(int width, double progress)
{
    printf(" [");

    for(int i = 0; i < width; i++)
    {
        printf(((double)i/width) < progress ? "=" : " ");
    }

    printf("]");
}

void RERendererStart()
{
    SMSimulator * reSimulator = reFrameCallback();;
    struct timeval startTime, currentTime;

    COContext * ctx = reSimulator->computer;
    COProgram * prog = COProgramNew(ctx, "render", SMKernelSource_render);
    showBuildLog(ctx, prog);
    COProgramSetGlobalCount(prog, RESOLUTION * RESOLUTION);

    COBuffer * output = COBufferNew(ctx, RESOLUTION * RESOLUTION,
                                    sizeof(PAFloat), false);

    PAFloat * image = (PAFloat *)calloc(RESOLUTION * RESOLUTION, sizeof(PAFloat));

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

        reSimulator = reFrameCallback();
        COProgramSetArgument(prog, 0, COArgumentNewWithBuffer(reSimulator->clParticles, false));
        COProgramSetArgument(prog, 1, COArgumentNewWithBuffer(reSimulator->clNewtonian, false));
        COProgramSetArgument(prog, 2, COArgumentNewWithBuffer(output, false));
        COProgramExecute(prog);
        COContextWait(ctx);
        COBufferGet(output, (void **)&image);

        printf("%f\n", image[0]);
    }

    printf("\n\n");
}

void RERendererSetFrameCallback(RERendererFrameCallback cb)
{
    reFrameCallback = cb;
}
