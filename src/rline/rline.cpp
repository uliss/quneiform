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

// ============================================================================
//
// ============================================================================


#include <stdlib.h>
#include <memory.h>
#include <fstream>
#include <stdio.h>
#include <assert.h>

#include "resource.h"
#include "rline.h"
#include "cline.h"
#include "cimage/ctiimage.h"
#include "cpage/cpage.h"
#include "lns32/lns.h"
#include "dpuma.h"
#include "puma/pumadef.h"
#include "rshelllines/rshelllinescom.h"
#include "dsnap.h"

#include "compat_defs.h"

Handle hMainWindow;

Handle RLINE_Root;

Handle hRLINE_Pass1;
Handle hRLINE_Pass2;
Handle hRLINE_Pass3;

Handle hMainDebug;

Handle RLINE_Search;
Handle hRLINE_Correct;

Handle hRLINE_Search_Debug;
Handle hRLINE_Search_Output;
Handle hRLINE_Search_Output_ShowFrags;
Handle hRLINE_CorrectOutput;
Handle hRLINE_CorrectDebug;
Handle hRLINE_CorrectDebugFrag;
Handle hRLINE_CorrectControl;
Handle hRLINE_CorrectDrawLines;
Handle hRLINE_CorrectDrawFrags;
Handle hRLINE_Pass3_Control;
Handle hRLINE_Pass3Input;
Handle hRLINE_Pass3Output;
Handle hRLINE_Pass3DrawLines;
Handle hRLINE_Pass3DrawFrags;
Handle RLINE_NoFillgap;
Handle RLINE_NoHorBorder;
Handle RLINE_NoVerBorder;
Handle RLINE_ShowLines;
Handle RLINE_DeleteAll;
Handle RLINE_ShowWithoutLines;
Handle hDotKilledD;
Handle hWriteLineInFile;
Handle hWriteLineInFile3;
Handle hSkipDelSmallLines;
Handle hLinesPass2;
Handle hPrint1Cr;
Handle hPrint2Cr;
Handle hPrint3Cr;
Handle hPrint4Cr;
Handle hDrawCrRast;
Handle hRLINE_Pass2Output;
Handle hRLINE_Pass2DrawLines;
Handle hRLINE_Pass2DrawFrags;
Handle hLinesPass2Debug;
Handle hLinesPass2DebugSkipDouble;
Handle hLinesPass2DebugSkipGlue;
Handle hLinesPass2DebugSkipFindLost;
Handle hDebugPrintCrossCheck;

Handle hDebugDrowTableZone;
Handle hDebugDrowFriendLines;
Handle hDebugDrowGroupOfExtensibleLines;
Handle hDebugDrawLosedVerticalLines;
Handle hWriteLineInFile2;

Handle hDebugShortLinesPrint;
Handle hDebugShortLinesDrawRect;
Handle hShowCP;
Handle hShowCheckedRects;
Handle hPrintCheckedRects;
Handle hShowCPLines;

int32_t hMinInterval = 10;
int32_t h1Interval = 25;
int32_t h2Interval = 63;
int32_t hMaxInterval = 150;
int32_t hSmallLineLen = 20;
int32_t hLongLineLen = 50;
int32_t hMaxDegreeDiff = 5;
int32_t hMaxShift = 36;
int32_t hRastDelta = 1;
int32_t hRLINE_Pass2Lost1 = 75;
int32_t hRLINE_Pass2Lost2 = 95;
//Handle    hUseCLine;
void * hTmpDIB;
uint32_t HorType;
uint32_t VerType;

Handle Parts;
Handle Part2;
Handle Part3;
Handle Exit2;
Handle Exit3;
Handle MainWork2;
Handle MainWork3;
Handle Prep2;
Handle GetContainer3;
Handle PutContainer2;
Handle PutContainer3;
Handle Epilog2;

Bool32 gbNOFILLGAP3 = FALSE;
Bool32 gbNOHBORDER = FALSE;
Bool32 gbNOVBORDER = FALSE;

CIMAGEIMAGECALLBACK cbk;

Imxs* swp_imxs;
Imxs imxs;

uint16_t min_h_len = 40, min_v_len = 40;
uint16_t rc16;
uint32_t rc32;

//Almi 16.05.01
int MinHorLenForTrue = 129;//113;//было 100
int MinVerLenForTrue = 94;//58; //было  50

