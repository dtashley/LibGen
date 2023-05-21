//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_cmode/ccmfatal.h 244 2018-08-05 19:05:46Z dashley $
//{04bbbc03-348f-4a4a-8bf0-953a8c53fd0d}
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
