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
/*  Редакция   :  13.03.01                                                    */
/*  Файл       :  'Line_VP_2_AM.CPP'                                          */
/*  Содержание :  Функции работы с памятью.                                   */
/*  Назначение :  Оптимизация работы с памятью.                               */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
/*#include <windows.h>*/
/*  interface our-other  */
#include "lnsdefs.h"
/*  interface my         */
#include "un_err.h"
#include "line_vp_2_am.h" //own functions
#include "linedesc.h"
#include "markdatadefs.h"
#include "am_comm.h"
/*  interface my-my      */
#include "un_buff.h"
#include "amt_geom.h"
#include "line_vp_util.h"
#include "line_am_util.h"
#include "puma_err.h"
/*------------own functions---------------------------------------------------*/
Bool FullMyLines_Mod (void *vLns, int nLns, void *vB, int *pForw, int *SizeForw
	, int ShifValuForw, Rect16 *pRcReg, int32_t SkewReg, Bool Hori, char *pStr, Bool ContWarn);
Bool FullMyLines_Mod (void *vLines, int count, void *vB, int *pForw, int *SizeForw
	, Rect16 *pRcReg, int32_t SkewReg, Bool Hori, char *pStr, Bool ContWarn);
void FullNewMyLine (RLT_LINE *pLine, LineInfo *pLns, Bool Hori, Bool ContWarn);
void FullNewMyLine (RLT_LINE* pLine,DLine* pline,Bool Hori,Bool ContWarn);
/*----------------------------------------------------------------------------*/

