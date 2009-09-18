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

#include "minmax.h"

#define NO_SIZES0
//
//  Получение размеров - стандартная высота больших.маленьких букв
//                     стандартная ширина
//  Получение размеров любого символа
//        ( размеров большой,маленькой букв)
//
//  ClearStat();  начать работу, закончить работу,
//                очистить статистику,освободить память
//
//  AddStat(int name,int width,int height,int valid);
//      добавить символ с именем name,размерами,валидностью
//      в общую статистику
//   return - сколько всего символов стало
//
//  GetCommonStat(int *sizes,int *probs);
//      получить статистику
//      sizes[0] >0 - высота большой буквы
//      probs[0] - "процент" (0-255) (оценка надежности)
//      sizes[1] >0 - высота маленькой буквы
//      probs[1] - "процент" (0-255) (оценка  надежности)
//      sizes[2] >0 - ширина
//      probs[2] - "процент" (0-255) (оценка  надежности)
//
//  AddStatLetter(int name,int width,int height,int valid);
//      добавить символ с именем name,размерами,валидностью
//      в статистику для символа name
//   return - сколько всего символов name стало
//
//  GetLetStat(int name,int *sizes,int *probs);
//      получить статистику для символа
//  return < 0 - error ( no memory )
//         >=0 - сколько символов с именем name использовалось
//
//      sizes[0] >0 - высота большой буквы
//      sizes[1] >0 - ширина большой буквы
//      probs[0] - "процент" (0-255) (оценка  надежности)
//      sizes[2] >0 - высота маленькой буквы
//      sizes[3] >0 - ширина маленькой буквы
//      probs[1] - "процент" (0-255) (оценка  надежности)
//
//
//  Пример использования
/*
  int comSize[4],prob[4];

    ClearStat();  // начать работу, очистить статистику
    for(;;)
	{
	  if(NextSymbol(&name,&width,&height,&valid) == 0)  break; // условно
      NumAll=AddStat(int name,int width,int height,int valid);
	  // if(NumAll > ... ) break;
      NumLet=AddStatLetter(int name,int width,int height,int valid);
	}


    GetCommonStat(comSize,prob);
	if( comSize[0] == 0) printf("Мало данных");
    else if( comSize[1] == 0) printf("Высота =%d,ширина=%d\n",
	       comSize[0],comSize[2]);
	else printf("Высота большой=%d,высота маленькой=%d,ширина=%d\n",
	       comSize[0],comSize[1],comSize[2]);

    for(i=1;i<256;i++)
	{
	 printf("\nСимвол %c ",(char)i);
	 j=GetLetStat(i,comSize,prob);
	 if( j< 0) printf("Error %d",j);
     if( comSize[0] == 0) printf("Мало данных");
     else if( comSize[2] == 0) printf("Высота =%d,ширина=%d\n",
	       comSize[0],comSize[1]);
	 else printf("Высота большой=%d,ширина большой %d;Высота маленькой=%d,ширина=%d",
	       comSize[0],comSize[1],comSize[2],comSize[3]);
    }

        // можно повторять при желании
    ...
	{
     NumLet=AddStatLetter(int name,int width,int height,int valid);
	}
	....
	GetLetStat(int name,int *sizes4,int *ocenka2);
	....

    ClearStat(); // все очистить, освободить память
*/
//

#define MAX_HEIGHT 128
#define MAX_WIDTH  256
#define POROG_HEIGHT 5
#define POROG_WIDTH  2
#define MIN_STAT     2  // minimal need for statistic
#define MIN_STAT_ONE 1  // need for one letter stat.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "leo.h"
#include "std.h"


static int allHeights[MAX_HEIGHT];
static int allWidthes[MAX_WIDTH];
static int allCount=0;
static int letCount[256];

// 6.10.1998 - Nick
static int allMetka[MAX_HEIGHT];
#define METKA_DIGIT  1
#define METKA_LETTER 2
#define METKA_BIG    4
#define METKA_SMALL  8
//

 #define INIT_SIZE 4096
 #define STEP_SIZE 256
 int allSize=0;
 int allLetter=0;
 typedef struct tagLetInfo {
	 int name;
     int width;
	 int height;
	 int cluster;
 } LetInfo;
 LetInfo *letInfo=NULL;

