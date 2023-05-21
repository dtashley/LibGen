//--------------------------------------------------------------------------------------------------
//Functions dealing with the size_t data type.
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
#define MODULE_LG_CPP_DD_SIZE_T

#include "lg_cpp_dd_size_t.hpp"


/*!
 * Tests whether a size_t argument is an integral power of 2, and returns true if so,
 * or false otherwise.
 *
 * \param[in]  arg               The size_t value to test for whether it is an integral
 *                               power of 2, having exactly 1 bit set.
 * \returns                      true if arg is an integral power of 2,  necessary to fit
 *                               within the range of a size_t.
 * \reentrancyandthreadsafety    Thread safe.
 * \errorsandexceptions          No exceptions or errors are possible.
 */
bool LgCppDd_SizeT_IsPowerOf2(const size_t arg) noexcept
{
   //The test below for an integer being a power of 2 came from a Google
   //search.
   if (
         (arg != 0)
         &&
         ((arg & (arg - 1)) == 0)
      )
   {
      return true;
   }
   else
   {
      return false;
   }
}

/*!
 * Calculates the smallest multiple of in_increment that is at least as large
 * as in_arg.
 * 
 * This function is most commonly used for dynamic memory allocation decisions
 * when the need can grow and shrink in small increments.
 * 
 * If in_increment is is the granularity of memory allocation, and if in_arg is
 * the amount of memory actually required, the output of this function can be used
 * as a guide for how much memory to allocate so as to lessen the need for
 * reallocation as the amount of memory required changes gradually.
 *
 * in_increment is not required to be an integral power of 2.  (If an integral
 * power of 2 were assumed, optimizations could be made.)
 *
 * \param[in]  in_arg            The number for which the smallest multiple of in_increment
 *                               at least as large as in_arg is to be calculated.
 * \param[in]  in_increment      The multiple.  A value of 0 is interpreted as 1,
 *                               but all other values are used as supplied.
 * \returns                      The smallest multiple of in_increment that is at least as
 *                               large as in_arg.
 * \reentrancyandthreadsafety    Thread safe.
 * \errorsandexceptions          No exceptions or errors are possible.
 */
size_t LgCppDd_SizeT_LeastMulNotSmallerThan(const size_t in_arg, const size_t in_increment) noexcept
{
   size_t in_increment_used;
   size_t rv = 0;

   //Adjust the domain of in_increment so that 0 is treated as 1.
   if (in_increment == 0)
      in_increment_used = 1;
   else
      in_increment_used = in_increment;

   if (in_arg == 0)
   {
      //In this case, the multiple not smaller than is 0.
      rv = 0;
   }
   else if (in_arg <= in_increment_used)
   {
      //This is the single multiple case.  Tested for efficiency to avoid
      //divisions on a very common path.
      rv = in_increment_used;
   }
   else
   {
      size_t size_max_modulo_in_increment_used;
         //The remainder from a division of SIZE_MAX by in_increment_used.
         //This is the amount that SIZE_MAX needs to be adjusted downward
         //to reach the highest legal multiple of in_increment_used.
      size_t upper_limit;
         //This should be the multiple of in_increment_used
         //no larger than SIZE_MAX.
      size_t quotient, modulus;
         //quotient and modulus of in_arg over the increment used.

      size_max_modulo_in_increment_used = SIZE_MAX % in_increment_used;
      upper_limit = SIZE_MAX - size_max_modulo_in_increment_used;

      if (in_arg > upper_limit)
      {
         //We are above the largest multiple of in_increment_used.  SIZE_MAX
         //is the right return value.  The path so far has involved one division.
         rv = SIZE_MAX;
      }
      else
      {
         //This is the most expensive path, which will involve in total two
         //divisions and one multiplication.
         quotient = in_arg / in_increment_used;
         modulus  = in_arg % in_increment_used;
           //Calculate floor(in_arg/in_increment_used) and the modulus.

         //The quotient will need to be bumped up by one if there is a
         //remainder.  But we need to be careful
         //of overflow.  We know that in_arg <= upper_limit.
         //  Case I:  in_arg == upper_limit
         //           Since upper_limit is an integer multiple of
         //           in_increment_used, there won't be a remainder, and
         //           the quotient won't be incremented.  The return value
         //           will be upper_limit.
         //  Case II: in_arg < upper_limit
         //           The quotient will be 1 less than the case above, but
         //           it may be incremented, leading to upper_limt.
         //           No possibility of overflow.
         if (modulus)
            quotient++;

         rv = quotient * in_increment_used;
      }
   }

   return rv;
}


