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

#include <math.h>
#include "lnslang.h"

   #ifndef __LNS_H
	#  include "lns.h"
   #endif

   #ifndef __EXTRLNS_H
	#  include "extrlns.h"
   #endif

	#ifndef __SBAMBUK_H
	#	include "sbambuk.h"
	#endif
	#ifndef __RBAMBUK_H
	#	include "rbambuk.h"
	#endif
	#ifndef __LBAMBUK_H
	#	include "lbambuk.h"
	#endif
	#ifndef __PARALINE_H
	#	include "paraline.h"
	#endif

   #ifndef __SWEEPER_H
   #  include "sweeper.h"
   #endif

#include "lns_skew1024.h"

#include "decl.h"

   Bool  cdecl  ExtrLinesIsOk(void);
	/* Implementation ************************/
	static   THVSegBambuk  *   hvSB = NULL;
	static   TRasterBambuk *   hRB  = NULL;
	static   TLinesBambuk  *   hLB  = NULL;
   static   TRasterBambuk *   vRB  = NULL;
   static   TLinesBambuk  *   vLB  = NULL;
   /*****************************************/

#include "rombabble.h"
   //*******************Rom********************
   RomBubble*	Rptr = NULL;
   RomBubble*	Rptr2 = NULL;
   RomBubble*	Rptr3 = NULL;
   CRomBabble*	RBlockFirst = NULL;
   CRomBabble*	RBlockCurrent = NULL;
   CRomBabble*	RBlockPrev = NULL;
   int			Romi = 0;
   //******************************************



//#define DELETE_PTR(ptr) if (ptr != NULL) { ::delete ptr; ptr = NULL; };
#define DELETE_PTR(ptr) if (ptr != NULL) { delete ptr; ptr = NULL; };
static void _destroy_lines(void)
{
   DELETE_PTR( hvSB );
   DELETE_PTR( hRB  );
   DELETE_PTR( hLB  );
   DELETE_PTR( vRB  );
   DELETE_PTR( vLB  );
}

void ExtrLinesInitLNS(void)
{ _destroy_lines();
   //*******************Rom********************
   RBlockCurrent = new CRomBabble;
   RBlockFirst = RBlockCurrent;
   RBlockPrev = NULL;
   Rptr = new RomBubble;
   Rptr->next = NULL; //*************Rom 17.11.98  16:30
   Rptr2 = NULL;
   Rptr3 = Rptr;
   Romi =0 ;
   //******************************************

}

void ExtrLinesDoneLNS(void)
{  _destroy_lines();
   DELETE_PTR( Rptr );
   Rptr2 = Rptr3 = NULL;
   Romi = 0;
   RBlockPrev = RBlockCurrent = RBlockFirst;
   while ( RBlockCurrent != NULL )
   {
	   RBlockCurrent = RBlockPrev->next;
	   delete(RBlockPrev);
	   RBlockPrev = RBlockCurrent;
   }
}

static void _squeezeIfTooMany(  Int32 & hor_len, Int32 & ver_len,
								Int32 & hor_cnt, Int32 & ver_cnt
                     )
{
   Int32 max_cnt = 0xFFF0 / sizeof( LineInfo );
   while (TRUE)
   {
      ExtrLinesGetCount( hor_len, ver_len, hor_cnt, ver_cnt );
      if (hor_cnt <= max_cnt && ver_cnt <= max_cnt )
         break;
      if (hor_cnt > max_cnt)
         hor_len++;
      if (ver_cnt > max_cnt)
         ver_len++;
   };
}

//******************************* Rom 8-2-99 **************
static void FillFragmentsCount( Int32 hnd, Bool hor, Int16* cnt, Word8* quality)
{
	Int16 Romii;
   Int16 SpacesLength, TotalLength;
	TLinesBambuk* lb = hor ? hLB : vLB;
	TRasterBambuk* rb = hor ? hRB : vRB;

	TLineInfo & li = lb->linesRoot[hnd];
	BEntry   lbe = li.linesBambukEntry;
	BHandle h = lb->firstEntryMember( lbe );
	TLineFragment lf;
	Romii = 0;

   if (hor)
      TotalLength = abs(li.lineAsIs.start.x-li.lineAsIs.end.x);
   else
      TotalLength = abs(li.lineAsIs.start.y-li.lineAsIs.end.y);


   SpacesLength = TotalLength;
   if (TotalLength <=0)
      return;
	while ( h !=NULLBHandle  )
	{
		lf = (*lb)[h];
      SpacesLength -= hor ?
         abs(lf.fragmentAsIs.start.x-lf.fragmentAsIs.end.x) :
         abs(lf.fragmentAsIs.start.y-lf.fragmentAsIs.end.y);

		SpacesLength=abs(SpacesLength);
		Romii++;
		h = lb->nextMember(h);
	}
	*cnt = Romii;
   *quality = ((TotalLength-SpacesLength)*255)/TotalLength;
}

//**********************************************************
#define LOG_TIME
#ifdef LOG_TIME
/*#include <windows.h> // OutputDebugString()*/
#define CONSOLE(str) OutputDebugString(str)
#include "xtimer.h"
#endif

