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

// увеличить вес словарных кластеров в зависимости от
// числа словарных символов
#define _ACTIVE_LINGVO_
// использовать признаки курсивности и т.п.
// при выборе кластеров
//#define _TEST_ITALIC_SERIF_

//#define _OUT_COUNT_FONT_
//#define _SAVE_CTB_

// use ASCII (DOS-coding) !!!

// дополнительная проверка колонок таблиц
#define _TEST_COLUMNS_
// дополнительная проверка не вошедших в шрифты кластеров
#define _TEST_STAY_

// базироваться в первую очередь на хорошем контексте
//#define _MAIN_LINGVO_

// производится попытка анализа - сколько шрифтов
// находится в базе
#define _TEST_MULTI_FONT_

// имена символов указаны правильно -
// а не с точноcтью до регистра
#define _GOOD_BIG_SMALL_


#define MAX_HEIGHT 128
#define MAX_WIDTH  128
#define POROG_BAD_LINGVO 80
#define POROG_PROB_GOOD 185
#define POROG_GOOD_DVA  210   // if from 2 symbols 1.10.98
#define POROG_VERYGOOD  250   // for clusters weight 1

#define POROG_360       220  //  можно взять из 3,6,0 как букву

#ifdef _MAIN_LINGVO_
 #define LINGVO_COUNT    128
#else
 #define LINGVO_COUNT    1
#endif

#define MAXFONT         8
#define UNRECOG_SYMBOL '~'

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "leo.h"
#include "ctb.h"
#include "sfont.h"
#include "fonrec.h"
#include "clu_lang.h"
#include "minmax.h"

int16_t DistanceHausDLL(uchar  *b1,int16_t xbyte1,int16_t yrow1,
						uchar  *b2,int16_t xbyte2,int16_t yrow2,
						int16_t porog);
static void CorrectSizes(int *minBig,int *maxBig,int *minLit,int *maxLit,int bSize,int lSize,int porogSize);

// from clu_ita.c
int32_t TestItSeBoldCluster(int numCluster, InfoCluster *infoC,
				 int sBig,int sLit,int *maxC, uint32_t *fifi,
				 FONTFIELD *ff);
// from clu_test.c
// проверить растр на противоречие с хорошими кластерами
//
int TestClusterGood(  Nraster_header *rh, int testClus,int startNum,int count,int NumAll,
					 int porog,  int16_t *nClus, uchar *metkaGood,uchar *metkaValid,int nCompare);
int TestIntersectFields(int count,FONTFIELD *f1,InfoCluster *infoC);

static int AnalizeSizes(InfoCluster *infoC,int numClus,
						uchar *metka, int fir);
static int GetNextFont(int *bSize,int *lSize,int *pSize,
					   int *minBig,int *maxBig,
					   int *minLit,int *maxLit,
					   uint32_t *fields);

// //////////
InfoCluster infoClusterStat[MAXWEICLUS];
int countCluster[256]; // how many clusters with name

// if not found very good symbol - get best from bad
static int TestFillAll  = 1;
// в одном поле (строке)   - один шрифт
static int OneFontField = 1;

static int maxCluster[256];   // which   best
static int max2Cluster[256];   // small or big - name same in examples!
static int bestSizes[MAX_HEIGHT];
static int bestWidth[MAX_WIDTH];

static uint32_t    usedFields[256][NFIELDDWORD];
static FONTFIELD fontField[MAXFONT];


