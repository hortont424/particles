#include <stdlib.h>

#include "SMSimulator.h"

/**
 * Allocate the space required for an empty SMArgument.
 *
 * @return The newly allocated empty argument.
 */
SMArgument * SMArgumentNew()
{
    return (SMArgument *)calloc(1, sizeof(SMArgument));
}

/**
 * Allocate the space required for an SMArgument, and fill it with a pointer
 * to the given OpenCL buffer (this doesn't work with file buffers, because
 * they aren't cl_mem objects and as such can't be passed into an OpenCL
 * kernel).
 *
 * @param buf The OpenCL-backed SMBuffer the argument should point at.
 * @return The newly allocated argument, pointing at an SMBuffer.
 */
SMArgument * SMArgumentNewWithBuffer(SMBuffer * buf)
{
    SMArgument * arg = SMArgumentNew();

    cl_mem * nativeBuffer = (cl_mem *)calloc(1, sizeof(cl_mem));
    (*nativeBuffer) = SMBufferGetCLBuffer(buf);

    arg->size = sizeof(cl_mem);
    arg->pointer = (void *)nativeBuffer;
    arg->owned = false;

    return arg;
}

/**
 * Allocate the space required for an SMArgument, and fill it with a pointer
 * to a copy of the given float.
 *
 * @param f A float for the argument to point at.
 * @return The newly allocated argument, pointing at a copy of the given float.
 */
SMArgument * SMArgumentNewWithFloat(float f)
{
    SMArgument * arg = SMArgumentNew();

    arg->size = sizeof(float);
    arg->pointer = (float *)calloc(1, sizeof(float));
    *((float *)arg->pointer) = f;
    arg->owned = true;

    return arg;
}

/**
 * Allocate the space required for an SMArgument, and fill it with a pointer
 * to a copy of the given int.
 *
 * @param i A int for the argument to point at.
 * @return The newly allocated argument, pointing at a copy of the given int.
 */
SMArgument * SMArgumentNewWithInt(int i)
{
    SMArgument * arg = SMArgumentNew();

    arg->size = sizeof(int);
    arg->pointer = (float *)calloc(1, sizeof(int));
    *((int *)arg->pointer) = i;
    arg->owned = true;

    return arg;
}

/**
 * Free the memory used by the given SMArgument, also freeing the object it
 * points at, if we own it.
 *
 * @param arg The SMArgument to free.
 */
void SMArgumentFree(SMArgument * arg)
{
    if(arg->owned)
        free(arg->pointer);

    free(arg);
}

/**
 * @param arg The SMArgument to inspect.
 * @return The size of the object the argument points at.
 */
size_t SMArgumentGetSize(SMArgument * arg)
{
    return arg->size;
}

/**
 * @param arg The SMArgument to inspect.
 * @return A pointer to the object the argument points at.
 */
void * SMArgumentGetPointer(SMArgument * arg)
{
    return arg->pointer;
}
