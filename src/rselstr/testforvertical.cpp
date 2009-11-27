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

#include <stdlib.h>
#include <string.h>
#include "puttocstr.h"
#include "rselstr.h"
#include "rneg/rneg.h"


extern int32_t my_top;
extern int32_t my_left;
extern int32_t my_right;
extern int32_t my_bottom;

extern int inf_let_h;
extern int min_let_w;
extern int min_let_h;
extern int inf_betw_str_h;

extern int DPIX;
extern int DPIY;


CCOM_handle hCCOM_new;




RSELSTR_FUNC(Bool32) RSELSTR_TestForVertical(CCOM_handle hCCOM,Handle hCPage,Rect16 RC,Bool32 neg,int param)
{
 if(RC.top<0||RC.left<0||RC.bottom<=RC.top||RC.right<=RC.left)
	 return FALSE;

// if(!prelist)
//	 return FALSE;

// LDPUMA_ConsoleN("Активизируется CHSTR");
 int TIME=clock();
 int code_strd=1;
 int16_t* begx=NULL;
 int16_t* movey=NULL;
 int* hi=NULL;
 uchar* flmovey=NULL;

 MainWindowD=NULL;
 MainWindowD=LDPUMA_GetWindowHandle ("Изображение после разворота");
 if(!MainWindowD)
    MainWindowD=LDPUMA_GetWindowHandle ("Main");

 inf_str_h=15;
 inf_str_w=8;
 inf_let_h=10;
 min_let_w=2;
 min_let_h=2;
 inf_betw_str_h=10;

 PAGEINFO info = {0};
 GetPageInfo(hCPage,&info);

 int skew=info.Incline2048;

 DPIX=info.DPIX;
 DPIY=info.DPIY;
 uchar ImageName[CPAGE_MAXNAME];


 inf_str_h=inf_str_h*((int)(DPIY)+1)/300;
 inf_str_w=inf_str_w*((int)(DPIY)+1)/300;
 inf_let_h=inf_let_h*((int)(DPIY)+1)/300;
 inf_betw_str_h=inf_betw_str_h*((int)(DPIY)+1)/300;
 min_let_w=min_let_w*((int)(DPIY)+1)/300;
 min_let_h=min_let_h*((int)(DPIY)+1)/300;

 uchar* pmaspold;
 uchar* pmasp;
 uchar Data[1000];
 memset (Data, 0, sizeof (Data));


 CHstr* my_str=new CHstr;
 my_str->left=RC.left;
 my_str->top=RC.top;
 my_str->h=RC.bottom-RC.top+1;
 my_str->w=RC.right-RC.left+1;
 my_str->neg=neg;

  if(my_str->ImageName)
  {
	  for(int i=0;i<CPAGE_MAXNAME;i++)
		  ImageName[i]=my_str->ImageName[i];
  }
  else
  {
	  for(int i=0;i<CPAGE_MAXNAME;i++)
		  ImageName[i]=info.szImageName[i];
  }


 Rect16 Rc;
 Rect16 unrotRc;
 CCOM_comp* comp=NULL;
 Bool32 ret;
 Bool32 fl_rotate;
 Rect16* pRc = NULL;
 int nRc=1;
 int len_mas=10;
 int add_len_mas=5;
// int time;

 CCOM_handle hStrCCOM;
 if(my_str->neg)
	 hStrCCOM=GetStrCCOM(hCPage,ImageName,RC,TRUE,TRUE);
 else
	 hStrCCOM=CreateContainer(hCPage,hCCOM,RC);

 if(!hStrCCOM)
	 return FALSE;

//Резка на строки по гипотезе вертикальности

  my_str->vertical=TRUE;
  Rc.top=my_str->top;
  Rc.bottom=my_str->top+my_str->h-1;
  Rc.left=my_str->left;
  Rc.right=my_str->left+my_str->w-1;
  nRc=1;
  len_mas=10;
  ret=InitStrMas(&pRc,len_mas);
  if(!ret)
  {
	if(hStrCCOM)
	  CCOM_DeleteContainer(hStrCCOM);
	delete my_str;
       return FALSE;
  }

  pRc[0].bottom=Rc.bottom;
  pRc[0].top=Rc.top;
  pRc[0].right=Rc.right;
  pRc[0].left=Rc.left;


  pmaspold = Data;
  ret=GetMasP(hCPage,ImageName,Rc,&pmaspold);
  if(!ret)
  {
	if(hStrCCOM)
	  CCOM_DeleteContainer(hStrCCOM);
  // GF: killed memory leak on 2004.01.29
	DelStrMas(&pRc); pRc = NULL;
	delete my_str;
       return FALSE;
  }

  int w=Rc.right-Rc.left+1;
  int h=Rc.bottom-Rc.top+1;
  pmasp=NULL;
  if(!(pmasp= new uchar[w*h/8+w+h]))
  {
   // GF: killed memory leak on 2004.01.29
	 DelStrMas(&pRc); pRc = NULL;
	 delete my_str;
	 ret=FALSE;
  }
  if(!ret)
  {
	if(hStrCCOM)
	  CCOM_DeleteContainer(hStrCCOM);
       return FALSE;
  }


  ToHorizont(pRc,pmasp,pmaspold);
  if(my_str->neg)
      Invert(pRc,pmasp);

  unrotRc.top=pRc[0].top;
  unrotRc.bottom=pRc[0].bottom;
  unrotRc.left=pRc[0].left;
  unrotRc.right=pRc[0].right;


  if(!InitRotateMas(pRc[0],&begx,&movey,&flmovey))
	  fl_rotate=FALSE;
  else
  {
//   time=clock();
   fl_rotate=RSELSTR_RotateRaster(pmasp,-skew,&(pRc[0]),begx,movey,flmovey,inf_betw_str_h);
//   if(fl_rotate)
//   LDPUMA_ConsoleN("Time rotate=%d",clock()-time);
  }

  int oldleft=pRc[0].top;
  if(!(RSELSTR_CutStr(&pRc,nRc,len_mas,pmasp,DPIX,DPIY,0)) )
  {
   // GF: killed memory leak on 2004.01.29
	 DelStrMas(&pRc); pRc = NULL;
	 delete[] pmasp;
	 delete my_str;
	 DeleteRotateMas(begx,movey,flmovey,hi);
	 ret=FALSE;
  }
  if(!ret)
  {
	if(hStrCCOM)
	  CCOM_DeleteContainer(hStrCCOM);
       return FALSE;
  }
  my_str->left-=oldleft-pRc[nRc-1].top;
  CCOM_handle HCCOM;
  if(!hStrCCOM)
	  HCCOM=hCCOM;
  else
	  HCCOM=hStrCCOM;
/*
  if(IfBadCut((Handle)HCCOM,pRc,Rc,nRc,pmasp,my_str))
  {
	 DelStrMas(pRc);
	 delete[] pmasp;
	 delete my_str;
	 DeleteRotateMas(begx,movey,flmovey,hi);
	 ret=FALSE;
  }
  if(!ret)
  {
	  CCOM_DeleteContainer(hStrCCOM);
       return FALSE;
  }
*/

//Первая проверка прошла, надо сохранить информацию
  CPrepHstr* temp=new CPrepHstr;
  temp->begx=begx;
  temp->fl_rotate=fl_rotate;
  temp->flmovey=flmovey;
  temp->h=my_str->h;
  temp->hi=hi;
  temp->hStrCCOM=hStrCCOM;
  if(my_str->ImageName)
  {
   int i;
   for(i=0;i<CPAGE_MAXNAME&&my_str->ImageName[i]!='\0';i++)
	  temp->ImageName[i]=my_str->ImageName[i];
   temp->ImageName[i]='\0';
  }
//  temp->ImageName=my_str->ImageName;
//  my_str->ImageName=NULL;
  temp->left=my_str->left;
  temp->oldleft=oldleft;
  temp->m_num_frag=my_str->m_num_frag;
  temp->movey=movey;
  temp->neg=my_str->neg;
  temp->nRc=nRc;
  temp->pRc=pRc;
  temp->top=my_str->top;
  temp->unrotRc.bottom=unrotRc.bottom;
  temp->unrotRc.top=unrotRc.top;
  temp->unrotRc.left=unrotRc.left;
  temp->unrotRc.right=unrotRc.right;
  temp->vertical=TRUE;
  temp->w=my_str->w;




//Резка на строки по гипотезе горизонтальности

  my_str->vertical=FALSE;
  nRc=1;
  len_mas=10;
  pRc=NULL;
  begx=NULL;
  movey=NULL;
  hi=NULL;
  flmovey=NULL;


  ret=InitStrMas(&pRc,len_mas);
  if(!ret)
  {
	   delete temp;
	   delete my_str;
       return FALSE;
  }

  pRc[0].bottom=Rc.bottom;
  pRc[0].top=Rc.top;
  pRc[0].right=Rc.right;
  pRc[0].left=Rc.left;


  pmaspold = Data;
  ret=GetMasP(hCPage,ImageName,Rc,&pmaspold);
  if(!ret)
  {
	   delete temp;
	   delete my_str;
     // GF: killed memory leak on 2004.01.29
	   DelStrMas(&pRc); pRc = NULL;
       return FALSE;
  }


  CopyP(pRc,pmasp,pmaspold);
  if(my_str->neg)
      Invert(pRc,pmasp);

  unrotRc.top=pRc[0].top;
  unrotRc.bottom=pRc[0].bottom;
  unrotRc.left=pRc[0].left;
  unrotRc.right=pRc[0].right;


  if(!InitRotateMas(pRc[0],&begx,&movey,&flmovey))
	  fl_rotate=FALSE;
  else
  {
//   time=clock();
   fl_rotate=RSELSTR_RotateRaster(pmasp,-skew,&(pRc[0]),begx,movey,flmovey,inf_betw_str_h);
//   if(fl_rotate)
//    LDPUMA_ConsoleN("Time rotate=%d",clock()-time);
  }


  if(!(RSELSTR_CutStr(&pRc,nRc,len_mas,pmasp,DPIX,DPIY,0)) )
  {
   // GF: killed memory leak on 2004.01.29
	 DelStrMas(&pRc); pRc = NULL;
	 delete[] pmasp;
	 delete my_str;
	 delete temp;
	 DeleteRotateMas(begx,movey,flmovey,hi);
	 ret=FALSE;
  }
  if(!ret)
  {
       return FALSE;
  }

/*
  if(IfBadCut((Handle)HCCOM,pRc,Rc,nRc,pmasp,my_str))
  {
//Прошла тривиальная проверка: добавляем приготовленный фрагмент

	 if(temp->fl_rotate)
	 {
      InitRotateMas(&(temp->hi),temp->nRc);
      RSELSTR_UnRotateRect(-skew,temp->pRc,temp->nRc,temp->unrotRc,temp->begx,temp->movey,temp->flmovey,temp->hi);
	 }
	 DelStrMas(pRc);
	 delete[] pmasp;
	 delete my_str;
	 DeleteRotateMas(begx,movey,flmovey,hi);
	 if(AddToPrepList(temp))
	 {
		 delete temp;
//		 LDPUMA_ConsoleN("Time work CHSTR=%d",clock()-TIME);
         return TRUE;
	 }
	 else
	 {
		 delete temp;
		 return FALSE;
	 }
  }
*/

//Нетривиальный анализ структуры
  if(MainVertDiagnostic(hCPage,hCCOM,temp,pRc,nRc,param))
  {
     if(temp->fl_rotate)
	 {
      InitRotateMas(&(temp->hi),temp->nRc);
      RSELSTR_UnRotateRect(-skew,temp->pRc,temp->nRc,temp->unrotRc,temp->begx,temp->movey,temp->flmovey,temp->hi);
	 }
   // GF: killed memory leak on 2004.01.29
	 DelStrMas(&pRc); pRc = NULL;
	 delete[] pmasp;
	 delete my_str;
	 DeleteRotateMas(begx,movey,flmovey,hi);
	 if(/*AddToPrepList(temp)*/AddToCpage(hCPage,temp))
	 {
		 delete temp;
//		 LDPUMA_ConsoleN("Time work CHSTR=%d",clock()-TIME);
         return TRUE;
	 }
	 else
	 {
		 delete temp;
		 return FALSE;
	 }
  }
  else
  {
   // GF: killed memory leak on 2004.01.29
	 DelStrMas(&pRc); pRc = NULL;
	 delete[] pmasp;
	 delete my_str;
	 DeleteRotateMas(begx,movey,flmovey,hi);
	 delete temp;
     return FALSE;
  }

// LDPUMA_ConsoleN("Time work CHSTR=%d",clock()-TIME);
}


