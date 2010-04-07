/* particles - libcomputer - COProgramLibrary.c
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
#include <string.h>
#include <stdlib.h>

#include <liblog/liblog.h>

#include "libcomputer.h"

#define LIBRARY_PATH "./kernels/"

/**
 * Allocate the space required for an COProgramLibrary, construct a mapping of
 * COPhysicsTypes to COPrograms, where the programs are loaded based on the
 * given kernel filenames below.
 *
 * @param ctx The simulation context to compile the programs within.
 * @return The newly allocated program library.
 */
COProgramLibrary * COProgramLibraryNew(COContext * ctx)
{
    COProgramLibrary * lib;
    lib = (COProgramLibrary *)calloc(1, sizeof(COProgramLibrary));

    lib->context = ctx;
    lib->programs = (COProgram **)calloc(PAPhysicsLastType,
                                         sizeof(COProgram *));

    COProgramLibraryLoadProgram(lib, PAPhysicsGravityType, "gravity.cl");
    COProgramLibraryLoadProgram(lib, PAPhysicsIntegrationType, "verlet.cl");

    return lib;
}

/**
 * Free the memory used by the given COProgramLibrary and its contents.
 *
 * @param lib The program library to be freed.
 */
void COProgramLibraryFree(COProgramLibrary * lib)
{
    for(int i = 0; i < PAPhysicsLastType; i++)
        if(lib->programs[i])
            COProgramFree(lib->programs[i]);

    free(lib->programs);
    free(lib);
}

/**
 * @param lib The program library to modify.
 * @param type The simulation type to load a kernel for.
 * @param filename The filename of the kernel to load.
 */
void COProgramLibraryLoadProgram(COProgramLibrary * lib, PAPhysicsType type,
                                 char * filename)
{
    char * kernelPath;
    size_t kernelPathLength = strlen(LIBRARY_PATH) + strlen(filename) + 1;

    kernelPath = (char *)calloc(kernelPathLength, sizeof(char));
    snprintf(kernelPath, kernelPathLength, "%s%s", LIBRARY_PATH, filename);

    lib->programs[type] = COProgramNew(lib->context, kernelPath);
    showBuildLog(lib->context, lib->programs[type]);
}

/**
 * Set the global element count on all loaded programs.
 *
 * @param lib The program library to modify.
 * @param globalCount The new global element count.
 */
void COProgramLibrarySetGlobalCount(COProgramLibrary * lib, size_t globalCount)
{
    for(int i = 0; i < PAPhysicsLastType; i++)
        if(lib->programs[i])
            COProgramSetGlobalCount(lib->programs[i], globalCount);
}

/**
 * Look up and return the loaded program for the given simulation type.
 *
 * @param lib The program library to inspect.
 * @param type The simulation type to return an COProgram for.
 */
COProgram * COProgramLibraryGetProgram(COProgramLibrary * lib,
                                       PAPhysicsType type)
{
    /// \todo Error checking, docs.
    return lib->programs[type];
}