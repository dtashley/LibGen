//--------------------------------------------------------------------------------------------------
//Unsigned 8-bit integer buffer, RAM-based.
// 
//Used mostly to buffer entire files in RAM for further analysis and processing, but could be
//used for any binary or text content.
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
#define MODULE_LG_CPP_DD_BUF_UINT8

#include <assert.h>
#include <fstream>
#include <istream>
#include <iostream>
#include <iomanip>
#include <string.h>

#include "lg_cpp_dd_buf_uint8.hpp"

#include "lg_cpp_cm_fio.hpp"
#include "lg_cpp_dd_charf.hpp"
#include "lg_cpp_dd_malloc.hpp"
#include "lg_cpp_dd_size_t.hpp"

//In order for this class to work correctly, there must be 8 bits per character.
//This is the case with most general machines, but there are exceptions.  If a
//platform has wider characters than 8 bits, I'm not sure how file reads and
//so on would work.
#if !defined(CHAR_BIT)
   #error "CHAR_BIT not defined"
#elif (CHAR_BIT != 8)
   #error "CHAR_BIT != 8"
#endif

#define LG_CPP_DD_BUF_UINT8_ALLOC_INCREMENT (1024U)
   //The amount of additional memory allocated when the existing allocation is exhausted.
   //A larger value is more efficient but has the potential to waste more memory.


/*!
 * \brief                       Default constructor.  Constructs a new object.
 * 
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
LgBufUint8::LgBufUint8() noexcept : m_n_allocd{ 0 },
                                    m_n_used{ 0 }, 
                                    m_errs{ 0 }, 
                                    m_bufptr{nullptr}
{
}


/*!
 * \brief                       Destructor.  Destroys an object.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
LgBufUint8::~LgBufUint8() noexcept
{
   m_n_allocd = 0;
   m_n_used   = 0;
   m_errs     = 0;
   LgCppCm_MallocFree(m_bufptr);  //Called function handles nullptr case.
   m_bufptr   = nullptr;
}


/*!
 * \brief                       Copy constructor.  Constructs a new object from an existing object.
 * 
 * \reentrancyandthreadsafety   Not evaluated.
 * 
 * \errorsandexceptions         A <i>std::bad_alloc</i> exception is possible in case of an
 *                              allocation failure.
 */
LgBufUint8::LgBufUint8(const LgBufUint8& buf)
{
   m_n_used   = buf.m_n_used;
   m_n_allocd = LgCppDd_SizeT_LeastMulNotSmallerThan(buf.m_n_used, 
                                                     LG_CPP_DD_BUF_UINT8_ALLOC_INCREMENT);
      //Ignore what the caller had allocated in favor of what is best.
      //This automatically trims.
   m_errs = buf.m_errs;
   if (m_n_allocd == 0)
   {
      //In this case, nullptr.
      m_bufptr = nullptr;
   }
   else
   {
      m_bufptr = (uint8_t*)LgCppCm_MallocMalloc(m_n_allocd);
   }

   //Copy the memory.  Although memcpy() likely guards against dereferencing
   //pointers in the 0 size case, still test.
   if ((m_n_used != 0) && (m_bufptr != nullptr) && (buf.m_bufptr != nullptr))  //Suppress compiler warning
   {
      memcpy(m_bufptr, buf.m_bufptr, m_n_used);
   }
}


/*!
 * \brief                       Copy assignment operator.  Copies an object if the copy target has already been constructed.
 * 
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         A <i>std::bad_alloc</i> exception is possible in case of an
 *                              allocation failure.
 */
LgBufUint8& LgBufUint8::operator=(const LgBufUint8& buf)
{
   if (this != &buf)  //Guard against self-assignment.
   {
      size_t necessary_allocation;

      //Figure out the allocation that would be desirable to contain
      //the copy source.
      necessary_allocation = LgCppDd_SizeT_LeastMulNotSmallerThan(buf.m_n_used,
                                                                  LG_CPP_DD_BUF_UINT8_ALLOC_INCREMENT);

      //If the copy target has less than that allocated, bump it up.  We don't
      //downsize, as that is inefficient.
      if (m_n_allocd < necessary_allocation)
      {
         m_bufptr = (uint8_t*)LgCppCm_MallocRealloc(m_bufptr, necessary_allocation);
            //nullptr case handled correctly.
         m_n_allocd = necessary_allocation;
      }

      //Copy the remaining fields and memory.
      m_n_used = buf.m_n_used;
      m_errs   = buf.m_errs;

      //Be cautious on the copy.  memcpy() probably does not dreference
      //memory on a count of 0, but take no chances.
      if ((m_n_used != 0) && (m_bufptr != nullptr) && (buf.m_bufptr != nullptr))
         memcpy(m_bufptr, buf.m_bufptr, m_n_used);
   }

   return *this;
}


