//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/cu_msgs.h 71 2016-11-05 11:07:06Z dashley $
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
/* Messages common to command-line utilities.  They are collected
** in one place to avoided redundancy.
*/

#ifndef CU_MSGS_H_INCLUDED
   #define CU_MSGS_H_INCLUDED

   #ifdef MODULE_CU_MSGS
      #define DECMOD_CU_MSGS
   #else
      #define DECMOD_CU_MSGS extern
   #endif

   //This is the structure type used to pass back to a caller
   //the results of looking at his command-line parameter(s).
   typedef struct
      {
      int debug;
         //True if -debug flag was used.
      int noformat;
         //True if -noformat flag was used.
      int help;
         //True if -help flag was used.
      int verbose;
         //True if the -verbose option was used.
      } CU_MSGS_std_cmd_line_par_results_struct;

   //Prints out the message which should occur when too few 
   //arguments are used on the command-line.
   DECMOD_CU_MSGS
   void CU_MSGS_too_few_args_msg(FILE *s, const char *pname);

   //Prints out the message which supplies download
   //and license information.
   DECMOD_CU_MSGS
   void CU_MSGS_toolset_info_msg(FILE *s, const char *pname);

   //Prints standard options, indented by 3 and 6 spaces.
   DECMOD_CU_MSGS
   void CU_MSGS_std_options(FILE *s, const char *pname);

   //Initializes the common parameter block structure, 
   //including any dynamic allocation (at this time, none).
   DECMOD_CU_MSGS
   void CU_MSGS_cmd_line_par_results_struct_create(
                CU_MSGS_std_cmd_line_par_results_struct *arg);

   //Processes the passed command-line argument into the 
   //structure.
   DECMOD_CU_MSGS
   void CU_MSGS_cmd_line_par_results_struct_process_arg(
           CU_MSGS_std_cmd_line_par_results_struct *argblock,
           const char *arg, int *recognized);

   //Finalizes the results, such as taking care of conflicting
   //switches, etc.
   void CU_MSGS_cmd_line_par_results_struct_finalize(
           CU_MSGS_std_cmd_line_par_results_struct *arg);

   //Destroys the command-line argument.  This is left as a 
   //hook in case dynamic allocation is ever required.
   DECMOD_CU_MSGS
   void CU_MSGS_cmd_line_par_results_struct_destroy(
           CU_MSGS_std_cmd_line_par_results_struct *arg);

   //Passed a pointer to a table of function pointers, each of which
   //returns a character string, emit version control information
   //to the passed stream.  Should supply program name in all upper
   //case.
   DECMOD_CU_MSGS
   void CU_MSGS_emit_vcinfo_from_ptr_table(FILE *s,
                                           const char *(**ptrs)(void),
                                           const char *pname);

   DECMOD_CU_MSGS const char *CU_MSGS_cvcinfo(void);
   DECMOD_CU_MSGS const char *CU_MSGS_hvcinfo(void);
   #define CU_MSGS_H_VERSION ("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/cu_msgs.h 71 2016-11-05 11:07:06Z dashley $")
#endif

//End of cu_msgs.h.