//////////////////
LEO_FUNC(void) LEO_ClearStat(void)
{
	memset(allHeights,0,sizeof(allHeights));
    memset(allMetka,0,sizeof(allMetka));
	memset(allWidthes,0,sizeof(allWidthes));
	allCount=0;

	memset(letCount,0,256*sizeof(int));

	if(letInfo) free(letInfo);
    letInfo=NULL;
	allSize=0;
	allLetter=0;
}
////////////////////
//            russian ДдЦцЩщ
static char *nonStandard="\x84\xA4\x96\xE6\x99\xE9";
LEO_FUNC(int32_t) LEO_AddStat(int32_t name,int32_t width,int32_t height,int32_t valid)
{
	  // add only good
	if( ( valid & LEO_VALID_FINAL ) == 0)
		 return allCount;
    //name = stdAnsiToAscii((uchar)name);
	if( width  < 0 || width  >= MAX_WIDTH ||
		height < 0 || height >= MAX_HEIGHT   )
          return allCount;

	if( name >= '0' && name <= '9' )
	{
       allHeights[height]++;
       allMetka[height] |= METKA_DIGIT;  // 6.10.98
	   allWidthes[width]++;
	   allCount++;
	}

    else if( name >= 'A' && name <= 'z' ||
		name >= 128 && strchr(nonStandard,name)==NULL )
    {
       allHeights[height]++;

       allMetka[height] |= METKA_LETTER;  // 6.10.98
       if( name == 160 || name == 165 )
          allMetka[height] |= METKA_SMALL;  // 6.10.98

	   allWidthes[width]++;
	   allCount++;
	}

	return allCount;
}
///////////////////////////
//
//  output heights of big,small letters
//  output width
//   if  sizes[i] = 0 -> not found
//
//  sizes[0]=heightBig sizes[1]=heightSmall
//  size[2]=width
//
//  return - how many analize
//
int GetStat(int *sizes,int allCount,int *allWidthes,int *allHeights,int *ocenka)
{
	int i,j;
	int bWidth;
	int bSize;
	int bSizeLeft;
	int sizeBig,sizeLit;

	memset(sizes,0,3*sizeof(int));
	memset(ocenka,0,3*sizeof(int));
	if( allCount < MIN_STAT ) return allCount;

	for(i=1,bWidth=0;i<MAX_WIDTH;i++)
	 if(allWidthes[i] > allWidthes[bWidth]) bWidth=i;

	for(i=1,bSize=0;i<MAX_HEIGHT;i++)
	 if(allHeights[i] > allHeights[bSize]) bSize=i;

	for(j=bSize-POROG_HEIGHT;j>1;j--) if( allHeights[j] > allHeights[j+1] ) break;
	for(bSizeLeft=j,j--;j>1;j--)
	 if(allHeights[j] > allHeights[bSizeLeft]) bSizeLeft=j;

	for(j=bSize+POROG_HEIGHT;j<MAX_HEIGHT;j++) if( allHeights[j] > allHeights[j-1] ) break;
	for(;j<MAX_HEIGHT;j++)
	 if(allHeights[j] > allHeights[bSizeLeft]) bSizeLeft=j;

    sizeLit=0;
	if( allHeights[bSizeLeft] < MIN_STAT )
	  sizeBig=bSize ;
	else
	{
     sizeBig=bSize;
	 if(bSize < bSizeLeft &&
        ( allMetka[bSize]& METKA_LETTER )   &&          // 6.10.98
          (( allMetka[bSize]& METKA_DIGIT) == 0 )  // 6.10.98
       )
       {sizeLit=bSize;sizeBig=bSizeLeft;}
	 else if (  bSizeLeft < bSize &&
               ( allMetka[bSizeLeft]& METKA_LETTER ) &&        // 6.10.98
               (( allMetka[bSizeLeft]& METKA_DIGIT) == 0 )     // 6.10.98
             )
      {sizeBig=bSize;sizeLit=bSizeLeft;}
	}

	sizes[0]=sizeBig;
	sizes[1]=sizeLit;
	sizes[2]=bWidth;

	if( sizeBig > 0)
		ocenka[0]=((allHeights[sizeBig-1]+allHeights[sizeBig]+
		  allHeights[sizeBig+1])*255) / allCount;
	if( sizeLit > 0)
		ocenka[1]=((allHeights[sizeLit-1]+allHeights[sizeLit]+
		  allHeights[sizeLit+1])*255) / allCount;
	if( bWidth > 0)
		ocenka[2]=((allWidthes[bWidth-1]+allWidthes[bWidth]+
		  allWidthes[bWidth+1])*255) / allCount;
	return allCount;
}
///////////////////
LEO_FUNC(int32_t) LEO_GetCommonStat(int32_t *sizes,int32_t *ocenka)
{
	return GetStat(sizes,allCount,allWidthes,allHeights,ocenka);
}
////////////////////


