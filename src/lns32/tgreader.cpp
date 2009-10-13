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

#include "lnslang.h"
#include "lns.h"

#include  "tgreader.h"
#include "sweeper.h"
#include "memops.h"

	void ExtrLinesInitLNS(void);

static  TImageOpen   __f_op = NULL;
static  TImageRead   __f_rd = NULL;
static  TImageClose  __f_cl = NULL;


   #include "fillgap3.h"
   #include "shortseg.h"

static Bool HBorder=TRUE;
static Bool VBorder=TRUE;

//********************************************

void  InitLNS( TImageOpen   f_op,
                                   TImageRead   f_rd,
                                   TImageClose  f_cl
             )
	{  __f_op = f_op;
		__f_rd = f_rd;
		__f_cl = f_cl;
		InitSweeperCallbacks(f_op,f_rd,f_cl);
      ExtrLinesInitLNS();
	}

#define Tiger_Callback_ImageOpen       (*__f_op)
#define Tiger_Callback_ImageRead       (*__f_rd)
#define Tiger_Callback_ImageClose      (*__f_cl)

#define BUF_LEN   0x7FFF
static int lines_counter=0;

extern LnsSetupStr lnsSetup;

TigerReader::TigerReader(void)
	{  lines_counter=0;
      wImageByteWidth=0;
      errCode = Tiger_Callback_ImageOpen( (Tiger_ImageInfo*)this) ?
                  ER_NONE      :
                  ER_CANTOPEN;
		if (!isOk())
		{  assert(0); return;
		};

      VBorder = !(lnsSetup.nOptions & LSS_NOVBORDER_FILTER);
      if (VBorder) // flag not set - look ini
      {
         VBorder = LnsGetProfileInt( "CleanVerticalBorder", 1 ); // sweep, by default
      }

      HBorder = !(lnsSetup.nOptions & LSS_NOHBORDER_FILTER);
      if (HBorder) // flag not set - look ini
      {
         HBorder = LnsGetProfileInt( "CleanHorisontalBorder", 1 ); // sweep, by default
      }

      lines_count = 0;
      dword_len = BUF_LEN / 4;
      line_buf = (uchar*)malloc( BUF_LEN ); // 24 apr 96, VP. Bug:  BUF_LEN <== (dword_len*4)
		if (line_buf == NULL)
      {  assert(0); errCode = ER_NOMEMORY;
         dword_len=0;
			return;
		};
      if ( (wAddX % 8) != 0 )
      {  assert(0);
      }

      FillGap3_Init(wImageByteWidth);
	}

TigerReader::~TigerReader(void)
	{

      FillGap3_Done();

      Tiger_Callback_ImageClose();
		if (line_buf != NULL)
         free( (void*)line_buf );
	}

void*   TigerReader::getLine(void)
   {
//#ifdef LNS_DEBUG
//   lines_counter++;
//   printf("\n[%d]\t", lines_counter);
//#endif
      if (lines_count == 0)    // there is no lines in buf...
      {  int32_t res = Tiger_Callback_ImageRead( (uchar*)line_buf, (BUF_LEN/wImageByteWidth)*wImageByteWidth ); // 03-09-94 01:45pm
         lines_count = res / wImageByteWidth;
         if (lines_count == 0)
         {  assert(0);
            return NULL;
         };
         cur_line = line_buf;
         if (bFotoMetrics == 0x00)  // white is zero
			{
				#ifdef NO_ASM
               register int32_t* cur_dword = (int32_t*)line_buf;
               int ww16 = dword_len >> 4;
               while (ww16--)
               {
                  cur_dword[ 0] = ~(cur_dword[ 0]);
                  cur_dword[ 1] = ~(cur_dword[ 1]);
                  cur_dword[ 2] = ~(cur_dword[ 2]);
                  cur_dword[ 3] = ~(cur_dword[ 3]);

                  cur_dword[ 4] = ~(cur_dword[ 4]);
                  cur_dword[ 5] = ~(cur_dword[ 5]);
                  cur_dword[ 6] = ~(cur_dword[ 6]);
                  cur_dword[ 7] = ~(cur_dword[ 7]);

                  cur_dword[ 8] = ~(cur_dword[ 8]);
                  cur_dword[ 9] = ~(cur_dword[ 9]);
                  cur_dword[10] = ~(cur_dword[10]);
                  cur_dword[11] = ~(cur_dword[11]);

                  cur_dword[12] = ~(cur_dword[12]);
                  cur_dword[13] = ~(cur_dword[13]);
                  cur_dword[14] = ~(cur_dword[14]);
                  cur_dword[15] = ~(cur_dword[15]);
                  cur_dword +=16;
               }

               int ww__ = dword_len & 15;
               while (ww__--)
                  *cur_dword++ = ~(*cur_dword);

				#else
					InvertSelf(line_buf, dword_len);
				#endif
			};

		};
      lines_count --;
      void* res_line = (void*)( cur_line );
		cur_line += wImageByteWidth;

      ///////////////////////////////////////////////////////
	  if (HBorder)       //*******Rom
      {  // kill hor border
         int32_t num_short_segm = 0;
         uchar* pp = (uchar*)res_line;
         int wi16 = wImageByteWidth >> 4;
         int wi   = wImageByteWidth & 15;
         while (wi16--)
         {
            num_short_segm += tabNumShortSegm[*pp++];
            num_short_segm += tabNumShortSegm[*pp++];
            num_short_segm += tabNumShortSegm[*pp++];
            num_short_segm += tabNumShortSegm[*pp++];

            num_short_segm += tabNumShortSegm[*pp++];
            num_short_segm += tabNumShortSegm[*pp++];
            num_short_segm += tabNumShortSegm[*pp++];
            num_short_segm += tabNumShortSegm[*pp++];

            num_short_segm += tabNumShortSegm[*pp++];
            num_short_segm += tabNumShortSegm[*pp++];
            num_short_segm += tabNumShortSegm[*pp++];
            num_short_segm += tabNumShortSegm[*pp++];

            num_short_segm += tabNumShortSegm[*pp++];
            num_short_segm += tabNumShortSegm[*pp++];
            num_short_segm += tabNumShortSegm[*pp++];
            num_short_segm += tabNumShortSegm[*pp++];
         };

         while (wi--)
            num_short_segm += tabNumShortSegm[*pp++];

         if (num_short_segm > wImageByteWidth) // line seems to be noise
            memset(res_line, 0xff, wImageByteWidth);
      }

      ///////////////////////////////////////////////////////
	  if (VBorder)			//*******Rom
      {  // kill left border - kill all for first white segm
         int max_border_width = wImageByteWidth >> 4; // 1/16 of page
         uchar* pp = (uchar*)res_line;
         while (max_border_width--)
         {
            if (*pp == 0xff)  // full white segm
               break;
            *pp++ = 0xff;
         }
      }

      ///////////////////////////////////////////////////////
	  if (VBorder)				//*******Rom
      {  // kill right border - kill all for first white segm
         int max_border_width = wImageByteWidth >> 4; // 1/16 of page
         uchar* pp = (uchar*)res_line;
         pp += wImageByteWidth - 1; // set to last byte
         while (max_border_width--)
         {
            if (*pp == 0xff)  // full white segm
               break;
            *pp-- = 0xff;
         }
      }



      if (wAddX)
      {
         uchar* pp = (uchar*)res_line;
         int32_t skipbytes = wAddX >> 3;
         pp += skipbytes;
         res_line = pp;
      }


      return res_line;
	}

