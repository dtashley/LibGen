//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/memory/cmalloc.h 244 2018-08-05 19:05:46Z dashley $
//{a08e4749-1ca1-4865-bdeb-7c5b02e94411}
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
This module defines a wrapper around the standard "C" memory allocation
function for simple console-mode applications.  If there are any memory
allocation errors encountered (such as out of memory), the behavior will
simply be to write an error message and to terminate.  This is adequate
for data-driven console mode applications.
*/

#ifndef CMALLOC_H_INCLUDED
   #define CMALLOC_H_INCLUDED

   #ifdef MODULE_CMALLOC
      #define DECMOD_CMALLOC
   #else
      #define DECMOD_CMALLOC extern
   #endif

   #define CMALLOC_H_VERSION ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/memory/cmalloc.h 244 2018-08-05 19:05:46Z dashley $")

   DECMOD_CMALLOC void *CMALLOC_malloc( size_t size );
   DECMOD_CMALLOC void *CMALLOC_calloc( size_t num, size_t size );
   DECMOD_CMALLOC void *CMALLOC_realloc( void *memblock, size_t size );
   DECMOD_CMALLOC void CMALLOC_free( void *memblock );
   DECMOD_CMALLOC const char *CMALLOC_cvcinfo(void);
   DECMOD_CMALLOC const char *CMALLOC_hvcinfo(void);
#endif

//End of cmalloc.h.
