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

#include "cutstr.h"
#include "rselstr.h"
#include <assert.h>
#include "compat_defs.h"



int IfStr(Rect16* pN,int nN,Bool vertical)
{
 int let_h;
 int let_w;
 int count=0;
	for(int i=0;i<nN;i++)
	{
		if(vertical)
		{
		    let_h=pN[i].right-pN[i].left+1;
			let_w=pN[i].bottom-pN[i].top+1;
		}
		else
		{
			let_h=pN[i].bottom-pN[i].top+1;
			let_w=pN[i].right-pN[i].left+1;
		}
		if(let_h>inf_let_h&&let_w>inf_let_h/3)
			count++;
		if(count>=1)
			return 1;
	}
 return 0;
}

void StrDrawRect(Handle wnd,uint32_t OperCode,uint32_t color,Rect16 *pRc,int nRc,Bool vert,Rect16 Rc)
{
	int i;
	Rect16 Rect;
	for (i=0; i<nRc; i++)
	{
	 if(!vert)
	 {
		Rect.bottom=pRc[i].bottom;
		Rect.left=pRc[i].left;
		Rect.right=pRc[i].right+1;
		Rect.top=pRc[i].top-1;
	 }
	 else
	 {
		 Rect.left=pRc[i].top;
		 Rect.right=pRc[i].bottom+1;
		 Rect.bottom=Rc.bottom;
		 Rect.top=Rc.top-1;
	 }
		LDPUMA_DrawRect (wnd, &(Rect),0, color, 1,OperCode );
	}

}


void StrDrawRect(Handle wnd,uint32_t OperCode,uint32_t color,Rect16 Rc)
{

	Rect16 Rect;

		Rect.bottom=Rc.bottom;
		Rect.left=Rc.left;
		Rect.right=Rc.right+1;
		Rect.top=Rc.top-1;

		LDPUMA_DrawRect (wnd, &(Rect),0, color, 1,OperCode );

}


Handle GetStrCCOM(Handle hCPage,uchar* ImageName,Rect16 Rc,Bool neg,Bool vertical)
{
 int min_h,min_w,max_h,max_w;
 int j;
 PAGEINFO info = {0};
 uchar Name[CPAGE_MAXNAME];
 GetPageInfo(hCPage,&info);

 if(ImageName)
 {
	 for (j=0; j<CPAGE_MAXNAME; j++)
		Name[j] = ImageName[j];
 }
 else
 {
 for (j=0; j<CPAGE_MAXNAME; j++)
		Name[j] = info.szImageName[j];
 }
 Handle lpDIB;
 if(!CIMAGE_ReadDIB(Name,&lpDIB,1))
	 return 0;
 ExcControl Control;
 if( vertical )
 {
	  min_h=min_let_w;
	  min_w=min_let_h;
	  max_w=Rc.right-Rc.left+1;
	  max_h=max_w*2;

 }
 else
 {
      min_h=min_let_h;
	  min_w=min_let_w;
	  max_h=Rc.bottom-Rc.top+1;
	  max_w=max_h*2;
 }
 Control.MinCompHei=min_h;
 Control.MinCompWid=min_w;
 Control.MaxCompHei=max_h;
 Control.MaxCompWid=max_w;
 Control.MaxScale=1;
 Control.Control=Ex_ExtraComp|Ex_DisableCut;
 if(neg)
	 Control.Control|=Ex_Invert;

 if(REXCExtraDIB(Control,(uchar*)(lpDIB),Rc.left,Rc.top,Rc.right-Rc.left+1,Rc.bottom-Rc.top+1))
    return REXCGetContainer();
 return 0;
 }


void Invert(Rect16* pRc,uchar* pmasp)
{
 int h=pRc[0].bottom-pRc[0].top+1;
 int w=pRc[0].right-pRc[0].left+1;
 int bytewide=(w)/8;
 if( ((w)%8) != 0)
	 bytewide++;
 int j=h*bytewide;
 for(int i=0;i<j;i++)
 	 pmasp[i]=~(pmasp[i]);

}


