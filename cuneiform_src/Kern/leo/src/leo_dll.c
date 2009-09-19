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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
/*#include <io.h>*/

#include "leo_tune.h"
#include "cpu.h"
#define PC_TYPE 0
#define MAX_FIELDS 1000
#include "leo.h"
#include "alphaset.h"
#include "leo_func.h"

#include "r35.h"
#include "evn.h"
#include "dif.h"
#include "ctb.h"
#include "fon.h"
#include "msk.h"

#include "std.h"
#include "snptools.h"
#include "snpdefs.h"

#include "minmax.h"

uchar field_number=0;
Bool32 leo_enable_fon_recog=FALSE;
Bool32 leo_Snp_In_Rect=FALSE;
int  LEO_error_code=ER_LEO_NO_ERROR;
int32_t   GLU_all ,
        GLU_rus ,
        GLU_dig ,
        GLU_rom ,
        GLU_rall;
FILE *  fp_glob;
Bool32 leo_enable_stored=FALSE;
 SnpTreeNode  stnCharRecog;      // stops afeter char recog
 SnpTreeNode  stnSnapCharRecog;
 SnpTreeNode  stnSnapCharProt;
 SnpTreeNode  stnReRecog;
// for registartion of alphabet set
int32_t leo_alpha_count=1;
char  *leo_alpha_reg[256];
uchar leo_current_alpha_ndx=0;
static int32_t fields_tab[MAX_FIELDS]={0};
static MemFunc* leo_mem;
static void* leo_alloc(uint32_t len){return malloc(len);}
static void  leo_free(void *ptr,uint32_t len){ free(ptr);}
static void* (*my_alloc)(uint32_t len)=leo_alloc;
static void  (*my_free)(void *,uint32_t len)=leo_free;
static Bool32 leo_is_load = FALSE;
static uchar save_alpha_valid=0, save_isPrint=0;


static Bool32   leo_init_evn_hnd=FALSE,
                leo_init_net_hnd=FALSE,
                leo_init_r35_hnd=FALSE,
                leo_init_msk_hnd=FALSE,
                leo_init_glu_hnd=FALSE;

static Bool32   leo_init_evn_prn=FALSE,
                leo_init_r35_prn=FALSE,
                leo_init_msk_prn=FALSE,
                leo_init_msk_prn_mtr=FALSE;

static Bool32   leo_init_net_ndx=FALSE,
                leo_init_r35_ndx=FALSE;

int32_t  leo_typ_of_font = LEO_FONT_NONE, leo_MSK_ndx[3]={0};
int nNdxWid , nNdxHei;
Bool32  no_init_ndx=TRUE, no_init_hnd=TRUE,  no_init_prn=TRUE;
uchar nIsPrint=0;
static char alphabet_dig[256],alphabet_all[256];
static   Bool32 leo_enable=TRUE;

// data from module leo.c
int set_cpu=0;
extern uchar leo_alpha_type, prn_roma_regim;
extern unsigned char alphabet[256];
extern int32_t leo_stick_nose_1;

///
LEO_FUNC(int32_t)   LEO_Str2FldNo(int32_t  str_no)
{
if( str_no>=MAX_FIELDS || str_no<=0 )
   return -1;
return fields_tab[str_no];
}

static Bool leoInsideRect( const Rect16* r, const Point16 * p)
{ return !( (p->x < r->left)||(p->x > r->right )||
            (p->y < r->top) ||(p->y > r->bottom)
          );
}

void Leo_SnpWaitUserInput(SnpTreeNode *stnCharRecog)
{
int sstatus=stnCharRecog->Status;
stnCharRecog->Status|= (STN_STOP | STN_DRAW);
SnpWaitUserInput(stnCharRecog);
if( (stnCharRecog->Status&(STN_STOP | STN_DRAW)) )
    stnCharRecog->Status=sstatus;
    return;
}

Bool32 leoSnpInRect( Rect16* pRect, int32_t nSkew )
{
   Point16 pt;
   if (!SnpIsActive())
      return FALSE;
   if (!SnpGetUserPoint( &pt))
      return FALSE;
   return leoInsideRect(pRect, &pt);
}


void    leo_SnpLog(char *tmp)
{
if (!SnpSkip(&stnCharRecog) )
    SnpLog(tmp);
return;
}

void leo_snapChar(RecVersions *ver, char *tit, int enable)
{
char buf[256], *t, title[256];
int i;

if (!SnpSkip(&stnCharRecog)||enable || leo_Snp_In_Rect)
	{
	t = buf;
	if( ver->lnAltCnt )
		{

		for(i=0;i<ver->lnAltCnt-1;i++)
            {
			t += sprintf(t,"%c(%d,%s),",
                stdAsciiToAnsi(ver->Alt[i].Code),
				ver->Alt[i].Prob,
				LEOGetMetName(ver->Alt[i].Method, title)
				);
            }
        t += sprintf(t,"%c(%d,%s)",
            stdAsciiToAnsi(ver->Alt[i].Code),
			ver->Alt[i].Prob,
			LEOGetMetName(ver->Alt[i].Method, title)
			);
		}
	else
			strcpy(buf,"-");
	SnpLog("%s %s", tit, buf);
	}
return;
}





void leo_snapChar3x5(RecVersions *ver, char *tit, int enable)
{
char buf[256], *t;
int i;
if (!SnpSkip(&stnCharRecog)||enable || leo_Snp_In_Rect)
	{
	t = buf;
	if( ver->lnAltCnt )
		{
		for(i=0;i<ver->lnAltCnt-1;i++)
			t += sprintf(t,"%c(%d,3x5,%d),",
                stdAsciiToAnsi(ver->Alt[i].Code),
				ver->Alt[i].Prob,
				ver->Alt[i].Info
				);
		t += sprintf(t,"%c(%d,3x5,%d)",
            stdAsciiToAnsi(ver->Alt[i].Code),
			ver->Alt[i].Prob,
			ver->Alt[i].Info
			);
		}
	else
			strcpy(buf,"-");
	SnpLog("%s %s", tit, buf);
	}
return;
}

void leo_snapSimpleKey(char *str, SnpTreeNode *stnRecog)
{
SnpLog("%s",str);
SnpLog("");
Leo_SnpWaitUserInput(stnRecog); // pass control to user
SnpHideRects((uint32_t)stnRecog);
return;
}

void leo_snapRes2Str(RecVersions *ver, char *buf)
{
char *t=buf, title[256];
int i;
if( ver->lnAltCnt )
    {
	for(i=0;i<ver->lnAltCnt-1;i++)
        {
	    t += sprintf(t,"%c(%d,%s),",
		    ver->Alt[i].Code,ver->Alt[i].Prob,
			LEOGetMetName(ver->Alt[i].Method, title));
        }
	t += sprintf(t,"%c(%d,%s)",
		ver->Alt[i].Code,ver->Alt[i].Prob,
		LEOGetMetName(ver->Alt[i].Method, title));
		}
else
    {
	t += sprintf(t,"Nonrecog");
	}
return;
}

