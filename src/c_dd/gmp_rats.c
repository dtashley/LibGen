//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/gmp_rats.c 90 2016-12-04 16:14:46Z dashley $
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
#define MODULE_GMP_RATS

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "bstrfunc.h"
#include "charfunc.h"
#include "gmp_ints.h"
#include "gmp_rats.h"

#if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
   #include "ccmalloc.h"
#elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
   #include "tclalloc.h"
#else
   #include <malloc.h>
#endif 


/******************************************************************/
/***  STATUS FUNCTIONS  *******************************************/
/******************************************************************/
//Functions in this category provide information about rational
//numbers.
//08/08/01:  Visual inspection OK.
int GMP_RATS_mpq_is_nan(const GMP_RATS_mpq_struct *rn)
   {
   assert(rn != NULL);

   //A rational number is NAN in one of two 
   //circumstances.  If either of the integer components
   //is NAN, or else if there is a zero denominator.
   if (GMP_INTS_mpz_get_flags(&(rn->num)) || GMP_INTS_mpz_get_flags(&(rn->den)))
      {
      return(1);
      }
   if (GMP_INTS_mpz_is_zero(&(rn->den)))
      {
      return(1);
      }

   //We're clean ...
   return(0);
   }


/******************************************************************/
/***  INITIALIZATION, CLEARING, AND SETTING FUNCTIONS  ************/
/******************************************************************/
//08/07/01:  Visual inspection OK.
void GMP_RATS_mpq_init(GMP_RATS_mpq_struct *arg)
   {
   //Eyeball the input parameter.
   assert(arg != NULL);

   //Initialize the numerator and denominator.
   GMP_INTS_mpz_init(&(arg->num));
   GMP_INTS_mpz_init(&(arg->den));

   //Canonically, we must start off as 0/1--canonical zero.
   GMP_INTS_mpz_set_ui(&(arg->num), 0);
   GMP_INTS_mpz_set_ui(&(arg->den), 1);
   }


//08/07/01:  Visual inspection OK.
void GMP_RATS_mpq_clear(GMP_RATS_mpq_struct *arg)
   {
   //Eyeball the input parameter.
   assert(arg != NULL);

   //Clear the numerator and denominator.  The called functions
   //will check for NULL pointers and so forth.
   GMP_INTS_mpz_clear(&(arg->num));
   GMP_INTS_mpz_clear(&(arg->den));
   }


//08/07/01:  Visual inspection OK.
void GMP_RATS_mpq_set_si(GMP_RATS_mpq_struct *arg,
                         int num,
                         int den)
   {
   //Eyeball the input parameters.
   assert(arg != NULL);

   //Set the numerator and denominator.
   GMP_INTS_mpz_set_si(&(arg->num), num);
   GMP_INTS_mpz_set_si(&(arg->den), den);
   }


//08/08/01:  Visual inspection OK.
void GMP_RATS_mpq_copy(      GMP_RATS_mpq_struct *dst,
                       const GMP_RATS_mpq_struct *src)
   {
   assert(dst != NULL);
   assert(src != NULL);

   GMP_INTS_mpz_copy(&(dst->num), &(src->num));
   GMP_INTS_mpz_copy(&(dst->den), &(src->den));
   }


//08/13/01:  Visual inspection OK.
void GMP_RATS_mpq_swap( GMP_RATS_mpq_struct *a,
                        GMP_RATS_mpq_struct *b)
   {
   assert(a != NULL);
   assert(b != NULL);

   //Handle the swap by swapping integer components.
   GMP_INTS_mpz_swap(&(a->num), &(b->num));
   GMP_INTS_mpz_swap(&(a->den), &(b->den));
   }


//08/13/01:  Visual inspection OK.
void GMP_RATS_mpq_swap_components(GMP_RATS_mpq_struct *arg)
   {
   assert(arg != NULL);

   GMP_INTS_mpz_swap(&(arg->num), &(arg->den));
   }


