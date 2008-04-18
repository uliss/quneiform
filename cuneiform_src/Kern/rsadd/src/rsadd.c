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

//////////////////////////////////////////////////////////////////
// RSADD : OVERLAYING RUSSIAN AND ENGLISH STRINGS               //
//////////////////////////////////////////////////////////////////
//         MAIN FUNCTION : Rsadd_take                           //
//////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "cstr.h"
#include "rsadd.h"
#include "resource.h"
#include "lang_def.h"
#include "leodefs.h"
#include <stdlib.h>
#define etap_name 'g'
#define BYTE    Word8
#include "ligas.h"
#include "minmax.h"

static Int16   crsadd_rus1=10;
static Int16   crsadd_rus2=10;
static Int16   crsadd_eng1=10;
static Int16   crsadd_eng2=10;
static Int16   crsadd_eng3 = 20;
static Int16   crsadd_re1 =15;
static Int16   crsadd_re2 =20;
static Int16   crsadd_dup1=220;
static Int16   crsadd_dup2=10;
static Int16   crsadd_dup3=180;
static Int16   crsadd_dup4r=150;
static Int16   crsadd_dup5r=170;
static Int16   crsadd_dup4e=150;
static Int16   crsadd_equa1=100;
static Int16   crsadd_equa2=100;    
#define MAX_LEN_WORD  68

Word16              wHeightRC      = 0;
Word16              wLowRC         = RSADD_ERR_NO;

typedef struct _RSADD_STAT
{
Word8   minp, maxp, minup, maxup;
Int16   norec, nall, nh, nm, ns;
Bool32  var_width, var_kegl, var_dig, case_up, case_dn, case_dig;
} RSADD_STAT;

static Word8 buf_for_output1[256];
static Word8 buf_for_output2[256];
static Word8 buf_for_output3[256];
static Word8 left_limit_word[] =" ./\x1e\x1f";
static Word8 right_limit_word[]=" -.,РЭЮ\'\":/";
static Word8 rus_similar[]="џ18$";
static Word8 eng_similar[]="SISS";
static Word8 rus_alias  []="ЈЇвЎ‚8";
static Word8 eng_alias  []="rnr68B";
static Word8 rus_equal  []="в®аЄебў¬…’“ЋђЂЌЉ•‘‚ЊҐгЁ®а еб“Ћђ•‘­Јэх123456789031"; 
static Word8 eng_equal  []="TOPKXCBMETYOPAHKXCBMeyuopaxcyopxcHIam1234567890‡є"; 
static Word8 rus_uni_letters[]="©‰й™д”л›¤„¦†нќпџоћЃ\xf0";
static Word8 eng_uni_letters[]="iwWRFGLZz\xb1\xb2\xb3\xb4\xb5\xb6\xb9"; //+N
static Word8 non_letters[]="«»()";
static Word8 true_terms[]="/-\x5F";

Bool16	my_snap_monitor_ori(CSTR_line *snap_line, Int32 num_lines){return FALSE;};
Bool16	my_snap_activity(Word8 a){return FALSE;};
Bool16	my_snap_show_text(Word8 *txt){return FALSE;};

Bool16	(*snap_monitor_ori)(CSTR_line *snap_line, Int32 num_lines)=my_snap_monitor_ori;
Bool16	(*snap_activity)(Word8 a)=my_snap_activity;
Bool16	(*snap_show_text)(Word8 *txt)=my_snap_show_text;
Word8	db_status=0;
Word8   spec_camera=0;

RSADD_FUNC(void) RSADD_SetRSTR( Word8 status,Word8 camera, 
                                Handle monitor_ori,
                                Handle activity,
                                Handle show_text)
{
db_status       = status;
spec_camera     = camera;
snap_monitor_ori= monitor_ori;
snap_activity   = activity;
snap_show_text  = show_text;
return;
}

extern  Int16   rec_versions_triad( char *w,Word8 lang);
static  Bool32  tables=FALSE;

static  void rsadd_clear_hide(CSTR_line lrus);
static  Bool32 rsadd_eng_L(CSTR_rast eng,CSTR_rast enge,CSTR_rast rus);
static  Bool32 rsadd_hide_segment(CSTR_rast rus,Int32 b,Int32 e,Int32 lim) ;

static CSTR_rast rsadd_next_word(CSTR_rast cs)
{
CSTR_rast       c=cs;
CSTR_rast_attr  attr;
RecVersions     vers;

CSTR_GetAttr(c,&attr);
if( attr.flg==CSTR_f_fict && attr.col==0x7fff ) 
    return (CSTR_rast)0; // last rast
do{
  c=CSTR_GetNext(c);
  if( !c )
    break;
  CSTR_GetAttr(c,&attr);
  CSTR_GetCollection(c,&vers);
  }while( c && memchr(left_limit_word,vers.Alt[0].Code,sizeof(left_limit_word)) &&
          !(attr.flg&CSTR_f_fict) );

return (attr.flg&CSTR_f_fict)?(CSTR_rast)0:c;
}