void SetFillAll(int sss)
{
	TestFillAll=sss;
	OneFontField = sss; // temporary?
}
/////////////////////
//  is one bitmap part of other ?
//
static int HausInnerDistance(Nraster_header *rh1,Nraster_header *rh2,int porog)
{
int dist;

//     if(rh[i].fat || rh[j].fat )  porog=0;

	 dist=DistanceHausDLL(rh1->pHau,(int16_t)(rh1->xbyte),rh1->h,
		  rh2->pHaur,(int16_t)(rh2->xbyte),
		  (int16_t)(rh2->h+1),(int16_t)porog);

	 if(dist <= porog ) return dist;

	 dist=DistanceHausDLL(rh2->pHau,(int16_t)(rh2->xbyte),rh2->h,
		  rh1->pHaur,(int16_t)(rh1->xbyte),
		  (int16_t)(rh1->h+1),(int16_t)porog);

	 return dist;
}
////////////////////////
// по номеру поля выставить бит в нужном месте
void MakeDWORDField(int i,uint32_t *fifi)
{
  int j;

	memset(fifi,0,NFIELDDWORD*sizeof(uint32_t));

	if(i<=0 || i > MAXFIELD)
		return;
	j=(i-1)>>5;     // 32
    fifi[j]=((uint32_t)1)<<(i-j*32-1);
}
/////////////////////////
void AddDWORDField(int i,uint32_t *fifi)
{
  int j;

	if(i<=0 || i > MAXFIELD)
		return;
	j=(i-1)>>5;     // 32
    fifi[j]|=((uint32_t)1)<<(i-j*32-1);
}
/////////////////////////
//
static int GetMaxFrom0(int numSymbol,Nraster_header *rh,
				  int clusDig,int16_t *nClus,
				  int bSize,int numClus,InfoCluster *infoC,
				  uchar *metkaGood,uchar *metkaValid)
{
	int i,best;

	for(i=0,best=-1;i<numClus;i++)
	{
		if( i==clusDig ) continue;
		if( infoC[i].let != 48 ) //'0'
			continue;
		if( metkaGood[i] < 220 ) continue;
		if( infoC[i].mh < bSize) continue;
		if( infoC[i].count <= 1) continue;
		if( metkaValid[i] & LEO_VALID_DIGIT ) continue;  // digit field
		if(best==-1 || infoC[i].count > infoC[best].count ||
		   infoC[i].count == infoC[best].count && metkaGood[i] > metkaGood[best])
		  best=i;
	}

	if( best < 0 ) return -1;

	// rename cluster !
	infoC[best].let=142; // russian ASCII 'O'
	best++;
    for(i=0;i<numSymbol;i++)
		if(nClus[i]==best) rh[i].let=142;
    return best-1;
}
//////////
//
//  попробовать на похожесть с иными
//
static char somnitLet[]="\x8E\xA1\x87\x4F"; // ОбЗO-eng
static char accordDig[]="0630";
static int TryRename(int testCluster,int numSymbol,Nraster_header *rh,
				     int16_t *nClus,int numClus,InfoCluster *infoC )
{
 int i,j;
 int curClus;
 int porog;
 char *qq;
 char accordName;

// return 0;

 if( testCluster < 0 || testCluster >= numClus)
	 return 0;
 // don't touch LINGVO
 if( infoC[testCluster].valid & LEO_VALID_LINGVO )
	 return 0;
 //  try only special
 if( (qq=strchr(somnitLet,infoC[testCluster].let)) == NULL )
	 return 0;
 accordName=accordDig[qq-somnitLet];

 // in nClus numbers from 1 !!!
 testCluster++;

 for(i=0;i<numSymbol;i++)
 {
   if( nClus[i] != testCluster ) continue;

   if(rh[i].fat) porog=0;
   else porog=2;

   for(j=0;j<numSymbol;j++)
   {
     if(rh[j].let != accordName )
		 continue;
     curClus=nClus[j]-1;

	 // only LINGVO !
	 if( (infoC[curClus].valid & LEO_VALID_LINGVO)==0)
		 continue;
	 // only tested !
	 if( infoC[curClus].good==0 )
		 continue;

	 if( HausInnerDistance(rh+i,rh+j,porog) <= porog ) break;
   }
   if(j < numSymbol) break;;
 }

 if( i < numSymbol )
 {
	// rename cluster !
	infoC[testCluster-1].let=accordName;
    for(i=0;i<numSymbol;i++)
		if(nClus[i]==testCluster) rh[i].let=accordName;
    return 1;
 }
 return 0;
}
///////////////////
//
// compare clusters '0' & russian O - 142 dos
//					'6' & russian 'б' - 161 dos
// infoC.isSame=0 !
// выбрать лучшую из 0 как букву О
//
static int TestO0b6(int numSymbol,Nraster_header *rh,
				  int clusDig,int clusLet,int16_t *nClus,
				  int bSize,int numClus,InfoCluster *infoC,
				  uchar *metkaGood,int nameLet)
{
 int i,j;
 int best=-1;    // best from new clusters
 int curClus;
 int porog;

 if( clusDig < 0 || clusLet < 0) return clusLet;

 // was > 2 - matrix2 find 4 !? 30.10.98
 if( abs( infoC[clusDig].mh-infoC[clusLet].mh) > 4 )
	 return clusLet;

 // use as metka for common clusters with clusDig
 for(i=0;i<numClus;i++) infoC[i].isSame=0;

 for(i=0;i<numSymbol;i++)
 {
   if(rh[i].let != nameLet ) continue;
   curClus=nClus[i]-1;
   if(curClus < 0 || curClus >= numClus ) continue;
   if( infoC[curClus].isSame ) continue;
   if( infoC[curClus].mh < bSize ) continue;
   if( infoC[curClus].invalid) continue;

   if(rh[i].fat) porog=0;
   else porog=2;

   for(j=0;j<numSymbol;j++)
   {
     if(nClus[j]!=clusDig+1) continue;
	 if( HausInnerDistance(rh+i,rh+j,porog) <= porog ) break;
   }
   if(j < numSymbol) infoC[curClus].isSame=1;
 }

 if( infoC[clusLet].isSame == 0) return clusLet;

 for(i=0,best=-1;i<numClus;i++)
 {
	 if( infoC[i].let != nameLet ) continue;
	 if( infoC[i].isSame !=0 ) continue;
	 if( infoC[i].invalid) continue;
	 if( infoC[i].mh < bSize ) continue;
	 if(metkaGood[i] < POROG_360 ) continue;  // check ???
	 if(best==-1 || infoC[i].count > infoC[best].count ||
		infoC[i].count == infoC[best].count && metkaGood[i] > metkaGood[best])
		best=i;
 }

 return (best >= 0 ?best : clusLet);
}
///////////////////////
void GetClusterStatistic(int numSymbol,int numCluster,Nraster_header *rh,
						int16_t *nClus,InfoCluster *infoC,int *countC,
						uchar *metkaGood,uchar *metkaValid,Bool addLingvo)
{
 int i;
 int curClus;
 int let;
 uint32_t odin = 1;

 memset(countC,0,256*sizeof(int));
 memset(infoC,0,numCluster*sizeof(InfoCluster));

 for(i=0;i<numSymbol;i++)
 {
    curClus=nClus[i]-1;
	if (curClus <0 || curClus >= numCluster ) continue;
	// no twins in version from CTB, only invalids !
    if( rh[i].num )
     infoC[curClus].invalid=TRUE;

     AddDWORDField(rh[i].nField,infoC[curClus].fields);

	 let=TestCursivName((uchar)rh[i].let);

#ifndef _GOOD_BIG_SMALL_
	// are big & small - before were only BIG
	// convert to big - exept a,б,e,
	if(let >= 162 && let < 176 && let != 165)
		let-=32;
	else if(let >=224 && let < 240)
		let-=80;
#endif

    infoC[curClus].let=let;

    infoC[curClus].mw+=rh[i].w;
    infoC[curClus].mh+=rh[i].h;
	if( infoC[curClus].count == 0)
         infoC[curClus].first=i;
	infoC[curClus].count++;
	if( infoC[curClus].count == 1 && infoC[curClus].let>=0 &&
		infoC[curClus].let < 256 )
		countC[infoC[curClus].let]++;
	infoC[curClus].prob   =MAX(infoC[curClus].prob,rh[i].prob);
	infoC[curClus].valid |= rh[i].valid;

#ifdef _ACTIVE_LINGVO_
	if( (rh[i].valid & LEO_VALID_LINGVO) &&
		infoC[curClus].isSame < 255 )
		infoC[curClus].isSame++;
#endif

	if( infoC[curClus].count < 255 )
	{
	  if( rh[i].italic ) infoC[curClus].nItalic++;
	  if( rh[i].serif )  infoC[curClus].nSerif++;
	  if( rh[i].gelv )   infoC[curClus].nGelv++;
	  if( rh[i].bold )   infoC[curClus].nBold++;
	}

	// 25.10.2000
	if( rh[i].tablColumn && rh[i].tablColumn <= 32)
           infoC[curClus].tablColumn |= (odin << (rh[i].tablColumn-1));
 }

  // middle width,height
 for(curClus=0;curClus<numCluster;curClus++)
 {
  if( (i=infoC[curClus].count) <= 0 ) continue;
  infoC[curClus].mw=(infoC[curClus].mw+(infoC[curClus].count/2))/infoC[curClus].count;
  infoC[curClus].mh=(infoC[curClus].mh+(infoC[curClus].count/2))/infoC[curClus].count;
  //  In metka - working
  if(metkaGood)
   metkaGood [curClus] = infoC[curClus].prob  ;
  if(metkaValid)
   metkaValid[curClus] = infoC[curClus].valid ;

  if( (infoC[curClus].nItalic << 1 ) >= i )
	  infoC[curClus].attr |= FON_CLU_ITALIC;
  if( (infoC[curClus].nSerif << 1 ) >= i )
	  infoC[curClus].attr |= FON_CLU_SERIF;
  if( (infoC[curClus].nGelv << 1 ) >= i )
	  infoC[curClus].attr |= FON_CLU_GELV;
  if( (infoC[curClus].nBold << 1 ) >= i )
	  infoC[curClus].attr |= FON_CLU_BOLD;

#ifdef _ACTIVE_LINGVO_
  if( addLingvo )
  {
   if( infoC[curClus].count == 2 && infoC[curClus].isSame > 1)
	   infoC[curClus].isSame=1;
   infoC[curClus].count += infoC[curClus].isSame;
  }

  infoC[curClus].isSame = 0; // working field
#endif
 }
}
////////////////////
// use maxCluster,max2Cluster - as static !
static int FindBest(int  let,int numCluster,InfoCluster *infoC,
					int minSize,int maxSize,
					uchar *metkaGood,uchar *metkaValid,
					int isCluster,int porogSize,
					Nraster_header *rh,int NumAll,
					int16_t *nClus)
{
	int i,best;

	if( let == UNRECOG_SYMBOL) return -1;

	for(i=0,best=-1;i<numCluster;i++)
	{
//		if( metkaGood[i] < POROG_PROB_GOOD ) continue;
		if(infoC[i].let != let )
			continue;
		if(infoC[i].invalid )
			continue;
		if( i == isCluster )
			continue;

		if( ( metkaValid[i] & LEO_VALID_FINAL ) == 0)
			continue;

	 // уже есть из этого поля ?
	    if( IntersectFields(usedFields[let],infoC[i].fields ) )
		 continue;

		// очень слабые не брать ... ?
		if( infoC[i].prob < 130 ||
			infoC[i].count == 1 && infoC[i].prob < 150 )
			continue;

		if( infoC[i].mh  <  minSize )  continue;
		if( infoC[i].mh  >= maxSize )  continue;
		if( isCluster >=0 &&
			abs( infoC[i].mh - infoC[isCluster].mh) < porogSize )
			continue;

		// test konflict with other
		if( infoC[i].count == 1 &&
            TestClusterGood( rh,i+1,infoC[i].first,1,NumAll,
							 0, // porog
							 nClus, metkaGood,NULL,0) > 0
			)
		{ infoC[i].invalid=TRUE;
		  continue;
		}

		if( best < 0 ||	infoC[i].count > infoC[best].count ||
			infoC[i].count == infoC[best].count &&
			infoC[i].prob > infoC[best].prob )
			best=i;
	}
	return best;
}
//////////////
static int AddSpecialName(int  let,int numCluster,InfoCluster *infoC,
					uchar *metkaGood )
{
	int i;

	for(i=0;i<numCluster;i++)
	{
		if( metkaGood[i] < POROG_PROB_GOOD ) continue;
		if( infoC[i].let != let ) continue;
		if( infoC[i].good ) continue;
		if( infoC[i].invalid ) continue;
		if( infoC[i].valid & LEO_VALID_LINGVO )
			infoC[i].good=1;
	}
	return 0;
}
//////////////
static int BestBySize(int let,int numCluster,InfoCluster *infoC,int bSize,int porog,
					  uchar *metkaGood)
{

	int i,best;
	int dist;

	for(i=0,best=-1;i<numCluster;i++)
	{
		if( metkaGood[i] < POROG_PROB_GOOD ) continue;
		if(infoC[i].let != let ) continue;
		if(infoC[i].invalid ) continue;
//		if(infoC[i].count <= 1) continue;
		if( (dist=abs(infoC[i].mh-bSize)) > porog ) continue;

		//if( best < 0 || abs(infoC[i].mh-bSize)< abs(infoC[best].mh-bSize) ||
		//	abs(infoC[i].mh-bSize) == abs(infoC[best].mh-bSize) &&
		if( best < 0 ||	infoC[i].count > infoC[best].count )
			best=i;
	}
	return best;
}
//////////////
Bool32 BadCluster(InfoCluster *infoC)
{
     if( infoC->invalid ) return TRUE;

	 if( (infoC->valid & LEO_VALID_FINAL ) == 0)
		 return TRUE;
	 if( infoC->valid & LEO_VALID_LINGVO )
		 return FALSE;

     if( infoC->count <= 2  && infoC->prob < POROG_GOOD_DVA )
		 return TRUE;
	 if( infoC->count <= 1  && infoC->prob < POROG_VERYGOOD )
		 return TRUE;
	 if ( infoC->prob < POROG_PROB_GOOD )
		 return TRUE;

	 return  FALSE;
}
////////////////
#define POROG_HEIGHT 4
static int AnalyzeInfo(int numCluster,InfoCluster *infoC,
					   uchar *metkaGood, uchar *metkaValid,
					   int *tipSizeBig,int *tipSizeLit,
					   int *porSize,int *standWidth)
{
 int i,j;
 int bSize,bWidth;
 int bSizeLeft;
 int sizeBig,sizeLit;
 int porogSize;
 //
 // now test sizes - if only one font !
 //
 memset( bestSizes,0,sizeof(bestSizes));
 memset( bestWidth,0,sizeof(bestWidth));

 for(j=0;j<numCluster;j++)
 {
	 if(infoC[j].let==UNRECOG_SYMBOL) continue;
     if(infoC[j].invalid ) continue;

	 if( ( metkaValid[j] & LEO_VALID_FINAL ) == 0)
		{metkaGood[j]=0; continue;}


	 if(  metkaValid[j] & LEO_VALID_LINGVO )
		   {
		      metkaGood[j]=MAX(metkaGood[j],POROG_GOOD_DVA);
#ifndef _ACTIVE_LINGVO_
	          infoC[j].count+=LINGVO_COUNT;  // make more
#endif
		   }


     if(infoC[j].count <= 2  && metkaGood[j] < POROG_GOOD_DVA )
		 {metkaGood[j]=0; continue;}

	 if(infoC[j].count <= 1  && (metkaGood[j] < POROG_VERYGOOD ||
		          !LetDigSymbol(infoC[j].let) ) )
		{metkaGood[j]=0; continue;}
     if( infoC[j].mh < MAX_HEIGHT) bestSizes[infoC[j].mh]++;
	 if( infoC[j].mw < MAX_WIDTH) bestWidth[infoC[j].mw]++;
 }

 for(i=2,bWidth=1;i<MAX_WIDTH;i++)
	 if(bestWidth[i] > bestWidth[bWidth]) bWidth=i;

 for(i=2,bSize=1;i<MAX_HEIGHT;i++)
	 if(bestSizes[i] > bestSizes[bSize]) bSize=i;

 for(j=bSize-POROG_HEIGHT;j>1;j--) if( bestSizes[j] > bestSizes[j+1] ) break;
 for(bSizeLeft=j,j--;j>1;j--)
	 if(bestSizes[j] > bestSizes[bSizeLeft]) bSizeLeft=j;

 for(j=bSize+POROG_HEIGHT;j<MAX_HEIGHT;j++) if( bestSizes[j] > bestSizes[j-1] ) break;
 for(;j<MAX_HEIGHT;j++)
	 if(bestSizes[j] > bestSizes[bSizeLeft]) bSizeLeft=j;

 if( bestSizes[bSizeLeft] < 2 )
	{ sizeBig=sizeLit=bSize;porogSize=POROG_HEIGHT;}
 else
	{
	 if(bSize < bSizeLeft) {sizeLit=bSize;sizeBig=bSizeLeft;}
	 else {sizeBig=bSize;sizeLit=bSizeLeft;}
	 porogSize=MAX(POROG_HEIGHT,(sizeBig-sizeLit+1)/2);
	}

 *tipSizeBig=sizeBig ;
 *tipSizeLit=sizeLit ;
 *porSize   =porogSize;

 // можно верить ?
 if(bestWidth[bWidth] > 3) *standWidth=bWidth;
 else *standWidth=0;
 return 1;
}
////////////////
#ifdef _TEST_MULTI_FONT_
static void AnalyzeMetki(int numCluster,InfoCluster *infoC,
					     uchar *metkaGood, uchar *metkaValid )
{
 int j;

 for(j=0;j<numCluster;j++)
 {
	 if(infoC[j].let==UNRECOG_SYMBOL) continue;
     if(infoC[j].invalid ) continue;

	 if( ( metkaValid[j] & LEO_VALID_FINAL ) == 0)
		{metkaGood[j]=0; continue;}

	 // Nick 29.06.2002  - if very bad
	 if( metkaGood[j] < POROG_BAD_LINGVO )
		 // for LANG_TURKISH
	 {
		 metkaGood[j]=0;
		 continue;
	 }

	 if(  metkaValid[j] & LEO_VALID_LINGVO )
	 {
		      metkaGood[j]=MAX(metkaGood[j],POROG_GOOD_DVA);
#ifndef _ACTIVE_LINGVO_
	          infoC[j].count+=LINGVO_COUNT;  // make more
#endif
	 }

     if(infoC[j].count <= 2  && metkaGood[j] < POROG_GOOD_DVA )
		 {metkaGood[j]=0; continue;}

	 if(infoC[j].count <= 1  && (metkaGood[j] < POROG_VERYGOOD ||
		 !LetDigSymbol(infoC[j].let) )  )
		{metkaGood[j]=0; continue;}

// new - Nick
	 if( infoC[j].prob < POROG_PROB_GOOD && infoC[j].count > 10 &&
         infoC[j].prob > 175 )
		  metkaGood[j]=POROG_PROB_GOOD;
 }
}
#endif
///////////////////
// получить кластеры с нужными размерами букв
static int GetBestClusters(int minSizeBig,int maxSizeBig,
						   int minSizeLit,int maxSizeLit,
						   int porogSize,
					int bWidth ,
					InfoCluster *infoC, int *countC, int numCluster,
					uchar *metkaGood,
					int *maxC,int *max2,uint32_t *curFields)
{
int i,j;
int best;

// get maximal clusters
 for(i=0;i<256;i++)
	maxC[i]=max2[i]=-1;

 for(j=0;j<numCluster;j++)
 {
     if( metkaGood[j] < POROG_PROB_GOOD ) continue;
	 if(infoC[j].invalid ) continue;

#ifdef _TEST_MULTI_FONT_
	 // уже взяли ?
     if( infoC[j].good ) 	 continue;
#endif

	 i=infoC[j].let;
	 if(i<0 || i>=256) continue;

	 // уже есть из этого поля ?
	 if( IntersectFields(usedFields[i],infoC[j].fields ) )
		 continue;

	 if( bWidth > 0)  // bestWidth[bWidth] > 3 )
	 {
	 // test wide letters Ж Ш Щ Ю
		if( WideSymbol(i) )
		{if( infoC[j].mw < bWidth - 2 )
#ifdef _TEST_MULTI_FONT_
		continue;
#else
			{metkaGood[j]=0; continue;}
#endif
		}
		else
		{if( infoC[j].mw >= bWidth*2  )
#ifdef _TEST_MULTI_FONT_
		continue;
#else
			{metkaGood[j]=0; continue;}
#endif
		}
	 }


#ifndef _GOOD_BIG_SMALL_
	 // COMMON_HEIGHT
	 if( LetDigSymbol(i) )
     {
		 // sizeLit - porogSize
      if( infoC[j].mh < minSizeLit ) continue;
	 // don't touch ДдЙйФфЦцЩщ
      if( infoC[j].mh > maxSizeBig &&  !NotStandardSymbol(i) )
		  continue;
	 }
#else
	 // big letters & digits
	 if( LetDigSymbol(i) )
	 {
      if( NotStandardSymbol(i) )
	  {
		  if( infoC[j].mh < minSizeBig ) continue;
	  }

	  else if( BigSymbol(i) )
      {
       if( infoC[j].mh < minSizeBig ) continue;
	  // don't touch ДЙЦЩ
       if( maxSizeBig && infoC[j].mh > maxSizeBig )
        continue;
	  }
	  else if( SmallSymbol(i) )
      {
      // not too small ?
       if( infoC[j].mh <  minSizeLit ) continue;

	  // not too high ?
       if( maxSizeLit && infoC[j].mh > maxSizeLit  )
		 continue;
	   if( maxSizeBig && infoC[j].mh > maxSizeBig )
		 continue;
	  }
	  else if( BigSmallSymbol(i))
	  {
       if( infoC[j].mh < minSizeLit ) continue;
	   if( maxSizeBig && infoC[j].mh > maxSizeBig )
		 continue;
	  }
	 }
#endif

	 best=maxC[i];

	 if( best < 0 || infoC[j].count > infoC[best].count ||
		 infoC[j].count == infoC[best].count &&
		 infoC[j].prob > infoC[best].prob ||
		 ( IntersectFields(infoC[best].fields,curFields)== 0  )&&
		 ( IntersectFields(infoC[j].fields, curFields) != 0  )
		 )
		{max2[i]=maxC[i]; maxC[i]=j;}

#ifndef _GOOD_BIG_SMALL_
	 else if( max2[i] < 0 || infoC[j].count > infoC[max2[i]].count )
		 max2[i]=j;
#endif
 }


#ifndef _GOOD_BIG_SMALL_
   // test sizes
 for(i=128;i<256;i++)
 {

     if( max2[i] < 0 ) continue;
	 // don't touch РрУуФф
	 if( i== 144 || i==147 || i== 148 ||
		 i==224 || i== 227 || i==228 )
		 continue;
	 if( abs( infoC[maxC[i]].mh - infoC[max2[i]].mh)  < porogSize )
		 max2[i]=-1;

 }
#endif


 maxC[UNRECOG_SYMBOL]=max2[UNRECOG_SYMBOL]=-1;

 if( (i=maxC['(']) >= 0 && ( infoC[i].mh < minSizeBig ) )
	 maxC['(']=-1;
 if( (i=maxC[')']) >= 0 && ( infoC[i].mh < minSizeBig ) )
	 maxC[')']=-1;

 return 1;
}
//////////////////
///////////////////
#if defined (_TEST_MULTI_FONT_) && defined(_GOOD_BIG_SMALL_)
static uint16_t inNewField[256];

