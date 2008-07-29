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

#define __RSELSTR__
#include<math.h>
#include<time.h>
#include "rselstr.h"
#include "roots.h"
#include "layout.h"
#include "recdefs.h"
#include "ccom.h"
#include "ccomdefs.h"
#include "ctiimage.h"
#include "cpage.h"
#include "dpuma.h"
#include "polyblock.h"
#include "rcutp.h"


#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
typedef DWORD   COLORREF;

#define TYPE_NormalZone		CPAGE_GetInternalType("TYPE_NormalZone")
#define TYPE_NormalRazrezZone		CPAGE_GetInternalType("TYPE_NormalRazrezZone")

# define ROOTS_QUANTUM			1024
#define k_cut 1.8
int count_cut_let;
Rect16 oldRoot;

extern int inf_let_h;
int inf_dust_h=4;
int inf_let_w=5;
int sup_dust_w=5;
int min_cut_down_let_w=3;
int sup_prob_w=20;
BOOL type_let;
extern Handle HCPAGE;
extern Handle hDebugCutStr;
extern Handle CutStrD;
extern Handle MainWindowD;
extern Handle DifferenCutToHistory;
extern Handle hTime;
int medium_statistics_h;
extern int medium_h;
Word32 code_str_cut_d=117;
Bool32 WasCut=FALSE;
Bool32 WasDif=FALSE;
Word32 Code_UB_Kill=0;
Word32 Code_UB_Create=0;

int CutStrings(POLY_* pBLOCK);
int GetStatisticsH(void);
void IfDifCut(void);



extern Handle HCCOM;
extern FILE* f_cut_str;
extern FILE* f_temp_cut;
extern FILE* f_old_cut;

int GetMediumHeight(POLY_*);
//--int GetMediumTop(ROOT* pRoots,int y);
//--int GetMediumBottom(ROOT* pRoots,int y);
//--BOOL CutRoot(ROOT* pRoot,BLOCK* pBkock);
//--BOOL MyAddRoot(CCOM_comp * comp);
//BLOCK* GetCrossBlock(ROOT*,BLOCK*);
//ROOT* IfNoStr(ROOT* pRoot,BLOCK* pCurrentBlock);
//-BOOL IfNearLetters(CCOM_comp*,CCOM_comp*);
//--BOOL MyReallocHist(BLOCK* pBlock2,ROOT* pCutRoot,BOOL BEG);
//--int GetNearDist(BLOCK* pBlock,ROOT* pRoot);
//-int GetCountCutLetters(CCOM_comp*);
BOOL GetMasP(Handle hCPage,Rect16 Rc,Word8** ppmasp);
BOOL Increase2(RecRaster* rast,CCOM_comp* comp);
//--void CleanRaster(RecRaster* rast,Rect16* rect,int scale);
//-BOOL IfWhiteRow(RecRaster* rast,int row);
//--void BlockHystogramDiscountRoot (BLOCK *pBlock, ROOT *pRoot);
//-BOOL IfZpt(CCOM_comp*);
//-BOOL IfCav(CCOM_comp*);
//-int GetRastWide(RecRaster* rast,int&,int&);
//-BOOL IfHightLetters(CCOM_comp*,BOOL Up,int,int);
int GetCountNumbers(int num);
void StrDrawRect(Handle wnd,Word32 OperCode,Word32 color,int top,int bottom,int left,int right);
//-int ExBound(CCOM_comp* comp,int bound);
//-BOOL NotNeedCut(CCOM_comp* comp);
//-void NotCutDownLetter(RecRaster* rast,int& bound,int rast_bound,int new_rast_bound,int delta,int mini_w,int scale);
BOOL IfEqv(char* buf1,char* buf2);
BOOL IfEqv(Rect16 r1,Rect16 r2);
BOOL AddLenBlockMas(POLY_** ppRc,int& len,int add);
void DelBlockMas(POLY_* masp);
BOOL InitBlockMas(POLY_** ppRc,int len);
int IsInPoly(Point16 a, void * pPoly);
BOOL CutComp(Handle hCPAGE,CCOM_handle hCCOM,CCOM_comp* comp,int bound,BOOL fl_cut);
//-int GetBoundFromStr(CCOM_comp* comp,BOOL IfCav,int& upper,int& downer);
//-int IfNeedCut(CCOM_comp* comp);
void UndoCutInRect(Handle hCPAGE,CCOM_handle hCCOM,Rect32* Rc);
//BOOL VerticaProbCell(Handle h,Point32 Cell);



void  RSELSTR_CutCompInTableZones(Handle hCPAGE,CCOM_handle hCCOM)
{}

void UndoCutInRect(Handle hCPAGE,CCOM_handle hCCOM,Rect32* Rc)
{
 CCOM_comp* comp;
 Int16 top=(Int16)Rc->top;
 Int16 bottom=(Int16)Rc->bottom;
 Int16 left=(Int16)Rc->left;
 Int16 right=(Int16)Rc->right;
 CCOM_USER_BLOCK ub;


 for(comp=CCOM_GetFirst(hCCOM,NULL);comp;comp=CCOM_GetNext(comp,NULL))
 {
  int size=sizeof(Word32);

  if(comp->upper>=top&&comp->left>=left&&comp->upper+comp->h<=bottom&&comp->left+comp->w<=right)
  {
   if(comp->large&CCOM_LR_KILLED)
   {
	 ub.code=Code_UB_Kill;
	 if(CCOM_GetUserBlock(comp,&ub))
	 {
		 if(ub.size==size)
		 {
			 if(*((Word32*)(ub.data))==Code_UB_Kill)
				 CCOM_Reanimate(comp);
		 }
	 }
   }
   else
   {
	ub.code=Code_UB_Create;
	if(CCOM_GetUserBlock(comp,&ub))
	{
	  if(ub.size==size)
	  {
		if(*((Word32*)(ub.data))==Code_UB_Create)
			CCOM_Kill(comp);
	  }
	}
   }
  }
 }
}




