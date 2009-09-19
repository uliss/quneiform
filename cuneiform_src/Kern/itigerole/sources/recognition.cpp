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

// Recognition.cpp : Implementation of CRecognition
#include "stdafx.h"

#include <io.h>
#include <stdio.h>
#include <process.h>

#if defined(__TIGER__)

	#define __NT__
	#include "tgcall.h"
	#include "lang_def.h"
	#include "puma.h"

	#define LANG_RUSENG     7

#elif defined(__PUMA__)

	#include "puma.h"

#endif

#include "Tiger.h"
#include "Recognition.h"

#include "crashrpt.h"

#define __EXTERN_INIT__
#include "tigerdef.h"

static char g_strTempPath[_MAX_PATH];

#define RCODE_VAL		hRetCode
#define RCODE_CLEAR		RCODE_VAL = S_OK; dwReturnCode = 0
#define RCODE_EXCEPTION RCODE_VAL = DISP_E_EXCEPTION; dwReturnCode = (uint32_t)-1

#define TEST_ON_LOAD	if(!IsLoad()){ SetError(IDS_ERROR_NOLOAD); return RCODE_VAL; }
#define TEST_ON_OPEN	if(!IsOpen()){ SetError(IDS_ERROR_NOOPEN); return RCODE_VAL; }


/////////////////////////////////////////
static int ProgressStep_dummy(unsigned long dStep, char * lpStepName, unsigned long dPercent)
{
	return TRUE;
}
static int ProgressStart_dummy()
{
	return TRUE;
}
static int ProgressStop_dummy()
{
	return TRUE;
}

FNPROGRESS_START fnProgressStart = ProgressStart_dummy;
FNPROGRESS_STOP  fnProgressStop = ProgressStop_dummy;
FNPROGRESS_STEP  fnProgressStep = ProgressStep_dummy;
/////////////////////////////////////////////////////////////////////////////
// CRecognition

STDAPI DllRegisterServer(void);

CRecognition::CRecognition()
{
	dwReturnCode = 0;
	bIsLoad = FALSE;
	bIsOpen = FALSE;
	hThread = NULL;
	DllRegisterServer(); //Восстанавливаем информацию об id интерфейсов в реестре,
	//а то она слетает, если зарегистрировать 2 pum'ы,а потом одну разрегистрировать.
}

CRecognition::~CRecognition()
{
	Bool rc;

	if(IsOpen())
		XClose(&rc);

	if(IsLoad())
		Unload();
}

void CRecognition::SetError(uint nID)
{
	char str[1024];
	if(LoadString(_Module.GetResourceInstance(),nID,str,sizeof(str))>0)
		SetError(str);
}

void CRecognition::SetError(char * str)
{
	USES_CONVERSION;
	ICreateErrorInfo * pErr;
	IErrorInfo *perrinfo;
	HRESULT hr;

	if(CreateErrorInfo(&pErr)==S_OK)
	{
		pErr->SetDescription(T2OLE(str));
		pErr->SetGUID(IID_IRecognition);
		pErr->SetSource(T2OLE("Cognitive.Tiger"));
		hr = pErr->QueryInterface(IID_IErrorInfo, (pvoid FAR*) &perrinfo);
		if (SUCCEEDED(hr))
		{
			SetErrorInfo(0, perrinfo);
			perrinfo->Release();
		}
		pErr->Release();
	}
	RCODE_EXCEPTION;
}