LEO_FUNC(int32_t) LEO_AddStatLetter(int32_t name,int32_t width,int32_t height,int32_t valid)
{

	if(name < 0 || name > 255 ) return 0;
//name = stdAnsiToAscii((uchar)name);
	  // add only good
	if( ( valid & LEO_VALID_FINAL ) == 0)
		 return letCount[name];

	if( width  < 0 || width  >= MAX_WIDTH ||
		height < 0 || height >= MAX_HEIGHT   )
          return letCount[name];

	if(letInfo==NULL)
	{
		letInfo=malloc(INIT_SIZE*sizeof(LetInfo));
		if(letInfo==NULL) return -1;
		allSize=INIT_SIZE;
		allLetter=0;
	}

	if( allLetter >= allSize )
	{void *tmp;
		tmp=realloc(letInfo,(allSize+STEP_SIZE)*sizeof(LetInfo));
		if( tmp==NULL) return -1;
		letInfo=tmp;
		allSize+=STEP_SIZE;
	}

	letInfo[allLetter].name  =name;
	letInfo[allLetter].width =width;
	letInfo[allLetter].height=height;
    letCount[name]++;
    allLetter++;

	return letCount[name];
}
//////////////
#define UNIQ_RUS "\x85\xa0\xa1\xa5"   // "Еабе"
#define HIGH_LET "\x90\x93\x94\xe0\xe3\xe4"   // "РУФруф"
static int ClusterAnalyze(int name,int count,int all,LetInfo *lInfo,
						  int *mysteck,
						  int NumClus,int *size,int *ocenka)
{
 int i,j,jj;
 int hei,wid;
 int *hh,*ww;    // cluster width,height

 if(NumClus <= 0) return 0;
 memset(mysteck,0,3*NumClus*sizeof(int));
 ww=mysteck+NumClus;
 hh=ww+NumClus;

 for(i=0;i<all;i++)
 {
   if(lInfo[i].name!=name) continue;
   mysteck[(j=lInfo[i].cluster)]++;
   ww[j]+=lInfo[i].width;
   hh[j]+=lInfo[i].height;
 }

 // j - best cluster
 for(i=1,j=0;i<NumClus;i++) if(mysteck[i]>mysteck[j]) j=i;
 if(mysteck[j] < MIN_STAT_ONE ) return 0;  // bas statistics

 // get sizes in clusters
 for(i=0;i<NumClus;i++)
 {
   if(mysteck[i]<=0) continue;
   hh[i]=(hh[i]+((mysteck[i])>>1))/mysteck[i];
   ww[i]=(ww[i]+((mysteck[i])>>1))/mysteck[i];
 }

 hei=hh[j];
 wid=ww[j];

 // who is next ? - for standard russian only !
 // use dos-coding  "Еабе"  (АБ- by 130!)
 if( name >= 130 && strchr(UNIQ_RUS,name) == NULL)
 {
	for(i=0,jj=-1;i<NumClus;i++)
	{
	 if(i==j) continue;
	 if( mysteck[i] < MIN_STAT_ONE ) continue;  // ???
	 if( abs(hh[i]-hei) <= POROG_HEIGHT &&
		( strchr(HIGH_LET,name)==NULL || abs(ww[i]-wid) <= POROG_WIDTH ))
	      continue;
	 if(jj<0 || mysteck[i]>mysteck[jj]) jj=i;
	}
 }
 else jj=-1;

 if( jj<0  )
 {size[0]=hei;
  size[1]=wid;
  ocenka[0]=(mysteck[j]*255)/count;
  return 1;
 }

 if(hei > hh[jj] || hei==hh[jj]&&wid>=ww[jj])
 {
  size[0]=hei;
  size[1]=wid;
  ocenka[0]=(mysteck[j]*255)/count;
  size[3]=ww[jj];
  size[2]=hh[jj];
  ocenka[1]=(mysteck[jj]*255)/count;
 }
 else
 {
  size[2]=hei;
  size[3]=wid;
  ocenka[1]=(mysteck[j]*255)/count;
  size[1]=ww[jj];
  size[0]=hh[jj];
  ocenka[0]=(mysteck[jj]*255)/count;
 }

 return 2;
}
/////////////////
static int ClusterSet(int name,int all,LetInfo *lInfo,
						  int *mysteck)
{
 int i,j;
 int IsSame,NumSame,IsNew;
 int CurClus;
 int fir,last;
 int wid,hei;
 int porogWid=1;           // ???
 int porogHei=1;

 for(i=0,fir=-1;i<all;i++)
 {
	 if(lInfo[i].name!=name) continue;
	 lInfo[i].cluster=-1;
	 if(fir<0) fir=i;
	 last=i;
 }
 last++;

  // main circle
 CurClus=0;
 lInfo[fir].cluster=CurClus;
 for(i=fir,j=fir+1,IsSame=NumSame=0;;)
  {
    wid=lInfo[i].width;
    hei=lInfo[i].height;

	for(IsNew=-1;j<last;j++)
	{
     if(lInfo[j].name != name) continue;
	 if(lInfo[j].cluster >= 0 ) continue;    // already tested

	 if( abs(lInfo[j].width -wid) > porogWid ||
		 abs(lInfo[j].height-hei) > porogHei
		)
	 {	if( IsNew < 0 )  IsNew=j; }

	 else
	  {  lInfo[j].cluster=CurClus;
	     mysteck[NumSame++]=j;
	  }
	}  // end j

  if(IsNew < 0 ) break;  /* study all */
  if(IsSame >= NumSame )  // is from the same cluster ? - no
		{
		 CurClus++;
		 i=IsNew;
		 lInfo[i].cluster=CurClus;
		 j=IsNew+1;
		}
  else
	{i=mysteck[IsSame++];    // continue add to the same cluster
	 j=IsNew;
	}

  }    // end main circle

 return CurClus+1;  // how many clusters
}
///////////////
//
//  sizes4 - [0]=heiBig,[1]=widBig,[2]=heiSmall,[3]=widSmall
//
//int GetLetStat(int name,int *sizes4,int *ocenka2)
LEO_FUNC(int32_t) LEO_GetLetStat(int32_t name,int32_t *sizes4,int32_t *ocenka2)
{
int i;
int *mysteck;

    memset(sizes4,0,4*sizeof(int));
    memset(ocenka2,0,2*sizeof(int));

	if( name < 0 || name > 255 ) return 0;
//name = stdAnsiToAscii((uchar)name);
	 //  symbols not enouph ?
    if( letCount[name] < MIN_STAT_ONE ) return 0;
	if( letInfo == NULL ) return 0;

    mysteck=malloc(3*letCount[name]*sizeof(int));
	if(mysteck==NULL) return -1;
	// set clusters
	i=ClusterSet(name,allLetter,letInfo,mysteck);
	// looking for better
    i=ClusterAnalyze(name,letCount[name],allLetter,letInfo,mysteck,
		i,sizes4,ocenka2);

    free(mysteck);
	return letCount[name];
}
//////////

