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
#include "rotate.h"
#include "rselstr.h"

/*    из-за чего-то происзодит залезание при чтении на границы pmasp,
      которое проявляется только в релизе и раз на раз не приходится...
      Проявляется только тогда, когда после памяти, выделенной под pmasp,
	  находится неопределенная память (вида ???????). Ошибка не исправлена,
	  поставлена только простейшая проверка. Ошибку еще нужно искать в логике!!!!!!!!!
*/
#ifdef _DEBUG

#include <crtdbg.h>
#define CHECK_PMASP(nowbyte) \
{ \
	if (nowbyte < 0 || nowbyte >= size_pmasp) \
	{ \
		_ASSERT(0); \
		return FALSE; \
	} \
}

#else

#define CHECK_PMASP(nowbyte) \
{ \
	if (nowbyte < 0 || nowbyte >= size_pmasp) \
	{ \
		return FALSE; \
	} \
}

#endif








RSELSTR_FUNC(Bool32) RSELSTR_RotateRaster(uchar* pmasp,int skew,Rect16* Rc,int16_t* begx,int16_t* movey,uchar* flmovey,int inf_betw_str_h)
{
 int oldskew=skew;
 if(skew<0)
	 skew=-skew;
 if(skew>=150)
	 return FALSE;
 int w;
 int h;

	 w=Rc->right-Rc->left+1;
	 h=Rc->bottom-Rc->top+1;
 int size_pmasp = h*((w+7)/8);

 if((double)(w*skew)/2048.<(double)(inf_betw_str_h))
	 return FALSE;


/* double a=(2048.*2048.)/(double)(skew*skew);
 a-=a*a;
 a+=1.;
 a-=(double)(skew*skew)/(2048.*2048); //a<=0;
 double b=(double)(w*skew)/1024.-(double)(h*skew)/1024.;
 b-=((double)(2*w)*2048.*2048.)/(double)(skew*skew);
 b+=((double)(2*h)*2048.*2048.*2048.)/((double)(skew*skew)*(double)(skew));
 double c=(double)(-h*h)-(double)(w*w);
 c-=((double)(skew*skew)*(double)(w*w))/(2048.*2048.)-((double)(h*w)*(double)(skew))/1024.;
 c-=((double)(h*h)*2048.*2048.)/(double)(skew*skew);
 c+=((double)(w*h)*4096.)/(double)(skew); //c<=0
 double D=b*b-4.*a*c;

 int x;
 if(D<0)
	 return FALSE;
 D=sqrt(D);
 x=(int)((-b+D)/(2.*a));
 x=(int)((-b-D)/(2.*a));
*/
 int16_t x=(int16_t)( ((double)(w)-(double)(h*2048)/(double)(skew))/(1.-2048.*2048./(double)(skew*skew)) );
 if((x<0)||(x>w))
	 return FALSE;
 if(x==0)
    x++;
 int16_t y=(int16_t)( (((double)(h)*2048.)/(double)(skew)-(double)(w))/(2048./(double)(skew)-(double)(skew)/2048.) );
 if((y<0)||(y>h))
	 return FALSE;
 y=h-y;
// begin insert by tanya 2003.09.15
// if(y==0)
//    y++;
// end insert by tanya 2003.09.15
 y++; // not delete by tanya 2003.09.15,
   /* не из-за этого происзодит залезание при чтении на границы pmasp,
      которое проявляется только в релизе и раз на раз не приходится...
      Проявляется только тогда, когда после памяти, выделенной под pmasp,
	  находится неопределенная память (вида ???????)
   */


 int16_t i,j;
 int numstr,newnumstr;
 int16_t bytewide,newbytewide;
 int nowbyte=0;


 int16_t newh;
 int16_t neww;


 if(oldskew<0)
 {

  for(i=x-1;i<w;i++)
   movey[i]=okrug((double)(skew*(i-x-1))/2048.);
  for(i=x-1;i>=0;i--)
   movey[i]=okrug((double)(2048*(x-1-i))/(double)(skew));

  for(i=x;i<w;i++)
  {
	  if(movey[i]-movey[i-1])
		  flmovey[i]=1;
	  else
		  flmovey[i]=0;
  }
  flmovey[0]=0;
  for(j=x-1;j<w;j++)
  {
	  if(movey[j]>movey[x-1])
	  break;
  }
  j-=x-1;
  int16_t k;
  i=x-1;
  while(i)
  {
   	 k=j-1;
	 flmovey[i]=1;
	 while(k&&i)
	 {
	  i--;
	  k--;
      flmovey[i]=0;
	 }
  }

  i=x-2;k=0;
  flmovey[w]=0;
  while(i>=0)
  {
   if(movey[i]>movey[i+1]&&movey[i+1]+k<h)
     begx[movey[i+1]+k]=i+1;
   j=movey[i+1]+1+k;
   if(flmovey[i+1]) k++;
   for(j;j<movey[i]+k&&j<h;j++)
     begx[j]=begx[j-1];
   i--;
  }
  if(movey[0]+k<h)
      begx[movey[0]+k]=0;
  for(i=movey[0]+k+1;i<=h;i++)
	  begx[i]=0;

  neww=w-x+1;
  newh=h-movey[neww-1];
  if(neww<=0||newh<=0)
	  return FALSE;

  uchar* safe=NULL;
  newbytewide=((neww+7)/8);
  bytewide=(w+7)/8;

  if(!(safe=new uchar[(int)(newbytewide)*(int)(newh)]))
	 return FALSE;

  nowbyte=0;

  newbytewide=(neww+7)/8-1;

  int realpj;
  numstr=begx[0]>>3;
  newnumstr=0;
  int16_t rect=(neww%8);
  if(rect==0)
	  rect=8;
  int16_t realrect;

  for(i=0;i<newh;i++)
  {
	nowbyte=numstr;
	realpj=begx[i];
	realrect=realpj%8;
	for(j=0;j<newbytewide;j++)
	{
		safe[newnumstr]=0;


	//k=0
          CHECK_PMASP(nowbyte)
          safe[newnumstr]|=( 128 & (((128>>realrect)&pmasp[nowbyte])<<realrect) );
		  realpj++;
		  realrect=realpj%8;
		  if(!realrect)
			  nowbyte++;
		  if(flmovey[realpj])
			  nowbyte+=bytewide;

	//k=1
          CHECK_PMASP(nowbyte)
		  safe[newnumstr]|=( ( 128 & (((128>>realrect)&pmasp[nowbyte])<<realrect) ) >>1 );
		  realpj++;
		  realrect=realpj%8;
		  if(!realrect)
			  nowbyte++;
		  if(flmovey[realpj])
			  nowbyte+=bytewide;

	//k=2
          CHECK_PMASP(nowbyte)
		  safe[newnumstr]|=( ( 128 & (((128>>realrect)&pmasp[nowbyte])<<realrect) ) >>2 );
		  realpj++;
		  realrect=realpj%8;
		  if(!realrect)
			  nowbyte++;
		  if(flmovey[realpj])
			  nowbyte+=bytewide;

	//k=3
          CHECK_PMASP(nowbyte)
		  safe[newnumstr]|=( ( 128 & (((128>>realrect)&pmasp[nowbyte])<<realrect) ) >>3 );
		  realpj++;
		  realrect=realpj%8;
		  if(!realrect)
			  nowbyte++;
		  if(flmovey[realpj])
			  nowbyte+=bytewide;


	//k=4
		  CHECK_PMASP(nowbyte)
		  safe[newnumstr]|=( ( 128 & (((128>>realrect)&pmasp[nowbyte])<<realrect) ) >>4 );
		  realpj++;
		  realrect=realpj%8;
		  if(!realrect)
			  nowbyte++;
		  if(flmovey[realpj])
			  nowbyte+=bytewide;


	//k=5
		  CHECK_PMASP(nowbyte)
		  safe[newnumstr]|=( ( 128 & (((128>>realrect)&pmasp[nowbyte])<<realrect) ) >>5 );
		  realpj++;
		  realrect=realpj%8;
		  if(!realrect)
			  nowbyte++;
		  if(flmovey[realpj])
			  nowbyte+=bytewide;


	//k=6
		  CHECK_PMASP(nowbyte)
		  safe[newnumstr]|=( ( 128 & (((128>>realrect)&pmasp[nowbyte])<<realrect) ) >>6 );
		  realpj++;
		  realrect=realpj%8;
		  if(!realrect)
			  nowbyte++;
		  if(flmovey[realpj])
			  nowbyte+=bytewide;

	//k=7
		  CHECK_PMASP(nowbyte)
		  safe[newnumstr]|=( ( 128 & (((128>>realrect)&pmasp[nowbyte])<<realrect) ) >>7 );
		  realpj++;
		  realrect=realpj%8;
		  if(!realrect)
			  nowbyte++;
		  if(flmovey[realpj])
			  nowbyte+=bytewide;


		newnumstr++;
	}
    safe[newnumstr]=0;
	    for(k=0;k<rect;k++)
		{
          CHECK_PMASP(nowbyte)
          safe[newnumstr]|=( ( 128 & (((128>>realrect)&pmasp[nowbyte])<<realrect) ) >>k );
		  realpj++;
		  realrect=realpj%8;
		  if(!(realpj%8))
			  nowbyte++;
		  if(realpj < w && flmovey[realpj])
			  nowbyte+=bytewide;
		}
   newnumstr++;
   if((begx[i+1]!=begx[i])&&(flmovey[begx[i]]))
	   ;
   else
    numstr+=bytewide;
   if(begx[i]!=begx[i+1])
    numstr-=((begx[i]>>3)-(begx[i+1]>>3));
  }

  Rc->bottom=Rc->top+newh-1;
  Rc->right=Rc->left+neww-1;

  newbytewide=(neww+7)/8;
  int j=(int)(newh)*(int)(newbytewide);
  memcpy(pmasp,safe,j);

  delete[] safe;

  return TRUE;
 }

 else
 {
  neww=0;
  for(i=0;i<w;i++)
  {
   movey[i]=okrug((double)(skew*i)/2048.);
   if((y==movey[i])&&(neww==0))
	   neww=i;
  }
  if(neww==0)
	  neww=w-x+1;
  for(i=1;i<w;i++)
  {
	  if(movey[i]-movey[i-1])
		  flmovey[i]=1;
	  else
		  flmovey[i]=0;
  }
  flmovey[0]=0;

  int16_t k=0;

  flmovey[w]=0;

  if(neww<=0)
	  return FALSE;

  newh=1;
  begx[0]=0;
  while(newh-k+y-1<h&&newh<h)
  {
   if(begx[newh-1]+neww>w)
	   neww--;
   begx[newh]=okrug((double)(skew*newh)/2048.);
   if((begx[newh]>begx[newh-1])&&(flmovey[begx[newh]]))
   {
	   k++;
	   newh++;
	   begx[newh]=begx[newh-1];
   }
   newh++;
  }

  if(newh<=0)
	  return FALSE;

  j=begx[newh-1];
  for(i=newh;i<=h;i++)
	  begx[i]=j;

  uchar* safe=NULL;
  newbytewide=((neww+7)/8);
  bytewide=(w+7)/8;

  if(!(safe=new uchar[(int)(newbytewide)*(int)(newh)]))
	 return FALSE;

  int my_temp_size = (int)(newbytewide)*(int)(newh);

  nowbyte=0;

  newbytewide=(neww+7)/8-1;
  bytewide=(w+7)/8;
  int realpj;
  numstr=(y-1)*bytewide;
  newnumstr=0;
  int16_t rect=(neww%8);
  if(rect==0)
	  rect=8;
  int16_t realrect;
  for(i=0;i<newh;i++)
  {
	nowbyte=numstr;
	realpj=begx[i];
	realrect=realpj%8;
	for(j=0;j<newbytewide;j++)
	{

//		if (newnumstr < 0 || newnumstr >= my_temp_size)
//		{
//#ifdef _DEBUG
//			_ASSERT(0);
//#endif
//			return FALSE;
//		}

		safe[newnumstr]=0;

	//k=0

          CHECK_PMASP(nowbyte)
          safe[newnumstr]|=( 128 & (((128>>realrect)&pmasp[nowbyte])<<realrect) );
		  realpj++;
		  realrect=realpj%8;
		  if(!realrect)
			  nowbyte++;
		  if(flmovey[realpj])
			  nowbyte-=bytewide;

	//k=1
          CHECK_PMASP(nowbyte)
		  safe[newnumstr]|=( ( 128 & (((128>>realrect)&pmasp[nowbyte])<<realrect) ) >>1 );
		  realpj++;
		  realrect=realpj%8;
		  if(!realrect)
			  nowbyte++;
		  if(flmovey[realpj])
			  nowbyte-=bytewide;

	//k=2
          CHECK_PMASP(nowbyte)
		  safe[newnumstr]|=( ( 128 & (((128>>realrect)&pmasp[nowbyte])<<realrect) ) >>2 );
		  realpj++;
		  realrect=realpj%8;
		  if(!realrect)
			  nowbyte++;
		  if(flmovey[realpj])
			  nowbyte-=bytewide;

	//k=3
          CHECK_PMASP(nowbyte)
		  safe[newnumstr]|=( ( 128 & (((128>>realrect)&pmasp[nowbyte])<<realrect) ) >>3 );
		  realpj++;
		  realrect=realpj%8;
		  if(!realrect)
			  nowbyte++;
		  if(flmovey[realpj])
			  nowbyte-=bytewide;


	//k=4
          CHECK_PMASP(nowbyte)
		  safe[newnumstr]|=( ( 128 & (((128>>realrect)&pmasp[nowbyte])<<realrect) ) >>4 );
		  realpj++;
		  realrect=realpj%8;
		  if(!realrect)
			  nowbyte++;
		  if(flmovey[realpj])
			  nowbyte-=bytewide;


	//k=5
          CHECK_PMASP(nowbyte)
		  safe[newnumstr]|=( ( 128 & (((128>>realrect)&pmasp[nowbyte])<<realrect) ) >>5 );
		  realpj++;
		  realrect=realpj%8;
		  if(!realrect)
			  nowbyte++;
		  if(flmovey[realpj])
			  nowbyte-=bytewide;


	//k=6
          CHECK_PMASP(nowbyte)
		  safe[newnumstr]|=( ( 128 & (((128>>realrect)&pmasp[nowbyte])<<realrect) ) >>6 );
		  realpj++;
		  realrect=realpj%8;
		  if(!realrect)
			  nowbyte++;
		  if(flmovey[realpj])
			  nowbyte-=bytewide;

	//k=7
          CHECK_PMASP(nowbyte)
		  safe[newnumstr]|=( ( 128 & (((128>>realrect)&pmasp[nowbyte])<<realrect) ) >>7 );
		  realpj++;
		  realrect=realpj%8;
		  if(!realrect)
			  nowbyte++;
		  if(flmovey[realpj])
			  nowbyte-=bytewide;

		newnumstr++;
	}
    safe[newnumstr]=0;
	    for(k=0;k<rect;k++)
		{
          CHECK_PMASP(nowbyte)
          safe[newnumstr]|=( ( 128 & (((128>>realrect)&pmasp[nowbyte])<<realrect) ) >>k );
		  realpj++;
		  realrect=realpj%8;
		  if(!(realpj%8))
			  nowbyte++;
		  if(realpj < w && flmovey[realpj])
			  nowbyte-=bytewide;
		}
   newnumstr++;
   if((begx[i+1]==begx[i])||(!flmovey[begx[i]]))
    numstr+=bytewide;
   if(begx[i]!=begx[i+1])
    numstr+=((begx[i+1]>>3)-(begx[i]>>3));
  }
/*
  //for debug
  bytewide=(neww+7)/8;
  for(i=0;i<newh;i++)
  {
	  for(j=0;j<bytewide;j++)
	  {
		  if(((j<20)||(j>70))&&(safe[i*bytewide+j]!=255))
			  nowbyte=nowbyte;
		  for(k=0;k<8;k++)
			  nowbyte=(int)(((128>>k)&(safe[i*bytewide+j]))>>(7-k));
	  }
  }
*/

  Rc->top=Rc->bottom-newh+1;
  Rc->right=Rc->left+neww-1;

  newbytewide=(neww+7)/8;
  int j=(int)(newh)*(int)(newbytewide);
  memcpy(pmasp,safe,j);


  delete[] safe;

  return TRUE;
 }


 return FALSE;
}

