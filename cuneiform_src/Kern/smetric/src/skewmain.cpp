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
/*  Редакция   :  17.11.99                                                    */
/*  Файл       :  'Zoning.CPP'                                                */
/*  Содержание :  Функции зонирования.                                        */
/*  Назначение :  Ориентация на странице.                                     */
/*----------------------------------------------------------------------------*/
#define __SMetric__

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
/*#include <windows.h>*/
/*  interface our-other  */
/*  interface our  */
#include "smetric.h"
/*  interface my        */
#include "un_err.h"
#include "am_comm.h"
#include "gystogra.h"
#include "skewtrip.h"
#include "markdatadefs.h"
#include "linedesc.h"
/*  interface my-my      */
#include "un_buff.h"
#include "amt_geom.h"
/*  interface our util  */
#include "skew1024.h"

const int SMETRIC_MIN_LINE_LNG = 60; //линии с длиной < SMETRIC_MIN_LINE_LNG не учитываются при вычислении угла наклона
static int32_t giResolX = 300;
static int32_t giResolY = 300;
/*------------extern functions------------------------------------------------*/
Bool CalcTripSkew (Rect16 *pRc, int nRc, int SkewProp, int DeltaProp
				, int StepProp, int MaxSize, Un_GYST *pBegGt, Un_GYST *pMidGt
				, Un_GYST *pEndGt, TRIP_SKEW *pTripSkew, Bool Hori, char *pStr);
Bool CalcUgol (Rect16 *pRc, int nRc, int MaxSize, Un_GYST *pBegGt
			, Un_GYST *pEndGt, int SkewProp, int DeltaProp, int StepProp
			, int32_t *pSkew, double *pLent, double *pNumb, Bool Hori, int Bou
			, char *pStr);
Bool CalcLinesSkew (void *vB, int32_t *pSkewHor, double *pWeiHor, int32_t *pSkewVer
					, double *pWeiVer);
/*----------------------------------------------------------------------------*/
typedef struct tagPAR_AnLi
{
	int       SkewWidForAnal;
	int       LitDeltaSkewNorm;
	int       BigDeltaSkewNorm;
}PAR_AnLi;
/*------------own functions---------------------------------------------------*/
void FindLinesSkew (void *vLine, int nLine, void *vP, PAR_AnLi *pParAnLi);
Bool CalcLinesSkew (void *vB, int32_t *pSkewHor, double *pWeiHor, int32_t *pSkewVer
					, double *pWeiVer);
void PrepareMyBoxes (Rect16 *pRc, int nRc, Rect16 **ppRcMy, int *nRcMy, int Size);
Bool FindTextSkew (void *vB, char *cWork, int SizeWork, INV_SKEW *pInvSkew
				   , char *pStr, Bool CalcMuchSkew);
Bool FindSkewForLines (void *vB, char *cWork, int SizeWork, INV_SKEW *pInSk
					, Rect16 *pRcReg, int32_t SkewReg, char *pStr);
SMetric_FUNC(Bool32) SMetric_FindMainSkew (void *vB, char *cWork, int SizeWork, int32_t *pSkew
			, int32_t *pSkewLocVerLin, Rect16 *pRcReg, int32_t SkewReg, char *pStr
			, Bool CalcMuchSkew, Bool TalkMuchSkew);
