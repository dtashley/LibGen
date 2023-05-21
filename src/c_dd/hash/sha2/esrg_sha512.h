//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/hash/sha2/esrg_sha512.h 242 2018-08-04 18:25:51Z dashley $
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
#ifndef ESRG_SHA512_H_INCLUDED
   #define ESRG_SHA512_H_INCLUDED

   #ifdef MODULE_ESRG_SHA512
      #define DECMOD_ESRG_SHA512
   #else
      #define DECMOD_ESRG_SHA512 extern
   #endif

   //Fundamental state for forming
   //SHA-512s.  Conceptually private to this module.
   struct ESRG_SHA512_Sha512StateStruct
      {
      unsigned __int64 H0, H1, H2, H3, H4, H5, H6, H7;
         //Directly from FIPS 180-3.  In retrospect, this might have
	     //been better implemented as an array.
      unsigned __int64 bit_count;
         //The count of bits processed thus far.  The algorithm here
         //works in bytes, not bits, so this is advanced by 8 on
         //each byte processed.  FIPS 180-3 calls for processing
	     //messages up to length 2^128, but for obvious reasons
	     //we don't do that.  2^64-1 bits is in excess of 2^61-1
	     //bytes, or somewhere around 2,000 terabytes.  This
	     //isn't a practical limit with current computer technology.
      unsigned __int64 M[16];
         //These are the words corresponding to the chars (below).  We don't
         //dare union to extract them because of big-endian/little-endian concerns.
	     //The "M" nomenclature is from FIPS 180-3.  At the time the
	     //SHA-512 rounds are done, the chars (below) are converted to words
	     //(this field) so that the rounds can be done using the words.
      unsigned char buf[128];
         //We can't proceed to execute a round unless we have the 
         //full 1024 bits = 16 words = 128 bytes of data.  We must
         //buffer it because we can't count on being called with data
         //blocks that are a multiple of 128.  We may have data hanging
         //around between calls.  We fill up this buffer from the low
         //end, i.e. [0], then [1], then [2], etc.
      };


   //Result structure, used to hold result.  Caller is allowed to
   //pick it apart.
   struct ESRG_SHA512_Sha512ResultStruct
      {
      unsigned __int64 sha512_words[8];
	     //Hash in binary form, as the 64-bit integers.
      char             sha512_chars[129];
         //Zero terminated string containing MD5 formed.
      };


   //Initializes the SHA512 calculation structure.
   DECMOD_ESRG_SHA512 
   void ESRG_SHA512_Sha512StateStructOpen(struct ESRG_SHA512_Sha512StateStruct *arg);

   //Adds data to it.  Zero length is OK.
   DECMOD_ESRG_SHA512 
   void ESRG_SHA512_Sha512StateStructAddData(struct ESRG_SHA512_Sha512StateStruct *arg,
                                                                 void *data,
                                                                 unsigned len);

   //Closes the structure and returns the SHA512.  This is destructive--one cannot
   //continue adding characters.  All characters returned are lower-case.
   DECMOD_ESRG_SHA512
   void ESRG_SHA512_Sha512StateStructClose(struct ESRG_SHA512_Sha512StateStruct  *state,
                                           struct ESRG_SHA512_Sha512ResultStruct *result);

   //Returns version control information.
   DECMOD_ESRG_SHA512 const char *ESRG_SHA512_cvcinfo(void);
   DECMOD_ESRG_SHA512 const char *ESRG_SHA512_hvcinfo(void);

   //Definition of the version of the H file, used for providing version control
   //information to external callers.
   #define ESRG_SHA512_H_VERSION ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/hash/sha2/esrg_sha512.h 242 2018-08-04 18:25:51Z dashley $")

#endif

//End of esrg_sha512.h.
