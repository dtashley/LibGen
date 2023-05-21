//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/gmp_ints.c 71 2016-11-05 11:07:06Z dashley $
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
#define MODULE_GMP_INTS

#include <assert.h>
#include <ctype.h>
#include <process.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


/* Only included the guarded allocation header if we are compiling
** a DOS console type application.  Other types of applications have
** other ways of protecting for out of memory.  Including the
** header would do no harm in these cases, but do no good, either.
*/
#if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
   #include "ccmalloc.h"
#elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
   #include "tclalloc.h"
#else
   /* Do nothing. */
#endif

#include "bstrfunc.h"
#include "charfunc.h"
#include "fcmiof.h"
#include "gmp_ints.h"
#include "intfunc.h"


/******************************************************************/
/***  CUSTOM ALLOCATION FUNCTIONS   *******************************/
/******************************************************************/
/* We need to decide here on how memory not on the stack will be
** allocated (i.e. what will become of the standard functions
** like malloc, free, etc.).  This is dependent on the type of
** application we're making.  The possible types are so far are:
**    APP_TYPE_SIMPLE_DOS_CONSOLE : 
**       Simple DOS console application.
**    APP_TYPE_IJUSCRIPTER_IJUCONSOLE:
**       The Tcl tool build.
** 
** The custom allocation functions here are a "portal" or "wrapper"
** for how the integer and rational number functions should
** get memory.
**
** The functions below are standard, except that the GNU MP team
** built more generality into what allocation schemes could be
** used by including size information in some calls that don't
** normally get it.  That is why there are some extra calls below
** where the information is discarded.  Other than that, these are
** standard allocation calls.
*/
//07/15/01:  Visual inspection only.  Function deemed too 
//simple for unit testing.
void *GMP_INTS_malloc( size_t size )
   {
   #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
      return(CCMALLOC_malloc(size));
   #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
      return(TclpAlloc(size));
   #else
      return(malloc(size));
   #endif 
   }


//07/15/01:  Visual inspection only.  Function deemed too 
//simple for unit testing.
void *GMP_INTS_calloc( size_t num, size_t size )
   {
   #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
      return(CCMALLOC_calloc(num, size));
   #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
      return(TclpCalloc(num, size));
   #else
      return(calloc(num, size));
   #endif 
   }


//07/15/01:  Visual inspection only.  Function deemed too 
//simple for unit testing.
void *GMP_INTS_realloc( void *memblock, size_t size )
   {
   #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
      return(CCMALLOC_realloc(memblock, size));
   #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
      return(TclpRealloc(memblock, size));
   #else
      return(realloc(memblock, size));
   #endif 
   }


//07/15/01:  Visual inspection only.  Function deemed too 
//simple for unit testing.
void *GMP_INTS_realloc_w_size( void *memblock, 
                               size_t old_size,
                               size_t size )
   {
   #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
      return(CCMALLOC_realloc(memblock, size));
   #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
      return(TclpRealloc(memblock, size));
   #else
      return(realloc(memblock, size));
   #endif 
   }


//07/15/01:  Visual inspection only.  Function deemed too 
//simple for unit testing.
void GMP_INTS_free( void *memblock )
   {
   #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
      CCMALLOC_free(memblock);
   #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
      TclpFree(memblock);
   #else
      free(memblock);
   #endif 
   }


//07/15/01:  Visual inspection only.  Function deemed too 
//simple for unit testing.
void GMP_INTS_free_w_size( void *memblock, size_t size )
   {
   #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
      CCMALLOC_free(memblock);
   #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
      TclpFree(memblock);
   #else
      free(memblock);
   #endif 
   }


/******************************************************************/
/***  PORTABILITY CHECK FUNCTIONS   *******************************/
/******************************************************************/
//Because there is the risk that Microsoft Visual C++ might 
//change in the future, the following function can be called
//to see if the assumptions about data sizes are valid.  This
//function returns TRUE if there is a problem, or FALSE
//otherwise.

//07/15/01:  Unit testing complete.
int GMP_INTS_data_sizes_are_wrong(void)
   {
   int i;
   GMP_INTS_limb_t tv;
   _int64 tv64;

   //Check the number of bit rolls required to get the limb
   //to go to zero again.  This had better be 32.
   tv = 1;
   i = 0;
   while (tv)
      {
      tv <<= 1;
      i++;
      }
   if (i != 32)
      return(1);

   //Check that an _int64 is really and truly 64 bits.
   tv64 = 1;
   i = 0;
   while (tv64)
      {
      tv64 <<= 1;
      i++;
      }
   if (i != 64)
      return(1);

   //Room for additional tests here if needed later.

   return(0);
   }


/******************************************************************/
/***  ERROR STRING IDENTIFICATION AND PROCESSING FUNCTIONS  *******/
/******************************************************************/

int GMP_INTS_identify_nan_string(const char *s)
   {
   assert(s != NULL);

   if (!strcmp(s, GMP_INTS_EF_INTOVF_POS_STRING))
      return(0);
   else if (!strcmp(s, GMP_INTS_EF_INTOVF_NEG_STRING))
      return(1);
   else if (!strcmp(s, GMP_INTS_EF_INTOVF_TAINT_POS_STRING))
      return(2);
   else if (!strcmp(s, GMP_INTS_EF_INTOVF_TAINT_NEG_STRING))
      return(3);
   else
      return(-1);
   }


const char *GMP_INTS_supply_nan_string(int idx)
   {
   assert((idx >= 0) && (idx <= 3));

   if (idx==0)
      return(GMP_INTS_EF_INTOVF_POS_STRING);
   else if (idx==1)
      return(GMP_INTS_EF_INTOVF_NEG_STRING);
   else if (idx==2)
      return(GMP_INTS_EF_INTOVF_TAINT_POS_STRING);
   else
      return(GMP_INTS_EF_INTOVF_TAINT_NEG_STRING);
   }


/******************************************************************/
/***  DEBUG PRINTING FUNCTIONS   **********************************/
/******************************************************************/
//These functions are for printing out integers and limbs
//and groups of limbs for unit testing and debugging.                        

//07/15/01:  Exempt from testing because debug/development
//function.
void GMP_INTS_print_limb_group(FILE *stream,
                               GMP_INTS_limb_srcptr lg,
                               GMP_INTS_size_t n,
                               char *desc)
   {
   int i;

   assert(stream != NULL);
   assert(n >= 0);
   assert(desc != NULL);

   if (!lg)
      {
      fprintf(stream, "   %s: NULL\n", desc);
      }
   else
      {
      for (i=n-1; i>=0; i--)
         {
         fprintf(stream, "   %s[%2d]: 0x%8X\n", desc, i, lg[i]);
         }
      }
   }


void GMP_INTS_mpz_print_int(FILE *stream,
                            const GMP_INTS_mpz_struct *arg,
                            char *desc)
   {
   int i;

   assert(stream != NULL);
   assert(arg != NULL);
   assert(desc != NULL);

   fprintf(stream, "Printing integer:\n   %s\n", desc);

   fprintf(stream, "               flags: %d\n", arg->flags);
   fprintf(stream, "   ptr value to body: %p\n", arg);
   fprintf(stream, "            n_allocd: %d\n", arg->n_allocd);
   fprintf(stream, "                size: %d\n", arg->size);
   fprintf(stream, "     limbs (ptr val): %p\n", arg->limbs);

   for (i=arg->n_allocd-1; i>=0; i--)
      {
      fprintf(stream, "   limb[%3d]: %8X\n", i, arg->limbs[i]);
      }
   }


/******************************************************************/
/***  LOW-LEVEL MACRO REPLACEMENTS   ******************************/
/******************************************************************/
//The functions in this category are replacements for macros.
//Clarity was gained at the expense of speed.

int GMP_INTS_mpz_get_flags (const GMP_INTS_mpz_struct *arg)
   {
   assert(arg != NULL);
   assert(arg->n_allocd > 0);

   return(arg->flags);
   } 


//07/15/01:  Visual inspection only.  Function deemed too 
//simple for unit testing.
GMP_INTS_size_t GMP_INTS_abs_of_size_t(GMP_INTS_size_t arg)
   {
   //Be sure that the bit pattern does not represent the maximum
   //negative argument.  Negating this would give the result of
   //zero, which is not what is intended.
   assert(arg != 0x80000000);

   if (arg < 0)
      return(-arg);
   else
      return(arg);
   }


//07/15/01:  Visual inspection only.  Function deemed too 
//simple for unit testing.
int GMP_INTS_mpz_sgn(const GMP_INTS_mpz_struct *arg)
   {
   assert(arg != NULL);
   assert(arg->n_allocd > 0);

   if (arg->size > 0)
      return(1);
   else if (arg->size == 0)
      return(0);
   else
      return(-1);
   }


//07/15/01:  Visual inspection only.  Function deemed too 
//simple for unit testing.
int GMP_INTS_mpz_is_neg(const GMP_INTS_mpz_struct *arg)
   {
   assert(arg != NULL);
   assert(arg->n_allocd > 0);

   if (GMP_INTS_mpz_sgn(arg) == -1)
      return(1);
   else
      return(0);
   }


//07/15/01:  Visual inspection only.  Function deemed too 
//simple for unit testing.
int GMP_INTS_mpz_is_zero(const GMP_INTS_mpz_struct *arg)
   {
   assert(arg != NULL);
   assert(arg->n_allocd > 0);

   if (GMP_INTS_mpz_sgn(arg) == 0)
      return(1);
   else
      return(0);
   }


//07/15/01:  Visual inspection only.  Function deemed too 
//simple for unit testing.
int GMP_INTS_mpz_is_pos(const GMP_INTS_mpz_struct *arg)
   {
   assert(arg != NULL);
   assert(arg->n_allocd > 0);

   if (GMP_INTS_mpz_sgn(arg) == 1)
      return(1);
   else
      return(0);
   }


//07/15/01:  Visual inspection only.  Function deemed too 
//simple for unit testing.
int GMP_INTS_mpz_is_odd(const GMP_INTS_mpz_struct *arg)
   {
   assert(arg != NULL);
   assert(arg->n_allocd > 0);

   if (arg->size == 0)
      return 0;
   else if ((arg->limbs[0] & 0x1) != 0)
      return 1;
   else
      return 0;
   }


//07/15/01:  Visual inspection only.  Function deemed too 
//simple for unit testing.
int GMP_INTS_mpz_is_even(const GMP_INTS_mpz_struct *arg)
   {
   assert(arg != NULL);
   assert(arg->n_allocd > 0);

   if (GMP_INTS_mpz_is_odd(arg))
      return 0;
   else
      return 1;
   }


void GMP_INTS_mpz_negate(GMP_INTS_mpz_struct *arg)
   {
   //Eyeball the input parameters.
   assert(arg != NULL);
   assert(arg->n_allocd > 0);
   assert(arg->limbs != NULL);

   arg->size = -(arg->size);
   }


//07/15/01:  Visual inspection only.  Function deemed too 
//simple for unit testing.
void GMP_INTS_mpn_normalize(GMP_INTS_limb_ptr  limb_array,
                            GMP_INTS_size_t   *idx)
   {
   assert(limb_array != NULL);
   assert(idx != NULL);
   assert(idx >= 0);

   while (*idx > 0)
      {
      if (limb_array[*idx - 1] != 0)
         break;
      (*idx)--;
      }
   }


//07/15/01:  Visual inspection only.  Function deemed too 
//simple for unit testing.
void GMP_INTS_mpn_copy_limbs(GMP_INTS_limb_ptr    dest,
                             GMP_INTS_limb_srcptr src,
                             GMP_INTS_size_t      n)
  {
  GMP_INTS_size_t i;

  assert(dest != NULL);
  assert(src != NULL);
  assert(n >= 0);

  for (i=0; i<n; i++)
    dest[i] = src[i];
  }


/******************************************************************/
/***  LOW-LEVEL ARITHMETIC FUNCTIONS   ****************************/
/******************************************************************/

int GMP_INTS_two_op_flags_map(int flags1, int flags2)
   {
   int cf;

   if (!flags1 && !flags2)
      {
      return(0);
      }
   else
      {
      cf = flags1 | flags2;

      if (cf & (GMP_INTS_EF_INTOVF_POS | GMP_INTS_EF_INTOVF_TAINT_POS))
         {
         //In either case here, the result will be tainted 
         //positive.
         return(GMP_INTS_EF_INTOVF_TAINT_POS);
         }
      else if (cf & (GMP_INTS_EF_INTOVF_NEG | GMP_INTS_EF_INTOVF_TAINT_NEG))
         {
         //In either case here, the result will be tainted
         //negative.
         return(GMP_INTS_EF_INTOVF_TAINT_NEG);
         }
      else
         {
         //This case is where the flags ints are non-zero, but
         //no known bits are set.  This is surely some kind of
         //an internal software error.  In debug mode, want to
         //alert to error.  In actual operation, want to just
         //pretend an ordinary positive taint.
         assert(0);
         return(GMP_INTS_EF_INTOVF_TAINT_POS);
         }
      }
   }


