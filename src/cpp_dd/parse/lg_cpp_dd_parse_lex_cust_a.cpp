//--------------------------------------------------------------------------------------------------
//Custom lexical analyzer A
// 
//Lexically analyzes (breaks into tokens) the content of a buffer of bytes, according to
//a custom scheme denoted "A".  This scheme is meant for configuration files where the tokens
//involve braces, brackets, commas, colons, quoted strings with escapes, 32-bit signed integers,
//and identifiers.
//--------------------------------------------------------------------------------------------------
//This file is part of LibGen, https://github.com/dtashley/DtaPublic, and is provided
//under The Unlicense, reproduced below.
//--------------------------------------------------------------------------------------------------
//This is free and unencumbered software released into the public domain.
//
//Anyone is free to copy, modify, publish, use, compile, sell, or
//distribute this software, either in source code form or as a compiled
//binary, for any purpose, commercial or non-commercial, and by any
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
#define MODULE_LG_CPP_DD_PARSE_LEX_CUST_A

#include <iostream>
#include <sstream>
#include <string>

#include "lg_cpp_dd_parse_lex_cust_a.hpp"

#include "lg_cpp_dd_charf.hpp"
#include "lg_cpp_dd_parse_rdp_buf_uint8.hpp"
#include "lg_cpp_dd_rpt_errlst_cd_a.hpp"


