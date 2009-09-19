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
// Written by Shahverdiev  Alik
// This file cration date: 18.10.98
//
// PageTree.cpp
//
// ============================================================================
//#include <afxtempl.h>
//#include <afxwin.h>
//#include <afxcoll.h>
#include "stdafx.h"
#include <search.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include "aldebug.h"
#include "cpage.h"
#include "cpagetyps.h"
#include "creatertf.h"
#include "dpuma.h"

#define MAIN
#define ZAGL
#define TABL__ ON

#include "lst3_win.h"
#include "ful_txt.h"
#include "globus.h"
#include "wind32.h"
#include "consmess.h"

#include "decl.h"
#include "minmax.h"

#ifdef alDebug
#define CONS_MESS1 if(det1)	ConsMess
#define CONS_MESS2 if(det2)	ConsMess
#define CONS_MESS3 if(det3)	ConsMess
#define CONS_MESS4 if(det4)	ConsMess
#define CONS_MESS6 if(det6)	ConsMess
#define CONS_MESS9 if(det9)	ConsMess
#define CONS_MESS20 if(det20)	ConsMess
#define CONS_MESS21 if(det21)	ConsMess
#define CONS_MESS22 if(det22)	ConsMess
#define CONS_MESS23 if(det23)	ConsMess
int det0 =1,     //draw step
    det1 =0,     //common
    det2 =0,     //search_interval
    det3 =0,     //SearchColHist
    det4 =0,     //geometry rtf
	   det5 =0,     //write rtf
	   det6 =0,     //sort && show fragm
    det7 =0,     //print string
	   det8 =0,     //font
	   det9 =0,     //bad return
    det10=0,     //ierarxiya colonok
	   det11=0,     //Get_all_term_fragms
	   det12=0,     //calculate ideal size of fragms
	   det13=0,     //Size page
	   det14=0,     //control frame
	   det15=0,     //frame_coor
	   det16=0,     //ordering and recalc colons
	   det17=0,     //indent
	   det18=0,     //межстрочное растояние
				det19=0,     //отладка ошибок
				det20=0,     //отладка realese version
				det21=0,     //отладка realese version
				det22=0,     //отладка realese version--memory
				det23=1,     //отладка realese version only name file
    dets =0;     //tmp break points



short FlagGraphic1=0,Graphic1Color=0;
std::vector <tagRECT>         *pTheGeomStep=NULL;
extern  std::vector <tagRECT> *pTheGeomStep1;
extern  std::vector <tagRECT> *pTheGeomStep2;
extern  std::vector <tagRECT> *pTheGeomTemp ;
extern  vectorWord           *pFragRectColor;
extern  void MyDrawForDebug(void);
extern  uint16_t                 *CountRect;

#else
	#define CONS_MESS1 1 ? 0 : ConsMess
	#define CONS_MESS2 1 ? 0 : ConsMess
	#define CONS_MESS3 1 ? 0 : ConsMess
	#define CONS_MESS4 1 ? 0 : ConsMess
	#define CONS_MESS6 1 ? 0 : ConsMess
	#define CONS_MESS9 1 ? 0 : ConsMess
	#define CONS_MESS20 1 ? 0 : ConsMess
	#define CONS_MESS21 1 ? 0 : ConsMess
	#define CONS_MESS22 1 ? 0 : ConsMess
	#define CONS_MESS23 1 ? 0 : ConsMess
#endif

#define  My_Debug ON
#define  COLH   struct h_colh

typedef void(*FMyDraw)(void);
typedef struct tagSETUP_GENERATE_TREE
{
	int16_t size_x,size_y; //standard sizes of symbols in page
} SETUP_GENERATE_TREE;

int16_t   FlagOdinSectorOdnaColonka = FALSE;
int16_t   MaxLev,NumColTrue,RegimExt;
const char    *errRtf="ED_RTF";
Rect16  *RectFragm;
FRAME   *ArrFrm;

extern  int16_t SizeYGlobUpp;
extern  SUB_ALLOC SubZn;
extern  int16_t MonoSpaceAllPage;
extern  int16_t HeiStrAllPage;
extern  uint32_t   RtfWriteMode;

/*
util     - memory alloc
util_lst - lists manipulation , etc.
sort_int - fast sorting
util_spl:
	OpenFullOutTiger  - RTF
	CalcStatTiger     - статистика об интервалах (внутри- и меж- словные и т.п.)
*/
extern "C" Bool PageTree(FILE *InFileName, CRtfPage* RtfPage, const char* OutFileName);
extern short __cdecl OpenFullOutTiger(FILE *FileName);
void   show_frm(int16_t NumFragm,FRAME **frm);
Bool   Alik_sort_function( const void *a, const void *b);
int    CalcStatTiger(void);
int16_t  GenerateTreeByFragm(Rect16 *RectFragm,int16_t NumFragm,
	                         SETUP_GENERATE_TREE *setup, FRAME ***Frm1, INF_TREE *Inf);
void   GetFirstIndexFragm(Rect16 *RectFragm,int16_t NumCol,int16_t *nc,FRAME **frm);
int16_t  CreateTreePlainTxt1(BOUND BndTxt,STRET *LineV,int16_t NumLV,STRET *LineH,
                          int16_t NumLH,FRAME **frm,int16_t NumFrm,INF_TREE *Inf,int16_t size_x,int16_t size_y);
int16_t  AddLine1(LINE_KNOT **Line1,int16_t *nCurr,int16_t *nMax,int16_t Coor,int16_t Thres);
int16_t  SearchColHist1(FRAME **frm,int16_t k_frm,BOUND *bnd,int16_t ave_x,int16_t ave_y,
                     int16_t reg,int16_t *k_int,int16_t **intr1,int16_t **begI,int16_t **endI,int16_t *NumMax);
int16_t  SearchInterval1(FRAME **frm,int16_t k_frm,int16_t **beg1,int16_t **end1,
                      int16_t *k_int1,BOUND *bnd,int16_t ave_dir,int16_t ave_ort,int16_t reg,int16_t *NumMax);
KNOTT* IncKnot(KNOTT *up,KNOTT *after,KNOTT **free);
void   FillFieldKNOTT1(KNOTT *ptr,int16_t Left,int16_t Right,int16_t Top,int16_t Bottom,
																					int16_t InBegFrm,int16_t NumFrm,int16_t InColA,
																					uint OrderChild,uint Type,uint AllowOCR,uint JustH,uint JustV,char *Name);
int16_t  SortHorLine1(LINE_KNOT *LineHK,int16_t NumH,LINE_KNOT *LineVK,int16_t NumV,
                   KNOTT *Root,KNOTT ***colt1,int16_t *k_colt1,FRAME **frm);
char   *get2_param(char *str,char *param,int16_t max_len);
int16_t  Check_IsItFalseHorLine(int16_t recalc,int16_t reg,FRAME **frm,int16_t *his,int16_t pos,int16_t len,
																													int16_t maxh,int16_t sum,int16_t len_group,int16_t *his_first_group,
																													int16_t *his_second_group,BOUND *bnd,int16_t k_frm);
int16_t  check_white_int(int16_t beg_white_int,int16_t end_white_int,int16_t maxh,
           											int16_t *his_second_group);
int16_t  GetRealSize( char* str,int16_t len,int16_t FontSize ,int16_t FontNumber,int16_t* strHeight);
void   Get_all_term_fragms( KNOTT *ptr,int16_t *Colt,int16_t *iv ,int16_t NumCol,FRAME **frm);
void   Get_all_term_fragms1( KNOTT* ptr,int16_t* Colt,int16_t* iv,int16_t NumCol,FRAME **frm);
int16_t  term_col_seriated(int16_t* term_ind,int16_t Count_Term_Col,int16_t* K_Ver_Flag_Term);
int16_t  OrderingAndRecalcCol(int16_t Nsector,int16_t Ncol,int16_t H0,int16_t L0,int16_t *W_P,int16_t *H_P,
																											int16_t dist_col,KNOTT *pRoot,FRAME **frm,
																											float Max_koof_for_width,float Max_koof_for_height);
int16_t  RecalcRect(int16_t L,int16_t H,int16_t *w,int16_t *h,KNOTT *ptrc,FRAME **frm,
        								 float Max_koof_for_width,float Max_koof_for_height);
int16_t GetOffsetVerticalCell(int16_t L,int16_t H,int16_t *w,int16_t *h,KNOTT *ptrc,FRAME **frm);
void RtfUnionRect_CRect_SRect(tagRECT *s1,SRECT *s2);
void RtfUnionRect_CRect_CRect(tagRECT *s1,tagRECT *s2);
void RtfAssignRect_CRect_SRect(tagRECT *s1,SRECT *s2);
void RtfAssignRect_CRect_Rect16(tagRECT *s1,Rect16 *s2);
void	RtfCalcRectSizeInTwips(tagRECT *s1, float Twips);
void RtfAssignRect_CRect_CRect(tagRECT *s1,tagRECT *s2);

//int16_t CreateEmptyRtfFile(void);

////////////// functions, which are moved from other modules //////////////
static HWND h_found=NULL;
extern "C" Bool __stdcall FindByPartOfTitle( HWND hwnd,	      // handle to parent window
                                             LPARAM lParam 	) // application-defined value
{
   char title[256];
   ::GetWindowText(hwnd, title, sizeof(title));
   if (strstr(title, (char*)lParam))
   {
		 h_found=hwnd;
      return FALSE;  // don't continue
   }
   return TRUE;
}

int ConsMess( const char* str, ... )
{
   // 1. find window
   h_found=NULL;
   EnumWindows( (WNDENUMPROC)FindByPartOfTitle, (long)"Console32" );
   if (!h_found)
      return 0;
   // 2. make string
   if (str==NULL)
      return 0;
   int count=0;

   char strbuf[4096];   // string to be put
   //   strbuf[0]='\n';
   {  va_list list;
      va_start(list, str );
      count=vsprintf( strbuf, str, list );
      va_end( list );
      strcat(strbuf,"\r\n");
   }
   // 3. send message
   COPYDATASTRUCT cd={0};
   cd.dwData = 1354235;
   cd.lpData = strbuf;
   cd.cbData = count+10;

#ifdef _MSC_VER
   SendMessage( h_found, WM_COPYDATA, (WPARAM)0, (LPARAM)(&cd) );
#endif
   return count;
}

Bool Alik_sort_function( const void *a, const void *b)
{
	if (((*(FRAME **)a)->up   >   (*(FRAME **)b)->up ) ||
     (((*(FRAME **)a)->up   ==  (*(FRAME **)b)->up ) &&
      ((*(FRAME **)a)->down >   (*(FRAME **)b)->down)))     return 1;
 if   ((*(FRAME **)a)->up   <   (*(FRAME **)b)->up )        return -1;
 return 0;
}

//==
int MaxArr(int *x,int n,int *PosExtr)
{ int i,in=0;
  do0(i,1,n) if(x[i] > x[in]) in=i;
  *PosExtr=in;
	return x[in];
}
//==
int MinArr(int *x,int n,int *PosExtr)
{ int i,in=0;
  do0(i,1,n) if(x[i] < x[in]) in=i;
  *PosExtr=in;
	return x[in];
}

//==
/* // !!! Art - устарело
int statis(TYPE *arr,int n,TYPE *ave1,TYPE *sig1,TYPE *med,TYPE *mod,int regim)
//если regim > 0, усекаем края выборки вместо [0,n] - [n/regim,n-n/regim]
{ int i; long ave=0,sig=0,work;

  u4sort(arr,n+1,sizeof(TYPE),(COMP_FUN)comp1); //медиана
  if(regim) //обрезаем края выборки
  { int beg=n/regim; if((n-=2*beg)<0){n+=2*beg;goto m1;}
    if(beg) do0(i,0,n) arr[i]=arr[i+beg];
  }
  m1: *med=arr[n>>1];
  do0(i,0,n) ave+=arr[i]; ave/=(long)(n+1);
  do0(i,0,n)
  { work=arr[i]-ave;
    sig+=work*work;
  }
  sig=(long)sqrt(sig/(float)(n+1));
  *ave1=(TYPE)ave; *sig1=(TYPE)sig;
  return 0;
}
*/ // !!! Art - устарело
//==
void bound_frm(FRAME **frm,int k_frm,BOUND *bnd)
{
	int ymin=32000,ymax=-32000,xmin=32000,xmax=-32000,i;
	do0(i,0,k_frm)
	{
		ymin=MIN(ymin,frm[i]->up);
		ymax=MAX(ymax,frm[i]->down);
		xmin=MIN(xmin,frm[i]->left);
		xmax=MAX(xmax,frm[i]->right);
	}
	bnd->left=xmin; bnd->right=xmax; bnd->up=ymin; bnd->down=ymax;
}

//==
void TestKNOTT1(KNOTT *ptr,LINE_KNOT *LineVK,LINE_KNOT *LineHK,int16_t NumFrm,int16_t NumVK,int16_t NumHK)
{
	RECT r1=ptr->Rect; KNOTT *up=ptr->up;
	if(r1.left >= r1.right || r1.top == r1.bottom ||
     r1.left < 0 || r1.left >= NumVK || r1.right < 0 || r1.right >= NumVK ||
     r1.top < 0 || r1.top >= NumHK || r1.bottom < 0 || r1.bottom >= NumHK ||
     LineVK[r1.left].beg >= LineVK[r1.right].beg ||
     LineHK[r1.top].beg >= LineHK[r1.bottom].beg ||
     ptr->InBegFrm < (up ? up->InBegFrm:0) ||
     ptr->InBegFrm+ptr->NumFrm > (up ? up->InBegFrm+up->NumFrm:NumFrm))
       ERR(1,"TestKNOTT1");
}

//===Для нераспознан.знакоместа Return=0
uchar Get1Alt(ZN *z,int na)
{ int ka=z->Title.Z_Num_Alt;
  if(ka <= 0 /*|| na >= ka*/)
		return 0;
  else
		return z->Alt[na].a_Code;
}

//==
int Statist(int *arr,int n,int *ave,int *sig,int *med,int *mod,int regim)
  { return n > 0 ? statis1(arr,n-1,ave,sig,med,mod,regim) : -1; }
//==
int statis1(TYPE *arr,int n,TYPE *ave1,TYPE *sig1,TYPE *med,TYPE *mod,int regim)
/*если regim > 0, усекаем края выборки вместо [0,n] - [n/regim,n-n/regim]*/
{
	int i;
	long ave=0,sig=0,work;

	if(*med)
  { u4sort(arr,n+1,sizeof(TYPE),(COMP_FUN)comp1);
    if(regim) /*обрезаем края выборки*/
    {
			int beg=n/regim;
			if((n-=2*beg)<0)
				{n+=2*beg;goto m1;}
      if(beg) do0(i,0,n)
				arr[i]=arr[i+beg];
    }
    m1: *med=arr[n>>1];
  }
  do0(i,0,n) ave+=arr[i]; ave/=(long)(n+1);
  if(*sig1)
  {
		do0(i,0,n)
			{ work=arr[i]-ave; sig+=work*work; }
    sig=(long)/*(float)*/sqrt(sig/(float)(n+1)); *sig1=(TYPE)sig;
  }
  *ave1=(TYPE)ave;
  return 0;
}

