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

//#include "lnslang.h"

#ifndef PPS_MAC
#pragma hdrstop
#endif

	#ifndef __PUANSO_H
	#	include "puanso.h"
	#endif

	#ifndef __RBAMBUK_H
	#	include "rbambuk.h"
	#endif

	#ifndef __LBAMBUK_H
	#	include "lbambuk.h"
	#endif

	#  ifndef __STDLIB_H
	#  	include <stdlib.h>
	#  endif

#include "hliner.h"
#include "decl.h"

	int cdecl compFragLevels( const void *frag1, const void *frag2 ); //used by qsort()

   void RegisterBlackSquare(int left, int top, int right, int bottom );
   void RegisterCheckBox( Rect16& rcbox );

   static Bool has_H_linked(  PLine  line, PLine  frag );
   static Bool has_V_linked(  PLine  line, PLine  frag );
	static inline int   line_length( PLine line );

   static TRasterBambuk*   rb_ = NULL;
   static TSegBambuk*      sb_ = NULL;

static int Dummy(int x) { return x+1;};

Bool TLinesBambuk::makeIt( TRasterBambuk* rb, TSegBambuk* sb, Bool isVertical ){

   *((TXYDim*)this) = *((TXYDim*)sb);         // 11-04-93 07:56pm

	if (isVertical){
		computeVRasters( rb, sb );
/*
      if (!linkVFragments())
         return WRONG();
*/
	}
	else {
		computeHRasters( rb, sb );
/*
      if (!linkHFragments())
         return WRONG();
*/
	};
   return TRUE;
};

/// 10.02.99, VP ------- registering fragments for external usage
#include "frag.h"
/// --------------------

