//--------------------------------------------------------------------------------------------------
//Recursive Descent Parser Functions Operating on a Uint8 Buffer
// 
//Functions to support recursive descent parsing operating on a Uint8
//buffer.
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
//software to the public domain.We make this dedication for the benefit
//of the public at large and to the detriment of our heirs and
//successors.We intend this dedication to be an overt act of
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
#define MODULE_LG_PARSE_RDP_BUF_UINT8

#include <assert.h>
#include <fstream>
#include <istream>
#include <iostream>
#include <iomanip>
#include <string.h>

#include "lg_parse_rdp_buf_uint8.hpp"

#include "lg_cpp_cm_fio.hpp"
#include "lg_cpp_dd_charf.hpp"
#include "lg_cpp_dd_malloc_alt.hpp"
#include "lg_cpp_dd_size_t.hpp"


/*!
 * \brief Adjusts the memory allocation associated with a buffer upwards to be
 *        at least the specified number of bytes.  Requests that would result
 *        in a downward allocation will be ignored.
 *
 *        An allocation failure is possible, in which case an out of memory exception
 *        will be generated.
 *
 * \param[in]     nbytes         The size to allocate upwards to.  Values that would result
 *                               in a downward allocation will be ignored, and no action
 *                               will be taken.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
void AllocateAtLeast(size_t nbytes)
{
   //size_t new_allocation;
}

//End of lg_parse_rdp_buf_uint8.cpp