GMP_INTS_limb_t GMP_INTS_mpn_add_1 (GMP_INTS_limb_ptr    res_ptr,
                                    GMP_INTS_limb_srcptr s1_ptr,
                                    GMP_INTS_size_t      s1_size,
                                    GMP_INTS_limb_t      s2_limb)
   {
   GMP_INTS_limb_t x;

   assert(res_ptr != NULL);
   assert(s1_ptr != NULL);
   assert(s1_size > 0);

   x = *s1_ptr++;
   s2_limb = x + s2_limb;
   *res_ptr++ = s2_limb;
   //Since limbs are unsigned, the test below tests if there
   //was a carry, i.e. a positive rollover.
   if (s2_limb < x)
      {
      while (--s1_size != 0)
         {
         x = *s1_ptr++ + 1;
         *res_ptr++ = x;
         if (x != 0)
            goto fin;
         }
      
      return 1;
      }
   
   fin:
   if (res_ptr != s1_ptr)
      {
      GMP_INTS_size_t i;
      for (i = 0; i < s1_size - 1; i++)
         {
         res_ptr[i] = s1_ptr[i];
         }
      }

   return 0;
   }


GMP_INTS_limb_t GMP_INTS_mpn_sub_1(GMP_INTS_limb_ptr    res_ptr,
                                   GMP_INTS_limb_srcptr s1_ptr,
                                   GMP_INTS_size_t      s1_size,
                                   GMP_INTS_limb_t      s2_limb)
   {
   GMP_INTS_limb_t x;

   assert(res_ptr != NULL);
   assert(s1_ptr  != NULL);
   assert(s1_size > 0);

   x = *s1_ptr++;
   s2_limb = x - s2_limb;
   *res_ptr++ = s2_limb;
   //The test below detects a borrow.
   if (s2_limb > x)
      {
      while (--s1_size != 0)
         {
         x = *s1_ptr++;
         *res_ptr++ = x - 1;
         if (x != 0)
            goto fin;
         }
      
      return 1;
      }
   
   fin:
   if (res_ptr != s1_ptr)
      {
      GMP_INTS_size_t i;
      for (i = 0; i < s1_size - 1; i++)
         {
            res_ptr[i] = s1_ptr[i];
         }
      }
      return 0;
   }


//07/15/01:  Am willing to skip unit-testing on this.
//Understand the logic (i.e. passes visual inspection), 
//and comes from GNU-MP.  Hope any defects here will be 
//caught in testing of GMP_INTS_mpz_mul() and other 
//higher-level functions.
GMP_INTS_limb_t GMP_INTS_mpn_mul_1 (GMP_INTS_limb_ptr res_ptr, 
						                  GMP_INTS_limb_srcptr s1_ptr, 
						                  GMP_INTS_size_t s1_size, 
						                  GMP_INTS_limb_t s2_limb)
   {
   GMP_INTS_limb_t cy_limb;
   GMP_INTS_size_t j;
   GMP_INTS_limb_t prod_high, prod_low;
   unsigned _int64 temp;
   
   assert(res_ptr != NULL);
   assert(s1_ptr != NULL);
   assert(s1_size > 0);

   /* The loop counter and index J goes from -S1_SIZE to -1.  This way
   the loop becomes faster.  */
   j = -s1_size;
   
   /* Offset the base pointers to compensate for the negative indices.  */
   s1_ptr -= j;
   res_ptr -= j;
   
   cy_limb = 0;
   do
      {
      //The original code here was the following macro:
      //umul_ppmm (prod_high, prod_low, s1_ptr[j], s2_limb);
      //Will use the 64-bit data type of MSVC++ to achieve
      //the same effect.
      //
      //NOTE AS OF 07/13/01:  I have looked at the assembly-
      //language, and the lines below are a real sore spot.
      //The multiply is fairly direct (although there is a
      //function call), but the shift does not behave as
      //expected--there is a function call and a loop to
      //go through the 32 iterations.  After logical testing,
      //may want to clean this out--this would surely
      //result in a speed increase.  This is a sore spot.
      temp = ((unsigned _int64)s1_ptr[j]) * ((unsigned _int64)s2_limb);
      prod_low  = (GMP_INTS_limb_t)temp;
      prod_high = (GMP_INTS_limb_t)(temp >> 32);

      prod_low += cy_limb;
      cy_limb = (prod_low < cy_limb) + prod_high;
      
      res_ptr[j] = prod_low;
      }
   while (++j != 0);
      
   return cy_limb;
   }


//07/15/01:  Am willing to skip unit-testing on this.
//Understand the logic (i.e. passes visual inspection), 
//and comes from GNU-MP.  Hope any defects here will be 
//caught in testing of GMP_INTS_mpz_add() and other 
//higher-level functions.
GMP_INTS_limb_t GMP_INTS_mpn_add_n(GMP_INTS_limb_ptr    res_ptr,  
                                   GMP_INTS_limb_srcptr s1_ptr, 
                                   GMP_INTS_limb_srcptr s2_ptr, 
                                   GMP_INTS_size_t      size)
   {
   GMP_INTS_limb_t x, y, cy;
   GMP_INTS_size_t j;

   assert(res_ptr != NULL);
   assert(s1_ptr  != NULL);
   assert(s2_ptr  != NULL);

   /* The loop counter and index J goes from -SIZE to -1.  This way
      the loop becomes faster.  */
   j = -size;

   /* Offset the base pointers to compensate for the negative indices.  */
   s1_ptr -= j;
   s2_ptr -= j;
   res_ptr -= j;

   cy = 0;
   do
      {
      y = s2_ptr[j];
      x = s1_ptr[j];
      y += cy;   /* add previous carry to one addend */
      cy = (y < cy);  /* get out carry from that addition */
      y = x + y;  /* add other addend */
      cy = (y < x) + cy; /* get out carry from that add, combine */
      res_ptr[j] = y;
      }
   while (++j != 0);

   return cy;
   }


//07/15/01:  Am willing to skip unit-testing on this.
//Understand the logic (i.e. passes visual inspection), 
//and comes from GNU-MP.  Hope any defects here will be 
//caught in testing of GMP_INTS_mpz_mul() and other 
//higher-level functions.
GMP_INTS_limb_t GMP_INTS_mpn_addmul_1 (GMP_INTS_limb_ptr    res_ptr, 
                                       GMP_INTS_limb_srcptr s1_ptr, 
                                       GMP_INTS_size_t      s1_size, 
                                       GMP_INTS_limb_t      s2_limb)
   {
   GMP_INTS_limb_t cy_limb;
   GMP_INTS_size_t j;
   GMP_INTS_limb_t prod_high, prod_low;
   GMP_INTS_limb_t x;
   unsigned _int64 temp;

   //Eyeball the inputs carefully.
   assert(res_ptr != NULL);
   assert(s1_ptr != NULL);
   assert(s1_size > 0);

   /* The loop counter and index J goes from -SIZE to -1.  This way
      the loop becomes faster.  */
   j = -s1_size;

   /* Offset the base pointers to compensate for the negative indices.  */
   res_ptr -= j;
   s1_ptr -= j;

   cy_limb = 0;
   do
      {
      //The original code here was the following macro:
      //umul_ppmm (prod_high, prod_low, s1_ptr[j], s2_limb);
      //Will use the 64-bit data type of MSVC++ to achieve
      //the same effect.
      //
      //NOTE AS OF 07/14/01:  I have not looked at the assembly-
      //language, but the assembly-language generated by what
      //is below is suspected to have performance problems.
      //May want to come back to this.
      temp = ((unsigned _int64)s1_ptr[j]) * ((unsigned _int64)s2_limb);
      prod_low  = (GMP_INTS_limb_t)temp;
      prod_high = (GMP_INTS_limb_t)(temp >> 32);

      prod_low += cy_limb;
      cy_limb = (prod_low < cy_limb) + prod_high;

      x = res_ptr[j];
      prod_low = x + prod_low;
      cy_limb += (prod_low < x);
      res_ptr[j] = prod_low;
      }
   while (++j != 0);

   return cy_limb;
   }


//07/15/01:  Am willing to skip unit-testing on this.
//Understand the logic (i.e. passes visual inspection), 
//and comes from GNU-MP.
GMP_INTS_limb_t GMP_INTS_mpn_add (GMP_INTS_limb_ptr    res_ptr,
                                  GMP_INTS_limb_srcptr s1_ptr,
                                  GMP_INTS_size_t      s1_size,
                                  GMP_INTS_limb_srcptr s2_ptr,
                                  GMP_INTS_size_t      s2_size)
   {
   GMP_INTS_limb_t cy_limb = 0;

   assert(res_ptr != NULL);
   assert(s1_ptr  != NULL);
   assert(s2_ptr  != NULL);

   //Numbers apparently must be arranged with sizes so that 
   //LIMBS(s1) >= LIMBS(s2).
   //Add the parts up to the most significant limb of S2.
   if (s2_size != 0)
      cy_limb = GMP_INTS_mpn_add_n (res_ptr, 
                                    s1_ptr, 
                                    s2_ptr, 
                                    s2_size);

   //Process the carry result, and propagate the carries up through
   //the parts of S1 that don't exist in S2, i.e. propagate the
   //carries upward in S1.
   if (s1_size - s2_size != 0)
      cy_limb = GMP_INTS_mpn_add_1 (res_ptr + s2_size,
                                    s1_ptr + s2_size,
                                    s1_size - s2_size,
                                    cy_limb);
   return cy_limb;
   }


//07/15/01:  Am willing to skip unit-testing on this.
//Understand the logic (i.e. passes visual inspection), 
//and comes from GNU-MP.
GMP_INTS_limb_t GMP_INTS_mpn_sub_n(GMP_INTS_limb_ptr    res_ptr, 
                                   GMP_INTS_limb_srcptr s1_ptr, 
                                   GMP_INTS_limb_srcptr s2_ptr, 
                                   GMP_INTS_size_t      size)
   {
   GMP_INTS_limb_t x, y, cy;
   GMP_INTS_size_t j;

   assert(res_ptr != NULL);
   assert(s1_ptr  != NULL);
   assert(s2_ptr  != NULL);

   /* The loop counter and index J goes from -SIZE to -1.  This way
      the loop becomes faster.  */
   j = -size;

   /* Offset the base pointers to compensate for the negative indices.  */
   s1_ptr -= j;
   s2_ptr -= j;
   res_ptr -= j;

   cy = 0;
   do
      {
      y = s2_ptr[j];
      x = s1_ptr[j];
      y += cy;   /* add previous carry to subtrahend */
      cy = (y < cy);  /* get out carry from that addition */
      y = x - y;  /* main subtract */
      cy = (y > x) + cy; /* get out carry from the subtract, combine */
      res_ptr[j] = y;
      }
   while (++j != 0);

   return cy;
   }


//07/17/01:  Am willing to skip unit-testing on this.
//Understand the logic (i.e. passes visual inspection), 
//and comes from GNU-MP.
GMP_INTS_limb_t GMP_INTS_mpn_sub (GMP_INTS_limb_ptr    res_ptr,
                                  GMP_INTS_limb_srcptr s1_ptr,
                                  GMP_INTS_size_t      s1_size,
                                  GMP_INTS_limb_srcptr s2_ptr,
                                  GMP_INTS_size_t      s2_size)
   {
   GMP_INTS_limb_t cy_limb = 0;

   assert(res_ptr != NULL);
   assert(s1_ptr  != NULL);
   assert(s2_ptr  != NULL);

   if (s2_size != 0)
      cy_limb = GMP_INTS_mpn_sub_n(res_ptr, 
                                   s1_ptr, 
                                   s2_ptr, 
                                   s2_size);

   if (s1_size - s2_size != 0)
      cy_limb = GMP_INTS_mpn_sub_1(res_ptr + s2_size,
                                   s1_ptr + s2_size,
                                   s1_size - s2_size,
                                   cy_limb);
   return cy_limb;
   }


//07/17/01:  Am willing to skip unit-testing on this.
//Understand the logic (i.e. passes visual inspection), 
//and comes from GNU-MP.
GMP_INTS_limb_t GMP_INTS_mpn_lshift(GMP_INTS_limb_ptr    wp,
                                    GMP_INTS_limb_srcptr up, 
                                    GMP_INTS_size_t      usize,
                                    unsigned int         cnt)
   {
   GMP_INTS_limb_t high_limb, low_limb;
   unsigned sh_1, sh_2;
   GMP_INTS_size_t i;
   GMP_INTS_limb_t retval;

   assert(wp != NULL);
   assert(up != NULL);
   assert(usize > 0);
   assert(cnt > 0);

   sh_1 = cnt;

   wp += 1;
   sh_2 = GMP_INTS_BITS_PER_LIMB - sh_1;
     //This automatically implies that can't call this function to shift more
     //than 31 places.
   i = usize - 1;
   low_limb = up[i];
   retval = low_limb >> sh_2;  //Return value is the amount shifted
                               //off the top.
   high_limb = low_limb;
   while (--i >= 0)
      {
      low_limb = up[i];
      wp[i] = (high_limb << sh_1) | (low_limb >> sh_2);
      high_limb = low_limb;
      }
   wp[i] = high_limb << sh_1;

   return retval;
   }


//07/17/01:  Am willing to skip unit-testing on this.
//Understand the logic more or less (i.e. passes visual inspection), 
//and comes from GNU-MP.
/* Shift U (pointed to by UP and USIZE limbs long) CNT bits to the right
   and store the USIZE least significant limbs of the result at WP.
   The bits shifted out to the right are returned.

   Argument constraints:
   1. 0 < CNT < BITS_PER_MP_LIMB
   2. If the result is to be written over the input, WP must be <= UP.
*/
GMP_INTS_limb_t GMP_INTS_mpn_rshift (GMP_INTS_limb_ptr    wp,
                                     GMP_INTS_limb_srcptr up, 
                                     GMP_INTS_size_t      usize,
                                     unsigned int         cnt)
   {
   GMP_INTS_limb_t high_limb, low_limb;
   unsigned sh_1, sh_2;
   GMP_INTS_size_t i;
   GMP_INTS_limb_t retval;

   assert(wp != NULL);
   assert(up != NULL);
   assert(usize > 0);
   assert(cnt > 0);

   sh_1 = cnt;

   wp -= 1;
   sh_2 = GMP_INTS_BITS_PER_LIMB - sh_1;
   high_limb = up[0];
   retval = high_limb << sh_2;
   low_limb = high_limb;

   for (i = 1; i < usize; i++)
      {
      high_limb = up[i];
      wp[i] = (low_limb >> sh_1) | (high_limb << sh_2);
      low_limb = high_limb;
      }
   wp[i] = low_limb >> sh_1;

   return retval;
   }