void TLinesBambuk::computeHRasters( TRasterBambuk* rb, TSegBambuk* sb ){

	rb_ = rb;
	sb_ = sb;
	assert(rb_);
	assert(sb_);

	BEntry last_comp = rb_->lastEntry();
	BHandle dash, seg;
   BEntry row;
	BHandle frag_handle = 0;
   PTLineFragment fragment;
	TPuanso puanso;
	double sum_phi = 0.0;
   Int32 sum_len = 0;

	int down, up;
/*
   #ifndef NDEBUG
      Bool lets_draw = FALSE;       // set true to look each raster - TRUE;
   #endif
*/
/// 10.02.99, VP ------- registering fragments for external usage
   Frag_HAlloc( rb->totalRasterCount());
   int nfrag =0;
/// --------------------

	for ( BEntry comp	= 0; comp <= last_comp; comp++ ) 
   {
      dash = rb_->firstEntryMember( comp );
		if (dash ==NULLBHandle)
         continue;
		// compute tenzor of inertia...
		puanso.flashPuanso();
		down = 0x7FF0;
		up = 0;
		do {
         TDash& dd = (*rb_)[ dash ];
         seg = dd.firstSegHandle;   assert( seg != NULLBHandle );
         row = dd.firstSegEntry;
			down = min( down, (int)row );
			do {
            TBlackSeg& bs = (*sb_)[ seg ];
				puanso.increase( bs.left,
                             bs.right,
									  row++
									);
				seg = bs.nLower;
         } while ( seg != NULLBHandle );
			up = max( up, (int)row );
		} while ((dash = rb_->nextMember( dash ))!=NULLBHandle);

      puanso.atOwnCoords();

      /////////////// VP: 15 jul 98 : cutting a line   
      Bool isLine = puanso.isALine();

/// 10.02.99, VP ------- registering fragments for external usage
      LnsFrag* frg = Frag_HGet( nfrag ); nfrag++;
      {
         if (frg)
         {
            if (isLine)
               frg->flags = LF_LINE;
            else
               frg->frag_handle = -1;
            frg->rc.left  = puanso.left;
            frg->rc.right = puanso.right-2;
            frg->rc.top = down;
            frg->rc.bottom = up-1;
		      frg->mass    =(long)puanso.mass   ;
		      frg->Xc      =puanso.Xc     ;
	         frg->Yc      =puanso.Yc     ;
	         frg->Phi     =puanso.Phi    ;
            frg->aveW    =puanso.aveW   ;   
		      frg->mainMxx =puanso.mainMxx;
	         frg->mainMyy =puanso.mainMyy;
//		      frg->relMxx  =puanso.relMxx ;	
//		      frg->relMxy  =puanso.relMxy ;	
//		      frg->relMyy  =puanso.relMyy ;	

			   frg->A.x 	= puanso.left;
			   frg->B.x 	= puanso.right - 1;
			   frg->A.y 	=(Int16)( puanso.Yc + (puanso.left-puanso.Xc)*puanso.Phi );
			   frg->B.y 	=(Int16)( puanso.Yc + (puanso.right-puanso.Xc)*puanso.Phi );
         }
      }
///-----------------------

      if (!isLine && (puanso.right-puanso.left) > 600)
      {  // try to recover long line with glued dust
         int initial_comp_width = puanso.right-puanso.left;
/////////////////////////////////////////////////////////////////////////////////
         {
         // 1. find zone with bad line width
            int wdt[512]={0};
            dash = rb_->firstEntryMember( comp );
		      do 
            {
               TDash& dd = (*rb_)[ dash ];
               seg = dd.firstSegHandle;   assert( seg != NULLBHandle );
               row = dd.firstSegEntry;
			      do 
               {
                  TBlackSeg& bs = (*sb_)[ seg ];
                  if (bs.right < 4000) // 4096, if more presize
                  {
                     wdt[bs.left >> 3]++;
                     wdt[(bs.right+1) >> 3]--;
                  }
                  seg = bs.nLower;
               } while ( seg != NULLBHandle );
		      } while ((dash = rb_->nextMember( dash ))!=NULLBHandle);

            for (int i=1; i<512; i++) 
               wdt[i] += wdt[i-1];

            int sum=0; int sumsq=0; int len=0; int max_width = 40;

            for (int pass=0; pass<2; pass++)
            {  sum=0; int sumsq=0; int len=0;
               for (int i=1; i<512; i++) 
               {
                  if (wdt[i] > 0 && wdt[i] < max_width)
                  {

                     sum += wdt[i];
                     sumsq += wdt[i]*wdt[i];
                     len ++;
                  }
               }

               if (len==0)
                  len=1;

               double ave_width = ((double)sum) / len;
               double dispers =  ( double(sumsq)/len) - ave_width*ave_width;
            
               max_width = (int)(ave_width + sqrt(dispers) + 1);
            }// pass

            int last_good = 0;
            int good_right[512]={0}; // index [i]=> right end of good good segment with i
            int good_left[512]={0};  // index [i]=> left  end of good good segment with i
            Bool outside = TRUE;
            for (int i=511; i>=0; i--)  // compute good_right[]
            {  
               if (wdt[i] && wdt[i] <= max_width) // good point
               {            
                  if (outside)   // good segment started
                  {  good_right[i] = i;
                     last_good = i;
                     outside = FALSE;                        
                  }
                  else // inside
                     good_right[i] = last_good;
               }
               else // bad point
               {
                  if (!outside) // prev point was start of good segm
                  {
                     outside = TRUE;
                  }
               }
            }
            outside = TRUE;
            for (int i=0; i < 512; i++)  // compute good_left[]
            {  
               if (wdt[i] && wdt[i] <= max_width) // good point
               {            
                  if (outside)   // good segment started
                  {  good_left[i] = i;
                     last_good = i;
                     outside = FALSE;                        
                  }
                  else // inside
                     good_left[i] = last_good;
               }
               else // bad point
               {
                  if (!outside) // prev point was start of good segm
                  {
                     outside = TRUE;
                  }
               }
            }
// 04 oct ////////////////
            {  // find longmost zone
               int maxzonelen=0; int maxzonestart = 0;
               for (int i=0; i < 512; i++)  // compute good_left[]
               {  
                  if (!good_right[i])
                     continue;
                  // reach start of zone
                  if (good_right[i]-i > maxzonelen)
                  {  
                     maxzonelen = good_right[i]-i;
                     maxzonestart = i;
                     i+= maxzonelen+1;
                  }
               }
               if ( maxzonelen > 30 ) //  we demand 240 pels of line
               {
                  for (int i=0; i<maxzonestart; i++)
                  {
                     good_left[i]=good_right[i]=0;
                  }
                  for (int i=maxzonestart+maxzonelen+1; i<512; i++)
                  {
                     good_left[i]=good_right[i]=0;
                  }
               }
               else
               {
                  for (int i=0; i<512; i++)
                  {
                     good_left[i]=good_right[i]=0;
                  }
               }
            }
//////////////////////////
         // 2. compute line parms without this zone(s)

            dash = rb_->firstEntryMember( comp );
		      if (dash ==NULLBHandle)
               continue;
		      // compute tenzor of inertia...
		      puanso.flashPuanso();
		      do 
            {
               TDash& dd = (*rb_)[ dash ];
               seg = dd.firstSegHandle;   assert( seg != NULLBHandle );
               row = dd.firstSegEntry;
			      do 
               {
                  TBlackSeg& bs = (*sb_)[ seg ];
                  int n_goodright = good_right[bs.left>>3];
                  if ( (bs.right>>3) <= n_goodright) // entire segment inside good zone
                  {  
                     puanso.increase( bs.left,bs.right,row);
                  }
                  else  // segment broken
                  {  int halflen = (bs.right-bs.left) >> 1;
                     if (halflen > 10)
                     {
                        if ( (n_goodright<<3) - bs.left > halflen) // more than half of segment on left side is good
                           puanso.increase( bs.left,n_goodright<<3,row);
                        else // let's try right part of segment
                        {
                           int n_goodleft = good_left[bs.right>>3];
                           if (n_goodleft && (bs.right>>3) - n_goodleft > halflen)   // some of right part is good
                              puanso.increase( n_goodleft<<3, bs.right,row);
                        }
                     }
                  }
                  row++;seg = bs.nLower;
               } while ( seg != NULLBHandle );
		      } while ((dash = rb_->nextMember( dash ))!=NULLBHandle);

		      puanso.atOwnCoords();
         // 3. compute full line cutting in angle of previous estimation
            if (puanso.isALine()) // wow - one of parts treated as line...
            {
               double phi = puanso.Phi;
               int xx = (int)puanso.Xc;
               int yy = (int)puanso.Yc;
               int ww = max_width + 10; //puanso.aveW * 3;
               int prj0 = (int)(yy + xx*phi);

               dash = rb_->firstEntryMember( comp );
		         if (dash ==NULLBHandle)
                  continue;
		         // compute tenzor of inertia...
		         puanso.flashPuanso();
		         do 
               {
                  TDash& dd = (*rb_)[ dash ];
                  seg = dd.firstSegHandle;   assert( seg != NULLBHandle );
                  row = dd.firstSegEntry;
			         do 
                  {

                     TBlackSeg& bs = (*sb_)[ seg ];
                     int prj = (int)(row + ((bs.right+bs.left)>>1)*phi);
                     if (abs( prj - prj0 ) < ww)
                     {
                        puanso.increase( bs.left,
                                         bs.right,
									              row
									            );
                     }
                     row++;seg = bs.nLower;
                  } while ( seg != NULLBHandle );
		         } while ((dash = rb_->nextMember( dash ))!=NULLBHandle);

		         puanso.atOwnCoords();
               isLine = puanso.isALine();
            }
         } // if (!isLine && (puanso.right-puanso.left) > 600)
      }

/////////////////////////////////////////////////////////////////////////////////
#if 0         
         int center = (puanso.right+puanso.left)>>1;

         // test left part of line
         dash = rb_->firstEntryMember( comp );
		   if (dash ==NULLBHandle)
            continue;
		   // compute tenzor of inertia...
		   puanso.flashPuanso();
		   do {
            TDash& dd = (*rb_)[ dash ];
            seg = dd.firstSegHandle;   assert( seg != NULLBHandle );
            row = dd.firstSegEntry;
			   do {
               TBlackSeg& bs = (*sb_)[ seg ];
               if (bs.left < center)
               {
				      puanso.increase( bs.left,
                                   bs.right < center ? bs.right : center,
									        row
									      );
               }
               row++;seg = bs.nLower;
            } while ( seg != NULLBHandle );
		   } while ((dash = rb_->nextMember( dash ))!=NULLBHandle);

		   puanso.atOwnCoords();

         if (!puanso.isALine())
         {  // try right part...
            dash = rb_->firstEntryMember( comp );
		      if (dash ==NULLBHandle)
               continue;
		      // compute tenzor of inertia...
		      puanso.flashPuanso();
		      do {
               TDash& dd = (*rb_)[ dash ];
               seg = dd.firstSegHandle;   assert( seg != NULLBHandle );
               row = dd.firstSegEntry;
			      do {
                  TBlackSeg& bs = (*sb_)[ seg ];
                  if (bs.right > center)
                  {
                     puanso.increase( bs.left > center ? bs.left : center,
                                      bs.right,
									           row
									         );
                  }
                  row++;seg = bs.nLower;
               } while ( seg != NULLBHandle );
		      } while ((dash = rb_->nextMember( dash ))!=NULLBHandle);

		      puanso.atOwnCoords();         
         }

         if (puanso.isALine()) // wow - one of parts treated as line...
         {
            double phi = puanso.Phi;
            int xx = puanso.Xc;
            int yy = puanso.Yc;
            int ww = puanso.aveW * 3;
            int prj0 = yy + xx*phi;

            dash = rb_->firstEntryMember( comp );
		      if (dash ==NULLBHandle)
               continue;
		      // compute tenzor of inertia...
		      puanso.flashPuanso();
		      do {
               TDash& dd = (*rb_)[ dash ];
               seg = dd.firstSegHandle;   assert( seg != NULLBHandle );
               row = dd.firstSegEntry;
			      do {
                  TBlackSeg& bs = (*sb_)[ seg ];
                  int prj = row + ((bs.right+bs.left)>>1)*phi;
                  if (abs( prj - prj0 ) < ww)
                  {
                     puanso.increase( bs.left,
                                      bs.right,
									           row
									         );
                  }
                  row++;seg = bs.nLower;
               } while ( seg != NULLBHandle );
		      } while ((dash = rb_->nextMember( dash ))!=NULLBHandle);

		      puanso.atOwnCoords();
            isLine = puanso.isALine();
         }
      }

      // end of 15 jul 98 (cutting a line)
      /////////////////////////////////////////////////////
#endif

		// now tenzor of inertia has computed. Make line now...
		if (isLine)
      {
			fragment = &((*this)[frag_handle]);
         if (frg)
            frg->frag_handle = (short)frag_handle;
			fragment->fragmentAsIs.start.x 	= puanso.left;
			fragment->fragmentAsIs.end.x 	= puanso.right - 1;
			fragment->fragmentAsIs.start.y 	=(Int32)( puanso.Yc + (puanso.left-puanso.Xc)*puanso.Phi );
			fragment->fragmentAsIs.end.y 	=(Int32)( puanso.Yc + (puanso.right-puanso.Xc)*puanso.Phi );
         fragment->fragmentAsIs.width10 = (short)(puanso.aveW * 10.);
         fragment->fragmentAsIs.flags = 0;
         if (puanso.right - puanso.left > 64)
         {
            int len = puanso.right - puanso.left;
            int factor = 1; // len >> 7; // div 128
				sum_phi += factor * len * puanso.Phi;
				sum_len += factor * len;
         }
			fragment->rasterBambukEntry = comp;
			frag_handle++;
		}; // is a line

      /////////////////////////////////////////////////
      /// 4 dec 98, VP
      /// Check for black squares:
      int height = up - down + 1;
      int width  = puanso.right-puanso.left;
      if ( height > 24 && abs(height + 2 - width) < 4 ) 
         // box is about square - diff between height and width less than 4 pels!
         // note: two pels addition for for height slightly compensates horisontal filtration
      {  double square = height* (double)width;
         if ( fabs(square - puanso.mass) < (square + puanso.mass) * 0.1 )
         {  // 80 percents of square is black
            RegisterBlackSquare(puanso.left, down, puanso.right-1, up);
         };
      };
      //////////////////////////////////////////////////
	}; // for
