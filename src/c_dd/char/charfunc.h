//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/char/charfunc.h 242 2018-08-04 18:25:51Z dashley $
//-------------------------------------------------------------------------------------------------
//This file is part of "David T. Ashley's Shared Source Code", a set of shared components
//integrated into many of David T. Ashley's projects.
//-------------------------------------------------------------------------------------------------
//This source code and any program in which it is compiled/used is provided under the MIT License,
//reproduced below.
//-------------------------------------------------------------------------------------------------
//Permission is hereby granted, free of charge, to any person obtaining a copy of
//this software and associated documentation files(the "Software"), to deal in the
//Software without restriction, including without limitation the rights to use,
//copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the
//Software, and to permit persons to whom the Software is furnished to do so,
//subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//-------------------------------------------------------------------------------------------------
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