//07/17/01:  Am willing to skip unit-testing on this.
//Understand the logic (i.e. passes visual inspection), 
//and comes from GNU-MP.
int GMP_INTS_mpn_cmp (GMP_INTS_limb_srcptr op1_ptr, 
                      GMP_INTS_limb_srcptr op2_ptr, 
                      GMP_INTS_size_t      size)
   {
   GMP_INTS_size_t i;
   GMP_INTS_limb_t op1_word, op2_word;

   assert(op1_ptr != NULL);
   assert(op2_ptr != NULL);

   for (i = size - 1; i >= 0; i--)
      {
      op1_word = op1_ptr[i];
      op2_word = op2_ptr[i];
      if (op1_word != op2_word)
         goto diff;
      }
   return 0;

   diff:
   //This can *not* be simplified to
   //   op2_word - op2_word
   //since that expression might give signed overflow.
   return (op1_word > op2_word) ? 1 : -1;
   }


//07/15/01:  Am willing to skip unit-testing on this.
//Understand the logic (i.e. passes visual inspection), 
//and comes from GNU-MP.  Hope any defects here will be 
//caught in testing of GMP_INTS_mpz_mul() and other 
//higher-level functions.
void GMP_INTS_mpn_mul_basecase (GMP_INTS_limb_ptr prodp,
                                GMP_INTS_limb_srcptr up, 
                                GMP_INTS_size_t usize,
                                GMP_INTS_limb_srcptr vp, 
                                GMP_INTS_size_t vsize)
   {
   assert(prodp != NULL);
   assert(up != NULL);
   assert(usize > 0);
   assert(vp != NULL);
   assert(vsize > 0);

   /* We first multiply by the low order one or two limbs, as the result can
      be stored, not added, to PROD.  We also avoid a loop for zeroing this
      way.  */
   prodp[usize] = GMP_INTS_mpn_mul_1 (prodp, up, usize, vp[0]);
   prodp++;
   vp++;
   vsize--;

   /* For each iteration in the loop, multiply U with one limb from V, and
      add the result to PROD.  */
   while (vsize != 0)
      {
      prodp[usize] = GMP_INTS_mpn_addmul_1 (prodp, up, usize, vp[0]);
      prodp++, 
      vp++, 
      vsize--;
      }
   }


//07/15/01:  No unit testing possible--this is a passthrough.
//In the original GNU MP code, there were several multiplication
//algorithms, and this function would select one based on the
//size of the operands and other considerations.  The code has been
//pared so that only simple multiplication is used, which is why
//this function contains only a single pass-thru function call.
void GMP_INTS_mpn_mul_n (GMP_INTS_limb_ptr    p, 
                         GMP_INTS_limb_srcptr a, 
                         GMP_INTS_limb_srcptr b, 
                         GMP_INTS_size_t      n)
   {
   GMP_INTS_mpn_mul_basecase (p, a, n, b, n);
   }


//07/16/01:  Visual inspection OK.  Will not perform unit testing.
GMP_INTS_limb_t GMP_INTS_mpn_mul(GMP_INTS_limb_ptr    prodp,
                                 GMP_INTS_limb_srcptr up, 
                                 GMP_INTS_size_t      un,
                                 GMP_INTS_limb_srcptr vp, 
                                 GMP_INTS_size_t      vn)
   {
   //This is a gutted version of the GNU MP function.  The GNU
   //MP function considered the case of a square, and also 
   //better algorithms that pay off with large operands.
   //This gutted version uses only basic multiplication
   //(O(N**2)).

   //Eyeball the input parameters.
   assert(prodp != NULL);
   assert(up != NULL);
   assert(un >= 0);
   assert(vp != NULL);
   assert(vn >= 0);

   /* Basic long multiplication. */
   GMP_INTS_mpn_mul_basecase (prodp, up, un, vp, vn);

   //Return the most significant limb (which might be zero).  
   //This is different than
   //most other functions, which return the spillover.
   return prodp[un + vn - 1];
   }


/******************************************************************/
/***  LIMB SPACE REALLOCATION FUNCTIONS   *************************/
/******************************************************************/

void *GMP_INTS_mpz_realloc (GMP_INTS_mpz_struct *m, 
                            GMP_INTS_size_t      new_size)
   {
   /* Never allocate zero space. */
   if (new_size <= 0)
      new_size = 1;

   m->limbs = (GMP_INTS_limb_ptr) 
               GMP_INTS_realloc_w_size (m->limbs,
                                        m->n_allocd * sizeof(GMP_INTS_limb_t),
                                        new_size * sizeof(GMP_INTS_limb_t));
   m->n_allocd = new_size;

   return (void *) m->limbs;
   }


/******************************************************************/
/***  PUBLIC INITIALIZATION AND MEMORY MANAGEMENT FUNCTIONS   *****/
/******************************************************************/

void GMP_INTS_mpz_init (GMP_INTS_mpz_struct *x)
   {
   assert(x != NULL);

   //The structure (the header block) exists in the
   //caller's area.  Most likely it is a local variable.
   //This is OK, because it doesn't take up much space.

   //Start off with no errors.
   x->flags = 0;
   
   //Allocate space for one limb, which is the most
   //basic amount.  This will grow, almost certainly.
   x->limbs = GMP_INTS_malloc(sizeof(GMP_INTS_limb_t));

   //Indicate that one limb was allocated.
   x->n_allocd = 1;

   //Set the size to 0.  This signals a value of zero.
   x->size = 0;
   }


void GMP_INTS_mpz_clear (GMP_INTS_mpz_struct *x)
   {
   //Be sure the passed pointer is not NULL.
   assert(x != NULL);

   //Be sure that the amount allocated is also above zero.
   //Anything else represents a logical error.
   assert(x->n_allocd > 0);

   //Be sure that the pointer to the allocated limbs
   //is not NULL.  Anything else would be a logical
   //error.
   assert(x->limbs != NULL);

   //Deallocate the space for the limbs.  The pointer is
   //set NULL and the allocated amount set to zero 
   // so in case clear is called again it will be
   //a detectable error.
   GMP_INTS_free_w_size(x->limbs, 
                        x->n_allocd * sizeof(GMP_INTS_limb_t));
   x->limbs = NULL;
   x->n_allocd = 0;
   }


/******************************************************************/
/***  PUBLIC ASSIGNMENT FUNCTIONS   *******************************/
/******************************************************************/

void GMP_INTS_mpz_copy(      GMP_INTS_mpz_struct *dst,
                       const GMP_INTS_mpz_struct *src)
   {
   GMP_INTS_size_t i, n;

   //Eyeball the input parameters.
   assert(dst != NULL);
   assert(dst->n_allocd > 0);
   assert(dst->limbs != NULL);
   assert(src != NULL);
   assert(src->n_allocd > 0);
   assert(src->limbs != NULL);

   //Source and destination may not be the same.
   assert(src != dst);
   
   //Figure out the real size of the source.  We need to take the absolute
   //value.
   n = GMP_INTS_abs_of_size_t(src->size);

   //Reallocate the destination to be bigger if necessary.
   if (dst->n_allocd < n)
      {
      GMP_INTS_mpz_realloc (dst, n);
      }

   //Copy the non-dynamic fields in the header.
   dst->flags     = src->flags;
   dst->size      = src->size;

   //Copy the limbs.
   for (i=0; i<n; i++)
      dst->limbs[i] = src->limbs[i];
   }


void GMP_INTS_mpz_set_ui (GMP_INTS_mpz_struct *dest, 
                          unsigned long int val)
   { 
   assert(dest != NULL);
   
   /* We don't check if the allocation is enough, since the rest of the
      package ensures it's at least 1, which is what we need here.  */

   dest->flags = 0;
      //A set operation resets any errors.

   if (val > 0)
      {
      dest->limbs[0] = val;
      dest->size = 1;
      }
   else
      {
      dest->size = 0;
      }
   }


void GMP_INTS_mpz_set_si (GMP_INTS_mpz_struct *dest, 
                          signed long int val)
   {
   assert(dest != NULL);

   /* We don't check if the allocation is enough, since the rest of the
      package ensures it's at least 1, which is what we need here.  */

   dest->flags = 0;
     //A set operation resets any errors.

   if (val > 0)
      {
      dest->limbs[0] = val;
      dest->size = 1;
      }
   else if (val < 0)
      {
      dest->limbs[0] = (unsigned long) -val;
      dest->size = -1;
      }
   else
      {
      dest->size = 0;
      }
   }


void GMP_INTS_mpz_set_simple_char_str(GMP_INTS_mpz_struct *z, 
                                      const char *s)
   {
   int sign=1;
   int digval;
   GMP_INTS_mpz_struct digvalz, k10;

   //Eyeball the arguments.
   assert(z != NULL);
   assert(z->n_allocd > 0);
   assert(z->limbs != NULL);
   assert(s != NULL);

   //Set the arbitrary integer to zero.  This will also kill
   //any error flags.
   GMP_INTS_mpz_set_ui(z, 0);

   //Allocate an integer for our private use to hold each digit
   //value.
   GMP_INTS_mpz_init(&digvalz);

   //Allocate the constant 10, which we will use often.
   GMP_INTS_mpz_init(&k10);
   GMP_INTS_mpz_set_ui(&k10, 10);

   //As long as there are are digits and no flags set, keep
   //multiplying and adding the value of the digit.  Non-
   //digits are simply ignored.
   while (!(z->flags) && (*s))
      {
      if (*s == '-')
         {
         sign = -sign;
         }
      else
         {
         digval = CHARFUNC_digit_to_val(*s);
         if (digval >= 0)
            {
            GMP_INTS_mpz_set_ui(&digvalz, digval);
            GMP_INTS_mpz_mul(z, z, &k10);
            GMP_INTS_mpz_add(z, z, &digvalz);
            }
         }
      s++;
      }

   //Adjust the final sign of the result.
   if (sign < 0)
      z->size = -(z->size);

   //Deallocate our temporary integers.
   GMP_INTS_mpz_clear(&digvalz);
   GMP_INTS_mpz_clear(&k10);
   }