#include "hliner.h"

Err16 ExtrLinesUnderTigerStatic(  Int32  hor_len, Int32  ver_len,
														Int32 &hor_cnt, Int32 &ver_cnt )
{
#ifdef LOG_TIME
   XTimer t("Lines Extraction");
#endif

   int err_code = ER_NONE;
   int i=0;

   TigerReader tg_rdr;

   HLiner_Setup(&tg_rdr);

   if (!tg_rdr.isOk())
      {  assert(0);  return tg_rdr.errCode; };

   _destroy_lines();
   int AddX = tg_rdr.wAddX;
   int AddY = tg_rdr.wAddY;
/* I. build segments bambuk */
   hvSB = new THVSegBambuk( tg_rdr );
	if (hvSB == NULL)   {
      err_code = ER_NOMEMORY;
		goto ErrorExit;
   };
	if (!hvSB->isOk())
   {
      err_code = hvSB->errCode;
		goto ErrorExit;
	};
/*
   if (  (hvSB->hBambuk.segCount == 0)
         &&
         (hvSB->vBambuk.segCount == 0)
      )
   {  err_code = ER_NOLINES;
      goto ErrorExit;
   };
*/
/* II. make horisontal lines */
	/* build H rasters */
	hRB = new TRasterBambuk( 	&(hvSB->hBambuk),
										(BHandle) MAX_H_DASHES_COUNT,   // members
										(BEntry)  MAX_H_RASTERS_COUNT   // entries
								  );
	if (hRB == NULL) {
      err_code = ER_NOMEMORY;
		goto ErrorExit;
   };
	if (!hRB->isOk()) {
      err_code = ER_NOMEMORY;
		goto ErrorExit;
   };

	/* build H lines */

   HLiner_Analyze();

	hLB = new TLinesBambuk( hRB,
									&(hvSB->hBambuk),
									(BEntry)  MAX_H_LINES_COUNT,
									FALSE 	// not vertical
							   );
	if (hLB == NULL) {
      err_code = ER_NOMEMORY;
		goto ErrorExit;
   };

	if (!hLB->isOk()) {
      err_code = ER_NOMEMORY;
		goto ErrorExit;
   };
	*((TXYDim*)hLB) = *((TXYDim*)hvSB);         // 11-05-93 07:24pm

/* III. make vertical lines */
	/* build V rasters */
	vRB = new TRasterBambuk( 	&(hvSB->vBambuk),
										(BHandle) MAX_V_DASHES_COUNT,  // members
										(BEntry)  MAX_V_RASTERS_COUNT  // entries
			  						);
	if (vRB == NULL)   {
      err_code = ER_NOMEMORY;
		goto ErrorExit;
   };
	if (!vRB->isOk())   {
      err_code = ER_NOMEMORY;
		goto ErrorExit;
   };

	/* build V lines */
	vLB = new TLinesBambuk( vRB,
									&(hvSB->vBambuk),
									(BEntry)  MAX_V_LINES_COUNT,
									TRUE 	// is vertical
								 );
	if (vLB == NULL)   {
      err_code = ER_NOMEMORY;
		goto ErrorExit;
   };
	if (!vLB->isOk())   {
      err_code = ER_NOMEMORY;
		goto ErrorExit;
   };
   *((TXYDim*)vLB) = *((TXYDim*)hvSB);         // 11-05-93 07:24pm
////////////////
Bool AnalyzeFragments( TLinesBambuk& hLB, TLinesBambuk& vLB);
Bool AnalyzeFragmentsII( TLinesBambuk& hLB, TLinesBambuk& vLB);

   if (!AnalyzeFragments( *hLB, *vLB))
   {
      err_code = ER_NOMEMORY;
		goto ErrorExit;
   }

   if (!AnalyzeFragmentsII( *hLB, *vLB))
   {
      err_code = ER_NOMEMORY;
		goto ErrorExit;
   }

   if (!hLB->linkHFragments())
   {
      err_code = ER_NOMEMORY;
		goto ErrorExit;
   }

   if (!vLB->linkVFragments())
   {
      err_code = ER_NOMEMORY;
		goto ErrorExit;
   }
/////////////////
/* IV. Make diff... */
   for ( i = 0; i < hLB->linesCount; i++)
   {
      Point32 & p1 = hLB->linesRoot[i].lineAsIs.end;
      p1.x += AddX; p1.y += AddY;
      Point32 & p2 = hLB->linesRoot[i].lineAsIs.start;
      p2.x += AddX; p2.y += AddY;
   };

   for ( i = 0; i < vLB->linesCount; i++)
   {
      Point32 & p1 = vLB->linesRoot[i].lineAsIs.end;
      p1.x += AddX; p1.y += AddY;
      Point32 & p2 = vLB->linesRoot[i].lineAsIs.start;
      p2.x += AddX; p2.y += AddY;
   };
/* IV. Send counts... */


   //=========== 10-19-95 06:46pm, Basil ====================
   _squeezeIfTooMany( hor_len, ver_len, hor_cnt, ver_cnt );
   //============================================================


   return ER_NONE; // Normal exit

ErrorExit :
   _destroy_lines();

   return err_code; // Bad exit
}

