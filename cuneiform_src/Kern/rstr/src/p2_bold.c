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

//#define _OLD_TABL_VERSION_

#define _TEST_THIN_
#define _NEW_SMALL_

//#define _PRINT_BOLD_

#define POROG_SOSED_MAX 11    //  standard = 11
#define POROG_ONELINE   5     //  old was 8, last 6

#define MIN_HEIGHT      12
#define MIN_SIZE_LINE   16

#define P2_BOLD         2
#define P2_LIGHT        1

#include <stdlib.h>
#include <string.h>
#include <memory.h>


#include "struct.h"
#include "status.h"
#include "func.h"
#include "lang.h"
#include "cstr.h"
#include "fon.h"
#include "std.h"
//#include "linear.h"
#include "p2libr.h"
#include "ligas.h"	// 08.09.2000 E.P.
#include "minmax.h"
/////////////////
void   clear_cells(void);
void p2_TextWord(CSTR_rast  c,CSTR_rast stop, char *intxt,Bool ansi);
extern int16_t  line_number;

static int FindBounds(int *val,int size);
static uchar SetBoldByTable(int nThick, int midThick,
							uchar colBold, int thick,
							int minVal, int maxVal);

#ifdef _PRINT_BOLD_
 static FILE *fbold=NULL;
 static int nFile=0;
#endif
///////////////////////
#define MIN_STAT_PAGE 5  // минимальная длина слова для
                         // оценки статистики страницы
#define POROG_INTERLINE 4  // если разница жирностей строк
                           // меньше порога, установить одинаково
#define POROG_ONECOL  7
#define POROG_ONECELL 9

#define MAXTHICK 256
#define MAXHEI   128
#define ADD_THIN 4
typedef struct tagThick {
        CSTR_rast first;
        CSTR_rast last;
        int16_t height;
        int16_t thick;
        uchar strih;
        uchar bold;
        uchar nLet;
        uchar nDig;
} WORDThick;

typedef struct tagThickColumn {
	uint16_t thick;
	int16_t nLet;  //  also mark for first not-empty
	uchar nThick;
	uchar bold;
	uchar minThick;
	uchar maxThick;
}
THICKparam;

#define BOLDMAXWORD 128
static WORDThick WordThick[BOLDMAXWORD];
static int thickBuffer[MAXTHICK];
static int heiAllBuffer[MAXHEI];
static int heiLitBuffer[MAXHEI];
static uchar mas[8]={128,64,32,16,8,4,2,1};

static int pageLight=0,pageBold=MAXTHICK;
static int pageLineLight=0,pageLineBold=MAXTHICK;
static int acceptLineLight=0,acceptLineBold=MAXTHICK;

static int BoundLIGHT=0;
static int BoundBOLD =0;

#define MAXSNAP 2048
static char snapText[MAXSNAP];

#define MAXGISTHICK 50
static int GistThickness[MAXGISTHICK];