void GMP_INTS_mpz_set_sci_not_num(GMP_INTS_mpz_struct *z,
                                  int *failure, 
                                  const char *s)
   {
   int parse_failure;
      //Return code from the floating point parsing
      //function.
   char mant_sign;
      //Sign character, if any, from the mantissa,
      //or N otherwise.
   size_t mant_bdp;
      //The index to the start of the mantissa before
      //the decimal point.
   size_t mant_bdp_len;
      //The length of the mantissa before the decimal
      //point.  Zero means not defined, i.e. that
      //no characters were parsed and interpreted as
      //that part of a floating point number.
   size_t mant_adp;
   size_t mant_adp_len;
      //Similar fields for after the decimal point.
   char exp_sign;
      //Sign of the exponent, if any, or N otherwise.
   size_t exp;
   size_t exp_len;
      //Similar fields as to the mantissa, but for the
      //exponent.
   size_t si;
      //Iteration variable.
   int exponent_val;
      //The value of the exponent.  We can't accept
      //an exponent outside the range of a 24-bit
      //signed integer.  The 24-bit limit is arbitrary.
      //For one thing, it gives room to detect overflow
      //as are adding and multiplying by 10.

   //Eyeball the input parameters.
   assert(z != NULL);
   assert(z->n_allocd > 0);
   assert(z->limbs != NULL);
   assert(failure != NULL);
   assert(s != NULL);

   //Start off believing no failure.
   *failure = 0;

   //Set the output to zero.  This is the default case for some
   //steps below.
   GMP_INTS_mpz_set_ui(z, 0);

   //Attempt to parse the number as a general number
   //in scientific notation.
   BSTRFUNC_parse_gen_sci_not_num(s,
                                  &parse_failure,
                                  &mant_sign,
                                  &mant_bdp,
                                  &mant_bdp_len,
                                  &mant_adp,
                                  &mant_adp_len,
                                  &exp_sign,
                                  &exp,
                                  &exp_len);

   //If it wouldn't parse as a general number, can't go further.
   if (parse_failure)
      {
      *failure = 1;
      return;
      }
   else if (!exp_len && !mant_adp_len)
      {
      //There was no exponent, and no portion after
      //the decimal point.  Can just parse as an integer.
      char *temp_buf;

      //Allocate the temporary buffer to be one character longer
      //than the length specified for the parsed mantissa.
      temp_buf = GMP_INTS_malloc(sizeof(char) * (mant_bdp_len + 1));

      //Copy from the parsed area into the temporary buffer.
      for (si=mant_bdp; si<(mant_bdp+mant_bdp_len); si++)
         temp_buf[si-mant_bdp] = s[si];
      temp_buf[mant_bdp_len] = 0;

      //Set the arbitrary integer to the value of the character
      //string.
      GMP_INTS_mpz_set_simple_char_str(z, temp_buf);

      //If the number parsed as negative, invert.
      if (mant_sign == '-')
         z->size = -z->size;

      //Deallocate the temporary buffer.
      GMP_INTS_free(temp_buf);
      }
   else if (!exp_len && mant_adp_len)
      {
      char *temp_buf;

      //In this case, there are digits after the decimal point,
      //but no exponent specified.  The only way this makes
      //sense is if all of the digits are zero--otherwise it
      //cannot be an integer.
      for (si=mant_adp; si<(mant_adp+mant_adp_len); si++)
         {
         if (s[si] != '0')
            {
            *failure = 1;
            return;
            }
         }

      //We're clean.  They are only zeros.  Execute as per
      //integer code.

      //Allocate the temporary buffer to be one character longer
      //than the length specified for the parsed mantissa.
      temp_buf = GMP_INTS_malloc(sizeof(char) * (mant_bdp_len + 1));

      //Copy from the parsed area into the temporary buffer.
      for (si=mant_bdp; si<(mant_bdp+mant_bdp_len); si++)
         temp_buf[si-mant_bdp] = s[si];
      temp_buf[mant_bdp_len] = 0;

      //Set the arbitrary integer to the value of the character
      //string.
      GMP_INTS_mpz_set_simple_char_str(z, temp_buf);

      //If the number parsed as negative, invert.
      if (mant_sign == '-')
         z->size = -z->size;

      //Deallocate the temporary buffer.
      GMP_INTS_free(temp_buf);
      }
   else if (exp_len)
      {
      //This is the most difficult case, where an exponent
      //is specified.  There are several complex subcases,
      //such as:
      //  a)If the exponent is too positive or too negative,
      //    we can't use it.  In general, we won't tackle
      //    an exponent that won't fit in a signed 24-bit
      //    integer.  This provides a range of from
      //    -8,388,608 to +8,388,607.  This dwarfs the
      //    100,000 or so digit preprocessor limit,
      //    and should be adequate for any practical
      //    application.
      //  b)If the exponent is zero, we ignore it.
      //  c)If the exponent is positive, it has to
      //    be large enough to overcome any 
      //    digits past the decimal point, otherwise
      //    we don't end up with an integer.
      //  d)If the exponent is negative, there have to
      //    be enough digits so that an integer remains
      //    after the exponent is applied.  This
      //    generally requires trailing zeros on the
      //    part before the decimal point.

      //First, tackle the exponent.  Process the
      //exponent into a signed integer.  We have to
      //balk at anything outside of 24 bits.  The
      //procedure used automatically handles
      //leading zeros correctly.
      exponent_val = 0;
      for (si=exp; si<(exp+exp_len); si++)
         {
         int val;

         val = CHARFUNC_digit_to_val(s[si]);

         assert(val >= 0 && val <= 9);

         exponent_val *= 10;
         exponent_val += val;

         if (((exp_sign=='-') && (exponent_val>8388608)) 
            || 
            ((exp_sign != '-') && (exponent_val>8388607)))
            {
            *failure = 1;
            return;
            }
         }

      //If we're here, the exponent has been computed and
      //is within 24 bits.  However, we need to adjust for
      //the sign.
      if (exp_sign == '-')
         exponent_val = -exponent_val;

      //We need to make accurate assertions about the
      //portion of the number, if any, after the decimal point.
      //This means that we need to effectively discard
      //trailing zeros.  To do this, we do not need to
      //relocate the string, we can just back off the index
      //to bypass any trailing zeros.
      while ((mant_adp_len > 0) && (s[mant_adp + mant_adp_len - 1]=='0'))
         mant_adp_len--;

      //We also need to make accurate assertions about the
      //portion of the number, if any, before the decimal
      //point.  It is known that the parsing function
      //isn't tolerant of multiple zeros, but zero is a 
      //special case.  Let's advance the pointer to the
      //part before the decimal point so that zero will
      //have zero length.
      while ((mant_bdp_len > 0) && (s[mant_bdp]=='0'))
         {
         mant_bdp++;
         mant_bdp_len--;
         }

      //If we are dealing with zero, who cares about the
      //exponent?  Just return the value of zero.
      if (!mant_bdp_len && !mant_adp_len)
         {
         *failure = 0;
         GMP_INTS_mpz_set_ui(z, 0);
         return;
         }
      
      //Beyond this point, we have something non-zero.
      //If the exponent is positive, it must be at least
      //as large as the number of digits beyond the
      //decimal point in order to form an integer.  If the
      //exponent is zero, there must be no digits after the
      //decimal point.  If the exponent is negative, there 
      //must be no digits after the decimal point, and the
      //trailing zeros on the part before the decimal point
      //must be adequate to handle the right decimal shift.
      if (exponent_val == 0)
         {
         if (mant_adp_len)
            {
            *failure = 1;
            return;
            }
         }
      else if (exponent_val > 0)
         {
         if ((int)mant_adp_len > exponent_val)
            {
            *failure = 1;
            return;
            }
         }
      else //exponent_val < 0
         {
         if (mant_adp_len)
            {
            *failure = 1;
            return;
            }
         else
            {
            //Count the number of trailing zeros on the part
            //before the decimal point.
            size_t trailing_zero_count;
            int idx;

            trailing_zero_count = 0;
        
            for(idx = mant_bdp + mant_bdp_len - 1; 
                (mant_bdp_len != 0) && (idx >= (int)mant_bdp);
                idx--)
                {
                if (s[idx] == '0')
                   trailing_zero_count++;
                else
                   break;
                }
             
            //Check on the assertion about trailing zeros.
            if ((int)trailing_zero_count < -exponent_val)
               {
               *failure = 1;
               return;
               }
            }
         }
     
         {
         //Create a string long enough to hold the digits
         //before the decimal point plus the ones after and
         //convert that to an arbitrary integer.
         //Form a power of 10 which is 10 exponentiated to
         //the absolute value of the exponent.  If the 
         //exponent was positive, multiply by it.  If the
         //exponent was negative, divide by it.
         char *conv_str;
         size_t sidx;
         GMP_INTS_mpz_struct power_of_ten, k10, trash;

         GMP_INTS_mpz_init(&power_of_ten);
         GMP_INTS_mpz_init(&k10);
         GMP_INTS_mpz_init(&trash);

         conv_str = GMP_INTS_malloc(sizeof(char) * (mant_bdp_len + mant_adp_len + 1));

         sidx=0;

         for (si=mant_bdp; si<(mant_bdp+mant_bdp_len); si++)
            {
            conv_str[sidx] = s[si];
            sidx++;
            }
         for (si=mant_adp; si<(mant_adp+mant_adp_len); si++)
            {
            conv_str[sidx] = s[si];
            sidx++;
            }
         conv_str[sidx] = 0;

         assert(sidx == (mant_bdp_len + mant_adp_len));
         
         GMP_INTS_mpz_set_simple_char_str(z, conv_str); 

         GMP_INTS_mpz_set_ui(&k10, 10);

         if (exponent_val > 0)
            GMP_INTS_mpz_pow_ui(&power_of_ten, &k10, exponent_val-mant_adp_len);
         else
            GMP_INTS_mpz_pow_ui(&power_of_ten, &k10, -exponent_val);

         if (exponent_val >= 0)
            {
            GMP_INTS_mpz_mul(z, z, &power_of_ten);
            }
         else
            {
            GMP_INTS_mpz_tdiv_qr (&k10, 
                                  &trash, 
                                  z, 
                                  &power_of_ten);
            GMP_INTS_mpz_copy(z, &k10);
            }

         //If the argument had a minus sign, invert.
         if (mant_sign == '-')
            z->size = -z->size;

         GMP_INTS_free(conv_str);

         GMP_INTS_mpz_clear(&trash);
         GMP_INTS_mpz_clear(&k10);
         GMP_INTS_mpz_clear(&power_of_ten);

         //Finally, if the arbitrary integer has overflowed, this is
         //a parse failure.  Must declare as such.
         if (z->flags)
            *failure = 1;
         }
      }
   else
      {
      *failure = 1;
      return;
      }
   }


void GMP_INTS_mpz_set_general_int(GMP_INTS_mpz_struct *z,
                                  int *failure, 
                                  const char *s)
   {
   //Eyeball the input parameters.
   assert(z != NULL);
   assert(z->n_allocd > 0);
   assert(z->limbs != NULL);
   assert(failure != NULL);
   assert(s != NULL);

   //Try to parse it as a simple integer.
   if (BSTRFUNC_is_sint_wo_commas(s))
      {
      GMP_INTS_mpz_set_simple_char_str(z, s);
      *failure = 0;
      return;
      }
   //If that didn't work, try to parse it as a simple
   //integer with commas.
   else if (BSTRFUNC_is_sint_w_commas(s))
      {
      GMP_INTS_mpz_set_simple_char_str(z, s);
      *failure = 0;
      return;
      }
   //If neither of those worked, try to parse it as
   //something containing scientific notation.
   else
      {
      GMP_INTS_mpz_set_sci_not_num(z, failure, s);

      if (!*failure)
         {
         //We were able to parse it that way.
         //Everything is set up, just return.
         return;
         }
      else
         {
         //We're out of options.  All parsing failed.
         GMP_INTS_mpz_set_ui(z, 0);
         *failure = 1;
         return;
         }
      }
   }


void GMP_INTS_mpz_parse_into_uint32(unsigned *result,
                                    int *failure,
                                    char *s)
   {
   GMP_INTS_mpz_struct arb_int;

   //Eyeball the input parameters.
   assert(result != NULL);
   assert(failure != NULL);
   assert(s != NULL);

   //Allocate space for the one arbitrary integer we need.
   GMP_INTS_mpz_init(&arb_int);

   //Try to parse the string into an arbitrary length integer
   //using all methods known to man.
   GMP_INTS_mpz_set_general_int(&arb_int, failure, s);

   //If the parse failed, we must declare failure and return
   //0.
   if (*failure)
      {
      *result = 0;
      *failure = 1;
      }
   else
      {
      //We might have success, but it might be negative or
      //too big.
      if (arb_int.size == 1)
         {
         *result = arb_int.limbs[0];
         *failure = 0;
         }
      else if (arb_int.size == 0)
         {
         *result =  0;
         *failure = 0;
         }
      else
         {
         *result = 0;
         *failure = 1;
         }
      }

   //Deallocate the arbitrary integer.
   GMP_INTS_mpz_clear(&arb_int);
   }


void GMP_INTS_mpz_swap(GMP_INTS_mpz_struct *a, 
                       GMP_INTS_mpz_struct *b)
   {
   GMP_INTS_mpz_struct temp;

   //Eyeball the input parameters.
   assert(a           != NULL);
   assert(a->n_allocd >  0);
   assert(a->limbs    != NULL);
   assert(b           != NULL);
   assert(b->n_allocd >  0);
   assert(b->limbs    != NULL);

   //Make the swap via memory copy.
   memcpy(&temp, a,     sizeof(GMP_INTS_mpz_struct));
   memcpy(a,     b,     sizeof(GMP_INTS_mpz_struct));
   memcpy(b,     &temp, sizeof(GMP_INTS_mpz_struct));
   }


/******************************************************************/
/***  PUBLIC ARITHMETIC FUNCTIONS   *******************************/
/******************************************************************/