static int CompareFonts(int numF,FONTFIELD *fontF,
						uint32_t *oldField, int allCount,
						int sBig,int sLit,uint16_t *inFont)
{
 int i,j;
 int best=-1,bestCount;
 int commonCount;

 for(i=0;i<numF;i++)
 {

			if(sBig >0 && fontF[i].sBig > 0 &&
				abs(sBig-fontF[i].sBig) > 2 )
				  continue;
            if(sLit >0 && fontF[i].sLit > 0 &&
				abs(sLit-fontF[i].sLit) > 2 )
				  continue;
			if( sBig > 0 && fontF[i].sLit >= sBig )
				 continue;
			if( fontF[i].sBig > 0 &&  sLit >= fontF[i].sBig  )
				 continue;

			for(j=0,commonCount=0;j<256;j++)
			{
               if(inFont[j] && fontF[i].inFont[j])
				   commonCount++;
			}

			if(best < 0 || commonCount < bestCount )
			{
				best=i;
				bestCount=commonCount;
			}
 }

 // есть похожие ?
 if( best < 0 ) return -1;

 if( commonCount == 0 )
	 return best;
 if( commonCount*3 < allCount )
	 return best;
//      пополнение ?
// if( ( (oldField | fontF[best].field) == oldField ) ||
//	 ( (oldField | fontF[best].field) == fontF[best].field )
//   )
//	 return best;

 return -1;
}
////////////////////////
// получить кластеры из поля
static int GetFieldClusters( InfoCluster *infoC, int numCluster,int16_t *nClus,
							Nraster_header *rh, int numSymbol,
					      uchar *metkaGood, 	int *maxC ,
						  uint32_t *testField,
						  FONTFIELD *fontF,int numF,int inField)
{
int i,j;
int best;
int sBig=0,nBig=0;
int sLit=0,nLit=0;
int nGood=0,nOld;
int allGood=0, allBad=0;
int porogS;
uint32_t oldField[NFIELDDWORD];

// get maximal clusters

   for(j=0;j<256;j++) maxC[j]=-1;
   memset(inNewField,0,sizeof(inNewField));

   for(j=0;j<numCluster;j++)
	{
//     if( metkaGood[j] < POROG_PROB_GOOD ) continue;
	 if(infoC[j].invalid ) continue;

	  // не из этого поля ?
	 if( NotEmptyField(infoC[j].fields) && (IntersectFields(testField,infoC[j].fields) == 0) )
		 continue;
	 if( !NotEmptyField(infoC[j].fields) &&
		 metkaGood[j] < 220 )
		 continue;

	 i=infoC[j].let;
     // weak ?
	 if( metkaGood[j] < POROG_PROB_GOOD )
	 {
		 allBad++;
         if( !TestFillAll )
		   continue;
	 }
     else if( NotEmptyField(infoC[j].fields) )
	 {
	  nGood++;
	  allGood++;

	   // big letters & digits
	  if( BigSymbol(i) && !NotStandardSymbol(i)) // || BigSmall ?
      {
	  // don't touch ДЙЦЩ
	    sBig+=infoC[j].mh;
	    nBig++;
	  }

	  if( SmallSymbol(i) )
      {
		  sLit+=infoC[j].mh;
		  nLit++;
	  }

	 }


	  best=maxC[i];

	  if( best < 0 ||
		 infoC[j].count > infoC[best].count ||
		 infoC[j].count == infoC[best].count &&
		 infoC[j].prob > infoC[best].prob )
		maxC[i]=j;

	}

   if(nGood <= 0)
	 return -1;

   // сомнительное поле ?
   if( allBad > ((allGood+1)>>1)  )
	   return -1;

   if(nBig > 0) sBig=(sBig+(nBig>>1))/nBig;
   if(nLit > 0) sLit=(sLit+(nLit>>1))/nLit;

   if(nBig<=0 || nLit <=0 || sLit >= sBig )
	   porogS=POROG_HEIGHT;
   else
	   porogS=(sBig-sLit)>>1;

    // test sizes
   for( i ='0' ; i < 256 ;i++)
   {
        if( (best=maxC[i]) < 0 )
		   continue;
		if( nBig > 0 && NotStandardSymbol(i) )
		{
		  if( infoC[best].mh < sBig-porogS )
			  maxC[i]=-1;
		  continue;
		}

	    if( nBig > 0 && BigSymbol(i) )
		{
		   if( infoC[best].mh < sBig-porogS ||
               infoC[best].mh > sBig+porogS
		      )
		    maxC[i]=-1;
		   continue;
		}
		if( nLit > 0 && SmallSymbol(i)  )
		{
	     if(  infoC[best].mh < sLit-porogS  ||
			  infoC[best].mh > sLit + porogS )
           maxC[i]=-1;
		 continue;
		}
		if( BigSmallSymbol(i) )
		{
		  if( nLit > 0 && infoC[best].mh < sLit-porogS  ||
			  nBig > 0 && infoC[best].mh > sBig + porogS )
           maxC[i]=-1;
		  continue;
		}
		if(LetDigSymbol(i) &&
		   nLit > 0 && infoC[best].mh < sLit-porogS )
           maxC[i]=-1;
	}

    maxC[UNRECOG_SYMBOL]=-1;
    if( sBig > 0 && (best=maxC['(']) >= 0 && ( infoC[best].mh < sBig  ) )
	 maxC['(']=-1;
    if( sBig > 0 && (best=maxC[')']) >= 0 && ( infoC[best].mh < sBig ) )
	 maxC[')']=-1;

	for(nOld=j=nGood=allGood=0,oldField[0]=oldField[1]=0;j<256;j++)
	{
     if( (best=maxC[j]) < 0) continue;
	// уже взяли раньше ?
     if( infoC[best].good )
	 {
		 nOld++;
		 maxC[j]=-1;
		 AddFields(oldField,infoC[best].fields);
		 continue;
	 }
	 if( OneFontField  && IntersectFields(usedFields[j],testField) )
	 {
         nOld++;
		 maxC[j]=-1;
		 AddFields(oldField,infoC[best].fields);
		 continue;
	 }

	 if(infoC[best].count == 1 &&
        TestFromGoodRaster(infoC[best].first,rh,numSymbol,
				   numCluster, nClus,
				   infoC, 8) > 0  // FUTURE - test POROG !!!!
       )
	 {
		 nOld++;
		 maxC[j]=-1;
		 continue;
	 }
	 if( metkaGood[best] == 0 && infoC[best].count == 1 &&
         TestClusterGood( rh,best+1,infoC[best].first,infoC[best].count,
		                  numSymbol, 2, // porog
						  nClus, metkaGood,NULL,0) > 0
		)
		{ infoC[best].invalid=TRUE;
	      maxC[j]=-1;
		  continue;
		}

	 if( TestFillAll && infoC[best].prob < POROG_PROB_GOOD  &&
         ( (infoC[best].valid & LEO_VALID_LINGVO)==0 )
	   ) // ??
	 {	 maxC[j]=-1;
	     continue;
	 }

	 nGood++;
	 allGood+=infoC[best].count;
	 inNewField[j]=best+1;
     AddFields(oldField,infoC[best].fields);
	}

   if(OneFontField && nGood <= 0 ||
      !OneFontField && nGood <= 1 )
       goto endNoFont;

/*	// не мало новых кластеров?
	if( nGood <= 5 && nOld*2 >= nGood )
		goto endNoFont;

	// не очень мало символов ?
    if( allGood*2 <= inField && nGood <= 5 )
		goto endNoFont;
*/
	if(sBig < 0 && sLit < 0)
			goto endNoFont;

	i=CompareFonts( numF, fontF, oldField,
					nOld+nGood, sBig, sLit, inNewField);

    if(  i < 0 ) // new font !
		{
	       if( numF >= MAXFONT )
			   goto endNoFont;

		   // не мало новых - для режима в одной строке 1 шрифт?
		   if( OneFontField && nGood < 10 &&
				 (nOld*2 >= nGood || allGood*2 <= inField )
	           )
		           goto endNoFont;

		   // не мало новых - для режима в одной строке м.б. много шрифтов?
           if( !OneFontField && nGood < 10 &&
				 nOld > nGood && allGood*2 <= inField
	           )
			   goto endNoFont;

		   SetFields(fontF[numF].field,testField);
		   SetFields(fontF[numF].testField,testField);
           fontF[numF].sBig=sBig;
		   fontF[numF].sLit=sLit;
		   i=numF;
		}

   // no intersection with other clusters - return numF
   return i;

endNoFont:
   for(j=0;j<256;j++) maxC[j]=-1;
   return -1;
}
#endif
//////////////////

