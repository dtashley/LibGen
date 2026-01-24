//--------------------------------------------------------------------------------------------------
//Character array functions.
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
#define MODULE_LG_CPP_DD_CHARF

#include <assert.h>
#include <string.h>

#include "lg_cpp_dd_carf.hpp"


/*!
 * Examines a C string to determine if it is consistent with being an unsigned
 * integer of any length.
 *
 * \param[in]  in_arg            Pointer to a 0-terminated C string.  Pointing to an empty
 *                               string is permissible, but the <i>in_arg</i> pointer may
 *                               not be nullptr or otherwise invalid.
 * \returns                      true if the string is consistent with being an unsigned integer
 *                               of any size, or false otherwise.  Such a string would be "0",
 *                               or a sequence of digits not beginning with '0', and with no
 *                               non-digit characters.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe.
 * \errorsandexceptions          This function cannot generate exceptions that should be caught.
 *                               If the caller passes a nullptr pointer, the behavior of a
 *                               compiled program is unknown (will this be an exception that
 *                               might have been caught in the C++ exception-handling framework?).
 */
bool LgCppDd_CarfIsUintWoCommas(const char * const in_arg) noexcept
{
   const char* arg;
      //Copy of the input argument, modifiable.
   bool rv = true;
      //Value to be returned to the caller.  Assigned to true, so
      //that only the false case needs to be assigned when those
      //cases are detected.

   assert(in_arg != nullptr);
      //Input pointer may not be null.

   arg = in_arg;
      //Make modifiable copy of input argument.

   if (!*arg) //Empty string.
   {
      //Empty string.  Not an integer.
      rv = false;
   }
   else
   {
      if (arg[0] == '0')  //Leading '0'?
      {
         if (arg[1])  //String continues?
         {
            //Leading '0' either on an integer, or on a string that is not
            //an integer.  Either way, incorrectly formed.
            rv = false;
         }
         else
         {
            //'0' only in the string.  0 is a legitimate unsigned integer.
            //rv = true;
         }
      }
      else
      {
         while (*arg) //While not at the end of string.
         {
            if (!LgCppDd_CharfIsDigit(*arg))
            {
               //Not a digit.  Therefore not an unsigned integer.
               rv = false;  //Return false.
               break;       //Exit the while() loop, which also exits all
                            //the if()-else clauses.
            }
            arg++;
         }
      }
   }

   return rv;
}


/*!
 * Examines a C string to determine if it is consistent with being an signed
 * integer of any length.
 *
 * \param[in]  in_arg            Pointer to a 0-terminated C string.  Pointing to an empty
 *                               string is permissible, but the <i>in_arg</i> pointer may
 *                               not be nullptr or otherwise invalid.
 * \returns                      true if the string is consistent with being signed integer
 *                               of any size, or false otherwise.  Such a string would be "0";
 *                               or a sequence of digits not beginning with '0', and with no
 *                               non-digit characters; or a '-' followed by a sequence of digits
 *                               not beginning with '0', and with no non-digit characters.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe.
 * \errorsandexceptions          This function cannot generate exceptions that should be caught.
 *                               If the caller passes a nullptr pointer, the behavior of a
 *                               compiled program is unknown (will this be an exception that
 *                               might have been caught in the C++ exception-handling framework?).
 */
bool LgCppDd_CarfIsSintWoCommas(const char* const in_arg) noexcept
{
   const char* arg;
      //Modifiable pointer.
   bool rv = true;
      //Return value.  Default is true.  Only false cases need to be
      //detected.

   //Input pointer may not be nullptr.
   assert(in_arg != nullptr);

   //Copy to pointer which may be modified.
   arg = in_arg;

   if (!*arg)  //Empty string?
   {
      //Empty string is not a signed integer.
      rv = false;
   }
   else
   {
      if (*arg == '-')  //Leading '-'?
      {
         if (arg[1] == '0')
         {
            rv = false; //String beginning with "-0" can't be proper
                        //signed integer.
         }
         else
         {
            //If the leading character is '-', the rest must follow the
            //rules for an unsigned integer.
            rv = LgCppDd_CarfIsUintWoCommas(arg + 1);
         }
      }
      else
      {
         //If the leading character is not a '-', the string must follow
         //the rules for an unsigned integer.
         rv = LgCppDd_CarfIsUintWoCommas(arg);
      }
   }

   return rv;
}