void leo_store_for_pass2(RecObject* object,uchar let)
{
object->recData.recRaster.Raster[REC_MAX_RASTER_SIZE-1]=stdAnsiToAscii(let);
return;
}


void leo_snapRaster(RecObject* object, SnpTreeNode *stnRecog)
{

SnpDrawRect(&object->recData.rect,
     0, //Skew - zero to real coords
     wRGB(0,0,255),
     -16, // one image pixel width
     (uint32_t)stnRecog    );

SnpDrawRaster( &object->recData.recRaster );
return;
}


void leoSetAlphabet(char     alphabet[], int leo_alpha_type)
{
R35SetAlphabet(alphabet);
DIFSetAlphabet(alphabet);
MSKSetAlphabet(alphabet);
EVNSetAlphabet(alphabet);
FONSetAlphabet(alphabet);
return;
}
/////////////////////////////////////////////////////////////////

LEO_FUNC(void) LEOSetPlatform(int32_t cpu)
{
R35SetPlatform(cpu);
MSKSetPlatform(cpu);
set_cpu=cpu?cpu:1;
return;
}

LEO_FUNC(int32_t) LEOGetCPU(void)
{
#ifdef PC_TYPE
	return GetCPUName();
#else
	return 0;
#endif
}


Bool32 leo_init_prn(void)
{
MemFunc* mem = leo_mem;
if( !no_init_prn )
    return TRUE;

if( !leo_init_evn_prn  )
  {
  if( EVNInitPRN(mem) )
    leo_init_evn_prn=TRUE;
  else
    {
    LEO_error_code=ER_LEO_NOINIT_EVN;
    return FALSE;
    }
  }

if( !leo_init_r35_prn )
    {
    if( R35InitPRN(mem) )
        leo_init_r35_prn=TRUE;
    else
        {
        LEO_error_code=ER_LEO_NOINIT_R35;
        return FALSE;
        }
    }

if( !leo_init_msk_prn )
    {
    leo_MSK_ndx[1]=MSKInit( mem, "rec4r&e.dat" );
    if( leo_MSK_ndx[1] )
        leo_init_msk_prn=TRUE;
    else
        {
        LEO_error_code=ER_LEO_NOINIT_IND;
        return FALSE;
        }
  }

if( !leo_init_msk_prn_mtr )
    {
    leo_MSK_ndx[2]=MSKInit( mem, "rec4mtr.dat" );
    if( leo_MSK_ndx[2] )
        leo_init_msk_prn_mtr=TRUE;
    else
        {
        LEO_error_code=ER_LEO_NOINIT_IND;
        return FALSE;
        }
  }

no_init_prn=FALSE;
return TRUE;
}


Bool32 leo_init_ndx(void)
{
MemFunc* mem = leo_mem;

if( !leo_init_r35_ndx )
    {
    if( R35InitNDX(mem) )
        leo_init_r35_ndx = TRUE;
    else
        {
        LEO_error_code=ER_LEO_NOINIT_R35;
        return FALSE;
        }
  }

return TRUE;
}

static Bool32 leo_init_all(void)
{
//LEOInitPass2( ); // DEBUG mode
return TRUE;
}

LEO_FUNC(Bool32)  LEOInit( MemFunc* mem )
{
int32_t   i;
leo_is_load = TRUE;
leo_enable_fon_recog=FALSE;
LEO_error_code=ER_LEO_NO_ERROR;

if( mem!=NULL )
	{
	my_alloc  = mem->alloc;
	my_free   = mem->free;
	}
else
	{
	my_alloc  = leo_alloc;
	my_free   = leo_free;
	}

memset(alphabet_dig,0,256);
memset(&alphabet_dig[48],1,10);
memset(alphabet_all,1,256);
for(i=0;i<256;i++)
    {
    leo_alpha_reg[i]=0;
    }

if( !set_cpu )
	LEOSetPlatform( LEOGetCPU() );

leo_clear_stat();

leo_mem = mem ;

if( !leo_init_all() )
    return FALSE;

if( _access("C:\\method.txt",0)!=-1) // DEBUG
    fp_glob=fopen("D:\\method.txt","wb");
return TRUE;
}

LEO_FUNC(Bool32)  LEOInitPass2( void )
{
leo_enable_fon_recog=FALSE;
if( _access(".\\tmp\\page.ctb",0)==-1) // DEBUG mode
    return FALSE;
FONDone();
if( FONInit(".\\tmp\\page.ctb")<1 )
    return FALSE;
leo_enable_fon_recog=TRUE;
return TRUE;
}

LEO_FUNC(void)  LEODonePass2(void)
{
FONDone();
return;
}

LEO_FUNC(void)  LEOFreeAlphabets(void)
{
int32_t i;
for(i=0;i<256;i++)
    {
    if( leo_alpha_reg[i] )
        {
        my_free(leo_alpha_reg[i],0);
        leo_alpha_reg[i]=0;
        }
    }
leo_alpha_count=1;
return ;
}

LEO_FUNC(void)  LEODone(void)
{
if( leo_is_load )
    {
    nIsPrint=0;

    R35Done();
    MSKDone();
    EVNDone();
    CTB_done();
    //LEODonePass2(); // DEBUG mode

    leo_close_cont();
    LEO_ClearStat();
    leo_is_load = FALSE;
    no_init_prn=TRUE;
    no_init_hnd=TRUE;
    no_init_ndx=TRUE;

    ////////////////
    leo_init_evn_hnd=leo_init_net_hnd=leo_init_r35_hnd=
    leo_init_msk_hnd=leo_init_glu_hnd=FALSE;
    leo_init_evn_prn= //leo_init_net_prn=
    leo_init_r35_prn= leo_init_msk_prn=
    leo_init_msk_prn_mtr=FALSE ;
    leo_init_net_ndx= leo_init_r35_ndx =FALSE;
    ///////////////////


    }
LEOFreeAlphabets();
if( fp_glob )
    fclose(fp_glob);
return ;
}

LEO_FUNC(void) LEOCloseCont(void)
{
leo_close_cont_temp();
return ;
}

LEO_FUNC(int32_t) LEOOpenCont(void)
{
return leo_open_cont_temp();
}


LEO_FUNC(Bool32)  LEOPushAlphabetType(uchar alpha_valid, uchar isPrint)
{
char my_alpha_cap[]="ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ";
char my_digit[]="0123456789";
char my_roma[]="VIX"; //MCD";
char my_alpha_cap_and_digit[]="ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ/0123456789";
char *palph, al[256]={0};


if( save_isPrint==isPrint && save_alpha_valid==alpha_valid )
    return TRUE;
else
    {
    save_alpha_valid=alpha_valid;
    save_isPrint    =isPrint;
    }

switch( alpha_valid&15 )
    {
    case LEO_VALID_ALL:
        palph = my_alpha_cap_and_digit;
        break;
    case LEO_VALID_RUSSIAN:
        palph = my_alpha_cap;
        break;
    case LEO_VALID_DIGIT:
        palph = my_digit;
        break;
    case LEO_VALID_LATIN:
        palph = my_roma;
        break;
    default:
        return FALSE;
    }
while( *palph )
    {
    al[ (uchar)*palph++ ]=1;
    }

nIsPrint = isPrint;
return LEOSetAlphabet( al );
}


