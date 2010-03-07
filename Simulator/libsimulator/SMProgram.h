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

    const char * name;          /**< Name of represented OpenCL kernel */
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
unsigned int SMProgramGetArgumentCount(SMProgram * prog);

/** @} */

#endif
