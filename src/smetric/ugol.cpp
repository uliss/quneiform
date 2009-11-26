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
/*  Author     :  Alexander Mikhailov                                        */
/*  Last Edit  :  17.12.99                                                   */
/*  Source     :  'Ugol.CPP'                                                 */
/*  Содержание :  Функции определения угла текста по белым полосам.          */
/*  Назначение :  Самозначимая часть алгоритма выделения белых полос.        */
/*---------------------------------------------------------------------------*/
#define __SMetric__

#include <stdio.h>
#include <math.h>
/*#include <windows.h>*/

#include "smetric.h"
/*  interface our util  */
	#include "gystogra.h"
	#include "skewtrip.h"
#include "skew1024.h"
/*------------own functions--------------------------------------------------*/
double Q_Estimate (Un_GYST *pGt);
double M_Estimate (Un_GYST *pGt);
int Estimate (Un_GYST *pGt);
Bool CalcTripSkew (Rect16 *pRc, int nRc, int SkewProp, int DeltaProp
				, int StepProp, int MaxSize, Un_GYST *pBegGt, Un_GYST *pMidGt
				, Un_GYST *pEndGt, TRIP_SKEW *pTripSkew, Bool Hori, char *pStr);
Bool CalcUgol (Rect16 *pRc, int nRc, int MaxSize, Un_GYST *pBegGt
			, Un_GYST *pEndGt, int SkewProp, int DeltaProp, int StepProp
			, int32_t *pSkew, double *pLent, double *pNumb, Bool Hori, int Bou
			, char *pStr);
