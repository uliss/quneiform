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

#include "rcutp.h"
//#include "cpage.h"
//#include "ctiimage.h"
#include "ccom.h"
#include "dpuma.h"
/*********************************************************************************************/
static int count_cut_let;
//static PAGEINFO info = {0};
//static char* image_name = NULL;
static uint16_t biBitCount = 0;
static bool env_set = false;
static int medium_h;
static int inf_let_w;
static int inf_dust_h;
static int inf_let_h;
static Handle HCCOM;
static int sup_dust_w;
static int min_cut_down_let_w;
static int sup_prob_w;
/*********************************************************************************************/
static int GetCountCutLetters(CCOM_comp* comp, uchar* pmasp);
static int IfNeedCut(CCOM_comp* comp, Bool* type_let);
//static Bool GetMasP(Rect16 Rc,uchar** ppmasp);
static Bool JNotInMas2(int j,int* mas,int size,int bound);
static Bool JNotInMas(int j,int* mas,int size,int bound);
static Bool IfZpt(CCOM_comp* comp);
static Bool IfCav(CCOM_comp* comp);
static int GetBoundFromStr(CCOM_comp* comp,Bool IfCav,int& max_bottom,int& min_top);
static Bool NotNeedCut(CCOM_comp* comp);
static int ExBound(CCOM_comp* comp,int bound);
static int GetRastWide(RecRaster* rast,int& left,int& right);
static Bool IfHightLetters(CCOM_comp* comp,Bool Up,int real_left,int real_right);
static Bool IfNearLetters(CCOM_comp* cutting,CCOM_comp* comp);
static void NotCutDownLetter(RecRaster* rast,int& bound,int rast_bound,int new_rast_bound,int delta,int mini_w,int scale);
static Bool IfWhiteRow(RecRaster* rast,int row);
/*********************************************************************************************/
RCUTP_FUNC(Bool32) RCUTP_RSelStr_SetEnv(/*char* szImageName,*/ int _medium_h, int _inf_let_w,
										int _inf_dust_h, int _inf_let_h, Handle hCCOM,
										int _sup_dust_w, int _min_cut_down_let_w,
										int _sup_prob_w, const uint16_t biBit)
{
	if (_medium_h < 0 || _inf_let_w < 0 || _inf_dust_h < 0 || _inf_let_h < 0 ||
		_sup_dust_w < 0 || _min_cut_down_let_w < 0 || _sup_prob_w < 0 || !hCCOM)
		return FALSE;

//	if (!GetPageInfo(HCPAGE,&info)) return FALSE;
//	memcpy(&info, pInfo, sizeof(PAGEINFO));

//	image_name = szImageName;
	biBitCount = biBit;
	medium_h = _medium_h;
	inf_let_w = _inf_let_w;
	inf_dust_h = _inf_dust_h;
	inf_let_h = _inf_let_h;
	sup_dust_w = _sup_dust_w;
	min_cut_down_let_w = _min_cut_down_let_w;
	sup_prob_w = _sup_prob_w;
	HCCOM = hCCOM;
	env_set = true;

	return TRUE;
}
/*********************************************************************************************/
RCUTP_FUNC(void) RCUTP_RSelStr_UnsetEnv()
{
	env_set = false;
}
/*********************************************************************************************/
RCUTP_FUNC(int) RCUTP_RSelStr_CP(CCOM_comp* comp, Bool* type_let, uchar* pmasp)
{
	if (!comp || !env_set || !pmasp) return 0;

	count_cut_let = GetCountCutLetters(comp, pmasp);

	return IfNeedCut(comp, type_let);
}
/*********************************************************************************************/
int GetCountCutLetters(CCOM_comp* comp, uchar* pmasp)
{
 uchar Data[1000];
 memset (Data, 0, sizeof (Data));
// uchar* pmasp=Data;
 Rect16 Rc;


// if(comp->h <= 2*medium_h)
 if(comp->h < 3*medium_h)
	 return 2;


 Rc.top=comp->upper;
 Rc.bottom=comp->upper+comp->h-1;
 Rc.left=comp->left;
 Rc.right=comp->left+comp->w-1;

/*
 if(!GetMasP(Rc,&pmasp))
	 return comp->h/medium_h;
*/

 int count=0;
 int w=Rc.right-Rc.left+1;
 int h=Rc.bottom-Rc.top+1;
 double* proc_black=new double[h];
 if(!proc_black)
	 return comp->h/medium_h;

 int newstr;
 int bytewide=(w+7)/8;
 uchar bytep;
 int rect=128>>(7-(bytewide*8-w));
 int count_black=0;
 int i;
 int k;
 int j;

 for(j=h-1;j>=0;j--)
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
/*********************************************************************************************/
int IfNeedCut(CCOM_comp* comp, Bool* type_let)
{
 int bound;
 int upper=-1;
 int downer=-1;
 *type_let=0;

 if(count_cut_let==2&&comp->w>inf_let_w+2&&comp->h<medium_h*2)
 {
	 if(IfZpt(comp))
	 {
		 *type_let=1;
		 bound=comp->upper+comp->h/4;
	 }
	 else
	 {
	  if(IfCav(comp))
	  {
		  *type_let=2;
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

LDPUMA_ConsoleN("bound=%d", bound);

 return bound;
}

/*********************************************************************************************/
Bool JNotInMas2(int j,int* mas,int size,int bound)
{
  int i;
  for(i=j+medium_h;i>=j;i--)
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
/*********************************************************************************************/
Bool JNotInMas(int j,int* mas,int size,int bound)
{
	for(size--;size>bound;size--)
	{
		if(mas[size]==j)
			return FALSE;
	}
	return TRUE;
}
/*********************************************************************************************/
Bool IfZpt(CCOM_comp* comp)
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
/*********************************************************************************************/
Bool IfCav(CCOM_comp* comp)
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
//Andrey: rast.lnPixWidth is not initialized here yet
// int real_right=rast.lnPixWidth;
 rast.lnPixWidth = rast1.lnPixWidth;
 int real_right=rast.lnPixWidth;

 if((GetRastWide(&rast,real_left,real_right)<<comp->scale)>inf_let_w)
	 return FALSE;

 real_left=comp->left+(real_left<<comp->scale);
 real_right=comp->left+(real_right<<comp->scale);

 if(IfHightLetters(comp,FALSE,real_left,real_right))
	 return TRUE;

 return FALSE;
}
/*********************************************************************************************/
int GetBoundFromStr(CCOM_comp* comp,Bool IfCav,int& max_bottom,int& min_top)
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
/*********************************************************************************************/
Bool NotNeedCut(CCOM_comp* comp)
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

 Bool ret1=FALSE;
 Bool ret2=FALSE;

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
/*********************************************************************************************/
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
 uchar bytep;
 Bool Not_move_down=FALSE;

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
/*********************************************************************************************/
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
/*********************************************************************************************/
Bool IfHightLetters(CCOM_comp* comp,Bool Up,int real_left,int real_right)
{
  int Root_Up=comp->upper;
  int Root_Down=comp->upper+comp->h-1;
  int left=real_left;
  int right=real_right;
  int double_width=inf_let_w*3;
  Bool fl_as_cav=FALSE;

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
/*********************************************************************************************/
Bool IfNearLetters(CCOM_comp* cutting,CCOM_comp* comp)
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
/*********************************************************************************************/
void NotCutDownLetter(RecRaster* rast,int& bound,int rast_bound,int new_rast_bound,int delta,int mini_w,int scale)
{
 int left_bound;
 int right_bound;
 uchar bytep;
 int bytewide=((rast->lnPixWidth+63)/64)*8;
 Bool fl_cut=FALSE;
 Bool fl_break=FALSE;
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
/*********************************************************************************************/
Bool IfWhiteRow(RecRaster* rast,int row)
{
 int bytewide=8*((rast->lnPixWidth+63)/64);
 int byte_row=row>>3;
 int rect_row=128>>(row%8);
 uchar* Raster=rast->Raster;
 for(int i=rast->lnPixHeight-1;i>=0;i--)
 {
	 if(Raster[i*bytewide+byte_row]&rect_row)
           return FALSE;
 }
 return TRUE;
}
/*********************************************************************************************/
