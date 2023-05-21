//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/gmp_ralg.c 71 2016-11-05 11:07:06Z dashley $
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
#define MODULE_GMP_RALG

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


#include "fcmiof.h"
#include "gmp_ints.h"
#include "gmp_rats.h"
#include "gmp_ralg.h"
#include "intfunc.h"

#if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
   #include "ccmalloc.h"
#elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
   #include "tclalloc.h"
#else
   /* Do nothing. */
#endif


/******************************************************************/
/***  INITIALIZATION AND DESTRUCTION FUNCTIONS  *******************/
/******************************************************************/
//08/16/01: Visual inspection OK.
void GMP_RALG_cfdecomp_init(
            GMP_RALG_cf_app_struct *decomp,
            int                    *failure,
            GMP_INTS_mpz_struct    *num,
            GMP_INTS_mpz_struct    *den)
   {
   int loop_counter, i;
   GMP_INTS_mpz_struct arb_temp1, arb_temp2;

   //Eyeball the input parameters.  The rest of the eyeballing
   //will occur as functions are called to manipulate the
   //numerator and denominator passed in.
   assert(decomp  != NULL);
   assert(failure != NULL);
   assert(num     != NULL);
   assert(den     != NULL);

   //Allocate space for temporary integers.
   GMP_INTS_mpz_init(&arb_temp1);
   GMP_INTS_mpz_init(&arb_temp2);

   //Begin believing no failure.
   *failure = 0;

   //Initialize the copy of the numerator and denominator and
   //copy these into the structure.
   GMP_INTS_mpz_init(&(decomp->num));
   GMP_INTS_mpz_copy(&(decomp->num), num);
   GMP_INTS_mpz_init(&(decomp->den));
   GMP_INTS_mpz_copy(&(decomp->den), den);

   //Allocate the space for the first increment of the
   //growable areas.  We need to use different memory
   //allocation functions depending on whether we're 
   //in a Tcl build or a DOS command-line utility
   //build.
   //Space for partial quotients.
   decomp->a =
      #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
         CCMALLOC_malloc(sizeof(GMP_INTS_mpz_struct) * GMP_RALG_CF_ALLOC_INCREMENT);
      #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
         (GMP_INTS_mpz_struct *)
         TclpAlloc(sizeof(GMP_INTS_mpz_struct) * GMP_RALG_CF_ALLOC_INCREMENT);
      #else
         malloc(sizeof(GMP_INTS_mpz_struct) * GMP_RALG_CF_ALLOC_INCREMENT);
      #endif 

   //Dividends.
   decomp->dividend =
      #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
         CCMALLOC_malloc(sizeof(GMP_INTS_mpz_struct) * GMP_RALG_CF_ALLOC_INCREMENT);
      #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
         (GMP_INTS_mpz_struct *)
         TclpAlloc(sizeof(GMP_INTS_mpz_struct) * GMP_RALG_CF_ALLOC_INCREMENT);
      #else
         malloc(sizeof(GMP_INTS_mpz_struct) * GMP_RALG_CF_ALLOC_INCREMENT);
      #endif 

   //Divisors.
   decomp->divisor =
      #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
         CCMALLOC_malloc(sizeof(GMP_INTS_mpz_struct) * GMP_RALG_CF_ALLOC_INCREMENT);
      #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
         (GMP_INTS_mpz_struct *)
         TclpAlloc(sizeof(GMP_INTS_mpz_struct) * GMP_RALG_CF_ALLOC_INCREMENT);
      #else
         malloc(sizeof(GMP_INTS_mpz_struct) * GMP_RALG_CF_ALLOC_INCREMENT);
      #endif 

   //Remainders.
   decomp->remainder =
      #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
         CCMALLOC_malloc(sizeof(GMP_INTS_mpz_struct) * GMP_RALG_CF_ALLOC_INCREMENT);
      #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
         (GMP_INTS_mpz_struct *)
         TclpAlloc(sizeof(GMP_INTS_mpz_struct) * GMP_RALG_CF_ALLOC_INCREMENT);
      #else
         malloc(sizeof(GMP_INTS_mpz_struct) * GMP_RALG_CF_ALLOC_INCREMENT);
      #endif 

   //Convergent numerators.
   decomp->p =
      #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
         CCMALLOC_malloc(sizeof(GMP_INTS_mpz_struct) * GMP_RALG_CF_ALLOC_INCREMENT);
      #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
         (GMP_INTS_mpz_struct *)
         TclpAlloc(sizeof(GMP_INTS_mpz_struct) * GMP_RALG_CF_ALLOC_INCREMENT);
      #else
         malloc(sizeof(GMP_INTS_mpz_struct) * GMP_RALG_CF_ALLOC_INCREMENT);
      #endif 

   //Convergent denominators.
   decomp->q =
      #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
         CCMALLOC_malloc(sizeof(GMP_INTS_mpz_struct) * GMP_RALG_CF_ALLOC_INCREMENT);
      #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
         (GMP_INTS_mpz_struct *)
         TclpAlloc(sizeof(GMP_INTS_mpz_struct) * GMP_RALG_CF_ALLOC_INCREMENT);
      #else
         malloc(sizeof(GMP_INTS_mpz_struct) * GMP_RALG_CF_ALLOC_INCREMENT);
      #endif 

   //Now the number of allocated slots is what we just allocated.
   decomp->nallocd = GMP_RALG_CF_ALLOC_INCREMENT;

   //The number of slots actually used is zero, to start with.
   decomp->n = 0;

   //There are a number of conditions that will lead to an error
   //where we can't successfully form the continued fraction 
   //decomposition.  These errors are:
   //   a)Either component is NAN.
   //   b)Zero denominator.
   //   c)Either component negative.
   //In these cases, we'll pretend we got 0/1 for the number
   //and set things accordingly, and we'll set the failure
   //flag for the caller.
   //
   if (GMP_INTS_mpz_get_flags(num) 
       ||
       GMP_INTS_mpz_get_flags(den)
       ||
       GMP_INTS_mpz_is_zero(den)
       ||
       GMP_INTS_mpz_is_neg(num)
       ||
       GMP_INTS_mpz_is_neg(den))
      {
      *failure  = 1;
      decomp->n = 1;

      GMP_INTS_mpz_set_ui(&(decomp->num),     0);
      GMP_INTS_mpz_set_ui(&(decomp->den),     1);

      GMP_INTS_mpz_init(decomp->dividend);
      GMP_INTS_mpz_set_ui(decomp->dividend,   0);

      GMP_INTS_mpz_init(decomp->divisor);
      GMP_INTS_mpz_set_ui(decomp->divisor,    1);

      GMP_INTS_mpz_init(decomp->a);
      GMP_INTS_mpz_set_ui(decomp->a,          0);

      GMP_INTS_mpz_init(decomp->remainder);
      GMP_INTS_mpz_set_ui(decomp->remainder,  0);

      GMP_INTS_mpz_init(decomp->p);
      GMP_INTS_mpz_set_ui(decomp->p,          0);

      GMP_INTS_mpz_init(decomp->q);
      GMP_INTS_mpz_set_ui(decomp->q,          1);

      goto return_point;
      }

   //If we're here there are not any errors that we 
   //are willing to detect.  We should be clear
   //for a continued fraction decomposition.
   loop_counter = 0;
   do
      {
      //Increment the count of "rows", because we're
      //about to add one.
      decomp->n++;

      //If we have used up all the space available
      //for integers, we have to allocate more.
      if (decomp->n > decomp->nallocd)
         {
         //We now have more allocated space.
         decomp->nallocd += GMP_RALG_CF_ALLOC_INCREMENT;

         //Be absolutely sure we have not made a greivous
         //error.
         assert(decomp->n <= decomp->nallocd);

         //Space for dividends.
         decomp->dividend =
            #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
            CCMALLOC_realloc(
               decomp->dividend,
               sizeof(GMP_INTS_mpz_struct) * decomp->nallocd);
            #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
            (GMP_INTS_mpz_struct *)
            TclpRealloc((char *)decomp->dividend,
                        sizeof(GMP_INTS_mpz_struct) * decomp->nallocd);
            #else
            realloc(decomp->dividend, sizeof(GMP_INTS_mpz_struct) * decomp->nallocd);
            #endif 

         //Space for divisors.
         decomp->divisor =
            #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
            CCMALLOC_realloc(
               decomp->divisor,
               sizeof(GMP_INTS_mpz_struct) * decomp->nallocd);
            #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
            (GMP_INTS_mpz_struct *)
            TclpRealloc((char *)decomp->divisor,
                        sizeof(GMP_INTS_mpz_struct) * decomp->nallocd);
            #else
            realloc(decomp->divisor, sizeof(GMP_INTS_mpz_struct) * decomp->nallocd);
            #endif 

         //Space for partial quotients.
         decomp->a =
            #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
            CCMALLOC_realloc(
               decomp->a,
               sizeof(GMP_INTS_mpz_struct) * decomp->nallocd);
            #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
            (GMP_INTS_mpz_struct *)
            TclpRealloc((char *)decomp->a,
                        sizeof(GMP_INTS_mpz_struct) * decomp->nallocd);
            #else
            realloc(decomp->a, sizeof(GMP_INTS_mpz_struct) * decomp->nallocd);
            #endif 

         //Space for remainders.
         decomp->remainder =
            #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
            CCMALLOC_realloc(
               decomp->remainder,
               sizeof(GMP_INTS_mpz_struct) * decomp->nallocd);
            #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
            (GMP_INTS_mpz_struct *)
            TclpRealloc((char *)decomp->remainder,
                        sizeof(GMP_INTS_mpz_struct) * decomp->nallocd);
            #else
            realloc(decomp->remainder, sizeof(GMP_INTS_mpz_struct) * decomp->nallocd);
            #endif 

         //Space for partial quotient numerators.
         decomp->p =
            #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
            CCMALLOC_realloc(
               decomp->p,
               sizeof(GMP_INTS_mpz_struct) * decomp->nallocd);
            #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
            (GMP_INTS_mpz_struct *)
            TclpRealloc((char *)decomp->p,
                        sizeof(GMP_INTS_mpz_struct) * decomp->nallocd);
            #else
            realloc(decomp->p, sizeof(GMP_INTS_mpz_struct) * decomp->nallocd);
            #endif 

         //Space for partial quotient denominators.
         decomp->q =
            #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
            CCMALLOC_realloc(
               decomp->q,
               sizeof(GMP_INTS_mpz_struct) * decomp->nallocd);
            #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
            (GMP_INTS_mpz_struct *)
            TclpRealloc((char *)decomp->q,
                        sizeof(GMP_INTS_mpz_struct) * decomp->nallocd);
            #else
            realloc(decomp->q, sizeof(GMP_INTS_mpz_struct) * decomp->nallocd);
            #endif 
         }

      //At this point, we have enough space to do the next round of operations.
      //Set up an index variable.
      i = decomp->n - 1;

      //Initialize all of the integers at this round.
      GMP_INTS_mpz_init(decomp->dividend    + i);
      GMP_INTS_mpz_init(decomp->divisor     + i);
      GMP_INTS_mpz_init(decomp->a           + i);
      GMP_INTS_mpz_init(decomp->remainder   + i);
      GMP_INTS_mpz_init(decomp->p           + i);
      GMP_INTS_mpz_init(decomp->q           + i);

      //Perform the next round of continued fraction decomposition.  This
      //is standard stuff.
      if (i==0)
         {
         //In the 0th round, we essentially perform initial assignments.
         GMP_INTS_mpz_copy(decomp->dividend, &(decomp->num));
         GMP_INTS_mpz_copy(decomp->divisor,  &(decomp->den));

         //Make the division to get quotient and remainder.
         GMP_INTS_mpz_tdiv_qr(decomp->a, decomp->remainder, decomp->dividend, decomp->divisor);

         //The convergents in the first round are always the quotient over 1.
         GMP_INTS_mpz_copy(decomp->p, decomp->a); 
         GMP_INTS_mpz_set_ui(decomp->q, 1);
         }
      else if (i==1)
         {
         //In the 1st round, the only point of caution is that we have to
         //consider p(k-2)/q(k-2) carefully.
         GMP_INTS_mpz_copy(decomp->dividend + 1, decomp->divisor   + 0);
         GMP_INTS_mpz_copy(decomp->divisor  + 1, decomp->remainder + 0);

         //Make the division to get quotient and remainder.
         GMP_INTS_mpz_tdiv_qr(decomp->a          + 1, 
                              decomp->remainder  + 1, 
                              decomp->dividend   + 1, 
                              decomp->divisor    + 1);

         //Need to compute the numerator of the convergent.  This will be:
         //  a(1) p(0) + p(-1) = a(1)p(0) + 1.
         GMP_INTS_mpz_mul(decomp->p + 1, decomp->a + 1, decomp->p + 0);
         GMP_INTS_mpz_set_ui(&arb_temp1, 1);
         GMP_INTS_mpz_add(decomp->p + 1, decomp->p + 1, &arb_temp1);

         //Need to compute the denominator of the convergent.  This will
         //be a(1)q(0) + q(-1) = a(1) q(0) = a(1).
         GMP_INTS_mpz_copy(decomp->q + 1, decomp->a + 1);
         }
      else
         {
         //In the general case, it is a simple formula.
         //Rotate in the dividend and divisor from the previous round.
         GMP_INTS_mpz_copy(decomp->dividend + i, decomp->divisor   + i - 1);
         GMP_INTS_mpz_copy(decomp->divisor  + i, decomp->remainder + i - 1);

         //Make the division to get quotient and remainder.
         GMP_INTS_mpz_tdiv_qr(decomp->a          + i, 
                              decomp->remainder  + i, 
                              decomp->dividend   + i, 
                              decomp->divisor    + i);

         //Need to compute the numerator of the convergent.  This will be:
         //  p(i) = a(i)p(i-1) + p(i-2)
         GMP_INTS_mpz_mul(decomp->p + i, decomp->a + i, decomp->p + i - 1);
         GMP_INTS_mpz_add(decomp->p + i, decomp->p + i, decomp->p + i - 2);

         //Need to compute the numerator of the convergent.  This will be:
         //  q(i) = q(i)q(i-1) + q(i-2)
         GMP_INTS_mpz_mul(decomp->q + i, decomp->a + i, decomp->q + i - 1);
         GMP_INTS_mpz_add(decomp->q + i, decomp->q + i, decomp->q + i - 2);
         }

      loop_counter++;
      } while(!GMP_INTS_mpz_is_zero(decomp->remainder + decomp->n - 1) && loop_counter < 100000);

   //In debug builds, be sure we did not terminate based on the loop counter.
   assert(loop_counter != 100000);

   return_point:

   //Deallocate space for temporary integers.
   GMP_INTS_mpz_clear(&arb_temp1);
   GMP_INTS_mpz_clear(&arb_temp2);
   }


