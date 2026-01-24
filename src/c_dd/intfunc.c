//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/intfunc.c 71 2016-11-05 11:07:06Z dashley $
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
#define MODULE_INTFUNC

#include "intfunc.h"


//08/16/01: Visual inspection OK.
int INTFUNC_max(int a, int b)
   {
   if (a > b)
      return(a);
   else
      return(b);
   }


//08/16/01: Visual inspection OK.
int INTFUNC_min(int a, int b)
   {
   if (a < b)
      return(a);
   else
      return(b);
   }


//08/16/01: Visual inspection OK.
int INTFUNC_is_even(int arg)
   {
   if ((arg & 0x1) == 0)
      return(1);
   else
      return(0);
   }


//08/16/01: Visual inspection OK.
int INTFUNC_is_odd(int arg)
   {
   if ((arg & 0x1) == 1)
      return(1);
   else
      return(0);
   }


unsigned int INTFUNC_rn_power_res_16807_mapping(unsigned int arg_in)
   {
   unsigned __int64 arg_local;

   arg_local =  arg_in;
   arg_local *= (unsigned __int64)16807;
   arg_local %= (unsigned __int64)2147483647;

   return((unsigned int)arg_local);
   }


//08/16/01: Visual inspection OK.
const char *INTFUNC_cvcinfo(void)
   {
   return("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/intfunc.c 71 2016-11-05 11:07:06Z dashley $");
   }


//08/16/01: Visual inspection OK.
const char *INTFUNC_hvcinfo(void)
   {
   return(INTFUNC_H_VERSION);
   }

//End of intfunc.c.
