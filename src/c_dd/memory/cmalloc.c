//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/memory/cmalloc.c 244 2018-08-05 19:05:46Z dashley $
//{9271a56e-a39a-4ba3-af90-c665b60de5e5}
//-------------------------------------------------------------------------------------------------
//This file is part of "Embedded Tool Set", a tool set designed to facilitate embedded system
//software and hardware development.
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
