//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/prng/lin_con/esrg_rand_int.h 242 2018-08-04 18:25:51Z dashley $
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
#ifndef ESRG_RAND_INT_H_INCLUDED
   #define ESRG_RAND_INT_H_INCLUDED

   #ifdef MODULE_ESRG_RAND_INT
      #define DECMOD_ESRG_RAND_INT
   #else
      #define DECMOD_ESRG_RAND_INT extern
   #endif

   //This module is grouped as a collection of algorithms.
   //At present, there is only one algorithm, but this may
   //change.
   //**************************************************************************
   //**************************************************************************
   //**********  Algorithm 01:  Power Residue 16807  **************************
   //**************************************************************************
   //**************************************************************************
   //This is the state vector.  This is conceptually private, and the
   //caller should not tamper with it or peek into it.
   struct ESRG_RAND_INT_Alg01RngState
      {
      unsigned rn_seed_whole;
         //The current whole random number.  This is the random
         //number that will be served up when the first algorithm
         //is used directly or when a random integer over a large
         //range is required.
      unsigned rn_seed_fractional;
         //The current seed in use for requests for bits or
         //small portions of bits.
      unsigned bit_buffer;
         //A buffer maintained to doll out bits and small random
         //numbers.  This buffer is emptied from the right.
      unsigned n_bb_valid;
         //The number of valid bits in the bit buffer.  When there
         //are no more valid bits, the bit buffer must be 
         //restocked.
      };
   
   //Initializes the random number generation structure.  If 
   //(init_val == -1), then the state will be randomized using the system
   //clock and other system randomness.  This means that the random 
   //sequence generated won't be predictable (it will vary from usage
   //to usage of this module).  If init_val is positive, the positive
   //value will be used to initialize the state.  The mapping from 
   //init_val is guaranteed to be deterministic (the same value always
   //leads to the same sequences), but uniqueness is not guaranteed:
   //different values of init_val may lead to the same sequences.
   DECMOD_ESRG_RAND_INT
   void ESRG_RAND_INT_Alg01_Init(struct ESRG_RAND_INT_Alg01RngState *state,
                                 int    init_val);

   //Returns a single random bit.
   DECMOD_ESRG_RAND_INT
   unsigned int ESRG_RAND_INT_Alg01_RandomBit(
                                 struct ESRG_RAND_INT_Alg01RngState *state
                                 );
   //Returns a single random byte.
   DECMOD_ESRG_RAND_INT
   unsigned int ESRG_RAND_INT_Alg01_RandomByte(
                                 struct ESRG_RAND_INT_Alg01RngState *state
                                 );

   DECMOD_ESRG_RAND_INT const char *ESRG_RAND_INT_cvcinfo(void);
   DECMOD_ESRG_RAND_INT const char *ESRG_RAND_INT_hvcinfo(void);
   #define ESRG_RAND_INT_H_VERSION ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/prng/lin_con/esrg_rand_int.h 242 2018-08-04 18:25:51Z dashley $")
#endif

//End of esrg_rand_int.h.
