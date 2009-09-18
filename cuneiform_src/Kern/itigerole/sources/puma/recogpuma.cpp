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

#include "stdafx.h"
#if defined(__PUMA__)

#include <io.h>
#include <stdio.h>
#include "puma.h"
#include "tigerdef.h"

extern FNPROGRESS_START fnProgressStart;
extern FNPROGRESS_STOP  fnProgressStop;
extern FNPROGRESS_STEP  fnProgressStep;


#define PUMANAME     "APUMA.DLL"


static void ProgressStart( void )
{
    fnProgressStart();
	g_pRecognition->Fire_ProgressStart();
}
static void ProgressFinish( void )
{
    fnProgressStop();
	g_pRecognition->Fire_ProgressFinish();
}
static Bool32 ProgressStep( uint32_t swStageNumber,const char* StageName, uint32_t swPercent )
{
	USES_CONVERSION;
    return fnProgressStep(swStageNumber,(char *)StageName,swPercent) &
		(Bool32)g_pRecognition->Fire_ProgressStep(swStageNumber,T2OLE((char *)StageName),swPercent);
}


char * _GetVersion()
{
	return LPUMA_GetVersion();
}

short _GetLanguage()
{
	short rc = 0;

	rc = (short)LPUMA_GetLanguage();

	return rc;
}

Bool32 _SetSpecialProject(long nSpecPrj)
{
	Bool32 rc = 0;

	rc = (Bool32)LPUMA_SetSpecialProject((uchar)nSpecPrj);

	return rc;
}

void _SetLanguage(short newVal)
{
	LPUMA_SetLanguage((uint32_t)newVal);
}

long _Status()
{
	long rc = 0;
	rc = LPUMA_GetReturnCode();
	return rc;
}

char * _ErrorMessage(long rc)
{
	static char szStr[2048];
	sprintf(szStr,"<0x%08X>: %s",rc,LPUMA_GetReturnString((uint32_t)rc ));
	return szStr;
}


Bool _Load(CRecognition * pThis)
{
	HINSTANCE hInstance = GetModuleHandle(PUMANAME);
	char szFullPath[_MAX_PATH];
	GetModuleFileName(hInstance,szFullPath,sizeof(szFullPath));
	char * p = strstr(strupr(szFullPath),PUMANAME);
	if(p)
		*p='\0';

	Bool rc = LPUMA_Load(szFullPath);

	g_pRecognition = pThis;
	if(!rc)
	{
		g_pRecognition->SetError(_ErrorMessage(_Status()));
		return rc;
	}

	if(rc)
		LPUMA_SetProgressFunction(ProgressStart,ProgressStep,ProgressFinish);

	return rc;
}

void _Unload()
{
	LPUMA_Unload();
}


Bool _Open(char * hDIB, char * FileName)
{
	Bool rc = FALSE;

	_Close( );
	if(hDIB==0)
	{
		g_pRecognition->SetError(IDS_ERROR_DIBISNULL);
		return FALSE;
	}
	rc = LPUMA_XOpen(hDIB, FileName);
	return rc;
}

Bool _Close()
{
	return LPUMA_XClose();
}

Bool _Recognition()
{
return  _FinalRecognition();
}

Bool _Save(void * hEdPage, char * lpFileName,long format, long code, Bool32 bAppend)
{
	Bool rc = FALSE;

	LPUMA_SetBold(g_bBold);
	LPUMA_SetItalic(g_bItalic);
	LPUMA_SetSize(g_bSize);
	LPUMA_SetFormat(g_bFormat);
	LPUMA_SetSerifName(g_strSerifName);
	LPUMA_SetSansSerifName(g_strSansSerifName);
	LPUMA_SetCourierName(g_strCourierName);

	rc = LPUMA_Save(hEdPage, lpFileName,format,code,bAppend);
	if(!rc)
		g_pRecognition->SetError(_ErrorMessage(_Status()));

	return rc;
}