void CopyP(Rect16* pRc,uchar* Raster,uchar* pmasp)
{
 int h=pRc[0].bottom-pRc[0].top+1;
 int w=pRc[0].right-pRc[0].left+1;
 int bytewide=(w)/8;
 if( ((w)%8) != 0)
	 bytewide++;
 int j=h*bytewide;
 for(int i=0;i<j;i++)
     Raster[i]=pmasp[i];

}

void ToHorizont(Rect16* pRc,uchar* Raster,uchar* pmasp)
{
 int h=pRc[0].bottom-pRc[0].top+1;
 int w=pRc[0].right-pRc[0].left+1;
 int bytewide=(w)/8;
 if( ((w)%8) != 0)
	 bytewide++;
 int vbytewide=(h)/8;
 if( ((h)%8) != 0)
	 vbytewide++;
 uchar bytep;
 int i,k;
 int nowbyte=0;
 int nowbyte2;
 int delj;
 int rectj;

 for(int j=0;j<w;j++)
 {
   delj=j>>3;
   rectj=j%8;
   for(i=0;i<vbytewide-1;i++)
   {
	bytep=0;
	nowbyte2=(h-1-(i<<3));

  //k=0
	   bytep|=( ( (128>>rectj) & pmasp[(nowbyte2)*bytewide+delj] )<<rectj );

  //k=1
	   bytep|=( ( (128>>rectj) & pmasp[(nowbyte2-1)*bytewide+delj] )<<rectj )>>1;

  //k=2
	   bytep|=( ( (128>>rectj) & pmasp[(nowbyte2-2)*bytewide+delj] )<<rectj )>>2;

  //k=3
	   bytep|=( ( (128>>rectj) & pmasp[(nowbyte2-3)*bytewide+delj] )<<rectj )>>3;

  //k=4
	   bytep|=( ( (128>>rectj) & pmasp[(nowbyte2-4)*bytewide+delj] )<<rectj )>>4;

  //k=5
	   bytep|=( ( (128>>rectj) & pmasp[(nowbyte2-5)*bytewide+delj] )<<rectj )>>5;

  //k=6
	   bytep|=( ( (128>>rectj) & pmasp[(nowbyte2-6)*bytewide+delj] )<<rectj )>>6;

  //k=7
	   bytep|=( ( (128>>rectj) & pmasp[(nowbyte2-7)*bytewide+delj] )<<rectj )>>7;

	Raster[nowbyte]=bytep;
	nowbyte++;
   }
   bytep=0;
   nowbyte2=(h-1-(i<<3));
   for(k=0;k<h-((vbytewide-1)<<3);k++)
	   bytep|=( ( (128>>rectj) & pmasp[(nowbyte2-k)*bytewide+delj] )<<rectj )>>k;
   Raster[nowbyte]=bytep;
   nowbyte++;
 }

 pRc[0].top=pRc[0].left;
 pRc[0].bottom=pRc[0].right;
 pRc[0].left=0;
 pRc[0].right=h-1;

}