//08/07/01:  Visual inspection OK.
void GMP_RATS_mpq_set_complex_slash_sepd_rat_num(const char *s,
                                                 int *failure,
                                                 GMP_RATS_mpq_struct *rn)
   {
   char *slash_posn, *numerator, *denominator;
   size_t s_len, numerator_len, denominator_len;
   size_t i;

   //Eyeball the input parameters.
   assert(s != NULL);
   assert(failure != NULL);
   assert(rn != NULL);

   //Start off believing there is no failure.
   *failure = 0;

   //Figure out if there is one and only one slash in the 
   //string.  If this condition isn't met, we cannot
   //go further.
   slash_posn = strchr(s, '/');
   if (!slash_posn)
      {
      *failure = 1;
      return;
      }
   if (strchr(slash_posn + 1, '/')) //There is a second occurence.
      {
      *failure = 1;
      return;
      }

   //At this point we have one and only one slash.
   //Crack the string in two.  We must do this because the 
   //input is a constant string.  We are not allowed to touch it
   //in the logical domain because of the "const" keyword.  We can't
   //do this in the physical domain because the debugger will nail
   //us for it.
   s_len = strlen(s);
   numerator_len = slash_posn - s;
   denominator_len = strlen(slash_posn + 1);
   #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
      numerator   = CCMALLOC_malloc(sizeof(char) * (numerator_len   + 1));
      denominator = CCMALLOC_malloc(sizeof(char) * (denominator_len + 1));
   #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
      numerator   = TclpAlloc(sizeof(char) * (numerator_len   + 1));
      denominator = TclpAlloc(sizeof(char) * (denominator_len + 1));
   #else
      numerator   = malloc(sizeof(char) * (numerator_len   + 1));
      denominator = malloc(sizeof(char) * (denominator_len + 1));
   #endif 

   assert(numerator   != NULL);
   assert(denominator != NULL);

   for (i=0; i<numerator_len; i++)
      {
      numerator[i] = s[i];
      }   
   numerator[numerator_len] = 0;
   
   for (i=0; i<denominator_len; i++)
      {
      denominator[i] = s[slash_posn - s + 1 + i];
      }
   denominator[denominator_len] = 0;

   //Try to parse out the numerator as an arbitrary integer.
   //If this can't be done, it is an immediate failure.
   GMP_INTS_mpz_set_general_int(&(rn->num),
                                failure, 
                                numerator);
   if (*failure)
      {
      *failure = 1;  //Clamp to 1, don't know what non-zero value
                     //was there.
      goto ret_pt;
      }

   //Try to parse out the denominator.
   GMP_INTS_mpz_set_general_int(&(rn->den),
                                failure, 
                                denominator);
   if (*failure)
      {
      *failure = 1;  //Clamp to 1, don't know what non-zero value
                     //was there.
      goto ret_pt;
      }

   //At this point, we have both a numerator and denominator.
   //Clean up and return.
   ret_pt:
   #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
      CCMALLOC_free(numerator);
      CCMALLOC_free(denominator);
   #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
      TclpFree(numerator);
      TclpFree(denominator);
   #else
      free(numerator);
      free(denominator);
   #endif 
   }