//== SizeWin - ширина окна при наложении гистограммы(if =1 => стандарт.гистограмма)
int statis2(TYPE *arr,int n,TYPE *med,TYPE *mod,int SizeWin,int *NumMod)
{
	int i,PosExtr,num; long ave=0,sig=0;
	int *his,mi=32000,ma=-32000,nmax,modd=-32000,ki,sum=0,k=(n+1)>>1;
	//---Расчет моды,медианы по гистограмме---
	int medd=-32000; const char *err="statis2";

	mi=MinArr(arr,n,&PosExtr);
	ma=MaxArr(arr,n,&PosExtr);
	ki=ma-mi; num=ki+SizeWin;
	if((his=(int*)malloc(num*sizeof(int)))==NULL)
		return NOT_ALLOC;
	memset(his,0,num*sizeof(int));
	if(SizeWin == 1) //стандарт.гистограмма
	{
		do0(i,0,n)
			++his[arr[i]-mi];
	}
	else if(SizeWin > 1) //огрубление гистограммы
	{
		if(SizeWin > 2)
			return -1; //!пока!
		do0(i,0,n)
		{
			++his[arr[i]-mi];
			++his[arr[i]-mi+1];
		}
	}
	else
		return -1;
	if(mod)
	{
		do0(i,0,ki)
			if(modd < his[i])
			{
				modd=his[i];
				nmax=i;
			}
		*mod=nmax+mi;
		*NumMod=his[nmax];
	}
	if(med)
	{
		do0(i,0,ki)
			if((sum+=his[i]) >= k)
			{
				medd=mi+i;
				break;
			}
		*med=medd;
	}
	free(his);
	return 0;
}

//Память под beg_as[][] захват-тся внутри, память под списк.пр-во KNOT3 снаружи
//==Генерация плоской АС на двусвязных списках
int GenAS(FRAME **frm,int k_frm,int dx,int dy,BOUND *bnd,KNOT3 *beg_free,
    int value,AS *As,RECT Rect)
{
	int xmin,xmax,ymin,ymax,kx,ky,nx,ny,i,delx,dely;
  KNOT3 ***beg,*ptr; FRAME *f;
  const char *err="GenAS";

	/*оконтуривание решетки пустой полосой*/
  xmin=bnd->left;xmax=bnd->right;ymin=bnd->up;ymax=bnd->down;
  xmin-=(dx+1);kx=((xmax-xmin+1)/dx)+1;xmax=xmin+(kx+1)*dx;
  ymin-=(dy+1);ky=((ymax-ymin+1)/dy)+1;ymax=ymin+(ky+1)*dy;
  if((beg=(KNOT3***)malloc((ky+1)*sizeof(KNOT3**)))==NULL)return -3;
  do0(i,0,ky)
  {
		if((beg[i]=(KNOT3**)malloc((kx+1)*sizeof(KNOT3*)))==NULL)
			return NOT_ALLOC;
    memset(beg[i],0,(kx+1)*sizeof(PTR));
  }
  if(value != INDEF)//заполнение ассоциат. стр-ры
  {
		do0(i,0,k_frm)
    { f=frm[i];
      ny=(((f->up  + f->down ) >>1)-ymin)/dy;
      nx=(((f->left+ f->right) >>1)-xmin)/dx;
			if(ny < 0 || nx < 0 || ny > ky || nx > kx)
					return -1;
      if( (ptr=(KNOT3*)inc_lst((KNOT**)&beg[ny][nx],(KNOT**)&beg_free))==NULL)
				return -4;
      ptr->f=f; ptr->cl=value;
      #if defined (__MRK__) || defined (__DOT__)
        ptr->beg=NULL;
      #endif
    }
  }
  else
  {
		do0(i,0,k_frm)
    { f=frm[i];
      ny=(((f->up  + f->down ) >>1)-ymin)/dy;
      nx=(((f->left+ f->right) >>1)-xmin)/dx;
      if( (ptr=(KNOT3*)inc_lst((KNOT**)&beg[ny][nx],(KNOT**)&beg_free))==NULL)return -4;
      ptr->f=f; delx=f->right-f->left; dely=f->down-f->up;
      ptr->cl= (delx >= Rect.left && delx <= Rect.right &&
                dely >= Rect.top  && dely <= Rect.bottom) ? 1 : value;
    }
  }
  As->bnd=bnd;As->kx=kx;As->ky=ky;As->dx=dx;As->dy=dy;
  As->beg_free=beg_free;As->beg_as=beg;
  As->xmin=xmin; As->ymin=ymin;
  return 0;
}

#ifdef alDebug
	//===if ColFrm < 0, no draw frm
	//================================================================================
	//=================================   ImageKnot1  ================================
  //================================================================================
	void ImageKnot1(KNOTT *ptr,LINE_KNOT *LineVK,LINE_KNOT *LineHK,int16_t col,
	                int16_t line_style,int16_t fill,int16_t ColFrm,FRAME **f,int16_t NumFrm,
									int16_t NumVK,int16_t NumHK)
	{
		RECT r,r1=ptr->Rect;
		int16_t fl= (ColFrm != INDEF) ? 0 : 1;

		if(ColFrm == INDEF)
 			  ColFrm=col;
	  r.left=LineVK[r1.left].beg;
		r.right=LineVK[r1.right].beg;
	  r.top=LineHK[r1.top].beg;
		r.bottom=LineHK[r1.bottom].beg;
	  image_rect(&r,col,line_style,fill);
	  if(ColFrm>=0 && ptr->InBegFrm >= 0 && ptr->NumFrm >= 0)
	  {
		 TestKNOTT1(ptr,LineVK,LineHK,NumFrm,NumVK,NumHK);
	   image_frame(&f[ptr->InBegFrm],ptr->NumFrm-1,0,line_style,fill);
	  }
	}

	//проход по дереву с отображением
	//================================================================================
	//=================================   ImageTree1  ================================
  //================================================================================
	int16_t ImageTree1(KNOTT *Root,LINE_KNOT *LineVK,LINE_KNOT *LineHK,
	  FRAME **frm,int16_t NumFrm,int16_t NumVK,int16_t NumHK)
	{
		STACK St;
	  int16_t DepthTree=20,col,ColFrm,i=1;
	  KNOTT *Curr;
	  char *err="ImageTree1";

		if(NewStack(DepthTree,&St))
			return NOT_ALLOC;
	  Curr=Root;
		col=0;

	  while(Curr != NULL)
	   {if(++col > 15)
	  		col=1;
	  	ColFrm=col;
	    //ImageKnot1(Curr,LineVK,LineHK,col,0xFFFF,_GBORDER,ColFrm,frm,NumFrm,NumVK,NumHK);
	    //--Рисуем по перекрестным ссылкам терминал. H-графы,списки их V-ссылок и им обратные
	    //if(Curr->InBegFrm == IN_NO && Curr->down == NULL) //Терм. H-узел
			if(det10)	{ConsMess("i=%2d   InBegFrm=%2d  NumFrm=%2d  InColA=%6d  OrderChild=%2d",
			 		                 i,Curr->InBegFrm,Curr->NumFrm,Curr->InColA,Curr->OrderChild);
		              ++i;}

			ImageKnot1(Curr,LineVK,LineHK,col,(int16_t)0xFFFF,_GBORDER,(int16_t)-1,frm,NumFrm,NumVK,
				         NumHK);
	    Curr=NextKnot(Curr,&St);
	    if(OverflowStack(&St))
	    	return NOT_ALLOC;
	   }
	  DelStack(&St);
	  return 0;
	}

	void init_font(void)
	{ ;	}
#endif /*DRAW*/

//==
static int16_t Realloc2(KNOTT*** colt,KNOTT*** colnt,KNOTT*** colnt1,
	int16_t **begI,int16_t **endI,int16_t **intr,int16_t nOld,int16_t nNew)
{ int16_t oldS =nOld*sizeof(PTR),newS =nNew*sizeof(PTR),
        oldS1=nOld*sizeof(int16_t),newS1=nNew*sizeof(int16_t);
  if((colt   && ((*colt=(KNOTT**)realloc_m(*colt,  oldS,newS))==NULL))||
     (colnt  && ((*colnt=(KNOTT**)realloc_m(*colnt, oldS,newS))==NULL))||
     (colnt1 && ((*colnt1=(KNOTT**)realloc_m(*colnt1,oldS,newS))==NULL))||
     (begI   && ((*begI=(int16_t*)realloc_m(*begI,oldS1,newS1))==NULL))||
     (endI   && ((*endI=(int16_t*)realloc_m(*endI,oldS1,newS1))==NULL))||
     (intr   && ((*intr=(int16_t*)realloc_m(*intr,oldS1,newS1))==NULL)))
       return NOT_ALLOC;
  return 0;
}

//==
void ConvertRect16ToBnd(Rect16 *r,SRECT *b)
{
	b->left=r->left; b->right=r->right; b->top=r->top; b->bottom=r->bottom;
}

#ifdef alDebug
void show_frm(int16_t NumFragm,FRAME **frm)
{
	int16_t i;

	for(i=0; i < NumFragm; ++i)
	{
		if(det6 || det17)
			ConsMess(" i=%d start_pos=%d l=%d r=%d t=%d b=%d",i,frm[i]->start_pos,
		           frm[i]->left,frm[i]->right,frm[i]->up,frm[i]->down);
	}
}
#endif

//==Generate Columns Tree by Cunei-array of fragments
//tree generation:
//	input
//		RectFragm[NumFragm],
//		setup - input parameters
//	output - Inf
int16_t GenerateTreeByFragm(Rect16 *RectFragm,int16_t NumFragm,
	SETUP_GENERATE_TREE *setup, FRAME ***Frm1, INF_TREE *Inf)
{
	FRAME  **frm;
	int16_t  i,fl=0;
	BOUND  BndAll;

	ArrFrm=(FRAME*) malloc(NumFragm*sizeof(FRAME));
	frm=   (FRAME**)malloc(NumFragm*sizeof(PTR));
	if(!ArrFrm || !frm)
		return NOT_ALLOC;

	*Frm1=frm;

	//--BndAll is all page boundaries--
	BndAll.left=32000; BndAll.right=-32000; BndAll.up=32000; BndAll.down=-32000;
	//--convert from RectFragm[NumFragm] to frm[NumFragm]--
	for(i=0; i < NumFragm; ++i)
	{
		frm[i]=&ArrFrm[i];
		//"i" is initial index for text information extraction from fragment arrays Zn,TitleWord,TitleStr,etc.
		ArrFrm[i].start_pos=i;
		ArrFrm[i].left =RectFragm[i].left;
		ArrFrm[i].right=RectFragm[i].right;
		ArrFrm[i].up   =RectFragm[i].top;
		ArrFrm[i].down =RectFragm[i].bottom;
#ifdef alDebug
		if (dets) ConsMess("i=%d  l=%d,r=%d,u=%d,d=%d",i,
			          RectFragm[i].left,RectFragm[i].right,RectFragm[i].top,RectFragm[i].bottom);
#endif
		BndAll.left =MIN(BndAll.left ,RectFragm[i].left);
		BndAll.right=MAX(BndAll.right,RectFragm[i].right);
		BndAll.up   =MIN(BndAll.up   ,RectFragm[i].top);
		BndAll.down =MAX(BndAll.down ,RectFragm[i].bottom);
	}
	//--calling internal function for tree generation--
	if(CreateTreePlainTxt1(BndAll,NULL,0,NULL,0,
     frm,NumFragm,Inf, setup->size_x, setup->size_y))
			return (int16_t)fl-200;

	return 0;
}

//Создать дерево стр-ры Text Plain
//Input:
//   BndTxt - page boundaries
//   LineV[NumLV],LineH[NumLH] - вертикальные и горизонтальные линии, найденные на листе
//   frm[NumFrm] - рамки букв (фрагментов в CunieForm) листа
//   size_x,size_y - стандарт. размер букв листа
//Output:
//   Inf - результирующая структура хранения дерева колонок листа
//Return: 0 - OK
//====
int16_t CreateTreePlainTxt1(BOUND BndTxt,STRET *LineV,int16_t NumLV,STRET *LineH,
    int16_t NumLH,FRAME **frm,int16_t NumFrm,INF_TREE *Inf,int16_t size_x,int16_t size_y)
