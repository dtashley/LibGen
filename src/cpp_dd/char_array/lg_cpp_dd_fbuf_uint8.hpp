//--------------------------------------------------------------------------------------------------
//Header for unsigned 8-bit integer file buffer, RAM-based.
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
#ifndef LG_CPP_DD_FBUF_UINT8_HPP_INCLUDED
#define LG_CPP_DD_FBUF_UINT8_HPP_INCLUDED

#include <cstdint>
#include <string>

#include "lg_cpp_dd_buf_uint8.hpp"

//Errors that can be stored.  Multiple errors are or'd together.
#define LG_CPP_DD_FBUF_UINT8_ERR_FINFO    (0x01U)
#define LG_CPP_DD_FBUF_UINT8_ERR_FOPEN    (0x02U)
#define LG_CPP_DD_FBUF_UINT8_ERR_FREAD    (0x04U)
#define LG_CPP_DD_FBUF_UINT8_ERR_FWRITE   (0x08U)
#define LG_CPP_DD_FBUF_UINT8_ERR_FCLOSE   (0x10U)
#define LG_CPP_DD_FBUF_UINT8_ERR_ACCESS   (0x20U)
#define LG_CPP_DD_FBUF_UINT8_ERR_INTERNAL (0x40U)

//! Enumeration to describe the state of a <i>LgFbufUint8</i>
enum class LgFbufUint8State
{
   Unocccupied, //!< Buffer empty, not bound to file.
   Occupied,    //!< File has been read, bound to file.
   Error,       //!< Error has occurred, awaiting recovery.
};

//!< \class LgFbufUint8
//!< Provides a file buffer of <i>uint8_t</i>.  Typically used for buffering files and
//!< maninpulating the content.  Content buffered may be binary or text--no assumptions
//!< are made about content.
//!< 
//!< The <i>malloc()</i> family of functions is used under the hood.  This is allowed
//!< by the C++ standards, so long as families of functions are not mixed for the
//!< same allocated blocks of memory.  The reason for using the <i>malloc()</i>
//!< family is the need to reallocate for efficiency (C++ <i>new</i> and
//!< <i>delete</i> do not support reallocation).
//!<
//!< No easy way could be found to keep a file open between the Fread()
//!< and the Fwrite(). For now accept the typical behavior of opening a file,
//!< reading it, closing it, opening it, writing it, then closing it.
class LgFbufUint8
{
   #ifndef UT_GTEST
   private:
   #else
   public:
   #endif
      enum class LgFbufUint8State m_state;
         //!< Major state of the file buffer.
      unsigned m_errs;
         //!< Cumulative errors encountered and that should be reported to the client, if polled,
         //!< maintained as a bitmask of individual errors that have occurred.  If errors exist,
         //!< the member functions will still try to operate as fully as possible.
         //!< Any error involving memory exhaustion will be generate an exception from a lower
         //!< layer that percolates up out of this class, and not be recorded here.
      std::string m_fname;
         //!< Filename from which the buffer contents were read, and to which the buffer contents
         //!< will be written by default if the choice is not overridden.  May be changed by the
         //!< user on demand.
      LgBufUint8 m_buf;
         //!< Buffer in RAM containing the bytes of the file.
public:
      //Fundamental constructors/destructors
      LgFbufUint8() noexcept;
      ~LgFbufUint8() noexcept;
      LgFbufUint8(const LgFbufUint8 &fbuf);
      LgFbufUint8& operator=(const LgFbufUint8& buf);
      LgFbufUint8(LgFbufUint8&& buf) noexcept;
      LgFbufUint8& operator=(LgFbufUint8&& buf) noexcept;

      //Overloaded constructors
      //
      //Comparisons
      int Cmp(const LgFbufUint8& other) const noexcept;
      bool operator<(const LgFbufUint8& other) const noexcept;
      bool operator<=(const LgFbufUint8& other) const noexcept;
      bool operator==(const LgFbufUint8& other) const noexcept;
      bool operator!=(const LgFbufUint8& other) const noexcept;
      bool operator>=(const LgFbufUint8& other) const noexcept;
      bool operator>(const LgFbufUint8& other) const noexcept;

      //Optional member functions
      unsigned ErrsGet() const noexcept;
      void ErrsClear() noexcept;
      bool Fread(const std::string& fname);
      bool Fwrite(const std::string& fname);
      void Clear() noexcept;
      void Trim();
      void ClearAndDeallocate() noexcept;
      /*!
       * \brief Returns the number of elements used.  Implemented inline for performance,
       *        as it could appear as a loop limit.
       */
      size_t Nelem() const noexcept
      {
         return m_buf.Nelem();
      }
      /*!
       * \brief Returns the filename string.  Implemented inline for performance.
       */
      std::string GetFilename(void) const
      {
          return m_fname;
      }
      /*! \brief Subscript operator overload for write operations.  Must not be
       *         used beyond <i>m_n_used</i> - 1, or the ordinary memory access
       *         problems will occur.  Inlined for performance.
       */
      uint8_t& operator[] (size_t index) noexcept
      {
         return m_buf.m_bufptr[index];
      }
      /*! \brief Subscript operator overload for read operations.  Must not be
       *         used beyond <i>m_n_used</i> - 1, or the ordinary memory access
       *         problems will occur.  Inlined for performance.
       */
      const uint8_t& operator[] (size_t index) const noexcept
      {
         return m_buf.m_bufptr[index];
      }
      uint8_t ReadOneByVal(const size_t read_pt) const noexcept;
      void ReadOne(const size_t read_pt, uint8_t* const read_dst) const noexcept;
      size_t Read(const size_t read_pt, uint8_t* const read_dst, const size_t read_n) const noexcept;
      void AssignOne(const size_t assign_pt, const uint8_t assign_val) noexcept;
      void Assign(const size_t assign_pt, const uint8_t* const assign_src, const size_t assign_n);
      void AppendOne(const uint8_t appendee);
      void Append(const uint8_t* const append_src, const size_t append_n);
      void InsertOne(const size_t insert_pt, const uint8_t insertee);
      void Insert(size_t insert_pt, const uint8_t* const insert_src, const size_t insert_n);
      void DeleteOne(const size_t delete_pt) noexcept;
      void Delete(const size_t delete_pt, const size_t delete_n);
      uint8_t ReadAndDeleteOne(const size_t read_delete_pt);
      void ReadAndDelete(const size_t read_delete_pt, uint8_t* const read_delete_dst, const size_t read_delete_n);
      void DebugWrite(std::ostream& outstream, const std::string& title, const bool include_ending_hbar = false) const;
};

#endif

//End of lg_cpp_dd_fbuf_uint8.hpp.
