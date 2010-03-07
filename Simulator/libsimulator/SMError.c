#include <stdio.h>

#include "SMSimulator.h"

void raiseOpenCLError(const char * errinfo, const void * private_info,
                      size_t cb, void * user_data)
{
    throwError("%s", errinfo);
}

void showBuildLog(SMContext * sim, SMProgram * prog)
{
    size_t len;
    char buf[2048];

    clGetProgramBuildInfo(prog->program, sim->devs, CL_PROGRAM_BUILD_LOG,
                          sizeof(buf), buf, &len);

    if(buf[0] != 0)
        printf("%s\n", buf);
}