RSELSTR_FUNC(Bool32) RSELSTR_CutStr(Rect16** ppRc,int& nRc,int& len_mas,uchar* pmasp,int DPIX,int DPIY,int str_w)
{
 inf_str_h=15;
 if(str_w<=0)
	 inf_str_w=8;
 else
	 inf_str_w=str_w;
 inf_let_h=10;
 min_let_w=2;
 min_let_h=2;
 inf_betw_str_h=10;
 inf_str_h=inf_str_h*((int)(DPIY)+1)/300;
 inf_str_w=inf_str_w*((int)(DPIY)+1)/300;
 inf_let_h=inf_let_h*((int)(DPIY)+1)/300;
 inf_betw_str_h=inf_betw_str_h*((int)(DPIY)+1)/300;
 min_let_w=min_let_w*((int)(DPIY)+1)/300;
 min_let_h=min_let_h*((int)(DPIY)+1)/300;


 if(nRc<=0) return FALSE;
 int i;
 int h,w;
 int add_len_mas=5;
 Rect16* pRc=(*ppRc);
 int up;
 int oldtop;
 Bool white=TRUE;

 for(i=nRc-1;i>=0;i--)
 {
  w=pRc[i].right-pRc[i].left+1;
  h=pRc[i].bottom-pRc[i].top+1;
  int bytewide;
  if( (w%8)==0)
		bytewide=w/8;
  else
  bytewide=w/8+1;
  int j=7-(bytewide*8-w);
  int rect=128>>j;
  int k;
  int top=pRc[i].top;
  oldtop=top;
  uchar bytep;
  int count_black=0;
  double proc;
  Bool str=FALSE;
  int oldoldtop=pRc[i].top;
  int W;
  int rightbyte;
  int leftbyte;
  int nowbyte;

  up = pRc[i].top;

  while(pRc[i].bottom-top>=0)
  {
   count_black=0;
   leftbyte=0;
   rightbyte=bytewide-1;
   if(pRc[i].bottom-top>=inf_let_h)
	   W=GetStrW(pmasp,inf_let_h,top-oldoldtop,h,w,leftbyte,rightbyte);
   else
	   W=w;
   if(W==w) rightbyte--;
   nowbyte=(top-oldoldtop)*bytewide;
   for(j=leftbyte;j<=rightbyte;j++)
   {
    bytep=pmasp[nowbyte+j];

	   if( (bytep&128)==0 ) count_black++;
	   if( (bytep&64)==0 ) count_black++;
	   if( (bytep&32)==0 ) count_black++;
	   if( (bytep&16)==0 ) count_black++;
	   if( (bytep&8)==0 ) count_black++;
	   if( (bytep&4)==0 ) count_black++;
	   if( (bytep&2)==0 ) count_black++;
	   if( (bytep&1)==0 ) count_black++;

   }
   if(w==W)
   {
    bytep=pmasp[nowbyte+bytewide-1];
    for(k=128;k>=rect;k=k>>1)
      if( (bytep&k)==0 ) count_black++;
   }
   proc=((double)(W-count_black))/((double)(W));
   if(white==TRUE)
   {
	 if(proc<=cut_str)
	 {
	    if(IfNewStr(pmasp,top-oldoldtop,h,bytewide,w,k,rect))
		{
		 if(nRc==1&&!str)
		    up=top;
		 white=FALSE;
		   if(str)
		   {
			if (nRc>=len_mas)
			{
		     if(!(AddLenStrMas(ppRc,len_mas,add_len_mas)) )
				 return FALSE;
			 pRc=(*ppRc);
			}
            StrMoveMasR(pRc,nRc,i);
			pRc[i+1].top=pRc[i].top;
			pRc[i+1].bottom=oldtop+(top-oldtop)/3;
			pRc[i].top=pRc[i+1].bottom+1;
		   }
		}
		else
		{
		   if(str)
		   {
			if (nRc>=len_mas)
			{
		     if(!(AddLenStrMas(ppRc,len_mas,add_len_mas)) )
				 return FALSE;
			 pRc=(*ppRc);
			}
            StrMoveMasR(pRc,nRc,i);
			pRc[i+1].top=pRc[i].top;
			pRc[i+1].bottom=top-1;
		   }
		   top+=k-1;
		   pRc[i].top=top;
		}

	 }

   }
   else
   {
	 if(proc>=cut_white)
	 {
		white=TRUE;
        oldtop=top;
		str=TRUE;
	 }

   }
   top++;
  }
 }

 if(pRc[nRc-1].top<up-inf_let_h)
	 pRc[nRc-1].top=up-inf_let_h;

 for(i=nRc-1;i>=0;i--)
 {
	 if(pRc[i].bottom-pRc[i].top+1<inf_str_h)
		 StrMoveMas(pRc,nRc,i);
 }
 return TRUE;
}


