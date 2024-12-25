# LibGen

*LibGen* is a general utility library, written in C and C++,
intended primarily for personal
computer and server *Windows* and _*nix_ platforms.  (However, many modules
may work correctly on many embedded platforms.  Dynamic memory allocation
and stream I/O statements are the major factor that would prevent some of
these modules from working correctly on embedded platforms.)

Although some components of this library may be useful to the Internet
community at large, the primary role of this library is to allow me
(Dave Ashley) to remove common functionality from various small programs
I've written.

The mnemonic for the repository name is *Lib*rary, *Gen*eral.

## Licensing

_LibGen_ is provided under _The Unlicense_ (full text below).  The license places
no restrictions (other than the inabilty to litigate) on a user of the software.
The software may be used without restriction or obligation in embedded products.

<i>
    *This is free and unencumbered software released into the public domain.<br><br>
    Anyone is free to copy, modify, publish, use, compile, sell, or<br>
    distribute this software, either in source code form or as a compiled<br>
    binary, for any purpose, commercial or non-commercial, and by any<br>
    means.<br><br>
    In jurisdictions that recognize copyright laws, the author or authors<br>
    of this software dedicate any and all copyright interest in the<br>
    software to the public domain. We make this dedication for the benefit<br>
    of the public at large and to the detriment of our heirs and<br>
    successors. We intend this dedication to be an overt act of<br>
    relinquishment in perpetuity of all present and future rights to this<br>
    software under copyright law.<br><br>
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,<br>
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF<br>
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.<br>
    IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR<br>
    OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,<br>
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR<br>
    OTHER DEALINGS IN THE SOFTWARE.<br><br>
    For more information, please refer to &lt;https://unlicense.org&gt;
</i>
<br><br>


## Overview
*LibGen* is a collection of C and C++ modules that I tend to re-use across
various programs I've written.

I use "source file" and "module" interchangeably, because each
_.c_ or _.cpp_ file is meant to be compiled into a software module.

The source files in this repository are meant to be incorporated directly
into a _Visual Studio_ or other project, rather than being combined
into a proper library.

In order for the source files in this repository to be usable
in a wide variety of applications, certain design and documentation
rules have to be documented and followed.  Those
rules are described in the next two sections.

## Design Rules

### C Versus C++ File Naming Conventions
C files have a _.c_ extension, and the associated header files
have a _.h_ extension.  C++ files have a _.cpp_ extension, and
the associated header files have a _.hpp_ extension.  In all cases,
a header file has the same base name as the associated C or C++ source
file.

### File Name Uniqueness
No two source/header files, anywhere in this repository, have the same
base name.

### File Name, Function, Class, and Constant Prefixes
All files, function names, class names, and constants are prefixed
with _LBGN_, _LbGn_, *Lbgn_*, or something similar to make library
membership apparent, and to lower the probability of naming collisions
with other parts of the program in which the library is being
used.

### Threading Rules
All of the functions and classes in this module are thread-safe, unless
otherwise noted, subject to the following qualifications and restrictions:

* The caller/client is responsible for ensuring that the memory or objects
  on which the functions operate are used in a thread-safe way.
* Thread-safe versions of library functions (_printf()_, _malloc()_, etc.) must
  be linked into the program.

Thread safety generally requires that no state persists outside the variable or
object being operated on; or that if such state exists it is stored in
thread-local storage.

### C, C++ Versions Supported

The compiler language version support assumed for C code is C99.

The compiler language version support assumed for C++ is C++17.

### Verbosity Levels

| Level | Brief Description | *stdout* | *stderr* | Long Description           |
| :---  |     :---          | :---     |  :---    | :---                  |
| `0`  | Silent             | No output | No output | No *stdout* or *stderr* output.  All results through process exit code and generated files. |
| `1`  | Errors only, on *stderr* only. | No output | Errors only | No output to *stdout*.  Errors, if any, are described on *stderr*. |
| `1`  | Errors only, on both *stdout* and *stderr*. | No output | Errors only | No output to *stdout*.  Errors, if any, are described on *stderr*. |
| `2`  | Errors on stderr and stdout, major steps announced on stdout. |


### Command-Line Preprocessor Definitions

