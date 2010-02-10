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

#include <math.h>
#include <time.h>
#include <cstdio>
#include <cstring>

#include "rselstr.h"
#include "roots.h"
#include "layout.h"
#include "recdefs.h"
#include "ccom/ccom.h"
#include "ccom/ccomdefs.h"
#include "cimage/ctiimage.h"
#include "cpage/cpage.h"
#include "dpuma.h"
#include "polyblock.h"
#include "rcutp/rcutp.h"
#include "minmax.h"

using namespace CIF;

#define RGB(r,g,b)          ((COLORREF)(((uchar)(r)|((uint16_t)((uchar)(g))<<8))|(((uint32_t)(uchar)(b))<<16)))
typedef uint32_t COLORREF;

#define TYPE_NormalZone		CPAGE_GetInternalType("TYPE_NormalZone")
#define TYPE_NormalRazrezZone		CPAGE_GetInternalType("TYPE_NormalRazrezZone")

# define ROOTS_QUANTUM			1024
#define k_cut 1.8
int count_cut_let;
Rect16 oldRoot;

extern int inf_let_h;
int inf_dust_h = 4;
int inf_let_w = 5;
int sup_dust_w = 5;
int min_cut_down_let_w = 3;
int sup_prob_w = 20;
Bool type_let;
extern Handle HCPAGE;
extern Handle hDebugCutStr;
extern Handle CutStrD;
extern Handle MainWindowD;
extern Handle DifferenCutToHistory;
extern Handle hTime;
int medium_statistics_h;
extern int medium_h;
uint32_t code_str_cut_d = 117;
Bool32 WasCut = FALSE;
Bool32 WasDif = FALSE;
uint32_t Code_UB_Kill = 0;
uint32_t Code_UB_Create = 0;

int CutStrings(POLY_* pBLOCK);
int GetStatisticsH(void);
void IfDifCut(void);

extern Handle HCCOM;
extern FILE* f_cut_str;
extern FILE* f_temp_cut;
extern FILE* f_old_cut;

int GetMediumHeight(POLY_*);
Bool GetMasP(Handle hCPage, Rect16 Rc, uchar** ppmasp);
Bool Increase2(RecRaster* rast, CCOM_comp* comp);
int GetCountNumbers(int num);
void StrDrawRect(Handle wnd, uint32_t OperCode, uint32_t color, int top,
		int bottom, int left, int right);
Bool IfEqv(char* buf1, char* buf2);
Bool IfEqv(Rect16 r1, Rect16 r2);
Bool AddLenBlockMas(POLY_** ppRc, int& len, int add);
void DelBlockMas(POLY_* masp);
Bool InitBlockMas(POLY_** ppRc, int len);
int IsInPoly(const CIF::Point16& a, POLY_* pPoly);
Bool CutComp(Handle hCPAGE, CCOM_handle hCCOM, CCOM_comp* comp, int bound,
		Bool fl_cut);
void UndoCutInRect(Handle hCPAGE, CCOM_handle hCCOM, Rect32* Rc);

void RSELSTR_CutCompInTableZones(Handle hCPAGE, CCOM_handle hCCOM) {
}


int IsInPoly(const CIF::Point16& a, POLY_* pPoly)
{
    int y, ind;
    int Count = 0;
    POLY_ *p = pPoly;
    int n = p->com.count;

    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;

        if (p->com.Vertex[i].y() == p->com.Vertex[j].y())
            continue;

        if (p->com.Vertex[i].y() > a.y() && p->com.Vertex[j].y() > a.y())
            continue;

        if (p->com.Vertex[i].y() < a.y() && p->com.Vertex[j].y() < a.y())
            continue;

        y = p->com.Vertex[i].y();
        ind = i;

        if (p->com.Vertex[j].y() > y) {
            y = p->com.Vertex[j].y();
            ind = j;
        }

        if ((y == a.y()) && (p->com.Vertex[ind].x() >= a.x()))
            Count++;

        else if (MIN(p->com.Vertex[i].y(), p->com.Vertex[j].y()) == a.y())
            continue;

        else {
            double t = ((double) (a.y() - p->com.Vertex[i].y()) / ((double) (p->com.Vertex[j].y()
                    - (double) p->com.Vertex[i].y())));

            if (t > 0 && t < 1 && (double) p->com.Vertex[i].x() + t
                    * ((double) p->com.Vertex[j].x() - (double) p->com.Vertex[i].x())
                    >= (double) a.x())
                Count++;
        }
    }

    return Count & 1;
}

