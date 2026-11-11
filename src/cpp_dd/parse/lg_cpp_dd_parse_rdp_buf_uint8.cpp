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
#define MODULE_LG_CPP_DD_PARSE_RDP_BUF_UINT8

#include <assert.h>
#include <fstream>
#include <istream>
#include <iostream>
#include <iomanip>
#include <string.h>

#include "lg_cpp_dd_parse_rdp_buf_uint8.hpp"

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


/*!
 * \brief Attempts to parse a single EOL appropriate to any platforms. If there are malformed
 *        line endings (such as CR-CR-LF), it will consume them by repeated calls so as not to
 *        stall parsing.
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
 * \param[in,out] io_linenum     The caller's current line number.  As each newline sequence is
 *                               encountered, this will be incremented.  nullptr is an acceptable
 *                               value, and indicates that the caller has no desire to have the
 *                               line number incremented.
 * \param[in,out] io_colnum      The caller's current column number. As each newline sequence is
 *                               encountered, this will be set to 1.  nullptr is an acceptable
 *                               value, and indicates that the caller has no desire to have the
 *                               line number incremented.
 *
 * \returns                      true if at least one byte could be consumed, or false otherwise (mirror
 *                               of out_found).
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
bool LgPrdpBu8ParseSingleEolAnyPfm(const LgBufUint8& in_buf, size_t in_start, size_t& out_end, bool* out_found, size_t* io_linenum, size_t* io_colnum)
{
   bool rv_found = false;
   const size_t nelem = in_buf.Nelem();
   size_t i;
   size_t local_linenum;
   size_t local_colnum;

   //Load up the local copies of line and column numbers, if the pointers are non-null.
   //We only store them back if the pointers are non-null.
   if (io_linenum)
      local_linenum = *io_linenum;
   else
      local_linenum = 0;

   if (io_colnum)
      local_colnum = *io_colnum;
   else
      local_colnum = 0;


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

      //Chew through an unlimited number of CRs (ASCII 13).  This also covers the case
      //of malformed EOL sequences.
      while ((i < nelem) && (in_buf[i] == 13))
      {
         i++;
         local_colnum++;
      }

      //Consume exactly one LF (ASCII 10) if it exists.
      if ((i < nelem) && (in_buf[i] == 10))
      {
         i++;
         local_linenum++;
         local_colnum = 1;
      }

      if (i != in_start)
      {
         //At least one byte was consumed.
         rv_found = true;
         out_end = i;
      }
   }

   //Store back the I/O values provided/accepted by pointer.
   if (io_linenum)
      *io_linenum = local_linenum;

   if (io_colnum)
      *io_colnum = local_colnum;

   if (out_found)
      *out_found = rv_found;

   return rv_found;
}


/*!
 * \brief Attempts to parse a single left brace curly brace ('{').
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
bool LgPrdpBu8ParseCurlyBraceLeft(const LgBufUint8& in_buf, size_t in_start, size_t& out_end, bool* out_found)
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

      //Consume exactly one '{' if it exists.  Guaranteed in bounds by test above.
      if (in_buf[i] == '{')
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


/*!
 * \brief Attempts to parse a single right brace curly brace ('}').
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
bool LgPrdpBu8ParseCurlyBraceRight(const LgBufUint8& in_buf, size_t in_start, size_t& out_end, bool* out_found)
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

      //Consume exactly one '}' if it exists.    Guaranteed in bounds by test above.
      if (in_buf[i] == '}')
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


/*!
 * \brief Attempts to parse a single left square bracket ('[').
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
bool LgPrdpBu8ParseSquareBracketLeft(const LgBufUint8& in_buf, size_t in_start, size_t& out_end, bool* out_found)
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

      //Consume exactly one '[' if it exists.  Guaranteed in bounds by test above.
      if (in_buf[i] == '[')
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


/*!
 * \brief Attempts to parse a single right square bracket (']').
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
bool LgPrdpBu8ParseSquareBracketRight(const LgBufUint8& in_buf, size_t in_start, size_t& out_end, bool* out_found)
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

      //Consume exactly one '{' if it exists.  Guaranteed in bounds by test above.
      if (in_buf[i] == ']')
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


/*!
 * \brief Attempts to parse a single colon (':').
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
bool LgPrdpBu8ParseColon(const LgBufUint8& in_buf, size_t in_start, size_t& out_end, bool* out_found)
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

      //Consume exactly one ':' if it exists.  Guaranteed in bounds by test above.
      if (in_buf[i] == ':')
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


/*!
 * \brief Attempts to parse a C++ comment.  This would start with "//" and extend until the end of
 *        the line, but not include EOL characters.
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
bool LgPrdpBu8ParseCppComment(const LgBufUint8& in_buf, size_t in_start, size_t& out_end, bool* out_found)
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

      //Try to absorb a single slash, if it is there.  Valid access guaranteed by test above.
      if (in_buf[i] == (uint8_t)'/')
      {
         i++;

         //Try to absorb the second slash, if it is there.
         if ((i < nelem) && (in_buf[i] == (uint8_t)'/'))
         {
            i++;

            //The second slash was there.  Advance until end of line or end of buffer.  It is
            //possible that there are no bytes to consume.
            while ((i < nelem) && ((in_buf[i] != 13) && (in_buf[i] != 10)))
            {
               i++;
            }
         }
         else
         {
            //The second slash wasn't there.  Have to reset i for a failure return.
            i = in_start;
         }
      }
      else
      {
         //The first slash was not there, so will fail.
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


/*!
 * \brief Attempts to parse a single standard identifier.  This would be one or more letters
 *        followed by zero or more letters, digits, and underscores.
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
bool LgPrdpBu8ParseStdIdentifier(const LgBufUint8& in_buf, size_t in_start, size_t& out_end, bool* out_found)
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
      //Parse any number of letters.

      //Possible to parse.
      i = in_start;
      out_end = in_start;

      //Consume an unlimited number of letters of either case.
      while ((i < nelem) && (LgCppDd_CharfIsLetter(in_buf[i])))
      {
         i++;
      }

      if (i != in_start)
      {
         //At least one letter consumed.  Can try to consume any mix of letters, digits, and underscores.
         while ((i < nelem) && (LgCppDd_CharfIsLetterDigitOrUnderscore(in_buf[i])))
         {
            i++;
         }
      }
      else
      {
         //Didn't start with a letter.  Not a standard identifier.
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
