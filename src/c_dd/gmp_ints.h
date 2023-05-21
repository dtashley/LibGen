//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/gmp_ints.h 71 2016-11-05 11:07:06Z dashley $
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
/*
This software module is a careful adaptation of the integer functions
in the GNU MP library.

By "adaptation" I mean the following:

   o Many of the macros and other constructs that affect readability
     have been removed.  This has been done for simplicity.

   o Only the needed functions (large integer and rational number 
     functions) have been included.  This is for use in Dave Ashley's
     set of tools.

   o The number of source modules has been dramatically reduced.

   o Assembly-language has been removed.  Instead, at the lowest
     level, the core is generic 'C' provided by GNU (this is
     selected if processor=none is the processor chosen).  This
     certainly has an effect on speed, but I would rather sacrifice
     quite a bit of speed and keep it simple (in one module).

   o The GNU MP library has been specifically ported for Microsoft
     Visual C++ 6.0.  In other words, its portability has been
     deliberately destroyed.  Do NOT try to use this on another
     platform (or at least certainly do NOT contact me if you have
     problems with this).

   o Certain other stylistic changes have been made.
*/
#ifndef GMP_INTS_H_INCLUDED
   #define GMP_INTS_H_INCLUDED

   #ifdef MODULE_GMP_INTS
      #define DECMOD_GMP_INTS
   #else
      #define DECMOD_GMP_INTS extern
   #endif

   typedef unsigned long int GMP_INTS_limb_t;
      //The fundamental data type for representing integers in binary
      //form.  For MSVC++, this is a 32-bit type.  This data type
      //is not normally needed publicly.
   #define GMP_INTS_BITS_PER_LIMB  (32)
      //This is the number of bits that each limb is assumed to hold.
      //There is an explicit function in the interface to check this,
      //in case anything changes with future versions of MSVC++.
      //This software module is NOT meant to be portable, or even
      //particularly quick.  It is meant to be verifiable and
      //testable instead.
   typedef long int  GMP_INTS_limb_signed_t;
      //Signed version of above.
   typedef GMP_INTS_limb_t *  GMP_INTS_limb_ptr;
      //Pointer to a limb (or, more characteristically, to an array
      //of limbs).
   typedef const GMP_INTS_limb_t * GMP_INTS_limb_srcptr;
      //Constant pointer used as the source pointer for copies
      //and other operations.
   typedef long int  GMP_INTS_size_t;
      //Used for representing the size of things to this module
   typedef long int  GMP_INTS_exp_t;
      //Used for representing exponent.  Assumed to be large enough
      //for this.

   //Below is fundamental structure which holds an arbitrary-size
   //integer.
   typedef struct
      {
      int flags;
                       //Bit-flags as to the condition of the
                       //integer.  The bitflags defined so far
                       //are designed to be mutually exclusive,
                       //and are listed below.  At the same time,
                       //we need to define the "official" strings
                       //for these bit flags.
                       #define GMP_INTS_EF_INTOVF_POS        (1)
                       #define GMP_INTS_EF_INTOVF_POS_STRING   \
                               "GMP_INTS_EF_INTOVF_POS"
                          //The integer has become too large
                          //growing in a positive direction.
                          //No further arithmetic will be 
                          //performed on it.
                       #define GMP_INTS_EF_INTOVF_NEG        (2)
                       #define GMP_INTS_EF_INTOVF_NEG_STRING   \
                               "GMP_INTS_EF_INTOVF_NEG"
                          //The integer has become too large
                          //growing in a negative direction.
                          //No further arithmetic will be 
                          //performed on it.
                       #define GMP_INTS_EF_INTOVF_TAINT_POS  (4)
                       #define GMP_INTS_EF_INTOVF_TAINT_POS_STRING   \
                               "GMP_INTS_EF_INTOVF_TAINT_POS"
                          //The integer has been arithmetically
                          //combined with an integer that has
                          //overflowed in the positive direction.
                          //This integer is thus considered tainted
                          //and no further arithmetic will be 
                          //performed on it.
                       #define GMP_INTS_EF_INTOVF_TAINT_NEG  (8)
                       #define GMP_INTS_EF_INTOVF_TAINT_NEG_STRING   \
                               "GMP_INTS_EF_INTOVF_TAINT_NEG"
                          //The integer has been arithmetically
                          //combined with an integer that has
                          //overflowed in the negative direction.
                          //This integer is thus considered tainted
                          //and no further arithmetic will be 
                          //performed on it.
                       //The flags above will prevent arithmetic
                       //on an integer and will taint any integer
                       //that comes into contact with it as the
                       //result of an operation.  These flags are
                       //cleared only by an assignment operation
                       //of any kind.
                       //
                       //The next logical questions are:
                       //   o Why do we want to make a size
                       //     limit on integers, anyway?
                       //   o How big is too big?
                       //
                       //We want a limit because this software module
                       //will be used in programs that should be
                       //reliable.  Allowing arbitrarily large results
                       //would invite users to break the software.
                       //It would also make testing impossible, because
                       //the module could not be tested out to its
                       //limits.  Assigning limits means that testing
                       //can be performed to the limits.  Also, there
                       //are certain constructs in the software that
                       //may break at high limits, such as _alloca().
                       //
                       //The question of how large is too large is a
                       //harder question.  Arbitrarily, let's decide
                       //that we want the equivalent of integers with
                       //100,000 decimal digits, which is about
                       //332,224 bits, or 10,382 limbs.  (By the way
                       //this limit can always be raised later, but
                       //the important point is that it exists and
                       //is tested to).  Any integer with more limbs
                       //than this is considered illegal.
                       #define GMP_INTS_MAXIMUM_LIMBS_PER_INT  (10382)
      int n_allocd;   /* Number of limbs allocated and pointed
                         to by the limbs field.  This gives the
                         number allocated (i.e. physically
                         allocated), but the next field tells how 
                         many are used.  Memory is never shrunk
                         automatically by this software module
                         (per the wise GNU MP design), so this
                         number does not ever automatically 
                         decrease.  */
      int size;       /* abs(size) is the number of limbs the
                         last field points to.  If _mp_size is
                         negative this is a negative number.  This
                         will be zero if the integer is zero. */
      GMP_INTS_limb_t *limbs;  
                      /* Pointer to the limbs.  The first field
                         of the structure (above) tells how many
                         spots are allocated, and the second
                         field (above) tells how many are used.
                         Numbers are stored least-significant
                         limbs first.  */
      } GMP_INTS_mpz_struct;


   /* Wrappers for non-stack memory allocation function to be used
   ** by integer, rational number, and integer and rational number
   ** algorithm functions.  These must be public because they are
   ** used by the other modules that deal with integers and rational
   ** numbers.
   */
   DECMOD_GMP_INTS void *GMP_INTS_malloc( size_t size );
   DECMOD_GMP_INTS void *GMP_INTS_calloc( size_t num, size_t size );
   DECMOD_GMP_INTS void *GMP_INTS_realloc( void *memblock, size_t size );
   DECMOD_GMP_INTS void *GMP_INTS_realloc_w_size( void *memblock, 
                                                  size_t old_size,
                                                  size_t size );
   DECMOD_GMP_INTS void GMP_INTS_free( void *memblock );
   DECMOD_GMP_INTS void GMP_INTS_free_w_size( void *memblock, size_t size );

   /******************************************************************/
   /***  PORTABILITY CHECK FUNCTIONS   *******************************/
   /******************************************************************/
   //Because there is the risk that Microsoft Visual C++ might 
   //change in the future, the following function can be called
   //to see if the assumptions about data sizes are valid.  This
   //function returns TRUE if there is a problem, or FALSE
   //otherwise.
   DECMOD_GMP_INTS int GMP_INTS_data_sizes_are_wrong(void);


   /******************************************************************/
   /***  ERROR STRING IDENTIFICATION AND PROCESSING FUNCTIONS  *******/
   /******************************************************************/
   //These functions are provided because some clients may deal
   //only with symbolic representations (Tcl/Tk, for example).
   //
   //Attempts to identify the passed string as one of the error
   //strings sanctioned by this module.  Returns -1 if not a match
   //or else the following values.
   //  0:  GMP_INTS_EF_INTOVF_POS
   //  1:  GMP_INTS_EF_INTOVF_NEG
   //  2:  GMP_INTS_EF_INTOVF_TAINT_POS
   //  3:  GMP_INTS_EF_INTOVF_TAINT_NEG
   DECMOD_GMP_INTS 
   int GMP_INTS_identify_nan_string(const char *s);

   //Returns the sanctioned string for the nan value indicated.
   //Integers that may be passed are the same as the 
   //indices above.  No parameter except 0-3 is allowed.
   DECMOD_GMP_INTS 
   const char *GMP_INTS_supply_nan_string(int idx);

   /******************************************************************/
   /***  DEBUG PRINTING FUNCTIONS   **********************************/
   /******************************************************************/
   //These functions are for printing out integers and limbs
   //and groups of limbs for unit testing and debugging.
   //
   //Prints out a group of limbs, preceded by a description.
   //n may be 0 and/or the lg pointer supplied may be NULL.
   DECMOD_GMP_INTS
   void GMP_INTS_print_limb_group(FILE *stream,
                                  GMP_INTS_limb_srcptr lg,
                                  GMP_INTS_size_t n,
                                  char *desc);
   //Prints an entire integer for diagnosis.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_print_int(FILE *stream,
                               const GMP_INTS_mpz_struct *arg,
                               char *desc);

   /****************************************************************/
   /***   MACRO REPLACEMENTS   *************************************/
   /****************************************************************/
   //The functions here are macros that have been replaced and
   //made functions for clarity.  Speed was sacrificed for clarity.
   DECMOD_GMP_INTS
   int GMP_INTS_mpz_get_flags(const GMP_INTS_mpz_struct *arg);
      //Allows a caller to obtain the flags of an integer.  
      //A non-zero value indicates trouble.  To figure out which
      //trouble, compare against the bit constants defined
      //with the "flags" field of the structure type, above.
   DECMOD_GMP_INTS GMP_INTS_size_t GMP_INTS_abs_of_size_t(GMP_INTS_size_t arg);
      //Returns the absolute value of one of the size arguments used
      //to indicate number of limbs.  This function is useful 
      //because integers natively stored with a negative size
      //for negative arguments, so the absolute value gives the
      //number of limbs consumed.
   DECMOD_GMP_INTS int GMP_INTS_mpz_sgn(const GMP_INTS_mpz_struct *arg);
      //Returns -1 if integer is negative, 0 if it is zero,
      //or 1 if it is positive.
   DECMOD_GMP_INTS int GMP_INTS_mpz_is_neg(const GMP_INTS_mpz_struct *arg);
   DECMOD_GMP_INTS int GMP_INTS_mpz_is_zero(const GMP_INTS_mpz_struct *arg);
   DECMOD_GMP_INTS int GMP_INTS_mpz_is_pos(const GMP_INTS_mpz_struct *arg);
      //These functions return 1 if the condition is met, or 0
      //otherwise.
   DECMOD_GMP_INTS int GMP_INTS_mpz_is_odd(const GMP_INTS_mpz_struct *arg);
      //Returns 1 if the integer is odd, or 0 if it is even.
   DECMOD_GMP_INTS int GMP_INTS_mpz_is_even(const GMP_INTS_mpz_struct *arg);
      //Returns 1 if the integer is even, or 0 if it is odd.
   DECMOD_GMP_INTS void GMP_INTS_mpz_negate(GMP_INTS_mpz_struct *arg);
      //Negates the number (changes the sign).
   DECMOD_GMP_INTS
   void GMP_INTS_mpn_normalize(GMP_INTS_limb_ptr  limb_array,
                               GMP_INTS_size_t   *idx);
      //Adjusts an index downward to bypass any most significant
      //zero limbs.  This was a macro in the GNU implementation.
      //This is for when the size of a result is overestimated
      //(the most pessimistic estimates must be made when 
      //allocating memory before an operation).
   DECMOD_GMP_INTS
   void GMP_INTS_mpn_copy_limbs(GMP_INTS_limb_ptr    dest,
                                GMP_INTS_limb_srcptr src,
                                GMP_INTS_size_t      n);
      //Copies limbs from source to destination.  This
      //function replaces a macro that was present in the
      //GNU code.  Again, clarity over speed.
   /****************************************************************/
   /***   LOW-LEVEL ARITHMETIC FUNCTIONS   *************************/
   /****************************************************************/
   //Gives the flags of a result integer as a function of the
   //flags of its two operands.  If the function requires only
   //one operand, just use zero as the second parameter and the
   //result will be correct.  Handles tainting.
   DECMOD_GMP_INTS
   int GMP_INTS_two_op_flags_map(int flags1, int flags2);

   //Adds the single limb s2_limb to the array of limbs s1, 
   //processing carries, and copies the result to the location 
   //res_ptr.  res_ptr is assumed to be [at least] the same size
   //as s1.  From the design, s1 and res_ptr may be the
   //same set of locations.  The
   //result is 1 if there was a carry out of the final limb or
   //0 otherwise.
   DECMOD_GMP_INTS
   GMP_INTS_limb_t GMP_INTS_mpn_add_1 (GMP_INTS_limb_ptr    res_ptr,
                                       GMP_INTS_limb_srcptr s1_ptr,
                                       GMP_INTS_size_t      s1_size,
                                       GMP_INTS_limb_t      s2_limb);

   //Very similar to the addition function.  Counts on the single
   //limb that is being subtracted being not enough to roll over the
   //integer that is being subtracted from.  No harm if this is done
   //(it just obeys 2-s complement rules) but this was probably not
   //the intent.  From the design, s1 and res_ptr may 
   //be the same set of locations.  The
   //result is 1 if there was a borrow out of the final limb or
   //0 otherwise.
   DECMOD_GMP_INTS
   GMP_INTS_limb_t GMP_INTS_mpn_sub_1(GMP_INTS_limb_ptr    res_ptr,
                                      GMP_INTS_limb_srcptr s1_ptr,
                                      GMP_INTS_size_t      s1_size,
                                      GMP_INTS_limb_t      s2_limb);

   //Multiplies a limb by a group of limbs.  The return value is 
   //the unsigned overflow out of the "top", which may be up to
   //one limb large (i.e. it is more than just a carry, it has 
   //a value besides 0 and 1).
   DECMOD_GMP_INTS
   GMP_INTS_limb_t GMP_INTS_mpn_mul_1(GMP_INTS_limb_ptr    res_ptr, 
                                      GMP_INTS_limb_srcptr s1_ptr, 
                                      GMP_INTS_size_t      s1_size, 
                                      GMP_INTS_limb_t      s2_limb);

   //Adds together two groups of limbs of same size.  The result
   //may be the same location as one or both of the two
   //operands.
   DECMOD_GMP_INTS
   GMP_INTS_limb_t GMP_INTS_mpn_add_n(GMP_INTS_limb_ptr    res_ptr,  
                                      GMP_INTS_limb_srcptr s1_ptr, 
                                      GMP_INTS_limb_srcptr s2_ptr, 
                                      GMP_INTS_size_t      size);

   //This function makes the mapping:
   //   res_ptr = res_ptr + s2_limb * s1_ptr
   //From the design, it appears that it is alright if 
   //res_ptr and s1_ptr are the same area.  The return
   //value is the excess that that should exist one limb
   //above the MSL of res_ptr (i.e. it is more than a carry, it
   //may be larger than one).
   DECMOD_GMP_INTS
   GMP_INTS_limb_t GMP_INTS_mpn_addmul_1(GMP_INTS_limb_ptr    res_ptr, 
                                         GMP_INTS_limb_srcptr s1_ptr, 
                                         GMP_INTS_size_t      s1_size, 
                                         GMP_INTS_limb_t      s2_limb);

   //This function adds in general two natural numbers.  Numbers must
   //be arranged so that S2 takes no more limbs than S1, i.e. 
   //LIMBS(s1) >= LIMBS(s2).  Memory must be allocated, and there must
   //be space for the result.  Not clear from the design if memory
   //areas can be coincident, but suspect can.
   DECMOD_GMP_INTS
   GMP_INTS_limb_t GMP_INTS_mpn_add(GMP_INTS_limb_ptr    res_ptr,
                                    GMP_INTS_limb_srcptr s1_ptr,
                                    GMP_INTS_size_t      s1_size,
                                    GMP_INTS_limb_srcptr s2_ptr,
                                    GMP_INTS_size_t      s2_size);

   //Subtracts two same-size operands.  They may be coincident in
   //memory.
   DECMOD_GMP_INTS
   GMP_INTS_limb_t GMP_INTS_mpn_sub_n(GMP_INTS_limb_ptr    res_ptr, 
                                      GMP_INTS_limb_srcptr s1_ptr, 
                                      GMP_INTS_limb_srcptr s2_ptr, 
                                      GMP_INTS_size_t      size);

   //This function subtracts in general two natural numbers.  Numbers must
   //be arranged so that S2 takes no more limbs than S1, i.e. 
   //LIMBS(s1) >= LIMBS(s2).  Memory must be allocated, and there must
   //be space for the result.  Not clear from the design if memory
   //areas can be coincident.  Result is S1-S2.
   DECMOD_GMP_INTS
   GMP_INTS_limb_t GMP_INTS_mpn_sub (GMP_INTS_limb_ptr    res_ptr,
                                     GMP_INTS_limb_srcptr s1_ptr,
                                     GMP_INTS_size_t      s1_size,
                                     GMP_INTS_limb_srcptr s2_ptr,
                                     GMP_INTS_size_t      s2_size);

   //Shifts UP of size USIZE left by CNT.  CNT must be less than the number
   //of bits per limb, which is at this time 32.  "wp" and "up" may be 
   //coincident.  Zero count is not allowed.  Value returned is the bits shifted
   //out the left.
   DECMOD_GMP_INTS
   GMP_INTS_limb_t GMP_INTS_mpn_lshift(GMP_INTS_limb_ptr    wp,
                                       GMP_INTS_limb_srcptr up, 
                                       GMP_INTS_size_t      usize,
                                       unsigned int         cnt);

   //Shifts UP of size USIZE right by CNT.  CNT must be less than the number
   //of bits per limb, which is at this time 32.  "wp" and "up" may be 
   //coincident.  Zero count is not allowed.  Value returned is the bits shifted
   //out the right.
   DECMOD_GMP_INTS
   GMP_INTS_limb_t GMP_INTS_mpn_rshift (GMP_INTS_limb_ptr    wp,
                                        GMP_INTS_limb_srcptr up, 
                                        GMP_INTS_size_t      usize,
                                        unsigned int         cnt);

   //Compares two natural numbers of same size and returns the expected
   //1 iff op1 > op2, -1 if op1 < op2, or 0 if they are equal.  Leading
   //zero limbs are alright and do not affect the result.
   DECMOD_GMP_INTS
   int GMP_INTS_mpn_cmp (GMP_INTS_limb_srcptr op1_ptr, 
                         GMP_INTS_limb_srcptr op2_ptr, 
                         GMP_INTS_size_t      size);
      
   //This is the basic multiplication of two natural numbers, each of
   //which may occuply many limbs.  In the 
   //original GNU MP code, there were several algorithms which could
   //be selected, depending on the size of the operands and other
   //factors.  This was the most basic case--the basic longhand
   //multiplication.  The code has been pared so that this is the
   //only case.
   DECMOD_GMP_INTS
   void GMP_INTS_mpn_mul_basecase (GMP_INTS_limb_ptr prodp,
                                   GMP_INTS_limb_srcptr up, 
                                   GMP_INTS_size_t usize,
                                   GMP_INTS_limb_srcptr vp, 
                                   GMP_INTS_size_t vsize);

   //This is the basic multiplication of two natural numbers.  In the old
   //GNU MP code, one of several algorithms would be selected.  The code
   //has been pared down so that this is just a passthrough to
   //GMP_INTS_mpn_mul_basecase().  Only the simplest multiplication
   //algorithm is used at this point.
   DECMOD_GMP_INTS
   void GMP_INTS_mpn_mul_n (GMP_INTS_limb_ptr    p, 
                            GMP_INTS_limb_srcptr a, 
                            GMP_INTS_limb_srcptr b, 
                            GMP_INTS_size_t      n);
   //Multiplication of two natural numbers.  Value returned is the most
   //significant limb of the array of predicted size un+vn, rather than
   //the spillover (this is unlike most functions).
   DECMOD_GMP_INTS
   GMP_INTS_limb_t GMP_INTS_mpn_mul(GMP_INTS_limb_ptr    prodp,
                                    GMP_INTS_limb_srcptr up, 
                                    GMP_INTS_size_t      un,
                                    GMP_INTS_limb_srcptr vp, 
                                    GMP_INTS_size_t      vn);

   /******************************************************************/
   /***  LIMB SPACE REALLOCATION FUNCTIONS   *************************/
   /******************************************************************/
   DECMOD_GMP_INTS
   void *GMP_INTS_mpz_realloc (GMP_INTS_mpz_struct *m, 
                               GMP_INTS_size_t      new_size);
      //Changes the number of limbs allocated.

   /******************************************************************/
   /***  PUBLIC INITIALIZATION AND MEMORY MANAGEMENT FUNCTIONS   *****/
   /******************************************************************/
   //Allocate space for an integer and sets it to zero.
   //This must be the first call made.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_init (GMP_INTS_mpz_struct *x);

   //Deallocates space for an integer.  This must be the
   //final call made.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_clear (GMP_INTS_mpz_struct *x);


   /******************************************************************/
   /***  PUBLIC ASSIGNMENT FUNCTIONS   *******************************/
   /******************************************************************/
   //Copies from one integer to another.  Must both be allocated and \
   //not be the same variable.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_copy(      GMP_INTS_mpz_struct *dst,
                          const GMP_INTS_mpz_struct *src);

   //Assigns the integer to be the value of an unsigned long.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_set_ui (GMP_INTS_mpz_struct *dest, 
                             unsigned long int val);

   //Assigns the integer to be the value of a signed long.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_set_si (GMP_INTS_mpz_struct *dest, 
                             signed long int val);
   //Assigns the integer to be the value of a simple string
   //(simple = no E notation).  Only form handled is
   //   [-]digits.
   //Other forms will not cause catastrophic failure but might
   //not behave as expected.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_set_simple_char_str(GMP_INTS_mpz_struct *z, 
                                         const char *s);
   //Assigns an arbitary integer to be a number with E notation.
   //*failure set non-zero if can't parse or set.  Number must
   //be pure integer--no lost precison when scientific notation
   //is processed.
   void GMP_INTS_mpz_set_sci_not_num(GMP_INTS_mpz_struct *z,
                                     int *failure, 
                                     const char *s);

   //Attempts to parse an integer using the following three     
   //formats:  a)simple integer, b)simple integer with commas,
   //c)integer in scientific notation.  Returns the value of
   //the integer if parse successful, or 0 and a failure flag
   //otherwise.
   void GMP_INTS_mpz_set_general_int(GMP_INTS_mpz_struct *z,
                                     int *failure, 
                                     const char *s);

   //Attempts to parse an integer as either a simple integer,
   //an integer with commas, or a number in scientific notation
   //into a UINT32.  If the number cannot be parsed into that,
   //the result is zero and failure is true.
   void GMP_INTS_mpz_parse_into_uint32(unsigned *result,
                                       int *failure,
                                       char *s);

   //Swaps a and b.  What this does is a swap of the area
   //pointed to (i.e. the control block).  Pointers held in
   //these blocks are therefore automatically swapped.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_swap(GMP_INTS_mpz_struct *a, 
                          GMP_INTS_mpz_struct *b);

   /****************************************************************/
   /***   PUBLIC INTEGER ARITHMETIC FUNCTIONS   ********************/
   /****************************************************************/
   //Adds two arbitrary integers to produce an arbitrary result.
   //The result space must be initialized already.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_add (      GMP_INTS_mpz_struct *w, 
                          const GMP_INTS_mpz_struct *u, 
                          const GMP_INTS_mpz_struct *v);

   //Adds the integer v to u and produces w.  u and w must be
   //be created already.
   void GMP_INTS_mpz_add_ui (      GMP_INTS_mpz_struct *w, 
                             const GMP_INTS_mpz_struct *u, 
                             unsigned long int          v);

   //Subtracts two aribtrary integers to produce an arbitrary
   //result.  The result space must be initialized already.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_sub (      GMP_INTS_mpz_struct *w, 
                          const GMP_INTS_mpz_struct *u, 
                          const GMP_INTS_mpz_struct *v);

   //Subtracts the integer v from u to produce w.  u and w must 
   //be created already.
   void GMP_INTS_mpz_sub_ui (      GMP_INTS_mpz_struct *w, 
                             const GMP_INTS_mpz_struct *u, 
                             unsigned long int          v);

   //Multiplies two arbitrary integers to produce an arbitrary
   //result.  The result space must be initialized already.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_mul (      GMP_INTS_mpz_struct *w, 
                          const GMP_INTS_mpz_struct *u, 
                          const GMP_INTS_mpz_struct *v);

   //Multiplies the arbitrary integer by a C-native signed
   //long.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_mul_si (      GMP_INTS_mpz_struct *w, 
                             const GMP_INTS_mpz_struct *u, 
                                   long int             v);
   //Multiplies the arbitrary integer by a C-native unsigned
   //long.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_mul_ui (      GMP_INTS_mpz_struct *w, 
                             const GMP_INTS_mpz_struct *u, 
                             unsigned long int          v);

   //Divides integers.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_tdiv_qr (      GMP_INTS_mpz_struct *quot, 
                                    GMP_INTS_mpz_struct *rem, 
                              const GMP_INTS_mpz_struct *num, 
                              const GMP_INTS_mpz_struct *den);

   //Calculates the factorial.  All values <=1 result in a
   //value of 1.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_fac_ui(GMP_INTS_mpz_struct *result, 
                            unsigned long int n);

   //Exponentiates a base to an exponent.  0^0=1, N^0=1.  The algorithm
   //used is successive squaring and multiplying.  Not the MOST efficient,
   //but OK.  The result and the base must be different variables.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_pow_ui(         GMP_INTS_mpz_struct *result,
                            const    GMP_INTS_mpz_struct *base,
                            unsigned                      exponent);

   //Takes the absolute value of the arbitrary integer passed.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_abs(GMP_INTS_mpz_struct *arg);

   //Calculates the gcd() of the two arguments.  If either argument is zero, the
   //result is automatically 1.  If either argument is negative, its absolute
   //value is used.  The two input pointers may not be the same, because doing
   //that is senseless--it would become a copy operation, because gcd(x,x) = x.
   //However, the result may be the same as either of the two inputs.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_gcd(GMP_INTS_mpz_struct *result,
                         const GMP_INTS_mpz_struct *arg1,
                         const GMP_INTS_mpz_struct *arg2);


   /******************************************************************/
   /***  PUBLIC CONVERSION AND OUTPUT FUNCTIONS   ********************/
   /******************************************************************/

   //Get an upper bound on the number of characters required for
   //representing an integer, including minus sign and commas,
   //Terminating zero, etc.  This is just a little wasteful, but 
   //will always reserve enough memory.
   DECMOD_GMP_INTS
   int GMP_INTS_mpz_size_in_base_10(const GMP_INTS_mpz_struct *arg);

   //Convert from integer to string.  Commas must be
   //added separately, and there is enough space reserved for
   //them.  This function is only warrantied not to overflow
   //a buffer allocated using the function above as a sizing
   //guide.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_to_string(char *out,
                               const GMP_INTS_mpz_struct *in);

   //Prints the integer passed to the stream in a long-hand
   //format.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_long_int_format_to_stream(FILE *s,
                                               const GMP_INTS_mpz_struct *i,
                                               const char *desc);

   //Prints the integer raw to a stream--just digits, no extra chars.
   DECMOD_GMP_INTS
   void GMP_INTS_mpz_arb_int_raw_to_stream(FILE *s,
                                        const GMP_INTS_mpz_struct *i);

   /******************************************************************/
   /***  COMPARISON AND SIZING FUNCTIONS   ***************************/
   /******************************************************************/
   //Returns 1 if arbitrary integer will fit in an unsigned integer,
   //or 0 otherwise.
   DECMOD_GMP_INTS
   int GMP_INTS_mpz_fits_uint_p (const GMP_INTS_mpz_struct *src);

   //Returns 1 if arbitrary integer will fit in a signed integer,
   //or 0 otherwise.
   DECMOD_GMP_INTS
   int GMP_INTS_mpz_fits_sint_p (const GMP_INTS_mpz_struct *src);

   //Retrieves an unsigned version of limb zero from the number,
   //or will return zero if the number is zero.  This is the
   //officially sanctioned way to get the value if it fits in
   //an unsigned.
   DECMOD_GMP_INTS
   unsigned GMP_INTS_mpz_get_limb_zero(const GMP_INTS_mpz_struct *src);

   //Returnes neg value if u<v, 0 if u==v, and 1 if u>v.
   DECMOD_GMP_INTS 
   int GMP_INTS_mpz_cmp (const GMP_INTS_mpz_struct *u, 
                         const GMP_INTS_mpz_struct *v);

   //Compares an arbitrary integer to an unsigned long
   //and returns relative ordering, <0 if u < v, 0 if u==v,
   //and >0 if u > v.
   DECMOD_GMP_INTS
   int GMP_INTS_mpz_cmp_ui (const GMP_INTS_mpz_struct *u, 
                            unsigned long int v_digit);
   //Compares arbitrary integer to a signed long
   //and returns relative ordering, <0 if u<v, 0 if u==v,
   //and >0 if u>v.
   DECMOD_GMP_INTS
   int GMP_INTS_mpz_cmp_si (const GMP_INTS_mpz_struct *u, 
                            signed long int v_digit);

   //Compares the absolute value of two arbitrary integers.
   DECMOD_GMP_INTS
   int GMP_INTS_mpz_cmpabs (const GMP_INTS_mpz_struct *u, 
                            const GMP_INTS_mpz_struct *v);
   //Compares the absolute value of an arbitrary integer and
   //an unsigned long.
   DECMOD_GMP_INTS
   int GMP_INTS_mpz_cmpabs_ui(const GMP_INTS_mpz_struct *u, 
                              unsigned long int v_digit);

   /****************************************************************/
   /***   VERSION CONTROL REPORTING FUNCTIONS   ********************/
   /****************************************************************/
   DECMOD_GMP_INTS const char *GMP_INTS_cvcinfo(void);
   DECMOD_GMP_INTS const char *GMP_INTS_hvcinfo(void);

   /* Preprocessor string to allow the H-file version to be
   ** compiled into the C-file.
   */
   #define GMP_INTS_H_VERSION ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/gmp_ints.h 71 2016-11-05 11:07:06Z dashley $")

#endif /* GMP_INTS_H_INCLUDED */

// End of gmp_ints.h.
