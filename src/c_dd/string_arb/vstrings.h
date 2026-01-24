//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/string_arb/vstrings.h 243 2018-08-04 18:28:20Z dashley $
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
/* Implementation of virtual strings, or effectively buffers
** or arbitrary characters.
*/

#ifndef VSTRINGS_H_INCLUDED
   #define VSTRINGS_H_INCLUDED

   #ifdef MODULE_VSTRINGS
      #define DECMOD_VSTRINGS
   #else
      #define DECMOD_VSTRINGS extern
   #endif

   #define VSTRINGS_H_VERSION ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/string_arb/vstrings.h 243 2018-08-04 18:28:20Z dashley $")

   DECMOD_VSTRINGS const char *VSTRINGS_cvcinfo(void);
   DECMOD_VSTRINGS const char *VSTRINGS_hvcinfo(void);
#endif

//End of vstrings.h.