//08/07/01:  Visual inspection OK.
void GMP_RATS_mpq_set_sci_not_rat_num(const char *s,
                                      int *failure,
                                      GMP_RATS_mpq_struct *rn)
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
   assert(s != NULL);
   assert(failure != NULL);
   assert(rn != NULL);
      //Subcomponents of the rational number will be checked as
      //we make integer calls, if we're in debug mode.

   //Start off believing no failure.
   *failure = 0;

   //Set the output to 0/1.  This is the default case for some
   //steps below.
   GMP_RATS_mpq_set_si(rn, 0, 1);

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
   else 
      {
      //The number parsed out.  The general strategy is to form a rational number
      //consisting of the mantissa, with the decimal point shifted fully right, over
      //a denominator of 1.  From there, we process the exponent and combine it with
      //the number of characters after the decimal point to form a virtual exponent.
      //If the exponent is positive, we multiply the numerator by the power of 10.
      //If the exponent is negative, we multiply the denominator by that power of 10.

      //We want to trim the trailing zeros off of the portion of the mantissa after the
      //decimal point.  We only need to back up indices, no need to make copies, etc.
      //Note that it is possible that there are only zeros, in which case we'll end
      //up with a length of zero.
      while ((mant_adp_len > 0) && (s[mant_adp + mant_adp_len - 1]=='0'))
         mant_adp_len--;

      //Trim the leading zeros off of the portion of the mantissa before the
      //decimal point.  Note that it is possible that there is only a zero,
      //so we may trim it down to nothing.
      while ((mant_bdp_len > 0) && (s[mant_bdp]=='0'))
         {
         mant_bdp++;
         mant_bdp_len--;
         }

      //If we have only zeros in the mantissa, both before the
      //decimal point and after, then we return 0.
      if ((mant_bdp_len + mant_adp_len) == 0)
         {
         *failure = 0;
         return;
         }
      
      //Convert the numerator to an integer which represents the
      //part before the mantissa and the part after the mantissa
      //concatenated as an integer.  We could call a function to do
      //this, but the function is not really any better in algorithm.
      //We can do it ourselves.
      GMP_INTS_mpz_set_ui(&(rn->num), 0);
      for (si = 0; si < mant_bdp_len; si++)
         {
         int val;

         GMP_INTS_mpz_mul_si(&(rn->num), &(rn->num), 10);
         val = CHARFUNC_digit_to_val(s[mant_bdp + si]);
         if (val >= 0)
            GMP_INTS_mpz_add_ui(&(rn->num), &(rn->num), val);
         }
      for (si = 0; si < mant_adp_len; si++)
         {
         int val;

         GMP_INTS_mpz_mul_si(&(rn->num), &(rn->num), 10);
         val = CHARFUNC_digit_to_val(s[mant_adp + si]);
         if (val >= 0)
            GMP_INTS_mpz_add_ui(&(rn->num), &(rn->num), val);
         }

      //The numerator should now have an integer which is 
      //The absolute value of the mantissa.  Process the possible
      //sign.
      if (mant_sign == '-')
         GMP_INTS_mpz_negate(&(rn->num));

      //We now need to form a value from the exponent, if any.
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

      //We need to adjust the exponent for the number of digits
      //after the decimal point.
      exponent_val -= mant_adp_len;   
      
      //Again, clip for size.
      if ((exponent_val < -8388608) || (exponent_val > 8388607))
         {
         *failure = 1;
         return;
         }
         
      //There are two cases to consider.  If the exponent
      //is positive, we need to multiply the numerator
      //by 10 exponentiated to the power of the exponent.
      //If the exponent is negative, we need to do the
      //same thing to the denominator.  If the exponent
      //is negative, we don't need to do anything.
      if (exponent_val > 0)
         {
         GMP_INTS_mpz_struct k10, k10_exponentiated;

         GMP_INTS_mpz_init(&k10);
         GMP_INTS_mpz_init(&k10_exponentiated);

         GMP_INTS_mpz_set_ui(&k10, 10);

         GMP_INTS_mpz_pow_ui(&k10_exponentiated, &k10, exponent_val);

         GMP_INTS_mpz_mul(&(rn->num), &(rn->num), &k10_exponentiated);

         GMP_INTS_mpz_clear(&k10);
         GMP_INTS_mpz_clear(&k10_exponentiated);

         *failure = 0;

         if (GMP_INTS_mpz_get_flags(&(rn->num)) || GMP_INTS_mpz_get_flags(&(rn->den)))
            *failure = 1;

         return;
         }
      else if (exponent_val < 0)
         {
         GMP_INTS_mpz_struct k10, k10_exponentiated;

         GMP_INTS_mpz_init(&k10);
         GMP_INTS_mpz_init(&k10_exponentiated);

         GMP_INTS_mpz_set_ui(&k10, 10);

         GMP_INTS_mpz_pow_ui(&k10_exponentiated, &k10, -exponent_val);

         GMP_INTS_mpz_mul(&(rn->den), &(rn->den), &k10_exponentiated);

         GMP_INTS_mpz_clear(&k10);
         GMP_INTS_mpz_clear(&k10_exponentiated);

         *failure = 0;

         if (GMP_INTS_mpz_get_flags(&(rn->num)) || GMP_INTS_mpz_get_flags(&(rn->den)))
            *failure = 1;

         return;
         }
      }
   }


//08/07/01:  Visual inspection OK.
void GMP_RATS_mpq_set_all_format_rat_num(const char *s,
                                         int *failure,
                                         GMP_RATS_mpq_struct *rn)
   {
   //Eyeball the input parameters.
   assert(s != NULL);
   assert(failure != NULL);
   assert(rn != NULL);

   //Assume no failure.
   *failure = 0;

   //Try in order to parse as integers with slash then
   //as number in scientific notation.
   GMP_RATS_mpq_set_complex_slash_sepd_rat_num(s,
                                               failure,
                                               rn);
   if (!*failure)
      return;

   GMP_RATS_mpq_set_sci_not_rat_num(s,
                                    failure,
                                    rn);

   if (*failure)
      *failure = 1;  //Clamp output.
   }


