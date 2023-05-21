//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/crchashfuncs.h 71 2016-11-05 11:07:06Z dashley $
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
//This software module is concerned with functions to support the generation
//of CRCs and hash functions.  CRCs and hash functions are similar in that they
//provide an abstraction of a large amount of data, although CRCs may have better
//(or different) statistical properties than hash functions.  These functions
//are split out from any specific application for maximum reusability of the
//code.

#ifndef CRCHASHFUNCS_H_INCLUDED
   #define CRCHASHFUNCS_H_INCLUDED

   #ifdef MODULE_CRCHASHFUNCS
      #define DECMOD_CRCHASHFUNCS
   #else
      #define DECMOD_CRCHASHFUNCS extern
   #endif

   //Define the state object that holds CRC-32 intermediate results as the CRC is
   //being formed.  This is conceptually private for the caller.
   //
   struct CRCHASHFUNCS_Crc32StateStruct
      {
      unsigned crc;
      //This is the only state for now.
      };

   //All CRC32 functions are courtesy of Richard A. Ellingson
   //from the Internet.
   //
   //Checks the CRC-32 table for integrity, returns non-zero if
   //the table is OK, or 0 if the table is corrupted.  The table
   //is a table of constants in memory--it is not known how
   //much protection the "const" keyword can give it, i.e. if
   //the tools can put it in a memory segment where some kind
   //of protection is afforded by the OS.
   DECMOD_CRCHASHFUNCS 
   int CRCHASHFUNCS_Crc32TableCheck(void);

   //Initializes the CRC formation structure.
   DECMOD_CRCHASHFUNCS 
   void CRCHASHFUNCS_Crc32StateStructOpen(struct CRCHASHFUNCS_Crc32StateStruct *arg);

   //Adds data to it.
   DECMOD_CRCHASHFUNCS
   void CRCHASHFUNCS_Crc32StateStructAddData(struct CRCHASHFUNCS_Crc32StateStruct *arg,
                                             void *data,
                                             unsigned len);

   //Reads the CRC out at any time.
   DECMOD_CRCHASHFUNCS 
   unsigned CRCHASHFUNCS_Crc32Extract(struct CRCHASHFUNCS_Crc32StateStruct *arg);

   //Closes the structure.
   DECMOD_CRCHASHFUNCS 
   void CRCHASHFUNCS_Crc32StateStructClose(struct CRCHASHFUNCS_Crc32StateStruct *arg);

   DECMOD_CRCHASHFUNCS const char *CRCHASHFUNCS_cvcinfo(void);
   DECMOD_CRCHASHFUNCS const char *CRCHASHFUNCS_hvcinfo(void);

   //Definition of the version of the H file, used for providing version control
   //information to external callers.
   #define CRCHASHFUNCS_H_VERSION ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/crchashfuncs.h 71 2016-11-05 11:07:06Z dashley $")
#endif

//End of crchashfuncs.h.
