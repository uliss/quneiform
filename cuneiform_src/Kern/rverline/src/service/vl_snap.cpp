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
/*  Last Edit  :  17.11.99                                                   */
/*  Source     :  'AM_Snap.CPP'                                             */
/*  Содержание :  Изобразительные функции.                                   */
/*  Назначение :  Визуализация результатов.                                  */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
/*#include <windows.h>*/
/*  interface our  */
#include "skew1024.h"
/*  interface our-other  */
#include "lnsdefs.h"
# include "ccom.h"
#include "pumadef.h"
/*  interface our-my     */
#include "rverline.h"
/*  interface my        */
#include "am_comm.h"
#include "vl_rule.h"
#include "am_err.h"
#include "vl_snap.h"
/*  interface my-my      */
#include "am_buff.h"
#include "compat_defs.h"
/*------------extern functions-----------------------------------------------*/
void   SetReturnCode_rverline (uint16_t rc);
/*---------------------------------------------------------------------------*/
void My_WaitUserInput (Handle myKey, Handle myWindow)
{
	uint32_t Key;
//	if (AM_Skip (AM_GetKeyOfRule (RU_FT_D_StopUnderHL)))
		Key = AM_WaitUserInput (myKey, myWindow);
//	else
//		AM_MessageBoxOk ("Ok!");
}
/*---------------------------------------------------------------------------*/
void Draw_Lines_Verif (LinesTotalInfo *pLti, uint32_t OperCode, Handle myWindow
					   , Handle myKey)
{
	int i, n;
	uint32_t ColorLine;
	LineInfo *pLns;
	n = pLti->Hor.Cnt;
	pLns = pLti->Hor.Lns;
	for (i=0; i<n; i++)
	{
		ColorLine = RGB(255,255,  0); //желтый
		if (pLns->Flags & LI_Pointed)
		{
			ColorLine = RGB(  0,255,  0); //зеленый
			if (pLns->Flags & LI_Doubt)
				ColorLine = RGB(200,255, 0); //грязно-зеленый
		}
		if (pLns->Flags & LI_IsTrue)
			ColorLine = RGB(  0,  0,255); //синий
		if (pLns->Flags & LI_IsFalse)
			ColorLine = RGB(255,  0,  0); //красный
		if (pLns->Flags & LI_NOTWHOLE)
			ColorLine = RGB(255,  0,255); //фиолетовый
		if (pLns->Flags & LI_COMPLEX)
			ColorLine = RGB(200,100,100); //коричневый
		AM_DrawLine (myWindow, &(pLns->A), &(pLns->B), 0, ColorLine, -100*pLns->Thickness, OperCode);
/*		AM_ConsolN ("линия [%d] : Seg = %d Qua = %d Wid = %d Fla = %d Bib = %d "
			, i, pLns->SegCnt, pLns->Quality, pLns->Thickness
			, pLns->Flags, pLns->Extractor);
		My_WaitUserInput (myKey, myWindow);*/
		pLns++;
	}
	n = pLti->Ver.Cnt;
	pLns = pLti->Ver.Lns;
	for (i=0; i<n; i++)
	{
		ColorLine = RGB(255,255,  0); //желтый
		if (pLns->Flags & LI_IsTrue)
			ColorLine = RGB(  0,  0,255); //синий
		if (pLns->Flags & LI_IsFalse)
			ColorLine = RGB(255,  0,  0); //красный
		if (pLns->Flags & LI_NOTWHOLE)
			ColorLine = RGB(255,  0,255); //фиолетовый
		if (pLns->Flags & LI_COMPLEX)
			ColorLine = RGB(200,100,100); //коричневый
//			ColorLine = (128+RGB(255,255,  0))/2;//RGB(255,100,100); //коричневый
		AM_DrawLine (myWindow, &(pLns->A), &(pLns->B), 0, ColorLine, -100*pLns->Thickness, OperCode);
		pLns++;
	}
}
/*---------------------------------------------------------------------------*/
void Draw_Lines_Corrected (LinesTotalInfo *pLti, uint32_t OperCode, Handle myWindow)
{
	int i, n;
	uint32_t ColorLine;
	LineInfo *pLns;
	ColorLine = RGB(  0,255,  0); //зеленый
	n = pLti->Hor.Cnt;
	pLns = pLti->Hor.Lns;
	for (i=0; i<n; i++)
	{
		if ((pLns->Flags & LI_NOTWHOLE))
			AM_DrawLine (myWindow, &(pLns->Anew), &(pLns->Bnew), 0, ColorLine, -100*pLns->Thickness, OperCode);
		pLns++;
	}
	n = pLti->Ver.Cnt;
	pLns = pLti->Ver.Lns;
	for (i=0; i<n; i++)
	{
		if ((pLns->Flags & LI_NOTWHOLE))
			AM_DrawLine (myWindow, &(pLns->Anew), &(pLns->Bnew), 0, ColorLine, -100*pLns->Thickness, OperCode);
		pLns++;
	}
}
/*---------------------------------------------------------------------------*/
void RLTDraw_O_VerifLines (void *vLti, int Code_1, int Code_2, Handle myWindow)
{
	LinesTotalInfo *pLti;
	Bool16 NoSnap1, NoSnap2;
	uint32_t OperCode1, OperCode2;
	Handle myKey = NULL;
	Handle myKey1;
	Handle myKey2;
	pLti = (LinesTotalInfo *)vLti;
	/*******************************************/
	OperCode1 = AM_GetOperCode ((uint16_t)Code_1);
	myKey1 = AM_GetKeyOfRule ((VL_RULES)Code_1);
	NoSnap1 = AM_Skip (myKey1);
	if (!NoSnap1)
		Draw_Lines_Verif (pLti, OperCode1, myWindow, myKey1);
	/*******************************************/
	OperCode2 = AM_GetOperCode ((uint16_t)Code_2);
	myKey2 = AM_GetKeyOfRule ((VL_RULES)Code_2);
	NoSnap2 = AM_Skip (myKey2);
	if (!NoSnap2)
		Draw_Lines_Corrected (pLti, OperCode2, myWindow);
	/*******************************************/
	if (!NoSnap1)
	{
		myKey = myKey1;
	}
	else
	{
		if (!NoSnap2)
			myKey = myKey2;
	}
	if (myKey!=NULL)
		My_WaitUserInput (myKey, myWindow);
	/*******************************************/
	if (!NoSnap1)
		AM_DeleteLines (myWindow, OperCode1);
	if (!NoSnap2)
		AM_DeleteLines (myWindow, OperCode2);
}
/*---------------------------------------------------------------------------*/
void RLTDraw_VL_R_Zhertvy (void **vvZher, int nZher, Handle myWindow, int Code)
{
	int i;
	Bool16 NoSnap;
	uint32_t ColorBox, OperCode;
	Handle myKey;
	CCOM_comp *pZher;
	Rect16 Rc;
	myKey = AM_GetKeyOfRule ((VL_RULES)Code);
	NoSnap = AM_Skip (myKey);
	if (NoSnap)
		return;
	OperCode = AM_GetOperCode ((uint16_t)Code);
	ColorBox = RGB(0  ,255,  0); //зеленый
	for (i=0; i<nZher; i++)
	{
		pZher = (CCOM_comp *)vvZher[i];
		Rc.left   = pZher->left;
		Rc.right  = pZher->left + pZher->w - 1;
		Rc.top    = pZher->upper;
		Rc.bottom = pZher->upper + pZher->h - 1;
		AM_DrawRect (myWindow, &Rc, 0, ColorBox, 1, OperCode);
	}
	My_WaitUserInput (myKey, myWindow);
	AM_DeleteRects (myWindow, OperCode);
}
/*---------------------------------------------------------------------------*/
void RLTDraw_I_PrimBoxes (Rect16 *pRc, int *pNote, int nRc, Handle myWindow, int Code)
{
	int i;
	Bool16 NoSnap;
	uint32_t ColorBox, OperCode;
	Handle myKey;
	myKey = AM_GetKeyOfRule ((VL_RULES)Code);
	NoSnap = AM_Skip (myKey);
	if (NoSnap)
		return;
	OperCode = AM_GetOperCode ((uint16_t)Code);
	for (i=0; i<nRc; i++)
	{
		ColorBox = RGB(0  ,  0,255); //синий
		if ((pNote!=NULL)&&(pNote[i]!=1))
			ColorBox = RGB(255,  0,  0); //красный
		AM_DrawRect (myWindow, &(pRc[i]), 0, ColorBox, 1, OperCode);
	}
	My_WaitUserInput (myKey, myWindow);
	AM_DeleteRects (myWindow, OperCode);
}
/*---------------------------------------------------------------------------*/
void MyErrorNoMem (const char *pStr)
{
	Handle myKeyErr;
	uchar  err8;
	uint16_t Code;
	err8 = (uchar)ER_ROUGH_NORMAL;
	Code = (uint16_t)(err8<<8);
	err8 = (uchar)ER_DETAIL_NO_MEMORY;
	Code |= (uint16_t)err8;
	SetReturnCode_rverline (Code);
	myKeyErr  = AM_GetKeyOfRule (RU_VL_C_ContErr);
	if (!AM_Skip (myKeyErr))
		AM_ConsolN ("Rlt-Error-Stop : Не хватило памяти под %s! Аварийное завершение.", pStr);
}
/*---------------------------------------------------------------------------*/
void MyErrorNoComment (char *pStr)
{
	Handle myKeyErr;
	uchar  err8;
	uint16_t Code;
	err8 = (uchar)ER_ROUGH_NORMAL;
	Code = (uint16_t)(err8<<8);
	err8 = (uchar)ER_DETAIL_NO_COMMENT;
	Code |= (uint16_t)err8;
	SetReturnCode_rverline (Code);
	myKeyErr  = AM_GetKeyOfRule (RU_VL_C_ContErr);
	if (!AM_Skip (myKeyErr))
		AM_ConsolN ("Rlt-Error-Stop : %s Аварийное завершение.", pStr);
}
/*---------------------------------------------------------------------------*/
