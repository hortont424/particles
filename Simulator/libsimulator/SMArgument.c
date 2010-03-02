#include <stdlib.h>

#include "SMSimulator.h"

SMArgument * SMArgumentNew(SMArgumentType type)
{
    SMArgument * arg = (SMArgument *)calloc(1, sizeof(SMArgument));

    arg->type = type;

    return arg;
}

SMArgument * SMArgumentFree(SMArgument * arg)
{
    free(arg);
}
