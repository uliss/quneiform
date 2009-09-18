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

//#define LNS_FUNC( ret_type ) __declspec(dllexport) ret_type
#include "lns.h"
#include "lnscheck.h"
#include "extrlns.h"
#include "sweeper.h"
#include "hliner.h"

static Err16 lnserr = ER_NONE;


/// 10.02.99, VP ------- registering fragments for external usage
#include "frag.h"
/// --------------------

/////////////////////////////////////////////////
static int nSquares = 0;
Rect16 BlackSquares[256]={0};

static void ResetBlackSquares()  // call on new page
{
   nSquares = 0;
}

void RegisterBlackSquare(int left, int top, int right, int bottom )
{
   if (nSquares < sizeof(BlackSquares) / sizeof(BlackSquares[0]) )
   {
      BlackSquares[nSquares].left  = left;
      BlackSquares[nSquares].top   = top;
      BlackSquares[nSquares].right = right;
      BlackSquares[nSquares].bottom= bottom;
      nSquares++;
   };
}

LNS_FUNC( Rect16* ) LnsGetBlackSquares( Int32* count )
{
   *count  = nSquares;
   return BlackSquares;
}

/////////////////////////////////////////////////
static int nCheckBoxes = 0;
Rect16 CheckBoxes[256] = {0};

static void ResetCheckBoxes()  // call on new page
{
   nCheckBoxes = 0;
}

void RegisterCheckBox( Rect16& rcbox )
{
   if (nCheckBoxes < sizeof(CheckBoxes) / sizeof(CheckBoxes[0]) )
      CheckBoxes[nCheckBoxes++] = rcbox;
}

LNS_FUNC( Rect16* ) LnsGetCheckBoxes( Int32* count )
{
   *count  = nCheckBoxes;
   return CheckBoxes;
}

//////////////////////////////////////////////////
void  InitLNS( TImageOpen   f_op,
               TImageRead   f_rd,
               TImageClose  f_cl
					 ); //tgreader.cpp

LNS_FUNC( void )            LnsPageStart( TImageAccess* img )
{
   InitLNS(img->f_op,img->f_re,img->f_cl);
   ResetBlackSquares();
   ResetCheckBoxes();
   HLiner_Init();
}


LNS_FUNC( Bool16 )          LnsExtractLines( Int32   min_h_len,
                                             Int32   min_v_len,
                                             Int32*  result_h_count,
                                             Int32*  result_v_count
                                             )
{
   lnserr = ExtrLinesUnderTigerStatic( min_h_len,   min_v_len,
                                    *result_h_count, *result_v_count );
	return lnserr == 0;
}
LNS_FUNC( Bool16 )			ExtractAllEvents(CLINE_handle hCLINE, LinesTotalInfo  *lti)
{
	return ExtractEvents(hCLINE, lti);
}

LNS_FUNC( Bool16 )          LnsGetCount(  Int32   min_h_len,
                                          Int32   min_v_len,
                                          Int32*  result_h_count,
                                          Int32*  result_v_count
                                       )
{
   return ExtrLinesGetCount(min_h_len, min_v_len,
                  *result_h_count,*result_v_count);
}