//====
{
	int16_t     i,nVmax=3*MAX_COL,nHmax=3*MAX_COL,nV,nH,j;
	int16_t     nT=nVmax+1,fl,tmp,InBegFrm,NumF,ThresX,ThresY;
	int16_t     left,right,top,bottom,NumMax=3*MAX_COL,MaxOld;
	int16_t     *intr,*begI,*endI,NumT=nVmax+1;
	int16_t     k_colnt,k_colnt1,k_colt,minz;
	int16_t     fl_beg/*Признак первого расщепления на колонки*/;
	int16_t     order/*Тип искомой упорядоченности:HOR - горизонт. или VER - вертикал.*/;
	int16_t     kf,kcol,del,del1;
	TREE2     Tree;
	BOUND     bndc;
	KNOTT     *PrevChild,*Child;
	KNOTT     **colt,**colnt,**colnt1,*ptr;
	LINE_KNOT *LineVK,*LineHK;
	STAT_CELL *StatCell=(STAT_CELL*)malloc(sizeof(STAT_CELL));
	const char*err="CreateTreePlainTxt1";

	CONS_MESS1("===  CreateTreePlainTxt1  === ");

	//LineVK, LineHK - виртуальные либо натуральные линии разграфки,
	//   сепарирующие найденные колонки, с помощью этих линий производитс
	//   горизонт. и вертикал. выравнивание колонок
	LineVK=(LINE_KNOT*)malloc((nVmax+2)*sizeof(LINE_KNOT));
	LineHK=(LINE_KNOT*)malloc((nHmax+2)*sizeof(LINE_KNOT));
	intr=(int16_t*)malloc(NumMax*sizeof(int16_t));
	begI=(int16_t*)malloc(NumMax*sizeof(int16_t));
	endI=(int16_t*)malloc(NumMax*sizeof(int16_t));
	//Заносим в разграфку натурал. линии после их ортогонализации
	//-- Замыкание системы H- и V-линий с краев по рамке листа --
	LineVK[0].beg=BndTxt.left;
	LineVK[1].beg=BndTxt.right;
	LineHK[0].beg=BndTxt.up  ;
	LineHK[1].beg=BndTxt.down ;
	LineVK[0].Thick=LineVK[1].Thick=LineHK[0].Thick=LineHK[1].Thick=0;
	LineVK[0].TypeLine=LineVK[1].TypeLine=LineHK[0].TypeLine=LineHK[1].TypeLine=HIDDEN_LINE;
	nV=nH=2;

	colt=  (KNOTT**)malloc(NumMax*sizeof(PTR));
	colnt= (KNOTT**)malloc(NumMax*sizeof(PTR));
	colnt1=(KNOTT**)malloc(NumMax*sizeof(PTR));
	if(colt==NULL||colnt==NULL||colnt1==NULL)
	{
		CONS_MESS9("NOT_ALLOC 609");
		return NOT_ALLOC;
	}
	//--Создание спискового простр-ва и корня дерева--
	Tree.NumKnot=NumMax*2;
	Tree.NumSeg=-1;
	if((fl=init_lst(&Tree.ArrSeg,&Tree.NumSeg,Tree.NumKnot,(KNOT**)&Tree.free,sizeof(KNOTT)))!=0)
	{
		CONS_MESS9("NOT_ALLOC 618");
		return -90-fl;
	}
	if((Tree.Root=IncKnot(NULL,NULL,&Tree.free)) == NULL)
	{
		CONS_MESS9("NOT_ALLOC 624");
		return -6;
	}
	FillFieldKNOTT1(Tree.Root,0,1,0,1,0,NumFrm,IN_NO,UNSORT,ROOT,TRUE,ALG_BEG,ALG_BEG,NULL);
	colnt[k_colnt=0]=Tree.Root; k_colt=-1;
	fl_beg=1; order=VER;//Так удобнее для RTF-converter
	ThresX=ThresY=(int16_t)(1.5*size_y);

	int16_t MaxAllowLev,flTerm;
	MaxAllowLev=6;
	MaxLev=0; flTerm=FALSE;

	//вначале считаем, что все фрагменты образуют одну колонку,
	//внешний цикл колонизации - на каждой итерации MaxLev производитс
	//попытка разделить те узлы-колонки, которые есть на этом уровне
	//и для тех узлов-колонок, которые поделились, строим дочерние
	//узлы-подколонки на следующем уровне MaxLev+1
	//colnt[k_colnt]   - массив узлов текущего уровня MaxLev
	//colnt1[k_colnt1] - массив узлов следующего уровня MaxLev+1
	//colt[k_colt] - массив терминальных колонок
	//признак конца итераций - на очередном уровне нет узлов, т.е. ни один
	//из узлов предыдущего уровня не удалось раздробить

	while(k_colnt >= 0)
	{
		if(++MaxLev > MaxAllowLev) //ограничение Word-a на глубину дерева
			flTerm=TRUE;

		k_colnt1=-1; /*Число нетерминал. колонок след. уровня*/
		//--цикл попытки дробления всех узлов текущего уровня--
		CONS_MESS1("while....................  k_colnt=%d ",k_colnt);

		do0(i,0,k_colnt)
		{
			CONS_MESS1("beg......................1 ");
			CONS_MESS1("i=%d ",i);
			ptr=colnt[i];          //текущий узел
			left=ptr->InBegFrm;    //индекс первой рамки (фрагмента) узла
			kf=ptr->NumFrm-1;      //число рамок(фрагментов)узла
			ptr->OrderChild=UNSORT;//вначале считаем, что рамки узла неупорядочены
														 //если же узел удастся разбить, то порядок
														 //узла определяется упорядоченностью его дочерних
														 //узлов (V- или H-порядок)
			CONS_MESS1("(index first fragm)left=%d , (count fragm) kf=%d ",left,kf);
             //bndc - рамка узла
			if(!fl_beg)
				bound_frm(&frm[left],kf,&bndc); //рамка узла есть рамка входящих в узел рамок фрагментов
			else
				bndc=BndTxt; //вначале рамка узла-корня есть рамка всего листа
			CONS_MESS1("fl_beg=%d, bndc : left=%d,  right=%d,  up=%d,  down=%d",fl_beg,bndc.left,bndc.right,bndc.up,bndc.down);
			//---
			MaxOld=NumMax;
			//-- попытка разбиение узла путем построения соответствующего --
			//   профиля: на ось Ox для order=HOR или на ось Oy для order=VER
			//   и поиска на нем межколонных зазоров
			if(flTerm)
				{fl=kcol=0;}
			else
			{
				if((fl=SearchColHist1(&frm[left],kf,&bndc,size_x,size_y,order,
        		&kcol,&intr,&begI,&endI,&NumMax)) < 0)
				{
					CONS_MESS9(" 715 !!!!! SearchColHist1 fl=%d",fl);
					return fl-2000;
				}
				CONS_MESS1("after SearchColHist1 MaxOld=%d, NumMax=%d",MaxOld,NumMax);
			}

			if(MaxOld != NumMax &&
				Realloc2(&colt,&colnt,&colnt1,NULL,NULL,NULL,MaxOld,NumMax) == NOT_ALLOC)
  				{
					CONS_MESS9("NOT_ALLOC 726");
					return NOT_ALLOC;
				}

			if(!fl && fl_beg)
			{
			//В I-ый раз пробуем ортогональное направления разбиения,
			//если не удалось разбиение в прямом направлении

			//вообще-то лучше сделать в корневом узле не предопределенный выбор
			//направления первого разбиения, если допустимы оба направления,
			//а выбор того направления, на котором между колонками больший зазор

				order = order == HOR ? VER:HOR;
				MaxOld=NumMax;
				if((fl=SearchColHist1(&frm[left],kf,&bndc,size_x,size_y,order,
					&kcol,&intr,&begI,&endI,&NumMax)) < 0)
				{
					CONS_MESS9("744 fl-2000");
					return fl-2000;
				}
				if(MaxOld != NumMax &&
					Realloc2(&colt,&colnt,&colnt1,NULL,NULL,NULL,MaxOld,NumMax) == NOT_ALLOC)
				{
					CONS_MESS9("752 NOT_ALLOC");
		          	return NOT_ALLOC;
				}
			}
			if(fl) //если разбиение удалось, присваиваем узлу порядок, определяемый направлением разбиени
				ptr->OrderChild=order;

			if(kcol > 0)
			{
				CONS_MESS1("beg------------------2");
				CONS_MESS1("kcol=%d",kcol);
				BOUND b;
				//,bnd;
		 		if(fl_beg && ptr->OrderChild == HOR)
					--MaxAllowLev;

				//Разбиение удалось => заносим в дерево дочерний куст подколонок данного узла
				PrevChild=NULL;
				do0(j,0,kcol) //цикл внесению дочерей в дерево
				{
				//вставляем дочерний узел в дерево:
				//      ptr - узел-родитель
				//      PrevChild - предыдущее дит
				//      Tree.free - свободное списковое пространство дерева
					CONS_MESS1("beg------------------3");
					CONS_MESS1("j=%d",j);
					if((Child=IncKnot(ptr,PrevChild,&Tree.free)) == NULL)
					{
						CONS_MESS9("788 !!!!!! IncKnot return -7");
						return -7;
					}

					//номер начального фрагмента InBegFrm и число фрагментов NumF данной колонки
					tmp=j ? intr[j-1]+1 : 0;
					InBegFrm=ptr->InBegFrm+tmp;
					NumF=intr[j]-tmp+1;
					CONS_MESS1("InBegFrm=%d ,  NumF=%d",InBegFrm,NumF);
					//--ищем близкие линии или генерим новые для границ колонки--
					bound_frm(&frm[InBegFrm],NumF-1,&b);
					del=begI[j]-(j ? endI[j-1] : 0);
					if(order == HOR)
					{
						CONS_MESS1("beg------------------4");
						CONS_MESS1("order == HOR");
						minz=MIN(ThresX,del);
						if((left= !j ? ptr->Rect.left :
							AddLine1(&LineVK,&nV,&nVmax,(int16_t)endI[j-1],minz)) < 0)
						{
							CONS_MESS9("819       left-100");
							return left-100;
						}
						if((right= j==kcol ? ptr->Rect.right :
						AddLine1(&LineVK,&nV,&nVmax,begI[j],minz)) < 0)
						{
							CONS_MESS9("826       right-200");
						 	return right-200;
						}
					//top=ptr->Rect.top; bottom=ptr->Rect.bottom;
					//!!!для разновысоких колонок можно уточнять и верхи и низы!!!
					//но тогда придется рассчитать рамки новых колонок
						del1=b.down-b.up;
						minz=MIN(ThresY,del1);
						if((top=AddLine1(&LineHK,&nH,&nHmax,b.up,minz)) < 0)
						{
							CONS_MESS9("836       top-100");
							return top-100;
						}
						if((bottom=AddLine1(&LineHK,&nH,&nHmax,b.down,minz)) < 0)
						{
							CONS_MESS9("842       bottom-100");
							return bottom-100;
						}
						CONS_MESS1("end------------------4");
					}
					else
					{
						CONS_MESS1("beg------------------5");
						CONS_MESS1("order == VER");
						minz=MIN(ThresY,del);
						if((top= !j ? ptr->Rect.top :
						AddLine1(&LineHK,&nH,&nHmax,endI[j-1],minz)) < 0)
						{
							CONS_MESS9("858       top-300");
							return top-300;
						}
						if((bottom= j==kcol ? ptr->Rect.bottom :
						AddLine1(&LineHK,&nH,&nHmax,begI[j],minz)) < 0)
						{
							CONS_MESS9("865       bottom-400");
							return bottom-400;
						}
					//left=ptr->Rect.left; right=ptr->Rect.right;
					//!!!для разношироких колонок можно уточнять и боковые линии!!!
						del1=b.right-b.left;
						minz=MIN(ThresX,del1);
						if((left=AddLine1(&LineVK,&nV,&nVmax,b.left,minz)) < 0)
						{
							CONS_MESS9("874       left-100");
							return left-100;
						}
						if((right=AddLine1(&LineVK,&nV,&nVmax,b.right,minz)) < 0)
						{
							CONS_MESS9("880       right-200");
							return right-200;
						}
						CONS_MESS1("end------------------5");
					}

				//--разрешение противоречия равенства индексов начальной и конечной--
				//  границ для малоразмерных колонок
					if(left == right) //узкая колонка
					{
						CONS_MESS1("left == right <<узкая колонка>>");
						if(j != kcol || !fl)
						{
					  //minz=fl ? (order==HOR ? begI[j]:b.right) : bnd.right;
							minz=fl ? (order==HOR ? begI[j]:b.right) : b.right;
							if((right=AddLine1(&LineVK,&nV,&nVmax,minz,-1)) < 0)
	                		{
								CONS_MESS9("917       right-200");
								return right-200;
							}
						}
						else
						{
				//		 minz=fl ? (order==HOR ? endI[j-1]:b.left) : bnd.right;
	     					minz=fl ? (order==HOR ? endI[j-1]:b.left) : b.right;
							if((left=AddLine1(&LineVK,&nV,&nVmax,minz,-1)) < 0)
	                		{
								CONS_MESS9("927       right-200");
								return right-200;
							}
						}
					}
					if(top == bottom) //низкая колонка
					{
						CONS_MESS1("top == bottom <<низкая колонка>>");
						if(j != kcol || !fl)
						{
		//				 minz=fl ? (order==VER ? begI[j]:b.down) : bnd.down;
							minz=fl ? (order==VER ? begI[j]:b.down) : b.down;
							if((bottom=AddLine1(&LineHK,&nH,&nHmax,minz,-1)) < 0)
		              		{
								CONS_MESS9("942       bottom-100");
								return bottom-100;
							}
						}
						else
						{
							minz=order==VER ? endI[j-1]:b.up;
							if((top=AddLine1(&LineHK,&nH,&nHmax,minz,-1)) < 0)
		             		{
								CONS_MESS9("952       top-100");
								return top-100;
							}
						}
					}

 					if(LineVK[left].beg-30 >= LineVK[right].beg ||
								LineHK[top].beg -10 >= LineHK[bottom].beg)  //LineHK[top].beg >= LineHK[bottom].beg)
						LDPUMA_Console(" Неправильные координаты фрагмента!!! ");


					if(LineVK[left].beg-30 >= LineVK[right].beg ||
							 LineHK[top].beg -10 >  LineHK[bottom].beg)  //LineHK[top].beg >= LineHK[bottom].beg)
					{
						CONS_MESS9("961 return -6");
						return -6;
					}

					//заполняем поля нового узла колонки
					//!!! поля left,right,top,bottom имеют смысл индексов линий разграфки
					FillFieldKNOTT1(Child,left,right,top,bottom,
						InBegFrm,NumF,IN_NO,UNSORT,CELL,TRUE,ALG_BEG,ALG_BEG,NULL);
					if(fl)
					{
						if(++k_colnt1 >= NumMax)
						{
							int16_t old=NumMax;
							NumMax=(int16_t)(NumMax*1.5);
							if(Realloc2(&colt,&colnt,&colnt1,&begI,&endI,&intr,old,NumMax) == NOT_ALLOC)
		                	{
								CONS_MESS9("976    NOT_ALLOC");
								return NOT_ALLOC;
							}
						}
						colnt1[k_colnt1]=Child;//Запоминаем очередную нетерминал. колонку
					}
         			PrevChild=Child;
					CONS_MESS1("end------------------3");
				}
				CONS_MESS1("end------------------2");
			}
			else /*Обнаружена терминал. колонка*/
			{
				CONS_MESS1("Обнаружена терминал. колонка");
				CONS_MESS1("beg------------------2.1");
				if(++k_colt >= NumMax)
				{
					int16_t old=NumMax;
					NumMax=(int16_t)(NumMax*1.5);
					if(Realloc2(&colt,&colnt,&colnt1,&begI,&endI,&intr,old,NumMax)
					== NOT_ALLOC)
					{
						CONS_MESS9("1012    NOT_ALLOC");
						return NOT_ALLOC;
					 }
				}
				colt[k_colt]=ptr;
				//заполняем поля очередного терминал. узла
				ptr->OrderChild=TERM;
				colt[k_colt]->InColA=(int)frm[left]->start_pos;
				CONS_MESS1("end------------------2.1");
			}
			CONS_MESS1("end------------------1");
		}
		//переписываем узлы следующего уровня в текущий уровень для след.итерации
		do0(i,0,k_colnt1) colnt[i]=colnt1[i];
		k_colnt=k_colnt1;
		fl_beg=0;
		order = order==HOR ? VER:HOR;//меняем порядок на ортогональный для след.итерации
		if(flTerm == TRUE)
			break;
	} //end of while
	free(colt);
	free(colnt);
	free(colnt1);
	free(intr);
	free(begI);
	free(endI);
	tmp=k_colt;

	//сортируем линии разграфки и перенумеруем их в узлах дерева
	if((fl=SortHorLine1(LineHK,nH,LineVK,nV,Tree.Root,&colt,&k_colt,frm)))
    {
		CONS_MESS9("1047    fl-260");
		return fl-260;
	}
	if(tmp != k_colt)
	{
		CONS_MESS9("1053    fl-11");
		return fl-11;
	}
#ifdef alDebug
	pTheGeomStep = pTheGeomStep2;
	for(i=0; i <= k_colt; ++i)
    ImageKnot1(colt[i],LineVK,LineHK,14,(int16_t)0xFFFF,_GBORDER,(int16_t)-1,frm,NumFrm,nV,nH);

	pTheGeomStep = pTheGeomStep1;
	FlagGraphic1=1;
	Graphic1Color=0;
	fl=ImageTree1(Tree.Root,LineVK,LineHK,frm,NumFrm,nV,nH);
	FlagGraphic1=0;
#endif
	//заносим в выходную "деревянную" структуру дерево и его причандалы
	Inf->Tree=Tree;           //дерево
	Inf->StatCell=StatCell;   //
	Inf->LineVK=LineVK;       //V-разграфка дерева LineVK[nV]
	Inf->nV=nV;               //
	Inf->LineHK=LineHK;       //H-разграфка дерева LineHK[nH]
	Inf->nH=nH;               //
	Inf->ColT=colt;           //терминал. колонки дерева colt[NumT]
	Inf->NumT=k_colt;         //
	if(!(Inf->bnd_col=(BOUND*)malloc((k_colt+1)*sizeof(BOUND))))
	{
		CONS_MESS9("1076    NOT_ALLOC");
		return NOT_ALLOC;
	}

	for(i=0; i <= k_colt; ++i)
	{
		int16_t in;

		in=colt[i]->InColA;
		colt[i]->AllowOCR=1;
		Inf->bnd_col[i].left = LineVK[colt[i]->Rect.left].beg;
		Inf->bnd_col[i].right= LineVK[colt[i]->Rect.right].beg;
		Inf->bnd_col[i].up   = LineHK[colt[i]->Rect.top].beg;
		Inf->bnd_col[i].down = LineHK[colt[i]->Rect.bottom].beg;
	}
	CONS_MESS1("==  !!!! end  CreateTreePlainTxt1   == ");
	CONS_MESS9("1103    return 0; ");
	free((KNOT**)StatCell);

	return 0;
}