/******************************************************************/
/***  NORMALIZATION FUNCTIONS  ************************************/
/******************************************************************/
//08/07/01:  Visual inspection OK.
void GMP_RATS_mpq_normalize_sign(GMP_RATS_mpq_struct *rn)
   {
   //Eyeball the input.
   assert(rn != NULL);

   if (GMP_INTS_mpz_is_neg(&rn->num) && GMP_INTS_mpz_is_neg(&rn->den))
      {
      //Both negative, can negate both, this leaves both positive,
      //which is the normalized form for a positive rational
      //number.
      GMP_INTS_mpz_negate(&rn->num);
      GMP_INTS_mpz_negate(&rn->den);
      }
   else if (!GMP_INTS_mpz_is_neg(&rn->num) && GMP_INTS_mpz_is_neg(&rn->den))
      {
      //Denominator neg, numerator non-neg, can negate both.  This
      //will leave numerator neg, denominator pos, which is
      //normalized form for negative rational number.
      GMP_INTS_mpz_negate(&rn->num);
      GMP_INTS_mpz_negate(&rn->den);
      }    
   }


//08/07/01:  Visual inspection OK.
void GMP_RATS_mpq_normalize(GMP_RATS_mpq_struct *rn)
   {
   //Eyeball the input.
   assert(rn != NULL);

   //Cover some special cases.  If either component has flags
   //set, don't even touch it.
   if (GMP_INTS_mpz_get_flags(&(rn->num)) || GMP_INTS_mpz_get_flags(&(rn->den)))
      {
      return;
      }
   //If the denominator is zero, normalize it to 1/0, the canonical
   //for for an illegal rational number.
   else if (GMP_INTS_mpz_is_zero(&(rn->den)))
      {
      GMP_RATS_mpq_set_si(rn, 1, 0);
      return;
      }
   //If the numerator is zero, convert the number to the canonical
   //form for zero of 0/1.
   else if (GMP_INTS_mpz_is_zero(&(rn->num)))
      {
      GMP_RATS_mpq_set_si(rn, 0, 1);
      return;
      }
   else
      {
      int num_is_neg;
      int den_is_neg;
      GMP_INTS_mpz_struct gcd, quotient, remainder;

      //Allocate space for the integers used.
      GMP_INTS_mpz_init(&gcd);
      GMP_INTS_mpz_init(&quotient);
      GMP_INTS_mpz_init(&remainder);

      //This is the most normal case, where we need to
      //look at reducing the numerator and denominator.
      //One way to do it would be to obtain the g.c.d.
      //and divide this out, and this is the route
      //we'll take.  However, must grab out the sign.
      if (GMP_INTS_mpz_is_neg(&(rn->num)))
         {
         num_is_neg = 1;
         GMP_INTS_mpz_negate(&(rn->num));
         }
      else
         {
         num_is_neg = 0;
         }

      if (GMP_INTS_mpz_is_neg(&(rn->den)))
         {
         den_is_neg = 1;
         GMP_INTS_mpz_negate(&(rn->den));
         }
      else
         {
         den_is_neg = 0;
         }

      //Calculate the GCD.
      GMP_INTS_mpz_gcd(&gcd, &(rn->num), &(rn->den));

      //Divide the numerator by the GCD and store it
      //back.
      GMP_INTS_mpz_tdiv_qr(&quotient, &remainder,
                           &(rn->num), &gcd);
      GMP_INTS_mpz_copy(&(rn->num), &quotient);

      //Divide the denominator by the GCD and store it
      //back.
      GMP_INTS_mpz_tdiv_qr(&quotient, &remainder,
                           &(rn->den), &gcd);
      GMP_INTS_mpz_copy(&(rn->den), &quotient);

      //We now need to adjust the sign.  Both the
      //numerator and denominator are definitely
      //positive.  Need to make the numerator
      //negative if either but not both of the
      //original signs were negative.
      if ((num_is_neg && !den_is_neg) || (!num_is_neg && den_is_neg))
         {
         GMP_INTS_mpz_negate(&(rn->num));
         }

      //Deallocate space for the integers used.
      GMP_INTS_mpz_clear(&gcd);
      GMP_INTS_mpz_clear(&quotient);
      GMP_INTS_mpz_clear(&remainder);

      return;
      }    
   }


