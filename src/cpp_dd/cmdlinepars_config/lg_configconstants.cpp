//--------------------------------------------------------------------------------------------------
//Storage of configuration and constants.
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
#define MODULE_LG_CONFIGCONSTANTS

#include <cstdint>

#include "lg_configconstants.hpp"

static volatile uint8_t verbosity_stored = 3;
   //The stored value of verbosity.  The assumption is made that there is no platform
   //that won't be forced to atomically read and write a byte.  However, this is
   //not guaranteed, and it will probably be necessary at a future point to switch
   //to a solution involving mutexes.
   //
   //The default verbosity is 3.
   //
   //Verbosity levels:
   //  0           : No stdout or stderr output.  All results through process exit code and generated files.
   //  1           : Terse error descriptions only, announced on* stderr* only.
   //  2           : Detailed error descriptions, announced on stderr only.
   //  3 (default) : Major steps announced on stdout. Detailed errors announced on stderr.
   //  4           : Major and minor steps announced on stdout.  Detailed errors announced on stderr.
   //  5           : Previous levels' output plus algorithm steps and notes announced on stdout.  Detailed errors announced on stderr.
   //  6           : Previous levels' output plus module/class public function entries and returns announced on stdout.
   //                Detailed errors announced on stderr.
   //  7           : Previous levels' output plus non-public function entries and returns announced on stdout.
   //                Detailed errors announced on stderr.
   //  8           : Previous levels' output plus line execution details announced on stdout. Detailed errors announced on
   //                stderr.
   //  9           : Maximum detail announced on stdout. Detailed errors announced on stderr.
static volatile bool verbosity_set = false;
   // true if verbosity has been set.  It can be set only once.


/*!
 * Returns the internally-stored verbosity level, which will be in the interval
 * [0, 9].  The semantics of the verbosity value are specified in the library's
 * README.md file.
 *
 * \returns                      The internally-stored verbosity level, which
 *                               will be in the interval [0, 9].
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe.
 * \errorsandexceptions          This function cannot generate exceptions.
 */
unsigned LgConfigconstants_VerbosityGet(void) noexcept
{
   volatile uint8_t verbosity_temp_storage;
   volatile uint8_t verbosity_temp_storage_unsigned;

   //Try to get a clean atomic snapshot of the stored verbosity.  The conversion
   //to unsigned may require multiple instructions or read more than once,
   //so try to get a clean atomic snapshot in case that happens.
   verbosity_temp_storage = verbosity_stored;

   //Convert to unsigned.
   verbosity_temp_storage_unsigned = verbosity_temp_storage;

   return verbosity_temp_storage_unsigned;
}


/*!
 * Sets the internally-stored verbosity level to the passed value.  Since verbosity is
 * in the interval [0, 9], any specified value outside this range will be clipped
 * into the range.
 *
 * \param[in]  in_verbosity      Verbosity integer to set.  Since verbosity is in the interval
 *                               [0, 9], any value outside this range will be clipped into the
 *                               range, and the clipped value will be stored.
 * \returns                      Nothing.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe.
 * \errorsandexceptions          This function cannot generate exceptions.
 */
void LgConfigconstants_VerbositySet(unsigned in_verbosity) noexcept
{
    volatile uint8_t verbosity_temp_storage;

    if (!verbosity_set)
    {
       //Clip.
       if (in_verbosity <= 9)
          verbosity_temp_storage = in_verbosity;
       else
          verbosity_temp_storage = 9;

       //Assign to the module-local variable.  I suspect this would be atomic on all
       //platforms, but that may not be a strong enough guarantee, and a mutex
       //may need to be used in the future.
       verbosity_stored = verbosity_temp_storage;

       verbosity_set = true;
    }
}

static volatile size_t linelength_stored = 78;
   //The stored value of the line length.  It must be in the range of 40-1000.  The default is 78.
   //The line length is the maximum number of characters that can be output per line until the \n
   //sequence.
static volatile bool linelength_set = false;
   //true if line length has been set.  It can be set only once.


/*!
 * Returns the internally-stored line length, which will be in the
 * interval [40, 1000].
 *
 * \returns                      The internally-stored line length, which
 *                               will be in the interval [40, 1000].
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe.
 * \errorsandexceptions          This function cannot generate exceptions.
 */
size_t LgConfigconstants_LinelengthGet(void) noexcept
{
    volatile size_t linelength_temp_storage;

    //Try to get a clean atomic snapshot of the stored linelength.
    linelength_temp_storage = linelength_stored;

    return linelength_temp_storage;
}


/*!
 * Sets the internally-stored line length to the passed value.  Since line length
 * must be in the interval [40, 1000], any specified value outside this range will be clipped
 * into the range.
 *
 * \param[in]  in_linelength     Line length to set.  Since verbosity is in the interval
 *                               [40, 1000], any value outside this range will be clipped into the
 *                               range, and the clipped value will be stored.
 * \returns                      Nothing.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe.
 * \errorsandexceptions          This function cannot generate exceptions.
 */
void LgConfigconstants_LinelengthSet(size_t in_linelength) noexcept
{
   volatile size_t linelength_temp_storage;

   if (!linelength_set)
   {
      //Clip as needed.
      if (in_linelength < 40)
         linelength_temp_storage = 40;
      else if (in_linelength > 1000)
         linelength_temp_storage = 1000;
      else
         linelength_temp_storage = in_linelength;

      //Assign to the module-local variable.  I suspect this would be atomic on all
      //platforms, but that may not be a strong enough guarantee, and a mutex
      //may need to be used in the future.
      linelength_stored = linelength_temp_storage;

      linelength_set = true;
   }
}

//End of lg_configconstants.cpp
