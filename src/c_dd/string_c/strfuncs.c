//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/string_c/strfuncs.c 242 2018-08-04 18:25:51Z dashley $
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
#define MODULE_STRFUNCS

#include <string.h>

#include "strfuncs.h"


//Converts from an unsigned char quantity to a hexadecimal digit, with
//upper-case/lower-case option.  Only the lowest four bits are considered.
DECMOD_STRFUNCS 
char Strfuncs_NibbleToHexadecimalChar(unsigned char arg, int use_upper_case)
   {
   char rv;

   switch (arg & 0x0F)
      {
      case  0:
         rv = '0';
         break;
      case  1:
         rv = '1';
         break;
      case  2:
         rv = '2';
         break;
      case  3:
         rv = '3';
         break;
      case  4:
         rv = '4';
         break;
      case  5:
         rv = '5';
         break;
      case  6:
         rv = '6';
         break;
      case  7:
         rv = '7';
         break;
      case  8: 
         rv = '8';
         break;
      case  9:
         rv = '9';
         break;
      case 10:
         rv = 'A';
         break;
      case 11:
         rv = 'B';
         break;
      case 12:
         rv = 'C';
         break;
      case 13:
         rv = 'D';
         break;
      case 14:
         rv = 'E';
         break;
      case 15:
         rv = 'F';
         break;
      }

   //If upper-case is not desired, convert to lower.
   if (!use_upper_case && (rv >= 'A'))
      rv += ('a'-'A');

   return(rv);
   }


//Given a single unsigned long number, assigns eight consecutive characters of a 
//string to be the hexadecimal digits of the number.  The eight characters must
//already be allocated in the caller's area.  An option is provided to use
//upper-case or lower-case.  The caller is responsible for null termination.
DECMOD_STRFUNCS
void Strfuncs_UlToHexString(unsigned long num,
                            char *output,
                            int use_upper_case)
   {
   unsigned i;

   for (i=0; i<8; i++)
      {
      output[7-i] = Strfuncs_NibbleToHexadecimalChar((unsigned char)num, use_upper_case);
      num >>= 4;
      }
   }


//Returns true if substr is a substring of arg.  An empty string is a substring
//of anything except a NULL pointer, and a NULL pointer is a substring of
//anything.
DECMOD_STRFUNCS 
int Strfuncs_IsSubstring(const char *substr, const char *arg)
   {
   size_t i;

   if (!substr)
      {
      return(1);
      //A NULL pointer is a substring of anything.
      }

   if (!arg)
      {
      return(0);
      //arg is NULL but substr is not, no substring.
      }

   //Will not be here unless both pointers are valid.  These are the
   //non-degenerate cases.
   i = 0;
   while(1)
      {
      if (!substr[i])
         {
         return(1);
         //Ran out of substr chars before arg chars.  Is a substring.
         }
      if (!arg[i])
         {
         return(0);
         //Ran out of arg characters with substr chars still remaining.
         //Definitely not a substring.
         }
      //Have exhausted neither string.  Not a substring if lack of equality.
      if (substr[i] != arg[i])
         {
         return(0);
         //Definitely not a substring.
         }

      //Can't make a determination.  Advance to next character.
      i++;
      }
   }


//Returns version control string for file.
//
DECMOD_STRFUNCS
const char *StrfuncsCversion(void)
{   
    return ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/string_c/strfuncs.c 242 2018-08-04 18:25:51Z dashley $");
}


//Returns version control string for associated .H file.
//
DECMOD_STRFUNCS
const char *StrfuncsHversion(void)
{   
    return (STRFUNCS_H_VERSION);
}

//End of strfuncs.c.
