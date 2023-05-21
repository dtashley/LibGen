//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/hash/md5/esrg_md5.h 242 2018-08-04 18:25:51Z dashley $
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
#ifndef ESRG_MD5_H_INCLUDED
   #define ESRG_MD5_H_INCLUDED

   #ifdef MODULE_ESRG_MD5
      #define DECMOD_ESRG_MD5
   #else
      #define DECMOD_ESRG_MD5 extern
   #endif

   //Fundamental state (or RFC 1321 calls it context) for forming
   //MD5s.  Conceptually private to this module.
   struct ESRG_MD5_Md5StateStruct
      {
      unsigned  A,  B,  C,  D;
         //Directly from RFC 1321.
      unsigned __int64 bit_count;
         //The count of bits processed thus far.  The algorithm here
         //works in bytes, not bits, so this is advanced by 8 on
         //each byte processed.
      unsigned X[16];
         //These are the words corresponding to the chars.  We don't
         //dare union them because of big-endian/little-endian concerns.
         //The name "X" comes directly from RFC 1321 nomenclature.
      unsigned char buf[64];
         //We can't proceed to execute a round unless we have the 
         //full 512 bits = 16 words = 64 bytes of data.  We must
         //buffer it because we can't count on being called with data
         //blocks that are a multiple of 64.  We may have data hanging
         //around between calls.  We fill up this buffer from the low
         //end, i.e. [0], then [1], then [2], etc.
      };

   //Result structure, used to hold result.  Caller is allowed to
   //pick it apart.
   struct ESRG_MD5_Md5ResultStruct
      {
      unsigned md5_words[4];
      char     md5_chars[33];
         //Zero terminated string containing MD5 formed.
      };

   //Initializes the MD5 calculation structure.
   DECMOD_ESRG_MD5 
   void ESRG_MD5_Md5StateStructOpen(struct ESRG_MD5_Md5StateStruct *arg);

   //Adds data to it.  Zero length is OK.
   DECMOD_ESRG_MD5 
   void ESRG_MD5_Md5StateStructAddData(struct ESRG_MD5_Md5StateStruct *arg,
                                                                 void *data,
                                                              unsigned len);

   //Closes the structure and returns the MD5.  This is destructive--one cannot
   //continue adding characters.  All characters returned are lower-case.
   DECMOD_ESRG_MD5
   void ESRG_MD5_Md5StateStructClose(struct ESRG_MD5_Md5StateStruct  *state,
                                     struct ESRG_MD5_Md5ResultStruct *result);

   //Returns version control information.
   DECMOD_ESRG_MD5 const char *ESRG_MD5_cvcinfo(void);
   DECMOD_ESRG_MD5 const char *ESRG_MD5_hvcinfo(void);

   //Definition of the version of the H file, used for providing version control
   //information to external callers.
   #define ESRG_MD5_H_VERSION ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/hash/md5/esrg_md5.h 242 2018-08-04 18:25:51Z dashley $")

#endif

//End of esrg_md5.h.