CCOM_handle CreateContainer(Handle hCPage,CCOM_handle hCCOM_old,Rect16 Rc)
{
 hCCOM_new=0;
 my_top=Rc.top;
 my_left=Rc.left;
 my_right=Rc.right;
 my_bottom=Rc.bottom;


 hCCOM_new = CCOM_CreateContainer();
 if(!hCCOM_new)
    return 0;

 CCOM_comp* comp=NULL;
 CCOM_comp* new_comp;
 comp=CCOM_GetFirst(hCCOM_old,Filter);

  while(comp)
  {
	if(my_top-comp->upper<(comp->h>>1)&&comp->upper+comp->h-1-my_bottom<(comp->h>>1)&&my_left-comp->left<(comp->w>>1)&&comp->left+comp->w-1-my_right<(comp->w>>1))
	{
	   new_comp=CCOM_New(hCCOM_new,comp->upper,comp->left,comp->w,comp->h);
	   if(new_comp)
	   {
		if(comp->size_linerep<0)
			;
		else
		{
         if(!CCOM_Copy(new_comp,comp) )
		       CCOM_Delete(hCCOM_new,comp);
		}
	   }

	}
	  comp=CCOM_GetNext(comp,Filter);
  }


  return hCCOM_new;
}

Bool AddToCpage(Handle hCPAGE,CPrepHstr* temp)
{
 int size_phstr=sizeof(CPrepHstr);
 int i;
 if(temp->nRc>MAX_STR_COUNT)
	 temp->nRc=MAX_STR_COUNT;

 for(i=0;i<temp->nRc;i++)
 {
	 temp->prc[i].bottom=temp->pRc[i].bottom;
	 temp->prc[i].top=temp->pRc[i].top;
	 temp->prc[i].left=temp->pRc[i].left;
	 temp->prc[i].right=temp->pRc[i].right;
 }
 if(temp->fl_rotate)
 {
	 for(i=0;i<temp->nRc;i++)
		 temp->phi[i]=temp->hi[i];
 }

 if(CPAGE_CreateBlock(hCPAGE,TYPE_RSELSTR_TEMP_PHSTR,0,0,temp,size_phstr))
 {
  temp->hStrCCOM=0;
  return TRUE;
 }
 else
	 return FALSE;
}

