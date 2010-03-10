#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#include "SMSimulator.h"

/**
 * Create an SMBuffer backed by an OpenCL memory object. Depending on whether
 * the context it's created in is on the CPU or GPU, reads and writes to this
 * buffer may involve transferring data between main and video memory, which is
 * much slower than working solely within either.
 *
 * @param sim The context in which to create the new buffer.
 * @param elementCount The number of elements in the buffer.
 * @param elementSize The size of an element in the buffer.
 * @return The newly allocated buffer.
 */
SMBuffer * SMBufferNew(SMContext * sim, long elementCount, size_t elementSize)
{
    SMBuffer * buf = (SMBuffer *)calloc(1, sizeof(SMBuffer));

    buf->context = sim;
    buf->type = SM_OPENCL_BUFFER;

    buf->elementCount = elementCount;
    buf->elementSize = elementSize;

    buf->gpuBuffer = clCreateBuffer(sim->ctx, CL_MEM_READ_WRITE,
                                    SMBufferGetSize(buf), NULL, NULL);

    return buf;
}

/**
 * Create an SMBuffer backed by an mmapped file. If the file exists, it is
 * overwritten; if it doesn't, it is created.
 *
 * @param sim The context in which to create the new buffer.
 * @param elementCount The number of elements in the buffer.
 * @param elementSize The size of an element in the buffer.
 * @param filename The filename to map into the buffer's memory.
 * @return The newly allocated buffer.
 */
SMBuffer * SMBufferNewWithFile(SMContext * sim, long elementCount,
                               size_t elementSize, const char * filename)
{
    SMBuffer * buf = (SMBuffer *)calloc(1, sizeof(SMBuffer));

    buf->context = sim;
    buf->type = SM_FILE_BUFFER;

    buf->elementCount = elementCount;
    buf->elementSize = elementSize;

    buf->fileBuffer = clCreateBuffer(sim->ctx, CL_MEM_READ_WRITE,
                                     SMBufferGetSize(buf), NULL, NULL);

    buf->file = open(filename, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);

    if(buf->file == -1)
    {
        throwError("couldn't create buffer file '%s'", filename);
        return NULL;
    }

    lseek(buf->file, SMBufferGetSize(buf), SEEK_SET);
    write(buf->file, "", 1);

    buf->fileBuffer = (void *)mmap(NULL, SMBufferGetSize(buf),
                                   PROT_READ | PROT_WRITE, MAP_SHARED,
                                   buf->file, 0);

    return buf;
}

/**
 * Free the memory used by the given SMBuffer. This will also unmap and close
 * the associated file if it's a file-backed buffer.
 *
 * @param buf The buffer to free.
 */
void SMBufferFree(SMBuffer * buf)
{
    switch(buf->type)
    {
        case SM_OPENCL_BUFFER:
            // Is there no way to free cl_mem objects?
            break;
        case SM_FILE_BUFFER:
            munmap(buf->fileBuffer, SMBufferGetSize(buf));
            close(buf->file);
            break;
        default:
            throwError("tried to free unknown buffer type");
    }

    free(buf);
}

/**
 * @param buf The SMBuffer to inspect.
 * @return The number of elements in the buffer.
 */
long SMBufferGetElementCount(SMBuffer * buf)
{
    return buf->elementCount;
}

/**
 * @param buf The SMBuffer to inspect.
 * @return The size of each element in the buffer.
 */
size_t SMBufferGetElementSize(SMBuffer * buf)
{
    return buf->elementSize;
}

/**
 * @param buf The SMBuffer to inspect.
 * @return The total size of the buffer.
 */
size_t SMBufferGetSize(SMBuffer * buf)
{
    return SMBufferGetElementSize(buf) * SMBufferGetElementCount(buf);
}

/**
 * @param buf The SMBuffer to inspect.
 * @return The cl_mem object which backs OpenCL buffers.
 */
cl_mem SMBufferGetCLBuffer(SMBuffer * buf)
{
    if(buf->type != SM_OPENCL_BUFFER)
        throwError("tried to get cl_mem from non-OpenCL buffer");

    return buf->gpuBuffer;
}

/**
 * @param buf The SMBuffer to inspect.
 * @return A pointer to the native buffer in memory, for file-backed buffers.
 */
void * SMBufferGetNativeBuffer(SMBuffer * buf)
{
    if(buf->type != SM_FILE_BUFFER)
        throwError("tried to get native buffer from non-file buffer");

    return buf->fileBuffer;
}

/**
 * Copy the contents of the given SMBuffer into the given native buffer. If
 * space hasn't been allocated for the native buffer, allocate just enough
 * space to fit the SMBuffer.
 *
 * Keep in mind that if the SMBuffer is OpenCL-backed, this is an expensive
 * operation, as it involves copying a chunk of data from video to main memory.
 *
 * @param buf The source SMBuffer.
 * @param data A pointer to the destination native buffer.
 */
void SMBufferGet(SMBuffer * buf, void ** data)
{
    /// \todo I'm not totally sure this makes sense.
    if(*data == NULL)
    {
        (*data) = (void *)calloc(1, SMBufferGetSize(buf));
    }

    switch(buf->type)
    {
        case SM_OPENCL_BUFFER:
            clEnqueueReadBuffer(buf->context->cmds, buf->gpuBuffer, CL_TRUE,
                                0, SMBufferGetSize(buf), *data, 0, NULL, NULL);
            break;
        case SM_FILE_BUFFER:
            break;
        default:
            throwError("tried to copy from unknown buffer type");
    }
}

/**
 * Copy the contents of the given native buffer into the given SMBuffer.
 *
 * Keep in mind that if the SMBuffer is OpenCL-backed, this is an expensive
 * operation, as it involves copying a chunk of data from main to video memory.
 *
 * @param buf The destination SMBuffer.
 * @param data The source native buffer.
 */
void SMBufferSet(SMBuffer * buf, void * data)
{
    switch(buf->type)
    {
        case SM_OPENCL_BUFFER:
            clEnqueueWriteBuffer(buf->context->cmds, buf->gpuBuffer, CL_TRUE,
                                 0, SMBufferGetSize(buf), data, 0, NULL, NULL);
            break;
        case SM_FILE_BUFFER:
            break;
        default:
            throwError("tried to copy to unknown buffer type");
    }
}
