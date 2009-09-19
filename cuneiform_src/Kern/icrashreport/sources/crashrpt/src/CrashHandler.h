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
//  Module: CrashHandler.h
//
//    Desc: CCrashHandler is the main class used by crashrpt to manage all
//          of the details associated with handling the exception, generating
//          the report, gathering client input, and sending the report.
//
// Copyright (c) 2003 Michael Carruth
//
// Modified 27.12.2007 by Redart, Cognitive Tech
///////////////////////////////////////////////////////////////////////////////

#ifndef _CRASHHANDLER_H_
#define _CRASHHANDLER_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "crashrpt.h"      // defines LPGETLOGFILE callback
#include "excprpt.h"       // bulk of crash report generation

#ifndef TStrStrMap
#include <map>
#include <atlmisc.h>

typedef std::map<CString,CString> TStrStrMap;
#endif // !defined TStrStrMap

////////////////////////////// Class Definitions /////////////////////////////

// ===========================================================================
// CCrashHandler
//
// See the module comment at top of file.
//
enum ActionType
{
	GUI,
	QuietStore,
	NoAction
};
class CCrashHandler
{
public:

   //-----------------------------------------------------------------------------
   // CCrashHandler
   //    Initializes the library and optionally set the client crash callback and
   //    sets up the email details.
   //
   // Parameters
   //    lpfn        Client crash callback
   //    lpcszTo     Email address to send crash report
   //    lpczSubject Subject line to be used with email
   //
   // Return Values
   //    none
   //
   // Remarks
   //    Passing NULL for lpTo will disable the email feature and cause the crash
   //    report to be saved to disk.
   //
   CCrashHandler(
      LPGETLOGFILE lpfn = NULL,           // Client crash callback
      const char * lpcszTo = NULL,             // EMail:To
      const char * lpcszSubject = NULL,         // EMail:Subject
	  bool bInstallHandler = true			// Устаналивать ли хендлер на непойманные исключения
      );

   //-----------------------------------------------------------------------------
   // ~CCrashHandler
   //    Uninitializes the crashrpt library.
   //
   // Parameters
   //    none
   //
   // Return Values
   //    none
   //
   // Remarks
   //    none
   //
   virtual
   ~CCrashHandler();

   //-----------------------------------------------------------------------------
   // AddFile
   //    Adds a file to the crash report.
   //
   // Parameters
   //    lpFile      Fully qualified file name
   //    lpDesc      File description
   //
   // Return Values
   //    none
   //
   // Remarks
   //    Call this function to include application specific file(s) in the crash
   //    report.  For example, applicatoin logs, initialization files, etc.
   //
   void
   AddFile(
      const char * lpFile,                     // File nae
      const char * lpDesc                      // File description
      );

   //-----------------------------------------------------------------------------
   // GenerateErrorReport
   //    Produces a crash report.
   //
   // Parameters
   //    pExInfo     Pointer to an EXCEPTION_POINTERS structure
   //
   // Return Values
   //    none
   //
   // Remarks
   //    Call this function to manually generate a crash report.
   //
   void
   GenerateErrorReport(
      PEXCEPTION_POINTERS pExInfo         // Exception pointers (see MSDN)
      );

protected:

   //-----------------------------------------------------------------------------
   // SaveReport
   //    Presents the user with a file save dialog and saves the crash report
   //    file to disk.  This function is called if an Email:To was not provided
   //    in the constructor.
   //
   // Parameters
   //    rpt         The report details
   //    lpcszFile   The zipped crash report
   //
   // Return Values
   //    True is successful.
   //
   // Remarks
   //    none
   //
   Bool
   SaveReport(
      CExceptionReport &rpt,
      const char * lpcszFile
      );

   //-----------------------------------------------------------------------------
   // MailReport
   //    Mails the zipped crash report to the address specified.
   //
   // Parameters
   //    rpt         The report details
   //    lpcszFile   The zipped crash report
   //    lpcszEmail  The Email:To
   //    lpcszDesc
   //
   // Return Values
   //    TRUE is sucessful.
   //
   // Remarks
   //    MAPI is used to send the report.
   //
   Bool
   MailReport(
      CExceptionReport &rpt,
      const char * lpcszFile,
      const char * lpcszEmail,
      const char * lpcszSubject
      );

   LPTOP_LEVEL_EXCEPTION_FILTER  m_oldFilter;      // previous exception filter
   LPGETLOGFILE                  m_lpfnCallback;   // client crash callback
   int                           m_pid;            // process id
   TStrStrMap                    m_files;          // custom files to add
   CString                       m_sTo;            // Email:To
   CString                       m_sSubject;       // Email:Subject
   bool							 m_bInstalled;	   // Определяет, установлен ли данный экземпляр в качестве обрабочика исключения
   bool							 m_COMInited;	   // Опеределяет, нужно ли вызывать CoUninitalaize в деструкторе

};

#endif	// #ifndef _CRASHHANDLER_H_
