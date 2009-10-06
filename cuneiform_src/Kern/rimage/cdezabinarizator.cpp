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

// CDezaBinarizator.cpp: implementation of the CDezaBinarizator class.
//
//////////////////////////////////////////////////////////////////////

#include "cdezabinarizator.h"
#include "cribinarizator.h"
#include "crimemory.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDezaBinarizator::CDezaBinarizator()
{
	mpBinarizatorControl = NULL;
	bWhitePixel = 0;
	ptbtek = NULL;
	ptbosn = NULL;
	ptb = NULL;
	ptgrey = NULL;

}

CDezaBinarizator::~CDezaBinarizator()
{

}

uint32_t CDezaBinarizator::OpenTrackBin(PCTBINTigerImageInfo Info, CRIBinarizator *pCaller, uint32_t tip1)
{
	int32_t lenpor;
	int32_t i;

	mpBinarizatorControl = pCaller;

	urov[0]    = urov[1] = 0;
	x = y = lg = lgn = indbl = 0;
	ypor = shed = sheds = 0;
	spx = spy = spx1 = spx2 = lg0i = 0;
	sdvig = sy1 = nstr = nstrb = 0;
	indbeg = indend = 0;
	lg0 = 0x00;

	////////////////////////////////////////////
	//	AK add
	xx = 0;
	////////////////////////////////////////////
	for (i=0; i<SB; i++)
		chet_color[i]=0L;

	colall      = 0L;
	tip         = tip1;     /* вЁЇ бЄ ­Ёа®ў ­Ёп 0-б« Ў®, 1-баҐ¤­Ґ, 2-¦Ёа­® */
	bWhitePixel = Info->bFotoMetrics;
	spx         = Info->wImageWidth;
	spy         = Info->wImageHeight;
	spx2        = (spx>>1)+1;
	spx1        = (spx+7)>>3;
	sdvig       = (8-(spx & 0x0007)) & 0x0007;
	lenpor      = LONGBUF/spx;
	nstr        = lenpor;
	lenpor      = lenpor*spx;

	if ((ptgrey=(puchar)DB_MALLOC(lenpor))==NULL)
		return(0);

	lenpor      = LONGBUF / spx1;
	nstrb       = lenpor;
	lenpor      = lenpor * spx1;

	if ((ptbosn=(puchar)DB_MALLOC(lenpor))==NULL)
		return(0);

	if (Read_por_first()==0)
		return(0);

	sy1 = indbl = 0;
	ptbtek = ptbosn;
	return(1);
}

puchar CDezaBinarizator::Black_file(puchar anPblack, int32_t stry)
{
	uint32_t ir = 0;
	puchar regPtb = ptb;
	puchar ptbend;
	puchar pblack = anPblack;
	int32_t shby = 0, shift = 0;

	for ( regPtb = ptgrey, ptbend = regPtb+spx*stry; regPtb != ptbend; regPtb++ )
	{
		ir = *regPtb ? (ir<<1)+1 : ir<<1;

		if ( ++shby==spx )
		{
			shby = 0;
			ir <<= sdvig;
			*pblack++=(uchar)ir;
			shift = 0;
		}
		else
			if ( ++shift == 8 )
			{
				*pblack++=(uchar)ir;
				shift = 0;
			}
	}
	return(pblack);
	/*
	unsigned short int ir, l ,len;
	short int shby, shift;
	unsigned char *ptbend;
	shby=shift=0;
	for (ptb=ptgrey, ptbend=ptb+spx*stry; ptb!=ptbend; ptb++)
	  { ir<<=1; if (*ptb) ir++;
		if (++shby==spx) { shby=0; ir<<=sdvig; shift=7; }
		if (!(++shift&0x8FF8)) continue;
	*pblack++=(unsigned char)ir;
		shift=0;
	  }
	return(pblack);
	*/
}

