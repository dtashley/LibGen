//--------------------------------------------------------------------------------------------------
//C++ data-driven functions, wrapper around malloc() family of functions, with the ability to
//throw an exception if out of memory.
//--------------------------------------------------------------------------------------------------
//This file is part of LibGen, https://github.com/dtashley/LibGen, and is provided
//under The Unlicense, reproduced below.
//--------------------------------------------------------------------------------------------------
//This is free and unencumbered software released into the public domain.
//
//Anyone is free to copy, modify, publish, use, compile, sell, or
//distribute this software, either in source code form or as a compiled
//binary, for any purpose, commercial or non - commercial, and by any
//means.
//
//In jurisdictions that recognize copyright laws, the author or authors
//of this software dedicate any and all copyright interest in the
//software to the public domain. We make this dedication for the benefit
//of the public at large and to the detriment of our heirs and
//successors. We intend this dedication to be an overt act of
//relinquishment in perpetuity of all present and future rights to this
//software under copyright law.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//OTHER DEALINGS IN THE SOFTWARE.
//
//For more information, please refer to <https://unlicense.org>
//--------------------------------------------------------------------------------------------------
#define MODULE_LG_CPP_DD_MALLOC

#include <exception>

#include <malloc.h>

#include "lg_cpp_dd_malloc.hpp"

/*!
 * Allocates a block of memory of <i>in_size</i> bytes, and returns a pointer to the allocated
 * block.
 *
 * \param[in]  in_size           The number of bytes to allocate.  This is processed as
 *                               described in the malloc() documentation.
 * \returns                      Pointer to allocated block of memory.  Because this is a
 *                               wrapper for <i>malloc()</i>, the pointer is aligned to the
 *                               most stringent alignment requirements of the platform.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe.
 * \errorsandexceptions          The function will throw a <i>std::bad_alloc</i> exception
 *                               if <i>malloc()</i> is unable to allocate the memory.
 */
void *LgCppCm_MallocMalloc(size_t in_size)
{
   void* rv = nullptr;

   rv = malloc(in_size);

   if (!rv)
   {
      throw std::bad_alloc{};
   }

   return rv;
}


/*!
 * Allocates a block of memory of correctly sized to hold <i>in_num</i> elements, each
 * of size <i>in_size</i> bytes, and returns a pointer to the allocated block.
 *
 * \param[in]  in_num            The number of elements for which memory should be allocated.
 * \param[in]  in_size           The size, in bytes, of each element.
 * \returns                      Pointer to allocated block of memory, per calloc() documentation.
 *                               If the block can't be allocated, a <i>std::bad_alloc</i> exception
 *                               is thrown.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe.
 * \errorsandexceptions          The function will throw a <i>std::bad_alloc</i> exception
 *                               if <i>calloc()</i> is unable to allocate the memory.
 */
void* LgCppCm_MallocCalloc(size_t in_num, size_t in_size)
{
   void* rv = nullptr;

   rv = calloc(in_num, in_size);

   if (!rv)
   {
       //Nullptr return with a non-zero block request.  With calloc(), this
       //can only mean out of memory.  Throw exception.  We never make it to
       //the return statement.
       throw std::bad_alloc{};
   }

   return rv;
}


/*!
 * Reallocates a block of memory that was previously allocated with <i>malloc()</i>,
 * <i>calloc()</i>, or <i>realloc()</i>.
 *
 * \param[in]  in_memblock       Pointer to the previously allocated block of memory.
 * \param[in]  in_size           The size of the new memory block, in bytes, which may be
 *                               less than, the same as, or greater than the size of the
 *                               original memory block.<br><br>This is processed per the
 *                               <i>realloc()</i> documentation.
 * \returns                      Pointer to reallocated block of memory.  A <i>std::bad_alloc</i>
 *                               exception will be thrown if the memory cannot be allocated.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe.
 * \errorsandexceptions          The function will throw a <i>std::bad_alloc</i> exception if <i>realloc()</i>
 *                               is unable to allocate the memory.
 */
void* LgCppCm_MallocRealloc(void* in_memblock, size_t in_size)
{
   void* rv = nullptr;

   rv = realloc(in_memblock, in_size);

   //If the return pointer is nullptr, this can only mean out of memory.
   //Throw an exception.  Per the realloc() specification, the original
   //memory is still allocated.
   if (!rv)
   {
       throw std::bad_alloc{};
   }

   return rv;
}


/*!
 * Deallocates a block of memory allocated by another function in this module.
 *
 * \param[in]  in_memblock       Pointer to the memory block to be deallocated.  This
 *                               will be treated per the <i>free()</i> documentation.
 * \returns                      Nothing.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe.
 */
void LgCppCm_MallocFree(void *in_memblock)
{
    //Free the block.
    free(in_memblock);
}

//End of lg_cpp_dd_malloc.cpp.
