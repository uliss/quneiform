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

/*----------------------------------------------------------------------------*/
#include <stdio.h>
/*  interface our-other  */
#include "polyblock.h"
#include "cpage.h"
/*  interface our-our    */
#include "un_err.h"
#include "un_buff.h"
/*  interface my-my      */
#include "amt_geom.h"
#include "markdataoper.h" //own functions
/*----------------------------------------------------------------------------*/
Bool FullRotatedRects_rv (void *vB, UN_DATAAIMS OldAim, UN_DATAAIMS NewAim
					   , int32_t SkewReg, char *pStr, char *pWho)
{
	int Ind, nTeor, nPartUnits, i;
	UN_BUFF *pB;
	Rect16 *pRcRe, *pRcId;
	pB = (UN_BUFF *)vB;
	/*  Доступ к преобразуемым данным.  */
	Ind = FindSuchAimedData (vB, UN_DT_Rect16, OldAim);
	if (Ind < 0)
		return RV_FALSE;
	nTeor = sizeof (Rect16);
	nPartUnits = pB->nPartUnits[Ind];
	if (nPartUnits > pB->SizeCurr/nTeor)
	{
		sprintf (pStr, "Не хватило памяти под %d %s!"
			, nPartUnits-pB->SizeCurr/nTeor, pWho);
		return RV_DOUBT;
	}
	pRcRe = (Rect16 *)(pB->vPart[Ind]);
	pRcId = (Rect16 *)(pB->vCurr);
	for (i=0; i<nPartUnits; i++)
	{
		RotateRect (pRcRe, pRcId, SkewReg);
		pRcRe++;
		pRcId++;
	}
	EndLastDataPart (vB, NewAim, UN_DT_Rect16, nTeor, nPartUnits);
	return RV_TRUE;
}
/*---------------------------------------------------------------------------*/
Bool CompIsGood (CCOM_comp * pcomp, int16_t Filter)
{
	switch (Filter)
	{
		case 0 :
			return TRUE;
		case 1 :
			if ((pcomp->upper<10)||(pcomp->left<10))
				return FALSE;
			if ((pcomp->h<=4)&&(pcomp->w<=20)&&(pcomp->w>=10)) //тире
				return TRUE;
			if ((pcomp->w<7)||(pcomp->w>120))
				return FALSE;
			if ((pcomp->h<10)||(pcomp->h>100))
				return FALSE;
			if (pcomp->h > (int)(4.8*pcomp->w + .5))
				return FALSE;
			if ((pcomp->h<20)&&(pcomp->w > (int)(2.1*pcomp->h + .5)))
				return FALSE;
			return TRUE;
		case 2 :
			if ((pcomp->h<=4)&&(pcomp->w<=4))
				return FALSE;
			return TRUE;
		case 3 :
			if ((pcomp->h<=4)||(pcomp->w<=4))
				return FALSE;
			return TRUE;
		default :
			return FALSE;
	}
}
/*---------------------------------------------------------------------------*/
Bool LoadComps_rv (Handle hC, void *vB, char *pStr, int Filter)
{
	CCOM_comp * pcomp;
	Bool GoodComp, Fir;
	int SizeCurr, Deficit, nTeor, nRc;
	UN_BUFF *pB;
	Rect16 *pCurr;
	pB = (UN_BUFF *)vB;
	nRc = 0;
	Deficit = 0;
	pCurr = (Rect16 *)pB->vCurr;
	SizeCurr = pB->SizeCurr;
	nTeor = sizeof (Rect16);
	Fir = TRUE;
	while (1)
	{
		/*  ключ к данным  */
		if (Fir)
		{
			pcomp = CCOM_GetFirst(hC, NULL);
			Fir = FALSE;
		}
		else
			pcomp = CCOM_GetNext (pcomp, NULL);
		if (pcomp==NULL)
			break;
		GoodComp = CompIsGood (pcomp, Filter);
		if (!GoodComp)
			continue;
		if (nTeor > SizeCurr)
		{
			Deficit++;
			continue;
		}
		nRc++;
		/*  собственно данные  */
		pCurr->left   = pcomp->left;
		pCurr->right  = pcomp->left + pcomp->w - 1;
		pCurr->top    = pcomp->upper;
		pCurr->bottom = pcomp->upper + pcomp->h - 1;
		pCurr++;
		SizeCurr -= sizeof (Rect16);
	}
	if (nRc==0)
	{
		sprintf (pStr, "Нет компонент");
		return RV_EMPTY;
	}
	EndLastDataPart (vB, UN_DA_CompReal, UN_DT_Rect16, nTeor, nRc);
	if (Deficit > 0)
	{
		sprintf (pStr, "Не хватило памяти под %d %s!", Deficit, "коробки компонент");
		return RV_DOUBT;
	}
	return RV_TRUE;
}
/*---------------------------------------------------------------------------*/
Bool MakeRectFromPict (Rect16 *pCurr, void *vPict)
{
	int n;
	POLY_ *pPict;
	pPict = (POLY_ *)vPict;
	n = pPict->com.count;
	if (n!=4)
		return FALSE;
	if (pPict->com.Vertex[0].x!=pPict->com.Vertex[3].x)
		return FALSE;
	if (pPict->com.Vertex[1].x!=pPict->com.Vertex[2].x)
		return FALSE;
	if (pPict->com.Vertex[0].y!=pPict->com.Vertex[1].y)
		return FALSE;
	if (pPict->com.Vertex[3].y!=pPict->com.Vertex[2].y)
		return FALSE;
	if (pPict->com.Vertex[0].x>=pPict->com.Vertex[1].x)
		return FALSE;
	if (pPict->com.Vertex[0].y>=pPict->com.Vertex[3].y)
		return FALSE;
	pCurr->left   = (int16_t)pPict->com.Vertex[0].x;
	pCurr->right  = (int16_t)pPict->com.Vertex[1].x;
	pCurr->top    = (int16_t)pPict->com.Vertex[0].y;
	pCurr->bottom = (int16_t)pPict->com.Vertex[3].y;
	return TRUE;
}
/*---------------------------------------------------------------------------*/
Bool LoadPicts_rv (Handle hC, void *vB, char *pStr)
{
	int SizeCurr, Deficit, nRc;
	uint32_t err32, nTeor, nReal;
	Bool ret;
	POLY_ Pict;
	Handle hBlockPictSpec;
	Handle hBlockPictPrev;
	UN_BUFF *pB;
	Rect16 *pCurr;
	pB = (UN_BUFF *)vB;
	nRc = 0;
	Deficit = 0;
	pCurr = (Rect16 *)pB->vCurr;
	SizeCurr = pB->SizeCurr;
	nTeor = sizeof (POLY_);
	hBlockPictPrev = NULL;
	while (1)
	{
		/*  ключ к данным  */
		if (hBlockPictPrev==NULL)
			hBlockPictSpec = CPAGE_GetBlockFirst (hC, TYPE_IMAGE);
		else
			hBlockPictSpec = CPAGE_GetBlockNext (hC, hBlockPictPrev, TYPE_IMAGE);
		err32 = CPAGE_GetReturnCode ();
		if (hBlockPictSpec==NULL)
			break;
		if (err32!=0)
		{
			if (hBlockPictPrev==NULL)
				sprintf (pStr, "[GetBlockFirst]");
			else
				sprintf (pStr, "[GetBlockNext]");
			return FALSE;
		}
		/*  собственно данные  */
		nReal = CPAGE_GetBlockData (hC, hBlockPictSpec, TYPE_IMAGE, (void *)(&Pict), nTeor);
		err32 = CPAGE_GetReturnCode ();
		if ((nReal!=nTeor)||(err32!=0))
		{
			sprintf (pStr, "[GetBlockData]");
			return FALSE;
		}
		hBlockPictPrev = hBlockPictSpec;
		if ((int)nTeor > SizeCurr)
		{
			Deficit++;
			continue;
		}
		nRc++;
		ret = MakeRectFromPict (pCurr, (void *)(&Pict));
	#include "am_comm.h"
#ifdef Almi
	#include "ft_rule.h"
		if (!ret)
		{
			if (!AM_Skip (FT_GetKeyOfRule (RU_FT_C_ContWarn)))
				AM_Console ("Не удалось осмыслить картинку!");
			continue;
		}
#endif
		pCurr++;
		SizeCurr -= sizeof (Rect16);
	}
	if (nRc==0)
	{
		sprintf (pStr, "Нет картинок");
		return RV_EMPTY;
	}
	EndLastDataPart (vB, UN_DA_PictReal, UN_DT_Rect16, sizeof (Rect16), nRc);
	if (Deficit > 0)
	{
		sprintf (pStr, "Не хватило памяти под %d %s!", Deficit, "коробки картинок");
		return RV_DOUBT;
	}
	return RV_TRUE;
}
/*----------------------------------------------------------------------------*/