/*!
 * \brief                       Move constructor.  Constructs an object from an existing constructed object that is end-of-life.
 * 
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
LgBufUint8::LgBufUint8(LgBufUint8&& buf) noexcept
{
   //Bring over data fields and resources.
   m_n_allocd     = buf.m_n_allocd;
   m_n_used       = buf.m_n_used;
   m_errs         = buf.m_errs;
   m_bufptr       = buf.m_bufptr;  //Snatch buffer pointer, allocated or not.

   //Prepare the source object for clean destruction.
   buf.m_n_allocd = 0;
   buf.m_n_used   = 0;
   buf.m_errs     = 0;
   buf.m_bufptr   = nullptr;
}


/*!
 * \brief                       Move assignment operator.  Assigns to a constructed object from a constructed object that is end-of-life.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
 LgBufUint8& LgBufUint8::operator=(LgBufUint8&& buf) noexcept
{
   if (this != &buf)  //Guard against self-assignment.
   {
      //Deallocate the target buffer, if it was allocated.
      if (m_bufptr)
         LgCppCm_MallocFree(m_bufptr);

      //Bring over data fields and resources.
      m_n_allocd = buf.m_n_allocd;
      m_n_used   = buf.m_n_used;
      m_errs     = buf.m_errs;
      m_bufptr   = buf.m_bufptr;  //Snatch buffer pointer, allocated or not.

      //Prepare the source object for clean destruction.
      buf.m_n_allocd = 0;
      buf.m_n_used   = 0;
      buf.m_errs     = 0;
      buf.m_bufptr   = nullptr;
   }

   return *this;
}


/*!
 * \brief General object comparison function.  Returns:
 *    <ul>
 *       <li>-1 if <i>*this</i> is less than <i>other</i>.</li>
 *       <li>0 if <i>*this</i> is equal to <i>other</i>.</li>
 *       <li>1 if the <i>*this</i> is greater than <i>other</i>.</li>
 *    </ul>
 *
 * The order of comparison is:
 *    <ol>
 *       <li>A zero-length buffer is less than any non-zero-length buffer.</li>
 *       <li>If two buffers are non-zero length, the portion within both of their
 *       lengths is evaluated.  The first lesser element belongs to the lesser
 *       buffer.</li>
 *       <li>If the portion within both of their lengths is identical, the shorter
 *       buffer is the lesser.</li>
 *    </ol>
 *
 *    For two buffers to be equal, they must be of equal length and have byte-for-byte
 *    identical contents.
 *
 * \param[in]  other   Buffer to compare against <i>*this</i>.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
int LgBufUint8::Cmp(const LgBufUint8& other) const noexcept
{
   if ((m_n_used == 0) && (other.m_n_used == 0))
   {
      //Two zero-length strings are equal.
      return 0;
   }
   else if (m_n_used == 0) //&& (other.m_n_used != 0)
   {
      return -1;
   }
   else if (other.m_n_used == 0) //&& (m_n_used != 0)
   {
      return 1;
   }
   else //Neither is zero-length
   {
      int result;

      //Compare the memory that they have in common.
      if (m_n_used <= other.m_n_used)
         result = memcmp(m_bufptr, other.m_bufptr, m_n_used);
      else
         result = memcmp(m_bufptr, other.m_bufptr, other.m_n_used);

      if (result < 0)
      {
         return -1;
      }
      else if (result > 0)
      {
         return 1;
      }
      else
      {
         //Tie will be broken by length.
         if (m_n_used < other.m_n_used)
         {
            return -1;
         }
         else if (m_n_used > other.m_n_used)
         {
            return 1;
         }
         else
         {
            return 0;
         }
      }
   }
}


/*!
 * \brief                       Overload of the less than ('<') operator for the <i>LgBufUint8</i> class.
 *                              Returns <i>true</i> if <i>*this</i> is less than <i>other</i>, or
 *                              <i>false</i> otherwise.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
*/
bool LgBufUint8::operator<(const LgBufUint8& other) const noexcept
{
   return (Cmp(other) < 0);
}


/*!
 * \brief                       Overload of the less than or equal to ('<=') operator for the 
 *                              <i>LgBufUint8</i> class.  Returns <i>true</i> if <i>*this</i> is
 *                              less than or equal to <i>other</i>, or <i>false</i> otherwise.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
bool LgBufUint8::operator<=(const LgBufUint8& other) const noexcept
{
   return (Cmp(other) <= 0);
}


/*!
 * \brief                       Overload of the equality test ('==') operator for the
 *                              <i>LgBufUint8</i> class.  Returns <i>true</i> if <i>*this</i> is
 *                              equal to <i>other</i>, or <i>false</i> otherwise.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
bool LgBufUint8::operator==(const LgBufUint8& other) const noexcept
{
   return (Cmp(other) == 0);
}


/*!
 * \brief                       Overload of the inequality test ('!=') operator for the
 *                              <i>LgBufUint8</i> class.  Returns <i>true</i> if <i>*this</i> is
 *                              unequal to <i>other</i>, or <i>false</i> otherwise.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
bool LgBufUint8::operator!=(const LgBufUint8& other) const noexcept
{
   return (Cmp(other) != 0);
}


/*!
 * \brief                       Overload of the greater than or equal to ('>=') operator for the
 *                              <i>LgBufUint8</i> class.  Returns <i>true</i> if <i>*this</i> is
 *                              greater than or equal to <i>other</i>, or <i>false</i> otherwise.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
bool LgBufUint8::operator>=(const LgBufUint8& other) const noexcept
{
   return (Cmp(other) >= 0);
}


/*!
 * \brief                       Overload of the greater than ('>') operator for the
 *                              <i>LgBufUint8</i> class.  Returns <i>true</i> if <i>*this</i> is
 *                              greater than <i>other</i>, or <i>false</i> otherwise.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
bool LgBufUint8::operator>(const LgBufUint8& other) const noexcept
{
   return (Cmp(other) > 0);
}


/*!
 * \brief                       Returns the errors set for the object. Errors are packed into the return value, or'd together
 *                              as bit flags.
 *
 *                              The errors include  LG_CPP_DD_BUF_UINT8_ERR_FINFO
 *                              and other similarly prefixed constants nearby in the code.
 * 
 *                              A return value of 0 indicates that no errors have been encountered.
 * 
 *                              Errors stick until cleared with the <i>ErrsClear()</i> method.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
unsigned LgBufUint8::ErrsGet() const noexcept
{
   return m_errs;
}


/*!
 * \brief                       Clears any recorded errors in the object.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
void LgBufUint8::ErrsClear() noexcept
{
   m_errs = 0;
}


/*!
 * \brief Reads from a file, in binary mode, into the object; and closes
 *        the file after it has been fully read.  Returns <i>true</i>
 *        on success or <i>false</i> on failure.
 *
 *        Errors are possible when gathering information about the file,
 *        when opening it, when reading it, or when closing it.  Such errors are recorded using
 *        the error recording mechanism for the object, and the function
 *        continues as best it can.
 *
 * \param[in]     fname          The name of the file from which to read.  This parameter
 *                               is not examined by this function:  whether the filename is
 *                               valid is decided solely by the operating system.
 * 
 * \returns                      <i>true</i> if the operation (info, open, read, close) is fully successful,
 *                               or <i>false</i> otherwise.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Errors are possible when gathering information about the file,
 *                               opening the file, reading the file, or closing the file.  Any of those errors
 *                               will be reported through the error recording mechanism of the
 *                               object.
 *                               <br><br>
 *                               It is possible for an allocation failure to occur while the
 *                               object is grown to accommodate the file contents.  This would result
 *                               in a <i>std::bad_alloc</i> exception.
 */
