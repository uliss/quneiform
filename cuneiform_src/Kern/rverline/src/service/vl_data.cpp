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
/*  Редакция   :  06.09.00                                                    */
/*  Файл       :  'VL_Data.CPP'                                              */
/*  Содержание :  Функции извлечения чужих и представления своих данных.      */
/*  Назначение :  Обмен с другими библиотеками.                               */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
/*#include <windows.h>*/
/*  interface our-other  */
#include "cpage.h"
#include "lnsdefs.h"
#include "rline.h"
#include "ccom.h"
#include "ctiimage.h"
#include "cline.h"
/*  interface our-my     */
#include "rverline.h"
/*  interface my        */
#include "am_err.h"
#include "am_comm.h"
#include "myraster.h"
/*  interface my-my      */
#include "am_buff.h"
/*------------own objects-----------------------------------------------------*/
FNCPAGE_GetBlockFirst GetBlockFirst;
FNCPAGE_GetBlockNext GetBlockNext;
FNCPAGE_GetBlockData GetBlockData;
/*------------extern functions------------------------------------------------*/
void MyErrorNoMem (const char* str);
void   SetReturnCode_rverline (uint16_t rc);
/*------------own functions---------------------------------------------------*/
Bool MyInit_CPage ();
Bool MyGetLines (LinesTotalInfo *pLti, int MaxNumLin, Handle hCPage, uint32_t *pHoriType, uint32_t *pVertType, char *pStr);
Bool MyGetComp (Handle hCCOM, Rect16 *pRc, int *nRC, int MyMaxC, int Filter);
void Error_CPage (const char *str);
Bool MyFormZhertvy (Handle hCCOM, void **vvZher, int *iZher, int nZher, int Filter);
Bool MySetZher (void **vvZher, int nZher, Handle hCPage);
Bool MyGetZher (void **vvZher, int *nZher, int MaxZher, Handle hCPage);
Bool MyGetRaster (Handle hCPage, VL_I_TASK *pTask, uchar **ppData);
/*----------------------------------------------------------------------------*/
Bool MyInit_CPage ()
{
	Bool ret;
	ret = CPAGE_GetExportData (CPAGE_FNCPAGE_GetBlockFirst, &GetBlockFirst);
	if (!ret)
	{
		Error_CPage ("[GetExportData][GetBlockFirst]");
		return FALSE;
	}
	ret = CPAGE_GetExportData (CPAGE_FNCPAGE_GetBlockNext, &GetBlockNext);
	if (!ret)
	{
		Error_CPage ("[GetExportData][GetBlockNext]");
		return FALSE;
	}
	ret = CPAGE_GetExportData (CPAGE_FNCPAGE_GetBlockData, &GetBlockData);
	if (!ret)
	{
		Error_CPage ("[GetExportData][GetBlockData]");
		return FALSE;
	}
	return TRUE;
}
/*----------------------------------------------------------------------------*/
Bool MyGetLines (LinesTotalInfo *pLti, int MaxNumLin, Handle *hCPage, Handle *pHoriType, Handle *pVertType, char *pStr)
{
	int i;
	uint32_t err32, nTeor, nReal;
	Handle hBlockLine;
	Handle hBlockLineHor;
	Handle hBlockLineVer;
	Handle hBlockLinePrev;
	LineInfo *pLHor, *pLVer;
	/***    ***/
	pLHor = pLti->Hor.Lns;
	pLVer = pLti->Ver.Lns;
	/***  Общая информация о линиях  ***/
	hBlockLine = CPAGE_GetBlockFirst (hCPage, RLINE_BLOCK_TYPE);
	if (!hBlockLine)
	{
		sprintf (pStr, "Линии не выделялись.");
		return RV_EMPTY;
	}
	err32 = CPAGE_GetReturnCode ();
	if (err32!=0)
	{
		Error_CPage ("[GetBlockFirst]");
		return FALSE;
	}
	nTeor = sizeof (LinesTotalInfo);
	nReal = CPAGE_GetBlockData (hCPage, hBlockLine, RLINE_BLOCK_TYPE, (void *)pLti, nTeor);
	err32 = CPAGE_GetReturnCode ();
	if ((nReal!=nTeor)||(err32!=0))
	{
		Error_CPage ("[GetBlockData]");
		return FALSE;
	}
	if (pLti->Hor.Cnt + pLti->Ver.Cnt >= MaxNumLin)
	{
		sprintf (pStr, "Не хватило памяти под %d линии!", pLti->Hor.Cnt + pLti->Ver.Cnt);
		return RV_DOUBT;
	}
	if ((pLti->Hor.Cnt==0)&&(pLti->Ver.Cnt==0))
	{
		sprintf (pStr, "Линии выделялись, но ни одной не выделено.");
		return RV_EMPTY;
	}
	/***  Горизонтальные линии  ***/
	for (i=0; i<pLti->Hor.Cnt; i++)
	{
		if (i==0)
			hBlockLineHor = CPAGE_GetBlockFirst (hCPage, (pLti->Hor.Lns));
		else
			hBlockLineHor = CPAGE_GetBlockNext (hCPage, hBlockLinePrev, (pLti->Hor.Lns));
		err32 = CPAGE_GetReturnCode ();
		if (err32!=0)
		{
			if (i==0)
				Error_CPage ("[GetBlockFirst]");
			else
				Error_CPage ("[GetBlockNext]");
			return FALSE;
		}
		nTeor = sizeof (LineInfo);
		nReal = CPAGE_GetBlockData (hCPage, hBlockLineHor, (pLti->Hor.Lns), (void *)&(pLHor[i]), nTeor);
		err32 = CPAGE_GetReturnCode ();
		if ((nReal!=nTeor)||(err32!=0))
		{
			Error_CPage ("[GetBlockData]");
			return FALSE;
		}
		hBlockLinePrev = hBlockLineHor;
	}
	/***  Вертикальные линии  ***/
	for (i=0; i<pLti->Ver.Cnt; i++)
	{
		if (i==0)
			hBlockLineVer = CPAGE_GetBlockFirst (hCPage, (pLti->Ver.Lns));
		else
			hBlockLineVer = CPAGE_GetBlockNext (hCPage, hBlockLinePrev, (pLti->Ver.Lns));
		err32 = CPAGE_GetReturnCode ();
		if (err32!=0)
		{
			if (i==0)
				Error_CPage ("[GetBlockFirst]");
			else
				Error_CPage ("[GetBlockNext]");
			return FALSE;
		}
		nTeor = sizeof (LineInfo);
		nReal = CPAGE_GetBlockData (hCPage, hBlockLineVer, (pLti->Ver.Lns), (void *)&(pLVer[i]), nTeor);
		err32 = CPAGE_GetReturnCode ();
		if ((nReal!=nTeor)||(err32!=0))
		{
			Error_CPage ("[GetBlockData]");
			return FALSE;
		}
		hBlockLinePrev = hBlockLineVer;
	}
	/***    ***/
	*pHoriType = pLti->Hor.Lns;
	*pVertType = pLti->Ver.Lns;
	pLti->Hor.Lns = pLHor;
	pLti->Ver.Lns = pLVer;
	return TRUE;
}
/*----------------------------------------------------------------------------*/
Bool MyGetLines (LinesTotalInfo *pLti, int MaxNumLin,CLINE_handle hCLINE,char *pStr)
{
	CLINE_handle hline;
	int count_hor=0;
	int count_ver=0;
	LineInfo *pLHor, *pLVer;
	int count=CLINE_GetLineCount(hCLINE);
	if(!count)
	{
		sprintf (pStr, "Линии не выделялись.");
		return RV_EMPTY;
	}
	if (count>=MaxNumLin)
	{
		sprintf (pStr, "Не хватило памяти под %d линии!", pLti->Hor.Cnt + pLti->Ver.Cnt);
		return RV_DOUBT;
	}

	/***    ***/
	pLHor = pLti->Hor.Lns;
	pLVer = pLti->Ver.Lns;

	for(hline=CLINE_GetFirstLine(hCLINE);hline;hline=CLINE_GetNextLine(hline))
	{
		CPDLine cpdata=CLINE_GetLineData(hline);
		if(!cpdata)
		{
		 return FALSE;
		}
//Andrey 05.09.01
        if (cpdata->Flags&LI_Pointed) continue;

		if(cpdata->Dir==LD_Horiz)
		{
			pLHor[count_hor].A.x=(int16_t)(cpdata->Line.Beg_X);
			pLHor[count_hor].A.y=(int16_t)(cpdata->Line.Beg_Y);
			pLHor[count_hor].B.x=(int16_t)(cpdata->Line.End_X);
			pLHor[count_hor].B.y=(int16_t)(cpdata->Line.End_Y);
			pLHor[count_hor].Thickness=cpdata->Line.Wid10/10;
			pLHor[count_hor].SegCnt=(int16_t)(cpdata->n_event);
			pLHor[count_hor].Quality=(cpdata->Dens*255)/100;
			pLHor[count_hor].Flags=cpdata->Flags;
			count_hor++;
		}
		else
		{
			pLVer[count_ver].A.x=(int16_t)(cpdata->Line.Beg_X);
			pLVer[count_ver].A.y=(int16_t)(cpdata->Line.Beg_Y);
			pLVer[count_ver].B.x=(int16_t)(cpdata->Line.End_X);
			pLVer[count_ver].B.y=(int16_t)(cpdata->Line.End_Y);
			pLVer[count_ver].Thickness=cpdata->Line.Wid10/10;
			pLVer[count_ver].SegCnt=(int16_t)(cpdata->n_event);
			pLVer[count_ver].Quality=(cpdata->Dens*255)/100;
			pLVer[count_ver].Flags=cpdata->Flags;
			count_ver++;
		}

	}

	/***    ***/
	pLti->Hor.Lns = pLHor;
	pLti->Ver.Lns = pLVer;
	pLti->Hor.Cnt = count_hor;
	pLti->Ver.Cnt = count_ver;
	return TRUE;
}
/*----------------------------------------------------------------------------*/
Bool MyReSetLines (void *vLti, int MaxNumLin, Handle hCPage, Handle HoriType, Handle VertType)
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
		Error_CPage ("[GetBlockFirst]");
		return FALSE;
	}
	nTeor = sizeof (LinesTotalInfo);
	nReal = CPAGE_SetBlockData (hCPage, hBlockLine, RLINE_BLOCK_TYPE, (void *)pLti, nTeor);
	err32 = CPAGE_GetReturnCode ();
