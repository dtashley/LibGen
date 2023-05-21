//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/gmp_rats.h 71 2016-11-05 11:07:06Z dashley $
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
#ifndef GMP_RATS_H_INCLUDED
   #define GMP_RATS_H_INCLUDED

   #ifdef MODULE_GMP_RATS
      #define DECMOD_GMP_RATS
   #else
      #define DECMOD_GMP_RATS extern
   #endif

   //The data structure of a rational number.  Generally,
   //it isn't necessary to keep error flags for the rational
   //number--instead, we can just use the NAN flags of integers,
   //with the convention that if either of the integers
   //has any of the NAN flags set, then the rational number
   //is NAN.  This will be good enough.  So let's agree
   //by convention that a rational number is either
   //valid or NAN--no other possibilities, and nothing more
   //specific.
   //
   //Generally, none of the algorithms in this module make
   //assumptions about whether the inputs are normalized,
   //although they may make assurances about whether the
   //outputs are normalized.  Anything except a denominator
   //of zero is considered acceptable for inputs.
   //
   //For the record, "normalized" according to my definition is:
   //   a)Zero has the unique representation 0/1.
   //   b)Numerator and denominator are coprime.
   //   c)Positive rational numbers have positive
   //     components.
   //   d)Negative rational numbers have numerator
   //     negative and denominator positive.
   typedef struct 
      {
      GMP_INTS_mpz_struct num;
         //The numerator.
      GMP_INTS_mpz_struct den;
         //The denominator.
      } GMP_RATS_mpq_struct;

   /******************************************************************/
   /***  STATUS FUNCTIONS  *******************************************/
   /******************************************************************/
   //Functions in this category provide information about rational
   //numbers.
   //
   //Returns TRUE if the rational number is NAN, which means
   //either integer component with flags set or a denominator
   //of zero.
   DECMOD_GMP_RATS
   int GMP_RATS_mpq_is_nan(const GMP_RATS_mpq_struct *rn);

   /******************************************************************/
   /***  INITIALIZATION, CLEARING, AND SETTING FUNCTIONS  ************/
   /******************************************************************/
   //Allocates an space for integers, initializes a rational number
   //to 0/1.
   DECMOD_GMP_RATS
   void GMP_RATS_mpq_init(GMP_RATS_mpq_struct *arg);
   //Destroys a rational number.
   DECMOD_GMP_RATS
   void GMP_RATS_mpq_clear(GMP_RATS_mpq_struct *arg);
   //Sets a rational number to the two integer components
   //specified.  It is not required that these components
   //be normalized.
   DECMOD_GMP_RATS
   void GMP_RATS_mpq_set_si(GMP_RATS_mpq_struct *arg,
                            int num,
                            int den);
   DECMOD_GMP_RATS
   void GMP_RATS_mpq_copy(      GMP_RATS_mpq_struct *dst,
                          const GMP_RATS_mpq_struct *src);

   //Swaps two rational numbers.
   DECMOD_GMP_RATS
   void GMP_RATS_mpq_swap( GMP_RATS_mpq_struct *a,
                           GMP_RATS_mpq_struct *b);

   //Swaps numerator and denominator.  Simple swap, no normalization.
   DECMOD_GMP_RATS
   void GMP_RATS_mpq_swap_components(GMP_RATS_mpq_struct *arg);

   //Sets a rational number to be a slash-separated integer.  The 
   //two components (before and after the slash) may be integers
   //in scientific notation, integers with commas, etc.  The
   //*failure flag is set TRUE if the number could not be parsed.
   //The rational number involved must already have been initialized,
   //so space is allocated.  The number is not normalized in any
   //way--the integer components of the rational number are
   //exactly the slash-separated components.  It is possible to
   //use this function to simply parse to see if one can form
   //a rational number, but this is an expensive process because
   //of the allocation and then destruction involved.  It seems
   //possible in the future that one would introduce a function
   //which won't take the scientifc notation, and so the word 
   //"complex" is included below.  Any subsequent function which
   //won't accept the scientific notation should be called "simple"
   //instead.
   DECMOD_GMP_RATS
   void GMP_RATS_mpq_set_complex_slash_sepd_rat_num(const char *s,
                                                    int *failure,
                                                    GMP_RATS_mpq_struct *rn);

   //Attempts to parse and set a rational number expressed in
   //pure scientific notation.
   DECMOD_GMP_RATS
   void GMP_RATS_mpq_set_sci_not_rat_num(const char *s,
                                         int *failure,
                                         GMP_RATS_mpq_struct *rn);

   //Attempts to parse and set a rational number which might be in
   //any accepted format.
   DECMOD_GMP_RATS
   void GMP_RATS_mpq_set_all_format_rat_num(const char *s,
                                            int *failure,
                                            GMP_RATS_mpq_struct *rn);

   /******************************************************************/
   /***  NORMALIZATION FUNCTIONS  ************************************/
   /******************************************************************/
   //Normalizes only the signs.  Non-negative number expressed as +/+,
   //negative numbers expressed only as -/+.
   DECMOD_GMP_RATS
   void
   GMP_RATS_mpq_normalize_sign(GMP_RATS_mpq_struct *rn);

   
   DECMOD_GMP_RATS
   void GMP_RATS_mpq_normalize(GMP_RATS_mpq_struct *rn);

   /******************************************************************/
   /***  ARITHMETIC FUNCTIONS  ***************************************/
   /******************************************************************/
   //Adds two rational numbers to produce normalized result.
   //Args may be same as result.  Any NAN inputs result in NAN
   //output:  NAN flagged using 1/0 as return value.
   DECMOD_GMP_RATS
   void GMP_RATS_mpq_add(      GMP_RATS_mpq_struct *result,
                         const GMP_RATS_mpq_struct *arg1,
                         const GMP_RATS_mpq_struct *arg2);
   //Subtract, follows the same rules as add.
   DECMOD_GMP_RATS
   void GMP_RATS_mpq_sub(      GMP_RATS_mpq_struct *result,
                         const GMP_RATS_mpq_struct *arg1,
                         const GMP_RATS_mpq_struct *arg2);
   //Multiply.
   DECMOD_GMP_RATS
   void GMP_RATS_mpq_mul(      GMP_RATS_mpq_struct *result,
                         const GMP_RATS_mpq_struct *arg1,
                         const GMP_RATS_mpq_struct *arg2);
   //Divide.
   DECMOD_GMP_RATS
   void GMP_RATS_mpq_div(      GMP_RATS_mpq_struct *result,
                         const GMP_RATS_mpq_struct *arg1,
                         const GMP_RATS_mpq_struct *arg2);

   /******************************************************************/
   /***  COMPARISON FUNCTIONS  ***************************************/
   /******************************************************************/
   //Compares two rational numbers.  The return value is neg if
   //arg1 < arg2, 0 if arg1 == arg2, and 1 if arg1 > arg2.  The
   //rational numbers passed are not required to be normalized,
   //either with respect to sign of components or with respect 
   //to coprimality.  There are some errors which may occur, either
   //because a denominator is zero or because intermediate results
   //overflow as the comparison is made.  For that reason, the
   //"failure" pointer is allowed.  If it is NULL, no failure information
   //is returned and any failure condition results in a return value
   //of zero and the failure can't be detected by the caller.
   //If the pointer is not NULL, the failure boolean will be
   //filled in so the caller can detect it.
   int GMP_RATS_mpq_cmp(const  GMP_RATS_mpq_struct *arg1,
                        const  GMP_RATS_mpq_struct *arg2,
                        int   *failure);

   /******************************************************************/
   /***  VERSION CONTROL REPORTING FUNCTIONS  ************************/
   /******************************************************************/
   DECMOD_GMP_RATS const char *GMP_RATS_cvcinfo(void);
   DECMOD_GMP_RATS const char *GMP_RATS_hvcinfo(void);
   #define GMP_RATS_H_VERSION ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/gmp_rats.h 71 2016-11-05 11:07:06Z dashley $")
#endif

//End of gmp_rats.h.