int CutStrings(POLY_* pBlock)
{
 medium_h=GetMediumHeight(pBlock);

 int cut_h=(int)((double)(medium_h)*k_cut);

 CCOM_comp* comp;
 CCOM_comp* prev_comp=NULL;

 PAGEINFO info={0};
 GetPageInfo(HCPAGE,&info);
 int nIncline=info.Incline2048;

 Point16 pLeftTop;
 Point16 pRightTop;
 Point16 pLeftBottom;
 Point16 pRightBottom;

 BOOL fl_cont=FALSE;

 comp=CCOM_GetFirst((CCOM_handle)HCCOM,NULL);
 prev_comp=comp;

//Andrey: устанавливаем переменные для метода определения точек разрезания, перемещенного в RCUTP
 if (!RCUTP_RSelStr_SetEnv(/*(char*)info.szImageName, */medium_h, inf_let_w, inf_dust_h, inf_let_h, HCCOM, sup_dust_w, min_cut_down_let_w, sup_prob_w, info.BitPerPixel))
	return 0;

 while(comp)
 {
  fl_cont=FALSE;
  if(comp->h<inf_let_h)
  {
	  prev_comp=comp;
	  comp=CCOM_GetNext(comp,NULL);
      fl_cont=TRUE;
  }
  if(fl_cont)
	  continue;


  pLeftTop.x  = comp->left;
  pLeftTop.y  = comp->upper-1;
  pRightTop.x = comp->left+comp->w-1;
  pRightTop.y = comp->upper-1;
  pLeftBottom.x  = comp->left;
  pLeftBottom.y  = comp->upper+comp->h-2;
  pRightBottom.x = comp->left+comp->w-1;
  pRightBottom.y = comp->upper+comp->h-2;


  IDEAL_XY(pLeftTop.x,pLeftTop.y);
  IDEAL_XY(pRightTop.x,pRightTop.y);
  IDEAL_XY(pLeftBottom.x,pLeftBottom.y );
  IDEAL_XY(pRightBottom.x,pRightBottom.y);

  if(IsInPoly(pLeftTop,pBlock) ||
  IsInPoly(pRightTop,pBlock) ||
  IsInPoly(pLeftBottom,pBlock) ||
  IsInPoly(pRightBottom,pBlock) )
  {
   if(comp->h>=cut_h&&comp->h<=medium_h*5&&comp->w>=inf_let_w-1)
//	   ||((medium_top-pRoot->yRow>=(int)(medium_h*(k_cut-1)))&&IfZpt(pRoot,pCurrentBlock)&&(pRoot->yRow+pRoot->nHeight-1>medium_bottom-medium_h/3))||((pRoot->yRow+pRoot->nHeight-medium_bottom>=(int)((k_cut-1)*medium_h))&&IfCav(pRoot,pCurrentBlock)&&(pRoot->yRow<medium_top+medium_h/3)))
   {
//&&((medium_top-pRoot->yRow>=(int)(medium_h*(k_cut-1)))||(pRoot->yRow+pRoot->nHeight-medium_bottom>=(int)((k_cut-1)*medium_h))
//--------------------------------------------------------------------------------
//Andrey: метод определения точки разрезания вынесен в RCUTP (04.02.2003)
/*	count_cut_let=GetCountCutLetters(comp);

	int bound=IfNeedCut(comp);
*/
//--------------------------------------------------------------------------------
	Word8 Data[1000];
	memset (Data, 0, sizeof (Data));
	Word8* pmasp=Data;
	Rect16 Rc;

	Rc.top=comp->upper;
	Rc.bottom=comp->upper+comp->h-1;
	Rc.left=comp->left;
	Rc.right=comp->left+comp->w-1;

	int bound = GetMasP(HCPAGE,Rc,&pmasp) ? RCUTP_RSelStr_CP(comp, &type_let, pmasp) : comp->h/medium_h;

	if(bound>0)
	{
		if(CutComp(HCPAGE,(CCOM_handle)HCCOM,comp,bound,1))
		{
			comp=prev_comp;
			fl_cont=TRUE;
		}
	}
	if(!fl_cont)
    	comp=CCOM_GetNext(comp,NULL);
   }
   else
	   comp=CCOM_GetNext(comp,NULL);
  }
  else
	  comp=CCOM_GetNext(comp,NULL);
 }

//Andrey: снятие действия установки переменных в RCUTP
 RCUTP_RSelStr_UnsetEnv();

/*			if(CutRoot(pRoot,pCurrentBlock))
			{
               if(pCutRoot=IfNoStr(pRoot,pCurrentBlock))
			   {
                pBlock2=GetCrossBlock(pCutRoot,pCurrentBlock);
				if(!pBlock2)
					pBlock2=pCurrentBlock;

				if(pBlock2!=pCurrentBlock)
					BlockHystogramDiscountRoot(pCurrentBlock,pCutRoot);

				if(pBlock2->Rect.yTop>pCutRoot->yRow)
				{
					if(!MyReallocHist(pBlock2,pCutRoot,TRUE))
                      ErrorNoEnoughMemory ((Word8*)"in CutStrings,BlocksHystogramsReAllocate");
				}
				if(pBlock2->Rect.yBottom<pCutRoot->yRow+pCutRoot->nHeight-1)
				{
					if(!MyReallocHist(pBlock2,pCutRoot,FALSE))
                      ErrorNoEnoughMemory ((Word8*)"in CutStrings,BlocksHystogramsReAllocate");
				}

				if((pRoot->bType&ROOT_DUST)&&(pCurrentBlock->nDust))
					(pCurrentBlock->nDust)--;
				if(!(pRoot->bType&ROOT_LETTER))
					(pCurrentBlock->nLetters)++;
				pRoot->bType=pRoot->u1.pNext->bType=ROOT_LETTER;
				(pBlock2->nRoots)++;
				(pBlock2->nLetters)++;

				//удалим
			    if(pCutRoot!=pRoot)
				{
                    pRoot->u1.pNext=pCutRoot->u1.pNext;
				}
				else
				{
					if(pRoot==pCurrentBlock->pRoots)
					{
						pCurrentBlock->pRoots=pRoot->u1.pNext;
					}
					else
					{
						pDobRoot=pCurrentBlock->pRoots;
						while(pDobRoot->u1.pNext!=pRoot)
							pDobRoot=pDobRoot->u1.pNext;
						pDobRoot->u1.pNext=pRoot->u1.pNext;

						if(pRoot==pCurrentBlock->pEndRoots)
					        pCurrentBlock->pEndRoots=pRoot->u1.pNext;
				        if(pRoot==pAfterRoots)
					        pAfterRoots=pAfterRoots->u1.pNext;
					}
				}
                //добавим
				    pCutRoot->u1.pNext=pBlock2->pRoots->u1.pNext;
				    pBlock2->pRoots->u1.pNext=pCutRoot;
				if(pBlock2->pRoots==pBlock2->pEndRoots)
					pBlock2->pEndRoots=pBlock2->pRoots->u1.pNext;
				if(pBlock2->pRoots==pAfterRoots)
					pAfterRoots=pAfterRoots->u1.pNext;
			   }
			   else
			   {
				if(pRoot->bType&ROOT_DUST)
					(pCurrentBlock->nDust)--;
				if(!(pRoot->bType&ROOT_LETTER))
					(pCurrentBlock->nLetters)++;
				pRoot->bType=pRoot->u1.pNext->bType=ROOT_LETTER;
				(pCurrentBlock->nRoots)++;
				(pCurrentBlock->nLetters)++;
				if(pRoot==pCurrentBlock->pEndRoots)
					pCurrentBlock->pEndRoots=pRoot->u1.pNext;
				if(pRoot==pAfterRoots)
					pAfterRoots=pAfterRoots->u1.pNext;
			   }
			 return 2;
			}
		}
    }
  }
 return 1;

  */
 return 1;
}

/*Andrey: moved to RCUTP
//----------------------
int IfNeedCut(CCOM_comp* comp)
{
 int bound;
 int upper=-1;
 int downer=-1;
 type_let=0;

 if(count_cut_let==2&&comp->w>inf_let_w+2&&comp->h<medium_h*2)
 {
	 if(IfZpt(comp))
	 {
		 type_let=1;
		 bound=comp->upper+comp->h/4;
	 }
	 else
	 {
	  if(IfCav(comp))
	  {
		  type_let=2;
		  bound=GetBoundFromStr(comp,TRUE,upper,downer);
	  }
	  else
	  {
		 if(NotNeedCut(comp))
			 return FALSE;
         bound=comp->upper+(comp->h)/count_cut_let;
		 bound=ExBound(comp,bound);
	  }
	 }
 }
 else
 {
	 if(NotNeedCut(comp))
		 return FALSE;
     bound=comp->upper+(comp->h)/count_cut_let;
	 bound=ExBound(comp,bound);
 }
 return bound;
}


int GetBoundFromStr(CCOM_comp* comp,BOOL IfCav,int& max_bottom,int& min_top)
{
 int up=comp->upper;
 int down=comp->upper+comp->h-1;
 if(IfCav)
	 down+=inf_let_h;
 min_top=down;
 max_bottom=up;

 CCOM_comp* co=CCOM_GetFirst((CCOM_handle)HCCOM,NULL);
 while(co)
 {
  if(co->h>inf_let_h&&(co->upper!=comp->upper||co->left!=comp->left||co->w!=comp->w||co->h!=comp->h))
  {
    if(comp->left>co->left&&comp->left-(co->left+co->w)<5*inf_let_h||comp->left<co->left&&co->left-(comp->left+comp->w)<5*inf_let_h)
	{
		if(abs(co->upper-up)<inf_let_h)
		{
			if(max_bottom<co->upper+co->h-1)
				max_bottom=co->upper+co->h-1;
		}
		if(abs(co->upper+co->h-down)<inf_let_h)
		{
			if(min_top>co->upper)
				min_top=co->upper;
		}
	}
  }
  co=CCOM_GetNext(co,NULL);
 }

 if(min_top==down||max_bottom==up||((max_bottom+min_top)>>1)<=comp->upper||((max_bottom+min_top)>>1)>=comp->upper+comp->h-1)
 {
	 min_top=-1;
	 max_bottom=-1;
	 if(IfCav)
		 return comp->upper+(comp->h*3)/4;
	 else
		 return comp->upper+comp->h/2;
 }

 return (max_bottom+min_top)>>1;
}
*/

int GetMediumHeight(POLY_* pBlock)
{
 CCOM_comp* comp;
 int sum_height=0;
 int count=0;
 Point16 pLeftTop;
 Point16 pRightTop;
 Point16 pLeftBottom;
 Point16 pRightBottom;

 PAGEINFO info={0};
 GetPageInfo(HCPAGE,&info);
 int nIncline=info.Incline2048;

 for(comp=CCOM_GetFirst((CCOM_handle)HCCOM,NULL);comp;comp=CCOM_GetNext(comp,NULL))
 {
  if(comp->h<inf_let_h)
      continue;

  pLeftTop.x  = comp->left;
  pLeftTop.y  = comp->upper-1;
  pRightTop.x = comp->left+comp->w-1;
  pRightTop.y = comp->upper-1;
  pLeftBottom.x  = comp->left;
  pLeftBottom.y  = comp->upper+comp->h-2;
  pRightBottom.x = comp->left+comp->w-1;
  pRightBottom.y = comp->upper+comp->h-2;


  IDEAL_XY(pLeftTop.x,pLeftTop.y);
  IDEAL_XY(pRightTop.x,pRightTop.y);
  IDEAL_XY(pLeftBottom.x,pLeftBottom.y );
  IDEAL_XY(pRightBottom.x,pRightBottom.y);

  if(IsInPoly(pLeftTop,pBlock) ||
  IsInPoly(pRightTop,pBlock) ||
  IsInPoly(pLeftBottom,pBlock) ||
  IsInPoly(pRightBottom,pBlock) )
  {
   sum_height+=comp->h;
   count++;
  }
 }

 if(!count)
    return medium_statistics_h;

 return sum_height/count;
}

