#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "SMSimulator.h"

int simulatorUsesCPU = false;

/**
 * Parse the given command line options, setting flags relevant to our
 * interests.
 *
 * @todo This should be made much more robust; it should remove arguments from
 * the argc/argv; it should provide -h/--help, etc.
 *
 * @param argc The number of arguments in the argument array.
 * @param argv The command line argument array.
 */
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