////////////////////
// add fragments from HLiner
   int nhl = HLiner_GetCount();
   for (int i=0; i<nhl; i++)
   {
      // array is allocated of count_of_rasters + count_of_hliner_lines, 
      // so we can add them without test

      Line16& li = *HLiner_GetLine(i);
		fragment = &((*this)[frag_handle]);
		fragment->fragmentAsIs.start.x 	= li.A.x;
		fragment->fragmentAsIs.start.y 	= li.A.y;
		fragment->fragmentAsIs.end.x	= li.B.x;
		fragment->fragmentAsIs.end.y	= li.B.y;
      fragment->fragmentAsIs.width10 = 20; //(short)(puanso.aveW * 10.);
      fragment->fragmentAsIs.flags = LF_HLINER;
		fragment->rasterBambukEntry = NULLBHandle;
		frag_handle++;
   }
///////////////////

   fragmentsCount = (Word16)frag_handle;  // count of line fragments
	if (fragmentsCount > 0 && sum_len > 0)
   {
		averagePhi = (sum_phi) / (double)(sum_len);
	}
   else
      averagePhi = 1.;  // sign off
};

void TLinesBambuk::computeVRasters( TRasterBambuk* rb, TSegBambuk* sb ){

	rb_ = rb;
	sb_ = sb;
	assert(rb_);
	assert(sb_);

	BEntry last_comp = rb_->lastEntry();
	BHandle dash, seg;
   BEntry col;
	BHandle frag_handle = 0;
//   PTLineFragment fragment;
	TPuanso puanso;
	double sum_phi = 0.0;
   Int32 sum_len = 0;

/// 10.02.99, VP ------- registering fragments for external usage
   Frag_VAlloc( rb->totalRasterCount() );
   int nfrag=0;
/// --------------------

	for ( BEntry comp	= 0; comp <= last_comp; comp++ ) 
   {  dash = rb_->firstEntryMember( comp );
      
		if (dash ==NULLBHandle)
         continue;

		// compute tenzor of inertia...
		puanso.flashPuanso();
      int left = 0x7ff0;
      int right= 0;
		do {
         TDash& dd = (*rb_)[ dash ];
         seg = dd.firstSegHandle;            assert( seg != NULLBHandle );
         col = dd.firstSegEntry;
         left = min( left, (int)col );
         do {
            TBlackSeg& bs = (*sb_)[ seg ];
            puanso.increase( bs.left,    // down
                             bs.right,   // up
									  col
									);
            col++; seg = bs.nLower;
         }  while ( seg != NULLBHandle );
         right = max( right, (int)col );
		} while ((dash = rb_->nextMember( dash ))!=NULLBHandle);

		puanso.atOwnCoords();

      /////////////// VP: 15 jul 98 : cutting a line   
      Bool isLine = puanso.isALine();

/// 10.02.99, VP ------- registering fragments for external usage
      LnsFrag* frg = Frag_VGet( nfrag ); nfrag++;
      {
         if (frg)
         {
            if (isLine)
               frg->flags = LF_LINE;
            else
               frg->frag_handle = -1; // else will set below

            frg->rc.left  = left; 
            frg->rc.right = right-1;
            frg->rc.top = puanso.left;
            frg->rc.bottom = puanso.right+1;

		      frg->mass    = (long)puanso.mass   ;
		      frg->Xc      =puanso.Yc     ;
	         frg->Yc      =puanso.Xc     ;
	         frg->Phi     =puanso.Phi    ;//???   transp?
            frg->aveW    =puanso.aveW   ;   
		      frg->mainMxx =puanso.mainMxx;//???   transp?
	         frg->mainMyy =puanso.mainMyy;//???
//		      frg->relMxx  =puanso.relMxx ;//???	
//		      frg->relMxy  =puanso.relMxy ;//???	
//		      frg->relMyy  =puanso.relMyy ;//???	
            frg->A.y     = puanso.left;
            frg->B.y     = puanso.right + 1;
            frg->A.x     = (Int16)( puanso.Yc + (puanso.left-puanso.Xc)*puanso.Phi );
            frg->B.x     = (Int16)( puanso.Yc + (puanso.right-puanso.Xc)*puanso.Phi );
         }
      }
///-----------------------

      if (!isLine && (puanso.right-puanso.left) > 600)
      {  // try to recover long line with glued dust
         int center = (puanso.right+puanso.left)>>1;

         // test left part of line
         dash = rb_->firstEntryMember( comp );
		   if (dash ==NULLBHandle)
            continue;
		   // compute tenzor of inertia...
		   puanso.flashPuanso();
		   do {
            TDash& dd = (*rb_)[ dash ];
            seg = dd.firstSegHandle;   assert( seg != NULLBHandle );
            col = dd.firstSegEntry;
			   do {
               TBlackSeg& bs = (*sb_)[ seg ];
               if (bs.left < center)
               {
				      puanso.increase( bs.left,
                                   bs.right < center ? bs.right : center,
									        col
									      );
               }
               col++;seg = bs.nLower;
            } while ( seg != NULLBHandle );
		   } while ((dash = rb_->nextMember( dash ))!=NULLBHandle);

		   puanso.atOwnCoords();

         if (!puanso.isALine())
         {  // try right part...
            dash = rb_->firstEntryMember( comp );
		      if (dash ==NULLBHandle)
               continue;
		      // compute tenzor of inertia...
		      puanso.flashPuanso();
		      do {
               TDash& dd = (*rb_)[ dash ];
               seg = dd.firstSegHandle;   assert( seg != NULLBHandle );
               col = dd.firstSegEntry;
			      do {
                  TBlackSeg& bs = (*sb_)[ seg ];
                  if (bs.right > center)
                  {
                     puanso.increase( bs.left > center ? bs.left : center,
                                      bs.right,
									           col
									         );
                  }
                  col++;seg = bs.nLower;
               } while ( seg != NULLBHandle );
		      } while ((dash = rb_->nextMember( dash ))!=NULLBHandle);

		      puanso.atOwnCoords();         
         }

         if (puanso.isALine()) // wow - one of parts treated as line...
         {
            double phi = puanso.Phi;
            int yy = (int)puanso.Xc;
            int xx = (int)puanso.Yc;
            int ww = (int)puanso.aveW * 3;
            int prj0 = (int)(xx - yy*phi);

            dash = rb_->firstEntryMember( comp );
		      if (dash ==NULLBHandle)
               continue;
		      // compute tenzor of inertia...
		      puanso.flashPuanso();
		      do {
               TDash& dd = (*rb_)[ dash ];
               seg = dd.firstSegHandle;   assert( seg != NULLBHandle );
               col = dd.firstSegEntry;
			      do {
                  TBlackSeg& bs = (*sb_)[ seg ];
                  int prj = (int)(col - ((bs.right+bs.left)>>1)*phi);
                  if (abs( prj - prj0 ) < ww)
                  {
                     puanso.increase( bs.left,
                                      bs.right,
									           col
									         );
                  }
                  col++;seg = bs.nLower;
               } while ( seg != NULLBHandle );
		      } while ((dash = rb_->nextMember( dash ))!=NULLBHandle);

		      puanso.atOwnCoords();
            isLine = puanso.isALine();
         }
      }

      // end of 15 jul 98 (cutting a line)
      /////////////////////////////////////////////////////

		// now tenzor of inertia has computed. Make line now...
		if (isLine){
         TLineFragment& frag = (*this)[frag_handle];
//////// 13 may 99
         if (frg)
            frg->frag_handle = (short)frag_handle;
////////
         frag.fragmentAsIs.start.y   = puanso.left;
         frag.fragmentAsIs.end.y  = puanso.right + 1;
         frag.fragmentAsIs.start.x   = (Int32)( puanso.Yc + (puanso.left-puanso.Xc)*puanso.Phi );
         frag.fragmentAsIs.end.x  = (Int32)( puanso.Yc + (puanso.right-puanso.Xc)*puanso.Phi );
         frag.fragmentAsIs.width10 = (short)(puanso.aveW * 10.);
         frag.fragmentAsIs.flags = 0;
         if (puanso.right - puanso.left > 64)
         {
            int len = puanso.right - puanso.left;
            int factor = 1; // len >> 7; // div 128
				sum_phi -= factor * len * puanso.Phi;
				sum_len += factor * len;
         }
         frag.rasterBambukEntry = comp;
			frag_handle++;
		}; // is a line

	}; // for
   fragmentsCount = (Int32)frag_handle;  // count of line fragments
	if (fragmentsCount > 0 && sum_len > 0)
   {
		averagePhi = (sum_phi) / (double)(sum_len);
	}
   else
      averagePhi = 1.; // sign off
};

