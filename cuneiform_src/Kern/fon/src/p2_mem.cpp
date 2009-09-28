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

///////////////////////
#include "fon.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*#include <io.h>*/
#include <fcntl.h>
#include <sys/stat.h>

#include "sfont.h"
#include "fonrec.h"
#include "ctb.h"

extern Nraster_header *rh;

static int32_t numAddedMemory=0;
////////////
int32_t GetNumMemory(void)
{
	return numAddedMemory;
}
/**************************************/
void EndNumMemory(void)
{
	numAddedMemory=0;
}
///////////////
static int16_t MakeBitmapsRecRaster(Nraster_header  *rhh,
								 RecRaster *r)
{
uchar *AddBuffer(int32_t sizebitmap);

 int16_t j,i;
 int16_t sx=rhh->w,sy=rhh->h,sxbyte;
 int32_t sizebitmap;
 uchar *pic,*pp;
 int32_t wb = ((r->lnPixWidth+63)/64)*8;

  rhh->xbyte=sxbyte=(sx>>3)+1;
  sizebitmap=sxbyte*sy;

  if( (rhh->pHau=AddBuffer(2*sizebitmap+sxbyte)) == NULL )
	  return -1;

  j=(sx+7)>>3;  // real bytes
  pic=rhh->pHau;
  pp=r->Raster;
  for(i=0;i<sy;i++,pic+=sxbyte,pp+=wb)
	  memcpy(pic,pp,j);

  rhh->pHaur=rhh->pHau+sxbyte*sy;
  // размазать и обнулить хвосты!
  rhh->fat=(uchar)Razmaz(rhh->pHau,rhh->pHaur,sxbyte,sx,sy,rhh->bold);

 return 0;
}
/////////////////////
// fill static Nraster_header rh !!!
FON_FUNC(int32_t) FONStoreRaster(RecRaster *r,uchar let, uchar IsPrint,
		uchar Prob, uchar Valid, int16_t line_number,uchar kegl,
		Rect16 *rect,uchar column)
{
 Nraster_header *rhh;

 if( IsPrint==0 )     // CTB[5]
      return 0;
                     // CTB[15]
 if( ( Valid & LEO_VALID_FINAL ) ==0 )
	  return 0;  // no in final

 if( numAddedMemory >= MAXSYM)
	 return 0;

 // захватить память
 if(numAddedMemory==0)
 {
	 EndHausdorfDLL();
	 if( StartHausdorfDLL(0,NULL,0) < 0)
		 return -1;
 }
 if(!rh)  return -1;

 rhh=rh+numAddedMemory;
 memset(rhh,0,sizeof(Nraster_header));

 rhh->w    =(uchar)r->lnPixWidth;  //CTBdata[1];
 rhh->h    =(uchar)r->lnPixHeight; //CTBdata[2];
 rhh->let  =let;                   //CTBdata[3];

 rhh->prob=Prob;   // probability
 rhh->valid=Valid;   // validity

 rhh->sr_row=rect->top;
 rhh->sr_col=rect->left;

 rhh->nInCTB=numAddedMemory+1;  // count from 1 !
 rhh->nField=line_number; //CTBdata[25];   // which field

 rhh->solid=1;
 if( IsPrint & CTB_PRINT_ITALIC )
	 rhh->italic=1;
 if( IsPrint & CTB_PRINT_BOLD )
	 rhh->bold=1;
 if( IsPrint & CTB_PRINT_SERIFIC )
	 rhh->serif=1;
 if( IsPrint & CTB_PRINT_GELV )
	 rhh->gelv=1;
 if( IsPrint & CTB_PRINT_NARROW )
	 rhh->narrow=1;

 rhh->kegl=kegl;
 rhh->tablColumn=column;

 if( MakeBitmapsRecRaster(rhh,r) < 0)
		  return -1;

 numAddedMemory++;

 return numAddedMemory;
}
/////////////////////
int32_t StartAddMemCluster(uchar *metkaValid,int32_t CurClus,
						 int16_t countFont,uint32_t *allFields)
{
 int i,CurCount;
 FONBASE *fBase;

 for(i=0,CurCount=0;i< CurClus;i++)
 {
	  if( (metkaValid[i] & METKA_VALID) != 0 )
		  CurCount++;
 }
 if(CurCount<=0)
	 return 0;

 FONDone();
 fBase=GetStaticFonbase();
 fBase->start = (welet *)malloc(CurCount*sizeof(welet));

 if(fBase->start==NULL)
     return -1;
 fBase->reserv=CurCount;

// now to static - fonts info
 fBase->countFont=countFont;
 // Информация о шрифтах в полях
 i=MIN(countFont,4);
 memcpy(fBase->fontFields,allFields,i*NFIELDDWORD*sizeof(uint32_t));

 return CurCount;
}
///////////////////
int32_t AddClusterMemFont(welet *wel)
{
 FONBASE *fBase=GetStaticFonbase();

 // есть еще место?
  if( fBase->inBase >= fBase->reserv)
	  return fBase->inBase;

  // not need not-solid for recognition !
  if( !(wel->attr & FON_CLU_SOLID ) || wel->invalid )
	  return fBase->inBase;

  memcpy(fBase->start+fBase->inBase,wel,sizeof(welet));

  FONCutOffClusters(fBase->start+fBase->inBase,1);
  fBase->inBase++;
  return fBase->inBase;
}
//////////////////////////