static CSTR_rast rsadd_end_word(CSTR_rast cs,Word8 *str,Word8 *word_len,Int16 left_limit,Int16 right_limit, Int32 need_space,
								Bool32 rightTrue) //Nick 11.05.2001
{
CSTR_rast       c=cs, n;
CSTR_rast_attr  attr, nattr;
Int16           i, dif;
RecVersions     vers;
Bool32          solid=FALSE,prev_over=FALSE;


if( rightTrue ) 
// поиск в первую очередь по правой границе
// Nick 11.05.2001
{
	Bool32 wasEnd = FALSE;
    CSTR_rast best=NULL;
	Int32 bestDist=32000;
	Int32 dist;
	
	for(c=cs;c;c=CSTR_GetNext(c) )
	{
		CSTR_GetAttr(c,&attr);

		if( attr.flg & CSTR_f_fict)
			break;
		if( attr.flg & CSTR_fn_hide) // ?
			continue;
		if( attr.flg & CSTR_f_space)
		{
			wasEnd = TRUE;
			continue;
		}


		if( attr.col > right_limit )
			break;

		dist = abs(right_limit - attr.col-attr.w);

		CSTR_GetCollection(c,&vers);

		// слово желательно без знаков!
		if( memchr(right_limit_word,vers.Alt[0].Code,sizeof(right_limit_word)) )
			dist ++;

		if( !best || dist < bestDist )
		{
			best = c;
			bestDist = dist;
		}

    } 

	if( best && bestDist < 5 )
	{
		best = CSTR_GetNext(best);
		
		// for snap
		 for(c=cs,i=0;c != best; c= CSTR_GetNext(c),i++ )
		 {
			CSTR_GetCollection(c,&vers);
            *str++  = vers.Alt[0].Code; 
		 }
        
		 c= best ? best : CSTR_GetLastRaster(CSTR_GetRasterLine(cs));

         *str='\0';
         *word_len = (Word8)i;
         return c;
	}

	// not found - to old version without bound
	c = cs;
}


dif=1;
CSTR_GetCollection(c,&vers);
*str++  = vers.Alt[0].Code;
i=1;
if( need_space==2 )
    {
    do{
        c=CSTR_GetNext(c);
        CSTR_GetCollection(c,&vers);
        *str++  = vers.Alt[0].Code;  
        CSTR_GetAttr(c,&attr);
        i++;
        }while(  c && 
                (!strchr(" ",vers.Alt[0].Code)||(attr.flg&CSTR_fn_hide)) &&
                !(attr.flg&CSTR_f_fict) && 
                 i<MAX_LEN_WORD-1);
    }
else
    {
    do{
      c=CSTR_GetNext(c);
      CSTR_GetCollection(c,&vers);
      *str++  = vers.Alt[0].Code;  
      CSTR_GetAttr(c,&attr);
      i++;
      if( attr.flg&CSTR_f_solid )
        solid=TRUE;
      if( (solid||prev_over) && vers.Alt[0].Code==0x1F )
        break;
      prev_over=( abs(attr.col+attr.w-right_limit)<dif  && 
          abs(attr.col-left_limit)<dif )  ;
      }while( c && !memchr(right_limit_word,vers.Alt[0].Code,sizeof(right_limit_word)) &&
              attr.col+attr.w<=right_limit+dif &&
              !(attr.flg&CSTR_f_fict) && i<MAX_LEN_WORD-1);
    }
if( c && attr.w && !prev_over &&
        !memchr(right_limit_word,vers.Alt[0].Code,sizeof(right_limit_word)) &&
          attr.col+attr.w-right_limit<attr.w/2+dif &&
          !(attr.flg&CSTR_f_fict) && i<MAX_LEN_WORD-1)
    {
    c=CSTR_GetNext(c);
    CSTR_GetCollection(c,&vers);
    *str++  = vers.Alt[0].Code;  
    CSTR_GetAttr(c,&attr);
    i++;
    }
*str='\0';
if( need_space==1 && c && !(attr.flg&CSTR_f_fict) &&
    vers.Alt[0].Code != liga_TM_usual && // Nick 11.05.2001
    vers.Alt[0].Code != liga_CC_usual &&
    vers.Alt[0].Code != liga_CR_usual 
   )
    {
    n = CSTR_GetNext(c);
    CSTR_GetAttr(n,&nattr);        
    if( /*attr.w &&*/ (nattr.flg&(CSTR_f_let|CSTR_f_bad|CSTR_f_punct)) )
        {
        Int16   dist=16000;
        if( attr.h )    dist = MIN( dist, attr.h);
        if(nattr.h )    dist = MIN( dist,nattr.h);
        if( attr.w )    dist = MIN( dist, attr.w);
        if(nattr.w )    dist = MIN( dist,nattr.w);
        if( dist!=16000 &&
            !strchr(true_terms,vers.Alt[0].Code) &&
            nattr.col - (attr.col+attr.w)>0 &&
            nattr.col - (attr.col+attr.w)<dist/3 )
            return (CSTR_rast)0;    
        }
    }
c=c ? c : CSTR_GetLastRaster(CSTR_GetRasterLine(cs));
if( i )
    {
    i--;
    str--;
    }
*str='\0';
*word_len = (Word8)i;
return c;
}
///////////
// Проверить окружение слова
// Nick 16.06.2001
static Bool32 GoodWordBounds(CSTR_rast cs, CSTR_rast best, Int32 engCol, Int32 engLastCol)
{
	CSTR_rast_attr rattr;
	Rect32 rect;
	CSTR_rast c;
	int iUp,iDn;
	RecVersions vers;
	int porog;
	Bool32 IsGood = FALSE;

    rect.top = 32000;
	rect.bottom = 0;
	rect.left=32000;
	rect.right=0;

		// test our word
	for(c=cs,iUp=0; c && c != best; c= CSTR_GetNext(c) )
	{
			 CSTR_GetAttr(c,&rattr);
			 if( rattr.flg & CSTR_f_space)
				 continue;
			 if( !(rattr.flg & CSTR_f_bad) )
				 IsGood = TRUE;

			 rect.top = MIN(rect.top,rattr.row);
			 rect.bottom = MAX(rect.bottom,rattr.row+rattr.h);
			 rect.left = MIN(rect.left,rattr.col);
			 rect.right = MAX(rect.right,rattr.col+rattr.w);

			 iUp++;
	}

	if( !IsGood )
		return TRUE;

	// длинное слово?
	if( iUp >= 5 )
		return TRUE;

	//iUp = rect.top + (rect.bottom-rect.top)/3;
	//iDn = rect.bottom - (rect.bottom-rect.top)/3;
	iUp = (rect.bottom + rect.top)/2 - 1;
	iDn = (rect.bottom + rect.top)/2 + 1;

	porog = MAX(3, (rect.bottom-rect.top)/4);

	if( best )
	{

		   CSTR_GetCollection(best,&vers);
		   CSTR_GetAttr(best, &rattr);

		   if( !memchr(right_limit_word,vers.Alt[0].Code,sizeof(right_limit_word)) &&
			   ! (rattr.flg & CSTR_f_space) &&
			   ! (rattr.flg & CSTR_f_bad) &&
			   rattr.col <= rect.right+porog 
			  )
		   {
			if( rattr.h*2 > rect.bottom-rect.top &&
				rattr.row + rattr.h > iDn && 
				rattr.row < iUp
			  )
			{
              // всех взяли ?
              if( rattr.col >= engLastCol )
			    return FALSE;

			  // test next
			  best = CSTR_GetNext(best);
			  if( !best )
				  return FALSE;

			  CSTR_GetCollection(best,&vers);
		      CSTR_GetAttr(best, &rattr);

		      if( !memchr(right_limit_word,vers.Alt[0].Code,sizeof(right_limit_word)) &&
			      !(rattr.flg & CSTR_f_space) &&
				  ! (rattr.flg & CSTR_f_bad) &&
			     rattr.h*2 > rect.bottom-rect.top &&
				 rattr.row + rattr.h > iDn && 
				 rattr.row < iUp
			   )
			  return FALSE;
			}
		   }
	}

	if(  (c=CSTR_GetPrev(cs))!=NULL )
	{
		CSTR_GetCollection(c,&vers);   
		CSTR_GetAttr(c, &rattr);

		if( memchr(right_limit_word,vers.Alt[0].Code,sizeof(right_limit_word)) ||
			memchr(left_limit_word,vers.Alt[0].Code,sizeof(left_limit_word)) ||
			(rattr.flg & CSTR_f_space) ||
			(rattr.flg & CSTR_f_bad) ||
			rattr.col +rattr.w < rect.left - porog
		 )
		 return TRUE;

		if( rattr.h*2 <= rect.bottom-rect.top ||
			rattr.row + rattr.h < iDn || 
			rattr.row > iUp
		  )
		  return TRUE;

		// могли захватить лишнего?
		c = cs;
		CSTR_GetCollection(c,&vers);   
		CSTR_GetAttr(c, &rattr);


		if( memchr(right_limit_word,vers.Alt[0].Code,sizeof(right_limit_word)) ||
			memchr(left_limit_word,vers.Alt[0].Code,sizeof(left_limit_word)) ||
			(rattr.flg & CSTR_f_space) ||
			(rattr.flg & CSTR_f_bad) 
		  )
		  return TRUE;

		if( rattr.col >= engCol )
			return FALSE;

		
		if( rattr.h*2 <= rect.bottom-rect.top ||
			rattr.row + rattr.h < iDn || 
			rattr.row > iUp
		  )
			  return TRUE;

		return FALSE;
	}

    return TRUE;
}
////////////////
static Bool32   rsadd_over_segm(Int16 rr,Int16 rl,Int16 er,Int16 el,Int16  dr)
{
return (el>=rl-1 && er<=rr+dr && (rr-rl-er+el)*128<40*(rr-rl) );
}

static Bool32 rsadd_over_words(CSTR_rast rus, CSTR_rast ruse,CSTR_rast  eng,CSTR_rast  enge)
{
Int16           rr,rl,er,el, dr;
CSTR_rast_attr  attr;
Bool32  ret, rleftcut;
if( spec_camera )
    return TRUE;
    
dr=1;
CSTR_GetAttr(rus, &attr);
rl=attr.r_col;
ruse = CSTR_GetPrev(ruse);
CSTR_GetAttr(ruse, &attr);
if( attr.flg&CSTR_f_space )
    {
    ruse = CSTR_GetPrev(ruse);
    CSTR_GetAttr(ruse, &attr);    
    }
rr=attr.r_col+attr.w;
if( attr.pos_inc==CSTR_erect_rest )
    dr += 1;
rleftcut = ( attr.cg_flag & CSTR_cg_cutr );
CSTR_GetAttr(eng, &attr);
el=attr.r_col;
enge = CSTR_GetPrev(enge);
CSTR_GetAttr(enge, &attr);
if( attr.flg&CSTR_f_space )
    {
    enge = CSTR_GetPrev(enge);
    CSTR_GetAttr(enge, &attr);    
    }
er=attr.r_col+attr.w;///2; // 17.12.92
ret = rsadd_over_segm(rr,rl,er,el,dr);
if( !ret && rleftcut )
    ret = rsadd_over_segm(rr,rl,(Int16)(attr.r_col+attr.w/2),el,dr);
return ret;
}