void UndoCutInRect(Handle hCPAGE, CCOM_handle hCCOM, Rect32* Rc) {
	CCOM_comp * comp;
	int16_t top = (int16_t) Rc->top;
	int16_t bottom = (int16_t) Rc->bottom;
	int16_t left = (int16_t) Rc->left;
	int16_t right = (int16_t) Rc->right;
	CCOM_USER_BLOCK ub;

	for (comp = CCOM_GetFirst(hCCOM, NULL); comp; comp = CCOM_GetNext(comp,
			NULL)) {
		int size = sizeof(uint32_t);

		if (comp->upper >= top && comp->left >= left && comp->upper + comp->h
				<= bottom && comp->left + comp->w <= right) {
			if (comp->large & CCOM_LR_KILLED) {
				ub.code = Code_UB_Kill;
				if (CCOM_GetUserBlock(comp, &ub)) {
					if (ub.size == size) {
						if (*((uint32_t*) (ub.data)) == Code_UB_Kill)
							CCOM_Reanimate(comp);
					}
				}
			} else {
				ub.code = Code_UB_Create;
				if (CCOM_GetUserBlock(comp, &ub)) {
					if (ub.size == size) {
						if (*((uint32_t*) (ub.data)) == Code_UB_Create)
							CCOM_Kill(comp);
					}
				}
			}
		}
	}
}

int CutStrings(POLY_* pBlock) {
	medium_h = GetMediumHeight(pBlock);

	int cut_h = (int) ((double) (medium_h) * k_cut);

	CCOM_comp * comp;
	CCOM_comp* prev_comp = NULL;

	PAGEINFO info = { 0 };
	GetPageInfo(HCPAGE, &info);
	int nIncline = info.Incline2048;

	Point16 pLeftTop;
	Point16 pRightTop;
	Point16 pLeftBottom;
	Point16 pRightBottom;

	Bool fl_cont = FALSE;

	comp = CCOM_GetFirst((CCOM_handle) HCCOM, NULL);
	prev_comp = comp;

	//Andrey: устанавливаем переменные для метода определения точек разрезания, перемещенного в RCUTP
	if (!RCUTP_RSelStr_SetEnv(/*(char*)info.szImageName, */medium_h, inf_let_w,
			inf_dust_h, inf_let_h, HCCOM, sup_dust_w, min_cut_down_let_w,
			sup_prob_w, info.BitPerPixel))
		return 0;

	while (comp) {
		fl_cont = FALSE;
		if (comp->h < inf_let_h) {
			prev_comp = comp;
			comp = CCOM_GetNext(comp, NULL);
			fl_cont = TRUE;
		}
		if (fl_cont)
			continue;

		pLeftTop.rx() =  comp->left;
		pLeftTop.ry() = comp->upper - 1;
		pRightTop.rx() =  comp->left + comp->w - 1;
		pRightTop.ry() = comp->upper - 1;
		pLeftBottom.rx() =  comp->left;
		pLeftBottom.ry() = comp->upper + comp->h - 2;
		pRightBottom.rx() =  comp->left + comp->w - 1;
		pRightBottom.ry() = comp->upper + comp->h - 2;

		IDEAL_XY(pLeftTop.rx(), pLeftTop.ry());
		IDEAL_XY(pRightTop.rx(), pRightTop.ry());
		IDEAL_XY(pLeftBottom.rx(), pLeftBottom.ry());
		IDEAL_XY(pRightBottom.rx(), pRightBottom.ry());

		if (IsInPoly(pLeftTop, pBlock) || IsInPoly(pRightTop, pBlock)
				|| IsInPoly(pLeftBottom, pBlock) || IsInPoly(pRightBottom,
				pBlock)) {
			if (comp->h >= cut_h && comp->h <= medium_h * 5 && comp->w
					>= inf_let_w - 1) {
				uchar Data[1000];
				memset(Data, 0, sizeof(Data));
				uchar* pmasp = Data;
				Rect16 Rc;

				Rc.top = comp->upper;
				Rc.bottom = comp->upper + comp->h - 1;
				Rc.left = comp->left;
				Rc.right = comp->left + comp->w - 1;

				int bound = GetMasP(HCPAGE, Rc, &pmasp) ? RCUTP_RSelStr_CP(
						comp, &type_let, pmasp) : comp->h / medium_h;

				if (bound > 0) {
					if (CutComp(HCPAGE, (CCOM_handle) HCCOM, comp, bound, 1)) {
						comp = prev_comp;
						fl_cont = TRUE;
					}
				}
				if (!fl_cont)
					comp = CCOM_GetNext(comp, NULL);
			} else
				comp = CCOM_GetNext(comp, NULL);
		} else
			comp = CCOM_GetNext(comp, NULL);
	}

	//Andrey: снятие действия установки переменных в RCUTP
	RCUTP_RSelStr_UnsetEnv();

	return 1;
}