bool LgBufUint8::Fread(const std::string& fname)
{
   std::ifstream infile;
   size_t file_size;
   size_t new_allocation;

   infile.open(fname, std::ios::binary);
      //Discovered experimentally that need to open as binary or line endings get translated,
      //and perhaps other changes made as well.

   if (!infile.is_open())
   {
      //Something has gone wrong.  Error out.
      m_errs |= LG_CPP_DD_BUF_UINT8_ERR_FOPEN;

      //Try to close, unsure if necessary.
      infile.close();

      //Return as-is.
      return false;
   }

   //Figure out how big the file is.
   infile.seekg(0, std::ios::end);                                   //Seek to end of file.
   file_size = LgCppDd_SizeT_StreamposToSizeT(infile.tellg());       //Obtain the file size, convert to size_t.
   infile.seekg(0, std::ios::beg);                                   //Seek back to the beginning so we can read it from there.

   //If the file size is 0, we can take an abbreviated path.
   if (file_size == 0)
   {
      infile.close();

      m_n_used = 0;  //Allocation doesn't matter with zero length.  No other field
                     //needs to be set.

      return true;
   }

   //If the file size is non-zero, adjust the allocation appropriately.
   new_allocation = LgCppDd_SizeT_LeastMulNotSmallerThan(file_size,
                                                         LG_CPP_DD_BUF_UINT8_ALLOC_INCREMENT);

   //Adjust allocation upward if necessary.  Do not down-allocate.
   if (new_allocation > m_n_allocd)
   {
      if (m_bufptr == nullptr)
      {
         m_bufptr   = (uint8_t*)LgCppCm_MallocMalloc(new_allocation);
         m_n_allocd = new_allocation;
      }
      else
      {
         m_bufptr = (uint8_t*)LgCppCm_MallocRealloc(m_bufptr, new_allocation);
         m_n_allocd = new_allocation;
      }
   }

   //If the file size is non-zero, read the file, close, return.
   m_n_used   = file_size;

   infile.read((char*)m_bufptr, file_size);

   //Check for read errors.  If one occurred, error out.
   if (infile.fail())
   {
      //Error reading stream.
      m_errs |= LG_CPP_DD_BUF_UINT8_ERR_FREAD;
      infile.close();
      return false;
   }

   infile.close();

   //Check for file close errors.  If one occurred, error out.
   if (infile.fail())
   {
      //Error closing stream.
      m_errs |= LG_CPP_DD_BUF_UINT8_ERR_FCLOSE;
      return false;
   }

   return true;
}


/*!
 * \brief Writes an object to a file, in binary mode; and closes
 *        the file after it has been fully written.  Returns <i>true</i>
 *        on success or <i>false</i> on failure.
 *
 *        Other than recording any errors, the object is not modified.
 * 
 *        Errors are possible when gathering information about the file,
 *        when writing it, or when closing it.  Such errors are recorded using
 *        the error recording mechanism for the object, and the function
 *        continues as best it can.
 *
 * \param[in]     fname          The name of the file to which to write the object.  This parameter
 *                               is not examined by this function:  whether the filename is
 *                               valid is decided solely by the operating system.
 *
 * \returns                      <i>true</i> if the operation (open, write, close) is fully successful,
 *                               or <i>false</i> otherwise.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Errors are possible when opening the file,
 *                               writing the file, or closing the file.  Any of those errors
 *                               will be reported through the error recording mechanism of the
 *                               object.
 */
bool LgBufUint8::Fwrite(const std::string &fname)
{
   std::ofstream outfile;

   outfile.open(fname, std::ios::binary);
      //Discovered experimentally that need to open as binary to prevent translations.

   if (!outfile.is_open())
   {
      //Something has gone wrong.  Error out.
      m_errs |= LG_CPP_DD_BUF_UINT8_ERR_FOPEN;

      //Try to close, unsure if necessary.
      outfile.close();

      //Return as-is.
      return false;
   }

   //Write the file.  We can write the entire file all at once.
   //Since this is a member function, we know that the representation
   //is compatible with write.
   if ((m_n_used > 0) && (m_bufptr != nullptr)) //Caution with zero-length writes and null pointers.
   {
      outfile.write((char *)m_bufptr, m_n_used);

      //Check for write errors.  If one occurred, error out.
      if (outfile.fail())
      {
         //Error reading stream.
         m_errs |= LG_CPP_DD_BUF_UINT8_ERR_FWRITE;
         outfile.close();
         return false;
      }
   }

   outfile.close();

   //Check for file close errors.  If one occurred, error out.
   if (outfile.fail())
   {
      //Error closing stream.
      m_errs |= LG_CPP_DD_BUF_UINT8_ERR_FCLOSE;
      return false;
   }

   return true;
}


/*!
 * \brief                        Sets a buffer to zero length and clears errors, but does not deallocate any allocated memory.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 * 
 * \errorsandexceptions          No exceptions or errors are possible.
 */
void LgBufUint8::Clear() noexcept
{
   m_n_used = 0;
   m_errs   = 0;

   //No need to adjust anything else.
}


/*!
 * \brief                        Reduces the memory allocated for a buffer to be the minimum size to hold the
 *                               used portion of the buffer, subject to blocking and allocation rules.
 * 
 * \reentrancyandthreadsafety    Not evaluated.
 * 
 * \errorsandexceptions          An allocation failure is possible, which would result in a
 *                               std::bad_alloc exception.
 */
