#ifndef _SM_ERROR_H_
#define _SM_ERROR_H_

#include "SMContext.h"
#include "SMProgram.h"

/**
 * @defgroup SMError SMError
 * @{
 */

#define throwError(f, ...) printf("Error (%s, line %d): " f "\n", \
                                  __FILE__, __LINE__, ##__VA_ARGS__)

void raiseOpenCLError(const char * errinfo, const void * private_info,
                      size_t cb, void * user_data);
void showBuildLog(SMContext * sim, SMProgram * prog);

/** @} */

#endif