/*!
 * Determines if the addition of 2 size_t operands would overflow, and returns
 * true if so, or false otherwise.
 *
 * \param[in]  arg1              The first argument to evaluate.
 * \param[in]  arg2              The second argument to evaluate.
 * \returns                      true if the addition of the two arguments would overflow,
 *                               or false otherwise.
 * \reentrancyandthreadsafety    Thread safe.
 * \errorsandexceptions          No exceptions or errors are possible.
 */
bool LgCppDd_SizeT_AdditionWouldOverflow(const size_t arg1, const size_t arg2) noexcept
{
   size_t arg1_headroom;
   
   arg1_headroom = SIZE_MAX - arg1;

   if (arg2 > arg1_headroom)
      return true;
   else
      return false;
}


/*!
 * Calculates the maximum value that may be added to a size_t addend without
 * overflow.
 *
 * \param[in]  arg               The argument to evaluate.
 * \returns                      The largest value that may be added to arg without
 *                               overflow.
 * \reentrancyandthreadsafety    Thread safe.
 * \errorsandexceptions          No exceptions or errors are possible.
 */
size_t LgCppDd_SizeT_MaximumAddend(const size_t arg) noexcept
{
   size_t arg_headroom;

   arg_headroom = SIZE_MAX - arg;

   return arg_headroom;
}


/*!
 * Maps from a quantity of type std::streampos to a quantity of type size_t, preventing
 * the simple truncation as would be performed by the C language.  Instead, clipping is
 * used.
 *
 * Clipping should provide reasonably graceful behavior.  If a file is too large to
 * be buffered in, only a fraction of it would be buffered in, and processing would
 * proceed from there.  This is not perfect behavior, but a reasonable failure mode.
 *
 * This is all somewhat contrived, as the numerical limits involved are unlikely to be
 * reached.
 *
 * The C++ standards provide few guarantees about the type std::streampos, except that
 * it can be converted to and from integer.  It typically resembles a structure rather
 * than an integer.
 *
 * \param[in]  arg               The std::streampos quantity to be clipped into the range of
 *                               a size_t.
 * \returns                      The std::streampos quantity, reduced if necessary to fit
 *                               within the range of a size_t.
 * \reentrancyandthreadsafety    Not evaluated.
 * \errorsandexceptions          No exceptions or errors are possible.
 */
size_t LgCppDd_SizeT_StreamposToSizeT(std::streampos arg) noexcept
{
   long long int streampos_rep;
   long long int size_max_rep;

   //Try to get the streampos.  Via overloads and so on, it should be convertible to
   //an integer.
   streampos_rep = arg;

   if (streampos_rep <= 0)
   {
      return 0;
   }
   else
   {
      if (sizeof(size_t) >= sizeof(long long int))
      {
         //size_t should safely hold a positive long long int.  Can just assign.
         return (size_t)streampos_rep;
      }
      else
      {
         //size_t can't safely hold a positive long long int.  However, a long long int
         //can hold SIZE_MAX.
         size_max_rep = (long long int)SIZE_MAX;

         if (streampos_rep > size_max_rep)
         {
            return SIZE_MAX;
         }
         else
         {
            return (size_t)streampos_rep;
         }
      }
   }
}


/*!
 * Maps from a quantity of type size_t to a quantity of type std::streampos, preventing
 * the simple truncation as would be performed by the C language.  Instead, clipping is
 * used.
 *
 * Clipping should provide reasonably graceful behavior.  If a buffer is too large to
 * be written out, only a fraction of it would be written out.  This is not perfect
 * behavior, but a reasonable failure mode.
 *
 * This is all somewhat contrived, as the numerical limits involved are unlikely to be
 * reached.
 *
 * The C++ standards provide few guarantees about the type std::streampos, except that
 * it can be converted to and from integer.  It typically resembles a structure rather
 * than an integer.
 *
 * \param[in]  arg               The size_t quantity to be fit into the range of
 *                               a std::streampos.
 * \returns                      The size_t quantity, reduced to fit within the
 *                               range of a std::streampos.
 * \reentrancyandthreadsafety    Not evaluated.
 * \errorsandexceptions          No exceptions or errors are possible.
 */
std::streampos LgCppDd_SizeT_SizeTToStreampos(size_t arg) noexcept
{
   unsigned long long streampos_max;
   unsigned long long arg_for_comparison;

   //Assign streampos max for comparison.  This should fit, as unsigned long long is the
   //largest integer in the system.
   streampos_max = std::numeric_limits<std::streamsize>::max();

   //Assign arg for comparison.  This should fit, as unsigned long long is the
   //largest integer in the system.
   arg_for_comparison = arg;

   if (arg_for_comparison <= streampos_max)
   {
      //We can safely assign.
      return (std::streampos)arg;
   }
   else
   {
      //We have to clip.
      return (std::streampos)streampos_max;
   }
}

//End of lg_cpp_dd_size_t.cpp