void LgBufUint8::Trim()
{
   size_t new_amount_allocated;

   new_amount_allocated = LgCppDd_SizeT_LeastMulNotSmallerThan(m_n_used,
                                                               LG_CPP_DD_BUF_UINT8_ALLOC_INCREMENT);

   if (new_amount_allocated < m_n_allocd)  //Only reallocate downward.
   {
      if (new_amount_allocated == 0)
      {
         if (m_bufptr)
         {
            //Previously allocated, free it.
            LgCppCm_MallocFree(m_bufptr);
            m_bufptr = nullptr;

            m_n_allocd = new_amount_allocated;
         }
         else
         {
            //m_bufptr already nullptr, no need to adjust.  m_n_allocd must also
            //be 0, or there is a data structure inconsistency.
            //
            //Should probably test m_n_allocd and throw exception if not 0.
            m_n_allocd = 0;
         }
      }
      else
      {
         if (m_bufptr)
         {
            //Need to reallocate.  Exception will be thrown if failure.
            m_bufptr = (uint8_t *)LgCppCm_MallocRealloc(m_bufptr, new_amount_allocated);

            m_n_allocd = new_amount_allocated;
         }
         else
         {
            //Should be unreachable, or there is a data structure inconsistency.
            //Should probably define and throw exception.
         }
      }
   }
}


/*!
 * \brief                        Sets buffer to empty, deallocates memory, and clears errors.  Largely equivlaent
 *                               to destruction.  The object is still logically valid and can still be used freely.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          No exceptions or errors are possible.
 */
void LgBufUint8::ClearAndDeallocate() noexcept
{
   m_n_allocd = 0;
   m_n_used   = 0;
   m_errs     = 0;
   LgCppCm_MallocFree(m_bufptr); //Called function handles nullptr case.
   m_bufptr   = nullptr;
}


/*!
 * \brief                        Reads one item by value at the position indicated.  Out-of-range subscripts
 *                               are handled safely.
 *
 * \param[in]  read_pt           The index, starting at 0, of the item to read.
 *                               Out of range values are handled safely, and will
 *                               result in 0 being returned.
 *
 * \returns                      The value of the item at the read_pt, or 0 if the <i>read_pt</i>
 *                               is invalid.  Out of range values of <i>read_pt</i> are not
 *                               recorded.
 * 
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          No exceptions or errors are possible.
 */
uint8_t LgBufUint8::ReadOneByVal(const size_t read_pt) const noexcept
{
   if (read_pt >= m_n_used)
   {
      //m_errs |= LG_CPP_DD_BUF_UINT8_ERR_ACCESS;
      //   Can't record error in a const member function.
      return 0;
   }
   else
   {
      return m_bufptr[read_pt];
   }
}


/*!
 * \brief                        Reads one item by pointer from the object at the position indicated.  Out-of-range subscripts
 *                               are handled safely.
 *
 * \param[in]     read_pt        The index, starting at 0, of the item to read.
 *                               Out of range values are handled safely, and will
 *                               result in 0 being returned.  The use of out of range
 *                               indices is not recorded.
 *
 * \param[in,out] read_dst       Pointer to location of where to place a copy of the data.
 *                               This pointer may legally be <i>nullptr</i>, in which case,
 *                               the pointer will not be dreferenced.
 * 
 * \reentrancyandthreadsafety    Not evaluated.
 * 
 * \errorsandexceptions          No exceptions or errors are possible.
 */
void LgBufUint8::ReadOne(const size_t read_pt, uint8_t* const read_dst) const noexcept
{
   uint8_t val_to_return = 0;

   if (read_pt >= m_n_used)
   {
      //m_errs |= LG_CPP_DD_BUF_UINT8_ERR_ACCESS;
      //   Can't record error in a const member function.
      val_to_return = 0;
   }
   else
   {
      val_to_return = m_bufptr[read_pt];
   }

   if (read_dst != nullptr)
   {
      *read_dst = val_to_return;
   }
}


/*!
 * \brief                        Reads multiple contiguous values from the object.
 *
 * \param[in]     read_pt        The index, starting at 0, of the first item to read.
 *                               Out of range values are handled safely.  The use of
 *                               out of range subscripts is not recorded.
 *                               
 * \param[in,out] read_dst       Pointer to location of where to place a copy of the data.
 *                               This pointer may legally be <i>nullptr</i>, in which case,
 *                               this pointer will not be dreferenced.  The area pointed
 *                               to must be large enough to handle <i>read_n</i> items.  All items
 *                               not actually read, due to a <i>read_pt</i> which is too large or
 *                               <i>read_n</i> going past the end of the buffer, will be assigned
 *                               0.
 * 
 * \param[in]     read_n         The number of items to read.  If <i>read_dst</i> is
 *                               non-<i>nullptr</i>, the pointer passed must have storage
 *                               to dereference at least this many consecutive items.  If 0
 *                               is used as an argument, the <i>read_dst</i> pointer will
 *                               not be dereferenced.
 *
 * \returns                      The number of items actually read.  This may be smaller
 *                               than <i>read_n</i> due to overrunning the buffer boundaries.
 * 
 * \reentrancyandthreadsafety    Not evaluated.
 * 
 * \errorsandexceptions          No exceptions or errors are possible.
 */
