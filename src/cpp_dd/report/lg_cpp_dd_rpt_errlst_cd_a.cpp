//--------------------------------------------------------------------------------------------------
//Class to contain and maniulate a list of errors that are characteristic of source code analysis,
//variant A.
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
#define MODULE_LG_CPP_DD_RPT_ERRLST_CD_A

#include "lg_cpp_dd_rpt_errlst_cd_a.hpp"

#include "lg_cpp_cm_fio.hpp"


/*!
 * \brief                       Default constructor.  Constructs a new object.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
LgCppDdErrlistCdA::LgCppDdErrlistCdA() noexcept
{
   //The vector member will be constructed automatically.
}


/*!
 * \brief                       Destructor.  Destroys an object.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
LgCppDdErrlistCdA::~LgCppDdErrlistCdA() noexcept
{
   //The vector member will be destroyed automatically.
}


/*!
 * \brief Returns the number of elements in the list of warnings/errors that are part of the
 *        object.
 *
 * \returns       Number of elements, with zero a possible return value.
 */
size_t LgCppDdErrlistCdA::Nelem(void) noexcept
{
   size_t rv;

   rv = m_errlist.size();

   return rv;
}


/*!
 * \brief Appends a record of warning or error to the the internal set of records.
 *
 *        An allocation failure is possible, in which case an out of memory exception
 *        will be generated.
 *
 * \param[in]     in_buf                       The buffer containing the reference that the warning or error
 *                                             is logged against.  This is normally an entire file.  The
 *                                             buffer itself is necessary to gather the excerpt.
 * \param[in]     in_filepath                  The path to the file, including the final path separator.
 *                                             The empty string is a sentinel value to indicate this information
 *                                             is not available or not provided.
 * \param[in]     in_filename                  The filename, not including the path.
 *                                             The empty string is a sentinel value to indicate this information
 *                                             is not available or not provided.
 * \param[in]     in_warn_err_charnum          The character number within the buffer the warning or error was
 *                                             detected.  Numbering starts with 1, and 0 is a sentinel value
 *                                             to indicate that the information was not provided.
 * \param[in]     in_warn_err_linenum          The line number within the buffer the warning or error was
 *                                             detected.  Numbering starts with 1, and 0 is a sentinel value
 *                                             to indicate that the information was not provided.
 * \param[in]     in_warn_err_columnnum        The column number within the line where the warning or error was
 *                                             detected.  Numbering starts with 1, and 0 is a sentinel value
 *                                             to indicate that the information was not provided.
 * \param[in]     in_warn_err_severity         The severity of the warning or error.  Must be a memmber of
 *                                             the enumeration provided in the .hpp file.
 * \param[in]     in_warn_err_id               A character string containing the identifier of the warning or
 *                                             error.  The semantics are up to the caller.  The empty string
 *                                             is a sentinel value to indicate that the information is not provided.
 *
 * \returns                                    true if the information could be added without a warning, or false if
 *                                             a warning occurred.  The warning is also included with the record.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
bool LgCppDdErrlistCdA::AppendFull(
                                   const LgBufUint8& in_buf,
                                   const std::string& in_filepath,
                                   const std::string& in_filename,
                                   const size_t in_warn_err_charnum,
                                   const size_t in_warn_err_linenum,
                                   const size_t in_warn_err_columnnum,
                                   const enum class LgCppDdErrlistCdASeverity in_warn_err_severity,
                                   const std::string& in_warn_err_id,
                                   const std::string& in_warn_err_desc,
                                   const std::string& in_warn_err_instance_desc
                                  )
{
   tLgCppDdErrlistCdARec rec_to_add;
      //Record that will be added.
   bool rv = true;
      //Return value.  true == no problems.
   bool warn = false;
      //True if there should be a warning.

   //Decide if there should be warnings 
   if (
         (in_warn_err_severity <= LgCppDdErrlistCdASeverity::Undefined)
            //Enum can't be the undefined value or less.  Enums are signed by default, so have to assume
            //the variable can hold values less than 0.
         ||
         (in_warn_err_severity >= LgCppDdErrlistCdASeverity::EnumMax)
            //Enum can't be outside the allowable range.
         ||
         (
            (in_warn_err_charnum != 0)
               //Not the sentinel value, and safe to subtract 1 from.
            &&
            ((in_warn_err_charnum - 1) > in_buf.Nelem())
               //Index too large, outside the buffer.
               //If, for example, a buffer has 0 characters, it would be possible to detect a warning
               //or error in character number 1.  This might be a case where something expected does
               //not appear.  Hence the ">" test.
         )
         ||
         (
            (in_warn_err_linenum != 0)
               //Not the sentinel value, and safe to subtract 1 from.
            &&
            ((in_warn_err_linenum - 1) > in_buf.Nelem())
               //If, for example, a buffer has 0 characters, it would be possible to detect a warning
               //or error in line number 1.  This might be a case where something expected does
               //not appear.  Hence the ">" test.  In general, a buffer with n characters can't
               //have an error detected on a line number greater than n+1, because in order to
               //advance from line to line, there would have to be at least a single EOL character.
            )
         ||
         (
            (in_warn_err_columnnum != 0)
               //Not the sentinel value, and safe to subtract 1 from.
            &&
            ((in_warn_err_columnnum - 1) >= in_buf.Nelem())
               //Can't be more columns than characters.
               //Same arguments with column number. A more complex check would also be possible.
         )
      )
   {
      //Issues
      warn = true;
      rv = false;
   }

   //Populate the structure from the caller's parameters.
   rec_to_add.filepath               = in_filepath;
   rec_to_add.filename               = in_filename;
   rec_to_add.warn_err_charnum       = in_warn_err_charnum;
   rec_to_add.warn_err_linenum       = in_warn_err_linenum;
   rec_to_add.warn_err_columnnum     = in_warn_err_columnnum;
   if ((in_warn_err_severity <= LgCppDdErrlistCdASeverity::Undefined) || (in_warn_err_severity >= LgCppDdErrlistCdASeverity::EnumMax))
      rec_to_add.warn_err_severity   = LgCppDdErrlistCdASeverity::Undefined;
   else
      rec_to_add.warn_err_severity   = in_warn_err_severity;
   rec_to_add.warn_err_id            = in_warn_err_id;
   rec_to_add.warn_err_desc          = in_warn_err_desc;
   rec_to_add.warn_err_instance_desc = in_warn_err_instance_desc;
   //Waiting to set buffer_context_excerpt[50].
   //Waiting to set buffer_context_used.
   //Waiting to set buffer_context_caret_pt.
   //Waiting to set content_warning.
   rec_to_add.content_warning = warn;
   //No need to set marked.  The default of false is correct.

   //Add the record to the vector.
   m_errlist.push_back(rec_to_add);

   return rv;
}


/*!
 * \brief Returns the number of warnings/errors that are marked.  This will always be
 *        less than or equal to Nelem().  A deduction about the number unmarked can
 *        be made as Nelem() - NMarked().
 *
 *
 * \returns                                    The number of elements that are marked.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
size_t LgCppDdErrlistCdA::NMarked(void)
{
   size_t nmarked = 0;

   for (const auto& rec : m_errlist)
   {
      if (rec.marked)
         nmarked++;
   }

   return nmarked;
}


/*!
 * \brief Marks a record, identified by in_elemidx.  If in_elemidx is out of bounds,
 *        no action will be taken.
 *
 * \param[in]     in_elemidx               The index of the element, starting at [0].  An index out of
 *                                         range will not cause any undesirable behavior, and the
 * \returns                                function call will have no effect.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
void LgCppDdErrlistCdA::Mark(size_t in_elemidx)
{
   size_t nelem;

   nelem = m_errlist.size();

   if (in_elemidx < nelem)  //Only take action if index in range.
   {
      m_errlist[in_elemidx].marked = true;
   }
}


/*!
 * \brief Marks all existing records.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
void LgCppDdErrlistCdA::MarkAll(void)
{
   for (auto& rec : m_errlist)
   {
      rec.marked = true;
   }
}


/*!
 * \brief Unmarks a record, identified by in_elemidx.  If in_elemidx is out of bounds,
 *        no action will be taken.
 *
 * \param[in]     in_elemidx               The index of the element, starting at [0].  An index out of
 *                                         range will not cause any undesirable behavior, and the
 * \returns                                function call will have no effect.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
void LgCppDdErrlistCdA::UnMark(size_t in_elemidx)
{
   size_t nelem;

   nelem = m_errlist.size();

   if (in_elemidx < nelem)  //Only take action if index in range.
   {
      m_errlist[in_elemidx].marked = false;
   }
}


/*!
 * \brief Unmarks all existing records.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
void LgCppDdErrlistCdA::LgCppDdErrlistCdA::UnMarkAll(void)
{
   for (auto& rec : m_errlist)
   {
      rec.marked = false;
   }
}


/*!
 * \brief Returns true if the specified record is marked, or false otherwise.
 *
 * \param[in]     in_elemidx               The index of the element, starting at [0].  An index out of
 *                                         range will not cause any undesirable behavior, and the
 * \returns                                function will return false.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
bool LgCppDdErrlistCdA::IsMarked(size_t in_elemidx)
{
   size_t nelem;
   bool rv = false;

   nelem = m_errlist.size();

   if (in_elemidx < nelem)  //Only take action if index in range.
   {
      rv = m_errlist[in_elemidx].marked;
   }

   return rv;
}


/*!
 * \brief Retrieves a copy of the specified records, if it exists.
 *
 * \param[in]     in_elemidx               The index of the element, starting at [0].  An index out of
 *                                         range will not cause any undesirable behavior, and out_success
 *                                         will be false and out_rec will be a freshly-initialized unassigned
 *                                         record.
 * \param[out]    out_success              true if the record could be successfully retrieved, false otherwise.
 * \param[out]    out_rec                  A copy of the record.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
void LgCppDdErrlistCdA::GetRec(size_t in_elemidx, bool& out_success, tLgCppDdErrlistCdARec& out_rec)
{
   tLgCppDdErrlistCdARec out_rec_copy;  //Used to provide uninit record of no success.
   bool rv = false;
   size_t nelem;

   nelem = m_errlist.size();

   if (in_elemidx < nelem)  //Only take action if index in range.
   {
      out_rec_copy = m_errlist[in_elemidx];
      rv = true;
   }

   out_success = rv;
   out_rec = out_rec_copy;
}


/*!
 * \brief Emits a list of errors and warnings to the specified output stream, in a text suitable for console-mode
 *        output.
 *
 * \param[in]     in_output_stream               The stream on which to output.
 * \param[in]     in_title                       The title to use.  If the empty string is passed, no title will be
 *                                               printed.
 * \param[in]     in_require_marked_or_unmarked  0 if all records should be printed, <0 if only unmarked records
 *                                               should be printed, >0 if only marked records should be printed.
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
void LgCppDdErrlistCdA::EmitListToStreamText(std::ostream& in_output_stream, const std::string in_title, int in_require_marked_or_unmarked)
{
   size_t n_in_list;
      //Number of elements in the list.
   size_t n_to_print;
      //Number of records to print.
   size_t i, j;
      //General iteration variables.

   //Figure out how many elements in the list.  This will be used later.
   n_in_list = m_errlist.size();

   //std::cout << "In Emit, n is:" << n_in_list << std::endl;

   //Count up the number of records to print.
   if (in_require_marked_or_unmarked == 0)
   {
      n_to_print = n_in_list;
   }
   else
   {
      n_to_print = 0;

      for (const auto& rec : m_errlist)
      {
         if (
               ((in_require_marked_or_unmarked < 0) && (!rec.marked))
               ||
               ((in_require_marked_or_unmarked > 0) && (rec.marked))
            )
         {
            n_to_print++;
         }
      }
   }

   //If there is a title, print it.
   if (in_title.size())
   {
      in_output_stream << in_title << std::endl;
      LgCppCm_FioHlineStream(in_output_stream);
   }

   //Print the records.
   if (!n_to_print)
   {
      in_output_stream << "No warning or error records." << std::endl;
      LgCppCm_FioHlineStream(in_output_stream);
   }
   else
   {
      std::string token_str;

      //std::cout << "else" << std::endl;

      j = 1;
      for (const auto& rec : m_errlist)
      {
         for (i = 0; i < n_in_list; i++) //Use integer index for easier access to index.
         {
            if (  
                  (in_require_marked_or_unmarked == 0)
                  ||
                  ((in_require_marked_or_unmarked < 0) && (!m_errlist[i].marked))
                  ||
                  ((in_require_marked_or_unmarked > 0) && (m_errlist[i].marked))
               )
            {
               //Record number.
               in_output_stream << "record: " << j << "/" << n_to_print << ", index: [" << i << "],";

               //Token type.
               LgCppDdErrlistCdA_WeTypeToString(m_errlist[i].warn_err_severity, token_str);
               in_output_stream << " warn_err_severity: " << token_str << std::endl;

               //File path.
               in_output_stream << "filepath: ";
               if (m_errlist[i].filepath.size() == 0)
                  in_output_stream << "N/A";
               else
                  in_output_stream << "\"" << m_errlist[i].filepath << "\"";
 
               //File name.
               in_output_stream << ", filename: ";
               if (m_errlist[i].filename.size() == 0)
                  in_output_stream << "N/A";
               else
                  in_output_stream << "\"" << m_errlist[i].filename << "\"";
               in_output_stream << std::endl;

               //warn_err_charnum.
               in_output_stream << "charnum: ";
               if (m_errlist[i].warn_err_charnum == 0)
                  in_output_stream << "N/A";
               else
                  in_output_stream << m_errlist[i].warn_err_charnum;

               //warn_err_linenum.
               in_output_stream << ", linenum: ";
               if (m_errlist[i].warn_err_linenum == 0)
                  in_output_stream << "N/A";
               else
                  in_output_stream << m_errlist[i].warn_err_linenum;

               //warn_err_columnnum.
               in_output_stream << ", columnnum: ";
               if (m_errlist[i].warn_err_columnnum == 0)
                  in_output_stream << "N/A";
               else
                  in_output_stream << m_errlist[i].warn_err_columnnum;
               in_output_stream << std::endl;

               //warn_err_id.
               in_output_stream << "warn_err_id: ";
               if (m_errlist[i].warn_err_id.size() == 0)
                  in_output_stream << "N/A";
               else
                  in_output_stream << "\"" << m_errlist[i].warn_err_id << "\"";

               //warn_err_desc.
               in_output_stream << ", warn_err_desc: ";
               if (m_errlist[i].warn_err_desc.size() == 0)
                  in_output_stream << "N/A";
               else
                  in_output_stream << "\"" << m_errlist[i].warn_err_desc << "\"";
               in_output_stream << std::endl;

               //warn_err_instance_desc.
               in_output_stream << "warn_err_instance_desc: ";
               if (m_errlist[i].warn_err_instance_desc.size() == 0)
                  in_output_stream << "N/A";
               else
                  in_output_stream << "\"" << m_errlist[i].warn_err_instance_desc << "\"";
               in_output_stream << std::endl;

               //buffer_context_used.
               in_output_stream << "buffer_context_used: ";
               in_output_stream << m_errlist[i].buffer_context_used;

               //buffer_context_excerpt
               {
                  size_t k;

                  in_output_stream << ", buffer_context_excerpt: \"";
                  for (k = 0; k < m_errlist[i].buffer_context_used; k++)
                  {
                     in_output_stream << m_errlist[i].buffer_context_excerpt[k];
                  }
               }
               in_output_stream << "\"" << std::endl;

               //buffer_context_caret_pt.
               in_output_stream << "buffer_context_caret_pt: ";
               in_output_stream << m_errlist[i].buffer_context_caret_pt;

               //content_warning.
               in_output_stream << ", content_warning: ";
               in_output_stream << m_errlist[i].content_warning;

               //marked.
               in_output_stream << ", marked: ";
               in_output_stream << m_errlist[i].marked;
               in_output_stream << std::endl;

               LgCppCm_FioHlineStream(in_output_stream);
            }

            j++;
         }
      }
   }

}


/*!
 * \brief Maps from enumeration of type LgCppDdErrlistCdASeverity to the text of the enumeration
 *        constant.
 *
 * \param[in]     in_type                  The enumeration constant to retrive the associated string for.
 * \param[out]    out_str                  The string associated with the enumeration constant, if "<illegal_value>"
 *                                         if the value is not valid as a type of warning/error.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
void LgCppDdErrlistCdA_WeTypeToString(const LgCppDdErrlistCdASeverity in_type, std::string& out_str)
{
   switch (in_type)
   {
      case LgCppDdErrlistCdASeverity::Undefined:
         out_str = "Undefined";
         break;
      case LgCppDdErrlistCdASeverity::Informational:
         out_str = "Informational";
         break;
      case LgCppDdErrlistCdASeverity::Advisory:
         out_str = "Advisory";
         break;
      case LgCppDdErrlistCdASeverity::Warning:
         out_str = "Warning";
         break;
      case LgCppDdErrlistCdASeverity::Medium:
         out_str = "Medium";
         break;
      case LgCppDdErrlistCdASeverity::High:
         out_str = "High";
         break;
      case LgCppDdErrlistCdASeverity::Severe:
         out_str = "Severe";
         break;
      default:
         out_str = "<illegal_value>";
         break;
   }
}