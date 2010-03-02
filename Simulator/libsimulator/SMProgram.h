#ifndef _SM_PROGRAM_H_
#define _SM_PROGRAM_H_

#include <OpenCL/opencl.h>

#include "SMContext.h"
#include "SMArgument.h"

typedef struct _SMProgram
{
    size_t globalCount, localCount;

    const char * name;
    SMArgument ** arguments;

    cl_program program;
    cl_kernel kernel;
} SMProgram;

SMProgram * SMProgramNew(SMContext * sim, const char * filename);
void SMContextExecuteProgram(SMContext * sim, SMProgram * prog);
void SMProgramSetArgument(SMProgram * prog, unsigned int i, SMArgument * arg);
unsigned int SMProgramGetArgumentCount(SMProgram * prog);

#endif