void SetReturnCode_rline(uint32_t);
void CleanLineData(void* pdata, int size);

Bool16 SampleImageOpen(CIMAGE_ImageInfo* lpImageInfo) {
	return swp_imxs->f_op((Imxs_ImageInfo*) lpImageInfo);
}
uint16_t SampleImageRead(pchar lpImage, uint16_t wMaxSize) {
	return swp_imxs->f_re((uchar*) lpImage, wMaxSize);
}
Bool16 SampleImageClose(void) {
	return swp_imxs->f_cl();
}

Bool16 DibOpen(Imxs_ImageInfo* lpImageInfo) {
	return cbk.CIMAGE_ImageOpen((CIMAGE_ImageInfo*) lpImageInfo);
}
int16_t DibRead(uchar* lpImage, uint16_t wMaxSize) {
	return cbk.CIMAGE_ImageRead((pchar) lpImage, wMaxSize);
}
Bool16 DibClose(void) {
	return cbk.CIMAGE_ImageClose();
}

Bool32 RLINE_SubInit(void) {
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////

Bool32 RLINE_SearchLines(void* lpInPage, void* phCLINE) {

	CLINE_handle* PHCLINE = (CLINE_handle*) phCLINE;
	CLINE_DeleteContainer(*PHCLINE);
	LinesTotalInfo lti; // Структура хранения линий
	PAGEINFO PInfo; // Описание страницы
	char* pImage; // Указатель на изображение
	LnsSetupStr ls;

	int32_t result_h_count, result_v_count;

	// Получаем PAGEINFO текущей страницы
	if (!CPAGE_GetPageData(lpInPage, PT_PAGEINFO, (void*) &PInfo, sizeof(PInfo))) {
		LDPUMA_Console(" Error in GetPageData ");
		rc32 = CPAGE_GetReturnCode();
		SetReturnCode_rline(rc32);
		return FALSE;
	}

	// Копируем в pImage из PInfo указатель на изображение, связанное со страницей
	pImage = PInfo.szImageName;

	min_h_len = (uint16_t) (PInfo.DPIX * 40 / 300);
	min_v_len = (uint16_t) (PInfo.DPIY * 40 / 300);

	if (!CIMAGE_GetCallbackImage(pImage, &cbk)) {
		LDPUMA_Console(" Error in GetCallbackImage ");
		SetReturnCode_rline(0);
		return FALSE;
	}

	imxs.f_op = DibOpen; // Мы не можем из за разных названий одного
	imxs.f_re = DibRead; // и того же типа в CIMAGE и LNS32 присвоить
	imxs.f_cl = DibClose; // imxs->... cbk->... нужно преобразование типов.


	// Подготавливаем страницу
	LnsPageStart(&imxs);

	// Установка параметров работы LNS32.dll
	ls.nOptions = 0;
	LnsSetup(&ls);

	if (!LDPUMA_Skip(RLINE_NoFillgap)) {
		gbNOFILLGAP3 = TRUE;
	} else {
		gbNOFILLGAP3 = FALSE;
	}
	ls.nOptions |= gbNOFILLGAP3 ? 0 : LSS_NOFILLGAP3_FILTER;

	if (!LDPUMA_Skip(RLINE_NoHorBorder)) {
		gbNOHBORDER = TRUE;
	} else {
		gbNOHBORDER = FALSE;
	}
	ls.nOptions |= gbNOHBORDER ? 0 : LSS_NOHBORDER_FILTER;

	if (!LDPUMA_Skip(RLINE_NoVerBorder)) {
		gbNOVBORDER = TRUE;
	} else {
		gbNOVBORDER = FALSE;
	}
	ls.nOptions |= gbNOVBORDER ? 0 : LSS_NOVBORDER_FILTER;

	LnsSetup(&ls);

	// Работаем...
	if (!LnsExtractLines(min_h_len, min_v_len, &result_h_count, &result_v_count)) {
		//rc16 = LnsGetError ();
		SetReturnCode_rline(rc16);
		CLINE_DeleteContainer(*PHCLINE);
		*PHCLINE = CLINE_CreateContainer(TRUE);
		return FALSE;
	}

	if (result_h_count) {
		lti.Hor.Lns = (LineInfo*) malloc(result_h_count * sizeof(LineInfo));
		if (lti.Hor.Lns == NULL) {
			SetReturnCode_rline(rc16);
			return FALSE;
		}
	} else
		lti.Hor.Lns = NULL;
	lti.Hor.Cnt = result_h_count;

	if (result_v_count) {
		lti.Ver.Lns = (LineInfo*) malloc(result_v_count * sizeof(LineInfo));
		if (lti.Ver.Lns == NULL) {
			SetReturnCode_rline(rc16);
			return FALSE;
		}
	} else
		lti.Ver.Lns = NULL;
	lti.Ver.Cnt = result_v_count;

	if (!LnsUpload(&lti, min_h_len, min_v_len)) {
		SetReturnCode_rline(rc16);
		return FALSE;
	}

	if (!LDPUMA_Skip(RLINE_ShowLines)) {
		Handle draw_window = LDPUMA_GetWindowHandle(NAME_IMAGE_ORTOMOVE);
		if (!draw_window)
			draw_window = LDPUMA_GetWindowHandle(PUMA_IMAGE_TURN);

		uint32_t x = 255 << 8;
		int32_t temp;
		int i;
		for (i = 0; i < lti.Hor.Cnt; i++) {
			temp = (-100) * lti.Hor.Lns[i].Thickness;
			assert(lti.Hor.Lns[i].Thickness > 0);
			LDPUMA_DrawLine(draw_window, &lti.Hor.Lns[i].A, &lti.Hor.Lns[i].B,
					0, x, (uint16_t) temp, 512);
		}
		for (i = 0; i < lti.Ver.Cnt; i++) {
			temp = (-100) * lti.Ver.Lns[i].Thickness;
			assert(lti.Ver.Lns[i].Thickness > 0);
			LDPUMA_DrawLine(draw_window, &lti.Ver.Lns[i].A, &lti.Ver.Lns[i].B,
					0, x, (uint16_t) temp, 512);
		}
		if (lti.Hor.Cnt || lti.Ver.Cnt) {
			LDPUMA_Console("Нажмите любую клавишу...\n");
			LDPUMA_WaitUserInput(RLINE_ShowLines, draw_window);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	int i;

	CLINE_handle hCLINE = CLINE_CreateContainer(TRUE);
	if (!hCLINE)
		return FALSE;
	CLINE_handle hline;
	DLine data;
	int size_line = sizeof(DLine);
	LineInfo* linfo;
	for (i = 0; i < lti.Hor.Cnt; i++) {
		hline = CLINE_AddNewLine(hCLINE);
		if (!hline) {
			CLINE_DeleteContainer(hCLINE);
			return FALSE;
		}
		linfo = &(lti.Hor.Lns[i]);
		CleanLineData(&data, size_line);
		data.ProcessingType = HorizantalLine;
		data.Line.Beg_X = linfo->A.x();
		data.Line.Beg_Y = linfo->A.y();
		data.Line.End_X = linfo->B.x();
		data.Line.End_Y = linfo->B.y();
		data.Line.Wid10 = linfo->Thickness * 10;
		data.Dens = (linfo->Quality * 100) / 255;
		data.Flags = linfo->Flags;
		data.n_event = linfo->SegCnt;
		data.Dir = LD_Horiz;

		if (!CLINE_SetLineData(hline, (CPDLine)(&data))) {
			CLINE_DeleteContainer(hCLINE);
			return FALSE;
		}

	}
	for (i = 0; i < lti.Ver.Cnt; i++) {
		hline = CLINE_AddNewLine(hCLINE);
		if (!hline) {
			CLINE_DeleteContainer(hCLINE);
			return FALSE;
		}
		linfo = &(lti.Ver.Lns[i]);
		CleanLineData(&data, size_line);

		data.Line.Beg_X = linfo->A.x();
		data.Line.Beg_Y = linfo->A.y();
		data.Line.End_X = linfo->B.x();
		data.Line.End_Y = linfo->B.y();
		data.Line.Wid10 = linfo->Thickness * 10;
		data.Dens = (linfo->Quality * 100) / 255;
		data.n_event = linfo->SegCnt;
		data.Flags = linfo->Flags;
		data.Dir = LD_Verti;
		data.ProcessingType = VerticalLine;

		if (!CLINE_SetLineData(hline, (CPDLine)(&data))) {
			CLINE_DeleteContainer(hCLINE);
			return FALSE;
		}
	}
	*PHCLINE = hCLINE;

	if (!LDPUMA_Skip(hRLINE_CorrectDebugFrag)) {
		ExtractAllEvents(hCLINE, &lti);

		if (!LDPUMA_Skip(hRLINE_Search_Output_ShowFrags))
			DrawFragsForAllLines(hCLINE, hRLINE_Search_Output_ShowFrags);
	}

	if (lti.Hor.Lns)
		free(lti.Hor.Lns);
	if (lti.Ver.Lns)
		free(lti.Ver.Lns);

	// Завершаем работу со страницей
	LnsPageFinish();

	if (!LDPUMA_Skip(RLINE_Search)) {
		LDPUMA_Console("Остановка перед завершением SerachLines");
		LDPUMA_WaitUserInput(RLINE_Search, NULL);
	}

	return TRUE;
}

Bool32 RLINE_DeleteLines(void* lpInPage, const char* lpOutDIB) {
	LinesTotalInfo lti2;
	PAGEINFO PInfo; // Описание страницы
	char* pImage; // Указатель на изображение
	LnsSetupStr ls;
	int i;

	int32_t result_h_count, result_v_count;
	Bool DelAllLines;
	DelAllLines = FALSE;
	CLINE_handle hCLINE = CLINE_GetMainContainer();

	// Получаем PAGEINFO текущей страницы
	if (!CPAGE_GetPageData(lpInPage, PT_PAGEINFO, (void*) &PInfo, sizeof(PInfo))) {
		LDPUMA_Console(" Error in GetPageData ");
		rc32 = CPAGE_GetReturnCode();
		SetReturnCode_rline(rc32);
		return FALSE;
	}

	// Копируем в pImage из PInfo указатель на изображение,
	// связанное со страницей
	pImage = (char*) &PInfo.szImageName;

	min_h_len = (uint16_t) (PInfo.DPIX * 40 / 300);
	min_v_len = (uint16_t) (PInfo.DPIY * 40 / 300);

	if (!CIMAGE_GetCallbackImage(pImage, &cbk)) {
		LDPUMA_Console(" Error in GetCallbackImage ");
		SetReturnCode_rline(0);
		return FALSE;
	}

	imxs.f_op = DibOpen; // Мы не можем из за разных названий одного
	imxs.f_re = DibRead; // и того же типа в CIMAGE и LNS32 присвоить
	imxs.f_cl = DibClose; // imxs->... cbk->... нужно преобразование типов.

	// Подготавливаем страницу
	LnsPageStart(&imxs);

	// Установка параметров работы LNS32.dll
	ls.nOptions = 0;
	LnsSetup(&ls);

	if (!LDPUMA_Skip(RLINE_NoFillgap)) {
		gbNOFILLGAP3 = TRUE;
	} else {
		gbNOFILLGAP3 = FALSE;
	}
	ls.nOptions |= gbNOFILLGAP3 ? 0 : LSS_NOFILLGAP3_FILTER;

	if (!LDPUMA_Skip(RLINE_NoHorBorder)) {
		gbNOHBORDER = TRUE;
	} else {
		gbNOHBORDER = FALSE;
	}
	ls.nOptions |= gbNOHBORDER ? 0 : LSS_NOHBORDER_FILTER;

	if (!LDPUMA_Skip(RLINE_NoVerBorder)) {
		gbNOVBORDER = TRUE;
	} else {
		gbNOVBORDER = FALSE;
	}
	ls.nOptions |= gbNOVBORDER ? 0 : LSS_NOVBORDER_FILTER;

	LnsSetup(&ls);

	//работаем...
	if (!LnsExtractLines(min_h_len, min_v_len, &result_h_count, &result_v_count)) {
		SetReturnCode_rline(rc16);
		return FALSE;
	}

	if (result_h_count) {
		lti2.Hor.Lns = (LineInfo*) malloc(result_h_count * sizeof(LineInfo));
		if (lti2.Hor.Lns == NULL) {
			SetReturnCode_rline(rc16);
			return FALSE;
		}
	} else
		lti2.Hor.Lns = NULL;
	lti2.Hor.Cnt = result_h_count;

	if (result_v_count) {
		lti2.Ver.Lns = (LineInfo*) malloc(result_v_count * sizeof(LineInfo));
		if (lti2.Ver.Lns == NULL) {
			SetReturnCode_rline(rc16);
			return FALSE;
		}
	} else
		lti2.Ver.Lns = NULL;
	lti2.Ver.Cnt = result_v_count;

	if (!LnsUpload(&lti2, min_h_len, min_v_len)) {
		SetReturnCode_rline(rc16);
		return FALSE;
	}

	CLINE_handle hline;
	if (DelAllLines) {
		for (i = lti2.Hor.Cnt - 1; i >= 0; i--)
			lti2.Hor.Lns[i].Flags = LI_SWEEP;
		for (i = lti2.Ver.Cnt - 1; i >= 0; i--)
			lti2.Ver.Lns[i].Flags = LI_SWEEP;
	} else {
		for (hline = CLINE_GetFirstLine(hCLINE); hline; hline
				= CLINE_GetNextLine(hline)) {
			CPDLine cpdata = CLINE_GetLineData(hline);
			if (!cpdata) {
				rc32 = CLINE_GetReturnCode();
				SetReturnCode_rline(rc32);
				return FALSE;
			}
			if (cpdata->Dir == LD_Horiz) {
				for (i = 0; i < lti2.Hor.Cnt; i++) {
					if ((lti2.Hor.Lns[i].A.x() == cpdata->Line.Beg_X)
							&& (lti2.Hor.Lns[i].B.x() == cpdata->Line.End_X)
							&& (lti2.Hor.Lns[i].A.y() == cpdata->Line.Beg_Y)
							&& (lti2.Hor.Lns[i].B.y() == cpdata->Line.End_Y)) {
						if (cpdata->Flags & LI_NOTWHOLE) {
							lti2.Hor.Lns[i].Anew = cpdata->lns_data.Anew;
							lti2.Hor.Lns[i].Bnew = cpdata->lns_data.Bnew;
						}
						lti2.Hor.Lns[i].Flags = cpdata->Flags;
						if (cpdata->Flags & LI_IsTrue)
							lti2.Hor.Lns[i].Flags |= LI_SWEEP;
						i = lti2.Hor.Cnt;
					}
				}
			} else {
				for (i = 0; i < lti2.Ver.Cnt; i++) {
					if ((lti2.Ver.Lns[i].A.x() == cpdata->Line.Beg_X)
							&& (lti2.Ver.Lns[i].B.x() == cpdata->Line.End_X)
							&& (lti2.Ver.Lns[i].A.y() == cpdata->Line.Beg_Y)
							&& (lti2.Ver.Lns[i].B.y() == cpdata->Line.End_Y)) {
						if (cpdata->Flags & LI_NOTWHOLE) {
							lti2.Ver.Lns[i].Anew = cpdata->lns_data.Anew;
							lti2.Ver.Lns[i].Bnew = cpdata->lns_data.Bnew;
						}
						lti2.Ver.Lns[i].Flags = cpdata->Flags;
						if (cpdata->Flags & LI_IsTrue)
							lti2.Ver.Lns[i].Flags |= LI_SWEEP;
						i = lti2.Ver.Cnt;
					}
				}
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////
	swp_imxs = LnsGetSweepedImage(&lti2);

	CIMAGEIMAGECALLBACK cbk1;
	cbk1.CIMAGE_ImageOpen = SampleImageOpen;
	cbk1.CIMAGE_ImageRead = SampleImageRead;
	cbk1.CIMAGE_ImageClose = SampleImageClose;

	if (!CIMAGE_GetCallbackImage(pImage, &cbk)) {
		SetReturnCode_rline(0);
		return FALSE;
	}

	if (!CIMAGE_WriteCallbackImage(lpOutDIB, cbk1)) {
		SetReturnCode_rline(0);
		return FALSE;
	}

	LnsPageFinish();

	if (!LDPUMA_Skip(RLINE_ShowWithoutLines)) {
		BITMAPINFOHEADER * lp = NULL;
		CIMAGE_ReadDIB(PUMA_IMAGE_DELLINE, (Handle*) &lp, TRUE);
	}

	if (lti2.Hor.Lns)
		free(lti2.Hor.Lns);
	if (lti2.Ver.Lns)
		free(lti2.Ver.Lns);

	return TRUE;
}

void CleanLineData(void* pdata, int size) {
	uchar* mas = (uchar*) pdata;
	for (int i = size - 1; i >= 0; i--) {
		mas[i] = 0;
	}
}