int16_t okrug(double d)
{
	return (int16_t)(d+.5);
}

RSELSTR_FUNC(Bool32) RSELSTR_UnRotateRect(int skew,Rect16* pRc,int nRc,Rect16 Rc,int16_t* begx,int16_t* movey,uchar* flmovey,int* hi)
{
 if(skew<0)
 {
  skew=-skew;
  int Skew=movey[Rc.right-Rc.left];
  int top;
  int bottom;
  int h=pRc[0].bottom-Rc.top+1;

  for(int i=nRc-1;i>=0;i--)
  {
	  bottom=GetUnRotateY(pRc[i].bottom-Rc.top,h,begx,movey,flmovey,-skew)+Rc.top;
	  top=GetUnRotateY(pRc[i].top-Rc.top,h,begx,movey,flmovey,-skew)+Rc.top;
	  hi[i]=bottom-top+1;
	  pRc[i].right+=begx[0];
	  pRc[i].top=top;
	  pRc[i].bottom=bottom+Skew;
  }
 }
 else
 {
  int Skew=movey[pRc[0].right-pRc[0].left];
  int top;
  int bottom;
  int h=Rc.bottom-Rc.top+1-Skew;
  for(int i=nRc-1;i>=0;i--)
  {
	  if(pRc[i].bottom-Rc.top-Skew>=0)
		  bottom=GetUnRotateY(pRc[i].bottom-Rc.top-Skew,h,begx,movey,flmovey,skew)+Rc.top+Skew;
	  else
		  bottom=GetUnRotateY(0,h,begx,movey,flmovey,skew)+Rc.top+Skew;
	  if(pRc[i].top-Rc.top-Skew>=0)
		  top=GetUnRotateY(pRc[i].top-Rc.top-Skew,h,begx,movey,flmovey,skew)+Rc.top+Skew;
	  else
		  top=GetUnRotateY(0,h,begx,movey,flmovey,skew)+Rc.top+Skew;
	  hi[i]=bottom-top+1;
      pRc[i].right+=begx[h-1];
	  pRc[i].top=top-Skew;
	  pRc[i].bottom=bottom;
  }

 }

 return TRUE;
}