Bool TLinesBambuk::linkHFragments( void ){
	// compute fragments levels
   double _averagePhi = averagePhi == 1. ? 0 : averagePhi;
   for( BHandle frag = 0; frag < fragmentsCount; frag++)
   {  TLineFragment& frg =(*this)[frag];
      frg.level = frg.fragmentAsIs.start.y -
			  (int)
           (   (double)( frg.fragmentAsIs.start.x ) * _averagePhi
			  );
	};
	// sort fragments by level
   if (fragmentsCount > 0)
      qsort( &((*this)[0]), fragmentsCount, sizeof(TLineFragment), compFragLevels );

//	cleanLinks();
	// first pass: make initial lines by fragments
   Bool not_linked[ MAX_H_RASTERS_COUNT ];
	for (int i = 0; i < MAX_H_RASTERS_COUNT; i++) not_linked[i] = TRUE;

	int cur_line = 0;
	BHandle base_frag = 0;  // first unlinked fragment
	BHandle next_frag = 0;  // current fragment to try link
	while(  base_frag < fragmentsCount ){
		// copy and link first
		appendMember( cur_line, base_frag );
		not_linked[ base_frag ] = FALSE;
		linesRoot[ cur_line ].lineAsIs = (*this)[base_frag].fragmentAsIs; //??
		linesRoot[ cur_line ].level = (*this)[base_frag].level;
		linesRoot[ cur_line ].linesBambukEntry = cur_line;
		linesCount++;
		// link next fragments in near levels (if can)
		next_frag = base_frag+1;
		while ( 	(next_frag < fragmentsCount)
					&&
					(	abs(
							linesRoot[cur_line].level - (*this)[next_frag].level
							) < CHECK_FRAGMENTS_DELTA
					)
				)
		{ // try to link...
			if	(	not_linked[next_frag]
					&&
					has_H_linked( 	&(linesRoot[ cur_line ].lineAsIs),
										&((*this)[ next_frag ].fragmentAsIs)
							 )
				){ appendMember( cur_line, next_frag );
					not_linked[ next_frag ] = FALSE;
					next_frag = base_frag;    // lets repeat trying
			};
			next_frag++;
		};
		// seek next base_frag
		base_frag++;
		while ( (base_frag < fragmentsCount)
					&&
				  !not_linked[ base_frag ]
				)
		{ 		base_frag ++;
		};
		cur_line++;
      if (cur_line > lastEntry())
      {
         assert(cur_line <= lastEntry());
         return FALSE;
      }
		#ifndef NDEBUG
			//if ( (cur_line % 10) == 0 )
			//	printf(".");
		#endif
	}; // while bas_frag...
	// second pass - try link lines ... (???)
	// <...>
   return TRUE;
};

