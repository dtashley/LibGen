//--------------------------------------------------------------------------------------------------
//Header file for custom lexical analyzer A.
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
#ifndef LG_CPP_DD_PARSE_LEX_CUST_A_HPP_INCLUDED
#define LG_CPP_DD_PARSE_LEX_CUST_A_HPP_INCLUDED

#include <cstdint>
#include <string>
#include <vector>

#include "lg_cpp_dd_buf_uint8.hpp"
#include "lg_cpp_dd_rpt_errlst_cd_a.hpp"

enum class LgCppDdParseCustLexATokenType
{
   Undefined,
      //!< Unused or undefined.  This is primarily to assure that for binary 0
      //!< initialization, the value is not a valid token.
   Comma,
      //!< Comma as a token.
   Colon,
      //!< Colon as a token.
   BraceLeft,
      //!< Left brace ('{') as a token.
   BraceRight,
      //!< Right brace ('}') as a token.
   BracketLeft,
      //!< Left bracket ('[') as a token.
   BracketRight,
      //!< Right bracket (']') as a token.
   Identifier,
      //!< Identifier as a token.  An identifier is a lower- or upper-case letter
      //!< followed by any number of lower- or upper-case letters, digits, and
      //!< underscores.
   DoubleQuotedString,
      //!< A double-quoted string.  This may not span lines.  The allowable escapes
      //!< in the string are '\\' for a backslash, '\"' for an embedded double quote,
      //!< '\n' for a Unix line break, and '\nxx', where 'xx' represents two hexadecimal
      //!< digits with the letters either lower- or upper-case, for an embedded binary
      //!< byte of the specified hexadecimal value.
   IntSigned32,
      //!< An integer that falls into the range that can be represented by a
      //!< a 32-bit signed integer, -2^31 to 2^31-1, inclusive.  Commas are not
      //!< allowed. Positive integers may not have a '+'.  Negative integers
      //!< have a single leading '-'.
   IntegerBlob,
      //!< A string that is consistent with an integer (see general rules in enum
      //!< constant above), but outside the range that can be represented by a 32-bit
      //!< signed integer.
   Blob,
      //!< A general collection of letters (both lower- and upper-case), digits,
      //!< and underscores that can't be parsed as an identifier, an unsigned 32-bit
      //!< integer, or an integer blob.  Any errors that result from the presence of
      //!< this sort of blob are the responsiblity of the client to detect.
   EnumLimit
      //!< Not a valid value. One past the largest enum value, used for out-of-bounds
      //!< testing.
};

struct LgCppDdParseLexCustAStruct
{
   enum class LgCppDdParseCustLexATokenType tokentype{};
      //!< The type of the token.
   std::string tokentext;
      //!< The text of the token.
   int32_t i32{};
      //!< If the token type is 32-bit integer, the integer.  Otherwise,
      //!< it will have the value of 0.
   size_t charnum{};
      //!< The character number within the buffer where the token starts.
      //!< First index is 1.
   size_t linenum{};
      //!< The line number on which the token starts.  The first line number
      //!< is 1.
   size_t columnnum{};
      //!< The column number within the line on which the token starts.  The
      //!< first column number is 1.
};
typedef struct LgCppDdParseLexCustAStruct tLgCppDdParseLexCustAStruct;


//! \class LgCppDdParseLexCustA
//! Custom lexographical analyzer A.
class LgCppDdParseLexCustA
{
#ifndef UT_GTEST
private:
#else
public:
#endif
    std::vector<tLgCppDdParseLexCustAStruct> m_tokenlist;
       //!< The array of records, one per leaf node.
public:
   LgCppDdParseLexCustA() noexcept;
   ~LgCppDdParseLexCustA() noexcept;
   bool Parse(const LgBufUint8& in_buf, bool* out_success, LgCppDdErrlistCdA& out_warnings_and_errors);
};
#endif