//	if ((nReal!=nTeor)||(err32!=0))
	if (!nReal||(err32!=0))
	{
		Error_CPage ("[SetBlockData]");
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
				Error_CPage ("[GetBlockFirst]");
			else
				Error_CPage ("[GetBlockNext]");
			return FALSE;
		}
		nTeor = sizeof (LineInfo);
		if(pLti->Hor.Lns[i].Flags&LI_Pointed)
		  pLti->Hor.Lns[i].Flags|=LI_IsTrue;
		nReal = CPAGE_SetBlockData (hCPage, hBlockLineHor, HoriType, (void *)&(pLti->Hor.Lns[i]), nTeor);
		err32 = CPAGE_GetReturnCode ();
//		if ((nReal!=nTeor)||(err32!=0))
		if (!nReal||(err32!=0))
		{
			Error_CPage ("[SetBlockData]");
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
				Error_CPage ("[GetBlockFirst]");
			else
				Error_CPage ("[GetBlockNext]");
			return FALSE;
		}
		nTeor = sizeof (LineInfo);
		if(pLti->Ver.Lns[i].Flags&LI_Pointed)
		  pLti->Ver.Lns[i].Flags|=LI_IsTrue;
		nReal = CPAGE_SetBlockData (hCPage, hBlockLineVer, VertType, (void *)&(pLti->Ver.Lns[i]), nTeor);
		err32 = CPAGE_GetReturnCode ();
