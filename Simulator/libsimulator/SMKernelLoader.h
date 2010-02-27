#ifndef _KERNEL_LOADER_H_
#define _KERNEL_LOADER_H_

#import "SMContext.h"
#import "SMProgram.h"

SMProgram * loadKernel(SMContext * sim, const char * filename);

#endif
