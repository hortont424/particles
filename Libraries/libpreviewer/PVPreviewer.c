/* particles - libpreviewer - PVPreviewer.c
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
#include <math.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <stdbool.h>
#include <png.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <liblog/liblog.h>
#include <libparticles/libparticles.h>

#include "libpreviewer.h"

static PVPreviewerFrameCallback pvFrameCallback = NULL;
static bool pvInitialized = false;
static SMSimulator * simulator = NULL;

int frameCount = 0;
static GLbyte * outputImage = NULL;
static GLbyte * rowPtrs[800];

void PVExportImage(int frame)
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
    png_set_IHDR(png_ptr, info_ptr, 800, 800, 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);

    glReadPixels(0, 0, 800, 800, GL_RGB, GL_UNSIGNED_BYTE, outputImage);

    png_write_image(png_ptr, (png_byte **)rowPtrs);
    png_write_end(png_ptr, info_ptr);
    fclose(file);

    png_destroy_write_struct(&png_ptr, &info_ptr);
}

void timer(int extra)
{
    if(pvFrameCallback)
        simulator = pvFrameCallback();

    glutPostRedisplay();
    glutTimerFunc(17, timer, 0);
}

static void display()
{
    float zoom = 0.2;

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(0.0 - zoom, 1.0 + zoom, 0.0 - zoom, 1.0 + zoom, -100.0, 100.0);

    glColor3f(1.0, 0.0, 0.0);
    glPointSize(2.0);

    if(simulator)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(float) * 4, (float *)simulator->particles + 1);
        glDrawArrays(GL_POINTS, 0, simulator->elementCount);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    // TODO: MAKE AN OPTION FOR IMAGE EXPORT
    //PVExportImage(frameCount);

    glutSwapBuffers();
    frameCount++;
}

void PVPreviewerInit(int * argc, const char ** argv)
{
    if(pvInitialized)
    {
        LOError("trying to initialize libpreviewer more than once");
        return;
    }
    else
    {
        pvInitialized = true;
    }

    glutInit(argc, (char **)argv);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Previewer");

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);

    outputImage = calloc(1, 800 * 800 * 3 * sizeof(GLbyte));
    for(unsigned int i = 0; i < 800; i++)
        rowPtrs[i] = &outputImage[i * 800 * 3];
}

void PVPreviewerStart()
{
    glutMainLoop();
}

void PVPreviewerSetFrameCallback(PVPreviewerFrameCallback cb)
{
    pvFrameCallback = cb;
}
