#ifndef _SM_PROGRAM_H_
#define _SM_PROGRAM_H_

#include <OpenCL/opencl.h>

#include "SMContext.h"

typedef struct _SMProgram
{
    size_t globalCount, localCount;

    const char * name;

    cl_program program;
    cl_kernel kernel;
} SMProgram;

SMProgram * SMProgramNew(SMContext * sim, const char * filename);
void SMProgramExecute(SMContext * sim, SMProgram * prog);

#endif
