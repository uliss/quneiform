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
/*  Редакция   :  30.11.99                                                    */
/*  Файл       :  'VerifyLn.CPP'                                              */
/*  Содержание :  Функции алгоритма верификации линий.                        */
/*  Назначение :  Верхний уровень алгоритмов верификации линий.               */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
/*#include <windows.h>*/
#include <stdlib.h>
/*  interface our-other  */
#include "lnsdefs.h"
#include "ctiimage.h"
/*  interface our-my     */
/*  interface my        */
#include "am_err.h"
#include "am_comm.h"
#include "vl_rule.h"
/*  interface our util  */
#include "skew1024.h"
/*----------------------------------------------------------------------------*/
#define       MaxZherOnLine         50
typedef struct tagAM_ZHERTVY
{
	int      nZher;
	int      iZher[MaxZherOnLine];
	Point16  LinBeg[MaxZherOnLine];
	Point16  LinEnd[MaxZherOnLine];
}AM_ZHERTVY;
/*------------extern functions------------------------------------------------*/
Bool HaveLinePartAtReg (Point16 *pA, Point16 *pB, Rect16 *pRc, int32_t Skew
						, int NearHor, int NearVer);
int InvestShortLineWithRastr_rv_pne (Handle hCPage, LineInfo *pLns);
/*------------own functions---------------------------------------------------*/
void ChoiseQuasiLetters (Rect16 *pRc, int *pWhatDo, int nComp);
void OldFormalVerification (LineInfo *pLns, int Lent);
void FormalVerification (LineInfo *pLns, int Lent);
void PutNewFlagOfLine (LineInfo *pLns, uint32_t Flag);
void GetAllZhertvy (LineInfo *pLns, AM_ZHERTVY *pZher, Rect16 *pRc
					, int *pWhatDo, int nComp, int Near, Bool Hori);
void MakeNormOrderForZher (LineInfo *pLns, AM_ZHERTVY *pZher);
int  FindNormParts (LineInfo *pLns, AM_ZHERTVY *pZher, Point16 *pBeg
				   , Point16 *pEnd, int MinLent);
void InvestLongLineWithBoxes (LineInfo *pLns, Rect16 *pRc, int *pWhatDo
					, int nComp, int *nZher, int *iZher, AM_ZHERTVY *pZher
					, int MaxZher, Bool Hori, int Lent);
void WriteResForLines (void *vLti);
void New_MarkVerifiedLines (void *vLti, Handle hCPage, Rect16 *pRc, int *pWhatDo
		, int nComp, int *nZher, int *iZher, int MaxZher, Bool AbleShortVert);