STDMETHODIMP CRecognition::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] =
	{
		&IID_IRecognition
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (::ATL::InlineIsEqualGUID(*arr[i], riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CRecognition::get_Language(short *pVal)
{
	// TODO: Add your implementation code here
	RCODE_CLEAR;
	TEST_ON_LOAD;
	EnterCriticalSection(&Critical);
	__try {
		g_uiLanguage = _GetLanguage();
	}
	__except(PassException(GetExceptionInformation()))
	{
		//SetError(IDS_ERROR_SYSTEM);
	}
	LeaveCriticalSection(&Critical);
	*pVal = (short) g_uiLanguage;
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::put_Language(short newVal)
{
	// TODO: Add your implementation code here
	RCODE_CLEAR;
	TEST_ON_LOAD;
	EnterCriticalSection(&Critical);
	__try{
		_SetLanguage(newVal);
		g_uiLanguage = _GetLanguage();
	}
	__except(PassException(GetExceptionInformation()))
	{
		//SetError(IDS_ERROR_SYSTEM);
	}
	LeaveCriticalSection(&Critical);
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::get_Speller(Bool *pVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	*pVal = g_bSpeller;
	return S_OK;
}

STDMETHODIMP CRecognition::put_Speller(Bool newVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	g_bSpeller = newVal;
	return S_OK;
}

STDMETHODIMP CRecognition::get_OneColumn(Bool *pVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	*pVal = g_bOneColumn;
	return S_OK;
}

STDMETHODIMP CRecognition::put_OneColumn(Bool newVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	g_bOneColumn = newVal;
	return S_OK;
}

STDMETHODIMP CRecognition::get_Fax100(Bool *pVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	*pVal = g_bFax100;
	return S_OK;
}

STDMETHODIMP CRecognition::put_Fax100(Bool newVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	g_bFax100 = newVal;
	return S_OK;
}

STDMETHODIMP CRecognition::get_DotMatrix(Bool *pVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	*pVal = g_bDotMatrix;
	return S_OK;
}

STDMETHODIMP CRecognition::put_DotMatrix(Bool newVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	g_bDotMatrix = newVal;
	return S_OK;
}

STDMETHODIMP CRecognition::get_UserDictName(BSTR *pVal)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	TEST_ON_LOAD;
	*pVal = T2BSTR(g_strUserDictName);
	return S_OK;
}

STDMETHODIMP CRecognition::put_UserDictName(BSTR newVal)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	TEST_ON_LOAD;
	strcpy(g_strUserDictName,OLE2T(newVal));
	return S_OK;
}

STDMETHODIMP CRecognition::get_Bold(Bool *pVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	*pVal = g_bBold;
	return S_OK;
}

STDMETHODIMP CRecognition::put_Bold(Bool newVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	g_bBold = newVal;
	return S_OK;
}

STDMETHODIMP CRecognition::get_Italic(Bool *pVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	*pVal = g_bItalic;
	return S_OK;
}

STDMETHODIMP CRecognition::put_Italic(Bool newVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	g_bItalic = newVal;
	return S_OK;
}

STDMETHODIMP CRecognition::get_Size(Bool *pVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	*pVal = g_bSize;
	return S_OK;
}

STDMETHODIMP CRecognition::put_Size(Bool newVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	g_bSize = newVal;
	return S_OK;
}

STDMETHODIMP CRecognition::get_Format(Bool *pVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	*pVal = g_bFormat;
	return S_OK;
}

STDMETHODIMP CRecognition::put_Format(Bool newVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	g_bFormat = newVal;
	return S_OK;
}

STDMETHODIMP CRecognition::get_SerifName(BSTR *pVal)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	TEST_ON_LOAD;
	*pVal = T2BSTR(g_strSerifName);
	return S_OK;
}

STDMETHODIMP CRecognition::put_SerifName(BSTR newVal)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	TEST_ON_LOAD;
	strcpy(g_strSerifName,OLE2T(newVal));
	return S_OK;
}

STDMETHODIMP CRecognition::get_SansSerifName(BSTR *pVal)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	TEST_ON_LOAD;
	*pVal = T2BSTR(g_strSansSerifName);
	return S_OK;
}

STDMETHODIMP CRecognition::put_SansSerifName(BSTR newVal)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	TEST_ON_LOAD;
	strcpy(g_strSansSerifName,OLE2T(newVal));

	return S_OK;
}

STDMETHODIMP CRecognition::get_CourierName(BSTR *pVal)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	TEST_ON_LOAD;
	*pVal = T2BSTR(g_strCourierName);
	return S_OK;
}

STDMETHODIMP CRecognition::put_CourierName(BSTR newVal)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	TEST_ON_LOAD;
	strcpy(g_strCourierName,OLE2T(newVal));
	return S_OK;
}

STDMETHODIMP CRecognition::get_ProgressStep(long *pVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	*pVal = (long)fnProgressStep;
	return S_OK;
}

STDMETHODIMP CRecognition::put_ProgressStep(long newVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	if(newVal == 0)
		fnProgressStep = ProgressStep;
	else
		fnProgressStep = (FNPROGRESS_STEP)newVal;
	return S_OK;
}

STDMETHODIMP CRecognition::get_ProgressStop(long *pVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	*pVal = (long)fnProgressStop;
	return S_OK;
}

STDMETHODIMP CRecognition::put_ProgressStop(long newVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	if(newVal == 0)
		fnProgressStop = ProgressStop;
	else
		fnProgressStop = (FNPROGRESS_STOP)newVal;

	return S_OK;
}

STDMETHODIMP CRecognition::get_ProgressStart(long *pVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	*pVal = (long)fnProgressStart;
	return S_OK;
}

STDMETHODIMP CRecognition::put_ProgressStart(long newVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	if(newVal == 0)
		fnProgressStart = ProgressStart;
	else
		fnProgressStart = (FNPROGRESS_START)newVal;

	return S_OK;
}

STDMETHODIMP CRecognition::GetReturnCode(long *rc)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	EnterCriticalSection(&Critical);
	__try{
		*rc = _Status();
	}
	__except(PassException(GetExceptionInformation()))
	{
		//SetError(IDS_ERROR_SYSTEM);
	}

	if(!*rc && dwReturnCode)
		*rc = dwReturnCode;

	LeaveCriticalSection(&Critical);
	return S_OK;
}