Bool InitPrepList(Handle hCPAGE)
{
 prelist=NULL;
 prelist=new CLPrepHstr;
 if(!prelist)
	 return FALSE;

 Handle pBlock;
 Handle pBlock_prev;
 int size_phstr=sizeof(CPrepHstr);
 int size_neg=sizeof(NegTemp);
 NegTemp neg_tmp;

 pBlock=CPAGE_GetBlockFirst(hCPAGE,TYPE_RSELSTR_TEMP_PHSTR);
 while(pBlock)
 {
	 pBlock_prev=pBlock;
	 CPrepHstr* add=prelist->Add();
	 CPrepHstr* temp=add->next;
     if(add)
	 {
		 CPAGE_GetBlockData(hCPAGE,pBlock,TYPE_RSELSTR_TEMP_PHSTR,add,size_phstr);

		 add->begx=NULL;
		 add->flmovey=NULL;
		 add->movey=NULL;
		 add->hi=NULL;
		 add->pRc=NULL;
		 add->next=temp;

		 add->pRc=new Rect16[add->nRc];
		 if(!add->pRc)
			 add->nRc=0;
		 for(int i=0;i<add->nRc;i++)
		 {
			 add->pRc[i].bottom=add->prc[i].bottom;
			 add->pRc[i].top=add->prc[i].top;
			 add->pRc[i].left=add->prc[i].left;
			 add->pRc[i].right=add->prc[i].right;
		 }
		 if(add->fl_rotate&&add->nRc)
		 {
			 add->hi=new int[add->nRc];
			 if(!add->hi)
				 add->nRc=0;
			 for(int i=0;i<add->nRc;i++)
				 add->hi[i]=add->phi[i];
		 }

	 }
	 pBlock=CPAGE_GetBlockNext(hCPAGE,pBlock,TYPE_RSELSTR_TEMP_PHSTR);
	 CPAGE_DeleteBlock(hCPAGE,pBlock_prev);
 }

 pBlock=CPAGE_GetBlockFirst(hCPAGE,TYPE_RNEG_TEMP_PHSTR);
 while(pBlock)
 {
	 pBlock_prev=pBlock;
	 CPrepHstr* add=prelist->Add();
	 CPrepHstr* temp=add->next;
     if(add)
	 {
		 CPAGE_GetBlockData(hCPAGE,pBlock,TYPE_RNEG_TEMP_PHSTR,&neg_tmp,size_neg);

		 add->begx=NULL;
		 add->flmovey=NULL;
		 add->movey=NULL;
		 add->hi=NULL;
		 add->pRc=NULL;
		 add->next=temp;

		 add->pRc=new Rect16[neg_tmp.nRc];
		 if(!add->pRc)
			 add->nRc=0;
		 else
			 add->nRc=neg_tmp.nRc;
		 for(int i=0;i<add->nRc;i++)
		 {
			 add->pRc[i].bottom=neg_tmp.prc[i].bottom;
			 add->pRc[i].top=neg_tmp.prc[i].top;
			 add->pRc[i].left=neg_tmp.prc[i].left;
			 add->pRc[i].right=neg_tmp.prc[i].right;
		 }
		 add->fl_rotate=neg_tmp.fl_rotate;
		 if(add->fl_rotate&&add->nRc)
		 {
			 add->hi=new int[add->nRc];
			 if(!add->hi)
				 add->nRc=0;
			 for(int i=0;i<add->nRc;i++)
				 add->hi[i]=neg_tmp.phi[i];
		 }
		 add->h=neg_tmp.h;
		 add->hStrCCOM=neg_tmp.hStrCCOM;
		 add->left=neg_tmp.left;
		 add->neg=TRUE;
		 add->oldleft=neg_tmp.oldleft;
		 add->top=neg_tmp.top;
		 add->vertical=neg_tmp.vertical;
		 add->w=neg_tmp.w;
	 }
	 pBlock=CPAGE_GetBlockNext(hCPAGE,pBlock,TYPE_RNEG_TEMP_PHSTR);
	 CPAGE_DeleteBlock(hCPAGE,pBlock_prev);
 }
 return TRUE;
}