/*--Andrey: commented because of no using
//---------------------------------------
int GetMediumTop(ROOT* pRoots,int y)
{
 ROOT* pRoot;
 int sum_top=0;
 int count=0;

	for(pRoot=pRoots;pRoot;pRoot=pRoot->u1.pNext)
    {
  			if(pRoot->nHeight<inf_let_h)
                continue;

            if(pRoot->nHeight>inf_let_h)
			{
                 sum_top+=pRoot->yRow;
				 count++;
			}
    }

	if(!count)
		return 0;
    return sum_top/count;
}


int GetMediumBottom(ROOT* pRoots,int y)
{
 ROOT* pRoot;
 int sum_bottom=0;
 int count=0;

	for(pRoot=pRoots;pRoot;pRoot=pRoot->u1.pNext)
    {
  			if(pRoot->nHeight<inf_let_h)
                continue;

            if(pRoot->nHeight>inf_let_h)
			{
                 sum_bottom+=pRoot->yRow+pRoot->nHeight-1;
				 count++;
			}
    }

	if(!count)
		return 0;
    return sum_bottom/count;
}
--*/
int GetMediumHeight(ROOT* pRoots)
{
 ROOT* pRoot;
 int sum_height=0;
 int count=0;

	for(pRoot=pRoots;pRoot;pRoot=pRoot->u1.pNext)
    {
  			if(IS_LAYOUT_DUST (*pRoot))
                continue;

            if((pRoot->bType&ROOT_LETTER)&&pRoot->nHeight>inf_let_h)
			{
                 sum_height+=pRoot->nHeight;
				 count++;
			}
    }

	if(!count)
		return 0;
	return sum_height/count;
}
/*-Andrey: commented because of not using
//---------------------------------------
BOOL CutRoot(ROOT* pRoot,BLOCK* pBlock)
{
 RecRaster rast;

 if(!CCOM_GetRaster((CCOM_comp*)(pRoot->pComp),&rast))
	 return FALSE;

 RecRaster rast2;

 for(int i=REC_MAX_RASTER_SIZE-1;i;i--)
     rast2.Raster[i]=0;

 CCOM_comp* comp=(CCOM_comp*)(pRoot->pComp);

 int bound;

 if(count_cut_let==2&&pRoot->nWidth>inf_let_w+2&&pRoot->nHeight<medium_h*2)
 {
	 if(IfZpt(comp))
		 bound=comp->upper+(comp->h)/4;
	 else
	 {
	  if(IfCav(comp))
		 bound=comp->upper+((comp->h)*3)/4;
	  else
	  {
//		 if(NotNeedCut(pRoot,pBlock))
//			 return FALSE;
         bound=comp->upper+(comp->h)/count_cut_let;
		 bound=ExBound(comp,bound);
	  }
	 }
 }
 else
 {
	 if(count_cut_let==2)
	 {
//		 if(NotNeedCut(pRoot,pBlock))
//			 return FALSE;
	 }
     bound=comp->upper+(comp->h)/count_cut_let;
	 bound=ExBound(comp,bound);
 }
 int rast_bound=(bound-comp->upper+1)>>(comp->scale);
 int nowbyte=0;
 int j=rast.lnPixHeight*8*((rast.lnPixWidth+63)/64);
 for(i=rast_bound*8*((rast.lnPixWidth+63)/64);i<j;i++)
 {
  rast2.Raster[nowbyte]=rast.Raster[i];
  rast.Raster[i]=0;
  nowbyte++;
 }

 rast2.lnPixHeight=rast.lnPixHeight-rast_bound;
 rast2.lnPixWidth=rast.lnPixWidth;
 rast2.lnRasterBufSize=REC_MAX_RASTER_SIZE;
 rast.lnPixHeight=rast_bound;

 Rect16 rect1;
 Rect16 rect2;

 rect1.top=comp->upper;
 rect1.bottom=bound;
 rect1.left=comp->left;
 rect1.right=rect1.left+comp->w-1;

 rect2.top=bound+1;
 rect2.bottom=comp->upper+comp->h-1;
 rect2.left=comp->left;
 rect2.right=comp->left+comp->w-1;

 CleanRaster(&rast,&rect1,comp->scale);
 CleanRaster(&rast2,&rect2,comp->scale);

 CCOM_comp* comp1=CCOM_New((CCOM_handle)HCCOM,rect1.top,rect1.left,rect1.right-rect1.left+1,rect1.bottom-rect1.top+1);
 if(!comp1)
	 return FALSE;
 CCOM_comp* comp2=CCOM_New((CCOM_handle)HCCOM,rect2.top,rect2.left,rect2.right-rect2.left+1,rect2.bottom-rect2.top+1);
 if(!comp2)
 {
	 CCOM_Delete((CCOM_handle)HCCOM,comp1);
	 return FALSE;
 }
 comp1->scale=comp2->scale=comp->scale;


 Word8* lp=NULL;
 lp=new Word8[256*256-1];
 Word8* old=lp;
 if(!lp)
 {
  CCOM_Delete((CCOM_handle)HCCOM,comp1);
  CCOM_Delete((CCOM_handle)HCCOM,comp2);
  delete[] lp;
  return FALSE;
 }

 Int16 lp_size;
 Int16 numcomp;
 while(comp1->scale)
 {
	 if(!Increase2(&rast,comp1))
		 break;
 }
 if(!CCOM_MakeLP(&rast,lp,&lp_size,&numcomp))
 {
  CCOM_Delete((CCOM_handle)HCCOM,comp1);
  CCOM_Delete((CCOM_handle)HCCOM,comp2);
  delete[] lp;
  return FALSE;
 }
 if(!CCOM_Store(comp1,numcomp,lp_size,lp,0,0,0,NULL,NULL))
 {
  CCOM_Delete((CCOM_handle)HCCOM,comp1);
  CCOM_Delete((CCOM_handle)HCCOM,comp2);
  delete[] lp;
  return FALSE;
 }
 while(comp2->scale)
 {
	 if(!Increase2(&rast2,comp2))
		 break;
 }
 if(!CCOM_MakeLP(&rast2,lp,&lp_size,&numcomp))
 {
  CCOM_Delete((CCOM_handle)HCCOM,comp1);
  CCOM_Delete((CCOM_handle)HCCOM,comp2);
  delete[] lp;
  return FALSE;
 }
 if(!CCOM_Store(comp2,numcomp,lp_size,lp,0,0,0,NULL,NULL))
 {
  CCOM_Delete((CCOM_handle)HCCOM,comp1);
  CCOM_Delete((CCOM_handle)HCCOM,comp2);
  delete[] lp;
  return FALSE;
 }

 if(!MyAddRoot(comp2))
 {
  CCOM_Delete((CCOM_handle)HCCOM,comp1);
  CCOM_Delete((CCOM_handle)HCCOM,comp2);
  delete[] lp;
  return FALSE;
 }

 ROOT* pNewRoot=&pRoots[nRoots-1];

 pRoot->nHeight=comp1->h;
 pRoot->nWidth=comp1->w;
 pRoot->pComp=comp1;
 pNewRoot->u1.pNext=pRoot->u1.pNext;
 pRoot->u1.pNext=pNewRoot;
 pNewRoot->bReached=pRoot->bReached;
 pNewRoot->nBlock=pRoot->nBlock;
 pNewRoot->nUserNum=pRoot->nUserNum;
 pNewRoot->pComp=comp2;
 IDEAL_XY(pNewRoot->xColumn,pNewRoot->yRow);

 if(f_cut_str)
 {
  int max_count=6;

//  if(oldRoot.top>comp->upper||oldRoot.bottom<comp->upper+comp->h-1||oldRoot.left>comp->left||oldRoot.right<comp->left+comp->w-1)
//  {
	 fprintf(f_cut_str,"Top:%d",comp1->upper);
	 for(i=GetCountNumbers(comp1->upper);i<=max_count;i++)
		 fprintf(f_cut_str," ");
	 fprintf(f_cut_str,"Bottom:%d",comp1->upper+comp1->h-1);
	 for(i=GetCountNumbers(comp1->upper+comp1->h-1);i<=max_count;i++)
		 fprintf(f_cut_str," ");
     fprintf(f_cut_str,"Left:%d",comp1->left);
	 for(i=GetCountNumbers(comp1->left);i<=max_count;i++)
		 fprintf(f_cut_str," ");
     fprintf(f_cut_str,"Right:%d",comp1->left+comp1->w-1);
	 for(i=GetCountNumbers(comp1->left+comp1->w-1);i<=max_count;i++)
		 fprintf(f_cut_str," ");

     fprintf(f_cut_str,"\n");

	 	 fprintf(f_cut_str,"Top:%d",comp2->upper);
	 for(i=GetCountNumbers(comp2->upper);i<=max_count;i++)
		 fprintf(f_cut_str," ");
	 fprintf(f_cut_str,"Bottom:%d",comp2->upper+comp2->h-1);
	 for(i=GetCountNumbers(comp2->upper+comp2->h-1);i<=max_count;i++)
		 fprintf(f_cut_str," ");
     fprintf(f_cut_str,"Left:%d",comp2->left);
	 for(i=GetCountNumbers(comp2->left);i<=max_count;i++)
		 fprintf(f_cut_str," ");
     fprintf(f_cut_str,"Right:%d",comp2->left+comp2->w-1);
	 for(i=GetCountNumbers(comp2->left+comp2->w-1);i<=max_count;i++)
		 fprintf(f_cut_str," ");

     fprintf(f_cut_str,"\n");

/-*
	 oldRoot.top=comp->upper;
	 oldRoot.bottom=comp->upper+comp->h-1;
	 oldRoot.left=comp->left;
	 oldRoot.right=comp->left+comp->w-1;
  }
  *-/
 }

 if(f_temp_cut)
 {
  int max_count=6;

//  if(oldRoot.top>comp->upper||oldRoot.bottom<comp->upper+comp->h-1||oldRoot.left>comp->left||oldRoot.right<comp->left+comp->w-1)
//  {
	 fprintf(f_temp_cut,"Top:%d",comp1->upper);
	 for(i=GetCountNumbers(comp1->upper);i<=max_count;i++)
		 fprintf(f_temp_cut," ");
	 fprintf(f_temp_cut,"Bottom:%d",comp1->upper+comp1->h-1);
	 for(i=GetCountNumbers(comp1->upper+comp1->h-1);i<=max_count;i++)
		 fprintf(f_temp_cut," ");
     fprintf(f_temp_cut,"Left:%d",comp1->left);
	 for(i=GetCountNumbers(comp1->left);i<=max_count;i++)
		 fprintf(f_temp_cut," ");
     fprintf(f_temp_cut,"Right:%d",comp1->left+comp1->w-1);
	 for(i=GetCountNumbers(comp1->left+comp1->w-1);i<=max_count;i++)
		 fprintf(f_temp_cut," ");

     fprintf(f_temp_cut,"\n");

	 	 fprintf(f_temp_cut,"Top:%d",comp2->upper);
	 for(i=GetCountNumbers(comp2->upper);i<=max_count;i++)
		 fprintf(f_temp_cut," ");
	 fprintf(f_temp_cut,"Bottom:%d",comp2->upper+comp2->h-1);
	 for(i=GetCountNumbers(comp2->upper+comp2->h-1);i<=max_count;i++)
		 fprintf(f_temp_cut," ");
     fprintf(f_temp_cut,"Left:%d",comp2->left);
	 for(i=GetCountNumbers(comp2->left);i<=max_count;i++)
		 fprintf(f_temp_cut," ");
     fprintf(f_temp_cut,"Right:%d",comp2->left+comp2->w-1);
	 for(i=GetCountNumbers(comp2->left+comp2->w-1);i<=max_count;i++)
		 fprintf(f_temp_cut," ");

     fprintf(f_temp_cut,"\n");

/-*
	 oldRoot.top=comp->upper;
	 oldRoot.bottom=comp->upper+comp->h-1;
	 oldRoot.left=comp->left;
	 oldRoot.right=comp->left+comp->w-1;
  }
  *-/
 }

 if(!LDPUMA_Skip(CutStrD))
 {
	  if(!WasCut)
		  WasCut=TRUE;

	  StrDrawRect(MainWindowD,code_str_cut_d,RGB(255,0,0),comp1->upper,comp1->upper+comp1->h-1,comp1->left,comp1->left+comp1->w-1);
      StrDrawRect(MainWindowD,code_str_cut_d,RGB(255,0,0),comp2->upper,comp2->upper+comp2->h-1,comp2->left,comp2->left+comp2->w-1);
 }
 CCOM_Delete((CCOM_handle)HCCOM,comp);

 delete[] lp;
 return TRUE;
}


BOOL MyAddRoot(CCOM_comp * comp)
{
 ROOT   RootRecord = {0};


		//***************Rom********************

		if ( (comp->h/comp->w > 20) || (comp->w/comp->h > 20))
			return FALSE;// Piter

		//**************************************

		//***************Rom********************
		if ( comp->h*comp->w < 6 )
			return FALSE;// Piter

		//**************************************

        RootRecord.bReached   = FALSE;
		RootRecord.pComp = comp;

		RootRecord.yRow = comp->upper;
        RootRecord.xColumn = comp->left;
		RootRecord.u1.pNext = NULL;
        RootRecord.nHeight = comp->h;
        RootRecord.nWidth = comp->w;
        RootRecord.nBlock = 0;
		RootRecord.nUserNum = 0;

		RootRecord.bType = ROOT_DUST|ROOT_RECOGNIZED ;
		if(comp->type & CCOM_CH_LETTER)
			RootRecord.bType = ROOT_DUST | ROOT_LETTER | ROOT_RECOGNIZED;
		if(comp->type & (CCOM_CH_DUST | CCOM_CH_PUNCT ))
			RootRecord.bType = ROOT_DUST;
		if(comp->type & CCOM_CH_DUST && (comp->w+7)/8*comp->h<2 )
			RootRecord.bType = 0 ;
		if(comp->type & CCOM_CH_GREAT )
			RootRecord.bType = 0;

        if (nRoots % ROOTS_QUANTUM == 0)
              return FALSE;

        pRoots [nRoots] = RootRecord;
        nRoots++;
 return TRUE;
}
--*/