uchar leo_register_alphabet(char alphabet[])
{
int32_t i;

for(i=1;i<=leo_alpha_count;i++)
    {
    if( !leo_alpha_reg[i] )
        break;
    if( !memcmp(leo_alpha_reg[i],alphabet,256) )
        return (uchar)i; // alphabet ready
    }
if( i>255 )
    return (uchar)0;
// new alphabet
leo_alpha_reg[i]=my_alloc(256);
if( !leo_alpha_reg[i] )
    return (uchar)0;
memcpy(leo_alpha_reg[i],alphabet,256);
leo_alpha_count = i;
return (uchar)i;
}

LEO_FUNC(Bool32)  LEOSetAlphabet(  char     ansi_letters[] ) // char table[0-255]
{
int i, n,all,eng,rus;

if( !ansi_letters )
    return FALSE;
memset(alphabet,0,256);
for(n=i=0;i<256;i++)
	if( ansi_letters[i] )
        alphabet[ stdAnsiToAscii((uchar)i) ]=1;
if(  alphabet['\"'] )
	{
	 alphabet[0xd7]= alphabet[0xd8]=1;
	}
for(eng=all=rus=leo_alpha_type=i=0;i<256;i++)
	{
    if( alphabet[i] && leo_accepted[i] )
        {
		leo_alpha_type |= typ_of_alpha[i];
        all++;
        if( i<128 && (i>57||i<48))
            eng++;
        if( i>=128 )
            rus++;
        }
	}
leo_current_alpha_ndx=leo_register_alphabet(alphabet);
prn_roma_regim = (
        !ansi_letters[(uchar)'А'] &&
        ansi_letters['I'] &&
        ansi_letters[(uchar)'П'] &&
        ansi_letters[(uchar)'У'] &&
        ansi_letters[(uchar)'Х'] &&
        ansi_letters[(uchar)'Ш'] &&
        ansi_letters['1']);

if( leo_alpha_type&ALPH_R_E )
    leo_alpha_type=ALPH_R_E;
if( leo_alpha_type&ALPH_RALL )
    leo_alpha_type=ALPH_RALL;

if( leo_alpha_type!=ALPH_RUS  &&
    leo_alpha_type!=ALPH_DIG  &&
    leo_alpha_type!=ALPH_ROM  &&
    leo_alpha_type!=ALPH_RALL &&
    leo_alpha_type!=ALPH_R_E )
	leo_alpha_type=ALPH_ALL;
if( !nIsPrint && (leo_alpha_type==ALPH_RALL || leo_alpha_type==ALPH_R_E) )
    leo_alpha_type=ALPH_ALL;

if( eng && !rus && all && leo_alpha_type!=ALPH_ROM && leo_alpha_type!=ALPH_DIG )
    leo_alpha_type=ALPH_ENG;

if( nIsPrint )
    alphabet[0]=1;

leoSetAlphabet( alphabet, leo_alpha_type);
LEOSetFont( prn_roma_regim ? LEO_FONT_TW : LEO_FONT_NONE); // Moscow Pension Tested
return TRUE;
}

LEO_FUNC(Bool32)  LEOSetupField(LeoFieldSetup* fs)
{
nIsPrint = 0;
LEOSetAlphabet(fs->AlphaTable);
DIFSetFont(0);
R35SetMTR(0);
nNdxWid  =  fs->BoxSize.x;
nNdxHei  =  fs->BoxSize.y;
field_number++;
if( field_number<MAX_FIELDS )
   {
   fields_tab[field_number]=fs->nFieldNo;
   }
nIsPrint = (uchar)((fs->nStyle&LS_PRINT)!=0);
if( nIsPrint )
    alphabet[0]=1;
return TRUE;
}

LEO_FUNC(void) LEOSetFont( int32_t typ_of_font)
{
leo_typ_of_font = typ_of_font;
DIFSetFont( typ_of_font);
if( leo_typ_of_font & LEO_FONT_MTR )
    R35SetMTR(1);
return;
}

LEO_FUNC(Bool32)  LEOSetupPage(LeoPageSetup* ps)
{
//static int loc_num=0;
// can be used filed ps->AlphaStylesTable for select recog.tables
leo_clear_stat();
LEO_ClearStat();
leo_sizes_init_stat();
leo_typ_of_font = LEO_FONT_NONE;
save_alpha_valid=save_isPrint=0;
if( !ps->nIdPage )
	ps->nIdPage=LEO_CONT_STD_PAGE_ID/*loc_num++*/;
if( ps->nIdPage==-1 )
	ps->nIdPage=0;
if( ps->nIdPage )
    leo_enable_stored=leo_cont_new_page(ps->nIdPage);
else
    leo_enable_stored=FALSE;
field_number=0;
memset(&fields_tab[0],0,MAX_FIELDS);
return leo_enable_stored;
}

LEO_FUNC(int16_t) LEOGetErr(void)
{
return LEO_error_code;
}



LEO_FUNC(void) LEOSortVersProb(RecVersions *v)
{
leo_sort_vers_prob(v);
return;
}



LEO_FUNC( void   )      LEORegisterSnpTree(
         SnpTreeNode* parent,         // parent Snp Node, may be NULL
         __SnpToolBox* p_snp_tools     // tools complect, may be NULL
                              )
{
   SnpSetTools( p_snp_tools );    // may be NULL, it's OK
#ifdef DPUMA_SNAP
   {
	SnpTreeNode p;
	SnpAddNode(&p,"Распознавание строк...",NULL);
    parent = (SnpTreeNode*)&p;
   }
// или Parent = NULL, если родитель -  корень
#endif
   SnpAddNode( &stnCharRecog,
      "Show Leo LTR Recognize results",  parent);
   SnpAddNode( &stnSnapCharRecog,
      "Show Leo LTR Extended Recognize results",  &stnCharRecog);
   SnpAddNode( &stnReRecog,
      "Show Font Rerecog",  &stnCharRecog);
   SnpAddNode( &stnSnapCharProt,
      "Show Leo LTR Save Char to CTBase",  &stnCharRecog);
};



LEO_FUNC(char *) LEOGetMetName(int32_t method, char *met_name)
{
switch( method )
    {
    case    REC_METHOD_3X5: strcpy(met_name,"3x5");break;
    case    REC_METHOD_MSK: strcpy(met_name,"msk");break;
    case    REC_METHOD_EVN: strcpy(met_name,"evn");break;
    case    REC_METHOD_NDX: strcpy(met_name,"ndx");break;
    case    REC_METHOD_SCE: strcpy(met_name,"sce");break;
    case    REC_METHOD_FINAL: strcpy(met_name,"fin");break;
    case    REC_METHOD_FON: strcpy(met_name,"fon");break;
	case    REC_METHOD_CNT: strcpy(met_name,"cnt");break;
    default               : strcpy(met_name,"unknown");break;
    }
return  met_name;
}

