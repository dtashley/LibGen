//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/intfunc.c 71 2016-11-05 11:07:06Z dashley $
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
