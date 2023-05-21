//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/cvxzone.h 71 2016-11-05 11:07:06Z dashley $
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
#ifndef CVXZONE_H_INCLUDED
   #define CVXZONE_H_INCLUDED

   #ifdef MODULE_CVXZONE
      #define DECMOD_CVXZONE
   #else
      #define DECMOD_CVXZONE extern
   #endif

   #define CVXZONE_ZONE_CONST_ALLOC_INC       (20)
      /* The allocation increment for constraints which make up
      ** a convex zone.  This is the number of slots that will
      ** be allocated at a time.  A larger value here creates more
      ** run-time efficiency but wastes more memory.
      */

   /* Types of equalities and inequalities.
   */
   #define CVXZONE_INEQ_LT                 (0)  /* <  */
   #define CVXZONE_INEQ_LE                 (1)  /* <= */
   #define CVXZONE_INEQ_EQ                 (2)  /* == */
   #define CVXZONE_INEQ_GE                 (3)  /* >  */
   #define CVXZONE_INEQ_GT                 (4)  /* >= */
   #define CVXZONE_INEQ_UDF                (5)  /* Unknown or undefined. */
   #define CVXZONE_INEQ_MAX                (6)  /* Max for bounds checking. */


   /* Defines a single constraint, i.e. x[a]-x[b] <|<= K as known
   ** to this module.  x[0] is always the value of zero.  Any given
   ** convex region or zone is a conjunction of such inequalities.
   */
   typedef struct
      {
      unsigned valid   :   1;
         /* TRUE if record is valid, i.e. used.
         */
      unsigned eq      :   1;
         /* TRUE if the inequality is <= rather than
         ** <.
         */
      int v1; 
      int v2;
         /* Subscripts of variables in v1 - v2 <|<= k.
         */
      int k;
         /* The constant involved.
         */
      } CVXZONE_constraint, *CVXZONE_constraint_ptr;

   /* Fundamental data structure that is a convex zone bounded
   ** by inequalities.  All of the clock variables are represented
   ** as integers.  The variable with index 0 (i.e. x[0]) is the
   ** special value 0.  Therefore, all clocks must be subscripted
   ** starting with the value "1" and go contiguously upwards, and
   ** this inconvenience is visible to the client, i.e. the client
   ** must communicate with this module in these terms.
   **
   ** Note that there is some ambiguity in this data structure 
   ** because the order of the space which is being considered
   ** is not specified anywhere (because there is no need to 
   ** specify it).  This module will fatally terminate
   ** the program in any context where a constraint exists for a variable
   ** that can't exist (so far there is only one such place this can
   ** occur).  The caller needs to be consistent in the order of the
   ** space (i.e. the caller needs to know the assumed order of the
   ** space).
   */
   typedef struct
      {
      unsigned is_canonical : 1;
         /* TRUE if the data structure is in canonical form.  This
         ** flag is maintained so that repeated needs to put in
         ** canonical form will not take any CPU time.  Essentially
         ** any operation that modifies this data structure will
         ** violate the canonical form and cause this flag to be
         ** set to 0.
         */
      unsigned is_empty     : 1;
         /* By default, a space with no constraints is taken to be
         ** full, i.e. to contain all of R-space.  This flag negates
         ** that, i.e. signals the empty set.  The canonical form
         ** of all-space is no constraints and this flag FALSE.  The
         ** canonical form of no-space is no constraints and this
         ** flag TRUE.
         */
      unsigned n_allocd;
         /* The number of slots allocated for constraints in this
         ** data structure.  This value grows, but never shrinks.
         ** This is the number allocated, but not necessarily the
         ** number used.  This value grows in steps of 
         ** CVXZONE_ZONE_CONST_ALLOC_INC.  If this value is zero,
         ** the pointer below must be NULL.
         */
      unsigned n_used;
         /* The number of slots which are used.  This means that the
         ** used slots range from 0 through this value - 1.
         ** This only flags the constraints which must be inspected
         ** and potentially apply, but not necessarily every constraint
         ** in this range applies.  In addition to the constraint being
         ** subscripted in 0..n-1, the constraint must also have its
         ** valid bit set.  The set of active constraints is thus
         ** those in the range of 0..n-1 with their valid bits set.
         */
      CVXZONE_constraint_ptr constraints;
         /* Pointer to the allocated block of constraints.  The number
         ** allocated is realloc'd up by CVXZONE_ZONE_CONST_ALLOC_INC
         ** slots each time one runs out of memory.
         */
      } CVXZONE_zone, *CVXZONE_zone_ptr;

   /**************************************************************************/
   /***** ALLOCATION, DEALLOCATION, AND COPY FUNCTIONS
   /**************************************************************************/
   /* Allocates a new zone and fills in the caller's pointer.  The
   ** pointed-to pointer must be NULL.  Zones begin life as "full"--containing
   ** all of R-space.
   */
   DECMOD_CVXZONE void CVXZONE_new(CVXZONE_zone_ptr *arg);

   /* Deallocates a zone and fills in the caller's pointer to NULL.  Previous
   ** pointer to zone must not be NULL.
   */
   DECMOD_CVXZONE void CVXZONE_delete(CVXZONE_zone_ptr *arg);

   /* Copies one zone to another.  The destination zone, which must
   ** already exist, is overwritten.
   */
   DECMOD_CVXZONE void CVXZONE_copy(CVXZONE_zone_ptr *dst, CVXZONE_zone_ptr *src);

   /* Clones a zone.  To clone means to create and copy in one step.
   */
   DECMOD_CVXZONE void CVXZONE_clone(CVXZONE_zone_ptr *clone, CVXZONE_zone_ptr *orig);

   /**************************************************************************/
   /***** MAINTENANCE FUNCTIONS                        
   /**************************************************************************/
   /* Trims extra memory from a zone (but won't convert it to canonical form).
   ** The default behavior for a zone is that memory is never reclaimed (this is
   ** probably the desired behavior--reclaiming memory costs time with no real
   ** benefit).  Note that trimming memory goes beyond canonical form ... it is
   ** not part of making something canonical.
   */
   DECMOD_CVXZONE void CVXZONE_maintain_memory_trim(CVXZONE_zone_ptr *arg);

   /* Converts a zone to canonical form.  Normally this type of operation is
   ** never necessary explicitly, because any function call that needs a zone 
   ** to be in canonical form will ensure this first.  This function call
   ** might never be used except for testing.
   */
   DECMOD_CVXZONE void CVXZONE_maintain_canonize(CVXZONE_zone_ptr *arg);


   /**************************************************************************/
   /***** ASSIGNMENT FUNCTIONS                         
   /**************************************************************************/
   /* Assigns a zone, which must already exist, to be the empty set.
   */
   DECMOD_CVXZONE void CVXZONE_assign_empty(CVXZONE_zone_ptr *arg);

   /* Assigns a zone, which must already exist, to be the full set (all of R^N).
   */
   DECMOD_CVXZONE void CVXZONE_assign_full(CVXZONE_zone_ptr *arg);


   /**************************************************************************/
   /***** TEST FUNCTIONS
   /**************************************************************************/
   /* Set testing functions.  In general these functions may modify even 
   ** operands that are not identified as outputs, because it may be necessary
   ** to convert sets to canonical form before operations can be performed.
   ** However, the sets will not be modified so as to change their logical
   ** value.
   */
   /* Tests whether a zone is empty.  Returns TRUE if empty.
   */
   DECMOD_CVXZONE int CVXZONE_test_empty(CVXZONE_zone_ptr *arg);

   /* Tests whether a zone is full.  Returns TRUE if full.  "Full" means covers all of
   ** N-dimensional space.
   */
   DECMOD_CVXZONE int CVXZONE_test_full(CVXZONE_zone_ptr *arg);

   /* Tests whether one set is equal to another.  Returns TRUE if equal.
   */
   DECMOD_CVXZONE int CVXZONE_test_proper_equal(CVXZONE_zone_ptr *set1,
                                                CVXZONE_zone_ptr *set2);

   /* Tests whether a point belongs to a set.  The point is specified as
   ** an integer array, with n entries, giving the successive coordinates.
   ** Element [0] is ignored, and element [1] is the value of x_1, element
   ** [2] is the value of x_2, etc.  Specifying too many coordinates will
   ** not be detected, as there will be simply no constraints on those
   ** coordinates and so the extra coordinates will effectively being ignored.
   ** Specifying too few coordinates may or may not be detected.  If a 
   ** constraint is detected which cannot be applied, this will fatally
   ** terminate the program.
   */
   DECMOD_CVXZONE int CVXZONE_test_point_membership(CVXZONE_zone_ptr *base_set,
                                                    int n_coords,
                                                    int *coords);

   /* Tests whether one set is a proper subset of another.  Returns TRUE if
   ** it is.
   */
   DECMOD_CVXZONE int CVXZONE_test_proper_subset(CVXZONE_zone_ptr *base_set,
                                                 CVXZONE_zone_ptr *potential_subset);

   /* Tests whether one set is an improper subset of another.  Returns TRUE if
   ** it is.  An improper subset is just like a subset, but equality is allowed.
   */
   DECMOD_CVXZONE int CVXZONE_test_improper_subset(CVXZONE_zone_ptr *base_set,
                                                   CVXZONE_zone_ptr *potential_subset);

   /**************************************************************************/
   /***** MODIFICATION FUNCTIONS
   /**************************************************************************/
   /* Adds a constraint to a set.  The constraint is of the form 
   ** arg1 - arg2 <|<= k.  A variable with index 0 is a special case,
   ** and this is treated as zero (i.e. it is a fictional variable).
   ** The "eq" flag makes the difference between "<" and "<=".  If "eq" is
   ** TRUE, then "<=" is assumed.
   **
   ** The set is the intersection of all the half-[hyper]regions formed by the
   ** constraints.  Note that forming a set by repeatedly adding constraints
   ** only works if set used to start is the "full" set.  Adding any constraints
   ** to an empty set will only result in an empty set that will be cleaned up the
   ** next time it is canonized.
   */
   DECMOD_CVXZONE void CVXZONE_modify_add_constraint(CVXZONE_zone_ptr *base_set,
                                                     int arg1_idx,
                                                     int arg2_idx,
                                                     int eq,
                                                     int k);


   /**************************************************************************/
   /***** CALCULATION FUNCTIONS
   /**************************************************************************/
   /* Calculates the intersection of two sets and assigns it to a third set, which
   ** must exist.  (BTW, note that union cannot be calculated in this framework,
   ** because it might result in a set which is not convex.  That is why no unioning
   ** function is provided here.)
   */
   DECMOD_CVXZONE void CVXZONE_calculate_intersection(CVXZONE_zone_ptr *dst,
                                                      CVXZONE_zone_ptr *src1,
                                                      CVXZONE_zone_ptr *src2);

#endif

//End of ccvxzone.h.