int32_t CDezaBinarizator::Our1(int32_t spr)
{
    puchar regPtb;
    uchar regUrov;
    int32_t regSpx = spx;

    ypor=spr;
    bufmark[0]=bufmark[1]=0;

	do       //CLB:
	{
		x=bufmark[0];
		y=bufmark[1];
		regPtb = ptb=ptgrey+y*spx+x;    // use register instead of static
		regUrov = (uchar)urov[1];              // see above Stas!!!

		while ( *regPtb < regUrov )
		{
			if (++x < regSpx)
			{
				regPtb++;
				continue;
			}
			else
				if (++y>=ypor)
				{
					ptb = regPtb;       // synchronize
					return(0);
				}
				else
				{
					x=0;
					regPtb++;
				}
		}

		ptb = regPtb;               // synchronize
		lg=urov[0];
		bufmark[0]=x;
		bufmark[1]=y;
		*ptb=0x00;
		indbeg=indend=0;
		st[indend++]=x;
		st[indend++]=y;
		ptb++;
		Right_str();
		st[indend++]=shed;

		while (indbeg!=indend)
		{
			Elem_st();
			indbeg+=3;
		}
	}
	while (TRUE);  //goto CLB;
	/*
	ypor=spr; bufmark[0]=bufmark[1]=0;
	CLB: x=bufmark[0]; y=bufmark[1]; ptb=ptgrey+y*spx+x;
	while ( *ptb<urov[1] )
	{
		if (++x<spx) { ptb++; continue; }
		if (++y>=ypor) return(0);
		x=0; ptb++;
	}
	lg=urov[0]; bufmark[0]=x; bufmark[1]=y;
	*ptb=0x00; indbeg=indend=0;
	st[indend++]=x; st[indend++]=y; ptb++;
	Right_str(); st[indend++]=shed;
	while (indbeg!=indend) { Elem_st(); indbeg+=3; }
	goto CLB;
	*/
}

int32_t CDezaBinarizator::Xygo(int32_t x1, int32_t y1)
{
	if (y1 >= ypor || y1 < 0 || x1 >= spx || x1 < 0)
		return(0);

	x   = x1;
	y   = y1;
	ptb = ptgrey + y * spx + x;
	return(1);
}

int32_t CDezaBinarizator::Grey_black()
{
	int32_t spypic, sy2, spybg;

	spybg = spypic = ((sy1 + nstrb) <= spy) ? nstrb : (spy-sy1);
	sy2 = ((sy1 + nstr) <= spy) ? nstr : (spy - sy1);

	while(spypic!=0)
	{
		if (sy2 > spypic)
			sy2 = spypic;

		if ( DB_GREYREAD(ptgrey, sy1, sy2) != sy2)
			return(0);

		Raspred(sy2);
		sy1  += sy2;
		Get1();
		Our1(sy2);
		ptbtek = Black_file(ptbtek,sy2);
		spypic -= sy2;
	}

	ptbtek = ptbosn;
	indbl = 0;
	return(spybg);
}

int32_t CDezaBinarizator::Elem_st()
{
	int32_t i;

	if (Xygo(st[indbeg], st[indbeg+1]+1)==0)
		return(0);

	bufmark[2]=x;
	bufmark[3]=y;
	Left_str();

	if (sheds)
	{
		Stek();
		st[indend++]=x;
		st[indend++]=y;
		st[indend++]=sheds;
		x=bufmark[2];
		y=bufmark[3];
		ptb=ptgrey+y*spx+x;
	}

	i=0;

	while (i <= st[indbeg+2])
	{
		i++;

		if (*ptb>=lg)
		{
			Stek();
			st[indend++]=x;
			st[indend++]=y;
			*ptb++=0x00;
			Right_str();
			i+=shed;
			st[indend++]=shed;
		}
		else
		{
			if (++x<spx)
				ptb++;
			else
			{
				x--;
				break;
			}
		}
	}
	return(1);
}

int32_t CDezaBinarizator::Read_por_first()
{
	int32_t str, i, j;

	i = spy>>1;
	str = (nstr<i) ? nstr:i;
	str=(str<=spy/6) ? str:spy/6;
	colall=1L*spx*str;
	j=spy/2;

	if (tip==2)
		j=spy-str;

	for (i=0; i<j; i+=str)
    {
		if (DB_GREYREAD(ptgrey, i, str) != str)
			return(0);

		Ras1_pred(str);
	}

	if (DB_GREYREAD(ptgrey,spy - str,str) != str)
		return(0);

	Ras1_pred(str);

	for (lg0i=1; lg0i<SB; lg0i++)
		if (chet_color[lg0i]!=0L)
			break;

	lgn = lg0i+1;
	lg0 = (uchar)lg0i;
	chet_color[lg0i] += chet_color[0];
	chet_color[0] = 0L;
	colall -= chet_color[lg0i];
	return(1);
}

