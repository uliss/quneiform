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

// CRLEd.cpp: implementation of the CRLEd class.
//
//////////////////////////////////////////////////////////////////////
#include <string.h>
#include "crled.h"
#include "crlmemory.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRLEd::CRLEd()
{
	mpEdBuffer = (puchar)RLINGLock(mhEdBuffer = RLINGAlloc(CRL_ED_BUFFER_SIZE));
	mpEdOutBuffer = (puchar)RLINGLock(mhEdOutBuffer = RLINGAlloc(CRL_ED_BUFFER_SIZE));
	memcpy(mHalfSpaces,"\x1e\x1f",3);
	memset(&mSdd, 0, sizeof(mSdd));
	memset(&mFdd, 0, sizeof(mFdd));
	memset(&mFd, 0, sizeof(mFd));
	mbReadyToAdd = FALSE;
}

CRLEd::~CRLEd()
{
	RLINGFree(mhEdBuffer);
	RLINGFree(mhEdOutBuffer);
}

void CRLEd::Init()
{
	mpEdFileEnd = mpEdBuffer;
	mpEdFileBound = &mpEdBuffer[CRL_ED_BUFFER_SIZE];
}

Bool32 CRLEd::MakeWord(CSTR_rast b, CSTR_rast e, puchar pLanguage)
{
	uchar           *l,p;
	CSTR_rast       c;
	UniVersions     uni;
	int16_t           k,i,n,h,wb, nlig, nl;
	CSTR_rast_attr  a;
	int16_t           top,bottom,left,right;

	Init();

	for(top=10000,bottom=0,left=10000,right=0,c=b;c&&c!=e;c=CSTR_GetNext(c))
    {
		CSTR_GetAttr(c,&a);
		if( a.col<left )
			left = a.col;
		if( a.col+a.w>right )
			right = a.col+a.w;
		if( a.row<top )
			top = a.row;
		if( a.row+a.h>bottom )
			bottom = a.row+a.h;
		if( c==b )
        {
			*pLanguage=a.language;
        }
    }
	h = bottom-top-1;
	wb= (right-left-1+7)/8;
	//    start sheet_disk_descr
    mSdd.code     =0x0a;
    mSdd.descr_lth=0x26;
    mSdd.resolution=300;
    Write((uchar*)&mSdd,sizeof(mSdd));
	// start fragm_disk_descr
    mFdd.code=0x0b;
    mFdd.language = *pLanguage;
    mFdd.height   = h;
    mFdd.w_width  = wb;
    mFdd.kegl     = 10;
    Write((uchar*)&mFdd,sizeof(mFdd));

	// start fragm_disk
    mFd.code=0x0b;
    Write((uchar*)&mFd,sizeof(mFd));

	for(c=b;c&&c!=e;c=CSTR_GetNext(c))
    {
		CSTR_GetAttr(c,&a);
		// start bitmap ref
		mBmr.code    =0;
		mBmr.pos     =a.cpos;
		mBmr.row     =a.r_row;
		mBmr.col     =a.r_col;
		mBmr.width   =a.w;
		mBmr.height  =a.h;
		CSTR_GetCollectionUni(c,&uni);
		n=(int16_t)uni.lnAltCnt;
		if(n>7) n=7;
		if(n<1) n=1;

		if( n && memchr(mHalfSpaces,uni.Alt[0].Liga,2) )
        {
			Write((uchar*)&mBmr,sizeof(mBmr));
			p=uni.Alt[0].Prob;

			if( p&1 )
				p--;

			p++;
			mVr.code = uni.Alt[0].Liga;
			mVr.prob = p;
			Write((uchar*)&mVr,sizeof(mVr));
			continue;  // half spaces for spelling analisys
        }
		nlig = strlen((char*)uni.Alt[0].Code);

		if( uni.lnAltCnt>1 && nlig>1 )
			return FALSE;

		for(nl=0;nl<nlig;nl++)
		{
			Write((uchar*)&mBmr,sizeof(mBmr));

			for(k=i=0;k<n;)
			{
				p=uni.Alt[i].Prob;
				if( p&1 )   p--;
				if( !p  )   p=2;    // all odd propability
				for(l = &uni.Alt[i++].Code[nl];*l && k<n;l++,k++)
				{ // for ligas : strlen(uni.Alt[i].Code)>1
					if( k==n-1 )
						p = p+1; // last even propability
					mVr.code = *l;
					mVr.prob =  p;
					Write((uchar*)&mVr,sizeof(mVr));
				}
			}
		}
	}

	mbReadyToAdd = TRUE;
	return TRUE;
}