//08/16/01: Visual inspection OK.
void GMP_RALG_cfdecomp_destroy(
            GMP_RALG_cf_app_struct *decomp
            )
   {
   int i;

   //Eyeball the input parameters.  Other eyeballing
   //will be done as integers are destroyed.
   assert(decomp != NULL);

   //First, destroy the things that are bound directly
   //to the record.
   GMP_INTS_mpz_clear(&(decomp->num));
   GMP_INTS_mpz_clear(&(decomp->den));

   //Now, destroy every integer which is allocated.
   for (i=0; i < decomp->n; i++)
      {
      GMP_INTS_mpz_clear(decomp->dividend     + i); 
      GMP_INTS_mpz_clear(decomp->divisor      + i); 
      GMP_INTS_mpz_clear(decomp->a            + i); 
      GMP_INTS_mpz_clear(decomp->remainder    + i); 
      GMP_INTS_mpz_clear(decomp->p            + i); 
      GMP_INTS_mpz_clear(decomp->q            + i); 
      }

   //Now, destroy the arrays of integers.
   #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
      CCMALLOC_free(decomp->dividend);
   #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
      TclpFree((char *)decomp->dividend);
   #else
      free(decomp->dividend);
   #endif 
   #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
      CCMALLOC_free(decomp->divisor);
   #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
      TclpFree((char *)decomp->divisor);
   #else
      free(decomp->divisor);
   #endif 
   #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
      CCMALLOC_free(decomp->a);
   #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
      TclpFree((char *)decomp->a);
   #else
      free(decomp->a);
   #endif 
   #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
      CCMALLOC_free(decomp->remainder);
   #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
      TclpFree((char *)decomp->remainder);
   #else
      free(decomp->remainder);
   #endif 
   #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
      CCMALLOC_free(decomp->p);
   #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
      TclpFree((char *)decomp->p);
   #else
      free(decomp->p);
   #endif 
   #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
      CCMALLOC_free(decomp->q);
   #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
      TclpFree((char *)decomp->q);
   #else
      free(decomp->q);
   #endif 
   }


/******************************************************************/
/***  FORMATTED OUTPUT FUNCTIONS  *********************************/
/******************************************************************/
//08/16/01: Visual inspection OK.
void GMP_RALG_cfdecomp_emit(
            FILE                        *s,
            char                        *banner,
            GMP_RALG_cf_app_struct      *decomp,
            int                          nf,
            int                          dap,
            const GMP_INTS_mpz_struct   *dap_denominator)
   {
   int i;

   GMP_INTS_mpz_struct arb_temp, arb_quotient, arb_remainder;

   //Eyeball the input parameters.  The banner is allowed to
   //be null, so can't check that.
   assert(s != NULL);
   assert(decomp != NULL);

   //Allocate our temporary integers.
   GMP_INTS_mpz_init(&arb_temp);
   GMP_INTS_mpz_init(&arb_quotient);
   GMP_INTS_mpz_init(&arb_remainder);

   //If banner requested and noformat option not used.
   if (banner && !nf)
      {
      FCMIOF_stream_bannerheading(s, banner, 1);
      }

   //Dump the input numerator.
   if (!nf)
      {
      GMP_INTS_mpz_long_int_format_to_stream(s,
                                             &(decomp->num),
                                             "Input Numerator");
      }
   else
      {
      GMP_INTS_mpz_arb_int_raw_to_stream(s, &(decomp->num));
      fprintf(s, "\n");
      }

   //Separator if not in unformatted mode.
   if (!nf)
      FCMIOF_stream_hline(s);

   //Dump the input denominator.
   if (!nf)
      {
      GMP_INTS_mpz_long_int_format_to_stream(s,
                                             &(decomp->den),
                                             "Input Denominator");
      }
   else
      {
      GMP_INTS_mpz_arb_int_raw_to_stream(s, &(decomp->den));
      fprintf(s, "\n");
      }

   //Separator if not in unformatted mode.
   if (!nf)
      FCMIOF_stream_hline(s);

   for (i=0; i<decomp->n; i++)
      {
      char strbuf[100];
         //Buffer to prepare description.

      //Print out the dividend at each round.
      if (!nf)
         {
         sprintf(strbuf, "dividend(%d)", i);
         GMP_INTS_mpz_long_int_format_to_stream(s,
                                                decomp->dividend + i,
                                                strbuf);
         }
      else
         {
         GMP_INTS_mpz_arb_int_raw_to_stream(s, decomp->dividend+i);
         fprintf(s, "\n");
         }

      //Separator if not in unformatted mode.
      if (!nf)
         FCMIOF_stream_hline(s);

      //Print out the divisor at each round.
      if (!nf)
         {
         sprintf(strbuf, "divisor(%d)", i);
         GMP_INTS_mpz_long_int_format_to_stream(s,
                                                decomp->divisor + i,
                                                strbuf);
         }
      else
         {
         GMP_INTS_mpz_arb_int_raw_to_stream(s, decomp->divisor+i);
         fprintf(s, "\n");
         }

      //Separator if not in unformatted mode.
      if (!nf)
         FCMIOF_stream_hline(s);

      //Print out partial quotient at each round.
      if (!nf)
         {
         sprintf(strbuf, "a(%d)", i);
         GMP_INTS_mpz_long_int_format_to_stream(s,
                                                decomp->a + i,
                                                strbuf);
         }
      else
         {
         GMP_INTS_mpz_arb_int_raw_to_stream(s, decomp->a+i);
         fprintf(s, "\n");
         }

      //Separator if not in unformatted mode.
      if (!nf)
         FCMIOF_stream_hline(s);

      //It doesn't make any sense to print out the 
      //remainder, because this becomes the divisor
      //for the next round.  It is just wasted output
      //lines.

      //Print out the convergent numerator at
      //each round.
      if (!nf)
         {
         sprintf(strbuf, "p(%d)", i);
         GMP_INTS_mpz_long_int_format_to_stream(s,
                                                decomp->p + i,
                                                strbuf);
         }
      else
         {
         GMP_INTS_mpz_arb_int_raw_to_stream(s, decomp->p+i);
         fprintf(s, "\n");
         }

      //Separator if not in unformatted mode.
      if (!nf)
         FCMIOF_stream_hline(s);

      //Print out the convergent denominator at
      //each round.
      if (!nf)
         {
         sprintf(strbuf, "q(%d)", i);
         GMP_INTS_mpz_long_int_format_to_stream(s,
                                                decomp->q + i,
                                                strbuf);
         }
      else
         {
         GMP_INTS_mpz_arb_int_raw_to_stream(s, decomp->q+i);
         fprintf(s, "\n");
         }

      //Separator if not in unformatted mode.
      if (!nf)
         FCMIOF_stream_hline(s);

      if (dap)
         {
         //Calculate the DAP numerator 
         GMP_INTS_mpz_mul(&arb_temp, dap_denominator, decomp->p + i);
         GMP_INTS_mpz_tdiv_qr(&arb_quotient, &arb_remainder,
                              &arb_temp, decomp->q + i);

         //Print DAP numerator.
         if (!nf)
            {
            sprintf(strbuf, "dap_h(%d)", i);
            GMP_INTS_mpz_long_int_format_to_stream(s,
                                                   &arb_quotient,
                                                   strbuf);
            }
         else
            {
            GMP_INTS_mpz_arb_int_raw_to_stream(s, &arb_quotient);
            fprintf(s, "\n");
            }

         //Separator if not in unformatted mode.
         if (!nf)
            FCMIOF_stream_hline(s);

         //Print DAP denominator.
         if (!nf)
            {
            sprintf(strbuf, "dap_k(%d)", i);
            GMP_INTS_mpz_long_int_format_to_stream(s,
                                                   dap_denominator,
                                                   strbuf);
            }
         else
            {
            GMP_INTS_mpz_arb_int_raw_to_stream(s, dap_denominator);
            fprintf(s, "\n");
            }

         //Separator if not in unformatted mode.
         if (!nf)
            FCMIOF_stream_hline(s);
         }
      }

   //Deallocate our temporary integers.
   GMP_INTS_mpz_clear(&arb_temp);
   GMP_INTS_mpz_clear(&arb_quotient);
   GMP_INTS_mpz_clear(&arb_remainder);
   }


