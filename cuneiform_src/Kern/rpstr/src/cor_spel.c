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

/********************************************************************/
/*                                                                  */
/*                   Spelling checker for ContainerSTR              */
/*                                                                  */
/********************************************************************/
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "cstr.h"
#include "ccom.h"
#include "crling.h"
#include "leo.h"
#include "std.h"

#include "rstr.h"
#include "dpuma.h"

#pragma pack(push, 1)
#include "cor_spel.h"
#include "lang_def.h"
#pragma pack(pop)

// проверять альтернативы ?
#define _TEST_ALTER_

#include "minmax.h"

#include "compat_defs.h"

#define SS_NEG_HALF_SPACE 0x1e
#define SS_POS_HALF_SPACE 0x1f
#define MAX_LEN_WORD  48

static  char    dash[]="-\x5F—";
static  uchar   ed_left_limit_word[] =" -()[{.,:!\'\"\xbb\xab?\x84</—\x99\xa9\xae";
static  uchar   ed_right_limit_word[]=" -()]}.,:;!\'\"\xbb\xab?/>\x84—\x99\xa9\xae";
static  uchar   ed_half_spaces[3]="\x1e\x1f";
static  uchar   multy_language;
extern  Bool32  skip_line, snap_enable ;
extern  Handle  hSnapSpell,hSnapMatch,hSnapEndWord,hSnapStartWord;
extern  char*   sp_err;

uchar   language;
static  uchar   ed_buffer[32000], edo_buffer[32000], *MED_file_bound, *MED_file_end ;
static  RecVersions ed_vers[100];
static  UniVersions rej_vers[100], rejo_vers[100];
static  Rect16  ed_rect[100];
static  int32_t   ed_nvers, nrej_vers, nrejo_vers;
;

struct  vers_ref            ed_vrs;
struct  bit_map_ref         bmr;
struct  fragm_disk          ed_fd={0};
struct  fragm_disk_descr    ed_fdd={0};
struct  sheet_disk_descr    ed_sdd={0};

static  Bool32 make_fictive_str(CSTR_line fln, CSTR_rast eng, CSTR_rast enge, CSTR_line ln, CSTR_line lnraw);
// Nick 27.01.2001
static int rpstr_case_notequal( uchar *in,uchar *out,
        int32_t lenin, int32_t lenout,UniVersions *uvs );
// Nick 18.06.2001
static  int32_t rpstr_test_spell_alter(CSTR_rast be,CSTR_rast en, int32_t nlim, UniVersions *uv);
// Nick 8.01.2002
static int  rpstr_test_21( CSTR_rast eng, CSTR_rast enge,
						   char *ewrd, int lang );

///////////////
static void ed_write(uchar* p, uint16_t size)
{
if (MED_file_bound -  MED_file_end < size)
    {
    return;
    }
memcpy (MED_file_end, p, size);
MED_file_end += size;
}

static  Bool32    ed_exclude_to_vers(int32_t size, uchar *str)
{
uchar  *p,*pe;
struct  vers_ref     vf;
struct  bit_map_ref  bm;
int32_t   i;
Bool32  new_bmp;

//i=-1;
i=ed_nvers=0;
ed_vers[0].lnAltCnt=0;
new_bmp=FALSE;
p  = edo_buffer+sizeof(ed_sdd)+sizeof(ed_fdd)+sizeof(ed_fd);
pe = &edo_buffer[size];
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
                ed_rect[i].top    = bm.row;
                ed_rect[i].left   = bm.col;
                ed_rect[i].bottom = bm.height;
                ed_rect[i].right  = bm.width;
                            new_bmp=TRUE;
                            }
            break;
        default:
            memcpy(&vf,p,sizeof(vf));
            p+=sizeof(vf);
            //if( vf.code<30  )
              //  return FALSE;
            ed_vers[i].Alt[ed_vers[i].lnAltCnt].Code = vf.code;
            ed_vers[i].Alt[ed_vers[i].lnAltCnt].Prob =
                (vf.prob>1?vf.prob:254);
            if( !ed_vers[i].lnAltCnt )
                *str++ = vf.code;
            ed_vers[i].lnAltCnt++;
                        new_bmp=FALSE;
            if( vf.prob&1 )
                {
                i++;
                ed_vers[i].lnAltCnt=0;
                }
            break;
        }


    }
ed_nvers = i;
for(i=0;i<ed_nvers;i++)
    {
    if( ed_vers[i].lnAltCnt && ed_vers[i].Alt[0].Code=='~' )
        {
        ed_vers[i].lnAltCnt =  0;
        }
    }
*str=0;
return TRUE;
}

static void ed_init()
{
MED_file_end=ed_buffer;
MED_file_bound=MED_file_end+sizeof(ed_buffer);
}

Bool32 correct_cstr(CSTR_rast b,CSTR_rast e, int32_t *start)
{
CSTR_rast_attr  attr;
CSTR_rast       c;
UniVersions     vrs;
int32_t           i,j,n;
Bool32          change_rect;
uint16_t          loc_language=35535, loc_charset=35535 ;

for(c=b,i=0;c && c!=e ;c=CSTR_GetNext(c))
    {
    CSTR_GetAttr(c,&attr);
    if( (attr.flg & (CSTR_f_let|CSTR_f_bad) ) )
        {
        if( loc_language==35535 )
            loc_language = attr.language;
        CSTR_GetCollectionUni(c,&vrs);
        if( loc_charset==35535 && (attr.flg&CSTR_f_let) )
            loc_charset = vrs.Alt[0].Charset;
        }
    }
for(c=b,i=*start;c && c!=e && i<ed_nvers;c=CSTR_GetNext(c),i++)
    {
    CSTR_GetCollectionUni(c,&vrs);
    CSTR_GetAttr(c,&attr);
    n = ed_vers[i].lnAltCnt;
    change_rect = ( ed_rect[i].top   !=attr.row  ||
                    ed_rect[i].bottom!=attr.h    ||
                    ed_rect[i].left  !=attr.col  ||
                    ed_rect[i].right !=attr.w );
    if( ed_vers[i].Alt[0].Code==SS_POS_HALF_SPACE &&  vrs.Alt[0].Code[0]==' ')
        {
        vrs.lnAltCnt=1;
        vrs.Alt[0].Code[0]=' ';
        vrs.Alt[0].Code[1]=0;
        vrs.Alt[0].Prob   =ed_vers[i].Alt[0].Prob;
        vrs.Alt[0].Liga   =ed_vers[i].Alt[0].Code;
        vrs.Alt[0].Method =REC_METHOD_DIC;
        vrs.Alt[0].Charset=(uchar)loc_charset;
        if( change_rect )
            vrs.Alt[0].Info=0;
        }
    else if( ed_vers[i].Alt[0].Code==SS_NEG_HALF_SPACE &&  vrs.Alt[0].Code[0]==0 && vrs.lnAltCnt==1 )
        {
        vrs.lnAltCnt=1;
        vrs.Alt[0].Code[0]=0;
        vrs.Alt[0].Prob   =ed_vers[i].Alt[0].Prob;
        vrs.Alt[0].Liga   =ed_vers[i].Alt[0].Code;
        vrs.Alt[0].Method =REC_METHOD_DIC;
        vrs.Alt[0].Charset=(uchar)loc_charset;
        if( change_rect )
            vrs.Alt[0].Info=0;
        }
    else
        {
        for(j=0;j<n;j++)
            {
            vrs.Alt[j].Code[0]=ed_vers[i].Alt[j].Code;
            vrs.Alt[j].Code[1]=0;
            vrs.Alt[j].Prob   =ed_vers[i].Alt[j].Prob;
            vrs.Alt[j].Liga   =stdAnsiToAscii(ed_vers[i].Alt[j].Code);
            vrs.Alt[j].Method =REC_METHOD_DIC;
            vrs.Alt[j].Charset=(uchar)loc_charset;
            if( change_rect )
                vrs.Alt[j].Info=0;
            }
        vrs.lnAltCnt=n;
        }
    CSTR_StoreCollectionUni(c,&vrs);

    attr.flg_spell = CSTR_fa_spell_correct;
    attr.language=(uchar)loc_language;
    CSTR_SetAttr(c,&attr);
    }
*start=i;
return TRUE;
}

Bool32 restruct_cstr(CSTR_rast b,CSTR_rast e, Bool32 cd)
{
CSTR_rast_attr  attr={0}, battr;
CSTR_rast       c, cp;
UniVersions     vrs={0};
int32_t           i,j,n,mincol,maxcol,minrow,maxrow,avwid ;
int32_t           mincolr,maxcolr,minrowr,maxrowr,avwidr ;
CSTR_line       line;
uint16_t          loc_language=35535, loc_charset=35535, loc_battr=35535 ;

CSTR_GetAttr(b,&battr);
line=CSTR_GetRasterLine(b);
cp=CSTR_GetPrevRaster(b,255);
minrowr=mincolr=minrow=mincol=32000;
maxrowr=maxcolr=maxrow=maxcol=-16000;
if( cd )
    b=CSTR_GetNext(b);
for(c=b,i=0;c && c!=e ;)
    {
    CSTR_GetAttr(c,&attr);
    if( attr.flg & (CSTR_f_let|CSTR_f_bad|CSTR_f_dust|CSTR_f_punct) )
        {
        if( attr.col+attr.w>maxcol ) maxcol = attr.col+attr.w;
        if( attr.col<mincol )        mincol = attr.col;
        if( attr.row+attr.h>maxrow ) maxrow = attr.row+attr.h;
        if( attr.row<minrow )        minrow = attr.row;

        if( attr.r_col+attr.w>maxcolr ) maxcolr = attr.r_col+attr.w;
        if( attr.r_col<mincolr )        mincolr = attr.r_col;
        if( attr.r_row+attr.h>maxrowr ) maxrowr = attr.r_row+attr.h;
        if( attr.r_row<minrowr )        minrowr = attr.r_row;
        }
    if( (attr.flg & (CSTR_f_let|CSTR_f_bad) ) )
        {
        if( loc_language==35535 )
            loc_language = attr.language;
        if( loc_battr==35535 )
            battr=attr;
        CSTR_GetCollectionUni(c,&vrs);
        if( loc_charset==35535 && (attr.flg&CSTR_f_let))
            loc_charset = vrs.Alt[0].Charset;
        }
    c=CSTR_DelRaster(c);
    }
avwid = (maxcol-mincol)/ed_nvers;
avwidr = (maxcolr-mincolr)/ed_nvers;
for(i=0; i<ed_nvers &&
         ed_vers[i].lnAltCnt && ed_vers[i].Alt[0].Code==' ';i++);
if( !i && cd )
    {
    i++;
    cp=CSTR_GetNext(cp);
    }
for(c=cp;i<ed_nvers;i++)
    {
    c=CSTR_InsertRaster(c); // c - prev raster
    if( !c )
        continue;
    attr.r_row  = (int16_t)minrowr   ;
    attr.r_col  = (int16_t)(mincolr+i*avwidr)  ;
    attr.row    = (int16_t)minrow   ;
    attr.col    = (int16_t)(mincol+i*avwid)  ;
    attr.h      = (int16_t)(maxrow-minrow);
    attr.w      = (int16_t)avwid ;
    attr.font   = battr.font       ;
    attr.keg    = battr.keg        ;
    attr.language=(uchar)loc_language;
    attr.flg_spell = CSTR_fa_spell_restruct;
    n           = ed_vers[i].lnAltCnt;
    attr.flg    = n ? CSTR_f_let : CSTR_f_bad;
    if( ed_vers[i].Alt[0].Code==SS_POS_HALF_SPACE )
        {
        vrs.lnAltCnt=1;
        vrs.Alt[0].Code[0]=' ';
        vrs.Alt[0].Code[1]=0;
        vrs.Alt[0].Prob   =ed_vers[i].Alt[0].Prob;
        vrs.Alt[0].Liga   =ed_vers[i].Alt[0].Code;
        vrs.Alt[0].Method =REC_METHOD_DIC;
        vrs.Alt[0].Charset=(uchar)loc_charset;
        vrs.Alt[0].Info=0;
        }
    else if( ed_vers[i].Alt[0].Code==SS_NEG_HALF_SPACE  )
        {
        vrs.lnAltCnt=1;
        vrs.Alt[0].Code[0]=0;
        vrs.Alt[0].Prob   =ed_vers[i].Alt[0].Prob;
        vrs.Alt[0].Liga   =ed_vers[i].Alt[0].Code;
        vrs.Alt[0].Method =REC_METHOD_DIC;
        vrs.Alt[0].Charset=(uchar)loc_charset;
        vrs.Alt[0].Info=0;
        }
    else
        {
        for(j=0;j<n;j++)
            {
            vrs.Alt[j].Code[0]= ed_vers[i].Alt[j].Code;
            vrs.Alt[j].Code[1]= 0;
            vrs.Alt[j].Liga   = stdAnsiToAscii(ed_vers[i].Alt[j].Code);
            vrs.Alt[j].Prob   = ed_vers[i].Alt[j].Prob;
            vrs.Alt[j].Method = REC_METHOD_DIC;
            vrs.Alt[j].Charset= (uchar)loc_charset;
            vrs.Alt[j].Info   = 0;
            }
        vrs.lnAltCnt=n;
        }
    vrs.lnAltMax=REC_MAX_VERS;

    CSTR_StoreCollectionUni(c,&vrs);
    CSTR_SetAttr(c,&attr);
    }
return TRUE;
}