static CSTR_rast rsadd_skip_punct(CSTR_rast endlet)
{
CSTR_rast       c=endlet;
CSTR_rast_attr  a;

 if( !c )
    return endlet;
 else
 { 
    CSTR_GetAttr(c,&a);
	
    if( !(a.flg& CSTR_f_punct) )
        return endlet;  
    }

 do  
 {
    c=CSTR_GetNextRaster(c,255);
    CSTR_GetAttr(c,&a);
 } 
 while( !(a.flg&CSTR_f_fict) && (a.flg&(CSTR_f_punct)));

 return !(a.flg&CSTR_f_fict)?c:CSTR_GetLastRaster(CSTR_GetRasterLine(c));
}



static void rsadd_cstr2word(CSTR_rast be,CSTR_rast en,Word8 *str)
{
CSTR_rast c;
RecVersions     vers;

for(c=be;c&&c!=en;c=CSTR_GetNext(c))
	{	
	CSTR_GetCollection(c,&vers);
	*str++  = vers.Alt[0].Code; 
	}

*str='\0';
return;
}

Bool32 rsadd_is_upper(Word8 Code, Word8 language)
{
Int32   code=(Int32)Code;

switch( language )
    {
    case    LANG_RUSSIAN:
        return( code>191 && code<224 || code>47&& code<58 );
    case    LANG_ENGLISH:
        return( code>64 && code<91 || code>47&& code<58 );            
    }
return FALSE;
}

Bool32 rsadd_is_thick(Word8 Code, Word8 language)
{
Int32   code=(Int32)Code;
if( code>49 && code<58 || code==48 )
    return TRUE;
switch( language )
    {
    case    LANG_RUSSIAN:
        return( code>191 && !strchr("шШщЩжЖыЫюЮ",code) );
    case    LANG_ENGLISH:
        return( strchr("QERTYUOPASDFGHJKLZXCVBNMqeyuopasdghkzxcvbn",code)!=NULL );            
    }
return FALSE;
}

Bool32 rsadd_is_digit(Word8 Code)
{
Int32   code=(Int32)Code;

return memchr("0123456789",code,10)!=NULL;
}

Bool32 rsadd_study_word(CSTR_rast beg, CSTR_rast end,RSADD_STAT *s)
{
CSTR_rast_attr  a;
CSTR_rast       r, re;
UniVersions     u;
Word16          minp, maxp, minup, maxup, minw, maxw;
Int16           norec, ndig, ndig_first, nup, nup_first, nupall, nall;
Bool32          unilet;

memset(s,0,sizeof(RSADD_STAT));
ndig=ndig_first=nup=nup_first=0;
nall=nupall=maxw=maxp=maxup=norec=0;
minw=minp=minup=256;
re = CSTR_GetPrev(end);
for(r=beg;r && r!= end; r=CSTR_GetNext(r), nall++)
    {
    CSTR_GetCollectionUni(r,&u);
    CSTR_GetAttr(r,&a);
    if( a.flg&CSTR_f_space )
        continue;
    if( u.lnAltCnt && u.Alt[0].Prob>2 )
        {
        if( !nall && memchr("<(\xd7\'\"",u.Alt[0].Liga ,3) )
            continue;
        if( r==re && memchr(">)\xd8\'\"",u.Alt[0].Liga ,3) )
            break;

        switch( a.language )
            {
            case    LANG_RUSSIAN:
                unilet=(strchr(rus_uni_letters,u.Alt[0].Liga)!=NULL);
                break;
            case    LANG_ENGLISH:
                unilet=(strchr(eng_uni_letters,u.Alt[0].Liga)!=NULL);
                break;
            default:
                unilet=FALSE;
                break;
            }
        if( unilet && u.Alt[0].Method!=REC_METHOD_EVN)
            {
            if( minup>u.Alt[0].Prob )
                minup = u.Alt[0].Prob ;
            if( maxup<u.Alt[0].Prob )
                maxup = u.Alt[0].Prob ;
            }
        if( minp>u.Alt[0].Prob )
            minp = u.Alt[0].Prob ;
        if( maxp<u.Alt[0].Prob )
            maxp = u.Alt[0].Prob ;

        if( u.Alt[0].Prob>220 )
            s->nh++;                
        else if( u.Alt[0].Prob>100 )
            s->nm++;                
        else
            s->ns++;
        if( rsadd_is_digit(u.Alt[0].Liga) )
            {
            if( !s->nall )
                ndig_first++;
            else
                ndig++;
            }
        if( rsadd_is_upper(u.Alt[0].Code[0],a.language) )
            {
            if( !s->nall )
                nup_first++;
            else
                nup++;
            nupall++;
            }
        if( rsadd_is_thick(u.Alt[0].Code[0],a.language) )
            {
            if( minw>a.w )
                minw=a.w;
            if( maxw<a.w )
                maxw=a.w;
            }
        }
    else
        norec++;
    s->nall++;
    }
if( minp==256 )    minp=0;
if( minup==256 )   minup=0;
s->minp      = (Word8)minp;
s->maxp      = (Word8)maxp;
s->norec     = norec;
s->var_width = ( maxw*2>minw*5 );
s->var_kegl  = ( !nup_first  && nupall || 
        nup+nup_first!=s->nall  && nup>0);
s->var_dig   = ( ndig_first && ndig+ndig_first!=s->nall|| 
        ndig+ndig_first!=s->nall && ndig>0 );
s->case_up   = (s->nall==nup+nup_first);
s->case_dn   = (nup+nup_first==0);
s->case_dig  = (ndig+ndig_first==s->nall);
s->maxup=(Word8)maxup;
s->minup=(Word8)minup;
return TRUE;
}

static void rsadd_DelWord(CSTR_rast rus,CSTR_rast ruse)
{
CSTR_rast c;

for(c=rus;c&&c!=ruse;)
    {
    c = CSTR_DelRaster (c);
    } 
return;
}

static Bool32 rsadd_CopyWord(   CSTR_rast   cn,
                                CSTR_rast   eng,CSTR_rast   enge, 
                                Bool32 rus_lang,
                                CSTR_rast   *svb,CSTR_rast   *sve,
								Int32 lastPos )
{
CSTR_rast   c;
CCOM_comp    *  comp;
RecRaster       rs;
UniVersions     vr;
CSTR_rast_attr  attr;

if( svb )
    *svb=0;
if( sve)
    *sve=0;
for(c=eng;c && c!=enge;c=CSTR_GetNext(c))
    {
    if( CSTR_GetAttr (c, &attr)                                 &&
        CSTR_GetImage (c, (Word8 *)(&rs), CSTR_TYPE_IMAGE_RS)   &&
        CSTR_GetCollectionUni(c,&vr)                            &&
        (comp=CSTR_GetComp(c))!=NULL )
        {
		// Nick 11.03.2001
        if( lastPos && ( !(attr.flg & CSTR_f_space) ) && 
			attr.col > lastPos )
			break;
		  
        if( rus_lang )
            attr.language=LANG_RUSSIAN;
        
        if( !(cn=CSTR_InsertRaster (cn))        ||
            !CSTR_SetAttr (cn, &attr)           ||
            !CSTR_StoreRaster (cn, &rs)         ||
            !CSTR_StoreCollectionUni (cn, &vr)  ||
            !CSTR_StoreScale(cn,comp->scale) )
             return FALSE;
        if( svb && c==eng )
            *svb=cn;
        if( sve)
            *sve=cn;
        } // end of getting information and storing
    } 
return TRUE;
}