/******************************************************************/
/***  FAREY SERIES PREDECESSOR AND SUCCESSOR FUNCTIONS  ***********/
/******************************************************************/
//08/16/01: Visual inspection OK.
void GMP_RALG_farey_predecessor(
                    GMP_RATS_mpq_struct *result,
              const GMP_RATS_mpq_struct *plus_two,
              const GMP_RATS_mpq_struct *plus_one,
              const GMP_INTS_mpz_struct *N)
   {
   GMP_RATS_mpq_struct result_copy;
      //Used to hold return value in case the result
      //is the same as either of the input arguments.
   GMP_INTS_mpz_struct temp1, temp2, floor_func;
      //Temporary integers.

   assert(result   != NULL);
   assert(plus_two != NULL);
   assert(plus_one != NULL);
   assert(N        != NULL);

   //Initialize the variables used.
   GMP_RATS_mpq_init(&result_copy);
   GMP_INTS_mpz_init(&temp1);
   GMP_INTS_mpz_init(&temp2);
   GMP_INTS_mpz_init(&floor_func);

   //Numerator of the term in the floor function.
   GMP_INTS_mpz_add(&temp1, &(plus_two->den), N);

   //Term in the floor function.  This is used to 
   //calculate both numerator and denominator, so we save it.
   GMP_INTS_mpz_tdiv_qr(&floor_func, &temp2, &temp1, &(plus_one->den));

   //Product of result of floor function and numerator--now
   //forming the numerator of the output.
   GMP_INTS_mpz_mul(&temp2, &floor_func, &(plus_one->num));

   //Final result assigned to numerator.
   GMP_INTS_mpz_sub(&(result_copy.num), &temp2, &(plus_two->num));

   //Product of result of floor function and denominator--now
   //forming the denominator of the output.
   GMP_INTS_mpz_mul(&temp2, &floor_func, &(plus_one->den));

   //Final result assigned to denominator.
   GMP_INTS_mpz_sub(&(result_copy.den), &temp2, &(plus_two->den));

   //Copy the result to the object owned by the caller.
   GMP_RATS_mpq_copy(result, &result_copy);

   //Deallocate dynamic memory.
   GMP_RATS_mpq_clear(&result_copy);
   GMP_INTS_mpz_clear(&temp1);
   GMP_INTS_mpz_clear(&temp2);
   GMP_INTS_mpz_clear(&floor_func);
   }


//08/16/01: Visual inspection OK.
void GMP_RALG_farey_successor(
                    GMP_RATS_mpq_struct *result,
              const GMP_RATS_mpq_struct *minus_two,
              const GMP_RATS_mpq_struct *minus_one,
              const GMP_INTS_mpz_struct *N)
   {
   GMP_RATS_mpq_struct result_copy;
      //Used to hold return value in case the result
      //is the same as either of the input arguments.
   GMP_INTS_mpz_struct temp1, temp2, floor_func;
      //Temporary integers.

   assert(result   != NULL);
   assert(minus_two != NULL);
   assert(minus_one != NULL);
   assert(N        != NULL);

   //Initialize the variables used.
   GMP_RATS_mpq_init(&result_copy);
   GMP_INTS_mpz_init(&temp1);
   GMP_INTS_mpz_init(&temp2);
   GMP_INTS_mpz_init(&floor_func);

   //Numerator of the term in the floor function.
   GMP_INTS_mpz_add(&temp1, &(minus_two->den), N);

   //Term in the floor function.  This is used to 
   //calculate both numerator and denominator, so we save it.
   GMP_INTS_mpz_tdiv_qr(&floor_func, &temp2, &temp1, &(minus_one->den));

   //Product of result of floor function and numerator--now
   //forming the numerator of the output.
   GMP_INTS_mpz_mul(&temp2, &floor_func, &(minus_one->num));

   //Final result assigned to numerator.
   GMP_INTS_mpz_sub(&(result_copy.num), &temp2, &(minus_two->num));

   //Product of result of floor function and denominator--now
   //forming the denominator of the output.
   GMP_INTS_mpz_mul(&temp2, &floor_func, &(minus_one->den));

   //Final result assigned to denominator.
   GMP_INTS_mpz_sub(&(result_copy.den), &temp2, &(minus_two->den));

   //Copy the result to the object owned by the caller.
   GMP_RATS_mpq_copy(result, &result_copy);

   //Deallocate dynamic memory.
   GMP_RATS_mpq_clear(&result_copy);
   GMP_INTS_mpz_clear(&temp1);
   GMP_INTS_mpz_clear(&temp2);
   GMP_INTS_mpz_clear(&floor_func);
   }


//08/16/01: Visual inspection OK.
void GMP_RALG_enclosing_farey_neighbors(
              const GMP_RATS_mpq_struct    *rn_in,
              const GMP_INTS_mpz_struct    *N,
              const GMP_RALG_cf_app_struct *cf_rep,
              int                          *equality,
                    GMP_RATS_mpq_struct    *left,
                    GMP_RATS_mpq_struct    *right)
   {
   GMP_RATS_mpq_struct rn_abs;
      //Absolute value of rational number supplied.
   GMP_RATS_mpq_struct previous_convergent;
      //Convergent before the one that has the denominator
      //not exceeding the order of the series.  Need to fudge
      //a little bit because don't have -1-th order convergents
      //tabulated.
   GMP_RATS_mpq_struct other_neighbor;
      //The other neighbor besides the highest-order convergent
      //without denominator too large.
   GMP_INTS_mpz_struct temp1, temp2, temp3, temp4;
      //Temporary integers.
   int ho_conv;
      //Index of highest-ordered convergent that does not have
      //denominator too large.

   //Eyeball the parameters.
   assert(rn_in     != NULL);
   assert(N         != NULL);
   assert(cf_rep    != NULL);
   assert(equality  != NULL);
   assert(left      != NULL);
   assert(right     != NULL);

   //Allocate dynamic variables.
   GMP_RATS_mpq_init(&rn_abs);
   GMP_RATS_mpq_init(&previous_convergent);
   GMP_RATS_mpq_init(&other_neighbor);
   GMP_INTS_mpz_init(&temp1);
   GMP_INTS_mpz_init(&temp2);
   GMP_INTS_mpz_init(&temp3);
   GMP_INTS_mpz_init(&temp4);

   //Zero is a troublesome case, because it requires us to
   //cross signs.  Split this case out explicitly.
   if (GMP_INTS_mpz_is_zero(&(rn_in->num)))
      {
      *equality = 1;  //0/1 a member of Farey series of any order.
      GMP_INTS_mpz_set_si(&(left->num), -1);
      GMP_INTS_mpz_copy(&(left->den), N);
      GMP_INTS_mpz_set_si(&(right->num), 1);
      GMP_INTS_mpz_copy(&(right->den), N);
      }
   else
      {
      //Make a copy of the rational number in.  As a condition of
      //using this function, it must be normalized, but it still
      //may be negative.  Our strategy is to treat the number as
      //positive, find the neighbors, then if it was negative
      //complement and re-order the neighbors.  In other words,
      //find neighbors to a negative number by symmetry, not
      //by forming the CF representation of a negative number.
      //Also, we can't touch the input parameter.
      GMP_RATS_mpq_copy(&rn_abs, rn_in);
      GMP_INTS_mpz_abs(&(rn_abs.num));

      //Find the index of the highest-ordered convergent
      //with a denominator not exceeding the denominator of
      //the rational number supplied.  The zero'th order
      //convergent has a denominator of 1, so that one
      //at least is safe.

      //Assign either the "left" or right
      //neighbor to be the highest-ordered
      //convergent with a denominator not exceeding the
      //denominator of the rational number input.  I say 
      //"either" because the properties of convergents let
      //us know based on the oddness or evenness of the order
      //which side it is on.
      ho_conv = 0;
      while (((ho_conv + 1) < cf_rep->n) && (GMP_INTS_mpz_cmp(cf_rep->q + ho_conv + 1, N) <= 0))
         {
         #if 0
         //Some questions about this loop--debugging output.
         printf("ho_conv : %d\n", ho_conv);
         GMP_INTS_mpz_long_int_format_to_stream(stdout,
                                                cf_rep->q + ho_conv + 1,
                                                "decomp_den");
         GMP_INTS_mpz_long_int_format_to_stream(stdout,
                                                &(rn_abs.den),
                                                "rn_in_den");
         printf("Compare result: %d\n\n", GMP_INTS_mpz_cmp(cf_rep->q + ho_conv + 1, &(rn_abs.den)));
         #endif

         ho_conv++;
         }

      if (INTFUNC_is_even(ho_conv))
         {
         GMP_INTS_mpz_copy(&(left->num), cf_rep->p + ho_conv);
         GMP_INTS_mpz_copy(&(left->den), cf_rep->q + ho_conv);
         }
      else
         {
         GMP_INTS_mpz_copy(&(right->num), cf_rep->p + ho_conv);
         GMP_INTS_mpz_copy(&(right->den), cf_rep->q + ho_conv);
         }

      //Now, we need to calculate the other neighbor based
      //on the standard formula.  This is a little tricky
      //because we don't have the -1-th order convergents
      //tabulated so we have to fudge a little bit.
      if (ho_conv == 0)
         {
         GMP_RATS_mpq_set_si(&previous_convergent, 1, 0);
         }
      else
         {
         GMP_INTS_mpz_copy(&(previous_convergent.num), cf_rep->p + ho_conv - 1);
         GMP_INTS_mpz_copy(&(previous_convergent.den), cf_rep->q + ho_conv - 1);
         }

      //Calculate the other neighbor according to the standard
      //formula.
      GMP_INTS_mpz_sub(&temp1, N, &(previous_convergent.den));
      GMP_INTS_mpz_tdiv_qr(&temp2, &temp3, &temp1, cf_rep->q + ho_conv);
      //temp2 now contains term from floor() function in formula.
      GMP_INTS_mpz_mul(&temp1, &temp2, cf_rep->p + ho_conv);
      GMP_INTS_mpz_add(&(other_neighbor.num), &temp1, &(previous_convergent.num));
      GMP_INTS_mpz_mul(&temp1, &temp2, cf_rep->q + ho_conv);
      GMP_INTS_mpz_add(&(other_neighbor.den), &temp1, &(previous_convergent.den));

      //Copy the other neighbor into the right slot.
      if (INTFUNC_is_even(ho_conv))
         {
         GMP_RATS_mpq_copy(right, &other_neighbor);
         }
      else
         {
         GMP_RATS_mpq_copy(left, &other_neighbor);
         }

      //Set the equality flag.  We have equality if and only
      //if the denominator of the rational number is less than
      //or equal to N.
      if (GMP_INTS_mpz_cmp(&(rn_abs.den), N) <= 0)
         {
         *equality = 1;
         }
      else
         {
         *equality = 0;
         }

      //In the event of equality, we don't really have the true
      //neighbors.  If the final convergent is even-ordered,
      //the left needs to be replaced.  If the final convergent
      //is odd-ordered, the right needs to be replaced.
      if (*equality)
         {
         if (INTFUNC_is_even(ho_conv))
            {
            //Left needs to be replaced.
            GMP_RALG_farey_predecessor(
                    left,
                    right,
                    &rn_abs,
                    N);
            }
         else
            {
            //Right needs to be replaced.
            GMP_RALG_farey_successor(
                    right,
                    left,
                    &rn_abs,
                    N);
            }
         }

      //OK, we should be all done.  The final catch is that if
      //the number supplied was negative, we need to invert
      //and re-order the neighbors.
      if (GMP_INTS_mpz_is_neg(&(rn_in->num)))
         {
         GMP_RATS_mpq_swap(left, right);
         GMP_INTS_mpz_negate(&(left->num));
         GMP_INTS_mpz_negate(&(right->num));
         }
      } //End if (rn==0) else clause

   //Deallocate dynamic variables.
   GMP_RATS_mpq_clear(&rn_abs);
   GMP_RATS_mpq_clear(&previous_convergent);
   GMP_RATS_mpq_clear(&other_neighbor);
   GMP_INTS_mpz_clear(&temp1);
   GMP_INTS_mpz_clear(&temp2);
   GMP_INTS_mpz_clear(&temp3);
   GMP_INTS_mpz_clear(&temp4);
   }



