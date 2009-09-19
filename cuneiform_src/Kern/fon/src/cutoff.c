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

// - if use special or 15% level of threshold ( was 10)
//   in p2_clu - too !
#define _NEW_POROG_

 // use weighted distance
#define _WEIDIST_
	  //  calculate distances. Now - when CutOffClusters!
#define _L1DIST_

#include "fon.h"
#include "sfont.h"
#include <memory.h>
#include <string.h>

int16_t NewFx=0,NewFy=0;
////////////////
/////////////////////
// make ideal picture from weighted
int16_t MakeIdeal(welet *wel,int16_t porog)
{
 int16_t i,j;
 char *rr,*rr1;
 int16_t sx=wel->w;
 int16_t sy=wel->h;
 int16_t fx=sx;  // first x
 int16_t lx=0;   // last  x
 int16_t fy=sy;  // first y
 int16_t ly=0;   // last  y
 int16_t summa;  // how many weighted points removed?
 int16_t startx=(WR_MAX_WIDTH-wel->w)/2;
 int16_t starty=(WR_MAX_HEIGHT-wel->h)/2;
 int16_t newstartx;
 int16_t newstarty;
 int16_t newsx,newsy;
 int16_t sdvigx;
 int16_t sdvigy;


  rr=wel->raster+starty*WR_MAX_WIDTH+startx;

  for(i=0,summa=0;i<sy;i++,rr+=WR_MAX_WIDTH)
	 {
	  for(j=0;j<sx;j++)
		{ if(rr[j]<=0) continue; // no point
		  if(rr[j] <= porog)   // remove spot
				 {
				  summa+=rr[j];
				  rr[j]=0; continue;
				 }
			 // check new bounds
		  if(j<fx) fx=j;
		  if(j>lx) lx=j;
		  if(i<fy) fy=i;
		  if(i>ly) ly=i;
	  }
	}

 NewFx=fx;  // first not-empty - used in AddClu
 NewFy=fy;

 if(summa==0) return 0; // nothing removed

 wel->summa-=summa;   // new summa

 lx++;
 ly++;
 newsx=lx-fx;
 newsy=ly-fy;
 newstartx=(WR_MAX_WIDTH -newsx)/2;
 newstarty=(WR_MAX_HEIGHT-newsy)/2;

 wel->w=newsx;
 wel->h=newsy;

 sdvigx = startx+fx-newstartx;
 sdvigy = starty+fy-newstarty;

	// need move ?
 if(sdvigx==0 && sdvigy==0) return summa;

	 // mov up / left
 if(sdvigy > 0 || sdvigy==0 &&sdvigx > 0)
  {
	rr=wel->raster+newstarty*WR_MAX_WIDTH+newstartx;
	rr1=rr+sdvigy*WR_MAX_WIDTH+sdvigx;
	for(i=0;i<newsy;i++,rr+=WR_MAX_WIDTH,rr1+=WR_MAX_WIDTH)
	 memcpy(rr,rr1,newsx);
  }
 else if(sdvigy==0 &&sdvigx < 0)   // move right
  {
	rr=wel->raster+newstarty*WR_MAX_WIDTH+newstartx;
	for(i=0;i<newsy;i++,rr+=WR_MAX_WIDTH)
	 memmove(rr,rr+sdvigx,newsx);
  }
 else   // move down - sdvigy < 0
  {
	rr=wel->raster+(newstarty+newsy-1)*WR_MAX_WIDTH+newstartx;
	rr1=rr+sdvigy*WR_MAX_WIDTH+sdvigx;
	for(i=0;i<newsy;i++,rr-=WR_MAX_WIDTH,rr1-=WR_MAX_WIDTH)
	 memcpy(rr,rr1,newsx);
  }

   // if set distance - not need set 0-s !
#ifndef _L1DIST_
  // set 0 - on moved space
  // upper lines
 rr=wel->raster+(starty+fy)*WR_MAX_WIDTH+startx+fx;
 for(i=starty+fy;i<newstarty;i++,rr+=WR_MAX_WIDTH)
	  memset(rr,0,newsx);
  // down lines
 rr=wel->raster+(newstarty+newsy)*WR_MAX_WIDTH+startx+fx;
 for(i=newstarty+newsy;i<starty+ly;i++,rr+=WR_MAX_WIDTH)
	  memset(rr,0,newsx);

 // left columns
 if( (j=newstartx-startx-fx) > 0)
  {
	rr=wel->raster+newstarty*WR_MAX_WIDTH+startx+fx;
	for(i=0;i<newsy;i++,rr+=WR_MAX_WIDTH) memset(rr,0,j);
  }

 // right columns
 if( (j=startx+lx-newstartx-newsx) > 0)
  {
	rr=wel->raster+newstarty*WR_MAX_WIDTH+newstartx+newsx;
	for(i=0;i<newsy;i++,rr+=WR_MAX_WIDTH) memset(rr,0,j);
  }
#endif
 return summa;
}
////////////////////////////////
////////////////
// start find distance L1 from all points to raster
int16_t AnalisFirstRow(uchar *row,int16_t fir,int16_t las,uchar *out,
						 int16_t sizex,int16_t sizey,int16_t fullx)
{
 register int16_t i,j;
 int16_t PrevDist;
 uchar *nextRow;

 if(fir >= las) return 0;

 if( fir<=0 && las >= sizex) return -1;

 if( fir > 0)
  {
	out[fir]=1;
	 // find distance from left & down
	for(i=fir+1;i<las;i++)
	 {
	  out[i]=out[i-1]+1;
		// if last point exist - use it too !
	  if(las < sizex && out[i] > las-i) out[i]=las-i;
	  PrevDist=MIN(out[i],sizey);
	  for(j=1,nextRow=row+fullx+i;j< PrevDist;j++,nextRow+=fullx)
			if( *nextRow) {out[i]=(uchar)j;break;}
	 }

	// check distance from right
	for(i=las-2;i>fir;i--)    out[i]=MIN(out[i],out[i+1]+1);
  }

 else
  {
	out[las-1]=1;
	 // find distance from right & down
	for(i=las-2;i>=fir;i--)
	 {
	  out[i]=out[i+1]+1;
	  PrevDist=MIN(out[i],sizey);
	  for(j=1,nextRow=row+fullx+i;j< PrevDist;j++,nextRow+=fullx)
			if( *nextRow) {out[i]=(uchar)j;break;}
	 }

	// check distance from left
	for(i=fir;i<las-1;i++)   out[i]=MIN(out[i],out[i+1]+1);
  }

 return 0;
}
/////////////////////////
int16_t AnalisNextRow (uchar *row,int16_t fir,int16_t las, uchar *out,
						 int16_t sizex,int16_t sizey,int16_t fullx)
{
 register int16_t i,j;
 int16_t PrevDist;
 uchar *nextRow;

  if(fir > 0)  out[fir]=1;
  else
	 {
		 // know distance for upper point !
	  out[fir]=out[fir-fullx]+1;
	  if(las < sizex && out[fir] > las-fir) out[fir]=las-fir;
	  if(out[fir] > 1)
		{
		 PrevDist=MIN(out[fir],sizey);
		 for(j=(out[fir]==2?1:out[fir]-2),nextRow=row+j*fullx+fir;j< PrevDist;
					  j++,nextRow+=fullx)
			if( *nextRow) {out[fir]=(uchar)j;break;}
		}
	 }

 for(i=fir+1;i<las;i++)
	 {
		 // know distance for upper point !
	  PrevDist=out[i-fullx];
		 // check left point
	  out[i]=MIN(out[i-1],PrevDist)+1;
		 // distance for points - not more 1
	  if(out[i] < PrevDist) continue;

		// know distance from right ?
	  if(las < sizex && out[i] > las-i) out[i]=las-i;
	  if(out[i]==1) continue;

	  if(PrevDist >1 ) j=PrevDist-1;
	  else j=PrevDist;

	  PrevDist=MIN(out[i],sizey);
	  if(j>=PrevDist) continue;

	  for(nextRow=row+j*fullx+i;j< PrevDist;j++,nextRow+=fullx)
			if( *nextRow) {out[i]=(uchar)j;break;}
	 }

	// check distance from right
 for(i=las-2;i>=fir;i--)    out[i]=MIN(out[i],out[i+1]+1);

 return 0;
}
///////////////////////
static int16_t FindDist(uchar *ras,int16_t sizex,int16_t sizey,int16_t fullx,uchar *out)
{
 int16_t i;
 int16_t fir,k;
 int16_t tsizey;   // stay rows
 int16_t fill;

 // at first - find distances in first row
 for(i=0,fir=-1,fill=0;i<sizex;i++)
  {
	if(ras[i] )  // find point from picture
	 {
	  out[i]=0;
	  fill++;
	  if(fir >= 0 ) AnalisFirstRow(ras,fir,i,out,sizex,sizey,fullx);
	  fir=-1;
	  continue;
	 }
	if(fir==-1) fir=i;   // first unknown empty point
  }

  if(fir >=0) i=AnalisFirstRow(ras,fir,sizex,out,sizex,sizey,fullx);

  // now - study all distances in other rows

  ras+=fullx;
  out+=fullx;
  for(k=1,tsizey=sizey-1;k<sizey;k++,ras+=fullx,out+=fullx,tsizey--)
  {

  for(i=0,fir=-1;i<sizex;i++)
	{
	if(ras[i])  // find point from picture
	  {
		out[i]=0;
		fill++;
		if(fir >= 0 ) AnalisNextRow(ras,fir,i,out,sizex,tsizey,fullx);
		fir=-1;
		continue;
	  }
	 if(fir==-1) fir=i;   // first unknown empty point
	}

  if(fir >=0) i=AnalisNextRow(ras,fir,sizex,out,sizex,tsizey,fullx);
 }  // end k

 return fill;
}
////////////////
// find distances in wel - results put to outwel
int16_t FindDistanceWr(welet *wel,welet *outwel)
{
 uchar *cur;
 int16_t startx,starty;
 uchar *out;
 register int16_t i,j;
 int16_t bound;
 int16_t sizex,sizey;
 int16_t fill;
 uchar *cout;
#ifdef _WEIDIST_
 uint16_t koef;
 uchar ckoef,maxvei;
 char *ras,*cras;
#else
 int16_t b2;
 uchar *fullout;
#endif

 startx=(WR_MAX_WIDTH-wel->w)/2;
 starty=(WR_MAX_HEIGHT-wel->h)/2;
 cur=(uchar *)wel->raster+starty*WR_MAX_WIDTH+startx;
 out=(uchar *)outwel->raster+starty*WR_MAX_WIDTH+startx;

 fill=FindDist(cur,wel->w,wel->h,(int16_t)WR_MAX_WIDTH,out);

 // set distances for all around
 sizex=wel->w;
 sizey=wel->h;

#ifdef _WEIDIST_

 if(fill <= 0) fill=1;
 koef=(uint16_t) (wel->summa/fill);

 if(koef < 1 ) koef=1;

 ckoef=MIN(128,koef);
 maxvei=128/koef;

  // set new weighted distance inside
 ras=wel->raster+starty*WR_MAX_WIDTH+startx;
 if(ckoef > 1)
 {
  for(i=0,cout=out;i<sizey;i++,cout+=WR_MAX_WIDTH,ras+=WR_MAX_WIDTH)
   {
   for(j=0;j<sizex;j++)
    if(cout[j] > 0)
     {
     if(cout[j] <= maxvei) ras[j]=-(cout[j]*ckoef);
     else ras[j]=-128;
     }
   }
 }

 else
  {
  for(i=0,cout=out;i<sizey;i++,cout+=WR_MAX_WIDTH,ras+=WR_MAX_WIDTH)
   {
   for(j=0;j<sizex;j++)    if(cout[j] > 0) ras[j]=-cout[j];
   }
 }

  // up - part
 ras=wel->raster+starty*WR_MAX_WIDTH+startx;
 for(i=0;i<sizex;i++)
 {
  bound=ras[i];
  if(bound > 0) bound=0;
  bound -=koef;
  cras=ras-WR_MAX_WIDTH+i;
  for(j=0;j<starty;j++,cras-=WR_MAX_WIDTH,bound-=koef)
   {if(bound < -128) break;
    *cras=(char)bound;
   }
  for(;j<starty;j++,cras-=WR_MAX_WIDTH)*cras=-128;
 }

  // down - part
 ras+=(sizey-1)*WR_MAX_WIDTH;
 starty+=sizey;
 for(i=0;i<sizex;i++)
 {
  bound=ras[i];
  if(bound > 0) bound=0;
  bound-=koef;
  cras=ras+WR_MAX_WIDTH+i;
  for(j=starty;j<WR_MAX_HEIGHT;j++,cras+=WR_MAX_WIDTH,bound-=koef)
   {if(bound < -128) break;
    *cras=(char)bound;
   }
  for(;j<WR_MAX_HEIGHT;j++,cras+=WR_MAX_WIDTH)*cras=-128;
 }

  // left part
 ras=wel->raster;
 for(i=0;i<WR_MAX_HEIGHT;i++,ras+=WR_MAX_WIDTH)
 {
  bound=ras[startx];
  if(bound>0) bound=0;
  bound-=koef;
  for(j=startx-1;j>=0;j--,bound-=koef)
    { if(bound<=-128) break;
      else ras[j]=(char)bound;
    }
  if(j>=0) memset(ras,-128,j+1);
 }

 // right part
 ras=wel->raster;
 startx+=sizex;
 for(i=0;i<WR_MAX_HEIGHT;i++,ras+=WR_MAX_WIDTH)
 {
  bound=ras[startx-1];
  if(bound>0) bound=0;
  bound-=koef;

  for(j=startx;j<WR_MAX_WIDTH;j++,bound-=koef)
   {if(bound<=-128) break;
    ras[j]=(char)bound;
   }
  if( j < WR_MAX_WIDTH ) memset(ras+j,-128,WR_MAX_WIDTH-j);
 }

 #else
  // left/right - parts
 fullout=(uchar *)outwel->raster+starty*WR_MAX_WIDTH;
 for(i=0,cout=out;i<sizey;i++,fullout+=WR_MAX_WIDTH,cout+=WR_MAX_WIDTH)
 {
   // left
  bound=*cout+startx;
  for(j=0;j<startx;j++,bound--) fullout[j]=bound;
   // right
  bound=*(cout+sizex-1)+1;
  for(j=startx+sizex;j<WR_MAX_WIDTH;j++,bound++) fullout[j]=bound;
 }

 // up part
 fullout=(uchar *)outwel->raster+(starty-1)*WR_MAX_WIDTH;
 cout=fullout+WR_MAX_WIDTH;
 /*
 fullout+=WR_MAX_WIDTH-1;
 cout+=WR_MAX_WIDTH-1;
 j=starty*WR_MAX_WIDTH;
 for(i=0;i<j;i++,fullout--,cout--) *fullout=*cout+1;
 */

 for(i=0;i<starty;i++,fullout-=WR_MAX_WIDTH,cout-=WR_MAX_WIDTH)
 {
  //memcpy(fullout,cout,WR_MAX_WIDTH);
  for(j=0;j<WR_MAX_WIDTH;j++) fullout[j]=cout[j]+1;
 }

 // down part
 fullout=(uchar *)outwel->raster+(starty+sizey)*WR_MAX_WIDTH;
 cout=fullout-WR_MAX_WIDTH;
 /*
 j=(WR_MAX_HEIGHT-starty-sizey)*WR_MAX_WIDTH;
 for(i=0;i<j;i++,fullout++,cout++) *fullout=*cout+1;
 */

 for(i=starty+sizey;i<WR_MAX_HEIGHT;i++,fullout+=WR_MAX_WIDTH,cout+=WR_MAX_WIDTH)
 {
  //memcpy(fullout,cout,WR_MAX_WIDTH);
  for(j=0;j<WR_MAX_WIDTH;j++) fullout[j]=cout[j]+1;
 }

 j=WR_MAX_WIDTH*WR_MAX_HEIGHT;
 ras=wel->raster;
 out=(uchar *)outwel->raster;
 for(i=0;i<j;i++) if(out[i] > 0) ras[i]=-out[i];
#endif
 return fill;
}
/////////////////
#define POROG_IDEAL 10
/**************/
FON_FUNC(int32_t) FONCutOffClusters(welet *ww,int16_t num)
{
int16_t TestFat(welet *wel,int16_t porog,int16_t porog_fat);
char porog;
welet tmp_wel;

 if(!ww) return 0;
 NewFx=NewFy=0;
 for(;num>0;num--,ww++)
 {
  if( ww->attr & FON_CLU_FIXED ) porog=ww->porog;
  else
#ifdef _NEW_POROG_
  {
	if ( ww->weight < 5 ) porog=0;
	else if(ww->weight <= 10 ) porog = 1;
	else porog=((int)ww->weight*(int)3 + 10)/20;  // 15%
  }
#else
	  porog= ww->weight /POROG_IDEAL;
#endif


#ifdef _TEST_FAT_
  if( ww->weight > 1 && ww->fixed==0 )
  {
 //   if( ((ww->summa*100L)/((int32_t)ww->mw*ww->mh*ww->weight)) < 75 )
	   porog=TestFat(ww,porog,_POROG_FAT_);
  }
#endif

  ww->porog=porog;
  if(porog > 0 ) MakeIdeal(ww,porog);

  ww->fill=FindDistanceWr(ww,&tmp_wel);
 }

return 1;
}
/**************/