LEO_FUNC(Bool32)  LEORecogPrintChar( RecObject*  object )
{
RecVersions ver;
int small_wid=7, i,n, id_rast, id_alph, pen_prop=0, pen_size=0;
uchar let_narrow[]="()|1";

if( no_init_prn )
    {
    if( !leo_init_prn() )
        return FALSE;
    no_init_prn=FALSE;
    }

DIFClearRightZone(object->recData.recRaster.Raster ,
        (int16_t)object->recData.recRaster.lnPixWidth,
        (int16_t)(REC_GW_WORD8(object->recData.recRaster.lnPixWidth)),
        (int16_t)object->recData.recRaster.lnPixHeight);

leo_Snp_In_Rect=leoSnpInRect(&object->recData.rect, 0);
if( leo_alpha_type==ALPH_ENG || leo_alpha_type==ALPH_R_E )
   small_wid=3;
if( leo_typ_of_font&LEO_FONT_MTR )
   small_wid=7;
LEO_error_code=ER_LEO_NO_ERROR;

pen_prop = leo_proport_penalty(0,0, object->recData.recRaster.lnPixWidth,
    object->recData.recRaster.lnPixHeight);

if( pen_prop>50 )
    {
    LEO_error_code=ER_LEO_SMALL_OBJECT;
	if (!SnpSkip(&stnCharRecog)|| leo_Snp_In_Rect)
			{
			leo_snapRaster(object, &stnCharRecog);
            leo_snapSimpleKey("TOO WIDE KILL ", &stnCharRecog);
			}
	return FALSE;
    }
if( !object )
	{
	LEO_error_code=ER_LEO_NULL_OBJECT;
	return FALSE;
	}

if( object->recResults.Alt[0].Info )
    {
    char tmp[256];
    sprintf(tmp,"Restored raster. CTBase Number = %d",object->recResults.Alt[0].Info-1);
    leo_SnpLog(tmp);
    }

if(leo_small_object(object,small_wid,small_wid))
	{
	LEO_error_code=ER_LEO_SMALL_OBJECT;
	if (!SnpSkip(&stnCharRecog)|| leo_Snp_In_Rect)
			{
			leo_snapRaster(object, &stnCharRecog);
            leo_snapSimpleKey("TOO SMALL RASTER", &stnCharRecog);
			}
	return FALSE;
	}
pen_size=leo_sizes_penalty( object->recData.recRaster.lnPixWidth,
                            object->recData.recRaster.lnPixHeight);
if(  pen_size>73 && !(object->recData.recRaster.lnPixWidth*3<
                    object->recData.recRaster.lnPixHeight&&
                    object->recData.lwCompCnt==1&&
                    object->recData.recRaster.lnPixHeight>10) )
    {
    LEO_error_code=ER_LEO_SMALL_OBJECT;
	if (!SnpSkip(&stnCharRecog)|| leo_Snp_In_Rect)
			{
			leo_snapRaster(object, &stnCharRecog);
            leo_snapSimpleKey("TOO SMALL RASTER", &stnCharRecog);
			}
	return FALSE;
    }
if(leo_big_object(object,150,150) )
	{
	LEO_error_code=ER_LEO_LARGE_OBJECT;
	if (!SnpSkip(&stnCharRecog)|| leo_Snp_In_Rect)
			{
			leo_snapRaster(object, &stnCharRecog);
            leo_snapSimpleKey("TOO BIG RASTER", &stnCharRecog);
			}
	return FALSE;
	}
if(1)
if( leo_Snp_In_Rect )
    {
    leo_snapRaster(object, &stnCharRecog);
    leo_snapSimpleKey("Before recog", &stnCharRecog);
    }

object->recResults.lnAltMax=REC_MAX_VERS;
if( object->recData.lwCompCnt==0 )
    object->recData.lwCompCnt= EVNGetNumComp(&object->recData.recRaster);
{
int prop, i;
leoRecogPrintChar(object );
if(object->recResults.lnAltCnt )
      object->recData.recRaster.Raster[REC_MAX_RASTER_SIZE-1] =
               stdAnsiToAscii(object->recResults.Alt[0].Code);
prop = leo_wide_object(object);
ver = object->recResults;

if( ver.lnAltCnt )
{
pen_prop = leo_proport_penalty(ver.Alt[0].Code, ver.Alt[0].Prob, object->recData.recRaster.lnPixWidth,
    object->recData.recRaster.lnPixHeight);
}
if( prop<33 || pen_prop>10 )
    {
    Bool32 nar=(object->recData.recRaster.lnPixWidth*2<
    object->recData.recRaster.lnPixHeight);
    if( prop<33 )
        {
        for(i=0;i<ver.lnAltCnt;i++)
            {
            if( nar && strchr(let_narrow,ver.Alt[i].Code) )
                continue;
            ver.Alt[i].Prob>>=1;
            if( ver.Alt[i].Prob<1 )
             ver.Alt[i].Prob=1;
            }
        leo_sort_vers_prob(&ver);
        }
    else
        {
        for(i=0;i<ver.lnAltCnt;i++)
            {
            if( nar && strchr(let_narrow,ver.Alt[i].Code) )
                continue;
            if( ver.Alt[i].Prob>pen_prop )
                ver.Alt[i].Prob-=pen_prop;
            else
                ver.Alt[i].Prob=1;
            }
        leo_sort_vers_prob(&ver);
        if( ver.Alt[0].Prob==1 )
            ver.lnAltCnt=0;
        }

    object->recResults = ver;
    if (!SnpSkip(&stnCharRecog) || leo_Snp_In_Rect )
        SnpLog("LEO BAD PROPORT MONUS: ");
    }

}


n = object->recResults.lnAltCnt;
for(i=0;i<n;i++)
    if( object->recResults.Alt[i].Prob==1 )
            object->recResults.Alt[i].Prob=2;
id_rast=0;
id_alph=LEO_VALID_NONE    ;
switch( leo_alpha_type )
    {
    case    ALPH_DIG    :
        id_alph=LEO_VALID_DIGIT   ;
        break;
    case    ALPH_RUS    :
        id_alph=LEO_VALID_RUSSIAN ;
        break;
    case    ALPH_ROM    :
        id_alph=LEO_VALID_LATIN   ;
        break;
    case    ALPH_ALL    :
        id_alph=LEO_VALID_ALL     ;
        break;
    default             :
        id_alph=LEO_VALID_NONE    ;
        break;
    }
if(leo_enable_stored )
    id_rast=leo_cont_store(&object->recData.recRaster,
                 (uchar)(n?object->recResults.Alt[0].Code:'~'),
                 (uchar)object->recData.lwCompCnt,
                 &object->recData.rect, (uchar)(leo_typ_of_font|0x01),
                 (uchar)(n?object->recResults.Alt[0].Prob:0),
                 (uchar)id_alph, &object->recResults,LEO_CONTROL_NONE);
object->recResults.Alt[0].Info = id_rast;
for(i=0;i<n;i++)
    object->recResults.Alt[i].Info = id_rast;
if (!SnpSkip(&stnCharRecog)|| leo_Snp_In_Rect)
      {
         char buf[256];
         memcpy(&ver,&object->recResults,sizeof(RecVersions));
         leo_snapRes2Str(&ver,buf);

         leo_snapRaster(object, &stnCharRecog);
         SnpLog("LEO PRN LTR : %s", buf);
         SnpLog("%s","");
         Leo_SnpWaitUserInput(&stnCharRecog);
		 SnpHideRects( (uint32_t)&stnCharRecog );
      }

if (!SnpSkip(&stnSnapCharProt))
    {
	uchar let='~';
    memcpy(&ver,&object->recResults,sizeof(RecVersions));
	if( ver.lnAltCnt )
       let = stdAnsiToAscii(ver.Alt[0].Code);
    CTB_AddRecRaster("sqprot", &object->recData.recRaster , let);
	}
leo_Snp_In_Rect=FALSE;
return TRUE;
}