///////////////
#include "lns_skew1024.h"
static Bool32  __HasCorners( LineInfo& li, LinesTotalInfo* plti, Bool32 is_hor )
{
   Rect16 rcA = { li.A.x-8, li.A.y-8, li.A.x+8, li.A.y+8 };
   Rect16 rcB = { li.B.x-8, li.B.y-8, li.B.x+8, li.B.y+8 };
   LnsInfoArray & ar = is_hor ? plti->Ver : plti->Hor;
   for (int i=0; i<ar.Cnt; i++)
   {
      LineInfo& linf = ar.Lns[i];
      if (
         Inside(rcA, linf.A) || Inside(rcB, linf.A) ||
         Inside(rcA, linf.B) || Inside(rcB, linf.B)
         )
         return TRUE;
   };
   return FALSE;
}
/////////////////////////////////////////////////////////
void  __RejectNearBound(LinesTotalInfo* plti)
{
   Rect32 imgrect; Set( imgrect, 0,0,plti->ImgSize.x-1,plti->ImgSize.y-1 );

   int hcnt=0, vcnt=0;  ltiGetNotNoise( plti, hcnt, vcnt );

    int i(0);
   for (i=0; i< plti->Hor.Cnt; i++ )
   {  LineInfo & li = plti->Hor.Lns[i];
      if (hcnt < 5)
         break;   // keep lines, if lack
      if (li.Flags & LI_NOISE)
         continue;
      Point32 Mn; Mn.x = MIN(li.A.x,li.B.x);   Mn.y = min(li.A.y,li.B.y);
      Point32 Mx; Mx.x = MAX(li.A.x,li.B.x);   Mx.y = max(li.A.y,li.B.y);
      if ((Mn.y < imgrect.top + 50) || (Mx.y > imgrect.bottom - 50))
      {
         if (!__HasCorners(li, plti, TRUE))
         {  li.Flags |= LI_NOISE;  hcnt--;continue; };
      };

      if (  ( ((Mn.y < imgrect.top + 100) || (Mx.y > imgrect.bottom - 100)) )
            &&
            ( (Mn.x < imgrect.left + 50) || ((Mx.x > imgrect.right - 50)) )
         )
      {
         if (!__HasCorners(li, plti, TRUE))
         { li.Flags |= LI_NOISE;  hcnt--;continue; };
      }
   };
   for (i=0; i< plti->Ver.Cnt; i++ )
   {  LineInfo & li = plti->Ver.Lns[i];
      if (li.Flags & LI_NOISE)
         continue;
      if (vcnt < 5)
         break;   // keep lines, if lack
      Point32 Mn; Mn.x = MIN(li.A.x,li.B.x);   Mn.y = min(li.A.y,li.B.y);
      Point32 Mx; Mx.x = MAX(li.A.x,li.B.x);   Mx.y = max(li.A.y,li.B.y);
      if ((Mn.x < imgrect.left + 50) || (Mx.x > imgrect.right - 50))
      {
         if (!__HasCorners(li, plti, FALSE))
         {  li.Flags |= LI_NOISE; vcnt--;continue; };
      }
      if (  ( ((Mn.x < imgrect.left + 100) || (Mx.x > imgrect.right - 100)) )
            &&
            ( (Mn.y < imgrect.top + 50) || ((Mx.y > imgrect.bottom - 50)) )
         )
      {
         if (!__HasCorners(li, plti, FALSE))
         {  li.Flags |= LI_NOISE;  vcnt--;continue; };
      }

      int mcx = (li.A.x + li.B.x) >> 1;
      if (  ( (mcx < imgrect.left + 200) || (mcx > imgrect.right - 200) )
         && (li.B.y-li.A.y > 200)   // not too short
         )
      {  // test lines near bound for collinear neibours
         Bool left = (mcx < imgrect.left + 200);
         int  len = li.B.y-li.A.y;
         int  ay = li.A.y + 50;
         int  by = li.B.y - 50;  // shortened for normal intersection area ( > 50)
         for ( int j=i+1; j< plti->Ver.Cnt; j++ )
         {
            LineInfo & li2 = plti->Ver.Lns[j];
            int len2 = li2.B.y-li2.A.y;
            if (len2 < 200)
               continue;   // ignore short
            int mcx2 = (li2.A.x + li2.B.x) >> 1;
            int xdelta = abs(mcx - mcx2);
            if (xdelta < 16 && Overlap(ay, by, li2.A.y,li2.B.y))
            {
               if (len < len2*2)
               {  // kill first, may be keep second
                  if (!(li.Flags & LI_NOISE))
                  {
                     li.Flags|= LI_NOISE;  vcnt--;
                  };
               }
               if (len2 < len*2)
               {  // kill second, may be keep first
                  if (!(li2.Flags & LI_NOISE))
                  {
                     li2.Flags|= LI_NOISE;  vcnt--;
                  };
               };
            };
         };
      };
   };
}
//////////////
LNS_FUNC( Bool16 )          LnsUpload( LinesTotalInfo* lti,
                                       Int32   min_h_len,
                                       Int32   min_v_len
                                       )
{
   Int32 h,v;
   if (ExtrLinesGetInfo(lti, min_h_len, min_v_len,h,v))
   {
      /*
      Bool res =  LC_Init(lti) &&
                  //LC_CorrectSkew(lti) &&
                  LC_MarkBadLines(lti);
      //assert(res);
      LC_Done(lti);
      if (!res)
         lnserr = ER_NOMEMORY;
      return res;
      */

      __RejectNearBound(lti);

      return TRUE;
   }
   lnserr = ER_CURRENTLYDISABLED;
   return FALSE;
}

/////////// sweeping /////////////////
static TImageAccess swpimg;

extern "C" {
   LNS_FUNC(Bool16)  LnsImageOpen(Tiger_ImageInfo* lpImageInfo);
   LNS_FUNC(Int16)   LnsImageRead(Word8* lpImage, Word16 wMaxSize);
   LNS_FUNC(Bool16)  LnsImageClose(void);
}

LNS_FUNC(Bool16)  LnsImageOpen(Tiger_ImageInfo* lpImageInfo)
{  return Sweeper_ImageOpen(lpImageInfo); }

LNS_FUNC(Int16)   LnsImageRead(Word8* lpImage, Word16 wMaxSize)
{  return Sweeper_ImageRead(lpImage, wMaxSize); }

LNS_FUNC(Bool16)  LnsImageClose(void)
{  return Sweeper_ImageClose(); }

LNS_FUNC( TImageAccess* )   LnsGetSweepedImage( LinesTotalInfo* lti )
{
   ExtrLinesPrepearToSweep(lti);
   memset( &swpimg, 0, sizeof(swpimg));
   swpimg.f_op = LnsImageOpen;
   swpimg.f_re = LnsImageRead;
   swpimg.f_cl = LnsImageClose;
   return &swpimg;
}
/////////////////////////////////////////////////
LNS_FUNC( Err16 )           LnsGetError()
   {  return lnserr; }

LNS_FUNC( void )            LnsPageFinish()
{
   HLiner_Done();
   ExtrLinesDoneLNS();
   Frag_VFree();
   Frag_HFree();
}

LnsSetupStr lnsSetup = {0};

LNS_FUNC( Bool16 )          LnsSetup( LnsSetupStr* ls ) // can call before LnsExtractLines
{
   if (ls)
      lnsSetup = *ls;

   return TRUE;
}
/////////////////////////////////////////

/// 10.02.99, VP ------- registering fragments for external usage
LNS_FUNC( int ) LnsGetFragCount( Bool horisontal )
{ return horisontal ? Frag_HCount() : Frag_VCount();}

LNS_FUNC( LnsFrag* ) LnsGetFragments( Bool horisontal )
{ return horisontal ? Frag_HGet(0) : Frag_VGet(0);}
/// --------------------
