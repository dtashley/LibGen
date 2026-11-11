//--------------------------------------------------------------------------------------------------
//Buffered report.
// 
//Buffers a report in RAM for output as plain text, CSS, and HTML.
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
#define MODULE_LG_CPP_DD_BUF_REPORT

#include <assert.h>
#include <fstream>
#include <istream>
#include <iostream>
#include <iomanip>
#include <string.h>

#include "lg_cpp_dd_buf_report.hpp"

#include "lg_cpp_cm_fio.hpp"
#include "lg_cpp_dd_charf.hpp"
#include "lg_cpp_dd_malloc.hpp"
#include "lg_cpp_dd_size_t.hpp"


//The types of elements that can be added to a report.
enum class LgCppDdBufReport_RptElement
{
   None,
   PageTitleExtratabular,
   LineHorizontalExtratabular,
   SectionTitleExtratabular,
   TableBegin,
   TableEnd,
   TableTitleRow,
   TableRowBegin,
   TableRowEnd,
   TableColumnTitle,
   TableRowTitle,
   TableCell
};


typedef struct
{
   int x;
} t_LgCppDdBufRptBlock_PageTitleExtratabular;

typedef struct
{
   float z;
} t_LgCppDdBufRptBlock_LineHorizontalExtratabular;

//Union of structures that can contain every every possible
//report element.
typedef struct
{
   enum class LgCppDdBufReport_RptElement tag;
   union
   {
      t_LgCppDdBufRptBlock_PageTitleExtratabular      *p_PageTitleExtratabular;
      t_LgCppDdBufRptBlock_LineHorizontalExtratabular *p_LineHorizontalExtratabular;
   } payload;
} t_LgCppDdBufRptBlock;

/*!
 * Default constructor.
 *
 * Because only member variable assignments are performed, exceptions are not
 * possible, hence the noexcept specifier.
 */
LgBufReport::LgBufReport() noexcept : m_n_allocd{ 0 },
                                      m_n_used{ 0 }, 
                                      m_elements{nullptr}
{
}


/*!
 * Destructor.
 *
 * Memory release cannot generate an exception, so no exceptions should be possible.
 */
LgBufReport::~LgBufReport() noexcept
{
   size_t ui;
   t_LgCppDdBufRptBlock *p;

   p = (t_LgCppDdBufRptBlock *)m_elements;

   if (p != nullptr)
   {
      for (ui = 0; ui < m_n_used; ui++)
      {
         switch (p[ui].tag)
         {
         case LgCppDdBufReport_RptElement::None:
            //Nothing to do here.
            break;
         case LgCppDdBufReport_RptElement::PageTitleExtratabular:
            delete p[ui].payload.p_PageTitleExtratabular;
            p[ui].payload.p_PageTitleExtratabular = nullptr;
            break;
         case LgCppDdBufReport_RptElement::LineHorizontalExtratabular:
            delete p[ui].payload.p_LineHorizontalExtratabular;
            p[ui].payload.p_LineHorizontalExtratabular = nullptr;
            break;
         case LgCppDdBufReport_RptElement::SectionTitleExtratabular:
            break;
         case LgCppDdBufReport_RptElement::TableBegin:
            break;
         case LgCppDdBufReport_RptElement::TableEnd:
            break;
         case LgCppDdBufReport_RptElement::TableTitleRow:
            break;
         case LgCppDdBufReport_RptElement::TableRowBegin:
            break;
         case LgCppDdBufReport_RptElement::TableRowEnd:
            break;
         case LgCppDdBufReport_RptElement::TableColumnTitle:
            break;
         case LgCppDdBufReport_RptElement::TableRowTitle:
            break;
         case LgCppDdBufReport_RptElement::TableCell:
            break;
         default:
            break;
         }
      }

      LgCppCm_MallocFree(p);
   }

   m_n_allocd = 0;
   m_n_used   = 0;
   m_elements = nullptr;
}

//End of lg_cpp_dd_buf_report.cpp
