//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/esrg_int_def.h 71 2016-11-05 11:07:06Z dashley $
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
/* This module defines the default integer used by the Tcl/Tk interpreters
** and by other software which does not explicitly deal with arbitrary length
** integers.  The hope is that, over time, the Tcl core can be migrated to use
** this functionality.  What should happen is that, with the flip of a preprocessor
** switch, the default integer size can be changed.
**
** The only real guideline for the size is that it must be at least as large
** as the maximum size of a file.
** 
** It is felt at the present time that 128-bit integers should be adequate for
** just about everything.
*/
//--------------------------------------------------------------------------------
#ifndef ESRG_INT_DEF_H_INCLUDED
   #define ESRG_INT_DEF_H_INCLUDED

   #ifdef MODULE_ESRG_INT_DEF
      #define DECMOD_ESRG_INT_DEF
   #else
      #define DECMOD_ESRG_INT_DEF extern
   #endif

   //Configuration constants.
   #define ESRG_INT_DEF_BITSIZE (128)
      //The size, in bits, of each integer.  This must be a multiple of the machines
      //native integer size.
   #define ESRG_INT_DEF_NATIVE_BITSIZE (32)
      //The size, in bits, of the native int of the machine.  This may be
      //any positive integer.
   #define ESRG_INT_DEF_NLIMBS (ESRG_INT_DEF_BITSIZE/ESRG_INT_DEF_NATIVE_BITSIZE)
      //The number of native integers required for each of the default
      //integers.

   struct ESRG_INT_DEF_struct
      {
      unsigned int flags;
         //The flags, as defined below.
      #define ESRG_INT_DEF_FLAG_IS_SIGNED  (0x00000001)
         //Integer is signed.  Defined at creation time and not changed.
      #define ESRG_INT_DEF_FLAG_NEG_ROLL   (0x00000002)
      #define ESRG_INT_DEF_FLAG_POS_ROLL   (0x00000004)
         //TRUE if has rolled over in a negative way or positive way.  
         //Depending on what is being done, this may mean result is invalid.
      #define ESRG_INT_DEF_FLAG_NAN        (0x00000008)
         //TRUE if the result is not a number.  This may be from division by zero,
         //square root of a negative number, etc.
      unsigned int limbs[ESRG_INT_DEF_NLIMBS];
         //The limbs of the integer.  These are arranged with the least significant
         //limb as [0], i.e. things are arranged least-significant component first.
         //If the integer is signed, it is a concatenated two's complement representation--
         //same as the ideal, but spread over multiple native ints.
      };

   typedef struct ESRG_INT_DEF_struct ESRG_INT_DEF_INT;

   //Required initialization before doing anything.
   //Destruction is not required--only initialization before
   //the first assignment or use.
   DECMOD_ESRG_INT_DEF void ESRG_INT_DEF_Create
      ( ESRG_INT_DEF_INT *arg ,
        int               is_signed);

   //Assigns a machine native unsigned integer to the value.
   DECMOD_ESRG_INT_DEF void ESRG_INT_DEF_AssignU
      ( ESRG_INT_DEF_INT *arg ,
        unsigned int      aval);

   //Assigns a machine native signed integer to the value.
   DECMOD_ESRG_INT_DEF void ESRG_INT_DEF_AssignS
      ( ESRG_INT_DEF_INT *arg, 
        int aval);

   //Compares two values to see their relative relationship.
   //Returned values are -1 if a<b, 0 if a==b, and 1 if a>b.
   //All cases of signed/unsigned mixes are handled.  Only the
   //bit patterns of the operands are considered--whether they
   //are the result of rollovers, etc., is not considered.
   DECMOD_ESRG_INT_DEF int ESRG_INT_DEF_Cmp
      ( ESRG_INT_DEF_INT *a, 
        ESRG_INT_DEF_INT *b );

#endif

//End of esrg_int_def.h.