//07/15/01:  Unit test and visual inspection passed.
void GMP_INTS_mpz_add (      GMP_INTS_mpz_struct *w, 
                       const GMP_INTS_mpz_struct *u, 
                       const GMP_INTS_mpz_struct *v)
   {
   GMP_INTS_limb_srcptr up, vp;
   GMP_INTS_limb_ptr wp;
   GMP_INTS_size_t usize, vsize, wsize;
   GMP_INTS_size_t abs_usize;
   GMP_INTS_size_t abs_vsize;

   //Look at the input parameters carefully.
   assert(w != NULL);
   assert(u != NULL);
   assert(v != NULL);
   assert(w->n_allocd > 0);
   assert(u->n_allocd > 0);
   assert(v->n_allocd > 0);
   assert(w->limbs != NULL);
   assert(u->limbs != NULL);
   assert(v->limbs != NULL);

   //Handle the case of a tainted result.  If either of the
   //two inputs are either direct overflows or tainted by
   //an overflow, mark the result tainted and do not perform
   //any arithmetic operation.
      {
      int taint;

      taint = GMP_INTS_two_op_flags_map(u->flags, v->flags);

      w->flags = 0; 
         //"w" starts off with a clean slate.  Must do this
         //after taint calculation in case locations of u or v 
         //are the same as w.
      if (taint)
         {
         w->flags = taint;
         return;
         }
      }

   usize = u->size;
   vsize = v->size;
   abs_usize = GMP_INTS_abs_of_size_t(usize);
   abs_vsize = GMP_INTS_abs_of_size_t(vsize);

   //Arrange things so that U has at least as many
   //limbs as V, i.e. limbs(U) >= limbs(V);
   if (abs_usize < abs_vsize)
      {
      const GMP_INTS_mpz_struct *tmp_ptr;
      GMP_INTS_size_t      tmp_size;

      //Swap U and V.  This does no harm, because we are
      //manipulating only local variables.  This does not
      //affect the caller.
      tmp_ptr = u;
      u = v;
      v = tmp_ptr;
      tmp_size = usize;
      usize = vsize;
      vsize = tmp_size;
      tmp_size = abs_usize;
      abs_usize = abs_vsize;
      abs_vsize = tmp_size;
      }

   /* True: ABS_USIZE >= ABS_VSIZE.  */

   /* If not space for w (and possible carry), increase space.  */
   wsize = abs_usize + 1;
   if (w->n_allocd < wsize)
      GMP_INTS_mpz_realloc(w, wsize);

   //These pointers must be obtained after realloc.  At this point,
   //u or v may be the same as w.
   up = u->limbs;
   vp = v->limbs;
   wp = w->limbs;

   if ((usize ^ vsize) < 0)
      {
      //U and V have different sign.  Need to compare them to determine
      //which operand to subtract from which.

      //This test is right since ABS_USIZE >= ABS_VSIZE.
      //If the equality case is ruled out, then U has more limbs
      //than V, which means that it is bigger in magnitude.
      if (abs_usize != abs_vsize)
         {
         GMP_INTS_mpn_sub (wp, up, abs_usize, vp, abs_vsize);
         wsize = abs_usize;

         //Normalize the result.  This was formerly a macro.
         //To normalize in this context means to trim the size
         //down to eliminate any leading zero limbs that came
         //about because the size of the result of an operation
         //was overestimated.
         GMP_INTS_mpn_normalize(wp, &wsize);

         if (usize < 0)
            wsize = -wsize;
         }
      else if (GMP_INTS_mpn_cmp (up, vp, abs_usize) < 0)
         {
         GMP_INTS_mpn_sub_n (wp, vp, up, abs_usize);
         wsize = abs_usize;
         GMP_INTS_mpn_normalize(wp, &wsize);
         if (usize >= 0)
            wsize = -wsize;
         }
      else
         {
         GMP_INTS_mpn_sub_n (wp, up, vp, abs_usize);
         wsize = abs_usize;
         GMP_INTS_mpn_normalize(wp, &wsize);
         if (usize < 0)
            wsize = -wsize;
         }
      }
   else
      {
      /* U and V have same sign.  Add them.  */
      GMP_INTS_limb_t cy_limb 
         = GMP_INTS_mpn_add (wp, up, abs_usize, vp, abs_vsize);
      wp[abs_usize] = cy_limb;
      wsize = abs_usize + cy_limb;
      if (usize < 0)
         wsize = -wsize;
      }

   w->size = wsize;

   //Handle the case of an overflowed result.  If the result
   //of the addition is too big or too small, mark it as
   //overflowed.
   if (w->size > GMP_INTS_MAXIMUM_LIMBS_PER_INT)
      {
      w->flags = GMP_INTS_EF_INTOVF_POS;
      }
   else if (w->size < -GMP_INTS_MAXIMUM_LIMBS_PER_INT)
      {
      w->flags = GMP_INTS_EF_INTOVF_NEG;
      }
   }


//07/15/01:  Unit testing skipped because of recursive
//nature.  Visual inspection OK.
void GMP_INTS_mpz_add_ui (      GMP_INTS_mpz_struct *w, 
                          const GMP_INTS_mpz_struct *u, 
                          unsigned long int          v)
   {
   //The GNU MP version of this is quite efficient, and this
   //makes sense since it is a common operation.  However,
   //for simplicity just define this recursively in terms
   //of the ADD function.  This can always be made quicker
   //later (by changing back to the GNU MP version).
   GMP_INTS_mpz_struct temp;

   //Eyeball the inputs carefully.
   assert(w != NULL);
   assert(w->n_allocd > 0);
   assert(w->limbs != NULL);
   assert(u != NULL);
   assert(u->n_allocd > 0);
   assert(u->limbs != NULL);

   //Create a temporary integer.
   GMP_INTS_mpz_init(&temp);

   //Set the temporary integer to the value of the input
   //argument.
   GMP_INTS_mpz_set_ui(&temp, v);

   //Do the actual addition.  This recursive definition
   //is inherently wasteful, but I'm after clarity, not
   //warp speed.
   GMP_INTS_mpz_add(w, u, &temp);

   //Destroy the temporary integer (this will reclaim the
   //memory).
   GMP_INTS_mpz_clear(&temp);
   }


//07/15/01:  Visual inspection passed.  Not unit tested
//because of symmetry with GMP_INTS_mpz_add().
void GMP_INTS_mpz_sub (      GMP_INTS_mpz_struct *w, 
                       const GMP_INTS_mpz_struct *u, 
                       const GMP_INTS_mpz_struct *v)
   {
   GMP_INTS_limb_srcptr up, vp;
   GMP_INTS_limb_ptr wp;
   GMP_INTS_size_t usize, vsize, wsize;
   GMP_INTS_size_t abs_usize;
   GMP_INTS_size_t abs_vsize;

   //Look at the input parameters carefully.
   assert(w != NULL);
   assert(u != NULL);
   assert(v != NULL);
   assert(w->n_allocd > 0);
   assert(u->n_allocd > 0);
   assert(v->n_allocd > 0);
   assert(w->limbs != NULL);
   assert(u->limbs != NULL);
   assert(v->limbs != NULL);

   //Handle the case of a tainted result.  If either of the
   //two inputs are either direct overflows or tainted by
   //an overflow, mark the result tainted and do not perform
   //any arithmetic operation.
      {
      int taint;

      taint = GMP_INTS_two_op_flags_map(u->flags, v->flags);

      w->flags = 0; 
         //"w" starts off with a clean slate.  Must do this
         //after taint calculation in case locations of u or v 
         //are the same as w.
      if (taint)
         {
         w->flags = taint;
         return;
         }
      }

   usize = u->size;
   vsize = -(v->size);  /* The "-" makes the difference from mpz_add */
   abs_usize = GMP_INTS_abs_of_size_t(usize);
   abs_vsize = GMP_INTS_abs_of_size_t(vsize);

   if (abs_usize < abs_vsize)
      {
      const GMP_INTS_mpz_struct *tmp_ptr;
      GMP_INTS_size_t      tmp_size;

      //Swap U and V.  This does no harm, because we are
      //manipulating only local variables.  This does not
      //affect the caller.
      tmp_ptr = u;
      u = v;
      v = tmp_ptr;
      tmp_size = usize;
      usize = vsize;
      vsize = tmp_size;
      tmp_size = abs_usize;
      abs_usize = abs_vsize;
      abs_vsize = tmp_size;
      }

   /* True: ABS_USIZE >= ABS_VSIZE.  */

   /* If not space for w (and possible carry), increase space.  */
   wsize = abs_usize + 1;
   if (w->n_allocd < wsize)
      GMP_INTS_mpz_realloc (w, wsize);

   /* These must be after realloc (u or v may be the same as w).  */
   up = u->limbs;
   vp = v->limbs;
   wp = w->limbs;

   if ((usize ^ vsize) < 0)
      {
      //U and V have different sign.  Need to compare them to determine
      //which operand to subtract from which.

      //This test is right since ABS_USIZE >= ABS_VSIZE.
      if (abs_usize != abs_vsize)
         {
         GMP_INTS_mpn_sub (wp, up, abs_usize, vp, abs_vsize);
         wsize = abs_usize;
         GMP_INTS_mpn_normalize(wp, &wsize);
         if (usize < 0)
            wsize = -wsize;
         }
      else if (GMP_INTS_mpn_cmp (up, vp, abs_usize) < 0)
         {
         GMP_INTS_mpn_sub_n (wp, vp, up, abs_usize);
         wsize = abs_usize;
         GMP_INTS_mpn_normalize(wp, &wsize);
         if (usize >= 0)
            wsize = -wsize;
         }
      else
         {
         GMP_INTS_mpn_sub_n (wp, up, vp, abs_usize);
         wsize = abs_usize;
         GMP_INTS_mpn_normalize (wp, &wsize);
         if (usize < 0)
            wsize = -wsize;
         }
      }
   else
      {
      /* U and V have same sign.  Add them.  */
      GMP_INTS_limb_t cy_limb 
         = GMP_INTS_mpn_add (wp, up, abs_usize, vp, abs_vsize);
      wp[abs_usize] = cy_limb;
      wsize = abs_usize + cy_limb;
      if (usize < 0)
         wsize = -wsize;
      }

   w->size = wsize;

   //Handle the case of an overflowed result.  If the result
   //of the addition is too big or too small, mark it as
   //overflowed.
   if (w->size > GMP_INTS_MAXIMUM_LIMBS_PER_INT)
      {
      w->flags = GMP_INTS_EF_INTOVF_POS;
      }
   else if (w->size < -GMP_INTS_MAXIMUM_LIMBS_PER_INT)
      {
      w->flags = GMP_INTS_EF_INTOVF_NEG;
      }
   }


//07/15/01:  Unit testing skipped because of recursive
//nature.  Visual inspection OK.
void GMP_INTS_mpz_sub_ui (      GMP_INTS_mpz_struct *w, 
                          const GMP_INTS_mpz_struct *u, 
                          unsigned long int          v)
   {
   //The GNU MP version of this is quite efficient, and this
   //makes sense since it is a common operation.  However,
   //for simplicity just define this recursively in terms
   //of the SUB function.  This can always be made quicker
   //later (by changing back to the GNU MP version).
   GMP_INTS_mpz_struct temp;

   //Eyeball the inputs carefully.
   assert(w != NULL);
   assert(w->n_allocd > 0);
   assert(w->limbs != NULL);
   assert(u != NULL);
   assert(u->n_allocd > 0);
   assert(u->limbs != NULL);

   //Create a temporary integer.
   GMP_INTS_mpz_init(&temp);

   //Set the temporary integer to the value of the input
   //argument.
   GMP_INTS_mpz_set_ui(&temp, v);

   //Do the actual subtraction.  This recursive definition
   //is inherently wasteful, but I'm after clarity, not
   //warp speed.
   GMP_INTS_mpz_sub(w, u, &temp);

   //Destroy the temporary integer (this will reclaim the
   //memory).
   GMP_INTS_mpz_clear(&temp);
   }


void GMP_INTS_mpz_mul (      GMP_INTS_mpz_struct *w, 
                       const GMP_INTS_mpz_struct *u, 
                       const GMP_INTS_mpz_struct *v)
   {
   GMP_INTS_size_t    usize = u->size;
   GMP_INTS_size_t    vsize = v->size;
   GMP_INTS_size_t    wsize;
   GMP_INTS_size_t    sign_product;
   GMP_INTS_limb_ptr  up, vp;
   GMP_INTS_limb_ptr  wp;
   GMP_INTS_limb_ptr  free_me = NULL;
   GMP_INTS_size_t    free_me_size;
   GMP_INTS_limb_t    cy_limb;

   //Eyeball the inputs.
   assert(w != NULL);
   assert(w->n_allocd > 0);
   assert(w->limbs != NULL);
   assert(u != NULL);
   assert(u->n_allocd > 0);
   assert(u->limbs != NULL);
   assert(v != NULL);
   assert(v->n_allocd > 0);
   assert(v->limbs != NULL);

   //Handle the case of a tainted result.  If either of the
   //two inputs are either direct overflows or tainted by
   //an overflow, mark the result tainted and do not perform
   //any arithmetic operation.
      {
      int taint;

      taint = GMP_INTS_two_op_flags_map(u->flags, v->flags);

      w->flags = 0; 
         //"w" starts off with a clean slate.  Must do this
         //after taint calculation in case locations of u or v 
         //are the same as w.
      if (taint)
         {
         w->flags = taint;
         return;
         }
      }

   sign_product = usize ^ vsize;
   usize = GMP_INTS_abs_of_size_t(usize);
   vsize = GMP_INTS_abs_of_size_t(vsize);

   //Handle the case of a certain result overflow (why do the math when
   //the result is certain?).  In general, when multiplying two inputs
   //whose sizes are M limbs and N limbs, the size of the result will be
   //either M+N or M+N-1 limbs.  If M+N-1 > MAX_ALLOWED, then can declare
   //an early overflow.
   if ((usize + vsize - 1) > GMP_INTS_MAXIMUM_LIMBS_PER_INT)
      {
      if (sign_product < 0)
         w->flags = GMP_INTS_EF_INTOVF_NEG;
      else 
         w->flags = GMP_INTS_EF_INTOVF_POS;

      return;
      }


   if (usize < vsize)
      {
      //Temporary variables just for the swap.
      const GMP_INTS_mpz_struct *tmp_ptr;
      GMP_INTS_size_t            tmp_size;

      //Swap U and V.
      tmp_ptr = u; 
      u = v; 
      v = tmp_ptr;
      tmp_size = usize; 
      usize = vsize; 
      vsize = tmp_size;
      }

   //Grab pointers to the arrays of limbs.
   up = u->limbs;
   vp = v->limbs;
   wp = w->limbs;

   /* Ensure W has space enough to store the result.  */
   wsize = usize + vsize;
   if (w->n_allocd < wsize)
      {
      if (wp == up || wp == vp)
         {
         free_me = wp;
         free_me_size = w->n_allocd;
         }
      else
         {
         GMP_INTS_free_w_size (wp, w->n_allocd * sizeof(GMP_INTS_limb_t));
         }

      w->n_allocd = wsize;
      wp = (GMP_INTS_limb_ptr) 
           GMP_INTS_malloc (wsize * sizeof(GMP_INTS_limb_t));
      w->limbs = wp;
      }
   else
      {
      /* Make U and V not overlap with W.  */
      if (wp == up)
         {
         /* W and U are identical.  Allocate temporary space for U.  */
         up = (GMP_INTS_limb_ptr) 
              _alloca(usize * sizeof(GMP_INTS_limb_t));
         /* Is V identical too?  Keep it identical with U.  */
         if (wp == vp)
            vp = up;
         /* Copy to the temporary space.  */
         GMP_INTS_mpn_copy_limbs(up, wp, usize);
         }
      else if (wp == vp)
         {
         /* W and V are identical.  Allocate temporary space for V.  */
         vp = (GMP_INTS_limb_ptr) 
              _alloca(vsize * sizeof(GMP_INTS_limb_t));
         /* Copy to the temporary space.  */
         GMP_INTS_mpn_copy_limbs(vp, wp, vsize);
         }
      }

   if (vsize == 0)
      {
      wsize = 0;
      }
   else
      {
      cy_limb = GMP_INTS_mpn_mul (wp, up, usize, vp, vsize);
      wsize = usize + vsize;
      wsize -= cy_limb == 0;
      }

   w->size = sign_product < 0 ? -wsize : wsize;

   if (free_me != NULL)
      GMP_INTS_free_w_size (free_me, free_me_size * sizeof(GMP_INTS_limb_t));

   //Final check for overflow.
   if (w->size > GMP_INTS_MAXIMUM_LIMBS_PER_INT)
      w->flags = GMP_INTS_EF_INTOVF_POS;
   else if (w->size < -GMP_INTS_MAXIMUM_LIMBS_PER_INT)
      w->flags = GMP_INTS_EF_INTOVF_NEG;
   }