/*
BLOCK* GetCrossBlock(ROOT* pRoot,BLOCK* pBlock)
{
 BLOCK* p;
 int num;
 for (num = nMinBlock; num <= nMaxBlock; num++)
 {
	 p=pBlockPointer[num];
	 if(p==pBlock)
		 continue;
	 if(p)
	 {
	  if(IfNearLetters(p,pRoot))
	  {
		 pRoot->nBlock=num;
		 return p;
	  }
	 }
 }
 return NULL;
}
*/

/*-Andrey: moved to RCUTP
//-----------------------
BOOL IfNearLetters(CCOM_comp* cutting,CCOM_comp* comp)
{
 CCOM_comp* co;
 int med_h=(2*(comp->upper)+comp->h-1)/2;
 int left=comp->left;
 int right=left+comp->w-1;
 int double_width=sup_prob_w;
 int up=comp->upper;
 int down=up+comp->h-1;
 int medium2_h;
 int left2;
 int up2;
 int right2;
 int down2;
 int cut_up=cutting->upper;
 int cut_down=cut_up+cutting->h-1;
 int cut_left=cutting->left;
 int cut_right=cut_left+cutting->w-1;


 for(co=CCOM_GetFirst((CCOM_handle)HCCOM,NULL);co;co=CCOM_GetNext(co,NULL))
 {

    if(co->h>inf_let_h)
	{
	 up2=co->upper;
	 left2=co->left;
	 down2=up2+co->h-1;
	 right2=left2+co->w-1;
	 medium2_h=(up2+down2)/2;

	 if(up2!=cut_up||left2!=cut_left||down2!=cut_down||right2!=cut_right)
	 {
	  if((up==cut_up&&down>=down2-2)||(up!=cut_up&&up<=up2+2))
	  {
		if((up2<=med_h)&&(down2>=med_h)&&(abs(med_h-medium2_h)<co->h/3))
		{
			if(left<left2)
			{
			    if(left2-right<double_width)
					return TRUE;
			}
            else
			{
				if(left-(right2)<double_width)
					return TRUE;
			}
		}
	  }
	 }
	}
 }
 return FALSE;
}
-*/

/*
ROOT* IfNoStr(ROOT* pRoot,BLOCK* pCurrentBlock)
{
 int medium_Root=(2*(pRoot->yRow)+pRoot->nHeight-1)/2;
 int medium_Next=(2*((pRoot->u1.pNext)->yRow)+(pRoot->u1.pNext)->nHeight-1)/2;
 int Root_left=pRoot->xColumn;
 int Root_right=pRoot->xColumn+pRoot->nWidth-1;
 int Root_double_width=(Root_right-Root_left+1)<<1;
 int Next_left=pRoot->u1.pNext->xColumn;
 int Next_right=pRoot->u1.pNext->xColumn+pRoot->u1.pNext->nWidth-1;
 int Next_double_width=(Next_right-Next_left+1)<<1;
 BOOL fl_Root=FALSE;
 BOOL fl_Next=FALSE;

 BLOCK* pBlockToRoot=GetCrossBlock(pRoot,pCurrentBlock);
 BLOCK* pBlockToNext=GetCrossBlock(pRoot->u1.pNext,pCurrentBlock);

 if((!pBlockToRoot)&&(!pBlockToNext))
	 return NULL;
 int min_dist_to_root=1000000;
 int min_dist_to_next=1000000;
 if(pBlockToRoot)
     min_dist_to_root=GetNearDist(pBlockToRoot,pRoot);
 if(pBlockToNext)
	 min_dist_to_next=GetNearDist(pBlockToNext,pRoot->u1.pNext);

 for(ROOT* pROOT=pCurrentBlock->pRoots;pROOT;pROOT=pROOT->u1.pNext)
 {
	 if(pROOT==pRoot||pROOT==pRoot->u1.pNext)
		 continue;
	 if(IS_LAYOUT_DUST(*pROOT))
      continue;

	 if((pROOT->bType&ROOT_LETTER)&&(pROOT->yRow<=medium_Root)&&(pROOT->yRow+pROOT->nHeight-1>=medium_Root))
	 {
		 if(Root_left<pROOT->xColumn)
		 {
			    if(pROOT->xColumn-Root_right<min_dist_to_root)
					fl_Root=TRUE;
		 }
         else
		 {
				if(Root_left-(pROOT->xColumn+pROOT->nWidth)<min_dist_to_root)
					fl_Root=TRUE;
		 }
	 }
     if((pROOT->bType&ROOT_LETTER)&&(pROOT->yRow<=medium_Next)&&(pROOT->yRow+pROOT->nHeight-1>=medium_Next))
	 {
		 if(Next_left<pROOT->xColumn)
		 {
			    if(pROOT->xColumn-Next_right<min_dist_to_next)
					fl_Next=TRUE;
		 }
         else
		 {
				if(Next_left-(pROOT->xColumn+pROOT->nWidth)<min_dist_to_next)
					fl_Next=TRUE;
		 }
	 }
 }
 if(!fl_Root)
	 return pRoot;
 if(!fl_Next)
	 return pRoot->u1.pNext;
 return NULL;
}
*/