/*!
 * \brief                       Default constructor.  Constructs a new object.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
LgCppDdParseLexCustA::LgCppDdParseLexCustA() noexcept
{
   //No additional initialization needed.  The vector member
   //is automatically constructed.
}


/*!
 * \brief                       Destructor.  Destroys an object.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
LgCppDdParseLexCustA::~LgCppDdParseLexCustA() noexcept
{
   //No additional dextruction needed.  The vector member
   //is automatically destroyed.
}


/*!
 * \brief Attempts to parse in_buf into a list of tokens appropriate for custom lexical
 *        analyzer A.
 *
 *        An allocation failure is possible, in which case an out of memory exception
 *        will be generated.
 *
 * \param[in]     in_buf                       The buffer containing to attempt to tokenize.  This is usually
 *                                             the content of a file.
 * \param[out]    out_success                  Mirror of the return value of the function.  The pointer may legall be
 *                                             nullptr, in which case it will not be dereferenced.
 * \param[in,out] out_warnings_and_errors      The list of warnings and errors provided by the caller will be appended to
 *                                             with any warnings and errors detected during the tokenization.
 *                                             The most normal case is for the caller to provide an empty list.
 *
 * \returns                                    true if no warnings or errors occurred during the tokenization, or false
 *                                             otherwise.  This will be identical to the value provided through *out_success
 *                                             if a non-nullptr value is provided for the pointer.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
bool LgCppDdParseLexCustA::Parse(const LgBufUint8& in_buf, bool* out_success, LgCppDdErrlistCdA& out_warnings_and_errors)
{
   bool exit_flag = false;
   const size_t n = in_buf.Nelem();
   size_t i;
   size_t tentative_token_end = 0;
   size_t line_no;
   size_t column_no;
   bool rv = true;
   const bool local_debug = true;
   std::ostringstream oss;

   //std::cout << "in Parse" << std::endl;

   //Loop through, looking for characters that don't belong.
   line_no = 1;
   column_no = 1;
   for (i = 0; i < n; i++)
   {
      if (in_buf[i] == 10)
      {
         line_no++;
         column_no = 1;
      }
      else if (in_buf[i] == 13)
      {
      }
      else if (LgCppDd_CharfAsciiCharIsLargelyPrintable(in_buf[i]))
      {
         column_no++;
      }
      else
      {
         oss << "Invalid byte, value: " << in_buf[i];
         out_warnings_and_errors.AppendFull(
                                            in_buf,
                                            "",
                                            "",
                                            i + 1,
                                            line_no,
                                            column_no,
                                            LgCppDdErrlistCdASeverity::High,
                                            "",
                                            "Invalid byte",
                                            oss.str()
                                           );
         rv = false;
      }
   }

   //If no invalid characters found, proceed with parse.
   if (rv)
   {
      i = 0;
      line_no = 1;
      column_no = 1;

      while ((i < n) && (!exit_flag))
      {
         if (LgPrdpBu8ParseSingleEolAnyPfm(in_buf, i, tentative_token_end, nullptr, &line_no, &column_no))
         {
            //End of line found.  Advance line and column number appropriately.
            if (local_debug)
            {
               std::cout << "LgCppDdParseLexCustA::Parse(): EOL, start: " << i << " end: " << tentative_token_end << std::endl;
            }
            i = tentative_token_end;
         }
         else if (LgPrdpBu8ParseSpcTbNoEols(in_buf, i, tentative_token_end))
         {
            //Whitespace was found.  Whitespace is not a token, so just remove.
            //it.
            if (local_debug)
            {
               std::cout << "LgCppDdParseLexCustA::Parse(): whitespace, start: " << i << " end: " << tentative_token_end << std::endl;
            }

            column_no += (tentative_token_end - i);
            i = tentative_token_end;
         }
         else if (LgPrdpBu8ParseCppComment(in_buf, i, tentative_token_end))
         {
            //C++-style comment was found.  This is not a token, so just remove
            if (local_debug)
            {
               std::cout << "LgCppDdParseLexCustA::Parse(): C++-style comment, start: " << i << " end: " << tentative_token_end << std::endl;
            }

            column_no += (tentative_token_end - i);
            i = tentative_token_end;
         }
         else if (LgPrdpBu8ParseColon(in_buf, i, tentative_token_end))
         {
            //Colon was found.  This is a token.
            if (local_debug)
            {
               std::cout << "LgCppDdParseLexCustA::Parse(): ':', start: " << i << " end: " << tentative_token_end << std::endl;
            }

            {
               tLgCppDdParseLexCustAStruct element_to_add;  //Will be constructed as it comes into scope.

               //Form up the record to be added.
               element_to_add.tokentype        = LgCppDdParseCustLexATokenType::Colon;
               element_to_add.tokentext.append((char*)in_buf.GetBufPtr() + i, tentative_token_end - i);
               element_to_add.i32              = 0;
               element_to_add.charnum          = i + 1;
               element_to_add.linenum          = line_no;
               element_to_add.columnnum        = column_no;

               //Add the record.
               m_tokenlist.push_back(element_to_add);
            }

            column_no += (tentative_token_end - i);
            i = tentative_token_end;
         }
         else if (LgPrdpBu8ParseCurlyBraceLeft(in_buf, i, tentative_token_end))
         {
            //Left curly brace was found.  This is a token.
            if (local_debug)
            {
               std::cout << "LgCppDdParseLexCustA::Parse(): '{', start: " << i << " end: " << tentative_token_end << std::endl;
            }

            {
               tLgCppDdParseLexCustAStruct element_to_add;  //Will be constructed as it comes into scope.

               //Form up the record to be added.
               element_to_add.tokentype        = LgCppDdParseCustLexATokenType::BraceLeft;
               element_to_add.tokentext.append((char*)in_buf.GetBufPtr() + i, tentative_token_end - i);
               element_to_add.i32              = 0;
               element_to_add.charnum          = i + 1;
               element_to_add.linenum          = line_no;
               element_to_add.columnnum        = column_no;

               //Add the record.
               m_tokenlist.push_back(element_to_add);
            }

            column_no += (tentative_token_end - i);
            i = tentative_token_end;
         }
         else if (LgPrdpBu8ParseCurlyBraceRight(in_buf, i, tentative_token_end))
         {
            //Left curly brace was found.  This is a token.
            if (local_debug)
            {
               std::cout << "LgCppDdParseLexCustA::Parse(): '}', start: " << i << " end: " << tentative_token_end << std::endl;
            }

            {
               tLgCppDdParseLexCustAStruct element_to_add;  //Will be constructed as it comes into scope.

               //Form up the record to be added.
               element_to_add.tokentype = LgCppDdParseCustLexATokenType::BraceRight;
               element_to_add.tokentext.append((char*)in_buf.GetBufPtr() + i, tentative_token_end - i);
               element_to_add.i32 = 0;
               element_to_add.charnum = i + 1;
               element_to_add.linenum = line_no;
               element_to_add.columnnum = column_no;

               //Add the record.
               m_tokenlist.push_back(element_to_add);
            }

            column_no += (tentative_token_end - i);
            i = tentative_token_end;
         }
         else if (LgPrdpBu8ParseSquareBracketLeft(in_buf, i, tentative_token_end))
         {
            //Left square bracket was found.  This is a token.
            if (local_debug)
            {
               std::cout << "LgCppDdParseLexCustA::Parse(): '[', start: " << i << " end: " << tentative_token_end << std::endl;
            }

            {
               tLgCppDdParseLexCustAStruct element_to_add;  //Will be constructed as it comes into scope.

               //Form up the record to be added.
               element_to_add.tokentype = LgCppDdParseCustLexATokenType::BracketLeft;
               element_to_add.tokentext.append((char*)in_buf.GetBufPtr() + i, tentative_token_end - i);
               element_to_add.i32 = 0;
               element_to_add.charnum = i + 1;
               element_to_add.linenum = line_no;
               element_to_add.columnnum = column_no;

               //Add the record.
               m_tokenlist.push_back(element_to_add);
            }

            column_no += (tentative_token_end - i);
            i = tentative_token_end;
         }
         else if (LgPrdpBu8ParseSquareBracketRight(in_buf, i, tentative_token_end))
         {
            //Left square bracket was found.  This is a token.
            if (local_debug)
            {
               std::cout << "LgCppDdParseLexCustA::Parse(): ']', start: " << i << " end: " << tentative_token_end << std::endl;
            }

            {
               tLgCppDdParseLexCustAStruct element_to_add;  //Will be constructed as it comes into scope.

               //Form up the record to be added.
               element_to_add.tokentype = LgCppDdParseCustLexATokenType::BracketRight;
               element_to_add.tokentext.append((char*)in_buf.GetBufPtr() + i, tentative_token_end - i);
               element_to_add.i32 = 0;
               element_to_add.charnum = i + 1;
               element_to_add.linenum = line_no;
               element_to_add.columnnum = column_no;

               //Add the record.
               m_tokenlist.push_back(element_to_add);
            }

            column_no += (tentative_token_end - i);
            i = tentative_token_end;
         }
         else if (LgPrdpBu8ParseStdIdentifier(in_buf, i, tentative_token_end))
         {
            //Identifier was found.  This is a token.
            if (local_debug)
            {
               std::cout << "LgCppDdParseLexCustA::Parse(): Identifier, start: " << i << " end: " << tentative_token_end << std::endl;
            }

            {
               tLgCppDdParseLexCustAStruct element_to_add;  //Will be constructed as it comes into scope.

               //Form up the record to be added.
               element_to_add.tokentype = LgCppDdParseCustLexATokenType::Identifier;
               element_to_add.tokentext.append((char*)in_buf.GetBufPtr() + i, tentative_token_end - i);
               element_to_add.i32 = 0;
               element_to_add.charnum = i + 1;
               element_to_add.linenum = line_no;
               element_to_add.columnnum = column_no;

               //Add the record.
               m_tokenlist.push_back(element_to_add);
            }

            column_no += (tentative_token_end - i);
            i = tentative_token_end;
         }
         else
         {
            //Incomplete parse.  Failure.
            if (local_debug)
            {
               std::cout << "LgCppDdParseLexCustA::Parse(): Unable to parse, start: " << i << std::endl;

               out_warnings_and_errors.AppendFull(in_buf, "", "", i + 1, line_no, column_no, LgCppDdErrlistCdASeverity::High, "", "", "");
               rv = false;
            }

            exit_flag = true;
         }
      }
   }

   if (out_success)
      *out_success = rv;
   return rv;
}