static Bool32 rsadd_make_dupes(CSTR_rast   rus,CSTR_rast   ruse,CSTR_rast   eng,CSTR_rast   enge, Word8 type)
{
CSTR_rast       r,re;
CSTR_rast_attr  a;
Bool32          dp=FALSE;

if( !rus || !ruse || !eng || !enge )
    return FALSE;

re=CSTR_GetNextRaster(ruse,255);
for(r=rus;r&&r!=re;r=CSTR_GetNext(r))
    {
    CSTR_GetAttr(r,&a);
    a.dlang_dup=type;
    CSTR_SetAttr(r,&a);
    }
re=CSTR_GetNextRaster(enge,255);
for(r=eng;r&&r!=re;r=CSTR_GetNext(r))
    {
    CSTR_GetAttr(r,&a);
    a.dlang_dup=type;
    CSTR_SetAttr(r,&a);
    }
r=CSTR_GetPrev(rus);
if( r )
    {
    CSTR_GetAttr(r,&a);
    if( CSTR_GetDupEnd(r) )
        dp=TRUE;
    CSTR_SetDupEnd(r,0);
    }
if( !dp )
    CSTR_SetDup(rus, eng);
CSTR_SetDupEnd(ruse, enge);

return TRUE;
}

static CSTR_rast rsadd_get_left_coord(CSTR_line ln,Int16 left, Int16 right)
{
CSTR_rast       rst;
CSTR_rast_attr  attr, pattr;
Int16           leftn, rightn;
RecVersions     vers;

rst = CSTR_GetFirstRaster(ln);
do  {
    CSTR_GetAttr(rst,&attr);
    CSTR_GetCollection(rst,&vers); 
    if( !(attr.flg&CSTR_f_space) )// prevously attr
        pattr = attr;
    rst = CSTR_GetNextRaster(rst, CSTR_f_let|CSTR_f_bad|CSTR_f_punct|CSTR_f_fict);
    CSTR_GetAttr(rst,&attr);
    leftn   =attr.col;
    rightn  =attr.col+attr.w;
    if( rightn>=left && right>=leftn )
        {
        Int16   dist=MIN(attr.h,pattr.h);
        dist = MIN( dist, attr.w);
        dist = MIN( dist,pattr.w);
        if( !strchr(true_terms,vers.Alt[0].Code) &&
			vers.Alt[0].Code != liga_TM_usual && // Nick 12.01.2001
			vers.Alt[0].Code != liga_CC_usual &&
			vers.Alt[0].Code != liga_CR_usual &&
            attr.col - (pattr.col+pattr.w)<dist/4 )
            {
            if( rightn==left )
                continue;
            else
                return (CSTR_rast)0;
            }
        else
            return rst;
        }
    } while(rst);
return (CSTR_rast)0;
}

CSTR_rast rsadd_get_right_coord(CSTR_rast rst,Int16 left, Int16 right)
{
CSTR_rast_attr  attr;
Int16           leftn, rightn;

CSTR_GetAttr(rst,&attr);
if( attr.col+attr.w>=right-1 )
    return CSTR_GetNext(rst);
do  {
    rst = CSTR_GetNextRaster(rst, 255);
    CSTR_GetAttr(rst,&attr);
    
    leftn   =attr.col;
    rightn  =attr.col+attr.w;
    if( rsadd_over_segm(rightn,leftn,right,left,1) ||
       ((attr.cg_flag&CSTR_cg_comp) && abs(rightn-right)<1  )||
       ((attr.cg_flag&CSTR_cg_cutl) && abs(rightn-right)<1)  )
        {
        rst=CSTR_GetNextRaster(rst,255);
        CSTR_GetAttr(rst,&attr);
        return !(attr.flg&(CSTR_f_let|CSTR_f_bad))
            ?rst:(CSTR_rast)0;
        }
    } while(rst);
return (CSTR_rast)0;
}

RSADD_FUNC(Word8)    RSADD_get_alias_class(Word8 let,Word8 lang)
{
Word8   *fe,*fa,*fs;
if( lang==LANG_RUSSIAN )
    {
    fe=rus_equal;
    fa=rus_alias;
    fs=rus_similar;
    }
else if( lang==LANG_ENGLISH )
    {
    fe=eng_equal;
    fa=eng_alias;
    fs=eng_similar;
    }
else
    return RSADD_as_none;    
if( strchr(fe,let) )
    return RSADD_as_equal;

if( strchr(fa,let) )
    return RSADD_as_alias;

if( strchr(fs,let) )
    return RSADD_as_similar;

return RSADD_as_none;
}

RSADD_FUNC(Word8  *) RSADD_get_aliases( Word8 let,Word8 lang, Word8 alias_class)
{
Word8   *fe,*fa,*fs,*se,*sa,*ss, *out, *f,*s, *o;

if( lang==LANG_RUSSIAN )
    {
    fe=rus_equal;
    fa=rus_alias;
    fs=rus_similar;
    se=eng_equal;
    sa=eng_alias;
    ss=eng_similar;
    }
else if( lang==LANG_ENGLISH )
    {
    se=rus_equal;
    sa=rus_alias;
    ss=rus_similar;
    fe=eng_equal;
    fa=eng_alias;
    fs=eng_similar;
    }
else
    {
    out=buf_for_output1;
    *out=0;
    return out;
    }
switch( alias_class )
    {
    case    RSADD_as_equal:
        o=out=buf_for_output1;
        f=fe;
        s=se;
        break;
    case    RSADD_as_alias:
        o=out=buf_for_output2;
        f=fa;
        s=sa;
        break;
    case    RSADD_as_similar:
        o=out=buf_for_output3;
        f=fa;
        s=ss;
        break;
    }
for(;*f;f++,s++)
    {
    if( *f==let )
        *out++ = *s;
    }
*out=0;
return o;
}

Bool32  find_in_paar_arrays(Word8 e,Word8 r,
                    Word8 *eng,
                    Word8 *rus)
{
Int32   ndx;
Bool32  comp,find;
Word8 * p;

find=comp=FALSE;
for(ndx=0,p=eng; *p;p++,ndx++)
    {
    if( *p==e )
        {
        find=TRUE;
        if( rus[ndx]==r )
            {
            comp = TRUE;  
            break;
            }
        }
    }
if( !comp || !find )
    return FALSE;

return TRUE;
}

static Bool32 rsadd_aliases(Word8 *r,Word8 *e, Word8 mode)
{

for(;*e&&*r;r++,e++)
    {
    if( *e=='~' || *r=='~' )
        return FALSE;
    if( *e==*r )
        continue;
    switch( mode )
        {
        case    0: // equal
            if( !find_in_paar_arrays(*e,*r,
                    eng_equal,
                    rus_equal) )
                return FALSE;
            break;
        case    1: // alias
            if( !find_in_paar_arrays(*e,*r,
                    eng_equal,
                    rus_equal) && 
                !find_in_paar_arrays(*e,*r,
                    eng_alias,
                    rus_alias))
                return FALSE;
            break;
        case    2: // similar
            if( !find_in_paar_arrays(*e,*r,
                    eng_equal,
                    rus_equal) && 
                !find_in_paar_arrays(*e,*r,
                    eng_alias,
                    rus_alias) && 
                !find_in_paar_arrays(*e,*r,
                    eng_similar,
                    rus_similar))
                return FALSE;
            break;
        }
    
    }
return TRUE;
}

