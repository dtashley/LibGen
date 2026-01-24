//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/char/charfunc.c 242 2018-08-04 18:25:51Z dashley $
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
#define MODULE_CHARFUNC

#include <assert.h>
#include <stddef.h>

#include "charfunc.h"


int CHARFUNC_digit_to_val(char digit)
   {
   switch (digit)
      {
      case '0':  return(0);
                 break;
      case '1':  return(1);
                 break;
      case '2':  return(2);
                 break;
      case '3':  return(3);
                 break;
      case '4':  return(4);
                 break;
      case '5':  return(5);
                 break;
      case '6':  return(6);
                 break;
      case '7':  return(7);
                 break;
      case '8':  return(8);
                 break;
      case '9':  return(9);
                 break;
      default:   return(-1);
                 break;
      }
   }


char CHARFUNC_nibble_to_lc_hex_digit(int nibble)
   {
   switch (nibble & 0x0F)
      {
      case  0:
         return('0');
         break;
      case  1:
         return('1');
         break;
      case  2:
         return('2');
         break;
      case  3:
         return('3');
         break;
      case  4:
         return('4');
         break;
      case  5:
         return('5');
         break;
      case  6:
         return('6');
         break;
      case  7:
         return('7');
         break;
      case  8:
         return('8');
         break;
      case  9:
         return('9');
         break;
      case 10:
         return('a');
         break;
      case 11:
         return('b');
         break;
      case 12:
         return('c');
         break;
      case 13:
         return('d');
         break;
      case 14:
         return('e');
         break;
      case 15:
         return('f');
         break;
      default:
         assert(0);
         return('?');
         break;
      }
   }


void CHARFUNC_int_to_lc_hex_rev(int arg, char *s)
   {
   int i;

   assert(s != NULL);

   for (i=0; i<8; i++)
      {
      s[i] = CHARFUNC_nibble_to_lc_hex_digit(arg);
      arg >>= 4;
      }
   }

const char *CHARFUNC_cvcinfo(void)
   {
   return("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/char/charfunc.c 242 2018-08-04 18:25:51Z dashley $");
   }


const char *CHARFUNC_hvcinfo(void)
   {
   return(CHARFUNC_H_VERSION);
   }

//End of charfunc.c.
