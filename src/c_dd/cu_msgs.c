//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/cu_msgs.c 71 2016-11-05 11:07:06Z dashley $
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
#define MODULE_CU_MSGS

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "cu_msgs.h"
#include "fcmiof.h"


#define CU_MSGS_TOOLSET_VERSION        "1.06.0000"


void CU_MSGS_too_few_args_msg(FILE *s, const char *pname)
   {
   assert(s != NULL);
   assert(pname != NULL);

   fprintf(s, "Too few command-line arguments.  Use \"%s -help\" for help.\n", pname);
   }


void CU_MSGS_toolset_info_msg(FILE *s, const char *pname)
   {
   int i;

   static char *p[] =
      {
      "This command-line tool is part of The ESRG Tool Set, Version " 
         CU_MSGS_TOOLSET_VERSION ",",
      "available for free download at http://ijutools.sourceforge.net.  This is an",
      "open-source product, licensed under the GNU Public License, and all source",
      "code and many additional tools and supplemental materials are also available",
      "for free download from the URL cited above.  Full documentation for this",
      "utility is maintained in the book (a work in progress) entitled \"A Practit-",
      "ioner's Guide To The Design And Development Of Small Microcontroller Soft-",
      "ware\", also available for free download both in .PDF format and as LaTeX",
      "source code from the URL cited above."
      };

   assert(s != NULL);
   assert(pname != NULL);

   for (i=0; i<sizeof(p)/sizeof(p[0]); i++)
      {
      fprintf(s, "%s\n", p[i]);
      }
   }


void CU_MSGS_std_options(FILE *s, const char *pname)
   {
   assert(s != NULL);
   assert(pname != NULL);

   printf("   -help\n");
   printf("      Displays the help message for this tool.\n");
   printf("   -verbose (-v)\n");
   printf("      Displays intermediate results and other useful information leading to\n");
   printf("      final result.\n");
   printf("   -noformat (-nf)\n");
   printf("      Suppresses attractive formatting of information.  This option is\n");
   printf("      useful if the output will be captured and supplied as input to another\n");
   printf("      program.\n");
   printf("   -debug\n");
   printf("      Displays information useful in debugging.\n");
   }


void CU_MSGS_cmd_line_par_results_struct_create(
        CU_MSGS_std_cmd_line_par_results_struct *arg)
   {
   assert(arg != NULL);

   memset(arg, 0, sizeof(CU_MSGS_std_cmd_line_par_results_struct));
   }


void CU_MSGS_cmd_line_par_results_struct_process_arg(
        CU_MSGS_std_cmd_line_par_results_struct *argblock,
        const char *arg,
        int *recognized)
   {
   assert(argblock != NULL);
   assert(arg != NULL);
   assert(recognized != NULL);

   *recognized = 1;

   if (!strcmp("-debug", arg))
      {
      argblock->debug = 1;
      }
   else if (!strcmp("-nf", arg) || !strcmp("-noformat", arg))
      {
      argblock->noformat = 1;
      }
   else if (!strcmp("-v", arg) || !strcmp("-verbose", arg))
      {
      argblock->verbose = 1;
      }
   else if (!strcmp("-help", arg) || !strcmp("-h", arg))
      {
      argblock->help = 1;
      }
   else
      {
      *recognized = 0;
      }
   }


void CU_MSGS_cmd_line_par_results_struct_finalize(
        CU_MSGS_std_cmd_line_par_results_struct *arg)
   {
   assert(arg != NULL);

   //-verbose and -debug cancel out -noformat.
   if (arg->verbose || arg->debug)
      arg->noformat = 0;
   }


void CU_MSGS_cmd_line_par_results_struct_destroy(
        CU_MSGS_std_cmd_line_par_results_struct *arg)
   {
   assert(arg != NULL);

   //Do nothing.  No destruction required.  Just left as a hook
   //in case dynamic allocation is ever required.
   }


void CU_MSGS_emit_vcinfo_from_ptr_table(FILE *s,
                                        const char *(**ptrs)(void),
                                        const char *pname)
   {
   const char *mptr;

   //Be sure the stream block is OK.
   assert(s != NULL);

   //Be sure the passed pointer to a pointer is not NULL.
   assert(ptrs != NULL);

   //Be sure the description is alright.
   assert(pname != NULL);

   //Print explanation.
   fprintf(s, "%s Version Control Manifest\n", pname);
   FCMIOF_stream_hline(s);

   //As long as the pointers are not NULL, call the function,
   //emit the information returned.

   while (*ptrs)
      {
      mptr = (**ptrs)();
      assert(mptr != NULL);
      fprintf(s, "%s\n", mptr);
      ptrs++;
      }
   }


const char *CU_MSGS_cvcinfo(void)
   {
   return("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/cu_msgs.c 71 2016-11-05 11:07:06Z dashley $");
   }


const char *CU_MSGS_hvcinfo(void)
   {
   return(CU_MSGS_H_VERSION);
   }

//End of cu_msgs.c.
