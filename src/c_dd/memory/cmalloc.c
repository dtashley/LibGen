//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/memory/cmalloc.c 244 2018-08-05 19:05:46Z dashley $
//{9271a56e-a39a-4ba3-af90-c665b60de5e5}
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
#define MODULE_CMALLOC

#include <malloc.h>
#include <process.h>
#include <stdio.h>

#include "cmalloc.h"

#include "../../c_cmode/ccmfatal.h"


void *CMALLOC_malloc( size_t size )
   {
   void *rv;

   rv = malloc(size);

   if (!rv)
      {
      CCMFATAL_fatal("NULL pointer from malloc()--probable out of memory.",
                     __FILE__,
                     __LINE__);
      }

   return(rv);
   }


void *CMALLOC_calloc( size_t num, size_t size )
   {
   void *rv;

   rv = calloc(num, size);

   if (!rv)
      {
      CCMFATAL_fatal("NULL pointer from calloc()--probable out of memory.",
                     __FILE__,
                     __LINE__);
      }

   return(rv);
   }

void *CMALLOC_realloc( void *memblock, size_t size )
   {
   void *rv;

   rv = realloc(memblock, size);

   if ((!rv) && (size))
      {
      CCMFATAL_fatal("NULL pointer from realloc()--probable out of memory.",
                     __FILE__,
                     __LINE__);
      }

   return(rv);
   }


void CMALLOC_free( void *memblock )
   {
   free(memblock);
   }


const char *CMALLOC_cvcinfo(void)
   {
   return("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/memory/cmalloc.c 244 2018-08-05 19:05:46Z dashley $");
   }


const char *CMALLOC_hvcinfo(void)
   {
   return(CMALLOC_H_VERSION);
   }

//End of cmalloc.c.