/*!
 * Examines a C string to determine if it is consistent with being an unsigned
 * integer of any length, with commas inserted at the proper locations.
 *
 * \param[in]  in_arg            Pointer to a 0-terminated C string.  Pointing to an empty
 *                               string is permissible, but the <i>in_arg</i> pointer may
 *                               not be nullptr or otherwise invalid.
 * \returns                      true if the string is consistent with being an unsigned integer
 *                               of any length with commas properly inserted, or false otherwise.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe.
 * \errorsandexceptions          This function cannot generate exceptions that should be caught.
 *                               If the caller passes a nullptr pointer, the behavior of a
 *                               compiled program is unknown (will this be an exception that
 *                               might have been caught in the C++ exception-handling framework?).
 */
bool LgCppDd_CarfIsUintWCommas(const char* const in_arg) noexcept
{
   //TODO:  Substantial change in porting.  Need to review and document logic.
   //       Need to especially document states.
   //
   const char* arg;
      //Modifiable copy of input pointer.
   bool rv = true;
      //Return value.  Default to true.  Only negative cases need to make assignment.
   int state;
   bool exit_flag;

   //Input pointer cannot be NULL.
   assert(in_arg != NULL);

   //Copy to modifiable pointer.
   arg = in_arg;

   //Can't be empty string.
   if (!*arg)
   {
      //Empty string.  Not an unsigned integer with commas.
      rv = false;
   }
   else
   {
      //If the first digit is 0, that must be the only digit.
      if (arg[0] == '0')
      {
         if (arg[1])
         {
            //'0' not the only digit.  Not an unsigned integer with commas.
            rv = false;
         }
         else
         {
            //rv = true;
         }
      }
      else
      {
         if (!LgCppDd_CharfIsDigit(*arg))
         {
            //First character not a digit.
            rv = false;
         }
         else
         {
            state = 0;
            exit_flag = false;
            arg++;

            while (!exit_flag)
            {
               switch (state)
               {
                  case 0: if (!*arg)
                          {
                             //rv        = true;
                             exit_flag = true;
                          }
                          else if (*arg == ',')
                          {
                             state = 3;
                          }
                          else if (LgCppDd_CharfIsDigit(*arg))
                          {
                             state = 1;
                          }
                          else
                          {
                             rv        = false;
                             exit_flag = true;
                          }
                          break;
                  case 1: if (!*arg)
                          {
                             //rv        = true;
                             exit_flag = true;
                          }
                          else if (*arg == ',')
                          {
                             state = 3;
                          }
                          else if ((*arg >= '0') && (*arg <= '9'))
                          {
                             state = 2;
                          }
                          else
                          {
                             rv        = false;
                             exit_flag = true;
                          }
                          break;
                  case 2: if (!*arg)
                          {
                             //rv        = true;
                             exit_flag = true;
                          }
                          else if (*arg == ',')
                          {
                             state = 3;
                          }
                          else if (LgCppDd_CharfIsDigit(*arg))
                          {
                             rv        = false;
                             exit_flag = true;
                          }
                          else
                          {
                             rv        = false;
                             exit_flag = true;
                          }
                          break;
                  case 3: if (!*arg)
                          {
                             rv        = false;
                             exit_flag = true;
                          }
                          else if (*arg == ',')
                          {
                             rv        = false;
                             exit_flag = true;
                          }
                          else if (LgCppDd_CharfIsDigit(*arg))
                          {
                             state = 4;
                          }
                          else
                          {
                             rv        = false;
                             exit_flag = true;
                          }
                          break;
                  case 4: if (!*arg)
                          {
                             rv        = false;
                             exit_flag = true;
                          }
                          else if (*arg == ',')
                          {
                             rv        = false;
                             exit_flag = true;
                          }
                          else if (LgCppDd_CharfIsDigit(*arg))
                          {
                             state = 5;
                          }
                          else
                          {
                             rv        = false;
                             exit_flag = true;
                          }
                          break;
                  case 5: if (!*arg)
                          {
                             rv        = false;
                             exit_flag = true;
                          }
                          else if (*arg == ',')
                          {
                             rv        = false;
                             exit_flag = true;
                          }
                          else if (LgCppDd_CharfIsDigit(*arg))
                          {
                             state = 6;
                          }
                          else
                          {
                             rv        = false;
                             exit_flag = true;
                          }
                          break;
                  case 6: if (!*arg)
                          {
                             //rv        = true;
                             exit_flag = true;
                          }
                          else if (*arg == ',')
                          {
                             state = 3;
                          }
                          else if (LgCppDd_CharfIsDigit(*arg))
                          {
                             rv        = false;
                             exit_flag = true;
                          }
                          else
                          {
                             rv        = false;
                             exit_flag = true;
                          }
                          break;
                  default:
                          assert(false);
                          break;
               }  //End switch().

               arg++;
            } //End while().
         }
      }
   }

   return rv;
}  //End function.