//08/16/01: Visual inspection OK.  Did not fully inspect the
//iterative part of this function.  Unit testing will be
//careful, expect that to catch any anomalies.
void GMP_RALG_consecutive_fab_terms(
                                    const GMP_RATS_mpq_struct *rn_in,
                                    const GMP_INTS_mpz_struct *kmax,
                                    const GMP_INTS_mpz_struct *hmax,
                                    int n_left_in,
                                    int n_right_in,
                                    GMP_RALG_fab_neighbor_collection_struct *result
                                    )
   {
   int   error_flag, equality_flag;
   char *estring_kmax_neg = "KMAX is zero, negative, or NAN.";
   char *estring_hmax_neg = "HMAX is negative or NAN.";
   char *estring_general  = "Unspecified general error in GMP_RALG_consecutive_fab_terms().";

   GMP_RATS_mpq_struct q_temp1, q_temp2, q_temp3, q_temp4, 
                       left_neighbor, right_neighbor, 
                       left_neighbor_abs, right_neighbor_abs,
                       hmax_over_one_neg, corner_point_neg,
                       abs_norm_recip_rn;

   //Eyeball input parameters.
   assert(rn_in      != NULL);
   assert(kmax       != NULL);
   assert(n_left_in  >= 0);
   assert(n_left_in  <= 0x00FFFFFF);
   assert(n_right_in >= 0);
   assert(n_right_in <= 0x00FFFFFF);
   assert(result     != NULL);

   //Allocate all of the dynamic memory we'll need for this function.  It will be
   //released at the end.
   GMP_RATS_mpq_init(&q_temp1);
   GMP_RATS_mpq_init(&q_temp2);
   GMP_RATS_mpq_init(&q_temp3);
   GMP_RATS_mpq_init(&q_temp4);
   GMP_RATS_mpq_init(&left_neighbor);
   GMP_RATS_mpq_init(&right_neighbor);
   GMP_RATS_mpq_init(&left_neighbor_abs);
   GMP_RATS_mpq_init(&right_neighbor_abs);
   GMP_RATS_mpq_init(&hmax_over_one_neg);
   GMP_RATS_mpq_init(&corner_point_neg);
   GMP_RATS_mpq_init(&abs_norm_recip_rn);

   //Zero out the result block.  This is the easiest way to give many variables
   //default values of 0, FALSE, and NULL.
   memset(result, 0, sizeof(GMP_RALG_fab_neighbor_collection_struct));

   //Allocate all integer and rational number structures in the result block.
   GMP_RATS_mpq_init(&(result->rn_in));
   GMP_INTS_mpz_init(&(result->kmax_in));
   GMP_INTS_mpz_init(&(result->hmax_in));
   GMP_RATS_mpq_init(&(result->hmax_over_one));
   GMP_RATS_mpq_init(&(result->corner_point));
   GMP_RATS_mpq_init(&(result->corner_point_minus_one));
   GMP_RATS_mpq_init(&(result->corner_point_plus_one));
   GMP_RATS_mpq_init(&(result->norm_rn));
   GMP_RATS_mpq_init(&(result->abs_norm_rn));

   //Fill in the rational number, exactly as passed.
   GMP_RATS_mpq_copy(&(result->rn_in), rn_in);

   //Fill in the number of left and right neighbors that the caller wants.
   //However, let's of course say nothing less than zero and nothing more
   //than 10000 neighbors on either side.
   result->n_left_in  = INTFUNC_min(INTFUNC_max(0, n_left_in), 10000);
   result->n_right_in = INTFUNC_min(INTFUNC_max(0, n_right_in), 10000);

   //Fill in the value of KMAX, exactly as passed.  If it is not at least
   //the value of 1 or if error flags, croak.
   GMP_INTS_mpz_copy(&(result->kmax_in), kmax);
   if (GMP_INTS_mpz_get_flags(kmax) || GMP_INTS_mpz_is_zero(kmax) || GMP_INTS_mpz_is_neg(kmax))
      {
      result->error = 
         #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
            CCMALLOC_malloc(sizeof(char) * (strlen(estring_kmax_neg) + 1));
         #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
            TclpAlloc(sizeof(char) * (strlen(estring_kmax_neg) + 1));
         #else
            malloc(sizeof(char) * (strlen(estring_kmax_neg) + 1));
         #endif 
      strcpy(result->error, estring_kmax_neg);
      goto return_point;
      }

   //Decide whether the caller intends to use HMAX.  Neg is error, but zero
   //is a signal that don't intend to use.
   if (hmax)
      {
      GMP_INTS_mpz_copy(&(result->hmax_in), hmax);
      if (GMP_INTS_mpz_get_flags(hmax) || GMP_INTS_mpz_is_neg(hmax))
         {
         result->error = 
            #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
               CCMALLOC_malloc(sizeof(char) * (strlen(estring_hmax_neg) + 1));
            #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
               TclpAlloc(sizeof(char) * (strlen(estring_hmax_neg) + 1));
            #else
               malloc(sizeof(char) * (strlen(estring_hmax_neg) + 1));
            #endif 
         strcpy(result->error, estring_hmax_neg);
         goto return_point;
         }
      else if (GMP_INTS_mpz_is_pos(hmax))
         {
         result->hmax_supplied = 1;
         }
      }

   //If HMAX has been supplied, assign and normalize the
   //corner point.
   if (result->hmax_supplied)
      {
      GMP_INTS_mpz_copy(&(result->corner_point.num), &(result->hmax_in));
      GMP_INTS_mpz_copy(&(result->corner_point.den), &(result->kmax_in));
      GMP_RATS_mpq_normalize(&(result->corner_point));
      }

   //If HMAX has been supplied, we want to get the continued
   //fraction representation of both the corner point and its
   //reciprocal.  This is because we're going to need to
   //find its adjacent points so we can easily crawl
   //around a rectangular region of the integer lattice.
   if (result->hmax_supplied)
      {
      //CF representation of corner point.
      GMP_RALG_cfdecomp_init(&(result->corner_point_cf_rep),
                             &error_flag,
                             &(result->corner_point.num),
                             &(result->corner_point.den));
      result->corner_point_cf_rep_formed = 1;

      //If there was an error forming the CF representation
      //of the corner point, bail out.
      if (error_flag)
         {
         result->error = 
            #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
               CCMALLOC_malloc(sizeof(char) * (strlen(estring_general) + 1));
            #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
               TclpAlloc(sizeof(char) * (strlen(estring_general) + 1));
            #else
               malloc(sizeof(char) * (strlen(estring_general) + 1));
            #endif 
         strcpy(result->error, estring_general);
         goto return_point;
         }

      //CF representation of reciprocal of corner point.
      GMP_RALG_cfdecomp_init(&(result->corner_point_recip_cf_rep),
                             &error_flag,
                             &(result->corner_point.den),
                             &(result->corner_point.num));
      result->corner_point_recip_cf_rep_formed = 1;

      //If there was an error forming the CF representation
      //of the reciprocal of the corner point, bail out.
      if (error_flag)
         {
         result->error = 
            #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
               CCMALLOC_malloc(sizeof(char) * (strlen(estring_general) + 1));
            #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
               TclpAlloc(sizeof(char) * (strlen(estring_general) + 1));
            #else
               malloc(sizeof(char) * (strlen(estring_general) + 1));
            #endif 
         strcpy(result->error, estring_general);
         goto return_point;
         }
      }

   //Normalize the rational number supplied.
   GMP_RATS_mpq_copy(&(result->norm_rn), rn_in);
   GMP_RATS_mpq_normalize(&(result->norm_rn));

   //Form the absolute value of the normalized
   //version, and set the neg flag.
   GMP_RATS_mpq_copy(&(result->abs_norm_rn),&(result->norm_rn));
   if (GMP_INTS_mpz_is_neg(&(result->abs_norm_rn.num)))
      {
      GMP_INTS_mpz_negate(&(result->abs_norm_rn.num));
      result->rn_is_neg = 1;
      }

   //Form the continued fraction representation of the
   //absolute value of the rational number supplied.
   //This is always required, because we cannot get any
   //neighbors without it.
   GMP_RALG_cfdecomp_init(&(result->rn_abs_cf_rep),
                          &error_flag,
                          &(result->abs_norm_rn.num),
                          &(result->abs_norm_rn.den));
   result->rn_abs_cf_rep_formed = 1;

   //If there was an error forming the CF representation
   //of the absolute value of rational number supplied, bail out.
   if (error_flag)
      {
      result->error = 
         #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
            CCMALLOC_malloc(sizeof(char) * (strlen(estring_general) + 1));
         #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
            TclpAlloc(sizeof(char) * (strlen(estring_general) + 1));
         #else
            malloc(sizeof(char) * (strlen(estring_general) + 1));
         #endif 
      strcpy(result->error, estring_general);
      goto return_point;
      }

   //Set the equality flag.  The rational number supplied is
   //in the series of interest if and only if, in its normalized
   //form, K <= KMAX, and if HMAX was supplied, H <= HMAX.
   if (GMP_INTS_mpz_cmp(&(result->abs_norm_rn.den), kmax) <= 0)
      {
      if (result->hmax_supplied)
         {
         if (GMP_INTS_mpz_cmp(&(result->abs_norm_rn.num), hmax) <= 0)
            {
            result->equality = 1;
            }
         else
            {
            result->equality = 0;
            }
         }
      else
         {
         result->equality = 1;
         }
      }
   else
      {
      result->equality = 0;
      }

   //The final cause of error is if the rational number
   //supplied is outside the interval [-HMAX/1, HMAX/1].
   //In such cases, simply refuse to calculate
   //any approximations.  This error can only occur
   //if HMAX is specified.  If only KMAX is specified,
   //this error cannot occur.
   if (result->hmax_supplied)
      {
      //Form the rational number HMAX/1.  We will use it for
      //a comparison.
      GMP_INTS_mpz_copy(&(result->hmax_over_one.num), hmax);
      GMP_INTS_mpz_set_ui(&(result->hmax_over_one.den), 1);

      //If the comparison shows that the absolute value of
      //the rational number in is larger than HMAX over 1,
      //then declare an error.
      if (GMP_RATS_mpq_cmp(&(result->abs_norm_rn),&(result->hmax_over_one),NULL) > 0)
         {
         result->error = 
            #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
               CCMALLOC_malloc(sizeof(char) * (strlen(estring_general) + 1));
            #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
               TclpAlloc(sizeof(char) * (strlen(estring_general) + 1));
            #else
               malloc(sizeof(char) * (strlen(estring_general) + 1));
            #endif 
         strcpy(result->error, estring_general);
         goto return_point;
         }
      }

   //If we're here, we're very much clean.  The only thing
   //that could go wrong is an overflow.

   //Allocate space for the left and right arrays of 
   //neighbors.
   if (result->n_left_in)
      {
      result->lefts = 
         #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
            CCMALLOC_malloc(sizeof(GMP_RALG_fab_neighbor_struct) * result->n_left_in);
         #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
            (GMP_RALG_fab_neighbor_struct *)
            TclpAlloc(sizeof(GMP_RALG_fab_neighbor_struct) * result->n_left_in);
         #else
            malloc(sizeof(GMP_RALG_fab_neighbor_struct) * result->n_left_in);
         #endif 
      }

   if (result->n_right_in)
      {
      result->rights = 
         #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
            CCMALLOC_malloc(sizeof(GMP_RALG_fab_neighbor_struct) * result->n_right_in);
         #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
            (GMP_RALG_fab_neighbor_struct *)
            TclpAlloc(sizeof(GMP_RALG_fab_neighbor_struct) * result->n_right_in);
         #else
            malloc(sizeof(GMP_RALG_fab_neighbor_struct) * result->n_right_in);
         #endif 
      }

   //If the rational number supplied is above the corner
   //point, we want to form the continued fraction representation
   //of its reciprocal.
   if (result->hmax_supplied)
      {
      if (GMP_RATS_mpq_cmp(&(result->abs_norm_rn),&(result->corner_point),NULL) > 0)
         {
         GMP_RALG_cfdecomp_init(&(result->rn_abs_recip_cf_rep),
                                &error_flag,
                                &(result->abs_norm_rn.den),
                                &(result->abs_norm_rn.num));
         result->rn_abs_recip_cf_rep_formed = 1;
         }
      }

   //If HMAX has been supplied, we want to calculate the points just below and above
   //the corner point.  The reason we want to do this is because we need to gracefully
   //"round the corner" in either direction.
   //
   //Calculate the point just to the left of the corner point.
   if (result->hmax_supplied)
      {
      GMP_RALG_enclosing_farey_neighbors(
                                      &(result->corner_point),
                                      &(result->kmax_in),
                                      &(result->corner_point_cf_rep),
                                      &equality_flag,
                                      &(result->corner_point_minus_one),
                                      &(q_temp1)
                                      );
      }

   //Calculate the point just to the right of the corner point.  This is
   //where HMAX is the dominant constraint.  We need to find the left
   //Farey neighbor to the reciprocal of the corner point in the Farey
   //series of order HMAX, then take its reciprocal.  There is the possibility
   //if KMAX=1 that this point will have a denominator of zero, but we
   //will accept that as a number here, since we should never hit it,
   //as it will be beyond HMAX/1.
   if (result->hmax_supplied)
      {
      GMP_RATS_mpq_copy(&q_temp1, &(result->corner_point));
      GMP_INTS_mpz_swap(&(q_temp1.num), &(q_temp1.den));
      GMP_RALG_enclosing_farey_neighbors(
                                      &q_temp1,
                                      &(result->hmax_in),
                                      &(result->corner_point_recip_cf_rep),
                                      &equality_flag,
                                      &(result->corner_point_plus_one),
                                      &(q_temp2)
                                      );
      GMP_INTS_mpz_swap(&(result->corner_point_plus_one.num), &(result->corner_point_plus_one.den));
      }

   //Calculate the complement of HMAX/1.  Nothing that we generate can go beyond
   //this to the south.
   if (result->hmax_supplied)
      {
      GMP_RATS_mpq_copy(&(hmax_over_one_neg), &(result->hmax_over_one));
      GMP_INTS_mpz_negate(&(hmax_over_one_neg.num));
      }

   //Also calculate the complement of HMAX/KMAX.
   if (result->hmax_supplied)
      {
      GMP_RATS_mpq_copy(&(corner_point_neg), &(result->corner_point));
      GMP_INTS_mpz_negate(&(corner_point_neg.num));
      }

   //Form the reciprocal of the absolute value of the normalized value of
   //the rational number in.
   GMP_RATS_mpq_copy(&abs_norm_recip_rn, &(result->abs_norm_rn));
   GMP_RATS_mpq_swap_components(&abs_norm_recip_rn);

   //OK, now we get down to brass tacks.  Iterate first to get the
   //left neighbors.  The ordinary complexity of this is also compounded
   //by the fact that we must handle negative numbers as well--everything
   //from -HMAX/1 to HMAX/1.
   //
   //PSEUDO-CODE:
   //  a)If the rational number to approximate is <= -HMAX/1 or there are no
   //    left neighbors requested, terminate with no neighbors on the left.
   //
   //  b)Find the right neighbor of the rational number supplied.
   //
   //  c)Find the left neighbor of the rational number supplied.
   //
   //  d)While (queued_count < count)
   //
   //     e)Queue the left neighbor, queued_count++
   //
   //     f)If (queued_count >= count), break.
   //
   //     g)If (left_neighbor <= -HMAX/1), break
   //
   //     h)Advance the frame one to the left.
   //
   //**************************************************************************
   //  a)If the rational number to approximate is <= -HMAX/1 or there are no
   //    left neighbors requested, terminate with no neighbors on the left.
   //**************************************************************************
   if ((result->hmax_supplied && GMP_RATS_mpq_cmp(&(result->norm_rn), &hmax_over_one_neg, NULL) <= 0)
      || (n_left_in <= 0))
      goto done_with_left_neighbors;

   //**************************************************************************
   //  b)Find the right neighbor of the rational number supplied.
   //**************************************************************************
   //  c)Find the left neighbor of the rational number supplied.
   //**************************************************************************
   if (!result->hmax_supplied)
      {
      //In this case, the notion of corner point is meaningless, because 
      //there is no constraint on H.  We can just go on our merry way.  Get
      //the two neighbors.
      GMP_RALG_enclosing_farey_neighbors(
                                      &(result->norm_rn),
                                      &(result->kmax_in),
                                      &(result->rn_abs_cf_rep),
                                      &equality_flag,
                                      &left_neighbor,
                                      &right_neighbor
                                      );
      //The enclosing Farey neighbor function is prohibited from identifying the
      //rational number itself as a Farey term.  If the number is in the Farey
      //series, we must replace the right neighbor, otherwise we cannot apply
      //the standard recursive formulas.
      if (equality_flag)
         {
         GMP_RATS_mpq_copy(&right_neighbor, &(result->norm_rn));
         }
      }
   else if (GMP_RATS_mpq_cmp(&(result->norm_rn), &corner_point_neg, NULL) < 0)
      {
      //The rational number specified is negative and below the negative corner point.
      //This means that HMAX is the dominant constraint.  We need to find the
      //neighbors in the Farey series of order HMAX, then reorder and invert, etc.
      GMP_RALG_enclosing_farey_neighbors(
                                      &abs_norm_recip_rn,
                                      &(result->hmax_in),
                                      &(result->rn_abs_recip_cf_rep),
                                      &equality_flag,
                                      &left_neighbor,
                                      &right_neighbor
                                      );

      //Again, if the number specified was already in the series of interest,
      //we need to swap in the right neighbor.
      if (equality_flag)
         {
         GMP_RATS_mpq_copy(&right_neighbor, &abs_norm_recip_rn);
         }

      //Take the reciprocal of both neighbors, which will put them out of order,
      //then negate them, which will put them back in order.
      GMP_RATS_mpq_swap_components(&left_neighbor);
      GMP_INTS_mpz_negate(&(left_neighbor.num));
      GMP_RATS_mpq_swap_components(&right_neighbor);
      GMP_INTS_mpz_negate(&(right_neighbor.num));
      }
   else if (GMP_RATS_mpq_cmp(&(result->norm_rn), &corner_point_neg, NULL) == 0)
      {
      //The rational number specified is the negative corner point.  In this case
      //Because we can never return the corner point itself as a left neighbor,
      //we need to set the left value to be the negative of one past, and the right
      //to be the negative of the corner point.
      GMP_RATS_mpq_copy(&left_neighbor, &(result->corner_point_plus_one));
      GMP_INTS_mpz_negate(&(left_neighbor.num));
      GMP_RATS_mpq_copy(&right_neighbor, &(result->corner_point));
      GMP_INTS_mpz_negate(&(right_neighbor.num));
      }
   else if ((GMP_RATS_mpq_cmp(&(result->norm_rn), &corner_point_neg, NULL) > 0) 
            && 
            (GMP_RATS_mpq_cmp(&(result->norm_rn), &(result->corner_point), NULL) < 0))
      {
      //The rational number specified is in the area dominated by the KMAX constraint
      //between -HMAX/KMAX and HMAX/KMAX.  The ordinary Farey neighbor function will
      //handle this correctly.  Again, we need to adjust the output if the number
      //is already formable, because the Farey neighbor function is prohibited from
      //returning the number itself as a neighbor.
      GMP_RALG_enclosing_farey_neighbors(
                                      &(result->norm_rn),
                                      &(result->kmax_in),
                                      &(result->rn_abs_cf_rep),
                                      &equality_flag,
                                      &left_neighbor,
                                      &right_neighbor
                                      );
      //The enclosing Farey neighbor function is prohibited from identifying the
      //rational number itself as a Farey term.  If the number is in the Farey
      //series, we must replace the right neighbor, otherwise we cannot apply
      //the standard recursive formulas.
      if (equality_flag)
         {
         GMP_RATS_mpq_copy(&right_neighbor, &(result->norm_rn));
         }
      }
   else if (GMP_RATS_mpq_cmp(&(result->norm_rn), &(result->corner_point), NULL) == 0)
      {
      //The rational number specified is the corner point.  In this case
      //because we can never return the corner point itself as a left neighbor,
      //we need to set the left value to be one before, and the right
      //to be the corner point.
      GMP_RATS_mpq_copy(&left_neighbor, &(result->corner_point_minus_one));
      GMP_RATS_mpq_copy(&right_neighbor, &(result->corner_point));
      }
   else
      {
      //The only possibility left is that the number is positive and above the
      //corner point where HMAX is the dominant constraint.
      GMP_RALG_enclosing_farey_neighbors(
                                      &abs_norm_recip_rn,
                                      &(result->hmax_in),
                                      &(result->rn_abs_recip_cf_rep),
                                      &equality_flag,
                                      &left_neighbor,
                                      &right_neighbor
                                      );

      //Again, if the number specified was already in the series of interest,
      //we need to swap in the neighbor.  This time, however, it must be the
      //left neighbor because taking the reciprocals will reverse the order.
      if (equality_flag)
         {
         GMP_RATS_mpq_copy(&left_neighbor, &abs_norm_recip_rn);
         }

      //Take the reciprocal of both neighbors, which will put them out of order,
      //then swap them, which will put them back in order.
      GMP_RATS_mpq_swap_components(&left_neighbor);
      GMP_RATS_mpq_swap_components(&right_neighbor);
      GMP_RATS_mpq_swap(&left_neighbor, &right_neighbor);
      }

   #if 0
   //Print out the left neighbor and right neighbor determined, for debugging.
   GMP_INTS_mpz_long_int_format_to_stream(stdout,
                                          &(left_neighbor.num),
                                          "left_neigh_num");
   GMP_INTS_mpz_long_int_format_to_stream(stdout,
                                          &(left_neighbor.den),
                                          "left_neigh_den");
   GMP_INTS_mpz_long_int_format_to_stream(stdout,
                                          &(right_neighbor.num),
                                          "right_neigh_num");
   GMP_INTS_mpz_long_int_format_to_stream(stdout,
                                          &(right_neighbor.den),
                                          "right_neigh_den");
   #endif


   //**************************************************************************
   //  d)While (queued_count < count)
   //**************************************************************************
   while (result->n_left_out < result->n_left_in)
      {
   //**************************************************************************
   //     e)Queue the left neighbor, queued_count++
   //**************************************************************************
      (result->lefts + result->n_left_out)->index = -(result->n_left_out + 1);
      GMP_RATS_mpq_init(&((result->lefts + result->n_left_out)->neighbor));
      GMP_RATS_mpq_copy(&((result->lefts + result->n_left_out)->neighbor), &left_neighbor);
      (result->n_left_out)++;

   //**************************************************************************
   //     f)If (queued_count >= count), break.
   //**************************************************************************
   //By the way, this step is to save unnecessary contortions once we've met
   //the quota.
      if (result->n_left_out >= result->n_left_in)
         break;

   //**************************************************************************
   //     g)If (left_neighbor <= -HMAX/1), break
   //**************************************************************************
   //This breaks us when we've queued the most negative number we can--can't go
   //further.  This only applies for cases where KMAX is also specified.
      if (result->hmax_supplied 
          && 
          GMP_RATS_mpq_cmp(&left_neighbor, &hmax_over_one_neg, NULL) <= 0)
         break;

   //**************************************************************************
   //     h)Advance the frame one to the left.
   //**************************************************************************
   //Advancing one frame to the left is a complicated affair, requiring several
   //subcases.  We break it up into regions which are best visualized using
   //a graph of the integer lattice with dots for each rational number.
      if (!(result->hmax_supplied))
         {
         //This is the case where we're are looking only in the 
         //Farey series.
         if (GMP_INTS_mpz_is_pos(&left_neighbor.num))
            {
            //In this case, the left neighbor and right neighbor
            //are both positive, and we can apply the standard
            //formulas.
            GMP_RALG_farey_predecessor(&q_temp1,
                                       &right_neighbor,
                                       &left_neighbor,
                                       &(result->kmax_in));
            GMP_RATS_mpq_copy(&right_neighbor, &left_neighbor);
            GMP_RATS_mpq_copy(&left_neighbor,  &q_temp1);
            }
         else if (GMP_INTS_mpz_is_zero(&left_neighbor.num))
            {
            //In this case, we are making the transition from positive
            //to negative.
            GMP_INTS_mpz_set_si(&(left_neighbor.num), -1); 
            GMP_INTS_mpz_copy(&(left_neighbor.den), &(result->kmax_in));
            GMP_RATS_mpq_set_si(&right_neighbor, 0, 1);
            }
         else
            {
            //Here, we are purely negative and decreasing.  Need to negate
            //the numbers, find successor, then negate.
            GMP_RATS_mpq_copy(&q_temp1, &left_neighbor);
            GMP_RATS_mpq_copy(&q_temp2, &right_neighbor);
            GMP_INTS_mpz_abs(&(q_temp1.num));
            GMP_INTS_mpz_abs(&(q_temp2.num));
            GMP_RALG_farey_successor(&q_temp3,
                                     &q_temp2,
                                     &q_temp1,
                                     &(result->kmax_in));
            GMP_RATS_mpq_copy(&right_neighbor, &left_neighbor);
            GMP_RATS_mpq_copy(&left_neighbor, &q_temp3);
            GMP_INTS_mpz_negate(&(left_neighbor.num));
            }
         }
      else if (GMP_RATS_mpq_cmp(&left_neighbor, &(result->corner_point), NULL) > 0)
         {
         //We are above the top corner point.  In this case HMAX is the dominant
         //constraint, and we find our food by taking reciprocals and applying
         //the standard relationships in the Farey series of order HMAX.
         GMP_RATS_mpq_copy(&q_temp1, &left_neighbor);
         GMP_RATS_mpq_copy(&q_temp2, &right_neighbor);
         GMP_RATS_mpq_swap_components(&q_temp1);
         GMP_RATS_mpq_swap_components(&q_temp2);
         GMP_RALG_farey_successor(&q_temp3,
                                  &q_temp2,
                                  &q_temp1,
                                  &(result->hmax_in));
         GMP_RATS_mpq_swap_components(&q_temp3);
         GMP_RATS_mpq_copy(&right_neighbor, &left_neighbor);
         GMP_RATS_mpq_copy(&left_neighbor, &q_temp3);
         }
      else if (GMP_RATS_mpq_cmp(&left_neighbor, &(result->corner_point), NULL) == 0)
         {
         //We are precisely at the corner point.  This is where we round the corner.
         GMP_RATS_mpq_copy(&right_neighbor, &left_neighbor);
         GMP_RATS_mpq_copy(&left_neighbor, &(result->corner_point_minus_one));
         }
      else if (GMP_INTS_mpz_is_pos(&left_neighbor.num))
         {
         //In this region we are going straight down the Farey series.
         GMP_RALG_farey_predecessor(&q_temp1,
                                    &right_neighbor,
                                    &left_neighbor,
                                    &(result->kmax_in));
         GMP_RATS_mpq_copy(&right_neighbor, &left_neighbor);
         GMP_RATS_mpq_copy(&left_neighbor,  &q_temp1);
         }
      else if (GMP_INTS_mpz_is_zero(&left_neighbor.num))
         {
         //In this case, we are making the transition from positive
         //to negative.
         GMP_INTS_mpz_set_si(&(left_neighbor.num), -1); 
         GMP_INTS_mpz_copy(&(left_neighbor.den), &(result->kmax_in));
         GMP_RATS_mpq_set_si(&right_neighbor, 0, 1);
         }
      else if (GMP_RATS_mpq_cmp(&left_neighbor, &corner_point_neg, NULL) > 0)
         {
         //Here, we are purely negative and decreasing.  Need to negate
         //the numbers, find successor, then negate.
         GMP_RATS_mpq_copy(&q_temp1, &left_neighbor);
         GMP_RATS_mpq_copy(&q_temp2, &right_neighbor);
         GMP_INTS_mpz_abs(&(q_temp1.num));
         GMP_INTS_mpz_abs(&(q_temp2.num));
         GMP_RALG_farey_successor(&q_temp3,
                                  &q_temp2,
                                  &q_temp1,
                                  &(result->kmax_in));
         GMP_RATS_mpq_copy(&right_neighbor, &left_neighbor);
         GMP_RATS_mpq_copy(&left_neighbor, &q_temp3);
         GMP_INTS_mpz_negate(&(left_neighbor.num));
         }
      else if (GMP_RATS_mpq_cmp(&left_neighbor, &corner_point_neg, NULL) == 0)
         {
         //This is where we are rounding the negative corner.
         GMP_RATS_mpq_copy(&right_neighbor, &left_neighbor);
         GMP_RATS_mpq_copy(&left_neighbor, &(result->corner_point_plus_one));
         GMP_INTS_mpz_negate(&(left_neighbor.num));
         }
      else
         {
         //Here we're going in the negative direction along the "bottom" of the
         //rectangle.
         GMP_RATS_mpq_copy(&q_temp1, &left_neighbor);
         GMP_RATS_mpq_copy(&q_temp2, &right_neighbor);
         GMP_INTS_mpz_abs(&(q_temp1.num));
         GMP_INTS_mpz_abs(&(q_temp2.num));
         GMP_RATS_mpq_swap_components(&q_temp1);
         GMP_RATS_mpq_swap_components(&q_temp2);
         GMP_RALG_farey_predecessor(&q_temp3,
                                    &q_temp2,
                                    &q_temp1,
                                    &(result->hmax_in));
         GMP_RATS_mpq_swap_components(&q_temp3);
         GMP_INTS_mpz_negate(&(q_temp3.num));
         GMP_RATS_mpq_copy(&right_neighbor, &left_neighbor);
         GMP_RATS_mpq_copy(&left_neighbor, &q_temp3);
         }
      }


   done_with_left_neighbors: ;

   //**************************************************************************
   //  a)If the rational number to approximate is >= HMAX/1 or there are no
   //    right neighbors requested, terminate with no neighbors on the right.
   //**************************************************************************
   if ((result->hmax_supplied && GMP_RATS_mpq_cmp(&(result->norm_rn), &(result->hmax_over_one), NULL) >= 0)
      || (n_right_in <= 0))
      goto done_with_right_neighbors;

   //**************************************************************************
   //  b)Find the right neighbor of the rational number supplied.
   //**************************************************************************
   //  c)Find the left neighbor of the rational number supplied.
   //**************************************************************************
   if (!result->hmax_supplied)
      {
      //In this case, the notion of corner point is meaningless, because 
      //there is no constraint on H.  We can just go on our merry way.  Get
      //the two neighbors.
      GMP_RALG_enclosing_farey_neighbors(
                                      &(result->norm_rn),
                                      &(result->kmax_in),
                                      &(result->rn_abs_cf_rep),
                                      &equality_flag,
                                      &left_neighbor,
                                      &right_neighbor
                                      );
      //The enclosing Farey neighbor function is prohibited from identifying the
      //rational number itself as a Farey term.  If the number is in the Farey
      //series, we must replace the left neighbor, otherwise we cannot apply
      //the standard recursive formulas.
      if (equality_flag)
         {
         GMP_RATS_mpq_copy(&left_neighbor, &(result->norm_rn));
         }
      }
   else if (GMP_RATS_mpq_cmp(&(result->norm_rn), &corner_point_neg, NULL) < 0)
      {
      //The rational number specified is negative and below the negative corner point.
      //This means that HMAX is the dominant constraint.  We need to find the
      //neighbors in the Farey series of order HMAX, then reorder and invert, etc.
      GMP_RALG_enclosing_farey_neighbors(
                                      &abs_norm_recip_rn,
                                      &(result->hmax_in),
                                      &(result->rn_abs_recip_cf_rep),
                                      &equality_flag,
                                      &left_neighbor,
                                      &right_neighbor
                                      );

      //Again, if the number specified was already in the series of interest,
      //we need to swap in the left neighbor.
      if (equality_flag)
         {
         GMP_RATS_mpq_copy(&left_neighbor, &abs_norm_recip_rn);
         }

      //Take the reciprocal of both neighbors, which will put them out of order,
      //then negate them, which will put them back in order.
      GMP_RATS_mpq_swap_components(&left_neighbor);
      GMP_INTS_mpz_negate(&(left_neighbor.num));
      GMP_RATS_mpq_swap_components(&right_neighbor);
      GMP_INTS_mpz_negate(&(right_neighbor.num));
      }
   else if (GMP_RATS_mpq_cmp(&(result->norm_rn), &corner_point_neg, NULL) == 0)
      {
      //The rational number specified is the negative corner point.  In this case
      //Because we can never return the corner point itself as a left neighbor,
      //we need to set the right value to be the negative of one before, and the left
      //to be the negative of the corner point.
      GMP_RATS_mpq_copy(&left_neighbor, &(result->corner_point));
      GMP_INTS_mpz_negate(&(left_neighbor.num));
      GMP_RATS_mpq_copy(&right_neighbor, &(result->corner_point_minus_one));
      GMP_INTS_mpz_negate(&(right_neighbor.num));
      }
   else if ((GMP_RATS_mpq_cmp(&(result->norm_rn), &corner_point_neg, NULL) > 0) 
            && 
            (GMP_RATS_mpq_cmp(&(result->norm_rn), &(result->corner_point), NULL) < 0))
      {
      //The rational number specified is in the area dominated by the KMAX constraint
      //between -HMAX/KMAX and HMAX/KMAX.  The ordinary Farey neighbor function will
      //handle this correctly.  Again, we need to adjust the output if the number
      //is already formable, because the Farey neighbor function is prohibited from
      //returning the number itself as a neighbor.
      GMP_RALG_enclosing_farey_neighbors(
                                      &(result->norm_rn),
                                      &(result->kmax_in),
                                      &(result->rn_abs_cf_rep),
                                      &equality_flag,
                                      &left_neighbor,
                                      &right_neighbor
                                      );
      //The enclosing Farey neighbor function is prohibited from identifying the
      //rational number itself as a Farey term.  If the number is in the Farey
      //series, we must replace the left neighbor, otherwise we cannot apply
      //the standard recursive formulas.
      if (equality_flag)
         {
         GMP_RATS_mpq_copy(&left_neighbor, &(result->norm_rn));
         }
      }
   else if (GMP_RATS_mpq_cmp(&(result->norm_rn), &(result->corner_point), NULL) == 0)
      {
      //The rational number specified is the positive corner point.  In this case.
      //because we can never return the corner point itself as a right neighbor,
      //we need to set the right value to be one after, and the left
      //to be the corner point.
      GMP_RATS_mpq_copy(&left_neighbor, &(result->corner_point));
      GMP_RATS_mpq_copy(&right_neighbor, &(result->corner_point_plus_one));
      }
   else
      {
      //The only possibility left is that the number is positive and at or above the
      //corner point where HMAX is the dominant constraint.
      GMP_RALG_enclosing_farey_neighbors(
                                      &abs_norm_recip_rn,
                                      &(result->hmax_in),
                                      &(result->rn_abs_recip_cf_rep),
                                      &equality_flag,
                                      &left_neighbor,
                                      &right_neighbor
                                      );

      //Again, if the number specified was already in the series of interest,
      //we need to swap in the neighbor.  This time, however, it must be the
      //right neighbor because taking the reciprocals will reverse the order.
      if (equality_flag)
         {
         GMP_RATS_mpq_copy(&right_neighbor, &abs_norm_recip_rn);
         }

      //Take the reciprocal of both neighbors, which will put them out of order,
      //then swap them, which will put them back in order.
      GMP_RATS_mpq_swap_components(&left_neighbor);
      GMP_RATS_mpq_swap_components(&right_neighbor);
      GMP_RATS_mpq_swap(&left_neighbor, &right_neighbor);
      }

   #if 0
   //Print out the left neighbor and right neighbor determined, for debugging.
   GMP_INTS_mpz_long_int_format_to_stream(stdout,
                                          &(left_neighbor.num),
                                          "left_neigh_num");
   GMP_INTS_mpz_long_int_format_to_stream(stdout,
                                          &(left_neighbor.den),
                                          "left_neigh_den");
   GMP_INTS_mpz_long_int_format_to_stream(stdout,
                                          &(right_neighbor.num),
                                          "right_neigh_num");
   GMP_INTS_mpz_long_int_format_to_stream(stdout,
                                          &(right_neighbor.den),
                                          "right_neigh_den");
   #endif


   //**************************************************************************
   //  d)While (queued_count < count)
   //**************************************************************************
   while (result->n_right_out < result->n_right_in)
      {
   //**************************************************************************
   //     e)Queue the right neighbor, queued_count++
   //**************************************************************************
      (result->rights + result->n_right_out)->index = (result->n_right_out + 1);
      GMP_RATS_mpq_init(&((result->rights + result->n_right_out)->neighbor));
      GMP_RATS_mpq_copy(&((result->rights + result->n_right_out)->neighbor), &right_neighbor);
      (result->n_right_out)++;

   //**************************************************************************
   //     f)If (queued_count >= count), break.
   //**************************************************************************
   //By the way, this step is to save unnecessary contortions once we've met
   //the quota.
      if (result->n_right_out >= result->n_right_in)
         break;

   //**************************************************************************
   //     g)If (right_neighbor >= HMAX/1), break
   //**************************************************************************
   //This breaks us when we've queued the most positive number we can--can't go
   //further.  This only applies for cases where KMAX is also specified.
      if (result->hmax_supplied 
          && 
          GMP_RATS_mpq_cmp(&right_neighbor, &(result->hmax_over_one), NULL) >= 0)
         break;

   //**************************************************************************
   //     h)Advance the frame one to the right.
   //**************************************************************************
   //Advancing one frame to the right is a complicated affair, requiring several
   //subcases.  We break it up into regions which are best visualized using
   //a graph of the integer lattice with dots for each rational number.
      if (!(result->hmax_supplied))
         {
         //This is the case where we're are looking only in the 
         //Farey series.
         if (GMP_INTS_mpz_is_neg(&right_neighbor.num))
            {
            //Neg and increasing towards zero.  Can handle by symmetry.
            GMP_RATS_mpq_copy(&q_temp1, &left_neighbor);
            GMP_RATS_mpq_copy(&q_temp2, &right_neighbor);
            GMP_INTS_mpz_abs(&(q_temp1.num));
            GMP_INTS_mpz_abs(&(q_temp2.num));
            GMP_RALG_farey_predecessor(&q_temp3,
                                     &q_temp1,
                                     &q_temp2,
                                     &(result->kmax_in));
            GMP_RATS_mpq_copy(&left_neighbor,  &right_neighbor);
            GMP_RATS_mpq_copy(&right_neighbor, &q_temp3);
            GMP_INTS_mpz_negate(&(right_neighbor.num));
            }
         else if (GMP_INTS_mpz_is_zero(&right_neighbor.num))
            {
            //Right term just hit zero and are increasing.  
            //Left will become 0/1, right becomes 1/KMAX.
            GMP_RATS_mpq_set_si(&left_neighbor, 0, 1);
            GMP_INTS_mpz_set_si(&(right_neighbor.num), 1); 
            GMP_INTS_mpz_copy(&(right_neighbor.den), &(result->kmax_in));
            }
         else
            {
            //Are above zero and increasing.  Can use standard Farey
            //successor formula.
            GMP_RALG_farey_successor(&q_temp1,
                                     &left_neighbor,
                                     &right_neighbor,
                                     &(result->kmax_in));
            GMP_RATS_mpq_copy(&left_neighbor,   &right_neighbor);
            GMP_RATS_mpq_copy(&right_neighbor,  &q_temp1);
            }
         }
      else if (GMP_RATS_mpq_cmp(&right_neighbor, &corner_point_neg, NULL) < 0)
         {
         //We are below the negative corner point and moving towards
         //zero, with HMAX the dominant constraint.  We can proceed by
         //symmetry, producing a Farey successor and negating and inverting.
         GMP_RATS_mpq_copy(&q_temp1, &left_neighbor);
         GMP_RATS_mpq_copy(&q_temp2, &right_neighbor);
         GMP_INTS_mpz_abs(&(q_temp1.num));
         GMP_INTS_mpz_abs(&(q_temp2.num));
         GMP_RATS_mpq_swap_components(&q_temp1);
         GMP_RATS_mpq_swap_components(&q_temp2);
         GMP_RALG_farey_successor(&q_temp3,
                                  &q_temp1,
                                  &q_temp2,
                                  &(result->hmax_in));
         GMP_RATS_mpq_swap_components(&q_temp3);
         GMP_INTS_mpz_negate(&(q_temp3.num));
         GMP_RATS_mpq_copy(&left_neighbor, &right_neighbor);
         GMP_RATS_mpq_copy(&right_neighbor, &q_temp3);
         }
      else if (GMP_RATS_mpq_cmp(&right_neighbor, &corner_point_neg, NULL) == 0)
         {
         //We are at the bottom negative corner point and need to "round the corner".
         GMP_RATS_mpq_copy(&left_neighbor, &right_neighbor);
         GMP_RATS_mpq_copy(&right_neighbor, &(result->corner_point_minus_one));
         GMP_INTS_mpz_negate(&(right_neighbor.num));
         }
      else if (GMP_INTS_mpz_is_neg(&right_neighbor.num))
         {
         //In this region we are going straight up the Farey series approaching
         //zero.  Need to negate to use standard relationships.
         GMP_RATS_mpq_copy(&q_temp1, &left_neighbor);
         GMP_RATS_mpq_copy(&q_temp2, &right_neighbor);
         GMP_INTS_mpz_abs(&(q_temp1.num));
         GMP_INTS_mpz_abs(&(q_temp2.num));
         GMP_RALG_farey_predecessor(&q_temp3,
                                    &q_temp1,
                                    &q_temp2,
                                    &(result->kmax_in));
         GMP_RATS_mpq_copy(&left_neighbor, &right_neighbor);
         GMP_RATS_mpq_copy(&right_neighbor,  &q_temp3);
         GMP_INTS_mpz_negate(&(right_neighbor.num));
         }
      else if (GMP_INTS_mpz_is_zero(&right_neighbor.num))
         {
         //Zero crossover.
         GMP_RATS_mpq_set_si(&left_neighbor, 0, 1);
         GMP_INTS_mpz_set_si(&(right_neighbor.num), 1); 
         GMP_INTS_mpz_copy(&(right_neighbor.den), &(result->kmax_in));
         }
      else if (GMP_RATS_mpq_cmp(&right_neighbor, &(result->corner_point), NULL) < 0)
         {
         //Below corner point.  Standard relationship applies.
         GMP_RALG_farey_successor(&q_temp1,
                                  &left_neighbor,
                                  &right_neighbor,
                                  &(result->kmax_in));
         GMP_RATS_mpq_copy(&left_neighbor,   &right_neighbor);
         GMP_RATS_mpq_copy(&right_neighbor,  &q_temp1);
         }
      else if (GMP_RATS_mpq_cmp(&right_neighbor, &(result->corner_point), NULL) == 0)
         {
         //At the positive corner point.
         GMP_RATS_mpq_copy(&left_neighbor, &right_neighbor);
         GMP_RATS_mpq_copy(&right_neighbor, &(result->corner_point_plus_one));
         }
      else
         {
         //Above the positive corner point and heading for HMAX/1.
         GMP_RATS_mpq_copy(&q_temp1, &left_neighbor);
         GMP_RATS_mpq_copy(&q_temp2, &right_neighbor);
         GMP_RATS_mpq_swap_components(&q_temp1);
         GMP_RATS_mpq_swap_components(&q_temp2);
         GMP_RALG_farey_predecessor(&q_temp3,
                                    &q_temp1,
                                    &q_temp2,
                                    &(result->hmax_in));
         GMP_RATS_mpq_swap_components(&q_temp3);
         GMP_RATS_mpq_copy(&left_neighbor, &right_neighbor);
         GMP_RATS_mpq_copy(&right_neighbor, &q_temp3);
         }
      }

   done_with_right_neighbors: ;

   //This is a single return point so we catch all the dynamic memory 
   //deallocation.
   return_point:
      GMP_RATS_mpq_clear(&q_temp1);
      GMP_RATS_mpq_clear(&q_temp2);
      GMP_RATS_mpq_clear(&q_temp3);
      GMP_RATS_mpq_clear(&q_temp4);
      GMP_RATS_mpq_clear(&left_neighbor);
      GMP_RATS_mpq_clear(&right_neighbor);
      GMP_RATS_mpq_clear(&left_neighbor_abs);
      GMP_RATS_mpq_clear(&right_neighbor_abs);
      GMP_RATS_mpq_clear(&hmax_over_one_neg);
      GMP_RATS_mpq_clear(&corner_point_neg);
      GMP_RATS_mpq_clear(&abs_norm_recip_rn);
   }


