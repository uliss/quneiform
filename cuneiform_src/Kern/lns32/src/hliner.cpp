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

#include "internal.h"
#pragma hdrstop
#include "lns.h"
#include "hliner.h"
#include "xmatrix.h"
#include "xstack.h"
#include "imgaccs.h"


/////////////////////////////////////////////////////
// Snp section
#include "snptools.h"
static SnpTreeNode  stnHLiner;
LNS_FUNC( void   )      LnsRegisterSnpTree(
         SnpTreeNode* parent,         // parent Snp Node, may be NULL
         __SnpToolBox* p_snp_tools     // tools complect, may be NULL
                              )
{
   SnpSetTools( p_snp_tools );    // may be NULL, it's OK
   SnpAddNode( &stnHLiner,"HLiner",  parent);
}
//////////////////////////////////////////////////////



static Int32 nByteWidth;
static Int32 nLine=0;

//static Bool  bReady = FALSE;

static XMatrix< Word8 > xmImageMap;
static XStack<Line16>   xsLines;
extern const Word8 bit1_cnt[256];
extern const Word8 bit0_cnt[256];
static const Word8* BlackBitsCountTbl=bit1_cnt;
static Tiger_ImageInfo ImageInfo={0};


Bool32   HLiner_Setup( Tiger_ImageInfo* pTgInfo )
{
   ImageInfo = *pTgInfo;

   if (!xmImageMap.Create( (ImageInfo.wImageWidth+15) / 16, (ImageInfo.wImageHeight+1) / 2 ))
      return FALSE;
   xmImageMap.MemSet(0);

   nLine=0;

   //if (ImageInfo.bFotoMetrics != 0) // white pixel is 0
   //   BlackBitsCountTbl = bit1_cnt;
   //else
      BlackBitsCountTbl = bit0_cnt;

   return TRUE;
}

Bool32   HLiner_Init( void )
{
   HLiner_Done();
   return TRUE;
};

void     HLiner_Done( void )
{
   xsLines.Destroy();
   xmImageMap.Destroy();
   Set0(ImageInfo);
   nLine=0;
};

inline Word8* GetMapLine( int n )
{
   return &(xmImageMap.Get( n, 0 ));
}

Bool32   HLiner_AddImageLine( Word8* pLine )
{
   if (nLine < 0 || nLine > ImageInfo.wImageHeight-1)
      RET_FALSE;

   Word8* res = GetMapLine(nLine/2);
   int nWords = (ImageInfo.wImageWidth+15) / 16; //Almi: Вася нашёл эту ошибку 21 mar 2002 - было: = ImageInfo.wImageByteWidth >> 1;
   while (nWords--)
   {
      *res += BlackBitsCountTbl[*pLine++];   // get 16 bits
      *res += BlackBitsCountTbl[*pLine++];
      res++;
   }
   nLine++;
   return TRUE;
}

Bool32   _TraceLine(int i, int j, Line16& ln )
{
   ln.A.x = (j << 4) + 8; // 16x2
   ln.A.y = i << 1;

   // trace up
   int i_top=i; int i_bot=i+1;

   while ( GetMapLine(i_top)[j] == 100 )
   {
      GetMapLine(i_top)[j] = 102;   // marked
      i_top --; // up
   }

   while ( GetMapLine(i_bot)[j] == 100 )
   {
      GetMapLine(i_bot)[j] = 102;   // marked
      i_bot ++; // down
   }

   int ii = (i_top + i_bot) >> 1;
   ln.A.y = i_top + i_bot; // 16x2

   int thick = 0;
   int jj = j;
   do
   {  jj ++;   // step right
      thick = 0;

      if (GetMapLine(ii)[jj] != 100) // try step up or down
      {
         if (GetMapLine(ii-1)[jj] == 100)
            ii--;
         else if (GetMapLine(ii+1)[jj] == 100)
            ii++;
         else  // line ended
         {
            ln.B.y = ii << 1; // 16x2
            ln.B.x = (jj << 4) - 8; // 16x2, prev cell
            break; // line stopped
         }
      };

      // update center of line and sweep
      i_top=ii; i_bot=ii+1;
      while ( GetMapLine(i_top)[jj] == 100 )
      {
         GetMapLine(i_top)[jj] = 102;   // marked
         i_top --; // up
      }
      while ( GetMapLine(i_bot)[jj] == 100 )
      {
         GetMapLine(i_bot)[jj] = 102;   // marked
         i_bot ++; // down
      }
      ii = (i_top + i_bot) >> 1;
   }  while (1);

   return (jj - j) > 3;
}

Int32    HLiner_Analyze(void) // returns count of extracted lines
{
   int gap = 3; // count of zero cells on up and down direction
   int wid = 3; // count of notzero cells on left or right side
   int nHeight = xmImageMap.GetHeight();
   int nWidth  = xmImageMap.GetWidth();

   for (int i=gap; i<nHeight-gap-1; i++)  // by lines
   {
      Word8* line_hi = GetMapLine(i - gap);
      Word8* line_on = GetMapLine(i);
      Word8* line_lo = GetMapLine(i + gap);
      for (int j=wid; j<nWidth-wid-1; j++)  // first column
      {
         if (!line_on[j])
            continue;

         if (  !(line_hi[j]+line_lo[j])
               && (line_on[j-wid] + line_on[j+wid])
            )
         {
            line_on[j] = 100; // seems to be line fragment
         }
         else
         {
            line_on[j] = 101;
         }
      }
   }

   // замазываем дырки и убиваем одиночек
   for (int i=gap; i<nHeight-gap-1; i++)  // by lines
   {
      Word8* line_on = GetMapLine(i);
      for (int j=wid; j<nWidth-wid-1; j++)  // first column
      {
         if (line_on[j] == 100)
         {
            if (line_on[j-1] != 100 && line_on[j+1] != 100)
               line_on[j] = 101; // kill
         }
         else
         {
            if (line_on[j-1] == 100 && line_on[j+1]==100)
               line_on[j] = 100; // recover
//            else if (line_on[j-2] == 100 && line_on[j+2]==100)
//               line_on[j] = 100; // recover
         }
      }
   }

   // now link lines from start to end
   //
   Word8* line_on = NULL;
   Line16 ln={0};
   for ( int i=gap; i<nHeight-gap-1; i++)  // by lines
   {
      Word8* line_on = GetMapLine(i);
      for (int j=wid; j<nWidth-wid-1; j++)  // first column
      {
         if (line_on[j] != 100)
            continue;
         // reach line cell
         if (  GetMapLine(i+1)[j-1] != 100 &&
               GetMapLine(i)[j-1]   != 100 &&
               GetMapLine(i-1)[j-1] != 100
            )
         {  // start new line
            if (!_TraceLine(i,j,ln))
               continue;
            if (!xsLines.Push(&ln))
               break;
         }
      }
   }

   if (!SnpSkip(&stnHLiner))
   {
      for (int i=0; i<xsLines.GetCurCnt(); i++)
      {
         Line16& li= xsLines[i];
         SnpDrawLine( &li.A,&li.B,0,wRGB(255,0,255),2,(Word32)&stnHLiner);
      }
      SnpWaitUserInput(&stnHLiner);
      SnpHideLines((Word32)&stnHLiner);
   }

   xmImageMap.Destroy(); // free, currently not used later

   return  xsLines.GetCurCnt();
}

Int32  HLiner_GetCount(void)
{
   return  xsLines.GetCurCnt();
};

Line16*  HLiner_GetLine( Int32 nLineIndex )
{
   return &(xsLines[nLineIndex]);
}; // 0..count-1
   // NULL - wrong index or not ready

