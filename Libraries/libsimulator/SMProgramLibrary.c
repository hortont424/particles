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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <liblog/liblog.h>

#include "SMProgramLibrary.h"

extern const char * SMKernelSource_normal;
extern const char * SMKernelSource_gravity;
extern const char * SMKernelSource_nbody;
extern const char * SMKernelSource_verlet;

/**
 * Allocate the space required for an SMProgramLibrary, construct a mapping of
 * COPhysicsTypes to COPrograms, where the programs are loaded based on the
 * given kernel filenames below.
 *
 * @param ctx The simulation context to compile the programs within.
 * @return The newly allocated program library.
 */
SMProgramLibrary * SMProgramLibraryNew(COContext * ctx)
{
    SMProgramLibrary * lib;
    lib = (SMProgramLibrary *)calloc(1, sizeof(SMProgramLibrary));

    lib->instances = NULL;
    lib->instanceCount = 0;
    lib->context = ctx;
    lib->programs = (COProgram **)calloc(PAPhysicsLastType,
                                         sizeof(COProgram *));

    SMProgramLibraryLoadProgram(lib, PAPhysicsNormalType, "normal",
                                SMKernelSource_normal);
    SMProgramLibraryLoadProgram(lib, PAPhysicsGravityType, "gravity",
                                SMKernelSource_gravity);
    SMProgramLibraryLoadProgram(lib, PAPhysicsNBodyType, "nbody",
                                SMKernelSource_nbody);
    SMProgramLibraryLoadProgram(lib, PAPhysicsIntegrationType, "verlet",
                                SMKernelSource_verlet);

    return lib;
}

/**
 * Free the memory used by the given SMProgramLibrary and its contents.
 *
 * @param lib The program library to be freed.
 */
void SMProgramLibraryFree(SMProgramLibrary * lib)
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
 * @param kernelName The name of the given kernel.
 * @param kernelSource The OpenCL source of the kernel to load.
 */
void SMProgramLibraryLoadProgram(SMProgramLibrary * lib, PAPhysicsType type,
                                 char * kernelName, const char * kernelSource)
{
    lib->programs[type] = COProgramNew(lib->context, kernelName, kernelSource);
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
    for(int i = 0; i < PAPhysicsLastType; i++)
        if(lib->programs[i])
            COProgramSetGlobalCount(lib->programs[i], globalCount);

    for(unsigned int i = 0; i < lib->instanceCount; i++)
        if(lib->instances[i])
            COProgramSetGlobalCount(lib->instances[i], globalCount);
}

/**
 * Look up and return the loaded program for the given simulation type.
 *
 * @param lib The program library to inspect.
 * @param type The simulation type to return an COProgram for.
 */
COProgram * SMProgramLibraryMakeProgram(SMProgramLibrary * lib,
                                        PAPhysicsType type)
{
    COProgram * prog;

    prog = lib->programs[type];

    if(!prog)
    {
        LOError("no kernel loaded for type %d", type);
        return NULL;
    }

    prog = COProgramCopy(prog);
    lib->instanceCount++;

    if(!lib->instances)
        lib->instances = (COProgram **)calloc(lib->instanceCount,
                                              sizeof(COProgram *));
    else
        lib->instances = (COProgram **)realloc(lib->instances,
                                               lib->instanceCount *
                                               sizeof(COProgram *));

    lib->instances[lib->instanceCount - 1] = prog;

    return prog;
}