int32_t CDezaBinarizator::Get1()
{
	int32_t i, j, imax, lg1, pg1, lg2, pg2, pgn;
	int32_t irl, srz, chet[256], srz1, srz2;

	for (i=0; i<256; i++)
		chet[i]=chet_color[i];

	srz=colall/100L;
	irl=0L;

	for (pgn=255; irl<srz; pgn--)
		irl+=chet[pgn];

	for (i=lgn,srz=0L; i<=pgn; i++)
		srz+=chet[i];

	if (lgn==pgn)
		pgn++;

	srz=(srz+(pgn-lgn)/2)/(pgn-lgn);

	for (imax=lgn; chet[imax]<srz; imax++) ;

	for ( ; imax<=pgn; imax++)
		if (chet[imax]>chet[imax+1])
			break;

	for (i=imax+1,srz=0L; i<=pgn; i++)
		srz+=chet[i];

	if (pgn==imax)
		pgn++;

	if ((pgn-lgn)<128)  /* § «ҐЇ  ¤«п б« Ўле вҐЄбв®ў */
	{
		srz1=chet[lg0i]*100L/colall;

		if (srz1<500L)
		{
			srz=colall/10L;

			for  (urov[0]=lgn, irl=0L; irl<srz; urov[0]++)
				irl+=chet[urov[0]];

			urov[1]=urov[0]+10;
			return(0);
		}
	}

	for (lg1=imax+1,srz=(srz+(pgn-imax)/2)/(pgn-imax);lg1<=pgn;lg1++)
		if (chet[lg1]<=srz && chet[lg1+1]<=srz)
			break;

	for (i=lg1+1,srz=0L;i<=pgn;i++)
		srz+=chet[i];

	if (pgn==lg1)
		pgn++;

	for (pg2=pgn,srz=(srz+(pgn-lg1)/2)/(pgn-lg1); pg2>lg1; pg2--)
		if (chet[pg2]>=srz && chet[pg2-1]>=srz && chet[pg2-2]>=srz)
			break;

	for (; pg2>lg1; pg2--)
		if (chet[pg2]<=srz && chet[pg2-1]<=srz && chet[pg2-2]<=srz)
			break;

	for (i=lg1+1,srz=0L; i<=pg2; i++)
		srz+=chet[i];

	if (pg2==lg1)
		pg2++;

	for (pg1=lg1,srz=(srz+(pg2-lg1)/2)/(pg2-lg1); pg1<pg2; pg1++)
		if (chet[pg1]<=srz && chet[pg1+1]<=srz && chet[pg1+2]<=srz)
			break;

	for (i=pg1+1,srz=0L; i<=pg2; i++)
		srz+=chet[i];

	if (pg2==pg1)
		pg2++;

	for (lg2=pg2,srz=(srz+(pg2-pg1)/2)/(pg2-pg1); lg2>pg1; lg2--)
		if (chet[lg2]<=srz && chet[lg2-1]<=srz && chet[lg2-2]<=srz)
			break;

	if (lg2==pg1)
	{
		xx=pg1;
		//goto Gt1;
	}
	else
	{
		for (i=pg1, irl=2L*chet[pg1]; i<=lg2; i++)
			if ((srz=labs(chet[i]-2L*chet[pg1]*(lg2-i)/(lg2-pg1)))<=irl)
			{
				irl=srz;
				xx=i;
			}
	}
//Gt1:
	srz=colall/25L;

	if ((pgn-lgn)<128 && xx>128)
		tip=0;

	for (j=xx, irl=0L; irl<srz; j--)
		irl+=chet[j];

	urov[0]=j;
	srz=colall/50L;

	for (j=xx, irl=0L; irl<srz; j++)
		irl+=chet[j];

	urov[1]=j;

	if (tip==0)
	{
		srz1=colall/20L;
		srz2=colall/10L;

		for (xx=lgn, irl=0L; irl<srz1; xx++)
			irl+=chet[xx];

		urov[0]=xx;

		for (j=xx, irl=0L; irl<srz2; j++)
			irl+=chet[j];

		urov[1]=j;
	}
	return(0);
}

void CDezaBinarizator::Right_str()
{
	shed=0;

	if (++x<spx)
		while (*ptb>=lg)
		{
			*ptb++=0x00;
			shed++;

			if (++x>=spx)
				break;
		}

	x--;
	ptb--;
}

void CDezaBinarizator::Left_str()
{
	sheds=0;
	ptb--;

	if (--x>=0)
		while (*ptb>=lg)
		{
			*ptb--=0x00;
			sheds++;

			if (--x<0)
				break;
		}

	x++;
	ptb++;
}

