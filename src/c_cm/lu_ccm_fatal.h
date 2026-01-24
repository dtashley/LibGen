//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_cmode/ccmfatal.h 244 2018-08-05 19:05:46Z dashley $
//{04bbbc03-348f-4a4a-8bf0-953a8c53fd0d}
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
/*
This module defines "C" console mode fatal error behavior.  For
the most part, it is just a wrapper to give fatal terminations
a consistent look.  Fatal terminations for "C" console-mode 
applications are things like out of memory.  They are generally
fairly routine.
*/

#ifndef CCMFATAL_H_INCLUDED
   #define CCMFATAL_H_INCLUDED

   #ifdef MODULE_CCMFATAL
      #define DECMOD_CCMFATAL
   #else
      #define DECMOD_CCMFATAL extern
   #endif

   #define CCMFATAL_H_VERSION ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_cmode/ccmfatal.h 244 2018-08-05 19:05:46Z dashley $")

   DECMOD_CCMFATAL void CCMFATAL_fatal(const char *desc,
                                       const char *file,
                                       int        line);
   DECMOD_CCMFATAL const char *CCMFATAL_cvcinfo(void);
   DECMOD_CCMFATAL const char *CCMFATAL_hvcinfo(void);
#endif 

// End of ccmfatal.h.
