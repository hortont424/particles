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
#include <png.h>

#include <liblog/liblog.h>
#include <libsimulator/libsimulator.h>
#include <libparticles/libparticles.h>

#include "librenderer.h"

#define FRAME_COUNT 1000
#define RESOLUTION 500

static RERendererFrameCallback reFrameCallback = NULL;
extern const char * SMKernelSource_render;

static GLbyte * rowPtrs[RESOLUTION];

void REExportImage(int frame)
{
    char filename[1024];
    FILE * file;
    png_structp png_ptr;
    png_infop info_ptr;

    snprintf(filename, 1024, "/tmp/particles/%09d.png", frame);

    file = fopen(filename, "wb");
    if(!file)
        printf("Error opening file '%s'.\n", filename);

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info_ptr = png_create_info_struct(png_ptr);

    if(setjmp(png_jmpbuf(png_ptr)))
    {
        LOError("png output");
        return;
    }

    png_init_io(png_ptr, file);
    png_set_IHDR(png_ptr, info_ptr, RESOLUTION, RESOLUTION, 8, PNG_COLOR_TYPE_GRAY,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);

    png_write_image(png_ptr, (png_byte **)rowPtrs);
    png_write_end(png_ptr, info_ptr);
    fclose(file);

    png_destroy_write_struct(&png_ptr, &info_ptr);
}

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
                                    sizeof(PAUChar), false);

    PAUChar * image = (PAUChar *)calloc(RESOLUTION * RESOLUTION, sizeof(PAUChar));
    for(unsigned int i = 0; i < RESOLUTION; i++)
        rowPtrs[i] = (GLbyte*)&image[i * RESOLUTION];

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

        //if(step == 350)
        {
            COProgramSetArgument(prog, 0, COArgumentNewWithBuffer(reSimulator->clParticles, false));
            COProgramSetArgument(prog, 1, COArgumentNewWithBuffer(reSimulator->clNewtonian, false));
            COProgramSetArgument(prog, 2, COArgumentNewWithBuffer(output, false));
            COProgramSetArgument(prog, 3, COArgumentNewWithInt(reSimulator->elementCount));
            COProgramExecute(prog);
            COContextWait(ctx);
            COBufferGet(output, (void **)&image);

            REExportImage(step);
            //exit(0);
        }

        //printf("%f\n", image[0]);
    }

    printf("\n\n");
}

void RERendererSetFrameCallback(RERendererFrameCallback cb)
{
    reFrameCallback = cb;
}