void CDezaBinarizator::Stek()
{
	int32_t r, ir;

	if (indend==BUFS)
	{
		if (indbeg==BUFS)
			indend=0;
		else
		{
			for (r = indbeg, ir = 0; r < (int32_t)indend; r++, ir++)
				st[ir]=st[r];

			indend-=indbeg;
		}

		indbeg=0;
	}
}

void CDezaBinarizator::Raspred(int32_t str)
{
	puchar regPtb;
	uchar regLg0 = lg0;
	uint32_t i, lineCount;
	int32_t l;

	l = chet_color[lg0i];

	for ( lineCount = 0, regPtb=ptgrey; lineCount < (uint32_t)str ; lineCount++ )
	{
		for( i = 0 ; i <= (uint32_t)spx2 ; i++, regPtb++ )
		{
			if ( *regPtb < regLg0 )
				*regPtb=regLg0;

			chet_color[*regPtb]++;
		}

		for( ; i < (uint32_t)spx ; i++, regPtb++ )
		{
			if ( *regPtb < regLg0 )
				*regPtb=regLg0;
		}
	}

	colall = colall + 1L * spx2 * str - (chet_color[lg0i] - l);
	ptb    = regPtb;
	/*
	unsigned short int len, i;
	long l;
	l=chet_color[lg0i]; len=spx*str;
	for (i=0, ptb=ptgrey; i<len; i++, ptb++)
	{ if (*ptb<lg0) *ptb=lg0;
	if ( (i%spx) <=spx2) chet_color[*ptb]++;
	}
	colall=colall+1L*spx2*str-(chet_color[lg0i]-l);
	*/
}

void CDezaBinarizator::Ras1_pred(int32_t str)
{
	puchar regPtb;
    int32_t i;
	int32_t regSpx2 = spx2;
	int32_t lineCount;

    for ( lineCount = str, regPtb=ptgrey; lineCount > 0 ; lineCount-- )
	{
        for( i=0 ; i <= regSpx2 ; i++, regPtb++ )
		{
            chet_color[*regPtb]++;
        }

		i += (spx - spx2 - 1);
		regPtb += (spx - spx2 - 1);
    }

	colall += 1L * spx2 * str;
	/*
	unsigned short int len, i;
	len=spx*str;

	for (i=0, ptb=ptgrey; i<len; i++,ptb++)
		if ( (i%spx) <=spx2) chet_color[*ptb]++;

	colall+=1L*spx2*str;
	*/
}

int32_t CDezaBinarizator::GetBinarized(puchar ptbl, uint32_t lenbl)
{
	int32_t spybr, ret, nstrb1;
	puchar ptbend;

	spybr  = lenbl/spx1;
	nstrb1 = spybr+indbl-nstrb;
	spybr  = (nstrb1<=(spy-sy1)) ? spybr:(spy-sy1+nstrb-indbl);
	ret    = 0;

	do      //GB1:
	{
		if (ptbtek==ptbosn)
			if ((nstrb=Grey_black())==0)
				return(ret);

		nstrb1 = nstrb-indbl;

		if (spybr<=nstrb1)
		{
			if(bWhitePixel)// Piter see flag invert
				for (ptbend=ptbtek+spx1*spybr; ptbtek!=ptbend;)
					*ptbl++=~*ptbtek++;
			else
				for (ptbend=ptbtek+spx1*spybr; ptbtek!=ptbend;)
					*ptbl++=*ptbtek++;

			indbl += spybr;

			if (indbl==nstrb/*1*/)
				ptbtek=ptbosn;// Piter new

			// indbl+=spybr; if (indbl==nstrb1) ptbtek=ptbosn;// Piter Old string
			return(ret+spybr);
		}

		if(bWhitePixel)// Piter see flag invert
			for (ptbend=ptbtek+spx1*nstrb1; ptbtek!=ptbend;)
				*ptbl++=~*ptbtek++;
		else
			for (ptbend=ptbtek+spx1*nstrb1; ptbtek!=ptbend;)
				*ptbl++=*ptbtek++;

		ptbtek = ptbosn;
		spybr  -= nstrb1;
		ret    += nstrb1;
	} while( TRUE );         //goto GB1;
}

int32_t CDezaBinarizator::CloseTrackBin()
{
	DB_FREE(ptgrey);
	DB_FREE(ptbosn);
	ptgrey = NULL;
	ptbosn = NULL;
	return(1);
}
