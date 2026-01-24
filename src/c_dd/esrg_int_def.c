//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/esrg_int_def.c 71 2016-11-05 11:07:06Z dashley $
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
