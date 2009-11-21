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

#include <string.h>
#include <stdlib.h>
#include "puttocstr.h"
#include "rselstr.h"

#include "compat_defs.h"

/*----------------------------------------------------------------------------*/

//Настройки


int inf_let_h=10;
int min_let_w=2;
int min_let_h=2;
int inf_betw_str_h=10;

int32_t my_top;
int32_t my_left;
int32_t my_right;
int32_t my_bottom;


/*----------------------------------------------------------------------------*/
//Параметры

int	DPIX;
int	DPIY;

/*-------------------------------------------------------------------------------------*/




Bool32 GetObjects (Handle hCCOM,Handle hCPage)
{
 if(list==NULL)
	return FALSE;


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
 const int neg_w=30;

 PAGEINFO info = {0};
 GetPageInfo(hCPage,&info);

 int skew=info.Incline2048;

 DPIX=info.DPIX;
 DPIY=info.DPIY;
 uchar ImageName[CPAGE_MAXNAME];
// uchar RotateImageName[CPAGE_MAXNAME];


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


 int max_str=CSTR_GetMaxNumber ();
 max_str++;
 CHstr* my_str=list->GetRoot();
 CPrepHstr* my_prep_str;
 CSTR_line string;
 CSTR_rast rast;
 CSTR_attr attr;
 CSTR_rast_attr r_attr;
 RecRaster rec;
 Rect16 Rc;
 Rect16 unrotRc;
 CCOM_comp* comp=NULL;
 Bool32 ret;
 Bool32 fl_rotate;
 Rect16* pRc = NULL;
 int nRc=1;
 int len_mas=100;
 int add_len_mas=50;
 int time;
 int scale;
 int oldleft;

 InitPrepList(hCPage);

//Основной цикл по фрагментам
 while(my_str!=NULL)
 {
  begx=NULL;
  movey=NULL;
  hi=NULL;
  flmovey=NULL;

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
   Rc.top=my_str->top;
   Rc.bottom=my_str->top+my_str->h-1;
   Rc.left=my_str->left;
   Rc.right=my_str->left+my_str->w-1;


  my_prep_str=NULL;//___commented by Andrey___ WasAlreadyPrep(my_str);
  if(my_prep_str)
  {
   my_str->left=my_prep_str->left;
   pRc=my_prep_str->pRc;
   my_prep_str->pRc=NULL;
   nRc=my_prep_str->nRc;
   hi=my_prep_str->hi;
   my_prep_str->hi=NULL;
   fl_rotate=my_prep_str->fl_rotate;
  }
  else
  {
   nRc=1;
   len_mas=100;
   ret=InitStrMas(&pRc,len_mas);
   if(!ret)
	  my_str=my_str->next;
   if(!ret )
	   continue;


   pRc[0].bottom=Rc.bottom;
   pRc[0].top=Rc.top;
   pRc[0].right=Rc.right;
   pRc[0].left=Rc.left;
//  fl_rotate=MyRotateImage(ImageName,RotateImageName,-skew,&(pRc[0]),my_str->vertical,my_str->m_num_frag,&rot,&unrot);


   pmaspold = Data;
   ret=GetMasP(hCPage,ImageName,Rc,&pmaspold);
   if(!ret)
	  my_str=my_str->next;
   if(!ret)
	  continue;

//  int w=pRc[0].right-pRc[0].left+1;
//  int h=pRc[0].bottom-pRc[0].top+1;

   int w=Rc.right-Rc.left+1;
   int h=Rc.bottom-Rc.top+1;
   pmasp=NULL;
   if(!(pmasp= new uchar[w*h/8+w+h]))
   {
   // GF: killed memory leak on 2004.01.29
	 DelStrMas(&pRc); pRc = NULL;
	 my_str=my_str->next;
	 ret=FALSE;
   }
   if(!ret)
	 continue;

   pRc[0].bottom=Rc.bottom;
   pRc[0].top=Rc.top;
   pRc[0].right=Rc.right;
   pRc[0].left=Rc.left;

   if(my_str->vertical)
      ToHorizont(pRc,pmasp,pmaspold);
   else
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
    time=clock();
    fl_rotate=RSELSTR_RotateRaster(pmasp,-skew,&(pRc[0]),begx,movey,flmovey,inf_betw_str_h);
//    if(fl_rotate)
//     LDPUMA_ConsoleN("Time rotate=%d",clock()-time);
   }
//  LDPUMA_WaitUserInput (StrD,MainWindowD);



   oldleft=pRc[0].top;

   if(my_str->neg)
   {
    if(!(RSELSTR_CutStr(&pRc,nRc,len_mas,pmasp,DPIX,DPIY,neg_w)) )
    {
   // GF: killed memory leak on 2004.01.29
	 DelStrMas(&pRc); pRc = NULL;
	 delete[] pmasp;
	 my_str=my_str->next;
	 DeleteRotateMas(begx,movey,flmovey,hi);
	 ret=FALSE;
    }
   }
   else
   {
    if(!(RSELSTR_CutStr(&pRc,nRc,len_mas,pmasp,DPIX,DPIY,0)) )
    {
   // GF: killed memory leak on 2004.01.29
	 DelStrMas(&pRc); pRc = NULL;
	 delete[] pmasp;
	 my_str=my_str->next;
	 DeleteRotateMas(begx,movey,flmovey,hi);
	 ret=FALSE;
    }
   }
   if(!ret)
	  continue;
   if(my_str->vertical)
   {
	my_str->left-=oldleft-pRc[nRc-1].top;
/*
    if(IfBadCut(hCCOM,pRc,Rc,nRc,pmasp,my_str))
    {
	 DelStrMas(pRc);
	 delete[] pmasp;
	 DeleteRotateMas(begx,movey,flmovey,hi);
	 ret=FALSE;
    }
	*/
   }
   if(!ret)
	  continue;

   if(fl_rotate)
   {
    InitRotateMas(&hi,nRc);
    RSELSTR_UnRotateRect(-skew,pRc,nRc,unrotRc,begx,movey,flmovey,hi);
   }

/* hi=NULL;
  if(!(hi=new int[nRc]))
  {
	 DelStrMas(pRc);
	 delete[] pmasp;
	 my_str=my_str->next;
	 ret=FALSE;
  }
  if(!ret)
	  continue;

  MyUnRotateRect(pRc,nRc,hi,unrot);
*/
   delete[] pmasp;
  }


  Handle hStrCCOM;
  CCOM_comp * pcomp;
  oldleft=my_str->left;
  Rect16* pN = NULL;
  CCOM_comp** pC;
  int nN=0;
  Bool nomem=FALSE;

  int i;

  for(int i2=nRc-1;i2>=0;i2--)//цикл по кол-ву строк в фрагменте
  {
   if(my_str->vertical==2)
	   i=nRc-1-i2;
   else
	   i=i2;
   if(my_str->vertical)
   {
	  Rc.left=pRc[i].top;
	  Rc.right=pRc[i].bottom;
   }
   else
   {
	  Rc.top=pRc[i].top;
	  Rc.bottom=pRc[i].bottom;
   }

   if(!my_prep_str)
   {
    if(my_str->neg)
       hStrCCOM=GetStrCCOM(hCPage, ImageName, Rc, my_str->neg, my_str->vertical);
    else
	   hStrCCOM=(CCOM_handle)hCCOM;
   }
   else
   {
	if(!my_prep_str->hStrCCOM)
	{
     if(my_str->neg)
        hStrCCOM=GetStrCCOM(hCPage,ImageName,Rc,my_str->neg,my_str->vertical);
     else
	    hStrCCOM=(CCOM_handle)hCCOM;
	}
	else
	{
		hStrCCOM=my_prep_str->hStrCCOM;
	}
   }

   if(!hStrCCOM)
	   continue;

   //for filter
   my_top=(int32_t)Rc.top;
   my_bottom=(int32_t)Rc.bottom;
   my_left=(int32_t)Rc.left;
   my_right=(int32_t)Rc.right;

   pcomp = CCOM_GetFirst(hStrCCOM,Filter);

   if(pcomp==NULL)
	  continue;

   nomem=FALSE;
   len_mas=100;
   nN=0;

   if(!(InitStrMas(&pN,len_mas)))
      continue;
   if(!(InitStrMas(&pC,len_mas)))
   {
     // GF: killed memory leak on 2004.01.29
	   DelStrMas(&pN); pN = NULL;
	   nomem=TRUE;
   }
   if(nomem)
      continue;

   InitRc(pN,nN,pcomp);
   pC[nN]=pcomp;
   nN++;

   for(;;)
   {
    pcomp = CCOM_GetNext(pcomp,Filter);
    if (pcomp==NULL)
       break;
    if (nN>=len_mas)
    {
		if(! AddLenStrMas(&pN,len_mas,add_len_mas))
			nomem=TRUE;
		else
		{
         len_mas-=add_len_mas;
         if(!AddLenStrMas(&pC,len_mas,add_len_mas))
			nomem=TRUE;
		}

    }
    if(nomem)
		break;
    InitRc(pN,nN,pcomp);
	pC[nN]=pcomp;
    nN++;
   }


//   if(nomem)
//	   continue;

//   SortLetter(pN,nN,my_str->vertical);
   int j;
   if(fl_rotate)
   {
	   if(my_str->vertical)
		   j=my_str->h;
	   else
		   j=my_str->w;
	   RSELSTR_CleanStr(pN,pC,nN,Rc.top,Rc.left,hi[i],j,skew,my_str->vertical);
   }
   if(!(j=IfStr(pN,nN,my_str->vertical)))
   {
    // GF: killed memory leak on 2004.01.29
	  DelStrMas(&pN); pN = NULL;
	  DelStrMas(pC);
	  StrMoveMas(pRc,nRc,i);
	  if(fl_rotate)
	  {
		  nRc++;
		  StrMoveMas(hi,nRc,i);
	  }

	  if(!my_prep_str)
	  {
        if(hStrCCOM!=(CCOM_handle)hCCOM)
         CCOM_DeleteContainer(hStrCCOM);
	  }
      else
	  {
	    if(!my_prep_str->hStrCCOM)
		{
          if(hStrCCOM!=(CCOM_handle)hCCOM)
            CCOM_DeleteContainer(hStrCCOM);
		}
	  }
   }
   if(!j)
	  continue;

//Заполнение в CSTR:

   string=CSTR_NewLine(max_str,0,-1);
   if(string==NULL)
   {
	  CleanCont();
    // GF: killed memory leak on 2004.01.29
	  DelStrMas(&pN); pN = NULL;
	  DelStrMas(pC);
    // GF: killed memory leak on 2004.01.29
	  DelStrMas(&pRc); pRc = NULL;
      DeleteRotateMas(begx,movey,flmovey,hi);
	  return FALSE;
   }
   max_str++;
   scale=GetStrScale(pN,nN,my_str->vertical);
   if(CSTR_GetLineAttr(string,&attr))
   {
	attr.scale=(uchar)(scale);
    attr.incline=skew;
    attr.fragment=my_str->m_num_frag;
	if(my_str->vertical)
	{
     attr.col=GetMinCol(pC,nN,my_str->vertical);
     attr.row=my_str->left+pRc[i].top-pRc[nRc-1].top;
     attr.wid=my_str->h;
	 if(fl_rotate)
		 attr.hei=hi[i];
	 else
         attr.hei=pRc[i].bottom-pRc[i].top+1;
	}
	else
	{
     attr.col=my_str->left;
     attr.row=pRc[i].top;
 	 attr.wid=my_str->w;
	 if(fl_rotate)
		 attr.hei=hi[i];
	 else
         attr.hei=pRc[i].bottom-pRc[i].top+1;
	}
    attr.r_row=attr.row;
    attr.r_col=attr.col;
    attr.VersionName[0]='C';
    attr.VersionName[1]='H';
    attr.VersionName[2]='S';
    attr.VersionName[3]='T';
    attr.VersionName[4]='R';
    attr.VersionName[5]='\0';
    attr.Flags = CSTR_STR_No;
	if(my_str->vertical==2)
	    attr.Flags|= CSTR_STR_UPDOWN;
	else
	{
		if(my_str->vertical==1)
	           attr.Flags|= CSTR_STR_DOWNUP;
	}
	if(my_str->neg)
		attr.Flags|= CSTR_STR_NEGATIVE;
    CSTR_SetLineAttr(string,&attr);
   }

   for(j=0;j<nN;j++)
   {
    Rect16 RastRec;
	if(!my_str->vertical&&pN[j].bottom-pN[j].top+1<min_let_h&&pN[j].right-pN[j].left+1<min_let_w)
		continue;
	if(my_str->vertical&&pN[j].bottom-pN[j].top+1<min_let_w&&pN[j].right-pN[j].left+1<min_let_h)
		continue;

	//correction
	if(pN[j].top!=pC[j]->upper)
		pN[j].top=pC[j]->upper;
	if(pN[j].bottom!=pC[j]->upper+pC[j]->h-1)
		pN[j].bottom=pC[j]->upper+pC[j]->h-1;
	if(pN[j].left!=pC[j]->left)
		pN[j].left=pC[j]->left;
	if(pN[j].right!=pC[j]->left+pC[j]->w-1)
		pN[j].right=pC[j]->left+pC[j]->w-1;

	if(my_str->vertical)
		scale=((pN[j].bottom-pN[j].top)>>6);
	else
		scale=((pN[j].right-pN[j].left)>>6);

    ret=StrGetRaster(hCPage,my_str->ImageName,pN[j],pC[j],&rec,my_str->vertical,my_str->neg,scale);
    if(!ret)
	  continue;

    GetRasterRect(ImageName,pN[j],&RastRec,&attr,0);

    rast=CSTR_NewRaster(string,RastRec.left,RastRec.top,RastRec.right-RastRec.left+1);
    if(!rast)
	   continue;

    if(CSTR_GetAttr(rast,&r_attr))
    {
     r_attr.col=RastRec.left;
     r_attr.h=RastRec.bottom-RastRec.top+1;
	 r_attr.w=RastRec.right-RastRec.left+1;
	 r_attr.row=RastRec.top;
	 r_attr.r_col=r_attr.col;
	 r_attr.r_row=r_attr.row;
	 r_attr.flg=CSTR_f_let|CSTR_f_bad;
    }
    if(!(ret=CSTR_SetAttr(rast,&r_attr)) )
        CSTR_DelRaster(rast);

    if(!ret)
	    continue;



    if(!CSTR_StoreRaster(rast,&rec))
        CSTR_DelRaster(rast);
	else
	{
	    rast->env->scale=(uchar)(scale);
	}

   }

   // GF: killed memory leak on 2004.01.29
   DelStrMas(&pN); pN = NULL;
   DelStrMas(pC);
   if(!my_prep_str)
   {
    if(hStrCCOM!=(CCOM_handle)hCCOM)
        CCOM_DeleteContainer(hStrCCOM);
   }
   else
   {
	if(!my_prep_str->hStrCCOM)
	{
     if(hStrCCOM!=(CCOM_handle)hCCOM)
        CCOM_DeleteContainer(hStrCCOM);
	}
   }
  }


// Прорисовка строк
  if(!fl_rotate)
  {
   if(nRc>0)
   {
    if(!(LDPUMA_Skip (StrD)) )
    {

      StrDrawRect(MainWindowD,code_strd,RGB(0,0,255),pRc,nRc,my_str->vertical,Rc);
      LDPUMA_WaitUserInput (StrD,MainWindowD);
      LDPUMA_DeleteRects (MainWindowD,code_strd);
    }
   }
  }
  else
  {
   if(nRc>0)
   {
    if(!(LDPUMA_Skip (StrD)) )
    {
	 for(i=nRc-1;i>=0;i--)
	 {
      StrDrawRect(MainWindowD,code_strd,RGB(0,0,255),&(pRc[i]),1,my_str->vertical,Rc);
      LDPUMA_WaitUserInput (StrD,MainWindowD);
      LDPUMA_DeleteRects (MainWindowD,code_strd);
	 }
    }
   }
  }

  my_str=list->GetNext(my_str);
  // GF: killed memory leak on 2004.01.29
  DelStrMas(&pRc); pRc = NULL;
  DeleteRotateMas(begx,movey,flmovey,hi);
 }


 CleanCont();
