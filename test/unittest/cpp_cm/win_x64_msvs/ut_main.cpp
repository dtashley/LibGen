//--------------------------------------------------------------------------------------------------
//This file is part of LibGen, https://github.com/dtashley/LibGen, and is provided
//under The Unlicense, reproduced below.
//--------------------------------------------------------------------------------------------------
//Unit test of C++ formatted I/O functions.
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
//For more information, please refer to < https://unlicense.org>
//--------------------------------------------------------------------------------------------------
#include <iostream>

#include "lg_cpp_cm_fio.hpp"

int main(void)
{
   time_t ltime = 0;

   std::cout << "Execution begins.\n";
   
   for (uint64_t ui = 0; ui <= 1000; ui++)
   {
      std::cout << ui << "\n";
      LgCppCm_FioRepcharStream(std::cout, '=', ui);
      std::cout << "\n";
   }

   for (uint64_t ui = 0; ui <= 1000; ui++)
   {
      std::cout << ui << "\n";
      LgCppCm_FioRepchar('*', ui);
      std::cout << "\n";
   }

   LgCppCm_FioHlineStream(std::cout, 30);
   LgCppCm_FioHlineStream(std::cout);

   LgCppCm_FioHline(30);
   LgCppCm_FioHline();

   LgCppCm_FioBannerheadingStream(std::cout,
                                  "Hello, World!",
                                  78,
                                  2);
   LgCppCm_FioBannerheadingStream(std::cout,
                                  "Hello, World, Again!",
                                  78);
   LgCppCm_FioBannerheadingStream(std::cout,
                                  "Hello, World, x3!");
   LgCppCm_FioBannerheadingStream(std::cout,
                                  "Hello, World, Again x4!",
                                  100);
   LgCppCm_FioBannerheadingStream(std::cout,
                                  "Hello, World, Again x5!",
                                  100,
                                  4);

   LgCppCm_FioBannerheading("Hello, World!",
                            78,
                            2);
   LgCppCm_FioBannerheading("Hello, World, Again!",
                            78);
   LgCppCm_FioBannerheading("Hello, World, x3!");
   LgCppCm_FioBannerheading("Hello, World, Again x4!",
                            100);
   LgCppCm_FioBannerheading("Hello, World, Again x5!",
                            100,
                            4);
   LgCppCm_FioBannerheading("Hello, World, Again x6!",
                            100,
                            1);
   LgCppCm_FioBannerheading("Hello, World, Again x7!",
                            100,
                            0);
   LgCppCm_FioBannerheading("Hello, World, Again x8!",
                            110,
                            0);
   LgCppCm_FioBannerheading("Hello, World, Again x9!",
                            120,
                            5);
   LgCppCm_TimeStream(std::cout, &ltime, true);

   std::cout << "\n";

   LgCppCm_Time();

   std::cout << "\n";

   std::cout << "Execution ends.\n";
}