Bool AddToPrepList(CPrepHstr* temp)
{
 CPrepHstr* add=prelist->Add();
 if(!add)
	 return FALSE;

// add->begx=temp->begx;
 add->fl_rotate=temp->fl_rotate;
// add->flmovey=temp->flmovey;
 add->h=temp->h;
 add->hi=temp->hi;
 add->hStrCCOM=temp->hStrCCOM;
 for(int i=0;i<CPAGE_MAXNAME;i++)
	 add->ImageName[i]=temp->ImageName[i];
// add->ImageName=temp->ImageName;
 add->left=temp->left;
 add->oldleft=temp->oldleft;
 add->m_num_frag=temp->m_num_frag;
// add->movey=temp->movey;
 add->neg=temp->neg;
 add->nRc=temp->nRc;
 add->pRc=temp->pRc;
 add->top=temp->top;
 add->unrotRc.bottom=temp->unrotRc.bottom;
 add->unrotRc.top=temp->unrotRc.top;
 add->unrotRc.left=temp->unrotRc.left;
 add->unrotRc.right=temp->unrotRc.right;
 add->vertical=temp->vertical;
 add->w=temp->w;
// temp->begx=NULL;
// temp->flmovey=NULL;
 temp->hi=NULL;
 temp->hStrCCOM=0;
// temp->ImageName=NULL;
// temp->movey=NULL;
 temp->pRc=NULL;

 return TRUE;
}

Bool32 Filter(int32_t upper, int32_t left, int32_t w, int32_t h)
{
	if( (upper+(h>>1)<my_top) ||
		(left+(w>>1)<my_left) ||
		(upper+(h>>1)>=my_bottom) ||
		(left+(w>>1)>=my_right) )
	{
		return FALSE;
	}

	return TRUE;
}