// LDPUMA_ConsoleN("Time work CHSTR=%d",clock()-TIME);
 return TRUE;
}


int MyGetMaxFragment(int ver)
{
 int n=1;
 CSTR_line line=CSTR_GetLineFirst (n,ver);
 while(line!=NULL)
 {
  n++;
  line=CSTR_GetLineFirst (n,ver);
 }
 return n-1;
}


void CleanCont()
{
 delete prelist;
 prelist=NULL;
}



Bool StrGetRaster(Handle hCPage,uchar* ImageName,Rect16 N,CCOM_comp* comp,RecRaster* rec,Bool vert,Bool neg,int scale)
{
	//Andrey - 05.03.2003 - в структуру RecRaster больше, чем REC_MAX_RASTER_SIZE, не влезет
	if (comp->h*REC_GW_WORD8(((comp->w + (1<<comp->scale) - 1)>>comp->scale)) > REC_MAX_RASTER_SIZE)
		return FALSE;

 int i,j,k;
// uchar Data[1000];
 uchar bytep;
 int h=N.bottom-N.top+1;
 int w=N.right-N.left+1;
 int bytewide=(w)/8;
 if( ((w)%8) != 0)
	 bytewide++;
 int vbytewide=(h)/8;
 if( ((h)%8) != 0)
	 vbytewide++;
 uchar pmasp[REC_MAX_RASTER_SIZE];
 for(i=REC_MAX_RASTER_SIZE-1;i>=0;i--)
	 pmasp[i]=0;
/* memset (Data, 0, sizeof (Data));
 pmasp=Data;
 if( !(GetMasP(hCPage,ImageName,N,&pmasp)) )
	 return FALSE;
*/
 int nowbyte;
 int nowbyte2;

 RecRaster rec2;
 if(comp)
   CCOM_GetRaster(comp,&rec2);
 scale=rec2.lnPixWidth/64;
 w=rec2.lnPixWidth;
 h=rec2.lnPixHeight;
 bytewide=(w+7)/8;
 vbytewide=(h+7)/8;
 k=(1+scale)*8;
 nowbyte=0;
 nowbyte2=0;
 int j2;

 if(vert==2)
 {
  for(i=h-1;i>=0;i--)
  {
	 nowbyte=(h-1-i)*bytewide;
	 nowbyte2=i*k+((w-1)>>3);
	 for(j2=w-1,j=0;j2>=0;j2--,j++)
	 {
		 if(j&&!(j%8))
			 nowbyte++;
		 if((j2%8)==7&&(j2!=w-1))
			 nowbyte2--;

		 pmasp[nowbyte]|=(128&(rec2.Raster[nowbyte2]<<(j2%8)))>>(j%8);
	 }
  }
 }
 else
 {
  for(i=h-1;i>=0;i--)
  {
	 nowbyte=i*bytewide;
	 nowbyte2=i*k;
	 for(j=bytewide-1;j>=0;j--)
	 {
		 pmasp[nowbyte]=rec2.Raster[nowbyte2];
		 nowbyte++;
		 nowbyte2++;
	 }
  }
 }


/* if(!neg)
 {
	 j=h*bytewide;
	 for(i=0;i<j;i++)
	   pmasp[i]=~(pmasp[i]);

 }
 */
 if((k=(w%8))>0)
 {
	  bytep=0;
	  for(j=128;j>(128>>k);j=j>>1)
		 bytep|=j;
 	  for(i=0;i<h;i++)
	     pmasp[(i+1)*bytewide-1]&=bytep;
 }

 for(i=0;i<scale;i++)
 {
	 if(!vert)
	 {

			 StrAr2(pmasp,h,w);
			 w=w/2;
			 h=h/2;
			 bytewide=(w)/8;
             if( ((w)%8) != 0)
	            bytewide++;

	 }
	 else
	 {

			 StrAr2(pmasp,h,w);
			 w=w/2;
			 h=h/2;
			 vbytewide=(h)/8;
             if( ((h)%8) != 0)
	            vbytewide++;

	 }
 }

 if( ((8*h> REC_MAX_RASTER_SIZE)&&(!vert))||((8*w> REC_MAX_RASTER_SIZE)&&vert) )
	 return FALSE;

 if(!vert)
 {
  nowbyte=0;
  nowbyte2=0;
  for(j=0;j<h;j++)
  {
   for(i=0;i<bytewide;i++)
   {
  	 (*rec).Raster[nowbyte]=pmasp[nowbyte2];
     nowbyte++;
	 nowbyte2++;
   }
   for(i=bytewide;i<8;i++)
   {
	 (*rec).Raster[nowbyte]=0;
	 nowbyte++;
   }
  }
  (*rec).lnPixHeight=h;
  (*rec).lnPixWidth=w;
  (*rec).lnRasterBufSize=REC_MAX_RASTER_SIZE;
 }
 else
 {
  int rectj;
  int delj;
  for(j=0;j<w;j++)
  {
   nowbyte=j<<3;
   rectj=j%8;
   delj=j>>3;
   for(i=vbytewide;i<8;i++)
	 (*rec).Raster[nowbyte+i]=0;
   for(i=0;i<vbytewide-1;i++)
   {
	bytep=0;
	nowbyte2=h-1-(i<<3);

	   bytep|=( ( (128>>rectj) & pmasp[bytewide*(nowbyte2)+delj] )<<rectj );
	   bytep|=( ( (128>>rectj) & pmasp[bytewide*(nowbyte2-1)+delj] )<<rectj )>>1;
       bytep|=( ( (128>>rectj) & pmasp[bytewide*(nowbyte2-2)+delj] )<<rectj )>>2;
	   bytep|=( ( (128>>rectj) & pmasp[bytewide*(nowbyte2-3)+delj] )<<rectj )>>3;
	   bytep|=( ( (128>>rectj) & pmasp[bytewide*(nowbyte2-4)+delj] )<<rectj )>>4;
	   bytep|=( ( (128>>rectj) & pmasp[bytewide*(nowbyte2-5)+delj] )<<rectj )>>5;
       bytep|=( ( (128>>rectj) & pmasp[bytewide*(nowbyte2-6)+delj] )<<rectj )>>6;
	   bytep|=( ( (128>>rectj) & pmasp[bytewide*(nowbyte2-7)+delj] )<<rectj )>>7;

    (*rec).Raster[nowbyte+i]=bytep;
   }
   bytep=0;
   nowbyte2=h-1-(i<<3);
   for(k=0;k<h-((vbytewide-1)<<3);k++)
	   bytep|=( ( (128>>rectj) & pmasp[bytewide*(nowbyte2-k)+delj] )<<rectj )>>k;
	(*rec).Raster[nowbyte+i]=bytep;
  }
  (*rec).lnPixHeight=w;
  (*rec).lnPixWidth=h;
  (*rec).lnRasterBufSize=REC_MAX_RASTER_SIZE;
 }

 return TRUE;
}


