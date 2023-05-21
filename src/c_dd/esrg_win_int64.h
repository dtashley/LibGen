//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/esrg_win_int64.h 71 2016-11-05 11:07:06Z dashley $
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
