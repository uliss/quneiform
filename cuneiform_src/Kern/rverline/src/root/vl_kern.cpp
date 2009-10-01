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
/*  Файл       :  'VL_Kern.CPP'                                               */
/*  Содержание :  Содержательные функции библиотеки "RVERLINE"                */
/*  Назначение :  Стандартный диалог с другими библиотеками                   */
/*----------------------------------------------------------------------------*/

#include <stdio.h>
/*#include <windows.h>*/
#include <string.h>
/*  interface our-other  */
#include "lnsdefs.h"
#include "cpage.h"
#include "pumadef.h"
#include "cline.h"
/*  interface our-my     */
#include "rverline.h"
/*  interface my        */
#include "am_err.h"
#include "am_comm.h"
#include "vl_rule.h"
#include "vl_snap.h"
/*  interface my-my      */
#include "am_buff.h"
#include "amt_geom.h"
/*----------------------------------------------------------------------------*/
#define       MaxComps       25000//12000
#define       MaxLines        2000
#define       MaxZherTotal    100
/*------------extern objects--------------------------------------------------*/
extern uint16_t      gwHeightRC_rver;
extern uint16_t      gwLowRC_rver;
//extern Handle      hUseCLine;
/*------------own objects-----------------------------------------------------*/
static Regime_VerifyLines   MainRegime;
static Rect16 Rc[MaxComps];
static int WhatDo[MaxComps];
/*------------extern functions------------------------------------------------*/
void   SetReturnCode_rverline (uint16_t rc);
uint16_t GetReturnCode_rverline ();
Bool WasInitRVERLINE ();
void GiveMemFor_FWP_Inst (int **ppWhatDo, int *nLimComp);
Bool MyGetLines (LinesTotalInfo *pLti, int MaxNumLin, Handle hCPage, uint32_t *pHoriType, uint32_t *pVertType, char *pStr);
Bool MyReSetLines (void *vLti, int MaxNumLin, Handle hCPage, uint32_t HoriType, uint32_t VertType);
Bool MyGetLines (LinesTotalInfo *pLti, int MaxNumLin, CLINE_handle hCLINE, char *pStr);
Bool MyReSetLines (void *vLti, int MaxNumLin, CLINE_handle hCLINE);
void MySetNegative (void *vB, Handle hCPage);
Bool MyGetComp (Handle hCCOM, Rect16 *pRc, int *nRC, int MyMaxC, int Filter);
void New_MarkVerifiedLines (void *vLti, Handle hCPage, Rect16 *pRc, int *pWhatDo
		, int nComp, int *nZher, int *iZher, int MaxZher, Bool AbleShortVert);
void AnyWarning (char *pStr);
Bool MyFormZhertvy (Handle hCCOM, void **vvZher, int *iZher, int nZher, int Filter);
void WriteResForLines (void *vLti);
Bool MySetZher (void **vvZher, int nZher, Handle hCPage);
Bool MyGetZher (void **vvZher, int *nZher, int MaxZher, Handle hCPage);
/*----------------------------------------------------------------------------*/
RVERLINE_FUNC(Bool32) RVERLINE_MarkLines (Handle hCComp, Handle hCPage)
{
	uchar  err8;
	uint16_t Code;
//	uint32_t HoriType, VertType;
	Bool ret, AbleShortVert;
	LinesTotalInfo  lti = {0};  // Структура хранения линий
	LineInfo LHor[MaxLines], LVer[MaxLines];
	Regime_VerifyLines   Regime;
	Handle myMainWindow;
	Handle myKeyWarn;
	char Str[256];
	char *file_name;
	Rect16 *pRc;
	int nRC, LimComp, rot;
	int *pWhatDo;
	int iZher[MaxZherTotal], nZher;
	void *Zhertvy[MaxZherTotal];
    PAGEINFO info = {0};
	CLINE_handle hCLINE=CLINE_GetMainContainer();
	ret = WasInitRVERLINE ();
	if (!ret)
		return FALSE;
	Regime = MainRegime;
	AbleShortVert = Regime&RVL_FutuTablCorr;
	/*  открыть файлы отписи результатов  */
	if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
		rot = AM_OpenRes_rv_fte (RU_VL_D_WrResLine, "C:\\Temp\\res_vl_");
	file_name = AM_GetFileName (NULL);
	sprintf (Str, "  <2 Н Страница = %s\n", file_name);
	if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
		rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, Str);
    GetPageInfo(hCPage, &info);
	if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_Info)))
		AM_ConsolN ("Res_X = %d  Res_Y = %d  W_page = %d  H_page = %d\n"
				, info.DPIX, info.DPIY, info.Width, info.Height);
	err8 = (uchar)ER_ROUGH_NORMAL;
	Code = (uint16_t)(err8<<8);
	err8 = (uchar)ER_DETAIL_EMPTY_FUNC;
	Code |= (uint16_t)err8;
	SetReturnCode_rverline (Code);
	/*  Считываю линии  */
	lti.Hor.Lns = LHor;
	lti.Ver.Lns = LVer;
//	if(!AM_Skip(hUseCLine))
		ret=MyGetLines(&lti,MaxLines,hCLINE,Str);