//08/16/01: Visual inspection OK.
void GMP_RALG_consecutive_fab_terms_result_free(
                                               GMP_RALG_fab_neighbor_collection_struct *arg
                                               )
   {
   int i;

   //Eyeball the input.
   assert(arg != NULL);

   //Deallocate all rational numbers and integers that MUST be allocated, i.e. they are
   //never conditional.
   GMP_RATS_mpq_clear(&(arg->rn_in));
   GMP_INTS_mpz_clear(&(arg->kmax_in));
   GMP_INTS_mpz_clear(&(arg->hmax_in));
   GMP_RATS_mpq_clear(&(arg->hmax_over_one));
   GMP_RATS_mpq_clear(&(arg->corner_point));
   GMP_RATS_mpq_clear(&(arg->corner_point_minus_one));
   GMP_RATS_mpq_clear(&(arg->corner_point_plus_one));
   GMP_RATS_mpq_clear(&(arg->norm_rn));
   GMP_RATS_mpq_clear(&(arg->abs_norm_rn));

   //Destroy any continued fraction representations that were
   //formed.
   if (arg->rn_abs_cf_rep_formed)
      {
      GMP_RALG_cfdecomp_destroy(&(arg->rn_abs_cf_rep));
      }
   if (arg->rn_abs_recip_cf_rep_formed)
      {
      GMP_RALG_cfdecomp_destroy(&(arg->rn_abs_recip_cf_rep));
      }
   if(arg->corner_point_cf_rep_formed)
      {
      GMP_RALG_cfdecomp_destroy(&(arg->corner_point_cf_rep));
      }
   if(arg->corner_point_recip_cf_rep_formed)
      {
      GMP_RALG_cfdecomp_destroy(&(arg->corner_point_recip_cf_rep));
      }

   //Walk through the lefts, freeing up any allocated rational numbers.
   for (i=0; i < arg->n_left_out; i++)
      {
      GMP_RATS_mpq_clear(&(arg->lefts[i].neighbor));
      }

   //Walk through the rights, freeing up any allocated rational numbers.
   for (i=0; i < arg->n_right_out; i++)
      {
      GMP_RATS_mpq_clear(&(arg->rights[i].neighbor));
      }
   
   //Deallocate any area assigned for lefts.
   if (arg->lefts)
      {
      #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
         CCMALLOC_free(arg->lefts);
      #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
         TclpFree((char *)arg->lefts);
      #else
         free(arg->lefts);
      #endif 

      arg->lefts = NULL;
      }

   //Deallocate any area assigned for rights.
   if (arg->rights)
      {
      #if defined(APP_TYPE_SIMPLE_DOS_CONSOLE)
         CCMALLOC_free(arg->rights);
      #elif defined(APP_TYPE_IJUSCRIPTER_IJUCONSOLE)
         TclpFree((char *)arg->rights);
      #else
         free(arg->rights);
      #endif 

      arg->rights = NULL;
      }
   }


