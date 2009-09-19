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
//  Module: CrashHandler.cpp
//
//    Desc: See CrashHandler.h
//
// Copyright (c) 2003 Michael Carruth
//
// Modified 27.12.2007 by Redart, Cognitive Tech
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CrashHandler.h"
#include "zlibcpp.h"
#include "excprpt.h"
#include "maindlg.h"
#include "process.h"
#include "mailmsg.h"

// global app module
CAppModule _Module;

// maps crash objects to processes
CSimpleMap<int, CCrashHandler*> _crashStateMap;

// unhandled exception callback set with SetUnhandledExceptionFilter()
int32_t WINAPI CustomUnhandledExceptionFilter(PEXCEPTION_POINTERS pExInfo)
{
   _crashStateMap.Lookup(_getpid())->GenerateErrorReport(pExInfo);

   return EXCEPTION_EXECUTE_HANDLER;
}

CCrashHandler::CCrashHandler(LPGETLOGFILE lpfn /*=NULL*/,
                             const char * lpcszTo /*=NULL*/,
                             const char * lpcszSubject /*=NULL*/,
							 bool bInstallHandler /*= true*/)
{
   // wtl initialization stuff...
	HRESULT hRes = ::CoInitialize(NULL);
	//Если COM уже проинициализировали, но в другом режиме - мы не бужем вызывать разинициализацию
	if (hRes == RPC_E_CHANGED_MODE)
		m_COMInited = false;
	else
	{
		ATLASSERT(SUCCEEDED(hRes));
		m_COMInited = SUCCEEDED(hRes);
	}


   hRes = _Module.Init(NULL, GetModuleHandle("CrashRpt.dll"));
   ATLASSERT(SUCCEEDED(hRes));

	::DefWindowProc(NULL, 0, 0, 0L);

   // initialize member data
   m_lpfnCallback = NULL;
   m_oldFilter    = NULL;

   // save user supplied callback
   if (lpfn)
      m_lpfnCallback = lpfn;

   if (bInstallHandler)
   {
	// add this filter in the exception callback chain
	m_oldFilter = SetUnhandledExceptionFilter(CustomUnhandledExceptionFilter);

	// attach this handler with this process
	m_pid = _getpid();
	if (!_crashStateMap.SetAt(m_pid, this))
		_crashStateMap.Add(m_pid, this);
   }
   // save optional email info
   m_sTo = lpcszTo;
   m_sSubject = lpcszSubject;
}

CCrashHandler::~CCrashHandler()
{
   // reset exception callback
   if (m_oldFilter)
      SetUnhandledExceptionFilter(m_oldFilter);

   _crashStateMap.Remove(m_pid);

   // uninitialize
   _Module.Term();
   if (m_COMInited)
	::CoUninitialize();

}

void CCrashHandler::AddFile(const char * lpFile, const char * lpDesc)
{
   // make sure the file exist
   HANDLE hFile = ::CreateFile(
                     lpFile,
                     GENERIC_READ,
                     FILE_SHARE_READ | FILE_SHARE_WRITE,
                     NULL,
                     OPEN_EXISTING,
                     FILE_ATTRIBUTE_NORMAL,
                     0);
   if (hFile)
   {
      // add file to report
      m_files[lpFile] = lpDesc;
      ::CloseHandle(hFile);
   }
}

