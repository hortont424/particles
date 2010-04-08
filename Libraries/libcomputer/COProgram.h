/* particles - libcomputer - COProgram.h
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

#ifndef _CO_PROGRAM_H_
#define _CO_PROGRAM_H_

#include <OpenCL/opencl.h>

#include "COContext.h"
#include "COArgument.h"

/**
 * @defgroup COProgram COProgram
 * @{
 */

typedef struct _COProgram
{
    size_t globalCount;         /**< Total number of kernel instances */
    size_t localCount;          /**< Number of parallel kernel instances */

    char * name;                /**< Name of represented OpenCL kernel */
    COArgument ** arguments;    /**< List of kernel arguments */

    cl_program program;         /**< OpenCL program */
    cl_kernel kernel;           /**< Compiled OpenCL kernel */

    COContext * context;        /**< The context that owns the program */
} COProgram;

COProgram * COProgramNew(COContext * ctx, const char * filename);
void COProgramFree(COProgram * prog);

void COProgramExecute(COProgram * prog);

void COProgramSetGlobalCount(COProgram * prog, size_t globalCount);
void COProgramSetArgument(COProgram * prog, unsigned int i, COArgument * arg);
COArgument * COProgramGetArgument(COProgram * prog, unsigned int i);
unsigned int COProgramGetArgumentCount(COProgram * prog);

/** @} */

#endif