//Return:
// >= 0 - среди существующих линий найдена близкая в смысле Thres (ее индекс)
//        или сгенерирована новая (ее индекс)
// < 0 - ERR
//Вход:               Coor - x-коор-та для V-линии или y-коор-та для H-линии
//										Thres - порог грубости отождествления линий
//Обменные параметры: Line1[nCurr] - система линий
//                    nMax - максимальное число линий
//==
int16_t AddLine1(LINE_KNOT **Line1,int16_t *nCurr,int16_t *nMax,int16_t Coor,int16_t Thres)
//==
{
	int16_t i,fl=-1,n; LINE_KNOT *Line=*Line1;
	for(i=0; i < *nCurr; ++i)
		if(abs(Line[i].beg-Coor) < Thres)
			return i;

	n=*nCurr;//Add new Line
	if(n >= *nMax) //обработка переполнение массива линий
	{
		*nMax=(int16_t)(*nMax*1.5);
		if((Line=(LINE_KNOT*)realloc_m(Line,n*sizeof(LINE_KNOT),*nMax*sizeof(LINE_KNOT))
		)==NULL)
			return NOT_ALLOC;
		*Line1=Line;
	}
	Line[n].beg=Coor;
	Line[n].Thick=0;
	Line[(*nCurr)++].TypeLine=HIDDEN_LINE;
	return n;
}

/*Классификация рамок по колонкам после устранения перекосов рекурсивным
делением ортогональных профилей
Вход: frm[k_frm] - рамки компонент (фрагментов);
      bnd - габариты текущего узла;
      ave_x,ave_y - среднее габаритов рамки компоненты;
			NumMax - максимал.число колонок
Выход:frm[k_frm]  - рамки переупорядочены поколонно, если колонки обнаружены;
      intr1[kcol] - правые границы найденных колонок в массиве frm,
      begI,endI   - геометрические границы межколон. интервалов
Return:
   0 - если колонки не обнаружены,
   kcol, если обнаружены,
   < 0 - ERR*/

//==
int16_t SearchColHist1(FRAME **frm,int16_t k_frm,BOUND *bnd,int16_t ave_x,int16_t ave_y,
    int16_t reg,int16_t *k_int,int16_t **intr1,int16_t **begI,int16_t **endI,int16_t *NumMax)
//==
{
	int16_t ave_dir,ave_ort,i,kcol,x,in,kf,fl,ki,MaxOld=*NumMax;
	int k_bloc;
	int16_t *intr=*intr1;
	KNOT4 *Free,**knot,**beg=(KNOT4**)malloc(*NumMax*sizeof(PTR)),*ptr;

	CONS_MESS3("===beg    SearchColHist1   ===");

	if(reg==HOR)
	{
		ave_dir=ave_x;ave_ort=ave_y;
		CONS_MESS3("reg==HOR,ave_dir=%d ; ave_ort=%d",ave_dir,ave_ort);
	}
	else
	{
		ave_dir=ave_y;ave_ort=ave_x;
		CONS_MESS3("reg==VER,ave_dir=%d ; ave_ort=%d",ave_dir,ave_ort);
	}

	if((fl=SearchInterval1(frm,k_frm,begI,endI,k_int,bnd,ave_dir,ave_ort,reg,NumMax))
      <= 0)
	{
		CONS_MESS3("!!!!! SearchInterval1 return<0");
		free(beg);
		return fl;
	}
	if(MaxOld != *NumMax)
	{
		if((intr=(int16_t*)realloc_m(intr,MaxOld*sizeof(int16_t),*NumMax*sizeof(int16_t)))==NULL ||
		(beg=(KNOT4**)realloc_m(beg,MaxOld*sizeof(PTR),*NumMax*sizeof(PTR)))==NULL)
			return NOT_ALLOC;
	}
	kcol=*k_int+1;
	CONS_MESS3("kcol=%d",kcol);

	do0(i,0,kcol) beg[i]=NULL;

	for(i=0; i < kcol; ++i) intr[i]=((*begI)[i]+(*endI)[i])>>1;

	k_bloc=-1;

	if((fl=init_lst((KNOT***)&knot,&k_bloc,k_frm+2,(KNOT**)&Free,
      sizeof(KNOT4)))!=0)
		return -fl-50;

	CONS_MESS3("k_frm=%d",k_frm);
	do0(i,0,k_frm) /*занесение рамок в списки колонок*/
	{
		x=(reg==HOR) ? frm[i]->left: frm[i]->up;
		if(kcol < 2)
			in=(x < intr[0]) ? 0 : 1;
		else
			in=search_int((int*)intr,*k_int,x);
		ptr=(KNOT4*)inc_lst((KNOT**)&beg[in],(KNOT**)&Free);
		ptr->f=frm[i];
	}
	kf=-1;
	ki=-1;
	do0(i,0,kcol)
	{
		ptr=beg[i];
		if(ptr==NULL)
			continue;
		while(ptr!=NULL)
		{
			frm[++kf]=ptr->f; ptr=ptr->next;
		}
		intr[++ki]=kf;
	}
	free_lst((KNOT**)knot,k_bloc);
	if(kf != k_frm)
	{
		CONS_MESS3("!!!!!!SearchColHist1: kf != k_frm;return -7");
		return -7;
	}
	*k_int=ki;
	free(beg);
	CONS_MESS3("===SearchColHist1: normal end; kcol=%d===",kcol);
	return kcol;
}


/*Поиск колонок по вертикали либо горизонтали - функция разбиения одного узла
	методом анализа профил
Вход: frm[k_frm] - рамки компонент (фрагментов);
      bnd - габариты текущего узла;
			ave_dir,ave_ort - среднее размера буквы-компоненты в прямом и перпендикулярном напр.
			NumMax - максимал.число колонок
      reg - режим колонизации (HOR - горизонтально-упорядоченные колонки,
                               VER - вертикально --//--).
Выход:frm[k_frm]  - рамки переупорядочены поколонно, если колонки обнаружены;
      intr1[kcol] - правые границы найденных колонок в массиве frm,
      beg1,end1   - [k_int1] геометрические границы межколон. интервалов
Выход:intr[k_int1]-середины найденных межколонных интервалов
reg=HOR - ищем горизонт. упорядоченные колонки,VER - вертикально-упорядоченные
Return:
  0 - разбиение не произошло,
  1 - разбиение произошло,
  < 0 - ERR*/

//===
int16_t SearchInterval1(FRAME **frm,int16_t k_frm,int16_t **beg1,int16_t **end1,
    int16_t *k_int1,BOUND *bnd,int16_t ave_dir,int16_t ave_ort,int16_t reg,int16_t *NumMax)
//===
{
	int16_t k_int,pos,min_col,min_int,maxh,minh,midh,kstr,len,len_group,i,j;
	int16_t beg_int,end_int,sumh,ave_h,mi,ma,Home,Fin;
	int16_t *his,*his_first_group,*his_second_group,last_real_line,
		    *Beg=*beg1,*End=*end1,tmp_pos;

	//--Расчет параметров одномерной колонизации--
	min_col=1;//миним. длина колонки в пикселах
	min_int=ave_dir*2;//миним. длина интервала в пикселах
	maxh   =0;//Макс.допустимая высота гистограммы в интервале-критерий обнаруж.начала интервала
	midh   =0;//Макс.допустимая средняя высота гист-мы в интервале
	minh   =1;//Мин.допустимая высота гистограммы в колонке-критерий обнаруж.начала колонки
            //после настоящего интервала и с левого края листа

	if(reg==HOR)
		min_int=5;/*ave_dir * 1.5*/;  //~~~

	CONS_MESS2("=== begin SearchInterval1===");
	CONS_MESS2("min_int= %d",min_int);

	if(reg==HOR)
	{
		Home=bnd->left; Fin=bnd->right;
		CONS_MESS2("reg==HOR; Home=%d; Fin=%d;",Home,Fin);
	}
	else
  	{
		Home=bnd->up; 	Fin=bnd->down;
    	len_group = bnd->right-bnd->left+2;
		CONS_MESS2("reg==VER; Home=%d; Fin=%d;",Home,Fin);
	}

	//Оценка числа строк(HOR)листа /ширины листа в символах(VER)
	kstr = (int16_t) (reg==HOR ? (bnd->down-bnd->up)/(2*ave_ort) :
                   (bnd->right-bnd->left)/(1.2*ave_ort));
  //--Вычисление профиля-гистограммы числа компонент, проецирующихся в данный пиксел--
	len=Fin-Home+2;//длина гистограммы
	CONS_MESS2("kstr=%d  len_hist=%d",kstr,len);
	if( (his=(int16_t*)malloc(len*sizeof(int16_t))) == NULL)
		return NOT_ALLOC;
	memset(his,0,len*sizeof(int16_t));
	--len;

	if(reg==VER)
	{
		if(len_group<0)
		{
   			CONS_MESS21("begin len_group=%d ",len_group);
			len_group=0;
		}
		if((his_first_group=(int16_t*)malloc((len_group+4)*sizeof(int16_t))) == NULL)
			return NOT_ALLOC;
		if((his_second_group=(int16_t*)malloc((len_group+4)*sizeof(int16_t))) == NULL)
			return NOT_ALLOC;
 		last_real_line=0;
		--len_group;
	}

	CONS_MESS2("k_frm=%d ",k_frm+1);
#ifdef alDebug
	{	pTheGeomTemp->clear();
		tagRECT rct;
		SetRect(&rct,bnd->left,bnd->up,bnd->right,bnd->down);
		pTheGeomTemp->push_back(rct);
	}
#endif

	do0(i,0,k_frm)
	{
	#ifdef alDebug
	{
		tagRECT rct;
		SetRect(&rct,frm[i]->left,frm[i]->up,frm[i]->right,frm[i]->down);
		pTheGeomTemp->push_back(rct);
	}
	#endif

		if(reg==HOR)
		{
			mi=frm[i]->left-Home; ma=frm[i]->right-Home;
		    CONS_MESS2("reg==HOR: frm[i]->left=%d,frm[i]->right=%d, mi=%d, ma=%d",
			                     frm[i]->left,frm[i]->right,mi,ma);
		}
		else
		{
			mi=frm[i]->up-Home;
			ma=frm[i]->down-Home;
		    CONS_MESS2("reg==VER: frm[i]->up=%d,frm[i]->down=%d,mi=%d,ma=%d",
			                     frm[i]->up,frm[i]->down,mi,ma);
		}

		if(mi < 0 || ma > len)
		{
		 	CONS_MESS2("!!!!!! mi < 0 || ma > len return -6");
			return -6;
		}

		do0(j,mi,ma) ++his[j];
	}
#ifdef alDebug
	*CountRect = pTheGeomTemp->size();
	CONS_MESS2("---Поиск межколон. интервалов---");
	if(det0) MyDrawForDebug();
#endif
	//--Поиск межколон. интервалов--
	k_int=-1; pos=-1;
	while((int16_t)his[++pos] < minh) /*поиск I позиции текста*/
    {
		if(pos>=len)
	    {
			if(reg==VER)
			{
				free(his_first_group);
				free(his_second_group);
			}
			free(his);
			*k_int1=-1;
			CONS_MESS2("===колонки не обнаружены===");
			return 0;
		} /*колонки не обнаружены*/
	}

	do
	{ /*ищем I позицию интервала*/
		while(pos < len-min_int-min_col && (int16_t)his[++pos] > maxh);
		if(pos >= len-min_int-min_col)
	    {
			CONS_MESS2("!!!!!! pos >= len-min_int-min_col");
			break;
		}
		beg_int=pos;
    /*ищем последовательность не менее чем min_int высотой < maxh*/
		while((int16_t)his[++pos] <= maxh)
			if(pos >= len || pos-beg_int >= min_int)
				break;
		if(pos - beg_int < min_int &&
		Check_IsItFalseHorLine(last_real_line,reg,frm,his,pos,len,maxh,min_int+min_col,
			                        len_group,his_first_group,his_second_group,bnd,k_frm))
			 continue;
		if(pos >= len)
			break;
		CONS_MESS2("beg_int=%d; pos=%d",beg_int,pos);
		tmp_pos=pos;
		sumh=0;
		do0(i,beg_int,pos)   sumh+=his[i];
		ave_h=sumh/(pos-beg_int+1);
		CONS_MESS2("ave_h=%d",ave_h);
		if(ave_h <= midh)/*интервал найден, ищем его конец*/
		{
			while((int16_t)his[++pos] <= maxh)
			{
				sumh+=his[pos];
				ave_h=sumh/(pos-beg_int+1);
				if(ave_h > midh || pos >= len-min_col)
					break; /*Обнаружен конец интервала*/
			}
			end_int=pos;

			CONS_MESS2("end_int=%d",end_int);
		#ifdef alDebug
			if(reg==VER)
			{
				tagRECT rct;
				SetRect(&rct,bnd->left,tmp_pos+Home,bnd->right,tmp_pos+Home);
				pTheGeomTemp->push_back(rct);//~
			}
			else
			{
				tagRECT rct;
				SetRect(&rct,tmp_pos+Home,bnd->up,tmp_pos+Home,bnd->down);
				pTheGeomTemp->push_back(rct);
			}
			if(det0) MyDrawForDebug();
		#endif

			if(pos < len-min_col) //не тривиальный интервал - т.е. еще не правая граница
			{
				Beg[++k_int]=beg_int+Home;
				End[k_int]=end_int+Home;
        //Фиксируем интервал лишь в случае достаточной длины колонки
				if((k_int > 0 && Beg[k_int] - End[k_int-1] < min_col) ||
				(Beg[k_int] - Home < min_col))
				{
					--k_int;
					CONS_MESS2(" колонка узкая,интервал не принимается! ");
				    continue;
				}
				last_real_line=end_int;
				if(k_int > *NumMax-2)
				{
					int16_t MaxOld=*NumMax;
					*NumMax=(int16_t)(*NumMax*1.5);
					if((Beg=(int16_t*)realloc_m(Beg,MaxOld*sizeof(int16_t),*NumMax*sizeof(int16_t)))==NULL ||
					(End=(int16_t*)realloc_m(End,MaxOld*sizeof(int16_t),*NumMax*sizeof(int16_t)))==NULL)
					{
						free(his);
						if(reg==VER)
						{
							free(his_first_group);
							free(his_second_group);
						}
						return -5;
					 }
				}
			}
		}
		pos+=min_col; /*для попадания внутрь колонки,если end_pos - внутри интервала*/
	} while(pos < len-min_int-min_col);
	free(his);
	if(reg==VER)
	{
		free(his_first_group);
		free(his_second_group);
	}

	*beg1=Beg; *end1=End; *k_int1=k_int;
	CONS_MESS2("k_int=%d",k_int);
	CONS_MESS2("=== end SearchInterval1===");
	return k_int >= 0 ? 1 : 0;
}