int FindBestClusters(int numSymbol,int numCluster,Nraster_header *rh,
					 int16_t *nClus,uchar *metka,uchar *metkaValid,
					 int maxOutFonts,uint32_t *ffFields)
{
 int i,j;
 int bSize;  // tipical size of big letter
 int lSize;  // tipical size of small letter
 int bWidth; // most standard width
 int porogSize; // distance big-small letters
 InfoCluster *infoCluster;

 int minBig,maxBig; // minimal,maximal BIG letters
 int minLit,maxLit; // minimal,maximal small letters
 int countFont=0;   // how many fonts
 int addToFont=0;
 uint32_t allField[NFIELDDWORD];   // какие поля в шрифте
 uint32_t curFields[NFIELDDWORD];

#ifdef _TEST_MULTI_FONT_
 int curFont;
#endif

 if( ffFields ) memset(ffFields,0,maxOutFonts*sizeof(uint32_t)*NFIELDDWORD);

 if(numCluster < MAXWEICLUS ) infoCluster=infoClusterStat;
 else
 {
	infoCluster=malloc(numCluster*sizeof(InfoCluster));
	if(!infoCluster)
	{infoCluster=infoClusterStat;
	 numCluster=MAXWEICLUS;
	}
 }

    memset(metka,0,numCluster);
    memset(metkaValid,0,numCluster);
    memset(usedFields,0,256*sizeof(uint32_t)*NFIELDDWORD);

	GetClusterStatistic(numSymbol,numCluster,rh,nClus,infoCluster,countCluster,
		metka,metkaValid,TRUE);

// for multifont test
#ifdef _TEST_MULTI_FONT_
  AnalyzeMetki(numCluster,infoCluster,metka,metkaValid);
  AnalizeSizes(infoCluster, numCluster,metka,1);

  for(curFont=0;;curFont++)
  {
   if( GetNextFont(&bSize,&lSize,&porogSize,
				   &minBig,&maxBig,
				   &minLit,&maxLit,curFields) <= 0 )
				   break;

   if(curFont == 0 )  // as in old one-font version
	   CorrectSizes(&minBig,&maxBig,&minLit,&maxLit,bSize,lSize,porogSize);

//   bWidth=GetTipicalWidth(minBig,maxBig,minLit,maxLit,
//	   infoCluster, numCluster,);
   bWidth=-1;
   GetBestClusters(minBig,maxBig,minLit,maxLit,porogSize,
				   bWidth ,infoCluster, countCluster, numCluster,
				   metka, maxCluster, max2Cluster,curFields);
   if(curFont != 0)
	   goto fillGood;

#else
  AnalyzeInfo(numCluster,infoCluster,
		metka,metkaValid,&bSize,&lSize,&porogSize,&bWidth);

  minBig=bSize-porogSize;
  maxBig=bSize+porogSize;
  minLit=lSize-porogSize;
  maxLit=lSize+porogSize;
  GetBestClusters(bSize-porogSize,bSize+porogSize,
		            lSize-porogSize,lSize+porogSize,porogSize,
					bWidth ,infoCluster, countCluster, numCluster,
					metka, maxCluster, max2Cluster,0);
#endif
	{
     int sizeDigit,j;

	 // SECOND PASS	for digits
     // now test only digits
	 for(i='0',j=sizeDigit=0;i<='9';i++)
	 {
	  if( maxCluster[i] < 0 ) continue;
	  maxCluster[i]=BestBySize(i,numCluster,infoCluster,bSize,porogSize,metka);
	  if( maxCluster[i] < 0 ) continue;
	  sizeDigit+=infoCluster[maxCluster[i]].mh;
	  j++;
	 }
	 if( j>1 )
	 {
	  sizeDigit=(sizeDigit+(j>>1))/j;
	  if( sizeDigit != bSize )
	   for(i='0';i<='9';i++)
		maxCluster[i]=BestBySize(i,numCluster,infoCluster,sizeDigit,porogSize,metka);
	 }
	} // end  SECOND PASS	for digits

	// test 0
	if( (i=maxCluster['0']) >= 0 && (metkaValid[i]&LEO_VALID_DIGIT))
	{
#ifndef _GOOD_BIG_SMALL_
      int from0;
      from0=GetMaxFrom0( numSymbol, rh,i,nClus,
				  bSize-porogSize,numCluster,infoCluster,
				  metka,metkaValid);
	  if(max2Cluster[142]<0) max2Cluster[142]=from0;
#endif

	  if(maxCluster[142]>=0 )
	    maxCluster[142]=TestO0b6(numSymbol,rh,i,maxCluster[142],
				  nClus,bSize-porogSize,numCluster,infoCluster,
				  metka,142);
#ifndef _GOOD_BIG_SMALL_
	  if(max2Cluster[142]>=0)
	    max2Cluster[142]=TestO0b6(numSymbol,rh,i,max2Cluster[142],
				  nClus,bSize-porogSize,numCluster,infoCluster,
				  metka,142);
#endif
	 }

	// test 6 & b
	if( (i=maxCluster['6']) >= 0 && (metkaValid[i]&LEO_VALID_DIGIT))
	{
	  if(maxCluster[161]>=0 )
	    maxCluster[161]=TestO0b6(numSymbol,rh,i,maxCluster[161],
				  nClus,bSize-porogSize,numCluster,infoCluster,
				  metka,161);
	}

/*	// test 3 & З (Ze)
	if( (i=maxCluster['3']) >= 0 && (metkaValid[i]&LEO_VALID_DIGIT))
	{
	  if(maxCluster[135]>=0 ) // ze
	    maxCluster[135]=TestO0b6(numSymbol,rh,i,maxCluster[135],
				  nClus,bSize-porogSize,numCluster,infoCluster,
				  metka,135);
	}
*/

fillGood:

 if (TestFillAll)
 {
    int porogMin,porogMax;
    for(i='0';i< 256;i++)
	{
      if(!LetDigSymbol(i) )
		  continue;
	  if( countCluster[i] <= 0 || maxCluster[i] >= 0 )
		  continue;

#ifdef _GOOD_BIG_SMALL_
	  if( BigSymbol(i) )
		{ porogMin = minBig;porogMax = maxBig; }
	  else if( SmallSymbol(i) )
		{ porogMin = minLit;porogMax = maxLit; }
	  else
#endif
		{ porogMin = minLit;porogMax = maxBig; }

      maxCluster[i]=FindBest(i,numCluster,infoCluster,porogMin,porogMax,
		                metka,metkaValid,-1,porogSize,rh,numSymbol,nClus);
	}
 }    // TEST_FILL_ALL

#ifdef _TEST_ITALIC_SERIF_
 {
  TestItSeBoldCluster(numCluster, infoCluster,
				 bSize,lSize, maxCluster, NULL,NULL);
 }
#endif

	// fill results addToFont- count added symbols
	if(countFont < MAXFONT )
		memset(fontField+countFont,0,sizeof(FONTFIELD));
	ClearFields(allField);
	for(i=0,addToFont=0;i<256;i++)
	{
	 if( (j=maxCluster[i]) >= 0 )
	 {

       if( TryRename(j,numSymbol,rh,nClus, numCluster,infoCluster ) )
		   continue;

	   // проверка не подтвержденных по словарю
	   if( TestFillAll == 0 &&
		   ( infoCluster[j].valid & LEO_VALID_LINGVO ) == 0 &&
		   infoCluster[j].count <= 3 &&  // was 2
           TestClusterGood( rh,j+1,infoCluster[j].first,infoCluster[j].count,
		                  numSymbol, 0, // porog
						  nClus, metka,metkaValid,0) > 0
		)
		{ infoCluster[j].invalid=TRUE;
		  continue;
		}

       AddFields(usedFields[i],infoCluster[maxCluster[i]].fields);
	   AddFields(allField     ,infoCluster[maxCluster[i]].fields);

	   addToFont++;  // mark - new font
	   if( countFont < MAXFONT)
		   fontField[countFont].inFont[i]=maxCluster[i]+1;
	   infoCluster[maxCluster[i]].good=TRUE;
	 }
#ifndef _GOOD_BIG_SMALL_
	 if( max2Cluster[i] >= 0 ) infoCluster[max2Cluster[i]].good=TRUE;
#endif
	}

	// достойны называться шрифтом ?
    if( addToFont < 10 && addToFont*2 < numCluster)
	{
		int ii;
		for(ii=0;ii<countFont && ii<MAXFONT;ii++)
		{
			if( abs(fontField[ii].sBig - bSize) < 2 &&
				abs(fontField[ii].sLit - lSize) < 2
			)
            break;
		}
		if(ii<countFont)
           addToFont=1; // чтобы ниже убрать метки !
	}

	if( !OneFontField && addToFont > 0 && addToFont < 7 &&
		addToFont*2 < numCluster )
	{
      for(i=0;i<256;i++)
	    if( (j=maxCluster[i]) >= 0 )
			infoCluster[j].good=0;
      addToFont=0;
	}

	if( addToFont > 0 )
	{
		if( countFont < MAXFONT)
		{
          fontField[countFont].sBig=bSize;
		  fontField[countFont].sLit=lSize;
          SetFields(fontField[countFont].field,allField);
		  ClearFields(fontField[countFont].testField);  // no special field
		}
		countFont++;   // if addToFont > 5 ?

			  // какие размеры остались ?
        if(!OneFontField)
          AnalizeSizes(infoCluster, numCluster,metka,0);
	}

#ifdef _TEST_MULTI_FONT_
  }  // end curFont
#endif

// 10.12.98
#if defined(_TEST_MULTI_FONT_) && defined(_GOOD_BIG_SMALL_)
   {
     uint32_t fifi[NFIELDDWORD];
	 int fld,newFont,inField,best;
	 int32_t FieldCount[MAXFIELD];

	 memset(FieldCount,0,sizeof(FieldCount));
	 for(fld=0;fld<numSymbol;fld++)
		 if( rh[fld].nField > 0 &&  rh[fld].nField <= MAXFIELD )
          FieldCount[rh[fld].nField-1]++;

	 for(fld=1;fld<=MAXFIELD;fld++)
	 {

		if( (inField=FieldCount[fld-1]) <= 1 )
			 continue;
		MakeDWORDField(fld,fifi);

		if( countFont < MAXFONT )
			memset(fontField+countFont,0,sizeof(FONTFIELD));
		if( (newFont=GetFieldClusters( infoCluster ,  numCluster,nClus,rh,numSymbol,
					      metka,maxCluster, fifi,fontField,countFont,inField)) < 0 )
						  continue;

		if( !OneFontField && newFont < MAXFONT )
		{
			int TestAddFontGood(int numCluster, InfoCluster *infoC,
				 int sBig,int sLit,int *maxC,int fromAll,uint32_t *fif);
			int add;
			add = TestAddFontGood( numCluster, infoCluster,
				  fontField[newFont].sBig,
				  fontField[newFont].sLit,
				  maxCluster, 1, fifi );
		}

#ifdef _TEST_ITALIC_SERIF_
		if( newFont < MAXFONT )
		{

         TestItSeBoldCluster(numCluster, infoCluster,
				 fontField[newFont].sBig,fontField[newFont].sLit,
				 maxCluster, fifi,
				 newFont < countFont? fontField+newFont:NULL);
		}
#endif

	// fill results
		for(i=0,addToFont=0;i<256;i++)
		{
		 if( (best=maxCluster[i]) < 0 )   continue;
		 // проверить похожие буквы
		 if( TryRename(best,numSymbol,rh,nClus, numCluster,infoCluster ) )
		   continue;
		 // проверка не подтвержденных по словарю
	     if( TestFillAll == 0 &&
		    ( infoCluster[best].valid & LEO_VALID_LINGVO ) == 0 &&
		    infoCluster[best].count <= 3 &&
            TestClusterGood( rh,best+1,infoCluster[best].first,infoCluster[best].count,
		                  numSymbol, 0, // porog
						  nClus, metka,metkaValid,0) > 0
		    )
		 { infoCluster[best].invalid=TRUE;
		   continue;
		 }
		 // пополнить фонт ?
		 if( newFont < countFont && newFont < MAXFONT &&
			 fontField[newFont].inFont[i] )
		 {
           if(infoCluster[best].count <= infoCluster[fontField[newFont].inFont[i]-1].count )
			   continue;
		   // else - replace old
		   infoCluster[fontField[newFont].inFont[i]-1].good=0;
		 }

		 addToFont++;
		 if(newFont < MAXFONT)
		 {
			 fontField[newFont].inFont[i]=best+1;
			 AddFields(fontField[newFont].field,infoCluster[best].fields);
		 }
		 AddFields(usedFields[i],infoCluster[best].fields);
	     infoCluster[best].good=TRUE;
		}

		if( newFont == countFont && addToFont > 0)
		 countFont++;
	 }
	}

    TestIntersectFields( MIN(countFont,MAXFONT),fontField,
						 infoCluster);

#endif
	{
      int let='i';
      AddSpecialName( let, numCluster,infoCluster, metka );
	}

#ifdef _TEST_STAY_
	// проверка - много ли осталось хорошего за бортом, добавка
	if( !TestFillAll )
	{
	 int TestStayGood(int numCluster, int numSymbol,InfoCluster *infoC,
				 FONTFIELD *fontField);
	 i=TestStayGood(numCluster, numSymbol,infoCluster,
		   countFont < MAXFONT? fontField+countFont:NULL);
	 if( i == 1)
		 countFont++;
	}
#endif

#ifdef _TEST_COLUMNS_
	// 25.10.2000
	{
	 int TectTablColumn(InfoCluster *infoCluster,int numCluster,int i,Nraster_header *rh,
		 int numAll,int16_t *nClus);
	 for(i=0;i<numCluster;i++)
	 {
        if( !infoCluster[i].good )
		  infoCluster[i].good = TectTablColumn(infoCluster,numCluster,i,rh,numSymbol,nClus);
	 }
	}
#endif

	// output - какие поля - к какому шрифту
	if( ffFields )
	{
		for(i=0;i<maxOutFonts && i < countFont && i < MAXFONT;i++)
		    SetFields((ffFields+i*NFIELDDWORD),fontField[i].field);
	}

	// fill output
	for(i=0;i<numCluster;i++)
	{
		if( infoCluster[i].good==FALSE )
			metkaValid[i] &= ~METKA_VALID;
		else
			metkaValid[i] |= METKA_VALID;
		metka[i]=MAX(metka[i],infoCluster[i].prob);
	}

	if( infoCluster!=infoClusterStat) free(infoCluster);

#ifdef _OUT_COUNT_FONT_
  {
	  FILE *fin=fopen(".\\tmp\\name_tif.tmp","rt");
	  char name[256];
	  int  len,jj,i1;
	  uint32_t ii;

	  if(fin != NULL)
	  {
		  memset(name,0,256);
          len=fread(name,1,256,fin);
		  fclose(fin);
		  if(countFont <= 1 )
		      fin=fopen(".\\tmp\\nick1.tmp","at");
		  else
			  fin=fopen(".\\tmp\\nick2.tmp","at");
		  fprintf(fin,"%s %d\n",name,countFont);

		  for(i=0;i<countFont;i++)
		  {
		   for(i1=0;i1<NFIELDDWORD;i1++)
		   {
            for(ii=1,jj=1;jj<=32;ii<<=1,jj++)
				if(ii & fontField[i][i1].field)
					fprintf(fin," %d",i1*32+ii);
		   }
           fprintf(fin,"\n");
		  }
		  fclose(fin);

#ifdef _SAVE_CTB_
          for(--len;len>=0;len--)
			  if( name[len]=='\\' || name[len] == ':')
				  break;
          CTB_copy(name+len+1,".\\tmp\\ct999999");
#endif
	  }

  }
#endif

  	return countFont;
}
/////////////////////////
int GetProbValid(int numSymbol,int numCluster,Nraster_header *rh,int16_t *nClus,
				 uchar *metkaGood,uchar *metkaValid)
{
int i,curClus;

 memset(metkaGood,1,numCluster); // to be not 0 !
 memset(metkaValid,METKA_VALID,numCluster);
 for(i=0;i<numSymbol;i++)
 {
    curClus=nClus[i]-1;
	if(curClus < 0 || curClus >= numCluster ) continue;
	metkaGood[curClus]=MAX(metkaGood[curClus],rh[i].prob);
	metkaValid[curClus] |= rh[i].valid;
 }
 return 0;
}
////////////////////////////////////////
//
// for not-single font version
static int MultiAnalyzeInfo(int numCluster,InfoCluster *infoC,
					   int *countC,int *maxC,int *max2,
					   uchar *metkaGood,int *minSizeBig,
					   uchar *metkaValid)
{
 int j;
 //
 // now test sizes - if only one font !
 //
 memset( bestSizes,0,sizeof(bestSizes));
 memset( bestWidth,0,sizeof(bestWidth));

 for(j=0;j<numCluster;j++)
 {
	 if(infoC[j].let==UNRECOG_SYMBOL)
		 {metkaGood[j]=0; continue;}
     if(infoC[j].invalid )
		 {metkaGood[j]=0; continue;}

	 if( ( metkaValid[j] & LEO_VALID_FINAL ) == 0)
		{metkaGood[j]=0; continue;}


	 if(  metkaValid[j] & LEO_VALID_LINGVO )
		   {
 		      metkaGood[j]=MAX(metkaGood[j],POROG_GOOD_DVA); // 255?
#ifndef _ACTIVE_LINGVO_
	          infoC[j].count+=LINGVO_COUNT;  // make more
#endif
		   }


	 // don't take suspicious
     if( metkaGood[j] < POROG_PROB_GOOD )
		 {metkaGood[j]=0; continue;}

#ifdef _SPEC_DVA_
	 if( infoC[j].count <= 2 && metkaGood[j] < POROG_GOOD_DVA )
		 {metkaGood[j]=0; continue;}
#endif

	 if(infoC[j].count <= 1  && (metkaGood[j] < POROG_VERYGOOD ||
		 infoC[j].let < '0' || infoC[j].let>'9' && infoC[j].let <'@' ||
		 infoC[j].let>'Z' && infoC[j].let <'a' ||
		 infoC[j].let>'z' && infoC[j].let < 128 ))
		{metkaGood[j]=0; continue;}
     if( infoC[j].mh < MAX_HEIGHT) bestSizes[infoC[j].mh]++;
	 if( infoC[j].mw < MAX_WIDTH) bestWidth[infoC[j].mw]++;
 }

 return 1;
}
//////////////////
int MultiFindBestClusters(int numSymbol,int numCluster,Nraster_header *rh,
					 int16_t *nClus,uchar *metka,uchar *metkaValid)
{
int i;
int bSize;  // minimal size of big letter
InfoCluster *infoCluster;

 memset(metka,0,numCluster);
 memset(metkaValid,0,numCluster);

 if(numCluster < MAXWEICLUS ) infoCluster=infoClusterStat;
 else
 {
	infoCluster=malloc(numCluster*sizeof(InfoCluster));
	if(!infoCluster)
	{infoCluster=infoClusterStat;
	 numCluster=MAXWEICLUS;
	}
 }

	GetClusterStatistic(numSymbol,numCluster,rh,nClus,infoCluster,countCluster,
		metka,metkaValid,TRUE);

    MultiAnalyzeInfo(numCluster,infoCluster,countCluster,maxCluster,max2Cluster,
		metka,&bSize,metkaValid);


	// fill output
	for(i=0;i<numCluster;i++)
	{
		if( metka[i] == 0 )
			metkaValid[i] &= ~METKA_VALID;
		else
			metkaValid[i] |= METKA_VALID;
		metka[i]=MAX(metka[i],infoCluster[i].prob);
	}

	if( infoCluster!=infoClusterStat) free(infoCluster);

	return 0;
}
/////////////////////////
//
typedef struct tagKuchka
{
	int16_t start;
	int16_t end;
	int count;
	uint32_t field[NFIELDDWORD];
} KUCHKA;