/*
Bool IfBadCut(Handle hCCOM,Rect16* pRc,Rect16 Rc,int nRc,uchar* pmasp,CHstr* my_str)
{
 if(my_str->neg)
	 return FALSE;

 if(nRc==0)
	 return TRUE;

 int num=0;

 int medium_size=GetMediumSize((CCOM_handle)hCCOM,Rc,num,my_str->vertical);
 int max_size=GetMaxSize((CCOM_handle)hCCOM,Rc,num,my_str->vertical);

 if(my_str->vertical)
 {
  if(num<=nRc)
  {
	  my_str->vertical=FALSE;
	  return TRUE;
  }
 }

 int* h_top= new int[nRc];
 int* h_bottom= new int[nRc];

 int incl_factor;
 if(my_str->vertical)
	 incl_factor=(Rect.right-Rect.+1)*abs(skew)/2048;
 else
	 incl_factor=(Rect.right-Rect.left+1)*abs(skew)/2048;

 for(int i=0;i<nRc;i++)
 {
	 GetStrBounds((CCOM_handle)hCCOM,pRc,nRc,i,&(h_top[i]),&(h_bottom[i]),my_str->vertical,Rc);
 }

 for(int i=0;i<nRc;i++)
 {
  if(h_bottom[i]-h_top[i]>3*max_size)
  {
	  my_str->vertical=FALSE;
	  delete[] h_top;
	  delete[] h_bottom;
	  return TRUE;
  }
 }

 delete[] h_top;
 delete[] h_bottom;


 return FALSE;
}
*/

int GetMediumSize(CCOM_handle hCCOM,Rect16 Rc,int& num,Bool vert)
{
	int sum_size=0;
	num=0;

	my_top=Rc.top;
	my_bottom=Rc.bottom;
	my_left=Rc.left;
	my_right=Rc.right;

	CCOM_comp* comp=CCOM_GetFirst(hCCOM,Filter);

    while(comp)
	{
		if(comp->w>(inf_let_h>>1)&&comp->h>(inf_let_h>>1)&&(comp->h>inf_let_h||comp->w>inf_let_h))
		{
         if(vert)
             sum_size+=comp->w;
		 else
             sum_size+=comp->h;
		 num++;
		}
		comp=CCOM_GetNext(comp,Filter);
	}

	if(!num)
		return ((inf_let_h*3)>>1);
	else
		return sum_size/num;
}

int GetMaxSize(CCOM_handle hCCOM,Rect16 Rc,int& num,Bool vert)
{
	int max_size=0;
	num=0;

	my_top=Rc.top;
	my_bottom=Rc.bottom;
	my_left=Rc.left;
	my_right=Rc.right;

	CCOM_comp* comp=CCOM_GetFirst(hCCOM,Filter);

    while(comp)
	{
		if(comp->w>(inf_let_h>>1)&&comp->h>(inf_let_h>>1))
		{
         if(vert)
		 {
			 if(comp->w>max_size)
				 max_size=comp->w;
		 }
		 else
		 {
			 if(comp->h>max_size)
                 max_size=comp->h;
		 }
		 num++;
		}
		comp=CCOM_GetNext(comp,Filter);
	}

	if(!num)
		return ((inf_let_h*3)>>1);
	else
		return max_size;
}

int GetMinCol(CCOM_comp** pC,int nN,Bool vert)
{
 int max=0;
 int min=1000000;
 if(vert==2)
 {
  for(int i=nN-1;i>=0;i--)
  {
	 if(pC[i]->upper<min&&pC[i]->w>=min_let_h&&pC[i]->h>=min_let_w)
		 min=pC[i]->upper;
  }
  return min;
 }
 else
 {
  for(int i=nN-1;i>=0;i--)
  {
	 if(pC[i]->upper+pC[i]->h-1>max&&pC[i]->w>=min_let_h&&pC[i]->h>=min_let_w)
		 max=pC[i]->upper+pC[i]->h-1;
  }
  return max;
 }
}

