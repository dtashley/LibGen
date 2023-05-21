//--------------------------------------------------------------------------------------------------
//C++ formatted console I/O functions.
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
#define MODULE_LG_CPP_CM_FIO

#include <iostream>

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "lg_cpp_cm_fio.hpp"

#define LG_CPP_CM_FIO_HORIZONTAL_BAR_SEP_CHAR    ('-')
   //Horizontal character used for separation.


/*!
 * Emits a character to a stream a specified number of times.
 *
 * \param[in]  in_output_stream  The stream to which to output the characters.  The stream
 *                               must be open and able to accept written characters.
 * \param[in]  in_c              The character to output repeatedly.
 * \param[in]  in_n              The number of times to emit the character.  0 is a permitted value
 *                               and results in no output.
 * \returns                      Nothing.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe, so long as either the stream
 *                               isn't accessed by multiple threads, or interleaving of output to the
 *                               stream is acceptable.
 * \errorsandexceptions          Exceptions are uncaught.  An error writing to the stream will result
 *                               in an uncaught exception.
 */
void LgCppCm_FioRepcharStream(std::ostream &in_output_stream, const char in_c, const size_t in_n)
{
   size_t n = in_n;
 
   while (n)
   {
      in_output_stream << in_c;
      --n;
   }
}


/*!
 * Emits a character to <i>std::cout</i> a specified number of times.
 *
 * \param[in]  in_c              The character to output repeatedly.
 * \param[in]  in_n              The number of times to emit the character.  0 is a permitted value
 *                               and results in no output.
 * \returns                      Nothing.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe, so long as either <i>std::cout</i>
 *                               isn't accessed by multiple threads, or interleaving of output to <i>std::cout</i>
 *                               is acceptable.
 * \errorsandexceptions          Exceptions are uncaught.  An error writing to the <i>std::cout</i> will result
 *                               in an uncaught exception.
 */
void LgCppCm_FioRepchar(const char in_c, size_t in_n)
{
   size_t n = in_n;

   while (n)
   {
      std::cout << in_c;
      --n;
   }
}


/*!
 * Emits a horizontal line of the specified length and a newline sequence to the specified stream.
 *
 * \param[in]  in_output_stream  The stream to which to output the horizontal line and newline
 *                               sequence.  The stream must be open and able to accept written characters.
 * \param[in]  in_line_len       The number of characters in the horizontal line.  0 is a permitted value
 *                               and results in no line output (however, the newline sequence will still
 *                               be output).
 * \returns                      Nothing.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe, so long as either the stream
 *                               isn't accessed by multiple threads, or interleaving of output to the
 *                               stream is acceptable.
 * \errorsandexceptions          Exceptions are uncaught.  An error writing to the stream will result
 *                               in an uncaught exception.
 */
void LgCppCm_FioHlineStream(std::ostream& in_output_stream, const size_t in_line_len)
{
   LgCppCm_FioRepcharStream(in_output_stream,
                             LG_CPP_CM_FIO_HORIZONTAL_BAR_SEP_CHAR,
                             in_line_len);
   in_output_stream << "\n";
}


/*!
 * Emits a horizontal line of the specified length and a newline sequence to <i>std::cout</i>.
 *
 * \param[in]  in_line_len       The number of characters in the horizontal line.  0 is a permitted value
 *                               and results in no line output (however, the newline sequence will still
 *                               be output).
 * \returns                      Nothing.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe, so long as either <i>std::cout</i>
 *                               isn't accessed by multiple threads, or interleaving of output to <i>std::cout</i>
 *                               is acceptable.
 * \errorsandexceptions          Exceptions are uncaught.  An error writing to the stream will result
 *                               in an uncaught exception.
 */
void LgCppCm_FioHline(const size_t in_line_len)
{
   LgCppCm_FioHlineStream(std::cout, in_line_len);
}