static Bool32 leo_choise_fon_or_leo_absent(uchar p_fon,uchar p_leo)
{
if( p_fon>200 )
    {
	if( p_leo<240 && p_fon>254 )
        return TRUE; // heigh FON & low LEO
	if( p_leo<220 && p_fon>253 )
        return TRUE; // heigh FON & low LEO
    if( p_leo<180 && p_fon>250 )
        return TRUE; // heigh FON & low LEO
	if( p_leo<140 && p_fon>220 )
        return TRUE; // heigh FON & low LEO
	if( p_leo<100 && p_fon>200 )
        return TRUE; // heigh FON & low LEO
    return FALSE; // low LEO or heigh FON
    }
return FALSE; // low FON
}

static Bool32 leo_choise_fon_or_leo(uchar p_fon,uchar p_leo)
{
if( p_fon>200 )
    {
    if( p_leo>253  )
        return FALSE; // heigh LEO & low FON
    if( p_leo>240 && p_fon<240 )
        return FALSE; // heigh LEO & low FON
    if( p_leo>220 && p_fon<230 )
        return FALSE; // heigh LEO & low FON
    if( p_leo>200 && p_fon<200 )
        return FALSE; // heigh LEO & low FON
    return TRUE; // low LEO or heigh FON
    }
return FALSE; // low FON
}

static Bool32 leo_near_letters(RecVersions *fon,uchar leo_code)
{
if( fon->lnAltCnt<2 )
    return FALSE;
if( !(leo_strchr_codes_ansi("0оО",  fon->Alt[0].Code) &&
      leo_strchr_codes_ansi("0оО",  leo_code) &&
      leo_strchr_codes_ansi("0оО",  fon->Alt[1].Code)) &&
    !(leo_strchr_codes_ansi("3зЗ",  fon->Alt[0].Code) &&
      leo_strchr_codes_ansi("3зЗ",  leo_code) &&
      leo_strchr_codes_ansi("3зЗ",  fon->Alt[1].Code)) &&
    !(leo_strchr_codes_ansi("пПлЛ", fon->Alt[0].Code) &&
      leo_strchr_codes_ansi("пПлЛ", leo_code) &&
      leo_strchr_codes_ansi("пПлЛ", fon->Alt[1].Code)) &&
    !(leo_strchr_codes_ansi("иИнН", fon->Alt[0].Code) &&
      leo_strchr_codes_ansi("иИнН", leo_code) &&
      leo_strchr_codes_ansi("иИнН", fon->Alt[1].Code)) &&
    !(leo_strchr_codes_ansi("б6", fon->Alt[0].Code) &&
      leo_strchr_codes_ansi("б6", leo_code) &&
      leo_strchr_codes_ansi("б6", fon->Alt[1].Code)))
    return FALSE;
return (fon->Alt[0].Prob-fon->Alt[1].Prob<5);
}

static void data2RecVersions( uchar *data, RecVersions *ver)
{
int32_t k;
ver->lnAltCnt        = data[16];
ver->Alt[0].Code     = data[3];
ver->Alt[0].Prob     = data[14];
ver->Alt[0].Method   = data[28];
for(k=1;k<ver->lnAltCnt;k++)
  {
  ver->Alt[k].Code     = data[17+k*2];
  ver->Alt[k].Prob     = data[18+k*2];
  ver->Alt[k].Method   = data[28+k];
  }
return;
}
static void RecVersions2data(  RecVersions *ver, uchar *data)
{
int32_t k;
 data[16]= (uchar)ver->lnAltCnt;
 data[3] = ver->Alt[0].Code    ;
 data[14]= ver->Alt[0].Prob    ;
 data[28]= ver->Alt[0].Method  ;
for(k=1;k<ver->lnAltCnt;k++)
  {
  data[17+k*2]= ver->Alt[k].Code     ;
  data[18+k*2]= ver->Alt[k].Prob     ;
  data[28+k]  = ver->Alt[k].Method   ;
  }
return;
}



// выбор и рспознавание
#define POROG_NICE    250
#define VERY_GOOD_FON 245
//////////////////
// установить новую оценку
static int GetNewProb(int oldRec,int newRec,int nice)
{
 int addRec=2;

    if(oldRec >= 254)
      return oldRec;

	if( newRec < 220 )
		return oldRec;

	if( newRec > VERY_GOOD_FON)
	{
		addRec = 30;
		newRec = MAX(newRec,nice);
	}
	else if(newRec > 240)
		addRec = 20;
    else if(newRec > 230)
		addRec = 10;
	else addRec = 5;

	return MIN(254,MAX(newRec,oldRec+addRec));
}
///////////
// надо взять иную первую альтернативу ?
// (уже знаем, что новая - неплохая, иначе сюда и не идем)
static Bool32 FonIsBetter(int newProb,int oldProb)
{
 if( newProb <= 100)
	 return FALSE;
 if( newProb > oldProb + 30)
	 return TRUE;
 if( newProb > VERY_GOOD_FON && newProb > oldProb + 20)
	 return TRUE;
 return FALSE;
}
//////////////
// Функция выбора - я пока в экспериментах менял
// только первую альтернативу и/или оценку

Bool32 LEO_SelectOldNewOkr(RecVersions *verOld,RecVersions *verNew)
{
Bool32  ret=FALSE;
uchar   oldprob;
  if( verOld->Alt[0].Code != verNew->Alt[0].Code )
  {
//	  fprintf(basOut," MISS");
// Новая альтернатива достойна внимания ?
	  if( ( verNew->lnAltCnt == 1 ||
		   verNew->lnAltCnt > 1 && verNew->Alt[0].Prob > verOld->Alt[1].Prob + 40 ) &&
		   FonIsBetter(verNew->Alt[0].Prob, verOld->Alt[0].Prob) // data[14]
        )
	  {
//      fprintf(basOut," NewName (prob=%d)",ver.Alt[0].Prob);
        verOld->Alt[0]=verNew->Alt[0];
        ret=TRUE;
	  }

	  return ret;
  }

// версии совпали - изменить (возможно) оценку
 oldprob=verOld->Alt[0].Prob;
  verOld->Alt[0].Prob = GetNewProb(verOld->Alt[0].Prob,verNew->Alt[0].Prob,POROG_NICE);
  if( oldprob!=verOld->Alt[0].Prob )
    ret=TRUE;
  return ret;
}
/////////////