Bool32 ed_make_word(CSTR_rast b, CSTR_rast e,uchar *language)
{
uchar           *l,p;
CSTR_rast       c;
UniVersions     uni;
int16_t           k,i,n,h,wb, nlig, nl;
CSTR_rast_attr  a;
int16_t           top,bottom,left,right;


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
        *language=a.language;
        }
    }
h = bottom-top-1;
wb= (right-left-1+7)/8;
//    start sheet_disk_descr
    ed_sdd.code     =0x0a;
    ed_sdd.descr_lth=0x26;
    ed_sdd.resolution=300;
    ed_write((uchar*)&ed_sdd,sizeof(ed_sdd));
// start fragm_disk_descr
    ed_fdd.code=0x0b;
    ed_fdd.language = *language;
    ed_fdd.height   = h;
    ed_fdd.w_width  = wb;
    ed_fdd.kegl     = 10;
    ed_write((uchar*)&ed_fdd,sizeof(ed_fdd));

 // start fragm_disk
    ed_fd.code=0x0b;
    ed_write((uchar*)&ed_fd,sizeof(ed_fd));

 for(c=b;c&&c!=e;c=CSTR_GetNext(c))
    {
    CSTR_GetAttr(c,&a);
    // start bitmap ref
    bmr.code    =0;
    bmr.pos     =a.cpos;
    bmr.row     =a.r_row;
    bmr.col     =a.r_col;
    bmr.width   =a.w;
    bmr.height  =a.h;
    CSTR_GetCollectionUni(c,&uni);
    n=(int16_t)uni.lnAltCnt;
    if(n>7) n=7;
    if(n<1) n=1;

    if( n && memchr(ed_half_spaces,uni.Alt[0].Liga,2) )
        {
        ed_write((uchar*)&bmr,sizeof(bmr));
        p=uni.Alt[0].Prob;
        if( p&1 )   p--;
        p++;
        ed_vrs.code = uni.Alt[0].Liga;
        ed_vrs.prob = p;
        ed_write((uchar*)&ed_vrs,sizeof(ed_vrs));
        continue;  // half spaces for spelling analisys
        }
    nlig = strlen(uni.Alt[0].Code);
    if( uni.lnAltCnt>1 && nlig>1 )
        return FALSE;
    for(nl=0;nl<nlig;nl++)
    {
    ed_write((uchar*)&bmr,sizeof(bmr));

    for(k=i=0;k<n;)
        {
        p=uni.Alt[i].Prob;
        if( p&1 )   p--;
        if( !p  )   p=2;    // all odd propability
        for(l = &uni.Alt[i++].Code[nl];*l && k<n;l++,k++)
            { // for ligas : strlen(uni.Alt[i].Code)>1
            if( k==n-1 )
                p = p+1; // last even propability
            ed_vrs.code = *l;
            ed_vrs.prob =  p;
            ed_write((uchar*)&ed_vrs,sizeof(ed_vrs));
            }
        }
     }
  }

return TRUE;
}

Bool32 ed_add_word(CSTR_rast b, CSTR_rast e,uchar *language)
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
        *language=a.language;
        }
    }

for(c=b;c&&c!=e;c=CSTR_GetNext(c))
    {
    CSTR_GetAttr(c,&a);
    // start bitmap ref
    bmr.code    =0;
    bmr.pos     =a.cpos;
    bmr.row     =a.r_row;
    bmr.col     =a.r_col;
    bmr.width   =a.w;
    bmr.height  =a.h;
    CSTR_GetCollectionUni(c,&uni);
    n=(int16_t)uni.lnAltCnt;
    if(n>7) n=7;
    if(n<1) n=1;

    if( n && memchr(ed_half_spaces,uni.Alt[0].Liga,2) )
        {
        ed_write((uchar*)&bmr,sizeof(bmr));
        p=uni.Alt[0].Prob;
        if( p&1 )   p--;
        p++;
        ed_vrs.code = uni.Alt[0].Liga;
        ed_vrs.prob = p;
        ed_write((uchar*)&ed_vrs,sizeof(ed_vrs));
        continue;  // half spaces for spelling analisys
        }
    nlig = strlen(uni.Alt[0].Code);
    if( uni.lnAltCnt>1 && nlig>1 )
        return FALSE;
    for(nl=0;nl<nlig;nl++)
    {
    ed_write((uchar*)&bmr,sizeof(bmr));

    for(k=i=0;k<n;)
        {
        p=uni.Alt[i].Prob;
        if( p&1 )   p--;
        if( !p  )   p=2;    // all odd propability
        for(l = &uni.Alt[i++].Code[nl];*l && k<n;l++,k++)
            { // for ligas : strlen(uni.Alt[i].Code)>1
            if( k==n-1 )
                p = p+1; // last even propability
            ed_vrs.code = *l;
            ed_vrs.prob =  p;
            ed_write((uchar*)&ed_vrs,sizeof(ed_vrs));
            }
        }
     }
  }

return TRUE;
}


static CSTR_rast rpstr_next_word(CSTR_rast cs)
{
CSTR_rast       c=cs;
CSTR_rast_attr  attr;
UniVersions     vers;

CSTR_GetAttr(c,&attr);
if( attr.flg==CSTR_f_fict && attr.col==0x7fff )
    return (CSTR_rast)0; // last rast
do{
  c=CSTR_GetNext(c);
  CSTR_GetAttr(c,&attr);
  CSTR_GetCollectionUni(c,&vers);
  }while( c && memchr(ed_left_limit_word,vers.Alt[0].Code[0],sizeof(ed_left_limit_word)) &&
          !(attr.flg&CSTR_f_fict) );

return (attr.flg&CSTR_f_fict)?(CSTR_rast)0:c;
}

static CSTR_rast rpstr_end_word(CSTR_rast cs,uchar *str,Bool32 *hsp)
{
CSTR_rast       c=cs, nc;
CSTR_rast_attr  attr, nattr;
int16_t           i;
UniVersions     vers, nuni;
Bool32 halfspace=FALSE;	// 01.06.2001 E.P.

*hsp = FALSE;
CSTR_GetCollectionUni(c,&vers);
CSTR_GetAttr(c,&attr);
language = attr.language;
strcpy(str,vers.Alt[0].Code);
i=strlen(vers.Alt[0].Code);
do{
  c=CSTR_GetNext(c);
  if( !c )
    break;
  CSTR_GetCollectionUni(c,&vers);
  if( vers.lnAltCnt && memchr(ed_half_spaces,vers.Alt[0].Liga,2) )
    {
    i++;
    *hsp=TRUE;
    strcat(str,vers.Alt[0].Liga==SS_NEG_HALF_SPACE?"\x1e":"\x1f");
	halfspace=TRUE;	// 01.06.2001 E.P.
    continue;
    }

  if( !(c && !memchr(ed_right_limit_word,vers.Alt[0].Code[0],
					sizeof(ed_right_limit_word)-1) &&
          !(attr.flg&CSTR_f_fict) && i<MAX_LEN_WORD-1))
    {
    int32_t rus_break_ge=FALSE;
    if( vers.Alt[0].Code[0]=='>' )
        {
        if( (nc=CSTR_GetNext(c))!=0 )
            {
            CSTR_GetCollectionUni(nc,&nuni);
            if( nuni.lnAltCnt && nuni.Alt[0].Code[0]==(uchar)'к' )
                {
                CSTR_GetAttr(nc,&nattr);
                CSTR_GetAttr(c,&attr);
                if( attr.col+attr.w>nattr.col-2 )
                    rus_break_ge=TRUE;
                }
            }
        }
    if( !rus_break_ge )
        break;
    }

  strcat(str,vers.Alt[0].Code);
  i+=strlen(vers.Alt[0].Code);
  CSTR_GetAttr(c,&attr);
  halfspace=FALSE;	// 01.06.2001 E.P.
  }while(1);

// Отрезать полупробел на конце слова. 01.06.2001 E.P.
if (halfspace)
	{
	str[strlen(str)-1] = 0;
	}

c=c ? c : CSTR_GetLastRaster(CSTR_GetRasterLine(cs));
return c;
}

static rpstr_is_digital(uchar w)
{
return (w>='0' && w<='9' || strchr("~",w) );
}

// for russian and english ansi codes
static rpstr_is_upper(uchar w)
{
return (w>='A' && w<='Z' || w>=(uchar)'А' && w<=(uchar)'Я' );
}

static rpstr_is_lower(uchar w)
{
return (w>='a' && w<='z' || w>=(uchar)'а' && w<=(uchar)'я' );
}

static rpstr_to_upper(uchar w)
{
if( w>='a' && w<='z'  )
    return (uchar)(w-32);
if( w>=(uchar)'а' && w<=(uchar)'я' )
    return (uchar)(w-32);
return w;
}

static rpstr_to_lower(uchar w)
{
if( w>='A' && w<='Z'  )
    return (uchar)(w+32);
if( w>=(uchar)'А' && w<=(uchar)'Я' )
    return (uchar)(w+32);
return w;
}