/*!
 * Emits a multi-line banner heading, terminated with a newline sequence, to the specified stream.
 *
 * \param[in]  in_output_stream  The stream to which the multi-line banner heading and terminating newline
 *                               sequence should be written.  The stream must be open and able to accept
 *                               written characters.
 * \param[in]  in_s              Pointer to a 0-terminated C string specifying the banner title, which will
 *                               be centered in the middle line of the banner heading.  This pointer may not
 *                               be <i>nullptr</i> or otherwise invalid.  The string length may be 0.
 * \param[in]  in_line_len       The number of characters per line in the banner heading banner heading to
 *                               be generated. <i>in_line_len</i> values which are smaller than the required
 *                               number of characters required per line may result in lines longer than this
 *                               value, as well as lines of inconsistent length.
 * \param[in]  in_n_extra_lines  The number of lines that will be emitted before and emitted after the line
 *                               containing the banner text.  This value may be 0, in which case no extra
 *                               lines will be emitted.
 * \returns                      Nothing.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe, so long as either the stream
 *                               isn't accessed by multiple threads, or interleaving of output to the
 *                               stream is acceptable.
 * \errorsandexceptions          Exceptions are uncaught.  An error writing to the stream will result
 *                               in an uncaught exception.
 */
void LgCppCm_FioBannerheadingStream(std::ostream& in_output_stream,
                                    const char *  const in_s,
                                    const size_t  in_line_len,
                                    const size_t  in_n_extra_lines)
{
   const size_t length_limit = 2047;
      //The maximum length that can be processed, to prevent arithmetic errors.
   const size_t lr_padding = 4;
      //The number of spaces on each side of what is printed.
   size_t i;
      //General iteration variable.
   size_t line_len;
      //Caller's in_line_len, trimmed to prevent numerical errors.
   size_t input_arg_len;
      //Length of the banner heading provided by the caller.  May
      //be clipped to prevent logical errors.
   size_t total_padding_chars, total_padding_chars_left, total_padding_chars_right;
      //The total number of characters available for padding, and the
      //number that will be used on the left, and on the right.  This
      //includes asterisks, and spaces.
   size_t n_left_asterisks, n_right_asterisks;
      //The number of asterisks that will be printed on the left and right, respectively.

   //Check the file pointer, string pointer, and other parameters.
   assert(in_s != NULL);

   //Figure out how many asterisks to print on each side of the
   //argument, and how many spaces.  We also need to figure out
   //how many characters of the input argument to print--if there
   //are too many characters, we need to truncate.

   //Figure out the length of the string passed by the caller.
   input_arg_len = strlen(in_s);

   //Trim input_arg_len to prevent arithmetic errors.
   if (input_arg_len > length_limit)
      input_arg_len = length_limit;

   //Trim in_line_len and transfer it to line_len.
   if (in_line_len > length_limit)
      line_len = length_limit;
   else
      line_len = in_line_len;
 
   //Figure out the number of characters available for padding on the left
   //and right.  If the total number of characters available is odd, we want
   //fewer characters on the right.
   if (line_len > input_arg_len)
      total_padding_chars = line_len - input_arg_len;
   else
      total_padding_chars = 0;

   if ((total_padding_chars % 2) == 0)
   {
      //Even.
      total_padding_chars_left = total_padding_chars_right = total_padding_chars / 2;
   }
   else
   {
      //Odd.
      total_padding_chars_left  = total_padding_chars / 2;
      total_padding_chars_right = total_padding_chars_left + 1;
   }

   //Figure out the number of asterisks on the left.
   if (total_padding_chars_left > lr_padding)
      n_left_asterisks = total_padding_chars_left - lr_padding;
   else
      n_left_asterisks = 0;

   //Figure out the number of asterisks on the right.
   if (total_padding_chars_right > lr_padding)
      n_right_asterisks = total_padding_chars_right - lr_padding;
   else
      n_right_asterisks = 0;

   // Print the correct number of solid lines of asterisks to the
   // standard output.
   for (i = 0; i < in_n_extra_lines; i++)
   {
      LgCppCm_FioRepcharStream(in_output_stream, '*', line_len);
      in_output_stream << "\n";
   }

   //Left asterisks of banner.
   LgCppCm_FioRepcharStream(in_output_stream, '*', n_left_asterisks);

   //Left spaces of banner.
   LgCppCm_FioRepcharStream(in_output_stream, ' ', lr_padding);

   //The banner text itself.
   for (i = 0; i < input_arg_len; i++)
   {
      in_output_stream << in_s[i];
   }

   //Right spaces of banner.
   LgCppCm_FioRepcharStream(in_output_stream, ' ', lr_padding);

   //Right asterisks of banner.
   LgCppCm_FioRepcharStream(in_output_stream, '*', n_right_asterisks);

   //Terminating newline.
   in_output_stream << "\n";

   /* Print the right number of solid lines of asterisks to the
   ** standard output.
   */
   for (i = 0; i < in_n_extra_lines; i++)
   {
      LgCppCm_FioRepcharStream(in_output_stream, '*', line_len);
      in_output_stream << "\n";
   }
}


