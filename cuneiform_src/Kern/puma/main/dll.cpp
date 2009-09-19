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

// ============================================================================
// Copyright (c) 1998 Cognitive Technology Ltd.
// Written by Peter Khlebutin
// This file creation date: 20.07.98
//
// dll_cpage.cpp :
// ============================================================================

/*#include <windows.h>*/
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "puma.h"

#include "resource.h"
#include "mpuma.h"
#include "pumadef.h"
#include "compat_defs.h"

//////////////////////////////////////////////////////////////////GLOBAL VARIABLES
static char				 s_szVersion[] = "Version OCR Puma "__DATE__".";
static Word16            gwHeightRC = 0;
static uint32_t            gwRC = 0;
static HANDLE            ghStorage = NULL;
static HINSTANCE         ghInst =  NULL;
static char				 szPath[_MAX_PATH] = ".";
static char				 szTempPath[_MAX_PATH] = ".";
static char				 szStorage[_MAX_PATH];
/////////////////////////////////////////
Bool APIENTRY DllMain( HINSTANCE  hModule,
uint32_t ul_reason_for_call,
                        LPVOID lpReserved )
{
	char * p;

    switch( ul_reason_for_call )
	{
    case DLL_PROCESS_ATTACH:
		ghInst = hModule;
		GetModuleFileName(hModule,szPath,sizeof(szPath));
		if(p = strstr(_strupr(szPath),"PUMA.DLL"))
			*(p-1)=0;
		else
		{
			MessageBox(NULL,"Start folder PUMA.DLL not found!",NULL,MB_ICONSTOP);
			return FALSE;
		}
		if(GetTempPath(sizeof(szTempPath),szTempPath))
			sprintf(szStorage,szFormatStorageName,szTempPath);
		else
		{
			MessageBox(NULL,"Temporary folder PUMA.DLL not found!",NULL,MB_ICONSTOP);
			return FALSE;
		}
		break;
    case DLL_THREAD_ATTACH:
		break;
    case DLL_THREAD_DETACH:
		break;
    case DLL_PROCESS_DETACH:
		break;
    }

    return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
PUMA_FUNC(Bool32) PUMA_Init(Word16 wHeightCode,HANDLE hStorage)
{
	gwHeightRC = wHeightCode;

	if(!InitMem())
		return FALSE;

	InitDebug();

 return ModulesInit((void*)ghStorage);
}
//////////////////////////////////////////////////////////////////////////////////
//
PUMA_FUNC(Bool32) PUMA_Done()
{
Bool32 rc = ModulesDone((void*)ghStorage);

	DoneDebug();

	DoneMem();

	return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
PUMA_FUNC(uint32_t) PUMA_GetReturnCode()
{
	return gwRC;
}
//////////////////////////////////////////////////////////////////////////////////
//
PUMA_FUNC(char *) PUMA_GetReturnString(uint32_t dwError)
{
	static char szBuffer[512];
	Word16 low = (Word16)(dwError &  0xFFFF);
	Word16 hei = (Word16)(dwError >> 16);

	if(hei == gwHeightRC)
	{
		if(!LoadString(ghInst,low + IDS_ERR_NO,(char *)szBuffer,sizeof(szBuffer)))
			LoadString(ghInst,IDS_ERR_NOTIMPLEMENT,(char *)szBuffer,sizeof(szBuffer));
	}
	else
	{
		char * p = GetModulesString(dwError);
		if(p)
			return p;
		LoadString(ghInst,IDS_ERR_NOTIMPLEMENT,(char *)szBuffer,sizeof(szBuffer));
	}

	return szBuffer;

}
//////////////////////////////////////////////////////////////////////////////////
//
PUMA_FUNC(Bool32) PUMA_GetExportData(uint32_t dwType, void * pData)
{
	Bool32 rc = TRUE;

	gwRC = 0;

#define CASE_FUNCTION(a)	case PUMA_FN##a:	*(FN##a *)pData = a; break
#define CASE_DATA(a,b,c)	case a: *(b *)pData = c; break

	switch(dwType)
	{
	CASE_FUNCTION(PUMA_XOpen);
	CASE_FUNCTION(PUMA_XClose);
	CASE_FUNCTION(PUMA_XPageAnalysis);
	CASE_FUNCTION(PUMA_XFinalRecognition);
	CASE_FUNCTION(PUMA_XSave);
	CASE_FUNCTION(PUMA_EnumLanguages);
	CASE_FUNCTION(PUMA_EnumFormats);
	CASE_FUNCTION(PUMA_EnumCodes);
	CASE_DATA(PUMA_Word32_Language,uint32_t,gnLanguage);
	CASE_DATA(PUMA_Bool32_Speller,Bool32,gbSpeller);
	CASE_DATA(PUMA_Bool32_OneColumn,Bool32,gbOneColumn);
	CASE_DATA(PUMA_Bool32_Fax100,Bool32,gbFax100);
	CASE_DATA(PUMA_Bool32_DotMatrix,Bool32,gbDotMatrix);
	CASE_DATA(PUMA_Bool32_Bold,Bool32,gbBold);
	CASE_DATA(PUMA_Bool32_Italic,Bool32,gbItalic);
	CASE_DATA(PUMA_Bool32_Size,Bool32,gbSize);
	CASE_DATA(PUMA_Bool32_Format,Bool32,gbFormat);
	CASE_DATA(PUMA_pchar_UserDictName,const char *,gpUserDictName);
	CASE_DATA(PUMA_pchar_SerifName,const char *,gpSerifName);
	CASE_DATA(PUMA_pchar_SansSerifName,const char *,gpSansSerifName);
	CASE_DATA(PUMA_pchar_CourierName,const char *,gpCourierName);
	CASE_DATA(PUMA_Word32_Pictures,uint32_t,gnPictures);
	CASE_DATA(PUMA_Word32_Tables,uint32_t,gnTables);
	CASE_DATA(PUMA_pchar_Version,char *,s_szVersion);
	CASE_DATA(PUMA_Word32_Format,Bool32,gnFormat);
	CASE_FUNCTION(PUMA_EnumFormatMode);
	CASE_FUNCTION(PUMA_EnumTable);
	CASE_FUNCTION(PUMA_EnumPicture);
	CASE_DATA(PUMA_Word8_Format,uchar,gnUnrecogChar);
	CASE_FUNCTION(PUMA_XGetRotateDIB);
	CASE_DATA(PUMA_Bool32_AutoRotate,Bool32,gbAutoRotate);
	CASE_FUNCTION(PUMA_RenameImageName);
	CASE_FUNCTION(PUMA_XSetTemplate);
	CASE_FUNCTION(PUMA_XGetTemplate);
	CASE_DATA(PUMA_Handle_CurrentEdPage,Handle,ghEdPage);
	CASE_FUNCTION(PUMA_Save);
	CASE_DATA(PUMA_Bool32_PreserveLineBreaks,Bool32,gnPreserveLineBreaks);
	CASE_FUNCTION(PUMA_XOpenClbk);
    CASE_FUNCTION(PUMA_GetSpecialBuffer);
	CASE_FUNCTION(PUMA_SetSpecialProject);
	case PUMA_LPPUMAENTRY_CED:
		{
			LPPUMAENTRY lp = (LPPUMAENTRY)pData;
			lp->fnInit = CED_Init;
			lp->fnDone = CED_Done;
			lp->fnGetReturnCode = CED_GetReturnCode;
			lp->fnGetReturnString = CED_GetReturnString;
			lp->fnGetExportData = CED_GetExportData;
			lp->fnSetImportData = CED_SetImportData;
		}
		break;
	case PUMA_LPPUMAENTRY_ROUT:
		{
			LPPUMAENTRY lp = (LPPUMAENTRY)pData;
			lp->fnInit = ROUT_Init;
			lp->fnDone = ROUT_Done;
			lp->fnGetReturnCode = ROUT_GetReturnCode;
			lp->fnGetReturnString = (FNGetReturnString)ROUT_GetReturnString;
			lp->fnGetExportData = ROUT_GetExportData;
			lp->fnSetImportData = ROUT_SetImportData;
		}
		break;
	CASE_FUNCTION(PUMA_SaveToMemory);
	 default:
		*(Handle *)pData = NULL;
		SetReturnCode_puma(IDS_ERR_NOTIMPLEMENT);
		rc = FALSE;
	}
#undef CASE_FUNCTION
#undef CASE_DATA

return rc;
}

//////////////////////////////////////////////////////////////////////////////////
PUMA_FUNC(Bool32) PUMA_SetImportData(uint32_t dwType, void * pData)
{
	Bool32 rc = TRUE;

	gwRC = 0;

#define CASE_DATA(a,b,c)		case a: c = *(b *)pData; break
#define CASE_DATAUP(a,b,c,d)	case a: if(c != *(b *)pData){c = *(b *)pData; SetUpdate(d,FLG_UPDATE_NO);}; break
#define CASE_PDATA(a,b,c)		case a: c = (b)pData; break

	switch(dwType)
	{
	CASE_DATAUP(PUMA_Word32_Language,uint32_t,gnLanguage,FLG_UPDATE_CCOM);
	CASE_DATA(PUMA_Bool32_Speller,Bool32,gbSpeller);
	CASE_DATAUP(PUMA_Bool32_OneColumn,Bool32,gbOneColumn,FLG_UPDATE_CPAGE);
	CASE_DATAUP(PUMA_Bool32_Fax100,Bool32,gbFax100,FLG_UPDATE_CCOM);
	CASE_DATAUP(PUMA_Bool32_DotMatrix,Bool32,gbDotMatrix,FLG_UPDATE_CCOM);
	CASE_DATA(PUMA_Bool32_Bold,Bool32,gbBold);
	CASE_DATA(PUMA_Bool32_Italic,Bool32,gbItalic);
	CASE_DATA(PUMA_Bool32_Size,Bool32,gbSize);
	CASE_DATA(PUMA_Bool32_Format,Bool32,gbFormat);
	CASE_PDATA(PUMA_pchar_UserDictName,char *,gpUserDictName);
	CASE_PDATA(PUMA_pchar_SerifName,char *,gpSerifName);
	CASE_PDATA(PUMA_pchar_SansSerifName,char *,gpSansSerifName);
	CASE_PDATA(PUMA_pchar_CourierName,char *,gpCourierName);
	CASE_DATAUP(PUMA_Word32_Pictures,uint32_t,gnPictures,FLG_UPDATE_CPAGE);
	CASE_DATAUP(PUMA_Word32_Tables,uint32_t,gnTables,FLG_UPDATE_CPAGE);
	CASE_DATA(PUMA_Word32_Format,Bool32,gnFormat);
	CASE_DATA(PUMA_Word8_Format,uchar,gnUnrecogChar);
	CASE_PDATA(PUMA_FNPUMA_ProgressStart, FNPUMA_ProgressStart ,fnProgressStart);
	CASE_PDATA(PUMA_FNPUMA_ProgressFinish,FNPUMA_ProgressFinish,fnProgressFinish);
	CASE_PDATA(PUMA_FNPUMA_ProgressStep,  FNPUMA_ProgressStep  ,fnProgressStep);
	CASE_DATAUP(PUMA_Bool32_AutoRotate,Bool32,gbAutoRotate,FLG_UPDATE);
	CASE_DATA(PUMA_Handle_CurrentEdPage,Handle,ghEdPage);
	 default:
		SetReturnCode_puma(IDS_ERR_NOTIMPLEMENT);
		rc = FALSE;
	}
	// Связь с предыдущими версиями
	switch(dwType)
	{
	case PUMA_Bool32_Format:
		gnFormat = gbFormat    ? gnFormat : 64;
		break;
	case PUMA_Word32_Format:
		gbFormat = gnFormat==64 ? 0 : 1;
		break;
	}

#undef CASE_DATA
#undef CASE_PDATA

return rc;
}

void SetReturnCode_puma(uint32_t rc)
{
Word16 low = (Word16)(rc &  0xFFFF);
Word16 hei = (Word16)(rc >> 16);

	if(hei)
		gwRC = rc;
	else
	{
		if(low >= IDS_ERR_NO)
			gwRC = (uint32_t)(gwHeightRC<<16)|(low - IDS_ERR_NO);
		else
			gwRC = low;
	}

	if(low > 0 && low != IDS_ERR_NO)
		LDPUMA_Console("%s\n",GetModulesString(gwRC));

	if(gwRC == nDebugReturnCode &&
		nDebugReturnCode)
			LDPUMA_Stop();
}

uint32_t GetReturnCode_puma()
{
uint32_t rc = gwRC;
Word16 low = (Word16)(gwRC &  0xFFFF);
Word16 hei = (Word16)(gwRC >> 16);

	if(hei == gwHeightRC || hei == 0)
		rc = low + IDS_ERR_NO;

return rc;
}
////////////////////////////////////////////////////////////
char * GetModulePath()
{
	return szPath;
}
////////////////////////////////////////////////////////////
char *	GetModuleTempPath()
{
	return szTempPath;
}
////////////////////////////////////////////////////////////
char * GetResourceString(uint32_t id)
{
    static char szBuffer[1024] = "";
	LoadString(ghInst,id,szBuffer,sizeof(szBuffer));
	return szBuffer;
}
//////////////////////////////////////////////////////////////////////////////////
//end of file
