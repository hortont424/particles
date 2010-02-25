#ifndef _ERROR_H_
#define _ERROR_H_

#include "simulator.h"

void throwError(const char * errorMessage);
void raiseOpenCLError(const char * errinfo, const void * private_info,
                      size_t cb, void * user_data);
void showBuildLog(SimulatorContext * sim, SimulatorProgram * prog);

#endif