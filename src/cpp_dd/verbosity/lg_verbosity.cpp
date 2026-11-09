//--------------------------------------------------------------------------------------------------
//Verbosity-related functions.
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
#define MODULE_LG_VERBOSITY

#include <cstdint>

#include "lg_verbosity.hpp"

static volatile uint8_t verbosity_stored = 3;
   //The stored value of verbosity.  The assumption is made that there is no platform
   //that won't be forced to atomically read and write a byte.  However, this is
   //not guaranteed, and it will probably be necessary at a future point to switch
   //to a solution involving mutexes.
   //
   //The default verbosity is 3.


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
void LgVerbosity_VerbositySet(unsigned in_verbosity) noexcept
{
    volatile uint8_t verbosity_temp_storage;

    //Clip.
    if (in_verbosity <= 9)
       verbosity_temp_storage = in_verbosity;
    else
       verbosity_temp_storage = 9;

    //Assign to the module-local variable.  I suspect this would be atomic on all
    //platforms, but that may not be a strong enough guarantee, and a mutex
    //may need to be used in the future.
    verbosity_stored = verbosity_temp_storage;
}


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
unsigned LgVerbosity_VerbosityGet(void) noexcept
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

//End of lg_verbosity.cpp