int GetStrW(uchar* pmasp,int spusk,int from,int h,int w,int& leftbyte,int& rightbyte)
{
 int bytewide=(w+7)/8;
 int j=7-(bytewide*8-w);
 int rect=128>>j;
 int k;
 int real_spusk=spusk;
 if(from+real_spusk>h)
	 real_spusk=h-from;
 int count_black;
 uchar bytep;

 leftbyte=0;
 rightbyte=bytewide-1;


 while(w-(leftbyte<<3)>=inf_str_w)
 {
  count_black=0;
  for(j=from;j<from+real_spusk;j++)
  {
	  bytep=pmasp[j*bytewide+leftbyte];

       if( (bytep&128)==0 ) count_black++;
	   if( (bytep&64)==0 ) count_black++;
	   if( (bytep&32)==0 ) count_black++;
	   if( (bytep&16)==0 ) count_black++;
  }
  if((double)(count_black)/(double)(real_spusk<<2)>=inf_let_black)
	  break;

  for(j=from;j<from+real_spusk;j++)
  {
	  bytep=pmasp[j*bytewide+leftbyte];

	   if( (bytep&8)==0 ) count_black++;
	   if( (bytep&4)==0 ) count_black++;
	   if( (bytep&2)==0 ) count_black++;
	   if( (bytep&1)==0 ) count_black++;

  }
  if((double)(count_black)/(double)(real_spusk<<2)>=inf_let_black)
	  break;
  leftbyte++;
 }

 if(w-leftbyte*8<inf_str_w)
 {
	leftbyte=0;
	rightbyte=bytewide-1;
    return w;
 }

 count_black=0;
 for(j=from;j<from+real_spusk;j++)
 {
	 bytep=pmasp[j*bytewide+rightbyte];
   for(k=128;k>=rect;k=k>>1)
	   if( (bytep&k)==0 ) count_black++;
 }
 if((double)(count_black)/(double)((8-(bytewide*8-w))*real_spusk)>=inf_let_black)
   return w-leftbyte*8;

 rightbyte--;
 while(((rightbyte-leftbyte+1)<<3)>=inf_str_w)
 {
  count_black=0;
  for(j=from;j<from+real_spusk;j++)
  {
	  bytep=pmasp[j*bytewide+rightbyte];

       if( (bytep&128)==0 ) count_black++;
	   if( (bytep&64)==0 ) count_black++;
	   if( (bytep&32)==0 ) count_black++;
	   if( (bytep&16)==0 ) count_black++;
  }
  if((double)(count_black)/(double)(real_spusk<<2)>=inf_let_black)
	  break;


  for(j=from;j<from+real_spusk;j++)
  {
	  bytep=pmasp[j*bytewide+rightbyte];

	   if( (bytep&8)==0 ) count_black++;
	   if( (bytep&4)==0 ) count_black++;
	   if( (bytep&2)==0 ) count_black++;
	   if( (bytep&1)==0 ) count_black++;

  }
  if((double)(count_black)/(double)(real_spusk<<2)>=inf_let_black)
	  break;
  rightbyte--;
 }

 if((rightbyte-leftbyte+1)*8<inf_str_w)
 {
	leftbyte=0;
	rightbyte=bytewide-1;
    return w;
 }

 else
	 return (rightbyte-leftbyte+1)*8;
}



Bool IfNewStr(uchar* pmasp,int i,int h,int bytewide,int w,int& l,int rect)
{
// if(LDPUMA_Skip (CutStrDel) )
	 return TRUE;
 int count_black=0;
 double proc;
 int j;
 int W;
 uchar bytep;
 int k;
 int leftbyte;
 int rightbyte;
 int nowbyte=0;
  for(l=i;l<h;l++)
  {
   nowbyte=l*bytewide;
   count_black=0;
   W=GetStrW(pmasp,inf_let_h,l,h,w,leftbyte,rightbyte);
   if(w==W) rightbyte--;
   for(j=leftbyte;j<=rightbyte;j++)
   {
    bytep=pmasp[nowbyte+j];

       if( (bytep&128)==0 ) count_black++;
	   if( (bytep&64)==0 ) count_black++;
	   if( (bytep&32)==0 ) count_black++;
	   if( (bytep&16)==0 ) count_black++;
	   if( (bytep&8)==0 ) count_black++;
	   if( (bytep&4)==0 ) count_black++;
	   if( (bytep&2)==0 ) count_black++;
	   if( (bytep&1)==0 ) count_black++;

  }
   if(w==W)
   {
    bytep=pmasp[nowbyte+bytewide-1];
    for(k=128;k>=rect;k=k>>1)
      if( (bytep&k)==0 ) count_black++;
   }
   proc=((double)(W-count_black))/((double)(W));
   if(proc>=cut_white)
	   break;
  }
 if(l-i-1>=inf_let_h)
	 return TRUE;
 return FALSE;
}