/*--Andrey: commented because of not using
BOOL MyReallocHist(BLOCK* pBlock2,ROOT* pCutRoot,BOOL BEG)
{
 if(BEG)
 {
	 pBlock2->Rect.yTop=pCutRoot->yRow;
	 int nHist=pBlock2->Rect.yBottom-pBlock2->Rect.yTop+1;
	 int* pHist=(int*)malloc(nHist*sizeof(int));
	 if(!pHist)
		 return FALSE;
	 int i=nHist-1;
	 for(int j=pBlock2->nHystColumns-1;j>=0;j--)
	 {
		 pHist[i]=pBlock2->pHystogram[j];
		 i--;
	 }
	 for(j=i;j>=0;j--)
		 pHist[i]=0;
	 free(pBlock2->pHystogram);
	 pBlock2->pHystogram=pHist;
	 pBlock2->nHystColumns=nHist;
 }
 else
 {
	 pBlock2->Rect.yBottom=pCutRoot->yRow+pCutRoot->nHeight-1;
	 int nHist=pBlock2->Rect.yBottom-pBlock2->Rect.yTop+1;
	 int* pHist=(int*)malloc(nHist*sizeof(int));
	 if(!pHist)
		 return FALSE;
	 for(int j=pBlock2->nHystColumns-1;j>=0;j--)
	 {
		 pHist[j]=pBlock2->pHystogram[j];
	 }
	 for(j=pBlock2->nHystColumns;j<nHist;j++)
		 pHist[j]=0;
	 free(pBlock2->pHystogram);
	 pBlock2->pHystogram=pHist;
	 pBlock2->nHystColumns=nHist;
 }
 return TRUE;
}


int GetNearDist(BLOCK* pBlock,ROOT* pRoot)
{
 ROOT* pDobRoot;
 int med_h=(2*(pRoot->yRow)+pRoot->nHeight-1)/2;
 int left=pRoot->xColumn;
 int right=pRoot->xColumn+pRoot->nWidth-1;
 int up=pRoot->yRow;
 int down=pRoot->yRow+pRoot->nHeight-1;
 int min_dist=1000000;
 ROOT* pMinDistRoot;
 for (pDobRoot=pBlock->pRoots;pDobRoot;pDobRoot=pDobRoot->u1.pNext)
 {
  	if(IS_LAYOUT_DUST(*pDobRoot))
      continue;
    if(pDobRoot->nHeight>inf_let_h)
	{
		if((pDobRoot->yRow<=med_h)&&(pDobRoot->yRow+pDobRoot->nHeight>med_h))
		{
			if(left<pDobRoot->xColumn)
			{
			    if(pDobRoot->xColumn-right<min_dist)
				{
					min_dist=pDobRoot->xColumn-right;
					pMinDistRoot=pDobRoot;
				}

			}
            else
			{
				if(left-(pDobRoot->xColumn+pDobRoot->nWidth)<min_dist)
				{
					min_dist=left-(pDobRoot->xColumn+pDobRoot->nWidth);
					pMinDistRoot=pDobRoot;
				}
			}
		}
	}
 }
 return min_dist;
}
--*/

int GetStatisticsH()
{
	int sum=0;
	int count=0;
	CCOM_comp* comp;

	comp=CCOM_GetFirst((CCOM_handle)HCCOM,NULL);

	while(comp)
	{
	  if(comp->h>inf_let_h)
	  {
		count++;
		sum+=comp->h;
	  }
		comp=CCOM_GetNext(comp,NULL);
	}

	if(!count)
		return 15;

    return sum/count;
}


/*Andrey: moved to RCUTP
//----------------------
BOOL JNotInMas(int j,int* mas,int size,int bound)
{
	for(size--;size>bound;size--)
	{
		if(mas[size]==j)
			return FALSE;
	}
	return TRUE;
}

BOOL JNotInMas2(int j,int* mas,int size,int bound)
{
  for(int i=j+medium_h;i>=j;i--)
  {
	for(int k=size-1;k>bound;k--)
	{
		if(mas[k]==i)
			return FALSE;
	}
  }
  for(i=j-medium_h;i<=j;i++)
  {
	for(int k=size-1;k>bound;k--)
	{
		if(mas[k]==i)
			return FALSE;
	}
  }
	return TRUE;
}

int GetCountCutLetters(CCOM_comp* comp)
{
 Word8 Data[1000];
 memset (Data, 0, sizeof (Data));
 Word8* pmasp=Data;
 Rect16 Rc;


 if(comp->h/medium_h<=2)
	 return 2;


 Rc.top=comp->upper;
 Rc.bottom=comp->upper+comp->h-1;
 Rc.left=comp->left;
 Rc.right=comp->left+comp->w-1;

 if(!GetMasP(HCPAGE,Rc,&pmasp))
	 return comp->h/medium_h;

 int count=0;
 int w=Rc.right-Rc.left+1;
 int h=Rc.bottom-Rc.top+1;
 double* proc_black=new double[h];
 if(!proc_black)
	 return comp->h/medium_h;

 int newstr;
 int bytewide=(w+7)/8;
 Word8 bytep;
 int rect=128>>(7-(bytewide*8-w));
 int count_black=0;
 int i;
 int k;

 for(int j=h-1;j>=0;j--)
 {
  newstr=j*bytewide;
  count_black=0;
  for(i=(bytewide-2);i>=0;i--)
  {
	  bytep=pmasp[newstr+i];

	   if( (bytep&128)==0 ) count_black++;
	   if( (bytep&64)==0 ) count_black++;
	   if( (bytep&32)==0 ) count_black++;
	   if( (bytep&16)==0 ) count_black++;
	   if( (bytep&8)==0 ) count_black++;
	   if( (bytep&4)==0 ) count_black++;
	   if( (bytep&2)==0 ) count_black++;
	   if( (bytep&1)==0 ) count_black++;

  }
  bytep=pmasp[newstr+bytewide-1];
  for(k=128;k>=rect;k=k>>1)
   if( (bytep&k)==0 ) count_black++;
  proc_black[j]=(double)(count_black)/(double)(w);
 }

 i=comp->h/medium_h-1;
 int* min_black=new int[i];
 if(!min_black)
 {
	 delete[] proc_black;
     return comp->h/medium_h;
 }

 double proc=1.;
 for(k=i-1;k>=0;k--)
 {
  for(j=h-1-medium_h;j>=medium_h;j--)
  {
   if(proc_black[j]<proc&&JNotInMas2(j,min_black,i,k))
   {
	   proc=proc_black[j];
	   min_black[k]=j;
   }
  }
  proc=1.;
 }


 int last_j=h-1;
 count=1;

 for(j=h-1-medium_h;j>=medium_h;j--)
 {
	 if(!JNotInMas(j,min_black,i,-1))
	 {
		 if(last_j-j>=medium_h)
		 {
			 count++;
			 last_j=j;
		 }
	 }
 }

 delete[] proc_black;
 delete[] min_black;
 if(count>comp->h/medium_h)
	 return comp->h/medium_h;
 else
	 return count;
}
-*/

BOOL GetMasP(Handle hCPage,Rect16 Rc,Word8** ppmasp)
{
    int prewide;
	int left=Rc.left;
	int h=Rc.bottom-Rc.top+1;
	int w=Rc.right-Rc.left+1;
	int upper=Rc.top-1;
	if( (w%8)==0)
		prewide=w;
	else
	{prewide=w/8+1;
	 prewide*=8;
	}

	PAGEINFO info = {0};
	CIMAGEInfoDataInGet DataInto = {0};
	CIMAGEInfoDataOutGet DataOut = {0};

	Word8 Name[CPAGE_MAXNAME];
	Bool ret;
	int i;

	//  1. Подготовка к запросу части изображения.
	DataInto.dwHeight   = (Word32)(h);
	DataInto.dwWidth    = (Word32)(prewide);
	DataInto.wByteWidth = (Word16)(prewide/8);
	DataInto.dwX        = left;
	DataInto.dwY        = upper;
	DataInto.MaskFlag   = 0x00;
    GetPageInfo(hCPage,&info);
	for (i=0; i<CPAGE_MAXNAME; i++)
		Name[i] = info.szImageName[i];
	DataOut.dwWidth    = DataInto.dwWidth;
	DataOut.dwHeight   = DataInto.dwHeight;
	DataOut.wByteWidth = DataInto.wByteWidth;
	DataOut.byBit      = (Word16)info.BitPerPixel;
	DataOut.lpData     = *ppmasp;

//	  5. Чтение части изображения.
	ret = CIMAGE_GetData (Name, &DataInto, &DataOut);
	if (!ret)
		return FALSE;
	if (DataOut.lpData==NULL)
		return FALSE;
	*ppmasp = DataOut.lpData;
	return TRUE;
}