/*!
 * Examines a C string to determine if it is consistent with being a signed
 * integer of any length, with commas inserted at the proper locations.
 *
 * \param[in]  in_arg            Pointer to a 0-terminated C string.  Pointing to an empty
 *                               string is permissible, but the <i>in_arg</i> pointer may
 *                               not be nullptr or otherwise invalid.
 * \returns                      true if the string is consistent with being an unsigned integer
 *                               of any length with commas properly inserted, or false otherwise.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe.
 * \errorsandexceptions          This function cannot generate exceptions that should be caught.
 *                               If the caller passes a nullptr pointer, the behavior of a
 *                               compiled program is unknown (will this be an exception that
 *                               might have been caught in the C++ exception-handling framework?).
 */
bool LgCppDd_CarfIsSintWCommas(const char* const in_arg) noexcept
{
   bool rv = true;
   const char* arg;

   //Eyeball the input parameter.
   assert(in_arg != nullptr);

   //Copy to modifiable parameter.
   arg = in_arg;

   if (!*arg)  //Empty string ain't an integer.
   {
      rv = false;
   }
   else
   {
      if (*arg == '-')
      {
         if (arg[1] == '0')
         {
            rv = false;
         }
         else
         {
            rv = LgCppDd_CarfIsUintWCommas(arg + 1);
         }
      }
      else
      {
         rv = LgCppDd_CarfIsUintWCommas(arg);
      }
   }

   return rv;
}


/*!
 * Reverses a C string in place (the first character becomes the last, the last the first, etc.).
 *
 * \param[in]  in_arg            Pointer to a 0-terminated C string.  Pointing to an empty
 *                               string is permissible, but the <i>in_arg</i> pointer may
 *                               not be nullptr or otherwise invalid.
 * \returns                      true if the string is consistent with being an unsigned integer
 *                               of any length with commas properly inserted, or false otherwise.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe.
 * \errorsandexceptions          This function cannot generate exceptions that should be caught.
 *                               If the caller passes a nullptr pointer, the behavior of a
 *                               compiled program is unknown (will this be an exception that
 *                               might have been caught in the C++ exception-handling framework?).
 */
void LgCppDd_CarfCstrReverse(char * const in_s) noexcept
{
   size_t l, begin, end;
   char* s;
   char temp;

   assert(in_s != NULL);
   s = in_s;
   l = strlen(s);

   if (l > 1) //Can't meaningfully reverse 0-length or single-character string.
   {
      begin = 0;
      end = l - 1;

      while (begin < end)
      {
         temp = s[begin];
         s[begin] = s[end];
         s[end] = temp;
         begin++;
         end--;
      }
   }
}