Bool MainVertDiagnostic(Handle hCPage,CCOM_handle hCCOM,CPrepHstr* temp,Rect16* pRc,int nRc,int param)
{

 PAGEINFO info = {0};
 GetPageInfo(hCPage,&info);

 int skew=info.Incline2048;

 CCOM_handle HCCOM;
 if(temp->hStrCCOM)
	 HCCOM=temp->hStrCCOM;
 else
	 HCCOM=hCCOM;

 Rect16 Rc;
 Rc.top=temp->top;
 Rc.left=temp->oldleft;
 Rc.bottom=Rc.top+temp->h-1;
 Rc.right=Rc.left+temp->w-1;

 Rect16 Rect;

 GetIdealRect(HCCOM,Rc,&Rect);

 if((Rect.right-Rect.left)*param>(Rect.bottom-Rect.top)*100)
	 return FALSE;

 int num=0;

 int medium_size_v=GetMediumSize(HCCOM,Rc,num,TRUE);
 int medium_size_h=GetMediumSize(HCCOM,Rc,num,FALSE);
 int max_size_v=GetMaxSize(HCCOM,Rc,num,TRUE);
 int max_size_h=GetMaxSize(HCCOM,Rc,num,FALSE);

 if(num<=0)
	 return FALSE;

 if(num<=temp->nRc)
 {
	 return FALSE;
 }

 NotInAll(HCCOM,Rc,temp->pRc,temp->nRc,TRUE);

 NotInAll(HCCOM,Rc,pRc,nRc,FALSE);

 if(temp->nRc==0)
	 return FALSE;
 if(nRc==0)
	 return TRUE;

 if(num<=temp->nRc)
 {
	 return FALSE;
 }

 if(NotIn(HCCOM,Rc,temp->pRc,temp->nRc,TRUE,medium_size_v))
	 return FALSE;

 if(NotIn(HCCOM,Rc,pRc,nRc,FALSE,medium_size_h))
	 return TRUE;

 int incl_h=(Rect.right-Rect.left+1)*abs(skew)/2048;
 int incl_v=(Rect.bottom-Rect.top+1)*abs(skew)/2048;

 if(nRc==1)
 {
	 if(Rect.bottom-Rect.top+1<3*(max_size_h>>1))
		 return FALSE;
	 else
		 return TRUE;
 }

 if(temp->nRc==1)
 {
     if(IfOneRowComp(HCCOM,Rect,TRUE))
	 {
         if(num>=2)
         {
          if((Rect.bottom-Rect.top)-medium_size_h*num>medium_size_h*(num-1))
              return FALSE;
         }

		 if(medium_size_v>2*inf_let_h&&medium_size_v>2*medium_size_h)//склеились буквы в строке
			 return FALSE;
		 else
	         return TRUE;
	 }
	 else
		 return FALSE;
 }

// int max_size=GetMaxSize((CCOM_handle)hCCOM,Rc,num,my_str->vertical);


//анализ строк

 int* h_top= new int[nRc];
 int* h_bottom= new int[nRc];
 int* v_top= new int[temp->nRc];
 int* v_bottom= new int[temp->nRc];

 Rect16 Recstr;

 Recstr.top=Rect.top;
 Recstr.bottom=Rect.bottom;

 int i(0);
 for(i=0;i<temp->nRc;i++)
 {
	 GetStrBounds(HCCOM,temp->pRc,temp->nRc,i,&(v_top[i]),&(v_bottom[i]),TRUE,Rc);
     Recstr.left=v_top[i];
	 Recstr.right=v_bottom[i];
	 if(!IfOneRowComp(HCCOM,Recstr,TRUE))
	 {
	   delete[] h_top;
	   delete[] h_bottom;
	   delete[] v_top;
	   delete[] v_bottom;
	   return FALSE;
	 }
 }


 Recstr.left=Rect.left;
 Recstr.right=Rect.right;

 for(i=0;i<nRc;i++)
 {
	 GetStrBounds(HCCOM,pRc,nRc,i,&(h_top[i]),&(h_bottom[i]),FALSE,Rc);
     Recstr.top=h_top[i];
	 Recstr.bottom=h_bottom[i];
	 if(!IfOneRowComp(HCCOM,Recstr,FALSE))
	 {
	   delete[] h_top;
	   delete[] h_bottom;
	   delete[] v_top;
	   delete[] v_bottom;
	   return TRUE;
	 }
 }



 for(i=0;i<temp->nRc;i++)
 {
  if(v_bottom[i]-v_top[i]>2*max_size_v)
  {
	  delete[] h_top;
	  delete[] h_bottom;
	  delete[] v_top;
	  delete[] v_bottom;
	  return FALSE;
  }
 }

 for(i=0;i<nRc;i++)
 {
  if(h_bottom[i]-h_top[i]>2*max_size_h)
  {
	  delete[] h_top;
	  delete[] h_bottom;
	  delete[] v_top;
	  delete[] v_bottom;
	  return TRUE;
  }
 }


 int medium_betw_h=0;
 for(i=nRc-2;i>=0;i--)
 {
	 if(h_top[i]-h_bottom[i+1]>=0)
        medium_betw_h+=h_top[i]-h_bottom[i+1];
 }
 medium_betw_h/=(nRc-1);
 medium_betw_h+=2*incl_h;

 int medium_betw_v=0;
 for(i=temp->nRc-2;i>=0;i--)
 {
	 if(v_top[i]-v_bottom[i+1]>=0)
        medium_betw_v+=v_top[i]-v_bottom[i+1];
 }
 medium_betw_v/=(temp->nRc-1);
 medium_betw_v+=2*incl_v;

 delete[] h_top;
 delete[] h_bottom;
 delete[] v_top;
 delete[] v_bottom;

 if(medium_betw_h<medium_betw_v)
	 return TRUE;
 else
	 return FALSE;

}


