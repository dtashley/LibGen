//--------------------------------------------------------------------------------------------------
//SHA1 calculation.
// 
//Used to calculate the SHA1 hash, as described in NIST FIPS 180-1.
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
#define MODULE_LG_CPP_DD_SHA1

#include <assert.h>
#include <fstream>
#include <istream>
#include <iostream>
#include <iomanip>
#include <string.h>

#include "lg_cpp_dd_sha1.hpp"

#include "lg_cpp_cm_fio.hpp"
#include "lg_cpp_dd_charf.hpp"
#include "lg_cpp_dd_malloc_alt.hpp"
#include "lg_cpp_dd_size_t.hpp"

/*!
 * \brief                       Private function that performs the SHA1 rounds 
 *                              as specified by FIPS 180-1.  The FIPS 180-1
 *                              algorithm variant requiring more space is used:
 *                              consumption on the stack should
 *                              be approximately 320 bytes.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
static void Sha1ProcessBlock(uint32_t* in_state, uint8_t* in_buffer)
{
   size_t i;
   uint32_t w[80];
   uint32_t a, b, c, d, e, temp;

   //for (i = 0; i < 64; i++)
   //{
   //    printf("%02X ", in_buffer[i]);
   //}
   //printf("\n");

   //Load words 0 through 15 from the buffer.  This comes directly from FIPS 180-1.
   for (i = 0; i < 16; i++)
   {
      w[i] = (((uint32_t)in_buffer[i * 4    ]) << 24) |
             (((uint32_t)in_buffer[i * 4 + 1]) << 16) |
             (((uint32_t)in_buffer[i * 4 + 2]) <<  8) |
             (((uint32_t)in_buffer[i * 4 + 3])      );
   }

   //Calculate words 16 through 79.  This comes directly from FIPS 180-1
   for (i = 16; i < 80; i++)
   {
      temp = w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16];
      w[i] = (temp << 1) | (temp >> 31);
   }

   //Initial conditions pre-loop.
   a = in_state[0];
   b = in_state[1];
   c = in_state[2];
   d = in_state[3];
   e = in_state[4];

   //Main loop.
   for (i = 0; i < 80; i++)
   {
      if (i < 20)
      {
         temp = ((a << 5) | (a >> 27)) + ((b & c) | (~b & d)) + e + w[i] + 0x5A827999;
      }
      else if (i < 40)
      {
         temp = ((a << 5) | (a >> 27)) + (b ^ c ^ d) + e + w[i] + 0x6ED9EBA1;
      }
      else if (i < 60)
      {
         temp = ((a << 5) | (a >> 27)) + ((b & c) | (b & d) | (c & d)) + e + w[i] + 0x8F1BBCDC;
      }
      else
      {
         temp = ((a << 5) | (a >> 27)) + (b ^ c ^ d) + e + w[i] + 0xCA62C1D6;
      }

      e = d;
      d = c;
      c = (b << 30) | (b >> 2);
      b = a;
      a = temp;
   }

   //Add the results back into the state.
   in_state[0] += a;
   in_state[1] += b;
   in_state[2] += c;
   in_state[3] += d;
   in_state[4] += e;
}


/*!
 * \brief                       Default constructor.  Constructs a new object.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
LgSha1::LgSha1() noexcept : m_bufferSize{ 0 },
                            m_bitCount{ 0 },
                            m_finalized{ false },
                            m_state{ 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0 },
                              //Initial state as defined in FIPS 180-1.
                            m_buffer{ 0 }
{
}


/*!
 * \brief                       Destructor.  Destroys an object.
 *
 * \reentrancyandthreadsafety   Not evaluated.
 *
 * \errorsandexceptions         No errors or exceptions are possible.
 */
LgSha1::~LgSha1() noexcept
{
   m_bufferSize = 0;
   m_bitCount = 0;
   m_finalized = false;
   memset(m_state, 0, sizeof(m_state));
   memset(m_buffer, 0, sizeof(m_buffer));
}