void StrAr2(uchar* pmasp,int h,int w)
{
 int i,j;
 uchar bytep;
 int newh=h/2;
 int neww=w/2;
 int newbytewide=(neww)/8;
 if( ((neww)%8) != 0)
	 newbytewide++;
 int bytewide=(w)/8;
 if( ((w)%8) != 0)
	 bytewide++;
 int nowbyte;
 int nowbyte2;
 for(i=0;i<h;i++)
 {
  nowbyte=i*bytewide;
  nowbyte2=i*newbytewide;
  for(j=0;j<bytewide>>1;j++)
  {
   bytep=0;

     bytep|=(128)&(pmasp[nowbyte+(j<<1)]|(pmasp[nowbyte+(j<<1) ]<<1));
     bytep|=(64)&((pmasp[nowbyte+(j<<1)]<<1)|(pmasp[nowbyte+(j<<1) ]<<2));
     bytep|=(32)&((pmasp[nowbyte+(j<<1)]<<2)|(pmasp[nowbyte+(j<<1) ]<<3));
     bytep|=(16)&((pmasp[nowbyte+(j<<1)]<<3)|(pmasp[nowbyte+(j<<1) ]<<4));


	 bytep|=(8)&((pmasp[nowbyte+(j<<1)+1]>>4)|(pmasp[nowbyte+(j<<1)+1]>>3));
	 bytep|=(4)&((pmasp[nowbyte+(j<<1)+1]>>3)|(pmasp[nowbyte+(j<<1)+1]>>2));
	 bytep|=(2)&((pmasp[nowbyte+(j<<1)+1]>>2)|(pmasp[nowbyte+(j<<1)+1]>>2));
	 bytep|=(1)&((pmasp[nowbyte+(j<<1)+1]>>1)|pmasp[nowbyte+(j<<1)+1]);

   pmasp[nowbyte2+j]=bytep;
  }
  if(j<newbytewide)
   pmasp[nowbyte2+j]=pmasp[nowbyte+bytewide-1];
 }

 for(i=0;i<h/2;i++)
 {
	 nowbyte=i*newbytewide;
     nowbyte2=(i*newbytewide)<<1;
	 for(j=0;j<newbytewide;j++)
		 pmasp[nowbyte+j]=pmasp[nowbyte2+j]|pmasp[nowbyte2+newbytewide+j];

 }
}