Bool32  rsadd_similar(Word8 *rwrd,Word8 *ewrd)
{
return rsadd_aliases(rwrd,ewrd,2);
}

Bool32  rsadd_get_hiprob(CSTR_rast rus, CSTR_rast ruse)
{
CSTR_rast       r;
CSTR_rast_attr  a;
UniVersions     u;

for(r=rus;r && r!=ruse;r=CSTR_GetNext(r))
    {
    CSTR_GetAttr(r,&a);
    CSTR_GetCollectionUni(r,&u);
    if( (a.flg&CSTR_f_let) && 
        u.lnAltCnt && 
        strchr(eng_uni_letters,u.Alt[0].Code[0]) &&
        u.Alt[0].Prob>220 )
        return TRUE;
    }
return FALSE;
}


Bool32  rsadd_get_solid(CSTR_rast rus, CSTR_rast ruse)
{
CSTR_rast       r;
CSTR_rast_attr  a;
UniVersions     u;

// Nick 29.10.2000 - remove left, right terms
BYTE left_terms[]="<(\"\'/[",
     right_terms[]="/)>]\"!\';?\x1e\x1f";
Bool32 wasRight;
int    nlet;

   for(r=rus;r && r!=ruse;r=CSTR_GetNext(r))
   {
    CSTR_GetCollectionUni(r,&u);
    if( !u.lnAltCnt || !strchr(left_terms,u.Alt[0].Code[0]) )
		break;
   }
   rus = r;

   wasRight =  FALSE;
   nlet = 0;

   for(r=rus;r && r!=ruse;r=CSTR_GetNext(r))
   {
    CSTR_GetCollectionUni(r,&u);

	if( strchr(right_terms,u.Alt[0].Code[0]) )
	{
		wasRight = TRUE;
		continue;
	}

	if( wasRight )
		return FALSE;

	CSTR_GetAttr(r,&a);
    if( !(a.flg&CSTR_f_solid) )
        return FALSE;
	nlet++;
   }

   if( nlet < 2 )
	   return FALSE;

/* old version
   for(r=rus;r && r!=ruse;r=CSTR_GetNext(r))
   {
    CSTR_GetAttr(r,&a);
    CSTR_GetCollectionUni(r,&u);
    if( !(a.flg&CSTR_f_solid) && 
         (u.lnAltCnt && !strchr(non_letters,u.Alt[0].Code[0]) ||
        !u.lnAltCnt ))
        return FALSE;
   }
*/

return TRUE;
}


static char *rsadd_prev_eng_group[]={
"www",
"http",
"mail",
"mail:",
"http:",
"\0"
};
static char *rsadd_prev_eng_group_err[]={
"wvw",
"ww",
"\0"
};
Bool32 rsadd_eng_group(Word8 *wrd,Word8 last)
{
Int32   i;
for(i=0; rsadd_prev_eng_group[i][0]!=0;i++)
    {
    if( !_strcmpi(rsadd_prev_eng_group[i],wrd) )
        return TRUE;
    }
if( last=='.' )
    {
    for(i=0; rsadd_prev_eng_group_err[i][0]!=0;i++)
        {
        if( !_strcmpi(rsadd_prev_eng_group_err[i],wrd) )
            return TRUE;
        }
    }
return FALSE;
}

Bool32  rsadd_eng_group_CSTR(CSTR_rast b,CSTR_rast e)
{
CSTR_rast       r;
CSTR_rast_attr  a;
UniVersions     u;
Word8           s[80];

for(s[0]=0,r=b;r&&r!=e;r=CSTR_GetNext(r))
    {
    CSTR_GetAttr(r,&a);
    CSTR_GetCollectionUni(r,&u);
    if( a.language!=LANG_ENGLISH || !u.lnAltCnt)
        return FALSE;
    strcat(s,u.Alt[0].Code);
    }

return rsadd_eng_group(s,0);
}

Bool32  rsadd_represent_word(CSTR_rast b,CSTR_rast e)
{
return (rsadd_get_solid(b,e)    ||
        rsadd_get_hiprob(b,e)   ||
        rsadd_eng_group_CSTR(b,e) );
}

static Word8 *russian_voc_spec[]={
"Ј.",
"\0"
};

Bool32   rsadd_is_short_form(Word8 *rwrd)
{
Word8   russian_year[]="Ј.";
Int32   i,ii,iv;
Word8   *p;

ii=strlen(rwrd);
iv=strlen(russian_year);
if( rwrd[0]==russian_year[0] )
    {    
    if( ii>=iv &&
        !memcmp(russian_year,rwrd,iv) )
        {
        if( ii>iv )
            {
            for(p=rwrd+iv;*p;p++)
                if( !strchr(".,",*p) )
                    break;
            if( !*p )
                return TRUE;
            }
        else
            return TRUE;
        }
     }
if( ii>iv && !memcmp(rwrd+ii-iv,russian_year,iv)  )
    {
    for(i=ii-iv-1;i>=0;i--)
        if( !strchr("1234567890",*(rwrd+i)) )
            break;
    if( i<0 )
        return TRUE;
    }
return FALSE;
}


void rsadd_revert_lang(CSTR_rast prev,CSTR_rast start,Word8 lnew)
{
CSTR_rast       r;
CSTR_rast_attr  a;
RecVersions     v;

if( !prev )
    return;
CSTR_GetAttr(prev,&a);
if( a.flg&CSTR_f_fict )
    return;
for(r=CSTR_GetPrev(start);r&&r!=prev;r=CSTR_GetPrev(r))
    {
    CSTR_GetCollection(r,&v);
    if( v.lnAltCnt && !strchr(right_limit_word,v.Alt[0].Code) )
        break;
    CSTR_GetAttr(r,&a);
    a.language = lnew;
    CSTR_SetAttr(r,&a);
    }

return;
}

RSADD_FUNC(void) RSADD_kill_dusts(CSTR_line lrus)
{
CSTR_rast       rus = CSTR_GetNext(CSTR_GetFirstRaster(lrus)), d;
CSTR_rast_attr  attr;

while( rus )
    {
    CSTR_GetAttr(rus,&attr);
    if( (attr.flg_new&CSTR_fn_temp) && attr.dlang_dup==CSTR_fd_none )
        {
        d=rus;
        rus=CSTR_GetNext(rus);
        CSTR_DelRaster(d);
        }
    else
        rus=CSTR_GetNext(rus);
    }
return;
}

Bool32  rsadd_bl_conflict(CSTR_rast eng,CSTR_rast enge,Int32 BL_small)
{
CSTR_rast       c;
CSTR_rast_attr  attr;
UniVersions     u;
Int32           all, small;

for(all=small=0,c=eng;c && c!=enge;c=CSTR_GetNext(c))
    {    
    CSTR_GetCollectionUni(c,&u);
    if( u.lnAltCnt && isupper(u.Alt[0].Code[0]) )
        {
        CSTR_GetAttr(c,&attr);
        if( (attr.h-BL_small)<2 )
            small++;
        all++;
        }
    }
return (small==all || all>2 && small*3>=all*2);
}

// Nick 11.03.2001
static int rsadd_GetLastPos(CSTR_rast fir, CSTR_rast rast)
{
	CSTR_rast_attr attr;

	if( !rast || fir == rast )
		return 0;

	rast = CSTR_GetPrevRaster(rast,CSTR_f_all);

    for( ;rast && rast != fir; rast = CSTR_GetPrevRaster(rast,CSTR_f_all))
	{
		CSTR_GetAttr(rast,&attr);

		if( ! (attr.flg & CSTR_f_space) )
		   return attr.col+attr.w;
	}

	return 0;
}

