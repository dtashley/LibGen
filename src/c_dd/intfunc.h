//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/intfunc.h 71 2016-11-05 11:07:06Z dashley $
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
#ifndef INTFUNC_H_INCLUDED
   #define INTFUNC_H_INCLUDED

   #ifdef MODULE_INTFUNC
      #define DECMOD_INTFUNC
   #else
      #define DECMOD_INTFUNC extern
   #endif

   //Max and min of two integers.
   DECMOD_INTFUNC int INTFUNC_max(int a, int b);
   DECMOD_INTFUNC int INTFUNC_min(int a, int b);

   //Odd and even..
   DECMOD_INTFUNC int INTFUNC_is_even(int arg);
   DECMOD_INTFUNC int INTFUNC_is_odd(int arg);

   //Power residue 16807 functions.
   //
   //Makes the recurrence mapping for the power residue random
   //number generator given by \alpha=16,807 and N=2^{31}-1.
   //One fact that may not be apparent is that with a non-zero
   //seed, the seed can't go to zero.  This is because 
   //2^{31}-1 is a Mersenne prime, and so multiplying a number
   //less than 2^{31}-1 by 16,807=7^3 can't result in a multiple
   //of 2^{31}-1, by the fundamental theorem of arithmetic.
   DECMOD_INTFUNC
   unsigned int INTFUNC_rn_power_res_16807_mapping(unsigned int arg_in);

   //Version control reporting functions.
   DECMOD_INTFUNC const char *INTFUNC_cvcinfo(void);
   DECMOD_INTFUNC const char *INTFUNC_hvcinfo(void);
   #define INTFUNC_H_VERSION ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/intfunc.h 71 2016-11-05 11:07:06Z dashley $")
#endif

//End of intfunc.h.