void Invert(RecRaster* rec)
{
 int bytewide=(rec->lnPixWidth)/8;
 if( ((rec->lnPixWidth)%8) != 0)
	 bytewide++;
 int nowbyte;
 for(int i=0;i<rec->lnPixHeight;i++)
 {
	 nowbyte=i<<3;
	 for(int j=0;j<bytewide;j++)
		 rec->Raster[nowbyte+j]=~(rec->Raster[nowbyte+j]);
 }
}


Bool MyRotateImage(uchar* ImageName,uchar* RotateImageName,int skew,Rect16* Rc,Bool vertical,int code,MATRIX* rot,MATRIX* unrot)
{
 int w;
 int own_skew=skew;
 int time;
 uint32_t use_marg;
 if(vertical)
	 w=Rc->bottom-Rc->top+1;
 else
	 w=Rc->right-Rc->left+1;
 if(skew<0)
	 skew=-skew;

 if((double)(w*skew)/2048.<(double)(inf_betw_str_h))
 {
	 InitRotateImageName(RotateImageName,ImageName);
	 return FALSE;
 }

 use_marg=0;

 InitRotateImageName(RotateImageName,code);

 time=clock();

 if(!RIMAGE_Rotate(ImageName,RotateImageName,own_skew,2048,use_marg))
 {
	 InitRotateImageName(RotateImageName,ImageName);
	 return FALSE;
 }
 LDPUMA_ConsoleN("Time rotate=%d",clock()-time);
 int32_t x;
 int32_t y;
 RIMAGE_RotatePoint(RotateImageName,Rc->left,Rc->top,&x,&y);
 Rc->left=(int16_t)(x);
 Rc->top=(int16_t)(y);
 RIMAGE_RotatePoint(RotateImageName,Rc->right,Rc->bottom,&x,&y);
 Rc->right=(int16_t)(x);
 Rc->bottom=(int16_t)(y);

 return TRUE;
}

