//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/esrg_win_int64.h 71 2016-11-05 11:07:06Z dashley $
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
#ifndef ESRG_WIN_INT64_H_INCLUDED
   #define ESRG_WIN_INT64_H_INCLUDED

   #ifdef MODULE_ESRG_WIN_INT64
      #define DECMOD_ESRG_WIN_INT64
   #else
      #define DECMOD_ESRG_WIN_INT64 extern
   #endif

   //Type definitions of the unsigned and signed 64-bit integers
   //manipulated by this module.
   typedef unsigned __int64 ESRG_WIN_INT64_UI64;
   typedef          __int64 ESRG_WIN_INT64_SI64;

   //Converts from a string to an unsigned 64-bit int.  The
   //only allowed representation of 0 is "0".  No commas are
   //allowed.  No leading 0's are allowed except for the
   //canonical representation of 0.  No leading or trailing
   //whitespace is allowed.  In case of error, the error flag
   //is set non-zero and the return result is 0.  No pointer
   //parameters may be NULL.

   DECMOD_ESRG_WIN_INT64 void ESRG_WIN_INT64_StringPlainToUi 
                                 (
                                 const char          *s,
                                 ESRG_WIN_INT64_UI64 *rv,
                                 int                 *err
                                 );
                                 
   DECMOD_ESRG_WIN_INT64 const char *ESRG_WIN_INT64_cvcinfo(void);
   DECMOD_ESRG_WIN_INT64 const char *ESRG_WIN_INT64_hvcinfo(void);
   #define ESRG_WIN_INT64_H_VERSION ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/esrg_win_int64.h 71 2016-11-05 11:07:06Z dashley $")

#endif

//End of esrg_win_int64.c.
