//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/string_c/bstrfunc.h 242 2018-08-04 18:25:51Z dashley $
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
#ifndef BSTRFUNC_H_INCLUDED
   #define BSTRFUNC_H_INCLUDED

   #ifdef MODULE_BSTRFUNC
      #define DECMOD_BSTRFUNC
   #else
      #define DECMOD_BSTRFUNC extern
   #endif

   DECMOD_BSTRFUNC void *BSTRFUNC_malloc( size_t size );
   DECMOD_BSTRFUNC void *BSTRFUNC_calloc( size_t num, size_t size );
   DECMOD_BSTRFUNC void *BSTRFUNC_realloc( void *memblock, size_t size );
   DECMOD_BSTRFUNC void  BSTRFUNC_free( void *memblock );

   //Returns 1 if the passed string is a valid unsigned integer
   //without commas, i.e. "0", or "23943284...".  Returns 0
   //if is not of that data type.  The the only allowed representation
   //for zero is a single "0" character.  The string must be 
   //"pure", i.e. no whitespace or non-digit characters or else
   //a value of 0 will be returned.
   DECMOD_BSTRFUNC int BSTRFUNC_is_uint_wo_commas(const char *arg);

   //Just like the function above, but requires commas, which must
   //be appropriately placed.  Generally, any unsigned integer will
   //return TRUE on the function above or on this one, and both 
   //will return TRUE for a number of 3 or fewer digits.
   DECMOD_BSTRFUNC int BSTRFUNC_is_uint_w_commas(const char *arg);

   //Returns 1 if the passed string is a valid signed integer
   //without commas, i.e. "0", or "-23943284...".  Returns 0
   //if is not of that data type.  The the only allowed representation
   //for zero is a single "0" character.  The string must be 
   //"pure", i.e. no whitespace or non-digit characters or else
   //a value of 0 will be returned.
   DECMOD_BSTRFUNC int BSTRFUNC_is_sint_wo_commas(const char *arg);

   //Just like the function above, but allows negative numbers with
   //commas, which must be appropriately placed.  Generally, any 
   //signed integer will return TRUE on the function above or on 
   //this one, and both will return TRUE for a number of 3 or 
   //fewer digits.
   DECMOD_BSTRFUNC int BSTRFUNC_is_sint_w_commas(const char *arg);

   //Parses a general number in scientific notation, looking
   //for errors, and splits it into components.
   //
   //   s
   //      The string to parse.  May not be NULL.
   //   *failure
   //      Set to 0 if parsing success.  At the present time,
   //      any non-zero value is failure, but this may be refined
   //      later with specific error codes.  Code written at this
   //      time should just test against zero.  The pointer to
   //      this variable ("failure") must not be NULL.  All other
   //      pointers may be NULL if the caller does not want the
   //      information back.  If there was a failure, no other
   //      information should be interpreted.
   //   *mant_sign
   //      Will be set to +, -, or N (N means that the sign was
   //      not specified--this should be interpreted as +,
   //      usually.
   //   *mant_bdp, *mant_bdp_len
   //      Digits of mantissa before decimal point.  If there is
   //      a zero here, only one is allowed.  Possibilities here
   //      are "", "0", or "NNN..NNN" where the leading digit is
   //      not zero.
   //   *mant_adp, *mant_adp_len
   //      Digits of mantissa after decimal point.  There may
   //      be any number of leading zeros here.
   //   *exp_sign,
   //      +, -, or N as described for mantissa sign, above.
   //   *exp, *exp_len
   //      The exponent, if any.  There may be any number of
   //      leading zeros.
   DECMOD_BSTRFUNC
   void BSTRFUNC_parse_gen_sci_not_num(const char *s,
                                       int *failure,
                                       char *mant_sign,
                                       size_t *mant_bdp,
                                       size_t *mant_bdp_len,
                                       size_t *mant_adp,
                                       size_t *mant_adp_len,
                                       char *exp_sign,
                                       size_t *exp,
                                       size_t *exp_len);

   //Reverses the order of characters in a string.
   DECMOD_BSTRFUNC void BSTRFUNC_str_reverse(char *s);

   //Adds commas into a string.  Space must exist.
   DECMOD_BSTRFUNC void BSTRFUNC_commanate(char *s);

   //Removes commas from a string.  Since the new string will be 
   //no longer than the old string, there are no worries about
   //space allocation.  The function is direct comma removal--
   //it pays no attention to syntax.
   DECMOD_BSTRFUNC void BSTRFUNC_decommanate(char *s);

   //Tries to parse a string as a 32-bit unsigned integer.
   //Supplies a return value, plus an error flag which is set
   //TRUE if something went wrong.
   DECMOD_BSTRFUNC
   void BSTRFUNC_parse_str_to_uint32(const char *s, 
                                  unsigned int *rv, 
                                  int *error);

   DECMOD_BSTRFUNC const char *BSTRFUNC_cvcinfo(void);
   DECMOD_BSTRFUNC const char *BSTRFUNC_hvcinfo(void);
   #define BSTRFUNC_H_VERSION ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/string_c/bstrfunc.h 242 2018-08-04 18:25:51Z dashley $")
#endif

//End of bstrfunc.h.
