//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/intfunc.h 71 2016-11-05 11:07:06Z dashley $
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