// данные по колонкам, строчкам таблиц
#define MAXTAB 8
#define MAXCOL 16
#define MAXROW 64
static THICKparam thickCol[MAXTAB][MAXCOL];
static THICKparam thickRow[MAXTAB][MAXROW];
static void TestColumnsThick(void);
static void TestRowsThick(void);
static void TestAccordRowsColumns(void);
static int Progib(int *hhh,int GreyLev, int numAccord);
//////////////////////////
static const char bigLitLet[]="bdfghiklpqty";
////////////
static int32_t LetDigSymbol(int i) // letter or digit
{
        if( i >= '0' && i <= '9' )
                return 1;
        if(
                i >= 'A' && i <= 'Z' ||
                i >= 'a' && i <= 'z' )
                return 2;
        if( language == LANG_RUSSIAN || language == LANG_ENGLISH)
        {
                if(             i >= 128 && i < 176  ||    // ascii A-п
                        i >= 224 //&& i < 240      //       р-я  & Italic!
          )
          return 2;
        }
        else if( i >= 0xC0 )
                return 2;

        return 0;
}
////////////
static int BigSymbol(int i)
{
        if( i=='Q' || i=='j')
                return 3;

        if( i >= '0' && i <= '9' ||
                i >= 'A' && i <= 'Z' ||
                strchr(bigLitLet,i)
      )
          return 2;

        if( language == LANG_RUSSIAN || language == LANG_ENGLISH)
        {
                if( i==0xe4 )
                        return 3;

                if( i >= 128 && i < 160 )
                        return 2;

        // бйруфд (д-курсив!)
          if( i==0xa1 || i == 0xa9 || i==0xe0 || i==0xe3 || i==0xf0 )
             return 2;

                // дцщ
         if( i == 0xa4 || i == 0xe6 || i == 0xe9)
                 return 1;
        }
        else
        {
                if(i>=0xC0 && i <0xDE)
                        return 3;
                if(i >= 0xDE )
                        return 2;
        }

        return 0;
}
////////////
static void FillFromLine(CCOM_lnhead *linerep,int size,int *thickBuf,int add)
{
 CCOM_lnhead    * line;
 CCOM_interval  * inter;
 int len,i,j;

 for(line=linerep,len=0; len<size && line->lth; len+=line->lth,line=(CCOM_lnhead *)((intptr_t)line+line->lth))
  {
         // начало линии (line->row) - нам не важно
   for(inter = (CCOM_interval*)(line+1), i=0; i<line->h; i++, inter++)
   {
      if(inter->l < 0)
                  return;
          j=MIN(MAXTHICK-1,inter->l+add);
          thickBuf[j]++;
      //StoreOneInterval( buf, inter->e-inter->l, MIN(w,inter->e));
   }
  }
}
///////////////////
static Bool32 FillThickBuffer(CCOM_comp *comp,int *thickBuf,int *height)
{
 uchar *pWord8;
 int16_t *pInt16;
 int w,h;

   if(!comp || !comp->linerep)
         return FALSE;

   pWord8 = &comp->linerep[0];
   pInt16 = (int16_t*)pWord8;

   w = comp->w;
   h = comp->h;
   if( comp->scale )
        {
        w =(w+(1<<comp->scale)-1)>> comp->scale;
        h =(h+(1<<comp->scale)-1)>> comp->scale;
        }
   *height=h;
   do  { // comps cycle
        //ret = Linerep2Raster((CCOM_lnhead*)(pWord8+2),(int16_t)((*pInt16)-2), w, h);
                FillFromLine((CCOM_lnhead*)(pWord8+2),(*pInt16)-2,thickBuf,0);
                if( comp->numcomp <= 1)
                        break;

        pWord8 += *pInt16;
        pInt16  = (int16_t*)pWord8;

        }
   while(  (*pInt16) > 0 );
   return TRUE;
}
//////////////////////
#define POROG_LIGHT 23
#define POROG_BOLD  33      // 31-32
//
////////////////
static int AddThickStat(CSTR_rast first,uchar name,int *thickBuffer)
{
 CCOM_comp *comp=CSTR_GetComp(first);
 int i,k,dig;

  if(!comp)
          return 0;

  dig=LetDigSymbol(name);
  if( dig == 0)
          return 0;

  if(!FillThickBuffer(comp,thickBuffer,&k))
          return 0;

  i=BigSymbol(name);

  if(i==0)
  {
          heiLitBuffer[MAX(0,MIN(k,MAXHEI-1))]++;
  }
  else
  {
   if(i==3) k=(k*2)/3;
   else if(i==2) k=(k*3)/4;
   else if(i==1) k=(k*4)/5;
  }

  heiAllBuffer[MAX(0,MIN(k,MAXHEI-1))]++;

  return i+1;
}
///////////////////
static int GetTipicalThick(int nLet,WORDThick *LineThick,int *thickBuf,int bestHeight,
                                                   WORDThick *commonThick, int addHeight)
{
 int i,k,bestWidth;
 int maxThick=0,minWidth;
 int maxSumma;
 int adjustedHeight; // Nick 25.12.2001

  memset(LineThick,0,sizeof(WORDThick));
  if(nLet <= 0 || bestHeight <= 0 )
          return 0;

  for(bestWidth=0,i=1;i<MAXTHICK;i++)
  {
         if( thickBuf[i] <= 0)
                 continue;

         if(thickBuf[i] > thickBuf[bestWidth])
                 bestWidth=i;
         if(thickBuf[i] == thickBuf[bestWidth] &&
            (commonThick == NULL || commonThick->strih!=bestWidth)
                )
         bestWidth=i;
         maxThick=i;
  }

  if( bestWidth <= 0 )
          return 0;

  // поищем лучшего среди толстых
  maxThick=MIN(maxThick, bestHeight>>1);
  maxSumma=MAX(thickBuf[bestWidth-1],thickBuf[bestWidth+1]);
  maxSumma+=thickBuf[bestWidth];
  for(k=-1,i=bestWidth+1,minWidth=thickBuf[bestWidth];i<=maxThick;i++)
  {
         if( thickBuf[i] > 2*minWidth &&
          (k < 0 || thickBuf[i] >= thickBuf[k] ) &&
            ( thickBuf[i]*10 > thickBuf[bestWidth]*7 &&
			  (thickBuf[i]+MAX(thickBuf[i+1],thickBuf[i-1]))*10 > maxSumma*7
			||
			  thickBuf[i]*10 > thickBuf[bestWidth]*6 &&
			  (thickBuf[i]+MAX(thickBuf[i+1],thickBuf[i-1]))*10 > maxSumma*8
			 )
           )
                 k=i;
     minWidth=MIN(minWidth,thickBuf[i]);
  }
  if(k>0)
          bestWidth=k;

  // отличается сильно от стандарта - тест
  if( commonThick && commonThick->strih>0 &&
      abs(bestWidth - commonThick->strih) > 1
     )
  {
     k=commonThick->strih;;
     if( commonThick->strih>1 && thickBuf[commonThick->strih] < thickBuf[commonThick->strih-1])
                 k--;
     if( commonThick->strih < MAXTHICK -1  &&
         thickBuf[k] < thickBuf[commonThick->strih+1]
       )
                 k=commonThick->strih+1;

     if( thickBuf[k]*10 > thickBuf[bestWidth]*7 ) // was 6 - Nick 25.11
                 bestWidth=k;
  }

  if( language == LANG_RUSSIAN &&
          commonThick &&
          commonThick->height > 0 &&
          bestHeight != commonThick->height &&
      abs( commonThick->strih - bestWidth) < 2
        )
        bestHeight = commonThick->height;

	adjustedHeight = bestHeight*10 + addHeight;

  if( bestWidth > 0 && bestWidth < MAXTHICK-1 &&
          thickBuf[bestWidth-1] != thickBuf[bestWidth+1]
         )
  {
   if( bestWidth>1 &&
           thickBuf[bestWidth-1] > thickBuf[bestWidth+1] &&
           thickBuf[bestWidth-2] +1 >= thickBuf[bestWidth+1] )
          k=(((bestWidth-1)*thickBuf[bestWidth-1] + bestWidth*thickBuf[bestWidth])*1000)/
             (adjustedHeight*(thickBuf[bestWidth]+thickBuf[bestWidth-1]));
   else if( bestWidth < MAXTHICK-1 &&
           thickBuf[bestWidth-1] < thickBuf[bestWidth+1] &&
           thickBuf[bestWidth+2] + 1 >= thickBuf[bestWidth-1] )
          k=(((bestWidth+1)*thickBuf[bestWidth+1] + bestWidth*thickBuf[bestWidth])*1000)/
             (adjustedHeight*(thickBuf[bestWidth]+thickBuf[bestWidth+1]));
   else
    k=(((bestWidth-1)*thickBuf[bestWidth-1] + bestWidth*thickBuf[bestWidth]+
       (bestWidth+1)*thickBuf[bestWidth+1])*1000)/
			(adjustedHeight*(thickBuf[bestWidth]+thickBuf[bestWidth-1]+thickBuf[bestWidth+1]));
  }
  else
   k=(bestWidth*1000)/adjustedHeight;

  LineThick->strih =(uchar)MIN(MAXTHICK-1,bestWidth);
  LineThick->height=bestHeight;
  LineThick->thick =k;
  LineThick->nLet  =nLet;
  return k;
}
///////////////////
static int FindAddHeight(int bestHeight, int *heiBuf, int size)
{
	int i;
	int para = 0;

	if( bestHeight <= 0 || bestHeight >= size - 1 ||
		heiBuf[bestHeight] <= 1
	  )
		return 0;

	if( heiBuf[bestHeight-1] >  heiBuf[bestHeight+1]*2 )
		para = bestHeight-1;
	else if( heiBuf[bestHeight+1] >  heiBuf[bestHeight-1]*2 )
		para = bestHeight+1;
	else
		return 0;

	if( heiBuf[para]*3 < heiBuf[bestHeight]*2 )
		return 0;

	i = (heiBuf[para]*5)/heiBuf[bestHeight];

	if( para > bestHeight )
		return i;
	else
		return -i;

}
///////////////
static int p2_SetBoldWord(CSTR_rast first,CSTR_rast last,
                          int nWord,WORDThick *lineThick,
                          WORDThick *prevThick,Bool32 veryThin,
						  int lineAddHeight )
{
  CSTR_rast fRast;
  CSTR_rast_attr  attr;
  int       numLet=0;
  int       nLet;
  RecVersions     rv;
  WORDThick   tmpThick={0};
  int porog;
  int i,bestHeight;
  int nLitLet;
  int addHeight = 0;

  memset(thickBuffer,0,sizeof(thickBuffer));
  memset(heiLitBuffer,0,sizeof(heiLitBuffer));
  memset(heiAllBuffer,0,sizeof(heiAllBuffer));

  for(fRast=first,nLet=nLitLet=0; fRast && fRast!=last; fRast=CSTR_GetNext(fRast) )
  {
        if( !CSTR_GetAttr (fRast, &attr) )
                return -1;
    if( !(attr.flg & CSTR_f_let))
                continue;
    CSTR_GetCollection(fRast,&rv);
    if(rv.lnAltCnt <= 0)
           continue;
    if( (i=AddThickStat(fRast,rv.Alt[0].Code,thickBuffer))>0)
        {
           nLet++;
           if(i==1) nLitLet++;
        }
 }
 if(nLet<=0)
         return -1;

 bestHeight=0;

 if( nLitLet > 0 )
 {
	for(i=1;i<MAXHEI;i++)
	{
         if( heiLitBuffer[i] == 0)
                 continue;
         if(heiLitBuffer[i]>heiLitBuffer[bestHeight] ||
			heiLitBuffer[i] == heiLitBuffer[bestHeight] &&
			lineThick->height > 0 &&
                abs(lineThick->height - i) < abs(lineThick->height - bestHeight)
           )
                 bestHeight=i;
	}
 }


 if( bestHeight== 0 ||
	 bestHeight != lineThick->height &&
	 ( heiLitBuffer[bestHeight] <= 1 ||  ( (nLitLet<<1) < nLet ) )
   )
 {
  for(bestHeight=0,i=1;i<MAXHEI;i++)
  {
     if( heiAllBuffer[i] == 0)
                 continue;
         if(heiAllBuffer[i]>heiAllBuffer[bestHeight] ||
        heiAllBuffer[i] == heiAllBuffer[bestHeight] &&
        lineThick->height > 0 &&
                abs(lineThick->height - i) < abs(lineThick->height - bestHeight)
           )
                 bestHeight=i;
  }
 }

 if(bestHeight <= 0 ||
    nLet <= 1 &&  language == LANG_RUSSIAN ||
    lineThick->height > 0 &&
    (abs(lineThick->height - bestHeight) < 2 )
   )
 {
       bestHeight = lineThick->height;
	   addHeight  = lineAddHeight;
 }
 else
	addHeight = FindAddHeight(bestHeight, heiLitBuffer, MAXHEI);


 GetTipicalThick(nLet,&tmpThick,thickBuffer,bestHeight,lineThick, addHeight);
 if(veryThin && tmpThick.thick > 0)
         tmpThick.thick+=ADD_THIN;

 if(snap_activity('j'))
 {
   int j;
   strcpy(snapText,"BoldGist.:");
   for(j=0;j<MAXTHICK;j++)
           if( thickBuffer[j]>0 && strlen(snapText)<MAXSNAP-32 )
                   sprintf(snapText+strlen(snapText),"(%d,%d)",j,thickBuffer[j]);
   snap_show_text(snapText);
 }

 if(nWord < BOLDMAXWORD )
  WordThick[nWord]=tmpThick;

 if( tmpThick.thick <= 0 )
         return -1;

 if(tmpThick.thick > POROG_BOLD)
         return P2_BOLD;
 if(tmpThick.thick < POROG_LIGHT)
         return P2_LIGHT;

 porog=10+MAX(0,5-tmpThick.nLet);
 if(lineThick->thick > 0 && tmpThick.thick > lineThick->thick+porog)
         return P2_BOLD;

 if(tmpThick.nLet > 5 )
         porog=5;
 else
     porog=POROG_SOSED_MAX-tmpThick.nLet;

 if( prevThick && prevThick->thick > 0 &&  abs(prevThick->thick-tmpThick.thick)<porog )
     return prevThick->bold;

 return 0;
}
///////////////////////
static void SetBoldness(int i,CSTR_rast first,CSTR_rast last)
{
 CSTR_rast       fRast;
 CSTR_rast_attr  attr;
 uchar     maskaAnd,maskaOr;

  if(i==P2_BOLD)
  {
          maskaAnd= ~CSTR_fp_light;
          maskaOr = CSTR_fp_bold;
  }
  else if(i==P2_LIGHT)
  {
          maskaAnd= ~CSTR_fp_bold;
          maskaOr = CSTR_fp_light;
  }
  else
          return;

  for(fRast=first; fRast && fRast!=last; fRast=CSTR_GetNext(fRast) )
  {
        if( !CSTR_GetAttr (fRast, &attr) )
                return;
        attr.font &=maskaAnd;
    attr.font |=maskaOr;
        CSTR_SetAttr (fRast, &attr);
  }

  return;
}
//////////////////
static int SetAllBoldness(int nWord,WORDThick *wthick,int bold)
{
        int i;
        for(i=0;i<nWord;i++)
        {
                if(wthick[i].bold==bold)
                        continue;
                SetBoldness(bold,wthick[i].first,wthick[i].last);
        if(snap_activity('j') )
                {
          p2_TextWord(wthick[i].first,wthick[i].last,snapText,TRUE);
                  if(bold==P2_BOLD) strcat(snapText," - BOLD");
                  else if(bold==P2_LIGHT) strcat(snapText," - LIGHT");
                  snap_show_text(snapText);
                }
                wthick[i].bold=bold;
        }
        return 1;
}
//////////////////
static int GetOneBold(int thick,int prevBold,int prevThick)
{
 int bold=0;
 int minVal=BoundLIGHT;
 int maxVal=BoundBOLD;

 //if( minVal == 0)     minVal=pageLineLight;
 //if( maxVal == 0 )    maxVal=pageLineBold;

 if( minVal == 0)
     minVal=pageLight;
 if( maxVal == 0)     // MAXTHICK )
     maxVal=pageBold;

 if( minVal == 0 && maxVal == MAXTHICK)
         return prevBold;

 if( minVal == 0) minVal = maxVal-10;
 if( maxVal == MAXTHICK) maxVal= minVal+10;

 if( thick-minVal < maxVal - thick )
                        bold=P2_LIGHT;
 else if( thick-minVal > maxVal - thick)
                        bold=P2_BOLD;
 else
         bold=prevBold;

 return bold;
}
///////////////////
static int CompareBoldSosed(WORDThick *wthick,int prevThick,int prevBold,int sosedThick )
{
 int porog;
 int bold;

      if( prevThick && abs(prevThick-wthick->thick)< 3 )
                  return prevBold;

      if( wthick->nLet > 1)
          {
       if(BoundLIGHT && wthick->thick < BoundLIGHT)
                  return P2_LIGHT;
           if(BoundBOLD && wthick->thick > BoundBOLD)
                  return P2_BOLD;
          }

      if( wthick->nLet > 5)
            porog=5;
      else
        porog=POROG_SOSED_MAX-wthick->nLet;

        if(  abs(prevThick-wthick->thick)<porog ||
                   sosedThick && abs(sosedThick-wthick->thick)<3
                )
        bold=prevBold;
      else
        bold=GetOneBold(wthick->thick,prevBold,prevThick);

      return bold;
}
////////////
static int SetTestBold(int nWord,WORDThick *wthick,int maxWord,WORDThick *prevLine)
{
    int i;
    int bold;
    int prevThick,prevBold;
    int sosedThick;

    if(prevLine->bold &&
      abs(prevLine->thick-wthick[maxWord].thick) < POROG_INTERLINE)
     bold=prevLine->bold;
    else
     bold=GetOneBold(wthick[maxWord].thick,prevLine->bold,prevLine->thick);

   SetBoldness(bold,wthick[maxWord].first,wthick[maxWord].last);
   i=maxWord;
   if( snap_activity('j') && wthick[maxWord].bold!=bold )
         {
          p2_TextWord(wthick[i].first,wthick[i].last,snapText,TRUE);
          if(bold==P2_BOLD) strcat(snapText," - BOLD");
          else if(bold==P2_LIGHT) strcat(snapText," - LIGHT");
          else  strcat(snapText," - unknown");
          snap_show_text(snapText);
         }
    wthick[maxWord].bold=bold;

    prevThick = wthick[maxWord].thick;
    prevBold=wthick[maxWord].bold;
        sosedThick=0;

    for(i=maxWord+1;i<nWord;i++)
        {
      bold=CompareBoldSosed(wthick+i,prevThick,prevBold,sosedThick);

      SetBoldness(bold,wthick[i].first,wthick[i].last);
      if( snap_activity('j') && wthick[i].bold!=bold )
                {
          p2_TextWord(wthick[i].first,wthick[i].last,snapText,TRUE);
                  if(bold==P2_BOLD) strcat(snapText," - BOLD");
                  else if(bold==P2_LIGHT) strcat(snapText," - LIGHT");
                  else  strcat(snapText," - unknown");
                  snap_show_text(snapText);
                }
          wthick[i].bold=bold;

          if( bold==prevBold)
                  sosedThick=wthick[i].thick;
          else
                  sosedThick=0;

          if( wthick[i].nLet >= 5)
                {
                    prevThick=wthick[i].thick;
                        prevBold=bold;
                }
        }

        prevThick = wthick[maxWord].thick;
    prevBold  = wthick[maxWord].bold;
        sosedThick=0;

        for(i=maxWord-1;i>=0;i--)
        {
      bold=CompareBoldSosed(wthick+i,prevThick,prevBold,sosedThick);

          SetBoldness(bold,wthick[i].first,wthick[i].last);
      if( snap_activity('j') && wthick[i].bold!=bold )
                {
          p2_TextWord(wthick[i].first,wthick[i].last,snapText,TRUE);
                  if(bold==P2_BOLD) strcat(snapText," - BOLD");
                  else if(bold==P2_LIGHT) strcat(snapText," - LIGHT");
                  else  strcat(snapText," - unknown");
                  snap_show_text(snapText);
                }
          wthick[i].bold=bold;

          if( bold==prevBold)
                  sosedThick=wthick[i].thick;
          else
                  sosedThick=0;

          if( wthick[i].nLet >= 5)
                {
                        prevThick=wthick[i].thick;
                        prevBold=bold;
                }
        }

        if( nWord <= 1 )
                return 1;
#ifdef _NEW_SMALL_
		for(i=0;i<nWord;i++)
		  if( wthick[i].nLet >= 2 ) break;
        i++;
#else
		i=1;
#endif
        for(;i<nWord-1;i++)
        {
			int j,k;

            if( wthick[i].nLet >= 3 ) continue;

            //if( wthick[i-1].nLet < 3 ||  wthick[i+1].nLet < 3 )
            //      continue;

            bold=wthick[i].bold;

#ifdef _NEW_SMALL_
			for(k=i-1;k>=0;k--)
				if(wthick[k].nLet >=2) break;
            if(k<0) k=i-1;
#else
            k=i-1;
#endif

			for(j=i+1;j<nWord;j++)
				if(wthick[j].nLet >=3) break;
            if(j>=nWord)
				j=i+1;

            if( wthick[k].bold != wthick[j].bold)
             {
              if(abs(wthick[k].thick-wthick[i].thick) <
                 abs(wthick[j].thick-wthick[i].thick) ||
                 abs(wthick[k].thick-wthick[i].thick) ==
                 abs(wthick[j].thick-wthick[i].thick) &&
                 wthick[j].nLet <= wthick[k].nLet
                )
                        bold=wthick[k].bold;
              else
                        bold=wthick[j].bold;

             }

             else if(   wthick[k].bold!= bold &&
                        wthick[j].bold!= bold
                   )
			 {
#ifdef _NEW_SMALL_
				int porog=(wthick[i].nLet==1?8:5);
                if( (bold==P2_LIGHT &&
				     wthick[i].thick+porog >= MIN(wthick[i-1].thick,wthick[j].thick)) ||
					 (bold==P2_BOLD &&
				     wthick[i].thick <= MAX(wthick[i-1].thick,wthick[j].thick)+porog)
				   )
#endif
                 bold=wthick[k].bold;
			 }

             if(bold!=wthick[i].bold)
             {
               SetBoldness(bold,wthick[i].first,wthick[i].last);
               if( snap_activity('j') && wthick[i].bold!=bold )
                 {
                  p2_TextWord(wthick[i].first,wthick[i].last,snapText,TRUE);
                  if(bold==P2_BOLD) strcat(snapText," - BOLD");
                  else if(bold==P2_LIGHT) strcat(snapText," - LIGHT");
                  else  strcat(snapText," - unknown");
                  snap_show_text(snapText);
                 }
                 wthick[i].bold=bold;
              }
        }


        // первый особый символ в строке
        if( wthick[0].nLet == 1 &&
            wthick[0].thick > wthick[1].thick + 5 &&
            wthick[0].height*2 > wthick[1].height*3 )
        {
                wthick[0].bold=P2_BOLD;
                SetBoldness(P2_BOLD,wthick[0].first,wthick[0].last);
        }

        return 1;
}
//////////////////
static int CorrectBoldness(int nWord,WORDThick *wthick,WORDThick *line,WORDThick *prevLine,
						   int nTabl,int nCol,int nRow)
{
        int i;
        int nBold=0,nLight=0;
        int maxLight=0,minBold=MAXTHICK;
        int minVal=MAXTHICK,maxVal=0;
        Bool isGoodStat;
        int maxWord;

//      memset(thickBuffer,0,sizeof(thickBuffer));
        for(i=0,maxWord=0,isGoodStat=FALSE;i<nWord;i++)
        {
       if(  wthick[i].nLet>wthick[maxWord].nLet  )
                  maxWord=i;

//       if(wthick[i].nLet < 3)
//                 continue;

           minVal=MIN(minVal,wthick[i].thick);
       maxVal=MAX(maxVal,wthick[i].thick);

           if( wthick[i].nLet < MIN_STAT_PAGE )
                   continue;
           if( wthick[i].height <= MIN_HEIGHT )
                   continue;

       if(wthick[i].bold==P2_BOLD)
           {
                   if( wthick[i].thick < minBold)
                           minBold=wthick[i].thick;

                   pageBold =MIN(pageBold, wthick[i].thick);
                   isGoodStat=TRUE;
                   nBold++;
           }
           else if(wthick[i].bold==P2_LIGHT)
           {
                    if( wthick[i].thick > maxLight)
                           maxLight=wthick[i].thick;

                    pageLight=MAX(pageLight,wthick[i].thick);
                        isGoodStat=TRUE;
                        nLight++;
           }

//         thickBuffer[MIN(MAXTHICK-1,wthick[i].thick)]++;
        }

        if(minVal > maxVal)
                return 0;



#ifndef _OLD_TABL_VERSION_
		 // если таблица -
		 if( nCol > 0   && nCol <= MAXCOL &&
			 nRow > 0   && nRow <= MAXROW &&
			 nTabl >= 0 && nTabl < MAXTAB
			)
		 {
			 uchar colBold = thickCol[nTabl][nCol-1].bold;
			 uchar rowBold = thickRow[nTabl][nRow-1].bold;
			 int   nThick = thickCol[nTabl][nCol-1].nThick;
			 int   nRowThick = thickRow[nTabl][nRow-1].nThick;

			 if(snap_activity('j'))
			 {
			  char tmp[256];
              sprintf(tmp,"Column thick %d,min=%d,max=%d,bold %d",
				  (int)thickCol[nTabl][nCol-1].nThick,
				  (int)thickCol[nTabl][nCol-1].minThick,
				  (int)thickCol[nTabl][nCol-1].maxThick,
				  (int)colBold);
              snap_show_text(tmp);
			  sprintf(tmp,"Row thick %d,min=%d,max=%d,bold %d",
				  (int)thickRow[nTabl][nRow-1].nThick,
				  (int)thickRow[nTabl][nRow-1].minThick,
				  (int)thickRow[nTabl][nRow-1].maxThick,
				  (int)rowBold);
              snap_show_text(tmp);
			 }

			 // определено по всей строке ?
			 if( nRowThick == 0 && rowBold )
			 {
                  SetAllBoldness(nWord,wthick,rowBold);
                  return rowBold;
			 }

			 // одинаково по колонке ?
			 if( nThick == 0 && colBold )
			 {
                  SetAllBoldness(nWord,wthick,colBold);
                  return colBold;
			 }

			 // одинаково по строке ?
			 if( minVal + POROG_ONECOL > maxVal  )
			 {
              if( BoundLIGHT && line->thick < BoundLIGHT)
                {
                  SetAllBoldness(nWord,wthick,P2_LIGHT);
                  return P2_LIGHT;
                 }
              if( BoundBOLD && line->thick > BoundBOLD)
                {
                  SetAllBoldness(nWord,wthick,P2_BOLD);
                  return P2_BOLD;
                 }
			 }


			 // оценим по колонке
			 colBold = SetBoldByTable( nThick,
				 thickCol[nTabl][nCol-1].thick, colBold,
				 line->thick, minVal, maxVal);
			 // по строке
			 rowBold = SetBoldByTable( nRowThick,
				 thickCol[nTabl][nRow-1].thick, rowBold,
				 line->thick, minVal, maxVal);

			 if(snap_activity('j'))
			 {
			  char tmp[256];
			  sprintf(tmp,"colBold=%d, rowBold=%d",colBold,rowBold);
              snap_show_text(tmp);
			 }

			 // совпадает ?
			 if( colBold && colBold == rowBold )
			 {
				 SetAllBoldness(nWord,wthick,colBold);
                 return colBold;
			 }

			 // не сильно разнятся в ячейке?
			 if( minVal + POROG_ONECELL  > maxVal &&
				 // есть общность по колонке или строке
			    (colBold || rowBold )
			   )
			 {
              if( !rowBold || !colBold)
			  {
                 if(!colBold)
					 colBold = rowBold;
			     SetAllBoldness(nWord,wthick,colBold);
                 return colBold;
			  }

			  if( nCol == 1 && nRow > 1 )
			  {
				  SetAllBoldness(nWord,wthick,colBold);
                  return colBold;
			  }
			  if( nCol > 1 && nRow == 1 )
			  {
				  SetAllBoldness(nWord,wthick,rowBold);
                  return rowBold;
			  }

			 // есть предыдущие - кто ближе ?
			  if ( abs(line->thick-nThick) > abs(line->thick-nRowThick) )
					 colBold = rowBold;

			  SetAllBoldness(nWord,wthick,colBold);
              return colBold;
			 }

			 if( minVal + POROG_ONECOL  >= maxVal )
			 // есть общность по ячейке
			 {
                colBold = thickCol[nTabl][nCol-1].bold;

				if(colBold )
				{
				 SetAllBoldness(nWord,wthick,colBold);
                 return colBold;
				}
			 }

           // если не вышло по таблице - теперь к общей процедуре
		 }
#endif

		if( BoundLIGHT && maxVal < BoundLIGHT)
                {
                  SetAllBoldness(nWord,wthick,P2_LIGHT);
                  return P2_LIGHT;
                 }
        if( BoundBOLD && minVal > BoundBOLD)
                {
                  SetAllBoldness(nWord,wthick,P2_BOLD);
                  return P2_BOLD;
                 }

        // если оценки близки, нет явных болдов или тощих -
        // особая проверка

        if( minVal + POROG_ONELINE > maxVal  )
        {
         int bb,boundBold,boundLight;

         if( BoundLIGHT && line->thick < BoundLIGHT)
                {
                  SetAllBoldness(nWord,wthick,P2_LIGHT);
                  return P2_LIGHT;
                 }
         if( BoundBOLD && line->thick > BoundBOLD)
                {
                  SetAllBoldness(nWord,wthick,P2_BOLD);
                  return P2_BOLD;
                 }

#ifdef _OLD_TABL_VERSION_
		 // если таблица -
		 // сравним с предыдущей строкой в столбце
		 if( nCol > 0   && nCol <= MAXCOL &&
			 nTabl >= 0 && nTabl < MAXTAB
			)
		 {
			 uchar colBold = thickCol[nTabl][nCol-1].bold;

			 if( thickCol[nTabl][nCol-1].maxThick > 0)
			 {
                  SetAllBoldness(nWord,wthick,colBold);
                  return colBold;
			 }

             if( thickCol[nTabl][nCol-1].nThick > 0 )
			 {
			  int nThick = thickCol[nTabl][nCol-1].nThick;

			  if( abs(line->thick-nThick) <= POROG_INTERLINE ||
                 colBold == P2_LIGHT &&  line->thick < nThick ||
                 colBold == P2_BOLD  &&  line->thick > nThick
			   )
			  {
                  SetAllBoldness(nWord,wthick,colBold);
                  return colBold;
              }
			 }
		 }
#endif


         if( BoundLIGHT && BoundBOLD && (BoundLIGHT == BoundBOLD) &&
                 (line->thick == BoundLIGHT) )
         {
                if( prevLine->thick &&
                        prevLine->thick + POROG_INTERLINE <=  line->thick
                  )
                  {
                  SetAllBoldness(nWord,wthick,P2_BOLD);
                  return P2_BOLD;
                 }
                if( prevLine->thick &&
                        prevLine->thick >= line->thick + POROG_INTERLINE
                  )
                  {
                  SetAllBoldness(nWord,wthick,P2_LIGHT);
                  return P2_LIGHT;
                 }
         }

         if( line->thick < POROG_LIGHT )
                {
                  SetAllBoldness(nWord,wthick,P2_LIGHT);
                  return P2_LIGHT;
                 }
         if( line->thick > POROG_BOLD )
                {
                  SetAllBoldness(nWord,wthick,P2_BOLD);
                  return P2_BOLD;
                }

        if(prevLine->bold == P2_LIGHT )
                {
                int add=(line->nLet > MIN_SIZE_LINE?0:line->nLet>12?1:2);

         if( prevLine->thick + POROG_INTERLINE >  line->thick ||
             prevLine->thick + POROG_INTERLINE ==  line->thick  &&
             acceptLineLight + 2 >  line->thick
                   )
                 {
                  SetAllBoldness(nWord,wthick,P2_LIGHT);
                  return P2_LIGHT;
                 }
                 if( prevLine->thick + POROG_INTERLINE +add <  line->thick &&
             line->thick > pageLineLight +1  &&
             line->thick > pageLight +1
                   )
                 {
                  SetAllBoldness(nWord,wthick,P2_BOLD);
                  return P2_BOLD;
                 }
                }

        if(prevLine->bold == P2_BOLD )
                {
         if( prevLine->thick - POROG_INTERLINE <  line->thick ||
                         prevLine->thick - POROG_INTERLINE ==  line->thick  &&
             acceptLineBold - 2 < line->thick
                   )
                 {
                  SetAllBoldness(nWord,wthick,P2_BOLD);
                  return P2_BOLD;
                 }
                 if( prevLine->thick - POROG_INTERLINE >  line->thick &&
                         line->thick < pageLineBold - 1  &&
             line->thick < pageBold - 1
                   )
                 {
                  SetAllBoldness(nWord,wthick,P2_LIGHT);
                  return P2_LIGHT;
                 }
                }

        if(pageLineLight!=0 || pageLineBold!=MAXTHICK )
         {
                        int vMin=pageLineLight,vMax=pageLineBold;
                        if(pageLineLight==0)
                                vMin=pageLineBold-2*POROG_INTERLINE;
                        if(pageLineBold==MAXTHICK)
                                vMax=pageLineLight+2*POROG_INTERLINE;

                        if( line->thick-vMin < vMax-line->thick)
                        {
                         SetAllBoldness(nWord,wthick,P2_LIGHT);
                         return P2_LIGHT;
                        }
                        else if( line->thick-vMin > vMax-line->thick)
                        {
                         SetAllBoldness(nWord,wthick,P2_BOLD);
                         return P2_BOLD;
                        }
         }

                // не вышло по строке целиком - смотрим по словам
         if(pageLight==0 && pageBold==MAXTHICK)
                          // всем - light
         {
                 SetAllBoldness(nWord,wthick,P2_LIGHT);
                 return P2_LIGHT;
         }

         if(pageBold==MAXTHICK)
                        boundBold=pageLight+10;
         else
                        boundBold=pageBold;

         if(pageLight==0)
                        boundLight=pageBold-10;
         else
                        boundLight=pageLight;

     if(minVal-boundLight < boundBold-maxVal)
                        bb=P2_LIGHT;
         else
                        bb=P2_BOLD;
         SetAllBoldness(nWord,wthick,bb);
         return bb;
        }

        SetTestBold(nWord,wthick,maxWord,prevLine);
    return 3;
}
/////////////////
static void PutBoldness(CSTR_rast fRast,CSTR_rast last,uchar bold)
{
 uchar maskaAnd=~(CSTR_fp_bold | CSTR_fp_light);
 CSTR_rast_attr  attr;

 for(; fRast && fRast != last; fRast=CSTR_GetNext(fRast) )
  {
        if( !CSTR_GetAttr (fRast, &attr) )      return;
        if(attr.flg & CSTR_f_space) continue;
        attr.font = (attr.font&maskaAnd)|bold;
        CSTR_SetAttr (fRast, &attr);
  }
}
/////////////////
static void PointsBoldness(CSTR_rast first,int nWord,WORDThick *wthick)
{
 CSTR_rast_attr  attr;
 uchar           maska = CSTR_fp_bold | CSTR_fp_light;
 int             i;

  if( nWord <= 0)
                return ;

  CSTR_GetAttr (wthick[0].first, &attr);
  PutBoldness(first,wthick[0].first,(uchar)(attr.font & maska));

  for(i=0;i<nWord-1;i++)
  {
    PutBoldness( wthick[i].last, wthick[i+1].first,(uchar)(attr.font & maska));
        CSTR_GetAttr (wthick[i+1].first, &attr);
  }

  PutBoldness( wthick[nWord-1].last, NULL, (uchar)(attr.font & maska));
}
///////////////////
static const char   bold_word_limits[]="-.,:;'"; // OLEG
// 0xba - обозначение для i с приклееной точкой
static const char Palki[]="![]()1Ifilt"; // liga_i макра // 08.09.2000 E.P.
static WORDThick  prevLineThick;
//////////////////
void p2_BoldInit(int p2)
{
// clear all
        memset(&prevLineThick,0,sizeof(WORDThick));

        snap_newpass('j');

        if(!p2)
        {
                pageLight=pageLineLight=0;
                pageBold =pageLineBold=MAXTHICK;
                BoundLIGHT=BoundBOLD=0;
                memset(GistThickness,0,sizeof(int)*MAXGISTHICK);
				memset(thickCol,0,sizeof(thickCol));
				memset(thickRow,0,sizeof(thickRow));
        }
        else // find BoundLIGHT
		{
                FindBounds(GistThickness,MAXGISTHICK);
				TestRowsThick();
				TestColumnsThick();
				TestAccordRowsColumns();
		}

        // производные величины
        acceptLineLight=0;
        acceptLineBold =MAXTHICK;

#ifdef _PRINT_BOLD_
 if(p2 )
 {
     fbold=fopen("mybold.txt","at");
     nFile++;
     if(fbold)
         {
             fprintf(fbold,"file %d\n",nFile);
			 fprintf(fbold,"boundLight=%d,boundBold=%d\n",BoundLIGHT,BoundBOLD);
			 fprintf(fbold,"pageLight=%d,pageBold=%d\n",pageLight,pageBold);
             fclose(fbold);
         }
 }
 else
         fbold=NULL;

#endif
}
//////////////////
int p2_SetBoldLine(CSTR_line line)
{
 CSTR_rast first,last;
 CSTR_rast_attr  attr;
 RecVersions     rv;
 WORDThick LineThick,*prevWord;
 int       nLet,nWord,i;
 int       bestHeight, addBestHeight = 0;
 Bool32    veryThin; // для сильно сжатого шрифта
 int       nThin;
 int       corRet;
 int       nLitLet;
 CSTR_attr       lineAttr;

 CSTR_GetLineAttr ( line, &lineAttr);

 // статистика по строке
 memset(&LineThick,0,sizeof(WORDThick));
 memset(thickBuffer,0,sizeof(thickBuffer));
 memset(heiLitBuffer,0,sizeof(heiLitBuffer));
 memset(heiAllBuffer,0,sizeof(heiAllBuffer));

 veryThin=FALSE;
 nThin=0;
 for(nLet=nLitLet=0,first=CSTR_GetNext(CSTR_GetFirstRaster(line));first;first=CSTR_GetNext(first))
 { // scan all string
   if( !CSTR_GetAttr(first,&attr) )
     return -1;
   if( !(attr.flg & CSTR_f_let) ) continue;
   CSTR_GetCollection(first,&rv);
   if(rv.lnAltCnt <= 0)
           continue;
   if( (corRet=AddThickStat(first,rv.Alt[0].Code,thickBuffer)) > 0 )
   {
           nLet++;
           if(corRet==1) nLitLet++;
           if(attr.w*2 < attr.h && !strchr(Palki,rv.Alt[0].Code) &&
				rv.Alt[0].Code != liga_i  && // 08.09.2000 E.P.
				!(language == LANG_TURKISH &&  // 30.05.2002 E.P.
					(rv.Alt[0].Code==i_sans_accent||rv.Alt[0].Code==II_dot_accent)
				 )

			  )
                   nThin++;
   }
 }

 if( nLet > 10 && nThin*10 > nLet*6)
         veryThin=TRUE;

 bestHeight=0;
 if( (nLitLet<<1) > nLet )
 {
  for(i=1;i<MAXHEI;i++)
  {
         if(heiLitBuffer[i]>heiLitBuffer[bestHeight] ||
        heiLitBuffer[i] == heiLitBuffer[bestHeight] &&
                heiAllBuffer[i] > heiAllBuffer[bestHeight]
           )
                 bestHeight=i;
  }
 }

 if(bestHeight==0 || heiLitBuffer[bestHeight] <= 1)
 {
  for(bestHeight=0,i=1;i<MAXHEI;i++)
  {
         if(heiAllBuffer[i]>heiAllBuffer[bestHeight] ||
        heiAllBuffer[i]==heiAllBuffer[bestHeight] &&
                heiLitBuffer[i]>heiLitBuffer[bestHeight]
           )
                 bestHeight=i;
  }
 }

 addBestHeight = FindAddHeight(bestHeight, heiLitBuffer, MAXHEI);

 GetTipicalThick(nLet,&LineThick,thickBuffer,bestHeight,NULL, addBestHeight);
 if(veryThin)
         LineThick.thick+=ADD_THIN;

 if(snap_activity('j'))
 {
   int j;
   strcpy(snapText,"BoldLineGist.:");
   for(j=0;j<MAXTHICK;j++)
           if( thickBuffer[j]>0 && strlen(snapText)<MAXSNAP-32 )
                   sprintf(snapText+strlen(snapText),"(%d,%d)",j,thickBuffer[j]);
   snap_show_text(snapText);
 }

 if(LineThick.thick > POROG_BOLD)
 {
         LineThick.bold=P2_BOLD;
     if(LineThick.nLet > MIN_SIZE_LINE && LineThick.height > MIN_HEIGHT )
                 pageLineBold=MIN(pageLineBold,LineThick.thick);
 }
 else if(LineThick.thick < POROG_LIGHT )
 {
         LineThick.bold=P2_LIGHT;
         if(LineThick.nLet > MIN_SIZE_LINE && LineThick.height > MIN_HEIGHT)
                 pageLineLight=MAX(pageLineLight,LineThick.thick);
 }

 prevWord=NULL;      // &prevLineThick;

 for(nWord=0,first=CSTR_GetNext(CSTR_GetFirstRaster(line));first;first=CSTR_GetNext(first))
 { // scan all string
   // search next word
   if( !CSTR_GetAttr(first,&attr) )
     return -1;
   if( attr.flg & CSTR_f_space ) continue;
   CSTR_GetCollection(first,&rv);
   if( rv.lnAltCnt && strchr(bold_word_limits,rv.Alt[0].Code) )
           continue;

   // last -  конец
   for(last=CSTR_GetNext(first);last; last=CSTR_GetNext(last))
   {
         if( !CSTR_GetAttr(last,&attr) )
       return -1;
         if( (attr.flg & CSTR_f_space ) )
                 break;
     CSTR_GetCollection(last,&rv);
     if( rv.lnAltCnt && strchr(bold_word_limits,rv.Alt[0].Code) )
        break;
   }

   if(snap_activity('j'))
   {
    p2_TextWord(first,last,snapText,TRUE);
        snap_show_text(snapText);
   }

   i=p2_SetBoldWord(first,last,nWord,&LineThick,prevWord,veryThin, addBestHeight);
   if( i >=0 )
   {
    WordThick[nWord].bold=i;
        WordThick[nWord].first=first;
        WordThick[nWord].last =last;

        if(snap_activity('j'))
        {
     clear_cells();

         sprintf(snapText,"nLet=%d,widLine=%d,hei=%d,thick=%d",
                 WordThick[nWord].nLet,WordThick[nWord].strih,WordThick[nWord].height,WordThick[nWord].thick);

         if(WordThick[nWord].bold==P2_BOLD)
                 strcat(snapText,"-BOLD");
     else if(WordThick[nWord].bold==P2_LIGHT)
                 strcat(snapText,"-LIGHT");

         snap_show_text(snapText);
     p2_Cstr2Cell( (CSTR_line)NULL,first,last,TRUE, 0);
         snap_monitor();
     clear_cells();
        }

        if( nWord < BOLDMAXWORD -1)
        {
                if( WordThick[nWord].nLet > 4 )
          prevWord=WordThick+nWord;
                nWord++;
        }
    SetBoldness(i,first,last);
   }

   if(!last)
           break;
   first=last;
 }

 if(snap_activity('j'))
 {
  int jj;
  snap_show_text("Start correct boldness");
  sprintf(snapText,"lineThick=%d,prevThick=%d,pageLight=%d,pageBold=%d,pageLineLight=%d,pageLineBold=%d",
          LineThick.thick,prevLineThick.thick,pageLight,pageBold,pageLineLight,pageLineBold);
  snap_show_text(snapText);

  strcpy(snapText,"WordGist.:");
  for(jj=0;jj<MAXGISTHICK;jj++)
           if( GistThickness[jj]>0 && strlen(snapText)<MAXSNAP-32 )
                   sprintf(snapText+strlen(snapText),"(%d,%d)",jj,GistThickness[jj]);
  snap_show_text(snapText);
  sprintf(snapText,"boundLight=%d,boundBold=%d",BoundLIGHT,BoundBOLD);
  snap_show_text(snapText);
 }

 corRet=CorrectBoldness(nWord,WordThick,&LineThick,&prevLineThick,
	 lineAttr.tab_number-1,lineAttr.tab_column,lineAttr.tab_row);
 // метки для точек и т.п.
 PointsBoldness(CSTR_GetNext(CSTR_GetFirstRaster(line)),nWord,WordThick);

 if(snap_activity('j'))
 {
  CSTR_line  l[]={line};
  snap_show_text("End correct boldness");
  snap_monitor_ori(&l[0],1);
 }


 if(LineThick.nLet > 20 || LineThick.nLet >= prevLineThick.nLet && LineThick.nLet > 5 )
 {
   int nl=0,nb=0;
   for(i=0;i<nWord;i++)
   {
           if( WordThick[i].height <= MIN_HEIGHT )
                   continue;

           if(WordThick[i].bold==P2_BOLD) nb++;
           else if(WordThick[i].bold==P2_LIGHT) nl++;
   }
   if(nl==0 && nb>0 )
   {
      LineThick.bold=P2_BOLD;
          if( corRet==P2_BOLD &&  p2_active && LineThick.nLet > MIN_SIZE_LINE)
                  acceptLineBold=MIN(acceptLineBold,LineThick.thick);
   }
   else  if(nl>0 && nb==0 )
   {
     LineThick.bold=P2_LIGHT;
         if( corRet==P2_LIGHT && p2_active && LineThick.nLet > MIN_SIZE_LINE)
                 acceptLineLight=MAX(acceptLineLight,LineThick.thick);
   }
   else LineThick.bold=0;
   prevLineThick=LineThick;
 }

 // запомним статистику по столбцу колонки
 if( lineAttr.tab_column > 0 &&
	 lineAttr.tab_column <= MAXCOL &&
	 lineAttr.tab_number > 0  &&
     lineAttr.tab_number <= MAXTAB
   )
 {
	 int nTab=lineAttr.tab_number-1;
	 int nCol=lineAttr.tab_column-1;
	 int nRow = lineAttr.tab_row-1;

	 if(p2_active && ( corRet == P2_BOLD || corRet == P2_LIGHT ) )
	 {
	  if(  thickCol[nTab][nCol].nThick != 0  )
	  {
       thickCol[nTab][nCol].nThick= (uchar)LineThick.thick;
       thickCol[nTab][nCol].bold=    corRet;
	  }

	  // запомним последнее по строке
	  if( nRow >=0 && nRow < MAXROW &&
		  thickRow[nTab][nRow].nThick != 0
		 )
	  {
		  thickRow[nTab][nRow].nThick= (uchar)LineThick.thick;
          thickRow[nTab][nRow].bold = corRet;
	  }
	 }
	 else if(!p2_active)
	 {
		 if(thickCol[nTab][nCol].minThick==0 ||
			LineThick.thick < thickCol[nTab][nCol].minThick
		   )
          thickCol[nTab][nCol].minThick=(uchar)LineThick.thick;
		 if( LineThick.thick > thickCol[nTab][nCol].maxThick )
          thickCol[nTab][nCol].maxThick=(uchar)LineThick.thick;

		 thickCol[nTab][nCol].nLet  += LineThick.nLet;
		 thickCol[nTab][nCol].thick += LineThick.nLet*LineThick.thick;


		 // add rows info
		 if( nRow >= 0 && nRow < MAXROW )
		 {
		  if(thickRow[nTab][nRow].minThick==0 ||
			LineThick.thick < thickRow[nTab][nRow].minThick
		   )
          thickRow[nTab][nRow].minThick=(uchar)LineThick.thick;

		  if( LineThick.thick > thickRow[nTab][nRow].maxThick )
           thickRow[nTab][nRow].maxThick=(uchar)LineThick.thick;

		  thickRow[nTab][nRow].nLet  += LineThick.nLet;
		  thickRow[nTab][nRow].thick += LineThick.nLet*LineThick.thick;
		 }
	 }
 }

 if( !p2_active )
 {
  for(i=0;i<nWord;i++)
  {
          if( WordThick[i].height > MIN_HEIGHT )
                 GistThickness[MIN(WordThick[i].thick,MAXGISTHICK-1)]++;
  }
 }

#ifdef _PRINT_BOLD_
 if( p2_active )
         fbold=fopen("mybold.txt","at");
 else
         fbold=NULL;

 if(fbold)
 {
   int ii;

   for(i=0;i<nWord;i++)
   {
       if(WordThick[i].bold !=P2_BOLD )
                   continue;

       for(ii=0,first=WordThick[i].first; first && first!=WordThick[i].last; first=CSTR_GetNext(first),ii++ )
           {
                CSTR_GetCollection(first,&rv);
                snapText[ii]=rv.Alt[0].Code;
           }
       snapText[ii]=0;
       fprintf(fbold,"%s\n",snapText);
	   if(lineAttr.tab_column)
	   {
		  fprintf(fbold,"tab=%d row=%d col=%d thick=%d\n",
			  lineAttr.tab_number,lineAttr.tab_row,
			  lineAttr.tab_column,
			  WordThick[i].thick);
	   }

   }
   fclose(fbold);
 }
#endif

 return 1;
}
/////////////////////////
static int FindBounds(int *val,int size)
{
    int i;
    int minVal;
    int maxVal;
    int bestMin=0,bestMax=0;
    int promMin,promMax;

    BoundLIGHT=BoundBOLD=0;

    minVal=MAX(pageLight,pageLineLight);

        // могут плавать границы pageLight,...
	while( minVal > 1 && val[minVal-1] && val[minVal-1] < val[minVal] )
       minVal--;

    maxVal=MIN(pageBold,pageLineBold);

    if( maxVal <= minVal)  // 25.11.99
		maxVal=minVal+1;

    while(maxVal < size-1 &&  val[maxVal+1] && val[maxVal+1] < val[maxVal])
       maxVal++;

    if( minVal == 0  && maxVal == MAXTHICK )
                return 0;

    if(minVal!=0)
        {
         for( i=minVal+1;i<size;i++)
         {
                    if( val[i] > val[minVal]*2 )
                                break;
                        if( val[i] < val[minVal] )
                                minVal=i;
         }

         for(i=minVal-2,bestMin=minVal-1;i>0;i--)
         {
                  if(val[i] > val[bestMin])
                         bestMin=i;
         }
        }

     if(maxVal!=MAXTHICK)
        {
         maxVal=MIN(maxVal,size-1);
         for( i=maxVal-1;i>0;i--)
         {
                        if(val[i] > val[maxVal]*2 )
                                break;
                        if( val[i] < val[maxVal])
                                maxVal=i;
         }

         for(i=maxVal+2,bestMax=maxVal+1;i<size;i++)
         {
                 if(val[i] > val[bestMax])
                         bestMax=i;
         }
        }

        if( minVal==0 )
        {
                minVal=maxVal-1;
                for(i=maxVal-2;i>0;i--)
                {
                        if(val[i]>val[minVal])
                                minVal = i;
                }

                // есть холмик ?
                if( val[minVal] <= 1 )
                {
                        for(;minVal>0;minVal--)
                                if( val[minVal] == 0 )
                                        break;
                        BoundLIGHT =  BoundBOLD  = minVal;
                        return 1;
                }

                bestMin=minVal;

                for( i=minVal+1;i<size;i++)
                {
                        if(val[i] > val[minVal]*2)
                                break;
                        if(val[i] < val[minVal])
                minVal  = i;
                }
        }

        if( maxVal== MAXTHICK)
        {
                maxVal=MIN(size-1,minVal+1);
                for(i=maxVal+1;i<size;i++)
                {
                        if(val[i]>val[maxVal])
                                maxVal=i;
                }

                // есть холмик ?
                if( val[maxVal] <= 1 )
                {
					// болды размазаны
                 if( val[bestMin]> 10 && val[maxVal] == 1 && val[minVal] == 0)
				 {
                   int sum=1,ss,mimi;
                   for( ss=maxVal-1;ss>minVal;ss--)
					   if( val[ss]) sum++;
					   else break;
                   mimi=ss;
				   for( ss=maxVal+1;ss<size;ss++)
					   if( val[ss]) sum++;
					   else break;
                   if(sum > 4 && bestMin+POROG_INTERLINE < maxVal )
				   {
                      BoundLIGHT = minVal;
					  BoundBOLD  = mimi;
					  return 2;
				   }
				 }
#ifdef _TEST_THIN_
				 // пропустили начало?
                 for(promMin=i=bestMin+1;i<maxVal;i++)
				 {
                      if(val[i]>val[promMin]*2)
						  break;
					  if(val[i] < val[promMin])
						  promMin=i;
				 }
				 for(i=promMax=promMin;i<maxVal;i++)
				 {
					 if(val[i]>val[promMax])
						 promMax=i;
				 }

				 if( promMax >= bestMin+10 &&
					 val[promMax] > 10 &&
                     val[bestMin] > 10 &&
					 val[bestMin] > val[promMin]*8 &&
                     val[promMax] > val[promMin]*8
				   )
				 {
                     BoundLIGHT = BoundBOLD  = promMin;
                     return 1;
				 }
#endif
				 // nothing
                 for(;maxVal<size;maxVal++)
                  if(val[maxVal]==0) break;
                 BoundLIGHT =   BoundBOLD  = maxVal;
                 return 1;
                }

                bestMax=maxVal;

                for( i=maxVal-1;i>0;i--)
                {
                        if(val[i] > val[maxVal]*2 )
                                break;
                        if( val[i] < val[maxVal])
                                maxVal=i;
                }
        }


        if(minVal > maxVal)
        {
                minVal=MAX(pageLineLight,pageLight);
                maxVal=MIN(pageLineBold,pageBold);

                if( minVal==0 || maxVal== MAXTHICK ||
                        minVal >= maxVal
                  )
                    return 0;

        for(i=minVal;i>0;i--)
                        if(val[i-1]<val[i]) break;
        bestMin=i;
                for(i=bestMin;i<maxVal;i++)
                        if(val[i]>val[i+1]) break;
        bestMin=i;

                for(i=maxVal+1;i<size;i++)
                        if(val[i-1] > val[i]) break;
        bestMax=i-1;
                for(i=bestMax;i>bestMin;i--)
                        if(val[i] > val[i-1]) break;
        bestMax=i;

                if( bestMax <= minVal ||
                        bestMin >= maxVal ||
                        bestMin+POROG_INTERLINE > bestMax
                 )
                 return 0;

                promMin=bestMin+1;
                for(i=bestMin+2;i<bestMax;i++)
                        if(val[i]<val[promMin])
                                promMin=i;

        if(val[promMin] +5 < val[bestMin] &&
           val[promMin] +5 < val[bestMax]
                  )
                {
                        BoundLIGHT = promMin;
                BoundBOLD  = promMin;
                        return 2;
                }

                return 0;
        }

        if(bestMin+POROG_INTERLINE > bestMax)
                return 1;

        if( val[bestMin] >= val[bestMax] )
        {
			promMin=promMax=minVal;
            for(i=minVal+1;i<=maxVal;i = promMin + 1 )
            {
				for( promMax =i; promMax < maxVal; promMax++ )
				{
					if( val[promMax+1] < val[promMax] )
							break;
				}

				if( val[promMax] > val[bestMax] &&
						val[promMax] > 2*val[promMin]  &&
						promMax > bestMin+POROG_INTERLINE
					)
				{
					 bestMax=promMax;
					 maxVal=promMin;
	                 break;
				}

				for( promMin = promMax+1; promMin < maxVal; promMin++ )
				{
					if( val[promMin+1] > val[promMin] )
							break;
				}

				if( val[promMax] >= val[bestMin] )
						bestMin = promMax;

				// иная точка раздела ?
				if( val[bestMin] > val[promMin]*2 )
					minVal = promMin;

			}
        }
        else
        {
           promMin=promMax=maxVal;
           for(i=maxVal-1;i>=minVal;i = promMin-1 )
           {
				for( promMax =i; promMax > minVal; promMax-- )
				{
					if( val[promMax-1] < val[promMax] )
							break;
				}

				if( val[promMax] > val[bestMin] &&
					val[promMax] > 2*val[promMin]  &&
					promMax < bestMax - POROG_INTERLINE
				  )
				{
					 bestMin=promMax;
					 minVal=promMin;
	                 break;
				}

				for( promMin = promMax - 1; promMin > minVal; promMin-- )
				{
					if( val[promMin - 1] > val[promMin] )
							break;
				}

				// расширяем область
				if( val[promMax] >= val[bestMax] )
						bestMax = promMax;

				// иная точка раздела ?
				if( val[bestMax] > val[promMin]*2 )
					maxVal = promMin;

            }
        }

        // есть прогиб ?
    if(  val[bestMax]*2 < val[maxVal] )
        {
                // all - to light
          for(;bestMax<size;bestMax++)
                          if(val[bestMax]==0) break;

          BoundLIGHT =
              BoundBOLD  = bestMax;
                  return 1;
        }
        // есть прогиб ?
    if(   val[bestMin]  < val[minVal]*2 )
        {
                // all - to bold
          for(;bestMin>0;bestMin--)
                          if(val[bestMin]==0) break;

          BoundLIGHT =
              BoundBOLD  = bestMin;
                  return 1;
        }

        if(minVal < maxVal)
        {
                for(i=minVal;i<=maxVal;i++)
                        if(val[i]==0) {minVal=i;break;}
                for(i=maxVal;i>=minVal;i--)
                        if(val[i]==0) {maxVal=i;break;}

						// 31.01,00
                for(i=minVal+1,promMax=minVal; i< maxVal;i++)
				{
                        if(val[i] > val[promMax] )
							promMax = i;
				}
				if( promMax > minVal)
				{
/*
					if( val[promMax] > MIN(val[bestMin],val[bestMax]) )
					{
                       if(val[minVal] > val[maxVal])
						   minVal = maxVal;
					   else if(val[minVal] < val[maxVal])
                           maxVal = minVal;
					}
*/
                    if( val[promMax] > val[bestMin] && val[minVal] > val[maxVal])
						   minVal = maxVal;
				    else if( val[promMax] > val[bestMax] && val[minVal] < val[maxVal])
                           maxVal = minVal;
				}
        }

        BoundLIGHT = minVal;
        BoundBOLD  = maxVal;

   return 2;
}
//////////////////
static int TestWidth(int *stats,int size,int midThick)
{
		    int start,end;
			int j;
			int porog = POROG_INTERLINE;

			for(j=midThick-1,start=midThick;j>0;j--)
				if( stats[j]>stats[start] )
					start = j;

            for(j=midThick+1,end=midThick;j<size;j++)
				if( stats[j]>stats[end] )
					end = j;

            if( end - start <= porog )
			  midThick = 0;

			return midThick;
}
//////////
static int TestStatsMinMax(int *stats, int *mins,int *maxs,
						   int maxLight, int minBold)
{
	int midThick;

// all intersect ?
//		if( maxLight <= minBold )
//			return 0;

		midThick = Progib(stats,MAXTHICK,0);

		if( midThick == 0 )
			return 0;

		if( stats[midThick] > 0 )
			return 0;

		midThick = TestWidth(stats,MAXTHICK,midThick);
		if( midThick == 0 )
			return 0;

		maxLight = Progib(mins,MAXTHICK,0);
		if( maxLight <= 0 )
			return 0;

		// сомнительный минимум ?
		if( mins[maxLight] > 0 )
			return 0;

		minBold = Progib(maxs,MAXTHICK,0);
		if( minBold <= 0 )
			return 0;

		if( maxs[minBold] > 0 )
			return 0;

        return midThick;
}
///////////
static void TestColumnsThick(void)
{
	int i,j;
	int midThick;
	int minBold;
	int maxLight;
	Bool32 isProgib;
	int32_t allThick;
	int32_t allNum;
	int32_t commonThick;
	uchar commonBold=0;


	for(i=0;i<MAXTAB;i++)
	{
	    int stats[MAXTHICK];
		int mins[MAXTHICK];
		int maxs[MAXTHICK];

		minBold = MAXTHICK;
		maxLight=0;
		midThick = 0;

		memset(mins,0,MAXTHICK*sizeof(int));
		memset(maxs,0,MAXTHICK*sizeof(int));
		memset(stats,0,MAXTHICK*sizeof(int));

		allThick = 0;
		allNum   = 0;

		for(j=0;j<MAXCOL;j++)
		{
		  if( thickCol[i][j].maxThick == 0 )
			 continue;

		  allThick += thickCol[i][j].thick;
		  allNum   += thickCol[i][j].nLet;

		  if(  thickCol[i][j].nLet > 0 )
			  thickCol[i][j].thick /= thickCol[i][j].nLet;


		  maxLight = MAX(maxLight,thickCol[i][j].minThick);
		  minBold  = MIN(minBold,thickCol[i][j].maxThick);

		  maxs[MIN(MAXTHICK-1,thickCol[i][j].maxThick)]++;
		  mins[MIN(MAXTHICK-1,thickCol[i][j].minThick)]++;

		  stats[MIN(MAXTHICK-1,thickCol[i][j].thick)]++;
		}


		commonThick = allThick/MAX(1,allNum);

		midThick = TestStatsMinMax(stats,mins,maxs,maxLight,minBold);

		if( midThick > 0)
		{

			for(j=0;midThick+j<MAXTHICK;j++)
				if( stats[midThick+j]>0 ) break;

            midThick += (j>>1);

			isProgib = TRUE;

			if( commonThick < midThick -1 )
				commonBold = P2_LIGHT;
			else if( commonThick > midThick + 1 )
				commonBold = P2_BOLD;
			else
				commonBold = 0;
		}

		else
		{
			isProgib = FALSE;

            if( BoundLIGHT && BoundBOLD )
				midThick = (BoundBOLD + BoundLIGHT)/2;
			else if(pageLight !=0 && pageBold != MAXTHICK)
				midThick = (pageBold + pageLight)/2;
            else
				midThick = (POROG_BOLD + POROG_LIGHT)/2;

			if( commonThick <= midThick )
				commonBold = P2_LIGHT;
			else
				commonBold = P2_BOLD;
		}


		for(j=0;j<MAXCOL;j++)
		{
         if( thickCol[i][j].maxThick == 0 )
			 continue;

#ifdef _OLD_TABL_VERSION_

         if(pageLight==0 || pageBold==MAXTHICK)
		 {
//           if(BoundLIGHT == 0 && BoundBOLD == 0 )
           if( thickCol[i][j].minThick >= POROG_BOLD )
			   thickCol[i][j].bold = P2_BOLD;
		   else
			   thickCol[i][j].bold = P2_LIGHT;
		   continue;
		 }

		 if( thickCol[i][j].maxThick  > thickCol[i][j].minThick +  POROG_ONECOL)
		 {
			 thickCol[i][j].maxThick = 0;
			 continue;
		 }

		 k=(thickCol[i][j].minThick-pageLight)-
		  (pageBold - thickCol[i][j].maxThick);

		 if( k < 0 )
			thickCol[i][j].bold = P2_LIGHT;
		 else if( k > 0 )
			thickCol[i][j].bold = P2_BOLD;
		 else  // mark - no solution
           thickCol[i][j].maxThick = 0;

#else
         if( BoundLIGHT && BoundBOLD )
		 {
           if( thickCol[i][j].minThick >= BoundLIGHT &&  thickCol[i][j].maxThick > BoundBOLD)
		   {
			   thickCol[i][j].bold = P2_BOLD;
			   continue;
		   }
		   if( thickCol[i][j].minThick < BoundLIGHT &&  thickCol[i][j].maxThick <= BoundBOLD)
		   {
			   thickCol[i][j].bold = P2_LIGHT;
		       continue;
		   }
		 }

// поставим основную жирность
         if( !isProgib )
		 {
			 // поставим - скорее всего жирность одинакова
             thickCol[i][j].bold = commonBold;

			 if( thickCol[i][j].maxThick  > thickCol[i][j].minThick +  POROG_ONECOL)
			      thickCol[i][j].nThick = MAX(1,MIN(255,thickCol[i][j].thick));
			 continue;
		 }

         if(  thickCol[i][j].thick > midThick + 2 )
		 {
			 thickCol[i][j].bold = P2_BOLD;

             if( thickCol[i][j].minThick < midThick - 1 &&
				 thickCol[i][j].maxThick  > thickCol[i][j].minThick +  POROG_ONECOL
			   )
			   thickCol[i][j].nThick = MAX(1,MIN(255,thickCol[i][j].thick));
			 continue;
		 }
		 if(  thickCol[i][j].thick < midThick - 2 )
		 {
			 thickCol[i][j].bold = P2_LIGHT;
			 if( thickCol[i][j].maxThick > midThick + 1 &&
				 thickCol[i][j].maxThick  > thickCol[i][j].minThick +  POROG_ONECOL
			   )
			   thickCol[i][j].nThick = MAX(1,MIN(255,thickCol[i][j].thick));
			 continue;
		 }

		 if( thickCol[i][j].maxThick  > thickCol[i][j].minThick +  POROG_ONECOL)
		 {
			 thickCol[i][j].nThick = MAX(1,MIN(255,thickCol[i][j].thick));
		 }

		 thickCol[i][j].bold = commonBold;
#endif
		}

	}
}
//////////
static int GetNumGreater(int *hhh,int size,int start)
{
	int sum,i;

	for(sum=0,i=start; i < size;i++ )
		sum+=hhh[i];

	return sum;
}
/////////////////
static int Progib(int *hhh,int GreyLev, int numAccord )
{
 int  i;
 int  lefth=-1;
 int  prommin = -1;
 uchar spusk = 0;
 int  numPoi;
 int  dimPlato;
 int  bestProgib,bestPlato;
 int  best;

 best = 0;
 for(numPoi=0,dimPlato=0,lefth=-1,i=1;i<GreyLev;i++)
  {

	if(hhh[i] >= hhh[i-1])
	  {
	   if(spusk == 0 )
	           continue;
	   spusk = 0;                 /*(i-1) - local minimum point */
	   if(lefth == -1 )
	        continue;

       if( prommin == -1 || hhh[i-1] <= hhh[prommin] )
	     {
		   int j,k;
		   // count the same
		   for(j=i, k=1;j<GreyLev;j++,k++)
		     if( hhh[j] != hhh[i-1] ) break;

           if(numAccord)
			   k = 100 - abs(numAccord - GetNumGreater(hhh,GreyLev,j));

		   if( prommin == -1 ||
			   hhh[i-1] < hhh[prommin] ||
		       hhh[i-1] == hhh[prommin] && k >= dimPlato
             )
		   {
		     prommin = i-1;
			 dimPlato = k;
		   }

           i=MAX(i,j-1);
		   continue;
		 }
	  }
	else
	  {
		if(spusk ) continue;
		spusk = 0xFF;                /* local maximum */
		if(lefth == -1 )
		{
		   if( hhh[i-1] > 0 )
		     lefth = i-1;
		   continue;
		}

	    if( prommin!=-1 && hhh[i-1] > hhh[prommin] && i-1-lefth > 2)
	    {
			int progib = MIN(hhh[i-1],hhh[lefth])-hhh[prommin];

			if( !best || progib	> bestProgib ||
				progib == bestProgib && dimPlato >= bestPlato )
			{
             best = prommin;
			 bestProgib = progib;
			 bestPlato = dimPlato;
			}

		}
		  /* right higher then left */
		if(hhh[i-1] >= hhh[lefth])
		{
			lefth=i-1;     /* global maximum left */
			prommin=-1;
			dimPlato = 0;
		}
	}   // end else
  }
		 // study last point
 if( prommin != -1  && lefth != -1 && hhh[GreyLev-1] > hhh[prommin] )
  {
	 int progib = MIN(hhh[GreyLev-1],hhh[lefth])-hhh[prommin];

	 if( !best || progib	> bestProgib ||
	 	 progib == bestProgib && dimPlato >= bestPlato )

             best = prommin;
  }

 return(best);
}
/*************************/
static void AddRowBold(int i,int midThick)
{
    int pageThick;
	int j;

	if( midThick <= 0 )
	{
	 if( BoundLIGHT && BoundBOLD )
				pageThick = (BoundBOLD + BoundLIGHT)/2;
	 else if(pageLight !=0 && pageBold != MAXTHICK)
				pageThick = (pageBold + pageLight)/2;
     else
				pageThick = (POROG_BOLD + POROG_LIGHT)/2;

	 midThick = pageThick;
	}


	for(j=0;j<MAXROW;j++)
	{
		 if( thickRow[i][j].maxThick == 0 )
			 continue;

		// среднее значение для строки таблицы
		 if( thickRow[i][j].bold == 0 )
		 {
			thickRow[i][j].nThick = (uchar)MAX(1,MIN(255,thickRow[i][j].thick));
			if( thickRow[i][j].thick <= midThick )
				thickRow[i][j].bold = P2_LIGHT;
			else
				thickRow[i][j].bold = P2_BOLD;
		 }
	}
}
/////////////////////