STDMETHODIMP CRecognition::GetReturnString(long rc, BSTR *str)
{
	// TODO: Add your implementation code here
	char * strResult;
	RCODE_CLEAR;
	USES_CONVERSION;
	TEST_ON_LOAD;

	EnterCriticalSection(&Critical);
	__try{
		strResult = _ErrorMessage(rc);
	}
	__except(PassException(GetExceptionInformation()))
	{
//		SetError(IDS_ERROR_SYSTEM);
	}
	LeaveCriticalSection(&Critical);

	*str = T2BSTR(strResult);
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::Load(Bool *rc)
{
	// TODO: Add your implementation code here
	RCODE_CLEAR;
	EnterCriticalSection(&Critical);
	LDPUMA_Init(0,0);
	__try {
		*rc = _Load(this);
		if(*rc)
		{
			g_uiLanguage = _GetLanguage();
		}
 		bIsLoad = *rc;

	}
	__except(PassException(GetExceptionInformation()))
	{
//		SetError(IDS_ERROR_SYSTEM);
	}

	LeaveCriticalSection(&Critical);
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::Unload()
{
	// TODO: Add your implementation code here
	RCODE_CLEAR;
	TEST_ON_LOAD;
	EnterCriticalSection(&Critical);
	__try{
		_Unload();
		bIsLoad = FALSE;
	}
	__except(PassException(GetExceptionInformation()))
	{
//		SetError(IDS_ERROR_SYSTEM);
	}
	LDPUMA_Done();
	LeaveCriticalSection(&Critical);
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::RecogDIBtoFile(long hDIB, BSTR FileName, short shFormat, short shCode, Bool * rc)
{
	// TODO: Add your dispatch handler code here
	RCODE_CLEAR;
	USES_CONVERSION;
	Bool h = FALSE;
	TEST_ON_LOAD;

	*rc = FALSE;

	EnterCriticalSection(&Critical);

	XOpen(hDIB,T2BSTR(""),&h);
	if(h)
	{
		XRecognition(FALSE,0,rc);
		if(*rc)
			XSave(FileName,(long)shFormat,(long)shCode,rc);
		XClose(rc);
	}

	LeaveCriticalSection(&Critical);
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::EnumLanguage(short PrevLang, short *lpNextLang)
{
	// TODO: Add your implementation code here
	RCODE_CLEAR;
	TEST_ON_LOAD;
	EnterCriticalSection(&Critical);
	__try{
		*lpNextLang = _EnumLanguage(PrevLang);
	}
	__except(PassException(GetExceptionInformation()))
	{
//		SetError(IDS_ERROR_SYSTEM);
	}
	LeaveCriticalSection(&Critical);
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::RecogDIBtoMemory(long hDIB,long *lphMem, short Format, short Code, Bool *rc)
{
	// TODO: Add your dispatch handler code here
	RCODE_CLEAR;
	USES_CONVERSION;
	char TempFile[_MAX_PATH];

	TEST_ON_LOAD;

	EnterCriticalSection(&Critical);

	strcpy(TempFile,_GetTempPath());
	strcat(TempFile,"temp.txt");
	__try{
		RecogDIBtoFile(hDIB,T2BSTR(TempFile),Format,Code,rc);
		if(*rc)
		{
		 FILE * f = fopen(TempFile,"rb");
		 if(f)
		 {
			uint size = _filelength(f->_file);

			*lphMem = (long)GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,size+1);
			if(*lphMem)
			{
			char * pData = (char *)GlobalLock((HANDLE)*lphMem);
				if(pData &&  fread(pData,1,size,f)==size)
				{
					pData[size] = '\0';
					*rc = TRUE;
				}
				else
				{
					SetError(IDS_ERROR_FILERESULT);
					*rc = FALSE;
				}
			GlobalUnlock((HANDLE)*lphMem);
			}
			else
			{
				SetError(IDS_ERROR_NOMEMORY);
				*rc = FALSE;
			}
			fclose(f);
			remove(TempFile);
		 } else
			{
				SetError(IDS_ERROR_FILERESULT);
				*rc = FALSE;
			}
		}
	}
	__except(PassException(GetExceptionInformation()))
	{
//		SetError(IDS_ERROR_SYSTEM);
	}
LeaveCriticalSection(&Critical);

return RCODE_VAL;
}

STDMETHODIMP CRecognition::RecogClipboard(Bool *rc)
{
	// TODO: Add your implementation code here
	char * hMem = NULL;
	RCODE_CLEAR;
	TEST_ON_LOAD;

	EnterCriticalSection(&Critical);

	OpenClipboard(GetActiveWindow());

	HANDLE hDIB = GetClipboardData(CF_DIB);
	if(hDIB)
	{
		RecogDIBtoMemory((long)hDIB, (long *)&hMem, 2, 2,rc);
		if(*rc)
		{
			EmptyClipboard();
			*rc = SetClipboardData(CF_TEXT,(HANDLE)hMem) == (HANDLE)hMem;
		}
	}
	else
		SetError(IDS_ERROR_CLIPBOARDEMPTY);
	CloseClipboard();

	LeaveCriticalSection(&Critical);
	return RCODE_VAL;
}

char * _GetTempPath( void )
{
	char szPath[_MAX_PATH];
	if(GetTempPath(sizeof(szPath),szPath))
	{
		strcpy(g_strTempPath,szPath);
		if(!strlen(g_strTempPath) &&
			g_strTempPath[strlen(g_strTempPath)-1]!='\\')
				strcat(g_strTempPath,"\\");
	}
	return g_strTempPath;
}

static HANDLE s_hDIB = NULL;

STDMETHODIMP CRecognition::XOpen(long hDIB, BSTR FileName, Bool *hRc)
{
	// TODO: Add your implementation code here
	RCODE_CLEAR;
	USES_CONVERSION;
	TEST_ON_LOAD;

	EnterCriticalSection(&Critical);
	__try{
		s_hDIB = (HANDLE)hDIB;
		char * pDIB = (char *)GlobalLock(s_hDIB);

		LDPUMA_CreateWindow(NULL,pDIB);

		*hRc = _Open(pDIB,OLE2T(FileName));
		bIsOpen = *hRc > 0;
	}
	__except(PassException(GetExceptionInformation()))
	{
//		SetError(IDS_ERROR_SYSTEM);
	}
	LeaveCriticalSection(&Critical);

	return RCODE_VAL;
}

STDMETHODIMP CRecognition::XClose( Bool *rc)
{
	// TODO: Add your implementation code here
	RCODE_CLEAR;
	TEST_ON_LOAD;
	TEST_ON_OPEN;

	EnterCriticalSection(&Critical);
	__try{
		_Close( );
		bIsOpen = FALSE;
		if(s_hDIB)
		{
			GlobalUnlock(s_hDIB);
			s_hDIB = NULL;
		}
	}
	__except(PassException(GetExceptionInformation()))
	{
//		SetError(IDS_ERROR_SYSTEM);
	}
	LeaveCriticalSection(&Critical);

	return RCODE_VAL;
}

void __cdecl RecognitionThread( void * p)
{
    //_ASSERT(SetThreadPriority(*rc,THREAD_PRIORITY_HIGHEST));
	EnterCriticalSection(&Critical);
		CRecognition * pThis = (CRecognition *)p;
		ResumeThread(pThis->hThread);
		Bool rc = _FinalRecognition();
		pThis->Fire_EndThread(rc,pThis->nContext);
	LeaveCriticalSection(&Critical);
	_endthread();
}

STDMETHODIMP CRecognition::XRecognition( Bool bThread, long context, Bool *rc)
{
	// TODO: Add your implementation code here
	RCODE_CLEAR;
	TEST_ON_LOAD;
	TEST_ON_OPEN;

	EnterCriticalSection(&Critical);
	nContext = context;
	if(bThread)
	{
		if(DuplicateHandle(
			GetCurrentProcess(),
			GetCurrentThread(),
			GetCurrentProcess(),
			&hThread,
			0,
			FALSE,
			DUPLICATE_SAME_ACCESS))
		{
			*rc = _beginthread(RecognitionThread,0x20000,this) >= 0;
		}
		else
		{
			*rc = FALSE;
			SetError(IDS_ERROR_NOCREATETHREAD);
			hThread = NULL;
		}
		if( !*rc )
			SetError(IDS_ERROR_NOCREATETHREAD);
	}
	else
	{
		__try {
			*rc = _FinalRecognition();
		}
		__except(PassException(GetExceptionInformation()))
		{
		//	SetError(IDS_ERROR_SYSTEM);
		}
	}
	LeaveCriticalSection(&Critical);

	if(bThread)
	{
		if(*rc)
			::SuspendThread(hThread);
		if(hThread)
			CloseHandle(hThread);
		hThread = NULL;
	}

	return RCODE_VAL;
}

STDMETHODIMP CRecognition::XSave( BSTR FileName, long format, long code, Bool *rc)
{
return Save(NULL, FileName, format, code, FALSE, rc);
}

STDMETHODIMP CRecognition::XPageAnalysis( Bool *rc)
{
	// TODO: Add your implementation code here
	RCODE_CLEAR;
	TEST_ON_LOAD;
	TEST_ON_OPEN;
	EnterCriticalSection(&Critical);
	__try {
		*rc = _PageAnalysis( TRUE );
	}
	__except(PassException(GetExceptionInformation()))
	{
	//	SetError(IDS_ERROR_SYSTEM);
	}
	LeaveCriticalSection(&Critical);
	return RCODE_VAL;
}


STDMETHODIMP CRecognition::GetLanguageString(long code, BSTR * lpstr)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	RCODE_CLEAR;
	char szName[160];

	switch(code)
	{
#if defined(__TIGER__)
	#define	CASE_LANG(lang)	case lang : if(LoadString(_Module.GetResourceInstance(),IDS_##lang,szName,sizeof(szName)))\
			{  *lpstr = T2BSTR(szName); } else {SetError(IDS_ERROR_NORESOURCE);} break;
#elif defined(__PUMA__)
	#define	CASE_LANG(lang)	case PUMA_##lang : if(LoadString(_Module.GetResourceInstance(),IDS_##lang,szName,sizeof(szName)))\
			{  *lpstr = T2BSTR(szName); } else {SetError(IDS_ERROR_NORESOURCE);} break;
#endif

		CASE_LANG( LANG_ENGLISH );
		CASE_LANG( LANG_GERMAN );
		CASE_LANG( LANG_FRENCH );
		CASE_LANG( LANG_RUSSIAN );
		CASE_LANG( LANG_SWEDISH );
		CASE_LANG( LANG_SPANISH );
		CASE_LANG( LANG_ITALIAN );
		CASE_LANG( LANG_UKRAINIAN );
		CASE_LANG( LANG_SERBIAN );
		CASE_LANG( LANG_CROATIAN );
		CASE_LANG( LANG_DANISH );
		CASE_LANG( LANG_PORTUGUESE );
		CASE_LANG( LANG_DUTCH );
		CASE_LANG( LANG_POLISH );
		CASE_LANG( LANG_RUSENG );
		CASE_LANG( LANG_DIG );
		CASE_LANG( LANG_UZBEK );	// 01.09.2000 E.P.
		CASE_LANG( LANG_KAZ );
		CASE_LANG( LANG_KAZ_ENG );
		CASE_LANG( LANG_CZECH );
		CASE_LANG( LANG_ROMAN );
		CASE_LANG( LANG_HUNGAR );
		CASE_LANG( LANG_BULGAR );
		CASE_LANG( LANG_SLOVENIAN ); // 03.12.2001 Nick
		CASE_LANG( LANG_LATVIAN	 );
		CASE_LANG( LANG_LITHUANIAN  );
		CASE_LANG( LANG_ESTONIAN	 );
		CASE_LANG( LANG_TURKISH	 );

#undef LANG_CASE
		default:
			SetError(IDS_ERROR_NORESOURCE);
	}

	return RCODE_VAL;
}

STDMETHODIMP CRecognition::EnumFormats(long prev, long * next)
{
	// TODO: Add your implementation code here
	RCODE_CLEAR;
	__try {
		*next = _EnumFormats(prev);
	}
	__except(PassException(GetExceptionInformation()))
	{
	//	SetError(IDS_ERROR_SYSTEM);
	}
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::GetFormatString(long code, BSTR * lpstr)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	RCODE_CLEAR;
	char szName[160];

	switch(code)
	{
#if defined(__TIGER__)
	#define CASE_FORMAT(format) case TIGER##format: if(LoadString(_Module.GetResourceInstance(),IDS##format,szName,sizeof(szName)))\
		{ *lpstr = T2BSTR(szName); } else {SetError(IDS_ERROR_NORESOURCE);} break;
#elif defined(__PUMA__)
	#define CASE_FORMAT(format) case PUMA##format: if(LoadString(_Module.GetResourceInstance(),IDS##format,szName,sizeof(szName)))\
		{ *lpstr = T2BSTR(szName); } else {SetError(IDS_ERROR_NORESOURCE);} break;
#endif
	CASE_FORMAT( _TOEDNATIVE  );
#if defined(__TIGER__)
	CASE_FORMAT( _TOASCII     );
	CASE_FORMAT( _TOSMARTASCII);
	CASE_FORMAT( _TOANSI      );
	CASE_FORMAT( _TOSMARTANSI );
#elif defined(__PUMA__)
	CASE_FORMAT( _TOTEXT      );
	CASE_FORMAT( _TOSMARTTEXT );
#ifdef _DEBUG
	CASE_FORMAT( _DEBUG_TOTEXT);
#endif
#endif
	CASE_FORMAT( _TORTF       );
	CASE_FORMAT( _TOTABLETXT  );
	CASE_FORMAT( _TOTABLECSV  );
	CASE_FORMAT( _TOTABLEDBF  );
	CASE_FORMAT( _TOTABLEODBC );
	CASE_FORMAT( _TOTABLEWKS  );
	CASE_FORMAT( _TOHTML      );
#undef CASE_FORMAT
	default:
			SetError(IDS_ERROR_NORESOURCE);
	}

	return RCODE_VAL;
}

STDMETHODIMP CRecognition::EnumCodes(long format, long prev, long *next)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	RCODE_CLEAR;
	__try {
		*next = _EnumCodes(format,prev);
	}
	__except(PassException(GetExceptionInformation()))
	{
	//	SetError(IDS_ERROR_SYSTEM);
	}
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::GetCodeString(long code, BSTR *lpstr)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	RCODE_CLEAR;
	char szName[160];

	switch(code)
	{
#if defined(__TIGER__)
	#define CASE_CODE(code) case TIGER##code: if(LoadString(_Module.GetResourceInstance(),IDS##code,szName,sizeof(szName)))\
		{ *lpstr = T2BSTR(szName); } else {SetError(IDS_ERROR_NORESOURCE);} break;
#elif defined(__PUMA__)
	#define CASE_CODE(code) case PUMA##code: if(LoadString(_Module.GetResourceInstance(),IDS##code,szName,sizeof(szName)))\
		{ *lpstr = T2BSTR(szName); } else {SetError(IDS_ERROR_NORESOURCE);} break;
#endif
	CASE_CODE( _CODE_UNKNOWN );
	CASE_CODE( _CODE_ASCII );
	CASE_CODE( _CODE_ANSI  );
	CASE_CODE( _CODE_KOI8  );
	CASE_CODE( _CODE_ISO   );
	CASE_CODE( _CODE_UTF8   );
#undef CASE_CODE
	default:
			SetError(IDS_ERROR_NORESOURCE);
	}
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::get_ModePicture(long *pVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	*pVal = g_nPicture;
	return S_OK;
}

STDMETHODIMP CRecognition::put_ModePicture(long newVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	g_nPicture = newVal;
	return S_OK;
}

STDMETHODIMP CRecognition::get_ModeTable(long *pVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	*pVal = g_nTable;
	return S_OK;
}

STDMETHODIMP CRecognition::put_ModeTable(long newVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	g_nTable = newVal;
	return S_OK;
}

STDMETHODIMP CRecognition::get_ModeFormat(long *pVal)
{
	// TODO: Add your implementation code here
 	TEST_ON_LOAD;
 	*pVal = g_nFormat;
 	return S_OK;
}

STDMETHODIMP CRecognition::put_ModeFormat(long newVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	g_nFormat = newVal;
	return S_OK;
}

STDMETHODIMP CRecognition::GetVersion(BSTR *bstrVersion)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	TEST_ON_LOAD;
	*bstrVersion = T2BSTR(_GetVersion());
	return S_OK;
}


STDMETHODIMP CRecognition::get_UnrecogChar(short *pVal)
{
	// TODO: Add your implementation code here
 	TEST_ON_LOAD;
 	*pVal = g_btUnrecogChar;
	return S_OK;
}

STDMETHODIMP CRecognition::put_UnrecogChar(short newVal)
{
	// TODO: Add your implementation code here
	TEST_ON_LOAD;
	g_btUnrecogChar = (char)newVal;
	return S_OK;
}

STDMETHODIMP CRecognition::EnumFormatMode(long nPrev, long * pMode)
{
	// TODO: Add your implementation code here
	RCODE_CLEAR;
	__try {
		*pMode = (long)_EnumFormatMode(nPrev);
	}
	__except(PassException(GetExceptionInformation()))
	{
	//	SetError(IDS_ERROR_SYSTEM);
	}
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::EnumTableMode(long nPrev, long * pMode)
{
	// TODO: Add your implementation code here
	RCODE_CLEAR;
	__try {
		*pMode = _EnumTableMode(nPrev);
	}
	__except(PassException(GetExceptionInformation()))
	{
	//	SetError(IDS_ERROR_SYSTEM);
	}
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::EnumPictureMode(long nPrev, long * pMode)
{
	// TODO: Add your implementation code here
	RCODE_CLEAR;
	__try {
		*pMode = _EnumPictureMode(nPrev);
	}
	__except(PassException(GetExceptionInformation()))
	{
	//	SetError(IDS_ERROR_SYSTEM);
	}
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::GetFormatModeString(long nMode, BSTR *lpbstr)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	RCODE_CLEAR;
	char szName[160];

#define CASE_CODE(code) case code: if(LoadString(_Module.GetResourceInstance(),IDS_##code,szName,sizeof(szName)))\
	{ *lpbstr = T2BSTR(szName); } else {SetError(IDS_ERROR_NORESOURCE);} break;
	switch(nMode)
	{
	CASE_CODE( PUMA_FORMAT_NONE );
	CASE_CODE( PUMA_FORMAT_ALL );
	CASE_CODE( PUMA_FORMAT_ONLY_FRAME  );
	default:
			SetError(IDS_ERROR_NORESOURCE);
	}
#undef CASE_CODE

	return RCODE_VAL;
}

STDMETHODIMP CRecognition::GetPictureModeString(long nMode, BSTR *lpbstr)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	RCODE_CLEAR;
	char szName[160];

#define CASE_CODE(code) case code: if(LoadString(_Module.GetResourceInstance(),IDS_##code,szName,sizeof(szName)))\
	{ *lpbstr = T2BSTR(szName); } else {SetError(IDS_ERROR_NORESOURCE);} break;
	switch(nMode)
	{
	CASE_CODE( PUMA_PICTURE_NONE );
	CASE_CODE( PUMA_PICTURE_ALL );
	default:
			SetError(IDS_ERROR_NORESOURCE);
	}
#undef CASE_CODE

	return RCODE_VAL;
}

STDMETHODIMP CRecognition::GetTableModeString(long nMode, BSTR *lpbstr)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	RCODE_CLEAR;
	char szName[160];

#define CASE_CODE(code) case code: if(LoadString(_Module.GetResourceInstance(),IDS_##code,szName,sizeof(szName)))\
	{ *lpbstr = T2BSTR(szName); } else {SetError(IDS_ERROR_NORESOURCE);} break;
	switch(nMode)
	{
	CASE_CODE( PUMA_TABLE_NONE );
	CASE_CODE( PUMA_TABLE_DEFAULT );
	CASE_CODE( PUMA_TABLE_ONLY_LINE  );
	CASE_CODE( PUMA_TABLE_ONLY_TEXT  );
	CASE_CODE( PUMA_TABLE_LINE_TEXT  );
	default:
			SetError(IDS_ERROR_NORESOURCE);
	}
#undef CASE_CODE

	return RCODE_VAL;
}

STDMETHODIMP CRecognition::XGetRotateDIB(long *phDIB, long *px0, long *py0, Bool *rc)
{
	// TODO: Add your implementation code here
	RCODE_CLEAR;

	*rc = _GetRotateDIB(phDIB, px0, py0);

	return RCODE_VAL;
}


STDMETHODIMP CRecognition::get_AutoRotate(long *pVal)
{
	// TODO: Add your implementation code here
	RCODE_CLEAR;
#if defined(__PUMA__)
	*pVal = LPUMA_GetAutoRotate();
#else
	SetError(IDS_ERR_NOTIMPLEMENT);
#endif
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::put_AutoRotate(long newVal)
{
	// TODO: Add your implementation code here
	RCODE_CLEAR;
#if defined(__PUMA__)
	 LPUMA_SetAutoRotate(newVal);
#else
	SetError(IDS_ERR_NOTIMPLEMENT);
#endif
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::SetPageSize(long width, long height)
{
	// TODO: Add your implementation code here
	RCODE_CLEAR;
#if defined(__PUMA__)
	LPUMA_SetPageSize(width, height);
#else
	SetError(IDS_ERR_NOTIMPLEMENT);
#endif
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::RenameImageFile(BSTR name)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	RCODE_CLEAR;
#if defined(__PUMA__)
	_RenameImageName(OLE2T(name));
#else
	SetError(IDS_ERR_NOTIMPLEMENT);
#endif
	return RCODE_VAL;
}
//////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CRecognition::XSetTemplate(long nLeft, long nTop, long nRight, long nBottom, Bool * rc)
{
	// TODO: Add your implementation code here
	RCODE_CLEAR;
	#if defined(__PUMA__)
	__try {
		Rect32 rect = {nLeft, nTop, nRight, nBottom};
		*rc = LPUMA_XSetTemplate(rect);
	}
	__except(PassException(GetExceptionInformation()))
	{
//		SetError(IDS_ERROR_SYSTEM);
	}
	#else
		*rc = FALSE;
		SetError(IDS_ERR_NOTIMPLEMENT);
	#endif
	return RCODE_VAL;
}
//////////////////////////////////////////////////////////////////////////////
int CRecognition::PassException(LPEXCEPTION_POINTERS  rc)
{
	//Создаем отчет об ошибке, отправляем рзработчикам
	//TODO Сделать его отключаемым
    char strMail[1024] = "", strMessage[1024] = "";
	::LoadString(_Module.GetResourceInstance(), IDS_CRASH_MAIL, strMail, sizeof(strMail));
	::LoadString(_Module.GetResourceInstance(), IDS_CRASH_CAPTION, strMessage, sizeof(strMessage));

	CrashHandler::StaticGenerateErrorReport(
	rc, NULL, strMail, strMessage);

	char name[_MAX_PATH];
	char ids[260];
 	char str[_MAX_PATH + sizeof(ids)];
	strcpy(str,"Unknown error.");

	uint32_t h = (uint32_t)rc->ExceptionRecord->ExceptionAddress & 0xFFFF0000;
	uint32_t s = 0;
	while(s = GetModuleFileName((HINSTANCE)h,name,sizeof(name)),(s==0 && h>0))
	{
		h -= 0x00010000;
	}
	if(!h)
		strcpy(name,"Unknown");
	else
	{
		char shortname[_MAX_PATH];
		char * lp = strrchr(name,'\\');
		if(lp)
		{
			strcpy(shortname,lp+1);
			strcpy(name,shortname);
		}
	}
	if(LoadString(_Module.GetResourceInstance(),IDS_ERR_EXCEPTION,ids,sizeof(ids)))
		sprintf(str,ids,name);

	SetError(str);

	return 1;
}

STDMETHODIMP CRecognition::get_CurrentEdPage(long *pVal)
{
	RCODE_CLEAR;
	#if defined(__PUMA__)
	__try {
		*pVal = (long)LPUMA_GetCurrentEdPage();
	}
	__except(PassException(GetExceptionInformation()))
	{
//		SetError(IDS_ERROR_SYSTEM);
	}
	#else
		SetError(IDS_ERR_NOTIMPLEMENT);
	#endif
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::put_CurrentEdPage(long newVal)
{
	RCODE_CLEAR;
	#if defined(__PUMA__)
	__try {
		LPUMA_SetCurrentEdPage((void *)newVal);
	}
	__except(PassException(GetExceptionInformation()))
	{
//		SetError(IDS_ERROR_SYSTEM);
	}
	#else
		SetError(IDS_ERR_NOTIMPLEMENT);
	#endif
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::get_PreserveLineBreaks(Bool *pVal)
{
	RCODE_CLEAR;
	#if defined(__PUMA__)
	__try {
		*pVal = LPUMA_GetPreserveLineBreaks();
	}
	__except(PassException(GetExceptionInformation()))
	{
//		SetError(IDS_ERROR_SYSTEM);
	}
	#else
		SetError(IDS_ERR_NOTIMPLEMENT);
	#endif
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::put_PreserveLineBreaks(Bool newVal)
{
	RCODE_CLEAR;
	#if defined(__PUMA__)
	__try {
		LPUMA_SetPreserveLineBreaks(newVal);
	}
	__except(PassException(GetExceptionInformation()))
	{
//		SetError(IDS_ERROR_SYSTEM);
	}
	#else
		SetError(IDS_ERR_NOTIMPLEMENT);
	#endif
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::Save(long hEdPage, BSTR FileName, long format, long code, Bool bAppend, Bool *rc)
{
	// TODO: Add your dispatch handler code here
	// TODO: Add your implementation code here
	USES_CONVERSION;
	RCODE_CLEAR;
	TEST_ON_LOAD;
	TEST_ON_OPEN;
	EnterCriticalSection(&Critical);
	__try {
		*rc = _Save((void*)hEdPage,OLE2T(FileName),format,code,bAppend);
	}
	__except(PassException(GetExceptionInformation()))
	{
	//	SetError(IDS_ERROR_SYSTEM);
	}
	LeaveCriticalSection(&Critical);
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::SaveToBSTR(long hEdPage, long format, long code, BSTR *lpBstr, Bool *rc)
{
	// TODO: Add your implementation code here
	USES_CONVERSION;
	RCODE_CLEAR;
	TEST_ON_LOAD;
	EnterCriticalSection(&Critical);
#if defined(__PUMA__)
	__try {
		char * lpMem = 0;
		// определим сколько нужно памяти
		uint32_t nSize = _SaveToMemory((Handle)hEdPage, format, code, lpMem, 0);
		if(nSize)
		{
			lpMem = new char [nSize];
			if(lpMem)
			{
				*rc = (nSize == _SaveToMemory((Handle)hEdPage, format, code, lpMem, nSize));
				*lpBstr = A2WBSTR(lpMem,nSize);
				delete [] lpMem;
			}
		}
	}
	__except(PassException(GetExceptionInformation()))
	{
//		SetError(IDS_ERROR_SYSTEM);
	}
#else
		SetError(IDS_ERR_NOTIMPLEMENT);
#endif
	LeaveCriticalSection(&Critical);
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::DeleteEdPage(long hEdPage)
{
	RCODE_CLEAR;
	#if defined(__PUMA__)
	__try {
		LCED_DeletePage((Handle)hEdPage);
	}
	__except(PassException(GetExceptionInformation()))
	{
//		SetError(IDS_ERROR_SYSTEM);
	}
	#else
		SetError(IDS_ERR_NOTIMPLEMENT);
	#endif
	return RCODE_VAL;
}

STDMETHODIMP CRecognition::SetSpecialProject(long nSpecPrj, /*[retval]*/ Bool * ret)
{
	TEST_ON_LOAD;
	EnterCriticalSection(&Critical);
	Bool32	rc;
	__try{
	rc = _SetSpecialProject(nSpecPrj);
	}

	__except(PassException(GetExceptionInformation()))
	{
		//SetError(IDS_ERROR_SYSTEM);
	}
	LeaveCriticalSection(&Critical);
	*ret = rc;
	return rc;
}

STDMETHODIMP CRecognition::XGetTemplate(long *lpnLeft, long *lpnTop, long *lpnRight, long *lpnBottom, Bool *rc)
{
	RCODE_CLEAR;
	#if defined(__PUMA__)
	__try {
		Rect32 rect;
		*rc = LPUMA_XGetTemplate(&rect);
		*lpnLeft=rect.left; *lpnTop=rect.top; *lpnRight=rect.right; *lpnBottom=rect.bottom;
	}
	__except(PassException(GetExceptionInformation()))
	{
//		SetError(IDS_ERROR_SYSTEM);
	}
	#else
		*rc = FALSE;
		SetError(IDS_ERR_NOTIMPLEMENT);
	#endif
	return RCODE_VAL;
}