//07/15/01:  Unit testing skipped because of recursive
//nature.  Visual inspection OK.
void GMP_INTS_mpz_mul_si (      GMP_INTS_mpz_struct *w, 
                          const GMP_INTS_mpz_struct *u, 
                                long int             v)
   {
   GMP_INTS_mpz_struct temp;

   //Eyeball the inputs carefully.
   assert(w != NULL);
   assert(w->n_allocd > 0);
   assert(w->limbs != NULL);
   assert(u != NULL);
   assert(u->n_allocd > 0);
   assert(u->limbs != NULL);

   //Create a temporary integer.
   GMP_INTS_mpz_init(&temp);

   //Set the temporary integer to the value of the input
   //argument.
   GMP_INTS_mpz_set_si(&temp, v);

   //Do the actual multiplication.  This recursive definition
   //is inherently wasteful, but I'm after clarity, not
   //warp speed.
   GMP_INTS_mpz_mul(w, u, &temp);

   //Destroy the temporary integer (this will reclaim the
   //memory).
   GMP_INTS_mpz_clear(&temp);
   }


//07/15/01:  Unit testing skipped because of recursive
//nature.  Visual inspection OK.
void GMP_INTS_mpz_mul_ui (      GMP_INTS_mpz_struct *w, 
                          const GMP_INTS_mpz_struct *u, 
                          unsigned long int          v)
   {
   GMP_INTS_mpz_struct temp;

   //Eyeball the inputs carefully.
   assert(w != NULL);
   assert(w->size >= 0);
   assert(w->limbs != NULL);
   assert(u != NULL);
   assert(u->size >= 0);
   assert(u->limbs != NULL);

   //Create a temporary integer.
   GMP_INTS_mpz_init(&temp);

   //Set the temporary integer to the value of the input
   //argument.
   GMP_INTS_mpz_set_ui(&temp, v);

   //Do the actual multiplication.  This recursive definition
   //is inherently wasteful, but I'm after clarity, not
   //warp speed.
   GMP_INTS_mpz_mul(w, u, &temp);

   //Destroy the temporary integer (this will reclaim the
   //memory).
   GMP_INTS_mpz_clear(&temp);
   }


void GMP_INTS_mpz_tdiv_qr (      GMP_INTS_mpz_struct *quot, 
                                 GMP_INTS_mpz_struct *rem, 
                           const GMP_INTS_mpz_struct *num, 
                           const GMP_INTS_mpz_struct *den)
   {
   GMP_INTS_size_t   abs_num_size, 
                     abs_den_size, 
                     quotient_sign, 
                     remainder_sign,
                     numerator_bitsize, 
                     denominator_bitsize,
                     division_loop_count,
                     division_loop_count_mod_32,
                     division_loop_count_div_32,
                     division_counter,
                     i;
   GMP_INTS_limb_t   temp_limb;
   GMP_INTS_limb_ptr trial_divisor;

   //Eyeball the input parameters.
   assert(quot != NULL);
   assert(quot->n_allocd > 0);
   assert(quot->limbs != NULL);
   assert(rem != NULL);
   assert(rem->n_allocd > 0);
   assert(rem->limbs != NULL);
   assert(num != NULL);
   assert(num->n_allocd > 0);
   assert(num->limbs != NULL);
   assert(den != NULL);
   assert(den->n_allocd > 0);
   assert(den->limbs != NULL);

   //We require for this function that the numerator, denominator, quotient, and
   //remainder all be distinct.
   assert(quot != rem);
   assert(quot != num);
   assert(quot != den);
   assert(rem  != num);
   assert(rem  != den);
   assert(num  != den);

   //The GNU code was probably very efficient, but exceeded
   //my abilities to analyze.  This is the classic
   //division algorithm.

   //First, start off with the quotient and remainder having
   //no error flags set.  These will be set if appropriate.
   quot->flags = 0; 
   rem->flags  = 0;

   //First, handle tainted inputs.  If the numerator or denominator
   //are bad or tainted, the quotient and remainder get tainted
   //automatically.
      {
      int taint;

      taint = GMP_INTS_two_op_flags_map(num->flags, den->flags);

      if (taint)
         {
         quot->flags = taint;
         rem->flags  = taint;
         return;
         }
      }

   //The second possible cause for taint is if the divisor is
   //zero.  This will get both the value of positive overflow.
   if (den->size == 0)
      {
      quot->flags = GMP_INTS_EF_INTOVF_POS;
      rem->flags  = GMP_INTS_EF_INTOVF_POS;
      return;
      }

   //Handle the special case of a numerator of zero.  If the numerator
   //is zero, the quotient and remainder are zero automatically.
   if (num->size == 0)
      {
      GMP_INTS_mpz_set_ui(quot, 0);
      GMP_INTS_mpz_set_ui(rem,  0);
      return;
      }

   //Generally, nothing else can go wrong as far as taint.  The
   //value of the quotient is confined to be no larger than the
   //numerator, and the value of the remainder is confined to
   //be no larger than denominator-1.  So, generally, if the
   //inputs are in size bounds, the outputs will be also.

   //Figure out how large in limbs the numerator and denominator actually
   //are.
   abs_num_size = GMP_INTS_abs_of_size_t(num->size);
   abs_den_size = GMP_INTS_abs_of_size_t(den->size);

   //Figure out the sign of things.  We want the following relationship
   //to be true:
   //   num/den = quot + rem/den.
   //The way to achieve this is to assign the sign of the quotient in the traditional
   //way, then to assign the remainder to have the same sign as the numerator.
   quotient_sign  = num->size ^ den->size;
   remainder_sign = num->size;

   //The remainder starts off with the absolute value of the numerator, and then
   //we subtract from it as part of the division loop.
   GMP_INTS_mpz_copy(rem, num);
      //We know after the copy that the amount of space allocated in the remainder
      //MUST be at least as large as the absolute value of the numerator.  So from
      //this point forward we use the space.
   assert(rem->n_allocd >= abs_num_size);

   //Figure out the number of significant bits in the numerator and denominator.
   //This determines the loop count over which we do the shift division loop.
   numerator_bitsize = GMP_INTS_BITS_PER_LIMB * abs_num_size;
   
   i = abs_num_size - 1;

   //We need to be extra careful here.  One failure mode is that an integer
   //data structure is corrupted and the "size" field reflects limbs
   //that are zero.  Need to watch that this kind of failure doesn't
   //cause memory access errors.
   assert(num->limbs[i] != 0);
   if (num->limbs[i] == 0)
      {
      quot->flags = GMP_INTS_EF_INTOVF_POS;
      rem->flags  = GMP_INTS_EF_INTOVF_POS;
      return;
      }

   temp_limb = 0x80000000;

   while (((num->limbs[i] & temp_limb) == 0) && (temp_limb != 0))
      {
      numerator_bitsize--;
      temp_limb >>= 1;
      }

   denominator_bitsize = GMP_INTS_BITS_PER_LIMB * abs_den_size;

   i = abs_den_size - 1;

   //We need to be extra careful here.  One failure mode is that an integer
   //data structure is corrupted and the "size" field reflects limbs
   //that are zero.  Need to watch that this kind of failure doesn't
   //cause memory access errors.
   assert(den->limbs[i] != 0);
   if (den->limbs[i] == 0)
      {
      quot->flags = GMP_INTS_EF_INTOVF_POS;
      rem->flags  = GMP_INTS_EF_INTOVF_POS;
      return;
      }

   temp_limb = 0x80000000;

   while (((den->limbs[i] & temp_limb) == 0) && (temp_limb != 0))
      {
      denominator_bitsize--;
      temp_limb >>= 1;
      }

   //The quotient starts off with the value of zero, but we consistently may 
   //mask 1 into it and shift left.  We need to be sure that we have as much
   //shift space there as is in the numerator.  For this purpose we need to 
   //prepare a block of clear memory as large as the numerator's.
   if (quot->n_allocd < abs_num_size)
      {
      GMP_INTS_mpz_realloc(quot, abs_num_size);  //Make it big enough.
      }
   //Now, zero the memory.
   for (i=0; i<abs_num_size; i++)
      quot->limbs[i] = 0;

   //Determine the division loop count.  This is the difference
   //in bit sizes between the numerator and denominator.  It is 
   //possible for this number to be negative, which means that the
   //main division loop will be executed zero times.  This gives the
   //right results.
   division_loop_count = numerator_bitsize - denominator_bitsize;

   //We need to calculate some important numbers from the division loop
   //count.  We need to know this number MOD 32 (which tells how far to
   //shift the divisor bitwise to line up with the numerator), and we 
   //also need this number DIV 32 for the limb-wise shift.
   division_loop_count_mod_32 = division_loop_count % 32;
   division_loop_count_div_32 = division_loop_count / 32;
   
   //We now need a shift register in which we shift the denominator up
   //for repeated comparisons.  We should dynamically allocate this to
   //be the same size as the numerator.  Using _alloca() is OK, as one
   //of the unit tests is to be sure that _alloca() will handle integer
   //of the maximum allowed size.
   trial_divisor = _alloca(abs_num_size * sizeof(GMP_INTS_limb_t));

   //Our trial divisor needs to start off with the divisor shifted up
   //so that the most significant bit is aligned with the numerator.
   for (i = 0; i < abs_num_size; i++)
      {
      if ((division_loop_count < 0) || (i < division_loop_count_div_32))
         {
         trial_divisor[i] = 0;
         }
      else
         {
         if ((i-division_loop_count_div_32) < abs_den_size)
            trial_divisor[i] = den->limbs[i - division_loop_count_div_32];
         else
            trial_divisor[i] = 0;
         }
      }

   //The code above planted the limbs in the right place.  Now need to shift bits
   //upward by the remaining number.
   if ((division_loop_count > 0) && (division_loop_count_mod_32 > 0))
      {
      //There is an existing function we can call to do the left shift.
      GMP_INTS_mpn_lshift(trial_divisor, 
                          trial_divisor, 
                          abs_num_size, 
                          division_loop_count_mod_32);
      }


   //Everything is ready to go.  Now begin the division loop itself.  It is possible
   //for the loop to execute zero times, which will happen if the denominator is longer
   //in bits than the numerator.  In such cases, we can't execute this loop even once
   //because the math assumes that the numerator is at least as long as the denominator.
   for (division_counter = 0; division_counter < division_loop_count+1; division_counter++)
      {
      //Shift the quotient left one bit.
      GMP_INTS_mpn_lshift(quot->limbs, 
                          quot->limbs, 
                          abs_num_size, 
                          1);

      //If the remainder is at least as large as the trial divisor, subtract the trial
      //divisor from the remainder and mask in the quotient.
      if (GMP_INTS_mpn_cmp(rem->limbs, 
                           trial_divisor,
                           abs_num_size) >= 0)
         {
         GMP_INTS_mpn_sub(rem->limbs,
                          rem->limbs,
                          abs_num_size,
                          trial_divisor,
                          abs_num_size);
         quot->limbs[0] |= 1;
         }

      //Shift the trial divisor right one bit.
      GMP_INTS_mpn_rshift(trial_divisor, 
                          trial_divisor, 
                          abs_num_size, 
                          1);
      }  //End for each iteration of the division loop.

   //Normalize the quotient and the remainder.  The normalization
   //process is to bring the sizes down if we have leading
   //zeros.
   quot->size = abs_num_size;
   GMP_INTS_mpn_normalize(quot->limbs, &(quot->size));
   rem->size  = abs_num_size;
   GMP_INTS_mpn_normalize(rem->limbs, &(rem->size));

   //Adjust the signs as required.
   if (quotient_sign < 0)
      quot->size = -(quot->size);
   if (remainder_sign < 0)
      rem->size = -(rem->size);
   }


void GMP_INTS_mpz_fac_ui(GMP_INTS_mpz_struct *result, 
                         unsigned long int n)
   {
   //Just multiply the numbers in ascending order.  The original
   //GNU library contained a much more elegant algorithm, but
   //this is more direct.

   unsigned long int k;

   GMP_INTS_mpz_set_ui (result, 1L);

   for (k = 2; (k <= n) && !(result->flags); k++)
      GMP_INTS_mpz_mul_ui (result, result, k);
   }


