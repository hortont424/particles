/* particles - libsimulator - SMProgramLibrary.c
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

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "Simulator.h"

#define LIBRARY_PATH "./kernels/"

/**
 * Allocate the space required for an SMProgramLibrary, construct a mapping of
 * SMPhysicsTypes to SMPrograms, where the programs are loaded based on the
 * given kernel filenames below.
 *
 * @param sim The simulation context to compile the programs within.
 * @return The newly allocated program library.
 */
SMProgramLibrary * SMProgramLibraryNew(SMContext * sim)
{
    SMProgramLibrary * lib;
    lib = (SMProgramLibrary *)calloc(1, sizeof(SMProgramLibrary));

    lib->context = sim;
    lib->programs = (SMProgram **)calloc(SMPhysicsLastType,
                                         sizeof(SMProgram *));

    SMProgramLibraryLoadProgram(lib, SMPhysicsGravityType, "gravity.cl");
    SMProgramLibraryLoadProgram(lib, SMPhysicsIntegrationType, "verlet.cl");

    return lib;
}

/**
 * Free the memory used by the given SMProgramLibrary and its contents.
 *
 * @param lib The program library to be freed.
 */
void SMProgramLibraryFree(SMProgramLibrary * lib)
{
    for(int i = 0; i < SMPhysicsLastType; i++)
        if(lib->programs[i])
            SMProgramFree(lib->programs[i]);

    free(lib->programs);
    free(lib);
}

/**
 * @param lib The program library to modify.
 * @param type The simulation type to load a kernel for.
 * @param filename The filename of the kernel to load.
 */
void SMProgramLibraryLoadProgram(SMProgramLibrary * lib, SMPhysicsType type,
                                 char * filename)
{
    char * kernelPath;
    size_t kernelPathLength = strlen(LIBRARY_PATH) + strlen(filename) + 1;

    kernelPath = (char *)calloc(kernelPathLength, sizeof(char));
    snprintf(kernelPath, kernelPathLength, "%s%s", LIBRARY_PATH, filename);

    lib->programs[type] = SMProgramNew(lib->context, kernelPath);
    showBuildLog(lib->context, lib->programs[type]);
}

/**
 * Set the global element count on all loaded programs.
 *
 * @param lib The program library to modify.
 * @param globalCount The new global element count.
 */
void SMProgramLibrarySetGlobalCount(SMProgramLibrary * lib, size_t globalCount)
{
    for(int i = 0; i < SMPhysicsLastType; i++)
        if(lib->programs[i])
            SMProgramSetGlobalCount(lib->programs[i], globalCount);
}

/**
 * Look up and return the loaded program for the given simulation type.
 *
 * @param lib The program library to inspect.
 * @param type The simulation type to return an SMProgram for.
 */
SMProgram * SMProgramLibraryGetProgram(SMProgramLibrary * lib,
                                       SMPhysicsType type)
{
    /// \todo Error checking, docs.
    return lib->programs[type];
}