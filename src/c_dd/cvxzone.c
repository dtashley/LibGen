//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/cvxzone.c 71 2016-11-05 11:07:06Z dashley $
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
/* Convex zones for timed automata verification.
*/
//-------------------------------------------------------------------------------------------------
#define MODULE_CVXZONE

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "ccmalloc.h"
#include "cvxzone.h"


/**************************************************************************/
/***** ALLOCATION, DEALLOCATION, AND COPY FUNCTIONS
/**************************************************************************/
/* Returns the number of constraint slots in memory that should be 
** allocated as a function of those required.
*/
static int CVXZONE_cr_num_to_alloc(int crs_reqd_num)
   {
   int rv;

   assert(crs_reqd_num >= 0);

   rv = crs_reqd_num + CVXZONE_ZONE_CONST_ALLOC_INC - 1;

   rv = rv - (rv % CVXZONE_ZONE_CONST_ALLOC_INC);

   assert(rv >= crs_reqd_num);

   return(rv);
   }


void CVXZONE_new(CVXZONE_zone_ptr *arg)
   {
   /* Eyeball the inputs. */
   assert(arg  != NULL);
   assert(*arg == NULL);

   /* Allocate the space for the zone itself.
   */
   *arg = CCMALLOC_malloc(sizeof(CVXZONE_zone));

   /* Zero the space. */
   memset(*arg, 0, sizeof(CVXZONE_zone));

   /* Set any fields that take on non-zero initial values.
   */
   (*arg)->is_canonical = 1;
   }


void CVXZONE_delete(CVXZONE_zone_ptr *arg)
   {
   /* Check the input parameters. */
   assert(arg  != NULL);
   assert(*arg != NULL);

   /* Free any allocated constraint block.
   */
   if ((*arg)->constraints)
      CCMALLOC_free((*arg)->constraints);

   /* Free the record itself.
   */
   CCMALLOC_free(*arg);

   /* Set the caller's pointer to NULL.
   */
   *arg = NULL;
   }


void CVXZONE_copy(CVXZONE_zone_ptr *dst, CVXZONE_zone_ptr *src)
   {
   CVXZONE_constraint_ptr constraints;
   int n_allocd;

   /* Check the input parameters. */
   assert(dst  != NULL);
   assert(src  != NULL);
   assert(*dst != NULL);
   assert(*src != NULL);

   /* Save the victim's pointer to constraints and also the amount of
   ** space allocated.  These will be written over by the memory
   ** copy.
   */
   constraints = (*dst)->constraints;
   n_allocd    = (*dst)->n_allocd;

   /* Do a block copy from source to destination.  This catches
   ** everything except the constraints.
   */
   memcpy(*dst, *src, sizeof(CVXZONE_zone));

   /* Write the memory pointer and the amount allocd back.
   */
   (*dst)->constraints = constraints;
   (*dst)->n_allocd    = n_allocd;

   /* Reallocate the destination constraint block if there is not
   ** enough space to hold the constraints.
   */
   if ((*dst)->n_used > (*dst)->n_allocd)
      {
      if ((*dst)->constraints)
         {
         /* There was already a memory area involved, but not large
         ** enough.
         */
         CCMALLOC_realloc((*dst)->constraints, CVXZONE_cr_num_to_alloc((*dst)->n_used) * sizeof(CVXZONE_constraint));
         }
      else
         {
         /* No memory area had been allocated.  Need to allocate it for
         ** the first time.
         */
         CCMALLOC_malloc(CVXZONE_cr_num_to_alloc((*dst)->n_used) * sizeof(CVXZONE_constraint));
         }
      }

   /* Copy over the constraint block.
   */
   if ((*dst)->n_used)
      {
      memcpy((*dst)->constraints, (*src)->constraints, (*dst)->n_used * sizeof(CVXZONE_constraint));
      }
   }


void CVXZONE_clone(CVXZONE_zone_ptr *clone, CVXZONE_zone_ptr *orig)
   {
   /* Check the inputs. */
   assert(clone  != NULL);
   assert(orig   != NULL);
   assert(*clone == NULL);
   assert(*orig  != NULL);

   /* Allocate the clone.
   */
   CVXZONE_new(clone);

   /* Copy to the clone.
   */
   CVXZONE_copy(clone, orig);
   }


/**************************************************************************/
/***** MAINTENANCE FUNCTIONS                        
/**************************************************************************/
/* Removes (i.e. compresses out) all the unused constraints, i.e. those with
** the valid flag not set.  This simply makes the existing constraints contiguous.
** This function does this unconditionally, i.e. the is_canonical flag is not
** considered.
*/
static void CVXZONE_compress_constraints(CVXZONE_zone_ptr *arg)
   {
   int src_idx, dst_idx;

   assert(arg  != NULL);
   assert(*arg != NULL);

   dst_idx = 0;

   for (src_idx = 0; src_idx < (int)((*arg)->n_used); src_idx++)
      {
      if ((*arg)->constraints[src_idx].valid)
         {
         if (src_idx != dst_idx)  /* No sense copying something onto itself. */
            {
            memcpy((*arg)->constraints + dst_idx, 
                   (*arg)->constraints + src_idx,
                   sizeof(CVXZONE_constraint));
            }
         dst_idx++;
         }
      }

   /* The copy cycle is complete.  We can infer how many slots were *actually* used.
   */
   (*arg)->n_used = dst_idx;
   }


void CVXZONE_maintain_memory_trim(CVXZONE_zone_ptr *arg)
   {
   assert(arg  != NULL);
   assert(*arg != NULL);

   /* First, compact the array of constraints.
   */
   CVXZONE_compress_constraints(arg);

   /* Now, realloc the array to be the exact right size.
   */
   if ((*arg)->n_used)
      {
      assert((*arg)->constraints != NULL);

      CCMALLOC_realloc((*arg)->constraints,
                       (*arg)->n_used * sizeof(CVXZONE_constraint));
      }
   else
      {
      if ((*arg)->constraints)
         {
         CCMALLOC_free((*arg)->constraints);
         (*arg)->constraints = NULL;
         }
      }
   }


DECMOD_CVXZONE void CVXZONE_maintain_canonize(CVXZONE_zone_ptr *arg)
   {
   assert(arg  != NULL);
   assert(*arg != NULL);


   }

//End of cvxzone.c.