#if 0
void BSTRFUNC_parse_gen_sci_not_num(const char *s,
                                    int *failure,
                                    char *mant_sign,
                                    size_t *mant_bdp,
                                    size_t *mant_bdp_len,
                                    size_t *mant_adp,
                                    size_t *mant_adp_len,
                                    char *exp_sign,
                                    size_t *exp,
                                    size_t *exp_len)
   {
   //For the parameters that can be NULL, these are the
   //local copies we keep.  We will assign these back to
   //the caller's area when appropriate before we leave.
   char i_mant_sign, i_exp_sign;
   size_t i_mant_bdp, i_mant_bdp_len, i_mant_adp, i_mant_adp_len,
          i_exp, i_exp_len;
   size_t i;
   int state;
   const char *arg;

   //Check out the parameters that can't be NULL.
   assert(s != NULL);
   assert(failure != NULL);

   //Initialize all variables to defaults.
   *failure           =  0 ;
   i_mant_sign        = 'N';
   i_exp_sign         = 'N';
   i_mant_bdp         =  0 ;
   i_mant_bdp_len     =  0 ;
   i_mant_adp         =  0 ;
   i_mant_adp_len     =  0 ;
   i_exp              =  0 ;
   i_exp_len          =  0 ;
   i                  =  0 ;
   state              =  0 ;

   //Process the string.  This is a big 10-state state
   //machine.  Because of the complexity, I've included
   //some state definitions as well in the comments.
   arg = s;

   while(1)
      {
      switch (state)
         {
         //Initial state from which we start processing.
         case 0 : if (!*arg)
                     {
                     //The string is empty.  This is not OK.
                     //Error out.
                     *failure = 1;
                     goto term_seq;
                     }
                  else if (*arg == '-')
                     {
                     //Leading - on the number.  Fine.
                     i_mant_sign = '-';
                     state = 1;
                     }
                  else if (*arg == '+')
                     {
                     //Leading + on the number.  Fine.
                     i_mant_sign = '+';
                     state = 2;
                     }
                  else if (*arg == '.')
                     {
                     //Leading . on the number.  Fine.
                     state = 5;
                     }
                  else if (*arg == '0')
                     {
                     i_mant_bdp = arg - s;
                     state = 4;
                     }
                  else if ((*arg >= '1') && (*arg <= '9'))
                     {
                     i_mant_bdp = arg - s;
                     state = 3;
                     }
                  else
                     {
                     //Unidentifiable crap.  This is not OK.
                     //Error out.
                     *failure = 1;
                     goto term_seq;
                     }
                  break;
         //The leading character of the string was '-'.  We
         //are processing a negative number.
         case 1 : if (!*arg)
                     {
                     //The string is empty.  This is not OK because
                     //it means the string was "-".
                     //Error out.
                     *failure = 1;
                     goto term_seq;
                     }
                  else if (*arg == '.')
                     {
                     //We have a - followed by a decimal point.
                     //This is impolite form, but will accept
                     //it as a number.
                     state = 5;
                     }
                  else if (*arg == '0')
                     {
                     //We have a '-' followed by a '0'.
                     //Seems reasonable.
                     i_mant_bdp = arg - s;
                     state = 4;
                     }
                  else if ((*arg >= '1') && (*arg <= '9'))
                     {
                     //We have a '-' followed by a non-zero
                     //digit.  Seems reasonable.
                     i_mant_bdp = arg - s;
                     state = 3;
                     }
                  else
                     {
                     //Unidentifiable crap.  This is not OK.
                     //Error out.
                     *failure = 1;
                     goto term_seq;
                     }
                  break;
         //The leading character of the string was '+'.  We
         //are processing an explicitly declared positive number.
         case 2 : if (!*arg)
                     {
                     //The string is empty.  This is not OK because
                     //it means the string was "+".
                     //Error out.
                     *failure = 1;
                     goto term_seq;
                     }
                  else if (*arg == '.')
                     {
                     //We have a + followed by a decimal point.
                     //This is impolite form, but will accept
                     //it as a number.
                     state = 5;
                     }
                  else if (*arg == '0')
                     {
                     //We have a '-' followed by a '0'.
                     //Seems reasonable.
                     i_mant_bdp = arg - s;
                     state = 4;
                     }
                  else if ((*arg >= '1') && (*arg <= '9'))
                     {
                     //We have a '+' followed by a non-zero
                     //digit.  Seems reasonable.
                     i_mant_bdp = arg - s;
                     state = 3;
                     }
                  else
                     {
                     //Unidentifiable crap.  This is not OK.
                     //Error out.
                     *failure = 1;
                     goto term_seq;
                     }
                  break;
         //We've encountered a non-zero digit either first as part
         //of the string or later after + or -.  We are processing
         //a mantissa that does not begin with "0".
         case 3 : if (!*arg)
                     {
                     //We've reached the end of the string.  This is 
                     //fine because we have a simple integer of the
                     //form NNNN, +NNNN, or -NNNN.  No problem with that.
                     i_mant_bdp_len = (arg - s) - i_mant_bdp;
                     *failure = 0;
                     goto term_seq;
                     }
                  else if (*arg == '.')
                     {
                     //Number followed by decimal point.  This
                     //means the mantissa is terminating.  No issue.
                     i_mant_bdp_len = (arg - s) - i_mant_bdp;
                     state = 5;
                     }
                  else if ((*arg >= '0') && (*arg <= '9'))
                     {
                     //Yet more digits.  Seems reasonable.
                     //The assignment isn't necessary, but it
                     //is aesthetic.
                     state = 3;
                     }
                  else if ((*arg == 'e') || (*arg == 'E'))
                     {
                     //Beginning of an exponent.  This again
                     //means the mantissa is terminating.  No issue.
                     i_mant_bdp_len = (arg - s) - i_mant_bdp;
                     state = 6;
                     }
                  else
                     {
                     //Unidentifiable crap.  This is not OK.
                     //Error out.
                     *failure = 1;
                     goto term_seq;
                     }
                  break;
         //We're processing a mantissa that begins with zero.  There
         //is only one zero allowed before the decimal point.
         case 4 : if (!*arg)
                     {
                     //We've reached the end of the string.  This is 
                     //fine because it means we have zero.  I'm going
                     //to be unusually permissive and allow -0 and +0,
                     //which isn't traditional.
                     i_mant_bdp_len = (arg - s) - i_mant_bdp;
                     *failure = 0;
                     goto term_seq;
                     }
                  else if (*arg == '.')
                     {
                     //Zero followed by decimal point.  This
                     //means the mantissa is terminating.  No issue.
                     i_mant_bdp_len = (arg - s) - i_mant_bdp;
                     state = 5;
                     }
                  else if ((*arg == 'e') || (*arg == 'E'))
                     {
                     //Beginning of an exponent.  This again
                     //means the mantissa is terminating.  No issue.
                     i_mant_bdp_len = (arg - s) - i_mant_bdp;
                     state = 6;
                     }
                  else
                     {
                     //Unidentifiable crap.  This is not OK.
                     //Error out.
                     *failure = 1;
                     goto term_seq;
                     }
                  break;
                  //A decimal point has been found in the string.
                  //This marks the end of the mantissa, for sure.
         case 5 : if (!*arg)
                     {
                     //We've reached the end of the string.  There are
                     //two possibilities here.  Either there have been no
                     //digits before the decimal points, which is a no-no,
                     //or there have been digits, which is fine.
                     if (i_mant_bdp_len)
                        {
                        *failure = 0;
                        goto term_seq;
                        }
                     else
                        {
                        *failure = 1;
                        goto term_seq;
                        }
                     }
                  else if ((*arg >= '0') && (*arg <= '9'))
                     {
                     //Digits after the decimal point.  Need to
                     //begin the portion after the decimal point.
                     i_mant_adp = arg - s;
                     state = 10;
                     }
                  else if ((*arg == 'e') || (*arg == 'E'))
                     {
                     //Beginning of an exponent.  This means
                     //that have no digits after the decimal point. 
                     //This is OK.
                     state = 6;
                     }
                  else
                     {
                     //Unidentifiable crap.  This is not OK.
                     //Error out.
                     *failure = 1;
                     goto term_seq;
                     }
                  break;
                 //The exponent has begin.
         case 6: if (!*arg)
                     {
                     //The string is empty.  This is not OK
                     //because it would mean an E with nothing
                     //following it.  This is a no-no.
                     //Error out.
                     *failure = 1;
                     goto term_seq;
                     }
                  else if (*arg == '-')
                     {
                     //Leading - on the exponent.  Fine.
                     i_exp_sign = '-';
                     state = 8;
                     }
                  else if (*arg == '+')
                     {
                     //Leading + on the exponent.  Fine.
                     i_exp_sign = '+';
                     state = 7;
                     }
                  else if ((*arg >= '0') && (*arg <= '9'))
                     {
                     //Beginning the digits of the exponent.  Fine.
                     i_exp = arg - s;
                     state = 9;
                     }
                  else
                     {
                     //Unidentifiable crap.  This is not OK.
                     //Error out.
                     *failure = 1;
                     goto term_seq;
                     }
                  break;

                  //Had a + sign before the exponent.  Now expecting digits.
         case  7: if (!*arg)
                     {
                     //The string is empty.  This is not OK
                     //because it would mean an E+ with nothing
                     //following it.  This is a no-no.
                     //Error out.
                     *failure = 1;
                     goto term_seq;
                     }
                  else if ((*arg >= '0') && (*arg <= '9'))
                     {
                     //Beginning the digits of the exponent.  Fine.
                     i_exp = arg - s;
                     state = 9;
                     }
                  else
                     {
                     //Unidentifiable crap.  This is not OK.
                     //Error out.
                     *failure = 1;
                     goto term_seq;
                     }
                  break;
                  //Had a - sign before the exponent.  Now expecting digits.
         case  8: if (!*arg)
                     {
                     //The string is empty.  This is not OK
                     //because it would mean an E- with nothing
                     //following it.  This is a no-no.
                     //Error out.
                     *failure = 1;
                     goto term_seq;
                     }
                  else if ((*arg >= '0') && (*arg <= '9'))
                     {
                     //Beginning the digits of the exponent.  Fine.
                     i_exp = arg - s;
                     state = 9;
                     }
                  else
                     {
                     //Unidentifiable crap.  This is not OK.
                     //Error out.
                     *failure = 1;
                     goto term_seq;
                     }
                  break;
                  //Had one digit of exponent.  Am processing remaining ones.
         case 9 : if (!*arg)
                     {
                     //We've reached the end of the string.  This is 
                     //fine because we have an exponent of the
                     //form NNNN, +NNNN, or -NNNN.  No problem with that.
                     i_exp_len = (arg - s) - i_exp;
                     *failure = 0;
                     goto term_seq;
                     }
                  else if ((*arg >= '0') && (*arg <= '9'))
                     {
                     //Yet more digits.  Seems reasonable.
                     //The assignment isn't necessary, but it
                     //is aesthetic.
                     state = 9;
                     }
                  else
                     {
                     //Unidentifiable crap.  This is not OK.
                     //Error out.
                     *failure = 1;
                     goto term_seq;
                     }
                  break;
                  //Processing digits after decimal point.  This state is out
                  //of sequence because added it after on diagram.
         case 10: if (!*arg)
                     {
                     //We've reached the end of the string.  This is 
                     //fine.
                     i_mant_adp_len = (arg - s) - i_mant_adp;
                     *failure = 0;
                     goto term_seq;
                     }
                  else if ((*arg >= '0') && (*arg <= '9'))
                     {
                     //Yet more digits.  Seems reasonable.
                     //The assignment isn't necessary, but it
                     //is aesthetic.
                     state = 10;
                     }
                  else if ((*arg == 'e') || (*arg == 'E'))
                     {
                     //Beginning of an exponent.  This terminates
                     //the digits after the decimal point.
                     i_mant_adp_len = (arg - s) - i_mant_adp;
                     state = 6;
                     }
                  else
                     {
                     //Unidentifiable crap.  This is not OK.
                     //Error out.
                     *failure = 1;
                     goto term_seq;
                     }
                  break;
         default:
                  abort();
                  break;
         }

      arg++;
      }


   //This is the setup for returning to the caller.  We can
   //only fill in those data pointer for the caller that are
   //not NULL.
   term_seq:
   if (mant_sign)
      *mant_sign = i_mant_sign;
   if (mant_bdp)
      *mant_bdp = i_mant_bdp;
   if (mant_bdp_len)
      *mant_bdp_len = i_mant_bdp_len;
   if (mant_adp)
      *mant_adp = i_mant_adp;
   if (mant_adp_len)
      *mant_adp_len = i_mant_adp_len;
   if (exp_sign)
      *exp_sign = i_exp_sign;
   if (exp)
      *exp = i_exp;
   if (exp_len)
      *exp_len = i_exp_len;
   }