LEO_FUNC(Bool32) LEOFonRerecogCTB(char *CTBname)
{
CTB_handle	hnd;
int			i,n,k;
RecRaster	r;
uchar		data[CTB_DATA_SIZE], let;
RecVersions ver={0}, old={0};
FontInfo	fontinfo;
Bool32		enable_let, enable_correct_case, local_snap;
int32_t		old_case ;
FonSpecInfo specInfo = {0};
Rect16		r16;

if(!leo_enable_stored)
    return FALSE;
if( FONInit(".\\tmp\\page.ctb")<1 )
    return FALSE;

if( !CTB_open(CTBname,&hnd,"w") )
    return FALSE;
FONFontInfo(&fontinfo);
n=hnd.num;
for(i=0;i<n;i++)
	{
    if( CTB_ReadRecRaster(&hnd, i , &let,&r, data ) )
        {
        if( !(data[15]&LEO_VALID_FINAL) )
            {
            data[3]='~';
            CTB_write_data(&hnd,i, data);
            continue;
            }
        // final result
		  memcpy(&r16,&data[6],sizeof(Rect16));
		  local_snap=leoSnpInRect(&r16, 0);
        if( data[5]==0 && (data[15]&LEO_VALID_DIGIT)  )
            {
            data2RecVersions(data,&old);
            if( old.lnAltCnt && old.Alt[0].Prob>=250 )
                {
                if (/*!SnpSkip(&stnReRecog)||*/ local_snap)
					{
					leo_snapChar(&old,"PASS1.51 NICE LEO COLLECTION : ",TRUE);
					SnpDrawRect(&r16,0,wRGB(0,0,255),-16,(uint32_t)&stnReRecog    );
					SnpDrawRaster( &r );
					Leo_SnpWaitUserInput(&stnReRecog);
					}
                continue;
                }
            if (!SnpSkip(&stnReRecog)|| local_snap)
					{
					leo_snapChar(&old,"PASS1.51 LEO COLLECTION : ",TRUE);
					SnpDrawRect(&r16,0,wRGB(0,0,255),-16,(uint32_t)&stnReRecog    );
					SnpDrawRaster( &r );
					Leo_SnpWaitUserInput(&stnReRecog);
					}
            FONSetAlphabet(alphabet_dig);

            enable_correct_case =  ((data[27]&LEO_CONTROL_CASE)==0);
            enable_let = (  fontinfo.count[data[3]]!=0 && FONCheckItself(data[3],i+1,-1,-1));
            memset((uchar*)&specInfo,0,sizeof(specInfo));
            specInfo.nInCTB    = i+1;
            specInfo.nFieldRow = data[25];
            specInfo.nLet      = data[3];
            specInfo.wFonFlags = 0;
            ver.lnAltCnt=0;
            FONRecogOkr(&r,&ver,&specInfo,2,70);
            if( LEO_SelectOldNewOkr(&old,&ver) )
                {
                data[3] = old.Alt[0].Code    ;
                data[14]= old.Alt[0].Prob    ;
                data[28]= REC_METHOD_FON  ;
                CTB_write_data(&hnd,i, data);
                if (!SnpSkip(&stnReRecog)|| local_snap)
                    {
                    leo_snapChar(&old,"PASS1.51 FNT RERECOG DIGITAL HAND : ",TRUE);
				    SnpDrawRect(&r16,0,wRGB(0,0,255),-16,(uint32_t)&stnReRecog    );
				    SnpDrawRaster( &r );
				    Leo_SnpWaitUserInput(&stnReRecog);
                    }
                }
            else if (!SnpSkip(&stnReRecog)|| local_snap)
                {
                leo_snapChar(&ver,"PASS1.51 FNT UNKNOWN CASE FOR DIGITAL HAND : ",TRUE);
				SnpDrawRect(&r16,0,wRGB(0,0,255),-16,(uint32_t)&stnReRecog    );
				SnpDrawRaster( &r );
				Leo_SnpWaitUserInput(&stnReRecog);
                }
            continue;
            }
        if( data[5] && !(data[15]&LEO_VALID_LINGVO) )
            { // Print text & non CNT valid
            data2RecVersions(data,&old);
            if (!SnpSkip(&stnReRecog)|| local_snap)
					{
					leo_snapChar(&old,"PASS1.5 LEO COLLECTION : ",TRUE);
					SnpDrawRect(&r16,0,wRGB(0,0,255),-16,(uint32_t)&stnReRecog    );
					SnpDrawRaster( &r );
					Leo_SnpWaitUserInput(&stnReRecog);
					}
            if( data[26] )
                FONSetAlphabet(leo_alpha_reg[data[26]]);
            else
                FONSetAlphabet(alphabet_all);

            enable_correct_case =  ((data[27]&LEO_CONTROL_CASE)==0);
            enable_let = (  fontinfo.count[data[3]]!=0 && FONCheckItself(data[3],i+1,-1,-1));
            memset((uchar*)&specInfo,0,sizeof(specInfo));
            specInfo.nInCTB    = i+1;
            specInfo.nFieldRow = data[25];
            specInfo.nLet      = data[3];
            specInfo.wFonFlags = 0;
            ver.lnAltCnt=0;

            FONRecogChar(&r,&ver,&specInfo);

			   if (!SnpSkip(&stnReRecog)|| local_snap)
				   {
				   leo_snapChar(&ver,"PASS1.5 FNT COLLECTION : ",TRUE);
				   SnpDrawRect(&r16,0,wRGB(0,0,255),-16,(uint32_t)&stnReRecog    );
				   SnpDrawRaster( &r );
				   Leo_SnpWaitUserInput(&stnReRecog);
				   }
            if( FONGetFontCount()==1 )
                enable_correct_case = FALSE;

            if( // best recog
                ver.lnAltCnt &&
                leo_choise_fon_or_leo(ver.Alt[0].Prob,data[14]) &&
                !(ver.lnAltCnt>1 && ver.Alt[0].Prob>253 && data[14]>253 && leo_comp_codes(ver.Alt[1].Code,data[3])) && // high prop
                (enable_let||leo_choise_fon_or_leo_absent(ver.Alt[0].Prob,data[14])) &&
                !leo_near_letters(&ver,data[3]) )
                { // storing FON collection
                old_case = stdLeoIsCase(data[3]);
                if( enable_correct_case )
                   {
                   switch( old_case )
                       {
                       case STD_LEO_CASE_UPPER:
                           for(k=0;k<ver.lnAltCnt;k++)
                               ver.Alt[k].Code  = stdLeoSetUpperCase(ver.Alt[k].Code);
                           break;
                       case STD_LEO_CASE_LOWER:
                           for(k=0;k<ver.lnAltCnt;k++)
                               ver.Alt[k].Code  = stdLeoSetLowerCase(ver.Alt[k].Code);
                           break;
                       default:
                           break;
                       }
                   }
                if( ver.lnAltCnt>1 )
                    { // expert LEO
                    if( ver.Alt[0].Prob==ver.Alt[1].Prob )
                        {
                        if( ver.Alt[1].Code==data[3] )
                            {
                            ver.Alt[1].Code=ver.Alt[0].Code;
                            ver.Alt[0].Code=data[3];
                            }
                        for(k=1;k<ver.lnAltCnt;k++)
                            {
                            ver.Alt[k].Prob--;
                            }
                        }

                    }
                if( data[3] == ver.Alt[0].Code &&
                        !(ver.lnAltCnt>1 && ver.Alt[0].Prob-ver.Alt[1].Prob<15))
                    {
                    if( ver.Alt[0].Prob>240 )
                        ver.Alt[0].Prob=255;
                    else
                        ver.Alt[0].Prob=MAX(data[3], ver.Alt[0].Prob);
                    data[15] |= LEO_VALID_FONT;
                    data[27] |= LEO_CONTROL_FON_CONFIRMED;
                    }
                else
                    {
                    for(k=0;k<ver.lnAltCnt;k++ )
                        if( ver.Alt[k].Prob>1 )
                            ver.Alt[k].Prob--;
                    }

                if( old.lnAltCnt && ver.lnAltCnt &&
                    old.Alt[0].Prob>ver.Alt[0].Prob )
                    leo_set_max_vers(&old,ver.Alt[0].Prob-1);

                leo_add_vers(&ver,&old);
                leo_sort_vers_prob( &ver );
                leo_compress(&ver);
                if( data[15] & LEO_VALID_FONT )
                  data[27] |= LEO_CONTROL_FON_CONFIRMED;
                else
                  data[27] |= LEO_CONTROL_FON_CHANGE;
                if( ver.lnAltCnt>4 )
                    ver.lnAltCnt=4;
				    if (!SnpSkip(&stnReRecog)|| local_snap)
					   {
					   if( data[15] & LEO_VALID_FONT )
                           leo_snapChar(&ver,"PASS1.5 FNT+LEO CONFIRMING : ",TRUE);
					   else
                           leo_snapChar(&ver,"PASS1.5 FNT+LEO CHANGING : ",TRUE);
					   SnpDrawRect(&r16,0,wRGB(0,0,255),-16,(uint32_t)&stnReRecog    );
					   SnpDrawRaster( &r );
					   Leo_SnpWaitUserInput(&stnReRecog);
					   }
                RecVersions2data(&ver,data);
                }
            else if( data[14]>253 && ver.lnAltCnt && //leo_comp_codes(ver.Alt[0].Code,data[3]) )
                     ver.Alt[0].Code==data[3] && // LEO[0]==FON[0]
                     !(ver.lnAltCnt>1 && ver.Alt[0].Prob-ver.Alt[1].Prob<15) )
				   {
               data[14]=255;
               data[15] |= LEO_VALID_FONT;
               data[27] |= LEO_CONTROL_FON_CONFIRMED;
				   if (!SnpSkip(&stnReRecog)|| local_snap)
					   {
                       data2RecVersions(data,&ver);
					   leo_snapChar(&ver,"PASS1.5 FNT+LEO CONFIRMING : ",TRUE);
					   Leo_SnpWaitUserInput(&stnReRecog);
					   }
				   }
            else if( data[14]>1 && data[14]!=255 )
				   {
               data[14]--;
               data[27] |= LEO_CONTROL_FON_ALERT;
               if( leo_near_letters(&ver,data[3]) )
                    data[27] |= LEO_CONTROL_FON_TWIN;
               for(k=0;k<old.lnAltCnt;k++)
                    {
                    if( data[18+k*2] )
                     data[18+k*2]--;
                    }
				   if (!SnpSkip(&stnReRecog)|| local_snap)
					   {
                  data2RecVersions(data,&ver);
                  if( data[27] &= LEO_CONTROL_FON_TWIN )
                     leo_snapChar(&ver,"PASS1.5 FNT+LEO ALERTING&TWIN : ",TRUE);
                  else
                     leo_snapChar(&ver,"PASS1.5 FNT+LEO ALERTING : ",TRUE);
                  Leo_SnpWaitUserInput(&stnReRecog);
					   }
				   } // end of last elseif
            CTB_write_data(&hnd,i, data);
            } // end of if non CNT valid
        } // end of if read
} // end of cykle
CTB_close(&hnd);
FONDone();

return TRUE;
}