void  GetIdealRect(CCOM_handle HCCOM,Rect16 Rc,Rect16* Rect)
{
	my_top=Rc.top;
	my_bottom=Rc.bottom;
	my_left=Rc.left;
	my_right=Rc.right;

	CCOM_comp* comp=CCOM_GetFirst(HCCOM,Filter);

    int top=10000;
	int bottom=0;
	int left=10000;
	int right=0;

    while(comp)
	{
		if(comp->w>(inf_let_h>>1)&&comp->h>(inf_let_h>>1)&&(comp->h>inf_let_h||comp->w>inf_let_h))
		{
			if(comp->upper<top)
				top=comp->upper;

			if(comp->left<left)
				left=comp->left;

			if(comp->upper+comp->h-1>bottom)
				bottom=comp->upper+comp->h-1;

			if(comp->left+comp->w-1>right)
				right=comp->left+comp->w-1;

 		}
		comp=CCOM_GetNext(comp,Filter);
	}

 Rect->top=top;
 Rect->bottom=bottom;
 Rect->left=left;
 Rect->right=right;
}

void GetStrBounds(CCOM_handle hCCOM,Rect16* pRc,int nRc,int num,int* ptop,int* pbottom,Bool vert,Rect16 Rc)
{
 int min_top=10000;
 int max_bottom=0;
 int top;
 int bottom;
 int left;
 int right;
 if(!vert)
 {
    top=pRc[num].top;
	bottom=pRc[num].bottom;
	left=pRc[num].left;
	right=pRc[num].right;

	CCOM_comp* comp=CCOM_GetFirst(hCCOM,NULL);
    while(comp)
	{
		if(comp->w>(inf_let_h>>1)&&comp->h>inf_let_h)
		{
		 if(top-comp->upper<=(comp->h>>1)&&comp->upper+comp->h-1-bottom<(comp->h>>1)&&left-comp->left<(comp->w>>1)&&comp->left+comp->w-1-right<(comp->w>>1))
		 {
			if(comp->upper<min_top)
				min_top=comp->upper;

			if(comp->upper+comp->h-1>max_bottom)
				max_bottom=comp->upper+comp->h-1;
		 }
 		}
		comp=CCOM_GetNext(comp,NULL);
	}
	if(min_top==10000)
	{
		min_top=top;
		max_bottom=bottom;
	}
 }
 else
 {
    left=pRc[num].top;
	top=Rc.top;
	bottom=Rc.bottom;
	right=left+pRc[num].bottom-pRc[num].top+1;

    CCOM_comp* comp=CCOM_GetFirst(hCCOM,NULL);
    while(comp)
	{
		if(comp->w>inf_let_h&&comp->h>(inf_let_h>>1))
		{
		 if(top-comp->upper<=(comp->h>>1)&&comp->upper+comp->h-1-bottom<(comp->h>>1)&&left-comp->left<(comp->w>>1)&&comp->left+comp->w-1-right<(comp->w>>1))
		 {
			if(comp->left<min_top)
				min_top=comp->left;

			if(comp->left+comp->w-1>max_bottom)
				max_bottom=comp->left+comp->w-1;
		 }
 		}
		comp=CCOM_GetNext(comp,NULL);
	}
	if(min_top==10000)
	{
		min_top=left;
		max_bottom=right;
	}

 }

 (*ptop)=min_top;
 (*pbottom)=max_bottom;

}

Bool NotIn(CCOM_handle hCCOM,Rect16 Rc,Rect16* pRc,int nRc,Bool vert,int medium_size)
{
	my_top=Rc.top;
	my_bottom=Rc.bottom;
	my_left=Rc.left;
	my_right=Rc.right;
	int left;
	int right;
	int top;
	int bottom;
	Bool fl_in;
	CCOM_comp* comp;

   if(!vert)
   {
 	comp=CCOM_GetFirst(hCCOM,Filter);
    while(comp)
	{
		if(comp->w>(inf_let_h>>1)&&comp->h>inf_let_h&&comp->h<((medium_size*3)>>1))
		{
		 fl_in=FALSE;
		 for(int i=0;i<nRc;i++)
		 {
		  top=pRc[i].top;
		  bottom=pRc[i].bottom;
		  left=pRc[i].left;
		  right=pRc[i].right;
		  if(top-comp->upper<=(comp->h>>1)&&comp->upper+comp->h-1-bottom<(comp->h>>1)&&left-comp->left<(comp->w>>1)&&comp->left+comp->w-1-right<(comp->w>>1))
		  {
			  fl_in=TRUE;
		  }
		 }
		 if(!fl_in)
			 return TRUE;
 		}
		comp=CCOM_GetNext(comp,Filter);
	}
   }
   else
   {
    comp=CCOM_GetFirst(hCCOM,Filter);
    while(comp)
	{
		if(comp->w>inf_let_h&&comp->h>(inf_let_h>>1))
		{
		 fl_in=FALSE;
		 for(int i=0;i<nRc;i++)
		 {
          left=pRc[i].top;
	      top=Rc.top;
	      bottom=Rc.bottom;
	      right=left+pRc[i].bottom-pRc[i].top+1;
		  if(top-comp->upper<=(comp->h>>1)&&comp->upper+comp->h-1-bottom<(comp->h>>1)&&left-comp->left<(comp->w>>1)&&comp->left+comp->w-1-right<(comp->w>>1))
		  {
			  fl_in=TRUE;
		  }
		 }
		 if(!fl_in)
			 return TRUE;
 		}
		comp=CCOM_GetNext(comp,Filter);
	}
   }

 return FALSE;
}