/*----------------------------------------------------------------------------*/
void ChoiseQuasiLetters (Rect16 *pRc, int *pWhatDo, int nComp)
{
	int i, h, w;
	for (i=0; i<nComp; i++)
	{
		h = pRc[i].bottom - pRc[i].top + 1;
		w = pRc[i].right - pRc[i].left + 1;
		pWhatDo[i] = FALSE;
		if ((pRc[i].top<10)||(pRc[i].left<10))
			continue;
		pWhatDo[i] = TRUE;
		if ((h<=4)&&(w<=20)&&(w>=10)) //тире
			continue;
		pWhatDo[i] = FALSE;
		if ((w<7)||(w>120))
			continue;
		if ((h<10)||(h>100))
			continue;
		if (h > (int)(2.4*w + .5))
			continue;
		if ((h<20)&&(w > (int)(2.1*h + .5)))
			continue;
		pWhatDo[i] = TRUE;
	}
}
/*----------------------------------------------------------------------------*/
void OldFormalVerification (LineInfo *pLns, int Lent)
{
	int Qual;
	while (1)
	{
		if (Lent>256)
		{
			Qual = 1;
			break;
		}
		if (Lent<=80)
		{
			Qual = 0;
			break;
		}
		if ((Lent>=128)&&(pLns->SegCnt>=2))
		{
			Qual = 2;
			break;
		}
		if (pLns->SegCnt>1)
		{
			Qual = 0;
			break;
		}
		Qual = 1;
		break;
	}
	if (Qual)
	{
		if (Qual==TRUE)
			pLns->Flags |= LI_IsTrue;
	}
	else
		pLns->Flags |= LI_IsFalse;
}
/*----------------------------------------------------------------------------*/
void FormalVerification (LineInfo *pLns, int Lent)
{
	int Qual;
	Qual = 2;
	if ((Lent>=256)||(pLns->SegCnt==1))
		Qual = TRUE;
	if ((pLns->SegCnt==2)&&(Lent<128))
		Qual = FALSE;
	if ((pLns->SegCnt>2)&&(Lent<256))
		Qual = FALSE;
	if (Qual)
	{
		if (Qual==TRUE)
			pLns->Flags |= LI_IsTrue;
	}
	else
		pLns->Flags |= LI_IsFalse;
}
/*----------------------------------------------------------------------------*/
void PutNewFlagOfLine (LineInfo *pLns, uint32_t Flag)
{
	uint32_t AntiFalse, AntiTrue;
	AntiFalse = 0xFFFFFFFF;
	AntiFalse ^= LI_IsFalse;
	AntiTrue  = 0xFFFFFFFF;
	AntiTrue  ^= LI_IsTrue;
	switch (Flag)
	{
		case 0 :
			pLns->Flags &= AntiTrue;
			pLns->Flags &= AntiFalse;
			break;
		case LI_IsFalse :
			pLns->Flags &= AntiTrue;
			pLns->Flags |= LI_IsFalse;
			break;
		case LI_IsTrue :
			pLns->Flags &= AntiFalse;
			pLns->Flags |= LI_IsTrue;
			break;
		case LI_NOTWHOLE :
			pLns->Flags &= AntiFalse;
			pLns->Flags |= LI_IsTrue;
			pLns->Flags |= LI_NOTWHOLE;
			break;
		case LI_COMPLEX :
			pLns->Flags &= AntiFalse;
			pLns->Flags |= LI_IsTrue;
			pLns->Flags |= LI_COMPLEX;
			break;
		default :
			break;
	}
}
/*----------------------------------------------------------------------------*/
void GetAllZhertvy (LineInfo *pLns, AM_ZHERTVY *pZher, Rect16 *pRc
					, int *pWhatDo, int nComp, int Near, Bool Hori)
{
	int i;
	Bool Cross;
	pZher->nZher = 0;
	for (i=0; i<nComp; i++)
	{
		if (pZher->nZher>=MaxZherOnLine)
			return;
		if (!pWhatDo[i])
			continue;
		pZher->LinBeg[pZher->nZher].x = pLns->A.x;
		pZher->LinBeg[pZher->nZher].y = pLns->A.y;
		pZher->LinEnd[pZher->nZher].x = pLns->B.x;
		pZher->LinEnd[pZher->nZher].y = pLns->B.y;
		Cross = HaveLinePartAtReg (&(pZher->LinBeg[pZher->nZher])
						, &(pZher->LinEnd[pZher->nZher]), &(pRc[i]), 0, Near, Near);
		if (!Cross)
			continue;
		if (Hori)
		{
			pZher->LinBeg[pZher->nZher].x -= 11;
			pZher->LinEnd[pZher->nZher].x += 11;
		}
		else
		{
			pZher->LinBeg[pZher->nZher].y -= 11;
			pZher->LinEnd[pZher->nZher].y += 11;
		}
		pZher->iZher[pZher->nZher] = i;
		(pZher->nZher)++;
	}
}
/*----------------------------------------------------------------------------*/
void MakeNormOrderForZher (LineInfo *pLns, AM_ZHERTVY *pZher)
{
	int i, k, Dist, best_i, best_dist, a;
	Point16 CurBeg, CurEnd;
	for (k=0; k<pZher->nZher; k++)
	{
		if (k==0)
		{
			CurBeg.x = pLns->A.x;
			CurBeg.y = pLns->A.y;
		}
		else
		{
			CurBeg.x = pZher->LinEnd[k-1].x;
			CurBeg.y = pZher->LinEnd[k-1].y;
		}
		best_dist = 1000000;
		best_i = k;
		for (i=k; i<pZher->nZher; i++)
		{
			CurEnd.x = pZher->LinBeg[i].x;
			CurEnd.y = pZher->LinBeg[i].y;
			Dist  = (CurBeg.x - CurEnd.x) * (CurBeg.x - CurEnd.x);
			Dist += (CurBeg.y - CurEnd.y) * (CurBeg.y - CurEnd.y);
			Dist  = (int) sqrt ((double)Dist);
			if (Dist > best_dist)
				continue;
			best_dist = Dist;
			best_i = i;
		}
		if (best_i==k)
			continue;
		a                       = pZher->iZher[best_i];
		pZher->iZher[best_i]    = pZher->iZher[k];
		pZher->iZher[k]         = a;
		a                       = pZher->LinBeg[best_i].x;
		pZher->LinBeg[best_i].x = pZher->LinBeg[k].x;
		pZher->LinBeg[k].x      = a;
		a                       = pZher->LinBeg[best_i].y;
		pZher->LinBeg[best_i].y = pZher->LinBeg[k].y;
		pZher->LinBeg[k].y      = a;
		a                       = pZher->LinEnd[best_i].x;
		pZher->LinEnd[best_i].x = pZher->LinEnd[k].x;
		pZher->LinEnd[k].x      = a;
		a                       = pZher->LinEnd[best_i].y;
		pZher->LinEnd[best_i].y = pZher->LinEnd[k].y;
		pZher->LinEnd[k].y      = a;
	}
}
/*----------------------------------------------------------------------------*/
int FindNormParts (LineInfo *pLns, AM_ZHERTVY *pZher, Point16 *pBeg
				   , Point16 *pEnd, int MinLent)
{
	int nNorm, i, Lent;
	Point16 CurBeg, CurEnd;
	nNorm = 0;
	for (i=0; i<=pZher->nZher; i++)
	{
		if (i==0)
		{
			CurBeg.x = pLns->A.x;
			CurBeg.y = pLns->A.y;
		}
		else
		{
			CurBeg.x = pZher->LinEnd[i-1].x;
			CurBeg.y = pZher->LinEnd[i-1].y;
		}
		if (i==pZher->nZher)
		{
			CurEnd.x = pLns->B.x;
			CurEnd.y = pLns->B.y;
		}
		else
		{
			CurEnd.x = pZher->LinBeg[i].x;
			CurEnd.y = pZher->LinBeg[i].y;
		}
		Lent  = (CurBeg.x - CurEnd.x) * (CurBeg.x - CurEnd.x);
		Lent += (CurBeg.y - CurEnd.y) * (CurBeg.y - CurEnd.y);
		Lent  = (int) sqrt ((double)Lent);
		if (Lent < MinLent)
			continue;
		pBeg[nNorm].x = CurBeg.x;
		pBeg[nNorm].y = CurBeg.y;
		pEnd[nNorm].x = CurEnd.x;
		pEnd[nNorm].y = CurEnd.y;
		nNorm++;
	}
	return nNorm;
}
/*----------------------------------------------------------------------------*/
void InvestLongLineWithBoxes (LineInfo *pLns, Rect16 *pRc, int *pWhatDo
				, int nComp, int *nZher, int *iZher, AM_ZHERTVY *pZher
				, int MaxZher, Bool Hori, int Lent)
{
	Point16  LinBeg[MaxZherOnLine + 1];
	Point16  LinEnd[MaxZherOnLine + 1];
	int j, Near, nSimpParts;
	uint32_t   Flag;
	Near = 0;
	if (Hori)
	{
		if (Lent<55)
			return;
		if (Lent<=2000)
			Near = 1;
	}
	else
	{
		if (Lent<44)
			return;
	}
	GetAllZhertvy (pLns, pZher, pRc, pWhatDo, nComp, Near, Hori);
	if (pZher->nZher > 0)
	{
		if (pZher->nZher > 1)
			MakeNormOrderForZher (pLns, pZher);
		nSimpParts = FindNormParts (pLns, pZher, LinBeg, LinEnd, 50);
		if (nSimpParts==0)
			Flag = LI_IsFalse;
		if (nSimpParts==1)
		{
			pLns->Anew.x = LinBeg[0].x;
			pLns->Anew.y = LinBeg[0].y;
			pLns->Bnew.x = LinEnd[0].x;
			pLns->Bnew.y = LinEnd[0].y;
			Flag = LI_NOTWHOLE;
		}
		if (nSimpParts >1)
		{
			Flag = LI_COMPLEX;
			for (j=0; j<pZher->nZher; j++)
			{
				if (*nZher>=MaxZher)
					break;
				iZher[*nZher] = pZher->iZher[j];
				(*nZher)++;
			}
		}
	}
	else
	{
		Flag = LI_IsTrue;
		if ((!Hori)&&(Lent<=70))
			Flag = 0;
/*		if (Hori&&(!AM_Skip (AM_GetKeyOfRule (RU_VL_Debug))))
		{//отладка нецелых линий
			pLns->Anew.x = pLns->A.x;
			pLns->Anew.y = pLns->Anew.y;
			pLns->Bnew.x = (int16_t)((pLns->A.x+pLns->B.x)*.5);
			pLns->Bnew.y = (int16_t)((pLns->A.y+pLns->B.y)*.5);
			Flag = LI_NOTWHOLE;
		}*/
	}
	PutNewFlagOfLine (pLns, Flag);
}
/*----------------------------------------------------------------------------*/
void WriteResForLines (void *vLti)
{
	int i, n, rot;
	int L_Bad, L_Doubt, L_Good;
	LineInfo *pLns;
	LinesTotalInfo *pLti;
	char str[256];
	pLti = (LinesTotalInfo *)vLti;
	/*****************  горизонтальная  ******************************/
	if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
		rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, "  <4 Н Гор.Линии\n");
	n = pLti->Hor.Cnt;
	L_Bad = 0; L_Doubt = 0; L_Good = 0;
	pLns = pLti->Hor.Lns;
	for (i=0; i<n; i++)
	{
		if (pLns->Flags & LI_IsFalse)
		{
			L_Bad++;
			sprintf (str, "  <4 О 1 %c %4d %4d %4d %4d %4d\n"
				, '-', pLns->A.x, pLns->A.y, pLns->B.x, pLns->B.y, pLns->Thickness);
			if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
				rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, str);
		}
		else
		{
			if  ((!(pLns->Flags & LI_IsTrue))
				||(pLns->Flags & LI_NOTWHOLE)||(pLns->Flags & LI_COMPLEX))
			{
				L_Doubt++;
				sprintf (str, "  <4 О 1 %c %4d %4d %4d %4d %4d\n"
					, '?', pLns->A.x, pLns->A.y, pLns->B.x, pLns->B.y, pLns->Thickness);
				if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
					rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, str);
			}
			else
			{
				L_Good++;
				sprintf (str, "  <4 О 1 %c %4d %4d %4d %4d %4d\n"
					, '+', pLns->A.x, pLns->A.y, pLns->B.x, pLns->B.y, pLns->Thickness);
				if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
					rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, str);
			}
		}
		pLns++;
	}
	sprintf (str, "  <4 Р %4d %4d %4d\n", L_Good, L_Doubt, L_Bad);
	if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
		rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, str);
	if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
		rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, "  <4 К Гор.Линии\n");
	/*****************  вериткальная  ******************************/
	if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
		rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, "  <4 Н Вер.Линии\n");
	n = pLti->Ver.Cnt;
	L_Bad = 0; L_Doubt = 0; L_Good = 0;
	pLns = pLti->Ver.Lns;
	for (i=0; i<n; i++)
	{
		if (pLns->Flags & LI_IsFalse)
		{
			L_Bad++;
			sprintf (str, "  <4 О 1 %c %4d %4d %4d %4d %4d\n"
				, '-', pLns->A.x, pLns->A.y, pLns->B.x, pLns->B.y, pLns->Thickness);
			if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
				rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, str);
		}
		else
		{
			if  ((!(pLns->Flags & LI_IsTrue))
				||(pLns->Flags & LI_NOTWHOLE)||(pLns->Flags & LI_COMPLEX))
			{
				L_Doubt++;
				sprintf (str, "  <4 О 1 %c %4d %4d %4d %4d %4d\n"
					, '?', pLns->A.x, pLns->A.y, pLns->B.x, pLns->B.y, pLns->Thickness);
				if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
					rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, str);
			}
			else
			{
				L_Good++;
				sprintf (str, "  <4 О 1 %c %4d %4d %4d %4d %4d\n"
					, '+', pLns->A.x, pLns->A.y, pLns->B.x, pLns->B.y, pLns->Thickness);
				if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
					rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, str);
			}
		}
		pLns++;
	}
	sprintf (str, "  <4 Р %4d %4d %4d\n", L_Good, L_Doubt, L_Bad);
	if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
		rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, str);
	if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
		rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, "  <4 К Вер.Линии\n");
}
/*----------------------------------------------------------------------------*/
void New_MarkVerifiedLines (void *vLti, Handle hCPage, Rect16 *pRc, int *pWhatDo
		, int nComp, int *nZher, int *iZher, int MaxZher, Bool AbleShortVert)
{
	int i, n, Lent, ret;
	LineInfo *pLns;
	LinesTotalInfo *pLti;
	AM_ZHERTVY  ZherOfLine;
	uint32_t AntiFalse, AntiTrue, AntiPoin;
	Bool WasPointed, WasLongPointed;
	AntiFalse = 0xFFFFFFFF;
	AntiFalse ^= LI_IsFalse;
	AntiTrue  = 0xFFFFFFFF;
	AntiTrue  ^= LI_IsTrue;
	AntiPoin  = 0xFFFFFFFF;
	AntiPoin  ^= LI_Pointed;
	pLti = (LinesTotalInfo *)vLti;
	ChoiseQuasiLetters (pRc, pWhatDo, nComp);
	*nZher = 0;
	/*****************  горизонтальная  ******************************/
	n = pLti->Hor.Cnt;
	pLns = pLti->Hor.Lns;
	for (i=0; i<n; i++)
	{
		Lent  = (pLns->A.x - pLns->B.x) * (pLns->A.x - pLns->B.x);
		Lent += (pLns->A.y - pLns->B.y) * (pLns->A.y - pLns->B.y);
		Lent  = (int) sqrt ((double)Lent);
		/*  определение отточий - обжалованию не подлежит  */ //пора переиграть
/*		if ( AM_Skip (AM_GetKeyOfRule (RU_VL_U_NoPointedLines)))
		{
			if ((pLns->SegCnt==1)&&(pLns->Quality==255)&&(pLns->Thickness==2))
			{
				pLns->Flags |= LI_Pointed;
				pLns++;
				continue;
			}
			if ((pLns->SegCnt==2)&&(pLns->Quality>=240)&&(pLns->Thickness==1))
			{
				pLns->Flags |= LI_Pointed;
				pLns->Flags |= LI_Doubt;
				pLns++;
				continue;
			}
		}*/ //Almi 13.09.01
		/*  верификация формальная (для всех)  */
		if (!AM_Skip (AM_GetKeyOfRule (RU_VL_U_NewFormalVerify)))
			FormalVerification (pLns, Lent);
		else
			OldFormalVerification (pLns, Lent);
		/*  верификация по тифу (для коротких)  */
		ret = RV_EMPTY;
		if ((Lent>60)&&(Lent<=350)&&(AM_Skip (AM_GetKeyOfRule (RU_VL_U_NoInvestImage))))
			ret = InvestShortLineWithRastr_rv_pne (hCPage, pLns);
		if (ret==RV_NEGATIVE)
			PutNewFlagOfLine (pLns, LI_IsFalse);
		if (ret==RV_POSITIVE)
			PutNewFlagOfLine (pLns, LI_IsTrue);
		/*  верификация по коробкам (для длинных и не отифенных коротких)  */
		if (!AM_Skip (AM_GetKeyOfRule (RU_VL_U_AbleHoriZher)))
		{
			if (((Lent>350)||(ret==RV_EMPTY))&&(AM_Skip (AM_GetKeyOfRule (RU_VL_U_NoInvestLongLines))))
				InvestLongLineWithBoxes (pLns, pRc, pWhatDo, nComp, nZher, iZher
					, &ZherOfLine, MaxZher, TRUE, Lent);
		}
		pLns++;
	}
	//******** горизонтальные отточия проверим ********//
	pLns = pLti->Hor.Lns;
	WasPointed = FALSE;
	WasLongPointed = FALSE;
	for (i=0; i<n; i++)
	{
		if (!(pLns[i].Flags & LI_Pointed))
			continue;
		WasPointed = TRUE;
		if (abs(pLns[i].A.x - pLns[i].B.x)>100)
			WasLongPointed = TRUE;
	}
	if (WasPointed&!WasLongPointed)
	{
		pLns = pLti->Hor.Lns;
		for (i=0; i<n; i++)
		{
			if ((pLns[i].Flags & LI_Pointed))
				pLns[i].Flags &= AntiPoin;
		}
	}
	/*****************  вертикальная  ******************************/
	n = pLti->Ver.Cnt;
	pLns = pLti->Ver.Lns;
	for (i=0; i<n; i++)
	{
		Lent  = (pLns->A.x - pLns->B.x) * (pLns->A.x - pLns->B.x);
		Lent += (pLns->A.y - pLns->B.y) * (pLns->A.y - pLns->B.y);
		Lent  = (int) sqrt ((double)Lent);
		if (!AM_Skip (AM_GetKeyOfRule (RU_VL_U_NewFormalVerify)))
			FormalVerification (pLns, Lent);
		else
			OldFormalVerification (pLns, Lent);
		if (AM_Skip (AM_GetKeyOfRule (RU_VL_U_NoInvestLongLines)))
		{
			if (Lent<=100)
				if (pLns->Flags & LI_IsFalse)
					pLns->Flags &= AntiFalse;
		}
		if (AM_Skip (AM_GetKeyOfRule (RU_VL_U_NoInvestLongLines)))
		{
			if (AbleShortVert&&(Lent>=94))
				InvestLongLineWithBoxes (pLns, pRc, pWhatDo, nComp, nZher, iZher
				, &ZherOfLine, MaxZher, FALSE, Lent);
			else
				if (pLns->Flags & LI_IsTrue)
					pLns->Flags &= AntiTrue;
		}
		pLns++;
	}
}
/*----------------------------------------------------------------------------*/
