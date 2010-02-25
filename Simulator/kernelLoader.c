#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <string.h>

#include "kernelLoader.h"
#include "error.h"

char * kernelNameFromFilename(const char * filename)
{
    char * lastSlash, * lastDot, * kernelName;
    unsigned int kernelNameLength;
    
    lastSlash = strrchr(filename, '/');
    lastDot = strrchr(filename, '.');
    
    kernelNameLength = (unsigned int)(lastDot - lastSlash - 1);
    kernelName = (char *)calloc(kernelNameLength + 1, sizeof(char));
    
    return strncpy(kernelName, lastSlash + 1, kernelNameLength);
}

SimulatorProgram * loadKernel(SimulatorContext * sim, const char * filename)
{
    struct stat fileInfo;
    int fileHandle;
    char * fileContent;
    char * kernelName;
    SimulatorProgram * prog;
    
    if(!strstr(filename, ".cl"))
    {
        throwError("not an OpenCL kernel file");
        return NULL;
    }
    
    fileHandle = open(filename, O_RDONLY);
    
    if(fileHandle == -1 || fstat(fileHandle, &fileInfo) == -1)
    {
        throwError("kernel file not found");
        return NULL;
    }

    // Since we use stat's size, we can't allow symlinks here
    if(!S_ISREG(fileInfo.st_mode))
    {
        throwError("kernel file is non-regular; symbolic link?");
        return NULL;
    }
    
    kernelName = kernelNameFromFilename(filename);
    
    fileContent = (char *)calloc(fileInfo.st_size + 1, sizeof(char));
    read(fileHandle, fileContent, fileInfo.st_size + 1);
    
    prog = compileProgram(sim, kernelName, fileContent);
    
    if(prog)
        printf("Successfully loaded kernel '%s'\n", kernelName);
    
    return prog;
}