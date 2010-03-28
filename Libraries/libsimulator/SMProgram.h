/* particles - libsimulator - SMProgram.h
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

#ifndef _SM_PROGRAM_H_
#define _SM_PROGRAM_H_

#include <OpenCL/opencl.h>

#include "SMContext.h"
#include "SMArgument.h"

/**
 * @defgroup SMProgram SMProgram
 * @{
 */

typedef struct _SMProgram
{
    size_t globalCount;         /**< Total number of kernel instances */
    size_t localCount;          /**< Number of parallel kernel instances */

    char * name;                /**< Name of represented OpenCL kernel */
    SMArgument ** arguments;    /**< List of kernel arguments */

    cl_program program;         /**< OpenCL program */
    cl_kernel kernel;           /**< Compiled OpenCL kernel */

    SMContext * context;        /**< The context that owns the program */
} SMProgram;

SMProgram * SMProgramNew(SMContext * sim, const char * filename);
void SMProgramFree(SMProgram * prog);

void SMProgramExecute(SMProgram * prog);

void SMProgramSetGlobalCount(SMProgram * prog, size_t globalCount);
void SMProgramSetArgument(SMProgram * prog, unsigned int i, SMArgument * arg);
SMArgument * SMProgramGetArgument(SMProgram * prog, unsigned int i);
unsigned int SMProgramGetArgumentCount(SMProgram * prog);

/** @} */

#endif
