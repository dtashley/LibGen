//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/esrg_int_def.h 71 2016-11-05 11:07:06Z dashley $
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
