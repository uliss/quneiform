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

/**********  Заголовок  **********/
/*  Автор      :  Александр Михайлов                                          */
/*  Редакция   :  18.12.00                                                    */
/*  Файл       :  'Pum_COMM.CPP'                                              */
/*  Содержание :  Реализация в рамках конкретного внешнего проекта (PUMA)     */
/*                всех используемых в содержательной части моих библиотек     */
/*                при отладке функций ввода/вывода.                           */
/*  Назначение :  Обмен с консолью.                                           */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
/*#include <windows.h>*/
/*  interface our-other  */
#include "dpuma.h"
/*  interface my        */
#define _AM_Comm_
#include "am_comm.h"  // Own functions
#include "am_err.h"
/*------------own objects-----------------------------------------------------*/
static uint32_t wRltMainCode;
#define     MaxResCodes   3
static FILE *fRes[MaxResCodes];
static int   cRes[MaxResCodes];
/*----------------------------------------------------------------------------*/
FILE * MyOpenFile (int k, const char *pFile)
{
	char Name[256], a[256];
	strcpy (Name, pFile);
	a[0] = '0' + (char)k;
	a[1] = '\0';
	strcat (Name, a);
	strcpy (a, ".txt");
	strcat (Name, a);
	return fopen (Name, "a");
}
/*----------------------------------------------------------------------------*/
int AM_OpenRes_rv_fte (int Code, const char *pFile)
{
	int i, k;
	for (i=0, k=-1; i<MaxResCodes; i++)
	{
		if ((fRes[i]!=NULL)&&(cRes[i]==Code))
			return RV_TRUE;
		if ((fRes[i]!=NULL)||(cRes[i]!=-1))
			continue;
		k = i;
		break;
	}
	if (k==-1)
		return RV_EMPTY;
	fRes[k] = MyOpenFile (k, pFile);
	if (fRes[k]==NULL)
		return RV_FALSE;
	cRes[k] = Code;
	return RV_TRUE;
}
/*----------------------------------------------------------------------------*/
int AM_SaveRes_rv_fte (int Code, const char *pFile)
{
	int i, k;
	for (i=0, k=-1; i<MaxResCodes; i++)
	{
		if ((fRes[i]==NULL)||(cRes[i]!=Code))
			continue;
		k = i;
		break;
	}
	if (k==-1)
		return RV_FALSE;
	fclose (fRes[k]);
	fRes[k] = MyOpenFile (k, pFile);
	if (fRes[k]==NULL)
		return RV_FALSE;
	return RV_TRUE;
}
/*----------------------------------------------------------------------------*/
int AM_WriteRes_rv_fte (int Code, const char *pStr)
{
	int i, k;
	for (i=0, k=-1; i<MaxResCodes; i++)
	{
		if ((fRes[i]==NULL)||(cRes[i]!=Code))
			continue;
		k = i;
		break;
	}
	if (k==-1)
		return RV_FALSE;
	fprintf (fRes[k], "%s", pStr);
	return RV_TRUE;
}
/*----------------------------------------------------------------------------*/
void CloseAllRes ()
{
	int i;
	for (i=0; i<MaxResCodes; i++)
	{
		if (fRes[i]==NULL)
			continue;
		fclose (fRes[i]);
		fRes[i] = NULL;
		cRes[i] =-1;
	}
}
/*----------------------------------------------------------------------------*/
Bool AM_InitComm (uint16_t wRltOwnCode)
{
	int i;
	wRltMainCode = (uint32_t)(wRltOwnCode<<16);
	LDPUMA_Init(0,NULL);
	AM_Console = (FN_AM_Console)LDPUMA_Console;
	AM_ConsolN = (FN_AM_Console)LDPUMA_ConsoleN;
	AM_MessageBoxOk = LDPUMA_MessageBoxOk;
	for (i=0; i<MaxResCodes; i++)
	{
		fRes[i] = NULL;
		cRes[i] = -1;
	}
	return TRUE;
}
/*----------------------------------------------------------------------------*/
void AM_DoneComm ()
{
	Bool ret;
	ret = LDPUMA_Done();
	return;
}
/*----------------------------------------------------------------------------*/
uint32_t AM_GetOperCode (uint16_t wLocCode)
{
	return (wRltMainCode|(wLocCode<<16));
}
/*----------------------------------------------------------------------------*/
void AM_RegiVert (Handle owner, const char *lpName, Handle parent)
{
	LDPUMA_Registry (owner, lpName, parent);
}
/*----------------------------------------------------------------------------*/
void AM_RegiVari (Handle owner, char *lpName, void *lpData, char *lpType)
{
	LDPUMA_RegVariable (owner, lpName, lpData, lpType);
}
/*----------------------------------------------------------------------------*/
void AM_RegiHelp (Handle owner, const char *lpName, Bool32 bAppend)
{
	LDPUMA_RegistryHelp (owner, lpName, bAppend);
}
/*----------------------------------------------------------------------------*/
uint32_t  AM_WaitUserInput (Handle owner, Handle wnd)
{
	return LDPUMA_WaitUserInput (owner, wnd);
}
/*----------------------------------------------------------------------------*/
void  AM_DrawLine (Handle wnd, Point16 *start, Point16 *end, int32_t skew, uint32_t rgb, int16_t pen, uint32_t key)
{
	LDPUMA_DrawLine (wnd, start, end, skew, rgb, pen, key);
}
/*----------------------------------------------------------------------------*/
void  AM_DrawRect (Handle wnd, Rect16 *pRc, int32_t skew, uint32_t rgb, int16_t pen, uint32_t key)
{
	LDPUMA_DrawRect (wnd, pRc, skew, rgb, pen, key);
}
/*----------------------------------------------------------------------------*/
void  AM_DeleteLines (Handle wnd, uint32_t key)
{
	LDPUMA_DeleteLines (wnd, key);
}
/*----------------------------------------------------------------------------*/
void  AM_DeleteRects (Handle wnd, uint32_t key)
{
	LDPUMA_DeleteRects (wnd, key);
}
/*----------------------------------------------------------------------------*/
Bool16 AM_Skip (Handle owner)
{
	return LDPUMA_Skip (owner);
}
/*----------------------------------------------------------------------------*/
Handle AM_CreateWindow (const char *lpTitle, void *lpDib)
{
	return LDPUMA_CreateWindow (lpTitle, lpDib);
}
/*----------------------------------------------------------------------------*/
Handle AM_GetWindowHandle (const char *name)
{
	return LDPUMA_GetWindowHandle (name);
}
/*----------------------------------------------------------------------------*/
char * AM_GetFileName (Handle wnd)
{
	return LDPUMA_GetFileName (wnd);
}
/*----------------------------------------------------------------------------*/
/*void AM_Console (const char *message, ...)
{
	int rc = 0;
	if (LDPUMA_Console)
	{
		va_list marker;
		va_start (marker, message);
		rc = LDPUMA_Console (message, marker);
		va_end (marker);
	}
//	return rc;
}*/
/*----------------------------------------------------------------------------*/