/*---------------------------------------------------------------------------*/
double Q_Estimate (Un_GYST *pGt)
{
	int i, a;
	double Estim;
	Estim = 0.;
	for (i=0; i<=pGt->End; i+=5)
	{
		a = pGt->Signal[i];
		if (i+1<=pGt->End)
			a += pGt->Signal[i+1];
		if (i+2<=pGt->End)
			a += pGt->Signal[i+2];
		if (i+3<=pGt->End)
			a += pGt->Signal[i+3];
		if (i+4<=pGt->End)
			a += pGt->Signal[i+4];
		Estim += a*a;
	}
	return Estim;
}
/*---------------------------------------------------------------------------*/
double M_Estimate (Un_GYST *pGt)
{
	int i, a, b, c, End;
	double Estim;
	int *Sig;
	Estim = 0.;
	a=0; b=0; c=0;
	End = pGt->End;
	Sig = pGt->Signal;
	for (i=1; i<=End-2; i++)
	{
		if (Sig[i]==0)
			continue;
		a     += (Sig[i]*Sig[i]);
		b     += (Sig[i]*Sig[i+1]);
		c     += (Sig[i]*Sig[i+2]);
	}
	Estim += a;
	Estim += (0.8*b);
	Estim += (0.3*c);

	Estim += (Sig[0    ]*Sig[0      ]);
	Estim += (Sig[End-1]*Sig[End-1  ]);
	Estim += (Sig[End  ]*Sig[End    ]);

	Estim += (Sig[0    ]*Sig[0    +1]*0.4);
	Estim += (Sig[End-1]*Sig[End-1+1]*0.4);

	Estim += (Sig[0    ]*Sig[0    +2]*0.3);
	return Estim;
}
/*---------------------------------------------------------------------------*/
Bool CalcTripSkew (Rect16 *pRc, int nRc, int SkewProp, int DeltaProp
				, int StepProp, int MaxSize, Un_GYST *pBegGt, Un_GYST *pMidGt
				, Un_GYST *pEndGt, TRIP_SKEW *pTripSkew, Bool Hori, char *pStr)
{
	int i;
	double Norm, best_Q, best_N_B, best_N_M, best_N_E;
	Bool ret;
	int32_t  SkewFirst_B, SkewFirst_M, SkewFirst_E;
	int32_t  SkewLast_B, SkewLast_M, SkewLast_E;
	/*  Определяем лучший угол.  */
	SkewFirst_B = SkewProp-DeltaProp;
	SkewLast_B = SkewProp-DeltaProp;
	SkewFirst_M = SkewProp-DeltaProp;
	SkewLast_M = SkewProp-DeltaProp;
	SkewFirst_E = SkewProp-DeltaProp;
	SkewLast_E = SkewProp-DeltaProp;
	best_N_B = 0; best_N_M = 0; best_N_E = 0;
	for (i=SkewProp-DeltaProp; i<=SkewProp+DeltaProp; i++)
	{
		if (Hori)
			ret = MakeTopMidBotGysts (pRc, nRc, i, MaxSize, pBegGt, pMidGt, pEndGt);
		else
			ret = MakeLefMidRigGysts (pRc, nRc, i, MaxSize, pBegGt, pMidGt, pEndGt);
		if (!ret)
		{
			sprintf (pStr, "Алгоритм определения угла текста по белым полосам\
				неприменим. Не смог построить гистограмму.");
			return FALSE;
		}
		Norm = M_Estimate (pBegGt);
		if (Norm >= best_N_B)
		{
			SkewLast_B  = i;
			if (Norm > best_N_B)
				SkewFirst_B = i;
			best_N_B = Norm;
		}
		Norm = M_Estimate (pMidGt);
		if (Norm >= best_N_M)
		{
			SkewLast_M  = i;
			if (Norm > best_N_M)
				SkewFirst_M = i;
			best_N_M = Norm;
		}
		Norm = M_Estimate (pEndGt);
		if (Norm >= best_N_E)
		{
			SkewLast_E  = i;
			if (Norm > best_N_E)
				SkewFirst_E = i;
			best_N_E = Norm;
		}
	}
	pTripSkew->Beg.Skew = (SkewFirst_B + SkewLast_B)/2;
	if (Hori)
		ret = MakeTopMidBotGysts (pRc, nRc, pTripSkew->Beg.Skew, MaxSize, pBegGt, pMidGt, pEndGt);
	else
		ret = MakeLefMidRigGysts (pRc, nRc, pTripSkew->Beg.Skew, MaxSize, pBegGt, pMidGt, pEndGt);
	best_Q = Q_Estimate (pBegGt);
	pTripSkew->Beg.LentRow = best_Q/((double)pBegGt->nElem);//число элементов в квазистроке
	pTripSkew->Beg.NumbRow = ((double)(pBegGt->nElem*pBegGt->nElem))/best_Q;//число квазистрок
	pTripSkew->Mid.Skew = (SkewFirst_M + SkewLast_M)/2;
	if (Hori)
		ret = MakeTopMidBotGysts (pRc, nRc, pTripSkew->Mid.Skew, MaxSize, pBegGt, pMidGt, pEndGt);
	else
		ret = MakeLefMidRigGysts (pRc, nRc, pTripSkew->Mid.Skew, MaxSize, pBegGt, pMidGt, pEndGt);
	best_Q = Q_Estimate (pMidGt);
	pTripSkew->Mid.LentRow = best_Q/((double)pMidGt->nElem);//число элементов в квазистроке
	pTripSkew->Mid.NumbRow = ((double)(pMidGt->nElem*pMidGt->nElem))/best_Q;//число квазистрок
	pTripSkew->End.Skew = (SkewFirst_E + SkewLast_E)/2;
	if (Hori)
		ret = MakeTopMidBotGysts (pRc, nRc, pTripSkew->End.Skew, MaxSize, pBegGt, pMidGt, pEndGt);
	else
		ret = MakeLefMidRigGysts (pRc, nRc, pTripSkew->End.Skew, MaxSize, pBegGt, pMidGt, pEndGt);
	best_Q = Q_Estimate (pEndGt);
	pTripSkew->End.LentRow = best_Q/((double)pEndGt->nElem);//число элементов в квазистроке
	pTripSkew->End.NumbRow = ((double)(pEndGt->nElem*pEndGt->nElem))/best_Q;//число квазистрок
	return TRUE;
}
/*---------------------------------------------------------------------------*/
Bool CalcUgol (Rect16 *pRc, int nRc, int MaxSize, Un_GYST *pBegGt
			, Un_GYST *pEndGt, int SkewProp, int DeltaProp, int StepProp
			, int32_t *pSkew, double *pLent, double *pNumb, Bool Hori, int Bou
			, char *pStr)
{
	int i;
	double Norm, best_N, best_Q;
	Bool ret;
	int32_t  SkewFirst, SkewLast;
	Un_GYST *pGt;
	/*  Определяем лучший угол.  */
	SkewFirst = SkewProp-DeltaProp;
	SkewLast  = SkewProp-DeltaProp;
	best_N = 0;
	if (Bou==0)
		pGt = pBegGt;
	else
		pGt = pEndGt;
	for (i=SkewProp-DeltaProp; i<=SkewProp+DeltaProp; i+=StepProp)
	{
		if (Hori)
			ret = MakeTopBotGysts (pRc, nRc, i, MaxSize, pBegGt, pEndGt);
		else
			ret = MakeLefRigGysts (pRc, nRc, i, MaxSize, pBegGt, pEndGt);
		if (!ret)
		{
			sprintf (pStr, "Алгоритм определения угла текста по белым полосам\
				неприменим. Не смог построить гистограмму.");
			return FALSE;
		}
		Norm = M_Estimate (pGt);
		if (Norm < best_N)
			continue;
		SkewLast  = i;
		if (Norm > best_N)
			SkewFirst = i;
		best_N = Norm;
		best_Q = Q_Estimate (pGt);
	}
	*pSkew = (SkewFirst + SkewLast)/2;
	*pLent = best_Q/((double)pGt->nElem);//число элементов в квазистроке
	*pNumb = ((double)(pGt->nElem*pGt->nElem))/best_Q;//число квазистрок
	return TRUE;
}
/*---------------------------------------------------------------------------*/
int Estimate (Un_GYST *pGt)
{
	int i, Estim;
	Estim = 0;
	for (i=0; i<=pGt->End; i++)
		Estim += pGt->Signal[i]*pGt->Signal[i];
//	Estim = ((1024*Estim + 512)/(pGt->nElem*pGt->nElem));
	return Estim;
}
/*---------------------------------------------------------------------------*/