int GetMediumHeight(POLY_* pBlock) {
	CCOM_comp * comp;
	int sum_height = 0;
	int count = 0;
	Point16 pLeftTop;
	Point16 pRightTop;
	Point16 pLeftBottom;
	Point16 pRightBottom;

	PAGEINFO info = { 0 };
	GetPageInfo(HCPAGE, &info);
	int nIncline = info.Incline2048;

	for (comp = CCOM_GetFirst((CCOM_handle) HCCOM, NULL); comp; comp
			= CCOM_GetNext(comp, NULL)) {
		if (comp->h < inf_let_h)
			continue;

		pLeftTop.rx() =  comp->left;
		pLeftTop.ry() = comp->upper - 1;
		pRightTop.rx() =  comp->left + comp->w - 1;
		pRightTop.ry() = comp->upper - 1;
		pLeftBottom.rx() =  comp->left;
		pLeftBottom.ry() = comp->upper + comp->h - 2;
		pRightBottom.rx() =  comp->left + comp->w - 1;
		pRightBottom.ry() = comp->upper + comp->h - 2;

		IDEAL_XY(pLeftTop.rx(), pLeftTop.ry());
		IDEAL_XY(pRightTop.rx(), pRightTop.ry());
		IDEAL_XY(pLeftBottom.rx(), pLeftBottom.ry());
		IDEAL_XY(pRightBottom.rx(), pRightBottom.ry());

		if (IsInPoly(pLeftTop, pBlock) || IsInPoly(pRightTop, pBlock)
				|| IsInPoly(pLeftBottom, pBlock) || IsInPoly(pRightBottom,
				pBlock)) {
			sum_height += comp->h;
			count++;
		}
	}

	if (!count)
		return medium_statistics_h;

	return sum_height / count;
}

int GetMediumHeight(ROOT* pRoots) {
	ROOT* pRoot;
	int sum_height = 0;
	int count = 0;

	for (pRoot = pRoots; pRoot; pRoot = pRoot->u1.pNext) {
		if (IS_LAYOUT_DUST(*pRoot))
			continue;

		if ((pRoot->bType & ROOT_LETTER) && pRoot->nHeight > inf_let_h) {
			sum_height += pRoot->nHeight;
			count++;
		}
	}

	if (!count)
		return 0;
	return sum_height / count;
}

int GetStatisticsH() {
	int sum = 0;
	int count = 0;
	CCOM_comp * comp;

	comp = CCOM_GetFirst((CCOM_handle) HCCOM, NULL);

	while (comp) {
		if (comp->h > inf_let_h) {
			count++;
			sum += comp->h;
		}
		comp = CCOM_GetNext(comp, NULL);
	}

	if (!count)
		return 15;

	return sum / count;
}