Bool32  rpstr_correct_case_old(uchar *in,uchar *out,
        int32_t lenin, int32_t lenout,UniVersions *uvs)
{
int32_t   nvers,i,n=(lenin<lenout)?lenin:lenout;
Bool32  ui,uo, lo, li;

Bool32 isUnknown=FALSE;
uchar *inIni=in;  // save for second pass
uchar *outIni=out;

if( lenin!=lenout )
{
	// Nick 27.01.2001
 nvers = rpstr_case_notequal( in, out, lenin, lenout, uvs );
 return (nvers!=0);
//    return FALSE;
}

for(nvers=i=0;i<n;i++,in++,out++)
    {
    if( *in!=*out )
        {
        ui=rpstr_is_upper(*in);
        lo=rpstr_is_lower(*out);
        li=rpstr_is_lower(*in);
        uo=rpstr_is_upper(*out);
        if( ui && lo )
            {
            *out = rpstr_to_upper(*out);
            uvs[i].Alt[0].Code[0]=*out;
            uvs[i].Alt[0].Code[1]=0;
            uvs[i].Alt[0].Liga=stdAnsiToAscii(uvs[i].Alt[0].Code[0]);
            nvers++;
            }
        else if( li && uo )
            {
            *out = rpstr_to_lower(*out);
            uvs[i].Alt[0].Code[0]=*out;
            uvs[i].Alt[0].Code[1]=0;
            uvs[i].Alt[0].Liga=stdAnsiToAscii(uvs[i].Alt[0].Code[0]);
            nvers++;
            }
        if( !ui && !lo) //Nick 27.01.2001
			isUnknown = TRUE;
        }
    }

if(isUnknown)  //Nick 27.01.2001
  nvers += rpstr_case_notequal( inIni, outIni, lenin, lenout, uvs );

return (nvers!=0);
}
/////////////////
Bool32  rpstr_correct_case(uchar *in,uchar *out,
        int32_t lenin, int32_t lenout,UniVersions *uvs, uchar cpos)
{
int32_t   nvers,i,n=(lenin<lenout)?lenin:lenout;
Bool32  ui,uo, lo, li;

Bool32 isUnknown = FALSE;
uchar *inIni=in;  // save for second pass
uchar *outIni=out;

if( lenin!=lenout )
{
// Nick 27.01.2001
 nvers = rpstr_case_notequal( in, out, lenin, lenout, uvs );
 return (nvers!=0);
//    return FALSE;
}

for(nvers=i=0;i<n;i++,in++,out++)
{
    if( *in!=*out )
	{
		if(i != 0 && i < n - 1)
		{
			ui = rpstr_is_upper(*(in - 1));
			uo = rpstr_is_upper(*(in + 1));
			li = rpstr_is_lower(*(in - 1));
			lo = rpstr_is_lower(*(in + 1));

			if(ui && uo)
			{// соседние буквы в верхнем регистре
				*out = rpstr_to_upper(*out);
				uvs[i].Alt[0].Code[0]=*out;
				uvs[i].Alt[0].Code[1]=0;
				uvs[i].Alt[0].Liga=stdAnsiToAscii(uvs[i].Alt[0].Code[0]);
				nvers++;
			}

			else if(li && lo)
			{// соседние буквы в нижнем регистре
				*out = rpstr_to_lower(*out);
				uvs[i].Alt[0].Code[0]=*out;
				uvs[i].Alt[0].Code[1]=0;
				uvs[i].Alt[0].Liga=stdAnsiToAscii(uvs[i].Alt[0].Code[0]);
				nvers++;
			}

			else  //Nick 27.01.2001
			 isUnknown = TRUE;
		}

		if(i == 0)
		{// первая буква
			if((cpos & CSTR_p_large) == CSTR_p_large)
			{
				*out = rpstr_to_upper(*out);
				uvs[i].Alt[0].Code[0]=*out;
				uvs[i].Alt[0].Code[1]=0;
				uvs[i].Alt[0].Liga=stdAnsiToAscii(uvs[i].Alt[0].Code[0]);
				nvers++;
			}
		}

		if(i == n - 1)
		{// последняя буква
			ui = rpstr_is_upper(*(in - 1));
			li = rpstr_is_lower(*(in - 1));

			if(ui)
			{
				*out = rpstr_to_upper(*out);
				uvs[i].Alt[0].Code[0]=*out;
				uvs[i].Alt[0].Code[1]=0;
				uvs[i].Alt[0].Liga=stdAnsiToAscii(uvs[i].Alt[0].Code[0]);
				nvers++;
			}

			if(li)
			{
				*out = rpstr_to_lower(*out);
				uvs[i].Alt[0].Code[0]=*out;
				uvs[i].Alt[0].Code[1]=0;
				uvs[i].Alt[0].Liga=stdAnsiToAscii(uvs[i].Alt[0].Code[0]);
				nvers++;
			}
		}
	}
}

if(isUnknown)  //Nick 27.01.2001
  nvers += rpstr_case_notequal( inIni, outIni, lenin, lenout, uvs );

return (nvers!=0);
}

Bool32 rpstr_alphabet_mixed( uchar *s )
{
int32_t   n,d;
for(d=n=0,s++;*s;s++,n++)
    {
    if( strchr("-+=*/$&%1234567890",*s) )
        d++;
    else if( SS_NEG_HALF_SPACE==*s )
        d++;
    else if( SS_POS_HALF_SPACE==*s )
        d++;
    }
return (d && d!=n);
}

Bool32 rpstr_alphabet_check(uchar *s)
{
int32_t   n,d,r,l=strlen(s)-1,hsp;
for(r=hsp=n=0,d=0;*s;s++,n++)
    {
    if( rpstr_is_digital(*s) )
        d++;
    else if( n && strchr("-+*/",*s) )
        d++;
    else if( n==0 && strchr("$",*s) )
        d++;
    else if( n==l && strchr("%$",*s) )
        d++;
    else if( SS_NEG_HALF_SPACE==*s )
        d++;
    else if( SS_POS_HALF_SPACE==*s )
        d++;

    if( strchr("VIXХ",*s) )
        r++;

    if( SS_NEG_HALF_SPACE==*s )
        hsp++;
    }
return( d!=n &&
	   // !(hsp*3>n && hsp>2 && n>3) &&  // Nick comment 13/08/01 - Enz_018
	   r!=n);
}

void rpstr_set_spell_flag(CSTR_rast b,CSTR_rast e,uchar flg_spell, uchar flg)
{
CSTR_rast_attr  attr;

	CSTR_GetAttr(b,&attr);
	attr.flg_spell = flg_spell;
	CSTR_SetAttr(b,&attr);
	for(;b && b!=e;b=CSTR_GetNext(b))
	{
		CSTR_GetAttr(b,&attr);
		attr.flg_spell = flg_spell;
		attr.flg      |= flg;
		if( (attr.flg&CSTR_f_solid)  && flg_spell==CSTR_fa_spell_reject )
			attr.flg -= CSTR_f_solid;
		CSTR_SetAttr(b,&attr);

		// Nick add 06.01.2002
		if( ( attr.flg & CSTR_f_space) && (flg_spell == CSTR_fa_spell_solid) )
		{
			UniVersions     vrs;

			CSTR_GetCollectionUni(b,&vrs);
			if( vrs.lnAltCnt > 0 && vrs.Alt[0].Liga == SS_POS_HALF_SPACE )
			{
				vrs.Alt[0].Liga = ' ';
				vrs.Alt[0].Prob = 254;
				CSTR_StoreCollectionUni(b,&vrs);
			}
		}
	}
	if( flg_spell&CSTR_fa_spell_nocarrying )
    {
		CSTR_GetAttr(e,&attr);
		if( attr.flg&CSTR_f_all )
            {
            attr.flg_spell = flg_spell;
            attr.flg      |= flg;
            CSTR_SetAttr(e,&attr);
            }
    }
return;
}


Bool32 rpstr_txt_spell(char * s,uchar lang)
{
int32_t                      Check = 0;
if( lang==LANG_ENGLISH && multy_language )
    { // second dict
    if( !RLING_CheckSecWord((pchar)s, &Check) )
        {
        sp_err=RLING_GetReturnString(RLING_GetReturnCode());
        return FALSE;
        }
    }
else
    { // first dict
    if( !RLING_CheckWord((pchar)s, &Check) )
        {
        sp_err=RLING_GetReturnString(RLING_GetReturnCode());
        return FALSE;
        }
    }

return (Check>0);
}

Bool32 is_last_word(CSTR_rast e)
{
CSTR_rast_attr  attr;
RecVersions     rv;

Bool32          is_dash=FALSE;
if( !e )
    return FALSE;
CSTR_GetAttr(e,&attr);
if( attr.flg==CSTR_f_fict )
    return FALSE;
for(; e  ; e=CSTR_GetNext(e))
    {
    CSTR_GetAttr(e,&attr);
    if( attr.flg&(CSTR_f_let|CSTR_f_bad) )
        return FALSE;
    CSTR_GetCollection(e,&rv);
    if( rv.lnAltCnt && strchr(dash,rv.Alt[0].Code) )
        is_dash=TRUE;
    }
return is_dash;
}

static void rpstr_cstr2word(CSTR_rast be,CSTR_rast en,uchar *str)
{
CSTR_rast c;
UniVersions     uvs;
*str='\0';
for(c=be;c&&c!=en;c=CSTR_GetNext(c))
        {
        CSTR_GetCollectionUni(c,&uvs);
        strcat(str, uvs.Alt[0].Code);
        }

return;
}

static  int32_t rpstr_cstr2uni(CSTR_rast be,CSTR_rast en,UniVersions *uv, int32_t nlim)
{
CSTR_rast   c;
int32_t       nv;

	for(nv=0,c=be;nv<nlim && c&&c!=en;c=CSTR_GetNext(c),nv++)
    {
        CSTR_GetCollectionUni(c,&uv[nv]);

		if( uv[nv].lnAltCnt<= 0 )
		{
			uv[nv].lnAltCnt = 1;
			uv[nv].Alt[0].Code[0] = '~';
			uv[nv].Alt[0].Code[1] = 0;
			uv[nv].Alt[0].Liga = '~';
			uv[nv].Alt[0].Prob = 1;
			uv[nv].Alt[0].Charset = 0;
			uv[nv].Alt[0].Info = 0;
			uv[nv].Alt[0].Method = 0;
		}
    }
return nv;
}


Bool32  rpstr_normal_spell(char *sec_wrd)
{
uint32_t          sizeout;
int32_t                   Check = 0;
if( !language && multy_language )
    { // second dict
    if( !RLING_CheckSecED((pchar)ed_buffer,(pchar)edo_buffer,
            (uint32_t)(MED_file_end-ed_buffer), &sizeout,
            &Check) )
        {
        sp_err=RLING_GetReturnString(RLING_GetReturnCode());
        return FALSE;
        }
    }
else
    { // first dict
    if( !RLING_CheckED((pchar)ed_buffer,(pchar)edo_buffer,
            (uint32_t)(MED_file_end-ed_buffer), &sizeout,
            &Check) )
        {
        sp_err=RLING_GetReturnString(RLING_GetReturnCode());
        return FALSE;
        }
    }
return ed_exclude_to_vers(sizeout, sec_wrd);
//return TRUE;
}

static uchar non_letters[]="«»()\x1f\x1e,.!?";
Bool32  rpstr_get_solid(CSTR_rast rus, CSTR_rast ruse)
{
CSTR_rast       r;
CSTR_rast_attr  a;
UniVersions     u;

for(r=rus;r && r!=ruse;r=CSTR_GetNext(r))
    {
    CSTR_GetAttr(r,&a);
    CSTR_GetCollectionUni(r,&u);
    if( !(a.flg&CSTR_f_solid) &&
        u.lnAltCnt && !strchr(non_letters,u.Alt[0].Code[0]) &&
        !strchr(ed_half_spaces,u.Alt[0].Liga) )
        return FALSE;
    }
return TRUE;
}