#ifdef _TEST_MULTI_FONT_
static int GetKuchki(int *bSizes,uint32_t *fifi,int size, KUCHKA *ku,
					 int maxAlt,int minCount)
{
int numBest;
int i,j;
int isKuchka;
int start,count;
uint32_t fields[NFIELDDWORD]={0,0};

 for(i=0,count=numBest=0,isKuchka=0;i<size;i++)
 {
	 if( bSizes[i] == 0)
	 {
       if(isKuchka==0) continue;
	   if(count < minCount)
	   {
		   isKuchka=0;
		   count=0;
		   ClearFields(fields);
		   continue;
	   }

	   for(j=0;j<numBest;j++)
	    if(count > ku[j].count ) break;

       if(j < maxAlt)
	   {
        if(numBest < maxAlt) numBest++;
	    memmove(ku+j+1,ku+j,(numBest-j-1)*sizeof(KUCHKA));
        ku[j].count=count;
	    ku[j].start=start;
	    ku[j].end=i;
	    SetFields(ku[j].field,fields);
	   }

	   isKuchka=0;
       count=0;
	   ClearFields(fields);
	   continue;
	 }

	 if(isKuchka==0)
		 start=i;
	 isKuchka=1;
     count+=bSizes[i];
	 AddFields(fields,(fifi+i*NFIELDDWORD));
 }

 if(isKuchka)
 {
	   for(j=0;j<numBest;j++)
	    if(count > ku[j].count ) break;

       if(j < maxAlt)
	   {
        if(numBest < maxAlt) numBest++;
	    memmove(ku+j+1,ku+j,(numBest-j-1)*sizeof(KUCHKA));
        ku[j].count=count;
	    ku[j].start=start;
	    ku[j].end=i;
	    SetFields(ku[j].field,fields);
	   }
 }

 return numBest;
}
//////////////
static int bigSizes[WR_MAX_HEIGHT];
static int litSizes[WR_MAX_HEIGHT];
static uint32_t bigFields[WR_MAX_HEIGHT][NFIELDDWORD];
static uint32_t litFields[WR_MAX_HEIGHT][NFIELDDWORD];