void InitRotateImageName(uchar* RotateImageName,uchar* ImageName)
{
	for(int i=0;i<CPAGE_MAXNAME;i++)
		RotateImageName[i]=ImageName[i];
}

void InitRotateImageName(uchar* RotateImageName,int code)
{
	RotateImageName[0]='C';
	RotateImageName[1]='H';
	RotateImageName[2]='S';
	RotateImageName[3]='T';
	RotateImageName[4]='R';
	RotateImageName[5]=' ';
	RotateImageName[6]='R';
	RotateImageName[7]='O';
	RotateImageName[8]='T';
	RotateImageName[9]='A';
	RotateImageName[10]='T';
	RotateImageName[11]='E';
	RotateImageName[12]=' ';
	int num=13;
	int size=0;
	int cod=code;
	while(code>0)
	{
		code/=10;
		size++;
	}
	code=1;
	int j;
	for(j=1;j<size;j++)
        code*=10;
    for(j=0;j<size;j++)
	{
		RotateImageName[num]=48+cod/code;
		num++;
		cod-=(cod/code)*code;
		code/=10;
	}
	RotateImageName[num]='\0';
}

void GetRasterRect(uchar* UnRotateImageName,Rect16 N,Rect16* Rc,CSTR_attr* attr,Bool fl_rotate)
{
 int top;
 int bottom;

 if(fl_rotate)
 {
	 int32_t x;
	 int32_t y;
  RIMAGE_RotatePoint(UnRotateImageName,N.left,N.top,&x,&y);
  Rc->top=(int16_t)(y);
  Rc->left=(int16_t)(x);
  RIMAGE_RotatePoint(UnRotateImageName,N.right,N.bottom,&x,&y);
  Rc->right=(int16_t)(x);
  Rc->bottom=(int16_t)(y);
 }
 else
 {
  Rc->top=N.top;
  Rc->bottom=N.bottom;
  Rc->left=N.left;
  Rc->right=N.right;
 }
 if(attr->Flags&CSTR_STR_DOWNUP)
 {
  top=Rc->top;
  bottom=Rc->bottom;
  Rc->top=Rc->left;
  Rc->bottom=Rc->right;
  Rc->left=2*(attr->col)-bottom;
  Rc->right=Rc->left+bottom-top;
 }
 if(attr->Flags&CSTR_STR_UPDOWN)
 {
  top=Rc->top;
  bottom=Rc->bottom;
  Rc->top=Rc->left;
  Rc->bottom=Rc->right;
  Rc->left=top;
  Rc->right=bottom;
 }
}