/******************************************************************/
/***  ARITHMETIC FUNCTIONS  ***************************************/
/******************************************************************/
//08/08/01:  Visual inspection OK.
void GMP_RATS_mpq_add(      GMP_RATS_mpq_struct *result,
                      const GMP_RATS_mpq_struct *arg1,
                      const GMP_RATS_mpq_struct *arg2)
   {
   GMP_RATS_mpq_struct rv;
   GMP_INTS_mpz_struct temp;

   //Eyeball the input parameters.
   assert(result != NULL);
   assert(arg1   != NULL);
   assert(arg2   != NULL);

   //Generally speaking, we do not want to require that
   //the arguments and the result be distinct, as this is
   //too much of a restriction on the caller.  The approach
   //taken, somewhat wasteful, is to allocate a place for
   //the return value.
   //
   //For addition, if we are adding a/b and c/d, the
   //result is necessarily algebraically
   //(ad + cb)/bd.
   //
   //If either rational number in the input is invalid,
   //flag the result as invalid.
   if (GMP_RATS_mpq_is_nan(arg1) || GMP_RATS_mpq_is_nan(arg2))
      {
      GMP_RATS_mpq_set_si(result, 1, 0);
      }
   else
      {
      //Both rational numbers are OK.  Can simply stage the
      //result by the algebraic identity and then
      //normalize it.  Only need one temporary variable.
      //
      //Initialize the rational number that we will use to
      //hold return value in case it is the same as one
      //or both of the arguments.
      GMP_RATS_mpq_init(&rv);

      //Initialize the temporary integer.
      GMP_INTS_mpz_init(&temp);

      //numerator = a * d
      GMP_INTS_mpz_mul(&(rv.num), &(arg1->num), &(arg2->den));

      //temp = c * b
      GMP_INTS_mpz_mul(&temp, &(arg2->num), &(arg1->den));

      //numerator = a * d + c * b
      GMP_INTS_mpz_add(&(rv.num), &(rv.num), &temp);

      //denominator = b * d
      GMP_INTS_mpz_mul(&(rv.den), &(arg1->den), &(arg2->den));

      //Copy the temporary result to the actual return value.
      //Had to wait until now in case result was the same
      //as either or both args.
      GMP_RATS_mpq_copy(result, &rv);

      //Normalize the result.
      GMP_RATS_mpq_normalize(result);

      //Free dynamic memory.
      GMP_RATS_mpq_clear(&rv);
      GMP_INTS_mpz_clear(&temp);
      }
   }


//08/08/01:  Visual inspection OK.
void GMP_RATS_mpq_sub(      GMP_RATS_mpq_struct *result,
                      const GMP_RATS_mpq_struct *arg1,
                      const GMP_RATS_mpq_struct *arg2)
   {
   GMP_RATS_mpq_struct negated_arg_2;

   //Eyeball the input parameters.
   assert(result != NULL);
   assert(arg1   != NULL);
   assert(arg2   != NULL);

   //For the subtract function, we could do it directly,
   //but might as well just define it recursively
   //in terms of add.  We can't modify the inputs,
   //so copy the second off and negate it.  All error
   //flags and so forth will propagate automatically.
   //
   //Allocate space for the negated arg 2.
   GMP_RATS_mpq_init(&negated_arg_2);

   //Copy from the original.
   GMP_RATS_mpq_copy(&negated_arg_2, arg2);

   //Negate the copy.  Negating the numerator will
   //do it.
   GMP_INTS_mpz_negate(&(negated_arg_2.num));

   //Make the add, which now is really a subtract.
   GMP_RATS_mpq_add(result, arg1, &negated_arg_2);

   //Destroy the temporary variable.
   GMP_RATS_mpq_clear(&negated_arg_2);
   }


//08/16/01: Visual inspection OK.
void GMP_RATS_mpq_mul(      GMP_RATS_mpq_struct *result,
                      const GMP_RATS_mpq_struct *arg1,
                      const GMP_RATS_mpq_struct *arg2)
   {
   //Eyeball the input parameters.
   assert(result != NULL);
   assert(arg1   != NULL);
   assert(arg2   != NULL);

   //If either rational number in the input is invalid,
   //flag the result as invalid.
   if (GMP_RATS_mpq_is_nan(arg1) || GMP_RATS_mpq_is_nan(arg2))
      {
      GMP_RATS_mpq_set_si(result, 1, 0);
      }
   else
      {
      //Rational number multiplication is a simple matter.
      //Just multiply components.  Don't need to worry
      //about rational numbers overlapping, as numerator
      //operations and denominator operations are separate.
      GMP_INTS_mpz_mul(&(result->num),
                       &(arg1->num),
                       &(arg2->num));
      GMP_INTS_mpz_mul(&(result->den),
                       &(arg1->den),
                       &(arg2->den));

      //Normalize it.
      GMP_RATS_mpq_normalize(result);
      }
   }


