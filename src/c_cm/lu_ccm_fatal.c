//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_cmode/ccmfatal.c 244 2018-08-05 19:05:46Z dashley $
//{375a45aa-6e13-45dc-aee9-ad5b4cffece7}
//-------------------------------------------------------------------------------------------------
//This file is part of "Embedded Tool Set", a tool set designed to facilitate embedded system
//software and hardware development.
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
#if 0
#define MODULE_CCMFATAL

#include <process.h>
#include <stdio.h>

#include "ccmfatal.h"


void CCMFATAL_fatal(const char *desc,
                    const char *file,
                    int        line)
   {
   printf("Fatal error.  Must terminate execution.\n");
   printf("File: %s, Line: %d.\n", file, line);
   printf("Error description: %s\n", desc);
   exit(4);  //Error code 4 for error termination.
   }


const char *CCMFATAL_cvcinfo(void)
   {
   return("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_cmode/ccmfatal.c 244 2018-08-05 19:05:46Z dashley $");
   }


const char *CCMFATAL_hvcinfo(void)
   {
   return(CCMFATAL_H_VERSION);
   }

//End of ccmfatal.c.
#endif