int GetStrScale(Rect16* pRc,int nRc,Bool vertical)
{
	int max_scale=0;
	int now_scale=0;
	int w;
 for(int i=nRc-1;i>=0;i--)
 {
	 if(vertical)
		 w=pRc[i].bottom-pRc[i].top;
	 else
		 w=pRc[i].right-pRc[i].left;
	 now_scale=w>>6;
	 if(now_scale>max_scale)
		 max_scale=now_scale;
 }
 return max_scale;
}

Bool InitRotateMas(Rect16 Rc,int16_t** ppbegx,int16_t** ppmovey,uchar** ppflmovey)
{
	int w=Rc.right-Rc.left+1;
	int h=Rc.bottom-Rc.top+1;
	if(!( (*ppbegx)=new int16_t[h+1]))
		return FALSE;
	if(!( (*ppmovey)=new int16_t[w+1]))
	{
		delete[] (*ppbegx);
		(*ppbegx)=NULL;
		return FALSE;
	}
	if(!( (*ppflmovey)=new uchar[w+1]))
	{
		delete[] (*ppbegx);
		(*ppbegx)=NULL;
		delete[] (*ppmovey);
		(*ppmovey)=NULL;
		return FALSE;
	}
	return TRUE;
}

Bool InitRotateMas(int** pphi,int n)
{
	if(!( (*pphi)=new int[n]))
		return FALSE;
	else
	    return TRUE;
}

void DeleteRotateMas(int16_t* pbegx,int16_t* pmovey,uchar* pflmovey,int* phi)
{
	delete[] pbegx;
	pbegx=NULL;
	delete[] pmovey;
	pmovey=NULL;
	delete[] pflmovey;
	pflmovey=NULL;
	delete[] phi;
	phi=NULL;
}

CPrepHstr* WasAlreadyPrep(CHstr* my_str)
{
 if(!prelist)
	 return NULL;

 if(my_str->vertical==0&&!my_str->neg)
	 return NULL;

 const int delta=5;

 CPrepHstr* prestr=prelist->GetRoot();

 while(prestr)
 {
	 if(my_str->neg)
	 {
	  if(abs(prestr->top-my_str->top)<=delta&&abs(prestr->left-my_str->left)<=delta&&abs(prestr->w-my_str->w)<=delta&&abs(prestr->h-my_str->h)<=delta&&prestr->neg)
		 return prestr;
	 }
	 else
	 {
	  if(prestr->top==my_str->top&&prestr->oldleft==my_str->left&&prestr->w==my_str->w&&prestr->h==my_str->h&&!prestr->neg)
		 return prestr;
	 }
	 prestr=prelist->GetNext(prestr);
 }

 return NULL;
}



