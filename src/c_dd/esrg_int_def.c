//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/esrg_int_def.c 71 2016-11-05 11:07:06Z dashley $
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
#define MODULE_ESRG_INT_DEF

#include <assert.h>
#include <string.h>

#include "esrg_int_def.h"


void ESRG_INT_DEF_Create( ESRG_INT_DEF_INT *arg, int is_signed )
   {
   assert(arg != NULL);

   memset(arg, 0, sizeof(ESRG_INT_DEF_INT));

   if (is_signed)
      {
      arg->flags = ESRG_INT_DEF_FLAG_IS_SIGNED;
      }
   }


void ESRG_INT_DEF_AssignU( ESRG_INT_DEF_INT *arg, unsigned int aval)
   {
   assert(arg != NULL);

   //Reset any pending error flags.  An assignment starts fresh.
   arg->flags &= ~(ESRG_INT_DEF_FLAG_NEG_ROLL | ESRG_INT_DEF_FLAG_POS_ROLL | ESRG_INT_DEF_FLAG_NAN);

   arg->limbs[0] = aval;
   memset(&(arg->limbs[1]), 0, sizeof(arg->limbs[0]) * (ESRG_INT_DEF_NLIMBS-1));

   //The only circumstance under which there is an overflow
   //is if the value is inherently signed and if the limbs per 
   //int is 1 and the MSB is set.
   if (arg->flags & ESRG_INT_DEF_FLAG_IS_SIGNED)
      {
      if (ESRG_INT_DEF_NLIMBS == 1)
         {
         if (aval & (1 << (ESRG_INT_DEF_NATIVE_BITSIZE-1)))
            {
            arg->flags |= ESRG_INT_DEF_FLAG_POS_ROLL;
            }
         }
      }
   }


void ESRG_INT_DEF_AssignS( ESRG_INT_DEF_INT *arg, int aval)
   {
   assert(arg != NULL);

   //Reset any pending error flags.  An assignment starts fresh.
   arg->flags &= ~(ESRG_INT_DEF_FLAG_NEG_ROLL | ESRG_INT_DEF_FLAG_POS_ROLL | ESRG_INT_DEF_FLAG_NAN);

   arg->limbs[0] = aval;
      //Assign the base limb.

   //Deal with the sign extension.  If the number is not signed,
   //don't extend at all.  If the number is signed, extend.
   if (arg->flags & ESRG_INT_DEF_FLAG_IS_SIGNED)
      {
      if (aval < 0)
         memset(&(arg->limbs[1]), -1, sizeof(arg->limbs[0]) * (ESRG_INT_DEF_NLIMBS-1));
      else
         memset(&(arg->limbs[1]),  0, sizeof(arg->limbs[0]) * (ESRG_INT_DEF_NLIMBS-1));
      }
   else
      {
      memset(&(arg->limbs[1]),  0, sizeof(arg->limbs[0]) * (ESRG_INT_DEF_NLIMBS-1));
      }
   
   //The only circumstance under which there is an overflow
   //is if the value is inherently unsigned and if the limbs per 
   //int is 1 and the MSB is set.
   if (!(arg->flags & ESRG_INT_DEF_FLAG_IS_SIGNED))
      {
      if (ESRG_INT_DEF_NLIMBS == 1)
         {
         if (aval & (1 << (ESRG_INT_DEF_NATIVE_BITSIZE-1)))
            {
            arg->flags |= ESRG_INT_DEF_FLAG_POS_ROLL;
            }
         }
      }
   }


int ESRG_INT_DEF_Cmp ( ESRG_INT_DEF_INT *a, ESRG_INT_DEF_INT *b )
   {
   assert(a != NULL);
   assert(b != NULL);

   return(0);
   }

//End of esrg_int_def.c.