Bool TLinesBambuk::linkVFragments( void ){
	// compute fragments levels
   double _averagePhi = averagePhi == 1. ? 0 : averagePhi;
   for( BHandle frag = 0; frag < fragmentsCount; frag++)
   {  TLineFragment& frg =(*this)[frag];
      frg.level = frg.fragmentAsIs.start.x +
			  (int)
           (   (double)( frg.fragmentAsIs.start.y ) * _averagePhi
			  );
	};
	// sort fragments by level
   if (fragmentsCount > 0)
      qsort( &((*this)[0]), fragmentsCount, sizeof(TLineFragment), compFragLevels );

//	cleanLinks();

	// first pass: make initial lines by fragments
   Bool not_linked[ MAX_V_RASTERS_COUNT ];
	for (int i = 0; i < MAX_V_RASTERS_COUNT; i++) not_linked[i] = TRUE;

	int cur_line = 0;
	BHandle base_frag = 0;  // first unlinked fragment
	BHandle next_frag = 0;  // current fragment to try link
	while(  base_frag < fragmentsCount ){
		// copy and link first
		appendMember( cur_line, base_frag );
		not_linked[ base_frag ] = FALSE;
		linesRoot[ cur_line ].lineAsIs = (*this)[base_frag].fragmentAsIs; //??
		linesRoot[ cur_line ].level = (*this)[base_frag].level;
		linesRoot[ cur_line ].linesBambukEntry = cur_line;
		linesCount++;
		// link next fragments in near levels (if can)
		next_frag = base_frag+1;
		while ( 	(next_frag < fragmentsCount)
					&&
					(	abs(
							linesRoot[cur_line].level - (*this)[next_frag].level
							) < CHECK_FRAGMENTS_DELTA
					)
				)
		{ // try to link...
			if	(	not_linked[next_frag]
					&&
					has_V_linked( 	&(linesRoot[ cur_line ].lineAsIs),
										&((*this)[ next_frag ].fragmentAsIs)
							 )
				){ appendMember( cur_line, next_frag );
					not_linked[ next_frag ] = FALSE;
					next_frag = base_frag;    // lets repeat trying
			};
			next_frag++;
		};
		// seek next base_frag
		base_frag++;
		while ( (base_frag < fragmentsCount)
					&&
				  !not_linked[ base_frag ]
				) { base_frag ++;
		};
		cur_line++;
      if (cur_line > lastEntry())
      {
         assert(cur_line <= lastEntry());
         return FALSE;
      }
		#ifndef NDEBUG
			//if ( (cur_line % 10) == 0 )
			//	printf(".");
		#endif
	};
	// second pass - try link lines ... (???)
	// <...>
   return TRUE;
};

/********************************************************/
int cdecl compFragLevels( const void *frag1, const void *frag2 ){
	return 	( (*(TLineFragment*)frag1).level -
				  (*(TLineFragment*)frag2).level
				);
};

#if 0  // old, before 04 jun 99
Bool  has_H_linked(  PLine  line, PLine  frag ){

   if (line->flags & LF_DONTLINK || frag->flags & LF_DONTLINK)
      return FALSE;

   Int32 sum_len = (line->end.x - line->start.x) + (frag->end.x - frag->start.x);
   Int32 gap = 0;

   if ( abs( line->width10 - frag->width10 ) > 15 ) // more than 1.5 pels width diff!
      return FALSE;

   if (  ( (gap = abs(frag->start.x - line->end.x)) < LINK_FRAGMENTS_GAP )
         &&
         ( (gap * LINK_FRAGMENTS_RATIO) < sum_len )
			&&
			( abs(frag->start.y - line->end.y) < LINK_FRAGMENTS_DELTA )
		) {
		line->end = frag->end;
		return (TRUE);
	};

   if (  ( (gap = abs(line->start.x - frag->end.x)) < LINK_FRAGMENTS_GAP )
         &&
         ( (gap * LINK_FRAGMENTS_RATIO) < sum_len )
			&&
			( abs(line->start.y - frag->end.y) < LINK_FRAGMENTS_DELTA )
		) {
		line->start = frag->start;
		return (TRUE);
	};

	return (FALSE);
};
#else // 04 jun 99 - join overlapped too
Bool  has_H_linked(  PLine  line, PLine  frag )
{
   if (line->flags & LF_DONTLINK || frag->flags & LF_DONTLINK)
      return FALSE;

   Int32 sum_len = (line->end.x - line->start.x) + (frag->end.x - frag->start.x);
   Int32 gap = 0;

   if ( abs( line->width10 - frag->width10 ) > 15 ) // more than 1.5 pels width diff!
   {
      if (!(line->flags & LF_HLINER || frag->flags & LF_HLINER)) // currently, 04 jun 99, HLiner reports not correct width 
         return FALSE;
   }

   if (frag->start.x > line->end.x) // fragment is on right side of the line
   {
      gap = frag->start.x - line->end.x;
      if (  gap < LINK_FRAGMENTS_GAP 
            &&
            (gap * LINK_FRAGMENTS_RATIO < sum_len )
			   &&
			   ( abs(frag->start.y - line->end.y) < LINK_FRAGMENTS_DELTA )
		   ) 
      {
		   line->end = frag->end;
		   return (TRUE);
	   };
      return (FALSE); // else not linked
   }

   if (line->start.x > frag->end.x) // fragment is on left side of the line
   {
      gap = line->start.x - frag->end.x;
      if (  gap < LINK_FRAGMENTS_GAP 
            &&
            gap * LINK_FRAGMENTS_RATIO < sum_len 
			   &&
			   abs(line->start.y - frag->end.y) < LINK_FRAGMENTS_DELTA 
		   ) 
      {
		   line->start = frag->start;
		   return (TRUE);
	   };
      return (FALSE); // else not linked
   }

   // here we have two overlapped lines...
   Point32 *p_left, *p_right; // leftmost and rightmost points
   Point32 *p_rest1,*p_rest2; // the rest points
   if (line->start.x < frag->start.x)
   {
      p_left = &line->start;
      p_rest1= &frag->start;
   }
   else
   {
      p_left = &frag->start;
      p_rest1= &line->start;
   }

   if (line->end.x > frag->end.x)
   {
      p_right= &line->end;
      p_rest2= &frag->end;
   }
   else
   {
      p_right= &frag->end;
      p_rest2= &line->end;
   }

   {  // test rest1
      int x1= p_rest1->x - p_left->x;
      int x2= p_right->x - p_rest1->x;

      if (x1 + x2 == 0)
         return (FALSE);
      int y = ( p_left->y * x2 + p_right->y * x1 ) / (x1 + x2);
      if (abs(p_rest1->y - y) > 4)
         return (FALSE); // more than 4 pixels diff
   }
   {  // test rest2
      int x1= p_rest2->x - p_left->x;
      int x2= p_right->x - p_rest2->x;

      if (x1 + x2 == 0)
         return (FALSE);
      int y = ( p_left->y * x2 + p_right->y * x1 ) / (x1 + x2);
      if (abs(p_rest2->y - y) > 4)
         return (FALSE); // more than 4 pixels diff
   }

   if (line->flags & LF_HLINER || frag->flags & LF_HLINER)
   {
      if ( abs(line->start.x - frag->start.x) >= 16 ) // diff is big
         line->start = *p_left; // change if diff more than 16 pels, else keep old
      else // diff small - get from not HLiner
      {
         if (line->flags & LF_HLINER && !(frag->flags & LF_HLINER)) // frag more precise
            line->start = frag->start;
      }

      if ( abs(line->end.x - frag->end.x) >= 16 ) // diff is big
         line->end = *p_right; // change if diff more than 16 pels, else keep old
      else // diff small - get from not HLiner
      {
         if (line->flags & LF_HLINER && !(frag->flags & LF_HLINER)) // frag more precise
            line->end = frag->end;
      }
   }
   else // both lines not from HLINER
   {
      line->start = *p_left;
      line->end   = *p_right;
   }

	return (TRUE);
};

