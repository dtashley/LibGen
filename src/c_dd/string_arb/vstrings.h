//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/string_arb/vstrings.h 243 2018-08-04 18:28:20Z dashley $
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
