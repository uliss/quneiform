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

#define _USE_CSTR_

#define MAX_ALT_BOU   4

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

#include "ligas.h"
#include "fon.h"
#include "sfont.h"
#include "fonrec.h"
#include "minmax.h"

uint16_t cmp(puchar r,uint16_t fullwb,uint16_t w,uint16_t h,welet * wl);
int16_t RazmazHalf(uchar *bSource,uchar *bDest,int16_t xbit,int16_t yrow);

static int LookLeft(int *startRow,int yrow,int bits,int height,
					int *outHeight);
static void FillLeft(uchar *pic,int xbyte,int yrow,int bits,
					 int *outStart);
typedef struct tagRECBOU
	{
	  uchar name;
	  uchar prob;
	  int  num;
	  int  dist;
	  int  movx;
	  int  movy;
	  int  yFirst;    // actual start
	  int  yHeight;   // actual size
	  int  bound[WR_MAX_HEIGHT];
	}
  RECBOU;

typedef struct tagREC2
	{
	  RecVersions vers;
	  RecRaster   rast;
	  int  firY;    // actual start
	  int  firX;
	  int  nClust;
	}
  RECS2;
/*
typedef struct tagMYSTACK {
	int name;
	int num;
	int dist;
	int firY;
	int height;
	int bounds[WR_MAX_HEIGHT];
} MYSTACK;
*/

extern uchar language;

static int *AllBounds=NULL;
static RECBOU AllRecBou[MAX_ALT_BOU];

// 0xba - обозначение для i с приклееной точкой
// 0xbc - обозначение для ! с приклееной точкой (без точки)
static const char Palki[]="![]()1Iil"; //\xba\xbc";

static const unsigned char mas00[8] = {255,127,63,31,15,7,3,1};
static const unsigned char mas11[8] = {0,128,192,224,240,248,252,254};
static const unsigned char mas1[8]  = {128,64,32,16,8,4,2,1};
static const unsigned char mas10[8]  ={255,128,192,224,240,248,252,254};
 ///////////////////////
   // meet first 0 in byte
static const int tabl0[256]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                             1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                             1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                             1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                             2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
                             2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
                             3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
                             4,4,4,4,4,4,4,4,5,5,5,5,6,6,7,8 };
    // meet first 1 in byte