#endif


/*!
 * Adds commas to a numeric string representing an unsigned or signed integer.  The space must exist
 * in the area pointed to.
 *
 * \param[in]  in_arg            Pointer to a 0-terminated C string.  Pointing to an empty
 *                               string is permissible, but the <i>in_arg</i> pointer may
 *                               not be nullptr or otherwise invalid.
 * \returns                      true if the string is consistent with being an unsigned integer
 *                               of any length with commas properly inserted, or false otherwise.
 * \reentrancyandthreadsafety    This function is re-entrant and thread safe.
 * \errorsandexceptions          This function cannot generate exceptions that should be caught.
 *                               If the caller passes a nullptr pointer, the behavior of a
 *                               compiled program is unknown (will this be an exception that
 *                               might have been caught in the C++ exception-handling framework?).
 */
void LgCppDd_CarfCstrCommanate(char* const in_s) noexcept
{
   size_t l;
   size_t ncommas;
   char *putpt, *getpt;
   char* s;
   size_t ndigits;

   assert(in_s);
   s = in_s;

   //If the leading character on the string is a 
   //'-', bump the pointer.  Then everything
   //else applies as for an unsigned.  Empty string does not pass this test
   //and so pointer not incremented.
   if (*s == '-')
     s++;

   //Be sure the string currently meets the syntax for
   //a signed integer.  If not, don't even touch it.
   if (!LgCppDd_CarfIsUintWoCommas(s))
      return;

   //Get the length of the current string.
   l = strlen(s);

   //Let's agree, just in case something slipped through
   //the cracks, that zero length strings are not of
   //interest to us.
   if (l==0)
     return;

   //The number of commas to add is precisely 
   //(N-1) div 3.
   if (l==0)
      ncommas = 0;
   else
      ncommas = (l-1)/3;

   //Walk through the string, adding commas.
   getpt = s + l - 1;
   putpt = s + l + ncommas;

   *putpt = 0; //Write the zero terminator.
   putpt--;

   ndigits = 0;

   while ((putpt > s) && (getpt > s))
      {
      *putpt = *getpt;
      putpt--;
      getpt--;
      ndigits++;
      if (((ndigits % 3) == 0) && (putpt != s))
         {
         *putpt = ',';
         putpt--;
         }
      assert((putpt >= s) && (getpt>=s));
      }
}


