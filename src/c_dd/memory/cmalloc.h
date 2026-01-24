//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/memory/cmalloc.h 244 2018-08-05 19:05:46Z dashley $
//{a08e4749-1ca1-4865-bdeb-7c5b02e94411}
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