static const int tabl1[256]={8,7,6,6,5,5,5,5,4,4,4,4,4,4,4,4,
                             3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
                             2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
                             2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
                             1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                             1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                             1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                             1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
////////////////
static const int firRight1[256]={8,
0,
1,0,
2,0,1,0,
3,0,1,0,2,0,1,0,
4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,

5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,

6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,

7,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0} ;

//////////////////////
void   EndBounds(void)
{
	if(AllBounds) free(AllBounds);
	AllBounds = NULL;
}
//////////////
static void SetBound(welet *wel,int *bou)
{
  int i,j;
  char *rastr=wel->raster;

  for(i=0;i<WR_MAX_HEIGHT;i++,rastr+=WR_MAX_WIDTH)
  {
     for(j=WR_MAX_WIDTH-1;j>=0;j--)
		 if(rastr[j] > 0) break;
     bou[i] = j;
  }
}
/////////////////
static Bool GetBounds(welet *wel,int num)
{
int *bou;

	if(AllBounds) return TRUE;
	AllBounds=malloc(WR_MAX_HEIGHT*num*sizeof(int));
	if(!AllBounds)
		return FALSE;

	// now set bounds itself
    for(bou=AllBounds;num;num--,wel++,bou+=WR_MAX_HEIGHT)
		SetBound(wel,bou);

	return TRUE;
}

///////////////////////
//
// штрафуем дальние точки
// return - сколько точек растра вылазит за серую область
//

static int DistToWeletBound(puchar r,int fullByte,int w,int h,welet * wl,
					   int xo,int yo,int porog,int *bou )
{
pchar curr;
int   i,j,jj;
uchar  cbyte,cc;
int rbyte;
int dist1;
int dist;
int firColumn,firRow;
int lastColumn;

 if( h+2 >= WR_MAX_HEIGHT )  return (porog+1);

 dist1 = wl->summa/wl->fill;
 dist1=-dist1;

 // куда положим растр - чтобы левая часть легла
 // на центр кластера

 firColumn  = (WR_MAX_WIDTH-wl->w)/2;
 firColumn +=(wl->w-wl->mw)>>1;
 firColumn += xo;             // variation

 firRow=(WR_MAX_HEIGHT-h)/2;
 firRow+=yo;                   // variation

 curr = wl->raster + WR_MAX_WIDTH * firRow;
 bou+=firRow;

 rbyte=(w+7)>>3;

 // какие точки вылазят за серую картинку ?
 for(i=0,dist=0; i < h;i++,curr += WR_MAX_WIDTH,r+=fullByte)
 {
  jj=firColumn;
  lastColumn = (bou[i] < 0 ? firColumn+wl->mw : bou[i]);

  for(j=0; jj <= lastColumn && j<rbyte ;j++)
  {
   cbyte=r[j];
   if(cbyte == 0) {jj+=8;continue;}


   for(cc=128;jj <= lastColumn && cc!=0 ;cc>>=1,jj++)
   {
    if( (cbyte & cc) && curr[jj] < dist1 )
	  dist++;
   } // end cc
   if(dist > porog) return dist;

  }  // end j
 }   // end i

  return (dist);
}
/////////////////
//
// штрафуем точки cluster's
// return - сколько точек черной области вылезло за окрестность растра
//
static int DistWeletRazmazBound(puchar r,int fullByte,int  w,int  h,welet * wl,
					 int  xo,int  yo, int porog,int *bou)
{
int16_t ww=wl->w, hh=wl->h;
pchar curr;
int16_t i,j;
uchar  cbyte,cc;
int16_t rbyte;
int dist;
int startx; // =(WR_MAX_WIDTH-w)/2;
int starty=(WR_MAX_HEIGHT-h)/2;
int stx=(WR_MAX_WIDTH-ww)/2;   // start cluster
int sty=(WR_MAX_HEIGHT-hh)/2;
int wei=wl->weight>>1;
int lasty,lastx;
uchar initCC;
uchar *rr;

  // берем только левую часть размазанного растра !
 if( w > wl->mw +2  ) startx = wl-> mw+2 ;
 else startx = w;
 startx = (WR_MAX_WIDTH-startx)/2;

 startx += xo;
 starty += yo; // variation
 rbyte=(w+7)>>3;

 curr = wl->raster+sty*WR_MAX_WIDTH;
 dist=0;

    // add upper cluster lines
 for(i=sty;i<starty;i++,curr+=WR_MAX_WIDTH)
 {
//	 if(bou[i] < 0) continue;
     for(j=stx;j<=bou[i];j++)
		 if(curr[j]> wei) dist++;
 }
 if(dist > porog) return dist;

 lasty=MIN(starty+h,sty+hh);
 lastx=MIN(startx+w,stx+ww);

 if( starty < sty) r+=(sty-starty)*fullByte;

 if(startx < stx)
 {initCC=128>>((stx-startx)&7);
  r+=(stx-startx)>>3;
 }
 else initCC=128;   // from first bit

   // now - start inside frame
 for(; i < lasty;i++,curr +=WR_MAX_WIDTH,r+=fullByte)
 {
  if( bou[i] < 0 ) continue;

  for(j=stx;j<startx;j++) if(curr[j] > wei ) dist++;
  if(dist > porog ) return dist;

  cc=initCC;
  rr=r;
  cbyte=*rr;

  lastx=MIN(bou[i]+1,startx+w);
  for(;j<lastx;j++,cc>>=1)
  {
   if(cc==0) {cc=128;cbyte=*(++rr);}
   if( ((cbyte & cc)==0) && curr[j]>wei  ) dist++;
  }  // end j
  if(dist > porog) return dist;

   // last points in row
  for(;j<= bou[i];j++) if(curr[j] > wei ) dist++;
  if(dist > porog) return dist;
 }   // end i

// add down lines
  for(;i<sty+hh;i++,curr+=WR_MAX_WIDTH)
  {
     for(j=stx;j<=bou[i];j++)   if(curr[j]> wei) dist++;
     if(dist > porog) return dist;
  }

  return (dist);
}
////////////////////
///////////
static int distOneBound(uchar *buf,uchar *bufrazmaz,int w,int h,
						int bestdist,welet *wel,int x,int y,
				        int *bou, int countRazmaz)
{
int dist,j;

  dist=DistToWeletBound(buf,(w+7)>>3,w,h,wel,x,y,bestdist-1,bou);
  if(dist >= bestdist) return dist;

  if( countRazmaz <= 0 )
	  countRazmaz = 1;

  j= DistWeletRazmazBound(bufrazmaz,(w+9)>>3,w+2,h+2,wel,x,y,(bestdist-dist-1)*countRazmaz, bou);


  return dist+((j+countRazmaz-1)/countRazmaz);
}
////////////
static int distWeletBound(uchar *buf,uchar *bufraz,int w,int h,welet * wl,int porog,
						  int *bou,RECBOU *recBou, int countRazmaz)
{
 uint16_t best,east,west,north,south,center;
 int   lbest; // local best
 int bound=2*MIN(50,w+h);
 int initPorog=porog;

 recBou->movx=0;
 recBou->movy=0;

 best=east=west=north=south=center=lbest=0;

// center - special threshold??? check
 center =best=distOneBound(buf,bufraz,w,h,bound,wl,0,0,bou,countRazmaz);
// center =best=distOne(buf,bufraz,w,h,porog,wl,0,0);
 if(best == 0 ) return best;
 if(best >= bound ) return porog+1;

 if(best < porog) porog=best;

 if((south = distOneBound(buf,bufraz,w,h,porog,wl, 0, 1,bou,countRazmaz)) < best )
 { best = south;
   recBou->movy=1;
 }
 if(best==0) return best;
 if(best < porog) porog=best;
 if((north = distOneBound(buf,bufraz,w,h,porog,wl, 0,-1,bou,countRazmaz)) < best )
 {
   best = north;
   recBou->movy=-1;
 }
 if(best==0) return best;
 if(best < porog) porog=best;

 if((east = distOneBound(buf,bufraz,w,h,porog,wl, 1, 0,bou,countRazmaz)) < best )
 {
    best = east;
	recBou->movx=1;
	recBou->movy=0;
 }
 if(best==0) return best;
 if(best < porog) porog=best;
 if((west = distOneBound(buf,bufraz,w,h,porog,wl,-1, 0,bou,countRazmaz)) < best )
 {
    best = west;
	recBou->movx=-1;
	recBou->movy=0;
 }

 if(best==0) return best;
 if(best < porog) porog=best;

 if(best < north && best < south && best < east && best < west)
	 return best;

 // nothing good ?
 if( best >= initPorog)
	 return best;

 center=best;  // to save old best for compare
 // where try move ? - now two positions may be
 if( north == center || east == center )
 {
  if((lbest = distOneBound(buf,bufraz,w,h,porog,wl,1,-1,bou,countRazmaz)) < best )
  {
    best = lbest;
	recBou->movx=1;
	recBou->movy=-1;
  }
 }

 if(east == center || south == center )
 {
  if((lbest = distOneBound(buf,bufraz,w,h,porog,wl,1, 1,bou,countRazmaz)) < best )
  {
    best = lbest;
	recBou->movx=1;
	recBou->movy=1;
  }
 }

 if(south == center || west == center)
 {
  if((lbest = distOneBound(buf,bufraz,w,h,porog,wl,-1, 1,bou,countRazmaz)) < best )
  {
    best = lbest;
	recBou->movx=-1;
	recBou->movy=1;
  }
 }

 if( west == center || north == center)
 {
  if((lbest = distOneBound(buf,bufraz,w,h,porog,wl,-1,-1,bou,countRazmaz)) < best )
  {
    best = lbest;
	recBou->movx=-1;
	recBou->movy=-1;
  }
 }

 return best;
}

///////////////////////////
///////
// ordSpisok - порядковый номер кластера -
//
int AddBoundVersion(RECBOU *curRec,
			        int num,int maxNames,
					RECBOU *allRec)
{
int j,k;

   for(j=0;j<num;j++ )
	 {
	   if( allRec[j].name == curRec->name )
	    {
         if(curRec->prob > allRec[j].prob)
		    allRec[j]=*curRec;
		 return num;
		} // was better the same

	   if( curRec->prob > allRec[j].prob ) break;
	 }

   if( j>=maxNames ) return num;

	  // was such name - to new position
   for(k=j+1;k<num;k++ )
   {
	 if( allRec[k].name == curRec->name )
	   {
		   memmove(allRec+j+1,allRec+j,(k-j)*sizeof(RECBOU));
		   allRec[j]=*curRec;
		   return num;
	   }
	}

   if(num<maxNames) num++;

   if( j < num-1)
    memmove(allRec+j+1,allRec+j,(num-j-1)*sizeof(RECBOU));

   allRec[j]=*curRec;

   return num;
}
//////////////

static int FirstRow[WR_MAX_HEIGHT];
static int NumInRow[WR_MAX_HEIGHT];

/////////
static Bool32 IsSinglePoint(int *bou)
{
 int i;
 int start[2];
 int size[2];
 int cur;

 for(i=cur=0, start[0]=start[1]=-1,size[0]=size[1]=0; i < WR_MAX_HEIGHT ; i++ )
 {
	 if( bou[i] >= 0 )
	 {
		 if( start[cur] < 0 )
			 start[cur] = i;

		 size[cur]++;
	 }
	 else if( cur==0)
		 cur++;
 }

 if( start[1] < 0 )
	 return FALSE;

 if( size[1] < size[0]*2 )
	 return FALSE;

 if( (start[1] - start[0] -size[0])*2 < size[0] )
	 return FALSE;

 return TRUE;
}
////////////
static int FindBestClustersBound(int w,int h,uchar *buf,uchar *bufrazmaz,
		      int NumClus, int porog,welet *wel,
			  RECBOU *recBounds,  int maxNames,
			  int *leftStart,
			  int col, int row, int nInCTB,int countRazmaz)
{
 int16_t i;
 int16_t dist;
 int num=0;
 int xbyte=(w+7)>>3;
 int xbyteRazmaz=(w+9)>>3;
 int *bou=AllBounds;
 int yStart,yHeight;
 RECBOU recBou;


 for(i=num=0 ;i<NumClus;i++,wel++,bou+=WR_MAX_HEIGHT)
 {
  if( wel->invalid ) continue;
  if( ! (wel->attr & FON_CLU_SOLID)  ) continue;

  if( wel->num < 0 )  // не из нашего алфавита
	   continue;

  if( strchr(Palki, wel->let) ||
	  wel->let==liga_i ||
	  (language == LANG_TURKISH &&  // 30.05.2002 E.P.
		(wel->let==i_sans_accent||wel->let==II_dot_accent)
	  ) ||
	  wel->let==liga_exm)
  {
	  if( wel->let != 'i' || !IsSinglePoint(bou) )
	   continue;
  }

    // не распознавать себя ?
  if( wel->weight == 1 &&
	  (wel->nInCTB ==nInCTB ||
	   abs(col-wel->sr_col)<=1 && abs(row-wel->sr_row)<=1
	  )
	)
	  continue;


  // как лучше накладывать ?
  yStart=LookLeft( leftStart,h,wel->mw,wel->mh,
					&yHeight);
  if(yStart < 0) continue;

  if( abs(wel->mh- yHeight)*4 > wel->mh ) continue; // check for height likeness
//  if( abs(wel->mw-w)*3 >= wel->mw ) continue;

  dist=distWeletBound(buf+yStart*xbyte,
	   bufrazmaz+yStart*xbyteRazmaz,
	   w,yHeight,
	   wel,porog+1,bou,&recBou, countRazmaz);

  if(dist <= porog)
  {
	  // fill right bound
     int firColumn,firRow;
	 int k;

	 firColumn  = (WR_MAX_WIDTH-wel->w)/2;
     firColumn +=(wel->w-wel->mw)>>1;
     firColumn += recBou.movx;             // variation

     firRow=(WR_MAX_HEIGHT-yHeight)/2;
     firRow+=recBou.movy;                   // var

	 recBou.yFirst   = yStart;
	 recBou.yHeight  = yHeight;

       // if good recog - fill output bounds

	 for(k=0;k<yStart;k++)
         recBou.bound[k] = 2;

	 for(k=0;k<yHeight;k++)
	 {
	  if( bou[k+firRow] < 0 )
		  recBou.bound[k+yStart]=2;
	  else
		  recBou.bound[k+yStart]=bou[k+firRow]-firColumn+2;
	 }

	 for(k=yStart+yHeight;k<h;k++)
		 recBou.bound[k]=2;

	 recBou.name =(uchar)wel->let;
     recBou.prob =(uchar)(MAX(0,255-STRAFPOINT*dist));
	 recBou.dist = dist;
     recBou.num  =i;

	 num=AddBoundVersion(&recBou,num,maxNames,recBounds);
     if(num>=maxNames)
	 {
	  porog = recBounds[maxNames-1].dist-1;
	  if(porog < 0) break;
	 }
//	if(dist==0) break;
  }

 }

 return num;
}

///////////////////////
static uchar buf[REC_MAX_RASTER_SIZE];
static uchar bufrazmaz[2*REC_MAX_RASTER_SIZE];

static int StartInRow[WR_MAX_HEIGHT];

// bounds - massiv yyrow*maxNames
int16_t RecogCluBound(uchar *rast,int16_t xbyte,int16_t xbit,int16_t yyrow,uchar *names,
			 uchar *probs,int16_t maxNames,welet *wl,int numWel,
			 int *bounds, int countRazmaz)
{
 int i,numAlt;
 int rbyte=(xbit+7)>>3;
 uchar *b1;
 int porog;
 int yrow=yyrow;

 if(wl==NULL || numWel <= 0) return 0;

 // установить правые границы кластеров - если еще не были
 if( !GetBounds(wl,numWel))
	 return 0;

 if(xbyte*yrow > REC_MAX_RASTER_SIZE) yrow=REC_MAX_RASTER_SIZE/xbyte;
 if(yrow > WR_MAX_HEIGHT-2) yrow = WR_MAX_HEIGHT-2;

 for(i=0,b1=buf;i<yrow;i++,b1+=rbyte,rast+=xbyte)
	 memcpy(b1,rast,rbyte);

// if( xbit < POROG_HALF_WIDTH && yrow < POROG_HALF_HEIGHT )
//   RazmazHalf(buf,bufrazmaz,xbit,yrow);

  Razmaz2(buf,bufrazmaz,xbit,(int16_t)yrow,0,
          (int16_t)POROG_ANGLES);
   //  - threshold add angles

  // left bound
  FillLeft(buf,rbyte,yrow,xbit,StartInRow);

  //porog=MAX(xbit,yrow);
  porog=MIN(50,xbit+yrow);

  if(maxNames > MAX_ALT_BOU ) maxNames=MAX_ALT_BOU;
  numAlt=FindBestClustersBound(xbit,yrow,buf,bufrazmaz,
		      numWel,porog,wl,
			  AllRecBou,maxNames,
			  StartInRow,-1024,-1024,0,countRazmaz);

// fill bounds & names
//  ......
//  memset(bounds,0,numAlt*yyrow*sizeof(int));
  for(i=0;i<numAlt;i++,bounds+=yyrow)
  {
    names[i]=AllRecBou[i].name;
	probs[i]=AllRecBou[i].prob;

	memcpy(bounds,&AllRecBou[i].bound[0],
		yyrow*sizeof(int));
  }


  return numAlt;
}
///////////////////////////
/***************************/
//
// найти положение левой границы символа шириной bits
//
static void FillLeft(uchar *pic,int xbyte,int yrow,int bits,
					 int *outStart)
{
 int i,j,k;

 if( yrow > WR_MAX_HEIGHT ) yrow=WR_MAX_HEIGHT;  // return -1; ???

// memset(FirstRow,0,WR_MAX_HEIGHT*sizeof(int));

 for(i=0;i<yrow;i++,pic+=xbyte)
  {
	 // study current row
	for(j=0;j<xbyte;j++)
		if( pic[j])  break;

	if( j>= xbyte )  {outStart[i]=-1;continue;}

	k=tabl1[pic[j]]; // first 1
	if( (j<<3)+k >= bits) {outStart[i]=-1;continue;}

    outStart[i]=(j<<3)+k;
 }

}
/////////////////
//
// найти положение символа высотой height  шириной bits высоком bitmap
static int LookLeft(int *startRow,int yrow,int bits,int height,
					int *outHeight)
{
 int  i,j;
 char IsBlack;
 int  NumStart;    // number of start points
 int  LRow,FRow;
 int  sizeup,sizedown;

 *outHeight=yrow;
 if(yrow <= height ) return 0; // start from 0

// memset(FirstRow,0,yrow*sizeof(int));
// memset(NumInRow,0,yrow*sizeof(int));

 for(i=0,NumStart=0,IsBlack=0;i<yrow;i++)
  {
	 // study current row
    if(startRow[i] < 0 || startRow[i] >= bits )
	{
		 if(IsBlack) NumStart++; // new rows
		 IsBlack=0;
	     continue;
	}

	if(IsBlack) NumInRow[NumStart]++;
	else
	{
	 FirstRow[NumStart]=i;
	 NumInRow[NumStart]=1;
	 IsBlack=1;
	}
  }

 if(IsBlack) NumStart++;

 if(NumStart <=0) return -1;  // empty

 // ищем самую хорошую полоску
 for(i=1,j=0;i<NumStart;i++)
	 if(NumInRow[i]>NumInRow[j]) j=i;

 if(NumInRow[j] >= height)
 {
	 int minBit=bits-3;
	 int fir=0,las=NumInRow[j];

	 if( NumInRow[j] > height + 3 )
     {
        for( i = 0; i < NumInRow[j]; i++)
        {
           if( startRow[i+FirstRow[j]] < minBit )
		   {
			   fir=i;
			   break;
           }
		}

		for( i = NumInRow[j]-1; i>0; i--)
        {
           if( startRow[i+FirstRow[j]] < minBit )
		   {
			   las=i+1;
			   break;
           }
		}

		if( las-fir >= height -2 )
		{
			*outHeight=las-fir;
			return FirstRow[j]+fir;
		}

	 }

	 *outHeight=NumInRow[j];
	 return FirstRow[j];
 }

 // где лучше добавить?
 FRow=FirstRow[j];
 LRow=FRow+NumInRow[j];

 for(i=j-1,j=j+1;LRow-FRow<height;)
 {
  if(i>=0 ) sizeup=NumInRow[i]-MAX(0,LRow-height-FirstRow[i]);
  else sizeup=-1;

  if(j<NumStart) sizedown =MIN(NumInRow[j],FRow+height-FirstRow[j]);
  else sizedown=-1;

  if(sizeup <=0 && sizedown <=0 ) break;

  if(sizeup>=sizedown)
	{
	 FRow=MAX(FirstRow[i],LRow-height);
	 i--;
	}
  else
	{
	 LRow=MIN(FirstRow[j]+NumInRow[j],FRow+height);
	 j++;
	}
 }

// FRow=MIN(FRow,yrow-height);
 *outHeight=LRow-FRow;
 return FRow;
}
/****************************************/
/////////////////////////
//
//   unkley functions
//
//////////////////
static int MinBoundLeft(uchar *pic,int xbyte,int yrow,int *bou )
{
 int i,j,k;
 uchar  cc;
 int minBou=xbyte<<3;
 int prevBou=minBou;
 int prevPoint=minBou;


 for(i=0;i<yrow;i++,pic+=xbyte)
  {
	 // study current row
	j= bou[i]>>3;
	if( j <xbyte )
    {
     cc=pic[j]&mas00[bou[i]&7];
	 while( cc==0 )
	 {
	    j++;
        if(j >= xbyte) break;
		cc=pic[j];
	 }
	}

	if( j>= xbyte )
	  continue;

    k=(j<<3)+tabl1[cc];

	// попробовать избежать отдельных точек - грязи
	if( k < minBou && i > 0 &&   k >= prevBou - 3  )
	{
     if( ( pic[(k+1)>>3]&mas1[(k+1)&7] ) &&
		 ( pic[(k+2)>>3]&mas1[(k+2)&7] ) )
	  minBou=k;
	}

	if( k >= prevPoint - 3)
		prevBou=k;

	prevPoint=k;
 }

 return minBou;
}
/////////////////

static void RemoveLeft(uchar *inBuf,
					 int xbyte,int yrow,
					 int startX,int *bou)
{
	int i,j;

	for(i=0;i<yrow;i++,inBuf+=xbyte)
	{
		if(bou[i] <= startX ) continue;
		j=bou[i]-startX;

		memset(inBuf,0,j>>3);
		inBuf[j>>3]&=mas00[j&7];
    }
}
////////////////
static void (*MoveWindowRow)(uchar *outrow,uchar *inrow,int SizeByte,int fbit);
///////////////////////////////////
void MoveWindowRow0(uchar *outrow,uchar *inrow,int SizeByte,int fbit)
{
	  memcpy(outrow,inrow,SizeByte);
}
//////////
void MoveWindowRow2(uchar *outrow,uchar *inrow,int SizeByte,int fbit)
{
 int i;
  switch(fbit)
  {
   case 1:
	for(i=0;i<SizeByte;i++)
	 outrow[i]=(inrow[i]<<1)|(inrow[i+1]>>7);
	break;
   case 2:
	for(i=0;i<SizeByte;i++)
	 outrow[i]=(inrow[i]<<2)|(inrow[i+1]>>6);
	break;
   case 3:
	for(i=0;i<SizeByte;i++)
	 outrow[i]=(inrow[i]<<3)|(inrow[i+1]>>5);
	break;
   case 4:
	for(i=0;i<SizeByte;i++)
	 outrow[i]=(inrow[i]<<4)|(inrow[i+1]>>4);
	break;
   case 5:
	for(i=0;i<SizeByte;i++)
	 outrow[i]=(inrow[i]<<5)|(inrow[i+1]>>3);
	break;
   case 6:
	for(i=0;i<SizeByte;i++)
	 outrow[i]=(inrow[i]<<6)|(inrow[i+1]>>2);
	break;
   case 7:
	for(i=0;i<SizeByte;i++)
	 outrow[i]=(inrow[i]<<7)|(inrow[i+1]>>1);
	break;
   default:return;
  }

}
///////////////
void MoveWindowRow1(uchar *outrow,uchar *inrow,int SizeByte,int fbit)
{
 int i;
  SizeByte--;
  switch(fbit)
  {
   case 1:
	for(i=0;i<SizeByte;i++)
	 outrow[i]=(inrow[i]<<1)|(inrow[i+1]>>7);
	outrow[SizeByte]=inrow[SizeByte]<<1;
	break;
   case 2:
	for(i=0;i<SizeByte;i++)
	 outrow[i]=(inrow[i]<<2)|(inrow[i+1]>>6);
	outrow[SizeByte]=inrow[SizeByte]<<2;
	break;
   case 3:
	for(i=0;i<SizeByte;i++)
	 outrow[i]=(inrow[i]<<3)|(inrow[i+1]>>5);
	outrow[SizeByte]=inrow[SizeByte]<<3;
	break;
   case 4:
	for(i=0;i<SizeByte;i++)
	 outrow[i]=(inrow[i]<<4)|(inrow[i+1]>>4);
	outrow[SizeByte]=inrow[SizeByte]<<4;
	break;
   case 5:
	for(i=0;i<SizeByte;i++)
	 outrow[i]=(inrow[i]<<5)|(inrow[i+1]>>3);
	outrow[SizeByte]=inrow[SizeByte]<<5;
	break;
   case 6:
	for(i=0;i<SizeByte;i++)
	 outrow[i]=(inrow[i]<<6)|(inrow[i+1]>>2);
	outrow[SizeByte]=inrow[SizeByte]<<6;
	break;
   case 7:
	for(i=0;i<SizeByte;i++)
	 outrow[i]=(inrow[i]<<7)|(inrow[i+1]>>1);
	outrow[SizeByte]=inrow[SizeByte]<<7;
	break;
   default:return;
  }

}
///////////////
// return number of bits
static int MoveAllLeft(uchar *inBuf,int xbyte,
					    int xbit, int yrow,
					    uchar *outBuf,int start)
{
 int i;
 int outSizeBit= xbit-start;
 int outSizeByte=(outSizeBit+7)>>3;
 int firBit=start&7;
 uchar hvost=mas10[outSizeBit&7];

 inBuf+=start>>3;

 if( firBit == 0 ) MoveWindowRow=MoveWindowRow0;
 else if( ( (xbit-1)&7 ) >= firBit )
      MoveWindowRow=MoveWindowRow1;
 else MoveWindowRow=MoveWindowRow2;

 for(i=0;i<yrow;i++,inBuf+=xbyte,outBuf+=outSizeByte)
 {
   MoveWindowRow(outBuf,inBuf,outSizeByte,firBit);
   outBuf[outSizeByte-1] &= hvost;
 }

 return outSizeBit; // bits in row
}
////////////
//
// найти первый кластер, подходящий по порогу
//
static int FindFirstClusterPorog(int w,int h,
			  uchar *buf,uchar *bufrazmaz,
		      int startClus,int NumClus, welet *wel,
			  int porog, int *outBou ,int *outDist,
			  int *yFir,int *yHei)
{
 int16_t i,j;
 int16_t dist;
 int xbyte=(w+7)>>3;
 int xbyteRazmaz=(w+9)>>3;
 int yStart,yHeight;
 RECBOU recBou;
 int *bou=AllBounds;

 wel+=startClus;
 bou+=WR_MAX_HEIGHT*startClus;

 for(i=startClus ;i<NumClus;i++,wel++,bou+=WR_MAX_HEIGHT)
 {
  if( wel->invalid ) continue;
  if( !(wel->attr & FON_CLU_SOLID)  ) continue;

  if( wel->num < 0 )
	  continue;

  if( strchr(Palki, wel->let) ||
		wel->let==liga_i ||
		  (language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(wel->let==i_sans_accent||wel->let==II_dot_accent)
		  ) ||
		wel->let==liga_exm
	)
	  continue;


  // как лучше накладывать ?
  yStart=LookLeft( StartInRow,h,wel->mw,wel->mh,
					&yHeight);
  if(yStart < 0) continue;

  if( abs(wel->mh- yHeight)*4 > wel->mh ) continue; // check for height likeness
//  if( abs(wel->mw-w)*3 >= wel->mw ) continue;

  dist=distWeletBound(buf+yStart*xbyte,
	   bufrazmaz+yStart*xbyteRazmaz,
	   w,yHeight,
	   wel,porog+1,bou,&recBou,1);

  if(dist <= porog)
  {
	  // fill right bound
     int firColumn,firRow;
	 int k,prevBou;

	 firColumn  = (WR_MAX_WIDTH-wel->w)/2;
     firColumn +=(wel->w-wel->mw)>>1;
     firColumn += recBou.movx;             // variation

     firRow=(WR_MAX_HEIGHT-yHeight)/2;
     firRow+=recBou.movy;                   // var

       // if good recog - fill output bounds

	 // what put on free space - find first not-empty?
	for(k=0,prevBou=0;k<yHeight;k++)
	 { if( bou[k+firRow] > 0 )
		{ prevBou=bou[k+firRow]-firColumn+1; break;}
	 }

	// now - fill output
	for(j=0;j< yStart;j++)
         outBou[j]=prevBou;
	for(k=0;k<yHeight;k++)
	 {
	  if( bou[k+firRow] < 0 )
		  outBou[yStart+k]=prevBou;
	  else
		  prevBou=outBou[yStart+k]=bou[k+firRow]-firColumn+1;
	 }
	for(j=yStart+yHeight; j < h;j++ )
		 outBou[j]=prevBou;

	*outDist=dist;
	*yFir   =yStart;
	*yHei   =yHeight;
//	recBou.name =(uchar)wel->let;
//  recBou.prob =(uchar)(255-dist);
//  recBou.order=i;

	return i;
  }

 }  // end i

 return -1;
}

///////////////////////

/////////////
#define    MAX_GLUE  16
#define    POROG_STOP 4

static RECBOU mystack[MAX_GLUE];
static RECBOU bestStack[MAX_GLUE];
static int  TekInStack=0;
static int  inBestStack=0;
//////////
static int AddToStack(int name,int num,int dist,int startX,int yrow,int *bou,int startY,
					  int height)
{
int i;

  if(TekInStack >= MAX_GLUE)
	  return -1;

  mystack[TekInStack].name =name;
  mystack[TekInStack].num =num;
  mystack[TekInStack].dist=dist;
  mystack[TekInStack].yFirst=startY;
  mystack[TekInStack].yHeight=height;
  // +1 - test !..
  for(i=0;i<yrow;i++)
    mystack[TekInStack].bound[i]=bou[i]+startX+1;
  TekInStack++;
  return TekInStack;
}
///////////////
static int UpdateSpisokAlter(int oldPorog,int *oldSumma)
{
  int i,maxDist;
  int summa;

  for(i=summa=0,maxDist=mystack[0].dist;i<TekInStack ;i++)
  {
      if(maxDist < mystack[i].dist )
		  maxDist=mystack[i].dist;
	  summa+=mystack[i].dist;
  }
  if(maxDist > oldPorog || maxDist==oldPorog && summa >= *oldSumma)
	  return oldPorog;

  memcpy(bestStack,mystack,TekInStack*sizeof(RECBOU));
  inBestStack=TekInStack;
  *oldSumma=summa;
  return maxDist;
}
//////////////
static int GetFromStack(uchar *inBuf,int xbyte,int xbit,int yrow,
						uchar *outBuf,int *oBit,
						int *bou)
{
//int *bou;
int startX;
int outByte,outBit;
int maxWidth=REC_MAX_RASTER_SIZE/MAX(1,yrow);


    if( !bou )
	{
      *oBit=MIN(maxWidth<<3,xbit);
      outByte = ((*oBit)+7) >> 3;
	  for(;yrow>0;yrow--,inBuf+=xbyte,outBuf+=outByte)
		  memcpy(outBuf,inBuf,outByte);
       return 0;
	}

//    bou=(best?&bestStack[num-1].bounds[0]:&mystack[num-1].bounds[0]);

    startX=MinBoundLeft(inBuf,xbyte,yrow,bou);

	if(startX >= xbit)
	{
		*oBit = 0;
		return startX;
    }

	if( startX + (maxWidth<<3)  < xbit )
       xbit= startX+(maxWidth<<3);

	outBit=MoveAllLeft(inBuf, xbyte, xbit, yrow,
					    outBuf, startX);
	outByte = (outBit+7)>>3;

    RemoveLeft(outBuf,outByte,yrow, startX,bou);

	*oBit=outBit;
	return startX;
}
///////////////
extern Bool IsSnap;
int AddBitmapToSnap(uchar *buf,int xbit,int yrow,int num,int dist);

int KleyRecog(uchar *inBuf,int xbyte,int xbit, int yrow,
					 welet *wl,int numWel,int porog,
					 uchar *names,uchar *probs,int maxNames)
{
int  startX=0,startY=0;
int  outBou[WR_MAX_HEIGHT];
int  startWel=0;
int  dist;
int  outByte,outBit;
int  k;
int  summaErrors=0x7FFF;
int  heiY;
//int  NumAlter;

 inBestStack=0;

 memset(names,0,maxNames);
 memset(probs,0,maxNames);

 if(wl==NULL || numWel <= 0) return 0;

 // установить правые границы кластеров - если еще не были
 if( !GetBounds(wl,numWel) )
	 return 0;

 if( yrow > WR_MAX_HEIGHT-2)
        yrow = WR_MAX_HEIGHT-2;

// int16_t RecogCluBound(uchar *rast,int16_t xbyte,int16_t xbit,int16_t yyrow,uchar *names,
//			 uchar *probs,int16_t maxNames,welet *wl,int numWel,
//			 int *bounds);

 TekInStack=0;
 for(startWel=0; ;)
 {
	 if( TekInStack < 0)
		 break;
	 startX=GetFromStack( inBuf, xbyte, xbit, yrow,
						  buf,&outBit,
						  TekInStack?&mystack[TekInStack-1].bound[0]:NULL);
	 outByte = (outBit+7)>>3;


	 // stay - recognize
	 if(  startX + POROG_STOP < xbit)
	 {

         Razmaz2(buf,bufrazmaz,(int16_t)(outBit),(int16_t)yrow,
			 (int16_t)0,(int16_t)POROG_ANGLES);

 // left bound
         FillLeft(buf,outByte,yrow,outByte<<3,StartInRow);
 // find - who less threshold
		 k= FindFirstClusterPorog(outByte<<3,yrow,
			  buf,bufrazmaz,
		      startWel, numWel, wl,
			  porog, outBou ,&dist,&startY,&heiY);

		 if(IsSnap)
			 AddBitmapToSnap( buf, outByte<<3, yrow, k<0?0:wl[k].let,dist);
	 }

	 else // все разрезали - добавим список
		 //  попробуем еще откатиться за новыми
		 //  вариантами ?
	 {
		 porog=UpdateSpisokAlter(porog,&summaErrors);
		 if(porog==0) break;
		 k=-1;
	 }

     // надо вернуться ?
	 if( k < 0 )
	 {
		 TekInStack--;
		 if( TekInStack < 0)  break;
		 startWel=mystack[TekInStack].num+1;
	 }
	 else
	 {   AddToStack(wl[k].let,k,dist,startX,yrow,outBou,startY,heiY);
		 startWel=0;
	 }

 }

  for(k=0;k<inBestStack && k < maxNames;k++)
  {
	   names[k]=(uchar)bestStack[k].name;
	   probs[k]=(uchar)(255-STRAFPOINT*bestStack[k].dist);
  }

 return k;
}
///////////////////

#ifdef _USE_CSTR_

#include "cstr.h"

#ifndef WIN32
typedef struct tagRECT {int left,top,right,bottom;} RECT;
#endif

// return - bytes in row
static int GetCommonSize(CSTR_rast fir,CSTR_rast last,RECT *rect)
{
 CSTR_rast_attr attr;

   if(!fir)
	 return -1;

   if( !CSTR_GetAttr(fir, &attr))
	   return -1;

   rect->left=attr.col;
   rect->right=attr.col+attr.w;
   rect->top=attr.row;
   rect->bottom=attr.row+attr.h;

   for(fir=CSTR_GetNext(fir);fir && fir!=last;fir=CSTR_GetNext(fir))
	{
        if( !CSTR_GetAttr (fir, &attr) )
                return -1;
		rect->left=MIN(rect->left,attr.col);
        rect->right=MAX(rect->right,attr.col+attr.w);
        rect->top=MIN(rect->top,attr.row);
        rect->bottom=MAX(rect->bottom,attr.row+attr.h);
	}

   return (rect->right-rect->left+7)>>3;
}
///////////////
static int FillInBuf(uchar *inBuf,int xbyte,int yrow,CSTR_rast fir,CSTR_rast las,
					 RECT *rect)
{
 CSTR_rast_attr attr;
 RecRaster recRast;
 int i,j;
 int sdvig,asdvig;
 uchar *bu,*inbu;
 int xbyte8;
 int inbyte;

   memset(inBuf,0,xbyte*yrow);

   for(;fir && fir!=las;fir=CSTR_GetNext(fir))
	{
        if( !CSTR_GetAttr (fir, &attr) )
                return -1;
		if( !CSTR_GetImage(fir,(uchar*)&recRast,CSTR_TYPE_IMAGE_RS ) )
                return -1;

		xbyte8 = REC_GW_WORD8(recRast.lnPixWidth);
		inbyte = (recRast.lnPixWidth+7)>>3;
		sdvig=(attr.col-rect->left)&7;
		asdvig=8-sdvig;

		bu=inBuf+(attr.row-rect->top)*xbyte+((attr.col-rect->left)>>3);
		inbu=recRast.Raster;

		for(i=0;i<attr.h;i++,bu+=xbyte,inbu+=xbyte8)
		{
			bu[0] |= inbu[0]>>sdvig;

			for(j=1;j<inbyte;j++)
				bu[j] |=(inbu[j-1]<<asdvig)|(inbu[j]>>sdvig);

			if( sdvig + attr.w > (inbyte<<3) )
                bu[j] |= (inbu[j-1]<<asdvig);
		}

	}

   return (rect->right-rect->left+7)>>3;
}
///////////////
static int RemoveRight(uchar *buff,int startX,int xbyte,int yrow,int *bou,
					   RECT *rect)
{
	int i,j,k;
	uchar *buf;

	rect->left = 0;
	rect->right= 0;
	for(buf=buff,i=0,rect->top=-1,rect->bottom=0;i<yrow;
	    i++,buf+=xbyte)
	{
       j=bou[i]-startX;
	   if(j<=0)
	   {
		   memset(buf,0,xbyte);
		   continue;
	   }

	   if( xbyte > (j>>3)+1 )
        memset(buf+(j>>3)+1,0,xbyte-(j>>3)-1);
	   buf[j>>3] &= mas11[j&7];

	   for(j=(j>>3),k=0;j>=0;j--)
	   {
		   if( buf[j] == 0)
			   continue;
           k=(j<<3)+8-firRight1[buf[j]];
           break;
	   }

	   if( k <= 0)
		   continue;

	   if( k > rect->right)
		   rect->right=k;

	   if( rect->top == -1)
		   rect->top=i;
	   rect->bottom=i;
	}

    if( rect->right <= 0)
		return 0;

    rect->bottom++;
	return 1;
}
//////////////
// use bufrazmaz as temporary buffer
static int GetAsRecRaster(uchar *inBuf,int xbit,int yrow,
						  RecRaster *rec, int nInStack,
						  RECT *rect)
{
 int *bou;
 int xbyte=(xbit+7)>>3;
 int startX;
 int outByte,outBit;
 int xbyte8;
 uchar *bb;
 int  yFir,yHei;
 int i;

 if( nInStack < 0)
   return -1;
 startX=GetFromStack( inBuf, xbyte, xbit, yrow,
					  bufrazmaz,&outBit,
					  nInStack?&bestStack[nInStack-1].bound[0]:NULL);
 outByte = (outBit+7)>>3;

 bou=&bestStack[nInStack].bound[0];
 yFir=bestStack[nInStack].yFirst;
 yHei=bestStack[nInStack].yHeight;

 if( RemoveRight(bufrazmaz+yFir*outByte,startX,outByte,yHei,bou+yFir,rect) < 0)
	 return -1;

 if( rect->right <= rect->left ||
	 rect->bottom <= rect->top )
	 return -1;

 // to real coordinates
 rect->top+=yFir;
 rect->bottom+=yFir;
 rect->left+=startX;
 rect->right+=startX;

 rec->lnPixWidth  = rect->right-rect->left;
 rec->lnPixHeight = rect->bottom-rect->top;
 rec->lnRasterBufSize=REC_MAX_RASTER_SIZE;
 xbyte8 = REC_GW_WORD8(rec->lnPixWidth);

 memset(rec->Raster,0,xbyte8*rec->lnPixHeight);

 for(bb=bufrazmaz+rect->top*outByte,i=0;i<rec->lnPixHeight;
    i++,bb+=outByte)
		memcpy(rec->Raster+i*xbyte8,bb,outByte);

 return startX;
}
///////////////
// from rcm.c
uchar decode_ASCII_to_[256][4]=
{
/*       0    1      2      3      4      5      6      7      8      9      a      b      c      d      e      f  */
/* 0*/ "\x00","\x01","\x02","\x03","\x04","\x05","\x06","\x07","\x08","\x09","\x0a","\x0b","\x0c","\x0d","\x0e","\x0f",
/* 1*/ "\x10","\x11","\x12","\x13","\x14","\x15","\x16","\x17","\x18","\x19","\x1a","\x1b","\x1c","\x1d","\x1e","\x1f",
/* 2*/ "\x20","\x21","\x22","\x23","\x24","\x25","\x26","\x27","\x28","\x29","\x2a","\x2b","\x2c","\x2d","\x2e","\x2f",
/* 3*/ "\x30","\x31","\x32","\x33","\x34","\x35","\x36","\x37","\x38","\x39","\x3a","\x3b","\x3c","\x3d","\x3e","\x3f",
/* 4*/ "\x40","\x41","\x42","\x43","\x44","\x45","\x46","\x47","\x48","\x49","\x4a","\x4b","\x4c","\x4d","\x4e","\x4f",
/* 5*/ "\x50","\x51","\x52","\x53","\x54","\x55","\x56","\x57","\x58","\x59","\x5a","\x5b","\x5c","\x5d","\x5e","\x2d\x2d",
/* 6*/ "\x60","\x61","\x62","\x63","\x64","\x65","\x66","\x67","\x68","\x69","\x6a","\x6b","\x6c","\x6d","\x6e","\x6f",
/* 7*/ "\x70","\x71","\x72","\x73","\x74","\x75","\x76","\x77","\x78","\x79","\x7a","\x7b","\x7c","\x7d","\x7e","\x7f",
/* 8*/ "\xc0","\xc1","\xc2","\xc3","\xc4","\xc5","\xc6","\xc7","\xc8","\xc9","\xca","\xcb","\xcc","\xcd","\xce","\xcf",
/* 9*/ "\xd0","\xd1","\xd2","\xd3","\xd4","\xd5","\xd6","\xd7","\xd8","\xd9","\xda","\xdb","\xdc","\xdd","\xde","\xdf",
/* a*/ "\xe0","\xe1","\xe2","\xe3","\xe4","\xe5","\xe6","\xe7","\xe8","\xe9","\xea","\xeb","\xec","\xed","\xee","\xef",
/* b*/ "\x7e","\x20","\x20","\x20","\x20","\x20","\x20","\x20","\x20","\x20","\x20","\x20","\x21","\x3f","\x20","\x20",
/* c*/ "\xB8","\x20","\xbf","\x20","\x20","\xaf","\xb2","\xa5","\xA8","\xba","\x20","\x20","\xaa","\x20","\x20","\x20",
/* d*/ "\x99","\x20","\x20","\x20","\x20","\x22","\xb4","\xab","\xbb","\x20","\xb3","\x20","\x20","\xa9","\xae","\x20",
/* e*/ "\xf0","\xf1","\xf2","\xf3","\xf4","\xf5","\xf6","\xf7","\xf8","\xf9","\xfa","\xfb","\xfc","\xfd","\xfe","\xff",
/* f*/ "\xe4","\xe4","\x20","\x20","\x20","\xf2","\x20","\xe8","\xe3","\x20","\x20","\x20","\x20","\xe0","\x95","\x20"
};
//////////////////
uchar CodePages[LANG_TOTAL]={
CSTR_ANSI_CHARSET            , // LANG_ENGLISH
CSTR_ANSI_CHARSET            , // LANG_GERMAN
CSTR_ANSI_CHARSET            , // LANG_FRENCH
CSTR_RUSSIAN_CHARSET         , // LANG_RUSSIAN
CSTR_ANSI_CHARSET            , // LANG_SWEDISH
CSTR_ANSI_CHARSET            , // LANG_SPANISH
CSTR_ANSI_CHARSET            , // LANG_ITALIAN
CSTR_RUSSIAN_CHARSET         , // LANG_RUSENG
CSTR_RUSSIAN_CHARSET         , // LANG_UKRAINIAN
CSTR_RUSSIAN_CHARSET         , // LANG_SERBIAN
CSTR_EASTEUROPE_CHARSET      , // LANG_CROATIAN
CSTR_EASTEUROPE_CHARSET      , // LANG_POLISH
CSTR_ANSI_CHARSET            , // LANG_DANISH
CSTR_ANSI_CHARSET            , // LANG_PORTUGUESE
CSTR_ANSI_CHARSET            , // LANG_DUTCH
CSTR_ANSI_CHARSET            , // LANG_DIG        15
CSTR_RUSSIAN_CHARSET         , // LANG_UZBEK	  16 // 01.09.2000 E.P.
CSTR_RUSSIAN_CHARSET         , // LANG_KAZ	      17
CSTR_RUSSIAN_CHARSET         , // LANG_KAZ_ENG    18
CSTR_EASTEUROPE_CHARSET      , // LANG_CZECH	  19
CSTR_EASTEUROPE_CHARSET      , // LANG_ROMAN	  20
CSTR_EASTEUROPE_CHARSET      , // LANG_HUNGAR	  21
CSTR_RUSSIAN_CHARSET         , // LANG_BULGAR	  22
CSTR_EASTEUROPE_CHARSET	     , // LANG_SLOVENIAN  23
BALTIC_CHARSET				 , // LANG_LATVIAN	  24
BALTIC_CHARSET				 , // LANG_LITHUANIAN 25
BALTIC_CHARSET				 , // LANG_ESTONIAN	  26
TURKISH_CHARSET				   // LANG_TURKISH	  27
};
//////////////////////
Bool32 p2_StoreVersions(CSTR_rast rast,RecVersions *rver,int lang)
{
int  i;
UniVersions cver,*ver;
uchar let;

//if( lang==LANG_ENGLISH && multy_language )
//    lang    = LANG_RUSENG;

 if(!rast) return FALSE;
 ver=&cver;
 memset(ver,0,sizeof(UniVersions));
 ver->lnAltCnt = MIN(REC_MAX_VERS,rver->lnAltCnt);
 ver->lnAltMax = REC_MAX_VERS;

 for(i=0;i<ver->lnAltCnt;i++)
      {
      let = rver->Alt[i].Code;
      strcpy(ver->Alt[i].Code , decode_ASCII_to_[let]);
      ver->Alt[i].Liga      =  let;
      ver->Alt[i].Prob      =  rver->Alt[i].Prob;
      ver->Alt[i].Method    =  rver->Alt[i].Method;
      ver->Alt[i].Charset   =  CodePages[lang]; // Oleg
      ver->Alt[i].Info      =  rver->Alt[i].Info;
     }

 return CSTR_StoreCollectionUni(rast,ver);
}
/////////////////
static int PutNewRasters(uchar *inBuf,int xbit,int yrow,CSTR_rast outFir,
						 int posLineX,int posLineY,int lang, int nNaklon, RECS2 *recs,
						 Bool32 fromNew, int countRazmaz)
{
   int i;
   RecRaster rec;
   CSTR_rast rastOut=outFir;
   CSTR_rast_attr attr={0};
   RecVersions vers;
   RECT rect;
   FonSpecInfo specInfo;


   for(i=0;i<inBestStack;i++)
   {
       if( recs && i ==  inBestStack-1 )
	   {
		   rec  = recs->rast;
		   vers = recs->vers;
		   rect.top  = recs->firY;
		   rect.left = recs->firX;

		   attr.nClust = recs->nClust;
	   }
	   else
	   {
		 if( GetAsRecRaster(inBuf,xbit,yrow,&rec,i,&rect) < 0)
		   return -1;

		 attr.nClust = bestStack[i].num+1;

// rerecognize - to add alternatives !?
	     memset(&vers,0,sizeof(RecVersions));

		 memset(&specInfo,0,sizeof(FonSpecInfo));
		 specInfo.countRazmaz = countRazmaz;
	     FONRecogChar(&rec,&vers,&specInfo);

	     if( vers.lnAltCnt <= 0 ) // from cut-glue
		 {
	      memset(&vers,0,sizeof(RecVersions));
	      vers.lnAltCnt = 1;
	      vers.Alt[0].Code=(uchar)bestStack[i].name;
	      vers.Alt[0].Prob=(uchar)(255-STRAFPOINT*bestStack[i].dist);
	      vers.Alt[0].Method=REC_METHOD_FON;
		 }
		 else
		 {
			 if( vers.lnAltCnt > 1 &&
				 (uchar)bestStack[i].name == vers.Alt[1].Code &&
				 vers.Alt[0].Prob == vers.Alt[1].Prob
               )
			 {
				 vers.Alt[1].Code = vers.Alt[0].Code;
				 vers.Alt[0].Code = (uchar)bestStack[i].name;
			 }
		 }
	   }

       attr.n_baton=255;  // =NO_BATONS;
       attr.pos_inc = 0;  // =erect_no;

       attr.language=(uchar)lang;
	   attr.flg =CSTR_f_let;

	   attr.w   = (short)rec.lnPixWidth;
	   attr.h   = (short)rec.lnPixHeight;

	   attr.clink  = vers.Alt[0].Prob;

	   attr.recsource    = CSTR_rs_bitcmp;
       attr.RecogHistory = CSTR_hi_fon;

	   if( fromNew )
	   {
        attr.row = rect.top+posLineY;
	    attr.col = rect.left+posLineX;
	    attr.r_row =attr.row+(short)((int32_t)nNaklon*attr.col/2048);
        attr.r_col =attr.col-(short)((int32_t)nNaklon*attr.row/2048);
	   }
	   else
	   {
		attr.r_row = rect.top+posLineY;
	    attr.r_col = rect.left+posLineX;
	    attr.row =attr.r_row-(short)((int32_t)nNaklon*attr.r_col/2048);
        attr.col =attr.r_col+(short)((int32_t)nNaklon*attr.r_row/2048);
	   }


                        // нельзя NewRaster - вставится после пробела,
                        // а надо до
       if( !(rastOut=CSTR_InsertRaster (rastOut)) ||
           !CSTR_SetAttr (rastOut, &attr) ||
           !CSTR_StoreRaster (rastOut, &rec) || //OLEG
           !p2_StoreVersions (rastOut, &vers,lang)
//           (rs.lnPixHeight && !CSTR_StoreScale(rastOut,comp->scale)) )// OLEG
         )
                 return 0;
   }

   return i;
}
//////////////////
FON_FUNC(int) FONRecogGlue(CSTR_rast firLeo,CSTR_rast lasLeo,
						   CSTR_rast firOut,int lang, int nNaklon)
{
 int  startX=0,startY=0;
 int  heiY;
 int  outBou[WR_MAX_HEIGHT];
 int  startWel=0;
 int  dist;
 int  outByte,outBit;
 int  k;
 int  summaErrors=0x7FFF;
 FONBASE *fonbase = GetStaticFonbase();
 welet *wl;
 int numWel;
 RECT wordRect;
 uchar *inBuf=NULL;
 int xbyte,xbit,yrow;
 int porog=20;

 if(!fonbase || !fonbase->start || fonbase->inBase <=0)
	 return 0;

 inBestStack=0;
 wl=fonbase->start;
 numWel=fonbase->inBase;

 // установить правые границы кластеров - если еще не были
 if( !GetBounds(wl,numWel))
	 return 0;

 // сделать общий растр
 xbyte=GetCommonSize(firLeo,lasLeo,&wordRect);
 if( xbyte <= 0)
	 return -1;

 yrow=wordRect.bottom-wordRect.top;
 if( yrow > WR_MAX_HEIGHT-2)
	 return -11;

 inBuf=malloc(xbyte*yrow);
 if( !inBuf )
	 return -1;

 xbit=wordRect.right-wordRect.left;

 if( !FillInBuf(inBuf,xbyte,yrow,firLeo,lasLeo,&wordRect) )
 {
	 free(inBuf);
	 return -2;
 }

 TekInStack=0;
 for(startWel=0; ;)
 {
	 if( TekInStack < 0)
		 break;
	 startX=GetFromStack( inBuf, xbyte, xbit, yrow,
						  buf,&outBit,
						  TekInStack?&mystack[TekInStack-1].bound[0]:NULL);
	 outByte = (outBit+7)>>3;


	 // stay - recognize
	 if(  startX + POROG_STOP < xbit)
	 {

         Razmaz2(buf,bufrazmaz,(int16_t)(outBit),(int16_t)yrow,
			 (int16_t)0,(int16_t)POROG_ANGLES);

 // left bound
         FillLeft(buf,outByte,yrow,outByte<<3,StartInRow);
 // find - who less threshold
		 k= FindFirstClusterPorog(outByte<<3,yrow,
			  buf,bufrazmaz,
		      startWel, numWel, wl,
			  porog, outBou ,&dist,&startY,&heiY);

		 if(IsSnap)
			 AddBitmapToSnap( buf, outByte<<3, yrow, k<0?0:wl[k].let,dist);
	 }

	 else // все разрезали - добавим список
		 //  попробуем еще откатиться за новыми
		 //  вариантами ?
	 {
		 porog=UpdateSpisokAlter(porog,&summaErrors);
		 //if(porog==0)
			 break;
		 k=-1;
	 }

     // надо вернуться ?
	 if( k < 0 )
	 {
		 TekInStack--;
		 if( TekInStack < 0)  break;
		 startWel=mystack[TekInStack].num+1;
	 }
	 else
	 {   // is memory in stack ?
		 if( AddToStack(wl[k].let,k,dist,startX,yrow,outBou,startY,heiY) < 0)
		 {
			 free(inBuf);
			 return -12;
		 }

		 startWel=0;
	 }

 }

 k = PutNewRasters( inBuf, xbit, yrow, firOut,
		wordRect.left,wordRect.top,lang,nNaklon,NULL,TRUE,1);

 free(inBuf);
 return k;
}
////////////////

#define MIN_WIDTH 8

////////////////
static RECS2 recs2[2];
////////////
FON_FUNC(int32_t) FONRecog2Glue(CSTR_rast firLeo,CSTR_rast lasLeo,
							  CSTR_rast firOut,CSTR_rast lasOut,
						    int lang,
							int porog, int nNaklon, int countRazmaz)
{
 int  startX=0;
 int  outByte,outBit;
 FONBASE *fonbase = GetStaticFonbase();
 welet *wl;
 int numWel;
 uchar *inBuf=NULL;
 int xbyte,xbit,yrow;
 int i,j,k;
 int better;
 int numAlt;
 RECT wordRect;
 CSTR_rast rst;
 CSTR_rast_attr attr;
 RecVersions vers;
 FonSpecInfo specInfo;
 if(!fonbase || !fonbase->start || fonbase->inBase <=0)
	 return 0;

 wl=fonbase->start;
 numWel=fonbase->inBase;

 // установить правые границы кластеров - если еще не были
 if( !GetBounds(wl,numWel))
	 return 0;

 inBestStack=0;

 // сделать общий растр - сначала получить размеры
 xbyte=GetCommonSize(firLeo,lasLeo,&wordRect);
 if( xbyte <= 0)
	 return -1;
 xbit=wordRect.right-wordRect.left;

 yrow=wordRect.bottom-wordRect.top;
 if( yrow > WR_MAX_HEIGHT-2)
	 return -11;

 if( xbit < 2*MIN_WIDTH || yrow < MIN_WIDTH )
	 return 0;

 inBuf=malloc(xbyte*yrow);
 if( !inBuf )
	 return -1;

 if( !FillInBuf(inBuf,xbyte,yrow,firLeo,lasLeo,&wordRect) )
 {
	 free(inBuf);
	 return -2;
 }

 // copy to buf, byte bounds
 startX=GetFromStack( inBuf, xbyte, xbit, yrow,
					  buf,&outBit,NULL);
 outByte = (outBit+7)>>3;

 Razmaz2(buf,bufrazmaz,(int16_t)(outBit),(int16_t)yrow,
			 (int16_t)0,(int16_t)POROG_ANGLES);

 FillLeft(buf,outByte,yrow,outByte<<3,StartInRow);

 // dont'recog by itself only!
 CSTR_GetAttr(firOut,&attr);
 CSTR_GetCollection(firOut,&vers);

 numAlt=FindBestClustersBound(xbit,yrow,buf,bufrazmaz,
		      numWel,(255-porog)/STRAFPOINT,fonbase->start,
			  AllRecBou,MAX_ALT_BOU,
			  StartInRow, attr.col, attr.row,
			  vers.lnAltCnt > 0 ? vers.Alt[0].Info:0,
			  countRazmaz);

 if(IsSnap)
 {
	 AddBitmapToSnap( buf, outByte<<3, yrow, '#' ,numAlt);
//	 AddBitmapToSnap( bufrazmaz, outBit+2, yrow+2, 0 ,numAlt);
     FONShowSnap();
 }

 if( numAlt <= 0 )
 {
     free(inBuf);
	 return 0;
 }

 for(i=0,better=-1;i<numAlt;i++)
 {
  int nAlt;
  int firY,lasY;
  int out8;
  uchar *oBuf;
  RECS2  *recs=(better==-1?recs2:recs2+1);

  startX=GetFromStack( inBuf, xbyte, xbit, yrow,
	 				   buf,&outBit,
					   AllRecBou[i].bound);
  outByte = (outBit+7)>>3;

  if( startX + MIN_WIDTH >= xbit)
	  continue;

  if(IsSnap)
  {
	 AddBitmapToSnap( buf, outBit, yrow, AllRecBou[i].name ,AllRecBou[i].prob);
	 FONShowSnap();
  }

  // find first-last rows
  for(j=0,oBuf=buf;j<yrow;j++,oBuf+=outByte)
  {
	  for(k=0;k<outByte;k++)
		  if( oBuf[k] ) break;
      if( k < outByte ) break;
  }
  firY=j;
  if(firY >= yrow)
	  continue;

  for(j=yrow,oBuf=buf+(yrow-1)*outByte;j>0;j--,oBuf-=outByte)
  {
	  for(k=0;k<outByte;k++)
		  if( oBuf[k] ) break;
      if( k < outByte ) break;
  }
  lasY=j;

  if(lasY-firY < MIN_WIDTH)
	  continue;

  recs->firX = startX;
  recs->firY = firY;

  out8=REC_GW_WORD8(xbit-startX);
  recs->rast.lnPixWidth  = xbit-startX;
  recs->rast.lnPixHeight = lasY-firY;
  recs->rast.lnRasterBufSize = REC_MAX_RASTER_SIZE;
  memset(recs->rast.Raster,0,out8*recs->rast.lnPixHeight);

  {
   uchar *iBuf = recs->rast.Raster;
   oBuf = buf+outByte*firY;
   for(j=recs->rast.lnPixHeight;j>0;j--,iBuf+=out8,oBuf+=outByte)
		  memcpy(iBuf,oBuf,outByte);
  }

  memset(&specInfo,0,sizeof(FonSpecInfo));
  specInfo.countRazmaz = countRazmaz;
  nAlt=FONRecogChar(&recs->rast,&recs->vers,&specInfo);

  if( nAlt <= 0)
	  continue;
  if( recs->vers.lnAltCnt <= 0  || recs->vers.Alt[0].Prob < porog)
	  continue;

  if( better == -1 || MIN(AllRecBou[better].prob,recs2[0].vers.Alt[0].Prob) <
	     MIN(AllRecBou[i].prob,recs->vers.Alt[0].Prob) )
  {
	  recs->nClust = specInfo.nClust;
	  if( better != -1)
		  memcpy(&recs2[0],recs,sizeof(RECS2));
	  better = i;
  }
 }

 if( better == -1 )
 {
	 free(inBuf);
	 return 0;
 }

 // remove old
 for(rst=CSTR_GetNext(firOut);rst && rst!=lasOut;)
	 rst = CSTR_DelRaster(rst);

 inBestStack = 2;
 bestStack[0] =  AllRecBou[better];

 k = PutNewRasters( inBuf, xbit, yrow, firOut,
				    wordRect.left, wordRect.top, lang, nNaklon,
					recs2, firOut == firLeo, countRazmaz );
 // remove first old - stay only new rasters
 CSTR_DelRaster(firOut);

 free(inBuf);
 return k;
}
////////////////

#endif
///////////////////
FON_FUNC(int32_t) FONRecogBroken(CSTR_rast firLeo,CSTR_rast lasLeo,
							  CSTR_rast firOut,CSTR_rast lasOut,
						    int lang,
							int porog, int nNaklon, int nRazmaz)
{
 int xbyte,xbit,yrow;
 int numAlt;
 RECT wordRect;
 CSTR_rast rst,rastOut;
 CSTR_rast_attr attr;
 RecRaster recRast;
 RecVersions verOld;
 FonSpecInfo specInfo;


 // сделать общий растр - сначала получить размеры
 xbyte=GetCommonSize(firLeo,lasLeo,&wordRect);
 if( xbyte <= 0)
	 return -1;

 xbit=wordRect.right-wordRect.left;
 yrow=wordRect.bottom-wordRect.top;
 if( yrow > WR_MAX_HEIGHT-2 || xbit > WR_MAX_WIDTH-2 )
	 return -11;

  xbyte=REC_GW_WORD8(xbit);

  if( xbyte*yrow > REC_MAX_RASTER_SIZE )
	  return -12;

  recRast.lnPixWidth  = xbit;
  recRast.lnPixHeight = yrow;
  recRast.lnRasterBufSize = REC_MAX_RASTER_SIZE;
  memset(recRast.Raster,0,xbyte*recRast.lnPixHeight);

  if( !FillInBuf(recRast.Raster,xbyte,yrow,firLeo,lasLeo,&wordRect) )
  {
	 return -2;
  }

 // dont'recog by itself only!
  CSTR_GetAttr(firOut,&attr);
  CSTR_GetCollection(firOut,&verOld);

 // fill specInfo
        memset(&specInfo,0,sizeof(FonSpecInfo));

//        specInfo.nFieldRow=p2globals.line_number;
        specInfo.col      =attr.col;
        specInfo.row      =attr.row;
        if( verOld.lnAltCnt > 0)
		{
                specInfo.nInCTB = verOld.Alt[0].Info;
                specInfo.nLet   = verOld.Alt[0].Code;
		}

		specInfo.countRazmaz = MAX(1,nRazmaz);  // was alwayes 4

  numAlt=FONRecogChar(&recRast,&verOld,&specInfo);

  if(IsSnap)
  {
	 AddBitmapToSnap( recRast.Raster, xbyte<<3, yrow, verOld.Alt[0].Code,verOld.Alt[0].Prob);
     FONShowSnap();
  }

  if( numAlt <= 0)
	  return 0;
  if( verOld.lnAltCnt <= 0  || verOld.Alt[0].Prob < porog)
	  return 0;

  // уменьшим ...
//  for( i=0; i < verOld.lnAltCnt; i++)
//    verOld.Alt[i].Prob = MAX(1,verOld.Alt[i].Prob-10);

 // remove old
  for(rst=CSTR_GetNext(firOut);rst && rst!=lasOut;)
	 rst = CSTR_DelRaster(rst);

  memset(&attr,0,sizeof(CSTR_rast_attr));
  attr.n_baton=255;  // =NO_BATONS;
  attr.pos_inc = 0;  // =erect_no;

  attr.language=(uchar)lang;
  attr.flg =CSTR_f_let;

  attr.w   = (short)recRast.lnPixWidth;
  attr.h   = (short)recRast.lnPixHeight;

  attr.clink  = verOld.Alt[0].Prob;
  attr.nClust = specInfo.nClust;
  attr.recsource    = CSTR_rs_bitcmp;
  attr.RecogHistory = CSTR_hi_fon;


  if( firOut == firLeo )  // from new
	   {
        attr.row = wordRect.top;
	    attr.col = wordRect.left;
	    attr.r_row =attr.row+(short)((int32_t)nNaklon*attr.col/2048);
        attr.r_col =attr.col-(short)((int32_t)nNaklon*attr.row/2048);
	   }
   else
	   {
		attr.r_row = wordRect.top;
		attr.r_col = wordRect.left;
		attr.row =attr.r_row-(short)((int32_t)nNaklon*attr.r_col/2048);
		attr.col =attr.r_col+(short)((int32_t)nNaklon*attr.r_row/2048);
	   }

                        // нельзя NewRaster - вставится после пробела,
                        // а надо до
  if( !(rastOut=CSTR_InsertRaster (firOut)) ||
      !CSTR_SetAttr (rastOut, &attr) ||
      !CSTR_StoreRaster (rastOut, &recRast) || //OLEG
      !p2_StoreVersions (rastOut, &verOld,lang)
//           (rs.lnPixHeight && !CSTR_StoreScale(rastOut,comp->scale)) )// OLEG
    )
                 return 0;

 // remove first old - stay only new rasters
  CSTR_DelRaster(firOut);

  return 1;
}
////////////////

