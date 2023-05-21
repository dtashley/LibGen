//$Header: svn://localhost/dtapublic/projs/emts/trunk/src/lib_c/c_emts/emts.h 317 2020-01-05 22:51:21Z dashley $
//{149fb278-422e-4dac-8c7a-bcf175ec0120}
//-------------------------------------------------------------------------------------------------
//This file is part of "Embedded Tool Set" (EMTS), a tool set designed to facilitate embedded
//system software and hardware development.
//-------------------------------------------------------------------------------------------------
//This source code and any program in which it is compiled/used is provided under the MIT License,
//reproduced below.
//-------------------------------------------------------------------------------------------------
//Permission is hereby granted, free of charge, to any person obtaining a copy of
//this software and associated documentation files(the "Software"), to deal in the
//Software without restriction, including without limitation the rights to use,
//copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the
//Software, and to permit persons to whom the Software is furnished to do so,
//subject to the following conditions:
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
#ifndef EMTS_H_INCLUDED
   #define EMTS_H_INCLUDED
   //------------------------------------------------------------------------------------------
   //For all the categories of constants below, "0" is not used.  The reason for that is that
   //the C standard provides that a symbol #define'd without a value tests as 0, and we have
   //to be able to differentiate that case from #define'ing to a value.
   //------------------------------------------------------------------------------------------
   //P L A T F O R M
   //------------------------------------------------------------------------------------------
   //Define constants for the platform.
   #define K_EMTS_PF_WINAPI    ( 1)  //Windows API (also sometimes called Win32, although this
                                     //a misnomer because 64-bit programs can also use the
                                     //Win32 API).
   #define K_EMTS_PF_MFC       ( 2)  //Windows API with MFC.
   #define K_EMTS_PF_WIN_NET   ( 3)  //Windows .NET.
   #define K_EMTS_PF_UNIX      ( 4)  //Unix.
   #define K_EMTS_PF_LINUX     ( 5)  //Linux.
   #define K_EMTS_PF_FREEBSD   ( 6)  //FreeBSD.
   #define K_EMTS_PF_CYGWIN    ( 7)  //Cygwin.
   #define K_EMTS_PF_ANDROID   ( 8)  //Android.
   #define K_EMTS_PF_FIRE_OS   ( 9)  //FireOS.
   #define K_EMTS_PF_IOS       (10)  //IOs.
   //------------------------------------------------------------------------------------------
   //Ensure that the platform is defined.
   #ifndef K_EMTS_PF
      #error "K_EMTS_PF not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure it is in range.
   #if (K_EMTS_PF < K_EMTS_PF_WINAPI) || (K_EMTS_PF > K_EMTS_PF_IOS)
      #error "K_EMTS_PF out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //O P E R A T I N G    S Y S T E M    W O R D    S I Z E
   //------------------------------------------------------------------------------------------
   //Be sure in range.
   #if (K_EMTS_WSZ_OS != 32) && (K_EMTS_WSZ_OS != 64)
      #error "K_EMTS_WSZ_OS must be 32 or 64"
   #endif
   //------------------------------------------------------------------------------------------
   //P R O C E S S O R    F O R    A S S E M B L Y    L A N G U A G E
   //------------------------------------------------------------------------------------------
   #define K_EMTS_PROC_AL_NONE     (1)    //No assembly-language available for platform.
   #define K_EMTS_PROC_AL_X86_32   (2)    //x86
   #define K_EMTS_PROC_AL_X86_64   (3)    //x64
   //------------------------------------------------------------------------------------------
   //Ensure that the processor assembly language is defined.
   #ifndef K_EMTS_PROC_AL
      #error "K_EMTS_PROC_AL not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure it is in range.
   #if (K_EMTS_PROC_AL < K_EMTS_PROC_AL_NONE) || (K_EMTS_PROC_AL > K_EMTS_PROC_AL_X86_64)
      #error "K_EMTS_PROC_AL out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //M A C H I N E    W O R D    S I Z E
   //------------------------------------------------------------------------------------------
   //Ensure that the machine word size is defined.
   #ifndef K_EMTS_WSZ_M
      #error "K_EMTS_WSZ_M not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure it is in range.
   #if (K_EMTS_WSZ_M < 16) || (K_EMTS_WSZ_M > 64)
      #error "K_EMTS_WSZ_M out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //N U M B E R    O F    B I T S    I N    C    L O N G    L O N G    I N T E G E R
   //------------------------------------------------------------------------------------------
   //Ensure defined.
   #ifndef K_EMTS_SZ_INT_LL
      #error "K_EMTS_SZ_INT_LL not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure size of long long integer is in range.
   #if (K_EMTS_SZ_INT_LL < 32) || (K_EMTS_SZ_INT_LL > 128)
      #error "K_EMTS_SZ_INT_LL out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //N U M B E R    O F    B I T S    I N    C    L O N G    I N T E G E R
   //------------------------------------------------------------------------------------------
   //Ensure defined.
   #ifndef K_EMTS_SZ_INT_L
      #error "K_EMTS_SZ_INT_L not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure size of long integer is in range.
   #if (K_EMTS_SZ_INT_L < 32) || (K_EMTS_SZ_INT_L > 128)
      #error "K_EMTS_SZ_INT_L out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //N U M B E R    O F    B I T S    I N    C    I N T E G E R
   //------------------------------------------------------------------------------------------
   //Ensure defined.
   #ifndef K_EMTS_SZ_INT
      #error "K_EMTS_SZ_INT not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure size of integer is in range.
   #if (K_EMTS_SZ_INT < 16) || (K_EMTS_SZ_INT > 128)
      #error "K_EMTS_SZ_INT out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //N U M B E R    O F    B I T S    I N    C    S H O R T    I N T E G E R
   //------------------------------------------------------------------------------------------
   //Ensure defined.
   #ifndef K_EMTS_SZ_INT_S
      #error "K_EMTS_SZ_INT_S not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure size of short integer is in range.
   #if (K_EMTS_SZ_INT_S < 16) || (K_EMTS_SZ_INT_S > 128)
      #error "K_EMTS_SZ_INT_S out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //M A C H I N E    I N T E G E R    R E P R E S E N T A T I O N
   //------------------------------------------------------------------------------------------
   #define K_EMTS_MIR_2SCOMP     (1)    //2's complement.
   #define K_EMTS_MIR_SIGNMAG    (2)    //Sign-magnitude.
   #define K_EMTS_MIR_OTHER      (3)    //Some other.
   //------------------------------------------------------------------------------------------
   //Ensure defined.
   #ifndef K_EMTS_MIR
      #error "K_EMTS_MIR not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure it is in range.
   #if (K_EMTS_MIR < K_EMTS_MIR_2SCOMP) || (K_EMTS_MIR > K_EMTS_MIR_OTHER)
      #error "EMTS_MIR out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //M A C H I N E    F L O A T I N G   P O I N T    U N I T
   //------------------------------------------------------------------------------------------
   #define K_EMTS_MFPU_NO        (1)  //Machine does not have a floating point unit.
   #define K_EMTS_MFPU_YES       (2)  //Machine does have a floating point unit.
   //------------------------------------------------------------------------------------------
   //Ensure defined.
   #ifndef K_EMTS_MFPU
      #error "K_EMTS_MFPU not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure it is in range.
   #if (K_EMTS_MFPU < K_EMTS_MFPU_NO) || (K_EMTS_MFPU > K_EMTS_MFPU_YES)
      #error "K_EMTS_MFPU out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //P R O J E C T    T Y P E
   //------------------------------------------------------------------------------------------
   #define K_EMTS_PROJTYPE_APP   (1)  //Project produces an application.
   #define K_EMTS_PROJTYPE_LIB   (2)  //Project produces a library.
   #define K_EMTS_PROJTYPE_OBJ   (3)  //Project produces an object file.
   //------------------------------------------------------------------------------------------
   //Ensure defined.
   #ifndef K_EMTS_PROJTYPE
      #error "K_EMTS_PROJTYPE not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure it is in range.
   #if (K_EMTS_PROJTYPE < K_EMTS_PROJTYPE_APP) || (K_EMTS_PROJTYPE > K_EMTS_PROJTYPE_OBJ)
      #error "K_EMTS_PROJTYPE out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //P R O J E C T    L I N K    T Y P E
   //------------------------------------------------------------------------------------------
   #define K_EMTS_LINKTYPE_STATIC   (1)  //Project is statically linked.
   #define K_EMTS_LINKTYPE_DYNAMIC  (2)  //Project is dynamically linked.
   //------------------------------------------------------------------------------------------
   //Ensure defined.
   #ifndef K_EMTS_LINKTYPE
      #error "K_EMTS_LINKTYPE not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure it is in range.
   #if (K_EMTS_LINKTYPE < K_EMTS_LINKTYPE_STATIC) || (K_EMTS_LINKTYPE > K_EMTS_LINKTYPE_DYNAMIC)
      #error "K_EMTS_LINKTYPE out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //P R O J E C T    D E B U G    L E V E L
   //------------------------------------------------------------------------------------------
   #define K_EMTS_DEBUGLVL_RELEASE  (1)  //Project is a release.
   #define K_EMTS_DEBUGLVL_DEBUG    (2)  //Project is intended for debugging or analysis.
   //------------------------------------------------------------------------------------------
   //Ensure defined.
   #ifndef K_EMTS_DEBUGLVL
      #error "K_EMTS_DEBUGLVL not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure it is in range.
   #if (K_EMTS_DEBUGLVL < K_EMTS_DEBUGLVL_RELEASE) || (K_EMTS_DEBUGLVL > K_EMTS_DEBUGLVL_DEBUG)
      #error "K_EMTS_DEBUGLVL out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //P R O J E C T    D E B U G    L O G    L E V E L
   //------------------------------------------------------------------------------------------
   #define K_EMTS_DEBUGLOGLVL_NONE                     ( 1)  //No log file opened, no logging.
   #define K_EMTS_DEBUGLOGLVL_ERR                      ( 2)  //Log file opened, errors logged.
   #define K_EMTS_DEBUGLOGLVL_SUSPICIOUS               ( 3)  //Log file opened, suspicious
                                                             //conditions and above logged.
   #define K_EMTS_DEBUGLOGLVL_SIGMILESTONES            ( 4)  //Log file opened, significant
                                                             //milestones and above logged.
   #define K_EMTS_DEBUGLOGLVL_MILESTONES               ( 5)  //Log file opened, milestones and
                                                             //above logged.
   #define K_EMTS_DEBUGLOGLVL_FUNCBOUNDARY             ( 6)  //Log file opened, function
                                                             //boundaries and above logged.
   #define K_EMTS_DEBUGLOGLVL_FUNCINTERNALSBASIC       ( 7)  //Log file opened, basic function
                                                             //internals and above logged.
   #define K_EMTS_DEBUGLOGLVL_FUNCINTERNALSDETAILED    ( 8)  //Log file opened, detailed
                                                             //function internals and above
                                                             //logged.
   //------------------------------------------------------------------------------------------
   //Ensure defined.
   #ifndef K_EMTS_DEBUGLOGLVL
      #error "K_EMTS_DEBUGLOGLVL not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure it is in range.
   #if (K_EMTS_DEBUGLOGLVL < K_EMTS_DEBUGLOGLVL_NONE) || (K_EMTS_DEBUGLOGLVL > K_EMTS_DEBUGLOGLVL_FUNCINTERNALSDETAILED)
      #error "K_EMTS_DEBUGLOGLVL out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //P R O J E C T    A S S E R T I O N    L E V E L
   //------------------------------------------------------------------------------------------
   #define K_EMTS_ASSERTLVL_NONE                       ( 1)  //No assertions.
   #define K_EMTS_ASSERTLVL_ERR_SERIOUS                ( 2)  //Assertions for serious errors.
   #define K_EMTS_ASSERTLVL_ERR                        ( 3)  //Assertions for errors and above.
   #define K_EMTS_ASSERTLVL_SUSPICIOUS_VERY            ( 4)  //Assertions for very suspicious
                                                             //conditions and above.
   #define K_EMTS_ASSERTLVL_SUSPICIOUS                 ( 5)  //Assertions for suspicious
                                                             //conditions and above.
   #define K_EMTS_ASSERTLVL_FULL                       ( 6)  //Full assertions.
   //------------------------------------------------------------------------------------------
   //Ensure defined.
   #ifndef K_EMTS_ASSERTLVL
      #error "K_EMTS_ASSERTLVL not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure it is in range.
   #if (K_EMTS_ASSERTLVL < K_EMTS_ASSERTLVL_NONE) || (K_EMTS_ASSERTLVL > K_EMTS_ASSERTLVL_FULL)
      #error "K_EMTS_ASSERTLVL out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //P R O G R A M    T Y P E
   //------------------------------------------------------------------------------------------
   #define K_EMTS_PROGTYPE_CONSOLE               ( 1)  //Program is a console-mode
                                                       //utility (text input, text
                                                       //output).
   #define K_EMTS_PROGTYPE_WINGUI                ( 2)  //Program is a graphical program under
                                                       //Windows.
   #define K_EMTS_PROGTYPE_TCL_A_CONSOLE         ( 3)  //Program is a Tcl console-mode utility,
                                                       //using Tcl code ported by Dave Ashley
                                                       //around 2004.
   #define K_EMTS_PROGTYPE_TCL_A_GUI             ( 4)  //Program is a Tcl/Tk graphical utility,
                                                       //ported by Dave Ashley around 2004.
   #define K_EMTS_PROGTYPE_TCL_B_CONSOLE         ( 5)  //Placeholder for future console port
                                                       //of Tcl.
   #define K_EMTS_PROGTYPE_TCL_B_GUI             ( 6)  //Placeholder for future graphical port
                                                       //of Tcl/Tk.
   #define K_EMTS_PROGTYPE_CLIKE_A_CONSOLE       ( 7)  //Placeholder for future
                                                       //console application involving "Clike"
                                                       //(a yet-to-be-developed C-like scripting
                                                       //language).
   #define K_EMTS_PROGTYPE_CLIKE_A_GUI           ( 8)  //Placeholder for future graphical
                                                       //application involving "Clike" (a
                                                       //yet-to-be-developed C-like scripting
                                                       //language).
   #define K_EMTS_PROGTYPE_UNIX_SWING            ( 9)  //Program developed using Unix Swing.
   #define K_EMTS_PROGTYPE_UNIX_AWT              (10)  //Program developed using Unix AWT.
   #define K_EMTS_PROGTYPE_CGIBIN_HELPER         (11)  //Program is invoked by CGI-BIN PHP,
                                                       //Python, or Perl scripts to implement
                                                       //functionality awkward under the
                                                       //scripting language.
   #define K_EMTS_PROGTYPE_CGIBIN_HTTPD          (12)  //Program is a CGI-BIN program invoked
                                                       //directly by httpd to answer HTTP[S]
                                                       //requests.
   #define K_EMTS_PROGTYPE_CGIBIN_SERVER         (13)  //Program listens on TCP ports and is an
                                                       //actual HTTP[S] server.
   #define K_EMTS_PROGTYPE_UNITTEST_MODULE       (14)  //Program is a unit test program
                                                       //compiled to test an individual
                                                       //software module.
   //------------------------------------------------------------------------------------------
   //Ensure defined.
   #ifndef K_EMTS_PROGTYPE
      #error "K_EMTS_PROGTYPE not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure it is in range.
   #if (K_EMTS_PROGTYPE < K_EMTS_PROGTYPE_CONSOLE) || (K_EMTS_PROGTYPE > K_EMTS_PROGTYPE_UNITTEST_MODULE)
      #error "K_EMTS_PROGTYPE out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //S C R E E N    S I Z E
   //------------------------------------------------------------------------------------------
   #define K_EMTS_SCREENSIZE_SMALL               ( 1)  //The target screen size is small (such
                                                       //as a cellphone or tablet computer).
   #define K_EMTS_SCREENSIZE_LARGE               ( 2)  //The target screen size is large (such
                                                       //as a larger tablet computer or
                                                       //a PC).
   #define K_EMTS_SCREENSIZE_ADAPTIVE            ( 3)  //The program adapts to the screen size.
   //------------------------------------------------------------------------------------------
   //Ensure defined.
   #ifndef K_EMTS_SCREENSIZE
      #error "K_EMTS_SCREENSIZE not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure it is in range.
   #if (K_EMTS_SCREENSIZE < K_EMTS_SCREENSIZE_SMALL) || (K_EMTS_SCREENSIZE > K_EMTS_SCREENSIZE_ADAPTIVE)
      #error "K_EMTS_SCREENSIZE out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //T H R E A D E D N E S S
   //------------------------------------------------------------------------------------------
   #define K_EMTS_THREADS_1R                   ( 1)  //The program runs with one thread, a
                                                     //greatly reduced priority (essentially,
                                                     //a background program).
   #define K_EMTS_THREADS_1                    ( 2)  //The program runs with one thread, at
                                                     //unmodified priority.
   #define K_EMTS_THREADS_2                    ( 3)  //The program runs with up to two threads,
                                                     //at unmodified priority.
   #define K_EMTS_THREADS_3                    ( 4)  //The program runs with up to three
                                                     //threads, at unmodified priority.
   #define K_EMTS_THREADS_4                    ( 5)  //The program runs with up to four
                                                     //threads, at unmodified priority.
   #define K_EMTS_THREADS_ADAPT_HALF_CORES     ( 6)  //The program adapts to the number of
                                                     //cores on the target system, attempting
                                                     //to use one half of the cores, at normal
                                                     //priority.
   #define K_EMTS_THREADS_ADAPT_ALL_CORES      ( 7)  //The program adapts to the number of
                                                     //cores on the target system, attempting
                                                     //to use all of the cores, at normal
                                                     //priority.
   #define K_EMTS_THREADS_ADAPT_ALL_LOGICAL    ( 8)  //The program adapts to use all logical
                                                     //cores (with the Intel architecture, this
                                                     //may be twice the number of physical
                                                     //cores).
   #define K_EMTS_THREADS_PROG_SET             ( 9)  //The number of threads and priority are
                                                     //set by the program (rather than at
                                                     //compile time).
   //------------------------------------------------------------------------------------------
   //Ensure defined.
   #ifndef K_EMTS_THREADS
      #error "K_EMTS_THREADS not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure it is in range.
   #if (K_EMTS_THREADS < K_EMTS_THREADS_1R) || (K_EMTS_THREADS > K_EMTS_THREADS_PROG_SET)
      #error "K_EMTS_THREADS out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //C H A R A C T E R    M O D E
   //------------------------------------------------------------------------------------------
   #define K_EMTS_CMODE_ASCII                  ( 1)  //Classic ASCII characters.
   #define K_EMTS_CMODE_UNICODE                ( 2)  //Support for Unicode.
   //------------------------------------------------------------------------------------------
   //Ensure defined.
   #ifndef K_EMTS_CMODE
      #error "K_EMTS_CMODE not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure it is in range.
   #if (K_EMTS_CMODE < K_EMTS_CMODE_ASCII) || (K_EMTS_CMODE > K_EMTS_CMODE_UNICODE)
      #error "K_EMTS_CMODE out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //M E M O R Y    A L L O C A T I O N    S T R A T E G Y
   //------------------------------------------------------------------------------------------
   #define K_EMTS_MALLOCSTRAT_OM_FATAL         ( 1)  //Simple strategy.  Out of memory
                                                     //results in immediate program
                                                     //termination.
   #define K_EMTS_MALLOCSTRAT_RSRV_NOTIF       ( 2)  //Sophisticated strategy with memory
                                                     //reserve and notification strategy
                                                     //before fatal program termination.
   //------------------------------------------------------------------------------------------
   //Ensure defined.
   #ifndef K_EMTS_MALLOCSTRAT
      #error "K_EMTS_MALLOCSTRAT not defined"
   #endif
   //------------------------------------------------------------------------------------------
   //Be sure it is in range.
   #if (K_EMTS_MALLOCSTRAT < K_EMTS_MALLOCSTRAT_OM_FATAL) || (K_EMTS_MALLOCSTRAT > K_EMTS_MALLOCSTRAT_RSRV_NOTIF)
      #error "K_EMTS_MALLOCSTRAT out of range"
   #endif
   //------------------------------------------------------------------------------------------
   //C O N S O L E    M O D E    L I N E    L E N G T H
   //------------------------------------------------------------------------------------------
   //If the console line length hasn't been defined on the command line, set it to a reasonable
   //default.
   #ifndef K_EMTS_CFG_CON_MODE_LINE_LEN
      #define K_EMTS_CFG_CON_MODE_LINE_LEN  (78)
         //For console mode programs, maximum line length to avoid aesthetic (but not logical)
         //issues.
   #endif
#endif
//End of emts.h.