void CCrashHandler::GenerateErrorReport(PEXCEPTION_POINTERS pExInfo)
{
   CExceptionReport  rpt(pExInfo);
   CMainDlg          mainDlg;
   CZLib             zlib;
   CString           sTempFileName = CUtility::getTempFileName();
   unsigned int      i;

   // let client add application specific files to report
   if (m_lpfnCallback && !m_lpfnCallback(this))
      return;

	//Определеяем, что делать с отчетом
	DumpType dumpType = Referenced;//По умолчанию сбрасываем только ту память, на которую идут ссылки в стеке
	ActionType actionType = GUI;//По умолчанию - выводим пользователю диалог
	CString action, storeFolder, dump;

	ATL::CRegKey rk;
	//Читаем из ключа с именем приложения
	int32_t lRet = rk.Open(HKEY_LOCAL_MACHINE,
	   TEXT("SOFTWARE\\Cognitive Technologies Ltd.\\CuneiForm\\PumaCrashRpt\\")
	   + CUtility::getAppName(), KEY_QUERY_VALUE);
	if(lRet != ERROR_SUCCESS)
	{
		//Читаем из дефолтного ключа
		lRet = rk.Open(HKEY_LOCAL_MACHINE,
			TEXT("SOFTWARE\\Cognitive Technologies Ltd.\\CuneiForm\\PumaCrashRpt\\Default"),
			KEY_QUERY_VALUE);
	}
	if(lRet == ERROR_SUCCESS)
	{
		//Читаем дейтсвие
		uint32_t dwBufLen = 1000;
		rk.QueryValue(action.GetBuffer(1000), "Action", &dwBufLen);
	    action.ReleaseBuffer(MAX(dwBufLen - 1, 0));

		//Читаем папку для автосохранения
		dwBufLen = 1000;
		rk.QueryValue(storeFolder.GetBuffer(1000), "StoreFolder", &dwBufLen);
	    storeFolder.ReleaseBuffer(MAX(dwBufLen - 1, 0));

		//Читаем тип дампа
		dwBufLen = 1000;
		rk.QueryValue(dump.GetBuffer(1000), "DumpType", &dwBufLen);
	    dump.ReleaseBuffer(MAX(dwBufLen - 1, 0));

		rk.Close();

		if (!action.IsEmpty())
		{
			if (action.CompareNoCase("GUI") == 0)
				actionType = GUI;
			else if (action.CompareNoCase("QuietStore") == 0)
				actionType = QuietStore;
			else if (action.CompareNoCase("NoAction") == 0)
				actionType = NoAction;

		}
		if (!dump.IsEmpty())
		{
			if (dump.CompareNoCase("Mini") == 0)
				dumpType = Mini;
			else if (dump.CompareNoCase("Referenced") == 0)
				dumpType = Referenced;
			else if (dump.CompareNoCase("Full") == 0)
				dumpType = Full;

		}
		if (storeFolder.CompareNoCase("Temp folder") == 0)
			storeFolder = getenv("TEMP");
	}

	if (actionType == NoAction)
		return;

   // add crash files to report
   m_files[rpt.getCrashFile(dumpType)] = CString((const char *)IDS_CRASH_DUMP);
   m_files[rpt.getCrashLog()] = CString((const char *)IDS_CRASH_LOG);

   // add symbol files to report
   for (i = 0; i < (uint)rpt.getNumSymbolFiles(); i++)
      m_files[(const char *)rpt.getSymbolFile(i)] =
      CString((const char *)IDS_SYMBOL_FILE);

   // zip the report
   if (!zlib.Open(sTempFileName))
      return;

   // add report files to zip
   TStrStrMap::iterator cur = m_files.begin();
   for (i = 0; i < m_files.size(); i++, cur++)
      zlib.AddFile((*cur).first);

   zlib.Close();

   if (actionType == GUI)
   {
	   // display main dialog
	   mainDlg.m_pUDFiles = &m_files;

	   //Сохраняем флаги по исключениям с плавающей точкой - кто-то их злобно сбрасывает при показе диалога
	   uint oldFpState = _controlfp(0, 0);
	   if (IDOK == mainDlg.DoModal())
	   {
	      if (m_sTo.IsEmpty() ||
		      !MailReport(rpt, sTempFileName, mainDlg.m_sEmail, mainDlg.m_sDescription))
		  {
			 SaveReport(rpt, sTempFileName);
		  }
	   }
	   //Восстанавливаем флаги
	   _controlfp(oldFpState, _MCW_DN | _MCW_EM | _MCW_IC | _MCW_RC | _MCW_PC);
   }
   else if (actionType == QuietStore)
   {
		// Just in-case it already exist
		::DeleteFile(storeFolder + '\\' + CUtility::formatSaveFileName() + ".zip");
		::CopyFile(sTempFileName, storeFolder + '\\' + CUtility::formatSaveFileName() + ".zip", TRUE);
   }
   DeleteFile(sTempFileName);
}

Bool CCrashHandler::SaveReport(CExceptionReport&, const char * lpcszFile)
{
   // let user more zipped report
   return (CopyFile(lpcszFile, CUtility::getSaveFileName(), TRUE));
}

Bool CCrashHandler::MailReport(CExceptionReport&, const char * lpcszFile,
                               const char * lpcszEmail, const char * lpcszDesc)
{
   CMailMsg msg;
   msg
      .SetTo(m_sTo)
      .SetFrom(lpcszEmail)
      .SetSubject(m_sSubject.IsEmpty()?_T("Incident Report"):m_sSubject)
      .SetMessage(lpcszDesc)
      .AddAttachment(lpcszFile, CUtility::getAppName() + _T(".zip"));

   return (msg.Send());
}