//;*****  h>w ==> p= (64*w)/h         ******
//;*****  h=w ==> p=64                ******
//;*****  h<w ==> p=128 - (64*h)/w    ******
typedef struct { int pmin,pmax;} PROP_STR;

static PROP_STR letters_prop_table_rus[256]={
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127, // 0x00
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127,
        1,127, // 0x01
        1,127,
        2, 34,// ! some italic
        1,127,// "
       36, 72,// #
       24, 48,// $
       36, 80,// %
       36, 84,// &
        1,127,// '
        7, 32,// (
        7, 32,// }
       52, 72,// *
       52, 76,// +
       20, 56,// ,
       80,120,// -
       32, 84,// .
       16,70 ,   // 2f
       30, 62,// 0
       10, 52,// 1
       30, 61,// 2 pape0 cursiv
       22, 61,// 3
       30, 61,// 4
       27, 61,// 5
       30, 61,// 6
       30, 61,// 7
       30, 61,// 8
       30, 63,// 9
        1,127,// :
        1,127,// ;
       19, 76,// <
        1,127,// =
       19, 76,// >
       27, 72,// ?       0x03
       44, 68,// @
       32, 80,// A
       36, 68,// B
       32, 72,// C
       36, 78,// D
       28, 72,// E
       26, 69,// F
       36, 72,// G
       32, 80,// H
        4, 52,// I
       12, 60,// J
       32, 80,// K
       20, 72,// L
       41, 88,// M
       32, 75,// N 80
       32, 74,// O      0x04
       32, 67,// P
       37, 72,// Q
       32, 76,// R
       30, 64,// S
       28, 76,// T
       34, 76,// U
       36, 76,// V
       38, 88,// W
       37, 76,// X
       32, 72,// Y
       36, 72,// Z
        8, 26,// [
        1,127,// back clash
        8, 26,// ]
        1,127,// ^
      112,124,// _      0x05
        1,127,// `
       36, 80,// a
       28, 62,// b
       32, 76,// c
       30, 64,// d
       36, 76,// e
       16, 50,// f
       28, 61,// g
       24, 64,// h
        4, 68,// i
        8, 42,// j
       24, 64,// k
        2, 48,// l
       48, 96,// m
       40, 88,// n
       36, 80,// o      0x06
       28, 64,// p
       28, 56,// q
       22, 72,// r
       30, 68,// s
        8, 55,// t
       36, 84,// u
       36, 80,// v
       46, 96,// w
       36, 80,// x
       28, 64,// y
       36, 72,// z
        8, 24,// {
        4, 52,// | stick from bI
        8, 24,// }
        1,127,// ~
        1,127,//        0x07f
       35, 75,// Ђ
       35, 74,// Ѓ
       33, 71,// ‚ jurnal 7 kegl
       28, 74,// ѓ futuris 10 kegl
       33, 69,// „
       35, 71,// …
       40, 90,// †
       33, 70,// ‡
       34, 75,// €
       35, 68,// ‰
       35, 75,// Љ
       37, 75,// ‹
       33, 88,// Њ
       33, 75,// Ќ 81 kudryash 14 kegl cursiv
       35, 85,// Ћ
       33, 80,// Џ
       33, 72,// ђ bodoni halfbold 12 kegl
       35, 75,// ‘
       37, 75,// ’
       35, 69,// “ lower bound corr from UFA stend
       32, 78,// ” UFA : Ўл«® 39,78
       37, 81,// •
       32, 67,// –
       32, 69,// —
       37, 97,//� changed
       32, 89,// ™
       38, 76,// љ  changed
       37, 86,// ›
       35, 69,// њ
       34, 69,// ќ
       37, 88,// ћ
       35, 75,// џ
       42, 75,//  
       32, 60,// Ў
       37, 75,// ў
       33, 84,// Ј
       42, 75,// ¤
       38, 75,// Ґ  7 kegl
       39, 98,// ¦
       37, 77,// § changed
       33, 75,// Ё
       35, 70,// ©
       41, 82,// Є changed 45,82 26-12-1994 for RUS_ENG
       45, 75,// «
       55, 75,// ¬
       33, 75,// ­
       41, 85,// ®
       33, 75,// Ї
        1,127,// °
       24, 58,// ±
       32, 68,// І
       32, 68,// і
       51, 80,// ґ
       44, 80,// µ
       40, 80,// ¶
       36, 80,// ·
       36, 80,// ё
       38, 90,// №
        2, 48,// є
        8, 40,// »
        2, 26,// ј
       27, 72,// Ѕ
        1,127,// ѕ
        1,127,// ї
        1,127,// А
        1,127,// Б
       51,100,// В serbian - n
        1,127,// Г
       51,110,// Д serbian - l
        1,127,// Е
       38, 88,// Ж
       37, 72,// З
        1,127,// И
        1,127,// Й
        1,127,// К
       51,100,// Л serbian - N
        1,127,// М
       51,110,// Н serbian - L
        1,127,// О
        1,127,// П
        1,127,// Р
        1,127,// С
        1,127,// Т
        1,127,// У
        1,127,// Ф
        1,127,// Х
        1,127,// Ц
        1,127,// Ч
        1,127,// Ш
        1,127,// Щ
        1,127,// Ъ
        1,127,// Ы
        1,127,// Ь
        1,127,// Э
        1,127,// Ю
       32, 66,// Я
       30, 72,// а
       33, 76,// б , 37
       34, 92,// в
       28, 79,// г  lazurski small kegl
       32, 68,// д  changed cune17/18 & UKRAINIAN PAPERS
       42, 86,// е
       36, 69,// ж
       42, 75,// з
       48, 97,// и
       45, 88,// й    jurnal 7 kegl
       52, 75,// к
       51, 94,// л
       33, 75,// м lower bound up     & ERECT
       42, 75,// н
       34, 93,// о lower bound up
       40, 75,// п
       32, 54,// р cursiv d
       36, 57,// с cursiv d tail down & ERECT
        1,127,// т
        1,127,// у
        1,127,// ф
       77, 94,// cursiv m
        1,127,// ц
       50, 82,// ч cursiv u
       28, 56,// ш cursiv ee
        1,127,// щ
        1,127,// ъ
        1,127,// ы
        1,127,// ь
       41, 76,// э cursiv a
        1,127,// ю
        1,127 //
              };