RSADD_FUNC(Bool32) RSADD_take(CSTR_line lrus,CSTR_line leng)
{
CSTR_attr       lattr;
CSTR_rast       rus, eng, ruse, enge;
Word8           rlen,rwrd[MAX_LEN_WORD+40];
Word8           elen,ewrd[MAX_LEN_WORD+40],*types[]={"no","rus","eng","rus-eng"};
CSTR_rast_attr  eattr,rattr, attr, tmpattr;
Int16           type = 0;
//Int16           rtri, etri;
Word8           buf[256],buf1[256],buf2[256],buf3[256];
CSTR_line       sln[2]={lrus, leng};
Bool32          rsolid, esolid, verdict, aliases,
                enorec, rnorec, equal, similar, short_russian=FALSE,
                need_rus_coord, end_eng_space, is_one_eword, is_one_rword,
                hide;
Word8           typea  ;
RSADD_STAT      estat, rstat;
CSTR_rast       svb,sve, cn,cne, nb, ne, pruse;
Int32           nall,neng,num_word, BL_cap,BL_small ;
RecVersions     v;
Int32           engCol;
Bool32          isSolid;


CSTR_GetLineAttr(lrus,&lattr);
BL_cap   = lattr.bs3-lattr.bs1;
BL_small = lattr.bs3-lattr.bs2;

// Nick 29.10.00
CSTR_GetLineAttr(leng,&lattr); 
BL_small = MIN(BL_small, lattr.bs3-lattr.bs2);


tables=(lattr.tab_column>0);
num_word=nall=neng=0;
hide=FALSE;
ruse = rus = CSTR_GetFirstRaster(lrus);
enge = CSTR_GetFirstRaster(leng);
while(1)
    {
    eng = rsadd_next_word(enge);
    if( !eng )
        break;
    enge = rsadd_end_word(eng,ewrd,&elen,-16000,32767,0,FALSE);
    num_word++;
    end_eng_space=FALSE;
    if( enge )
        {
        CSTR_GetAttr(enge,&eattr);
        end_eng_space=eattr.flg&CSTR_f_space;
        }
    need_rus_coord=FALSE;
    CSTR_GetAttr(enge,&attr);
    is_one_eword=( (attr.flg&CSTR_f_fict) && num_word==1 );
        
    CSTR_GetCollection(enge,&v);
    if( rsadd_eng_group(ewrd,(Word8)((attr.flg&CSTR_f_punct)?v.Alt[0].Code:0)) )
        {
        enge = rsadd_end_word(eng,ewrd,&elen,-16000,32767,2,FALSE);
        need_rus_coord=TRUE;
        }
    nall++;
    CSTR_GetAttr(eng,&eattr);
    rus  = rsadd_get_left_coord(lrus,eattr.col,(Int16)(eattr.col+eattr.w));
    if( !rus  )
        {
        strcpy(buf,"can't find russian for ");
        strcat(buf,ewrd);
        if( db_status && snap_activity(etap_name) )
            {				
            snap_show_text(buf);
		    snap_monitor_ori(sln,2);
		    }
        continue;
        }
    cn=CSTR_GetPrevRaster(enge,255);
    CSTR_GetAttr(cn,&eattr);
    while( eattr.flg & CSTR_f_space )
        {
        cn=CSTR_GetPrevRaster(cn,255);
        CSTR_GetAttr(cn,&eattr);
        }
    pruse = ruse;    
    ruse = rsadd_end_word (rus,rwrd,&rlen,eattr.col,(Int16)(eattr.col+eattr.w),1,TRUE);
    
    if( need_rus_coord )
         {
         CSTR_GetAttr(enge,&attr);
         ruse=rsadd_get_right_coord(ruse,eattr.col,(Int16)(eattr.col+eattr.w));
         if( !ruse && (attr.flg&CSTR_f_fict) )
            ruse = CSTR_GetLastRaster(lrus);
         }

	isSolid = FALSE;
	{
		CSTR_rast_attr tmp;
		CSTR_GetAttr(eng,&tmp);
		engCol = tmp.col;
		isSolid = (tmp.flg & CSTR_f_solid)?TRUE:FALSE;
	}

    if( !ruse || !isSolid && !GoodWordBounds(rus,ruse,engCol,eattr.col+eattr.w) )
        {
        if( end_eng_space ||
            rsadd_represent_word(eng, enge)  )
            ruse=rsadd_get_right_coord(rus,eattr.col,(Int16)(eattr.col+eattr.w));
        if( is_one_eword )
            ruse =  CSTR_GetPrev(CSTR_GetLastRaster(lrus));
        
        if( !ruse || !isSolid && !GoodWordBounds(rus,ruse,engCol,eattr.col+eattr.w) )
            {
            strcpy(buf,"can't find russian for <");
            strcat(buf,ewrd);
            strcat(buf,">");
            if( db_status && snap_activity(etap_name) )
                {				
                snap_show_text(buf);
		        snap_monitor_ori(sln,2);
		        }
            continue;
            }
        }
    if( !strcmp(ewrd,rwrd) )
        {
        strcpy(buf,"skip equal words <");
        strcat(buf,ewrd);
        strcat(buf,"> <");
        strcat(buf,rwrd);
        strcat(buf,">");
        if( db_status && snap_activity(etap_name) )
            {				
            snap_show_text(buf);
		    snap_monitor_ori(sln,2);
		    }
        continue;
        }
    if( !rsadd_over_words(rus, ruse, eng, enge) )
        {
        if( rsadd_represent_word(eng,enge) )
            ruse=rsadd_get_right_coord(ruse,eattr.col,(Int16)(eattr.col+eattr.w));
        if( !rsadd_over_words(rus, ruse, eng, enge) )
            ruse = rsadd_end_word (rus,rwrd,&rlen,eattr.col,(Int16)(eattr.col+eattr.w),2,TRUE);
        if( !rsadd_over_words(rus, ruse, eng, enge))
            {
            hide = rsadd_eng_L(eng,enge,rus);
            ruse = rsadd_end_word (rus,rwrd,&rlen,eattr.col,(Int16)(eattr.col+eattr.w),2,TRUE);
            }

        }
    strcpy(buf, "");
    strcpy(buf1,"");
    strcpy(buf2,"");
    strcpy(buf3,"");

    CSTR_GetAttr(ruse,&rattr);
    is_one_rword=( (rattr.flg&CSTR_f_fict) && num_word==1 );
    if( is_one_eword && is_one_rword || rsadd_over_words(rus, ruse, eng, enge) )
        {
        CSTR_GetAttr(eng,&eattr);
        CSTR_GetAttr(rus,&rattr);
        rsadd_study_word(rus,ruse,&rstat);
        rsadd_study_word(eng,enge,&estat);
        rsolid=rsadd_get_solid(rus,ruse);
        esolid=rsadd_get_solid(eng,enge);
        verdict=FALSE;
        aliases=FALSE;
        equal=FALSE;
        if( rlen==elen )
            {
            aliases = rsadd_aliases(rwrd,ewrd,1);
            equal   = rsadd_aliases(rwrd,ewrd,0);
            if( esolid && rsolid )
                esolid = rsolid =FALSE;
            }
        if( estat.ns && elen<4 || estat.ns>1 )
            esolid = FALSE;
        if( rstat.ns && rlen<4 || rstat.ns>1 )
            rsolid = FALSE;
        type = 0;
        switch( rattr.rus_eng_word )
            {
            case CSTR_ruseng_rus:   
                if( !verdict )
                verdict = ( estat.minp>=rstat.minp+crsadd_rus1 && esolid || 
                    estat.minp>=rstat.minp+crsadd_rus2 && !rsolid  );
                type = 1;
                break;
            case CSTR_ruseng_eng:
                if( !verdict )
                verdict = ( esolid || 
                    estat.minup>180 && rstat.minup<100 && !rsolid ||
                    estat.minp>=rstat.minp+crsadd_eng1 && !rsolid || 
                    estat.minp>=rstat.minp-crsadd_eng3 && rstat.var_dig && !estat.var_dig ||
                    aliases && !rsolid && estat.minp>=rstat.minp+crsadd_eng2 );
                type = 2;
                break;
            case CSTR_ruseng_re :               
            case CSTR_ruseng_alt:
                if( !verdict && !estat.var_dig  )
                verdict = ( esolid || 
                    estat.minup>200 && rstat.minup<100 && !rsolid ||
                    estat.minp>=rstat.minp+crsadd_re1 && !rsolid || 
                    aliases && rsolid && estat.minp>=rstat.minp+crsadd_re2 );
                type = 3;
                break;
            }
        
//        rtri=rec_versions_triad( rwrd,LANG_RUSSIAN);
//        etri=rec_versions_triad( ewrd,LANG_ENGLISH);

        enorec=FALSE;
        if( estat.norec && !rstat.norec )
            enorec=TRUE;
        if( estat.norec && rstat.norec && estat.norec>rstat.norec )
            enorec=TRUE;
        rnorec=FALSE;
        if( rstat.norec && !estat.norec )
            rnorec=TRUE;
        if( estat.norec && rstat.norec && rstat.norec>estat.norec )
            rnorec=TRUE;

        if( enorec )
            verdict=FALSE;
        if( rnorec && 
            !(rnorec==1 && rstat.minp>estat.minp+150 && estat.minup<100 ) &&
            !(rnorec==1 && rstat.minup>250 && estat.minup<100 ) &&
            !(rnorec==1 && !rstat.var_dig && rstat.minp>estat.minp+20 && estat.maxup && estat.maxup<180 ) )
            verdict=TRUE;
        if( rstat.norec && rstat.norec<2 )
            {
            if( estat.var_width && !rstat.var_width )
                verdict=FALSE;
            if( estat.var_kegl && aliases && !rstat.var_kegl )
                verdict=FALSE;
            if( estat.var_dig && !rstat.var_dig && type!=2)
                verdict=FALSE;
            }
        similar = FALSE;
        if( !equal && !aliases )
            similar = rsadd_similar(rwrd,ewrd);
        if( equal && abs(estat.minp-rstat.minp)<crsadd_equa1 && abs(estat.maxp-rstat.maxp)<crsadd_equa2 ) 
            verdict=FALSE;
        if( !estat.var_kegl && (isupper(ewrd[0])||isupper(ewrd[1])) &&
            rsadd_bl_conflict(eng,enge,BL_small) )
            {
            estat.var_kegl = TRUE;
            verdict=FALSE;
            }
        sprintf(buf1,"type=%s esolid=%c,rsolid=%c,similar=%c,alias=%c,equal=%c",
            types[type],
            esolid?'y':'n',rsolid?'y':'n',
            similar?'y':'n',
            aliases?'y':'n',
            equal?'y':'n'
            );
        sprintf(buf2,"eng : min=%d max=%d umin=%d umax=%d norec=%d nall=%d hei=%d mid=%d low=%d var:wid=%c keg=%c dig=%c",
                estat.minp, estat.maxp, estat.minup, estat.maxup,
                estat.norec, estat.nall, estat.nh, estat.nm, estat.ns,
                estat.var_width?'y':'n',estat.var_kegl?'y':'n',estat.var_dig?'y':'n');
        sprintf(buf3,"rus : min=%d max=%d umin=%d umax=%d norec=%d nall=%d hei=%d mid=%d low=%d var:wid=%c keg=%c dig=%c",
                rstat.minp, rstat.maxp, rstat.minup, rstat.maxup,
                rstat.norec, rstat.nall, rstat.nh, rstat.nm, rstat.ns,
                rstat.var_width?'y':'n',rstat.var_kegl?'y':'n',rstat.var_dig?'y':'n');
        short_russian=FALSE;
        if( verdict ) 
            {
            neng++;
            rsadd_revert_lang(pruse,rus,LANG_ENGLISH);
            ruse = rsadd_skip_punct(ruse);
            enge = rsadd_skip_punct(enge);
            rsadd_cstr2word(rus,ruse,rwrd);
            rsadd_cstr2word(eng,enge,ewrd);
            short_russian=rsadd_is_short_form(rwrd);
            if( short_russian )
                {
                strcat(buf,"russian: <");
		        strcat(buf,rwrd);
                strcat(buf,"> is short form");
                }
            else
                {
                Bool32  copy_pass2=FALSE;
                copy_pass2=( similar || short_russian || estat.minp<crsadd_dup4e );

				// Nick 02.02.2001
				if( ( !esolid || rsolid ) &&
					(
					 estat.minp < rstat.minp && estat.nh < rstat.nh ||
					 estat.minp < rstat.minp && estat.maxp < rstat.maxp && estat.nh <= rstat.nh ||
					 estat.maxp <= rstat.maxp && 
					 (estat.nh < rstat.nh || rstat.nh + 1 >= rstat.nall && rstat.minp > 180 )&& 
					 ( estat.maxp < rstat.maxp || rstat.nh + 1 >= rstat.nall ) &&
					 estat.maxup < 220  // not hei
					)
				  )
					copy_pass2=TRUE;

                if( rstat.norec==rstat.nall )
                    copy_pass2=FALSE;
                strcat(buf,"english: <");
		        strcat(buf,ewrd);
                strcat(buf,"> replace russian: <");
                strcat(buf,rwrd);
			    strcat(buf,">");
                cn=CSTR_GetPrevRaster(rus,255);
                cne=CSTR_GetPrevRaster(eng,255);

				{ // Nick 11.03.2001
                 int lastRus = rsadd_GetLastPos(rus,ruse);

                 if( copy_pass2 )
                    rsadd_CopyWord(cne,rus,ruse, 1, &svb, &sve, 0 );
                 rsadd_DelWord(rus,ruse);
                 if( !rsadd_CopyWord(cn,eng,enge, estat.case_dig,&nb,&ne, lastRus ) )
                    return FALSE;
                 rsadd_DelWord(eng,enge);
				}
                
                if( copy_pass2 )
                    {
                    if( similar || short_russian )
                        typea = CSTR_fd_similar;
                    else
                        typea = CSTR_fd_lowprob;
                    rsadd_make_dupes(nb,ne,svb,sve,typea);
                    strcat(buf,"+ save for pass2");
                    }
                CSTR_GetAttr(cn,&tmpattr);
                if( !(tmpattr.flg & (CSTR_f_space|CSTR_f_fict)) )
                    {
                    UniVersions u;
                    memset(&tmpattr,0,sizeof(CSTR_attr));
                    tmpattr.flg=CSTR_f_space;
                    u.lnAltCnt=1;
                    u.Alt[0].Liga=SS_POS_HALF_SPACE;
                    u.Alt[0].Prob=150;
                    strcpy(u.Alt[0].Code," ");
                    cn=CSTR_InsertRaster(cn);
                    CSTR_SetAttr(cn,&tmpattr);
                    CSTR_StoreCollectionUni(cn,&u);
                    }
                enge = CSTR_GetPrevRaster(enge,255);

				if( ruse )
				{ 
                 RecVersions     vers; 
				 CSTR_GetCollection(ruse,&vers);

		         if( !memchr(right_limit_word,vers.Alt[0].Code,sizeof(right_limit_word)) )
                  ruse = CSTR_GetPrevRaster(ruse,255);
				}
                }
            } // end of solid word
        else if(    (rstat.minp<crsadd_dup4r && !rsolid && !estat.var_dig && !estat.var_kegl ) || 
                    !aliases && !rsolid && !(estat.var_kegl&&!rstat.var_kegl) && abs(rstat.minp-estat.minp)<10 && abs(rstat.maxp-estat.maxp)<10 ||
                    similar || short_russian ||
                    nall>4 && neng*5>nall*3 && rstat.minp<crsadd_dup5r && !rsolid && !estat.var_dig && !estat.var_kegl  ||
                    (!enorec && 
                   (rattr.rus_eng_word&(CSTR_ruseng_re| CSTR_ruseng_alt)) && 
                   (rstat.minp>crsadd_dup2 && estat.minp>crsadd_dup1 && estat.minp>rstat.minp-crsadd_dup2||
                    aliases && estat.minp>crsadd_dup3 ||
                    equal )) )
            { // save for pass2
            if( estat.norec!=estat.nall )
                {
                if( !strcmp("Ј",rwrd) )
                    {
                    ruse = rsadd_skip_punct(ruse);
                    enge = rsadd_skip_punct(enge);
                    rsadd_cstr2word(rus,ruse,rwrd);
                    rsadd_cstr2word(eng,enge,ewrd);
                    }
                if( rsadd_is_short_form(rwrd) )
                    {
                    strcat(buf,"russian: <");
		            strcat(buf,rwrd);
                    strcat(buf,"> is short form");
                    }
                else
                    {
                    strcat(buf,"english: <");
                    strcat(buf,ewrd);
                    strcat(buf,"> saved for pass2");
                    if( equal )
                        typea = CSTR_fd_equal;
                    else if( similar )
                        typea = CSTR_fd_similar;
                    else if( aliases )
                        typea = CSTR_fd_alias;
                    else if( rstat.minp<crsadd_dup4r )
                        typea = CSTR_fd_lowprob;
                    else
                        typea = CSTR_fd_alias;
                    rsadd_make_dupes(rus,CSTR_GetPrev(ruse),eng,CSTR_GetPrev(enge),typea);
                    }
                }
            }
        else
            {
            strcat(buf,"english: <");
            strcat(buf,ewrd);
            strcat(buf,"> deleted");
            rsadd_DelWord(eng,enge);
            }
            
        } // end of over words
    else
        {
        strcat(buf,"can't overaly english: <");
        strcat(buf,ewrd);
        strcat(buf,"> and russian: <");
        strcat(buf,rwrd);
        strcat(buf,">");
        }
    if( db_status && snap_activity(etap_name) )
        {				
		snap_show_text(buf1);
        snap_show_text(buf2);
        snap_show_text(buf3);
        snap_show_text(buf);
		snap_monitor_ori(sln,2);
		}
    } // enf of while( 1 )
RSADD_kill_dusts(lrus);
RSADD_kill_dusts(leng);
if( hide )
    rsadd_clear_hide(lrus);
return TRUE;
}