#endif

inline int   line_length( PLine line )
{
	return max(abs(line->end.x-line->start.x), abs(line->end.y-line->start.y));
};


Bool  has_V_linked(  PLine  line, PLine  frag ){

   if (line->flags & LF_DONTLINK || frag->flags & LF_DONTLINK)
      return FALSE;

   if ( abs( line->width10 - frag->width10 ) > 15 ) // more than 1.5 pels width diff!
      return FALSE;


   Int32 sum_len = (line->end.y - line->start.y) + (frag->end.y - frag->start.y);

	PLine fst = frag->start.y < line->start.y ? frag : line; 
	PLine sec = frag->start.y < line->start.y ? line : frag;

	int min_len = min( line_length(fst), line_length(sec) );
	// test delta
	Int32 delta =	abs(fst->end.x - sec->start.x);
	int delta_limit = (min_len < 50) ? 5 : 10;
	if (delta > delta_limit)
		return FALSE;

	// test gap
	Int32 gap   =	abs(fst->end.y - sec->start.y);
	int gap_limit = (min_len < 50) ? 15 : 30;

   if (gap > gap_limit)
		return FALSE;
   if ( gap * LINK_FRAGMENTS_RATIO > sum_len )
		return FALSE;
	
	// make connection
	if (frag->start.y < line->start.y)
		line->start = frag->start;
	else
		line->end = frag->end;
	
	return (TRUE);
};

/////////////////////////////////////
#define CORNER_DELTA    3     //  <= | ver...start.x - hor...start.x | ; y also
#define HV_LENGTH_DELTA 6     //  <= | checkbox height-width |
#define VV_LENGTH_DELTA 3     //  <= | left_len - right_len  |

#define MAX_CHBOX_SIZE  80   
#define MIN_CHBOX_SIZE  20    

static TLineFragment* _curr = NULL;
int cdecl byStartY( const void *int1, const void *int2)
{
   return   _curr[*(int*)int1].fragmentAsIs.start.y -
            _curr[*(int*)int2].fragmentAsIs.start.y;
};

/*
#define MAX_LETTER_HEIGHT 70
int cdecl byStrokes( const void *int1, const void *int2)
{
   Line & ln1 = _curr[*(int*)int1].fragmentAsIs;
   Line & ln2 = _curr[*(int*)int2].fragmentAsIs;

   int islong1 = line_length(&ln1) > MAX_LETTER_HEIGHT;
   int islong2 = line_length(&ln2) > MAX_LETTER_HEIGHT;

   if ( islong1 || islong2 )
      return islong2 - islong1; // drop long lines to the end of list

   // get centers
   Point32 c1 = { ln1.start.x + ln1.end.x, ln1.start.y + ln1.end.y};
   Point32 c2 = { ln2.start.x + ln2.end.x, ln2.start.y + ln2.end.y};

   return (c1.x - c2.x) + 5 * (c1.y - c2.y);
};
*/
static
void _RegisterCheckBox( Line& left,
                        Line& top, 
                        Line& right, 
                        Line& bottom
                      )
{
   Rect16 rcb;
   rcb.left    = (short)(( left.start.x   +   left.end.x   ) >> 1);
   rcb.right   = (short)(( right.start.x  +   right.end.x  ) >> 1);
   rcb.top     = (short)(( top.start.y    +   top.end.y    ) >> 1);
   rcb.bottom  = (short)(( bottom.start.y +   bottom.end.y ) >> 1);

   int width  = rcb.right  - rcb.left;
   int height = rcb.bottom - rcb.top;

   left.flags |= LF_DONTLINK;
   right.flags |= LF_DONTLINK;
   top.flags |= LF_DONTLINK;
   bottom.flags |= LF_DONTLINK;

   RegisterCheckBox(rcb);
}

inline Bool OutRect( Point32& pt, Rect32& rc )
{
   return (pt.x < rc.left) || (pt.x > rc.right) || (pt.y < rc.top) || (pt.y > rc.bottom); 
}

inline Bool OutRange( int x, int minim, int maxim )
{
   return (x < minim) || (x > maxim); 
}

inline void Point2Rect( Rect32& rc, Point32& pt, int delta )
{
   rc.left  = pt.x - delta;
   rc.right = pt.x + delta;
   rc.top   = pt.y - delta;
   rc.bottom= pt.y + delta;
}