int32_t   size_short_language(uchar language)
{
int32_t   s;
switch( language )
    {
    case    LANG_GERMAN:
        s=2;
        break;
    case    LANG_RUSSIAN:
    case    LANG_ENGLISH:
    case    LANG_RUSENG:
    default:
        s=3;
        break;
    }
return s;
}

int32_t   size_short_language_aux(uchar language)
{
int32_t   s;
switch( language )
    {
    case    LANG_GERMAN:
        s=3;
        break;
    case    LANG_RUSSIAN:
    case    LANG_ENGLISH:
    case    LANG_RUSENG:
    default:
        s=3;
        break;
    }
return s;
}

static uchar CodePages[LANG_TOTAL]={
CSTR_ANSI_CHARSET            , // LANG_ENGLISH		0
CSTR_ANSI_CHARSET            , // LANG_GERMAN		1
CSTR_ANSI_CHARSET            , // LANG_FRENCH		2
CSTR_RUSSIAN_CHARSET         , // LANG_RUSSIAN		3
CSTR_ANSI_CHARSET            , // LANG_SWEDISH		4
CSTR_ANSI_CHARSET            , // LANG_SPANISH		5
CSTR_ANSI_CHARSET            , // LANG_ITALIAN		6
CSTR_RUSSIAN_CHARSET         , // LANG_RUSENG		7
CSTR_RUSSIAN_CHARSET         , // LANG_UKRAINIAN	8
CSTR_RUSSIAN_CHARSET         , // LANG_SERBIAN		9
CSTR_EASTEUROPE_CHARSET      , // LANG_CROATIAN		10
CSTR_EASTEUROPE_CHARSET      , // LANG_POLISH		11
CSTR_ANSI_CHARSET            , // LANG_DANISH		12
CSTR_ANSI_CHARSET            , // LANG_PORTUGUESE	13
CSTR_ANSI_CHARSET            , // LANG_DUTCH		14
CSTR_ANSI_CHARSET            , // LANG_DIG			15
CSTR_RUSSIAN_CHARSET         , // LANG_UZBEK		16
CSTR_RUSSIAN_CHARSET         , // LANG_KAZ			17
CSTR_RUSSIAN_CHARSET         , // LANG_KAZ_ENG		18
CSTR_EASTEUROPE_CHARSET      , // LANG_CZECH		19
CSTR_EASTEUROPE_CHARSET      , // LANG_ROMAN		20
CSTR_EASTEUROPE_CHARSET      , // LANG_HUNGAR		21
CSTR_RUSSIAN_CHARSET         , // LANG_BULGAR		22
CSTR_EASTEUROPE_CHARSET	     , // LANG_SLOVENIAN    23
BALTIC_CHARSET				 , // LANG_LATVIAN	    24
BALTIC_CHARSET				 , // LANG_LITHUANIAN   25
BALTIC_CHARSET				 , // LANG_ESTONIAN	    26
TURKISH_CHARSET				   // LANG_TURKISH		27
};

static int Lang_Console(char *text, uchar lang)
{
char    buf[1024];
LDPUMA_SetConsoleProperty(0,0,0,0,0,0,0,CodePages[lang],NULL);
strcpy(buf,text);
strcat(buf,"\n");
return LDPUMA_Console(buf);
}

int Snap_Console(char *text)
{
return Lang_Console(text,language);
}

int Snap_ConsoleLang(char *text,uchar lang)
{
return Lang_Console(text,lang);
}

int32_t uni_correct_check(CSTR_rast b,CSTR_rast e, int32_t *start)
{
CSTR_rast_attr  attr;
CSTR_rast       c;
UniVersions     vrs;
int32_t           i,n,pen,nall;

for(vrs=rejo_vers[*start],i=*start,c=b,nall=pen=0;
    c && c!=e && i<nrejo_vers;c=CSTR_GetNext(c),i++,nall++)
    {
    CSTR_GetCollectionUni(c,&vrs);
    CSTR_GetAttr(c,&attr);
    n = rejo_vers[i].lnAltCnt;
    if( vrs.lnAltCnt &&
        strcmp(rejo_vers[i].Alt[0].Code,vrs.Alt[0].Code) ) // символ изменилс
        pen++;
    }
if( nall==1 )
    {
    i=*start;
    c=b;
    CSTR_GetCollectionUni(c,&vrs);
    if( strchr("иИ",vrs.Alt[0].Code[0]) &&
        strchr("Йй",rejo_vers[i].Alt[0].Code[0]) )
        pen++;
    }
*start=nall;
return pen;
}

Bool32 uni_correct_cstr(CSTR_rast b,CSTR_rast e, int32_t *start, Bool32 enable_test, Bool32 enable_take)
{
CSTR_rast_attr  attr;
CSTR_rast       c;
UniVersions     vrs;
int32_t           i,j,n;
uint16_t          loc_language=35535, loc_charset=35535 ;

for(c=b,i=0;c && c!=e ;c=CSTR_GetNext(c))
    {
    CSTR_GetAttr(c,&attr);
    if( (attr.flg & (CSTR_f_let|CSTR_f_bad) ) )
        {
        if( loc_language==35535 )
            loc_language = attr.language;
        CSTR_GetCollectionUni(c,&vrs);
        if( loc_charset==35535 && (attr.flg&CSTR_f_let) )
            loc_charset = vrs.Alt[0].Charset;
        }
    }
if( enable_test )
    {
    for(vrs=rejo_vers[*start],i=*start,c=b;
        c && c!=e && i<nrejo_vers;c=CSTR_GetNext(c),i++)
        {
        CSTR_GetCollectionUni(c,&vrs);
        CSTR_GetAttr(c,&attr);
        n = rejo_vers[i].lnAltCnt;
        if( attr.flg & CSTR_f_space )
            return FALSE;
        if( vrs.lnAltCnt && (strchr("|",rejo_vers[i].Alt[0].Code[0])) )
            return FALSE;
        if( vrs.lnAltCnt &&
            strcmp(rejo_vers[i].Alt[0].Code,vrs.Alt[0].Code) && // символ изменилс
            ((attr.RecogHistory&CSTR_hi_fon) && vrs.Alt[0].Prob>220 || // символ с хороше шрифтовой оценкой
            (attr.RecogHistory&CSTR_hi_3x5) && vrs.Alt[0].Prob>246)&&  // символ с хорошей оценкой 3х5
            !(strchr("ЙйиИ",vrs.Alt[0].Code[0])&&strchr("иИЙй",rejo_vers[i].Alt[0].Code[0]))&&
            !(attr.keg<18 && strchr("зэ",vrs.Alt[0].Code[0])&&strchr("зэ",rejo_vers[i].Alt[0].Code[0]))&&
            !(strchr("ГгТт",vrs.Alt[0].Code[0])&&strchr("гГтТ",rejo_vers[i].Alt[0].Code[0])))
            // образы "ЙйиИ" и "ГгТт" переходят друг в друга, несмотря на оценки
            {
            return FALSE;
            }
        }
    }
if( enable_take )
    {
    for(vrs=rejo_vers[*start],i=*start,c=b;
        c && c!=e && i<nrejo_vers;c=CSTR_GetNext(c),i++)
        {
        CSTR_GetCollectionUni(c,&vrs);
        CSTR_GetAttr(c,&attr);
        n = rejo_vers[i].lnAltCnt;
        if( rejo_vers[i].Alt[0].Code[0]==SS_POS_HALF_SPACE &&  vrs.Alt[0].Code[0]==' ')
            {
            vrs.lnAltCnt=1;
            vrs.Alt[0].Code[0]=' ';
            vrs.Alt[0].Code[1]=0;
            vrs.Alt[0].Prob   =rejo_vers[i].Alt[0].Prob;
            vrs.Alt[0].Liga   =SS_POS_HALF_SPACE;
            vrs.Alt[0].Method =REC_METHOD_DIC;
            vrs.Alt[0].Charset=(uchar)loc_charset;
            vrs.Alt[0].Info=0;
            }
        else if( rejo_vers[i].Alt[0].Code[0]==SS_NEG_HALF_SPACE &&  vrs.Alt[0].Code[0]==0 && vrs.lnAltCnt==1 )
            {
            vrs.lnAltCnt=1;
            vrs.Alt[0].Code[0]=0;
            vrs.Alt[0].Prob   =rejo_vers[i].Alt[0].Prob;
            vrs.Alt[0].Liga   =SS_NEG_HALF_SPACE;
            vrs.Alt[0].Method =REC_METHOD_DIC;
            vrs.Alt[0].Charset=(uchar)loc_charset;
            vrs.Alt[0].Info=0;
            }
        else
            {
            for(j=0;j<n;j++)
                {
                strcpy(vrs.Alt[j].Code,rejo_vers[i].Alt[j].Code);
                vrs.Alt[j].Prob   =rejo_vers[i].Alt[j].Prob;
                vrs.Alt[j].Liga   =stdAnsiToAscii(rejo_vers[i].Alt[j].Code[0]);
                vrs.Alt[j].Method =REC_METHOD_DIC;
                vrs.Alt[j].Charset=(uchar)loc_charset;
                vrs.Alt[j].Info=0;
                }
            vrs.lnAltCnt=n;
            }
        CSTR_StoreCollectionUni(c,&vrs);

        attr.flg_spell = CSTR_fa_spell_correct;
        attr.language=(uchar)loc_language;
        CSTR_SetAttr(c,&attr);
        }
    }
*start=i;
return TRUE;
}

Bool32 rec_correct_cstr(CSTR_rast b,CSTR_rast e, Bool32 ret_space, Bool32 test_cursive)
{
CSTR_rast_attr  attr;
CSTR_rast       c;
UniVersions     vrs;
int32_t           i,n;
Bool32          iscursive;

for(iscursive=i=0,c=b;
    c && c!=e && i<ed_nvers;c=CSTR_GetNext(c))
    {
    CSTR_GetAttr(c,&attr);
    iscursive += (attr.font&CSTR_fp_it);
    }

for(i=0,c=b;
    c && c!=e && i<ed_nvers;c=CSTR_GetNext(c),i++)
    {
    CSTR_GetCollectionUni(c,&vrs);
    CSTR_GetAttr(c,&attr);
    n = ed_vers[i].lnAltCnt;
    if( attr.flg & CSTR_f_space )
        return ret_space;
    if( vrs.lnAltCnt && ('|'==ed_vers[i].Alt[0].Code) )
        return FALSE;
    if( vrs.lnAltCnt &&
        ed_vers[i].Alt[0].Code!=vrs.Alt[0].Code[0] && // символ изменилс
        ((attr.RecogHistory&CSTR_hi_fon) && vrs.Alt[0].Prob>220 || // символ с хороше шрифтовой оценкой
        (attr.RecogHistory&CSTR_hi_3x5) && vrs.Alt[0].Prob>246)  // символ с хорошей оценкой 3х5
        )
        {
        if( !(strchr("ЙйиИ",vrs.Alt[0].Code[0])&&strchr("иИЙй",ed_vers[i].Alt[0].Code))&&
            !(strchr("сСеоО",vrs.Alt[0].Code[0])&&strchr("сСеоО",ed_vers[i].Alt[0].Code))&&
            !(strchr("il1",vrs.Alt[0].Code[0])&&strchr("il1",ed_vers[i].Alt[0].Code))&&
            !(strchr("ГгТт",vrs.Alt[0].Code[0])&&strchr("гГтТ",ed_vers[i].Alt[0].Code)))
            {
            if(  test_cursive && iscursive &&
                (strchr("ве",vrs.Alt[0].Code[0])&&strchr("ве",ed_vers[i].Alt[0].Code)||
                 strchr("кхн",vrs.Alt[0].Code[0])&&strchr("кхн",ed_vers[i].Alt[0].Code)) )
                 continue;
            return FALSE;
            }
        }
    }

return TRUE;
}