/******************************************************************/
/***  PUBLIC CONVERSION AND OUTPUT FUNCTIONS   ********************/
/******************************************************************/
//07/18/01:  Visual inspection OK.  Function returns
//reasonable values even out to 100,000 digits--seems OK.
int GMP_INTS_mpz_size_in_base_10(const GMP_INTS_mpz_struct *arg)
   {
   _int64 n;

   //Eyeball the input parameter.
   assert(arg != NULL);
   assert(arg->n_allocd > 0);
   assert(arg->limbs != NULL);

   //Get the number of limbs occupied by the integer.
   //Because even the digit zero takes some space,
   //don't accept zero for an answer.
   n = GMP_INTS_abs_of_size_t(arg->size);
   if (n==0)
      n = 1;
   
   //Convert this to the number of bits.  Generously 
   //ignore any unused leading bits.
   n *= 32;

   //Used a slightly high best rational approximation in F_{65535}
   //to go from the number of bits to the number of 
   //digits.  The division discards, so bump the result
   //up by 1 to compensate for possible truncation.  The number
   //we are aproximating is ln(2)/ln(10).
   n *= 12655;
   n /= 42039;
   n++;

   //Compensate for possible commas in the result.  Again,
   //consider truncation.
   n *= 4;
   n /= 3;
   n++;

   //Compensate for the minus sign, the trailing zero,
   //cosmic rays striking the computer from the martian
   //listening post camoflaged on the moon, and the
   //possibility that we might need to put text in the
   //string if any flag is set.
   n += 100;

   //And that should be a good return value.
   return((int) n);
   }


//07/19/01:  Visual inspection and unit test is OK.
void GMP_INTS_mpz_to_string(char *out,
                            const GMP_INTS_mpz_struct *in)
   {
   //Eyeball the input parameters.
   assert(out != NULL);
   assert(in != NULL);
   assert(in->n_allocd > 0);
   assert(in->limbs != NULL);

   //If any of the flags are set, stuff in the text.
   if (in->flags)
      {
      if (in->flags & GMP_INTS_EF_INTOVF_POS)
         {
         strcpy(out, GMP_INTS_EF_INTOVF_POS_STRING);
         }
      else if (in->flags & GMP_INTS_EF_INTOVF_NEG)
         {
         strcpy(out, GMP_INTS_EF_INTOVF_NEG_STRING);
         }
      else if (in->flags & GMP_INTS_EF_INTOVF_TAINT_POS)
         {
         strcpy(out, GMP_INTS_EF_INTOVF_TAINT_POS_STRING);
         }
      else if (in->flags & GMP_INTS_EF_INTOVF_TAINT_NEG)
         {
         strcpy(out, GMP_INTS_EF_INTOVF_TAINT_NEG_STRING);
         }
      else
         {
         strcpy(out, "INTERNAL_ERROR");
         }
      }  
   else
      {
      //Ordinary integer conversion.
      GMP_INTS_mpz_struct num, den, quot, rem, k10;

      //Allocate space for the temporary integers.
      GMP_INTS_mpz_init(&num);
      GMP_INTS_mpz_init(&den);
      GMP_INTS_mpz_init(&quot);
      GMP_INTS_mpz_init(&rem);
      GMP_INTS_mpz_init(&k10);

      //Assign the constant 10.
      GMP_INTS_mpz_set_ui(&k10, 10);

      //If the integer is zero, assign that.
      if (in->size == 0)
         {
         strcpy(out, "0");
         }
      else
         {
         //We have to do a full conversion.  The algorithm
         //is division by 10, each time obtaining the least
         //significant digit, until finally the quotient is 
         //zero.
         char *ptr;

         ptr = out;

         GMP_INTS_mpz_copy(&num, in);
         GMP_INTS_mpz_copy(&den, &k10);
         do
            {
#if 0
            printf("Values before division:\n");
            FCMIOF_hline();
            GMP_INTS_mpz_print_int(stdout, &num, "Numerator");
            FCMIOF_hline();
            GMP_INTS_mpz_print_int(stdout, &den, "Denominator");
            FCMIOF_hline();
            GMP_INTS_mpz_print_int(stdout, &quot, "Quotient");
            FCMIOF_hline();
            GMP_INTS_mpz_print_int(stdout, &rem, "Remainder");
            FCMIOF_hline();

if (num.size > 1)
            FCMIOF_hline();
#endif

            GMP_INTS_mpz_tdiv_qr(&quot, &rem, &num, &den);
#if 0
            printf("Values after division:\n");
            FCMIOF_hline();
            GMP_INTS_mpz_print_int(stdout, &num, "Numerator");
            FCMIOF_hline();
            GMP_INTS_mpz_print_int(stdout, &den, "Denominator");
            FCMIOF_hline();
            GMP_INTS_mpz_print_int(stdout, &quot, "Quotient");
            FCMIOF_hline();
            GMP_INTS_mpz_print_int(stdout, &rem, "Remainder");
            FCMIOF_hline();
#endif

            if (rem.size != 0)
               {
               *ptr = '0' + (char)(rem.limbs[0]);
               }
            else
               {
               *ptr = '0';
               }
            ptr++;
            GMP_INTS_mpz_copy(&num, &quot);
            //printf("digit\n");
            }
         while (!GMP_INTS_mpz_is_zero(&quot));

         //Finally, if the input was negative, tack on the 
         //minus sign.
         if (GMP_INTS_mpz_is_neg(in))
            {
            *ptr = '-';
            ptr++;
            }
        
         //Finally, tack on the trailing zero terminator.
         *ptr = 0;
         ptr++;

         //Reverse the string.
         BSTRFUNC_str_reverse(out);
         }
 
      //Deallocate the integers.
      GMP_INTS_mpz_clear(&num);
      GMP_INTS_mpz_clear(&den);
      GMP_INTS_mpz_clear(&quot);
      GMP_INTS_mpz_clear(&rem);
      GMP_INTS_mpz_clear(&k10);
      } 
   }


void GMP_INTS_mpz_long_int_format_to_stream(FILE *s,
                                            const GMP_INTS_mpz_struct *i,
                                            const char *desc)
   {
   int line_len;
   int digits_per_line;
   char *digits;
   int num_digits;
   int nlines;
   int cur_line;
   int number_desc_width;

   //Eyeball the inputs, make sure the caller isn't doing
   //something stupid.
   assert(s != NULL);
   assert(i != NULL);
   assert(i->n_allocd > 0);
   assert(i->limbs != NULL);
   assert(desc != NULL);

   //Obtain the line length assumed for formatted output.
   line_len = FCMIOF_get_line_len();

   //The description width allowed is 20.
   number_desc_width = 20;

   /* The number of digits per line that we assume must be a multiple of
   ** three.  The formula below was not examined very carefully, but it
   ** works fine for a line length of 78.  If line length is changed,
   ** this formula may need to be examined very carefully and rewritten.
   */
   digits_per_line = INTFUNC_max(3, ((((line_len-42)*3)/4)/3)*3);
   assert(digits_per_line >= 3);

   /* We now need to get a digit string corresponding to this 
   ** number.  First, need to figure out how much and
   ** allocate the space.
   */
   digits = GMP_INTS_malloc(GMP_INTS_mpz_size_in_base_10(i) * sizeof(char));
   GMP_INTS_mpz_to_string(digits, i);

   //If the number is negative, delete the leading minus sign.
   //The rest of the display algorithm needs an unsigned
   //series of digits.
   if (*digits == '-')
      {
      int i = 0;

      do
         {
         digits[i] = digits[i+1];
         i++;
         }
      while(digits[i-1]);
      }

   //Figure out how many digits in the string representation.
   num_digits = strlen(digits);

   /* As the first order of business, figure out how many lines the beast
   ** will require.
   */
   if (i->flags)
      {
      nlines = 1;  /* Only one line required for NAN verbeage. */
      }
   else if (GMP_INTS_mpz_is_zero(i))
      {
      nlines = 1;  /* The zero value requires one line. */
      }
   else
      {
      /* In any other case, have a formula.
      */
      nlines = 1 + (num_digits - 1) / digits_per_line;
      }

   /* Iterate through each line, spitting out whatever is appropriate. */
   for (cur_line = 0; cur_line < nlines; cur_line++)
      {
      int cur_digit_on_line;

      /* If this is the first line, spit out the description, right-aligned.
      ** Otherwise, spit spaces.
      */
      if (!cur_line)
         {
         /* First line. */
         int len;

         len = strlen(desc);

         if (len <= number_desc_width)
            {
            /* Description is shorter or equal, pad on left. */
            FCMIOF_stream_repchar(s, ' ', number_desc_width - len);
            fprintf(s, "%s", desc);
            }
         else
            {
            /* Description is too long, truncate. */
            int i;

            for (i=0; i<number_desc_width; i++)
               fprintf(s, "%c", desc[i]);
            }

         fprintf(s, ": ");

         /* If the number is negative, throw in a minus sign. */
         if (GMP_INTS_mpz_is_neg(i) && !(i->flags))
            {
            fprintf(s, "- ");
            }
         else
            {
            fprintf(s, "  ");
            }
         }
      else
         {
         /* Every line but first line. */
         FCMIOF_stream_repchar(s, ' ', number_desc_width+4);
         }

      for(cur_digit_on_line=0; cur_digit_on_line < digits_per_line; cur_digit_on_line++)
         {
         int idx_into_string;
            /* Index into the string which is our digit of interest.
            */

         /* Compute the index.  The equation is based on the ordering
         ** of presentation, for example,
         **
         **            7 6
         **          5 4 3
         **          2 1 0.
         **
         ** With a little thought, the equation should make sense.
         ** The index won't always be used to index into the string.
         */
         idx_into_string = 
           ((((nlines-1) - cur_line) * digits_per_line)
           +
           (digits_per_line - 1 - cur_digit_on_line));

         /* Print the appropriate digit or a space.  The NAN case and the 
         ** zero case need to be treated specially.
         */
         if (i->flags)
            {
            /* Not a number.  Everything is blank, except spell out
            ** description of condition at the end of the string of 
            ** digits.
            */
            int index_from_right;
            int virtual_index;

            index_from_right = digits_per_line - 1 - cur_digit_on_line;
               //The index calculated above is calculated so that the
               //final position on the line has index [0].
            assert(index_from_right >= 0 && index_from_right < digits_per_line);

            //Now, calculate the "virtual index".  The virtual index
            //is the actual number of characters from the right, taking
            //into account commas.
            virtual_index = index_from_right + index_from_right/3;

            if (((index_from_right % 3) == 2) && cur_digit_on_line)
               {
               //We are one position past a comma.  This means
               //that we might need a "fill" character to go
               //where the comma should have gone.

               if (virtual_index + 1 < num_digits)
                  {
                  //The character we should print exists.
                  fprintf(s, "%c", digits[num_digits - 2 - virtual_index]);
                  }
               else
                  {
                  //The character doesn't exist, because the error
                  //string is apparently too short.  Must print a
                  //space, instead.
                  fprintf(s, " ");
                  }
               }

            //We've done the fill character, if the position we're in
            //is one past a comma.  Now, do the ordinary character
            //corresponding to a digit position. 
            if (virtual_index < num_digits)
               {
               //The character we should print exists.
               fprintf(s, "%c", digits[num_digits - 1 - virtual_index]);
               }
            else
               {
               //The character doesn't exist, because the error
               //string is apparently too short.  Must print a
               //space, instead.
               fprintf(s, " ");
               }
            }
         else if (GMP_INTS_mpz_is_zero(i))
            {
            /* This is the zero case.  For zero, there is only one line,
            ** and every character except the last one is a blank.
            */
            if (cur_digit_on_line == (digits_per_line - 1))
               {
               fprintf(s, "0");
               }
            else
               {
               fprintf(s, " ");
               }
            }
         else
            {
            /* This is a valid number which is not zero.  Need to print
            ** the digits.
            */

            if (idx_into_string < num_digits)
               {
               int actual_index;

               actual_index = num_digits - 1 - idx_into_string;
                  //This is a string reversal mapping.  The original
                  //code stored strings least significant digit first,
                  //but this code uses most significant digit first.
               assert((actual_index >= 0) && (actual_index < num_digits));
               fprintf(s, "%c", digits[actual_index]);
               }
            else
               {
               fprintf(s, " ");
               }
            } /* End of digit case.

         /* Now handle the commas.  The rules for commas are straightforward.
         **    a)NAN never has a comma.
         **    b)Zeros never have a comma.
         **    c)The final line, last digit never has a comma.
         **    d)Everything else in multiples of three ...
         */
         if (!(idx_into_string % 3) && (idx_into_string))
            {
            if (i->flags)
               {
               //fprintf(s, " ");
               }
            else if (!num_digits)
               {
               fprintf(s, " ");
               }
            else
               {
               if (idx_into_string < num_digits)
                  {
                  fprintf(s, ",");
                  }
               else
                  {
                  fprintf(s, " ");
                  }
               }
            }
         } /* End for each digit on the current line. */
      
      /* For the first line, print out an informative message
      ** advising of the number of digits.  For all other lines
      ** print nothing.
      */
      if (!cur_line && !(i->flags))
         {
         if (nlines == 1)
            fprintf(s, " ");

         if (num_digits <= 1)
            {
            fprintf(s, "  (      1 digit )\n");
            }
         else if (num_digits < 1000)
            {
            fprintf(s, "  (%7d digits)\n", num_digits);
            }
         else
            {
            fprintf(s, "  (%3d,%03d digits)\n", num_digits / 1000, num_digits % 1000);
            }
         }
      else
         {
         fprintf(s, "\n");
         }
      } /* End for each line. */

   //Deallocate the string space.
   GMP_INTS_free(digits);
   }