//		if ((nReal!=nTeor)||(err32!=0))
		if (!nReal||(err32!=0))
		{
			Error_CPage ("[SetBlockData]");
			return FALSE;
		}
		hBlockLinePrev = hBlockLineVer;
	}
	return TRUE;
}
/*----------------------------------------------------------------------------*/
Bool MyReSetLines (void *vLti, int MaxNumLin, CLINE_handle hCLINE)
{
	LinesTotalInfo *pLti;
	pLti = (LinesTotalInfo *)vLti;
	CLINE_handle hline;
	DLine data;
	LineInfo* HorLine=pLti->Hor.Lns;
	LineInfo* VerLine=pLti->Ver.Lns;
	int count_hor=0;
	int count_ver=0;
	int size_dline=sizeof(DLine);

	for(hline=CLINE_GetFirstLine(hCLINE);hline;hline=CLINE_GetNextLine(hline))
	{
		CPDLine cpdata=CLINE_GetLineData(hline);
		if(!cpdata)
		{
		 return FALSE;
		}

//Andrey 05.09.01
        if (cpdata->Flags&LI_Pointed)
            continue;

		if(!CLINE_CopyData(&data,cpdata,size_dline))
			return FALSE;

		if(cpdata->Dir==LD_Horiz)
		{
			data.Flags=HorLine[count_hor].Flags;
			if(HorLine[count_hor].Flags&LI_Pointed)
			{
				data.Flags|=LI_IsTrue;
				data.Type=NR_DT_LinePointed;
			}
			data.lns_data.Anew.x=HorLine[count_hor].Anew.x;
			data.lns_data.Anew.y=HorLine[count_hor].Anew.y;
			data.lns_data.Bnew.x=HorLine[count_hor].Bnew.x;
			data.lns_data.Bnew.y=HorLine[count_hor].Bnew.y;

			CLINE_SetLineData(hline,(CPDLine)(&data));
			count_hor++;
		}
		else
		{
			data.Flags=VerLine[count_ver].Flags;
			if(VerLine[count_ver].Flags&LI_Pointed)
			{
				data.Flags|=LI_IsTrue;
				data.Type=NR_DT_LinePointed;
			}
			data.lns_data.Anew.x=VerLine[count_ver].Anew.x;
			data.lns_data.Anew.y=VerLine[count_ver].Anew.y;
			data.lns_data.Bnew.x=VerLine[count_ver].Bnew.x;
			data.lns_data.Bnew.y=VerLine[count_ver].Bnew.y;

			CLINE_SetLineData(hline,(CPDLine)(&data));
			count_ver++;
		}

	}
	return TRUE;
}
/*----------------------------------------------------------------------------*/
void Error_CPage (const char *str)
{
	uchar  err8;
	uint16_t Code;
	err8 = (uchar)ER_ROUGH_NORMAL;
	Code = (uint16_t)(err8<<8);
	err8 = (uchar)ER_DETAIL_FUNC_CPAGE;
	Code |= (uint16_t)err8;
	SetReturnCode_rverline (Code);
	AM_Console ("RVERLINE : Ошибка чужой библиотеки - [CPAGE]%s", str);
}
/*----------------------------------------------------------------------------*/
Bool CompIsGood (CCOM_comp * pcomp, int Filter)
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
/*----------------------------------------------------------------------------*/
Bool MyGetComp (Handle hCCOM, Rect16 *pRc, int *nRC, int MyMaxC, int Filter)
{
	CCOM_comp * pcomp;
	Bool GoodComp;
	pcomp = CCOM_GetFirst (hCCOM, NULL);
	*nRC = 0;
	GoodComp = CompIsGood (pcomp, Filter);
	if (GoodComp)
	{
		pRc[*nRC].left   = pcomp->left;
		pRc[*nRC].right  = pcomp->left + pcomp->w - 1;
		pRc[*nRC].top    = pcomp->upper;
		pRc[*nRC].bottom = pcomp->upper + pcomp->h - 1;
		(*nRC)++;
	}
	while (1)
	{
		pcomp = CCOM_GetNext (pcomp, NULL);
		if (pcomp==NULL)
			break;
		GoodComp = CompIsGood (pcomp, Filter);
		if (!GoodComp)
			continue;
		if (*nRC>=MyMaxC)
		{
			MyErrorNoMem ("коробки компонент");
			return FALSE;
		}
		pRc[*nRC].left   = pcomp->left;
		pRc[*nRC].right  = pcomp->left + pcomp->w - 1;
		pRc[*nRC].top    = pcomp->upper;
		pRc[*nRC].bottom = pcomp->upper + pcomp->h - 1;
		(*nRC)++;
	}
	return TRUE;
}
/*----------------------------------------------------------------------------*/
Bool MyFormZhertvy (Handle hCCOM, void **vvZher, int *iZher, int nZher, int Filter)
{
	CCOM_comp * pcomp;
	int nC, nX, i, nZ;
	Bool GoodComp;
	nC = 0, nX = 0, nZ = 0;
	while (1)
	{
		if (nZ==nZher)
			break;
		if (nX==0)
			pcomp = CCOM_GetFirst (hCCOM, NULL);
		else
			pcomp = CCOM_GetNext (pcomp, NULL);
		if (pcomp==NULL)
			break;
		nX++;
		GoodComp = CompIsGood (pcomp, Filter);
		if (!GoodComp)
			continue;
		for (i=0; i<nZher; i++)
		{
			if (nC!=iZher[i])
				continue;
			vvZher[nZ] = (void *)pcomp;
			nZ++;
		}
		nC++;
	}
	if (nZ==nZher)
		return TRUE;
	else
		return FALSE;
}
/*----------------------------------------------------------------------------*/
Bool MySetZher (void **vvZher, int nZher, Handle hCPage)
{
	uint32_t err32, nTeor;//, nReal;//dwTableType
	Handle hBlockZher;
	int i;
	nTeor = sizeof (void *);
	for (i=0; i<nZher; i++)
	{
		hBlockZher = CPAGE_CreateBlock
			(hCPage, RVERLINE_ZHERTVY_LINIY, 0, 0, (void *)&(vvZher[i]) , nTeor);
		err32 = CPAGE_GetReturnCode ();
		if ((hBlockZher==NULL)||(err32!=0))
		{
			Error_CPage ("[CreateBlock]");
			return FALSE;
		}
/*		nReal = CPAGE_SetBlockData (hCPage, hBlockZher, RVERLINE_ZHERTVY_LINIY, (void *)&(vvZher[i]), nTeor);
		err32 = CPAGE_GetReturnCode ();
//		if ((nReal!=nTeor)||(err32!=0))
		if (!nReal||(err32!=0))
		{
			Error_CPage ("[GetBlockData]");
			return FALSE;
		}*/
	}
	return TRUE;
}
/*----------------------------------------------------------------------------*/
Bool MyGetZher (void **vvZher, int *nZher, int MaxZher, Handle hCPage)
{
	uint32_t err32, nTeor, nReal;
	Handle hBlockZher;
	Handle hBlockPrev;
	int i;
	nTeor = sizeof (void *);
	i=0;
	while (1)
	{
		if (i==0)
			hBlockZher = CPAGE_GetBlockFirst (hCPage, RVERLINE_ZHERTVY_LINIY);
		else
			hBlockZher = CPAGE_GetBlockNext (hCPage, hBlockPrev, RVERLINE_ZHERTVY_LINIY);
		err32 = CPAGE_GetReturnCode ();
		if (err32!=0)
		{
			if (i==0)
				Error_CPage ("[GetBlockFirst]");
			else
				Error_CPage ("[GetBlockNext]");
			return FALSE;
		}
		if (!hBlockZher)
			break;
		if (i>=MaxZher)
			return FALSE;
		nReal = CPAGE_GetBlockData (hCPage, hBlockZher, RVERLINE_ZHERTVY_LINIY, (void *)&(vvZher[i]), nTeor);
		err32 = CPAGE_GetReturnCode ();
//		if ((nReal!=nTeor)||(err32!=0))
		if (!nReal||(err32!=0))
		{
			Error_CPage ("[GetBlockData]");
			return FALSE;
		}
		hBlockPrev = hBlockZher;
		i++;
	}
	*nZher = i;
	return TRUE;
}
/*----------------------------------------------------------------------------*/
Bool MyGetRaster (Handle hCPage, VL_I_TASK *pTask, uchar **ppData)
{
    PAGEINFO info = {0};
	CIMAGEInfoDataInGet DataInto = {0};
	CIMAGEInfoDataOutGet DataOut = {0};
	uchar Name[256];
	Bool ret;
	int i;
	/*  1. Подготовка к запросу части изображения.  */
	DataInto.dwHeight   = (uint32_t)pTask->MyHeight;
	DataInto.dwWidth    = pTask->MyExtrWidth;
	DataInto.wByteWidth = (int16_t)((pTask->MyExtrWidth+7)/8);//06.09.00
	DataInto.dwX        = pTask->MyLeft;
	DataInto.dwY        = pTask->MyTop;
	DataInto.MaskFlag   = 0x00;
    GetPageInfo(hCPage,&info);
	for (i=0; i<256; i++)
		Name[i] = info.szImageName[i];
	DataOut.dwWidth    = DataInto.dwWidth;
	DataOut.dwHeight   = DataInto.dwHeight;
	DataOut.wByteWidth = DataInto.wByteWidth;
	DataOut.byBit      = (uint16_t)info.BitPerPixel;
	DataOut.lpData     = *ppData;
	/*  5. Чтение части изображения.  */
	ret = CIMAGE_GetData (Name, &DataInto, &DataOut);
	if (!ret)
		return FALSE;
	if (DataOut.lpData==NULL)
		return FALSE;
	*ppData = DataOut.lpData;
	return TRUE;
}
/*----------------------------------------------------------------------------*/