/*!
 * \brief Adds in in_nbytes at in_bptr to the SHA1 calculation.
 *
 * \param[in]     in_bptr        Pointer to the input data.  If this pointer is null, the function
 *                               does nothing.
 * \param[in]     in_nbytes      The number of bytes to add.  If in_nbytes is 0, the function the
 *                               function does nothing and will not dereference in_bptr.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
void LgSha1::Add(uint8_t const *in_bptr, size_t in_nbytes) noexcept
{
   if ((!m_finalized) && (in_bptr != nullptr) && (in_nbytes != 0))
   {
      while (in_nbytes)
      {
         while ((in_nbytes > 0) && (m_bufferSize < 64)) 
         {
            m_buffer[m_bufferSize++] = *in_bptr++;
            in_nbytes--;
            m_bitCount += 8;
         }

         if (m_bufferSize >= 64)
         {
            Sha1ProcessBlock(m_state, m_buffer);
            m_bufferSize = 0;
         }
      }
   }
}


/*!
 * \brief Adds one byte to the SHA1 calculation.
 *
 * \param[in]     in_b           Byte to add.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
void LgSha1::AddOne(uint8_t in_b) noexcept
{
   (*this).Add(&in_b, 1);
}


/*!
 * \brief Finalizes the SHA1 calculation. After this function is called, no more data can be added
 *        to the calculation.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
void LgSha1::Finalize(void) noexcept
{
   uint8_t padding_0 = 0x00;
   uint8_t padding_1 = 0x80;
   uint8_t size_in_bits[8];
   uint64_t bit_count_copy = m_bitCount;

   if (!m_finalized)
   {
      //Because this class accepts bytes (rather than bits), the '1' padding can be achieved by
      //adding 0x80 to the buffer.  The rest of the padding is 0x00, which is already in the buffer.
      (*this).Add(&padding_1, 1);

      //Continue to add 0's until there are exactly 56 bytes in the buffer.  This is because the
      //last 8 bytes will be used to store the bit count.
      while (m_bufferSize != 56)
      {
         (*this).Add(&padding_0, 1);
      }

      m_bitCount = bit_count_copy;  //Restore the bit count, because the Add() calls above changed it.

      size_in_bits[0] = (uint8_t)((m_bitCount >> 56) & 0xFF);
      size_in_bits[1] = (uint8_t)((m_bitCount >> 48) & 0xFF);
      size_in_bits[2] = (uint8_t)((m_bitCount >> 40) & 0xFF);
      size_in_bits[3] = (uint8_t)((m_bitCount >> 32) & 0xFF);
      size_in_bits[4] = (uint8_t)((m_bitCount >> 24) & 0xFF);
      size_in_bits[5] = (uint8_t)((m_bitCount >> 16) & 0xFF);
      size_in_bits[6] = (uint8_t)((m_bitCount >> 8)  & 0xFF);
      size_in_bits[7] = (uint8_t)((m_bitCount >> 0)  & 0xFF);
      (*this).Add(size_in_bits, 8);

      //printf("Hash: %08X %08X %08X %08X %08X\n", m_state[0], m_state[1], m_state[2], m_state[3], m_state[4]);

      //Perform finalization logic here
      m_finalized = true;
   }
}


/*!
 * \brief Returns true if the SHA1 calculation has been finalized, false otherwise.
 *        Note #1: If the calculation has not been finalized, the hash value is not
 *                 valid and should not be used.
 *        Note #2: The finalized Boolean does not give complete information about the state
 *                 of the object.  If data has been added to the object, it may still be in
 *                 a non-pristine state with finalized false.  There is no way to determine
 *                 if the object is pristine.  This is not seen as a limitation, because of
 *                 the way these objects are used.
 *
 * \returns        true if the object has been finalized, false otherwise.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 */
bool LgSha1::IsFinalized(void) const noexcept
{
    return m_finalized;
}