/*!
 * Emits a multi-line banner heading, terminated with a newline sequence, to <i>std::cout</i>.
 *
 * \param[in]  in_s              Pointer to a 0-terminated C string specifying the banner title, which will
 *                               be centered in the middle line of the banner heading.  This pointer may not
 *                               be <i>nullptr</i> or otherwise invalid.  The string length may be 0.
 * \param[in]  in_line_len       The number of characters per line in the banner heading banner heading to
 *                               be generated. <i>in_line_len</i> values which are smaller than the required
 *                               number of characters required per line may result in lines longer than this
 *                               value, as well as lines of inconsistent length.
 * \param[in]  in_n_extra_lines  The number of lines that will be emitted before and emitted after the line
 *                               containing the banner text.  This value may be 0, in which case no extra
 *                               lines will be emitted.
 * \returns                      Nothing.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe, so long as either <i>std::cout</i>
 *                               isn't accessed by multiple threads, or interleaving of output to <i>std::cout</i>
 *                               is acceptable.
 * \errorsandexceptions          Exceptions are uncaught.  An error writing to the stream will result
 *                               in an uncaught exception.
 */
void LgCppCm_FioBannerheading(const char* const in_s, const size_t in_line_len, const size_t in_n_extra_lines)
{
   assert(in_s != nullptr);

   LgCppCm_FioBannerheadingStream(std::cout, in_s, in_line_len, in_n_extra_lines);
}


/*!
 * Writes a time and date to a stream, in a form similar to "22:57:24 Wed Mar 20 2024".  The output
 * will consist of exactly 24 characters, and no newline sequences will be output.
 *
 * \param[in]       in_output_stream  The stream to which the time (24 characters) should be written.
 *                                    The stream must be open and able to accept written characters.
 * \param[in, out]  in_ltime          Pointer to the time structure maintained by the caller, or nullptr if the
 *                                    caller is not providing a time structure.  This parameter points to what is
 *                                    potentially both an input and an output.  If this parameter is nullptr,
 *                                    regardless of the value of <i>in_acquire_time</i>, this function will acquire
 *                                    the time and use it for what is output.  If this pointer is non-nullptr
 *                                    and <i>in_acquire_time</i> is false, the time provided by the caller will
 *                                    be used as the basis for the printed time, and the caller's variable will
 *                                    not be modified (in this case, this variable is only an input).
 *                                    If this pointer is non-nullptr and <i>in_acquire_time</i> is true,
 *                                    the time will be acquired, used for what is output, and assigned to the
 *                                    caller's variable (in this case, this variable is only an output).
 * \param[in]       in_acquire_time   Whether to interact with the operating system to acquire the time.  Described
 *                                    above.
 * \returns                           Nothing.
 * \reentrancyandthreadsafety         This function is re-entrant and thread safe, so long as either the stream
 *                                    isn't accessed by multiple threads, or interleaving of output to the
 *                                    stream is acceptable.
 * \errorsandexceptions               Exceptions are uncaught.  An error writing to the stream will result
 *                                    in an uncaught exception.
 */
