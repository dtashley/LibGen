//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/string_c/bstrfunc.h 242 2018-08-04 18:25:51Z dashley $
//--------------------------------------------------------------------------------------------------
//This file is part of LibGen, https://github.com/dtashley/LibGen, and is provided
//under The Unlicense, reproduced below.
//--------------------------------------------------------------------------------------------------
//This is free and unencumbered software released into the public domain.
//
//Anyone is free to copy, modify, publish, use, compile, sell, or
//distribute this software, either in source code form or as a compiled
//binary, for any purpose, commercial or non - commercial, and by any
//means.
//
//In jurisdictions that recognize copyright laws, the author or authors
//of this software dedicate any and all copyright interest in the
//software to the public domain.We make this dedication for the benefit
//of the public at large and to the detriment of our heirs and
//successors.We intend this dedication to be an overt act of
//relinquishment in perpetuity of all present and future rights to this
//software under copyright law.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//OTHER DEALINGS IN THE SOFTWARE.
//
//For more information, please refer to <https://unlicense.org>
//--------------------------------------------------------------------------------------------------
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