Bool ExtrLinesGetCount(   Int32  hor_len, Int32  ver_len,
								  Int32 &hor_cnt, Int32 &ver_cnt
                            )
{

   if (hLB == NULL) return WRONG();
   if (vLB == NULL) return WRONG();
   if (!hLB->isOk()) return WRONG();
   if (!vLB->isOk()) return WRONG();

   hor_cnt = 0;
   for ( int i = 0; i < hLB->linesCount; i++)
   {  if (  (hLB->linesRoot[i].lineAsIs.end.x -
             hLB->linesRoot[i].lineAsIs.start.x
            )  >  hor_len
         ) hor_cnt++;
   };
   ver_cnt = 0;
   for ( int i = 0; i < vLB->linesCount; i++)
   {  if (  (vLB->linesRoot[i].lineAsIs.end.y -
             vLB->linesRoot[i].lineAsIs.start.y
            )  >  ver_len
         ) ver_cnt++;
   };
   return TRUE;
}

static void  LCpy( Point16& dst, Point32& src )
{
dst.x=(Int16)(src.x);
dst.y=(Int16)(src.y);
}


Bool ExtrLinesGetInfo(  LinesTotalInfo * lti,
								Int32  hor_len, Int32  ver_len,
								Int32 &hor_cnt, Int32 &ver_cnt
                     )
{
	Int16 cnt;//******************Rom
	Word8 Quality;//******************Rom

   if (hLB == NULL) return WRONG();
   if (vLB == NULL) return WRONG();
	if (!hLB->isOk()) return WRONG();
   if (!vLB->isOk()) return WRONG();

   Int32 max_cnt = 0xFFF0 / sizeof( LineInfo );
   _squeezeIfTooMany( hor_len, ver_len, hor_cnt, ver_cnt );

   hor_cnt = 0;
   for ( int i = 0; i < hLB->linesCount; i++)
	{  if (  (hLB->linesRoot[i].lineAsIs.end.x -
				 hLB->linesRoot[i].lineAsIs.start.x
				)  >  hor_len
			)
			{
				if (hor_cnt >= lti->Hor.Cnt)
					return WRONG();
				LineInfo& li = lti->Hor.Lns[hor_cnt];
				FillFragmentsCount( i, TRUE, &cnt, &Quality);
				LCpy( li.A, hLB->linesRoot[i].lineAsIs.start );
				LCpy( li.B, hLB->linesRoot[i].lineAsIs.end   );
            li.Thickness = ( hLB->linesRoot[i].lineAsIs.width10 + 5 ) / 10;
				//li.A = hLB->linesRoot[i].lineAsIs.start;
				//li.B = hLB->linesRoot[i].lineAsIs.end;
				li.ExtrDllHnd = i;
            li.Extractor = LI_LNSDLL;
			li.Flags = hLB->linesRoot[i].lineAsIs.flags; //******************Rom
			li.Anew = li.A;//******Rom
			li.Bnew = li.B;//******Rom
			li.SegCnt = cnt; //******** Rom 8-2-99
			li.Quality = Quality; //******* Rom 23-2-99
				hor_cnt++;
            assert( hor_cnt <= max_cnt );
			};
	};
	ver_cnt = 0;
	for ( int i = 0; i < vLB->linesCount; i++)
	{  if (  (vLB->linesRoot[i].lineAsIs.end.y -
				 vLB->linesRoot[i].lineAsIs.start.y
				)  >  ver_len
			)
			{
				if (ver_cnt >= lti->Ver.Cnt)
					return WRONG();
				LineInfo& li = lti->Ver.Lns[ver_cnt];
				FillFragmentsCount( i, FALSE, &cnt, &Quality);
				LCpy( li.A, vLB->linesRoot[i].lineAsIs.start );
				LCpy( li.B, vLB->linesRoot[i].lineAsIs.end   );
            li.Thickness = ( vLB->linesRoot[i].lineAsIs.width10 + 5 ) / 10;
				//li.A = vLB->linesRoot[i].lineAsIs.start;
				//li.B = vLB->linesRoot[i].lineAsIs.end;
				li.ExtrDllHnd = i;
            li.Extractor = LI_LNSDLL;
			li.Flags = vLB->linesRoot[i].lineAsIs.flags; //******************Rom
			li.Anew = li.A;//******Rom
			li.Bnew = li.B;//******Rom
			li.SegCnt = cnt; //******** Rom 8-2-99
			li.Quality = Quality; //******* Rom 23-2-99
				ver_cnt++;
            assert( ver_cnt <= max_cnt );
			};
	};

   lti->ImgResolution.y = hvSB->yres;
   lti->ImgResolution.x = hvSB->xres;

	lti->ImgSize.y = hvSB->height();
	lti->ImgSize.x = hvSB->width();

   if (hLB->averagePhi != 1. && vLB->averagePhi != 1.)
	   lti->Skew1024  =  (int)(512 * (hLB->averagePhi + vLB->averagePhi));
   else
   {
	   lti->Skew1024=0;
      if          (hLB->averagePhi != 1.)
         lti->Skew1024  =  (int)(1024 * hLB->averagePhi);
      else  if    (vLB->averagePhi != 1.)
         lti->Skew1024  =  (int)(1024 * vLB->averagePhi);
   }

   int skew = lti->Skew1024;
   for ( int i = 0; i < hor_cnt; i++)
	{
		LineInfo& li = lti->Hor.Lns[i];
      li.Ar = li.A; Deskew(li.Ar, skew);
      li.Br = li.B; Deskew(li.Br, skew);
	};
   for ( int i = 0; i < ver_cnt; i++)
	{
		LineInfo& li = lti->Ver.Lns[i];
      li.Ar = li.A; Deskew(li.Ar, skew);
      li.Br = li.B; Deskew(li.Br, skew);
	};

   return TRUE;
}