size_t LgBufUint8::Read(const size_t read_pt, uint8_t* const read_dst, const size_t read_n) const noexcept
{
   size_t n_read = 0;

   if (read_n > 0)  //Do nothing with a read count of 0.
   {
      //Populate the caller's buffer with 0.  This handles all cases of buffer inability
      //to read.
      if (read_dst != nullptr)
      {
         memset(read_dst, 0, read_n);
      }

      //The number read is the minimum of:
      //  a)The number of elements at and after read_pt up until
      //    the end of the buffer.
      //  b)The number of elements requested, read_n.
      if (read_pt >= m_n_used)
      {
         //No elements to read at and beyond read_pt.  This also covers the
         //m_n_used == 0 case.
         n_read = 0;
      }
      else if ((m_n_used - read_pt) < read_n)
      {
         //Regarding the test above:  we know read_pt < m_n_used because
         //of the if() condition above.  So (a)it is safe to do the subtraction,
         //(b)the result will be 1 or greater, and (c)"m_n_used - read_pt"
         //is the correct calculation of the number of items that can be
         //read from the buffer starting at read_pt.
         // 
         //The number of elements remaining in the buffer is the
         //limiting factor.  This is how many we read.
         n_read = m_n_used - read_pt;
      }
      else
      {
         //We can read the full requested number.
         n_read = read_n;
      }

      //Copy out the requested number, if the pointer is not nullptr.
      if ((m_bufptr != nullptr) && (read_dst != nullptr) && (n_read > 0))
      {
         memcpy(read_dst, m_bufptr + read_pt, n_read);
      }
      else
      {
         n_read = 0;
      }
   } //End if (read_n > 0)

   return n_read;
}


/*!
 * \brief                        Assigns one element of object.  An out of range subscript is handled by taking no action.
 *
 * \param[in]  assign_pt         The index, starting at 0, of the item to assign.
 *                               Out of range values are handled safely, and will
 *                               result in no action taken.
 *
 * \param[in]  assign_val        The value to assign.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          No exceptions or errors are possible.
 */
void LgBufUint8::AssignOne(const size_t assign_pt, const uint8_t assign_val) noexcept
{
   if (assign_pt < m_n_used) //m_n_used >= 1, which means the allocation has been made.
   {
      m_bufptr[assign_pt] = assign_val;
   }
}


/*!
 * \brief                        Assigns zero or more elements of an object.
 *
 * \param[in]     assign_pt      The index, starting at 0, identifying the first
 *                               element to be assigned.
 *
 * \param[in]     assign_src     The values to assign.  If this pointer is <i>nullptr</i>,
 *                               the pointer is not dereferenced 0-values are used
 *                               to assign into the object.
 *
 * \param[in]     assign_n       The number of items to assign.  This value may be
 *                               zero.  Any items assigned are staged in the same order
 *                               in the buffer as they were in the caller's area.
 *                               If this value is large enough to logically exceed
 *                               what the underlying object can accommodate without
 *                               increasing its size, only as many items as can be assigned
 *                               will be inserted.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          No exceptions are errors are possible.
 */
void LgBufUint8::Assign(const size_t assign_pt, const uint8_t* const assign_src, const size_t assign_n)
{
   size_t headroom;
   size_t implemented_assign_n;

   if (
         (assign_pt < m_n_used)   //Starting index must be assignable.  This also precludes m_n_used==0,
                                  //as nothing unsigned can be less than 0.
         &&
         (assign_n != 0)          //Must desire to assign 1 or more items, or nothing to do.
      )
   {
      headroom = m_n_used - assign_pt;
         //Figure out how many elements we have room to assign.

      //Calculate the minimum of what we are requesting and what we can do.
      if (assign_n > headroom)
         implemented_assign_n = headroom;
      else
         implemented_assign_n = assign_n;

      //If the caller's pointer is nullptr, block fill with 0, else copy.
      if (assign_src == nullptr)
         memset(m_bufptr + assign_pt, 0, implemented_assign_n);
      else
         memcpy(m_bufptr + assign_pt, assign_src, implemented_assign_n);
   }
}


/*!
 * \brief                        Appends one item to the end of the object.
 *
 * \param[in]     appendee       The item to append.
 * 
 * \reentrancyandthreadsafety    Not evaluated.
 * 
 * \errorsandexceptions          If the object can't be grown (extremely unlikely for technical
 *                               reasons), the function silently returns without modifying the
 *                               object.  A bad allocation exception is possible if memory cannot be
 *                               allocated to grow the object.
 */
void LgBufUint8::AppendOne(const uint8_t appendee)
{
   size_t proposed_allocation;

   //If the number of elements used is already SIZE_MAX, can't do anything.  This is
   //an extremely unlikely scenario on a PC platform.
   if (m_n_used < SIZE_MAX)
   {
      //Calculate the right allocation for the new size.
      proposed_allocation = LgCppDd_SizeT_LeastMulNotSmallerThan(m_n_used + 1,
                                                                 LG_CPP_DD_BUF_UINT8_ALLOC_INCREMENT);

      //If the right allocation is larger than the current allocation, bump the
      //amount allocated up.  If the right allocation is less, we don't allocate
      //downward, as  that would be inefficient.
      if (proposed_allocation > m_n_allocd)
      {
         m_bufptr = (uint8_t*)LgCppCm_MallocRealloc(m_bufptr, proposed_allocation);
            //nullptr case covered by called function.
         m_n_allocd = proposed_allocation;
      }

      //There is now one more element.
      m_n_used++;

      //Assign the item.  m_n_used starting as 0 and being 1 after increment is covered
      //correctly.
      m_bufptr[m_n_used - 1] = appendee;
   }
}


/*!
 * \brief                        Appends zero or more items to the end of the buffer.  If the maximum memory
 *                               allocation would be exceeded, only the number of items that would be permissible
 *                               are appended.
 *
 * \param[in]     append_src     Pointer to content to append.  This pointer may be <i>nullptr</i>, in which
 *                               case the pointer is not dereferenced and only 0-elements are appended.
 * 
 * \param[in]     append_n       Number of items to append.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 * 
 * \errorsandexceptions          If the object can't be grown (extremely unlikely for technical
 *                               reasons), the function silently returns without modifying the
 *                               object.  A <i>std::bad_alloc</i> exception is possible if memory cannot be
 *                               allocated to grow the object.
 */
