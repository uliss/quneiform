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

/**********  Заголовок  ****************************************************************************/
/*  Автор,                                                                                         */
/*  комментарии                                                                                    */
/*  правка     : Алексей Коноплёв                                                                  */
/*  Редакция   :  08.09.00                                                                         */
/*  Файл       :  'AboutLines.Buffer.CPP'                                                          */
/*  Содержание :  Функции работы с памятью.                                                        */
/*  Назначение :  Оптимизация работы с памятью.                                                    */
/*  Комментарий:  выдрана Михайловым из таблиц                                                     */
/*                                                                                                 */
/*-------------------------------------------------------------------------------------------------*/
#include <stdio.h>
/*#include <windows.h>*/
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "rsdefines.h"
#include "rsmemory.h"
#include "rsfunc.h"
#include "resource.h"
/*  interface my         */
#include "puma_err.h"
#include "aboutlines.buffer.h"
/*  interface my-my      */
#include "un_buff.h"
#include "un_err.h"
/*------------extern functions------------------------------------------------*/
Bool GetSomeKeys_rv (void *vB, Handle *pKeyHor, Handle *pKeyVer
					 , int *pCntHor, int *pCntVer, char *pStr);
Bool LoadLinesTotalInfo_rv (Handle hC, void *vB, char *pStr);
Bool LoadLinesSpecInfo (Handle hC, void *vB, Handle Key, int Cnt);
Bool LoadComps_rv (Handle hC, void *vB, char *pStr, int Filter);
Bool ReferForLinesAM (void *vLinePool, void *vB, int Aim);
Bool ReferForLinesVP (void *vLti, void *vB);
/*----------------------------------------------------------------------------*/
Bool GetComplexData_rv (int Type, int Aim, void *vB, void **vvData)
{
	int Ind;
	Bool ret;
	UN_BUFF *pB;
	pB = (UN_BUFF *)vB;
	*vvData = NULL;
	switch ((UN_LOADDATA)Type)
	{
		case UN_LD_LinesVP :
			Ind = FindSuchData (vB, UN_DT_LinesTotalInfo);
			if (Ind < 0)
				return RV_EMPTY;
			*vvData = pB->vPart[Ind];
			ret = ReferForLinesVP (*vvData, vB);
			return ret;
		case UN_LD_LinesAM :
			Ind = FindSuchAimedData (vB, UN_DT_RltLinePool, Aim);
			if (Ind < 0)
				return RV_EMPTY;
			*vvData = pB->vPart[Ind];
			ret = ReferForLinesAM (*vvData, vB, Aim);
			return ret;
		default :
			return RV_FALSE;
	}
}
/*----------------------------------------------------------------------------*/
Bool LoadData_rv (Handle hC, int Type, void *vB, char *pStr, int Filter)
{
	Bool ret;
	Handle KeyHor, KeyVer; //// а ключи надо бы запомнить!
	int CntHor, CntVer;
	UN_BUFF *pB;
	pB = (UN_BUFF *)vB;
	switch ((UN_LOADDATA)Type)
	{
		case UN_LD_LinesVP :
			/***  Общая информация о линиях  ***/
			ret = LoadLinesTotalInfo_rv (hC, vB, pStr); //f-t-e-d
			if (ret!=RV_TRUE)
				return ret;
			ret = GetSomeKeys_rv (vB, &KeyHor, &KeyVer, &CntHor, &CntVer, pStr); //t-e-d
			if (ret!=RV_TRUE)
			{
				CleanLastDataPart (vB);
				return ret;
			}
			/***  Горизонтальные линии  ***/
			if (CntHor>0)
			{
				ret = LoadLinesSpecInfo (hC, vB, KeyHor, CntHor);
				pB->AimPart[pB->nPart-1] = UN_DA_Hori;
				if (!ret)
					return ret;
			}
			/***  Вертикальные линии  ***/
			if (CntVer>0)
			{
				ret = LoadLinesSpecInfo (hC, vB, KeyVer, CntVer);
				pB->AimPart[pB->nPart-1] = UN_DA_Vert;
				return ret;
			}
			return RV_TRUE;
		case UN_LD_CompRe :
			ret = LoadComps_rv (hC, vB, pStr, Filter); //t-e-d
			if (ret==RV_DOUBT)
				CleanLastDataPart (vB);
			return ret;
		case UN_LD_PictRe :
		default :
			return RV_FALSE;
	}
}
/*----------------------------------------------------------------------------*/
void Error_MyNoMem (char *pStr, int HowMuch, char *pWhat)
{
//	SetReturnCode_rstuff (Code);
	sprintf (pStr, "RSource.AboutLines: Не хватило памяти под %d %s!", HowMuch, pWhat);
}
/*----------------------------------------------------------------------------*/
