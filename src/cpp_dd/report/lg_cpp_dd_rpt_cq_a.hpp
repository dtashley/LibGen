//--------------------------------------------------------------------------------------------------
//Header file for memory-buffered reports.
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
#ifndef LG_CPP_DD_BUF_REPORT_HPP_INCLUDED
#define LG_CPP_DD_BUF_REPORT_HPP_INCLUDED

#include <string>

//The types of reports that can be produced.
enum class LgCppDdBufReport_RptType
{
   None,
   Plaintext,
   Css,
   Html
};


//The colors that can be used for a cell.
enum class LgCppDdBufReport_TblCellColor
{
   None,
   Default,
   White,
   Green,
   Yellow,
   Blue,
   Red,
   Orange
};


//Buffer containing a report.
//
class LgBufReport
{
private:
   size_t m_n_allocd;
      //The number of slots allocated for elements.
   size_t m_n_used;
      //The number of elements actually used.
   void* m_elements;
      //Pointer to the array of blocks.  If m_n_allocd is 0, this must be nullptr.
      //If m_n_allocd is != 0, this must not be nullptr.

public:
   //Fundamental constructors/destructors
   LgBufReport() noexcept;
   ~LgBufReport() noexcept;
};

#endif

//End of lg_cpp_dd_buf_report.hpp.
