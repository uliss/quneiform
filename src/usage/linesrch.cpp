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

#include <stdio.h>
#include "DingDesc.H"
#include "LineSrch.H"
#include "LineSrchFun.H"
/*------------own functions---------------------------------------------------*/
void MarkDens (int BegT, int EndT, int BegL, int EndL, int *pDens, int nDens);
void FindNextQuasiLine (const RLT_DING *pDing, const int nDing, const FT_SRCH_QUASILINE *pSrch
						, FT_OFOR_LINE *pOforLine, const Bool OnlyStrong);
/*----------------------------------------------------------------------------*/
void MarkDens (int BegT, int EndT, int BegL, int EndL, int *pDens, int nDens)
{
	int j, BegCur, EndCur, Lent;
	if ((BegT>=EndT)||(BegL>=EndL)||(BegL>=EndT)||(BegT>=EndL))
		return;
	BegCur = BegL;
	if (BegCur < BegT)
		BegCur = BegT;
	EndCur = EndL;
	if (EndCur > EndT)
		EndCur = EndT;
	Lent = EndT - BegT;
	BegCur = ((BegCur-BegT)*(nDens-1))/Lent;
	EndCur = ((EndCur-BegT)*(nDens-1))/Lent;
	for (j=BegCur; j<=EndCur; j++)
		pDens[j] = TRUE;
}
/*----------------------------------------------------------------------------*/
void FindNextQuasiLine (const RLT_DING *pDing, const int nDing, const FT_SRCH_QUASILINE *pSrch
						, FT_OFOR_LINE *pOforLine, const Bool OnlyStrong)
{
	int i, jj, j, k, m, BegLev, EndLev, NewBegLev, NewEndLev, TotDens;
	int MaxCurWid, MaxNewWid;
	Bool Density[100], WorkDens[100];
	double RelDens;
	/*  цикл поиска кандидатов в объединители  */
	for (k=0; k<nDing; k++)
	{
		MaxCurWid = 0;
		if (pSrch->FromBeg)
			i = k;
		else
			i = nDing - 1 - k;
		if (!(pDing[i].Type&RLT_DT_Line))
			continue;
		if (OnlyStrong)
		{
			if (pDing[i].Type&RLT_DT_Pointed)
				continue;
			if (pDing[i].Type&RLT_DT_Doubt)
				continue;
		}
		if (pDing[i].Level<pSrch->MinLev)
			continue;
		if (pDing[i].Level>pSrch->MaxLev)
			continue;
		BegLev = pDing[i].Level - pDing[i].Width/2;
		EndLev = pDing[i].Level + pDing[i].Width/2;
		if (EndLev-BegLev > pSrch->MaxWidUnion)
			continue;
		for (j=0; j<100; j++)
		{
			Density[j] = FALSE;
		}
		MarkDens (pSrch->Beg, pSrch->End, pDing[i].Beg, pDing[i].End, Density, 100);
		/*  проверяем объединителя  */
		TotDens = 0;
		for (j=0; j<100; j++)
		{
			if (Density[j])
				TotDens++;
		}
		if (TotDens==0)
			continue;
		MaxCurWid = pDing[i].Width;
		/*  объединитель собирает союзников  */
		for (m=0; m<nDing; m++)
		{
			if (pSrch->FromBeg)
				j = m;
			else
				j = nDing - 1 - m;
			if (j==i)
				continue;
			if (!(pDing[j].Type&RLT_DT_Line))
				continue;
			if (OnlyStrong)
			{
				if (pDing[j].Type&RLT_DT_Pointed)
					continue;
				if (pDing[j].Type&RLT_DT_Doubt)
					continue;
			}
			if (pDing[j].Level<pSrch->MinLev)
				continue;
			if (pDing[j].Level>pSrch->MaxLev)
				continue;
			/* не слабоват ли для такого союзника сам объединитель */
			if (pDing[j].End-pDing[j].Beg > pDing[i].End-pDing[i].Beg) //07.06.01
				continue;
			NewBegLev = BegLev;
			NewEndLev = EndLev;
			if (NewBegLev > pDing[j].Level - pDing[j].Width/2)
				NewBegLev = pDing[j].Level - pDing[j].Width/2;
			if (NewEndLev < pDing[j].Level + pDing[j].Width/2)
				NewEndLev = pDing[j].Level + pDing[j].Width/2;
			MaxNewWid = MaxCurWid;
			if (MaxNewWid < pDing[j].Width)
				MaxNewWid = pDing[j].Width;
			if (NewEndLev-NewBegLev > pSrch->MaxWidUnion + MaxNewWid)
				continue;
			/*  проверяем союзника  */
			for (jj=0; jj<100; jj++)
			{
				WorkDens[jj] = FALSE;
			}
			MarkDens (pSrch->Beg, pSrch->End, pDing[j].Beg, pDing[j].End, WorkDens, 100);
			TotDens = 0;
			for (jj=0; jj<100; jj++)
			{
				if (WorkDens[jj])
					TotDens++;
			}
			if (TotDens==0)
				continue;
			BegLev = NewBegLev;
			EndLev = NewEndLev;
			MarkDens (pSrch->Beg, pSrch->End, pDing[j].Beg, pDing[j].End, Density, 100);
			if (MaxCurWid < pDing[j].Width)
				MaxCurWid = pDing[j].Width;
		}
		/*  проверяем улов  */
		TotDens = 0;
		for (j=0; j<100; j++)
		{
			if (Density[j])
				TotDens++;
		}
		RelDens = (double)TotDens/100.;
		if (RelDens < 1.-pSrch->MaxHoleDens)
			continue;
		pOforLine->Level = EndLev + BegLev;
		if (pOforLine->Level>=0)
			pOforLine->Level = (pOforLine->Level+1)/2;
		else
			pOforLine->Level = (pOforLine->Level-1)/2;
		pOforLine->Level = pDing[i].Level; //07.06.01
		pOforLine->Width = EndLev - BegLev;
		pOforLine->WasFound = TRUE;
		return;
	}
}
/*----------------------------------------------------------------------------*/
void FindAllQuasiLines (const RLT_DINGPOOL *pDingPool, const Rect16 *pRcReg, const Bool Hori
						, int *pLev, int *nLev, const int MaxNumLev
						, const double MaxHoleDens, const int MaxWidUnion)
{
//	int nDing;
//	RLT_DING *pDing;
	FT_SRCH_QUASILINE Srch = {0};
	FT_OFOR_LINE    L_Cur = {0};
	Srch.FromBeg = TRUE;
	Srch.MaxHoleDens = MaxHoleDens;
	Srch.MaxWidUnion = MaxWidUnion;
	*nLev = 0;
	if (Hori)
	{
		Srch.Beg    = pRcReg->left;
		Srch.End    = pRcReg->right;
		Srch.MaxLev = pRcReg->bottom - 30;
		L_Cur.Level = pRcReg->top;
//		pDing = pDingPool->Hori;
//		nDing = pDingPool->nHori;
	}
	else
	{
		Srch.Beg    = pRcReg->top;
		Srch.End    = pRcReg->bottom;
		Srch.MaxLev = pRcReg->right - 30;
		L_Cur.Level = pRcReg->left;
//		pDing = pDingPool->Vert;
//		nDing = pDingPool->nVert;
	}
	while (1)
	{
		L_Cur.WasFound = FALSE;
		Srch.MinLev  = L_Cur.Level + 1;//3;
        if (Hori)
            FindNextQuasiLine (pDingPool->Hori, pDingPool->nHori, &Srch, &L_Cur, FALSE);
        else
            FindNextQuasiLine (pDingPool->Vert, pDingPool->nVert, &Srch, &L_Cur, FALSE);
		if (!L_Cur.WasFound)
			return;
		if (*nLev >= MaxNumLev)
			return;
		pLev[*nLev] = L_Cur.Level;
		(*nLev)++;
	}
}
/*----------------------------------------------------------------------------*/