//08/16/01: Visual inspection OK.
void GMP_RALG_consecutive_fab_terms_result_dump(
                                               FILE *s,
                                               GMP_RALG_fab_neighbor_collection_struct *arg
                                               )
   {
   int i;
   char buf[250];

   //Eyeball the input parameters.
   assert(s   != NULL);
   assert(arg != NULL);

   //Announce intent.
   FCMIOF_stream_bannerheading(s,
                               "Emitting Neighbor Decomp For Analysis", 
                               1);

   //Dump the fields, in order.
   GMP_INTS_mpz_long_int_format_to_stream(s,
                                          &(arg->rn_in.num),
                                          "rn_in_num");
   GMP_INTS_mpz_long_int_format_to_stream(s,
                                          &(arg->rn_in.den),
                                          "rn_in_den");
   GMP_INTS_mpz_long_int_format_to_stream(s,
                                          &(arg->kmax_in),
                                          "kmax_in");
   fprintf(s, "       hmax_supplied:                          %12d\n", arg->hmax_supplied);
   GMP_INTS_mpz_long_int_format_to_stream(s,
                                          &(arg->hmax_in),
                                          "hmax_in");
   if (arg->error)
      {
      fprintf(s, "               error: %s\n", arg->error);
      }
   else
      {
      fprintf(s, "               error:                                     NULL\n");
      }

   GMP_INTS_mpz_long_int_format_to_stream(s,
                                          &(arg->corner_point.num),
                                          "corner_point_num");
   GMP_INTS_mpz_long_int_format_to_stream(s,
                                          &(arg->corner_point.den),
                                          "corner_point_den");

   GMP_INTS_mpz_long_int_format_to_stream(s,
                                          &(arg->corner_point_minus_one.num),
                                          "cor_pt_minus_one_num");
   GMP_INTS_mpz_long_int_format_to_stream(s,
                                          &(arg->corner_point_minus_one.den),
                                          "cor_pt_minus_one_den");

   GMP_INTS_mpz_long_int_format_to_stream(s,
                                          &(arg->corner_point_plus_one.num),
                                          "cor_pt_plus_one_num");
   GMP_INTS_mpz_long_int_format_to_stream(s,
                                          &(arg->corner_point_plus_one.den),
                                          "cor_pt_plus_one_den");

   GMP_INTS_mpz_long_int_format_to_stream(s,
                                          &(arg->hmax_over_one.num),
                                          "hmax/1_num");
   GMP_INTS_mpz_long_int_format_to_stream(s,
                                          &(arg->hmax_over_one.den),
                                          "hmax/1_den");

   GMP_INTS_mpz_long_int_format_to_stream(s,
                                          &(arg->norm_rn.num),
                                          "norm_rn_num");
   GMP_INTS_mpz_long_int_format_to_stream(s,
                                          &(arg->norm_rn.den),
                                          "norm_rn_den");

   GMP_INTS_mpz_long_int_format_to_stream(s,
                                          &(arg->abs_norm_rn.num),
                                          "abs_norm_rn_num");
   GMP_INTS_mpz_long_int_format_to_stream(s,
                                          &(arg->abs_norm_rn.den),
                                          "abs_norm_rn_den");

   fprintf(s, "           rn_is_neg:                          %12d\n", arg->rn_is_neg);

   fprintf(s, "           n_left_in:                          %12d\n", arg->n_left_in);

   fprintf(s, "          n_right_in:                          %12d\n", arg->n_right_in);

   fprintf(s, "rn_abs_cf_rep_formed:                          %12d\n", arg->rn_abs_cf_rep_formed);

   if (arg->rn_abs_cf_rep_formed)
      {
      GMP_RALG_cfdecomp_emit(s, "Abs Of RN In CF Decomp", &(arg->rn_abs_cf_rep), 0, 0, NULL);
      }

   fprintf(s, "rn_abs_recip_cf_rep_formed:                    %12d\n", arg->rn_abs_recip_cf_rep_formed);

   if (arg->rn_abs_recip_cf_rep_formed)
      {
      GMP_RALG_cfdecomp_emit(s, "Abs Of Recip Of RN In CF Decomp", &(arg->rn_abs_recip_cf_rep), 0, 0, NULL);
      }

   fprintf(s, "corner_point_cf_rep_formed:                    %12d\n", arg->corner_point_cf_rep_formed);

   if (arg->corner_point_cf_rep_formed)
      {
      GMP_RALG_cfdecomp_emit(s, "Corner Point CF Decomp", &(arg->corner_point_cf_rep), 0, 0, NULL);
      }

   fprintf(s, "cor_pt_recip_cf_rep_formed:                    %12d\n", arg->corner_point_recip_cf_rep_formed);

   if (arg->corner_point_recip_cf_rep_formed)
      {
      GMP_RALG_cfdecomp_emit(s, "Corner Point Recip CF Decomp", &(arg->corner_point_recip_cf_rep), 0, 0, NULL);
      }

   fprintf(s, "                  equality:                    %12d\n", arg->equality);

   fprintf(s, "                n_left_out:                    %12d\n", arg->n_left_out);

   fprintf(s, "               n_right_out:                    %12d\n", arg->n_right_out);

   for (i=0; i < arg->n_left_out; i++)
      {
      sprintf(buf, "Contents Of Left Neighbor Array [%d]", i);
      FCMIOF_stream_bannerheading(s,
                                  buf, 
                                  0);
      fprintf(s, "                     index:                    %12d\n", arg->lefts[i].index);
      GMP_INTS_mpz_long_int_format_to_stream(s,
                                             &(arg->lefts[i].neighbor.num),
                                             "neighbor_num");
      GMP_INTS_mpz_long_int_format_to_stream(s,
                                             &(arg->lefts[i].neighbor.den),
                                             "neighbor_den");
      }

   for (i=0; i < arg->n_right_out; i++)
      {
      sprintf(buf, "Contents Of Right Neighbor Array [%d]", i);
      FCMIOF_stream_bannerheading(s,
                                  buf, 
                                  0);
      fprintf(s, "                     index:                    %12d\n", arg->rights[i].index);
      GMP_INTS_mpz_long_int_format_to_stream(s,
                                             &(arg->rights[i].neighbor.num),
                                             "neighbor_num");
      GMP_INTS_mpz_long_int_format_to_stream(s,
                                             &(arg->rights[i].neighbor.den),
                                             "neighbor_den");
      }

   FCMIOF_stream_hline(s);
   }


/******************************************************************/
/***  VERSION CONTROL REPORTING FUNCTIONS  ************************/
/******************************************************************/

//08/16/01: Visual inspection OK.
const char *GMP_RALG_cvcinfo(void)
   {
   return("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/gmp_ralg.c 71 2016-11-05 11:07:06Z dashley $");
   }


//08/16/01: Visual inspection OK.
const char *GMP_RALG_hvcinfo(void)
   {
   return(GMP_RALG_H_VERSION);
   }

//End of gmp_ralg.c.