uint32_t _SaveToMemory(Handle hEdPage, int32_t format, int32_t code, char * lpMem, uint32_t size )
{
	Bool rc = 0;

	LPUMA_SetBold(g_bBold);
	LPUMA_SetItalic(g_bItalic);
	LPUMA_SetSize(g_bSize);
	LPUMA_SetFormat(g_bFormat);
	LPUMA_SetSerifName(g_strSerifName);
	LPUMA_SetSansSerifName(g_strSansSerifName);
	LPUMA_SetCourierName(g_strCourierName);

	rc = LPUMA_SaveToMemory(hEdPage,format, code, lpMem, size);
	if(!rc)
		g_pRecognition->SetError(_ErrorMessage(_Status()));

	return rc;
}

Bool _PageAnalysis(Bool bUpdateCPAGE)
{
	Bool rc = FALSE;

	LPUMA_SetSpeller(g_bSpeller);
	LPUMA_SetOneColumn(g_bOneColumn);
	LPUMA_SetFax100(g_bFax100);
	LPUMA_SetDotMatrix(g_bDotMatrix);
	LPUMA_SetPictures(g_nPicture);
	LPUMA_SetTables(g_nTable);

	rc = LPUMA_XPageAnalysis();
	if(!rc)
	{
		uint32_t status = _Status();
		if(status)
			g_pRecognition->SetError(_ErrorMessage(status));
	}
	return rc;
}

Bool _FinalRecognition()
{
Bool rc = FALSE;

	LPUMA_SetSpeller(g_bSpeller);
	LPUMA_SetOneColumn(g_bOneColumn);
	LPUMA_SetFax100(g_bFax100);
	LPUMA_SetDotMatrix(g_bDotMatrix);
	LPUMA_SetPictures(g_nPicture);
	LPUMA_SetTables(g_nTable);
	LPUMA_SetFormatMode(g_nFormat);
	LPUMA_SetUnrecogChar(g_btUnrecogChar);

	LPUMA_SetBold(g_bBold);
	LPUMA_SetItalic(g_bItalic);
	LPUMA_SetSize(g_bSize);
	LPUMA_SetFormat(g_bFormat);
	LPUMA_SetSerifName(g_strSerifName);
	LPUMA_SetSansSerifName(g_strSansSerifName);
	LPUMA_SetCourierName(g_strCourierName);

	rc = LPUMA_XFinalRecognition();
	if(!rc)
	{
		uint32_t status = _Status();
		if(status)
			g_pRecognition->SetError(_ErrorMessage(status));
	}

return rc;
}

long	_EnumFormats(long prev)
{
	return LPUMA_EnumFormats((int32_t)prev);
}

long	_EnumCodes(long f,long prev)
{
	return (long)LPUMA_EnumCodes((int32_t)f,(int32_t)prev);
}

short  _EnumLanguage(short PrevLang)
{
	short rc = -1;
	rc = (short)LPUMA_EnumLanguages((int32_t)PrevLang);
	return rc;
}

long	_EnumFormatMode(long nPrev)
{
	long rc = -1;
	rc = LPUMA_EnumFormatMode(nPrev);
	return rc;
}

long	_EnumTableMode(long nPrev)
{
	long rc = -1;
	rc = LPUMA_EnumTable(nPrev);
	return rc;
}

long	_EnumPictureMode(long nPrev)
{
	long rc = -1;
	rc = LPUMA_EnumPicture(nPrev);
	return rc;
}

Bool	_GetRotateDIB(long * phDIB, long * x0, long * y0)
{
	Bool rc = FALSE;
	Point32 p = {0,0};

	rc = LPUMA_XGetRotateDIB((void**)phDIB,&p);

	*x0 = p.x;
	*y0 = p.y;

	return rc;
}

void	_RenameImageName(char * FileName)
{
	LPUMA_RenameImageName(FileName);
}

#endif