Bool GetMasP(Handle hCPage, Rect16 Rc, uchar** ppmasp) {
	int prewide;
	int left = Rc.left;
	int h = Rc.bottom - Rc.top + 1;
	int w = Rc.right - Rc.left + 1;
	int upper = Rc.top - 1;
	if ((w % 8) == 0)
		prewide = w;
	else {
		prewide = w / 8 + 1;
		prewide *= 8;
	}

	PAGEINFO info = { 0 };
	CIMAGE_InfoDataInGet DataInto = { 0 };
	CIMAGE_InfoDataOutGet DataOut = { 0 };

	char Name[CPAGE_MAXNAME];
	Bool ret;
	int i;

	//  1. Подготовка к запросу части изображения.
	DataInto.dwHeight = (uint32_t)(h);
	DataInto.dwWidth = (uint32_t)(prewide);
	DataInto.wByteWidth = (uint16_t)(prewide / 8);
	DataInto.dwX = left;
	DataInto.dwY = upper;
	DataInto.MaskFlag = 0x00;
	GetPageInfo(hCPage, &info);
	for (i = 0; i < CPAGE_MAXNAME; i++)
		Name[i] = info.szImageName[i];
	DataOut.dwWidth = DataInto.dwWidth;
	DataOut.dwHeight = DataInto.dwHeight;
	DataOut.wByteWidth = DataInto.wByteWidth;
	DataOut.byBit = (uint16_t) info.BitPerPixel;
	DataOut.lpData = *ppmasp;

	//	  5. Чтение части изображения.
	ret = CIMAGE_GetData(Name, &DataInto, &DataOut);
	if (!ret)
		return FALSE;
	if (DataOut.lpData == NULL)
		return FALSE;
	*ppmasp = DataOut.lpData;
	return TRUE;
}

