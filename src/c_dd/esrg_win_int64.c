//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/esrg_win_int64.c 71 2016-11-05 11:07:06Z dashley $
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
#define MODULE_ESRG_WIN_INT64

#include <assert.h>
#include <string.h>

#include "charfunc.h"
#include "esrg_win_int64.h"


void ESRG_WIN_INT64_StringPlainToUi 
                                 (
                                 const char          *s,
                                 ESRG_WIN_INT64_UI64 *rv,
                                 int                 *err
                                 )
   {
   int cval;
   ESRG_WIN_INT64_UI64 mul_10_max = 1844674407370955161;
      //The maximum value that can be successfully
      //multiplied by 10 and still stay within 64 bits.
   ESRG_WIN_INT64_UI64 result;

   assert(s   != NULL);
   assert(rv  != NULL);
   assert(err != NULL);

   //By default, no error on return.
   *err = 0;

   //Cover the case of an illegal first character or
   //a zero length string.
   cval = CHARFUNC_digit_to_val(*s);
   if (cval == -1)
      goto err_return;

   //Cover the case of canonical zero.
   if (cval == 0)
      {
      if (s[1] != 0)
         {
         //There is a '0', but not a null char trailing.
         goto err_return;
         }
      else
         {
         //Canonical zero.
         *rv = 0;
         return;
         }
      }

   //If we're here, we have a digit which is not a zero,
   //and presumably more to follow.  Iterate through, applying
   //the standard formula.
   //
   //Overflow can occur under two circumstances.  Either we 
   //multiply by 10 and overflow that way, or else we add a 
   //digit and overflow that way.
   //
   //The multiply by 10 overflow can be prevented by
   //knowing the maximum constant that can be multiplied by 
   //10 without overflow (we know this at compile-time).
   //The add overflow can be detected by the observation that
   //if the result is less than the digit we added, we
   //overflowed.
   result = 0;
   do
      {
      if (result > mul_10_max)  //Can't mul--already too large.
         goto err_return;

      result *= 10;

      result += cval;

      if (result < cval)  //Overflow.
         goto err_return;

      s++;
      if (*s)
         {
         cval = CHARFUNC_digit_to_val(*s);
         }
      else
         {
         cval = 0;
         }

      if (cval == -1)
         goto err_return;

      } while (*s);

    
   *rv = result;
   return;

   //---------
   err_return:
   *rv = 0;
   *err = 1;
   return;
   }


const char *ESRG_WIN_INT64_cvcinfo(void)
   {
   return("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/esrg_win_int64.c 71 2016-11-05 11:07:06Z dashley $");
   }


const char *ESRG_WIN_INT64_hvcinfo(void)
   {
   return(ESRG_WIN_INT64_H_VERSION);
   }

//End of esrg_win_int64.c.
