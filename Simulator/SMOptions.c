#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "SMOptions.h"
#include "SMError.h"

int simulatorUsesCPU = false;

void parseSimulatorOptions(int argc, char * argv[])
{
    char currentOption;

    while((currentOption = getopt(argc, argv, "c")) != EOF)
    {
        switch(currentOption)
        {
            case 'c':
                simulatorUsesCPU = true;
                break;
        }
    }
}