//	else
//	    ret = MyGetLines (&lti, MaxLines, hCPage, &HoriType, &VertType, Str);
	switch (ret)
	{
		case RV_TRUE :
			break;
		case RV_EMPTY :
			myKeyWarn = AM_GetKeyOfRule (RU_VL_C_ContWarn);
			if (!AM_Skip (myKeyWarn))
				AM_ConsolN ("Rlt-WarningExternal-Abort : \
				\n Функция верификации линий реально не исполнялась. \
				\n Причина - отсутствие линий. \n%s", Str);
			if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
				rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, "  <2 К Страница\n");
			return TRUE;
		case RV_DOUBT :
			err8 = (uchar)ER_ROUGH_CALL_REFUSED;
			Code = (uint16_t)(err8<<8);
			err8 = (uchar)ER_DETAIL_NO_MEMORY;
			Code |= (uint16_t)err8;
			SetReturnCode_rverline (Code);
			myKeyWarn  = AM_GetKeyOfRule (RU_VL_C_ContWarn);
			if (!AM_Skip (myKeyWarn))
				AM_ConsolN ("Rlt-WarningCommon-Abort : \
				\n Функция верификации линий реально не исполнялась. \
				\n Вероятны неверное выделение линий либо неверный вызов \
				   данной функции.\
				\n%s", Str);
			if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
				rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, "  <2 К Страница\n");
			return TRUE;
		case RV_FALSE :
		default :
			if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
				rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, "  <2 К Страница\n");
			return FALSE;
	}
	/*  Считываю компоненты  */
	pWhatDo = WhatDo;
	pRc = Rc;
	LimComp = MaxComps;
	ret = MyGetComp (hCComp, pRc, &nRC, LimComp, 2);
	if (!ret)
	{
		if (!AM_Skip (AM_GetKeyOfRule (RU_VL_C_ContWarn)))
			AM_ConsolN ("Rlt-Restart : Нет, попробуем продолжить! \
				\n Ужесточен отбор коробок компонент для верификации линий!");
		ret = MyGetComp (hCComp, pRc, &nRC, LimComp, 3);
	}
	if (!ret)
	{
		if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
			rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, "  <2 К Страница\n");
		return TRUE;
	}
	// Получение дескриптора моего окна
	myMainWindow = AM_GetWindowHandle (PUMA_IMAGE_TURN);
	if (myMainWindow==NULL)
		myMainWindow = AM_CreateWindow (NULL, NULL);
	/*  Отрисовка входных данных  */
	RLTDraw_O_VerifLines ((void *)(&lti), RU_VL_I_PrimLines, RU_Unknown, myMainWindow);
	RLTDraw_I_PrimBoxes (pRc, NULL, nRC, myMainWindow, RU_VL_I_PrimBoxes);
	/*  Содержательная часть  */
	if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
		rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, "  <3 Н Линии\n");
	New_MarkVerifiedLines ((void *)(&lti), hCPage, pRc, pWhatDo, nRC, &nZher, iZher, MaxZherTotal, AbleShortVert);
	ret = MyFormZhertvy (hCComp, Zhertvy, iZher, nZher, 2);
	if (!ret)
	{
		if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
			rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, "  <3 К Линии\n");
		if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
			rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, "  <2 К Страница\n");
		return FALSE;
	}
	ret = MySetZher (Zhertvy, nZher, hCPage);
	if (!ret)
	{
		if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
			rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, "  <3 К Линии\n");
		if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
			rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, "  <2 К Страница\n");
		return FALSE;
	}
	/*  Проверка
	ret = MyGetZher (Zhertvy, &nZher, MaxZherTotal, hCPage);
	if (!ret)
		return FALSE;  */
	/*  Отписывание результатов работы  */
	WriteResForLines ((void *)(&lti));
	/*  Отрисовка результатов работы  */
  	RLTDraw_O_VerifLines ((void *)(&lti), RU_VL_O_VerifLines, RU_VL_O_CorrLines, myMainWindow);
	if (nZher>0)
		RLTDraw_VL_R_Zhertvy (Zhertvy, nZher, myMainWindow, RU_VL_O_SaveComps);
	/*  Перезаписываю линии с проставленными флагами их верификации  */
//	if(!AM_Skip(hUseCLine))
		ret=MyReSetLines(&lti,MaxLines,hCLINE);
//	else
//	    ret = MyReSetLines ((void *)(&lti), MaxLines, hCPage, HoriType, VertType);
	if (!ret)
	{
		if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
			rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, "  <3 К Линии\n");
		if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
			rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, "  <2 К Страница\n");
		return FALSE;
	}
	if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
		rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, "  <3 К Линии\n");
	if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
		rot = AM_WriteRes_rv_fte (RU_VL_D_WrResLine, "  <2 К Страница\n");
	if (!AM_Skip (AM_GetKeyOfRule (RU_VL_D_WrResLine)))
		rot = AM_SaveRes_rv_fte  (RU_VL_D_WrResLine, "C:\\Temp\\res_vl_");
	return TRUE;
}
/*----------------------------------------------------------------------------*/
RVERLINE_FUNC(Bool32) RVERLINE_SetImportData(uint32_t dwType, void *pData)
{
	uchar err8;
	if (gwHeightRC_rver==0)
	{
		err8 = (uchar)ER_ROUGH_CALL_REFUSED;
		gwLowRC_rver = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_WAS_NOT_INIT;
		gwLowRC_rver |= (uint16_t)err8;
		return FALSE;
	}
	switch (dwType)
	{
		case RVERLINE_DTRVERLINE_RegimeOfVerifyLines :
			if (pData!=NULL)
				memcpy (&MainRegime, pData, sizeof (Regime_VerifyLines));
			else
				memset (&MainRegime, 0, sizeof (Regime_VerifyLines));
			break;
		default :
			err8 = (uchar)ER_ROUGH_CALL_REFUSED;
			gwLowRC_rver = (uint16_t)(err8<<8);
			err8 = (uchar)ER_DETAIL_BAD_PARAMETRS;
			gwLowRC_rver |= (uint16_t)err8;
		return FALSE;
	}
	return TRUE;
}
/*----------------------------------------------------------------------------*/