Bool ConvertLinesToAM_rv (int FromType, int ToType, void *vB, int *pForw
					, int SizeForw, Rect16 *pRcReg, int32_t SkewReg, char *pStr, Bool ContWarn)
{
	int i, Aim, Ind, nFrom, ShifValuForw, SizeF;
	int *pF;
	void *vFrom;
	Bool ret, Hori;
	UN_BUFF *pB;
	pB = (UN_BUFF *)vB;
	switch ((UN_LOADDATA)FromType)
	{
		case UN_LD_LinesVP :
			/*  Инициализируем постоянную часть преобразованных данных.  */
			ret = TakeFixedPlace_rv (vB, UN_DT_RltLinePool, UN_DA_Linii, pStr); // f-t-d
			if (ret!=RV_TRUE)
				return ret;
			/*  Инициализируем переменную часть преобразованных данных.  */
			pB->vPart[pB->nPart] = pB->vCurr;
			pB->nPartUnits[pB->nPart] = 0;
			pB->SizePartTotal[pB->nPart] = 0;
			pB->TypePart[pB->nPart] = UN_DT_RltLine;
			pB->AimPart[pB->nPart] = UN_DA_Linii;
			(pB->nPart)++;
			/*************************************/
			nFrom = 0;
			for (i=0; i<2; i++)
			{
				Aim  = UN_DA_Hori;
				Hori = TRUE;
				ShifValuForw = 0;
				pF = pForw;
				SizeF = SizeForw;
				if (i > 0)
				{
					Aim  = UN_DA_Vert;
					Hori = FALSE;
					ShifValuForw = pB->nPartUnits[pB->nPart-1];
					pF = pForw + nFrom;
					SizeF = SizeForw - nFrom;
				}
				/*  Доступ к преобразуемым данным.  */
				Ind = FindSuchAimedData (vB, UN_DT_LineInfo, Aim);
				if (Ind < 0)
					continue;
				/*  Преобразовываем данные.  */
				vFrom = pB->vPart[Ind];
				nFrom = pB->nPartUnits[Ind];
				ret = FullMyLines_Mod (vFrom, nFrom, vB, pF, &SizeF
					, ShifValuForw, pRcReg, SkewReg, Hori, pStr, ContWarn); // t-d
				if (ret!=RV_TRUE)
				{
					CleanLastDataPart (vB);
					CleanLastDataPart (vB);
					return ret;
				}
			}
			LastForLinesAM (vB);
			return ret;
		case UN_LD_LinesVP2 :
			/*  Инициализируем постоянную часть преобразованных данных.  */
			ret = TakeFixedPlace_rv (vB, UN_DT_RltLinePool, UN_DA_Linii, pStr); // f-t-d
			if (ret!=RV_TRUE)
				return ret;
			/*  Инициализируем переменную часть преобразованных данных.  */
			pB->vPart[pB->nPart] = pB->vCurr;
			pB->nPartUnits[pB->nPart] = 0;
			pB->SizePartTotal[pB->nPart] = 0;
			pB->TypePart[pB->nPart] = UN_DT_RltLine;
			pB->AimPart[pB->nPart] = UN_DA_Linii;
			(pB->nPart)++;
			/*************************************/
			nFrom = 0;
			for (i=0; i<2; i++)
			{
				Hori = TRUE;
				Aim  = UN_DA_Hori;
				pF = pForw;
				SizeF = SizeForw;
				if (i > 0)
				{
					Aim  = UN_DA_Vert;
					pF = pForw + nFrom;
					SizeF = SizeForw - nFrom;
					Hori = FALSE;
				}
				/*  Доступ к преобразуемым данным.  */
				Ind = FindSuchAimedData(vB,UN_DT_ClineLine,Aim);
				if (Ind < 0)
					continue;
				/*  Преобразовываем данные.  */
				vFrom = pB->vPart[Ind];
				nFrom = pB->nPartUnits[Ind];
				ret = FullMyLines_Mod (vFrom, nFrom, vB, pF, &SizeF
					, pRcReg, SkewReg, Hori, pStr, ContWarn); // t-d
				if (ret!=RV_TRUE)
				{
					CleanLastDataPart (vB);
					CleanLastDataPart (vB);
					return ret;
				}
			}
			LastForLinesAM (vB);
			return ret;
		default :
			return RV_FALSE;
	}
}
/*----------------------------------------------------------------------------*/
Bool FullMyLines_Mod (void *vLns, int nLns, void *vB, int *pForw, int *SizeForw
	, int ShifValuForw, Rect16 *pRcReg, int32_t SkewReg, Bool Hori, char *pStr, Bool ContWarn)
{
	int i, nLine, SizeCurr, nTeor, LentOld, LentNew;
	LineInfo *pLns;
	Bool IsLimit, BadLine;
	UN_BUFF *pB;
	RLT_LINE *pLine;
	Point16 LinBeg, LinEnd;
	pB = (UN_BUFF *)vB;
	IsLimit = ((pRcReg->left!=0)||(pRcReg->right!=0)||(pRcReg->top!=0)||(pRcReg->bottom!=0));
	pLns = (LineInfo *)vLns;
	pLine = (RLT_LINE *)pB->vCurr;
	nLine = 0;
	SizeCurr = pB->SizeCurr;
	nTeor = sizeof (RLT_LINE);
	for (i=0; i<nLns; i++)
	{
		LinBeg.x = pLns->A.x;
		LinBeg.y = pLns->A.y;
		LinEnd.x = pLns->B.x;
		LinEnd.y = pLns->B.y;
		if (pLns->Flags & LI_NOTWHOLE)
		{
			LinBeg.x = pLns->Anew.x;
			LinBeg.y = pLns->Anew.y;
			LinEnd.x = pLns->Bnew.x;
			LinEnd.y = pLns->Bnew.y;
		}
		LentOld  =  (LinEnd.x-LinBeg.x)*(LinEnd.x-LinBeg.x);
		LentOld += ((LinEnd.y-LinBeg.y)*(LinEnd.y-LinBeg.y));
		LentOld  = (int)sqrt ((double)LentOld);
		if (*SizeForw <= 0)
		{
			sprintf (pStr, "Не хватило памяти под 1 очередной элемент эволюции!");
			return RV_DOUBT;
		}
		pForw[i] = -1;
		(*SizeForw)--;
		BadLine = (pLns->Flags & LI_IsFalse);
		if (!BadLine)
			BadLine = IsWarnAtLine ((void *)pLns, Hori);
		if ((!BadLine)&&IsLimit)
			BadLine = !HaveLinePartAtReg (&LinBeg, &LinEnd, pRcReg, SkewReg, 2*Hori, 2*(!Hori));
		if (!BadLine)
		{
			LentNew  =  (LinEnd.x-LinBeg.x)*(LinEnd.x-LinBeg.x);
			LentNew += ((LinEnd.y-LinBeg.y)*(LinEnd.y-LinBeg.y));
			LentNew  = (int)sqrt ((double)LentNew);
			BadLine = (10*LentNew<LentOld);
		}
		if (!BadLine)
		{
			pLine->Beg.x = LinBeg.x;
			pLine->Beg.y = LinBeg.y;
			pLine->End.x = LinEnd.x;
			pLine->End.y = LinEnd.y;
			FullNewMyLine (pLine, pLns, Hori, ContWarn);
			if (nTeor > pB->SizeCurr)
			{
				sprintf (pStr, "Не хватило памяти под 1 очередную структуру RLT_LINE!");
				return RV_DOUBT;
			}
			pForw[i] = nLine + ShifValuForw;
			nLine++;
			pLine++;
			SizeCurr -= nTeor;
		}
		pLns++;
	}
	/*  формальности с буфером  */
	pB->SizePartUnits[pB->nPart-1] = nTeor;
	pB->nPartUnits[pB->nPart-1] += nLine;
	pB->SizePartTotal[pB->nPart-1] += nLine * nTeor;
	pB->SizeCurr = SizeCurr;
	pB->vCurr = (void *)pLine;
	return RV_TRUE;
}
/*----------------------------------------------------------------------------*/
Bool FullMyLines_Mod (void *vLines, int count, void *vB, int *pForw, int *SizeForw
	, Rect16 *pRcReg, int32_t SkewReg, Bool Hori, char *pStr, Bool ContWarn)
{
	int i, nLine, SizeCurr, nTeor, LentOld, LentNew;
	int move=0;
	DLine* pline=(DLine*)vLines;
	Bool IsLimit, BadLine;
	UN_BUFF *pB;
	RLT_LINE *pLine;
	Point16 LinBeg, LinEnd;
	pB = (UN_BUFF *)vB;
	IsLimit = ((pRcReg->left!=0)||(pRcReg->right!=0)||(pRcReg->top!=0)||(pRcReg->bottom!=0));
	pLine = (RLT_LINE *)pB->vCurr;
	nLine = 0;
	SizeCurr = pB->SizeCurr;
	nTeor = sizeof (RLT_LINE);
	if(!Hori)
		move=pB->nPartUnits[pB->nPart-1];
	for (i=0; i<count; i++,pline++)
	{
		LinBeg.x =(int16_t)pline->Line.Beg_X;
		LinBeg.y =(int16_t)pline->Line.Beg_Y;
		LinEnd.x =(int16_t)pline->Line.End_X;
		LinEnd.y =(int16_t)pline->Line.End_Y;
		LentOld  =  (LinEnd.x-LinBeg.x)*(LinEnd.x-LinBeg.x);
		LentOld += ((LinEnd.y-LinBeg.y)*(LinEnd.y-LinBeg.y));
		LentOld  = (int)sqrt ((double)LentOld);
		if (*SizeForw <= 0)
		{
			sprintf (pStr, "Не хватило памяти под 1 очередной элемент эволюции!");
			return RV_DOUBT;
		}
		pForw[i] = -1;
		(*SizeForw)--;
		BadLine = (pline->Flags & LI_IsFalse);
		if (!BadLine)
			BadLine = IsWarnAtLine ((void*)&pline->Line,Hori,0);
		if ((!BadLine)&&IsLimit)
			BadLine = !HaveLinePartAtReg (&LinBeg, &LinEnd, pRcReg, SkewReg, 2*Hori, 2*(!Hori));
		if (!BadLine)
		{
			LentNew  =  (LinEnd.x-LinBeg.x)*(LinEnd.x-LinBeg.x);
			LentNew += ((LinEnd.y-LinBeg.y)*(LinEnd.y-LinBeg.y));
			LentNew  = (int)sqrt ((double)LentNew);
			BadLine = (10*LentNew<LentOld);
		}
		if (!BadLine)
		{
			pLine->Beg.x = LinBeg.x;
			pLine->Beg.y = LinBeg.y;
			pLine->End.x = LinEnd.x;
			pLine->End.y = LinEnd.y;
			FullNewMyLine (pLine, pline, Hori, ContWarn);
			if (nTeor > pB->SizeCurr)
			{
				sprintf (pStr, "Не хватило памяти под 1 очередную структуру RLT_LINE!");
				return RV_DOUBT;
			}
			pForw[i] = nLine+move;
			nLine++;
			pLine++;
			SizeCurr -= nTeor;
		}
	}
	/*  формальности с буфером  */
	pB->SizePartUnits[pB->nPart-1] = nTeor;
	pB->nPartUnits[pB->nPart-1] += nLine;
	pB->SizePartTotal[pB->nPart-1] += nLine * nTeor;
	pB->SizeCurr = SizeCurr;
	pB->vCurr = (void *)pLine;
	return RV_TRUE;
}
/*----------------------------------------------------------------------------*/
void FullNewMyLine (RLT_LINE *pLine, LineInfo *pLns, Bool Hori, Bool ContWarn)
{
	pLine->Wid   = pLns->Thickness;
	pLine->nSeg  = pLns->SegCnt;
	pLine->Plot  = pLns->Quality;
	if (pLine->Wid<=0)
	{
     #ifdef Almi
		if (ContWarn)
           AM_Console ("Rlt-Warning-Continue : Обнаружена линия неположительной толщины!");
     #endif
		pLine->Wid = 1;
	}
	pLine->Len  = (pLns->A.x - pLns->B.x) * (pLns->A.x - pLns->B.x);
	pLine->Len += (pLns->A.y - pLns->B.y) * (pLns->A.y - pLns->B.y);
	pLine->Len  = (int) sqrt ((double)pLine->Len);
	if (Hori)
		pLine->Skew  = (int)(1024*(pLns->A.y - pLns->B.y)/(double)(pLns->A.x - pLns->B.x) + .5);
	else
		pLine->Skew  = -(int)(1024*(pLns->A.x - pLns->B.x)/(double)(pLns->A.y - pLns->B.y) + .5);
	pLine->Skew2Delta = (int)(1024*(pLine->Wid+2)/(double)pLine->Len + .5);
	MarkLineType ((void *)pLine, Hori, ContWarn);
	if (pLns->Flags & LI_Pointed)
		pLine->Type |= RLT_LT_Pointed;
	if (pLns->Flags & LI_NOTWHOLE)
		pLine->Type |= RLT_LT_Doubt;
	if (!(pLns->Flags & LI_IsTrue))//26.04.01
		pLine->Type |= RLT_LT_Doubt;
}
/*----------------------------------------------------------------------------*/
void FullNewMyLine (RLT_LINE* pLine,DLine* pline,Bool Hori,Bool ContWarn)
{
	NR_SimpLine* pSLine=&(pline->Line);
	pLine->Wid   = pSLine->Wid10/10;
	pLine->nSeg  = (int16_t)pline->n_event;
	pLine->Plot  = (pline->Qual*255)/100;
//	pLine->nSeg=1;
	if (pLine->Wid<=0)
	{
     #ifdef Almi
		if (ContWarn)
			AM_Console ("Rlt-Warning-Continue : Обнаружена линия неположительной толщины!");
     #endif
		pLine->Wid = 1;
	}
	pLine->Len  = (pSLine->Beg_X - pSLine->End_X) * (pSLine->Beg_X - pSLine->End_X);
	pLine->Len += (pSLine->Beg_Y - pSLine->End_Y) * (pSLine->Beg_Y - pSLine->End_Y);
	pLine->Len  = (int) sqrt ((double)pLine->Len);
	if (Hori)
		pLine->Skew  = (int)(1024*(pSLine->Beg_Y - pSLine->End_Y)/(double)(pSLine->Beg_X - pSLine->End_X) + .5);
	else
		pLine->Skew  = -(int)(1024*(pSLine->Beg_X - pSLine->End_X)/(double)(pSLine->Beg_Y - pSLine->End_Y) + .5);
	pLine->Skew2Delta = (int)(1024*(pLine->Wid+2)/(double)pLine->Len + .5);
	MarkLineType ((void *)pLine, Hori, ContWarn);
	if (pline->Flags & LI_Pointed)
		pLine->Type |= RLT_LT_Pointed;
	if (pline->Flags & LI_NOTWHOLE)
		pLine->Type |= RLT_LT_Doubt;
	if (!(pline->Flags & LI_IsTrue))//26.04.01
		pLine->Type |= RLT_LT_Doubt;
}
/*----------------------------------------------------------------------------*/