static void TestRowsThick(void)
{
	int i,j;
	int minBold,maxLight;
	int mins[MAXTHICK];
	int maxs[MAXTHICK];
	int stats[MAXTHICK];
	int midThick;

	for(i=0;i<MAXTAB;AddRowBold(i,midThick),i++)
	{
		minBold = MAXTHICK;
		maxLight=0;
		midThick = 0;

		memset(mins,0,MAXTHICK*sizeof(int));
		memset(maxs,0,MAXTHICK*sizeof(int));
		memset(stats,0,MAXTHICK*sizeof(int));

		for(j=0;j<MAXROW;j++)
		{
		  if( thickRow[i][j].maxThick == 0 )
			 continue;

          if(  thickRow[i][j].nLet > 0 )
			  thickRow[i][j].thick /= thickRow[i][j].nLet;

		  maxLight = MAX(maxLight,thickRow[i][j].minThick);
		  minBold  = MIN(minBold,thickRow[i][j].maxThick);

		  maxs[MIN(MAXTHICK-1,thickRow[i][j].maxThick)]++;
		  mins[MIN(MAXTHICK-1,thickRow[i][j].minThick)]++;

		  stats[MIN(MAXTHICK-1,thickRow[i][j].thick)]++;
		}

		// all intersect ?
		if( maxLight <= minBold )
			continue;

		maxLight = Progib(mins,MAXTHICK,0);
		if( maxLight <= 0 )
			continue;

		// сомнительный минимум ?
		if( mins[maxLight] > 0 )
			continue;


		minBold = Progib(maxs,MAXTHICK,GetNumGreater(mins,MAXTHICK,maxLight+1));
		if( minBold <= 0 )
			continue;

		if( maxs[minBold] > 0 )
			continue;

		midThick = Progib(stats,MAXTHICK,GetNumGreater(mins,MAXTHICK,maxLight+1));

		if( midThick > 0 )
		  midThick = TestWidth(stats,MAXTHICK,midThick);

		if( midThick <= 0 )
			continue;

		for(j=0;j<MAXROW;j++)
		{
         if( thickRow[i][j].maxThick == 0 )
			 continue;

		 if( thickRow[i][j].maxThick > minBold &&
			 thickRow[i][j].minThick > maxLight &&
			 thickRow[i][j].thick > midThick
		   )
		 {
			  thickRow[i][j].bold = P2_BOLD;
		 }

         else  if( thickRow[i][j].maxThick < minBold &&
			 thickRow[i][j].minThick < maxLight &&
			 thickRow[i][j].thick < midThick
			)
		 {
			  thickRow[i][j].bold = P2_LIGHT;
		 }

		}  // end j - rows



	}  // end i - tables

}
/////////////////
static uchar SetBoldByTable(int nThick, int midThick,
							uchar colBold,
							int thick,
							int minVal, int maxVal)
{
// есть предыдущий

		// сравним с предыдущей (средней) строкой в столбце
	  if( colBold && abs(thick-nThick) < POROG_ONECOL ||
          colBold == P2_LIGHT &&  thick <= nThick ||
          colBold == P2_BOLD  &&  thick >= nThick
	    )
		return colBold;

	  if( minVal + POROG_ONECOL  >= maxVal )
				 // есть общность по ячейке
	  {
                if( colBold == P2_BOLD &&
					thick + POROG_ONECOL < nThick &&
					thick <= midThick +1
				   )
                  return P2_LIGHT;

				if( colBold == P2_LIGHT &&
					thick > POROG_ONECOL + nThick &&
					thick >= midThick - 1
				   )
                  return P2_BOLD;

	  }

	  else
	  {
                if( colBold == P2_BOLD &&
					maxVal + POROG_ONECOL < nThick &&
					thick <= midThick + 1
				   )
                  return P2_LIGHT;

				if( colBold == P2_LIGHT &&
					minVal > POROG_ONECOL + nThick &&
					thick >= midThick - 1
				   )
                  return P2_BOLD;

	  }


	  return 0;
}
//////////////////////
static void TestAccordRowsColumns(void)
{
	int i,j;
	uchar bold;

	for(i=0;i<MAXTAB;i++)
	{
		bold = 0;
		for(j=0;j<MAXCOL;j++)
		{
            if( thickCol[i][j].maxThick == 0)
				continue;
			if( thickCol[i][j].nThick == 0 )
				bold |= thickCol[i][j].bold;
		}
		if( !bold )
			continue;

		for(j=0;j<MAXROW;j++)
		{
			if( thickRow[i][j].maxThick == 0)
				continue;
			if( thickRow[i][j].nThick )
				continue;

			if( thickRow[i][j].bold != bold )
				thickRow[i][j].nThick = (uchar)MAX(1,MIN(255,thickRow[i][j].thick));
		}
	}
}
//////////////
