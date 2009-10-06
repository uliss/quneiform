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

#include<stdlib.h>

#include "cpage.h"
#include "ccom/ccom.h"
#include "layout.h"
#include "recdefs.h"
#include "dpuma.h"
#include "ctiimage.h"
#include "exc.h"
#include "cpagetyps.h"
#include "polyblock.h"
//#include "rreccom.h"

#define RGB(r,g,b)          ((COLORREF)(((uchar)(r)|((uint16_t)((uchar)(g))<<8))|(((uint32_t)(uchar)(b))<<16)))
typedef uint32_t COLORREF;

int inf_cut_h = 15;
int sup_cut_h;
int inf_let_h_cut_comp = 10;
int sup_let_h = 30;
int medium_h;
extern int inf_let_w;
double coef_cut = .75;
Bool WasCutComp;
extern Bool32 WasCut;
Bool WasDifHis;
int code_comp_cut_d = 117;

extern Handle MainWindowD;
extern Handle CutCompPrintFILE;
extern Handle hCutComp;
extern Handle CutCompD;
extern Handle CutStrD;
extern Handle DifferenCutCompToHistory;
extern FILE* f_cut_comp;
extern FILE* f_old_cut_comp;
extern FILE* f_temp_cut_comp;
extern FILE* f_temp_cut;
extern FILE* f_cut_str;
extern FILE* f_old_cut;
extern int inf_let_h;
extern Bool type_let;
extern int inf_dust_h;
extern uint32_t Code_UB_Kill;
extern uint32_t Code_UB_Create;

Bool IfNeedCutComp(CCOM_comp* comp, Point32 beg, Point32 end, int& yCut);
int GetLineY(Point32 beg, Point32 end, int x);
Bool CutComp(Handle hCPAGE, CCOM_handle hCCOM, CCOM_comp* comp, int bound,
		Bool fl_cut);
void CleanRaster(RecRaster* rast, Rect16* rect, int scale);
Bool Increase2(RecRaster* rast, CCOM_comp* comp);
int GetCountNumbers(int num);
void DrawRect(Handle wnd, uint32_t OperCode, uint32_t color, int top, int bottom,
		int left, int right);
Bool IfWhiteRow(RecRaster* rast, int row);
int GetMediumH(CCOM_handle hCCOM);
void IfDifCutComp(void);
Bool IfEqv(char* buf1, char* buf2);
Bool IfEqv(Rect16 r1, Rect16 r2);
Handle GetStrCCOM(Handle hCPage, uchar* ImageName, Rect16 Rc, Bool neg,
		Bool vertical, RecRaster* rast, int min_h);
void MyKillComp(CCOM_comp* comp);
void MyCreateComp(CCOM_comp* comp);

Bool IfNeedCutComp(CCOM_comp* comp, Point32 beg, Point32 end, int& yCut) {
	int top = comp->upper;
	int left = comp->left;
	int right = left + comp->w - 1;
	int bottom = top + comp->h - 1;
	int w = comp->w;
	int h = comp->h;
	int medium_w = (right + left) / 2;
	int y;
	double coef;
	int max_y;
	int min_y;

	if (h < inf_cut_h || h > sup_cut_h || w < inf_let_w)
		return FALSE;

	if (beg.y >= end.y) {
		max_y = beg.y;
		min_y = end.y;
	} else {
		max_y = end.y;
		min_y = beg.y;
	}

	if ((bottom > min_y) && (top < max_y) && (medium_w >= beg.x) && (medium_w
			<= end.x))
		;
	else
		return FALSE;

	y = GetLineY(beg, end, medium_w);

	if (y >= bottom || y <= top)
		return FALSE;

	if (h > medium_h * 2) {
		if ((y - top) > inf_let_h_cut_comp && (bottom - y) > inf_let_h_cut_comp) {
			yCut = y;
			return TRUE;
		}
	}

	coef = (double) (y - top) / (double) (bottom - y);

	if ((coef <= 1. && coef >= coef_cut)
			|| (coef > 1. && coef <= 1. / coef_cut)) {
		yCut = y;
		return TRUE;
	} else
		return FALSE;

}

int GetLineY(Point32 beg, Point32 end, int x) {
	int lenght = end.x - beg.x + 1;
	int spusk = beg.y - end.y;

	if (lenght <= 0)
		return beg.y;
	return beg.y - ((x - beg.x + 1) * spusk) / lenght;
}

