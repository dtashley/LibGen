//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/string_c/strfuncs.h 242 2018-08-04 18:25:51Z dashley $
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
#ifndef STRFUNCS_H_INCLUDED
   #define STRFUNCS_H_INCLUDED

   #ifdef MODULE_STRFUNCS
      #define DECMOD_STRFUNCS
   #else
      #define DECMOD_STRFUNCS extern
   #endif

   #define STRFUNCS_H_VERSION ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/string_c/strfuncs.h 242 2018-08-04 18:25:51Z dashley $")

   DECMOD_STRFUNCS 
      char Strfuncs_NibbleToHexadecimalChar(unsigned char arg, int use_upper_case);
   DECMOD_STRFUNCS
      void Strfuncs_UlToHexString(unsigned long num,
                                  char *output,
                                  int use_upper_case);
   DECMOD_STRFUNCS 
      int Strfuncs_IsSubstring(const char *substr, const char *arg);
   DECMOD_STRFUNCS const char *StrfuncsCversion(void);
   DECMOD_STRFUNCS const char *StrfuncsHversion(void);


#endif

//End of strfuncs.h.
