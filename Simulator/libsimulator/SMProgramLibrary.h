#ifndef _SM_PROGRAM_LIBRARY_H_
#define _SM_PROGRAM_LIBRARY_H_

#include <OpenCL/opencl.h>

#include "SMContext.h"
#include "SMProgram.h"
#include "SMPhysics.h"

/**
 * @defgroup SMProgramLibrary SMProgramLibrary
 * @{
 */

typedef struct _SMProgramLibrary
{
    SMProgram ** programs;      /**< The array of loaded programs, indexed by
                                     SMPhysicsType identifier */

    SMContext * context;        /**< The context that owns the programs */
} SMProgramLibrary;

SMProgramLibrary * SMProgramLibraryNew(SMContext * sim);
void SMProgramLibraryFree(SMProgramLibrary * lib);

void SMProgramLibraryLoadProgram(SMProgramLibrary * lib, SMPhysicsType type,
                                 char * filename);
void SMProgramLibrarySetGlobalCount(SMProgramLibrary * lib, size_t globalCount);
SMProgram * SMProgramLibraryGetProgram(SMProgramLibrary * lib,
                                       SMPhysicsType type);

/** @} */

#endif
