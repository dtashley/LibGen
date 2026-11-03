//--------------------------------------------------------------------------------------------------
//Header file for SHA1 calculation.
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
#ifndef LG_CPP_DD_SHA1_INCLUDED
#define LG_CPP_DD_SHA1_INCLUDED

#include <cstdint>
#include <string>


//!< \class LgSha1
//!< Facilitate the calculation of the SHA1 hash, as described in NIST FIPS 180-1.
//!< The general paradigm is that the hash state is initialized (often by construction),
//!< data is added to the hash calculation (always in a byte buffer, all at once or
//!< incrementally), the hash is finalized, and then the hash value can be extracted.
class LgSha1
{
   #ifndef UT_GTEST
   private:
   #else
   public:
   #endif
   size_t   m_bufferSize;
      //!< The number of bytes presently in the buffer.  These are being held in order to reach
      //!< finalization or enough bytes added to fill the buffer.
   uint64_t m_bitCount;
      //!< The number of bits in the message whose SHA1 is being calculated, so far.
   bool     m_finalized;
      //!< false if calculation has not been finalized, true if it has.  An SHA1 that has been
      //!< finalized cannot have more data added or be finalized again.
   uint32_t m_state[5];
      //!< The state as defined in FIPS 180-1.
   uint8_t  m_buffer[64];
      //!< The buffer to hold pending bytes to be hashed.
public:
      LgSha1() noexcept;
      ~LgSha1() noexcept;
      void Add(uint8_t const *in_bptr, size_t in_nbytes) noexcept;
      void AddOne(uint8_t in_b) noexcept;
      void Finalize(void) noexcept;
};

#if 0
class LgBufUint8
{
   #ifndef UT_GTEST
   private:
   #else
   public:
   #endif
      size_t m_n_allocd;
         //!< The number of bytes allocated.  If this value is non-zero, <i>m_bufptr</i> must be non-<i>nullptr</i>.
         //!< If this value is zero, <i>m_bufptr</i> must be <i>nullptr</i>.
         //!<
         //!< This value is not required to be a multiple of any specific value.  However, for
         //!< most manipulations, the number of elements allocated is increased and decreased in multiples of
         //!< <i>LG_CPP_DD_BUF_UINT8_ALLOC_INCREMENT</i> (a private constant), for efficiency.
      size_t m_n_used;
         //!< The number of bytes used in <i>m_bufptr[]</i>, starting from element [0].
         //!< Valid indices to access <i>m_bufptr[]</i> range from 0 through <i>m_n_used</i>-1.
      uint8_t *m_bufptr;
         //!< Pointer to the start of the buffer, a one-dimensional array of <i>uint8_t</i>.
         //!< This value be <i>nullptr</i> if <i>m_n_allocd</i> is 0, and not <i>nullptr</i>
         //!< if <i>m_n_allocd</i> is not 0.
   public:
      //Fundamental constructors/destructors
      LgBufUint8() noexcept;
      ~LgBufUint8() noexcept;
      LgBufUint8(const LgBufUint8 &buf);
      LgBufUint8& operator=(const LgBufUint8& buf);
      LgBufUint8(LgBufUint8&& buf) noexcept;
      LgBufUint8& operator=(LgBufUint8&& buf) noexcept;

      //Overloaded constructors
      //
      //Comparisons
      int Cmp(const LgBufUint8& other) const noexcept;
      bool operator<(const LgBufUint8& other) const noexcept;
      bool operator<=(const LgBufUint8& other) const noexcept;
      bool operator==(const LgBufUint8& other) const noexcept;
      bool operator!=(const LgBufUint8& other) const noexcept;
      bool operator>=(const LgBufUint8& other) const noexcept;
      bool operator>(const LgBufUint8& other) const noexcept;

      //Optional member functions
      void AllocateAtLeast(size_t nbytes);
      void SetNelem(size_t nbytes);
      /*!
       * \brief Returns the pointer to the internal buffer.  Used only for file read and
       *        write operations.
       */
      uint8_t *GetBufPtr(void) const noexcept
      {
          return m_bufptr;
      }
      void Clear() noexcept;
      void Trim();
      void ClearAndDeallocate() noexcept;
      /*!
       * \brief Returns the number of elements used.  Implemented inline for performance,
       *        as it could appear as a loop limit.
       */
      size_t Nelem() const noexcept
      {
         return m_n_used;
      }
      /*! \brief Subscript operator overload for write operations.  Must not be
       *         used beyond <i>m_n_used</i> - 1, or the ordinary memory access
       *         problems will occur.  Inlined for performance.
       */
      uint8_t& operator[] (size_t index) noexcept
      {
         return m_bufptr[index];
      }
      /*! \brief Subscript operator overload for read operations.  Must not be
       *         used beyond <i>m_n_used</i> - 1, or the ordinary memory access
       *         problems will occur.  Inlined for performance.
       */
      const uint8_t& operator[] (size_t index) const noexcept
      {
         return m_bufptr[index];
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


//End of lg_cpp_dd_buf_uint8.hpp.

#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>

class Sha1
{
public:
    Sha1() noexcept;

    void Update(std::span<const std::uint8_t> data);
    void Update(std::string_view data);

    [[nodiscard]] std::string Finalize();

    [[nodiscard]] static std::string Hash(std::span<const std::uint8_t> data);
    [[nodiscard]] static std::string Hash(std::string_view data);

private:
    void ProcessBlock(const std::uint8_t* block) noexcept;

    std::uint32_t state_[5];
    std::uint64_t bitCount_;
    std::uint8_t buffer_[64];
    std::size_t bufferSize_;
    bool finalized_;
};
#endif
#endif