The code is parameterized by preprocessor definitions specified on the
command line.  To avoid as much confusion as possible, the switches:
* Are all prefixed with *LBGN_CLDF_*.
* Except for the prefix, are common with the switches for *LibNum*
  (https://github.com/dtashley/LibNum).
* All command-line define switches are optional. If switches in a given
  category are absent, a default choice is made (the default is unspecified
  in the documentation below).

Switches are divided into two categories:
* Switches that are either present or absent (i.e. are Boolean in nature).
* Switches that are enumerated in nature (must be specified, and must be one
  of a set of values).

## `LBGN_CLDF_PFORM_SW` (Mnemonic: *L*i*bG*e*n* *C*ommand-*L*ine *D*e*f*inition, *P*lat*form*, *S*oft*w*are)
Must be one of the following values:

| Constant                     | Interpretation |
| :---                         |     :---       |
| `LBGN_CLDF_PFORM_SW_WINAPI`  | Windows API, also sometimes called Win32, although this is a misnomer because 64-bit programs and 64-bit operating systems also use the Windows API. |
| `LBGN_CLDF_PFORM_SW_WINAPI_MFC`  | Windows API with MFC. |
| `LBGN_CLDF_PFORM_SW_WIN_NET` | Windows .NET. |
| `LBGN_CLDF_PFORM_SW_UNIX`    | Unix. |
| `LBGN_CLDF_PFORM_SW_LINUX`   | Linux. |
| `LBGN_CLDF_PFORM_SW_FREEBSD` | FreeBSD. |
| `LBGN_CLDF_PFORM_SW_CYGWIN`  | Cygwin. |
| `LBGN_CLDF_PFORM_SW_MSYS`    | MSYS. |
| `LBGN_CLDF_PFORM_SW_ANDROID` | Android. |
| `LBGN_CLDF_PFORM_SW_FIRE_OS` | FireOS. |
| `LBGN_CLDF_PFORM_SW_IOS`     | iOS. |
| `LBGN_CLDF_PFORM_SW_UNSPECIFIED` (default)  | Operating system not specified, system with none of the operating systems listed above, or system with no operating system. |

## `LBGN_CLDF_OS_ADR_SIZE` (Mnemonic: *L*i*bG*e*n* *C*ommand-*L*ine *D*e*f*inition, *O*perating *S*ystem *Ad*d*r*ess *Size*)
Must be one of the following values:

| Constant                     | Interpretation |
| :---                         |     :---       |
| `LBGN_CLDF_OS_ADR_SIZE_32`  | 32-bit operating system with 32-bit addresses. |
| `LBGN_CLDF_OS_ADR_SIZE_64`  | 64-bit operating system with 32-bit addresses. |
| `LBGN_CLDF_OS_ADR_SIZE_UNSPECIFIED` (default)  | Nature of operating system and address size unspecified. |

## `LBGN_CLDF_PFORM_ARCH` (Mnemonic: *L*i*bG*e*n* *C*ommand-*L*ine *D*e*f*inition, *P*lat*form* *Arch*itecture)
Must be one of the following values:

| Constant                     | Interpretation |
| :---                         |     :---       |
| `LBGN_CLDF_PFORM_ARCH_X86_32`       | x86 architecture, 32-bit. |
| `LBGN_CLDF_PFORM_ARCH_X86_64`       | x86 architecture, 64-bit. |
| `LBGN_CLDF_PFORM_ARCH_UNSPECIFIED` (default) | Architecture unspecified. |

## `LBGN_CLDF_PROJTYPE`
Must be one of the following values:

| Constant                     | Interpretation |
| :---                         |     :---       |
| `LBGN_CLDF_PROJTYPE_OBJ`           | Source files are being used to produce an object file. |
| `LBGN_CLDF_PROJTYPE_LIB`           | Source files are being used to produce a classic library. |
| `LBGN_CLDF_PROJTYPE_WINDLL`        | Source files are being used to produce Windows DLL. |
| `LBGN_CLDF_PROJTYPE_EXECUTABLE`           | Source files are being used to produce an executable program. |
| `LBGN_CLDF_PROJTYPE_UNSPECIFIED` (default)  | The type of project is not specified. |

## `LBGN_CLDF_ASSERTIONS`
Must be one of the following values:

| Constant                     | Interpretation |
| :---                         |     :---       |
| `LBGN_CLDF_ASSERTIONS_DISABLED` (default)     | Assertions are disabled. |
| `LBGN_CLDF_ASSERTIONS_ENABLED`               | Assertions are enabled. |

## `LBGN_CLDF_LOGGING`
Must be one of the following values:

| Constant                     | Interpretation |
| :---                         |     :---       |
| `LBGN_CLDF_LOGGING_DISABLED`              | Logging functionality should not be included. |
| `LBGN_CLDF_LOGGING_ENABLED`            | Logging functionality should be included. |

## `LBGN_CLDF_PROGTYPE`
Must be one of the following values:

| Constant                     | Interpretation |
| :---                         |     :---       |
| `LBGN_CLDF_PROGTYPE_CONSOLE`              | Program is a console-mode utility (text input, text output). |
| `LBGN_CLDF_PROGTYPE_WINGUI`               | Program is a graphical program under Windows. |
| `LBGN_CLDF_PROGTYPE_TCL_A_CONSOLE`        | Program is Tcl console-mode utility, using Tcl code ported by Dave Ashley around 2004. |
| `LBGN_CLDF_PROGTYPE_TCL_A_GUI`            | Program is Tcl/Tk graphical utility, using Tcl code ported by Dave Ashley around 2004. |
| `LBGN_CLDF_PROGTYPE_TCL_B_CONSOLE`        | Placeholder for future console port of Tcl. |
| `LBGN_CLDF_PROGTYPE_TCL_B_GUI`            | Placeholder for future graphical port of Tcl/Tk. |
| `LBGN_CLDF_PROGTYPE_CLIKE_A_CONSOLE`      | Placeholder for future console application involving _Clike_, a yet to be developed scripting language. |
| `LBGN_CLDF_PROGTYPE_CLIKE_A_GUI`          | Placeholder for future graphical port of _Clike_, a yet to be developed scripting language. |
| `LBGN_CLDF_PROGTYPE_UNIX_SWING`           | Program developed using Unix Swing. |
| `LBGN_CLDF_PROGTYPE_UNIX_AWT`             | Program developed using Unix AWT. |
| `LBGN_CLDF_PROGTYPE_CGIBIN_HELPER`        | Program is invoked by CGI-BIN PHP, Python, or Perl scripts, like to implement functionality awkward under the scripting language. |
| `LBGN_CLDF_PROGTYPE_CGIBIN_HTTPD`         | Program is a CGI-BIN program invoked directly by Apache to answer HTTP[S] requests. |
| `LBGN_CLDF_PROGTYPE_CGIBIN_SERVER`        | Program listens on TCP port(s) and is an actual HTTP[S] server. |
| `LBGN_CLDF_PROGTYPE_UNITTEST_COV`         | Program is a unit test program to test library components and/or measure testing statement/branch coverage. |
| `LBGN_CLDF_PROGTYPE_UNSPECIFIED`          | Program type is not specified. |

## `LIBNUM_CLSW_THREADS`
Must be one of the following values:

| Constant                     | Interpretation |
| :---                         |     :---       |
| `LIBNUM_CLSW_THREADS_1R`                    | The program runs with one thread and a greatly reduced priority (essentially, a background program). |
| `LIBNUM_CLSW_THREADS_1`                     | The program runs with 1 thread for a console-mode program, or 2 threads for a GUI program, at unmodified priority. |
| `LIBNUM_CLSW_THREADS_HALF_PHYS_CORES`       | The program runs with a number of threads that will not use more than one half of the physical cores available. |
| `LIBNUM_CLSW_THREADS_ALL_PHYS_CORES`        | The program runs with a number of threads that will not use more than all physical cores available. |
| `LIBNUM_CLSW_THREADS_ALL_LOGICAL_CORES`     | The program runs with a number of threads that will not use more than all the logical cores available. |
| `LIBNUM_CLSW_THREADS_PROG_SET`              | The number of threads and priority are set by the program, possibly in response to command-line switches, rather than at compile time. |
| `LIBNUM_CLSW_THREADS_UNSPECIFIED`           | The number of threads and priority are unspecified. |

## `LIBNUM_CLSW_CON_MODE_LINE_LEN`
Optional.  Unsigned integer.  Must be >=38 and <=1000.  The number of printable characters per line.  If not set, defaults to 78.

## `LIBNUM_CLSW_MALLOC_STRAT`
Must be one of the following values.  Applies to malloc() and realloc() only, not new and delete.

| Constant                     | Interpretation |
| :---                         |     :---       |
| `LIBNUM_CLSW_MALLOC_STRAT_C`                 | The standard C strategy is used.  malloc() and realloc() return NULL on allocation failure. |
| `LIBNUM_CLSW_MALLOC_STRAT_CPP_EXCEP`         | An exception is thrown when out of memory. |
| `LIBNUM_CLSW_MALLOC_STRAT_CPP_EXCEP_NOTIF`   | A reserve pool is released when out of memory.  The state can be polled.  When the reserve pool is exhausted, and exception is thrown. |

## Documentation Rules

### Doxygen is Used for Documentation
_Doxygen_ is used as the documentation tool for this library, and comments
in the source code are formatted accordingly.
