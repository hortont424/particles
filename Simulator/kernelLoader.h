#ifndef _KERNEL_LOADER_H_
#define _KERNEL_LOADER_H_

#import "simulator.h"

SimulatorProgram * loadKernel(SimulatorContext * sim, const char * filename);

#endif