Bool Increase2(RecRaster* rast, CCOM_comp* comp) {
	int newbytewide = ((rast->lnPixWidth) * 2 + 7) / 8;
	int bytewide = (rast->lnPixWidth + 7) / 8;
	int newh = rast->lnPixHeight * 2;
	if (newbytewide * newh > REC_MAX_RASTER_SIZE)
		return FALSE;
	if (comp->h * ((comp->w + 7) / 8) > REC_MAX_RASTER_SIZE)
		return FALSE;

	uchar bytep;
	uchar bytep2;
	int nowbyte = 0;
	int nowbyte2 = 0;
	int k;
	int j;
	int count = (rast->lnPixWidth + 63) / 64;
	int new_count = (newbytewide + 7) / 8;

	uchar* str = new uchar[REC_MAX_RASTER_SIZE];
	if (!str)
		return FALSE;

	for (k = REC_MAX_RASTER_SIZE - 1; k >= 0; k--)
		str[k] = 0;

	int i;
	for (i = rast->lnPixHeight - 1; i >= 0; i--) {
		nowbyte = i * count * 8;
		nowbyte2 = i * new_count * 8;
		for (int j = new_count; j > 0; j--) {
			bytep = 0;
			bytep2 = rast->Raster[nowbyte];
			bytep |= 128 & bytep2;
			bytep |= 64 & (bytep2 >> 1);
			bytep |= 32 & (bytep2 >> 1);
			bytep |= 16 & (bytep2 >> 2);
			bytep |= 8 & (bytep2 >> 2);
			bytep |= 4 & (bytep2 >> 3);
			bytep |= 2 & (bytep2 >> 3);
			bytep |= 1 & (bytep2 >> 4);
			str[nowbyte2] = bytep;
			nowbyte2++;
			bytep = 0;
			bytep |= 128 & (bytep2 << 4);
			bytep |= 64 & (bytep2 << 3);
			bytep |= 32 & (bytep2 << 3);
			bytep |= 16 & (bytep2 << 2);
			bytep |= 8 & (bytep2 << 2);
			bytep |= 4 & (bytep2 << 1);
			bytep |= 2 & (bytep2 << 1);
			bytep |= 1 & bytep2;
			str[nowbyte2] = bytep;
			nowbyte2++;
			nowbyte++;

			bytep = 0;
			bytep2 = rast->Raster[nowbyte];
			bytep |= 128 & bytep2;
			bytep |= 64 & (bytep2 >> 1);
			bytep |= 32 & (bytep2 >> 1);
			bytep |= 16 & (bytep2 >> 2);
			bytep |= 8 & (bytep2 >> 2);
			bytep |= 4 & (bytep2 >> 3);
			bytep |= 2 & (bytep2 >> 3);
			bytep |= 1 & (bytep2 >> 4);
			str[nowbyte2] = bytep;
			nowbyte2++;
			bytep = 0;
			bytep |= 128 & (bytep2 << 4);
			bytep |= 64 & (bytep2 << 3);
			bytep |= 32 & (bytep2 << 3);
			bytep |= 16 & (bytep2 << 2);
			bytep |= 8 & (bytep2 << 2);
			bytep |= 4 & (bytep2 << 1);
			bytep |= 2 & (bytep2 << 1);
			bytep |= 1 & bytep2;
			str[nowbyte2] = bytep;
			nowbyte2++;
			nowbyte++;

			bytep = 0;
			bytep2 = rast->Raster[nowbyte];
			bytep |= 128 & bytep2;
			bytep |= 64 & (bytep2 >> 1);
			bytep |= 32 & (bytep2 >> 1);
			bytep |= 16 & (bytep2 >> 2);
			bytep |= 8 & (bytep2 >> 2);
			bytep |= 4 & (bytep2 >> 3);
			bytep |= 2 & (bytep2 >> 3);
			bytep |= 1 & (bytep2 >> 4);
			str[nowbyte2] = bytep;
			nowbyte2++;
			bytep = 0;
			bytep |= 128 & (bytep2 << 4);
			bytep |= 64 & (bytep2 << 3);
			bytep |= 32 & (bytep2 << 3);
			bytep |= 16 & (bytep2 << 2);
			bytep |= 8 & (bytep2 << 2);
			bytep |= 4 & (bytep2 << 1);
			bytep |= 2 & (bytep2 << 1);
			bytep |= 1 & bytep2;
			str[nowbyte2] = bytep;
			nowbyte2++;
			nowbyte++;

			bytep = 0;
			bytep2 = rast->Raster[nowbyte];
			bytep |= 128 & bytep2;
			bytep |= 64 & (bytep2 >> 1);
			bytep |= 32 & (bytep2 >> 1);
			bytep |= 16 & (bytep2 >> 2);
			bytep |= 8 & (bytep2 >> 2);
			bytep |= 4 & (bytep2 >> 3);
			bytep |= 2 & (bytep2 >> 3);
			bytep |= 1 & (bytep2 >> 4);
			str[nowbyte2] = bytep;
			nowbyte2++;
			bytep = 0;
			bytep |= 128 & (bytep2 << 4);
			bytep |= 64 & (bytep2 << 3);
			bytep |= 32 & (bytep2 << 3);
			bytep |= 16 & (bytep2 << 2);
			bytep |= 8 & (bytep2 << 2);
			bytep |= 4 & (bytep2 << 1);
			bytep |= 2 & (bytep2 << 1);
			bytep |= 1 & bytep2;
			str[nowbyte2] = bytep;
			nowbyte2++;
			nowbyte++;
		}
	}

	uchar* raster = rast->Raster;
	for (i = REC_MAX_RASTER_SIZE - 1; i >= 0; i--)
		raster[i] = str[i];

	for (i = newh - 1; i >= 0; i = i - 2) {
		nowbyte = i * 8* new_count ;
		nowbyte2 = (i - 1) * 8* new_count ;
		k = (i / 2) * 8* new_count ;
		for (j = new_count; j > 0; j--) {
			rast->Raster[nowbyte] = rast->Raster[nowbyte2] = rast->Raster[k];
			k++;
			nowbyte++;
			nowbyte2++;
			rast->Raster[nowbyte] = rast->Raster[nowbyte2] = rast->Raster[k];
			k++;
			nowbyte++;
			nowbyte2++;
			rast->Raster[nowbyte] = rast->Raster[nowbyte2] = rast->Raster[k];
			k++;
			nowbyte++;
			nowbyte2++;
			rast->Raster[nowbyte] = rast->Raster[nowbyte2] = rast->Raster[k];
			k++;
			nowbyte++;
			nowbyte2++;
			rast->Raster[nowbyte] = rast->Raster[nowbyte2] = rast->Raster[k];
			k++;
			nowbyte++;
			nowbyte2++;
			rast->Raster[nowbyte] = rast->Raster[nowbyte2] = rast->Raster[k];
			k++;
			nowbyte++;
			nowbyte2++;
			rast->Raster[nowbyte] = rast->Raster[nowbyte2] = rast->Raster[k];
			k++;
			nowbyte++;
			nowbyte2++;
			rast->Raster[nowbyte] = rast->Raster[nowbyte2] = rast->Raster[k];
			k++;
			nowbyte++;
			nowbyte2++;
		}
	}
	rast->lnPixHeight *= 2;
	rast->lnPixWidth *= 2;
	(comp->scale)--;
	if (!(comp->scale)) {
		rast->lnPixHeight = comp->h;
		rast->lnPixWidth = comp->w;
	}
	delete[] str;
	return TRUE;
}

