//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/gmp_ralg.h 71 2016-11-05 11:07:06Z dashley $
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
/* GNU multiple precision rational number algorithms 
** (and some extras).
*/

#ifndef GMP_RALG_H_INCLUDED
   #define GMP_RALG_H_INCLUDED

   #ifdef MODULE_GMP_RALG
      #define DECMOD_GMP_RALG
   #else
      #define DECMOD_GMP_RALG extern
   #endif

   typedef struct 
      {
      int n;
         //The number of continued fraction partial quotients
         //and convergents included in this structure.  Indices
         //are only valid until one less than this value.
      int nallocd;
         //The number of slots allocated for partial quotients,
         //convergents, etc.  This goes in blocks of the size
         //below to avoid numerous reallocation calls.
         #define  GMP_RALG_CF_ALLOC_INCREMENT  (10)
      GMP_INTS_mpz_struct num;
         //The numerator of the rational number whose continued
         //fraction decomposition is contained in this structure.
         //It must be non-negative.  The fraction is not
         //required to be reduced.
      GMP_INTS_mpz_struct den;
         //The denominator.  It must be positive.
      GMP_INTS_mpz_struct *a;
         //Pointer to array of arbitrary integers.  Each
         //is one of the partial quotients.  This array
         //is allocated out to nallocd items but only n 
         //are used.
      GMP_INTS_mpz_struct *dividend;
         //The dividend of each division round.
      GMP_INTS_mpz_struct *divisor;
         //The divisor at each round.
      GMP_INTS_mpz_struct *remainder;
         //The remainder of each round.
      GMP_INTS_mpz_struct *p;
      GMP_INTS_mpz_struct *q;
         //The convergents.
      } GMP_RALG_cf_app_struct;


   //This structure type holds a single neighbor to a rational number we wish
   //to approximate in either the Farey series of a certain order or 
   //in a rectangular region of the integer lattice.  These neighbors
   //are manipulated in potentially large arrays.
   typedef struct {
      int index;
         //The relative index of the neighbor.  0 means that it is
         //the reduced and normalized form of the number to be
         //approximated (although at the present time the number to be
         //approximated is never included among the neighbors), 
         //"1" means nearest neighbor to the right,
         //"2" means second nearest neighbor to the right, "-1" means
         //neighbor to the left, etc.
      GMP_RATS_mpq_struct neighbor;
         //The neighbor itself, fully reduced and normalized.
      } GMP_RALG_fab_neighbor_struct;

   //This structure type holds a collection of neighbors which were
   //produced on demand to a rational number, either in a Farey series
   //or in a rectangular region of the integer lattice.  The collection
   //contains the neighbors plus a great deal of other information.
   typedef struct {
      GMP_RATS_mpq_struct rn_in;
         //The rational number which was supplied to have its neighbors
         //found.  This number is raw as supplied by the caller and 
         //not necessarily normalized.
      GMP_INTS_mpz_struct kmax_in;
         //The value of KMAX supplied.
      int hmax_supplied;
         //TRUE if the problem includes a bound on the numerator, i.e.
         //rectangular region of integer lattice rather than Farey series.
      GMP_INTS_mpz_struct hmax_in;
         //The value of HMAX if it was supplied, or zero otherwise.
      char *error;
         //NULL if no error occured in finding the neighbors.  If non-NULL,
         //points to a descriptive string explaining the error.  
         //If this error flag is set, contents of this structure
         //below this point should not be used--they are not 
         //necessarily valid.
      GMP_RATS_mpq_struct corner_point;
         //The normalized (reduced) version of the "corner point"
         //(HMAX/KMAX).  This will only be filled in if
         //HMAX is specified, or it will be 0/1 otherwise.
      GMP_RATS_mpq_struct corner_point_minus_one;
      GMP_RATS_mpq_struct corner_point_plus_one;
         //The points just before and just above the corner point
         //in the rectangular region of interest.      
      GMP_RATS_mpq_struct hmax_over_one;
         //If HMAX is supplied, the rational number which is
         //HMAX/1.  Used for comparisons if we are asked
         //to approximate a number out of range.
      GMP_RATS_mpq_struct norm_rn;
         //The normalized form of the rational number supplied.
      GMP_RATS_mpq_struct abs_norm_rn;
         //The absolute value of the normalized form of the rational 
         //number supplied.
      int rn_is_neg;
         //Set TRUE if the rational number supplied was negative.  This is 
         //not a problem, because symmetry is exploited.  For the purposes
         //of this structure, the Farey series extends on both sides of zero
         //forever, and a rectangular region of the integer lattice extends
         //from -HMAX/1 to HMAX/1.
      int n_left_in;
      int n_right_in;
         //The number of left neighbors to the rational number supplied
         //requested, and the number of right neighbors.  Note that if
         //one of the bounds of the series of interest is encountered,
         //the actual number supplied may be less.
      GMP_RALG_cf_app_struct rn_abs_cf_rep;
         //The CF representation of the absolute value of the
         //rational number supplied.  This always has to be
         //formed, because it is the only way to immediately
         //get neighbors.
      int rn_abs_cf_rep_formed;
         //Boolean flag to indicate if above has been formed.
         //This is necessary because otherwise don't have
         //any obvious clues about whether to free this.
      GMP_RALG_cf_app_struct rn_abs_recip_cf_rep;
         //The CF representation of the reciprocal of the 
         //absolute value of the rational number supplied.
         //If HMAX is supplied, this may need to be formed,
         //because it will allow us to zero in on neighbors
         //where HMAX is the dominant constraint.
      int rn_abs_recip_cf_rep_formed;
         //Boolean flag to indicate if above has been formed.
         //This is necessary because otherwise don't have
         //any obvious clues about whether to free this.
      GMP_RALG_cf_app_struct corner_point_cf_rep;
         //The CF representation of the corner point.  This is
         //necessary to find the predecessor to the corner point 
         //in F_KMAX.  This will only be calculated if HMAX 
         //is specified.
      int corner_point_cf_rep_formed;
         //Boolean flag to indicate if above has been formed.
         //This is necessary because otherwise don't have
         //any obvious clues about whether to free this.
      GMP_RALG_cf_app_struct corner_point_recip_cf_rep;
         //The CF representation of the reciprocal of the
         //corner point.  This is necessary to find the 
         //successor to the corner point along the HMAX constaint
         //in the rectangular region of the integer lattice.
         //This will only be calculated if HMAX is specified.
      int corner_point_recip_cf_rep_formed;
         //Boolean flag to indicate if above has been formed.
         //This is necessary because otherwise don't have
         //any obvious clues about whether to free this.
      int equality;
         //TRUE if the number supplied was already in the series of interest,
         //or FALSE otherwise.      
      int n_left_out;
      int n_right_out;
         //The actual number of neighbors supplied.  These may be less
         //than the number requested only if a boundary of the series of interest
         //was encountered.
      GMP_RALG_fab_neighbor_struct *lefts;
      GMP_RALG_fab_neighbor_struct *rights;
         //Arrays of neighbors in the series of interest.  These are
         //only stuffed as indicated by "n_left_out" and "n_right_out"--anything
         //else will hit on dynamic memory that is not initialized.  The
         //left array begins -1 and proceeds leftward through increasingly 
         //negative indices.  The right begins with 1 and 
         //proceeds through increasingly positive indices.  Each neighbor is 
         //irreducible and normalized.  The rational number to be approximated
         //is NEVER included among the neighbors.
      } GMP_RALG_fab_neighbor_collection_struct;

   /******************************************************************/
   /***  INITIALIZATION AND DESTRUCTION FUNCTIONS  *******************/
   /******************************************************************/
   //Decomposes a rational number into its partial quotients
   //and convergents.  This initializes the "decomp" structure
   //by allocating space, etc.  The numerator must be non-negative
   //and the denominator must be positive, or else there will be
   //a failure.  To make the interface simpler, the destruction
   //call must ALWAYS be made, even in the event of a failure.
   //If a failure occurs, the rational number will be decomposed
   //as if it were 0/1.  This means that if the failure flag
   //is missed, it will still seem to be a valid rational number.
   //It is not required that the rational number be reduced.
   //The two integers passed are not changed at all, but they
   //are copied into the decomp structure without modification.
   //After this operation, the destruction call must be made.
   DECMOD_GMP_RALG void GMP_RALG_cfdecomp_init(
            GMP_RALG_cf_app_struct *decomp,
            int                    *failure,
            GMP_INTS_mpz_struct    *num,
            GMP_INTS_mpz_struct    *den);
   //Destroys all the the data allocated.
   DECMOD_GMP_RALG void GMP_RALG_cfdecomp_destroy(
            GMP_RALG_cf_app_struct *decomp
            );

   /******************************************************************/
   /***  INITIALIZATION AND DESTRUCTION FUNCTIONS  *******************/
   /******************************************************************/
   //Prints out a continued fraction decomposition.  The banner parameter
   //may be NULL, which indicates no banner.  nf flags whether to
   //print with "no format", i.e. unformatted strings, separated by
   //newlines.  dap indicates whether to include decimal approximation
   //information.  The dap_denominator only needs to be non-NULL if
   //"dap" is selected.
   DECMOD_GMP_RALG
   void GMP_RALG_cfdecomp_emit(
                               FILE                   *s,
                               char                   *banner,
                               GMP_RALG_cf_app_struct *decomp,
                               int                    nf,
                               int                    dap,
                               const GMP_INTS_mpz_struct    *dap_denominator);

   /******************************************************************/
   /***  FAREY SERIES PREDECESSOR AND SUCCESSOR FUNCTIONS  ***********/
   /******************************************************************/
   //The two functions below compute Farey predecessors and 
   //successors.  These are straight sequences of arithmetic
   //operations--no checking for overflow is done.  The behavior
   //below zero will probably not be correct, so these should
   //not be used to go below 0/1 or with operands below 0/1.
   DECMOD_GMP_RALG
   void GMP_RALG_farey_predecessor(
                       GMP_RATS_mpq_struct *result,
                 const GMP_RATS_mpq_struct *plus_two,
                 const GMP_RATS_mpq_struct *plus_one,
                 const GMP_INTS_mpz_struct *N);
   DECMOD_GMP_RALG
   void GMP_RALG_farey_successor(
                       GMP_RATS_mpq_struct *result,
                 const GMP_RATS_mpq_struct *minus_two,
                 const GMP_RATS_mpq_struct *minus_one,
                 const GMP_INTS_mpz_struct *N);
   
   //Finds the enclosing Farey neighbors to an arbitrary positive,
   //zero, or negative rational number which may or may not be in
   //the Farey series of order N.
   //
   //INPUTS
   //          rn: Rational number whose neighbors to find.  MUST
   //              be normalized, meaning reduced and sign adjusted
   //              so numerator contains the sign.
   //           N: Order of series.  MUST be positive.
   //      cf_rep: The continued fraction representation of the
   //              non-negative version of the rational number 
   //              whose neighbors to find.  The rational number
   //              need not have been reduced when the continued
   //              fraction representation was formed, i.e. only
   //              partial quotients and convergents are used.
   //OUTPUTS
   //    equality: Set TRUE if the rational number is in the Farey
   //              series, or FALSE otherwise.  This affects the
   //              generation of neighbors.  If the number is
   //              already in the Farey series its other neighbor
   //              is generated, i.e. the rational number of
   //              interest is never one of the neighbors returned.
   //        left: The left neighbor.
   //       right: The right neighbor.
   DECMOD_GMP_RALG
   void GMP_RALG_enclosing_farey_neighbors(
                 const GMP_RATS_mpq_struct    *rn,
                 const GMP_INTS_mpz_struct    *N,
                 const GMP_RALG_cf_app_struct *cf_rep,
                 int                          *equality,
                       GMP_RATS_mpq_struct    *left,
                       GMP_RATS_mpq_struct    *right);

   //Provides up to 2**24 - 1 (in concept--we don't recommend this!) terms in either
   //the Farey series or in a rectangular region of the integer lattice.  The caller
   //must provide the space to be used, and must free it afterwards--this function 
   //performs no dynamic allocation.
   DECMOD_GMP_RALG
   void GMP_RALG_consecutive_fab_terms(
      const GMP_RATS_mpq_struct *rn_in,
         //The rational number whose neighbors are to be found.
      const GMP_INTS_mpz_struct *kmax,
         //Maximum denominator.
      const GMP_INTS_mpz_struct *hmax,
         //Maximum numerator, or a NULL pointer or a non-null pointer but
         //an arbitrary integer value of 0 if only the Farey series is being
         //considered.
      int n_left_in,
         //The number of left neighbors requested.  Must be non-negative.
      int n_right_in,
         //The number of right neighbors requested.  Must be non-negative.
      GMP_RALG_fab_neighbor_collection_struct *result
         //The structure containing all results.  Even if there is an error
         //condition, this must be freed when done by the caller.
      );

   //Frees the dynamic memory consumed by an earlier call to generate the
   //neighbor information.
   DECMOD_GMP_RALG
   void GMP_RALG_consecutive_fab_terms_result_free(
      GMP_RALG_fab_neighbor_collection_struct *arg
         //The structure whose members to free up.  After this call,
         //the structure must be considered invalid.
      );

   //Debugging function.  Dumps the contents of the neighbor structure to the indicated
   //stream for analysis.
   DECMOD_GMP_RALG
   void GMP_RALG_consecutive_fab_terms_result_dump(
         FILE *s,
         GMP_RALG_fab_neighbor_collection_struct *arg);

   /******************************************************************/
   /***  VERSION CONTROL REPORTING FUNCTIONS  ************************/
   /******************************************************************/
   DECMOD_GMP_RALG const char *GMP_RALG_cvcinfo(void);
   DECMOD_GMP_RALG const char *GMP_RALG_hvcinfo(void);
   #define GMP_RALG_H_VERSION ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/gmp_ralg.h 71 2016-11-05 11:07:06Z dashley $")
#endif

//End of gmp_ralg.h.