/*!
 * \brief Clears an object back to a pristine state.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 */
void LgSha1::Clear(void) noexcept
{
    m_bufferSize = 0;
    m_bitCount   = 0;
    m_finalized  = false;
    m_state[0]   = 0x67452301;
    m_state[1]   = 0xefcdab89;
    m_state[2]   = 0x98badcfe;
    m_state[3]   = 0x10325476;
    m_state[4]   = 0xc3d2e1f0;
    memset(m_buffer, 0, sizeof(m_buffer));
}


/*!
 * \brief Extracts the SHA1 hash to a C string.  The C string will be 40 characters long, plus a
 *        null terminator.
 *
 * \param[in]     in_pstr        Pointer from caller providing area with space for at least
 *                               41 characters (40 characters of SHA1 hash and 1 null C string
 *                               terminator).  If this pointer is null, the function does nothing.
 * \param[in]     in_uc          true if the hexadecimal digits should be upper case, or false
 *                               if they should be lower case.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
void LgSha1::ExtractToCstring(char* in_pstr, bool in_uc) const noexcept
{
   if (in_pstr != nullptr)
   {
      if (m_finalized)
      {
         for (size_t i = 0; i < 5; ++i)
         {
            if (in_uc)
            {
               sprintf(in_pstr + i * 8, "%08X", m_state[i]);
            }
            else
            {
               sprintf(in_pstr + i * 8, "%08x", m_state[i]);
            }
         }
         in_pstr[40] = '\0'; // Null-terminate the string
      }
      else
      {
         // If not finalized, return an empty string.
         in_pstr[0] = '\0';
      }
   }
}


/*!
 * \brief Extracts the SHA1 hash to an array of bytes, with no terminator.  Only bytes
 *        [0] through [19] will be written to the array.
 *
 * \param[in]     in_pbarray     Pointer from caller providing area with space for at least
 *                               20 bytes (4 bytes per SHA1 hash value).  If this pointer is null,
 *                               the function does nothing.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
void LgSha1::ExtractToByteArray(uint8_t* in_pbarray) const noexcept
{
   if (in_pbarray != nullptr)
   {
      if (m_finalized)
      {
         for (size_t i = 0; i < 5; ++i)
         {
            in_pbarray[i * 4    ] = (uint8_t)((m_state[i] >> 24) & 0xFF);
            in_pbarray[i * 4 + 1] = (uint8_t)((m_state[i] >> 16) & 0xFF);
            in_pbarray[i * 4 + 2] = (uint8_t)((m_state[i] >>  8) & 0xFF);
            in_pbarray[i * 4 + 3] = (uint8_t)((m_state[i] >>  0) & 0xFF);
         }
      }
      else
      {
         //If not finalized, return all 0's.  This is a nearly impossible
         //result, and is effectively a sentinel value.
         memset(in_pbarray, 0, 20);
      }
   }
}


/*!
 * \brief Extracts the SHA1 hash to an array of 32-bit unsigned integers, with no terminator.  Only
 *        integers [0] through [4] will be written to the array.
 *
 * \param[in]   in_pu32array     Pointer from caller providing area with space for at least
 *                               5 32-bit (4 bytes per SHA1 hash value).  If this pointer is null,
 *                               the function does nothing.
 *
 * \reentrancyandthreadsafety    Not evaluated.
 *
 * \errorsandexceptions          Out of memory is a possible exception.
 */
void LgSha1::ExtractToUint32Array(uint32_t* in_pu32array) const noexcept
{
   if (in_pu32array != nullptr)
   {
      if (m_finalized)
      {
         for (size_t i = 0; i < 5; ++i)
         {
            in_pu32array[i] = m_state[i];
         }
      }
      else
      {
         //If not finalized, return all 0's.  This is a nearly impossible
         //result, and is effectively a sentinel value.
         memset(in_pu32array, 0, sizeof(uint32_t) * 5);
      }
   }
}