CSTR_rast   exist_pos_half(CSTR_rast  b,CSTR_rast  e)
{
UniVersions uvs;
for(;b!=e;b=CSTR_GetNext(b))
    {
    CSTR_GetCollectionUni(b,&uvs);
    if( uvs.lnAltCnt && uvs.Alt[0].Liga==SS_POS_HALF_SPACE )
        return b;
    }
return 0;
}


static  char    double_eng[]="ETYOPAHKXCBMeyuopaxcnr";
static  char    double_rus[]="ЕТУОРАНКХСВМеуиорахспг";
//static  char    double_rus[]="…’“ЋђЂЌЉ•‘‚ЊҐгЁ®а ебЇ";
Bool32 rpstr_double_word(CSTR_rast   beg,    CSTR_rast   end,
                          uchar lang)
{
CSTR_rast_attr  a;
CSTR_rast       r;
UniVersions     u;
char           *arr1, *arr2, *p;

if( lang==LANG_RUSSIAN )
    {
    arr1 = double_rus;
    arr2 = double_eng;
    }
else
    {
    arr2 = double_rus;
    arr1 = double_eng;
    }

for(r=beg;r&&r!=end;r=CSTR_GetNext(r))
    {
    CSTR_GetAttr(r,&a);
    CSTR_GetCollectionUni(r,&u);
    if( (a.flg&CSTR_f_let) && u.lnAltCnt )
        {
        p = strchr(arr1,u.Alt[0].Code[0]);
        if( !p )
            return FALSE;
        }
    }
return TRUE;
}

void rpstr_correct_ruseng(CSTR_rast   beg,    CSTR_rast   end,
                          uchar lang)
{
CSTR_rast_attr  a;
CSTR_rast       r;
int32_t           i, n;
UniVersions     u,u1;
char           *arr1, *arr2, *p;
uchar           lang1, charset=CSTR_RUSSIAN_CHARSET;

if( lang==LANG_RUSSIAN )
    {
    arr1 = double_rus;
    arr2 = double_eng;
    lang1=LANG_ENGLISH;
    }
else
    {
    arr2 = double_rus;
    arr1 = double_eng;
    lang1=LANG_RUSSIAN;
    }

for(r=beg;r&&r!=end;r=CSTR_GetNext(r))
    {
    CSTR_GetAttr(r,&a);
    CSTR_GetCollectionUni(r,&u);
    u1=u;
    for(n=i=0;i<u.lnAltCnt;i++)
        {
        p = strchr(arr1,u.Alt[i].Code[0]);
        if( p )
            {
            u.Alt[i].Code[0]= arr2[p-arr1];
            u.Alt[i].Code[1]= 0;
            u.Alt[i].Liga   =stdAnsiToAscii(u.Alt[i].Code[0]);
            u.Alt[i].Charset=charset;
            u1.Alt[n]=u.Alt[i];
            n++;
            }
        u1.lnAltCnt=n;
        }


    CSTR_StoreCollectionUni(r,&u);
    a.language=lang1;
    CSTR_SetAttr(r,&a);
    }

return;
}

void rpstr_correct_attr(CSTR_rast   beg,    CSTR_rast   end,
                        CSTR_rast   prevbeg,CSTR_rast   prevend)
{
CSTR_rast_attr  a;
Bool32          cur,curp;
Bool32          bol,bolp;
CSTR_rast       r;
int32_t           it, bo, all;

for(it=all=bo=0,bol=cur=FALSE,r=beg;r&&r!=end;r=CSTR_GetNext(r))
    {
    CSTR_GetAttr(r,&a);
    all++;
    if( a.font&CSTR_fp_it )     it++;
    if( a.font&CSTR_fp_bold )   bo++;
    }
if( it==all )   cur=TRUE;
if( bo==all )   bol=TRUE;
for(it=all=bo=0,bolp=curp=FALSE,r=prevbeg;r&&r!=prevend;r=CSTR_GetNext(r))
    {
    CSTR_GetAttr(r,&a);
    all++;
    if( a.font&CSTR_fp_it )     it++;
    if( a.font&CSTR_fp_bold )   bo++;
    }
if( it==all )   curp=TRUE;
if( bo==all )   bolp=TRUE;

if( cur && !curp )
    {
    for(r=prevbeg;r&&r!=prevend;r=CSTR_GetNext(r))
        {
        CSTR_GetAttr(r,&a);
        if( a.font&CSTR_fp_str )
            a.font-= CSTR_fp_str ;
        if( !(a.font&CSTR_fp_it) )
            a.font += CSTR_fp_it;
        CSTR_SetAttr(r,&a);
        }
    r=prevend;
    CSTR_GetAttr(r,&a);
    if( a.font&CSTR_fp_str )
        a.font-= CSTR_fp_str ;
    if( !(a.font&CSTR_fp_it) )
        a.font += CSTR_fp_it;
    CSTR_SetAttr(r,&a);
    }

if( !cur && curp )
    {
    for(r=beg;r&&r!=end;r=CSTR_GetNext(r))
        {
        CSTR_GetAttr(r,&a);
        if( a.font&CSTR_fp_str )
            a.font-= CSTR_fp_str ;
        if( !(a.font&CSTR_fp_it) )
            a.font += CSTR_fp_it;
        CSTR_SetAttr(r,&a);
        }
    }
if( bol && !bolp || !bol && bolp )
    {
    for(r=beg;r&&r!=end;r=CSTR_GetNext(r))
        {
        CSTR_GetAttr(r,&a);
        if( a.font&CSTR_fp_light )
            a.font-= CSTR_fp_light ;
        if( !(a.font&CSTR_fp_bold) )
            a.font += CSTR_fp_bold;
        CSTR_SetAttr(r,&a);
        }
    for(r=prevbeg;r&&r!=prevend;r=CSTR_GetNext(r))
        {
        CSTR_GetAttr(r,&a);
        if( a.font&CSTR_fp_light )
            a.font-= CSTR_fp_light ;
        if( !(a.font&CSTR_fp_bold) )
            a.font += CSTR_fp_bold;
        CSTR_SetAttr(r,&a);
        }
    }
return;
}
/*
void save_frh( CSTR_rast beg,CSTR_rast end, int32_t num_ln)
{
CSTR_rast_attr  attr;
FILE       *fp=fopen("d:\\frh.txt","at");
if( fp )
    {
    UniVersions c;
    int32_t       i;
    if( num_ln<2 && num_ln!=-1)
        fprintf(fp,"New Line\n");
    fprintf(fp,"-- line %d \n", num_ln);
    while( beg && beg!=end)
        {
        CSTR_GetAttr(beg,&attr);
        fprintf(fp,"{%d,%d,%d,%d} ",attr.row,attr.col,attr.row+attr.h,attr.col+attr.w);
        CSTR_GetCollectionUni(beg,&c);
        if( c.lnAltCnt )
            {
            for(i=0;i<c.lnAltCnt;i++)
                fprintf(fp,"%s(%d) ",c.Alt[i].Code,c.Alt[i].Prob);
            }
        else
            {
            fprintf(fp,"%s(%d) ",c.Alt[0].Code,c.Alt[0].Prob);
            }
        fprintf(fp,"\n");
        beg = CSTR_GetNext(beg);
        }
    }
fclose(fp);
}*/

static void rpstr_correct_dash(CSTR_rast prevend)
{
UniVersions u;
CSTR_GetCollectionUni(prevend,&u);
if( u.lnAltCnt && u.Alt[0].Liga==dash[1]||u.Alt[0].Liga==dash[2] )
    {
    u.Alt[0].Code[0]=u.Alt[0].Liga=dash[0];
    u.Alt[0].Code[1]=0;
    CSTR_StoreCollectionUni(prevend,&u);
    }
return ;
}

static void rpstr_set_nocarrying(CSTR_rast prevend)
{
	CSTR_rast_attr  a;

	CSTR_GetAttr(prevend,&a);
	a.flg_spell = CSTR_fa_spell_nocarrying;
	CSTR_SetAttr(prevend,&a);
}

// по соглашению с Аликом
static void rpstr_temporary_kill_neghalf_segment(CSTR_rast start, CSTR_rast stop )
{
CSTR_rast       rst;
UniVersions     u;
for(rst = start;rst&&rst!=stop;)
    {
    CSTR_GetCollectionUni(rst,&u);
    if( u.lnAltCnt && u.Alt[0].Liga==SS_NEG_HALF_SPACE )
        rst=CSTR_DelRaster(rst);
    else
        rst=CSTR_GetNext(rst);
    }

return;
}

static void rpstr_temporary_kill_poshalf_segment(CSTR_rast start, CSTR_rast stop )
{
CSTR_rast       rst;
UniVersions     u;
stop=CSTR_GetNext(stop);
for(rst = start;rst&&rst!=stop;rst=CSTR_GetNext(rst))
    {
    CSTR_GetCollectionUni(rst,&u);
    if( u.lnAltCnt && u.Alt[0].Liga==SS_POS_HALF_SPACE )
        {
        u.Alt[0].Liga=' ';
        u.Alt[0].Code[0]=' ';
        u.Alt[0].Code[1]=0;
        CSTR_StoreCollectionUni(rst,&u);
        }
    }

return;
}

static void rpstr_temporary_kill_poshalf(CSTR_line ln )
{
rpstr_temporary_kill_poshalf_segment(CSTR_GetFirstRaster(ln), CSTR_GetPrev(CSTR_GetLastRaster(ln)) );
return;
}

static void rpstr_temporary_kill_longdash(CSTR_line ln )
{
CSTR_rast       rst;
CSTR_rast_attr  a;
UniVersions     u;
rst = CSTR_GetLastRaster(ln);
rst=CSTR_GetPrev(rst);
if( rst )
    {
    CSTR_GetCollectionUni(rst,&u);
    if( u.lnAltCnt && u.Alt[0].Liga==0x5F )
        {
        CSTR_GetAttr(rst,&a);
        if( !(a.flg_spell & CSTR_fa_spell_nocarrying) )
            {
            u.Alt[0].Liga='-';
            u.Alt[0].Code[0]='-';
            u.Alt[0].Code[1]=0;
            CSTR_StoreCollectionUni(rst,&u);
            }
        }
    }

return;
}

Bool32 rpsrt_line_breace_unknown(CSTR_rast eng)
{
CSTR_rast_attr  attr;
CSTR_GetAttr(eng,&attr);
if( attr.w*3<=attr.h )
	return TRUE;
if( attr.h*3<=attr.w )
	return TRUE;
return FALSE;
}

int32_t rstr_hsp_num(uchar *wrd)
{
int32_t   n;
for(n=0;*wrd;wrd++)
    {
    if( *wrd==SS_NEG_HALF_SPACE || *wrd==SS_POS_HALF_SPACE )
        n++;
    }
return n;
}

