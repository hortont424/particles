/* particles - libsimulator - SMProgramLibrary.h
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

#ifndef _SM_PROGRAM_LIBRARY_H_
#define _SM_PROGRAM_LIBRARY_H_

#include <OpenCL/opencl.h>

#include <libparticles/libparticles.h>
#include <libcomputer/libcomputer.h>

/**
 * @defgroup SMProgramLibrary SMProgramLibrary
 * @{
 */

typedef struct _SMProgramLibrary
{
    COProgram ** programs;      /**< The array of loaded programs, indexed by
                                     PAPhysicsType identifier */

    COContext * context;        /**< The context that owns the programs */

    COProgram ** instances;     /**< List of instances of library programs */
    unsigned int instanceCount; /**< Number of entries in instances */
} SMProgramLibrary;

SMProgramLibrary * SMProgramLibraryNew(COContext * ctx);
void SMProgramLibraryFree(SMProgramLibrary * lib);

void SMProgramLibraryLoadProgram(SMProgramLibrary * lib, PAPhysicsType type,
                                 char * kernelName, const char * kernelSource);
void SMProgramLibrarySetGlobalCount(SMProgramLibrary * lib, size_t globalCount);
COProgram * SMProgramLibraryMakeProgram(SMProgramLibrary * lib,
                                        PAPhysicsType type);

/** @} */

#endif