Bool32 CRLEd::AddWord(CSTR_rast b, CSTR_rast e, puchar pLanguage)
{
	uchar           *l,p;
	CSTR_rast       c;
	UniVersions     uni;
	int16_t           k,i,n, nlig, nl;
	CSTR_rast_attr  a;

	for(c=b;c && c!=e;c=CSTR_GetNext(c))
    {
		CSTR_GetAttr(c,&a);
		if( c==b )
        {
			*pLanguage=a.language;
        }
    }

	for(c=b;c&&c!=e;c=CSTR_GetNext(c))
    {
		CSTR_GetAttr(c,&a);
		// start bitmap ref
		mBmr.code    =0;
		mBmr.pos     =a.cpos;
		mBmr.row     =a.r_row;
		mBmr.col     =a.r_col;
		mBmr.width   =a.w;
		mBmr.height  =a.h;
		CSTR_GetCollectionUni(c,&uni);
		n=(int16_t)uni.lnAltCnt;
		if(n>7) n=7;
		if(n<1) n=1;

		if( n && memchr(mHalfSpaces,uni.Alt[0].Liga,2) )
        {
			Write((uchar*)&mBmr,sizeof(mBmr));
			p=uni.Alt[0].Prob;

			if( p&1 )
				p--;

			p++;
			mVr.code = uni.Alt[0].Liga;
			mVr.prob = p;
			Write((uchar*)&mVr,sizeof(mVr));
			continue;  // half spaces for spelling analisys
        }
		nlig = strlen((char*)uni.Alt[0].Code);

		if( uni.lnAltCnt>1 && nlig>1 )
			return FALSE;

		for(nl=0;nl<nlig;nl++)
		{
			Write((uchar*)&mBmr,sizeof(mBmr));

			for(k=i=0;k<n;)
			{
				p=uni.Alt[i].Prob;
				if( p&1 )   p--;
				if( !p  )   p=2;    // all odd propability

				for(l = &uni.Alt[i++].Code[nl];*l && k<n;l++,k++)
				{ // for ligas : strlen(uni.Alt[i].Code)>1
					if( k==n-1 )
						p = p+1; // last even propability

					mVr.code = *l;
					mVr.prob =  p;
					Write((uchar*)&mVr,sizeof(mVr));
				}
			}
		}
	}

	return TRUE;
}

void CRLEd::Write(puchar pP, uint16_t wSize)
{
	if (mpEdFileBound -  mpEdFileEnd < wSize)
    {
		return;
    }

	memcpy (mpEdFileEnd, pP, wSize);
	mpEdFileEnd += wSize;
}

Bool32 CRLEd::ExcludeToVers(int32_t size, char* pStr)
{
	uchar  *p,*pe;
	//struct  vers_ref     vf;
	//struct  bit_map_ref  bm;
	struct  VersRef     vf;
	struct  BitMapRef   bm;
	int32_t   i;
	Bool32	new_bmp;

	//i=-1;
	i=miEdNVers=0;
	mEdVers[0].lnAltCnt=0;
	new_bmp=FALSE;
	p  = mpEdOutBuffer + sizeof(mSdd)+sizeof(mFdd)+sizeof(mFd);
	pe = &mpEdOutBuffer[size];

	for(;p<pe;)
    {
		switch( *p )
        {
        case    0:
            memcpy(&bm,p,sizeof(bm));
            p+=sizeof(bm);

			if( !new_bmp && !(!bm.row && !bm.col &&
				!bm.height && !bm.width) )
			{
                //i++;
                //memset(&ed_vers[i],0,sizeof(RecVersions));
                mEdRect[i].top    = bm.row;
                mEdRect[i].left   = bm.col;
                mEdRect[i].bottom = bm.height;
                mEdRect[i].right  = bm.width;
				new_bmp=TRUE;
			}
            break;
        default:
            memcpy(&vf,p,sizeof(vf));
            p+=sizeof(vf);
            //if( vf.code<30  )
			//  return FALSE;
            mEdVers[i].Alt[mEdVers[i].lnAltCnt].Code = vf.code;
            mEdVers[i].Alt[mEdVers[i].lnAltCnt].Prob =
                (vf.prob>1?vf.prob:254);
            if( !mEdVers[i].lnAltCnt )
                *pStr++ = vf.code;
            mEdVers[i].lnAltCnt++;
			new_bmp=FALSE;
            if( vf.prob&1 )
			{
                i++;
                mEdVers[i].lnAltCnt=0;
			}
            break;
        }


    }
	miEdNVers = i;
	for(i=0;i<miEdNVers;i++)
    {
		if( mEdVers[i].lnAltCnt && mEdVers[i].Alt[0].Code=='~' )
        {
			mEdVers[i].lnAltCnt =  0;
        }
    }
	*pStr=0;
	return TRUE;
}

Rect16 CRLEd::GetRectElement(uint32_t i)
{
	Rect16 Out={0,0,0,0};

	if ( mbReadyToAdd )
		if ( /*i > -1 &&*/ i < CRL_ED_RECT_ARRAY_SIZE )
			return mEdRect[i];

	return Out;
}

RecVersions CRLEd::GetVersElement(uint32_t i, uint32_t * wNVers)
{
	RecVersions Out ={0,0,0};

	if ( mbReadyToAdd )
		if ( /*i > -1 &&*/ i < CRL_ED_VERS_ARRAY_SIZE )
		{
			*wNVers = miEdNVers;
			return mEdVers[i];
		}

	*wNVers = 0;
	return Out;
}