//////////////        Check_IsItFalseHorLine
////
int16_t Check_IsItFalseHorLine(int16_t last_real_line,int16_t reg,FRAME **frm,int16_t *his,int16_t pos,
														 int16_t len,int16_t maxh,int16_t sum,int16_t len_group,
														 int16_t *his_first_group,
														 int16_t *his_second_group,BOUND *bnd,int16_t k_frm)
{
	int16_t Home,Fin,old_pos,i,j,mi,ma,beg_white_int,end_white_int,k_frm_first,k_frm_second;
	std::vector<tagRECT>  First_Group;
	std::vector<tagRECT>  Second_Group;

	if(reg==HOR || len_group<=0) goto end1;

	CONS_MESS2("===begin Check_IsItFalseHorLine===");
	CONS_MESS2("pos=%d len=%d last_real_line=%d",pos,len,last_real_line);

	if(last_real_line>0) --last_real_line;
	Home=bnd->left;
	Fin =bnd->right;
///////////////////        First_Group             /////////////////////////////////////
	do0(i,0,k_frm)
	{
		if(frm[i]->up >= last_real_line + bnd->up && frm[i]->down <= pos +  bnd->up )
		{
			tagRECT rct;
			SetRect(&rct,frm[i]->left,frm[i]->up,frm[i]->right,frm[i]->down);
			First_Group.push_back(rct);
		}
	}
	k_frm_first=First_Group.size()-1;
	CONS_MESS2(" new_count_frm-first=%d ",k_frm_first+1);

	memset(his_first_group,0,(len_group+1)*sizeof(int16_t));
	do0(i,0,k_frm_first)
	{
		mi=First_Group[i].left  - Home;
		ma=First_Group[i].right - Home;
		if(mi < 0 || ma > len_group)
		{
			CONS_MESS2("!!!!!! mi < 0 || ma > len First_Group ");
			goto end1;
		}

		do0(j,mi,ma) ++his_first_group[j];
	}

////////////////////////// Second_Group   ////////////////////////////////////////////////
	old_pos=pos;
	while(pos <= len  && (int16_t)his[++pos] > maxh);
	CONS_MESS2("Second_Group new pos = %d",pos);
	do0(i,0,k_frm)
	{
		if(frm[i]->up >= old_pos + bnd->up && frm[i]->down <= pos +  bnd->up )
		{
			tagRECT rct;
			SetRect(&rct,frm[i]->left,frm[i]->up,frm[i]->right,frm[i]->down);
			Second_Group.push_back(rct);
		}
	}

	k_frm_second=Second_Group.size()-1;
	CONS_MESS2("Second_Group new_count_frm=%d ",k_frm_second+1);

	memset(his_second_group,0,(len_group+1)*sizeof(int16_t));
	do0(i,0,k_frm_second)
	{
		mi=Second_Group[i].left -Home;
		ma=Second_Group[i].right-Home;
		if(mi < 0 || ma > len_group)
		{
		 	CONS_MESS2("!!!!!! mi < 0 || ma > len Second_Group");
			goto end1;
		}
		do0(j,mi,ma) ++his_second_group[j];
	}

/////////  Compary white intervals //////////////////////////////
	do0(i,0,len_group)
	{
		while(i<= len_group && (int16_t)his_first_group[++i] <= maxh);
		while(i<= len_group && (int16_t)his_first_group[++i] >  maxh);
		if(i>=len_group)
			break;

		while(i<= len_group && (int16_t)his_first_group[++i] > maxh);
		beg_white_int=i;
		CONS_MESS2("First Group beg_white_int=%d ",i);
		while(i<= len_group && (int16_t)his_first_group[++i] <= maxh);
		end_white_int=i;
		if(end_white_int>=len_group)
			break;
		CONS_MESS2("First Group end_white_int=%d ",i);

		if(beg_white_int==end_white_int)
		{
			CONS_MESS2("beg_white_int==end_white_int i=%d ",i);
			goto end1;
		}
		if(!check_white_int(beg_white_int,end_white_int,maxh,his_second_group))
		{
			CONS_MESS2("===Can't find white interval->First Group===");
			goto end0;
		}
	}

	do0(i,0,len_group)
	{
		while(i<= len_group && (int16_t)his_second_group[++i] <= maxh);
		while(i<= len_group && (int16_t)his_second_group[++i] >  maxh);
		if(i>=len_group)
			break;

		while(i<= len_group && (int16_t)his_second_group[++i] > maxh);
		beg_white_int=i;
		CONS_MESS2("Second Group beg_white_int=%d ",i);

		while(i<= len_group && (int16_t)his_second_group[++i] <= maxh);
		end_white_int=i;
		if(end_white_int>=len_group)
			break;
		CONS_MESS2("Second Group end_white_int=%d ",i);

		if(beg_white_int==end_white_int)
		{
			CONS_MESS2("Second Group beg_white_int==end_white_int i=%d ",i);
			goto end1;
		}
		if(!check_white_int(beg_white_int,end_white_int,maxh,his_first_group))
		{
			CONS_MESS2("===Second Group Can't find white interval===");
			goto end0;
		}
	}

end1:
//	First_Group.RemoveAll( );
//	Second_Group.RemoveAll( );
	return 1;
end0:
//	First_Group.RemoveAll( );
//	Second_Group.RemoveAll( );
	return 0;
}

int16_t check_white_int(int16_t beg_white_int,int16_t end_white_int,int16_t maxh,
											int16_t *his_second_group)
{
 int16_t i,count_white_picsels=0;

 do0(i,beg_white_int,end_white_int)
 {
  if((int16_t)his_second_group[++i] <= maxh) ++count_white_picsels;
 }
 if (count_white_picsels>=2)
	 return 1;
 else
	 return 0;
}


/*Добавить узел в дерево после дочернего эл-та after узла-родителя up:
если after=NULL, добавляем в начало списка дочерей узла up,
если up=NULL, создаем корень дерева,
up - узел-родитель по дереву,
after - узел-дочь, после кот. следует вставить новый узел,
free - голова общего списка свобод. памяти дерева*/
//============return: указатель на новый включен. в дерево узел или NULL
KNOTT* IncKnot(KNOTT *up,KNOTT *after,KNOTT **free)
//==
{ KNOTT *New;
	if(up != NULL)
	{
		if(after == NULL)
			New=(KNOTT*)inc_lst((KNOT**)&up->down,(KNOT**)free);
	  else
			New=(KNOTT*)inc_after_lst((KNOT*)after,(KNOT**)&up->down,(KNOT**)free);
	  if(New == NULL)
			return New;
	}
	else
	{ New=NULL;
	  New=(KNOTT*)inc_lst((KNOT**)&New,(KNOT**)free);//корень
	}
	New->up=up; New->down=NULL;
	return New;
}
//===Заполнение полей узла KNOTT ортогонал. дерева TREE2
void FillFieldKNOTT1(KNOTT *ptr,int16_t Left,int16_t Right,int16_t Top,int16_t Bottom,
  int16_t InBegFrm,int16_t NumFrm,int16_t InColA,
  uint OrderChild,uint Type,uint AllowOCR,uint JustH,uint JustV,char *Name)
//==
{
	ptr->Rect.top=Top;          //индекс линии - верхней границы
	ptr->Rect.bottom=Bottom;    //--/-- нижней
	ptr->Rect.left=Left;        //--/-- левой
	ptr->Rect.right=Right;      //--/-- правой
	ptr->InBegFrm=InBegFrm;     //индекс начал. фрагмента колонки
	ptr->NumFrm=NumFrm;         //число фрагментов колонки (фрагменты одной колонки идут в массиве frm подряд)
	ptr->InColA=InColA;         //индекс первого фрагмента данной колонки
	ptr->OrderChild=OrderChild; //порядок дочерей узла
	ptr->Type=Type;             //
	ptr->AllowOCR=AllowOCR;     //
	ptr->JustH=JustH;           //
	ptr->JustV=JustV;           //
	ptr->Name=Name;             //
	ptr->RefOrt=ptr->RefH=NULL; //
}
//==
int16_t compINDEX_SORT1(INDEX_SORT *a,INDEX_SORT *b)
//==
	{return (a->value >= b->value ? 1:-1);}
//==
int16_t compLINE_KNOT1(LINE_KNOT *a,LINE_KNOT *b)
//==
	{return (a->beg >= b->beg ? 1:-1);}
//сортировка H-линий дерева по вертикали
//и попутное заполнение массива терминал. V-колонок (k_colt1 - индекс)
//если V-линии не упорядочены, они также сортируютс
//==
int16_t SortHorLine1(LINE_KNOT *LineHK,int16_t NumH,LINE_KNOT *LineVK,int16_t NumV,
    KNOTT *Root,KNOTT ***colt1,int16_t *k_colt1,FRAME **frm)
//==
{
	INDEX_SORT *Ind=(INDEX_SORT *)malloc(MAX(NumH,NumV)*sizeof(INDEX_SORT));
  STAT_CELL StDefault;
  int16_t *Index=(int16_t *)malloc(NumH*sizeof(int16_t)),
        *IndexV=(int16_t *)malloc(NumV*sizeof(int16_t));
  int16_t i,DepthTree=20,flV;
  STACK St;
	KNOTT *Curr,**colt;
	int16_t k_colt;

	StDefault.dyLow=NORM_SCAN(20);
  //--сортировка H-линий--
  for(i=0; i < NumH; ++i) {Ind[i].ind=i;Ind[i].value=LineHK[i].beg;}
  u4sort(Ind,NumH,sizeof(INDEX_SORT),(COMP_FUN)compINDEX_SORT1);
  u4sort(LineHK,NumH,sizeof(LINE_KNOT),(COMP_FUN)compLINE_KNOT1);
  for(i=0; i < NumH; ++i) Index[Ind[i].ind]=i;
  //--сортировка V-линий--
  flV=0;
  for(i=0; i < NumV; ++i)
  { Ind[i].ind=i;Ind[i].value=LineVK[i].beg;
    if(i && LineVK[i-1].beg > LineVK[i].beg) flV=1;
  }
  if(flV)//V-линии неупорядочены
  { u4sort(Ind,NumV,sizeof(INDEX_SORT),(COMP_FUN)compINDEX_SORT1);
    u4sort(LineVK,NumV,sizeof(LINE_KNOT),(COMP_FUN)compLINE_KNOT1);
  }
  for(i=0; i < NumV; ++i) IndexV[Ind[i].ind]=i;
  free(Ind);
  if(NewStack(DepthTree,&St)) //создать стек для обхода дерева
		return NOT_ALLOC;

	Curr=Root; k_colt=0;
  while(Curr != NULL)//I проход TREE-перенумерация H-линий и расчет числа TERM Cell
  {
		Curr->Rect.top=Index[Curr->Rect.top];
    Curr->Rect.bottom=Index[Curr->Rect.bottom];
    Curr->Rect.left=IndexV[Curr->Rect.left];
    Curr->Rect.right=IndexV[Curr->Rect.right];
    if(Curr->NumFrm > 0)
			++k_colt;
    Curr=NextKnot(Curr,&St); //перейти к следующему узлу
    if(OverflowStack(&St)) //проверка переполнения стека
			return NOT_ALLOC;
  }

	if((colt=(KNOTT**)malloc(k_colt*sizeof(KNOTT*))) == NULL)
		return NOT_ALLOC;
  ClearStack(&St);//Для повтор.прохода по дереву чистим стек
  Curr=Root;
	k_colt=-1;
  #define VER_ORDER_QUE_STR
  #ifdef VER_ORDER_QUE_STR
	  while(Curr != NULL)//II проход - заполнение массива TERM Cell
	  { if(Curr->down == NULL && Curr->AllowOCR && Curr->InBegFrm != IN_NO &&
	      Curr->NumFrm > 0)
				colt[++k_colt]=Curr;
	    Curr=NextKnot(Curr,&St);
	  }
  #else
	  while(Curr != NULL)
	  { if(Curr->InBegFrm == IN_NO && Curr->down == NULL) //Терм. H-узел
	    { while(CurrV)
	      { if(CurrV->down == NULL && CurrV->AllowOCR && CurrV->InBegFrm != IN_NO &&
	          CurrV->NumFrm > 0) colt[++k_colt]=CurrV;
	        CurrV=CurrV->RefH;
	      }
	    }
	    Curr=NextKnot(Curr,&St);
	  }
  #endif
  DelStack(&St);
  *colt1=colt; *k_colt1=k_colt;
  free(Index);
  free(IndexV);
  return 0;
}


#define MAX_STYLE 100
int16_t  K_PointInInch=72;
extern float Twips;
float HalfPoint;
static uint cr=13,lf=10;
#define FONT_OCR struct h_font_ocr
#define STYLE struct h_style
#define PARAG struct h_parag
STYLE { int16_t Alignment;
        int16_t IndentFirst,IndentLeft,IndentRight;
        int16_t SpaceBefore,SpaceAfter,SpaceBetwLine;
        int16_t Font,SizeFont;};
PARAG { int16_t NumStyle; //Индекс в табл. стилей
        int16_t BegStr, EndStr; //Гранич. строки абзаца
        SRECT bnd;
        int16_t MaxSize; };       //
COLH { SRECT bnd;/*рамка объединения*/};
FONT_OCR { char *FileBase; };

//==
int16_t CalcNumDau(KNOTT *Knot)
//==
{
	int16_t n=0;
	KNOTT *ptr;

	if(!Knot)
		ERR(1,errRtf);

	for(ptr=Knot->down; ptr; ptr=ptr->next)
		n++;
	return n;
}