void StrMoveMas(Rect16* pRc,int& nRc,int num)
{
 int i;
  for(i=num;i<(nRc-1);i++)
  {
	  pRc[i].left=pRc[i+1].left;
      pRc[i].top=pRc[i+1].top;
	  pRc[i].right=pRc[i+1].right;
	  pRc[i].bottom=pRc[i+1].bottom;
  }
  nRc--;
}

void StrMoveMas(CCOM_comp** pRc,int& nRc,int num)
{
 int i;
  for(i=num;i<(nRc-1);i++)
  {
	  pRc[i]=pRc[i+1];
  }
  nRc--;
}

void StrMoveMas(int* phi,int& n,int num)
{
 int i;
  for(i=num;i<(n-1);i++)
   phi[i]=phi[i+1];
  n--;
}


void StrMoveMasR(Rect16* pRc,int& nRc,int num)
{
 int i;
  for(i=nRc;i>num;i--)
  {
	  pRc[i].left=pRc[i-1].left;
      pRc[i].top=pRc[i-1].top;
	  pRc[i].right=pRc[i-1].right;
	  pRc[i].bottom=pRc[i-1].bottom;
  }
  nRc++;
}

Bool GetMasP(Handle hCPage,uchar* ImageName,Rect16 Rc,uchar** ppmasp)
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
	uchar Name[CPAGE_MAXNAME];
	Bool ret;
	int i;

	GetPageInfo(hCPage,&info);
	if(ImageName)
	{
        for (i=0; i<CPAGE_MAXNAME; i++)
		    Name[i]=ImageName[i];
	}
	else
	{
		for (i=0; i<CPAGE_MAXNAME; i++)
	    	Name[i] = info.szImageName[i];
	}

	/*  1. Подготовка к запросу части изображения.  */
	DataInto.dwHeight   = (uint32_t)(h);
	DataInto.dwWidth    = (uint32_t)(prewide);
	DataInto.wByteWidth = (uint16_t)(prewide/8);
	DataInto.dwX        = left;
	DataInto.dwY        = upper;
	DataInto.MaskFlag   = 0x00;

	DataOut.dwWidth    = DataInto.dwWidth;
	DataOut.dwHeight   = DataInto.dwHeight;
	DataOut.wByteWidth = DataInto.wByteWidth;
	DataOut.byBit      = (uint16_t)info.BitPerPixel;
	DataOut.lpData     = *ppmasp;
	/*  5. Чтение части изображения.  */
	ret = CIMAGE_GetData (Name, &DataInto, &DataOut);
	if (!ret)
		return FALSE;
	if (DataOut.lpData==NULL)
		return FALSE;
	*ppmasp = DataOut.lpData;
	return TRUE;
}

void InitRc(Rect16* pRc,int nRC,CCOM_comp * pcomp)
{
 pRc[nRC].left   = pcomp->left;
 pRc[nRC].right  = pcomp->left + pcomp->w - 1;
 pRc[nRC].top    = pcomp->upper;
 pRc[nRC].bottom = pcomp->upper + pcomp->h - 1;
}


//////////////////////////////////////////////////////////////////////////
// GF [ 2004/01/29 ] ==--->

//////////////////////////////////////////////////////////////////////////
// =--> отладочная вставка для слежения за качеством заплатки
		 // позвать отладчик из кода
		 #ifdef _X86_
			 #define DebugBreak()    _asm { int 3 }

			 // для DebugBreakEx() (см. ниже);
			 // существует только в Windows старше W98 (W98 годится)
			 // или старше NT 4.0 (NT 4.0 годится)
			 #ifndef WINBASEAPI
			 #define WINBASEAPI
			 #endif
			 #ifndef VOID
			 #define VOID
			 #endif
			 extern "C" {
			 WINBASEAPI Bool WINAPI IsDebuggerPresent(VOID);
			 }

			 // остановка под отладчиком:
			 #define DebugBreakEx()    \
				do { if ( IsDebuggerPresent() ) \
				DebugBreak(); } while (0)

		 #endif
