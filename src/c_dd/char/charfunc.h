//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/char/charfunc.h 242 2018-08-04 18:25:51Z dashley $
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
#ifndef CHARFUNC_H_INCLUDED
   #define CHARFUNC_H_INCLUDED

   #ifdef MODULE_CHARFUNC
      #define DECMOD_CHARFUNC
   #else
      #define DECMOD_CHARFUNC extern
   #endif

   //Returns the value of a digit (0-9), or -1 if the
   //passed character is not a digit.
   DECMOD_CHARFUNC int CHARFUNC_digit_to_val(char digit);

   //Returns a lower-case character corresponding to the 
   //lowest nibble of the argument.
   DECMOD_CHARFUNC char CHARFUNC_nibble_to_lc_hex_digit(int nibble);

   //Converts a passed integer to 8 hex digits, lower case letters,
   //in reverse order (LSB first), with no terminator.  s may not
   //be null, and caller must provide space for s[0] through s[7].
   //s[8] is not necessary (no terminator is appended.
   DECMOD_CHARFUNC void CHARFUNC_int_to_lc_hex_rev(int arg, char *s);

   DECMOD_CHARFUNC const char *CHARFUNC_cvcinfo(void);
   DECMOD_CHARFUNC const char *CHARFUNC_hvcinfo(void);
   #define CHARFUNC_H_VERSION ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/char/charfunc.h 242 2018-08-04 18:25:51Z dashley $")
#endif

//End of charfunc.h.