int GetCountNumbers(int num) {
	int count = 0;
	if (num == 0)
		return 1;
	for (num; num > 0; num = num / 10)
		count++;
	return count;
}

void StrDrawRect(Handle wnd, uint32_t OperCode, uint32_t color, int top,
		int bottom, int left, int right) {

	Rect16 Rect;

	Rect.bottom = bottom;
	Rect.left = left;
	Rect.right = right + 1;
	Rect.top = top - 1;

	LDPUMA_DrawRect(wnd, &(Rect), 0, color, -100, OperCode);

}

void IfDifCut(void) {
	if (!f_old_cut || !f_temp_cut)
		return;

	char buf1[10000];
	char buf2[10000];
	fgets(buf1, 10000, f_temp_cut);
	Bool fl_exist = FALSE;

	while (fgets(buf2, 10000, f_old_cut)) {
		if (IfEqv(buf1, buf2))
			fl_exist = TRUE;
		if (fl_exist)
			break;
	}
	if (!fl_exist)
		return;

	Rect16 pOldCut[1000];
	int nOldCut = 0;
	Rect16 pNewCut[1000];
	int nNewCut = 0;
	Bool fl_empty = FALSE;

	if (!fgets(buf2, 10000, f_old_cut))
		fl_empty = TRUE;

	int i;
	for (i = 999; i >= 0; i--) {
		pNewCut[i].bottom = pOldCut[i].bottom = 0;
		pNewCut[i].top = pOldCut[i].top = 0;
		pNewCut[i].left = pOldCut[i].left = 0;
		pNewCut[i].right = pOldCut[i].right = 0;
	}

	while ((buf2[1] != ':') && (buf2[2] != '\\') && (!fl_empty)) {
		i = 4;
		while (buf2[i] != ' ') {
			pOldCut[nOldCut].top *= 10;
			if (buf2[i] != '0')
				pOldCut[nOldCut].top += buf2[i] - '1' + 1;
			i++;
		}
		i = 18;
		while (buf2[i] != ' ') {
			pOldCut[nOldCut].bottom *= 10;
			if (buf2[i] != '0')
				pOldCut[nOldCut].bottom += buf2[i] - '1' + 1;
			i++;
		}
		i = 30;
		while (buf2[i] != ' ') {
			pOldCut[nOldCut].left *= 10;
			if (buf2[i] != '0')
				pOldCut[nOldCut].left += buf2[i] - '1' + 1;
			i++;
		}
		i = 43;
		while (buf2[i] != ' ' && buf2[i] != '\n' && buf2[i] != '\0') {
			pOldCut[nOldCut].right *= 10;
			if (buf2[i] != '0')
				pOldCut[nOldCut].right += buf2[i] - '1' + 1;
			i++;
		}
		nOldCut++;

		if (!fgets(buf2, 10000, f_old_cut))
			break;
	}

	fl_empty = FALSE;
	if (!fgets(buf1, 10000, f_temp_cut))
		fl_empty = TRUE;

	while ((buf1[1] != ':') && (buf1[2] != '\\') && (!fl_empty)) {
		i = 4;
		while (buf1[i] != ' ') {
			pNewCut[nNewCut].top *= 10;
			if (buf1[i] != '0')
				pNewCut[nNewCut].top += buf1[i] - '1' + 1;
			i++;
		}
		i = 18;
		while (buf1[i] != ' ') {
			pNewCut[nNewCut].bottom *= 10;
			if (buf1[i] != '0')
				pNewCut[nNewCut].bottom += buf1[i] - '1' + 1;
			i++;
		}
		i = 30;
		while (buf1[i] != ' ') {
			pNewCut[nNewCut].left *= 10;
			if (buf1[i] != '0')
				pNewCut[nNewCut].left += buf1[i] - '1' + 1;
			i++;
		}
		i = 43;
		while (buf1[i] != ' ' && buf1[i] != '\n' && buf1[i] != '\0') {
			pNewCut[nNewCut].right *= 10;
			if (buf1[i] != '0')
				pNewCut[nNewCut].right += buf1[i] - '1' + 1;
			i++;
		}
		nNewCut++;

		if (!fgets(buf1, 10000, f_temp_cut))
			break;
	}

	if (!nOldCut && !nNewCut)
		return;

	int j;
	// старая резка:
	for (i = nOldCut - 1; i >= 0; i--) {
		for (j = nNewCut - 1; j >= 0; j--) {
			if (IfEqv(pOldCut[i], pNewCut[j]))
				break;
		}
		if (j < 0) {
			StrDrawRect(MainWindowD, code_str_cut_d, RGB(0,0,255),
					pOldCut[i].top, pOldCut[i].bottom, pOldCut[i].left,
					pOldCut[i].right);
			WasDif = TRUE;
		}
	}
	// новая резка:
	for (i = nNewCut - 1; i >= 0; i--) {
		for (j = nOldCut - 1; j >= 0; j--) {
			if (IfEqv(pNewCut[i], pOldCut[j]))
				break;
		}
		if (j < 0) {
			StrDrawRect(MainWindowD, code_str_cut_d, RGB(255,0,0),
					pNewCut[i].top, pNewCut[i].bottom, pNewCut[i].left,
					pNewCut[i].right);
			WasDif = TRUE;
		}
	}

}

