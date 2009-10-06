/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Разрешается повторное распространение и использование как в виде исходного кода,
так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

      * При повторном распространении исходного кода должны оставаться указанное
        выше уведомление об авторском праве, этот список условий и последующий
        отказ от гарантий.
      * При повторном распространении двоичного кода в документации и/или в
        других материалах, поставляемых при распространении, должны сохраняться
        указанная выше информация об авторском праве, этот список условий и
        последующий отказ от гарантий.
      * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
        быть использованы в качестве средства поддержки и/или продвижения
        продуктов, основанных на этом ПО, без предварительного письменного
        разрешения.

ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Cognitive Technologies nor the names of its
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

///////////////////////////////////////////////////////////////////////////////
//
//  Module: CrashRpt.h
//
//    Desc: Defines the interface for the CrashRpt.DLL.
//
// Copyright (c) 2003 Michael Carruth
//
// Modified 27.12.2007 by Redart, Cognitive Tech
///////////////////////////////////////////////////////////////////////////////

#ifndef _CRASHRPT_H_
#define _CRASHRPT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h>

// CrashRpt.h
#ifdef CRASHRPTAPI

// CRASHRPTAPI should be defined in all of the DLL's source files as
// #define CRASHRPTAPI extern "C" __declspec(dllexport)

#else

// This header file is included by an EXE - export
#define CRASHRPTAPI extern "C" __declspec(dllimport)

#endif

// Client crash callback
typedef Bool (CALLBACK *LPGETLOGFILE) (pvoid lpvState);

namespace CrashHandler
{
//-----------------------------------------------------------------------------
// Install
//    Initializes the library and optionally set the client crash callback and
//    sets up the email details.
//
// Parameters
//    pfn         Client crash callback
//    lpTo        Email address to send crash report
//    lpSubject   Subject line to be used with email
//
// Return Values
//    If the function succeeds, the return value is a pointer to the underlying
//    crash object created.  This state information is required as the first
//    parameter to all other crash report functions.
//
// Remarks
//    Passing NULL for lpTo will disable the email feature and cause the crash
//    report to be saved to disk.
//
CRASHRPTAPI
pvoid
Install(
   IN LPGETLOGFILE pfn OPTIONAL,                // client crash callback
   IN const char * lpTo OPTIONAL,                    // Email:to
   IN const char * lpSubject OPTIONAL                // Email:subject
   );

CRASHRPTAPI
pvoid
Create(
   IN LPGETLOGFILE pfn OPTIONAL,                // client crash callback
   IN const char * lpTo OPTIONAL,                    // Email:to
   IN const char * lpSubject OPTIONAL                // Email:subject
   );

//-----------------------------------------------------------------------------
// Uninstall
//    Uninstalls the unhandled exception filter set up in Install().
//
// Parameters
//    lpState     State information returned from Install()
//
// Return Values
//    void
//
// Remarks
//    This call is optional.  The crash report library will automatically
//    deinitialize when the library is unloaded.  Call this function to
//    unhook the exception filter manually.
//
CRASHRPTAPI
void
Uninstall(
   IN pvoid lpState                            // State from Install()
   );

//-----------------------------------------------------------------------------
// AddFile
//    Adds a file to the crash report.
//
// Parameters
//    lpState     State information returned from Install()
//    lpFile      Fully qualified file name
//    lpDesc      Description of file, used by details dialog
//
// Return Values
//    void
//
// Remarks
//    This function can be called anytime after Install() to add one or more
//    files to the generated crash report.
//
CRASHRPTAPI
void
AddFile(
   IN pvoid lpState,                           // State from Install()
   IN const char * lpFile,                           // File name
   IN const char * lpDesc                            // File desc
   );

//-----------------------------------------------------------------------------
// GenerateErrorReport
//    Generates the crash report.
//
// Parameters
//    lpState     State information returned from Install()
//    pExInfo     Pointer to an EXCEPTION_POINTERS structure
//
// Return Values
//    void
//
// Remarks
//    Call this function to manually generate a crash report.
//
CRASHRPTAPI
void
GenerateErrorReport(
   IN pvoid lpState,
   IN PEXCEPTION_POINTERS pExInfo OPTIONAL
   );

CRASHRPTAPI
void
StaticGenerateErrorReport(
   IN PEXCEPTION_POINTERS pExInfo OPTIONAL,
   IN LPGETLOGFILE pfn OPTIONAL,                // client crash callback
   IN const char * lpTo OPTIONAL,                    // Email:to
   IN const char * lpSubject OPTIONAL                // Email:subject
   );

}
#endif
