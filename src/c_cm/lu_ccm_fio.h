//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_cmode/fcmiof.h 244 2018-08-05 19:05:46Z dashley $
//{f84eaac3-3de8-489b-afed-5d7f83e92748}
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
#ifndef FCMIOF_H_INCLUDED
   #define FCMIOF_H_INCLUDED

   extern int LU_CCM_stream_repchar(FILE *s, char c, size_t n);


   //********************************************************************************
   // FCMIOF_repchar():
   //--------------------------------------------------------------------------------
   // DESCRIPTION
   //   Emits a character to stdout a specified number of times.
   //
   // INPUTS
   //    n       
   //       The number of times to emit the character.  0 is a permitted value
   //       and results in no output.
   //
   // RETURN VALUE
   //   The return value from the final printf() function call, unless any
   //   printf() function call returns a negative value (an error code), in which
   //   case this function returns immediately with that error code.
   //********************************************************************************
   extern int FCMIOF_repchar(char c, size_t n);


   //********************************************************************************
   // FCMIOF_stream_hline():
   //--------------------------------------------------------------------------------
   // DESCRIPTION
   //   Emits a horizontal line of the hyphens of the specified length to the
   //   specified stream.
   //
   // INPUTS
   //    s       
   //       The stream to which to output the characters.  NULL and otherwise
   //       invalid pointers are not allowed.
   //    line_len
   //       The number of characters to use for the line.  0 is an allowed value
   //       and results in 
   //
   // RETURN VALUE
   //   The return value from the final fprintf() function call, unless any
   //   fprintf() function call returns a negative value (an error code), in which
   //   case this function returns immediately with that error code.
   //********************************************************************************
   extern int FCMIOF_stream_hline(FILE *s, size_t line_len);


   //********************************************************************************
   // FCMIOF_hline():
   //--------------------------------------------------------------------------------
   // DESCRIPTION
   //   Emits a horizontal line of the hyphens of the specified length to stdout.
   //
   // INPUTS
   //    line_len
   //       The number of characters to use for the line.  0 is an allowed value
   //       and results in 
   //
   // RETURN VALUE
   //   The return value from the final printf() function call, unless any
   //   printf() function call returns a negative value (an error code), in which
   //   case this function returns immediately with that error code.
   //********************************************************************************
   extern int FCMIOF_hline(size_t line_len);


   //********************************************************************************
   // FCMIOF_stream_bannerheading():
   //--------------------------------------------------------------------------------
   // DESCRIPTION
   //   Prints a banner heading bracketed by asterisks to the specified stream.
   //   This function is useful for separating different sections of output.
   //
   // INPUTS
   //    f       
   //       The stream to which to output the characters.  NULL and otherwise
   //       invalid pointers are not allowed.
   //    s
   //       The text to use for the banner.  This string may be of 0 length, but
   //       the pointer "s" may not be NULL or otherwise invalid.
   //    line_len
   //       The number of characters to use for each line.  0 is an allowed value
   //       and results in lines with no output.
   //    n_extra_lines
   //       The number of lines of asterisks to use before and after the banner.
   //       0 is an allowed value and results in no lines of asterisks.
   //
   // RETURN VALUE
   //   The return value from the final printf() function call, unless any
   //   printf() function call returns a negative value (an error code), in which
   //   case this function returns immediately with that error code.
   //********************************************************************************
   extern int FCMIOF_stream_bannerheading(FILE *f,
                                                 char *s, 
                                                 size_t line_len,
                                                 size_t n_extra_lines);

   /****************************************************************************/
   /* FCMIOF_bannerheading():                                                  */
   /*--------------------------------------------------------------------------*/ 
   /* DESCRIPTION                                                              */
   /*   Prints a banner heading bracketed by astreisks to the standard output. */
   /*   This function is useful for separating different sections of output.   */
   /****************************************************************************/
   extern int FCMIOF_bannerheading(char *s, size_t line_len, size_t n_extra_lines);


   extern void FCMIOF_time_stream(FILE *s, time_t ltime);


   /* Two functions below return version control information for .C and
   ** .H files.
   */
   extern const char *FCMIOF_cvcinfo(void);
   extern const char *FCMIOF_hvcinfo(void);
   #define FCMIOF_H_VERSION ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_cmode/fcmiof.h 244 2018-08-05 19:05:46Z dashley $")

#endif

//End of fcmiof.h.
