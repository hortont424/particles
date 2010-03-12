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

typedef enum _SMArgumentType
{
    SM_BUFFER_ARGUMENT,         /**< argument points to an SMBuffer */
    SM_POINTER_ARGUMENT         /**< argument points to an arbitrary pointer */
} SMArgumentType;

typedef struct _SMArgument
{
    SMArgumentType type;    /**< Type of argument */
    void * pointer;         /**< Pointer to arbitrary argument object */
    size_t size;            /**< Size of arbitrary argument object */

    bool owned;             /**< Whether we own (and should free) the pointer */
    bool backBuffer;        /**< If true (and this is a buffer argument), use
                                 the back buffer when accessing the buffer */

    cl_mem * bufferCache;   /**< For buffer args, a cached native pointer */
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
