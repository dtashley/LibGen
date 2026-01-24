# LibGen

*LibGen* is a general non-numeric utility library, written in C and C++,
intended primarily for personal
computer and server *Windows* and _*nix_ platforms.

The mnemonic for the repository name is *Lib*rary, *Gen*eral.

*LibGen* described here is not related to
[*Library Genesis*](https://en.wikipedia.org/wiki/Library_Genesis).
The similarity in naming is accidental.

*LibGen* is intended to be used in source
code form, where the source code of *LibGen* is added as
necessary to other projects.  (This differs from the
the classic definition of *library* in computing, where
object modules are combined together into a library searched by
a linker.)

*LibGen* is one of three repositories I've authored containing reusable code that can
be incorporated into other projects.  These three repositories are:

* [*EMTS*](https://github.com/dtashley/Emts): a framework for
toolsets, intended to be used in building toolsets to support embedded
system development.

* [*LibGen*](https://github.com/dtashley/LibGen) (this repository):
a non-numeric library, primarily written in C and C++.

* [*LibNum*](https://github.com/dtashley/LibNum):
a numeric library, written in C, C++, and assembly language.

## Licensing

_LibGen_ is provided under _The Unlicense_ (full text below).  The license places
no restrictions (other than inabilty to litigate) on a user of the software.
The software may be used without restriction or obligation in embedded products.

*This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.*
    
*In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.*

*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.*

*For more information, please refer to &lt;https://unlicense.org&gt;*

## Design Rules

### C Versus C++ File Naming Conventions

C files have a _.c_ extension, and the associated header files
have a _.h_ extension.  C++ files have a _.cpp_ extension, and
the associated header files have a _.hpp_ extension.  In all cases,
a header file has the same base name as the associated C or C++ source
file.

### File Name Uniqueness

No two source/header files, anywhere in this repository, regardless
of language (C versus C++) may have the same base name.  Because the
base file names are guaranteed unique, the source/header file names
are also guaranteed unique.  Name collisions within *LibGen*
are not possible.

*EMTS* and *LibNum*, being essentially companions to *LibGen*, have no base file
names in common with *LibGen*.  Source file name collisons between the
three repositories are not possible.

### File Name, Function, Class, and Constant Prefixes

All source file names, free function names, class names, and constants are prefixed
with _Lg_ or _Lbgn_ (in various casing styles) to make library
membership apparent, and to lower the probability of naming collisions
with other components of a program in which the library is being
used.

For *EMTS*, the preferred prefixes are _Et_ and _Emts_.

For *LibNum*, the preferred prefixes are  _Ln_ and _Lbnm_.

### Threading Rules

Functions and classes in this library are documented as much as possible
for their thread safety.  When a function or class is described as
*thread safe*, these qualifications apply:

* The caller/client is responsible for ensuring that the memory or objects
  on which the functions operate are not used by multiple threads
  in a thread-unsafe way.
  * For example, if a function described as thread-safe writes a buffer
    that is passed by reference, if the *same* buffer is manipulated
    from multiple threads, the function may not
    work as intended.  The caller/client is responsible for this aspect
    of thread safety.
* A description of thread safety assumes that thread-safe versions
  of library functions (_printf()_, _malloc()_, etc.) are linked into
  the program.
* The assumption is made by *LibGen* that non-static local variables
  (typically stored on the stack) are automatically thread-local.
* No decisions have been made about other types of thread-local storage
  (besides the call stack and variables on the stack).

### C, C++ Language Standards Supported

The compiler language version support assumed for C code is C99.

The compiler language version support assumed for C++ is C++17.

### Use of Preprocessor Definitions ###

The code is parameterized by preprocessor definitions included in the makefile, IDE,
batch file, shell script, etc.
* All *LibGen* preprocessor definitons are prefixed with *LBGN_CLDF_*.
  (*CLDF* is mnemonic for *c*ommand-*l*ine *d*e*f*inition).
* All command-line define definitions are optional. If definitions in a given
  category are absent, a default choice is made (the default is specified
  in the documentation below).

Definitions are divided into four categories:
* Switches that are either present or absent and aren't given a value.
  * Always tested using ```#ifdef```, ```#ifndef```, or ```defined()```.
  * Effectively Boolean in nature.
* Switches that are integers and intended to be Boolean.
  * May be defined using ```FALSE```, ```TRUE```, ```0```, or ```1```.
  * Are always tested using ```#if SYMBOL```, ```#if SYMBOL==0```,
    or ```if SYMBOL!=0```.
* Switches that are enumerated in nature (one of a set of named values).
  * Are always tested against the named value using ```#if SYMBOL==NAMED_VALUE```,
    typically in an ```#if```-```#elif```-```#else```-```#endif``` structure.
* Switches that define integer, floating point, or string constants.

All translation units in the program being built must be
compiled/assembled using the same preprocessor switches
(this is a standard best practice).  In the case of
*LibGen* / *EMTS* / *LibNum*,
compiling/assembling different translation units with
different switches may result in undefined behavior.

### Verbosity Levels

Most programs incorporating this library have an optional
verbosity level, typically specified with a *-v* option.  The verbosity
levels, 0-9, as known to the library, are described below.  It is
anticipated that all programs using the library will use the
same convention for verbosity level.

The conventions for verbosity level are:
* The program incorporating this library will decide on a verbosity
  level, perhaps using a simple default, or perhaps considering
  command-line options.
* This library will be the reservoir of verbosity for any program
  incorporating this library.  Programs using the library should call
  the *LgVerbosity_VerbositySet()* function as soon as the verbosity level
  is decided (often by command-line parameters).
* The verbosity level will be determined and communicated to *LibGen* before
  multithreaded operation starts, and the verbosity level will not
  be changed thereafter, so that there are no threading issues.
* Verbosity level and the resulting behavior must be consistent with
  the table below.
* The default verbosity level is 3.  This is the value that will be used
  if no verbosity is specified by calling the library (or if verbosity is
  specified as 3 by calling the library).
* It won't be economical to incorporate all output-generating code in
  all programs, as this might result in large executables.  For this
  reason, the `LBGN_CLDF_VERBOSITY_SUPPORT_MAX` preprocessor directive
  is specified.
  * `LBGN_CLDF_VERBOSITY_SUPPORT_MAX` is a preprocessor constant that defines
    the highest verbosity that the program must accommodate.
  * `LBGN_CLDF_VERBOSITY_SUPPORT_MAX` must be in the range of
     [3, 9].  (It is assumed that all programs will support at least
     verbosity level 3.)
  * A verbosity specified at runtime that is larger than
    `LBGN_CLDF_VERBOSITY_SUPPORT_MAX` won't create
    more output than a verbosity equal to `LBGN_CLDF_VERBOSITY_SUPPORT_MAX` (because
    the program as built won't contain the code necessary to be more verbose than
    `LBGN_CLDF_VERBOSITY_SUPPORT_MAX`).
  * `LBGN_CLDF_VERBOSITY_SUPPORT_MAX` should be tested only for output-generating
    statements involving verbosity of 4 or higher, as it is expected that
    all programs will support verbosity levels 0 through 3.

It is anticipated that `LBGN_CLDF_VERBOSITY_SUPPORT_MAX` will appear in source code
in the following form, using verbosity level 5 as an example:

```
#if (defined(LBGN_CLDF_VERBOSITY_SUPPORT_MAX)) && (LBGN_CLDF_VERBOSITY_SUPPORT_MAX >= 5)
  if (verbosity >= 5)
  {
    //Output statement(s) for verbosity level 5 go here.
  }
#endif
```

The meanings of the values of verbosity are described below.

| Level | Brief Description | *stdout* | *stderr* | Long Description           |
| :---  |     :---          | :---     |  :---    | :---                  |
| `0`  | Silent             | No output. | No output. | No *stdout* or *stderr* output.  All results through process exit code and generated files. |
| `1`  | Terse error descriptions only, announced on *stderr* only. | No output. | Terse errors only. | No output to *stdout*.  Errors, if any, are described in a terse form on *stderr*. |
| `2`  | Detailed error descriptions, announced on *stderr* only. | No output. | Detailed errors only. | No output to *stdout*.  Errors, if any, are described in a detailed form on *stderr*. |
| `3`  (default) | Major steps announced on *stdout*.  Detailed errors announced on *stderr*. | Major steps. | Detailed errors. | Major steps announced on *stdout*.  Detailed errors announced on *stderr*. |
| `4`   | Major and minor steps announced on *stdout*. Detailed errors announced on *stderr*. | Major and minor steps. | Detailed errors. | Major and minor steps announced on *stdout*. Detailed errors announced on *stderr*. |
| `5`   | Previous levels' output plus algorithm steps and notes announced on *stdout*.  Detailed errors announced on *stderr*. | Previous levels' output plus algorithm steps and notes. | Detailed errors. | Previous levels' output plus algorithm steps and notes announced on *stdout*.  Detailed errors announced on *stderr*. |
| `6`   | Previous levels' output plus module/class public function entries and returns announced on *stdout*. Detailed errors announced on *stderr*. | Previous levels' output plus module/class public function entries and returns. | Detailed errors. | Previous levels' output plus module/class public function entries and returns announced on *stdout*. Detailed errors announced on *stderr*. |
| `7`   | Previous levels' output plus non-public function entries and returns announced on *stdout*. Detailed errors announced on *stderr*. | Previous levels' output plus non-public function entries and returns. | Detailed errors. | Previous levels' output plus non-public function entries and returns announced on *stdout*. Detailed errors announced on *stderr*. |
| `8`   | Previous levels' output plus line execution details announced on *stdout*. Detailed errors announced on *stderr*. | Previous levels' output plus line execution details. | Detailed errors. | Previous levels' output plus line execution details announced on *stdout*. Detailed errors announced on *stderr*. |
| `9`   | Maximum detail announced on *stdout*. Detailed errors announced on *stderr*. | Maximum detail. | Detailed errors. | Maximum detail announced on *stdout*. Detailed errors announced on *stderr*. |

### Threading

As of early 2026, a modern personal computer typically has a processor with 6-96 cores.
The number of threads a program should use isn't just a function of the number of processor cores,
as a user may desire to use the computer for other tasks while a program using *LibGen* is running.

It is anticipated that:
- A program will have a default threading strategy.
- The default threading strategy may be overridden with command-line options, GUI picks, stored configurations,
  adaptive algorithms, etc.
- The threading strategy may be changed even while the program is performing multi-threaded operations.
- Threading decisions will be made outside of *LibGen*, and the only distinction meaningful for
  *LibGen* is single-threaded versus multi-threaded.

The strategy used is:
- *LibGen* is the repository for a program's default threading strategy.  This default strategy
  is specified via preprocessor symbols and compiled in to the program.
- *LibGen* is also the repository for a program's current threading strategy.
- *LibGen* will adapt its behavior as necessary to the program's current strategy.

### Logging

As with threading, *LibGen* is the repository of information about logging.

`LBGN_CLDF_LOGGING` must be defined to the value of `LBGN_CLDF_LOGGING_ENABLED`
for logging code to be included.

Log files are plain text, ASCII, with platform-dependent line endings.

The default name of the log file is *YYYYMMDD_HHMMSS_MMM_PPPPPPPP_libgen_log.txt*, where:
- *YYYYMMDD* are the 4-digit decimal year, 2-digit decimal month, and 2-digit decimal day of month on which the log file was opened.
- *HHMMSS* are the 2-digit hour (24-hour format), 2-digit minute, and 2-digit number of seconds of the time at which the log file was opened.
- *MMM* is the number of milliseconds within the second.
- *PPPPPPPP* is the process ID of the program opening the log file, in hexadecimal, using upper-case letters.

*Note:* The components of the date and time used to form the filename are gathered in a way so that they are
guaranteed consistent with each other.  Additionally, precautions are taken so that the filename is guaranteed
unique.

### Assertions

Assertions are implemented using the macro
```LBGN_ASSERT()```.  With assertions disabled, no code is generated; and with
assertions enabled, code to exit the program is generated.

## Preprocessor Definitions Supported

The following command-line preprocessor definitions are supported.

### `LBGN_CLDF_PFORM_SW` (Mnemonic: *L*i*bG*e*n* *C*ommand-*L*ine *D*e*f*inition, *P*lat*form*, *S*oft*w*are)

Enumerated, and must be one of the following values:

| Constant                     | Interpretation |
| :---                         |     :---       |
| `LBGN_CLDF_PFORM_SW_WINAPI`  | Windows API, also sometimes called *Win32* (a misnomer because 64-bit programs and 64-bit operating systems also use the Windows API) |
| `LBGN_CLDF_PFORM_SW_WINAPI_MFC`  | Windows API with MFC |
| `LBGN_CLDF_PFORM_SW_WIN_NET` | Windows .NET |
| `LBGN_CLDF_PFORM_SW_UNIX`    | Unix |
| `LBGN_CLDF_PFORM_SW_LINUX`   | Linux |
| `LBGN_CLDF_PFORM_SW_FREEBSD` | FreeBSD |
| `LBGN_CLDF_PFORM_SW_CYGWIN`  | Cygwin |
| `LBGN_CLDF_PFORM_SW_MSYS`    | MSYS |
| `LBGN_CLDF_PFORM_SW_ANDROID` | Android |
| `LBGN_CLDF_PFORM_SW_FIRE_OS` | FireOS |
| `LBGN_CLDF_PFORM_SW_IOS`     | iOS |
| `LBGN_CLDF_PFORM_SW_UNSPECIFIED` (default)  | Operating system not specified, system with operating system not listed above, or system with no operating system |

### `LBGN_CLDF_PROJTYPE`

Must be one of the following values:

| Constant                     | Interpretation |
| :---                         |     :---       |
| `LBGN_CLDF_PROJTYPE_LIB`           | Source files are being used to produce a classic library |
| `LBGN_CLDF_PROJTYPE_WINDLL`        | Source files are being used to produce Windows DLL |
| `LBGN_CLDF_PROJTYPE_NIXSL`         | Source files are being used to produce *nix shared libary |
| `LBGN_CLDF_PROJTYPE_EXECUTABLE`           | Source files are being used to produce an executable program |
| `LBGN_CLDF_PROJTYPE_UNSPECIFIED` (default)  | The type of project is not specified |

### `LBGN_CLDF_LOGGING`

Must be one of the following values:

| Constant                     | Interpretation |
| :---                         |     :---       |
| `LBGN_CLDF_LOGGING_DISABLED` (default)             | Logging functionality should not be included |
| `LBGN_CLDF_LOGGING_ENABLED`            | Logging functionality should be included |

### `LBGN_CLDF_ASSERTIONS`

Must be one of the following values:

| Constant                     | Interpretation |
| :---                         |     :---       |
| `LBGN_CLDF_ASSERTIONS_DISABLED` (default)     | Assertions are disabled |
| `LBGN_CLDF_ASSERTIONS_ENABLED`               | Assertions are enabled |

### `LBGN_CLDF_VERBOSITY_SUPPORT_MAX` (Mnemonic: *L*i*bG*e*n* *C*ommand-*L*ine *D*e*f*inition, *Verbosity* *Support* *Max*imum)

This preprocessor definition is an integer in the range of [3, 9], with a default
(if not specified) of 3.  The verbosity levels are specified in a section above.

This preprocessor definition defines the maximum verbosity level that will be supported by the built
program.  This preprocessor definition is intended to prevent overly large executables.  (For debugging,
the program using the library would generally be rebuilt with a larger value of `LBGN_CLDF_VERBOSITY_SUPPORT_MAX`
and a larger value of verbosity specified.)


## `LBGN_CLDF_PROGTYPE`

Must be one of the following values:

| Constant                     | Interpretation |
| :---                         |     :---       |
| `LBGN_CLDF_PROGTYPE_CONSOLE`               | Program is a console-mode program (text input, text output). |
| `LBGN_CLDF_PROGTYPE_WINGUI`                | Program is a graphical program under Windows. |
| `LBGN_CLDF_PROGTYPE_TCL_CONSOLE`           | Program is Tcl console-mode program. |
| `LBGN_CLDF_PROGTYPE_TCL_GUI`               | Program is Tcl/Tk graphical program. |
| `LBGN_CLDF_PROGTYPE_CLIKE_CONSOLE`         | Placeholder for future console application involving _Clike_, a yet to be developed scripting language. |
| `LBGN_CLDF_PROGTYPE_CLIKE_GUI`             | Placeholder for future graphical port of _Clike_, a yet to be developed scripting language. |
| `LBGN_CLDF_PROGTYPE_PYTHON_STATIC_CONSOLE` | Statically-linked extension to a Python interpreter running as a console-mode program. |
| `LBGN_CLDF_PROGTYPE_PYTHON_STATIC_GUI`     | Statically-linked extension to a Python interpreter running as a GUI program. |
| `LBGN_CLDF_PROGTYPE_PYTHON_DLL_CONSOLE`    | Dynamically-linked library designed to be used by a Python interpreter running as a console-mode program. |
| `LBGN_CLDF_PROGTYPE_PYTHON_DLL_GUI`        | Dynamically-linked library designed to be used by a Python interpreter running as a GUI program. |
| `LBGN_CLDF_PROGTYPE_UNIX_SWING`            | Program developed using Unix Swing. |
| `LBGN_CLDF_PROGTYPE_UNIX_AWT`              | Program developed using Unix AWT. |
| `LBGN_CLDF_PROGTYPE_CGIBIN_HELPER`         | Program is invoked by CGI-BIN PHP, Python, or Perl scripts, like to implement functionality awkward under the scripting language. |
| `LBGN_CLDF_PROGTYPE_CGIBIN_HTTPD`          | Program is a CGI-BIN program invoked directly by Apache to answer HTTP[S] requests. |
| `LBGN_CLDF_PROGTYPE_CGIBIN_SERVER`         | Program listens on TCP port(s) and is an actual HTTP[S] server. |
| `LBGN_CLDF_PROGTYPE_UNITTEST_COV`          | Program is a unit test program to test library components and/or measure testing statement/branch coverage. |
| `LBGN_CLDF_PROGTYPE_UNSPECIFIED` (default) | Program type is not specified. |

### `LIBNUM_CLSW_THREADING_ENABLED`

Must be one of the following values:

| Constant                     | Interpretation |
| :---                         |     :---       |
| `LIBNUM_CLSW_THREADING_ENABLED_NO` (default)            | *LibGen* is not compiled/built to support threading. |
| `LIBNUM_CLSW_THREADING_ENABLED_YES`                     | *LibGen* is compiled/built to support threading.  (*Note*: If this option is selected, there may be more *LibGen* modules required in the build, because multi-threading typically requires the use of operating system and C/C++ library features.) |

### `LIBNUM_CLSW_THREADING_DEFAULT`

This parameter must be one of the following values.  The running program may exercise finer control over the number
of cores used, and this parameter only specifies the default if the running program does not change the default.

| Constant                     | Interpretation |
| :---                         |     :---       |
| `LIBNUM_CLSW_THREADING_1R`                    | The program runs with one thread and a greatly reduced priority (essentially, a background program). |
| `LIBNUM_CLSW_THREADING_1` (default)           | The program runs with 1 thread for a console-mode program, or 2 threads for a GUI program, at default priority. |
| `LIBNUM_CLSW_THREADING_ONE_EIGHTH_PHYS_CORES`     | The program runs with a number of threads that will not use more than one eighth of the physical cores available. |
| `LIBNUM_CLSW_THREADING_ONE_QUARTER_PHYS_CORES`    | The program runs with a number of threads that will not use more than one quarter of the physical cores available. |
| `LIBNUM_CLSW_THREADING_ONE_HALF_PHYS_CORES`       | The program runs with a number of threads that will not use more than one half of the physical cores available. |
| `LIBNUM_CLSW_THREADING_THREE_QUARTERS_PHYS_CORES`    | The program runs with a number of threads that will not use more than three quarters of the physical cores available. |
| `LIBNUM_CLSW_THREADING_ALL_PHYS_CORES`        | The program runs with a number of threads that will not use more than all physical cores available. |
| `LIBNUM_CLSW_THREADING_ONE_EIGHTH_LOGICAL_CORES`     | The program runs with a number of threads that will not use more than one eighth of the logical cores available. |
| `LIBNUM_CLSW_THREADING_ONE_QUARTER_LOGICAL_CORES`    | The program runs with a number of threads that will not use more than one quarter of the logical cores available. |
| `LIBNUM_CLSW_THREADING_ONE_HALF_LOGICAL_CORES`       | The program runs with a number of threads that will not use more than one half of the logical cores available. |
| `LIBNUM_CLSW_THREADING_THREE_QUARTERS_LOGICAL_CORES`    | The program runs with a number of threads that will not use more than three quarters of the logical cores available. |
| `LIBNUM_CLSW_THREADING_ALL_LOGICAL_CORES`     | The program runs with a number of threads that will not use more than all the logical cores available. |
| `LIBNUM_CLSW_THREADING_UNLIMITED`             | The program may use an unlimited number of cores. |

## `LIBNUM_CLSW_CON_MODE_LINE_LEN`
Unsigned integer.  Must be >=38 and <=1000.  The number of printable characters per line.  If not set, defaults to 78.

## `LIBNUM_CLSW_MALLOC_STRAT`
Must be one of the following values.  Applies to malloc() and realloc() only, not new and delete.

| Constant                     | Interpretation |
| :---                         |     :---       |
| `LIBNUM_CLSW_MALLOC_STRAT_C`                 | The standard C strategy is used.  malloc() and realloc() return NULL on allocation failure. No exceptions are thrown. |
| `LIBNUM_CLSW_MALLOC_STRAT_CPP_EXCEP`         | An exception is thrown when out of memory. |
| `LIBNUM_CLSW_MALLOC_STRAT_CPP_EXCEP_NOTIF`   | A reserve pool is released when out of memory.  The state can be polled.  When the reserve pool is exhausted, an exception is thrown. |

## Documentation Rules

### Doxygen is Used for Documentation
_Doxygen_ is used as the documentation tool for this library, and comments
in the source code are formatted accordingly.
