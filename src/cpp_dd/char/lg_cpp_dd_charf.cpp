//--------------------------------------------------------------------------------------------------
//Character functions.
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
#define MODULE_LG_CPP_DD_CHARF

#include <assert.h>
#include <string.h>

#include "lg_cpp_dd_charf.hpp"


/*!
 * Determines whether an ASCII character is a digit ('0'-'9'); returns <i>true</i> if
 * so or <i>false</i> otherwise.
 *
 * \param[in]  in_digit          The character to test to determine if it is a digit.
 * \returns                      <i>true</i> if the character is a digit, or <i>false</i>
 *                               otherwise.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe.
 * \errorsandexceptions          No exceptions are possible.
 */
bool LgCppDd_CharfIsDigit(const char in_digit) noexcept
{
   switch (in_digit)
   {
      case '0':  
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':  return true;
                 break;
      default:   return false;
                 break;
   }
}


/*!
 * Maps from an ASCII digit to its value.  For example, '3' maps to 3.
 *
 * \param[in]  in_digit          The ASCII digit to convert to its digit value.
 * \returns                      The value of the ASCII digit passed.  If the character
 *                               passed is not an ASCII digit, the value of 10 is returned.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe.
 * \errorsandexceptions          No exceptions are possible.
 */
unsigned LgCppDd_CharfDigitToVal(const char in_digit) noexcept
{
   switch (in_digit)
   {
      case '0':  return 0;
                 break;
      case '1':  return 1;
                 break;
      case '2':  return 2;
                 break;
      case '3':  return 3;
                 break;
      case '4':  return 4;
                 break;
      case '5':  return 5;
                 break;
      case '6':  return 6;
                 break;
      case '7':  return 7;
                 break;
      case '8':  return 8;
                 break;
      case '9':  return 9;
                 break;
      default:   return 10;
                 break;
   }
}


/*!
 * Maps from a nibble (in the range of 0-15) to the ASCII character representing the
 * value ('0'-'9', 'a'-'f').
 *
 * \param[in]  in_nibble         The nibble for which to return the representative ASCII
 *                               character.  Only the least significant 4 bits of the passed
 *                               value are considered.
 * \returns                      ASCII character representing the least significant nibble
 *                               of <i>in_nibble</i>.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe.
 * \errorsandexceptions          No exceptions are possible.
 */
char LgCppDd_CharfNibbleToLcHexDigit(const unsigned in_nibble) noexcept
{
   char rv = '?';  //Default value if nibble doesn't match a value, which
                   //should be impossible.

   switch (in_nibble & 0x0F)
   {
      case  0:  rv = '0';
                break;
      case  1:  rv = '1';
                break;
      case  2:  rv = '2';
                break;
      case  3:  rv = '3';
                break;
      case  4:  rv = '4';
                break;
      case  5:  rv = '5';
                break;
      case  6:  rv = '6';
                break;
      case  7:  rv = '7';
                break;
      case  8:  rv = '8';
                break;
      case  9:  rv = '9';
                break;
      case 10:  rv = 'a';
                break;
      case 11:  rv = 'b';
                break;
      case 12:  rv = 'c';
                break;
      case 13:  rv = 'd';
                break;
      case 14:  rv = 'e';
                break;
      case 15:  rv = 'f';
                break;
      }

   return rv;
}


/*!
 * Converts from an integer to its ASCII hexadecimal form, with the hexadecimal digits
 * in the reverse order.  All hexadecimal digits will be lower-case.
 *
 * \param[in]      in_arg            The integer to convert to reverse hexadecimal form.
 * \param[in,out]  s                 Pointer to the caller's buffer are to receive the hexadecimal
 *                                   digits.  This pointer may not be <i>nullptr</i> or otherwise
 *                                   invalid.  At function exit, <i>*s</i> will be populated with
 *                                   the hexadecimal digits.  The digits will be populated up until
 *                                   the minimum of the number of nibbles in an integer and the
 *                                   number of characters implied by the <i>in_n_chars_avail</i>
 *                                   parameter.  All unused positions in the caller's buffer
 *                                   <i>*s</i> up through <i>s[in_n_chars_avail</i> will be set
 *                                   to '0'.
 * \param[in]      in_n_chars_avail  The number of characters in the caller's buffer <i>*s</i>
 *                                   that may be used.  If this value is 0, the caller's buffer
 *                                   will not be used.
 * \returns                          Nothing.
 * \reentrancyandthreadsafety        This function is re-entrant and thread safe.
 * \errorsandexceptions              No exceptions are possible.
 */
void LgCppDd_CharfIntToLcHexRev(const int in_arg, char* const s, const size_t in_n_chars_avail) noexcept
{
   size_t i;
   int arg;
   size_t n_iterations;
   const size_t nibbles_per_int = sizeof(int) * 2;

   //The pointer may not be null.
   assert(s != nullptr);

   //Copy the input argument.
   arg = in_arg;

   //Figure out how many iterations.  This is the smaller of:
   //  a)The number of nibbles in an integer.
   //  b)The number of characters available in the caller's character array.
   if (nibbles_per_int <= in_n_chars_avail)
      n_iterations = nibbles_per_int;
   else
      n_iterations = in_n_chars_avail;

   //Convert and stuff the hexadecimal digits.
   for (i = 0; i < n_iterations; i++)
   {
      s[i] = LgCppDd_CharfNibbleToLcHexDigit(arg);
      arg >>= 4;
   }

   //Fill any unused positions in the caller's buffer with '0'.
   for ( ; i < in_n_chars_avail; i++)
   {
      s[i] = '0';
   }
}

//End of lg_cpp_dd_charf.cpp.