int GetUnRotateY(int y,int h,int16_t* begx,int16_t* movey,uchar* flmovey,int skew)
{
	int k=-1;
	int i=0;
	int j=0;
   if(skew<0)
   {
	while(movey[i])
		i++;
	i--;
	while(i>=0)
	{
	 for(j=movey[i+1];j<movey[i];j++)
	 {
		 k++;
		 if(k==y)
			 return j;
	 }
     if(flmovey[i+1])
		 k++;
	 if(k==y)
		 return j-1;
	 i--;
	}
	for(j=movey[0];j<h;j++)
	{
		k++;
		if(y==k)
			return j;
	}
    return j;
   }
   else
   {
	while(j<h-1)
	{
     while(begx[j]==begx[j+1])
	 {
		 k++;
		 if(y==k)
			 return j;
		 j++;
	 }
	  if(flmovey[begx[j]])
		  k++;
	  if(k==y)
		  return j-1;
	  k++;
	  if(k==y)
		  return j;
	  j++;
	}
		j++;
		return j;
   }
}

RSELSTR_FUNC(void) RSELSTR_CleanStr(Rect16* pN,CCOM_comp** pC,int& nN,int top,int left,int h,int w,int skew,Bool32 vertical)
{
 if(skew>0)
 {
  int x;
  int y;
  Bool cont;
  for(int i=nN-1;i>=0;i--)
  {
   cont=FALSE;
   if(vertical)
   {
	   x=top+w-1-pN[i].top;
	   y=(pN[i].left+pN[i].right)/2-left;
   }
   else
   {
	   x=pN[i].right-left;
	   y=(pN[i].top+pN[i].bottom)/2-top;
   }
   if(y<okrug((double)(x*skew)/2048.))
   {
	   StrMoveMas(pN,nN,i);
	   if(pC)
	   {
		   nN++;
		   StrMoveMas(pC,nN,i);
	   }
	   cont=TRUE;
   }
   if(cont)
	   continue;
   if(vertical)
   {
      x=top+w-1-pN[i].bottom;
      y=(pN[i].right+pN[i].left)/2-left-h+1;
   }
   else
   {
	  x=pN[i].left-left;
      y=(pN[i].bottom+pN[i].top)/2-top-h+1;
   }
   if(y>okrug((double)(x*skew)/2048.))
   {
	   StrMoveMas(pN,nN,i);
        if(pC)
	   {
		   nN++;
		   StrMoveMas(pC,nN,i);
	   }
   }
  }
 }
 else
 {
  int x;
  int y;
  skew=-skew;
  Bool cont;
  for(int i=nN-1;i>=0;i--)
  {
   cont=FALSE;
   if(vertical)
   {
	   x=pN[i].bottom-top;
	   y=(pN[i].left+pN[i].right)/2-left;
   }
   else
   {
	   x=left+w-pN[i].left-1;
	   y=(pN[i].top+pN[i].bottom)/2-top;
   }
   if(y<okrug((double)(x*skew)/2048.))
   {
	   StrMoveMas(pN,nN,i);
	   if(pC)
	   {
		   nN++;
		   StrMoveMas(pC,nN,i);
	   }
	   cont=TRUE;
   }
   if(cont)
	   continue;
   if(vertical)
   {
      x=top-pN[i].top;
      y=(pN[i].right+pN[i].left)/2-left-h+1;
   }
   else
   {
	  x=left+w-1-pN[i].right;
      y=(pN[i].bottom+pN[i].top)/2-top-h+1;
   }
   if(y>okrug((double)(x*skew)/2048.))
   {
	   StrMoveMas(pN,nN,i);
	   if(pC)
	   {
		   nN++;
		   StrMoveMas(pC,nN,i);
	   }
   }
  }
 }
}

