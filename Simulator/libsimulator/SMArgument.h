#ifndef _SM_ARGUMENT_H_
#define _SM_ARGUMENT_H_

#include <stdbool.h>
#include <sys/types.h>
#include <OpenCL/opencl.h>

#include "SMBuffer.h"

/**
 * @defgroup SMArgument SMArgument
 * @{
 */

typedef struct _SMArgument
{
    void * pointer;     /**< Pointer to arbitrary argument object */
    size_t size;        /**< Size of arbitrary argument object */

    bool owned;         /**< Whether we own (and should free) the pointer */
} SMArgument;

SMArgument * SMArgumentNew();
SMArgument * SMArgumentNewWithBuffer(SMBuffer * buf, bool backBuffer);
SMArgument * SMArgumentNewWithFloat(float f);
SMArgument * SMArgumentNewWithInt(int i);
void SMArgumentFree(SMArgument * arg);

size_t SMArgumentGetSize(SMArgument * arg);
void * SMArgumentGetPointer(SMArgument * arg);

/** @} */

#endif