Bool CutComp(Handle hCPAGE, CCOM_handle hCCOM, CCOM_comp* comp, int bound,
		Bool fl_cut) {
	RecRaster rast;
	int ii;

	if (!CCOM_GetRaster(comp, &rast))
		return FALSE;

	RecRaster rast2;

	for (ii = REC_MAX_RASTER_SIZE - 1; ii; ii--)
		rast2.Raster[ii] = 0;

	int rast_bound = (bound - comp->upper + 1) >> (comp->scale);
	int nowbyte = 0;
	int j = rast.lnPixHeight * 8* ( (rast.lnPixWidth+63)/64);
	Rect16 rect1;
	Rect16 rect2;
	CCOM_comp* comp1;
	CCOM_comp* comp2;
	uchar* lp=NULL;
	// uchar* old;
			// int16_t lp_size;
			// int16_t numcomp;


			for(ii=rast_bound*8*((rast.lnPixWidth+63)/64);ii<j;ii++)
			{
				rast2.Raster[nowbyte]=rast.Raster[ii];
				rast.Raster[ii]=0;
				nowbyte++;
			}

			rast2.lnPixHeight=rast.lnPixHeight-rast_bound;
			rast2.lnPixWidth=rast.lnPixWidth;
			rast2.lnRasterBufSize=REC_MAX_RASTER_SIZE;
			rast.lnPixHeight=rast_bound;

			rect1.top=comp->upper;
			rect1.bottom=bound;
			rect1.left=comp->left;
			rect1.right=rect1.left+comp->w-1;

			rect2.top=bound+1;
			rect2.bottom=comp->upper+comp->h-1;
			rect2.left=comp->left;
			rect2.right=comp->left+comp->w-1;

			// CleanRaster(&rast,&rect1,comp->scale);
			// CleanRaster(&rast2,&rect2,comp->scale);

			comp1=CCOM_New(hCCOM,rect1.top,rect1.left,rect1.right-rect1.left+1,rect1.bottom-rect1.top+1);
 if(!comp1)
	 return FALSE;
 comp2=CCOM_New(hCCOM,rect2.top,rect2.left,rect2.right-rect2.left+1,rect2.bottom-rect2.top+1);
 if(!comp2)
 {
	 CCOM_Delete(hCCOM,comp1);
	 return FALSE;
 }
 comp1->scale=comp2->scale=comp->scale;

 while(comp1->scale)
 {
	 if(!Increase2(&rast,comp1))
		 break;
 }

 while(comp2->scale)
 {
	 if(!Increase2(&rast2,comp2))
		 break;
 }
 CCOM_Delete(hCCOM,comp1);
 CCOM_Delete(hCCOM,comp2);

 CCOM_handle hCutCCOM1=0;
 CCOM_handle hCutCCOM2=0;
 Rect16 Rc;

 Rc.left=comp->left;
 Rc.right=comp->left+comp->w-1;
 Rc.top=comp->upper;
 Rc.bottom=bound;

 int min_h_1=inf_let_h;
 int min_h_2=inf_let_h;
 if(type_let==1)
	 min_h_1=inf_dust_h;
 if(type_let==2)
	 min_h_2=inf_dust_h;

 hCutCCOM1=GetStrCCOM(hCPAGE,NULL,Rc,FALSE,FALSE,&rast,min_h_1);

 if(!hCutCCOM1)
	 return FALSE;

 Rc.left=comp->left;
 Rc.right=comp->left+comp->w-1;
 Rc.top=bound+1;
 Rc.bottom=comp->upper+comp->h-1;

 hCutCCOM2=GetStrCCOM(hCPAGE,NULL,Rc,FALSE,FALSE,&rast2,min_h_2);

 if(!hCutCCOM2)
 {
	 CCOM_DeleteContainer(hCutCCOM1);
	 return FALSE;
 }

 comp1=CCOM_GetFirst(hCutCCOM1,NULL);
 if(comp1==NULL)
 {
	 CCOM_DeleteContainer(hCutCCOM1);
	 CCOM_DeleteContainer(hCutCCOM2);
	 return FALSE;
 }

 CCOM_comp* newcomp;
 CCOM_comp* prev_comp=comp1;

 while(comp1)
 {
   newcomp=CCOM_New(hCCOM,comp1->upper+comp->upper-1,comp1->left+comp->left,comp1->w,comp1->h);
   if(newcomp)
   {
	   CCOM_Copy(newcomp,comp1);
	   MyCreateComp(newcomp);
   }
   prev_comp=comp1;
   comp1=CCOM_GetNext(comp1,NULL);
 }
 comp1=prev_comp;

 comp2=CCOM_GetFirst(hCutCCOM2,NULL);
 if(comp2)
	 prev_comp=comp2;
 else
	 prev_comp=comp1;

 while(comp2)
 {
   newcomp=CCOM_New(hCCOM,comp2->upper+bound,comp2->left+comp->left,comp2->w,comp2->h);
   if(newcomp)
   {
	   CCOM_Copy(newcomp,comp2);
	   MyCreateComp(newcomp);
   }
   prev_comp=comp2;
   comp2=CCOM_GetNext(comp2,NULL);
 }
 comp2=prev_comp;


 comp1->upper+=comp->upper-1;
 comp1->left+=comp->left;
 if(comp2!=comp1)
 {
  comp2->upper+=bound;
  comp2->left+=comp->left;
 }

 FILE* f=NULL;
 if(fl_cut==0)
     f=f_cut_comp;
 if(fl_cut==1)
	 f=f_cut_str;

 if(f)
 {
  int max_count=6;
	 int i;
//  if(oldRoot.top>comp->upper||oldRoot.bottom<comp->upper+comp->h-1||oldRoot.left>comp->left||oldRoot.right<comp->left+comp->w-1)
//  {
	 fprintf(f,"Top:%d",comp1->upper);
	 for(i=GetCountNumbers(comp1->upper);i<=max_count;i++)
		 fprintf(f," ");
	 fprintf(f,"Bottom:%d",comp1->upper+comp1->h-1);
	 for(i=GetCountNumbers(comp1->upper+comp1->h-1);i<=max_count;i++)
		 fprintf(f," ");
     fprintf(f,"Left:%d",comp1->left);
	 for(i=GetCountNumbers(comp1->left);i<=max_count;i++)
		 fprintf(f," ");
     fprintf(f,"Right:%d",comp1->left+comp1->w-1);
	 for(i=GetCountNumbers(comp1->left+comp1->w-1);i<=max_count;i++)
		 fprintf(f," ");

     fprintf(f,"\n");

	 	 fprintf(f,"Top:%d",comp2->upper);
	 for(i=GetCountNumbers(comp2->upper);i<=max_count;i++)
		 fprintf(f," ");
	 fprintf(f,"Bottom:%d",comp2->upper+comp2->h-1);
	 for(i=GetCountNumbers(comp2->upper+comp2->h-1);i<=max_count;i++)
		 fprintf(f," ");
     fprintf(f,"Left:%d",comp2->left);
	 for(i=GetCountNumbers(comp2->left);i<=max_count;i++)
		 fprintf(f," ");
     fprintf(f,"Right:%d",comp2->left+comp2->w-1);
	 for(i=GetCountNumbers(comp2->left+comp2->w-1);i<=max_count;i++)
		 fprintf(f," ");

     fprintf(f,"\n");

/*
	 oldRoot.top=comp->upper;
	 oldRoot.bottom=comp->upper+comp->h-1;
	 oldRoot.left=comp->left;
	 oldRoot.right=comp->left+comp->w-1;
  }
  */
 }

 FILE* f_temp=NULL;
 if(fl_cut==0)
	 f_temp=f_temp_cut_comp;
 if(fl_cut==1)
	 f_temp=f_temp_cut;

 if(f_temp)
 {
  int max_count=6;
	 int i;
//  if(oldRoot.top>comp->upper||oldRoot.bottom<comp->upper+comp->h-1||oldRoot.left>comp->left||oldRoot.right<comp->left+comp->w-1)
//  {
	 fprintf(f_temp,"Top:%d",comp1->upper);
	 for(i=GetCountNumbers(comp1->upper);i<=max_count;i++)
		 fprintf(f_temp," ");
	 fprintf(f_temp,"Bottom:%d",comp1->upper+comp1->h-1);
	 for(i=GetCountNumbers(comp1->upper+comp1->h-1);i<=max_count;i++)
		 fprintf(f_temp," ");
     fprintf(f_temp,"Left:%d",comp1->left);
	 for(i=GetCountNumbers(comp1->left);i<=max_count;i++)
		 fprintf(f_temp," ");
     fprintf(f_temp,"Right:%d",comp1->left+comp1->w-1);
	 for(i=GetCountNumbers(comp1->left+comp1->w-1);i<=max_count;i++)
		 fprintf(f_temp," ");

     fprintf(f_temp,"\n");

	 	 fprintf(f_temp,"Top:%d",comp2->upper);
	 for(i=GetCountNumbers(comp2->upper);i<=max_count;i++)
		 fprintf(f_temp," ");
	 fprintf(f_temp,"Bottom:%d",comp2->upper+comp2->h-1);
	 for(i=GetCountNumbers(comp2->upper+comp2->h-1);i<=max_count;i++)
		 fprintf(f_temp," ");
     fprintf(f_temp,"Left:%d",comp2->left);
	 for(i=GetCountNumbers(comp2->left);i<=max_count;i++)
		 fprintf(f_temp," ");
     fprintf(f_temp,"Right:%d",comp2->left+comp2->w-1);
	 for(i=GetCountNumbers(comp2->left+comp2->w-1);i<=max_count;i++)
		 fprintf(f_temp," ");

     fprintf(f_temp,"\n");

/*
	 oldRoot.top=comp->upper;
	 oldRoot.bottom=comp->upper+comp->h-1;
	 oldRoot.left=comp->left;
	 oldRoot.right=comp->left+comp->w-1;
  }
  */
 }

 if((!LDPUMA_Skip(CutCompD)&&fl_cut==0)||(!LDPUMA_Skip(CutStrD)&&fl_cut==1))
 {
	  if(!WasCutComp&&fl_cut==0)
		  WasCutComp=TRUE;
	  if(!WasCut&&fl_cut==1)
		  WasCut=TRUE;

	  DrawRect(MainWindowD,code_comp_cut_d,RGB(255,0,0),comp1->upper,comp1->upper+comp1->h-1,comp1->left,comp1->left+comp1->w-1);
      DrawRect(MainWindowD,code_comp_cut_d,RGB(255,0,0),comp2->upper,comp2->upper+comp2->h-1,comp2->left,comp2->left+comp2->w-1);
 }


 CCOM_DeleteContainer(hCutCCOM1);
 CCOM_DeleteContainer(hCutCCOM2);

 MyKillComp(comp);

 return TRUE;
// delete[] lp;
}

