//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/prng/lin_con/esrg_rand_int.c 242 2018-08-04 18:25:51Z dashley $
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
#define MODULE_ESRG_RAND_INT

#include <assert.h>
#include <string.h>
#include <time.h>

#include "esrg_rand_int.h"
#include "intfunc.h"


void ESRG_RAND_INT_Alg01_Init(struct ESRG_RAND_INT_Alg01RngState *state,
                              int    init_val)
   {
   int i;

   assert(state != NULL);
   assert((init_val==-1) || (init_val>0));

   //Regardless of init type, zero out the structure.  Among other things,
   //this sets the number of remaining random bits to zero.
   memset(state, 0, sizeof(*state));

   if (init_val == -1)
      {
      //This is the system randomness initialization case.  Some value
      //of system time should be adequate to start with.
      state->rn_seed_whole      = time(NULL);
      state->rn_seed_fractional = time(NULL);

      //Flip a few bits in the seeds to hopefully make them different
      //(but it doesn't matter if they aren't).
      state->rn_seed_whole      ^= 0xA5A5A5A5;
      state->rn_seed_fractional ^= 0x5A5A5A5A;

      //Be absolutely sure that the seeds can't be zero.
      state->rn_seed_whole      |= 0x1;
      state->rn_seed_fractional |= 0x2;

      //Iterate a small number of times to add further randomness
      //to the two seeds.
      for (i=0; i<3; i++)
         {
         state->rn_seed_whole 
            = INTFUNC_rn_power_res_16807_mapping(state->rn_seed_whole);
         }

      for (i=0; i<7; i++)
         {
         state->rn_seed_fractional 
            = INTFUNC_rn_power_res_16807_mapping(state->rn_seed_fractional);
         }
      }
   else
      {
      //This is the branch where the random seed is supplied.  We can
      //use very much the same approach as above to try to get something
      //random.
      state->rn_seed_whole      = init_val;
      state->rn_seed_fractional = init_val;

      //Flip a few bits in the seeds to hopefully make them different
      //(but it doesn't matter if they aren't).
      state->rn_seed_whole      ^= 0xA5A5A5A5;
      state->rn_seed_fractional ^= 0x5A5A5A5A;

      //Be absolutely sure that the seeds can't be zero.
      state->rn_seed_whole      |= 0x1;
      state->rn_seed_fractional |= 0x2;

      //Iterate a small number of times to add further randomness
      //to the two seeds.
      for (i=0; i<3; i++)
         {
         state->rn_seed_whole 
            = INTFUNC_rn_power_res_16807_mapping(state->rn_seed_whole);
         }

      for (i=0; i<7; i++)
         {
         state->rn_seed_fractional 
            = INTFUNC_rn_power_res_16807_mapping(state->rn_seed_fractional);
         }
      }

   //At this point the state is randomized.  The number of bits valid
   //in the bit reservoir is set to zero, so will force a restock
   //next time.
   }


//Restocks the bit reservoir.
static void ESRG_RAND_INT_Alg01_RestockBitBuffer(
                                                struct ESRG_RAND_INT_Alg01RngState *state
                                                )
   {
   assert(state != NULL);

   //We need to keep chugging along until we get an integer with the top two
   //bits clear.  This is the only way to get a random distribution of bits.
   do
      {
      assert(state->rn_seed_fractional != 0);
      state->rn_seed_fractional 
         = INTFUNC_rn_power_res_16807_mapping(state->rn_seed_fractional);
      } while (state->rn_seed_fractional & 0xC0000000);

   //At this point the lower 30 bits should be random.
   state->bit_buffer = state->rn_seed_fractional;
   state->n_bb_valid = 30;
   }


unsigned int ESRG_RAND_INT_Alg01_RandomBit(
                                 struct ESRG_RAND_INT_Alg01RngState *state
                                 )
   {
   int rv;

   assert(state != NULL);

   //Restock the supply of bits if necessary.
   if (state->n_bb_valid == 0)
      ESRG_RAND_INT_Alg01_RestockBitBuffer(state);

   if (state->bit_buffer & 0x01)
      rv = 1;
   else
      rv = 0;

   state->bit_buffer >>= 1;
   state->n_bb_valid--;

   return(rv);
   }


unsigned int ESRG_RAND_INT_Alg01_RandomByte(
                                 struct ESRG_RAND_INT_Alg01RngState *state
                                 )
   {
   int rv;

   assert(state != NULL);

   //Be sure enough bits to yank a byte.
   if (state->n_bb_valid < 8)
      ESRG_RAND_INT_Alg01_RestockBitBuffer(state);

   //Grab the byte and return.
   rv = state->bit_buffer & 0xFF;
   state->bit_buffer >>= 8;  //Must roll down to get new set of bits.
   state->n_bb_valid -= 8;

   return(rv);
   }


const char *ESRG_RAND_INT_cvcinfo(void)
   {
   return("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/prng/lin_con/esrg_rand_int.c 242 2018-08-04 18:25:51Z dashley $");
   }


const char *ESRG_RAND_INT_hvcinfo(void)
   {
   return(ESRG_RAND_INT_H_VERSION);
   }

//End of esrg_rand_int.c.