// return quality if validation, 0 if error
LEO_FUNC(uchar) LEOValidRestore_Char( RecVersions *resin,RecVersions *resout)
{
RecRaster   rr;
int16_t       idr;
uchar       nLns;
Rect16      rect;
uchar       IsPrint;
uchar       data[CTB_DATA_SIZE],ltr,prb, vld;
int32_t       id_page = LEO_CONT_STD_PAGE_ID, i;

LEO_error_code=ER_LEO_NO_ERROR;
if(!leo_enable_stored)
    return 0;
resout->lnAltCnt=0;
resout->lnAltMax=REC_MAX_VERS;
if( !resin->lnAltCnt || resin->Alt[ 0 ].Info==0 )
    return 0;

idr = resin->Alt[0].Info;
if( !leo_cont_restore(&rr, id_page, idr, &nLns, &rect, &IsPrint, data) )
    return 0;
ltr = data[3];
prb = data[14];
vld = data[15];

if( ltr!='~' )
   {
   resout->Alt[0].Code=ltr;
   resout->Alt[0].Prob=prb;
   resout->Alt[0].Method=data[28];
   resout->Alt[0].CodeExt=0;
   resout->Alt[0].Info=idr;
   if( !resout->lnAltCnt )
      resout->lnAltCnt++;
   if( data[16] )
        {
        resout->lnAltCnt = data[16];
        for(i=1;i<resout->lnAltCnt;i++)
            {
            resout->Alt[i].Code  =data[17+2*i];
            resout->Alt[i].Prob  =data[18+2*i];
			resout->Alt[i].Method=data[28+i];
            resout->Alt[i].CodeExt=0;
            resout->Alt[i].Info=idr;
            }
        }

   }

leo_Snp_In_Rect=leoSnpInRect(&rect, 0);
if (!SnpSkip(&stnCharRecog)|| leo_Snp_In_Rect)
      {
        RecObject object;
        RecVersions ver;
        char buf[256];

         ver = *resout;
         for(i=0;i<ver.lnAltCnt;i++)
            ver.Alt[i].Code = stdAsciiToAnsi(ver.Alt[i].Code);
         leo_snapRes2Str(&ver,buf);
         memcpy(&object.recData.recRaster,&rr,sizeof(RecRaster));
         memcpy(&object.recData.rect,&rect,sizeof(Rect16));

         if(object.recResults.lnAltCnt )
            object.recData.recRaster.Raster[REC_MAX_RASTER_SIZE-1] =
               stdAnsiToAscii(object.recResults.Alt[0].Code);

         leo_snapRaster(&object, &stnCharRecog);
         leo_store_for_pass2(&object,
            (uchar)(ver.lnAltCnt ? ver.Alt[0].Code : (uchar)'0'));
         SnpLog("LEO PRN RERECOG FNT LTR (num=%d): %s", idr-1, buf);
         SnpLog("%s","");
         Leo_SnpWaitUserInput(&stnCharRecog); // pass control to user
		 SnpHideRects( (uint32_t)&stnCharRecog );
      }
return data[27]&(~LEO_CONTROL_CASE) ;
}