//08/16/01: Visual inspection OK.
void GMP_RATS_mpq_div(      GMP_RATS_mpq_struct *result,
                      const GMP_RATS_mpq_struct *arg1,
                      const GMP_RATS_mpq_struct *arg2)
   {
   GMP_RATS_mpq_struct rv;

   //Eyeball the input parameters.
   assert(result != NULL);
   assert(arg1   != NULL);
   assert(arg2   != NULL);

   //If either rational number in the input is invalid,
   //flag the result as invalid.
   if (GMP_RATS_mpq_is_nan(arg1) || GMP_RATS_mpq_is_nan(arg2))
      {
      GMP_RATS_mpq_set_si(result, 1, 0);
      }
   else
      {
      //Rational number division is a simple matter.
      //Just multiply components.  We do need to worry
      //about rational numbers overlapping, so must
      //make a copy of the return value.  If denominator
      //of return value is zero, it is NAN, but caller
      //should detect this.
      //
      //Allocate return value.
      GMP_RATS_mpq_init(&rv);

      //Calculate quotient.
      GMP_INTS_mpz_mul(&(rv.num),
                       &(arg1->num),
                       &(arg2->den));
      GMP_INTS_mpz_mul(&(rv.den),
                       &(arg1->den),
                       &(arg2->num));

      //Normalize quotient.
      GMP_RATS_mpq_normalize(&rv);

      //Copy to its destination.
      GMP_RATS_mpq_copy(result, &rv);

      //Deallocate temporary return value.
      GMP_RATS_mpq_clear(&rv);
      }
   }