static char *rpstr_short_prefix[]={
"по",
"в",
"и",
"а",
"но",
"ее",
"По",
"В",
"И",
"А",
"Но",
"Не",
"не",
"Ее",
"Он",
"он",
"Кто",
"кто",
"\0"
};
static char *rpstr_short_postfix[]={
"по",
"в",
"и",
"а",
"но",
"ее",
"По",
"В",
"И",
"А",
"Но",
"Ее",
"Он",
"он",
"Не",
"не",
"Кто",
"кто",
"\0"
};
static char *rpstr_disable_words[]={
"=",
"+",
"-",
"см",
"шт",
"кг",
"Мб",
"её",
"млн",
"Млн",
"мая",
"Мая",
"№",
"#",
"\0"
};
Bool32 rpstr_is_voc_word(uchar *wrd, char *voc[])
{
int32_t   i;
for(i=0; voc[i][0]!=0;i++)
    {
    if( !strcasecmp(voc[i],wrd) )
        return TRUE;
    }
return FALSE;
}

Bool32 rpstr_is_short_prefix(uchar *wrd)
{
return rpstr_is_voc_word(wrd, rpstr_short_prefix);
}

Bool32 rpstr_is_short_postfix(uchar *wrd)
{
return rpstr_is_voc_word(wrd, rpstr_short_postfix);
}

Bool32 rpstr_disable_short_words(uchar * wrd)
{
return rpstr_is_voc_word(wrd, rpstr_disable_words);
}

Bool32 is_cursive(CSTR_rast eng)
{
CSTR_rast_attr  attr;
CSTR_GetAttr(eng,&attr);
return (attr.font&CSTR_fp_it);
}

Bool32 is_first_capital(CSTR_rast eng)
{
UniVersions u;

if( language!=LANG_ENGLISH && language!=LANG_RUSSIAN && language!=LANG_RUSENG )
    return FALSE;
CSTR_GetCollectionUni(eng,&u);
if( !u.lnAltCnt || !rpstr_is_upper(u.Alt[0].Code[0]) )
    return FALSE;
eng=CSTR_GetNext(eng);
if( !eng )
    return FALSE;
CSTR_GetCollectionUni(eng,&u);
if( !u.lnAltCnt || !rpstr_is_lower(u.Alt[0].Code[0]) )
    return FALSE;
return TRUE;
}

uint32_t myMonitorProc(Handle wnd,Handle hwnd,uint32_t message,uint32_t wParam,uint32_t lParam);
static void unis2word(UniVersions *uvs,int32_t n,uchar *str)
{
int32_t   i;
*str='\0';
for(i=0;i<n;i++)
        {
        strcat(str, uvs[i].Alt[0].Code);
        }

return;
}

Bool32   rpstr_correct_spell(CSTR_line ln,
    CSTR_rast *addbeg, CSTR_rast *addend, int32_t *linefrag,
    int32_t num_ln,Bool32 disable_new_dict, Bool32 disable_check_word)
{
CSTR_rast       eng, enge, senge, tmp;
CSTR_rast_attr  attr;
uchar           ewrd[MAX_LEN_WORD+40],pwrd[MAX_LEN_WORD+40],
                third_wrd[MAX_LEN_WORD+40],
                sec_wrd[MAX_LEN_WORD+40],
                language1, snapstr[1024];
int32_t                   loc_debug=0, loc_debug_replace=1, chg, prevfrag=*linefrag;
Bool32          snap, first,pos, cd;
CSTR_rast       prevbeg=*addbeg, prevend=*addend;
CSTR_attr       lattr;
Bool32          hsp, sf, ss;
int32_t           short_word, short_word_aux;

CSTR_GetLineAttr(ln,&lattr);
cd=( lattr.Flags & CSTR_STR_CapDrop );
multy_language=(lattr.language==7);
*addbeg=(CSTR_rast)0;
*addend=(CSTR_rast)0;
*linefrag=-1;
first=TRUE;
snap = ( !LDPUMA_SkipEx(hSnapSpell,TRUE,TRUE,1) && !skip_line && snap_enable );
senge = enge = CSTR_GetFirstRaster(ln);
while(1)
    {
    eng = rpstr_next_word(senge);
    if( !eng )
        break;
    senge = enge = rpstr_end_word(eng,ewrd,&hsp);
	if( ewrd[0]=='~' && rpsrt_line_breace_unknown(eng) )
		{
		strcpy(pwrd,&ewrd[1]);
		strcpy(ewrd,pwrd);
		eng=CSTR_GetNext(eng);
		}
	tmp= CSTR_GetPrev(enge);
	if( ewrd[strlen(ewrd)-1]=='~' && rpsrt_line_breace_unknown(tmp) )
		{
		ewrd[strlen(ewrd)-1]=0;
		enge =tmp;
		}

    // eng - начало слова, enge - конец
    // анализ переноса (prevbeg,prevend) - (eng,enge)
    if( first && prevbeg && prevend && lattr.fragment!=prevfrag )
        { // текущая и предыдущая строки из разных фрагм
        CSTR_GetAttr(prevend,&attr);
        attr.flg_spell = CSTR_fa_spell_nocarrying;
        CSTR_SetAttr(prevend,&attr);
        }
    if( first && prevbeg && prevend && lattr.fragment==prevfrag )
        { // текущая и предыдущая строки из одного фрагм
        first=FALSE;
        rpstr_cstr2word(prevbeg , prevend,pwrd);
        // pwrd - слово с пред строки
        if( !rpstr_alphabet_check(ewrd) || !rpstr_alphabet_check(pwrd) ||
            rpstr_is_upper(pwrd[strlen(pwrd)-1])&&rpstr_is_lower(ewrd[0]) ||
            rpstr_is_upper(ewrd[strlen(ewrd)-1])&&rpstr_is_lower(pwrd[0]))
            { // одна из частей неудовлетворительна
            CSTR_GetAttr(prevend,&attr);
            attr.flg_spell = CSTR_fa_spell_nocarrying;
            CSTR_SetAttr(prevend,&attr);
            if( snap )
                {
                sprintf(snapstr,"different style of words <%s><%s> ",pwrd,ewrd);
                Snap_Console(snapstr);
                }
            }
        else
            { // обе части похожи на части одного слова
            if( strlen(pwrd)>3 )
                sf = rpstr_txt_spell(pwrd,language);
            else
                sf = rpstr_is_short_prefix(pwrd);//FALSE;
            if( strlen(ewrd)>3 )
                ss = rpstr_txt_spell(ewrd,language);
            else
                ss = rpstr_is_short_postfix(ewrd);//FALSE;
            // sf(ss)- валидности частей
            if( snap )
                {
                sprintf(snapstr,"carrying word <%s><%s> from prev line",pwrd,ewrd);
                Snap_Console(snapstr);
                }
            strcat(pwrd,ewrd);
            // pwrd - скленное слово
            if( rpstr_txt_spell(pwrd,language) )
                { // склеенное слово валидно
                rpstr_set_spell_flag(eng,    enge,   CSTR_fa_spell_solid,CSTR_f_solid);
                rpstr_set_spell_flag(prevbeg,prevend,CSTR_fa_spell_solid,CSTR_f_solid);
                rpstr_correct_attr(eng,enge,prevbeg,prevend);
                rpstr_correct_dash(prevend);
                if( snap )
                    {
                    sprintf(snapstr,"solid carrying word %s",pwrd);
                    Snap_Console(snapstr);
                    }
                continue;
                }
            ed_init();
            // convert part1 -->> ed
            if( !ed_make_word(prevbeg,prevend,&language1) )
                {
                if( snap )
                    {
                    sprintf(snapstr,"can't make ed for <%s>",ewrd);
                    Snap_Console(snapstr);
                    }
                continue;
                }
            // convert part2 -->> ed
            if( !ed_add_word(eng,enge,&language) )
                {
                if( snap )
                    {
                    sprintf(snapstr,"can't make ed for <%s>",ewrd);
                    Snap_Console(snapstr);
                    }
                continue;
                }
            // анализ двучзычных частей
            if( lattr.language==LANG_RUSENG && language1!=language )
                {
                Bool32  pd=rpstr_double_word(prevbeg,prevend,language1);
                Bool32  d =rpstr_double_word(eng,enge,language);
                rpstr_cstr2word(prevbeg , prevend,third_wrd);
                if( pd && !d )
                    {
                    rpstr_correct_ruseng(prevbeg,prevend,language1);
                    if( snap )
                        {
                        sprintf(snapstr,"convert lang <%s> in carried word",third_wrd);
                        Snap_ConsoleLang(snapstr,language1);
                        }
                    }
                if( !pd && d )
                    {
                    rpstr_correct_ruseng(eng,enge,language);
                    if( snap )
                        {
                        sprintf(snapstr,"convert lang <%s> in carried word",third_wrd);
                        Snap_Console(snapstr);
                        }
                    }
                }
            // спеллинг одноязычных частей в склеенном виде
            if( language1==language )
                {
                if( !rpstr_normal_spell(sec_wrd) ) // sec_wrd - txt result
                    {
                    if( snap )
                        {
                        sprintf(snapstr,"internal error! <%s>",ewrd);
                        Snap_Console(snapstr);
                        }
                    break;
                    continue; // внутренняя ошибка
                    }
                // out ed is ready
                chg=strcmp(pwrd, sec_wrd);
                if( chg )
                    { // была коррекци
                    int32_t   phsp=rstr_hsp_num(pwrd);
                    if( strlen(pwrd)-phsp==strlen(sec_wrd)-rstr_hsp_num(sec_wrd) ||
                        strlen(pwrd)==strlen(sec_wrd) )
                        {
                        pos=0;
                        if( phsp && strlen(pwrd)!=strlen(sec_wrd))
                            {
                            rpstr_temporary_kill_neghalf_segment(prevbeg, prevend) ;
                            rpstr_temporary_kill_neghalf_segment(eng, enge) ;
                            }
                        correct_cstr(prevbeg,prevend,&pos); // correct word
                        correct_cstr(eng,    enge,&pos); // correct word
                        rpstr_correct_dash(prevend);
                        rpstr_correct_attr(eng,enge,prevbeg,prevend);
                        if( snap )
                            {
                            sprintf(snapstr,"correct  <%s>",sec_wrd);
                            Snap_Console(snapstr);
                            }
                        continue; // скорректированная склейка подтвердилась
                        }
                    }
                rpstr_correct_attr(eng,enge,prevbeg,prevend);
                // склейка не подтвердилась
                if( !sf || !ss )
                    {
                    rpstr_set_spell_flag(prevbeg,prevend,CSTR_fa_spell_reject,0);
                    rpstr_correct_dash(prevend);
                    if( snap )
                        {
                        sprintf(snapstr,"reject first tail on the carrying word %s","");
                        Snap_Console(snapstr);
                        }
                    rpstr_set_spell_flag(eng,enge,CSTR_fa_spell_reject,0);
                    if( snap )
                        {
                        sprintf(snapstr,"reject second tail on the carrying word %s","");
                        Snap_Console(snapstr);
                        }
                    }
                else if( sf && ss )
                    {
                    CSTR_GetAttr(prevend,&attr);
                    attr.flg_spell = CSTR_fa_spell_nocarrying;
                    CSTR_SetAttr(prevend,&attr);
                    }
                continue;
                }
            }
        } // конец обработки переноса
    first=FALSE; // признак первой строки (для нее нет переноса)
    if( is_last_word(enge) )
        { // последнее слово в строке
        tmp=exist_pos_half(eng,enge); // позитивный полупробел
        if( tmp )
            {
            senge=enge=tmp;
            }
        else
            {
            if( snap )
                {
                sprintf(snapstr,"last word <%s>",ewrd);
                Snap_Console(snapstr);
                }
            *addbeg=eng;
            *addend=enge;
            *linefrag=lattr.fragment;
            rpstr_temporary_kill_poshalf( ln ); // перекодировки п\п
            return TRUE;
            }
        }
    // нормальное слово
    CSTR_GetAttr(eng,&attr);
    if( snap )
        {
        sprintf(snapstr,"before <%s>",ewrd);
        Snap_Console(snapstr);
        }
    language=attr.language;
    short_word      = size_short_language(language);
    short_word_aux  = size_short_language_aux(language);
    // ограничители по длине
    if( !rpstr_alphabet_check(ewrd) )
        {
        if( rpstr_alphabet_mixed(ewrd) )
            {
            rpstr_set_spell_flag(eng,enge,CSTR_fa_spell_reject,0);
            if( snap )
                {
                sprintf(snapstr,"mixed alphabet in short word <%s>",ewrd);
                Snap_Console(snapstr);
                }
            }
        else
            {
            // non defined case
            rpstr_set_spell_flag(eng,enge,CSTR_fa_spell_none,0);
            if( snap )
                {
                sprintf(snapstr,"not word or too short word <%s>",ewrd);
                Snap_Console(snapstr);
                }
            }

        continue; // can't used spell checking
        }
    {
    int32_t   len=strlen(ewrd);
    if( len<short_word && len<short_word_aux )
        { // проверяем короткие слова
        if( rpstr_disable_short_words(ewrd) || len==1)
            {
            rpstr_set_spell_flag(eng,enge,CSTR_fa_spell_none,0);
            if( snap )
                {
                sprintf(snapstr,"short word <%s> is skipped",ewrd);
                Snap_Console(snapstr);
                }
            }
        else if( !rpstr_txt_spell(ewrd,language) )
            {
            rpstr_set_spell_flag(eng,enge,CSTR_fa_spell_reject,0);
            if( snap )
                {
                sprintf(snapstr,"short word <%s> is reject",ewrd);
                Snap_Console(snapstr);
                }
            }
        else
            {
            // solid word
            rpstr_set_spell_flag(eng,enge,CSTR_fa_spell_none,0);
            if( snap )
                {
                sprintf(snapstr,"solid short word <%s>",ewrd);
                Snap_Console(snapstr);
                }
            }

        continue; // can't used spell checking
        }

    if( len==short_word || len==short_word_aux )
        { // валидируем короткие слова
        if( rpstr_txt_spell(ewrd,language) )
            {
            rpstr_set_spell_flag(eng,enge,CSTR_fa_spell_solid,CSTR_f_solid);
            if( snap )
                {
                sprintf(snapstr,"solid short word <%s>",ewrd);
                Snap_Console(snapstr);
                }
            continue;
            }
        }
    }
    if( rpstr_get_solid(eng,enge) )
        { // слово уже валидировано во время распознавани
        rpstr_set_spell_flag(eng,enge,CSTR_fa_spell_solid,0);
        if( snap )
            {
            sprintf(snapstr,"word <%s> is solid",ewrd);
            Snap_Console(snapstr);
            }
        continue;
        }

    ed_init();// создаём ЕД

    if( !ed_make_word(eng,enge,&language) )
        {
        if( snap )
            {
            sprintf(snapstr,"can't make ed for <%s>",ewrd);
            Snap_Console(snapstr);
            }
        continue;
        }
    if( !rpstr_normal_spell(sec_wrd) ) // sec_wrd - txt result
        {
        if( snap )
            {
            sprintf(snapstr,"internal error! <%s>",ewrd);
            Snap_Console(snapstr);
            }
        continue;
        }
    // сформирован обновленный словарём ЕД

    chg=strcmp(ewrd, sec_wrd);
    if( chg )
        { // слово изменилось
        if( strlen(ewrd)==strlen(sec_wrd) )
            { // длина не изменилась
            pos=0;
            if( strlen(ewrd)>(size_t)short_word
            //&& (!is_first_capital(eng) || rec_correct_cstr(eng,enge,TRUE,TRUE))
            )
                {
                correct_cstr(eng,enge,&pos); // correct word
                if( snap )
                    {
                    sprintf(snapstr,"correct  <%s>",sec_wrd);
                    Snap_Console(snapstr);
                    }
                }
            else if( strlen(ewrd)==(size_t)short_word )
                { // short word
                pos=0;
                if( rec_correct_cstr(eng,enge,FALSE,FALSE) )
                    {
                    correct_cstr(eng,enge,&pos); // correct word
                    if( snap )
                        {
                        sprintf(snapstr,"correct short <%s>",sec_wrd);
                        Snap_Console(snapstr);
                        }
                    }
                continue;
                }
            }
        else
            { // длина изменилась
            restruct_cstr(eng,enge, cd); // restruct word
            if( snap )
                {
                sprintf(snapstr,"restruct  <%s>",sec_wrd);
                Snap_Console(snapstr);
                }
            }
        }
    else
        { // слово не изменилось
        rpstr_set_spell_flag(eng,enge,CSTR_fa_spell_reject,0); // отвергнутое слово
        if( snap )
            {
            sprintf(snapstr,"reject  <%s>",sec_wrd);
            Snap_Console(snapstr);
            }
        }
    }
rpstr_temporary_kill_poshalf( ln ); // перекодировки п\п

return  TRUE;
}