void DrawRect(Handle wnd, uint32_t OperCode, uint32_t color, int top, int bottom,
		int left, int right) {

	Rect16 Rect;

	Rect.bottom = bottom;
	Rect.left = left;
	Rect.right = right + 1;
	Rect.top = top - 1;

	LDPUMA_DrawRect(wnd, &(Rect), 0, color, -100, OperCode);

}

int GetMediumH(CCOM_handle hCCOM) {
	int sum_h = 0;
	int count = 0;
	for (CCOM_comp* comp = CCOM_GetFirst(hCCOM, NULL); comp; comp
			= CCOM_GetNext(comp, NULL)) {
		if (comp->h > inf_let_h_cut_comp && comp->h < sup_let_h) {
			sum_h += comp->h;
			count++;
		}
	}
	if (!count)
		return (inf_let_h_cut_comp * 3) / 2;
	else
		return sum_h / count;
}

void IfDifCutComp(void) {
	if (!f_old_cut_comp || !f_temp_cut_comp)
		return;

	char buf1[10000];
	char buf2[10000];
	fgets(buf1, 10000, f_temp_cut_comp);
	Bool fl_exist = FALSE;

	while (fgets(buf2, 10000, f_old_cut_comp)) {
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

	if (!fgets(buf2, 10000, f_old_cut_comp))
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

		if (!fgets(buf2, 10000, f_old_cut_comp))
			break;
	}

	fl_empty = FALSE;
	if (!fgets(buf1, 10000, f_temp_cut_comp))
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

		if (!fgets(buf1, 10000, f_temp_cut_comp))
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
			DrawRect(MainWindowD, code_comp_cut_d, RGB(0,0,255),
					pOldCut[i].top, pOldCut[i].bottom, pOldCut[i].left,
					pOldCut[i].right);
			WasDifHis = TRUE;
		}
	}
	// новая резка:
	for (i = nNewCut - 1; i >= 0; i--) {
		for (j = nOldCut - 1; j >= 0; j--) {
			if (IfEqv(pNewCut[i], pOldCut[j]))
				break;
		}
		if (j < 0) {
			DrawRect(MainWindowD, code_comp_cut_d, RGB(255,0,0),
					pNewCut[i].top, pNewCut[i].bottom, pNewCut[i].left,
					pNewCut[i].right);
			WasDifHis = TRUE;
		}
	}

}