Bool NotInAll(CCOM_handle hCCOM,Rect16 Rc,Rect16* pRc,int& nRc,Bool vert)
{
	my_top=Rc.top;
	my_bottom=Rc.bottom;
	my_left=Rc.left;
	my_right=Rc.right;
	int left;
	int right;
	int top;
	int bottom;
	CCOM_comp* comp;
	int i(0);

	Bool* fl_in = new Bool[nRc];
	for(i=0;i<nRc;i++)
        fl_in[i]=FALSE;

   if(!vert)
   {
 	comp=CCOM_GetFirst(hCCOM,Filter);
    while(comp)
	{
		if(comp->w>(inf_let_h>>1)&&comp->h>inf_let_h)
		{
		 for(i=0;i<nRc;i++)
		 {
		  top=pRc[i].top;
		  bottom=pRc[i].bottom;
		  left=pRc[i].left;
		  right=pRc[i].right;
		  if(top-comp->upper<=(comp->h>>1)&&comp->upper+comp->h-1-bottom<(comp->h>>1)&&left-comp->left<(comp->w>>1)&&comp->left+comp->w-1-right<(comp->w>>1))
		  {
			  fl_in[i]=TRUE;
		  }
		 }
 		}
		comp=CCOM_GetNext(comp,Filter);
	}
   }
   else
   {
    comp=CCOM_GetFirst(hCCOM,Filter);
    while(comp)
	{
		if(comp->w>inf_let_h&&comp->h>(inf_let_h>>1))
		{
		 for(i=0;i<nRc;i++)
		 {
          left=pRc[i].top;
	      top=Rc.top;
	      bottom=Rc.bottom;
	      right=left+pRc[i].bottom-pRc[i].top+1;
		  if(top-comp->upper<=(comp->h>>1)&&comp->upper+comp->h-1-bottom<(comp->h>>1)&&left-comp->left<(comp->w>>1)&&comp->left+comp->w-1-right<(comp->w>>1))
		  {
			  fl_in[i]=TRUE;
		  }
		 }
 		}
		comp=CCOM_GetNext(comp,Filter);
	}
   }

 Bool ret=FALSE;
 for(i=nRc-1;i>=0;i--)
 {
	 if(fl_in[i]==FALSE)
		 StrMoveMas(pRc,nRc,i);
 }

 delete[] fl_in;

 return ret;
}

Bool IfOneRowComp(CCOM_handle hCCOM,Rect16 Rc,Bool vert)
{
	my_top=Rc.top;
	my_bottom=Rc.bottom;
	my_left=Rc.left;
	my_right=Rc.right;
	CCOM_comp* comp;
	CCOM_comp** ppComp;
	int num=0;
	int len_mas=30;
	int add_len_mas=10;

	if(!InitStrMas(&ppComp,len_mas))
		return TRUE;

   if(!vert)
   {
 	comp=CCOM_GetFirst(hCCOM,Filter);
    while(comp)
	{
	 if(my_top-comp->upper<(comp->h>>1)&&comp->upper+comp->h-1-my_bottom<(comp->h>>1)&&my_left-comp->left<(comp->w>>1)&&comp->left+comp->w-1-my_right<(comp->w>>1))
	 {
	  if(comp->w>(inf_let_h>>1)&&comp->h>inf_let_h)
	  {
        if(num>=len_mas)
		{
			if(!AddLenStrMas(&ppComp,len_mas,add_len_mas))
			{
				DelStrMas(ppComp);
				return TRUE;
			}
		}
		ppComp[num]=comp;
		num++;
	  }
	 }
	 comp=CCOM_GetNext(comp,Filter);
	}
   }
   else
   {
    comp=CCOM_GetFirst(hCCOM,Filter);
    while(comp)
	{
	 if(my_top-comp->upper<(comp->h>>1)&&comp->upper+comp->h-1-my_bottom<(comp->h>>1)&&my_left-comp->left<(comp->w>>1)&&comp->left+comp->w-1-my_right<(comp->w>>1))
	 {
	  if(comp->w>inf_let_h&&comp->h>(inf_let_h>>1)||comp->w>inf_let_h/3&&comp->h>inf_let_h)
	  {
        if(num>=len_mas)
		{
			if(!AddLenStrMas(&ppComp,len_mas,add_len_mas))
			{
				DelStrMas(ppComp);
				return TRUE;
			}
		}
		ppComp[num]=comp;
		num++;
	  }
	 }
	 comp=CCOM_GetNext(comp,Filter);
	}
   }

 SortLetter(ppComp,num,vert);

 int i;
 if(!vert)
 {
	 for(i=0;i<num-1;i++)
	 {
		 if(ppComp[i+1]->upper>ppComp[i]->upper+ppComp[i]->h-1||ppComp[i+1]->upper+ppComp[i+1]->h-1<ppComp[i]->upper)
		 {
			 DelStrMas(ppComp);
			 return FALSE;
		 }
	 }
 }
 else
 {
	 for(i=0;i<num-1;i++)
	 {
		 if(ppComp[i+1]->left>ppComp[i]->left+ppComp[i]->w-1||ppComp[i+1]->left+ppComp[i+1]->w-1<ppComp[i]->left)
		 {
			 DelStrMas(ppComp);
			 return FALSE;
		 }
	 }
 }

 DelStrMas(ppComp);
 return TRUE;
}
