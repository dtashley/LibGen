//--------------------------------------------------------------------------------------------------
//Unsigned 8-bit integer file buffer, RAM-based.
// 
//Used to buffer entire files in RAM for further analysis and processing.
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
#define MODULE_LG_CPP_DD_BUF_UINT8

#include <assert.h>
#include <fstream>
#include <istream>
#include <iostream>
#include <iomanip>
#include <string.h>

#include "lg_cpp_dd_fbuf_uint8.hpp"

#include "lg_cpp_cm_fio.hpp"
#include "lg_cpp_dd_charf.hpp"
#include "lg_cpp_dd_malloc_alt.hpp"
#include "lg_cpp_dd_size_t.hpp"


/*!
 * \brief                       Default constructor.  Constructs a new object.
 * 
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
LgFbufUint8::LgFbufUint8() noexcept : m_state{ LgFbufUint8State::Unocccupied },
                                      m_errs{ 0 }
{
   //m_buf constructor called automatically
}


/*!
 * \brief                       Destructor.  Destroys an object.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
LgFbufUint8::~LgFbufUint8() noexcept
{
   m_state = LgFbufUint8State::Unocccupied;
   m_errs  = 0;

   //m_buf destructor called automatically
}

/*!
 * \brief                       Copy constructor.  Constructs a new object from an existing object.
 * 
 * \reentrancyandthreadsafety   Not evaluated.
 * 
 * \errorsandexceptions         A <i>std::bad_alloc</i> exception is possible in case of an
 *                              allocation failure.
 */
LgFbufUint8::LgFbufUint8(const LgFbufUint8& fbuf) : m_state{fbuf.m_state},
                                                    m_errs{fbuf.m_errs},
                                                    m_fname{fbuf.m_fname},
                                                    m_buf{fbuf.m_buf}
{
   //Member initializer list causes copy constructor (rather than copy assignment
   //operator) to be called.  This is the desired behavior for the LgBufUint8 and
   //std::string members.
}


/*!
 * \brief                       Copy assignment operator.  Copies an object if the
 *                              copy target has already been constructed.
 * 
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         A <i>std::bad_alloc</i> exception is possible in case of an
 *                              allocation failure.
 */
LgFbufUint8& LgFbufUint8::operator=(const LgFbufUint8& fbuf)
{
   if (this != &fbuf)  //Guard against self-assignment.
   {
      m_state = fbuf.m_state;
      m_errs = fbuf.m_errs;
      m_fname = fbuf.m_fname;
      m_buf = fbuf.m_buf;
   }

   return *this;
}


/*!
 * \brief                       Move constructor.  Constructs an object from an existing constructed
 *                              object that is end-of-life.
 * 
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
LgFbufUint8::LgFbufUint8(LgFbufUint8&& fbuf) noexcept : m_state{fbuf.m_state},
                                                        m_errs{fbuf.m_errs},
                                                        m_fname{std::move(fbuf.m_fname)},
                                                        m_buf{std::move(fbuf.m_buf)}
{
   //std::move causes move constructor to be called for buffer and std::string.
}


/*!
 * \brief                       Move assignment operator.  Assigns to a constructed object from a
 *                              constructed object that is end-of-life.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
 LgFbufUint8& LgFbufUint8::operator=(LgFbufUint8&& fbuf) noexcept
{
   if (this != &fbuf)  //Guard against self-assignment.
   {
      m_state = fbuf.m_state;
      m_errs = fbuf.m_errs;
      m_fname = std::move(fbuf.m_fname);
      m_buf = std::move(fbuf.m_buf);
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
 *       <li>A buffer in the state <i>LgFbufUint8State::Error</i> is before all others in
 *           the sort order, and all such buffers are tied.</li>
 *       <li>A buffer in the state <i>LgFbufUint8State::Unocccupied</i> is next in the
 *           sort order, and all such buffers are tied.</li>
 *       <li>Buffers in the state <i>LgFbufUint8State::Error</i> are sorted in the order
 *           of the <i>LgBufUint8/i> buffer (with rules defined in that class).</li>
 *    </ol>
 *
 * \param[in]  other   File buffer to compare against <i>*this</i>.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
int LgFbufUint8::Cmp(const LgFbufUint8& other) const noexcept
{
   if ((m_state == LgFbufUint8State::Error) && (other.m_state == LgFbufUint8State::Error))
   {
      //Buffers in the error state are tied.
      return 0;
   }
   else if (m_state == LgFbufUint8State::Error)
   {
       //Error is always the lesser.
       return -1;
   }
   else if (other.m_state == LgFbufUint8State::Error)
   {
       //Error is always the lesser.
       return 1;
   }
   else if ((m_state == LgFbufUint8State::Unocccupied) && (other.m_state == LgFbufUint8State::Unocccupied))
   {
      //Buffers in the error state are tied.
      return 0;
   }
   else if (m_state == LgFbufUint8State::Unocccupied)
   {
       //Unoccupied is always the lesser, except for Error.
       return -1;
   }
   else if (other.m_state == LgFbufUint8State::Unocccupied)
   {
       //Unoccupied is always the lesser, except for Error.
       return 1;
   }
   else if ((m_state == LgFbufUint8State::Occupied) && (other.m_state == LgFbufUint8State::Occupied))
   {
       //Fall back on the relative values of the buffers.
       return m_buf.Cmp(other.m_buf);
   }
   else
   {
      //Something about the states are illegal.  Return 0.
      return 0;
   }
}


/*!
 * \brief                       Overload of the less than ('<') operator for the <i>LgFbufUint8</i> class.
 *                              Returns <i>true</i> if <i>*this</i> is less than <i>other</i>, or
 *                              <i>false</i> otherwise.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
*/
bool LgFbufUint8::operator<(const LgFbufUint8& other) const noexcept
{
   return (Cmp(other) < 0);
}