/*----------------------------------------------------------------------------*/
//установить разрешение по X. Возвращает предыдущее значение /tanya
int32_t SMetric_SetResolX(int32_t size)
{
	int32_t ret = giResolX;
	giResolX = size;
	return ret;
}
/*----------------------------------------------------------------------------*/
//получить разрешение по X /tanya
int32_t SMetric_GetResolX()
{
	return giResolX;
}
/*----------------------------------------------------------------------------*/
//установить разрешение по Y. Возвращает предыдущее значение /tanya
int32_t SMetric_SetResolY(int32_t size)
{
	int32_t ret = giResolY;
	giResolY = size;
	return ret;
}
/*----------------------------------------------------------------------------*/
//получить разрешение по Y /tanya
int32_t SMetric_GetResolY()
{
	return giResolY;
}
/*----------------------------------------------------------------------------*/
RLT_LINEDIR_TYPE LineSect (char Type)
{
	if ((Type&RLT_LT_Kosaya)==RLT_LT_Kosaya)
	{
		if ((Type&RLT_LT_Vertic)==RLT_LT_Vertic)
			return RLT_LDIR_Kosa1;
		else
			return RLT_LDIR_Kosa2;
	}
	else
	{
		if ((Type&RLT_LT_Vertic)==RLT_LT_Vertic)
			return RLT_LDIR_Verti;
		else
			return RLT_LDIR_Horiz;
	}
}
/*----------------------------------------------------------------------------*/
void FindLinesSkew (void *vLine, int nLine, void *vP, PAR_AnLi *pParAnLi)
{
	int i, j, k, Posit, Sect;
	int i_best, w_best, w_curr, Skew;
	int Rasp[513][4];
	RLT_LINE *pL;
	RLT_LINEATDIR *pP;
	pL = (RLT_LINE *)vLine;
	pP = (RLT_LINEATDIR *)vP;
	/*  Заполняем распределение по углам "хороших" линий  */
	for (j=0; j<4; j++)
		for (i=0; i<513; i++)
			Rasp[i][j] = 0;
	for (i=0; i<nLine; i++)
	{
		if ((pL[i].Type&RLT_LT_SkewConflict)==RLT_LT_SkewConflict)
			continue;
		// Beg 08.05.2003
		if (abs(pL[i].Beg.x - pL[i].End.x) > abs(pL[i].Beg.y - pL[i].End.y))
		{
			// горизонтальная линия
			if (pL[i].Len<SMETRIC_MIN_LINE_LNG*SMetric_GetResolX()/300)//!!! Надо*Разреш/300
				continue;
		}
		else
		{
			if (pL[i].Len<SMETRIC_MIN_LINE_LNG*SMetric_GetResolY()/300)//!!! Надо*Разреш/300
				continue;
			// вертикальная линия
		}
		// End 08.05.2003
		Sect = LineSect (pL[i].Type);
		Posit = pL[i].Skew;
		if (Posit>256) Posit=256;
		if (Posit<-256) Posit=-256;
		Posit += 256;
		Rasp[Posit][Sect] += pL[i].Len;
	}
	/*  Решение по углу  */
	for (j=0; j<4; j++)
	{
		i_best = 256;
		w_best = 0;
		for (i=0; i<513-pParAnLi->SkewWidForAnal; i++)
		{
			w_curr = 0;
			for (k=0; k<=pParAnLi->SkewWidForAnal; k++)
				w_curr += Rasp[i+k][j];
			if (w_best>=w_curr)
				continue;
			i_best = i;
			w_best = w_curr;
		}
		Skew = 0;
		w_best = 0;
		for (k=0; k<=pParAnLi->SkewWidForAnal; k++)
		{
			Skew += (Rasp[i_best+k][j]*(i_best+k-256));
			w_best += Rasp[i_best+k][j];
		}
		if (Skew>=0)
			Skew = (int)((double)Skew/(double)(w_best)+.5);
		else
			Skew = (int)((double)Skew/(double)(w_best)-.5);
		pP[j].SkewPool = Skew;
		pP[j].WeightSkew = w_best;
	}
}
/*----------------------------------------------------------------------------*/
Bool CalcLinesSkew (void *vB, int32_t *pSkewHor, double *pWeiHor, int32_t *pSkewVer
					, double *pWeiVer)
{
	UN_BUFF *pB;
	int Ind, nLine;
	void *vLine;
	RLT_LINEATDIR *pP;
	PAR_AnLi ParAnLi = {0};
	ParAnLi.SkewWidForAnal = 9;
	pB = (UN_BUFF *)vB;
	Ind = FindSuchData (vB, UN_DT_RltLine);
	if (Ind < 0)
		return FALSE;
	vLine = pB->vPart[Ind];
	nLine = pB->nPartUnits[Ind];
	pP = ((RLT_LINEPOOL *)pB->vPart[Ind-1])->Pool;
	FindLinesSkew (vLine, nLine, (void *)pP, &ParAnLi);
	*pWeiHor  = pP[RLT_LDIR_Horiz].WeightSkew;
	*pWeiVer  = pP[RLT_LDIR_Verti].WeightSkew;
	*pSkewHor = pP[RLT_LDIR_Horiz].SkewPool;
	*pSkewVer = pP[RLT_LDIR_Verti].SkewPool;
	return TRUE;
}
/*----------------------------------------------------------------------------*/
void PrepareMyBoxes (Rect16 *pRc, int nRc, Rect16 **ppRcMy, int *nRcMy, int Size)
{
	int Lim, i;
	Lim = (Size*sizeof (int))/sizeof (Rect16);
	*nRcMy = 0;
	for (i=0; i<nRc; i++)
	{
		if (pRc[i].right  - pRc[i].left + 1 > 50)
			continue;
		if (pRc[i].right  - pRc[i].left + 1 < 2)
			continue;
		if (pRc[i].bottom - pRc[i].top  + 1 > 50)
			continue;
		if (pRc[i].bottom - pRc[i].top  + 1 < 13)
			continue;
		if (*nRcMy>=Lim)
		{
			*ppRcMy = pRc;
			*nRcMy = nRc;
			return;
		}
		memcpy (&((*ppRcMy)[*nRcMy]), &(pRc[i]), sizeof (Rect16));
		(*nRcMy)++;
	}
}
/*----------------------------------------------------------------------------*/
Bool FindTextSkew (void *vB, char *cWork, int SizeWork, INV_SKEW *pInSk
				   , char *pStr, Bool CalcMuchSkew)
{
	int Ind, nRc, nRcMy, Size1, SizeR;
	double Lent, Numb;
	int *iWork;
	Un_GYST FirGt, SecGt, TriGt;
	UN_BUFF *pB;
	Rect16 *pRc, *pRcMy;
	Bool ret;
	int32_t SkewHorRough, SkewVerRough;
	iWork = (int *)cWork;
	pB = (UN_BUFF *)vB;
	Ind = FindSuchAimedData (vB, UN_DT_Rect16, UN_DA_CompReal);
	if (Ind < 0)
		return FALSE;
	pRc = (Rect16 *)(pB->vPart[Ind]);
	nRc = pB->nPartUnits[Ind];
	Size1 = SizeWork / sizeof (int);
	FirGt.Signal = iWork;
	SecGt.Signal = (&iWork[Size1/4]);
	TriGt.Signal = (&iWork[2*(Size1/4)]);
	pRcMy = (Rect16 *)(&iWork[3*(Size1/4)]);
	SizeR = ((Size1/4)*sizeof (int))/sizeof (Rect16);
	PrepareMyBoxes (pRc, nRc, &pRcMy, &nRcMy, SizeR);
	ret = CalcUgol (pRcMy, nRcMy, Size1/4, &FirGt, &SecGt, 0, 100, 10
			, &SkewHorRough, &Lent, &Numb, TRUE, 0, pStr);
	if (!ret)
		return ret;
	if (CalcMuchSkew)
	{
		ret = CalcTripSkew (pRcMy, nRcMy, SkewHorRough, 10, 1, Size1/4, &FirGt, &SecGt, &TriGt
					   , &pInSk->Hor, TRUE, pStr);
		if (!ret)
			return ret;
		ret = CalcUgol (pRcMy, nRcMy, Size1/4, &FirGt, &SecGt, 0, 100, 10
				, &SkewVerRough, &Lent, &Numb, FALSE, 0, pStr);
		if (!ret)
			return ret;
		ret = CalcTripSkew (pRcMy, nRcMy, SkewVerRough, 10, 1, Size1/4, &FirGt, &SecGt, &TriGt
					   , &pInSk->Ver, FALSE, pStr);
	}
	else
	{
		ret = CalcUgol (pRcMy, nRcMy, Size1/4, &FirGt, &SecGt, SkewHorRough, 10, 1
				, &(pInSk->Hor.Beg.Skew), &(pInSk->Hor.Beg.LentRow)
				, &(pInSk->Hor.Beg.NumbRow), TRUE, 0, pStr);
	}
	return ret;
}
/*----------------------------------------------------------------------------*/
Bool FindSkewForLines (void *vB, char *cWork, int SizeWork, INV_SKEW *pInSk
					, Rect16 *pRcReg, int32_t SkewReg, char *pStr)
{
	Bool ret;
	ret = CalcLinesSkew (vB, &pInSk->SkewHorLin, &pInSk->WeigHorLin
		, &pInSk->SkewVerLin, &pInSk->WeigVerLin);
	if (!ret)
		return ret;
	CleanLastDataPart (vB);
	CleanLastDataPart (vB);
	return RV_TRUE;
}
/*----------------------------------------------------------------------------*/
SMetric_FUNC(Bool32) SMetric_FindMainSkew (void *vB, char *cWork, int SizeWork, int32_t *pSkew
			, int32_t *pSkewLocVerLin, Rect16 *pRcReg, int32_t SkewReg, char *pStr
			, Bool CalcMuchSkew, Bool TalkMuchSkew)
{
	char str[256];
	double Wei;
	int32_t SkewLine = 10000;
	Bool ret, VerDisp;
	INV_SKEW InvSkew = {0};
	*pSkewLocVerLin = 0;
		/*  2.1. Угол по тексту.  */
	ret = FindTextSkew (vB, cWork, SizeWork, &InvSkew, pStr, CalcMuchSkew);
	if (!ret)
		return ret;
		/*  2.2. Угол по линиям.  */
	ret = FindSkewForLines (vB, cWork, SizeWork, &InvSkew, pRcReg, SkewReg, pStr);
//	if ((ret!=RV_TRUE)&&(ret!=RV_EMPTY))
//		return ret;
		/*  2.3. Финальный угол.  */
	if (ret==RV_TRUE)
	{
		Wei  = InvSkew.WeigHorLin;
		Wei += InvSkew.WeigVerLin;
		SkewLine  = (int)(InvSkew.SkewHorLin * InvSkew.WeigHorLin);
		SkewLine += (int)(InvSkew.SkewVerLin * InvSkew.WeigVerLin);
		if (SkewLine>=0)
			SkewLine = (int)((double)SkewLine/Wei+.5);
		else
			SkewLine = (int)((double)SkewLine/Wei-.5);
		if (InvSkew.WeigHorLin + InvSkew.WeigVerLin > 1500)
		{
			if (abs(InvSkew.Hor.Beg.Skew-SkewLine)<=2)
				*pSkew = (InvSkew.Hor.Beg.Skew + SkewLine)/2;
			else
				*pSkew = (InvSkew.Hor.Beg.Skew + 2*SkewLine)/3;
		}
		else //28.04.01
			*pSkew = InvSkew.Hor.Beg.Skew;
	}
	else
		*pSkew = InvSkew.Hor.Beg.Skew;
	VerDisp  = (InvSkew.WeigHorLin>=6000); //много гориз. линий
	VerDisp *= (InvSkew.WeigVerLin>=6000); //много верти. линий
	VerDisp *= (abs(InvSkew.SkewHorLin-InvSkew.SkewVerLin)>=8); //их углы сильно отличаются
	VerDisp *= (abs(InvSkew.SkewVerLin-*pSkew)>=6); //угол вер.лин. сильно отличаются от фин.
	if (VerDisp)
		*pSkewLocVerLin = (InvSkew.SkewVerLin-*pSkew);
	if (TalkMuchSkew)
	{
		AM_ConsolN  ("Горизонтальный угол :");
		AM_ConsolN  ("  Линии    : Угол = %d ДлинаОбщая = %d"
			, InvSkew.SkewHorLin, (int)InvSkew.WeigHorLin);
		sprintf (str, "  Тек.Нач. : Угол = %d ДлинаГруппы = %5.1f ЧислоГрупп = %5.1f"
			, InvSkew.Hor.Beg.Skew, InvSkew.Hor.Beg.LentRow, InvSkew.Hor.Beg.NumbRow);
		AM_ConsolN (str);
		sprintf (str, "  Тек.Сер. : Угол = %d ДлинаГруппы = %5.1f ЧислоГрупп = %5.1f"
			, InvSkew.Hor.Mid.Skew, InvSkew.Hor.Mid.LentRow, InvSkew.Hor.Mid.NumbRow);
		AM_ConsolN (str);
		sprintf (str, "  Тек.Кон. : Угол = %d ДлинаГруппы = %5.1f ЧислоГрупп = %5.1f"
			, InvSkew.Hor.End.Skew, InvSkew.Hor.End.LentRow, InvSkew.Hor.End.NumbRow);
		AM_ConsolN (str);
		AM_ConsolN  ("Вертикальный угол :");
		AM_ConsolN  ("  Линии    : Угол = %d ДлинаОбщая = %d"
			, InvSkew.SkewVerLin, (int)InvSkew.WeigVerLin);
		sprintf (str, "  Тек.Нач. : Угол = %d ДлинаГруппы = %5.1f ЧислоГрупп = %5.1f"
			, InvSkew.Ver.Beg.Skew, InvSkew.Ver.Beg.LentRow, InvSkew.Ver.Beg.NumbRow);
		AM_ConsolN (str);
		sprintf (str, "  Тек.Сер. : Угол = %d ДлинаГруппы = %5.1f ЧислоГрупп = %5.1f"
			, InvSkew.Ver.Mid.Skew, InvSkew.Ver.Mid.LentRow, InvSkew.Ver.Mid.NumbRow);
		AM_ConsolN (str);
		sprintf (str, "  Тек.Кон. : Угол = %d ДлинаГруппы = %5.1f ЧислоГрупп = %5.1f"
			, InvSkew.Ver.End.Skew, InvSkew.Ver.End.LentRow, InvSkew.Ver.End.NumbRow);
		AM_ConsolN (str);
		if (SkewLine!=10000)
			AM_ConsolN ("Итог линии : Угол = %d", SkewLine);
		AM_ConsolN ("Итог       : Угол = %d", *pSkew);
	}
	return RV_TRUE;
}
/*----------------------------------------------------------------------------*/
