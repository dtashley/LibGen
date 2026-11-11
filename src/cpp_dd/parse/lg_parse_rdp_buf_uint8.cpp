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
 * \brief Attempts to parse conventional whitespace (space and tab characters only),
 *        starting at a given point in a LgBufUint8 buffer.
 *
 * \param[in]     in_buf         The buffer to parse.
 * \param[in]     in_start       The index of the first byte to examine.  If this byte is outside
 *                               the buffer, no invalid dreferencing will occur, out_end will be
 *                               set to one byte beyond the end of the buffer, and nothing can
 *                               be parsed.
 * \param[out]    out_end        One past the last byte consumed.  If the desired item could not
 *                               be consumed, out_end will be set to the value of in_buf.  If the
 *                               buffer is fully consumed, out_end will be set to one beyond beyond
 *                               the end of the buffer.
 * \param[out]    out_found      true if at least one byte could be consumed, or false otherwise.
 *                               nullptr is a valid value, and is a signal that the caller has no
 *                               interest in receiving the value through the parameter list (it is
 *                               still provided as a return value from the function).
 *
 * \returns                      true if at least one byte could be consumed, or false otherwise (mirror
 *                               of out_found).
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
bool LgPrdpBu8ParseSpcTbNoEols(const LgBufUint8& in_buf, size_t in_start, size_t& out_end, bool* out_found)
{
   bool rv_found = false;
   size_t nelem = in_buf.Nelem();
   size_t i;

   if (in_start >= nelem)
   {
      //The start index is one past the buffer (buffer exhausted), or even larger (invalid
      //start value).  In either case, the return value is false and out_end will be set
      //to one past the end of the buffer.
      out_end = nelem;
   }
   else
   {
      //Possible to parse.
      i = in_start;
      out_end = in_start;

      while ((i < nelem) && ((in_buf[i] == (uint8_t)' ') || (in_buf[i] == (uint8_t)'\t')))
      {
         i++;
      }

      if (i != in_start)
      {
         //At least one byte was consumed.
         rv_found = true;
         out_end = i;
      }
   }

   if (out_found)
      *out_found = rv_found;

   return rv_found;
}

//End of lg_parse_rdp_buf_uint8.cpp
