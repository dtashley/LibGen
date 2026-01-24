//--------------------------------------------------------------------------------------------------
//Header file for character array functions.
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
#ifndef LG_CPP_DD_CARF_HPP_INCLUDED
   #define LG_CPP_DD_CARF_HPP_INCLUDED

   #include "lg_cpp_dd_charf.hpp"

   extern bool LgCppDd_CarfIsUintWoCommas(const char* const in_arg) noexcept;
   extern bool LgCppDd_CarfIsSintWoCommas(const char* const in_arg) noexcept;
   extern bool LgCppDd_CarfIsUintWCommas(const char* const in_arg) noexcept;
   extern bool LgCppDd_CarfIsSintWCommas(const char* const in_arg) noexcept;
   extern void LgCppDd_CarfCstrReverse(char* const in_s) noexcept;
   extern void LgCppDd_CarfCstrCommanate(char* const in_s) noexcept;

#endif

//End of lg_cpp_dd_carf.hpp.