#if 0
//07/28/01:  Visual inspection only.
void BSTRFUNC_decommanate(char *s)
   {
   char *src, *dst;

   assert(s != NULL);

   src = dst = s;

   while(1)
      {
      if (!*src)
         {
         *dst = 0;
         break;
         }
      else if (*src == ',')
         {
         src++;
         }
      else
         {
         *dst = *src;
         src++;
         dst++;
         }
      }
   }
#endif


#if 0
void BSTRFUNC_parse_str_to_uint32(const char *s, 
                                  unsigned int *rv, 
                                  int *error)
   {
   unsigned _int64 temp;
   int digval;

   //Eyeball the input arguments.
   assert(s != NULL);
   assert(error != NULL);

   //Start off believing there is no error.
   *error = 0;

   //The string has to parse out as an unsigned integer or an
   //unsigned integer with commas, or we won't touch it.
   if (!BSTRFUNC_is_uint_wo_commas(s) && !BSTRFUNC_is_uint_w_commas(s))
      {
      *error = 1;
      return;
      }

   //OK, we know we have an unsigned integer, either without commas,
   //or with properly placed commas.  Our only worry now is that it
   //is of a size that will fit in 32 bits.  The way we can deal with
   //this is to format it into 64 bits and watch for overflow.
   temp = 0;

   while (*s)
      {
      digval = CHARFUNC_digit_to_val(*s);
      if (digval >= 0)
         {
         temp *= 10;
         temp += digval;
         if ((temp >> 32) & 0xFFFFFFFF)
            {
            //We've had an overflow.
            *error = 1;
            return;
            }
         }
      s++;
      }

   *rv = (unsigned int)temp;
   }


const char *BSTRFUNC_cvcinfo(void)
   {
   return("$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_datd/string_c/bstrfunc.c 242 2018-08-04 18:25:51Z dashley $");
   }


const char *BSTRFUNC_hvcinfo(void)
   {
   return(BSTRFUNC_H_VERSION);
   }
#endif

//End of bstrfunc.c.
