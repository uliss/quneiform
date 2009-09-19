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
/*  Редакция   :  12.03.01                                                    */
/*  Файл       :  'Util_Line_VP.CPP'                                          */
/*  Содержание :  Утиль работы с линиями формата LNS.                         */
/*  Назначение :  Формальности ввода/вывода (с унифицированным буфером и без).*/
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
/*#include <windows.h>*/
/*  interface our-other  */
#include "cpage.h"
#include "cline.h"
#include "lnsdefs.h"
#include "rline.h"
/*  interface my        */
#include "un_err.h"
#include "am_comm.h"

#ifdef Almi
#include "FT_Rule.H"
#endif

#include "markdatadefs.h"
/*  interface my-my      */
#include "un_buff.h"
#include "line_vp_util.h" //own functions
#include "puma_err.h"
/*----------------------------------------------------------------------------*/
/* JussiP: this function is never called and the signature clashes with
 * the next one with the same name.
 */
#if 0
Bool LoadLinesVP_rv (Handle hC, int Type, void *vB, char *pStr, uint16_t *pCode)
{
	Bool ret;
	uint32_t KeyHor, KeyVer; //// а ключи надо бы запомнить!
	int CntHor, CntVer;
	UN_BUFF *pB;
	pB = (UN_BUFF *)vB;
	switch ((UN_LOADDATA)Type)
	{
		case UN_LD_LinesVP :
			/***  Общая информация о линиях  ***/
			ret = LoadLinesTotalInfo_rv (hC, vB, pStr); //f-t-e-d
			if (ret!=RV_TRUE)
			{
				if (ret==RV_FALSE)
					Error_CPage_Code (pCode);
				if (ret==RV_DOUBT)
					Error_MyNoMem_Code (pCode);
				return ret;
			}
			ret = GetSomeKeys_rv (vB, &KeyHor, &KeyVer, &CntHor, &CntVer, pStr); //t-e-d
			if (ret==RV_DOUBT)
			{
				Error_MyNoMem_Code (pCode);
			}
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
				{
					Error_CPage_Code (pCode);
					return ret;
				}
			}
			/***  Вертикальные линии  ***/
			if (CntVer>0)
			{
				ret = LoadLinesSpecInfo (hC, vB, KeyVer, CntVer);
				pB->AimPart[pB->nPart-1] = UN_DA_Vert;
				if (!ret)
				{
					Error_CPage_Code (pCode);
				}
				return ret;
			}
			return RV_TRUE;
		default :
			return RV_FALSE;
	}
}
#endif
/*---------------------------------------------------------------------------*/
Bool LoadLinesVP_rv (CLINE_handle hC, int Type, void *vB, char *pStr, uint16_t *pCode)
{
	Bool ret;
	UN_BUFF *pB;
	pB = (UN_BUFF *)vB;
	switch ((UN_LOADDATA)Type)
	{
		case UN_LD_LinesVP2 :
			//Hori lines
			ret = LoadLinesInfo_rv (hC, vB, pStr,TRUE); //f-t-e-d
			pB->AimPart[pB->nPart-1] = UN_DA_Hori;
			if (ret!=RV_TRUE)
			{
				if (ret==RV_FALSE)
					Error_CPage_Code (pCode);
				if (ret==RV_DOUBT)
					Error_MyNoMem_Code (pCode);
				return ret;
			}
			//Vert lines
			ret = LoadLinesInfo_rv (hC, vB, pStr,FALSE); //f-t-e-d
			pB->AimPart[pB->nPart-1] = UN_DA_Vert;
			if (ret!=RV_TRUE)
			{
				if (ret==RV_FALSE)
					Error_CPage_Code (pCode);
				if (ret==RV_DOUBT)
					Error_MyNoMem_Code (pCode);
				return ret;
			}
			return RV_TRUE;
		default :
			return RV_FALSE;
	}
}
/*---------------------------------------------------------------------------*/
void GetKeysPumaVP (void *vLti, Handle *pKeyHor, Handle *pKeyVer)
{
	LinesTotalInfo *pLti;
	pLti = (LinesTotalInfo *)vLti;
	*pKeyHor = pLti->Hor.Lns;
	*pKeyVer = pLti->Ver.Lns;
}
/*---------------------------------------------------------------------------*/
Bool GetSomeKeys_rv (void *vB, Handle *pKeyHor, Handle *pKeyVer
					 , int *pCntHor, int *pCntVer, char *pStr)
{
	UN_BUFF *pB;
	LinesTotalInfo *pLti;
	int SizeNeed;
	pB = (UN_BUFF *)vB;
	pLti = (LinesTotalInfo *)(pB->vPart[pB->nPart-1]);
	SizeNeed = (pLti->Hor.Cnt + pLti->Ver.Cnt) * sizeof (LinesTotalInfo);
	if (SizeNeed > pB->SizeCurr)
	{
		sprintf (pStr, "Не хватило памяти под %d линии!", pLti->Hor.Cnt + pLti->Ver.Cnt);
		return RV_DOUBT;
	}
	if (SizeNeed==0)
	{
		sprintf (pStr, "Линии выделялись, но ни одной не выделено.");
		return RV_EMPTY;
	}
	*pKeyHor = pLti->Hor.Lns;
	*pKeyVer = pLti->Ver.Lns;
	*pCntHor = pLti->Hor.Cnt;
	*pCntVer = pLti->Ver.Cnt;
	return RV_TRUE;
}
/*---------------------------------------------------------------------------*/
Bool LoadLinesTotalInfo_rv (Handle hC, void *vB, char *pStr)
{
	uint32_t err32, nTeor, nReal;
	Handle hBlockLine;
	UN_BUFF *pB;
	pB = (UN_BUFF *)vB;
	/*  ключ к данным  */
	hBlockLine = CPAGE_GetBlockFirst (hC, RLINE_BLOCK_TYPE);
	if (!hBlockLine)
	{
		sprintf (pStr, "Линии не выделялись.");
		return RV_EMPTY;
	}
	err32 = CPAGE_GetReturnCode ();
	if (err32!=0)
	{
       #ifdef Almi
		AM_Console ("RLTABLE : Ошибка чужой библиотеки - [CPAGE]%s", "[GetBlockFirst]");
       #endif
		return RV_FALSE;
	}
	/*  собственно данные  */
	nTeor = sizeof (LinesTotalInfo);
	if ((int)nTeor>pB->SizeCurr)
	{
		sprintf (pStr, "Не хватило памяти под 1, инфо-линии!");
		return RV_DOUBT;
	}
	nReal = CPAGE_GetBlockData (hC, hBlockLine, RLINE_BLOCK_TYPE, pB->vCurr, nTeor);
	err32 = CPAGE_GetReturnCode ();
	if ((nReal!=nTeor)||(err32!=0))
	{
      #ifdef Almi
		AM_Console ("RLTABLE : Ошибка чужой библиотеки - [CPAGE]%s", "[GetBlockData]");
      #endif
		return RV_FALSE;
	}
	EndLastDataPart (vB, UN_DA_Unknown, UN_DT_LinesTotalInfo, nTeor, 1);
	return RV_TRUE;
}
/*---------------------------------------------------------------------------*/
Bool LoadLinesInfo_rv (CLINE_handle hC, void *vB, char *pStr,Bool Hori)
{
	int Cnt;
	uint32_t err32, nTeor;
	UN_BUFF *pB;
	char *vCurr;
	pB = (UN_BUFF *)vB;
	vCurr = (char *)pB->vCurr;
	nTeor = sizeof (DLine);
	Cnt=CLINE_GetLineCount(hC);
	if(Cnt*((int)nTeor)>pB->SizeCurr)
	{
		sprintf (pStr, "Не хватило памяти под 1, инфо-линии!");
		return RV_DOUBT;
	}
	Cnt=0;
	for(CLINE_handle hline=CLINE_GetFirstLine(hC);hline;hline=CLINE_GetNextLine(hline))
	{
		CPDLine cpdata=CLINE_GetLineData(hline);
		err32 = CLINE_GetReturnCode ();
		if(!cpdata)
		{
         #ifdef Almi
			AM_Console ("RLTABLE : Ошибка чужой библиотеки - [CLINE]%s", "[GetLineData]");
         #endif
			return FALSE;
		}
		if(Hori&&cpdata->Dir==LD_Horiz||(!Hori&&cpdata->Dir!=LD_Horiz))
		{
		 CLINE_CopyData(vCurr,cpdata,nTeor);
		 vCurr += nTeor;
		 Cnt++;
		}
	}
	EndLastDataPart (vB,UN_DA_Unknown,UN_DT_ClineLine,nTeor,Cnt);
	return TRUE;
}
/*---------------------------------------------------------------------------*/
Bool LoadLinesSpecInfo (Handle hC, void *vB, Handle Key, int Cnt)
{
	int i;
	uint32_t err32, nTeor, nReal;
	Handle hBlockLineSpec;
	Handle hBlockLinePrev;
	UN_BUFF *pB;
	char *vCurr;
	pB = (UN_BUFF *)vB;
	vCurr = (char *)pB->vCurr;
	nTeor = sizeof (LineInfo);
	for (i=0; i<Cnt; i++)
	{
		/*  ключ к данным  */
		if (i==0)
			hBlockLineSpec = CPAGE_GetBlockFirst (hC, Key);
		else
			hBlockLineSpec = CPAGE_GetBlockNext (hC, hBlockLinePrev, Key);
		err32 = CPAGE_GetReturnCode ();
		if ((err32!=0)||(hBlockLineSpec==NULL))
		{
         #ifdef Almi
			if (i==0)
				AM_Console ("RLTABLE : Ошибка чужой библиотеки - [CPAGE]%s", "[GetBlockFirst]");
			else
				AM_Console ("RLTABLE : Ошибка чужой библиотеки - [CPAGE]%s", "[GetBlockNext]");
         #endif
			return FALSE;
		}
		/*  собственно данные  */
		nReal = CPAGE_GetBlockData (hC, hBlockLineSpec, Key, (void *)vCurr, nTeor);
		err32 = CPAGE_GetReturnCode ();
		if ((nReal!=nTeor)||(err32!=0))
		{
         #ifdef Almi
			AM_Console ("RLTABLE : Ошибка чужой библиотеки - [CPAGE]%s", "[GetBlockData]");
         #endif
			return FALSE;
		}
		hBlockLinePrev = hBlockLineSpec;
		vCurr += nTeor;
	}
	EndLastDataPart (vB, UN_DA_Unknown, UN_DT_LineInfo, nTeor, Cnt);
	return TRUE;
}
/*---------------------------------------------------------------------------*/
Bool MyReSetLines (void *vLti, int MaxNumLin, Handle hCPage, Handle HoriType
				   , Handle VertType, char *pStr)
{
	int i;
	uint32_t err32, nTeor;//, nReal;
	Bool32 nReal;//differ
	Handle hBlockLine;
	Handle hBlockLineHor;
	Handle hBlockLineVer;
	Handle hBlockLinePrev;
	void *Hor, *Ver;
	LinesTotalInfo *pLti;
	pLti = (LinesTotalInfo *)vLti;
	/***  Общая информация о линиях  ***/
	Hor = pLti->Hor.Lns;
	Ver = pLti->Ver.Lns;
	pLti->Hor.Lns = (LineInfo *)HoriType;
	pLti->Ver.Lns = (LineInfo *)VertType;
	hBlockLine = CPAGE_GetBlockFirst (hCPage, RLINE_BLOCK_TYPE);
	err32 = CPAGE_GetReturnCode ();
	if (err32!=0)
	{
		sprintf (pStr, "RLTABLE : Ошибка чужой библиотеки - [CPAGE][GetBlockFirst]");
		return FALSE;
	}
	nTeor = sizeof (LinesTotalInfo);
	nReal = CPAGE_SetBlockData (hCPage, hBlockLine, RLINE_BLOCK_TYPE, (void *)pLti, nTeor);
	err32 = CPAGE_GetReturnCode ();
//	if ((nReal!=nTeor)||(err32!=0))
	if (!nReal||(err32!=0))
	{
		sprintf (pStr, "RLTABLE : Ошибка чужой библиотеки - [CPAGE][SetBlockData]");
		return FALSE;
	}
	pLti->Hor.Lns = (LineInfo *)Hor;
	pLti->Ver.Lns = (LineInfo *)Ver;
	/***  Горизонтальные линии  ***/
	for (i=0; i<pLti->Hor.Cnt; i++)
	{
		if (i==0)
			hBlockLineHor = CPAGE_GetBlockFirst (hCPage, HoriType);
		else
			hBlockLineHor = CPAGE_GetBlockNext (hCPage, hBlockLinePrev, HoriType);
		err32 = CPAGE_GetReturnCode ();
		if (err32!=0)
		{
			if (i==0)
				sprintf (pStr, "RLTABLE : Ошибка чужой библиотеки - [CPAGE][GetBlockFirst]");
			else
				sprintf (pStr, "RLTABLE : Ошибка чужой библиотеки - [CPAGE][GetBlockNext]");
			return FALSE;
		}
		nTeor = sizeof (LineInfo);
		nReal = CPAGE_SetBlockData (hCPage, hBlockLineHor, HoriType, (void *)&(pLti->Hor.Lns[i]), nTeor);
		err32 = CPAGE_GetReturnCode ();
//		if ((nReal!=nTeor)||(err32!=0))
		if (!nReal||(err32!=0))
		{
			sprintf (pStr, "RLTABLE : Ошибка чужой библиотеки - [CPAGE][SetBlockData]");
			return FALSE;
		}
		hBlockLinePrev = hBlockLineHor;
	}
	/***  Вертикальные линии  ***/
	for (i=0; i<pLti->Ver.Cnt; i++)
	{
		if (i==0)
			hBlockLineVer = CPAGE_GetBlockFirst (hCPage, VertType);
		else
			hBlockLineVer = CPAGE_GetBlockNext (hCPage, hBlockLinePrev, VertType);
		err32 = CPAGE_GetReturnCode ();
		if (err32!=0)
		{
			if (i==0)
				sprintf (pStr, "RLTABLE : Ошибка чужой библиотеки - [CPAGE][GetBlockFirst]");
			else
				sprintf (pStr, "RLTABLE : Ошибка чужой библиотеки - [CPAGE][GetBlockNext]");
			return FALSE;
		}
		nTeor = sizeof (LineInfo);
		nReal = CPAGE_SetBlockData (hCPage, hBlockLineVer, VertType, (void *)&(pLti->Ver.Lns[i]), nTeor);
		err32 = CPAGE_GetReturnCode ();
//		if ((nReal!=nTeor)||(err32!=0))
		if (!nReal||(err32!=0))
		{
			sprintf (pStr, "RLTABLE : Ошибка чужой библиотеки - [CPAGE][SetBlockData]");
			return FALSE;
		}
		hBlockLinePrev = hBlockLineVer;
	}
	return TRUE;
}
/*----------------------------------------------------------------------------*/
Bool MyReSetLines(void* vLines,int count,CLINE_handle hCLINE,char *pStr)
{
	uint32_t nTeor;
	nTeor=sizeof(DLine);
	DLine* pbeg=(DLine*)vLines;
	DLine* pend=&(pbeg[count]);
	DLine* p;
	CPDLine cpdata;
	int Beg_X;
	int End_X;
	int Beg_Y;
	int End_Y;
	for(CLINE_handle hline=CLINE_GetFirstLine(hCLINE);hline&&count;hline=CLINE_GetNextLine(hline))
	{
	 cpdata=CLINE_GetLineData(hline);
	 if(!cpdata)
	 {
			sprintf (pStr, "RLTABLE : Ошибка чужой библиотеки - [CLINE][SetLineData]");
			return FALSE;
	 }
	 Beg_X=cpdata->Line.Beg_X;
	 End_X=cpdata->Line.End_X;
	 Beg_Y=cpdata->Line.Beg_Y;
	 End_Y=cpdata->Line.End_Y;
	 for(p=pbeg;p<pend;p++)
	 {
	  if(p->Line.Beg_X==Beg_X&&p->Line.Beg_Y==Beg_Y&&p->Line.End_X==End_X&&p->Line.End_Y==End_Y)
	  {
		if(!CLINE_SetLineData(hline,p))
		{
			sprintf (pStr, "RLTABLE : Ошибка чужой библиотеки - [CLINE][SetLineData]");
			return FALSE;
		}
		count--;
	  }
	 }
	}
	return TRUE;
}
/*----------------------------------------------------------------------------*/
Bool ReferForLinesVP (void *vLti, void *vB)
{
	int Ind;
	Bool IsData;
	UN_BUFF *pB;
	LinesTotalInfo *pLti;
	pLti = (LinesTotalInfo *)vLti;
	pB = (UN_BUFF *)vB;
	IsData = FALSE;
	Ind = FindSuchAimedData (vB, UN_DT_LineInfo, UN_DA_Hori);
	if (Ind >= 0)
	{
		IsData = TRUE;
		pLti->Hor.Lns = (LineInfo *)pB->vPart[Ind];
	}
	Ind = FindSuchAimedData (vB, UN_DT_LineInfo, UN_DA_Vert);
	if (Ind >= 0)
	{
		IsData = TRUE;
		pLti->Ver.Lns = (LineInfo *)pB->vPart[Ind];
	}
	if (!IsData)
		return FALSE;
	return TRUE;
}
/*----------------------------------------------------------------------------*/
Bool PrepareLinesVP_rv (void *vB, void **vvData)
{
	int Ind;
	Bool ret;
	UN_BUFF *pB;
	pB = (UN_BUFF *)vB;
	*vvData = NULL;
	Ind = FindSuchData (vB, UN_DT_LinesTotalInfo);
	if (Ind < 0)
		return RV_EMPTY;
	*vvData = pB->vPart[Ind];
	ret = ReferForLinesVP (*vvData, vB);
	return ret;
}
/*----------------------------------------------------------------------------*/
Bool ReferForAndCountLinesVP (void *vB, void **vvData,int* pcount,Bool Hori)
{
	int Ind;
	UN_BUFF *pB;
	pB = (UN_BUFF *)vB;
	*vvData = NULL;
	if(Hori)
	    Ind = FindSuchAimedData(vB, UN_DT_ClineLine,UN_DA_Hori);
	else
		Ind=FindSuchAimedData(vB, UN_DT_ClineLine,UN_DA_Vert);
	if (Ind < 0)
		return RV_EMPTY;
	*vvData = pB->vPart[Ind];
	*pcount = pB->nPartUnits[Ind];
	return TRUE;
}
/*----------------------------------------------------------------------------*/
Bool IsWarnAtLine (void *vLns, Bool Hori)
{
	LineInfo *pLns = (LineInfo *)vLns;
	if (Hori)
	{
		if (abs(pLns->A.y - pLns->B.y) > abs(pLns->A.x - pLns->B.x))
		{
		   #ifdef Almi
			if (!AM_Skip (FT_GetKeyOfRule (RU_FT_C_ContErr)))
				AM_Console ("Rlt-Error-Ignore : Переданная линия не является горизонтальной! Она игнорируется.");
           #endif
			return TRUE;
		}
		if (pLns->A.x > pLns->B.x)
		{
		   #ifdef Almi
			if (!AM_Skip (FT_GetKeyOfRule (RU_FT_C_ContWarn)))
				AM_Console ("Rlt-Warning-Continue : Плохо описана горизонтальная линия!");
           #endif
			return TRUE;
		}
	}
	else
	{
		if (abs(pLns->A.y - pLns->B.y) < abs(pLns->A.x - pLns->B.x))
		{
          #ifdef Almi
			if (!AM_Skip (FT_GetKeyOfRule (RU_FT_C_ContErr)))
				AM_Console ("Rlt-Error-Ignore : Переданная линия не является вертикальной! Она игнорируется.");
          #endif
			return TRUE;
		}
		if (pLns->A.y > pLns->B.y)
		{
          #ifdef Almi
			if (!AM_Skip (FT_GetKeyOfRule (RU_FT_C_ContWarn)))
				AM_Console ("Rlt-Warning-Continue : Плохо описана вертикальная линия!");
          #endif
			return TRUE;
		}
	}
	return FALSE;
}
/*----------------------------------------------------------------------------*/
Bool IsWarnAtLine (void *vLine,Bool Hori,Bool zero)
{
	NR_SimpLine* pLine = (NR_SimpLine*)vLine;
	if (Hori)
	{
		if (abs(pLine->Beg_Y-pLine->End_Y)>abs(pLine->Beg_X-pLine->End_X))
		{
          #ifdef Almi
			if (!AM_Skip (FT_GetKeyOfRule (RU_FT_C_ContErr)))
				AM_Console ("Rlt-Error-Ignore : Переданная линия не является горизонтальной! Она игнорируется.");
          #endif
			return TRUE;
		}
		if (pLine->Beg_X>pLine->End_X)
		{
          #ifdef Almi
			if (!AM_Skip (FT_GetKeyOfRule (RU_FT_C_ContWarn)))
				AM_Console ("Rlt-Warning-Continue : Плохо описана горизонтальная линия!");
          #endif
			return TRUE;
		}
	}
	else
	{
		if (abs(pLine->Beg_Y-pLine->End_Y)<abs(pLine->Beg_X-pLine->End_X))
		{
          #ifdef Almi
			if (!AM_Skip (FT_GetKeyOfRule (RU_FT_C_ContErr)))
				AM_Console ("Rlt-Error-Ignore : Переданная линия не является вертикальной! Она игнорируется.");
          #endif
			return TRUE;
		}
		if (pLine->Beg_Y>pLine->End_Y)
		{
          #ifdef Almi
			if (!AM_Skip (FT_GetKeyOfRule (RU_FT_C_ContWarn)))
				AM_Console ("Rlt-Warning-Continue : Плохо описана вертикальная линия!");
          #endif
			return TRUE;
		}
	}
	return FALSE;
}
/*----------------------------------------------------------------------------*/
void MarkTableLines (void *vLti, int *pForw, int MyMaxL)
{
	int i, n, Lent;
	LineInfo *pLns;
	LinesTotalInfo *pLti;
	uint32_t AntiFalse, AntiTrue, AntiIsAtTable, AntiIsNotAtTable;
	AntiFalse         = 0xFFFFFFFF;
	AntiFalse        ^= LI_IsFalse;
	AntiTrue          = 0xFFFFFFFF;
	AntiTrue         ^= LI_IsTrue;
	AntiIsAtTable     = 0xFFFFFFFF;
	AntiIsAtTable    ^= LI_IsAtTable;
	AntiIsNotAtTable  = 0xFFFFFFFF;
	AntiIsNotAtTable ^= LI_IsNotAtTable;
	pLti = (LinesTotalInfo *)vLti;
	n = pLti->Hor.Cnt;
	pLns = pLti->Hor.Lns;
	for (i=0; i<n; i++)
	{
		if (LineBringToTable (i, pForw, MyMaxL))
		{
			pLns->Flags |= LI_IsAtTable;
			pLns->Flags &= AntiIsNotAtTable;
		}
		else
		{
			if (!(pLns->Flags & LI_IsAtTable))
				pLns->Flags |= LI_IsNotAtTable;
		}
		pLns++;
	}
	n = pLti->Ver.Cnt;
	pLns = pLti->Ver.Lns;
	for (i=0; i<n; i++)
	{
		if (LineBringToTable (i + MyMaxL, pForw, MyMaxL))
		{
			pLns->Flags |= LI_IsAtTable;
			pLns->Flags &= AntiIsNotAtTable;
			Lent  = (pLns->A.x - pLns->B.x) * (pLns->A.x - pLns->B.x);
			Lent += (pLns->A.y - pLns->B.y) * (pLns->A.y - pLns->B.y);
			Lent  = (int) sqrt ((double)Lent);
			/*  Короткие табличные вертикальные - настоящие!  */
			if (Lent<70)
			{
				pLns->Flags &= AntiFalse;
				pLns->Flags |= LI_IsTrue;
			}
		}
		else
		{
			if (!(pLns->Flags & LI_IsAtTable))
				pLns->Flags |= LI_IsNotAtTable;
		}
		pLns++;
	}
}
/*----------------------------------------------------------------------------*/
void MarkTableLines (void *vHorLines,int hor_count,void *vVerLines,int ver_count, int *pForw, int MyMaxL)
{
	int Lent;
	int i;
	DLine* pline;
	int line_size=sizeof(DLine);
	uint32_t AntiFalse, AntiTrue, AntiIsAtTable, AntiIsNotAtTable;
	AntiFalse         = 0xFFFFFFFF;
	AntiFalse        ^= LI_IsFalse;
	AntiTrue          = 0xFFFFFFFF;
	AntiTrue         ^= LI_IsTrue;
	AntiIsAtTable     = 0xFFFFFFFF;
	AntiIsAtTable    ^= LI_IsAtTable;
	AntiIsNotAtTable  = 0xFFFFFFFF;
	AntiIsNotAtTable ^= LI_IsNotAtTable;
	//Hor lines
	pline=(DLine*)vHorLines;
	for(i=0;i<hor_count;i++,pline++)
	{
		if (LineBringToTable (i, pForw, MyMaxL))
		{
			pline->Flags |= LI_IsAtTable;
			pline->Flags &= AntiIsNotAtTable;
		}
		else
		{
			if (!(pline->Flags & LI_IsAtTable))
				pline->Flags |= LI_IsNotAtTable;
		}
	}

	//Ver lines
	pline=(DLine*)vVerLines;
	ver_count+=MyMaxL;
	for(i=MyMaxL;i<ver_count;i++,pline++)
	{
		if (LineBringToTable (i, pForw, MyMaxL))
		{
			pline->Flags |= LI_IsAtTable;
			pline->Flags &= AntiIsNotAtTable;
			Lent  = (pline->Line.Beg_X - pline->Line.End_X) * (pline->Line.Beg_X - pline->Line.End_X);
			Lent += (pline->Line.Beg_Y - pline->Line.End_Y) * (pline->Line.Beg_Y - pline->Line.End_Y);
			Lent  = (int) sqrt ((double)Lent);
			/*  Короткие табличные вертикальные - настоящие!  */
			if (Lent<70)
			{
				pline->Flags &= AntiFalse;
				pline->Flags |= LI_IsTrue;
			}
		}
		else
		{
			if (!(pline->Flags & LI_IsAtTable))
				pline->Flags |= LI_IsNotAtTable;
		}
	}
}
/*----------------------------------------------------------------------------*/
Bool LineBringToTable (int i, int *pForw, int MyMaxL)
{
	int k, l;
	l = pForw[i];
	for (k=0; k<=3; k++)
	{
		if (l<0)
			return FALSE;
		l = pForw[2*(k+1)*MyMaxL + l];
	}
	return TRUE;
}
/*---------------------------------------------------------------------------*/