//Return:
//0 - OK.
// Режим работы USE_FRAME_AND_COLUMN
//=========Разбиение страницы на сектора
//         Секторы на горизантальные колонки
//         Горизантальные колонки на вертикальные колонки
//         Определение терминальности вертикальных колонок
//  Запольнение RtfPage.
Bool PageTree( FILE *InFileName, CRtfPage* RtfPage, const char* OutFileName)
{
	int16_t  nc,ns,nw,nz,fl,i,i_ns1,i_nsb,i_nse,j,ih,iv,iv1,kp,kp1,kp2,
		      n_beg,dist_hor_col=240,dist_sec=360,flag_vse_term=1,OldNumCol;
	int16_t	 FlagBadColumn;
	const char   *err="PageTree";
	SRECT  bnd;
	int16_t  ***Colt,K_Sect,*K_Hor,**K_Ver_Flag_Term,**K_Ver_Add_On,**K_Ver_Offset,**K_Ver;
	COLH   **ColH,**ColH_New;
	INF_TREE Inf;
	FRAME **frm;
	Bool  FlagBadBad = FALSE;
	KNOTT *RootUdal=NULL;


	CRtfSector*             pRtfSector;
	CRtfHorizontalColumn*   pRtfHorizontalColumn;
	CRtfVerticalColumn*     pRtfVerticalColumn;
	CRtfFragment*           pRtfFragment;
	CRtfString*             pRtfString;
	CRtfWord*               pRtfWord;
	CRtfChar*               pRtfChar;

	SETUP_GENERATE_TREE setup;
	Inf.Tree.Root = (KNOTT*)malloc(sizeof(KNOTT));
	KNOTT *pRoot=Inf.Tree.Root,*ptr,*ptr1,*ptr2;
	RootUdal=pRoot;

	ArrFrm  = NULL;
	frm     = NULL;
	K_Hor=NULL;
	K_Ver=NULL;
	K_Ver_Flag_Term=NULL;
	K_Ver_Add_On=NULL;
	K_Ver_Offset=NULL;
	Colt=NULL;
	ColH=NULL;
	ColH_New=NULL;

	CONS_MESS23("Begin FileName=%s ",OutFileName);
	#ifdef alDebug
		FlagGraphic1=0;
	#endif

	//settings for GenFullTxtfromTree
	par_ful.AllowSpell=0;
	par_ful.KodNoRecogOut='~';
	Inf.TypeDoc=PLAIN;

	FlagOdinSectorOdnaColonka = TRUE;
	//--Инициализация системы:чтение internal-файла--
	#define TRUE_READ_ED   //Alik 17.04.97
	FlagBadColumn = 0;

	if(!OpenFullOutTiger(InFileName))
	{
#ifdef alDebug
		free((KNOT**)Inf.Tree.Root);
  		if(det20 || det23)
		{	ConsMess("Formatter End ");
			if(RtfWriteMode)
 			ConsMess("*************************************************************");
		}
#endif
		return TRUE;
	}
	CONS_MESS20("OpenFullOutTiger ");

	if( NumCol>=0 )
		fl=CalcStatTiger();
	else
		fl=1;

	setup.size_x=setup.size_y=SizeYGlob;
	OldNumCol = NumCol;
	++NumCol;
	// Форматирование текстовых фрагментов
	if(fl=GenerateTreeByFragm(RectFragm,NumCol,&setup,&frm,&Inf))
	{
		--NumCol;
		FlagBadBad=TRUE;
		CONS_MESS22("  NameFile-> %s ",OutFileName);
		goto BadReturn;
	}
	CONS_MESS22("GenerateTreeByFragm ");
	 //---объединяем результаты распознавания текстовых фрагментов в колонки---
	if( Inf.NumT )
		FlagOdinSectorOdnaColonka=FALSE;
	for(i=0; i <= Inf.NumT; ++i)
	{
		int16_t num,inCol,nc,ns,r,l,t,b,TotalNumStr,m;

		l     = Inf.bnd_col[i].left;
		r     = Inf.bnd_col[i].right;
		t     = Inf.bnd_col[i].up;
		b     = Inf.bnd_col[i].down;
		n_beg = Inf.ColT[i]->InBegFrm;
		num   = Inf.ColT[i]->NumFrm;

		Inf.ColT[i]->Type     = 1;
		Inf.ColT[i]->InBegFrm = (int)frm[n_beg]->start_pos;
		inCol                 = Inf.ColT[i]->InBegFrm;

 		if(num>1)
		{
			qsort((void *)(frm+n_beg),num,sizeof(FRAME *),Alik_sort_function);

			for(j=0; j < num-1; ++j)
			{
				if(frm[n_beg+j]->down > frm[n_beg+j+1]->up)
				{
					FlagOdinSectorOdnaColonka = FALSE;
					break;
				}
			}

			for(j=0; j < num-1; ++j)
			{
				if(frm[n_beg+j]->down-30 > frm[n_beg+j+1]->up)
				{
					Inf.ColT[i]->Type=0;
					Inf.ColT[i]->InBegFrm=n_beg;
					break;
				}
			}

			if(Inf.ColT[i]->Type) inCol = Inf.ColT[i]->InBegFrm=(int)frm[n_beg]->start_pos;
		}

		CONS_MESS6("beg=%d num=%d inCol=%d",n_beg,num,NumCol-inCol);
	#ifdef alDebug
		 if(det6 && num>1 && !Inf.ColT[i]->Type) {ConsMess(" Фрагмент не отсортирован !!! ");
		                                         ConsMess("********* end multiframe ********");}
	#endif
   //многофрагментная колонка => объединяем строки фрагментов
		if( num>1 && (Inf.ColT[i]->Type  || ( FlagOdinSectorOdnaColonka && !Inf.NumT ) ))
		{
			TotalNumStr=0;
			m=0;
			RectFragm[inCol].left=Inf.bnd_col[i].left;
			RectFragm[inCol].right=Inf.bnd_col[i].right;
			RectFragm[inCol].top=Inf.bnd_col[i].up;
			RectFragm[inCol].bottom=Inf.bnd_col[i].down;

			for(j=0; j < num; ++j)
			{
				nc=(int16_t)frm[n_beg+j]->start_pos;
				TotalNumStr += NumStr[nc]+1;
			}

			TITLE_STR    *tS;
			TITLE_WORD  **tW;
			ZN          ***Z;

			tS=(TITLE_STR*  )Submalloc(TotalNumStr*sizeof(TITLE_STR),&SubZn);
			tW=(TITLE_WORD**)Submalloc(TotalNumStr*sizeof(PTR),&SubZn);
			Z =(ZN***)       Submalloc(TotalNumStr*sizeof(PTR),&SubZn);
			if(!tS || !tW || !Z)
				return NOT_ALLOC;

			for(j=0; j<num; ++j)
			{
				nc = (int16_t)frm[n_beg+j]->start_pos;  //прибавить строки колонки nc к inCol
				if(j)
					TitleStr[nc][0].S_Attr=1;
			#ifdef alDebug
				if(dets && det6) ConsMess("nc=%d ",NumCol-nc);
			#endif
				for(ns=0; ns <= NumStr[nc]; ++ns)
				{
					tS[m] = TitleStr[nc][ns];
					tW[m] = TitleWord[nc][ns];
					Z[m]  = Zn[nc][ns];
					m++;
				}
				NumStr[nc]=-1; //для неосвобождения повторно их памяти или уплотнени
			}

			if(FlagOdinSectorOdnaColonka)
				inCol=0;
			TitleStr[inCol]  = tS;
			TitleWord[inCol] = tW;
			Zn[inCol]        = Z ;
			NumStr[inCol]    = TotalNumStr-1;
			CONS_MESS6("********* end multiframe ********");
		}
	}

 	pRoot=Inf.Tree.Root;
	Inf.StatCell=(STAT_CELL*)malloc(NumCol*sizeof(STAT_CELL));
	Inf.StatCell[0].HeiStr=42;//~т.к. из ED-файла приходит неверные значени
	Inf.MonoSpaceTrue     =22;//~
	Inf.k_str=NumStr;

	 //---Заполнение структуры колонок K_Sect,K_Hor,K_Ver,Colt,ColH---***********************
	k_lev=MaxLev-1;
	--NumCol;
	if(FlagOdinSectorOdnaColonka)
	{
		OldNumCol=NumCol;
		NumCol=0;
	}

 	//converter from Inf, где лежит дерево, в трехуровневую uint16_t-схему
	 //представления иерархии колонок:
	 // K_Sect
	 // 		K_Hor[K_Sect]
	 // 			K_Ver[K_Sect][K_Hor[K_Sect]]
	 // K_Ver_Flag_Term указывает свойства колонки :
	 // 0----простая колонка; 1----простая колонка c терм. фрагментами; 2----- сложная колонка
	{
		k_col[1]=CalcNumDau(pRoot)-1;
   /*****************     Начал. порядок - горизон. *********************************/
		if(pRoot->OrderChild == HOR)
		{
			CONS_MESS4("Начал. порядок - горизон.");
			CONS_MESS4("Количество колон =%d",k_col[1]+1);

			K_Sect=0;
			K_Hor=(int16_t*)malloc((K_Sect+1)*sizeof(int16_t));
			K_Hor[0]=k_col[1];
			K_Ver=(int16_t**)malloc((K_Sect+1)*sizeof(int16_t*));
			K_Ver_Flag_Term=(int16_t**)malloc((K_Sect+1)*sizeof(int16_t*));
			Colt=(int16_t***)malloc((K_Sect+1)*sizeof(int16_t**));
			if(K_Hor==NULL||K_Ver_Flag_Term==NULL||K_Ver==NULL||Colt==NULL)
				return NOT_ALLOC;
			if((K_Ver[0]=(int16_t*) malloc((K_Hor[0]+1)*sizeof(int16_t)))==NULL||
			   (K_Ver_Flag_Term[0]=(int16_t*) malloc((K_Hor[0]+1)*sizeof(int16_t)))==NULL||
				(Colt [0]=(int16_t**)malloc((K_Hor[0]+1)*sizeof(PTR)))==NULL)
				return NOT_ALLOC;

			for(ih=0,ptr=pRoot->down; ih <= K_Hor[0]; ++ih,ptr=ptr->next)
			{
				K_Ver_Flag_Term[0][ih]=0;
				iv=0;
				kp=CalcNumDau(ptr)-1;
				CONS_MESS4(" #Col=%d",ih+1);
				if(kp < 0) //Терминал.колонка
				{
					if(ptr->NumFrm>1 && !ptr->Type)
					{
						K_Ver[0][ih]=ptr->NumFrm-1;
						K_Ver_Flag_Term[0][ih]=2;
					}
					else
						K_Ver[0][ih]=0;

					if((Colt[0][ih]=(int16_t*)malloc((K_Ver[0][ih]+1)*sizeof(int16_t)))==NULL)
						return NOT_ALLOC;
					Get_all_term_fragms1( ptr,Colt[0][ih],&iv,NumCol,frm );
				#ifdef alDebug
					if(det4 && ptr->NumFrm>1 && !ptr->Type)
						ConsMess("Колонка сложной структуры (фреймы) ");
				 #endif
				}
				else
				{
					K_Ver[0][ih]=ptr->NumFrm-1;
					if((Colt[0][ih]=(int16_t*)malloc((K_Ver[0][ih]+1)*sizeof(int16_t)))==NULL)
						return NOT_ALLOC;
				#ifdef alDebug
					if(det4 && dets) ConsMess(" Выдельяем память для %d term fragm",ptr->NumFrm);
				#endif

					for(iv1=0,iv=0,ptr1=ptr->down; iv1 <= kp; ++iv1,ptr1=ptr1->next)
					{
       //Поиск терминальных фрагментов
						kp1=CalcNumDau(ptr1)-1;
						if(kp1 >= 0)
						{
							Get_all_term_fragms( ptr1,Colt[0][ih],&iv,NumCol,frm );
							K_Ver_Flag_Term[0][ih]=2;
						}
						else
						{
							if(ptr1->NumFrm>1 && !ptr1->Type)
							{
								K_Ver_Flag_Term[0][ih]=2;
					 			CONS_MESS4(">>> %d не отсортированных фрагмента",ptr1->NumFrm);

								i_nse=ptr1->InBegFrm+ptr1->NumFrm;
								for(i_nsb=ptr1->InBegFrm; i_nsb<i_nse; ++iv,++i_nsb)
								{
									Colt[0][ih][iv]=(int16_t)frm[i_nsb]->start_pos;
									CONS_MESS4(" #term=%d",NumCol+1-Colt[0][ih][iv]);
								}
							}
							else
							{
								if(!K_Ver_Flag_Term[0][ih])  K_Ver_Flag_Term[0][ih]=1;
									Colt[0][ih][iv]=ptr1->InBegFrm;
								CONS_MESS4(" #term=%d",NumCol+1-Colt[0][ih][iv]);
								iv++;
							}
						}
					}
					K_Ver[0][ih] = --iv;
				#ifdef alDebug
					if(det4 && dets) ConsMess("Кол-во терм. колонок=%d",K_Ver[0][ih]+1);
				 #endif
				}
			#ifdef alDebug
				if(det4)
				{
					if(!K_Ver_Flag_Term[0][ih])  ConsMess("Колонка простая");
 					else
					if(det4 && K_Ver_Flag_Term[0][ih]==1) ConsMess("Колонка простая и состоит из терм.фраг-тов");
					else  ConsMess("Колонка сложной структуры (фреймы) ");
				}
			#endif
			}
		}
/*****************     Начал. порядок - вертикал. *********************************/
		else
		if(pRoot->OrderChild == VER)
		{
		 	CONS_MESS4("Начал. порядок - вертикал.");
			CONS_MESS4("Количество секций =%d",k_col[1]+1);

			K_Sect=k_col[1];
			K_Hor=(int16_t*)malloc((K_Sect+1)*sizeof(int16_t));
			K_Ver=(int16_t**)malloc((K_Sect+1)*sizeof(int16_t*));
			K_Ver_Flag_Term=(int16_t**)malloc((K_Sect+1)*sizeof(int16_t*));
			Colt =(int16_t***)malloc((K_Sect+1)*sizeof(int16_t**));
			if(K_Hor==NULL||K_Ver_Flag_Term==NULL||K_Ver==NULL||Colt==NULL)
   				return NOT_ALLOC;
			//******  Цикл по секциям,внутри каждой - до 2-уровневой иерархии ***********
			for(i=0,ptr=pRoot->down; i <= K_Sect; ++i,ptr=ptr->next)
			{
				kp=CalcNumDau(ptr)-1;
				CONS_MESS4("***Секция #%d- из   %d   гориз. колонок ",i+1,kp<0?1:kp+1);

				//Секция - одна терминал. колонка
				if(kp < 0)
				{
					K_Hor[i]=0;
					K_Ver[i]=(int16_t*)malloc((K_Hor[i]+1)*sizeof(int16_t));
					K_Ver_Flag_Term[i]=(int16_t*)malloc((K_Hor[i]+1)*sizeof(int16_t));
					Colt[i] =(int16_t**)malloc((K_Hor[i]+1)*sizeof(int16_t*));
					if(K_Ver[i]==NULL||K_Ver_Flag_Term[i]==NULL||Colt[i]==NULL)
     					return NOT_ALLOC;

					if(ptr->NumFrm>1 && !ptr->Type)
					{
						K_Ver[i][0]=ptr->NumFrm-1;
						K_Ver_Flag_Term[i][0]=2;
					}
					else
					{
						K_Ver[i][0]=0;
						K_Ver_Flag_Term[i][0]=0;
					}

					if((Colt[i][0]=(int16_t*)malloc((K_Ver[i][0]+1)*sizeof(int16_t)))==NULL)
						return -3;

					if(ptr->NumFrm>1 && !ptr->Type)
					{
						CONS_MESS4(">>> %d не отсортированных фрагмента",ptr->NumFrm);

						i_nse=ptr->InBegFrm+ptr->NumFrm;
						for(i_ns1=0,i_nsb=ptr->InBegFrm; i_nsb<i_nse; ++i_ns1,++i_nsb)
						{
							Colt[i][0][i_ns1]=(int16_t)frm[i_nsb]->start_pos;
							CONS_MESS4(" #term=%d",NumCol+1-Colt[i][0][i_ns1]);
						}
						CONS_MESS4("Колонка сложной структуры (фреймы) ");
					}
					else
					{
						Colt[i][0][0]=ptr->InBegFrm;
						CONS_MESS4(" #term=%d",NumCol+1-Colt[i][0][0]);
					}
				}
	//Секция - из kp H-колонок
				else
				{
					K_Hor[i]=kp;
					K_Ver[i]=(int16_t*)malloc((K_Hor[i]+1)*sizeof(int16_t));
					K_Ver_Flag_Term[i]=(int16_t*)malloc((K_Hor[i]+1)*sizeof(int16_t));
					Colt[i] =(int16_t**)malloc((K_Hor[i]+1)*sizeof(int16_t*));
					if(K_Ver[i]==NULL||K_Ver_Flag_Term[i]==NULL||Colt[i]==NULL)
						return NOT_ALLOC;
					//************  цикл по H-дочерям секции ****************************
					for(ih=0,ptr1=ptr->down; ih <= kp; ++ih,ptr1=ptr1->next)
					{
						kp1=CalcNumDau(ptr1)-1;//Число дочерей H-col
						K_Ver_Flag_Term[i][ih]=0;
					 	CONS_MESS4(" #Col=%d",ih+1);

						if(kp1 < 0)            //Терм. H-col
						{
							if(ptr1->NumFrm>1 && !ptr1->Type)
							{
								K_Ver[i][ih]=ptr1->NumFrm-1;
								K_Ver_Flag_Term[i][ih]=2;
							}
							else
							{
								K_Ver[i][ih]=0;
								K_Ver_Flag_Term[i][ih]=0;
							}

							if((Colt[i][ih]=(int16_t*)malloc((K_Ver[i][ih]+1)*sizeof(int16_t)))==NULL)
								return -3;

							if(ptr1->NumFrm>1 && !ptr1->Type)
							{
							 	CONS_MESS4(">>> %d не отсортированных фрагмента",ptr1->NumFrm);

								i_nse=ptr1->InBegFrm+ptr1->NumFrm;
								for(i_ns1=0,i_nsb=ptr1->InBegFrm; i_nsb<i_nse; ++i_ns1,++i_nsb)
								{
									Colt[i][ih][i_ns1]=(int16_t)frm[i_nsb]->start_pos;
									CONS_MESS4(" #term=%d",NumCol+1-Colt[i][ih][i_ns1]);
								}
							}
							else
							{
								Colt[i][ih][0]=ptr1->InBegFrm;
								CONS_MESS4(" #term=%d",NumCol+1-Colt[i][ih][0]);
							}
						}
						else
						{
							K_Ver[i][ih]=ptr1->NumFrm-1;
							if((Colt[i][ih]=(int16_t*)malloc((K_Ver[i][ih]+1)*sizeof(int16_t)))==NULL)
								return -3;
						#ifdef alDebug
						 	if(det4 && dets) ConsMess(" Выдельяем память для %d term fragm",ptr1->NumFrm);
						#endif

							for(iv1=0,iv=0,ptr2=ptr1->down; iv1 <= kp1; ++iv1,ptr2=ptr2->next)
							{
        //Поиск терминальных фрагментов
								kp2=CalcNumDau(ptr2)-1;
								if(kp2 >= 0)
								{
									Get_all_term_fragms( ptr2,Colt[i][ih],&iv,NumCol,frm );
									K_Ver_Flag_Term[i][ih]=2;
								}
								else
								{
									if(ptr2->NumFrm>1 && !ptr2->Type)
									{
										K_Ver_Flag_Term[i][ih]=2;
										CONS_MESS4(">>> %d не отсортированных фрагмента",ptr2->NumFrm);

										i_nse=ptr2->InBegFrm+ptr2->NumFrm;
										for(i_nsb=ptr2->InBegFrm; i_nsb<i_nse; ++iv,++i_nsb)
										{
											Colt[i][ih][iv]=(int16_t)frm[i_nsb]->start_pos;
											CONS_MESS4(" #term=%d",NumCol+1-Colt[i][ih][iv]);

										}
									}
									else
									{
										if(!K_Ver_Flag_Term[i][ih])
											K_Ver_Flag_Term[i][ih]=1;
										Colt[i][ih][iv]=ptr2->InBegFrm;
										CONS_MESS4(" #term=%d",NumCol+1-Colt[i][ih][iv]);

										iv++;
									}
								}
							}
							K_Ver[i][ih] = --iv;
						#ifdef alDebug
							if(det4 && dets) ConsMess("Кол-во терм. колонок=%d",K_Ver[i][ih]+1);
						#endif

						}
					#ifdef alDebug
						if(det4){
						if(!K_Ver_Flag_Term[i][ih])  ConsMess("Колонка простая");
						else
						if(K_Ver_Flag_Term[i][ih]==1) ConsMess("Колонка простая и состоит из терм.фраг-тов");
						else  ConsMess("Колонка сложной структуры (фреймы) ");}
					#endif

					}
					//************ Конец цикла по H-дочерям секции ****************************
				}
			}
		}
		else
		{
			if(!NumCol) //090899 update
			{
				K_Sect=0;
				K_Hor=(int16_t*)malloc((K_Sect+1)*sizeof(int16_t));
				K_Hor[0]=0;
				K_Ver=(int16_t**)malloc((K_Sect+1)*sizeof(int16_t*));
				K_Ver_Flag_Term=(int16_t**)malloc((K_Sect+1)*sizeof(int16_t*));
				Colt=(int16_t***)malloc((K_Sect+1)*sizeof(int16_t**));
				if(K_Hor==NULL||K_Ver_Flag_Term==NULL||K_Ver==NULL||Colt==NULL)
					return NOT_ALLOC;
  				if((K_Ver[0]=(int16_t*) malloc((K_Hor[0]+1)*sizeof(int16_t)))==NULL||
					(K_Ver_Flag_Term[0]=(int16_t*) malloc((K_Hor[0]+1)*sizeof(int16_t)))==NULL||
					(Colt [0]=(int16_t**)malloc((K_Hor[0]+1)*sizeof(PTR)))==NULL)
	 			return NOT_ALLOC;
				K_Ver_Flag_Term[0][0]=0;
				K_Ver[0][0]=0;
				if((Colt[0][0]=(int16_t*)malloc((K_Ver[0][0]+1)*sizeof(int16_t)))==NULL)
					return NOT_ALLOC;
				Colt[0][0][0]=0;
			}
			else
			{
BadReturn:
				FlagBadColumn   = 1;
				K_Sect          = 0;
				K_Hor           = (int16_t*)malloc((K_Sect+1)*sizeof(int16_t));
				K_Hor[0]        = 0;
				K_Ver           = (int16_t**)malloc((K_Sect+1)*sizeof(int16_t*));
				K_Ver_Flag_Term = (int16_t**)malloc((K_Sect+1)*sizeof(int16_t*));
				Colt            = (int16_t***)malloc((K_Sect+1)*sizeof(int16_t**));

				if( K_Hor==NULL || K_Ver_Flag_Term==NULL || K_Ver==NULL || Colt==NULL )
					return NOT_ALLOC;

  				if(( K_Ver[0]           = (int16_t*) malloc((K_Hor[0]+1)*sizeof(int16_t)))==NULL  ||
						( K_Ver_Flag_Term[0] = (int16_t*) malloc((K_Hor[0]+1)*sizeof(int16_t)))==NULL  ||
						( Colt [0]           = (int16_t**)malloc((K_Hor[0]+1)*sizeof(PTR)))  ==NULL)
	 				return NOT_ALLOC;

				K_Ver_Flag_Term[0][0]=2;
				K_Ver[0][0]=NumCol;

				if((Colt[0][0]=(int16_t*)malloc((K_Ver[0][0]+1)*sizeof(int16_t)))==NULL)
					return NOT_ALLOC;

				do0(i,0,NumCol)
				{
					Colt[0][0][i]=i;
				}
			}

		}
	}

	do0(i,0,K_Sect)
	{
		do0(ih,0,K_Hor[i])
		{
			do0(iv,0,K_Ver[i][ih])
			{
				nc=Colt[i][ih][iv];
 				RectFragm[nc].left   = (int16_t)(RectFragm[nc].left   * Twips);
	 			RectFragm[nc].right  = (int16_t)(RectFragm[nc].right  * Twips);
		 		RectFragm[nc].top    = (int16_t)(RectFragm[nc].top    * Twips);
				RectFragm[nc].bottom = (int16_t)(RectFragm[nc].bottom * Twips);
			}
		}
	}


//////////////////////////////////////////////////////////////////////////////////////////
//  Создание массива укрупненных колонок Colh--*****************************************//
//  преобр-ние из 3-уровневой в 2-уровневую систему колонок  (ideal size)               //
//////////////////////////////////////////////////////////////////////////////////////////
	if((ColH=(COLH**)malloc((K_Sect+1)*sizeof(COLH*)))==NULL)
		return -3;

	K_Ver_Add_On = (int16_t**)malloc((K_Sect+1)*sizeof(int16_t*));
	K_Ver_Offset = (int16_t**)malloc((K_Sect+1)*sizeof(int16_t*));
	if(K_Ver_Add_On==NULL||K_Ver_Offset==NULL)
		return NOT_ALLOC;

	do0(i,0,K_Sect)
	{
		SRECT BndTmp;
		if((ColH[i]=(COLH*)malloc((K_Hor[i]+1)*sizeof(COLH)))==NULL)return -3;
		K_Ver_Add_On[i]=(int16_t*)malloc((K_Hor[i]+1)*sizeof(int16_t));
		K_Ver_Offset[i]=(int16_t*)malloc((K_Hor[i]+1)*sizeof(int16_t));
		if(K_Ver_Add_On[i]==NULL||K_Ver_Offset[i]==NULL)
			return NOT_ALLOC;

		do0(ih,0,K_Hor[i])
		{
			K_Ver_Add_On[i][ih]=0;
			K_Ver_Offset[i][ih]=0;
			if(K_Ver_Flag_Term[i][ih]>=2)
				flag_vse_term=0;
			do0(iv,0,K_Ver[i][ih])
			{
				nc=Colt[i][ih][iv];
				if(!iv)
					ConvertRect16ToBnd(&RectFragm[nc],&bnd); //!! или берем по линиям с выравниванием
				else
				{
					ConvertRect16ToBnd(&RectFragm[nc],&BndTmp);
					MyUnionRect(&bnd,&BndTmp,&bnd);
				}
			}
			ColH[i][ih].bnd.left   = bnd.left;
			ColH[i][ih].bnd.right  = bnd.right;
			ColH[i][ih].bnd.top    = bnd.top;
			ColH[i][ih].bnd.bottom = bnd.bottom;
		}
	}

	CONS_MESS20("Подсчет реалных размеров кеглей ");
/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     //
//                  Подсчет реалных размеров кеглей                                    //
//                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////
	do0(i,0,K_Sect)
	{	//sect begin
		int index_word;
		do0(ih,0,K_Hor[i])
		{//hor. col.  begin
			do0(iv,0,K_Ver[i][ih])
			{	//vert. col.  begin
				nc=Colt[i][ih][iv];
				if( NumStr[nc] < 0)
					continue;
				RtfPage->m_arFragments.push_back( new CRtfFragment() );
				RtfPage->Count.RtfTextFragments++;
				j = RtfPage->m_arFragments.size();
				pRtfFragment = RtfPage->m_arFragments[j-1];
				pRtfFragment->m_wStringsCount = NumStr[nc]+1;
				pRtfFragment->m_wType         = FT_TEXT;
				pRtfFragment->m_rect.left     = RectFragm[nc].left;
				pRtfFragment->m_rect.right    = RectFragm[nc].right;
				pRtfFragment->m_rect.top      = RectFragm[nc].top;
				pRtfFragment->m_rect.bottom   = RectFragm[nc].bottom;
				pRtfFragment->m_Flag          = FragFlag [nc]; //nega_str сделать цикл и занести в массив RtfString признаки негативности

				do0(ns,0,NumStr[nc])
				{	//str. begin
					if(TitleStr[nc][ns].S_Gen.S_NumWord<=0)
						continue;
					pRtfFragment->m_arStrings.push_back( new CRtfString() );

					pRtfString=pRtfFragment->m_arStrings[ns];
					pRtfString->m_wWordsCount = TitleStr[nc][ns].S_Gen.S_NumWord;//nega_str добавить m_Flag в RtfString и занести туда признак NEGATIVE
					pRtfString->S_Flags       = TitleStr[nc][ns].S_Flags; //NEGA_STR

					do0(nw,0,TitleStr[nc][ns].S_Gen.S_NumWord-1)
					{//word begin
						if(TitleWord[nc][ns][nw].W_Gen.W_NumSym == 0)
						{
							pRtfString->m_wWordsCount--;
							continue;
						}
						pRtfString->m_arWords.push_back( new CRtfWord() );
						index_word = pRtfString->m_arWords.size();
						pRtfWord=pRtfString->m_arWords[index_word-1];
						pRtfWord->m_wCharsCount = TitleWord[nc][ns][nw].W_Gen.W_NumSym;
						pRtfWord->m_wIdealFontPointSize = ((TitleWord[nc][ns][nw]).W_Gen).FontSize;
						pRtfWord->m_wFontNumber         = ((TitleWord[nc][ns][nw]).W_Gen).FontNumber;

						do0(nz,0,TitleWord[nc][ns][nw].W_Gen.W_NumSym-1)
						{	//char begin
							pRtfWord->m_arChars.push_back( new CRtfChar() );
							pRtfChar=pRtfWord->m_arChars[nz];


							pRtfChar->m_wCountAlt=MIN(Zn[nc][ns][nw][nz].Title.Z_Num_Alt,REC_MAX_VERS);
							for (int alt=0;alt<Zn[nc][ns][nw][nz].Title.Z_Num_Alt&&alt<REC_MAX_VERS;alt++)
							{
								pRtfChar->m_chrVersions[alt].m_bChar        = Zn[nc][ns][nw][nz].Alt[alt].a_Code;
								pRtfChar->m_chrVersions[alt].m_bProbability = Zn[nc][ns][nw][nz].Alt[alt].a_Prob;
							}
							pRtfChar->m_bFlg_spell_nocarrying    = Zn[nc][ns][nw][nz].Alt[0].a_SpellNoCarrying; //~ не знак переноса, а дефис в слове (пр: красно-белый)
							pRtfChar->m_bFlg_cup_drop            = Zn[nc][ns][nw][nz].Alt[0].a_FlagCupDrop;
							pRtfChar->m_blanguage                = Zn[nc][ns][nw][nz].Alt[0].a_language;
							pRtfChar->m_bFlg_spell               = Zn[nc][ns][nw][nz].Alt[0].a_Spell;
							pRtfChar->m_wFontNumber              = ((TitleWord[nc][ns][nw]).W_Gen).FontNumber;
							pRtfChar->m_wFontPointSize           = ((TitleWord[nc][ns][nw]).W_Gen).FontSize;

							RtfAssignRect_CRect_SRect( &pRtfChar->m_Realrect,  &Zn[nc][ns][nw][nz].Title.Z_RealRect );
							RtfAssignRect_CRect_SRect( &pRtfChar->m_Idealrect, &Zn[nc][ns][nw][nz].Title.Z_Rect );
						}//char end
					}//word end
				}//str end
			}//vert.end
		}//hor.end
	}//sec.end

	RtfPage->CorrectKegl(); // actually it is calculation of the kegles
	RtfPage->ChangeKegl();  // it is properly writing

//////////////////////////////////////////////////////////////////////////////////////////
//Новые координаты гор.колонок после пересчета реал. размеров вер.кол , параг. и строк  //
//////////////////////////////////////////////////////////////////////////////////////////
	if((ColH_New=(COLH**)malloc((K_Sect+1)*sizeof(COLH*)))==NULL)     return -3;
	do0(i,0,K_Sect)
	{
		if((ColH_New[i]=(COLH*)malloc((K_Hor[i]+1)*sizeof(COLH)))==NULL) return -3;
		do0(ih,0,K_Hor[i])
		{
			ColH_New[i][ih].bnd.left    = ColH[i][ih].bnd.left;
			ColH_New[i][ih].bnd.right   = ColH[i][ih].bnd.right;
			ColH_New[i][ih].bnd.top     = ColH[i][ih].bnd.top;
			ColH_New[i][ih].bnd.bottom  = ColH[i][ih].bnd.bottom;
		}
	}

//                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////
// Добавляем отформатированные текстовые фрагменты (Page).
	RtfPage->Count.RtfSectors = K_Sect;
	do0(i,0,K_Sect)
	{	//sect begin
		int index_word;
		RtfPage->m_arSectors.push_back( new CRtfSector() );
		pRtfSector = RtfPage->m_arSectors[i];
		pRtfSector->m_wHorizontalColumnsCount = K_Hor[i];

		do0(ih,0,K_Hor[i])
		{//hor. col.  begin
			pRtfSector->m_arHorizontalColumns.push_back( new CRtfHorizontalColumn() );
			pRtfHorizontalColumn	= pRtfSector->m_arHorizontalColumns[ih];

			RtfUnionRect_CRect_SRect(&pRtfHorizontalColumn->m_rectReal, &ColH_New[i][ih].bnd);
			RtfUnionRect_CRect_CRect(&pRtfSector->m_rectReal, &pRtfHorizontalColumn->m_rectReal);
			RtfUnionRect_CRect_CRect(&RtfPage->m_rectReal, &pRtfSector->m_rectReal);

			RtfUnionRect_CRect_SRect(&pRtfHorizontalColumn->m_rect, &ColH[i][ih].bnd);
			RtfUnionRect_CRect_CRect(&pRtfSector->m_rect, &pRtfHorizontalColumn->m_rect);
			RtfUnionRect_CRect_CRect(&RtfPage->m_rect, &pRtfSector->m_rect);

			pRtfHorizontalColumn->m_wVerticalColumnsCount	= K_Ver[i][ih];
			pRtfHorizontalColumn->m_wType 	= K_Ver_Flag_Term[i][ih];

			do0(iv,0,K_Ver[i][ih])
			{	//vert. col.  begin
				nc=Colt[i][ih][iv];
				if( NumStr[nc] < 0)
					continue;

				if( K_Hor[i]==0 && K_Ver[i][ih]==0 && NumStr[nc]==0 )
					pRtfSector->m_FlagOneString = TRUE;

				pRtfHorizontalColumn->m_arVerticalColumns.push_back( new CRtfVerticalColumn() );
				pRtfVerticalColumn	= pRtfHorizontalColumn->m_arVerticalColumns.back();

				pRtfVerticalColumn->m_wFragmentsCount = 1;
				pRtfVerticalColumn->m_arFragments.push_back( new CRtfFragment() );
				pRtfFragment=pRtfVerticalColumn->m_arFragments[/*iv*/ 0]; //nega ~? м.б. [iv] вместо [0]?
				pRtfFragment->m_wType = FT_TEXT;
				RtfAssignRect_CRect_Rect16( &pRtfVerticalColumn->m_rect,     &RectFragm[nc] );
				RtfAssignRect_CRect_Rect16( &pRtfVerticalColumn->m_rectReal, &RectFragm[nc] );

				RtfAssignRect_CRect_Rect16( &pRtfFragment->m_rect,     &RectFragm[nc] );
				RtfAssignRect_CRect_Rect16( &pRtfFragment->m_rectReal, &RectFragm[nc] );

				pRtfFragment->m_wStringsCount = NumStr[nc]+1;
				pRtfFragment->m_Flag          = FragFlag[nc]; //nega

				do0(ns,0,NumStr[nc])
				{	//str. begin
					if(TitleStr[nc][ns].S_Gen.S_NumWord<=0)
						continue;

					pRtfFragment->m_arStrings.push_back( new CRtfString() );
					pRtfString=pRtfFragment->m_arStrings[ns];

					if(TitleStr[nc][ns].S_Attr)
					{
						pRtfFragment->m_Attr=1;
						pRtfString->m_Attr=TRUE;
					}
					else
						pRtfString->m_Attr=FALSE;

					pRtfString->m_wWordsCount = TitleStr[nc][ns].S_Gen.S_NumWord;
					pRtfString->S_Flags       = TitleStr[nc][ns].S_Flags; //NEGA_STR

					do0(nw,0,TitleStr[nc][ns].S_Gen.S_NumWord-1)
					{	//word begin
						if(TitleWord[nc][ns][nw].W_Gen.W_NumSym == 0)
						{
							pRtfString->m_wWordsCount--;
							continue;
						}
						pRtfString->m_arWords.push_back( new CRtfWord() );
						index_word = pRtfString->m_arWords.size();
						pRtfWord=pRtfString->m_arWords[index_word-1];

						pRtfWord->m_wCharsCount        = TitleWord[nc][ns][nw].W_Gen.W_NumSym;
						pRtfWord->m_wFontNumber        = ((TitleWord[nc][ns][nw]).W_Gen).FontNumber;
						pRtfWord->m_wIdealFontPointSize= ((TitleWord[nc][ns][nw]).W_Gen).FontSize;

						if(NumStr[nc]==0 && TitleStr[nc][ns].S_Gen.S_NumWord==1)
							pRtfWord->m_wRealFontPointSize = RtfPage->GetMinKegl(pRtfWord->m_wIdealFontPointSize);
						else
							pRtfWord->m_wRealFontPointSize = RtfPage->GetNewKegl(pRtfWord->m_wIdealFontPointSize);

						do0(nz,0,TitleWord[nc][ns][nw].W_Gen.W_NumSym-1)
						{
							pRtfWord->m_arChars.push_back( new CRtfChar() );
							pRtfChar=pRtfWord->m_arChars[nz];


							pRtfChar->m_wCountAlt=MIN(Zn[nc][ns][nw][nz].Title.Z_Num_Alt,REC_MAX_VERS);
							for (int alt=0;alt<Zn[nc][ns][nw][nz].Title.Z_Num_Alt&&alt<REC_MAX_VERS;alt++)
							{
								pRtfChar->m_chrVersions[alt].m_bChar        = Zn[nc][ns][nw][nz].Alt[alt].a_Code;
								pRtfChar->m_chrVersions[alt].m_bProbability = Zn[nc][ns][nw][nz].Alt[alt].a_Prob;
							}
							pRtfChar->m_bFlg_spell_nocarrying    = Zn[nc][ns][nw][nz].Alt[0].a_SpellNoCarrying;
							pRtfChar->m_bFlg_cup_drop            = Zn[nc][ns][nw][nz].Alt[0].a_FlagCupDrop;
							pRtfChar->m_blanguage                = Zn[nc][ns][nw][nz].Alt[0].a_language;
							pRtfChar->m_bFlg_spell               = Zn[nc][ns][nw][nz].Alt[0].a_Spell;
							pRtfChar->m_wFontNumber              = ((TitleWord[nc][ns][nw]).W_Gen).FontNumber;
							pRtfChar->m_wFontPointSize           = ((TitleWord[nc][ns][nw]).W_Gen).FontSize;

							RtfAssignRect_CRect_SRect( &pRtfChar->m_Realrect,  &Zn[nc][ns][nw][nz].Title.Z_RealRect );
							RtfAssignRect_CRect_SRect( &pRtfChar->m_Idealrect, &Zn[nc][ns][nw][nz].Title.Z_Rect );
						}
					}//word end
				}//str end
			}//vert.end
		}//hor.end
	}//sec.end

#ifdef alDebug
	if(det20 || det23)
	{ ConsMess("Formatter End ");
	  if(RtfWriteMode)
	  ConsMess("*************************************************************");
	}
#endif
	do0(i,0,K_Sect)
	{
		do0(ih,0,K_Hor[i])
	    free(Colt[i][ih]);
		free(K_Ver[i]);
		free(K_Ver_Flag_Term[i]);
		free(K_Ver_Add_On[i]);
		free(K_Ver_Offset[i]);
		free(Colt[i]);
		free(ColH[i]);
		free(ColH_New[i]);
	}

	if(K_Hor)            free(K_Hor);
	if(K_Ver)	           free(K_Ver);
	if(K_Ver_Flag_Term)		free(K_Ver_Flag_Term);
	if(K_Ver_Add_On)		   free(K_Ver_Add_On);
	if(K_Ver_Offset)		   free(K_Ver_Offset);
	if(Colt)		           free(Colt);
	if(ColH)		           free(ColH);
	if(ColH_New)		       free(ColH_New);
	if(RootUdal)	        free((KNOT**)RootUdal);
	if(ArrFrm)    	      free(ArrFrm);
	if(frm)              free(frm);

	if(FlagOdinSectorOdnaColonka)
	{
		int16_t tmpNumCol;
		tmpNumCol = NumCol;
		NumCol    = OldNumCol;
		if(NumStr)
			FreeStructFull();
		NumCol    = tmpNumCol;
	}
	else
		if(NumStr)           FreeStructFull();

	if(NumStr)           free(NumStr);
	if(UserNumber)	      free(UserNumber);
	if(FragFlag)	      free(FragFlag);
	if(RectFragm)        free(RectFragm);

//#ifdef alDebug
//if(det20)	{ ConsMess("End FileName=%s ",OutFileName); }
//#endif



	if(FlagBadBad)
		return TRUE;

	free((KNOT**)Inf.Tree.Root);
	free((KNOT**)Inf.Tree.ArrSeg);
	free((KNOT**)Inf.LineVK);
	free((KNOT**)Inf.LineHK);
	free((KNOT**)Inf.bnd_col);
	free((KNOT**)Inf.StatCell);
	free((KNOT**)Inf.ColT);

	return TRUE;
}