void LgBufUint8::Append(const uint8_t* const append_src, const size_t append_n)
{
   size_t headroom;
   size_t implemented_append_n;
   size_t required_allocation;

   if (append_n != 0)  //Nothing to do in the zero case.
   {
      //Calculate the number of elements we could actually add.
      headroom = LgCppDd_SizeT_MaximumAddend(m_n_used);

      //Scale back the request if it is too large.
      if (append_n > headroom)
         implemented_append_n = headroom;
      else
         implemented_append_n = append_n;

      //Calculate the right allocation for the new size.
      required_allocation = LgCppDd_SizeT_LeastMulNotSmallerThan(m_n_used + implemented_append_n,
                                                                 LG_CPP_DD_BUF_UINT8_ALLOC_INCREMENT);

      //If allocation needs to be adjusted upward, reallocate.
      if (required_allocation > m_n_allocd)
      {
         m_bufptr = (uint8_t*)LgCppCm_MallocRealloc(m_bufptr, required_allocation);
            //nullptr case covered by called function.
         m_n_allocd = required_allocation;
      }

      //Move in the new elements using one of two methods.  If the caller's pointer is
      //nullptr, block fill with 0; or if not, copy.
      if (append_src == nullptr)
      {
         memset(m_bufptr + m_n_used, 0, implemented_append_n);
      }
      else
      {
         memcpy(m_bufptr + m_n_used, append_src, implemented_append_n);
      }

      //Increase the number used to match what is used.
      m_n_used += implemented_append_n;
   } //End if (append_n != 0)
}


/*!
 * \brief                        Inserts one item into the object.
 *
 * \param[in]     insert_pt      The index, starting at 0, identifying the item before
 *                               which the passed item will be inserted.  A value of
 *                               <i>m_n_used</i> is equivalent to an append.  Values larger than
 *                               <i>m_n_used</i> are treated as m_n_used and will also result
 *                               in an append.
 * 
 * \param[in]     insertee       The value to insert.
 * 
 * \reentrancyandthreadsafety    Not evaluated.
 * 
 * \errorsandexceptions          If the object is at the maximum allocation size (very unlikely
 *                               for technical reasons), this function will silently take no
 *                               action. A failure to allocate or reallocate memory will result
 *                               in a <i>std::bad_alloc</i> exception.
 */
void LgBufUint8::InsertOne(const size_t insert_pt, const uint8_t insertee)
{
   size_t needed_allocation;

   //If the number of elements used is already SIZE_MAX, can't do anything.
   if (m_n_used < SIZE_MAX)
   {
      //Calculate the right allocation for the new size.
      needed_allocation = LgCppDd_SizeT_LeastMulNotSmallerThan(m_n_used + 1,
                                                               LG_CPP_DD_BUF_UINT8_ALLOC_INCREMENT);

      //If allocation needs to be larger, reallocate, possibly adjusting pointer.
      //We don't allocate down, as this would be inefficient.
      if (needed_allocation > m_n_allocd)
      {
         m_bufptr = (uint8_t*)LgCppCm_MallocRealloc(m_bufptr, needed_allocation);
           //nullptr case covered by called function.

         //There is now this much allocated.
         m_n_allocd = needed_allocation;
      }

      //There is now one more element.
      m_n_used++;

      if (insert_pt >= (m_n_used - 1))
      {
         //Equivalent to an append.
         m_bufptr[m_n_used - 1] = insertee;
      }
      else
      {
         //True insertion.
         //
         //Shift the memory by 1 byte.
         //
         //Analysis of parameter.
         //  destination
         //    insert_pt points to one beyond the byte that will be overwritten.
         //    Since insert_pt < (m_n_used - 1), insert_pt + 1 may point to the last
         //    element.
         //  source
         //    insert_pt points to one beyond the byte that will be overwritten.
         //    Since insert_pt < (m_n_used - 1), insert_pt may not point to the last
         //    element.
         //  n
         //    Since insert_pt < (m_n_used - 1), m_n_used - insert_pt is at least 2,
         //    and m_n_used - insert_pt - 1 is at least 1.
         memmove(m_bufptr + insert_pt + 1, m_bufptr + insert_pt, m_n_used - insert_pt - 1);

         //Assign the location.
         m_bufptr[insert_pt] = insertee;
      }
   }
}


/*!
 * \brief                        Inserts zero or more items into the object.
 *
 * \param[in]     insert_pt      The index, starting at 0, identifying the position before
 *                               which the passed item(s) will be inserted.  A value of
 *                               <i>m_n_used</i> is equivalent to an append.  Values larger than
 *                               <i>m_n_used</i> are treated as <i>m_n_used</i> and will also result
 *                               in an append.
 * 
 * \param[in]     insert_src     The values to insert.  If this pointer is nullptr,
 *                               the pointer is not dereferenced 0-values are used
 *                               to insert into the buffer.
 * 
 * \param[in]     insert_n       The number of items to insert.  This value may be
 *                               zero.  Any items inserted are staged in the same order
 *                               in the buffer as they were in the caller's area.
 *                               If this value is large enough to logically exceed
 *                               what the underlying indices can accommodate, only
 *                               as many items as can be inserted will be inserted.
 * 
 * \reentrancyandthreadsafety    Not evaluated.
 * 
 * \errorsandexceptions          A failure to allocate or reallocate memory will result
 *                               in a <i>std::bad_alloc</i> exception.
 */