/*!
 * \brief                       Overload of the less than or equal to ('<=') operator for the 
 *                              <i>LgFbufUint8</i> class.  Returns <i>true</i> if <i>*this</i> is
 *                              less than or equal to <i>other</i>, or <i>false</i> otherwise.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
bool LgFbufUint8::operator<=(const LgFbufUint8& other) const noexcept
{
   return (Cmp(other) <= 0);
}


/*!
 * \brief                       Overload of the equality test ('==') operator for the
 *                              <i>LgFbufUint8</i> class.  Returns <i>true</i> if <i>*this</i> is
 *                              equal to <i>other</i>, or <i>false</i> otherwise.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
bool LgFbufUint8::operator==(const LgFbufUint8& other) const noexcept
{
   return (Cmp(other) == 0);
}


/*!
 * \brief                       Overload of the inequality test ('!=') operator for the
 *                              <i>LgFbufUint8</i> class.  Returns <i>true</i> if <i>*this</i> is
 *                              unequal to <i>other</i>, or <i>false</i> otherwise.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
bool LgFbufUint8::operator!=(const LgFbufUint8& other) const noexcept
{
   return (Cmp(other) != 0);
}


/*!
 * \brief                       Overload of the greater than or equal to ('>=') operator for the
 *                              <i>LgFbufUint8</i> class.  Returns <i>true</i> if <i>*this</i> is
 *                              greater than or equal to <i>other</i>, or <i>false</i> otherwise.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
bool LgFbufUint8::operator>=(const LgFbufUint8& other) const noexcept
{
   return (Cmp(other) >= 0);
}


/*!
 * \brief                       Overload of the greater than ('>') operator for the
 *                              <i>LgFbufUint8</i> class.  Returns <i>true</i> if <i>*this</i> is
 *                              greater than <i>other</i>, or <i>false</i> otherwise.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
bool LgFbufUint8::operator>(const LgFbufUint8& other) const noexcept
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
unsigned LgFbufUint8::ErrsGet() const noexcept
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
void LgFbufUint8::ErrsClear() noexcept
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
bool LgFbufUint8::Fread(const std::string& fname)
{
   std::ifstream infile;
   size_t file_size;

   //Clear the errors before starting the operation.
   m_state = LgFbufUint8State::Unocccupied;
   m_errs  = 0;

   //Record the filename.
   m_fname = fname;

   infile.open(fname, std::ios::binary);
   //Discovered experimentally that need to open as binary or line endings get translated,
   //and perhaps other changes made as well.

   if (!infile.is_open())
   {
      //Something has gone wrong.  Error out.
      m_state  = LgFbufUint8State::Error;
      m_errs  |= LG_CPP_DD_FBUF_UINT8_ERR_FOPEN;

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

      m_buf.Clear();  //Set the buffer to zero length.

      return true;
   }

   //If the file size is non-zero, adjust the allocation appropriately.
   m_buf.AllocateAtLeast(file_size);
   m_buf.SetNelem(file_size);
   infile.read((char*)(m_buf.GetBufPtr()), file_size);

   //Check for read errors.  If one occurred, error out.
   if (infile.fail())
   {
      //Error reading stream.
      m_state  = LgFbufUint8State::Error;
      m_errs  |= LG_CPP_DD_FBUF_UINT8_ERR_FREAD;
      infile.close();
      return false;
   }

   infile.close();

   //Check for file close errors.  If one occurred, error out.
   if (infile.fail())
   {
      //Error closing stream.
      m_state  = LgFbufUint8State::Error;
      m_errs  |= LG_CPP_DD_FBUF_UINT8_ERR_FCLOSE;
      return false;
   }

   m_state = LgFbufUint8State::Occupied;

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
bool LgFbufUint8::Fwrite(const std::string &fname)
{
    std::ofstream outfile;

    //Clear the errors before starting the operation.
    m_state = LgFbufUint8State::Unocccupied;
    m_errs  = 0;

    //Record the filename.
    m_fname = fname;

    outfile.open(fname, std::ios::binary);
    //Discovered experimentally that need to open as binary to prevent translations.

    if (!outfile.is_open())
    {
        //Something has gone wrong.  Error out.
        m_state  = LgFbufUint8State::Error;
        m_errs  |= LG_CPP_DD_FBUF_UINT8_ERR_FOPEN;

        //Try to close, unsure if necessary.
        outfile.close();

        //Return as-is.
        return false;
    }

    //Write the file.  We can write the entire file all at once.
    //Since this is a member function, we know that the representation
    //is compatible with write.
    if (m_buf.Nelem() > 0)
    {
        outfile.write((char *)(m_buf.GetBufPtr()), m_buf.Nelem());

        //Check for write errors.  If one occurred, error out.
        if (outfile.fail())
        {
            //Error reading stream.
            m_state  = LgFbufUint8State::Error;
            m_errs  |= LG_CPP_DD_FBUF_UINT8_ERR_FWRITE;
            outfile.close();
            return false;
        }
    }

    outfile.close();

    //Check for file close errors.  If one occurred, error out.
    if (outfile.fail())
    {
        //Error closing stream.
        m_state  = LgFbufUint8State::Error;
        m_errs  |= LG_CPP_DD_FBUF_UINT8_ERR_FCLOSE;
        return false;
    }

    m_state = LgFbufUint8State::Occupied;

    return true;
}


/*!
 * \brief                        Sets a buffer to zero length, clears errors, sets state to unoccupied,
 *                               but does not deallocate any allocated memory in the buffer.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 * 
 * \errorsandexceptions          An exception may be possible with std::string.  This is unknown.
 */