void Get_all_term_fragms1( KNOTT* ptr,int16_t* Colt,int16_t* iv,int16_t NumCol,FRAME **frm)
 {
  int16_t i_nsb,i_nse;

  if(ptr->NumFrm>1 && !ptr->Type)
		{
			#ifdef alDebug
    if(det4) ConsMess(">>> %d не отсортированных фрагмента",ptr->NumFrm);
			#endif

   i_nse=ptr->InBegFrm+ptr->NumFrm;
   for(i_nsb=ptr->InBegFrm; i_nsb<i_nse; ++*iv,++i_nsb)
			{
    Colt[*iv]=(int16_t)frm[i_nsb]->start_pos;
				#ifdef alDebug
     if(det4)  ConsMess(" #term=%d",NumCol+1-Colt[*iv]);
				#endif
			}
		}
  else
	 {
	  Colt[*iv]=ptr->InBegFrm;  ++*iv;
			#ifdef alDebug
	   if(det11) ConsMess(" #term=%d",NumCol+1-ptr->InBegFrm);
			#endif
		}
}

void Get_all_term_fragms( KNOTT* ptr,int16_t* Colt,int16_t* iv,int16_t NumCol,FRAME **frm)
 {
 	int16_t i,i1,i2,i3,kp,kp1,kp2,kp3,kp4;
  KNOTT *ptr1,*ptr2,*ptr3,*ptr4;

  kp=CalcNumDau(ptr)-1;
  for(i=0,ptr1=ptr->down; i <= kp; ++i,ptr1=ptr1->next)
		{
		 kp1=CalcNumDau(ptr1)-1;//Число дочерей col
   if(kp1 < 0)            //Терм. col
    Get_all_term_fragms1( ptr1, Colt, iv, NumCol,frm);
   else
			{
    for(i1=0,ptr2=ptr1->down; i1 <= kp1; ++i1,ptr2=ptr2->next)
				{
				 kp2=CalcNumDau(ptr2)-1;
     if(kp2 < 0)            //Терм. col
					 Get_all_term_fragms1( ptr2, Colt, iv, NumCol,frm);
     else
					{
      for(i2=0,ptr3=ptr2->down; i2 <= kp2; ++i2,ptr3=ptr3->next)
						{
				   kp3=CalcNumDau(ptr3)-1;
					  if(kp3 < 0)            //Терм. col
						 	Get_all_term_fragms1( ptr3, Colt, iv, NumCol,frm);
       else
							{
        for(i3=0,ptr4=ptr3->down; i3 <= kp3; ++i3,ptr4=ptr4->next)
								{
				     kp4=CalcNumDau(ptr4)-1;
						   if(kp4 < 0)            //Терм. col
								 	Get_all_term_fragms1( ptr4, Colt, iv, NumCol,frm);
										#ifdef alDebug
            else
          	 if(det11)  ConsMess("   Ошибка !!!   ");
									#endif

								}
							}
					 }
					}
			 }
			}
		}
 }

