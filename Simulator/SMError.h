#ifndef _SM_ERROR_H_
#define _SM_ERROR_H_

#include "SMContext.h"
#include "SMProgram.h"

void throwError(const char * errorMessage);
void raiseOpenCLError(const char * errinfo, const void * private_info,
                      size_t cb, void * user_data);
void showBuildLog(SMContext * sim, SMProgram * prog);

#endif