BOOL Increase2(RecRaster* rast,CCOM_comp* comp)
{
	int newbytewide=((rast->lnPixWidth)*2+7)/8;
	int bytewide=(rast->lnPixWidth+7)/8;
	int newh=rast->lnPixHeight*2;
	if(newbytewide*newh>REC_MAX_RASTER_SIZE)
		return FALSE;
	if(comp->h*((comp->w+7)/8)>REC_MAX_RASTER_SIZE)
		return FALSE;

	Word8 bytep;
	Word8 bytep2;
	int nowbyte=0;
	int nowbyte2=0;
	int k;
	int j;
	int count=(rast->lnPixWidth+63)/64;
	int new_count=(newbytewide+7)/8;

	Word8* str=new Word8[REC_MAX_RASTER_SIZE];
	if(!str)
		return FALSE;

	for(k=REC_MAX_RASTER_SIZE-1;k>=0;k--)
		str[k]=0;

	int i;
	for(i=rast->lnPixHeight-1;i>=0;i--)
	{
		nowbyte=i*count*8;
		nowbyte2=i*new_count*8;
	 for(int j=new_count;j>0;j--)
	 {
      bytep=0;
	  bytep2=rast->Raster[nowbyte];
	  bytep|=128&bytep2;
	  bytep|=64&(bytep2>>1);
	  bytep|=32&(bytep2>>1);
	  bytep|=16&(bytep2>>2);
	  bytep|=8&(bytep2>>2);
	  bytep|=4&(bytep2>>3);
	  bytep|=2&(bytep2>>3);
	  bytep|=1&(bytep2>>4);
	  str[nowbyte2]=bytep;
	  nowbyte2++;
	  bytep=0;
      bytep|=128&(bytep2<<4);
	  bytep|=64&(bytep2<<3);
	  bytep|=32&(bytep2<<3);
	  bytep|=16&(bytep2<<2);
	  bytep|=8&(bytep2<<2);
	  bytep|=4&(bytep2<<1);
	  bytep|=2&(bytep2<<1);
	  bytep|=1&bytep2;
	  str[nowbyte2]=bytep;
	  nowbyte2++;
	  nowbyte++;

      bytep=0;
	  bytep2=rast->Raster[nowbyte];
	  bytep|=128&bytep2;
	  bytep|=64&(bytep2>>1);
	  bytep|=32&(bytep2>>1);
	  bytep|=16&(bytep2>>2);
	  bytep|=8&(bytep2>>2);
	  bytep|=4&(bytep2>>3);
	  bytep|=2&(bytep2>>3);
	  bytep|=1&(bytep2>>4);
	  str[nowbyte2]=bytep;
	  nowbyte2++;
	  bytep=0;
      bytep|=128&(bytep2<<4);
	  bytep|=64&(bytep2<<3);
	  bytep|=32&(bytep2<<3);
	  bytep|=16&(bytep2<<2);
	  bytep|=8&(bytep2<<2);
	  bytep|=4&(bytep2<<1);
	  bytep|=2&(bytep2<<1);
	  bytep|=1&bytep2;
	  str[nowbyte2]=bytep;
	  nowbyte2++;
	  nowbyte++;

      bytep=0;
	  bytep2=rast->Raster[nowbyte];
	  bytep|=128&bytep2;
	  bytep|=64&(bytep2>>1);
	  bytep|=32&(bytep2>>1);
	  bytep|=16&(bytep2>>2);
	  bytep|=8&(bytep2>>2);
	  bytep|=4&(bytep2>>3);
	  bytep|=2&(bytep2>>3);
	  bytep|=1&(bytep2>>4);
	  str[nowbyte2]=bytep;
	  nowbyte2++;
	  bytep=0;
      bytep|=128&(bytep2<<4);
	  bytep|=64&(bytep2<<3);
	  bytep|=32&(bytep2<<3);
	  bytep|=16&(bytep2<<2);
	  bytep|=8&(bytep2<<2);
	  bytep|=4&(bytep2<<1);
	  bytep|=2&(bytep2<<1);
	  bytep|=1&bytep2;
	  str[nowbyte2]=bytep;
	  nowbyte2++;
	  nowbyte++;

      bytep=0;
	  bytep2=rast->Raster[nowbyte];
	  bytep|=128&bytep2;
	  bytep|=64&(bytep2>>1);
	  bytep|=32&(bytep2>>1);
	  bytep|=16&(bytep2>>2);
	  bytep|=8&(bytep2>>2);
	  bytep|=4&(bytep2>>3);
	  bytep|=2&(bytep2>>3);
	  bytep|=1&(bytep2>>4);
	  str[nowbyte2]=bytep;
	  nowbyte2++;
	  bytep=0;
      bytep|=128&(bytep2<<4);
	  bytep|=64&(bytep2<<3);
	  bytep|=32&(bytep2<<3);
	  bytep|=16&(bytep2<<2);
	  bytep|=8&(bytep2<<2);
	  bytep|=4&(bytep2<<1);
	  bytep|=2&(bytep2<<1);
	  bytep|=1&bytep2;
	  str[nowbyte2]=bytep;
	  nowbyte2++;
	  nowbyte++;
	 }
	}

	Word8* raster=rast->Raster;
    for(i=REC_MAX_RASTER_SIZE - 1;i>=0;i--)
		raster[i]=str[i];

    for(i=newh-1;i>=0;i=i-2)
	{
		 nowbyte=i*8*new_count;
		 nowbyte2=(i-1)*8*new_count;
		 k=(i/2)*8*new_count;
		for(j=new_count;j>0;j--)
		{
		 rast->Raster[nowbyte]=rast->Raster[nowbyte2]=rast->Raster[k];
		 k++;
		 nowbyte++;
		 nowbyte2++;
		 rast->Raster[nowbyte]=rast->Raster[nowbyte2]=rast->Raster[k];
		 k++;
		 nowbyte++;
		 nowbyte2++;
		 rast->Raster[nowbyte]=rast->Raster[nowbyte2]=rast->Raster[k];
		 k++;
		 nowbyte++;
		 nowbyte2++;
		 rast->Raster[nowbyte]=rast->Raster[nowbyte2]=rast->Raster[k];
		 k++;
		 nowbyte++;
		 nowbyte2++;
		 rast->Raster[nowbyte]=rast->Raster[nowbyte2]=rast->Raster[k];
		 k++;
		 nowbyte++;
		 nowbyte2++;
		 rast->Raster[nowbyte]=rast->Raster[nowbyte2]=rast->Raster[k];
		 k++;
		 nowbyte++;
		 nowbyte2++;
		 rast->Raster[nowbyte]=rast->Raster[nowbyte2]=rast->Raster[k];
		 k++;
		 nowbyte++;
		 nowbyte2++;
		 rast->Raster[nowbyte]=rast->Raster[nowbyte2]=rast->Raster[k];
		 k++;
		 nowbyte++;
		 nowbyte2++;
		}
	}
 rast->lnPixHeight*=2;
 rast->lnPixWidth*=2;
 (comp->scale)--;
 if(!(comp->scale))
 {
  rast->lnPixHeight=comp->h;
  rast->lnPixWidth=comp->w;
 }
  delete[] str;
 return TRUE;
}

/*--Andrey: commented because of not using
//----------------------------------------
void CleanRaster(RecRaster* rast,Rect16* rect,int scale)
{
 int count_move=0;
 int count=(rast->lnPixWidth+63)/64;
 int i;
 int j;
 int w=rast->lnPixWidth;
 Word8* Raster=rast->Raster;
 int nowbyte;
 while(IfWhiteRow(rast,0))
 {
  for(i=rast->lnPixHeight-1;i>=0;i--)
  {
   nowbyte=(i<<3)*count;

   for(j=count-1;j>=0;j--)
   {
   Raster[nowbyte]=Raster[nowbyte]<<1;
   nowbyte++;
   Raster[nowbyte-1]|=(Raster[nowbyte]&128)>>7;

   Raster[nowbyte]=Raster[nowbyte]<<1;
   nowbyte++;
   Raster[nowbyte-1]|=(Raster[nowbyte]&128)>>7;

   Raster[nowbyte]=Raster[nowbyte]<<1;
   nowbyte++;
   Raster[nowbyte-1]|=(Raster[nowbyte]&128)>>7;

   Raster[nowbyte]=Raster[nowbyte]<<1;
   nowbyte++;
   Raster[nowbyte-1]|=(Raster[nowbyte]&128)>>7;

   Raster[nowbyte]=Raster[nowbyte]<<1;
   nowbyte++;
   Raster[nowbyte-1]|=(Raster[nowbyte]&128)>>7;

   Raster[nowbyte]=Raster[nowbyte]<<1;
   nowbyte++;
   Raster[nowbyte-1]|=(Raster[nowbyte]&128)>>7;

   Raster[nowbyte]=Raster[nowbyte]<<1;
   nowbyte++;
   Raster[nowbyte-1]|=(Raster[nowbyte]&128)>>7;

   Raster[nowbyte]=Raster[nowbyte]<<1;
    if(j)
    {
     nowbyte++;
     Raster[nowbyte-1]|=(Raster[nowbyte]&128)>>7;
    }
   }
  }
  count_move++;
  if(count_move>REC_MAX_RASTER_SIZE)
	  return;
 }
 if(count_move)
 {
  rast->lnPixWidth=rast->lnPixWidth-count_move;
  rect->left=rect->left+(count_move<<scale);
 }
 count_move=0;
 while(IfWhiteRow(rast,rast->lnPixWidth-1))
 {
	 count_move++;
	 (rast->lnPixWidth)--;

 }
 if(count_move)
 {
  rect->right=rect->right-(count_move<<scale);
 }
 int new_count=(rast->lnPixWidth+63)/64;
 j=rast->lnPixHeight;
 if(new_count<count)
 {
  int h=rast->lnPixHeight;
  int nowbyte2=0;
  int endbyte;
  for(i=0;i<h;i++)
  {
	  nowbyte=i*new_count*8;
	  endbyte=(i+1)*new_count*8;
	  nowbyte2=i*count*8;
	  while(nowbyte<endbyte)
	  {
		  Raster[nowbyte]=Raster[nowbyte2];
		  nowbyte++;
		  nowbyte2++;
	  }
  }
 }
}
--*/

