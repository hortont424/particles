#include <stdio.h>

#include "error.h"

void throwError(const char * errorMessage)
{
    printf("Error: %s\n", errorMessage);
}

void raiseOpenCLError(const char * errinfo, const void * private_info,
                      size_t cb, void * user_data)
{
    throwError(errinfo);
}

void showBuildLog(SimulatorContext * sim, SimulatorProgram * prog)
{
    size_t len;
    char buf[2048];
    
    clGetProgramBuildInfo(prog->program, sim->devs, CL_PROGRAM_BUILD_LOG,
                          sizeof(buf), buf, &len);
    
    if(buf[0] != 0)
        printf("%s\n", buf);
}