Bool AnalyzeFragments( TLinesBambuk& hLB, TLinesBambuk& vLB)
{
#if 1
   /////////////////////////////
   // prepeare data
   int nv = vLB.fragmentsCount;
   int nh = hLB.fragmentsCount;

	if (nv==0 || nh==0)
		return TRUE;

   if (nv > MAX_V_RASTERS_COUNT)
      return WRONG();
   if (nh > MAX_H_RASTERS_COUNT)
      return WRONG();

   int vv[ MAX_V_RASTERS_COUNT ] = {0};
   int hh[ MAX_H_RASTERS_COUNT ] = {0};
   
   for (int i=0; i < nv; i++ ) vv[i]=i;
   for (int i=0; i < nh; i++ ) hh[i]=i;

   TLineFragment* _lf_hor = &(hLB[0]);
   TLineFragment* _lf_ver = &(vLB[0]);

   _curr = _lf_ver; qsort(&vv, nv, sizeof(vv[0]), byStartY);
   _curr = _lf_hor; qsort(&hh, nh, sizeof(hh[0]), byStartY);

   /////////////////////////////
   // start checkbox search

   int prev_ver = 0; 
   for ( int top_hor=0; top_hor < nh; top_hor++ ) 
   {  // by all horisontal fragments, 
      // in order up to down (start.y increases)
      Line& top = _lf_hor[hh[top_hor]].fragmentAsIs; // potential top of checkbox
      int top_len = top.end.x - top.start.x;
      if (OutRange( top_len, MIN_CHBOX_SIZE, MAX_CHBOX_SIZE ))
         continue;

      Rect32 corner0; Point2Rect(corner0, top.start, CORNER_DELTA);
      Rect32 corner1; Point2Rect(corner1, top.end,   CORNER_DELTA);
      int zone_start  = min(corner0.top,    corner1.top);
      int zone_finish = max(corner0.bottom, corner1.bottom);

      //// skip to start of test zone
      while (  prev_ver < nv && _lf_ver[vv[prev_ver]].fragmentAsIs.start.y < zone_start )
         prev_ver++;

      if (!(prev_ver < nv))
         break;   // finished

      // we at start of test zone (prev_ver - is first vertical segment)
     
      for ( int left_ver = prev_ver; left_ver < nv; left_ver++ )
      {
         Line& left = _lf_ver[vv[left_ver]].fragmentAsIs;
         if (left.start.y > zone_finish)
            break; // out of zone
         
         if ( OutRect(left.start, corner0 ) )
            continue; // no connection
         
         int left_len = left.end.y - left.start.y;
         if ( abs(left_len - top_len) > HV_LENGTH_DELTA )
            continue;   // diff length!
         /////////////////////////////////////////////////////////////////////
         // now we have left-top corner; try to make bottom and right parts
         
         for ( int right_ver = prev_ver; right_ver < nv; right_ver++ )
         {  
            Line& right = _lf_ver[vv[right_ver]].fragmentAsIs;
            if (right.start.y > zone_finish)
               break; // out of zone

            if ( OutRect( right.start, corner1 ) )
               continue; // no connection
         
            int right_len = right.end.y - right.start.y;
            if ( abs(right_len - left_len) > VV_LENGTH_DELTA )
               continue;   // diff length!
            /////////////////////////////////////////////////////////////////////
            // now we have 'П'- connection; test for bottom
            Rect32 corner2; Point2Rect(corner2, left.end,  CORNER_DELTA);
            Rect32 corner3; Point2Rect(corner3, right.end, CORNER_DELTA);
            int bottom_zone_finish = max(left.end.y, right.end.y) + CORNER_DELTA;
            for (int bottom_hor = top_hor; bottom_hor < nh; bottom_hor++ )
            {
               Line& bottom = _lf_hor[hh[bottom_hor]].fragmentAsIs;
               if (bottom.start.y > bottom_zone_finish)
                  break; // out of zone

               int bottom_len = bottom.end.x - bottom.start.x;
               if ( abs(bottom_len - top_len) > VV_LENGTH_DELTA )
                  continue;   // diff length!

               if ( OutRect( bottom.start, corner2 ) )
                  continue; // no connection

               if ( OutRect( bottom.end, corner3 ) )
                  continue; // no connection
      
               /////////////////////////////////////////////////////////////////////
               // now we have connected rect - register checkbox
               _RegisterCheckBox(left, top, right, bottom);
            }
            /////////////////////////////////////////////////////////////////////
         }
         /////////////////////////////////////////////////////////////////////
      }
   }
   

#endif   //0
   return TRUE;
}

///////////////////////////////////////////
// search vertical lines for letter sticks
/*#include <windows.h> // OutputDebugString*/
#include "lns.h"
static LnsFrag* vfrags = NULL;
static LnsFrag* hfrags = NULL;

inline int xlevel( LnsFrag& lf1) { return (lf1.A.x + lf1.B.x) >> 1; };
inline int ylevel( LnsFrag& lf1) { return (lf1.A.y + lf1.B.y) >> 1; };

inline int yheight( LnsFrag& lf1) { return (lf1.B.y - lf1.A.y); };
inline int xheight( LnsFrag& lf1) { return (lf1.B.x - lf1.A.x); };

inline int yproj_pt( Point16& pt, int phi ) // computes projection of point by angle phi to Y-axis
{
   return pt.y - ((phi * pt.x) >> 10);
}

inline int yproj_frag( LnsFrag& hor_lf, int* phi ) // computes angle of fragment and it's projection to Y-axis
{
   *phi = ( (hor_lf.B.y - hor_lf.A.y) << 10 ) / (hor_lf.B.x - hor_lf.A.x);
   return yproj_pt( hor_lf.B, *phi );
}

static int yproject( LnsFrag& lf1, LnsFrag& lf2 ) 
{ 
   assert(lf1.B.y > lf1.A.y);
   assert(lf2.B.y > lf2.A.y); // end point has larger y coord

   if (lf1.B.y < lf2.A.y || lf1.A.y > lf2.B.y) // not intersected
      return 0;

   int h1 = yheight(lf1);
   int h2 = yheight(lf2);
   int minh = min(h1, h2);
   assert(minh > 0);

   int a = max(lf1.A.y,lf2.A.y);
   int b = min(lf1.B.y,lf2.B.y);

   assert(b >= a);
   
   return (100*(b-a))/minh;
};

int cdecl comp_vfrags_byXLevel( const void *int1, const void *int2)
{
   LnsFrag & lf1 = vfrags[*(int*)int1];
   LnsFrag & lf2 = vfrags[*(int*)int2];
   return   xlevel(lf1) - xlevel(lf2);
};

int cdecl comp_vfrags_byYLevel( const void *int1, const void *int2)
{
   LnsFrag & lf1 = vfrags[*(int*)int1];
   LnsFrag & lf2 = vfrags[*(int*)int2];
   return   ylevel(lf1) - ylevel(lf2);
};
int cdecl comp_hfrags_byYLevel( const void *int1, const void *int2)
{
   LnsFrag & lf1 = hfrags[*(int*)int1];
   LnsFrag & lf2 = hfrags[*(int*)int2];
   return   ylevel(lf1) - ylevel(lf2);
};