/*Andrey: moved to RCUTP
//----------------------
BOOL IfWhiteRow(RecRaster* rast,int row)
{
 int bytewide=8*((rast->lnPixWidth+63)/64);
 int byte_row=row>>3;
 int rect_row=128>>(row%8);
 Word8* Raster=rast->Raster;
 for(int i=rast->lnPixHeight-1;i>=0;i--)
 {
	 if(Raster[i*bytewide+byte_row]&rect_row)
           return FALSE;
 }
 return TRUE;
}

BOOL IfZpt(CCOM_comp* comp)
{
 int bound=comp->upper+inf_dust_h-1;

 RecRaster rast;
 CCOM_GetRaster(comp,&rast);
 rast.lnPixHeight=(bound-comp->upper+1)>>(comp->scale);

 int real_left=0;
 int real_right=rast.lnPixWidth;

 if((GetRastWide(&rast,real_left,real_right)<<comp->scale)>inf_let_w)
	 return FALSE;

 real_left=comp->left+(real_left<<comp->scale);
 real_right=comp->left+(real_right<<comp->scale);

 if(IfHightLetters(comp,TRUE,real_left,real_right))
	return TRUE;

 return FALSE;
}


BOOL IfCav(CCOM_comp* comp)
{
 int bound=comp->upper+comp->h-1-inf_dust_h;

 RecRaster rast1;
 CCOM_GetRaster(comp,&rast1);
 RecRaster rast;
 int i;
 int bytewide=((rast1.lnPixWidth+63)/64)*8;
 int j=0;
 int nowbyte1;
 int nowbyte;
 for(i=(bound-comp->upper+1)>>(comp->scale);i<rast1.lnPixHeight;i++)
 {
  for(nowbyte1=i*bytewide,nowbyte=j*bytewide;nowbyte<(j+1)*bytewide;nowbyte++,nowbyte1++)
        rast.Raster[nowbyte]=rast1.Raster[nowbyte1];
  j++;
 }
 rast.lnPixHeight=rast1.lnPixHeight-((bound-comp->upper+1)>>(comp->scale));

 int real_left=0;
 int real_right=rast.lnPixWidth;

 if((GetRastWide(&rast,real_left,real_right)<<comp->scale)>inf_let_w)
	 return FALSE;

 real_left=comp->left+(real_left<<comp->scale);
 real_right=comp->left+(real_right<<comp->scale);

 if(IfHightLetters(comp,FALSE,real_left,real_right))
	 return TRUE;

 return FALSE;
}


int GetRastWide(RecRaster* rast,int& left,int& right)
{
	int wide=rast->lnPixWidth;
	int i=0;
	while(i<rast->lnPixWidth&&IfWhiteRow(rast,i))
	{
		i++;
		wide--;
		left++;
	}
	if(!wide)
		return 0;
	i=rast->lnPixWidth-1;
	while(i>=0&&IfWhiteRow(rast,i))
	{
		i--;
		wide--;
		right--;
	}
	return wide;
}


BOOL IfHightLetters(CCOM_comp* comp,BOOL Up,int real_left,int real_right)
{
  int Root_Up=comp->upper;
  int Root_Down=comp->upper+comp->h-1;
  int left=real_left;
  int right=real_right;
  int double_width=inf_let_w*3;
  BOOL fl_as_cav=FALSE;

  CCOM_comp* co=CCOM_GetFirst((CCOM_handle)HCCOM,NULL);
  while(co)
  {
   if(co->h>inf_let_h&&comp!=co)
   {
	  if(Up)
	  {
		if((co->upper+co->h>Root_Up+2)&&((co->upper+co->h+co->upper-1)/2<Root_Up))
		{
			if(left<co->left)
			{
			    ;
			}
            else
			{
				if(left-(co->left+co->w)<double_width)
					return TRUE;
			}
		}
	  }
	  else
	  {
		if((co->upper<Root_Down+(co->h)/3)&&( ((co->upper+co->h+co->upper-1)/2>Root_Down-2) || (fl_as_cav&&(co->upper+co->h-1>Root_Down)) ))
		{
		    fl_as_cav=TRUE;

			if(left<co->left)
			{
			    if(co->left-right<double_width)
					return TRUE;
			}
            else
			{
				if(left-(co->left+co->w)<double_width)
					return TRUE;
			}
		}
	  }
   }
   co=CCOM_GetNext(co,NULL);
 }
 return FALSE;
}
-*/

int GetCountNumbers(int num)
{
	int count=0;
	if(num==0)
		return 1;
	for(num;num>0;num=num/10)
		count++;
	return count;
}

void StrDrawRect(Handle wnd,Word32 OperCode,Word32 color,int top,int bottom,int left,int right)
{

	Rect16 Rect;

		Rect.bottom=bottom;
		Rect.left=left;
		Rect.right=right+1;
		Rect.top=top-1;

		LDPUMA_DrawRect (wnd, &(Rect),0, color, -100,OperCode );

}

/*Andrey: moved to RCUTP
//----------------------
int ExBound(CCOM_comp* comp,int bound)
{
 RecRaster rast;
 int i;
 int j;
 if(!CCOM_GetRaster(comp,&rast))
	 return bound;

 int rast_bound=(bound-comp->upper+1)>>(comp->scale); //rast_height of first let
 int delta=inf_let_h/3+1;
 int mini_w=10000;
 int new_rast_bound=rast_bound;
 int new_rast_bound_up=rast_bound;
 int left_bound;
 int right_bound;
 int bytewide=((rast.lnPixWidth+63)/64)*8;
 Word8 bytep;
 BOOL Not_move_down=FALSE;

 if(rast_bound+delta>rast.lnPixHeight||rast_bound-delta<0)
	 return bound;

 for(i=rast_bound-delta;i<=rast_bound+delta;i++)
 {
   left_bound=0;
   for(j=0;j<rast.lnPixWidth;j++)
   {
	if(!(j%8))
    bytep=rast.Raster[i*bytewide+(j>>3)];
	if(bytep&(128>>(j%8)))
		break;
	else
		left_bound++;
   }
   bytep=rast.Raster[i*bytewide+((rast.lnPixWidth-1)>>3)];
   right_bound=rast.lnPixWidth-1;
   for(j=rast.lnPixWidth-1;j>=0;j--)
   {
	if((j%8)==7)
        bytep=rast.Raster[i*bytewide+(j>>3)];
	if(bytep&(128>>(j%8)))
		break;
	else
		right_bound--;
   }
   if(right_bound-left_bound+1<mini_w)
   {
	   mini_w=right_bound-left_bound+1;
	   new_rast_bound_up=i;
   }
   if(right_bound-left_bound+1<=mini_w)
   {
	 if(!Not_move_down)
	 {
	   mini_w=right_bound-left_bound+1;
	   new_rast_bound=i;
	 }
   }
   else
   {
	   if(right_bound-left_bound+1>mini_w*2)
		   Not_move_down=TRUE;
   }
 }

 int upper=-1;
 int downer=-1;
 int str_bound=GetBoundFromStr(comp,FALSE,upper,downer);
 if(count_cut_let==2&&(abs(str_bound-bound)>=(inf_let_h*3)/4||downer-upper>=inf_let_h))
	 str_bound=bound;

 int rast_str_bound=(str_bound-comp->upper+1)>>(comp->scale);
 int bound_ret=bound;

   left_bound=0;
   i=rast_str_bound;
   for(j=0;j<rast.lnPixWidth;j++)
   {
	if(!(j%8))
    bytep=rast.Raster[i*bytewide+(j>>3)];
	if(bytep&(128>>(j%8)))
		break;
	else
		left_bound++;
   }
   bytep=rast.Raster[i*bytewide+((rast.lnPixWidth-1)>>3)];
   right_bound=rast.lnPixWidth-1;
   for(j=rast.lnPixWidth-1;j>=0;j--)
   {
	if((j%8)==7)
        bytep=rast.Raster[i*bytewide+(j>>3)];
	if(bytep&(128>>(j%8)))
		break;
	else
		right_bound--;
   }
 i=right_bound-left_bound+1;

 new_rast_bound=(new_rast_bound+new_rast_bound_up)/2;
 if((mini_w<<(comp->scale))<sup_dust_w)
 {
	 if(rast_bound<new_rast_bound)
		 bound_ret+=((new_rast_bound-rast_bound)<<(comp->scale));
	 else
		 bound_ret-=((rast_bound-new_rast_bound)<<(comp->scale));
	 if((mini_w>=min_cut_down_let_w)&&((new_rast_bound>rast_bound&&count_cut_let!=2)||(new_rast_bound>rast_str_bound&&count_cut_let==2)))
	      NotCutDownLetter(&rast,bound_ret,rast_bound,new_rast_bound,delta,mini_w,comp->scale);
     if(bound_ret>bound&&count_cut_let==2&&(i<=mini_w+1||downer<bound_ret))
	 {
		 bound_ret=(bound_ret+str_bound)/2;
	 }
 }
 else
 {
	 if(count_cut_let==2&&(i<=mini_w+1||downer<bound))
	     bound_ret=(bound_ret+str_bound)/2;
 }
 return bound_ret;
}


void NotCutDownLetter(RecRaster* rast,int& bound,int rast_bound,int new_rast_bound,int delta,int mini_w,int scale)
{
 int left_bound;
 int right_bound;
 Word8 bytep;
 int bytewide=((rast->lnPixWidth+63)/64)*8;
 BOOL fl_cut=FALSE;
 BOOL fl_break=FALSE;
 int oldleft;
 int oldright;
 int min_wide=10000;
 int max_wide=0;
 int i;
 int j;

   for(oldleft=0;oldleft<rast->lnPixWidth;oldleft++)
   {
	if(!(oldleft%8))
    bytep=rast->Raster[new_rast_bound*bytewide+(oldleft>>3)];
	if(bytep&(128>>(oldleft%8)))
		break;
   }
   bytep=rast->Raster[new_rast_bound*bytewide+((rast->lnPixWidth-1)>>3)];
   oldright=rast->lnPixWidth-1;
   for(oldright=rast->lnPixWidth-1;oldright>=0;oldright--)
   {
	if((oldright%8)==7)
        bytep=rast->Raster[new_rast_bound*bytewide+(oldright>>3)];
	if(bytep&(128>>(oldright%8)))
		break;
   }


 for(i=new_rast_bound-1;i>=rast_bound-delta;i--)
 {
   left_bound=0;
   right_bound=rast->lnPixWidth-1;
   for(j=oldleft-1;j<=oldright+1;j++)
   {
	   if( rast->Raster[i*bytewide+(j>>3)]&(128>>(j%8)) )
	   {
		   left_bound=right_bound=j;
	   }
   }
   bytep=rast->Raster[i*bytewide+(right_bound>>3)];
   for(j=right_bound;j<rast->lnPixWidth;j++)
   {
	if(!(j%8))
    bytep=rast->Raster[i*bytewide+(j>>3)];
	if(!(bytep&(128>>(j%8))))
		break;
	else
		right_bound++;
   }
   bytep=rast->Raster[i*bytewide+(left_bound>>3)];
   for(j=left_bound;j>=0;j--)
   {
	if((j%8)==7)
        bytep=rast->Raster[i*bytewide+(j>>3)];
	if(!(bytep&(128>>(j%8))))
		break;
	else
		left_bound--;
   }
   if(right_bound-left_bound+1>=inf_let_h)
   {
	 if(!fl_cut&&right_bound-left_bound+1>2*mini_w)
	 {
         if(min_wide>right_bound-left_bound+1)
			 min_wide=right_bound-left_bound+1;
		 if(max_wide<right_bound-left_bound+1)
			 max_wide=right_bound-left_bound+1;

		 fl_cut=TRUE;
	 }
   }
   else
   {
	   if(fl_cut)
		   fl_break=TRUE;
   }
   if(right_bound-left_bound+1>mini_w)
	   mini_w=right_bound-left_bound+1;

   if(fl_break)
	   break;
 }

 //дырка?

 left_bound=0;
 right_bound=rast->lnPixWidth-1;
 for(j=oldleft-1;j<=oldright+1;j++)
 {
	if( rast->Raster[i*bytewide+(j>>3)]&(128>>(j%8)) )
	{
		left_bound=right_bound=j;
	}
 }
 bytep=rast->Raster[i*bytewide+(right_bound>>3)];
 for(j=right_bound;j<rast->lnPixWidth;j++)
 {
  if(!(j%8))
    bytep=rast->Raster[i*bytewide+(j>>3)];
  if(!(bytep&(128>>(j%8))))
	 break;
  else
	 right_bound++;
 }
 bytep=rast->Raster[i*bytewide+(left_bound>>3)];
 for(j=left_bound;j>=0;j--)
 {
  if((j%8)==7)
     bytep=rast->Raster[i*bytewide+(j>>3)];
  if(!(bytep&(128>>(j%8))))
	break;
  else
	left_bound--;
 }
 for(;left_bound<=right_bound;left_bound++)
 {
	 bytep=rast->Raster[i*bytewide+(left_bound>>3)];
	 if(!(bytep&(128>>(left_bound%8))))
		 return;
 }

 if(fl_cut&&max_wide-min_wide<=2)
 {
	 bound-=((new_rast_bound-i)<<scale);
 }
}


BOOL NotNeedCut(CCOM_comp* comp)
{
 CCOM_comp Test;
 CCOM_comp Test_Next;

 int bound=comp->upper+(comp->h)/count_cut_let;

 Test.upper=comp->upper;
 Test.left=Test_Next.left=comp->left;
 Test.w=Test_Next.w=comp->w;
 Test.h=bound-Test.upper+1;
 Test_Next.upper=bound+1;
 Test_Next.h=comp->h-Test.h;

 BOOL ret1=FALSE;
 BOOL ret2=FALSE;

 if(IfNearLetters(comp,&Test))
	ret1=TRUE;
 if(count_cut_let==2)
 {
  if(IfNearLetters(comp,&Test_Next))
	ret2=TRUE;
 }
 else
	 ret2=TRUE;


 return (!(ret1&&ret2));
}
-*/