/******************************************************************/
/***  COMPARISON FUNCTIONS  ***************************************/
/******************************************************************/
//08/16/01: Visual inspection OK.
int GMP_RATS_mpq_cmp(const  GMP_RATS_mpq_struct *arg1,
                     const  GMP_RATS_mpq_struct *arg2,
                     int   *failure)
   {
   int arg1_sgn;
   int arg2_sgn;
   int rv, failure_rv;
   GMP_INTS_mpz_struct prod1, prod2;

   //Eyeball the input parameters.  Note that the third
   //parameter may be NULL.
   assert(arg1 != NULL);
   assert(arg2 != NULL);

   //If either of the input arguments are NAN, we 
   //cannot compare arguments.  We return 0, and it
   //depends on the caller whether it is important
   //that the comparison is bogus.
   if (GMP_RATS_mpq_is_nan(arg1) || GMP_RATS_mpq_is_nan(arg2))
      {
      if (failure != NULL)
         *failure = 1;
      return(0);
      }

   //Calculate the sign of the left argument.  The encoding
   //we'll use is -1 means negative, 0 means zero, and
   //1 means positive.
   if (GMP_INTS_mpz_is_zero(&(arg1->num)))
      {
      arg1_sgn = 0;
      }
   else if (GMP_INTS_mpz_is_neg(&(arg1->num)) && GMP_INTS_mpz_is_neg(&(arg1->den)))
      {
      arg1_sgn = 1;
      }
   else if (GMP_INTS_mpz_is_neg(&(arg1->num)) && GMP_INTS_mpz_is_pos(&(arg1->den)))
      {
      arg1_sgn = -1;
      }
   else if (GMP_INTS_mpz_is_pos(&(arg1->num)) && GMP_INTS_mpz_is_neg(&(arg1->den)))
      {
      arg1_sgn = -1;
      }
   else if (GMP_INTS_mpz_is_pos(&(arg1->num)) && GMP_INTS_mpz_is_pos(&(arg1->den)))
      {
      arg1_sgn = 1;
      }

   //Calculate the sign of the right argument.  The encoding
   //we'll use is -1 means negative, 0 means zero, and
   //1 means positive.
   if (GMP_INTS_mpz_is_zero(&(arg2->num)))
      {
      arg2_sgn = 0;
      }
   else if (GMP_INTS_mpz_is_neg(&(arg2->num)) && GMP_INTS_mpz_is_neg(&(arg2->den)))
      {
      arg2_sgn = 1;
      }
   else if (GMP_INTS_mpz_is_neg(&(arg2->num)) && GMP_INTS_mpz_is_pos(&(arg2->den)))
      {
      arg2_sgn = -1;
      }
   else if (GMP_INTS_mpz_is_pos(&(arg2->num)) && GMP_INTS_mpz_is_neg(&(arg2->den)))
      {
      arg2_sgn = -1;
      }
   else if (GMP_INTS_mpz_is_pos(&(arg2->num)) && GMP_INTS_mpz_is_pos(&(arg2->den)))
      {
      arg2_sgn = 1;
      }

   //OK, can handle some simple cases where the signs of the
   //operands are different or both are zero.
   if ((arg1_sgn == 0) && (arg2_sgn == 0))
      {
      if (failure != NULL)
         *failure = 0;
      return(0);
      }
   else if ((arg1_sgn == -1) && (arg2_sgn > -1))
      {
      if (failure != NULL)
         *failure = 0;
      return(-1);
      }
   else if ((arg1_sgn == 0) && (arg2_sgn < 0))
      {
      if (failure != NULL)
         *failure = 0;
      return(1);
      }
   else if ((arg1_sgn == 0) && (arg2_sgn > 0))
      {
      if (failure != NULL)
         *failure = 0;
      return(-1);
      }
   else if ((arg1_sgn == 1) && (arg2_sgn < 1))
      {
      if (failure != NULL)
         *failure = 0;
      return(1);
      }

   //OK at this point, we cannot make a simple determination
   //as to the relative ordering.  The signs of arg1 and
   //arg2 are both the same, either both positive or both
   //negative.  We have to do a multiplication to sort
   //it out.
   //
   //Allocate the two integers to hold multiplication
   //results.
   GMP_INTS_mpz_init(&prod1);
   GMP_INTS_mpz_init(&prod2);

   //Cross-multiply to get relative magnitudes.
   GMP_INTS_mpz_mul(&prod1, &(arg1->num), &(arg2->den));
   GMP_INTS_mpz_mul(&prod2, &(arg1->den), &(arg2->num));
   
   //Take absolute values.
   GMP_INTS_mpz_abs(&prod1);
   GMP_INTS_mpz_abs(&prod2);

   //If we overflowed either multiplication and generated
   //a NAN, we cannot complete the compare.
   if (GMP_INTS_mpz_get_flags(&prod1) || GMP_INTS_mpz_get_flags(&prod2))
      {
      failure_rv = 1;
      rv = 0;
      }
   //If both rational numbers were effectively positive, we can
   //use the relative ordering of the products as the relative
   //ordering of the rational numbers.
   else if (arg1_sgn == 1)
      {
      //Compare the integers.
      rv = GMP_INTS_mpz_cmp(&prod1, &prod2);

      //Clamp the return value.
      if (rv < 0)
         rv = -1;
      else if (rv == 0)
         rv = 0;
      else if (rv > 0)
         rv = 1;

      //There was no error.
      failure_rv = 0;
      }
   else
      {
      //The only case that *should* allow us to be here is
      //if the sign of both numbers is neg.
      assert(arg1_sgn == -1);

      //Compare the integers.
      rv = GMP_INTS_mpz_cmp(&prod1, &prod2);

      //Invert and clamp the return value.
      if (rv < 0)
         rv = 1;
      else if (rv == 0)
         rv = 0;
      else if (rv > 0)
         rv = -1;

      //There was no error.
      failure_rv = 0;
      }

   //Deallocate the two integers.
   GMP_INTS_mpz_clear(&prod1);
   GMP_INTS_mpz_clear(&prod2);

   //Return the return values.
   if (failure != NULL)
      *failure = failure_rv;
   return(rv);
   }


/******************************************************************/
/***  VERSION CONTROL REPORTING FUNCTIONS  ************************/
/******************************************************************/
//08/07/01:  Visual inspection OK.
const char *GMP_RATS_cvcinfo(void)
   {
   return("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/gmp_rats.c 90 2016-12-04 16:14:46Z dashley $");
   }


//08/07/01:  Visual inspection OK.
const char *GMP_RATS_hvcinfo(void)
   {
   return(GMP_RATS_H_VERSION);
   }

//End of gmp_rats.c.