int32_t leo_narrow, leo_narrow_pen;
extern uchar leo_alpha_type;
#include "alphaset.h"

void leo_sizes_init_stat(void)
{
leo_narrow=leo_narrow_pen=0;
return;
}

uchar leo_proport_penalty(uchar let,uchar prolet, int32_t w, int32_t h)
{
int32_t comSizes[4], prob[4], wid, hei, pr, pr1, prop, prp;
uchar name;
int32_t pmin=0, pmax=255;

#ifdef NO_SIZES
return 0;
#endif
if( !w || !h )
    return 255;

if( let )
    {
    name = stdAnsiToAscii((uchar)let);
    if( memchr("№",let,1) )
        name=let;
    pmin=letters_prop_table_rus[name].pmin;
    pmax=letters_prop_table_rus[name].pmax;
    if( LEO_GetLetStat((int32_t)let, comSizes,prob)>=0 && comSizes[0] && comSizes[2])
        { //   normal statistic                             && not empty
        hei = MIN(comSizes[0],comSizes[2]);
        wid = MAX(comSizes[1],comSizes[3]);
        pr = MIN(prob[0],prob[1]); // continued after all "else"
        }
    else
        { // statistic is not ready
        if( h>w )
            pr= (64*w)/h         ;
        else if( h==w )
            pr=64                ;
        else if( h<w )
            pr=128 - (64*h)/w    ;

        if( pr>=pmin && pr<=pmax )
            {
            if( pr<pmin+3 )
                leo_narrow++;
            if( comSizes[0] && leo_alpha_type==ALPH_DIG &&
                h*5<=comSizes[0]*4 )
                return 50;
            return 0;
            }
        else
            {
            if( pr<=pmin/2 || pr>=pmax*3/2)
                return 255;

            if( prolet>230 && pr>pmin-5 && pr<pmin )
                {
                leo_narrow_pen++;
                LEO_GetCommonStat(comSizes,prob);

                if( comSizes[0] )
                    {
                    hei = comSizes[0];
                    wid = comSizes[2];
                    prop= MIN( prob[0], prob[2]);

                    if( prop>64 && hei>wid)
                        {
                        prop= (64*wid)/hei     ;
                        if( abs(pr-prop)<5 )
                            return 0;
                        }
                    }
                }
            if( pr<pmin ) // было const 255,255-75 ,const 0, 60-255, const 255
                {
                if( leo_narrow>5 && leo_narrow_pen>1 || leo_narrow>10 )
                    pr=(pmin+pr)/2;
                return 128*(256-(256*pr)/pmin)/256;
                //return 255-255*((2*256*pr)/pmin-256)/256;
                }
            if( pr>pmax ) // стало const 255, 255-0, const 0 ,0-255, const 255
                return 10+255*2*((256*pr)/pmax-256)/256;
            }
        }
    }
else
    { // no letter
    LEO_GetCommonStat(comSizes,prob);

    if( comSizes[0]==0 )
        return 0;  // empty stat

    if( comSizes[1]==0 )
        { // all
        hei = comSizes[0];
        wid = comSizes[2];
        pr  = MIN( prob[0], prob[2]);
        }
    else
        { // different Capital & Small
        hei = MIN(comSizes[0],comSizes[1]);
        pr  = MIN( prob[0], prob[1]);
        wid = comSizes[2];
        pr  = MIN( pr, prob[2]);
        }

    }

if( !hei || !wid )
    return 0;

if( hei>wid )
    prop= (64*wid)/hei     ;
else if( hei==wid )
    prop=64                ;
else if( hei<wid )
    prop=128 - (64*hei)/wid;

if( h>w )
    prp= (64*w)/h         ;
else if( h==w )
    prp=64                ;
else if( h<w )
    prp=128 - (64*h)/w    ;

if( !let && prp<79 && prp>32 || (pmax!=255 && prp>=pmin && prp<=pmax ))
    return 0;

if( !let && prp<96 && prp>32 )
if( prp<62 && prop>66 || prop<62 && prp>66 )
    return 0;

pr1 = ( abs( prp-prop )*100/prop );
if( pr1>100 )
    pr1=100;

if( h>=w && prp>=pmin && prp<=pmax ||
    h<w &&pr1<45 && !( pmax && w>wid && w*100/wid>120 && prp>pmax) )
    return 0;

return (uchar)((pr1*255)/100);  // large prop
}


uchar leo_sizes_penalty( int32_t w, int32_t h)
{
int32_t comSizes[4], prob[4], wid, hei, pr;
#ifdef NO_SIZES
return 0;
#endif


    LEO_GetCommonStat(comSizes,prob);

    if( comSizes[0]==0 )
        return 0;  // empty stat
    if( comSizes[0] && comSizes[2] )
        {
        if( h*w*6<=comSizes[0]* comSizes[2]  )
                return 100;
        }

    if( comSizes[1] )
        { // different Capital & Small
        hei = MIN(comSizes[0],comSizes[1]);
        pr  = MIN( prob[0], prob[1]);

        if( pr>=64 )
            {
            if( h*2<=hei  )
                return 100;
            if( h*4<=hei*3 )
                return (uchar)(100*(hei*3-4*h)/hei);
            }
        wid = comSizes[2];
        if( prob[2] >=100 && pr>60 && (h*2<=hei||w*2<=wid) && h*w*4<=hei*wid*3  )
            {
            if( h*w*4<=hei*wid  )
                return 100;
            if( h*w*4<=hei*wid*3 )
                return (uchar)(50*(hei*wid*3-4*h*w)/(hei*wid));
            }
        }



return 0;
}