#define CONSOLE(str) OutputDebugString(str)
#include "xtimer.h"
Bool AnalyzeFragmentsII( TLinesBambuk& hLB, TLinesBambuk& vLB)
{
   XTimer t("AnalyzeFragmentsII");
   /////////////////////////////
   // prepeare data
   int nv = LnsGetFragCount(FALSE); // get vertical fragments count
	if (nv==0)
		return TRUE;

   if (nv > MAX_V_RASTERS_COUNT)
      return WRONG();
   int vv[ MAX_V_RASTERS_COUNT ] = {0};

 
   for (int i=0; i < nv; i++ ) vv[i]=i;

   vfrags = LnsGetFragments(FALSE);
   qsort(&vv, nv, sizeof(vv[0]), comp_vfrags_byXLevel);

   /////////////////////
   // start text linkage

   for ( int cur =0; cur < nv; cur++ ) 
   {  // by all vertical fragments
      // in order left to right (start.x + end.x increases)
      LnsFrag& lf1 = vfrags[vv[cur]];

#define MAX_STICK_HEIGHT 60  // == 60 pels
#define MAX_STICK_DELTA  40  // == 40 pels

      if (yheight( lf1 ) > MAX_STICK_HEIGHT)
         continue;

      int lev1 = xlevel(lf1);
      for (int nxt = cur+1; nxt < nv; nxt++)
      {
         LnsFrag& lf2 = vfrags[vv[nxt]];
         int lev2 = xlevel(lf2);
         if (lev2 > lev1 + MAX_STICK_DELTA)
            break;   // out of proximity zone
         if (yheight( lf2 ) > MAX_STICK_HEIGHT)
            continue;
         if (yproject( lf1, lf2 ) > 50)    // more than 50% of shortest stick projected to another
         {
            lf1.flags |= LF_STICK_RIGHT;
            lf2.flags |= LF_STICK_LEFT;
         }
      }

      if (lf1.flags & (LF_STICK_RIGHT | LF_STICK_LEFT) && lf1.frag_handle != -1)
      {
         TLineFragment& lfrg  = vLB[lf1.frag_handle];
         lfrg.fragmentAsIs.flags |= LF_DONTLINK;
      }

   }

   ///////////////////////////////////
   // now vertical sticks are marked - 
   // let test horisontal near sticks
   // to prevent extracting false line
   // from text fragments

   /////////////////////////////////////////////////////////
   // sort both vertical and horisontal fragments up to down

   int nh = LnsGetFragCount(TRUE); // get horisontal fragments count
	if (nh==0)
		return TRUE;

   if (nh > MAX_H_RASTERS_COUNT)
      return WRONG();
   int hh[ MAX_H_RASTERS_COUNT ] = {0};

 
   for (int i=0; i < nh; i++ ) hh[i]=i;

   hfrags = LnsGetFragments(TRUE);

   qsort(&hh, nh, sizeof(hh[0]), comp_hfrags_byYLevel);
   qsort(&vv, nv, sizeof(vv[0]), comp_vfrags_byYLevel); 


   /////////////////////////////////////////////////////////
   // for each horisontal fragment test count of vertical 
   // sticks touching it

   int vcur_start = 0; // current start of zone for vertical segments
   for ( int hcur =0; hcur < nh; hcur++ ) 
   {  // by all horisontal fragments
      // in order up to down (start.y + end.y increases)
      LnsFrag& hor_lf = hfrags[hh[hcur]];

#define MAX_WORD_LENGTH 300 // 300 pels
      if (xheight( hor_lf ) > MAX_WORD_LENGTH) // line too long, don't test
         continue;

      int ylev_hor = ylevel( hor_lf );

      int phi=0;
      int yproj_hor = yproj_frag( hor_lf, &phi );  

      //////////////////////////////
      // update vcur_start position
      for ( ; vcur_start < nv; vcur_start++)
      {
         LnsFrag& vlf = vfrags[vv[vcur_start]];
         int ylev = ylevel( vlf );
         if (ylev > ylev_hor - MAX_STICK_HEIGHT) // reach zone
            break;
      }

      // test inside [ylev_hor - MAX_STICK_HEIGHT, ylev_hor + MAX_STICK_HEIGHT] strip
      // <=> from vcur_start until level not too big
      int xa  = hor_lf.A.x;
      int xb  = hor_lf.B.x;
      xa -= 5; xb += 5;

      int  nstick_len = 0;
      int nsticks_near = 0;      // count of sticks intersected this line
      int nsticks_intersect = 0; // count of sticks intersected this line
      int left    =  10000;
      int right   = -10000; 

      for ( int vcur = vcur_start; vcur < nv; vcur++)
      {
         LnsFrag& vlf = vfrags[vv[vcur]];

         int ylev = ylevel( vlf );
         if (ylev > ylev_hor + MAX_STICK_HEIGHT) // out of zone (by Y coord)
            break;

         int xlev = xlevel( vlf );
         if (xlev < xa || xlev > xb)            // out of range (by X coord)
            continue;

         if (yheight(vlf) > MAX_STICK_HEIGHT)   // not a stick - continue
            continue;         

         // we have vertical segment near  if (ylev > ylev_hor - MAX_STICK_HEIGHT) // reach zone
         int ya = yproj_pt( vlf.A, phi );
         int yb = yproj_pt( vlf.B, phi );
         if (ya > yproj_hor +5 || yb < yproj_hor-5) 
            // stick not intersectes nor touches horisontal fragment
            continue;

         nstick_len += yheight(vlf);
         nsticks_near ++;
         left = min(left, xlev);
         right = max(right, xlev);

         if (ya > yproj_hor || yb < yproj_hor) 
            // stick not intersectes nor touches horisontal fragment
            nsticks_intersect++;         
      };

#define MIN_FREE_LINE_END 30   // 30 pels should be free from vertical sticks at the end of line

      if (nsticks_near > 0)
      {
         if (abs(hor_lf.A.x - left) < MIN_FREE_LINE_END &&     // both ends are not free
             abs(hor_lf.B.x - right) < MIN_FREE_LINE_END &&
             nstick_len > hor_lf.B.x - hor_lf.A.x              // sum length of intersected frags > hor len
            )
         {
            if (hor_lf.frag_handle != -1)
            {
               TLineFragment& lfrg  = hLB[hor_lf.frag_handle];
               lfrg.fragmentAsIs.flags |= LF_DONTLINK;
               lfrg.fragmentAsIs.flags |= LF_TEXT;
            }

            hor_lf.flags |= LF_TEXT;
         }
      }
   }

   return TRUE;
}