void LgBufUint8::Insert(size_t insert_pt, const uint8_t* const insert_src, const size_t insert_n)
{
   size_t headroom;
   size_t implemented_insert_n;
   size_t required_allocation;

   if ((insert_n != 0) && (m_n_used < SIZE_MAX))  //Nothing to do if the insertion of zero elements is requested
                                                  //and/or we don't have room to insert.  However, on a PC platform,
                                                  //it would be exceedingly hard to fail to mee the second
                                                  //condition.
   {
      //Calculate the number of elements we could actually add.
      headroom = LgCppDd_SizeT_MaximumAddend(m_n_used);

      //Scale back the request if it is too large.
      if (insert_n > headroom)
         implemented_insert_n = headroom;
      else
         implemented_insert_n = insert_n;

      //Calculate the right allocation for the new size.
      required_allocation = LgCppDd_SizeT_LeastMulNotSmallerThan(m_n_used + implemented_insert_n,
                                                                 LG_CPP_DD_BUF_UINT8_ALLOC_INCREMENT);

      //If allocation needs to be adjusted upward, reallocate.  Don't reallocate downward, as
      //that would be very inefficient.
      if (required_allocation > m_n_allocd)
      {
         m_bufptr = (uint8_t*)LgCppCm_MallocRealloc(m_bufptr, required_allocation);
            //nullptr case covered by called function.
         m_n_allocd = required_allocation;
      }

      //Adjust the insert point so it is legal, and block copy memory to make room
      //for the insertion.
      if (insert_pt >= m_n_used)
      {
         //Append.
         //
         //Clip the insert point make it the index of the first element to be reassigned.
         insert_pt = m_n_used;

         //This is an append.  No need to copy memory out of the way.
      }
      else
      {
         //Move the existing elements out of the way to make room for the insert.
         //
         //Analysis of parameters:
         //   dst
         //      The highest-indexed array element we would copy to would be
         //        insert_pt + implemented_insert_n + m_n_used - insert_pt - 1
         //        = implemented_insert_n + m_n_used - 1
         //      This makes sense because implemented_insert_n was calculated
         //      to the no larger than the maximum number of elements that can be
         //      inserted, on top of the m_n_used existing, and the maximum
         //      element in such an array would be indexed [n_elements-1].
         //      Also, the allocation requested was for at least 
         //      m_n_used + implemented_insert_n, so we won't overrun the allocated
         //      memory.
         //   src
         //      This is the first character to be bumped up due to the insertion.
         //      Makes sense.
         //   n
         //      We know that insert_pt < m_n_used, so this value is positive.
         memmove(m_bufptr + insert_pt + implemented_insert_n,
                 m_bufptr + insert_pt,
                 m_n_used - insert_pt);
      }

      //Move in the new elements using one of two methods.  If the caller's pointer is
      //nullptr, block fill with 0; or if not, copy.
      if (insert_src == nullptr)
      {
         memset(m_bufptr + insert_pt, 0, implemented_insert_n);
      }
      else
      {
         memcpy(m_bufptr + insert_pt, insert_src, implemented_insert_n);
      }

      //Increase the number used to match what is used.
      m_n_used += implemented_insert_n;
   }
}


/*!
 * \brief                        Deletes the <i>delete_pt</i>'th character, with first character addressed as 0, shifting all other
 *                               characters to fill the void.  If an invalid <i>delete_pt</i> is detected, an improper memory access is
 *                               prevented, no action is taken, and no error is logged.
 *
 * \param[in]  delete_pt         Index of character to delete.  A value that corresponds to a
 *                               non-existent element will result in no action taken.
 *
 * \reentrancyandthreadsafety    Not evaluated.

 * \errorsandexceptions          No exceptions or errors are possible.
 */
void LgBufUint8::DeleteOne(const size_t delete_pt) noexcept
{
   if (delete_pt < m_n_used)  //If the delete_pt specifies a valid element.  Note that
                              //this also filters out the case of m_n_used==0, as with
                              //unsigned arithmetic nothing can be less than 0.
   {
      if (delete_pt < (m_n_used - 1)) //If we are deleting last element, no need to shift memory.
                                      //If delete_pt is invalid, no need to shift memory.
      {
         memmove(m_bufptr + delete_pt,
                    //Copy down on top of delete point.  Guaranteed to be within allocation as
                    //delete_pt < m_n_used.
                 m_bufptr + delete_pt + 1,
                    //Copy from one above delete point.  Guaranteed to be within allocation as
                    //the delete_pt < (m_n_used - 1), so the maximum value of delete_pt is
                    //m_n_used - 2.
                 (m_n_used - delete_pt) - 1);
                    //Maximum value of delete_pt is m_n_used-2, so subtraction will give
                    //2 or larger.  Result will be 1 or larger.
      }

      //If we are here, due to the first test of the function, m_n_used>0, so the decrement
      //is safe.
      m_n_used--;
   }
}


/*!
 * \brief                        Deletes <i>delete_n</i> consecutive elements, starting with the <i>delete_pt</i>'th
 *                               element, with indexing starting at 0.  If <i>delete_pt</i> is outside the buffer, no action is taken.
 *                               If <i>delete_pt</i> is inside the buffer but <i>delete_n</i> would exceed the number of buffer elements,
 *                               as many elements as possible are deleted.
 *
 * \param[in]  delete_pt         Index of first element to delete.  Out of range values are detected,
 *                               no invalid memory access is performed, and the error is silently
 *                               ignored.
 * 
 * \param[in]  delete_n          Number of elements to be deleted.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          No exceptions or errors are possible.
 */