LEO_FUNC(Bool32) LEOSetValid(int16_t id_rast, uchar code, uchar valid,uchar control)
{
if(!leo_enable_stored)
    return FALSE;
return leo_cont_set_valid(LEO_CONT_STD_PAGE_ID, id_rast, stdAnsiToAscii(code), valid, control);
}

LEO_FUNC(Bool32) LEODelFinal(int16_t id_rast)
{
if(!leo_enable_stored)
    return FALSE;
return leo_cont_del_final(LEO_CONT_STD_PAGE_ID, id_rast);
}


LEO_FUNC(Bool32) LEORecogCharPRN_expert(RecObject*  object)
{

leo_Snp_In_Rect=leoSnpInRect(&object->recData.rect, 0);
LEO_error_code=ER_LEO_NO_ERROR;
if( !object )
	{
	LEO_error_code=ER_LEO_NULL_OBJECT;
	return FALSE;
	}
if( no_init_prn )
    {
    if( !leo_init_prn() )
        return FALSE;
    no_init_prn=FALSE;
    }
if(object->recData.recRaster.lnPixWidth<5 ||
   object->recData.recRaster.lnPixHeight<5 )
	{
	LEO_error_code=ER_LEO_SMALL_OBJECT;
	if (!SnpSkip(&stnCharRecog)|| leo_Snp_In_Rect)
			{

			SnpDrawRect(&object->recData.rect,
                      0, //Skew - zero to real coords
                     wRGB(255,0,0),
                     -16, // one image pixel width
                     (uint32_t)&stnCharRecog
                    );
			SnpDrawRaster( &object->recData.recRaster );
			SnpLog("%s","TOO SMALL RASTER");
			Leo_SnpWaitUserInput(&stnCharRecog); // pass control to user
			SnpHideRects((uint32_t)&stnCharRecog);
			}
	return FALSE;
	}
if(object->recData.recRaster.lnPixWidth>150 ||
   object->recData.recRaster.lnPixHeight>150 )
	{
	LEO_error_code=ER_LEO_LARGE_OBJECT;
	if (!SnpSkip(&stnCharRecog)|| leo_Snp_In_Rect)
			{

			SnpDrawRect(&object->recData.rect,
                      0, //Skew - zero to real coords
                     wRGB(255,0,0),
                     -16, // one image pixel width
                     (uint32_t)&stnCharRecog
                    );
			SnpDrawRaster( &object->recData.recRaster );
			SnpLog("%s","TOO BIG RASTER");
			Leo_SnpWaitUserInput(&stnCharRecog); // pass control to user
			SnpHideRects((uint32_t)&stnCharRecog);
			}

	return FALSE;
	}


leo_decode_to_acsii(&object->recResults);
leoRecogCharPRN_expert(&object->recData.recRaster, &object->recResults);

if (!SnpSkip(&stnCharRecog)|| leo_Snp_In_Rect)
      {
         char buf[256];
         RecVersions ver;

         memcpy(&ver,&object->recResults,sizeof(RecVersions));
         leo_snapRes2Str(&ver,buf);

         leo_snapRaster(object, &stnCharRecog);
         SnpLog("EXPERT PRN LTR : %s", buf);
         SnpLog("%s","");
         Leo_SnpWaitUserInput(&stnCharRecog); // pass control to user
         SnpHideRects( (uint32_t)&stnCharRecog );
      }

return TRUE;
}

LEO_FUNC(int16_t) LEO_ContStore(RecRaster *r,uchar let, uchar nLns,Rect16 *rect,uchar IsPrint,
		uchar	Prob, uchar Valid, RecVersions *Res)
{
if(!leo_enable_stored)
    return 0;

return  leo_cont_store(r,let, nLns, rect, IsPrint, Prob, Valid, Res,LEO_CONTROL_NONE);
}

LEO_FUNC(int32_t)     LEO_GetGlobalIncline(void)
{
return leo_get_global_incline();
}

LEO_FUNC(Bool32) LEO_ContRestoreObject(int16_t       idr, RecObject *object,
                                            uchar *alpha, uchar *isPrint)
{
uchar       nLns;
Rect16      rect;
uchar       data[CTB_DATA_SIZE];

if(!leo_enable_stored)
    return FALSE;
if( !leo_cont_restore(&object->recData.recRaster, LEO_CONT_STD_PAGE_ID, idr,
                      &nLns, &rect, isPrint, data) )
    return FALSE;
object->recData.lwCompCnt = nLns;
*alpha = data[15];
memcpy(&object->recData.rect,&data[6],sizeof(Rect16));
return TRUE;
}

LEO_FUNC(Bool32)  LEORecogCharDebug( RecObject*  object )
{
leo_cuneiform(object);

leo_decode_to_ansi( &object->recResults );
return TRUE;
}

LEO_FUNC(int16_t) LEO_GetNoCutPoint(uchar *RASTER, int16_t WB, uchar NWIDTH, uchar NLENGTH)
{
return DIF_GetNoCutPoint(RASTER,  WB, NWIDTH, NLENGTH);
}

LEO_FUNC(Bool32) LEO_StoreCollection(RecVersions *ver)
{
   return leo_cont_store_collection(ver);
}

LEO_FUNC(Bool32) LEO_GetStoringMode(void)
{
return leo_enable_stored;
}

LEO_FUNC(void)  LEO_SetStoringMode(Bool32 Mode)
{
leo_enable_stored = Mode;
return;
}

LEO_FUNC(Bool32)  LEORecogChar(RecObject* obj  )
{
    return FALSE;
}

LEO_FUNC(Bool32)  LEORecogChar_expert(RecObject*  object)
{
    return FALSE;
}

LEO_FUNC(Bool32)  LEORecogId_expert(int32_t id_page,RecVersions *exp)
{
    return FALSE;
}

LEO_FUNC(Bool32)  LEORecogId_Char(int32_t id_page, RecVersions *resin, RecVersions *resout)
{
    return FALSE;
}

LEO_FUNC(Bool32)  LEOValidId_Char(int32_t id_page, RecVersions *resin, RecVersions *resout)
{
    return FALSE;
}

LEO_FUNC(Bool32)  LEORecogRestore_Char(RecVersions *resin, RecVersions *resout)
{
    return FALSE;
}

LEO_FUNC(Bool32)  LEORecogHndMethod( RecObject*  object , int Method, int gra_type)
{
    return FALSE;
}
