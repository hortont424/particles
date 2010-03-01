#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "SMSimulator.h"

int simulatorUsesCPU = false;

void SMOptionsParse(int argc, char * const * argv)
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