void IfDifCut(void)
{
 if(!f_old_cut||!f_temp_cut)
	 return;

 char buf1[10000];
 char buf2[10000];
 fgets(buf1,10000,f_temp_cut);
 BOOL fl_exist=FALSE;

 while(fgets(buf2,10000,f_old_cut))
 {
	 if(IfEqv(buf1,buf2))
		 fl_exist=TRUE;
	 if(fl_exist)
		 break;
 }
 if(!fl_exist)
	 return;

 Rect16 pOldCut[1000];
 int nOldCut=0;
 Rect16 pNewCut[1000];
 int nNewCut=0;
 BOOL fl_empty=FALSE;

 if(!fgets(buf2,10000,f_old_cut))
    fl_empty=TRUE;

 int i;
 for(i=999;i>=0;i--)
 {
	 pNewCut[i].bottom=pOldCut[i].bottom=0;
	 pNewCut[i].top=pOldCut[i].top=0;
	 pNewCut[i].left=pOldCut[i].left=0;
	 pNewCut[i].right=pOldCut[i].right=0;
 }

 while((buf2[1]!=':')&&(buf2[2]!='\\')&&(!fl_empty))
 {
	 i=4;
	 while(buf2[i]!=' ')
	 {
	   pOldCut[nOldCut].top*=10;
       if(buf2[i]!='0')
		  pOldCut[nOldCut].top+=buf2[i]-'1'+1;
	   i++;
	 }
	 i=18;
     while(buf2[i]!=' ')
	 {
	   pOldCut[nOldCut].bottom*=10;
       if(buf2[i]!='0')
		  pOldCut[nOldCut].bottom+=buf2[i]-'1'+1;
	   i++;
	 }
	 i=30;
	 while(buf2[i]!=' ')
	 {
	   pOldCut[nOldCut].left*=10;
       if(buf2[i]!='0')
		  pOldCut[nOldCut].left+=buf2[i]-'1'+1;
	   i++;
	 }
	 i=43;
	 while(buf2[i]!=' '&&buf2[i]!='\n'&&buf2[i]!='\0')
	 {
	   pOldCut[nOldCut].right*=10;
       if(buf2[i]!='0')
		  pOldCut[nOldCut].right+=buf2[i]-'1'+1;
	   i++;
	 }
	 nOldCut++;

  if(!fgets(buf2,10000,f_old_cut))
	  break;
 }

 fl_empty=FALSE;
 if(!fgets(buf1,10000,f_temp_cut))
    fl_empty=TRUE;


 while((buf1[1]!=':')&&(buf1[2]!='\\')&&(!fl_empty))
 {
	 i=4;
	 while(buf1[i]!=' ')
	 {
	   pNewCut[nNewCut].top*=10;
       if(buf1[i]!='0')
		  pNewCut[nNewCut].top+=buf1[i]-'1'+1;
	   i++;
	 }
	 i=18;
     while(buf1[i]!=' ')
	 {
	   pNewCut[nNewCut].bottom*=10;
       if(buf1[i]!='0')
		  pNewCut[nNewCut].bottom+=buf1[i]-'1'+1;
	   i++;
	 }
	 i=30;
	 while(buf1[i]!=' ')
	 {
	   pNewCut[nNewCut].left*=10;
       if(buf1[i]!='0')
		  pNewCut[nNewCut].left+=buf1[i]-'1'+1;
	   i++;
	 }
	 i=43;
	 while(buf1[i]!=' '&&buf1[i]!='\n'&&buf1[i]!='\0')
	 {
	   pNewCut[nNewCut].right*=10;
       if(buf1[i]!='0')
		  pNewCut[nNewCut].right+=buf1[i]-'1'+1;
	   i++;
	 }
	 nNewCut++;

  if(!fgets(buf1,10000,f_temp_cut))
	  break;
 }

 if(!nOldCut&&!nNewCut)
	 return;

 int j;
// старая резка:
 for(i=nOldCut-1;i>=0;i--)
 {
  for(j=nNewCut-1;j>=0;j--)
  {
   if(IfEqv(pOldCut[i],pNewCut[j]))
	   break;
  }
  if(j<0)
  {
	StrDrawRect(MainWindowD,code_str_cut_d,RGB(0,0,255),pOldCut[i].top,pOldCut[i].bottom,pOldCut[i].left,pOldCut[i].right);
    WasDif=TRUE;
  }
 }
// новая резка:
 for(i=nNewCut-1;i>=0;i--)
 {
  for(j=nOldCut-1;j>=0;j--)
  {
   if(IfEqv(pNewCut[i],pOldCut[j]))
	   break;
  }
  if(j<0)
  {
	StrDrawRect(MainWindowD,code_str_cut_d,RGB(255,0,0),pNewCut[i].top,pNewCut[i].bottom,pNewCut[i].left,pNewCut[i].right);
    WasDif=TRUE;
  }
 }

}

BOOL IfEqv(char* buf1,char* buf2)
{
	int i=0;
	while(buf1[i]!='\0'&&buf2[i]!='\0'&&buf1[i]==buf2[i])
		i++;
	if(buf1[i]==buf2[i])
		return TRUE;
	return FALSE;
}

BOOL IfEqv(Rect16 r1,Rect16 r2)
{
	if(r1.top!=r2.top)
		return FALSE;
	if(r1.bottom!=r2.bottom)
		return FALSE;
	if(r1.left!=r2.left)
		return FALSE;
	if(r1.right!=r2.right)
		return FALSE;
	return TRUE;
}

BOOL AddLenBlockMas(POLY_** ppRc,int& len,int add)
{
 POLY_ *dop;
 int i;
 if(!(InitBlockMas(&dop,len)) )
	 return FALSE;
 for(i=0;i<len;i++)
 {
	 for(int j=0;j<(*ppRc)[i].com.count;j++)
	 {
	  dop[i].com.Vertex[j].x=(*ppRc)[i].com.Vertex[i].x;
	  dop[i].com.Vertex[j].y=(*ppRc)[i].com.Vertex[i].y;
	 }
 }

 DelBlockMas(*ppRc);
 if(!(InitBlockMas(ppRc,len+add)) )
 {   (*ppRc)=dop;
	 return FALSE;
 }

 for(i=0;i<len;i++)
 {
	 for(int j=0;j<(*ppRc)[i].com.count;j++)
	 {
	  (*ppRc)[i].com.Vertex[i].x=dop[i].com.Vertex[j].x;
	  (*ppRc)[i].com.Vertex[i].y=dop[i].com.Vertex[j].y;
	 }
 }

 len+=add;
 DelBlockMas(dop);
 return TRUE;
}

void DelBlockMas(POLY_ *masp)
{
	delete[] masp;
}

BOOL InitBlockMas(POLY_** ppRc,int len)
{
	(*ppRc)=NULL;
	if(!((*ppRc)=new POLY_[len]))
		return FALSE;
	return TRUE;
}