void rsadd_clear_hide(CSTR_line lrus)
{
CSTR_rast       rus = CSTR_GetNext(CSTR_GetFirstRaster(lrus));
CSTR_rast_attr  attr;

while( rus )
    {
    CSTR_GetAttr(rus,&attr);
    if(  attr.flg_new & CSTR_fn_hide )
        {
        attr.flg_new -= CSTR_fn_hide;
        CSTR_SetAttr(rus,&attr);
        }
    rus=CSTR_GetNext(rus);
    }
return;
}

Bool32 rsadd_hide_segment(CSTR_rast rus,Int32 b,Int32 e,Int32 lim)
{
CSTR_rast_attr  attr;
Int32           ri, le, right, hide;
CSTR_rast       rri;

right=-16000;
hide=0;
do  {
    CSTR_GetAttr(rus,&attr);
    if( attr.flg&(CSTR_f_let|CSTR_f_bad|CSTR_f_punct) )
        {
        le=attr.col;
        ri=le+attr.w;
        if( ri>right && ri<b )
            {
            rri=rus;
            right=ri;
            }
        if( le>e )
            {
            if( right==-16000)
                return FALSE;
            for( rri=CSTR_GetNext(rri); rri && rri!=rus;rri=CSTR_GetNext(rri))
                {
                CSTR_GetAttr(rri,&attr);
                if( attr.flg&(CSTR_f_space|CSTR_f_punct) )
                    {
                    attr.flg_new |= CSTR_fn_hide;
                    CSTR_SetAttr(rri,&attr);
                    hide++;
                    }             
                }
            return (hide>0);
            }
        }
    rus=CSTR_GetNext(rus);
    }while(rus && attr.col<=lim);
return FALSE;
}