Bool32 make_fictive_str(CSTR_line fln, CSTR_rast eng, CSTR_rast enge, CSTR_line ln, CSTR_line lnraw)
{
CSTR_attr       lattr;
CSTR_rast       start,  stop;
CSTR_rast       c,start_new= CSTR_GetFirstRaster (fln), stop_new = CSTR_GetLastRaster (fln), cnew;
CSTR_rast_attr  attr;
RecRaster       rs;
UniVersions     vr;
CCOM_comp    *  comp;
int16_t           left, right;

CSTR_GetLineAttr(ln,&lattr);
CSTR_SetLineAttr(fln,&lattr);

if( !lnraw )
    return FALSE;
start= CSTR_GetFirstRaster (lnraw);
stop = CSTR_GetLastRaster (lnraw);
CSTR_GetAttr (eng, &attr);
if( !(attr.flg&(CSTR_f_let|CSTR_f_bad|CSTR_f_punct|CSTR_f_dust)) )
    return FALSE;
left = attr.r_col;
enge = CSTR_GetPrev(enge);
if( !enge )
    return FALSE;
CSTR_GetAttr (enge, &attr);
if( !(attr.flg&(CSTR_f_let|CSTR_f_bad|CSTR_f_punct|CSTR_f_dust)) )
    return FALSE;
right = attr.r_col+attr.w;
for(c=CSTR_GetNextRaster (start,CSTR_f_all); c && c!=stop; c=CSTR_GetNextRaster (c,CSTR_f_all))
    {
    if( CSTR_GetAttr (c, &attr) &&
        !(attr.r_col+attr.w<left || attr.r_col>right) &&
        CSTR_GetImage (c, (uchar *)&rs, CSTR_TYPE_IMAGE_RS) &&
        CSTR_GetCollectionUni(c,&vr) &&
        (comp=CSTR_GetComp(c))!=NULL )
        {
        if( !(cnew=CSTR_NewRaster (fln,attr.col, attr.row, attr.w)) )
            return FALSE;
        if( !CSTR_SetAttr (cnew, &attr) )
            return FALSE;
        if( !CSTR_StoreRaster (cnew, &rs) )
            return FALSE;
        if( !CSTR_StoreCollectionUni (cnew, &vr) )
            return FALSE;
        if( !CSTR_StoreScale(cnew,comp->scale) )
         return FALSE;
        }
    }
return TRUE;
}

// Nick 27.01.2001
static int rpstr_case_notequal( uchar *in,uchar *out,
        int32_t lenin, int32_t lenout,UniVersions *uvs )
{
	int nSmall;
	int nBig;
	Bool32 firBig;
	int i;
	int nvers;  // new versions

	// Посмотрим исходное слово

	nvers = 0;

	firBig = FALSE;
	nSmall = 0;
	nBig   = 0;
	for(i=0;i<lenin;i++)
	{
		if( rpstr_is_upper(in[i]) )
		{
			nBig++;
			if( i == 0)
				firBig = TRUE;
		}
		else if( rpstr_is_lower(in[i]) )
		{
			nSmall++;
		}
	}

	if( nSmall <= 0 )
	{
		if( nBig > 0 ) // all - to upper
		{
			for(i=0;i<lenout;i++)
			{
             if( !rpstr_is_lower(out[i]) )
				 continue;
			 out[i] = rpstr_to_upper(out[i]);
             uvs[i].Alt[0].Code[0]=out[i];
             uvs[i].Alt[0].Code[1]=0;
             uvs[i].Alt[0].Liga=stdAnsiToAscii(uvs[i].Alt[0].Code[0]);
             nvers++;
			}
		}
		return nvers;
	}

	if( nBig <= 0 || (nBig == 1 && firBig) )
		// all - to lower
	{
		if( firBig )
		{
			if( rpstr_is_lower (out[0]) )
			{
			 out[0] = rpstr_to_upper(out[0]);
             uvs[0].Alt[0].Code[0]=out[0];
             uvs[0].Alt[0].Code[1]=0;
             uvs[0].Alt[0].Liga=stdAnsiToAscii(uvs[0].Alt[0].Code[0]);
             nvers++;
			}
			i = 1;
		}
		else
			i = 0;

		for(;i<lenout;i++)
		{
             if( !rpstr_is_upper (out[i]) )
				 continue;
			 out[i] = rpstr_to_lower(out[i]);
             uvs[i].Alt[0].Code[0]=out[i];
             uvs[i].Alt[0].Code[1]=0;
             uvs[i].Alt[0].Liga=stdAnsiToAscii(uvs[i].Alt[0].Code[0]);
             nvers++;
		}
	}

    return nvers;
}
///////////
// проверить альтернативы
// с кем путаются буквы
static uchar *twinAlts[256]={
	"","","","","","","","","","","","","","","","",
    "","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","", //32-47
	"","","","","","","","","","", // 48 - 57 (0-9)
	"","","","","","", //58-63
	// @ABCDEFG
	"","H","ES","OG","O","B","P","C",// 64-79 @A
	// HIJKLMNO
	"AKMNRX","LT","I","H","I","H","H","CGQ",
	// PQRSTUVW
	"FR","O","HP","B","I","V","UY","",
	// XYZ
	"H","V","","","","","","",
	// @abcdefg
	"","o","hn","og","o","cor","litr","oq",// 64-79 @a
	// hijklmno
	"bn","fljtr","flirt","","fitr","","ho","cde",
	// pqrstuvw
	"o","g","eflit","","flir","ov","ouy","",
	// xyz
	"","v","","","","","","",

	"","","","","","","","","","","","","","","","", // 128
	"","","","","","","","","","","","","","","","", // 144
	"","","","","","","","","","","","","","","","", // 160
	"","","","","","","","","","","","","","","","", // 176
	//АБВГДЕЖЗ
	"Д","ВЕ","БЕ","Т","А","БВ","","Э", //192
	//ИЙКЛМНОП
	"Й","И","ИМНХЬ","АП","КИНП","КИМПХЫЯ","С","КЛИМН", //200
	//РСТУФХЦЧ
    "","О","Г","","","КН","Ч","Ц", // 208
	//ШЩЪЫЬЭЮЯ
	"Щ","Ш","ЬЫ","НЬЪ","КЪЫ","З","","Н", // 216
	//абвгдежз
	"вно","ое","ае","т","","авс","","аэ", //224
	//ийклмноп
	"кмнйя","ип","имнпхь","инп","кинп","акимпхыя","с","клиймн", //232
	//рстуфхцч
    "","о","г","","","кн","ч","ц", // 240
	//шщъыьэю
	"щ","ш","ьы","ньъ","къы","аз","","ан" // 248
};
//////////////////
static Bool32 IsInAlter(uchar *Code,CSTR_rast c)
{
	UniVersions vers;
	uchar codeAlt[4];
	int32_t i;

	    CSTR_GetCollectionUni(c,&vers);

		strcpy(&codeAlt[0],Code);
		for(i=0;i<4;i++)
		{
         if( codeAlt[i] == 0 )
			 break;
		 if( rpstr_is_upper(codeAlt[i]) )
		  codeAlt[i] = rpstr_to_lower(codeAlt[i]);
         else if( rpstr_is_lower(codeAlt[i]) )
		  codeAlt[i] = rpstr_to_upper(codeAlt[i]);
		}

		for(i=0;i<vers.lnAltCnt;i++)
		{
			if( !strcmp(vers.Alt[i].Code,Code) )
				return TRUE;
			if( !strcmp(vers.Alt[i].Code,codeAlt) )
				return TRUE;

			// проверим близнецов
			if( Code[1] == 0 && vers.Alt[i].Code[1] == 0 )
			{
				if( strchr( twinAlts[vers.Alt[i].Code[0]],Code[0]) )
					return TRUE;
				if( strchr( twinAlts[vers.Alt[i].Code[0]],codeAlt[0] ) )
					return TRUE;
			}
		}

		return FALSE;
}
//////////////