void LgFbufUint8::Clear()
{
   m_state = LgFbufUint8State::Unocccupied;
   m_errs  = 0;
   m_fname = ""; //Exception may be possible.  This is unknown.
   m_buf.Clear();
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
void LgFbufUint8::Trim()
{
   m_buf.Trim();
}


/*!
 * \brief                        Sets buffer to empty, deallocates memory, and clears errors.  Largely equivlaent
 *                               to destruction.  The object is still logically valid and can still be used freely.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          No exceptions or errors are possible.
 */
void LgFbufUint8::ClearAndDeallocate() noexcept
{
    m_state = LgFbufUint8State::Unocccupied;
    m_errs  = 0;
    m_fname = ""; //Exception may be possible.  This is unknown.
    m_buf.ClearAndDeallocate();
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
uint8_t LgFbufUint8::ReadOneByVal(const size_t read_pt) const noexcept
{
   return m_buf.ReadOneByVal(read_pt);
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
void LgFbufUint8::ReadOne(const size_t read_pt, uint8_t* const read_dst) const noexcept
{
   m_buf.ReadOne(read_pt, read_dst);
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
size_t LgFbufUint8::Read(const size_t read_pt, uint8_t* const read_dst, const size_t read_n) const noexcept
{
   return m_buf.Read(read_pt, read_dst, read_n);
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
void LgFbufUint8::AssignOne(const size_t assign_pt, const uint8_t assign_val) noexcept
{
   m_buf.AssignOne(assign_pt, assign_val);
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
void LgFbufUint8::Assign(const size_t assign_pt, const uint8_t* const assign_src, const size_t assign_n)
{
   m_buf.Assign(assign_pt, assign_src, assign_n);
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
void LgFbufUint8::AppendOne(const uint8_t appendee)
{
   m_buf.AppendOne(appendee);
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
void LgFbufUint8::Append(const uint8_t* const append_src, const size_t append_n)
{
   m_buf.Append(append_src, append_n);
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
void LgFbufUint8::InsertOne(const size_t insert_pt, const uint8_t insertee)
{
    m_buf.InsertOne(insert_pt, insertee);
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
void LgFbufUint8::Insert(size_t insert_pt, const uint8_t* const insert_src, const size_t insert_n)
{
   m_buf.Insert(insert_pt, insert_src, insert_n);
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
void LgFbufUint8::DeleteOne(const size_t delete_pt) noexcept
{
   m_buf.DeleteOne(delete_pt);
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
void LgFbufUint8::Delete(const size_t delete_pt, const size_t delete_n)
{
   m_buf.Delete(delete_pt, delete_n);
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
uint8_t LgFbufUint8::ReadAndDeleteOne(const size_t read_delete_pt)
{
   return m_buf.ReadAndDeleteOne(read_delete_pt);
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
void LgFbufUint8::ReadAndDelete(const size_t read_delete_pt, uint8_t* const read_delete_dst, const size_t read_delete_n)
{
   m_buf.ReadAndDelete(read_delete_pt, read_delete_dst, read_delete_n);
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
void LgFbufUint8::DebugWrite(std::ostream& outstream, const std::string& title, const bool include_ending_hbar) const
{
   outstream << "Debug write of : " << title << "\n";
   outstream << "m_fname        : " << "\"" << m_fname << "\"\n";

   m_buf.DebugWrite(outstream, title, include_ending_hbar);
}

//End of lg_cpp_dd_fbuf_uint8.cpp