Handle GetStrCCOM(Handle hCPage, uchar* ImageName, Rect16 Rc, Bool neg,
		Bool vertical, RecRaster* rast, int min_h) {
	int min_w, max_h, max_w;
	// int j;
	PAGEINFO info = { 0 };
	// uchar Name[CPAGE_MAXNAME];
	GetPageInfo(hCPage, &info);

	// if(ImageName)
	// {
	//	 for (j=0; j<CPAGE_MAXNAME; j++)
	//		Name[j] = ImageName[j];
	// }
	// else
	// {
	// for (j=0; j<CPAGE_MAXNAME; j++)
	//		Name[j] = info.szImageName[j];
	// }
	// Handle lpDIB;
	// if(!CIMAGE_ReadDIB(Name,&lpDIB,1))
	//	 return 0;
	ExcControl Control;
	if (vertical) {
		min_h = 2;
		min_w = 2;
		max_w = Rc.right - Rc.left + 1;
		max_h = max_w * 2;

	} else {
		min_w = 2;
		max_h = Rc.bottom - Rc.top + 1;
		max_w = Rc.right - Rc.left + 1;
	}

	int bytewide = ((rast->lnPixWidth + 63) / 64) * 8;
	Bool fotomet = FALSE;
	Bool RevOv = FALSE;

	Control.MinCompHei = min_h;
	Control.MinCompWid = min_w;
	Control.MaxCompHei = rast->lnPixHeight + 1;
	Control.MaxCompWid = bytewide * 8;
	Control.MaxScale = 0;
	Control.Control = Ex_ExtraComp/*|Ex_EvnRecog*/;//Andrey: опознавалка вынесена в отдельный модуль RRecCom, и кроме того, не вызвана функция REXC_SetEVNProperties для опознавания методом "Event"

	if (REXCExtra(Control, rast->Raster, bytewide, RevOv, bytewide * 8,
			rast->lnPixHeight, (info.DPIX * 10000) / 254, (info.DPIY * 10000)
					/ 254, 0, 0, 0, 0, fotomet)) {
		/* 	 //Andrey: здесь как раз и идет опознавалка
		 RRecComControl rcontrol;
		 rcontrol.MinCompHei=min_h;
		 rcontrol.MinCompWid=min_w;
		 rcontrol.MaxCompHei=rast->lnPixHeight+1;
		 rcontrol.MaxCompWid=bytewide*8;
		 rcontrol.MaxScale=0;
		 rcontrol.flags = RECOG_EVN;

		 Handle hCCOM = (Handle)REXCGetContainer();

		 if (hCCOM)
		 //Andrey: вообще-то так делать нельзя (в смысле передавать последними 2мя параметрами то, что передается), но пока я не знаю, как здесь получить язык распознавания, поэтому вот таким образом пропускается инициализация
		 if (!RRECCOM_Recog(hCCOM, rcontrol, NULL, -1))
		 LDPUMA_ConsoleN(RRECCOM_GetReturnString(RRECCOM_GetReturnCode()));

		 return (int32_t)hCCOM;
		 */return REXCGetContainer();
	}
	return 0;
}