static KUCHKA kuBig[MAXFONT];
static KUCHKA kuLit[MAXFONT];

static int numBig;        // num groups
static int numLit;        // num groups
static int allBig,allLit; // all letters
static int goodBig;       // large groups
static int goodLit;       // large groups

// now - very simple
static int GetAccord(int size,int num,KUCHKA *ku)
{
 int i;

 for(i=0;i<num;i++)
	 if( size >= ku[i].start && size < ku[i].end ) return i;

 return -1;
}
///////////////
// now - very simple
static int GetMiddleValue(int *sizes,KUCHKA *ku)
{
 int i,best=ku->start;

 for(i=ku->start+1;i<ku->end;i++)
	 if( sizes[i] > sizes[best] ) best=i;

 return best;
}
//////////////
/*****************/
// function, sizeof
// return best point
// in valProg - value of progib
static int ProgibFun(int *hhh,int GreyLev,int *valProg)
{
 int i;
 int prog=0,leftprog=0;
 int lefth=-1;
 int best=-1,prommin = -1;
 uchar spusk = 0;

	// not need start
 for(i=1;i<GreyLev;i++)
  {
   if(hhh[i] >= hhh[i-1])
     {if(spusk == 0 )continue;
      spusk = 0;                       /* local minimum point */
      if(lefth == -1 )continue;

	  if(hhh[lefth]-hhh[i-1] > leftprog )  /* kandidat for best point */
	  {prommin = i-1;
	   leftprog = hhh[lefth]-hhh[i-1];   /* progib from left position */
	  }
     }
   else
     {
      if(spusk ) continue;
      spusk = 0xFF;                /* local maximum */
      if(lefth == -1 ) {lefth = i-1;continue;}

      if(hhh[i-1] >= hhh[lefth])         /* right higher then left */
	  {
	   if(prommin > 0)
	   {
	    prog = leftprog;
	    best = prommin;
	   }

	   lefth = i-1;                /* global maximum left */
	   prommin=-1;      //  our point stay in left part!
	   continue;
	  }

     if( prommin!=-1 && hhh[i-1] - hhh[prommin] >= prog )  /* progib from right */
	 {
	   prog = hhh[i-1]-hhh[prommin];
	   best = prommin;
	 }
   }   // end if
  }

 if(prommin > 0 && lefth != -1 && leftprog >= prog && hhh[GreyLev-1] - hhh[prommin] > prog )
     {
	   prog = MIN(leftprog,hhh[GreyLev-1]-hhh[prommin]);
	   best = prommin;
     }

 *valProg=prog;
 return(best);
}
////////////////////
static int TestAcc(int testNum,int num,KUCHKA *ku,int *sizes,uint32_t *fields)
{
 int i,j;
 int progib,valProg;
 int countl,countr;
 int fir,las;

 progib=ProgibFun(sizes+ku[testNum].start,ku[testNum].end-ku[testNum].start,&valProg);
 // exist ?
 if(progib <= 0)
	 return num;
 // not too small ?
 if(sizes[progib] >= valProg)
	 return num;
 progib+=ku[testNum].start;

 // study left part
 countl=sizes[progib];
 for(i=progib-1;i>=ku[testNum].start;i--)
     countl+= sizes[i];

 // study right part
 countr=sizes[progib];
 for(i=progib+1;i<ku[testNum].end;i++)
     countr+= sizes[i];

 fir=ku[testNum].start;
 las=ku[testNum].end;

 memcpy(ku+testNum,ku+testNum+1,(num-testNum-1)*sizeof(KUCHKA));
 num--;

 for(j=0;j<num;j++)
	    if(countl > ku[j].count) break;
 if(j < MAXFONT)
 {
	 if(num < MAXFONT) num++;
	 memmove(ku+j+1,ku+j,(num-j-1)*sizeof(KUCHKA));
     ku[j].count=countl;
	 ku[j].start=fir;
	 ku[j].end  =progib+1;
	 ClearFields(ku[j].field);

     for(fir=ku[j].start;fir<ku[j].end;fir++)
           AddFields(ku[j].field , (fields+fir*NFIELDDWORD));
 }

 for(j=0;j<num;j++)
	    if(countr > ku[j].count) break;
 if(j < MAXFONT)
 {
	 if(num < MAXFONT) num++;
	 memmove(ku+j+1,ku+j,(num-j-1)*sizeof(KUCHKA));
     ku[j].count=countr;
	 ku[j].start=progib;
	 ku[j].end  =las;
	 ClearFields(ku[j].field);

     for(fir=ku[j].start;fir<ku[j].end;fir++)
           AddFields(ku[j].field, (fields+fir*NFIELDDWORD));
 }

 return num;
}
//////////////
// сколько кучек и какие заслуживают внимания ?
static int GetCountGood(int num,KUCHKA *ku,int all)
{
int i;
int    count;
uint32_t usedFields[NFIELDDWORD];
KUCHKA tmpKuch;

    if(num <= 0) return 0;
	count=1;
	SetFields(usedFields,ku[0].field);
    for(i=1;i<num;i++)
	{
		if( OneFontField && IntersectFields(ku[i].field,usedFields) )
			continue;
		if( count < i )
		{
			tmpKuch=ku[i];
			memmove(ku+count+1,ku+count,(i-count)*sizeof(KUCHKA));
			ku[count]=tmpKuch;
		}
		count++;
		AddFields(usedFields , ku[count].field);
	}
	return count;
}
///////////////
static int AnalizeSizes(InfoCluster *infoC,int numClus,
						uchar *metka, int fir)
{
 int i,j;
 int hei;
 int accord[MAXFONT],acc;

 numBig=numLit=0;
 memset(bigSizes,0,WR_MAX_HEIGHT*sizeof(int));
 memset(litSizes,0,WR_MAX_HEIGHT*sizeof(int));
 memset(bigFields,0,WR_MAX_HEIGHT*sizeof(uint32_t)*NFIELDDWORD);
 memset(litFields,0,WR_MAX_HEIGHT*sizeof(uint32_t)*NFIELDDWORD);

 if( fir) allBig=allLit=0;

 for(j=0;j<numClus;j++)
 {
	 if( infoC[j].good )
		 continue;
	 if( metka[j] == 0 || infoC[j].prob < POROG_PROB_GOOD )
		 continue;
	 hei=MIN(infoC[j].mh, WR_MAX_HEIGHT -1);

	 i=infoC[j].let;

	 	 // don't touch ДдЙйФфЦцЩщ
     if(  NotStandardSymbol(i) )
		 continue;

	 	 // don't touch "бру" - or add to big ?
	 if( BigSmallSymbol(i) )
		 continue;

	 if( BigSymbol(i) )
	 { bigSizes[hei]+=infoC[j].count;
	   AddFields(bigFields[hei],infoC[j].fields);
	   if(fir) allBig+=infoC[j].count;
	 }
	 else  if( SmallSymbol (i) )
	 { litSizes[hei]+=infoC[j].count;
	   AddFields(litFields[hei],infoC[j].fields);
	   if(fir) allLit+=infoC[j].count;
	 }
 }


 numBig = GetKuchki(bigSizes,&bigFields[0][0],WR_MAX_HEIGHT,kuBig,
	 MAXFONT,OneFontField?2:5);   // 1 ?
 numLit = GetKuchki(litSizes,&litFields[0][0],WR_MAX_HEIGHT,kuLit,
					 MAXFONT,OneFontField?2:5);   // 1 ?

 for(i=0;i<numBig;i++)
	 if(kuBig[i].count*10 < allBig) break;
 goodBig=i;

 // test kuchki
 for(i=0;i<goodBig;i++)
 {
	 j=GetMiddleValue(bigSizes,kuBig+i);
	 accord[i]=acc=GetAccord((j*3)>>2,numLit,kuLit);
	 if(acc < 0) continue;
	 for(j=0;j<i;j++)
		if(accord[j]==acc) numLit=TestAcc(acc,numLit,kuLit,litSizes,&litFields[0][0]);
 }
 for(i=0;i<numLit;i++)
	 if(kuLit[i].count*10 < allLit) break;
 goodLit=i;

 for(i=0;i<goodLit;i++)
 {
	 j=GetMiddleValue(litSizes,kuLit+i);
	 accord[i]=acc=GetAccord((j<<2)/3,numBig,kuBig);
	 if(acc < 0) continue;
	 for(j=0;j<i;j++)
		if(accord[j]==acc) numBig=TestAcc(acc,numBig,kuBig,bigSizes,&bigFields[0][0]);
 }

 // new goodBig ?
 goodBig=GetCountGood(numBig,kuBig,allBig);
 goodLit=GetCountGood(numLit,kuLit,allLit);

 return MAX(goodBig,goodLit);
}
//////////////
static int GetNextFont(int *bSize,int *lSize,int *pSize,
					   int *minBig,int *maxBig,
					   int *minLit,int *maxLit,
					   uint32_t *fields)
{
int sizeBig,sizeLit;
int i;

 if(goodBig <= 0 && goodLit <= 0 )
	 return -1;

 if(goodBig >= 1 && goodLit >= 1 )
 {
   sizeBig=GetMiddleValue(bigSizes,kuBig);
   sizeLit=GetMiddleValue(litSizes,kuLit);

   if(sizeLit < sizeBig - POROG_HEIGHT )
   {
    *bSize=sizeBig;
    *lSize=sizeLit;
	*minBig=kuBig[0].start;
	*maxBig=kuBig[0].end;
	*minLit=kuLit[0].start;
	*maxLit=kuLit[0].end;
	SetFields(fields, kuBig[0].field);
	AddFields(fields, kuLit[0].field);
	memcpy(kuBig,kuBig+1,(numBig-1)*sizeof(KUCHKA));
	memcpy(kuLit,kuLit+1,(numLit-1)*sizeof(KUCHKA));
	*pSize=(sizeBig-sizeLit+1)>>1;
	numBig--;
	numLit--;
	goodBig--;
	goodLit--;
	return 2;
   }
 }

 if(numLit <= 0 || numBig>0 && kuBig[0].count >= kuLit[0].count )
 {
	 sizeBig=GetMiddleValue(bigSizes,kuBig);
	 sizeLit=(sizeBig*3)>>2;
	 i=GetAccord(sizeLit,numLit,kuLit);
     if( i >= 0 )  // есть соответствие
	 {
        *minBig=kuBig[0].start;
	    *maxBig=kuBig[0].end;
	    *minLit=kuLit[i].start;
	    *maxLit=kuLit[i].end;
		SetFields(fields, kuBig[0].field);
		AddFields(fields, kuLit[i].field);

		sizeLit=GetMiddleValue(litSizes,kuLit+i);
        *bSize=sizeBig;
        *lSize=sizeLit;

		memcpy(kuBig,kuBig+1,(numBig-1)*sizeof(KUCHKA));
		memcpy(kuLit+i,kuLit+i+1,(numLit-i-1)*sizeof(KUCHKA));
		*pSize=(sizeBig-sizeLit+1)>>1;
		numBig--;
		numLit--;

		goodBig--;
		if( i < goodLit) goodLit--;
		return 2;
	 }

	 // нет соответствия
	 // искать по положениям рамок - FUTURE !!!
	    *bSize=sizeBig;
        *lSize=sizeLit;
		*pSize=(sizeBig-sizeLit+1)>>1;
		*minBig=kuBig[0].start;
	    *maxBig=kuBig[0].end;
		SetFields(fields,kuBig[0].field) ;
	    *minLit=(*lSize)-(*pSize);
	    *maxLit=(*lSize)+(*pSize);

	 	memcpy(kuBig,kuBig+1,(numBig-1)*sizeof(KUCHKA));
		numBig--;
		goodBig--;
		return 1;
 }

//  if(numBig <= 0 || numLit>0 && countLit[0] > countBig[0] )

 	 sizeLit=GetMiddleValue(litSizes,kuLit);
	 sizeBig=(sizeLit<<2)/3;
	 i=GetAccord(sizeBig,numBig,kuBig);
     if( i >= 0 )  // есть соответствие
	 {
		sizeBig=GetMiddleValue(bigSizes,kuBig+i);

		*bSize=sizeBig;
        *lSize=sizeLit;
		*minBig=kuBig[i].start;
	    *maxBig=kuBig[i].end;
	    *minLit=kuLit[0].start;
	    *maxLit=kuLit[0].end;
		SetFields(fields, kuBig[i].field);
		AddFields(fields, kuLit[0].field);

		memcpy(kuBig+i,kuBig+i+1,(numBig-i-1)*sizeof(KUCHKA));
		memcpy(kuLit,kuLit+1,(numLit-1)*sizeof(KUCHKA));
		*pSize=(sizeBig-sizeLit+1)>>1;
		numBig--;
		numLit--;
		goodLit--;
		if( i < goodBig) goodBig--;
		return 2;
	 }

	 // нет соответствия
	    *bSize=sizeBig;
        *lSize=sizeLit;
		*pSize=(sizeBig-sizeLit+1)>>1;
		*minLit= kuLit[0].start;
	    *maxLit= kuLit[0].end;
		SetFields(fields,kuLit[0].field);
	    *minBig=(*bSize)-(*pSize);
	    *maxBig=(*bSize)+(*pSize);

	 	memcpy(kuLit,kuLit+1,(numLit-1)*sizeof(KUCHKA));
		numLit--;
		goodLit--;
		return 1;

//	{ sizeBig=sizeLit=bSize;porogSize=POROG_HEIGHT;}
}
/////////////
static void CorrectSizes(int *minBig,int *maxBig,int *minLit,int *maxLit,int bSize,int lSize,int porogSize)
{
	int i;

	*minBig=bSize-porogSize;
	*maxBig=bSize+porogSize;
	*minLit=lSize-porogSize;
	*maxLit=lSize+porogSize;
	for(i=0;i<goodBig;i++)
	{
		if(bSize > kuBig[i].end && *minBig < kuBig[i].end )
			*minBig=kuBig[i].end;
		else if(bSize < kuBig[i].start && *maxBig > kuBig[i].start)
			*maxBig=kuBig[i].start;
	}
	for(i=0;i<goodLit;i++)
	{
		if(lSize > kuLit[i].end && *minLit < kuLit[i].end)
			*minLit=kuLit[i].end;
		else if(lSize < kuLit[i].start && *maxLit > kuLit[i].start)
			*maxLit=kuLit[i].start;
	}
}
////////////
#endif