// <--= отладочная вставка для слежения за качеством заплатки
//////////////////////////////////////////////////////////////////////////

Bool InitStrMas(Rect16** ppRc,int len)
{
	if (NULL != *ppRc)
	{
      // проверка корректности заплатки (вызов IsBadWritePtr() специфичен для платформы Windows)
			if ( IsBadWritePtr( (pvoid)(*ppRc), sizeof(Rect16) ) )
			{
					// если мы здесь, то где-то перед вызовом InitStrMas()
					// (*ppRc) объявлен, но не обнулен (надо найти, где и обнулить)
					#ifdef _DEBUG
								DebugBreakEx();
					#else
					      OutputDebugString("puma/RSELSTR/chstr_cutstr.cpp(~688) {InitStrMas(), заплатка на утечку памяти}: на этом файле надо отладиться!\n");
					#endif
					//#ifdef _DEBUG
					{ // устроить один раз ассерт
					  static bool bCallOnce = true;
            assert(bCallOnce); // см. комментарий 10 строками выше
						bCallOnce = false;
					}
			}
			else // нам передали для обнуления указатель,
			{    // указывающий на "живую" память -- так что
			     // скорее всего, мы имеем дело с утечкой
				// !!!
				// или так: DelStrMas(ppRc);
				delete [] (*ppRc);
				// !!!
			}
			(*ppRc)=NULL;
	}

	if(!((*ppRc)=new Rect16[len]))
		return FALSE;
	return TRUE;
}

Bool InitStrMas(CCOM_comp*** ppRc,int len)
{
	(*ppRc)=NULL;
	if(!((*ppRc)=new CCOM_comp*[len]))
		return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// GF [ 2004/01/29 ] ==--->
void DelStrMas(Rect16** pmasp)
{
	delete[] (*pmasp);
  (*pmasp) = NULL;
}

void DelStrMas(CCOM_comp** masp)
{
	delete[] masp;
}

Bool AddLenStrMas(Rect16** ppRc,int& len,int add)
{
 Rect16 *dop = NULL;
 int i;
 if(!(InitStrMas(&dop,len)) )
	 return FALSE;
 for(i=0;i<len;i++)
	 dop[i]=(*ppRc)[i];
 // GF: killed memory leak on 2004.01.29
 DelStrMas(ppRc); *ppRc = NULL;
 if(!(InitStrMas(ppRc,len+add)) )
 {   (*ppRc)=dop;
	 return FALSE;
 }
 for(i=0;i<len;i++)
	 (*ppRc)[i]=dop[i];
 len+=add;
 // GF: killed memory leak on 2004.01.29
 DelStrMas(&dop); dop = NULL;
 return TRUE;
}

Bool AddLenStrMas(CCOM_comp*** ppRc,int& len,int add)
{
 CCOM_comp** dop;
 int i;
 if(!(InitStrMas(&dop,len)) )
	 return FALSE;
 for(i=0;i<len;i++)
	 dop[i]=(*ppRc)[i];
 DelStrMas(*ppRc);
 if(!(InitStrMas(ppRc,len+add)) )
 {   (*ppRc)=dop;
	 return FALSE;
 }
 for(i=0;i<len;i++)
	 (*ppRc)[i]=dop[i];
 len+=add;
 DelStrMas(dop);
 return TRUE;
}



void SortLetter(CCOM_comp** ppComp,int n,Bool vert)
{
 CCOM_comp* comp;
 int j,i;

 for(i=n-1 ; i>0 ; i--)
 {
  for(j=1 ; j<=i ; j++)
  {

	  if( ((!vert)&&(ppComp[j-1]->left>ppComp[j]->left))||((vert)&&(ppComp[j-1]->upper+ppComp[j-1]->h<ppComp[j]->upper+ppComp[j]->h)) )
	  {
		  comp=ppComp[j];
		  ppComp[j]=ppComp[j-1];
		  ppComp[j-1]=comp;
	  }
  }
 }
}