void LgCppCm_TimeStream(std::ostream& in_output_stream, time_t* const in_ltime, const bool in_acquire_time)
{
   char* p;
   time_t local_ltime;
   time_t* p_local_time = nullptr;

   //Set the data structure pointer.  If the incoming pointer is nullptr,
   //we have no choice except to use the local memory.
   if (!in_ltime)
   {
      p_local_time = &local_ltime;
   }
   else
   {
      p_local_time = in_ltime;
   }

   //Acquire the time if appropriate.  We acquire it under two conditions:
   //   a)The incoming pointer is nullptr.  In this case, we have to initialize
   //     the local variable.
   //   b)The acquire_time input is true.  In this case, the caller owns the
   //     memory, but we are being requested to set it to the correct time.
   if ((!in_ltime) || (in_acquire_time))
      time(p_local_time);

   //Convert the time to a string and get a pointer to the library's static data.
   //TO INVESTIGATE:  behavior of the multi-threaded library.
   p = ctime(p_local_time);

   if (p)
   {
      //p is not nullptr.  Can use it.
      size_t i;

      //Print the time, HH:MM:SS, 8 characters.
      for (i = 11; i < 19; i++)
      {
         in_output_stream << p[i];
      }

      //Separating space.
      in_output_stream << " ";

      //Print the date, ddd mmm dd, 10 characters.
      for (i = 0; i < 10; i++)
      {
         in_output_stream << p[i];
      }

      //Separating space.
      in_output_stream << " ";

      //Print the year, YYYY, 4 characters.
      for (i = 20; i < 24; i++)
      {
         in_output_stream << p[i];
      }
   }
   else
      {
         //p is nullptr.  Can't use it.
         in_output_stream << "??:??:?? ??? ??? ?? ????";
      }
   }


/*!
 * Writes a time and date to <i>std::cout</i>, in a form similar to "22:57:24 Wed Mar 20 2024".  The output
 * will consist of exactly 24 characters, and no newline sequences will be output.
 *
 * \param[in, out]  in_ltime          Pointer to the time structure maintained by the caller, or nullptr if the
 *                                    caller is not providing a time structure.  This parameter points to what is
 *                                    potentially both an input and an output.  If this parameter is nullptr,
 *                                    regardless of the value of <i>in_acquire_time</i>, this function will acquire
 *                                    the time and use it for what is output.  If this pointer is non-nullptr
 *                                    and <i>in_acquire_time</i> is false, the time provided by the caller will
 *                                    be used as the basis for the printed time, and the caller's variable will
 *                                    not be modified (in this case, this variable is only an input).
 *                                    If this pointer is non-nullptr and <i>in_acquire_time</i> is true,
 *                                    the time will be acquired, used for what is output, and assigned to the
 *                                    caller's variable (in this case, this variable is only an output).
 * \param[in]       in_acquire_time   Whether to interact with the operating system to acquire the time.  Described
 *                                    above.
 * \returns                           Nothing.
 * \reentrancyandthreadsafety         This function is re-entrant and thread safe, so long as either the stream
 *                                    isn't accessed by multiple threads, or interleaving of output to the
 *                                    stream is acceptable.
 * \errorsandexceptions               Exceptions are uncaught.  An error writing to the stream will result
 *                                    in an uncaught exception.
 */
void LgCppCm_Time(time_t* const in_ltime, const bool in_acquire_time)
{
   LgCppCm_TimeStream(std::cout, in_ltime, in_acquire_time);
}

//End of lg_cpp_cm_fio_cpp.c.
