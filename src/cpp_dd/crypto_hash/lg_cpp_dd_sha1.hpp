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
      bool IsFinalized(void) const noexcept;
      void Clear(void) noexcept;
      void ExtractToCstring(char *in_pstr, bool in_uc = false) const noexcept;
      void ExtractToByteArray(uint8_t* in_pbarray) const noexcept;
      void ExtractToUint32Array(uint32_t* in_pu32array) const noexcept;
};

#endif