Bool IfEqv(char* buf1, char* buf2) {
	int i = 0;
	while (buf1[i] != '\0' && buf2[i] != '\0' && buf1[i] == buf2[i])
		i++;
	if (buf1[i] == buf2[i])
		return TRUE;
	return FALSE;
}

Bool IfEqv(Rect16 r1, Rect16 r2) {
	if (r1.top != r2.top)
		return FALSE;
	if (r1.bottom != r2.bottom)
		return FALSE;
	if (r1.left != r2.left)
		return FALSE;
	if (r1.right != r2.right)
		return FALSE;
	return TRUE;
}

Bool AddLenBlockMas(POLY_** ppRc, int& len, int add) {
	POLY_ *dop;
	int i;
	if (!(InitBlockMas(&dop, len)))
		return FALSE;
	for (i = 0; i < len; i++) {
		for (int j = 0; j < (*ppRc)[i].com.count; j++) {
			dop[i].com.Vertex[j].rx() =  (*ppRc)[i].com.Vertex[i].x();
			dop[i].com.Vertex[j].ry() = (*ppRc)[i].com.Vertex[i].y();
		}
	}

	DelBlockMas(*ppRc);
	if (!(InitBlockMas(ppRc, len + add))) {
		(*ppRc) = dop;
		return FALSE;
	}

	for (i = 0; i < len; i++) {
		for (int j = 0; j < (*ppRc)[i].com.count; j++) {
			(*ppRc)[i].com.Vertex[i].rx() =  dop[i].com.Vertex[j].x();
			(*ppRc)[i].com.Vertex[i].ry() = dop[i].com.Vertex[j].y();
		}
	}

	len += add;
	DelBlockMas(dop);
	return TRUE;
}

void DelBlockMas(POLY_ *masp) {
	delete[] masp;
}

Bool InitBlockMas(POLY_** ppRc, int len) {
	(*ppRc) = NULL;
	if (!((*ppRc) = new POLY_[len]))
		return FALSE;
	return TRUE;
}