void GMP_INTS_mpz_arb_int_raw_to_stream(FILE *s,
                                        const GMP_INTS_mpz_struct *i)
   {
   int size_reqd;
   char *digits;

   //Eyeball the input parameters.
   assert(s != NULL);
   assert(i != NULL);
   assert(i->n_allocd > 0);
   assert(i->limbs != NULL);

   size_reqd = GMP_INTS_mpz_size_in_base_10(i);
   digits = GMP_INTS_malloc(size_reqd * sizeof(char));
   GMP_INTS_mpz_to_string(digits, i);
   fprintf(s, "%s", digits);
   GMP_INTS_free(digits);
   }


//07/24/01:  Passed visual inspection and unit tests.
void GMP_INTS_mpz_pow_ui(         GMP_INTS_mpz_struct *result,
                         const    GMP_INTS_mpz_struct *base,
                         unsigned                      exponent)
   {
   GMP_INTS_mpz_struct temp;
      //Temporary location to hold the base raised to
      //a binary power (repeated squaring).

   //Eyeball the input parameters.
   assert(result != NULL);
   assert(result->n_allocd > 0);
   assert(result->limbs != NULL);
   assert(base != NULL);
   assert(base->n_allocd > 0);
   assert(base->limbs != NULL);

   //For this function, the base and the result may not
   //be the same object.
   assert(result != base);

   //If the base is tained, the output is tainted by association.
      {
      int taint;

      taint = GMP_INTS_two_op_flags_map(base->flags, 0);

      if (taint)
         {
         result->flags = taint;
         return;
         }
      }

   //Allocate our temporary variable and set it to the base.
   GMP_INTS_mpz_init(&temp);
   GMP_INTS_mpz_copy(&temp, base);

   //The result begins with the value of 1.
   GMP_INTS_mpz_set_ui(result, 1);

   //Loop through, processing each bit of the exponent.  This is a fairly effective
   //algorithm, but not the optimal one (Knuth points this out).
   while (exponent && !result->flags)
      {
      if (exponent & 0x1)
         {
         GMP_INTS_mpz_mul(result, result, &temp);
         }

      //Square the temporary variable.  Because squaring of arb integer
      //may be very expensive, the test against 1 (i.e. last iteration)
      //certainly pays for itself.
      if (exponent != 1)
         GMP_INTS_mpz_mul(&temp, &temp, &temp);
     
      exponent >>= 1;
      }

   //Deallocate our temporary variable.
   GMP_INTS_mpz_clear(&temp);
   }


void GMP_INTS_mpz_abs(GMP_INTS_mpz_struct *arg)
   {
   //Eyeball the input parameter.
   assert(arg != NULL);
   assert(arg->n_allocd > 0);
   assert(arg->limbs != NULL);

   //Take the absolute value.
   if (arg->size < 0)
      arg->size = -arg->size;
   }


//07/29/01:  Visual inspection passed.  Seems to work fine--not explicitly unit-tested
//directly, but was tested from Tcl.
void GMP_INTS_mpz_gcd(GMP_INTS_mpz_struct       *result,
                      const GMP_INTS_mpz_struct *arg1,
                      const GMP_INTS_mpz_struct *arg2)
   {
   GMP_INTS_mpz_struct u, v, q, r;
   int loop_count;

   //Eyeball the inputs carefully.
   assert(result != NULL);
   assert(result->n_allocd > 0);
   assert(result->limbs != NULL);
   assert(arg1 != NULL);
   assert(arg1->n_allocd > 0);
   assert(arg1->limbs != NULL);
   assert(arg2 != NULL);
   assert(arg2->n_allocd > 0);
   assert(arg2->limbs != NULL);

   //Args are not allowed to be same object.
   assert(arg1 != arg2);

   //If either input is error or taint, taint the output.
      {
      int taint;

      taint = GMP_INTS_two_op_flags_map(arg1->flags, arg2->flags);

      result->flags = 0; 
         //"result" starts off with a clean slate.  Must do this
         //after taint calculation in case locations of arg1 or arg2 
         //are the same as result.
      if (taint)
         {
         result->flags = taint;
         return;
         }
      }

   //If either input is zero, the result is 1.
   if (GMP_INTS_mpz_is_zero(arg1) || GMP_INTS_mpz_is_zero(arg2))
      {
      GMP_INTS_mpz_set_ui(result, 1);
      return;
      }

   //Allocate space for locals.
   GMP_INTS_mpz_init(&u);
   GMP_INTS_mpz_init(&v);
   GMP_INTS_mpz_init(&q);
   GMP_INTS_mpz_init(&r);

   //We are following Knuth Vol 2, p. 337, the modern Euclidian algorithm.
   //Note:  There are faster algorithms for GCD, but because I hacked up the
   //GMP multiple-precision library so badly, those aren't included.  This one
   //is logically correct but sub-optimal.  Perhaps at a later time faster 
   //algorithms will be re-included.
   //Copy inputs to u and v.
   GMP_INTS_mpz_copy(&u, arg1);
   GMP_INTS_mpz_copy(&v, arg2);
   
   //Take the absolute value of each argument.  We know that neither is zero, 
   //but one or both might be negative.
   GMP_INTS_mpz_abs(&u);
   GMP_INTS_mpz_abs(&v);

   //Begin Euclid's algorithm.  There are really three possibilities:
   //  a)We terminate normally.
   //  b)Somehow we generate a math error and terminate based on flags.
   //  c)Due to some unknown error in the math functions, we go on forever,
   //    and the program locks up.
   GMP_INTS_mpz_set_ui(&r, 1);
   loop_count = 0;
   while (!GMP_INTS_mpz_is_zero(&r) && !q.flags && !r.flags && (loop_count < 100000))
      {
      loop_count++;

      GMP_INTS_mpz_tdiv_qr(&q, &r, &u, &v);
      GMP_INTS_mpz_copy(&u, &v);
      GMP_INTS_mpz_copy(&v, &r);
      }

   //Let's hope we didn't get out of the loop based on loop count.
   assert(loop_count != 100000);

   //u now contains the answer.
   GMP_INTS_mpz_copy(result, &u);

   //Deallocate space for locals.
   GMP_INTS_mpz_clear(&u);
   GMP_INTS_mpz_clear(&v);
   GMP_INTS_mpz_clear(&q);
   GMP_INTS_mpz_clear(&r);
   }


/******************************************************************/
/***  COMPARISON AND SIZING FUNCTIONS   ***************************/
/******************************************************************/
//07/24/01:  Visual inspection only, due to simplicity.
int GMP_INTS_mpz_fits_uint_p (const GMP_INTS_mpz_struct *src)
   {
   GMP_INTS_size_t size;
   GMP_INTS_limb_t mpl;

   //Eyeball the input parameter.
   assert(src != NULL);
   assert(src->n_allocd > 0);
   assert(src->limbs != NULL);

   mpl =  src->limbs[0];
   size = src->size;
   if (size < 0 || size > 1)
     return(0);

   //The following line came from the original GNU code.
   //It isn't necessary in our case since limbs and ints are
   //both 32 bits, but it will do no harm.
   return (mpl <= (~(unsigned int) 0));
   }


unsigned GMP_INTS_mpz_get_limb_zero(const GMP_INTS_mpz_struct *src)
   {
   //Eyeball the inputs.
   assert(src != NULL);
   assert(src->n_allocd > 0);
   assert(src->limbs != NULL);

   if (!src->size)
      return(0);
   else
      return(src->limbs[0]);
   }


//07/24/01:  Visual inspection only.  Understood the comparisons
//and seems like they should work, but ... a little beyond my
//comfort zone without testing.  Trusting GNU on this one ...
int GMP_INTS_mpz_fits_sint_p (const GMP_INTS_mpz_struct *src)
   {
   GMP_INTS_size_t size;
   GMP_INTS_limb_t mpl;

   //Eyeball the input parameter.
   assert(src != NULL);
   assert(src->n_allocd > 0);
   assert(src->limbs != NULL);

   mpl  = src->limbs[0];
   size = src->size;
   if (size > 0)
      {
      if (size > 1)
	      return 0;
      return mpl < ~((~(unsigned int) 0) >> 1);
      }
   else
      {
      if (size < -1)
	     return 0;
      return mpl <= ~((~(unsigned int) 0) >> 1);
      }
   }


//07/24/01:  Visual inspection only.  One issue that caught
//my eye is that in one place function returned neg value if
//< and pos value if >.  Was within spec, but corrected because
//it concerned me as I often test against -1 and 1.  Seems
//to invite accidents.
int GMP_INTS_mpz_cmp (const GMP_INTS_mpz_struct *u, 
                      const GMP_INTS_mpz_struct *v)
   {
   GMP_INTS_size_t usize = u->size;
   GMP_INTS_size_t vsize = v->size;
   GMP_INTS_size_t size;
   GMP_INTS_limb_srcptr up, vp;
   int cmp;

   //Eyeball the input parameters.
   assert(u != NULL);
   assert(u->n_allocd > 0);
   assert(u->limbs != NULL);
   assert(v != NULL);
   assert(v->n_allocd > 0);
   assert(v->limbs != NULL);

   if (usize < vsize)
      return(-1);
   else if (usize > vsize)
      return(1);

   if (usize == 0)
      return(0);

   size = GMP_INTS_abs_of_size_t(usize);

   up = u->limbs;
   vp = v->limbs;

   cmp = GMP_INTS_mpn_cmp (up, vp, size);

   if (cmp == 0)
      return(0);

   if ((cmp < 0) == (usize < 0))
      return(1);
   else
      return(-1);
   }


//07/24/01:  Not visually inspected.  Relying on
//GNU ...
int GMP_INTS_mpz_cmp_ui (const GMP_INTS_mpz_struct *u, 
                         unsigned long int v_digit)
   {
   GMP_INTS_size_t usize = u->size;

   //Eyeball the input parameter.
   assert(u != NULL);
   assert(u->n_allocd > 0);
   assert(u->limbs != NULL);

   if (usize == 0)
      return -(v_digit != 0);

   if (usize == 1)
      {
      GMP_INTS_limb_t u_digit;

      u_digit = u->limbs[0];
      if (u_digit > v_digit)
	      return 1;
      if (u_digit < v_digit)
	      return -1;
      return 0;
      }

   return (usize > 0) ? 1 : -1;
   }


//07/24/01:  Not visually inspected.  Relying on GNU.
int GMP_INTS_mpz_cmp_si (const GMP_INTS_mpz_struct *u, 
                         signed long int v_digit)
   {
   GMP_INTS_size_t usize = u->size;
   GMP_INTS_size_t vsize;
   GMP_INTS_limb_t u_digit;

   //Eyeball the input parameter.
   assert(u != NULL);
   assert(u->n_allocd > 0);
   assert(u->limbs != NULL);

   vsize = 0;
   if (v_digit > 0)
      vsize = 1;
   else if (v_digit < 0)
      {
      vsize = -1;
      v_digit = -v_digit;
      }

   if (usize != vsize)
      return usize - vsize;

   if (usize == 0)
      return 0;

   u_digit = u->limbs[0];

   if (u_digit == (GMP_INTS_limb_t) (unsigned long) v_digit)
      return 0;

   if (u_digit > (GMP_INTS_limb_t) (unsigned long) v_digit)
      return usize;
   else
      return -usize;
   }


//07/24/01:  Not visually inspected.  Counting on GNU.
int GMP_INTS_mpz_cmpabs (const GMP_INTS_mpz_struct *u, 
                         const GMP_INTS_mpz_struct *v)
   {
   GMP_INTS_size_t usize = u->size;
   GMP_INTS_size_t vsize = v->size;
   GMP_INTS_limb_srcptr up, vp;
   int cmp;

   //Eyeball the input parameters.
   assert(u != NULL);
   assert(u->n_allocd > 0);
   assert(u->limbs != NULL);
   assert(v != NULL);
   assert(v->n_allocd > 0);
   assert(v->limbs != NULL);

   usize = GMP_INTS_abs_of_size_t(usize);
   vsize = GMP_INTS_abs_of_size_t(vsize);

   if (usize != vsize)
      return usize - vsize;

   if (usize == 0)
      return 0;

   up = u->limbs;
   vp = v->limbs;

   cmp = GMP_INTS_mpn_cmp (up, vp, usize);

   return cmp;
   }

//07/24/01:  Not visually inspected.  Counting on GNU.
int GMP_INTS_mpz_cmpabs_ui(const GMP_INTS_mpz_struct *u, 
                           unsigned long int v_digit)
   {
   GMP_INTS_size_t usize = u->size;

   //Eyeball the input parameter.
   assert(u != NULL);
   assert(u->n_allocd > 0);
   assert(u->limbs != NULL);

   if (usize == 0)
      return -(v_digit != 0);

   usize = GMP_INTS_abs_of_size_t(usize);

   if (usize == 1)
      {
      GMP_INTS_limb_t u_digit;

      u_digit = u->limbs[0];
      if (u_digit > v_digit)
	      return 1;
      if (u_digit < v_digit)
	      return -1;
      return 0;
      }

   return 1;
   }


/******************************************************************/
/***  VERSION CONTROL IDENTITY FUNCTIONS   ************************/
/******************************************************************/

//07/18/01:  Visual inspection only.  Function deemed too 
//simple for unit testing.
const char *GMP_INTS_cvcinfo(void)
   {
   return("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/gmp_ints.c 71 2016-11-05 11:07:06Z dashley $");
   }


//07/18/01:  Visual inspection only.  Function deemed too 
//simple for unit testing.
const char *GMP_INTS_hvcinfo(void)
   {
   return(GMP_INTS_H_VERSION);
   }

//End of gmp_ints.c.
