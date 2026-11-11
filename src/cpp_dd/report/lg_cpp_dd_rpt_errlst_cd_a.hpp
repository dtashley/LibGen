//--------------------------------------------------------------------------------------------------
//Header for class to contain and maniulate a list of warnings and errors that are characteristic
//of source code analysis, variant A.
//--------------------------------------------------------------------------------------------------
//This file is part of LibGen, https://github.com/dtashley/LibGen, and is provided
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
#ifndef LG_CPP_DD_RPT_ERRLST_CD_A_INCLUDED
#define LG_CPP_DD_RPT_ERRLST_CD_A_INCLUDED

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "lg_cpp_dd_buf_uint8.hpp"


//! The severity of a warning or error.
enum class LgCppDdErrlistCdASeverity
{
   Undefined,
      //!< Unused or undefined.  This is so that zero initialization results in
      //!< an invalid value.
   Informational,
      //!< Informational only.  No merit for advice, warning, or error.
   Advisory,
      //!< Suspicious to some degree.
   Warning,
      //!< Confirmed condition that could have consequences for the logical operation
      //!< of the program.
   Medium,
      //!< Confirmed condition that is more likely to have consequences for the
      //!< logical operation of the program.
   High,
      //!< Confirmed condition that is likely to have large consequences for the
      //!< logical operation of the program.
   Severe,
      //!< Confirmed condition that is likely to have severe consequences for the
      //!< logical operation of the program.
   EnumMax,
      //!< Not a valid severity value.  Used to test for exceeding the maximum value
      //!< of the enum.
};


//!< The fundamental record representing a single warning or error.
struct LgCppDdErrlistCdARec
{
   std::string filepath;
      //!< The path leading up to the filename, including the final path separator.  This may
      //!< be left as the empty string, in which case the path won't be used.  In many
      //!< applications, the software component compiling the list may not know the file path
      //!< or file name.
   std::string filename;
      //!< The filename, without the path. This may be left as the empty string, in which case
      //!< the path won't be used.  In many applications, the software component compiling the
      //!< list may not know the file path or file name.
   size_t warn_err_charnum{};
      //!< The character number of the location of the warning or error within the file, with
      //!< the first character being numbered 1.  0 is reserved as a sentinel value for not known
      //!< or not recorded.
   size_t warn_err_linenum{};
      //!< The character number of the location of the warning or error within the file, with
      //!< the first character being numbered 1.  0 is reserved as a sentinel value for not known
      //!< or not recorded.
   size_t warn_err_columnnum{};
      //!< The column number within the line of the warning or error, with the first column
      //!< being numbered 1.  0 is reserved as a sentinel value for not known
      //!< or not recorded.
   enum class LgCppDdErrlistCdASeverity warn_err_severity{};
      //!< Severity of the warning or error.
   std::string warn_err_id;
      //!< A short string identifying the warning or error, perhaps something like "CS-NO-TAB".
      //!< Semantics and allowable values defined by clients.
   std::string warn_err_desc;
      //!< A longer description of the warning or error, with description specific to the error,
      //!< but not specific to the instance of the error.
   std::string warn_err_instance_desc;
      //!< Description specific to the instance of the error.
   char buffer_context_excerpt[50]{};
      //!< An excerpt from the file, possibly used to print context.
   size_t buffer_context_used{};
      //!< The number of characters, starting at [0], that are occupied in the context buffer.
   size_t buffer_context_caret_pt{};
      //!< The index of the character in the context buffer, starting at [0], that most closely
      //!< corresponds to the warning or error.
   bool content_warning{};
      //!< True if a warning was issued when this record was added, due to its content.
   bool marked{};
      //!< Used for various forms of filtering and client analysis.
};
typedef struct LgCppDdErrlistCdARec tLgCppDdErrlistCdARec;

//!< \class LgCppDdErrlistCdA
//!< A list of warnings and errors characteristic of what might be noted in a software source file,
//!< variant A.
class LgCppDdErrlistCdA
{
#ifndef UT_GTEST
private:
#else
public:
#endif
   std::vector<tLgCppDdErrlistCdARec> m_errlist;
      //!< The array of records, one per warning or error.
public:
   LgCppDdErrlistCdA() noexcept;
   ~LgCppDdErrlistCdA() noexcept;
   size_t Nelem(void) noexcept;
   bool AppendFull(
                   const LgBufUint8 &in_buf,
                   const std::string &in_filepath,
                   const std::string &in_filename,
                   const size_t in_warn_err_charnum,
                   const size_t in_warn_err_linenum,
                   const size_t in_warn_err_columnnum,
                   const enum class LgCppDdErrlistCdASeverity in_warn_err_severity,
                   const std::string &in_warn_err_id,
                   const std::string &in_warn_err_desc,
                   const std::string &in_warn_err_instance_desc
                  );
   size_t NMarked(void);
   void Mark(size_t in_elemidx);
   void MarkAll(void);
   void UnMark(size_t in_elemidx);
   void UnMarkAll(void);
   bool IsMarked(size_t in_elemidx);
   void GetRec(size_t in_elemidx, bool &out_success, tLgCppDdErrlistCdARec &out_rec);
   void EmitListToStreamText(std::ostream& in_output_stream, const std::string in_title = "", int in_require_marked_or_unmarked = 0);
};

extern void LgCppDdErrlistCdA_WeTypeToString(const LgCppDdErrlistCdASeverity in_type, std::string &out_str);

#endif