Bool32 rsadd_eng_L(CSTR_rast eng,CSTR_rast enge,CSTR_rast rus)
{
Int32           lim,hide;
UniVersions     uvs;
CSTR_rast_attr  attr;
CSTR_rast       pr=CSTR_GetPrev(enge);

CSTR_GetAttr(pr,&attr);
lim = attr.col+attr.w;
hide=0;
for( ; eng && eng!=enge; eng=CSTR_GetNext(eng))
    {
    CSTR_GetCollectionUni(eng,&uvs);
    if( uvs.lnAltCnt && uvs.Alt[0].Liga=='L' && uvs.Alt[0].Prob>200 )
        {
        CSTR_GetAttr(eng,&attr);
        if( rsadd_hide_segment(rus,attr.col,attr.col+attr.w,lim) )
            hide ++;
        }
    }
return (hide>0);
}
// standart functions 



RSADD_FUNC(Bool32)  RSADD_Init( Word16 wHeightCode, Handle hStorage )
{
wHeightRC = wHeightCode;
wLowRC=RSADD_ERR_NO;
return TRUE;
}



RSADD_FUNC(void)    RSADD_Done(void)
{
return;
}

RSADD_FUNC(Word32)  RSADD_GetReturnCode(void)
{
if( wLowRC==RSADD_ERR_NO)
  return 0;
return (wHeightRC<<16)|(wLowRC-RSADD_ERR_MIN);
}

RSADD_FUNC(char*)   RSADD_GetReturnString(Word32 dwError)
{
	Word16 rc = (Word16)(dwError & 0xFFFF + RSADD_ERR_MIN);
	static char szBuffer[512];

	if( dwError >> 16 != wHeightRC)
		wLowRC = RSADD_ERR_NOTIMPLEMENT;

	if( rc > 0 && rc <= RSADD_ERR_MAX-RSADD_ERR_MIN  )
		strcpy((char *)szBuffer,RSADD_error_name [rc]);
	else
		return NULL;

	return szBuffer;
}

RSADD_FUNC(Bool32)  RSADD_SetImportData(Word32 dwType, void * pData)
{

  wLowRC = RSADD_ERR_NO;
	switch(dwType)
	{

	default:
		wLowRC = RSADD_ERR_NOTIMPLEMENT;
		return FALSE;
	}
return TRUE;
}

RSADD_FUNC(Bool32)  RSADD_GetExportData(Word32 dwType, void * pData)
{
	Bool32 rc = TRUE;

#define EXPORT(a) *(Word32*)(pData)=          (Word32)a;
  wLowRC = RSADD_ERR_NO;
	switch(dwType)
	{
	case    RSADD_TAKE:				// главная функция слияния двух строк
        *(FNRSADD_take*)(pData)=			RSADD_take;
		break;
    case    RSADD_GETALIASES:		// список родственных символов
        *(FNRSADD_get_aliases*)(pData)=		RSADD_get_aliases;
        break;
    case    RSADD_GETALIASCLASS:    // класс родственных символов
        *(FNRSADD_get_alias_class*)(pData)=	RSADD_get_alias_class;
        break;
	case    RSADD_KILL_DUSTS:		// уничтожить лишние дусты
        *(FNRSADD_kill_dusts*)(pData)=		RSADD_kill_dusts;
        break;
	case    RSADD_SETRSTR:			// приём информации и функций из б-ки RSTR
        *(FNRSADD_SetRSTR*)(pData)=	RSADD_SetRSTR;
        break;

    default:
		wLowRC = RSADD_ERR_NOTIMPLEMENT;
		rc = FALSE;
	}
#undef EXPORT
return rc;
}