void MyKillComp(CCOM_comp* comp) {
	if (Code_UB_Kill && Code_UB_Create) {
		int size = sizeof(uint32_t);
		CCOM_USER_BLOCK ub;
		ub.code = Code_UB_Create;
		if (!CCOM_GetUserBlock(comp, &ub)) {
			ub.code = Code_UB_Kill;
			ub.size = size;
			ub.data = (uchar*) (&Code_UB_Kill);
			CCOM_SetUserBlock(comp, &ub);
		}
	}

	CCOM_Kill(comp);
}

void MyCreateComp(CCOM_comp* comp) {
	int size = sizeof(uint32_t);
	CCOM_USER_BLOCK ub;
	ub.code = Code_UB_Create;
	ub.size = size;
	ub.data = (uchar*) (&Code_UB_Create);
	CCOM_SetUserBlock(comp, &ub);
}

void DrawComps(Handle hCCOM) {
	CCOM_handle hccom = (CCOM_handle) hCCOM;
	extern Handle hDrawComp;

	CCOM_comp * comp;
	for (comp = CCOM_GetFirst(hccom, NULL); comp; comp = CCOM_GetNext(comp,
			NULL))
		DrawRect(MainWindowD, 117, RGB(255,0,0), comp->upper, comp->upper
				+ comp->h - 1, comp->left, comp->left + comp->w - 1);

	LDPUMA_WaitUserInput(hDrawComp, MainWindowD);
	LDPUMA_DeleteRects(MainWindowD, 117);
}