static rpstr_is_letter(uchar w)
{
   if (w>='A' && w<='Z' || w>=(uchar)'А' && w<=(uchar)'Я' )
	   return TRUE;
   if (w>='a' && w<='z' || w>=(uchar)'а' && w<=(uchar)'я' )
	   return TRUE;

   return FALSE;
}
////////////////
// проверить альтернативы словар
// Nick 18.06.2001
static  int32_t rpstr_test_spell_alter(CSTR_rast be,CSTR_rast en, int32_t nlim, UniVersions *uv)
{
CSTR_rast   c,last;
int32_t       nv;
int32_t       newRecog;
UniVersions vers;
Bool32 goodEnd;
Bool32 bigFirst;
int32_t  numBig;
int32_t  numSpaceOld;
Bool32 testStart,testEnd;
CSTR_rast_attr attr;
int32_t  allNew;
Bool32 wasEnglish=FALSE;
Bool32 wasLiga = FALSE;

 if( hSnapEndWord && !LDPUMA_SkipEx(hSnapEndWord,FALSE,FALSE,1) )
	 testEnd = FALSE;
 else
	 testEnd = TRUE;

 if( hSnapStartWord && !LDPUMA_SkipEx(hSnapStartWord,FALSE,FALSE,1) )
	 testStart = FALSE;
 else
	 testStart = TRUE;

 if( !testStart && !testEnd )
	 return 1;

 if( !be || nlim <= 0 )
	 return 0;

 for( c=be; c&&c!=en;c=CSTR_GetNext(c) )
    last = c;

 // проверим окончание
 goodEnd = IsInAlter(uv[nlim-1].Alt[0].Code,last);

 // проверим начало
 CSTR_GetCollectionUni(be,&vers);
 if( vers.lnAltCnt > 0 && rpstr_is_upper(vers.Alt[0].Code[0]))
     bigFirst = TRUE;
 else
	 bigFirst = FALSE;

 // проверяем, только если подозрительное окончание
 // или с заглавной буквы
 // или первое слово в строке
 // перепроверять все было плохо - 16.08.01 Nick
 if( goodEnd && !bigFirst )
 {
	 // not first in line?
	 if( CSTR_GetPrev(be) )
	   return 1;
 }

 numBig = 0;
 newRecog = 0;
 numSpaceOld = 0;
 allNew = 0;
 for(nv=0,c=be;nv<nlim && c&&c!=en;c=CSTR_GetNext(c),nv++)
   {
        CSTR_GetCollectionUni(c,&vers);

		// в старой версии были нераспознанные - берем новую
		if( vers.lnAltCnt <= 0 )
			return 1;

		// в старой были очень плохие - берем новую
		if( vers.Alt[0].Prob <= 160 )
			return 1;

		// в новой нераспознанные (невозможно !?) - старую
		if( uv[nv].lnAltCnt <= 0 )
			return 0;

		// space or half-space
		if( vers.Alt[0].Code[0] == ' ' )
		{
			numSpaceOld++;
			nv--;
			continue;
		}

		// были не все буквы!
		if( !rpstr_is_letter( vers.Alt[0].Code[0] ))
			return 1;

		// были лигатуры?
		if( vers.Alt[0].Code[1] )
			wasLiga = TRUE;

		// окончания активнее для русских ! Nick 03.08.2001
		CSTR_GetAttr(c,&attr);
		if( attr.language == LANG_ENGLISH)
			wasEnglish = TRUE;

		if( rpstr_is_upper(vers.Alt[0].Code[0]) )
			numBig++;

        if( uv[nv].Alt[0].Code[0] != vers.Alt[0].Code[0] )
			allNew++;

		if( !IsInAlter(uv[nv].Alt[0].Code,c) )
			newRecog++;
   }

 // не дошли до конца или были лигатуры?
 if( c&&c!=en || wasLiga )
	 nv = nlim+1;    // сколько - не знаем, но чтоб не совпадало

 if( newRecog <= 0 && nv == nlim )
	 return 1;

 if( testStart && bigFirst && newRecog )
 {
	 // замена в названии - опасно
	 if( numBig == 1 && nv == nlim )
		 return 0;
 }

 // замена только окончания - плохо
 if( testEnd && !goodEnd )
 {
	 if( ( newRecog == 1 && nv==nlim ) ||
		 // приклейка или отрезание окончани
		 ( /*!wasEnglish &&*/ newRecog == 0 && nv !=nlim )
	   )
       return 0;
 }

 // Nick 13.08.01 - есть плюсы
 if( newRecog == 1 && nv==nlim && allNew == 1 )
	 return 0;

 return 1;
}
/////////////////
// Nick 7.01.2002
// попытка типичных для распознавания замен
#define MAX_STANDARD 15
static char *standIni[MAX_STANDARD]={
	"m", "m", "rn", "ri", "rri", "n","ll","g","g","li","h","U","Li","nn","nn"};
static char *standReplace[MAX_STANDARD]={
	"rn", "rri", "m", "n", "m", "ri","g","ll","il","h","li","Li","U","rm","mr"};
/////////
// найти нужное место
static CSTR_rast rpstr_find_in_word(CSTR_rast cs,CSTR_rast ce,
									uchar *ewrd, int start, int len,
									CSTR_rast *last)
{
 CSTR_rast       c;
 int16_t           i;
 UniVersions     vers;
 int allLen = strlen(ewrd), oneLen;
 CSTR_rast first=NULL;
 int	curLen;


	curLen = 0;
	for(c=cs, i = 0 ; c && c!= ce; c=CSTR_GetNext(c), i+= oneLen )
	{
		if( curLen == len )
			break;

		CSTR_GetCollectionUni(c,&vers);
		if( vers.lnAltCnt <= 0 )
			return NULL;

		oneLen = strlen(vers.Alt[0].Code);
		if( i + oneLen > allLen )
			return NULL;

		if( memcmp(ewrd+i,vers.Alt[0].Code,oneLen) )
			return NULL;

		if( i == start )
			first = c;

		if( first )
			curLen += oneLen;
	}


	if( !first )
		return NULL;

	if( curLen != len )
		return NULL;

	*last = c;

	return first;
}
/////////////////////
static int	ReplacePartWord(CSTR_rast eng, CSTR_rast enge,
							char *ewrd,int start,
							char *oldPart,
							char *newPart,
							int lang)
{
	CSTR_rast_attr  attr={0}, battr={0};
	CSTR_rast       c, cp;
	UniVersions     vers={0};
	int32_t           mincol,maxcol,minrow,maxrow,avwid ;
	int32_t           ii, mincolr,maxcolr,avwidr ;
	uint16_t          loc_charset=35535, loc_battr=35535 ;
	int oldLen = strlen(oldPart), newLen=strlen(newPart);
	CSTR_rast first, last;
	int midProb;
	int nSymb;


	if( newLen <= 0 )
		return 0;

	minrow=mincol=mincolr=32000;
	maxrow=maxcol=maxcolr=-16000;


	first = rpstr_find_in_word( eng, enge,
								ewrd, start, strlen(oldPart),
								&last);
	if( !first )
		return 0;

	cp=CSTR_GetPrevRaster(first,255);
	if( !cp )
		return 0;

	// set flag solid
	for(c=eng;c && c!=enge ; c=CSTR_GetNext(c) )
    {
		CSTR_GetAttr(c,&attr);
		attr.flg |= CSTR_f_solid;
		CSTR_SetAttr(c,&attr);
	}

	nSymb = 0;
	midProb = 0;
	for(c=first;c && c!=last ; nSymb++ )
    {
		CSTR_GetAttr(c,&attr);
		if( attr.flg & (CSTR_f_let|CSTR_f_bad|CSTR_f_dust|CSTR_f_punct) )
        {
			if( attr.col+attr.w>maxcol ) maxcol = attr.col+attr.w;
			if( attr.col<mincol )        mincol = attr.col;
			if( attr.r_col+attr.w>maxcolr ) maxcolr = attr.r_col+attr.w;
			if( attr.r_col<mincolr )        mincolr = attr.r_col;
			if( attr.row+attr.h>maxrow ) maxrow = attr.row+attr.h;
			if( attr.row<minrow )        minrow = attr.row;
        }


		CSTR_GetCollectionUni(c,&vers);
		if( attr.flg & CSTR_f_let )
        {
			battr = attr;
			if( loc_charset==35535 )
				loc_charset = vers.Alt[0].Charset;
        }

		if( vers.lnAltCnt > 0 )
			midProb += vers.Alt[0].Prob;

		c=CSTR_DelRaster(c);
    }

	if( nSymb <= 0 )
		return 0;

	midProb /= nSymb;

	avwid = (maxcol-mincol)/newLen;
	avwidr = (maxcolr-mincolr)/newLen;


	for(c=cp, ii=0 ; ii < newLen; ii++)
    {
		c=CSTR_InsertRaster(c); // c - prev raster
		if( !c )
			continue;

		attr.r_row  = (int16_t)minrow   ;
		attr.r_col  = (int16_t)(mincolr+ii*avwidr)  ;
		attr.row    = (int16_t)minrow   ;
		attr.col    = (int16_t)(mincol+ii*avwid)  ;
		attr.h      = (int16_t)(maxrow-minrow);
		attr.w      = (int16_t)avwid ;
		attr.font   = battr.font       ;
		attr.keg    = battr.keg        ;
		attr.language=(uchar)lang;
		attr.flg_spell = CSTR_fa_spell_restruct;
		attr.flg    = CSTR_f_let | CSTR_f_solid;

		vers.Alt[0].Code[0]= newPart[ii];
		vers.Alt[0].Code[1]= 0;
		vers.Alt[0].Liga   = newPart[ii];
		vers.Alt[0].Prob   = (uchar)midProb;
		vers.Alt[0].Method = REC_METHOD_DIC;
		vers.Alt[0].Charset= (uchar)loc_charset;
		vers.Alt[0].Info   = 0;

		vers.lnAltCnt=1;
		vers.lnAltMax=REC_MAX_VERS;

		CSTR_StoreCollectionUni(c,&vers);
		CSTR_SetAttr(c,&attr);
    }

	return 1;
}