Bool ExtrLinesIsOk(void)
{
	if ((hvSB == NULL) || (!hvSB->isOk()))  return WRONG();
	if ((hRB  == NULL) || (!hRB->isOk() ))  return WRONG();
	if ((hLB  == NULL) || (!hLB->isOk() ))  return WRONG();
	if ((vRB  == NULL) || (!vRB->isOk() ))  return WRONG();
	if ((vLB  == NULL) || (!vLB->isOk() ))  return WRONG();

   return TRUE;
}

static Bool _PreSwp( LnsInfoArray& larr, Bool hor )
{
   if (larr.Cnt == 0)    return TRUE;
   if (larr.Lns == NULL) return WRONG();

   TLinesBambuk* lb = hor ? hLB : vLB;
   if (lb == NULL) return WRONG();
   TRasterBambuk* rb = hor ? hRB : vRB;
   if (rb == NULL) return WRONG();
   TSegBambuk* sb = hor ? &(hvSB->hBambuk) : &(hvSB->vBambuk);
	if (sb == NULL) return WRONG();

   Int32 root_len = lb->linesRoot.volume();
   for ( int i =0; i<larr.Cnt; i++ )
   {
      if ((larr.Lns[i].Flags & LI_COVERED) != 0)
      {
         LineInfo & cover = larr.Lns[larr.Lns[i].IndCover];
         if (!(cover.Flags & LI_SWEEP))
            continue;  // if not must be sweepped - do nothing here;
      }
      else if ((larr.Lns[i].Flags & LI_SWEEP) == 0)
      {  continue;  // if not must be sweepped - do nothing here;
      }

      if (larr.Lns[i].Extractor != LI_LNSDLL)
      {  continue;  // if not extracted by this - do nothing here;
      }

      // if here - this is noise line; let's delete it from bambuks!

	  //******************Rom****************
	  Int32 RightX = larr.Lns[i].B.x;        // set restrictions for case of sweeping
	  Int32 LeftX = larr.Lns[i].A.x;         // not whole line; initially - full line
	  Int32 TopY = larr.Lns[i].A.y;
	  Int32 BottomY = larr.Lns[i].A.y;

	  if(larr.Lns[i].A.y>larr.Lns[i].B.y)
	  {
		  BottomY = larr.Lns[i].B.y;
	  }
	  else
	  {
		  TopY = larr.Lns[i].B.y;
	  }
	  //*************************************


	  if (larr.Lns[i].Flags & LI_NOTWHOLE)  // correct restriction if not whole line should be sweeped
      {
		  if ( larr.Lns[i].Anew.x > larr.Lns[i].A.x)
		  {
			  if (larr.Lns[i].Anew.x > larr.Lns[i].B.x) larr.Lns[i].Anew.x = larr.Lns[i].A.x;
			  LeftX = larr.Lns[i].Anew.x;
		  }
		  if ( larr.Lns[i].Anew.y > larr.Lns[i].A.y)
		  {
			  if (larr.Lns[i].Anew.y > TopY) larr.Lns[i].Anew.y = larr.Lns[i].A.y;
			  BottomY = larr.Lns[i].Anew.y;
		  }
		  if ( larr.Lns[i].Bnew.x < larr.Lns[i].B.x)
		  {
			  if (larr.Lns[i].Bnew.x < larr.Lns[i].A.x) larr.Lns[i].Bnew.x = larr.Lns[i].B.x;
			  RightX = larr.Lns[i].Bnew.x;
		  }
		  if ( larr.Lns[i].Bnew.y < larr.Lns[i].B.y)
		  {
			  if (larr.Lns[i].Bnew.y < BottomY) larr.Lns[i].Bnew.y = larr.Lns[i].B.y;
			  TopY = larr.Lns[i].Bnew.y;
		  }
      }
	  //*************************************
////////////////////
      Int32 hnd = larr.Lns[i].ExtrDllHnd;
      if (hnd >= root_len ) return WRONG();
      TLineInfo & li = lb->linesRoot[hnd];
		BEntry   lbe = li.linesBambukEntry;
      BHandle h = lb->firstEntryMember( lbe );
      while ( h !=NULLBHandle  )
      {
         TLineFragment& lf = (*lb)[h];

         if (lf.fragmentAsIs.flags & LF_HLINER)
         {
            h = lb->nextMember(h);
            continue;
         }

			BEntry rbe = lf.rasterBambukEntry;
         BHandle hr = rb->firstEntryMember( rbe );
#ifndef NO_17FEB99         // prepear to detect touching letters
         int frag_width[4000];  // width by len distribution

         // compute real left and right fragment's bounds
         int frag_left=0xffff; int frag_right = 0;
		          while ( hr != NULLBHandle )
         {
				TDash& dash = (*rb)[hr];
				BHandle sh = dash.firstSegHandle;
				while ( sh != NULLBHandle )
            {
					TBlackSeg& bs = (*sb)[sh];
               frag_left = MIN( frag_left, bs.left );
               frag_right = MAX( frag_right, bs.right );
               sh = bs.nLower;
            }
            hr = rb->nextMember(hr);
         }
         hr =  rb->firstEntryMember( rbe );

         int frag_len = frag_right-frag_left+1;
         int frag_base =frag_left;

         double ave_frag_width = 2.0; // default, for any

         // avoid sweep short fragments at the bound of line
	//  cos probably part of letters:
#ifndef SHORT_FRAGMENTS_ARE_TESTED_TO_BE_NOT_PART_OF_LETTER
         if (  hor &&
               frag_len < 50 && // short fragment
               (  lf.fragmentAsIs.end.x == li.lineAsIs.end.x // at the end of line
                ||lf.fragmentAsIs.start.x == li.lineAsIs.start.x
               )
            )
         {  // skip it, don't sweep.
            h = lb->nextMember(h);
            continue;
         }
#endif  // SHORT_FRAGMENTS_ARE_TESTED_TO_BE_NOT_PART_OF_LETTER

	 // accumulate width distribution
         if (hor && frag_len > 0 && frag_len*sizeof(frag_width[0]) < sizeof(frag_width))
         {
            memset(frag_width, 0, frag_len*sizeof(frag_width[0]));
            while ( hr != NULLBHandle )
            {
				   TDash& dash = (*rb)[hr];
				   BHandle sh = dash.firstSegHandle;
				   while ( sh != NULLBHandle )
               {
					   TBlackSeg& bs = (*sb)[sh];
                  frag_width[ bs.left -frag_base]++;
                  frag_width[ bs.right-frag_base]--;
                  sh = bs.nLower;
               }
               hr = rb->nextMember(hr);
            }
            int sum_pel = 0;
            for (int i=1;i<frag_len; i++)
            {
               frag_width[i] += frag_width[i-1];
               sum_pel += frag_width[i];
            }
            ave_frag_width = sum_pel / frag_len;

            hr = rb->firstEntryMember( rbe ); // reset hr
         }
#endif // NO_17FEB99


		 // start main loop of presweeping:
         while ( hr != NULLBHandle )
         {
				TDash& dash = (*rb)[hr];
				Romi = 0;
				BHandle sh = dash.firstSegHandle;
				while ( sh != NULLBHandle )
            {
					TBlackSeg& bs = (*sb)[sh];
				if (hor)
				{
#ifndef NO_17FEB99         // prepear to detect touching letters:
                           // keep short segment if local jump of width > 1.5 pel
               if (!(bs.right - bs.left < 24 && // short interval and width jump > 1.5 pel
                     (  double(frag_width[(bs.left+bs.right)/2 - frag_base]) > ave_frag_width + 1.5
                      ||double(frag_width[bs.left+4 - frag_base]) > ave_frag_width + 1.5
                      ||double(frag_width[bs.right-4 - frag_base]) > ave_frag_width + 1.5
                     )
                  ) )
#endif
					if ( (bs.right > LeftX) && (bs.left < RightX) )
					{
						if (bs.left < LeftX-5) bs.left = LeftX;
						if (bs.right > RightX+5) bs.right = RightX;
						int xx = bs.left;
						bs.left = bs.right;
						bs.right = xx;
					}
					// swap segment to indicate it's SWEEPABILITY
				}
				//*********************Rom**********************
				else
				{
					if ( (bs.right > BottomY) && (bs.left < TopY) )
					{
						if (bs.left < BottomY-5) bs.left = BottomY;
						if (bs.right > TopY+5) bs.right = TopY;
						if(RBlockCurrent->Current >= RBlockCurrent->Max)
						{
							RBlockPrev = RBlockCurrent;
							RBlockCurrent = new CRomBabble;
							RBlockPrev->next = RBlockCurrent;
						}
						Rptr2 = &RBlockCurrent->Data[RBlockCurrent->Current];
						Rptr2->Begin  = bs.left;
						Rptr2->End    = bs.right+4;
						Rptr2->Column = dash.firstSegEntry+Romi;
						RBlockCurrent->Current++;
						Rptr2->next = NULL;
						Rptr3->next = Rptr2;
						Rptr3 = Rptr3->next;
					}
					Romi++;
				}
				//**********************************************
               sh = bs.nLower;
            }
            hr = rb->nextMember(hr);
         }

         h = lb->nextMember(h);
      }
	}
   return TRUE;
}
Bool ExtractEvents(CLINE_handle hCLINE, LinesTotalInfo  *lti)
{

	Bool hor;//горизонтальный - вертикальный
	LnsInfoArray larr;
	CLINE_handle hLine, hEvent, hInterval;//хэндлы
	DEvent Event;//структура для дэшей
	DInterval Interval;//структура для интервалов
	CPDLine pLine;//указатель на структуру для линии
	int count_lines = 0;
	Bool first_ver = 1;
	for( hLine = CLINE_GetFirstLine(hCLINE); hLine; hLine=CLINE_GetNextLine(hLine) )
	{//цикл по всем линиям в контейнере ( и гор. и верт. )

		pLine=CLINE_GetLineData(hLine);//указатель по хэндлу

		if(!pLine)//проверка на вшивость
			continue;
		if( pLine->Dir == LD_Horiz )//определение какая линия, горизонтальная или вертикальная
		{
			hor = TRUE;
			larr = lti->Hor;// взяли структуру гори. линии а ля Lns32
		}
		else
		{
			if( first_ver )
			{
				first_ver = 0;
				count_lines = 0;
			}

			hor = FALSE;
			larr = lti->Ver;// взяли структуру верт. линии а ля Lns32
		}
		// проверим готовность структур данных ( кусок извлечен из _preSwp() )
		TLinesBambuk* lb = hor ? hLB : vLB;
		if (lb == NULL) return WRONG(); // false
		TRasterBambuk* rb = hor ? hRB : vRB;
		if (rb == NULL) return WRONG();
		TSegBambuk* sb = hor ? &(hvSB->hBambuk) : &(hvSB->vBambuk);
		Int32 root_len = lb->linesRoot.volume();
		if (sb == NULL) return WRONG();

		//вот это главная проблема. Здесь на руках имеем линия а ля Контейнер CLINE, как узнать
		//какая линия а ля Lns32 ей соответствует?????

		//int i = from_pLine_to_larr_index(pLine); // pLine->??

		//TLineInfo& li = larr[i]; // взяли описатель i-й линии
		/* Возможен нулевой хэндл !
		while( larr.Lns[count_lines].ExtrDllHnd == 0 )
		{
			count_lines++;
		}
		*/
        if (larr.Lns[count_lines].Extractor != LI_LNSDLL)
	    {
			//int u = 0;
			continue;  // не из LNSDLL Пришлось закоментировать!!!
		}


		Int32 hnd = larr.Lns[count_lines].ExtrDllHnd;     /// начинаем проход по линии Lns[i]
		count_lines++;
		/*
		if( hnd == 0 )
			return WRONG();
			*/
		if (hnd >= root_len ) return WRONG();
		TLineInfo & li = lb->linesRoot[hnd];
		BEntry   lbe = li.linesBambukEntry;      /// линия - набор фрагментов, каждый из которых является компонентой связности  (КС)



		BHandle h = lb->firstEntryMember( lbe ); /// берем первый фрагмент

		while ( h !=NULLBHandle  ) /// цикл по фрагментам линии
		{
			 TLineFragment& lf = (*lb)[h]; /// описатель фрагмента линии

			if (lf.fragmentAsIs.flags & LF_HLINER) /// этот кусок линии был выделен другим алгоритмом
			{
				h = lb->nextMember(h);
				continue;
			}

			BEntry rbe = lf.rasterBambukEntry;  /// вход в список  эвентов - 'Dash'
			BHandle hr = rb->firstEntryMember( rbe ); /// номер первого эвент
			while ( hr != NULLBHandle ) /// по всем эвентам
			{
				TDash& dash = (*rb)[hr]; /// взяли дэш (=эвент)
				hEvent = CLINE_AddNewEvent( hLine ); // создали эвент
				BHandle sh = dash.firstSegHandle;
				while ( sh != NULLBHandle )  /// пошли по его списку интервалов
				{
					TBlackSeg& bs = (*sb)[sh];///взяли интервал
					hInterval = CLINE_AddNewEventInv( hEvent );
					Interval.Pos = bs.left;//наполняем структуру
  					Interval.Lent = bs.right-bs.left+1;//наполняем структуру
					CLINE_SetEventInvData( hInterval, &Interval );//откладываем в контейнер интервал ( сегмент )
					sh = bs.nLower;
				}
				Event.Hori = pLine->Dir == LD_Horiz;//hor;//pLine->Dir;//наполняем структуру
				Event.Increase = TRUE;//наполняем структуру
				Event.Lev_0 = dash.firstSegEntry; /// начинается с этой строки изображения
				Event.Width = 0;

				CLINE_SetEventData( hEvent, &Event );//откладываем в контейнер эвент ( дэш )
				hr = rb->nextMember(hr); // переходим к следующему дэшу
			}
			h = lb->nextMember(h);
		} // конец цикла по фрагментам линии
	} // конец цикла по hLINE


   return TRUE;
}
/*
Bool _MyPreSwp( LnsInfoArray& larr, Bool hor )
{
	int count = 0;
   if (larr.Cnt == 0)    return TRUE;
   if (larr.Lns == NULL) return WRONG();

   TLinesBambuk* lb = hor ? hLB : vLB;
   if (lb == NULL) return WRONG();
   TRasterBambuk* rb = hor ? hRB : vRB;
   if (rb == NULL) return WRONG();
   TSegBambuk* sb = hor ? &(hvSB->hBambuk) : &(hvSB->vBambuk);
	if (sb == NULL) return WRONG();

   Int32 root_len = lb->linesRoot.volume();
   for ( int i =0; i<larr.Cnt; i++ )
   {

      if ((larr.Lns[i].Flags & LI_COVERED) != 0)
      {
         LineInfo & cover = larr.Lns[larr.Lns[i].IndCover];
         if (!(cover.Flags & LI_SWEEP))
            continue;  // if not must be sweepped - do nothing here;
      }
      else if ((larr.Lns[i].Flags & LI_SWEEP) == 0)
      {  continue;  // if not must be sweepped - do nothing here;
      }

      if (larr.Lns[i].Extractor != LI_LNSDLL)
      {  continue;  // if not extracted by this - do nothing here;
      }

      // if here - this is noise line; let's delete it from bambuks!

	  //******************Rom****************
	  Int32 RightX = larr.Lns[i].B.x;        // set restrictions for case of sweeping
	  Int32 LeftX = larr.Lns[i].A.x;         // not whole line; initially - full line
	  Int32 TopY = larr.Lns[i].A.y;
	  Int32 BottomY = larr.Lns[i].A.y;

	  if(larr.Lns[i].A.y>larr.Lns[i].B.y)
	  {
		  BottomY = larr.Lns[i].B.y;
	  }
	  else
	  {
		  TopY = larr.Lns[i].B.y;
	  }
	  //*************************************


	  if (larr.Lns[i].Flags & LI_NOTWHOLE)  // correct restriction if not whole line should be sweeped
      {
		  if ( larr.Lns[i].Anew.x > larr.Lns[i].A.x)
		  {
			  if (larr.Lns[i].Anew.x > larr.Lns[i].B.x) larr.Lns[i].Anew.x = larr.Lns[i].A.x;
			  LeftX = larr.Lns[i].Anew.x;
		  }
		  if ( larr.Lns[i].Anew.y > larr.Lns[i].A.y)
		  {
			  if (larr.Lns[i].Anew.y > TopY) larr.Lns[i].Anew.y = larr.Lns[i].A.y;
			  BottomY = larr.Lns[i].Anew.y;
		  }
		  if ( larr.Lns[i].Bnew.x < larr.Lns[i].B.x)
		  {
			  if (larr.Lns[i].Bnew.x < larr.Lns[i].A.x) larr.Lns[i].Bnew.x = larr.Lns[i].B.x;
			  RightX = larr.Lns[i].Bnew.x;
		  }
		  if ( larr.Lns[i].Bnew.y < larr.Lns[i].B.y)
		  {
			  if (larr.Lns[i].Bnew.y < BottomY) larr.Lns[i].Bnew.y = larr.Lns[i].B.y;
			  TopY = larr.Lns[i].Bnew.y;
		  }
      }
	  //*************************************
////////////////////
      Int32 hnd = larr.Lns[i].ExtrDllHnd;
      if (hnd >= root_len ) return WRONG();
      TLineInfo & li = lb->linesRoot[hnd];
		BEntry   lbe = li.linesBambukEntry;
      BHandle h = lb->firstEntryMember( lbe );
      while ( h !=NULLBHandle  )
      {
         TLineFragment& lf = (*lb)[h];

         if (lf.fragmentAsIs.flags & LF_HLINER)
         {
            h = lb->nextMember(h);
            continue;
         }

			BEntry rbe = lf.rasterBambukEntry;
         BHandle hr = rb->firstEntryMember( rbe );
#ifndef NO_17FEB99         // prepear to detect touching letters
         int frag_width[4000];  // width by len distribution

         // compute real left and right fragment's bounds
         int frag_left=0xffff; int frag_right = 0;
		          while ( hr != NULLBHandle )
         {
				TDash& dash = (*rb)[hr];
				BHandle sh = dash.firstSegHandle;
				while ( sh != NULLBHandle )
            {
					TBlackSeg& bs = (*sb)[sh];
               frag_left = MIN( frag_left, bs.left );
               frag_right = MAX( frag_right, bs.right );
               sh = bs.nLower;
            }
            hr = rb->nextMember(hr);
         }
         hr =  rb->firstEntryMember( rbe );

         int frag_len = frag_right-frag_left+1;
         int frag_base =frag_left;

         double ave_frag_width = 2.0; // default, for any

         // avoid sweep short fragments at the bound of line
	//  cos probably part of letters:
#ifndef SHORT_FRAGMENTS_ARE_TESTED_TO_BE_NOT_PART_OF_LETTER
         if (  hor &&
               frag_len < 50 && // short fragment
               (  lf.fragmentAsIs.end.x == li.lineAsIs.end.x // at the end of line
                ||lf.fragmentAsIs.start.x == li.lineAsIs.start.x
               )
            )
         {  // skip it, don't sweep.
            h = lb->nextMember(h);
            continue;
         }
#endif  // SHORT_FRAGMENTS_ARE_TESTED_TO_BE_NOT_PART_OF_LETTER

	 // accumulate width distribution
         if (hor && frag_len > 0 && frag_len*sizeof(frag_width[0]) < sizeof(frag_width))
         {
            memset(frag_width, 0, frag_len*sizeof(frag_width[0]));
            while ( hr != NULLBHandle )
            {
				   TDash& dash = (*rb)[hr];
				   BHandle sh = dash.firstSegHandle;
				   while ( sh != NULLBHandle )
               {
					   TBlackSeg& bs = (*sb)[sh];
                  frag_width[ bs.left -frag_base]++;
                  frag_width[ bs.right-frag_base]--;
                  sh = bs.nLower;
               }
               hr = rb->nextMember(hr);
            }
            int sum_pel = 0;
            for (int i=1;i<frag_len; i++)
            {
               frag_width[i] += frag_width[i-1];
               sum_pel += frag_width[i];
            }
            ave_frag_width = sum_pel / frag_len;

            hr = rb->firstEntryMember( rbe ); // reset hr
         }
#endif // NO_17FEB99


		 // start main loop of presweeping:
         while ( hr != NULLBHandle )
         {
				TDash& dash = (*rb)[hr];
				Romi = 0;
				BHandle sh = dash.firstSegHandle;
				while ( sh != NULLBHandle )
            {
					TBlackSeg& bs = (*sb)[sh];
				if (hor)
				{
#ifndef NO_17FEB99         // prepear to detect touching letters:
                           // keep short segment if local jump of width > 1.5 pel
               if (!(bs.right - bs.left < 24 && // short interval and width jump > 1.5 pel
                     (  double(frag_width[(bs.left+bs.right)/2 - frag_base]) > ave_frag_width + 1.5
                      ||double(frag_width[bs.left+4 - frag_base]) > ave_frag_width + 1.5
                      ||double(frag_width[bs.right-4 - frag_base]) > ave_frag_width + 1.5
                     )
                  ) )
#endif
					if ( (bs.right > LeftX) && (bs.left < RightX) )
					{
						if (bs.left < LeftX-5) bs.left = LeftX;
						if (bs.right > RightX+5) bs.right = RightX;
						int xx = bs.left;
						bs.left = bs.right;
						bs.right = xx;
						count++;
					}
					// swap segment to indicate it's SWEEPABILITY
				}
				//*********************Rom**********************
				else
				{
					if ( (bs.right > BottomY) && (bs.left < TopY) )
					{
						if (bs.left < BottomY-5) bs.left = BottomY;
						if (bs.right > TopY+5) bs.right = TopY;
						if(RBlockCurrent->Current >= RBlockCurrent->Max)
						{
							RBlockPrev = RBlockCurrent;
							RBlockCurrent = new CRomBabble;
							RBlockPrev->next = RBlockCurrent;
						}
						Rptr2 = &RBlockCurrent->Data[RBlockCurrent->Current];
						Rptr2->Begin  = bs.left;
						Rptr2->End    = bs.right+4;
						Rptr2->Column = dash.firstSegEntry+Romi;
						RBlockCurrent->Current++;
						Rptr2->next = NULL;
						Rptr3->next = Rptr2;
						Rptr3 = Rptr3->next;
					}
					Romi++;
				}
				//**********************************************
               sh = bs.nLower;
            }
            hr = rb->nextMember(hr);
         }

         h = lb->nextMember(h);
      }
	}
   return TRUE;
}
*/
Bool ExtrLinesPrepearToSweep( LinesTotalInfo * lti )
{
   if (lti==NULL) return WRONG();
   //Rptr = GetRomptr();
   if (lti->Hor.Cnt > 0)
   {
      if (lti->Hor.Lns==NULL) return WRONG();
      if (!_PreSwp( lti->Hor, TRUE ))
         return WRONG();
   }
   //***************************Rom**********************
   if (lti->Ver.Cnt > 0)
   {
      if (lti->Ver.Lns==NULL) return WRONG();
      if (!_PreSwp( lti->Ver, FALSE ))
         return WRONG();
   }

   SortList  ( Rptr );
   SetRomptr ( Rptr );
   //****************************************************


   InitSweeperData( hvSB );
   return TRUE;
}

//**********************Rom***************
Bool  SortList ( RomBubble* rptr)
{
	Bool			IsOk = TRUE;
	Bool			Unsorted = TRUE;
	RomBubble*		Rptr0=NULL;
	RomBubble*		Rptr1=NULL;
	RomBubble*		Rptr2=NULL;

	while (( Unsorted ) && (Rptr->next!=NULL))
	{
		Rptr0 = Rptr;
		Rptr1 = Rptr->next;
		Unsorted = FALSE;
		while (Rptr1->next != NULL )
		{
			if(Rptr1->Begin > Rptr1->next->Begin)
			{
				Rptr2 = Rptr1->next;
				Rptr1->next = Rptr1->next->next;
				Rptr2->next = Rptr1;
				Rptr1 = Rptr2;
				Rptr0->next = Rptr2;
				Unsorted = TRUE;
			}
			else
			{
				Rptr0 = Rptr1;
				Rptr1 = Rptr1->next;
			}
		}
	}

	return IsOk;
}