void RtfAssignRect_CRect_Rect16(tagRECT *s1,Rect16 *s2)
{
 s1->left   =  s2->left;
 s1->right  =  s2->right;
 s1->top    =  s2->top;
 s1->bottom =  s2->bottom;
}

void RtfAssignRect_CRect_SRect(tagRECT *s1,SRECT *s2)
{
 s1->left   =  s2->left;
 s1->right  =  s2->right;
 s1->top    =  s2->top;
 s1->bottom =  s2->bottom;
}

void RtfAssignRect_CRect_CRect(tagRECT *s1,tagRECT *s2)
{
 s1->left   =  s2->left;
 s1->right  =  s2->right;
 s1->top    =  s2->top;
 s1->bottom =  s2->bottom;
}

void RtfUnionRect_CRect_SRect(tagRECT *s1,SRECT *s2)
{
 s1->left   = MIN(s1->left  , s2->left);
 s1->right  = MAX(s1->right , s2->right);
 s1->top    = MIN(s1->top   , s2->top);
 s1->bottom = MAX(s1->bottom, s2->bottom);
}

void RtfUnionRect_CRect_CRect(tagRECT *s1,tagRECT *s2)
{
 s1->left   = MIN(s1->left  , s2->left);
 s1->right  = MAX(s1->right , s2->right);
 s1->top    = MIN(s1->top   , s2->top);
 s1->bottom = MAX(s1->bottom, s2->bottom);
}

void	RtfCalcRectSizeInTwips(tagRECT *s1, float Twips)
{
 s1->left   = (int32_t)(s1->left   * Twips);
 s1->right  = (int32_t)(s1->right  * Twips);
 s1->top    = (int32_t)(s1->top    * Twips);
 s1->bottom = (int32_t)(s1->bottom * Twips);
}

//==Объединение пары рамок
void MyUnionRect(SRECT *s1,SRECT *s2,SRECT *u)
//==
{
 u->left  =MIN(s1->left ,s2->left);
 u->right =MAX(s1->right,s2->right);
 u->top   =MIN(s1->top  ,s2->top);
 u->bottom=MAX(s1->bottom,s2->bottom);
}

//==
char *get2_param(char *str,char *param,int16_t max_len)
//==
{ int16_t len;
  len=-1; while(str[++len]==' '); str+=len;/*Убираем пробелы слева от параметра*/
  len=-1;
  while(++len < max_len && str[len] != ' ') /*Поиск первого пробела справа*/
  { if( (param[len]=str[len]) == 0) break; /*Detect END STRING*/
    if(len && (uint)str[len-1] == cr && (uint)str[len] == lf) /*Detect <cr><lf>*/
      { --len; break; }
  }
  /*Если параметр очень длинный, возвращаем неудачу его выделения*/
  if(len < max_len-1) param[len]=0; else param[len=0]=0;
  return str+len;
}

#ifdef alDebug
void clear(void)  {;}
void pause_internal(void)  {;}

void image_frm(FRAME *f,int col,int line_style,int fill)
{
	RECT f1;
	f1.left=f->left;
  f1.right=f->right;
	f1.top=f->up;
	f1.bottom=f->down;
	image_rect(&f1,col,line_style,fill);
}

void image_bnd(BOUND *f,int col,int line_style,int fill){}

void image_frame(FRAME **frm,int k,int col,int line_style,int fill)
{
	int i;
  do0(i,0,k)
		image_frm(frm[i],col,line_style,fill);
}

void bounds_frm(int ii,FRAME **frm,int nx){}

void BoundsRect(int ii,RECT *frm,int nx){}

void image_rect(RECT *f,int col,int line_style,int fill)
{
	CONS_MESS1(" left=%d,  right=%d,  up=%d,  down=%d",f->left,f->right,f->top,f->bottom);
	if( pTheGeomStep == pTheGeomStep1 )
	{
		pFragRectColor->push_back(Graphic1Color);
		if(Graphic1Color==0)
		{
			f->left=MAX(0,f->left-12);
			f->top=MAX(0,f->top-12);
	        f->right+=12;f->bottom+=12;
		}
		if(Graphic1Color==1)
		{
			f->left=MAX(0,f->left-8);
			f->top=MAX(0,f->top-8);
	        f->right+=8;f->bottom+=8;
		}
		if(Graphic1Color==2)
		{
			f->left=MAX(0,f->left-4);
			f->top=MAX(0,f->top-4);
	        f->right+=4;f->bottom+=4;
		}
	}
	tagRECT rct;
	SetRect(&rct,f->left,f->top,f->right,f->bottom);
	pTheGeomStep->push_back(rct);
}

#endif