void LgBufUint8::Delete(const size_t delete_pt, const size_t delete_n)
{
   size_t maximum_delete_n;
   size_t delete_n_implemented;

   if (delete_n > 0) //Client must want to delete at least one item
   {
      if (delete_pt < m_n_used) //Can't delete outside the buffer.
                                //This automatically tests for m_n_used==0, as
                                //no unsigned integer can be less than 0.
      {
         //Calculate how large delete_n may be.  delete_pt < m_n_used, so subtraction is safe.
         maximum_delete_n = m_n_used - delete_pt;  //Result is >= 1

         //Calculate the value we will actually use.  This is the minimum of what the caller is
         //requesting and what the maximum is.
         if (delete_n > maximum_delete_n)
            delete_n_implemented = maximum_delete_n;
         else
            delete_n_implemented = delete_n;

         if (delete_pt < (m_n_used - delete_n_implemented))
            //The test above rules out simple truncation.  In a truncation event,
            //   delete_pt == (m_n_used - delete_n_implemented).
            //If it is only a truncation event, the memory move is not necessary.
            //Only need to adjust m_n_used.
         {
            //Copy memory down to close the gap.
            //
            //A number of characteristics of the arguments to memmove() need to be verified.
            //   dst
            //      Need to verify that this is a valid memory address within the
            //      allocated area.
            //
            //      delete_pt < m_n_used, see test above, so this constraint is met.
            //   src
            //      Need to verify that this is a valid memory address within the allocated
            //      area.
            //      
            //      delete_pt < (m_n_used - delete_n_implemented)
            //      (delete_pt + delete_n_implemented) < m_n_used
            // 
            //      So this points to a valid memory location.
            //
            //      Need to verify that the terminal copy source location,
            //
            //         (delete_pt + delete_n_implemented) + (m_n_used - delete_pt - delete_n_implemented) - 1
            //
            //      is < m_n_used.  Simplifying the expression above indicates that the terminal copy source
            //      location is PRECISELY m_n_used-1, which is expected with a delete as memory is copied down.
            //   n
            //      Need to verify that this is less than m_n_used, and at least 1.
            //
            //      It is required as a condition to get there that:
            //
            //      delete_pt < (m_n_used - delete_n_implemented)
            // 
            //      Algebra directly leads to:
            // 
            //      delete_pt - delete_n_implemented < m_n_used
            // 
            //      We need to verify that this is at least 1.  Given the inequality above, the
            //      subtraction result will be at least 1.
            //
            //      Finally, need to verify that the subtraction, in an unknown order, won't
            //      cause trouble with underflow (although not normally a factor on 2's complement
            //      machines).  It can't be guaranteed that delete_pt >= delete_n_implemented,
            //      so will place parenthesis, although for subtraction I think the order would
            //      be left to right anyway.
            memmove(m_bufptr + delete_pt,
                    m_bufptr + delete_pt + delete_n_implemented,
                    (m_n_used - delete_pt) - delete_n_implemented);
         }

         //Adjust the count.  In the case of simple truncation, this alone will take care of it.
         m_n_used -= delete_n_implemented;
      }
   }
}


/*!
 * \brief                        Reads then deletes the object element at position <i>read_delete_pt</i>.  An illegal
 *                               <i>read_delete_pt</i> value is detected and results in no action and a value 0 being returned.
 *
 * \param[in]  read_delete_pt    Index of object element to read and delete.  An invalid value
 *                               is detected and will result in a value of 0 being read and no
 *                               other action taken.
 * 
 * \returns                      The <i>uint8_t</i> read, or 0 if a read was not possible.
 * 
 * \reentrancyandthreadsafety    Not evaluated.
 * 
 * \errorsandexceptions          No exceptions or errors are possible.
 */
uint8_t LgBufUint8::ReadAndDeleteOne(const size_t read_delete_pt)
{
   uint8_t rv = 0;

   rv = ReadOneByVal(read_delete_pt);
      //Behavior is consistent with this function: 0 returned if out of range argument.

   Delete(read_delete_pt, 1);
      //Behavior is consistent with this function: no action taken if out of range subscript.

   return rv;
}


/*!
 * \brief                        Reads then deletes the object elements at position read_delete_pt and extending
 *                               for <i>read_delete_n</i> elements.  An illegal <i>read_delete_pt</i> value is detected
 *                               and results in no action and a value 0 being returned.  A <i>read_delete_n</i> value
 *                               that causes the read to extend beyond the end of the object will cause the available
 *                               elements to be read and the unavailable elements to be read as 0.
 *
 * \param[in]  read_delete_pt    Index of buffer element to read and delete.  An invalid value
 *                               is detected and will result in a value of 0 being read and no
 *                               other action taken.
 * 
 * \param[in]  read_delete_dst   Pointer through which to return read value.  The pointer will
 *                               only be dereferenced if it is not <i>nullptr</i>.  If the pointer
 *                               is not <i>nullptr</i>, the caller must have provided enough
 *                               memory to accommodate at least <i>read_delete_n</i> elements.
 * 
 * \errorsandexceptions          No exceptions or errors are possible.
 */
void LgBufUint8::ReadAndDelete(const size_t read_delete_pt, uint8_t* const read_delete_dst, const size_t read_delete_n)
{
   Read(read_delete_pt, read_delete_dst, read_delete_n);
      //Read() handles the possible issues with values and pointers in a way consistent
      //with this function's behavior.
   Delete(read_delete_pt, read_delete_n);
     //Behavior is consistent with this function.
}


/*!
 * \brief                           Prints debugging information about a buffer object to the specified stream.
 * 
 * \param[in]  outstream            The stream to which to print.
 * 
 * \param[in]  title                The title to use for the debug printout.
 *
 * \param[in]  include_ending_hbar  <i>true</i> if a trailing horizontal line should be
 *                                  appended to the output.
 * 
 * \reentrancyandthreadsafety       Not evaluated.
 * 
 * \errorsandexceptions             Not evaluated.
 */
void LgBufUint8::DebugWrite(std::ostream& outstream, const std::string& title, const bool include_ending_hbar) const
{
   size_t i;

   outstream << "Debug write of: " << title << "\n";
   outstream << "m_n_allocd    : " << m_n_allocd << "\n";
   outstream << "m_n_used      : " << m_n_used   << "\n";
   outstream << "m_bufptr      : " << (unsigned long long)m_bufptr   << "\n";
   outstream << "m_errs        : " << m_errs     << "\n";
   for (i = 0; i < m_n_used; i++)
   {
      outstream << "[" << std::setw(10) << i << "] ";
      outstream << std::setw(3) << (unsigned)m_bufptr[i] << "  '";
      if (LgCppDd_CharfAsciiCharIsLargelyPrintable(m_bufptr[i]))
         outstream << m_bufptr[i];
      else
         outstream << "*";

      outstream << "'";

      if ((i >= (m_n_used - 1)) || ((i & 1) == 1))
         outstream << "\n";
      else
         outstream << "  ";
   }

   if (include_ending_hbar)
      LgCppCm_FioHlineStream(outstream);
}

//End of lg_cpp_dd_buf_uint8.cpp
