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
/*  Редакция   :  06.03.01                                                    */
/*  Файл       :  'Util_Line_AM.CPP'                                          */
/*  Содержание :  Утиль работы с линиями моего формата.                       */
/*  Назначение :  Формальности для работы с унифицированным буфером.          */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*#include <windows.h>*/
/*  interface my        */
#include "linedesc.h"
#include "markdatadefs.h"
/*  interface our  */
#include "skew1024.h"
/*  interface my-my      */
#include "am_comm.h"
#include "un_buff.h"
#include "un_err.h"
#include "line_am_util.h" //own functions
/*----------------------------------------------------------------------------*/
void LastForLinesAM (void *vB)
{
	UN_BUFF *pB;
	RLT_LINEPOOL *pLinePool;
	pB = (UN_BUFF *)vB;
	pLinePool = (RLT_LINEPOOL *)pB->vPart[pB->nPart-2];
	pLinePool->nLine = pB->nPartUnits[pB->nPart-1];
}
/*----------------------------------------------------------------------------*/
Bool ReferForLinesAM (void *vLinePool, void *vB, int Aim)
{
	int Ind;
	UN_BUFF *pB;
	RLT_LINEPOOL *pLinePool;
	pLinePool = (RLT_LINEPOOL *)vLinePool;
	pB = (UN_BUFF *)vB;
	Ind = FindSuchAimedData (vB, UN_DT_RltLine, Aim);
	if (Ind < 0)
		return FALSE;
	pLinePool->pLine = (RLT_LINE *)pB->vPart[Ind];
	return TRUE;
}
/*---------------------------------------------------------------------------*/
Bool TakeFixedPlace_rv (void *vB, int Type, int Aim, char *pStr)
{
	UN_BUFF *pB;
	int nTeor;
	pB = (UN_BUFF *)vB;
	switch ((UN_DATATYPES)Type)
	{
		case UN_DT_RltLinePool :
			nTeor = sizeof (RLT_LINEPOOL);
			if (nTeor > pB->SizeCurr)
			{
				sprintf (pStr, "Не хватило памяти под %s!", "1 структуру RLT_LINEPOOL");
				return RV_DOUBT;
			}
			memset (pB->vCurr, 0, nTeor);
			EndLastDataPart (vB, Aim, Type, nTeor, 1);
			return RV_TRUE;
		default :
			return RV_FALSE;
	}
}
/*----------------------------------------------------------------------------*/
Bool PrepareLinesAM_rv (int Aim, void *vB, void **vvData)
{
	int Ind;
	Bool ret;
	UN_BUFF *pB;
	pB = (UN_BUFF *)vB;
	*vvData = NULL;
	Ind = FindSuchAimedData (vB, UN_DT_RltLinePool, Aim);
	if (Ind < 0)
		return RV_EMPTY;
	*vvData = pB->vPart[Ind];
	ret = ReferForLinesAM (*vvData, vB, Aim);
	return ret;
}
/*----------------------------------------------------------------------------*/
void MarkLineType (void *vLine, Bool Hori, Bool ContWarn)
{
	RLT_LINE *pLine = (RLT_LINE *)vLine;
	pLine->Type = RLT_LT_Line;
	if (abs(pLine->Skew)>256)
	{
		pLine->Type |= RLT_LT_Kosaya;
		if (ContWarn)
		{
          #ifdef Almi
			if (Hori)
				AM_Console ("Rlt-Warning-Continue : Обнаружена косая горизонтальная линия!");
			else
				AM_Console ("Rlt-Warning-Continue : Обнаружена косая вертикальная линия!");
          #endif
			if (Hori)
			{
				if (pLine->Skew < 0)
				{
					pLine->Type |= RLT_LT_Vertic;
                #ifdef Almi
					if (ContWarn)
						AM_Console ("Rlt-Warning-Continue : Косая горизонтальная линия будет описана как косая вертикальная!");
                #endif
				}
			}
			else
			{
				if (pLine->Skew > 0)
				{
					pLine->Type |= RLT_LT_Vertic;
				}
				else
				{
                 #ifdef Almi
					if (ContWarn)
						AM_Console ("Rlt-Warning-Continue : Косая вертикальная линия будет описана как косая горизонтальная!");
                 #endif
				}
			}
		}
	}
	else
		if (!Hori)
			pLine->Type |= RLT_LT_Vertic;
	if (pLine->Len>=128)
		pLine->Type |= RLT_LT_Long;
}
/*----------------------------------------------------------------------------*/
