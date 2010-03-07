#ifndef _SM_BUFFER_H_
#define _SM_BUFFER_H_

#include <sys/types.h>
#include <OpenCL/opencl.h>

#include "SMContext.h"

/**
 * @defgroup SMBuffer SMBuffer
 * @{
 */

typedef enum _SMBufferType
{
    SM_FILE_BUFFER,
    SM_OPENCL_BUFFER
} SMBufferType;

typedef struct _SMBuffer
{
    SMBufferType type;      /**< Type of buffer */
    
    cl_mem gpuBuffer;       /**< OpenCL buffer (for SM_OPENCL_BUFFER) */
    void * fileBuffer;      /**< Native buffer (for SM_FILE_BUFFER) */
    int file;               /**< File descriptor (for SM_FILE_BUFFER) */

    size_t elementSize;     /**< Size of each element in the buffer */
    long elementCount;      /**< Number of elements in the buffer */

    SMContext * context;    /**< The context that owns the buffer */
} SMBuffer;

SMBuffer * SMBufferNew(SMContext * sim, long elementCount, size_t elementSize);
SMBuffer * SMBufferNewWithFile(SMContext * sim, long elementCount,
                               size_t elementSize, const char * filename);
void SMBufferFree(SMBuffer * buf);

size_t SMBufferGetSize(SMBuffer * buf);
long SMBufferGetElementCount(SMBuffer * buf);
size_t SMBufferGetElementSize(SMBuffer * buf);
cl_mem SMBufferGetCLBuffer(SMBuffer * buf);
void * SMBufferGetNativeBuffer(SMBuffer * buf);

void SMBufferGet(SMBuffer * buf, void ** data);
void SMBufferSet(SMBuffer * buf, void * data);

/** @} */

#endif
