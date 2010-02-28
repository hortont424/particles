#ifndef _KERNEL_LOADER_H_
#define _KERNEL_LOADER_H_

#include "SMContext.h"
#include "SMProgram.h"

SMProgram * loadKernel(SMContext * sim, const char * filename);

#endif
