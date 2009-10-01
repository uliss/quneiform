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

#define _USE_GLUE_BOX_

 #include <stdlib.h>
 #include <sys/stat.h>
/* #include <io.h>*/

 
 #include <setjmp.h>
 #include <assert.h>
 #include <stdio.h>
 #include <fcntl.h>
/* #include <direct.h>*/
 #include <string.h>

#include  "std.h"
#include "globus.h"
#include "imgaccs.h"
#include "resource.h"
#include "func.h"
#include "alphabet.h"
#include "status.h"
#include "ligas.h"
#include "linear.h"
#include "linutil.h"
#include "p2libr.h"
#include "leo.h"
#include "tm.h"
#include "lang.h"
#include "ccom.h"
#include "struct.h"
#include "cstr.h" // OLEG
#include "fon.h"
#include "dpuma.h"
#include "rsadd.h"

#include "lang.h"
#include "minmax.h"

//#include "p2.h"
//#include "rstr_p2.h"

static int p2_checkUpperLower(void);

//======== EXTERN FUNCTIONS
Bool _spell(pchar s,uchar lang);
void clear_cells(void);
int cell2UniVers(UniVersions *ver, cell *c); // pass3
int CopyVers2CSTR(RecVersions *ver, cell *c);    // pass3

//======== EXTERN BUFFERS
extern Handle hSnapLEO;
extern puchar kit_curr, kit_end;
extern uchar decode_ASCII_to_[256][4];
extern uchar CodePages[];
extern uchar fon_alphabet_language[3][256];

extern uchar line_alphabet;
extern int16_t  line_number;
extern uchar line_scale;
extern uchar line_tabcell;//OLEG
extern int16_t  nIncline;   //
////////////

static Bool32 p2_twin(uchar ch);
static int recogGlueRaster( const s_glue * gl,
						    RecVersions *vers );

#define PROPMAX   25
///////////////
Bool32 p2_accept_Cell( cell *c,CSTR_rast_attr *rst, CCOM_comp *cmp, int16_t scale)
{
 c_comp ec={0};
 int16_t shift=scale?scale:0;
 uint16_t zero=0;

 if( c )
    c->env=0;

 if( !cmp )
    return FALSE;

 if (cmp->type != ch_great)
 {
  ec.size     = sizeof(c_comp)+cmp->size_linerep +2;
  if( kit_curr+ec.size >= kit_end )
    {
     // ErrorExit(RSTR_ERR_NOPLACE); - // Nick 03.04.2002
     return FALSE;
    }

  ec.upper    = cmp->upper>>shift;
  ec.left     = cmp->left>>shift;
  ec.h        = cmp->h>>shift;
  ec.w        = cmp->w>>shift;
  ec.rw       = ((ec.w+7)/8);
  ec.type     = cmp->type;
  ec.lines    = sizeof(c_comp);
  ec.nl       = cmp->nl;
  ec.begs     = cmp->begs;
  ec.ends     = cmp->ends;
  ec.large    = cmp->large;
  ec.scale    = cmp->scale;

  if( c )
      c->env      = (c_comp*)kit_curr;

  memcpy(kit_curr, &ec, sizeof(c_comp));
  kit_curr += sizeof(c_comp);
  memcpy(kit_curr, cmp->linerep, cmp->size_linerep);
  kit_curr += cmp->size_linerep;
  memcpy(kit_curr, &zero, 2);
  kit_curr += 2;
 }

 if( c )
 {
  c->h       =cmp->h>>shift;
  c->w       =cmp->w>>shift;
 }

 return TRUE;
}
///////////////////
static void AttrCstr2Cell(cell *attr,CSTR_rast_attr *c)
{
        attr->row   = c->row     ;
        attr->col   = c->col     ;
        //attr->w     = c->w       ;
        //attr->h     = c->h       ;
        attr->r_row = c->r_row   ;
        attr->r_col = c->r_col   ;

        attr->bdiff = c->bdiff   ;
        attr->difflg= c->difflg  ;
        attr->basflg= c->basflg  ;
        attr->bas1  = c->bas1    ;
        attr->bas2  = c->bas2    ;
        attr->bas3  = c->bas3    ;
        attr->bas4  = c->bas4    ;
        attr->cg_flag=c->cg_flag ;
        attr->cpos  = c->cpos   ;
        attr->bas_acc= c->bas_acc   ;
        attr->dlang_dup = c->dlang_dup; // oleg
        attr->flg    = c->flg ;

                // проверка флагов - бывали бяки в сырой строке
        if( attr->flg == c_f_solid ||
                        attr->flg == c_f_confirmed  )
                        attr->flg = c_f_bad;

                if( attr->flg & c_f_dust )
            attr->flg &= ~(c_f_punct|c_f_let|c_f_bad);
                else if( attr->flg & c_f_punct )
              attr->flg &= ~(c_f_let|c_f_bad);
                else if( attr->flg  & c_f_bad )
              attr->flg &= ~(c_f_let);

        attr->reasno = c->reasno;
        attr->keg = c->keg;
        attr->font = c->font ;
        attr->font_new = c->font_new ;
        attr->dens = c->dens ;
        attr->recsource = c->recsource ;
        attr->history =   0;
        if( c->RecogHistory & CSTR_hi_evn)
            attr->history |=    c_rs_ev;
        if( c->RecogHistory & CSTR_hi_3x5)
            attr->history |=    c_rs_BOX;
        if( c->RecogHistory & CSTR_hi_3x5s )
            attr->history |=   c_rs_BOXs;
        if( c->RecogHistory & CSTR_hi_desk )
            attr->history |=   c_rs_deskr ;
        if( c->RecogHistory & CSTR_hi_fon )
            attr->history |=    c_rs_bitcmp;
                if( c->RecogHistory & CSTR_hi_leo )
            attr->history |=    c_rs_LEO;
        attr->stick_inc = c->stick_inc ;
                attr->save_stick_inc = c->save_stick_inc ;
        attr->bas_acc = c->bas_acc ;
        attr->left = c->left ;
        attr->right = c->right ;
        attr->shape = c->shape ;
        attr->rus_eng_word = c->rus_eng_word ;
        attr->broken_II = c->broken_II ;
        attr->language = c->language ;
        attr->pos_inc  = c->pos_inc;
                attr->clink    = c->clink;
                attr->clu_attr = c->clu_attr;
                attr->nClust   = c->nClust;
        attr->flg_new  = c->flg_new;
		// Nick 20.02.2001
		attr->cg_flag_fine = c->cg_flag_fine;

		// Nick 11.03.2002
		attr->accent_leader = c->accent_leader;
		attr->bottom_accent = c->bottom_accent;
  return;
}

void p2_insert_cell(cell *c,cell *prev)
{
cell *next=cell_l();
prev->next = c;
next->prev = c;
c->next = next;
c->prev = prev;
if( c->flg&(c_f_let|c_f_bad) )
    {
    prev = next->prevl;
    prev->nextl = c;
    next->prevl = c;
    c->nextl = next;
    c->prevl = prev;
    }
return;
}
////////////////
//
// перевести в cell от first < last
// if(first == NULL ) -> first= первый в lin
//
int32_t p2_Cstr2Cell( CSTR_line lin,CSTR_rast first,CSTR_rast last,Bool32 needVers, uint32_t disable )
{
 cell           *c1,*c2, *next;
 CSTR_rast_attr  cur;
 CSTR_rast       curr;
 CCOM_comp      *cmp;
 int16_t             i;
 UniVersions     evn;
 int32_t           numCell;

 if( first == NULL )
 {
  if( lin == (CSTR_line)NULL )
          return 0;

  first=CSTR_GetFirstRaster(lin);
  last =CSTR_GetLastRaster (lin);
  if(!first)
          return 0;

  first=CSTR_GetNext(first);
  if(!first)
          return 0;
 }

 // main circle
 c1=cell_f();
 for (numCell=0,curr=first; curr&&curr!=last; curr=CSTR_GetNextRaster (curr, CSTR_f_all))
  {
  CSTR_GetAttr(curr,&cur);

  if(cur.flg & disable )
          continue;

  cmp=CSTR_GetComp(curr);
  CSTR_GetCollectionUni(curr,&evn);
  if( (cur.flg&CSTR_f_space) && evn.lnAltCnt )
    {
    c2=new_cell();
    c2->nvers=1;
    c2->row=c2->col=c2->r_row=c2->r_col=c2->w=c2->h=0;
    c2->flg=c_f_space;
    c2->vers[1].let=0;
    switch( evn.Alt[0].Prob )
        {
        case    254:
            c2->vers[0].let=' ';
            break;
        case    150:
            c2->vers[0].let=SS_POS_HALF_SPACE;
            break;
        case    75:
            c2->vers[0].let=SS_NEG_HALF_SPACE;
            break;
        }
    c2->vers[0].prob = evn.Alt[0].Prob;
    next = c1->next;
    c1->next=next->prev=c2;
    c2->next=next; c2->prev=c1;
    c1=c2;
    numCell++;
    continue;
    }
  if( !cmp )
          continue;

  {  // test cmp - плохие не берем
      if ( (cmp->type&CCOM_CH_GREAT) || cmp->scale>2-1*0)
           continue;
      if ( cur.h>PROPMAX*cur.w)
            continue;
      if (!cmp->scale && (cur.w > RASTER_MAX_WIDTH ||  cur.h > RASTER_MAX_HEIGHT )     )
            continue;
      if ( cmp->scale && ((cur.w>>line_scale) > RASTER_MAX_WIDTH ||
                          (cur.h>>line_scale) > RASTER_MAX_HEIGHT )     )
            continue;
  }


  // проверим - есть ли место !
  if( cmp && cmp->type != ch_great)
  {
   // Nick 03.04.2002 если нет памяти
   if( kit_curr+sizeof(c_comp)+cmp->size_linerep +2 >= kit_end )
	   break;
  }


  c2=new_cell(); //tools

  c2->bdiff=0;
  c2->cg_flag=0;
  c2->dens=255;    // density not yet made by BOX


    if(! p2_accept_Cell(c2,&cur, cmp, line_scale) )//(Bool)(cmp->scale!=0)) )
        {
          free_cell(c2);  // нельзя del_cell() если не было insert_cell()
          break;
        }

    if(c2->env)
        c2->env->scale=cmp->scale;


  AttrCstr2Cell(c2,&cur);

  if(c2->env)
    {
    c2->env->upper=c2->r_row;
    c2->env->left =c2->r_col;
    if( line_scale )// cmp->scale )
        {
        c2->env->upper>>=line_scale;
        c2->env->left >>=line_scale;
        }
    }
  if( line_scale ) // cmp->scale )
    {
    c2->col     >>=line_scale;
    c2->r_col   >>=line_scale;
    c2->row     >>=line_scale;
    c2->r_row   >>=line_scale;
    }
  if( needVers && evn.lnAltCnt )
        {
     c2->nvers=(int16_t)MIN(VERS_IN_CELL-1,evn.lnAltCnt);
     for(i=0;i< c2->nvers;i++)
        {
        c2->vers[i].let =evn.Alt[i].Liga; //Code;
        c2->vers[i].prob=evn.Alt[i].Prob;
        }
         c2->vers[c2->nvers].let=0;
     c2->vers[c2->nvers].prob=0;
        }
  else
  {
          void set_bad_cell(cell *c);
          set_bad_cell(c2);

#ifdef _USE_DETOUCH_
          if(cur.flg & CSTR_f_detouch)
                  c2->flg |= c_f_detouch;
#endif

          c2->nvers=0;
  }

  if( !needVers && (c2->flg & c_f_punct) )
    {
    if( c2->h<CCOM_DUST_SIZE )
      c2->flg = c_f_dust;
    else
      c2->flg = c_f_bad;
    }

  p2_insert_cell(c2,c1);
  c1=c2;

  // Nick add
  c2->dupstart=(int32_t)CSTR_GetDup(curr);
  c2->dupend  =(int32_t)CSTR_GetDupEnd(curr);

  numCell++;
 }

 //c2=cell_l();
 //c1->next=c1->nextl=c2;
 //c2->prev=c2->prevl=c1;


 return numCell;
}
////////////////////
// cell - not dust , not space
static void p2_CopyAttr2CSTR(CSTR_rast_attr *attr, cell *c)
{
        memset(attr,0,sizeof(CSTR_rast_attr));

        attr->w     = c->w       ;
        attr->h     = c->h       ;
        attr->r_row = c->r_row   ;
        attr->r_col = c->r_col   ;

  //attr->row=attr->r_row-(int16_t)((int32_t)nIncline*attr->r_col/2048);
  //attr->col=attr->r_col+(int16_t)((int32_t)nIncline*attr->r_row/2048);
        attr->row=c->row;
        attr->col=c->col;

        attr->bdiff = c->bdiff   ;
        attr->difflg= c->difflg  ;
        attr->basflg= c->basflg  ;
        attr->bas1  = c->bas1    ;
        attr->bas2  = c->bas2    ;
        attr->bas3  = c->bas3    ;
        attr->bas4  = c->bas4    ;
        attr->cg_flag=c->cg_flag ;
        attr->cpos  = c->cpos   ;
        attr->bas_acc= c->bas_acc   ;
        attr->dlang_dup = c->dlang_dup; // oleg
                attr->flg    = (uchar) c->flg;
                if( attr->flg & CSTR_f_dust )
            attr->flg &= ~(CSTR_f_punct|CSTR_f_let|CSTR_f_bad);
                else if( attr->flg & CSTR_f_punct )
              attr->flg &= ~(CSTR_f_let|CSTR_f_bad);
                else if( attr->flg  & CSTR_f_let )
              attr->flg &= ~(CSTR_f_bad);

        attr->reasno = c->reasno;
        attr->keg = c->keg;
        attr->font = c->font ;
        attr->font_new = c->font_new ;
        attr->dens = c->dens ;
        attr->recsource = c->recsource ;
        attr->RecogHistory = CSTR_hi_none  ;
        if( c->history & c_rs_ev)
            attr->RecogHistory |= CSTR_hi_evn   ;
        if( c->history & c_rs_BOX)
            attr->RecogHistory |= CSTR_hi_3x5   ;
        if( c->history & c_rs_BOXs )
            attr->RecogHistory |= CSTR_hi_3x5s  ;
        if( c->history & c_rs_deskr )
            attr->RecogHistory |= CSTR_hi_desk  ;
        if( c->history & c_rs_bitcmp )
            attr->RecogHistory |= CSTR_hi_fon   ;
                if( c->history & c_rs_LEO )
            attr->RecogHistory |= CSTR_hi_leo   ;
        attr->stick_inc = c->stick_inc ;
                attr->save_stick_inc = c->save_stick_inc ;
        attr->bas_acc = c->bas_acc ;
        attr->left = c->left ;
        attr->right = c->right ;
        attr->shape = c->shape ;
        attr->rus_eng_word = c->rus_eng_word ;
        attr->broken_II = c->broken_II ;
        attr->language = c->language ;
        attr->pos_inc  = c->pos_inc;
        attr->clink    = c->clink;
                attr->clu_attr = c->clu_attr;
                attr->nClust   = c->nClust;

        attr->n_baton=c->n_baton;
        if( c->n_baton!=NO_BATONS )
            memcpy(attr->save_baton,c->save_baton,c->n_baton*sizeof(STICK));
        attr->flg_new  = c->flg_new;
		// Nick 20.02.2001
		attr->cg_flag_fine = c->cg_flag_fine;

		// Nick 11.03.2002
		attr->accent_leader = c->accent_leader;
		attr->bottom_accent = c->bottom_accent;

  return;
}
/////////////////////////
void p2_CellsToCSTR(CSTR_line lino)
{
cell*    c, cc;
CSTR_rast_attr  attr;
CSTR_attr               attrlin={0};
CSTR_rast       rst, old_rst=CSTR_GetFirstRaster(lino);
UniVersions     uvs;

 CSTR_GetLineAttr(lino, &attrlin);
 for (c = cell_f()->next; c != cell_l(); c = c -> next)
   {
   cc = *c;
   // don't take dust  !
   if( c->flg_new & c_fn_temp )
    continue;
   if( c->flg & c_f_space  )
    {
    if( (rst=CSTR_InsertRaster(old_rst))!=0 )
       {
        p2_CopyAttr2CSTR(&attr, c);
        CSTR_SetAttr (rst, &attr);
        uvs.lnAltCnt=1;
        switch( c->vers[0].let )
            {
            case    SS_NEG_HALF_SPACE:
                uvs.Alt[0].Code[0]=0;
                uvs.Alt[0].Liga   =SS_NEG_HALF_SPACE;
                uvs.Alt[0].Prob   =75;
                break;
            case    SS_POS_HALF_SPACE:
                uvs.Alt[0].Code[0]=' ';
                uvs.Alt[0].Code[1]=0;
                uvs.Alt[0].Liga   =SS_POS_HALF_SPACE;
                uvs.Alt[0].Prob   =150;
                break;
            case    SS_SPACE         :
                uvs.Alt[0].Code[0]=' ';
                uvs.Alt[0].Code[1]=0;
                uvs.Alt[0].Liga   =SS_SPACE;
                uvs.Alt[0].Prob   =254;
                break;
            }
        CSTR_StoreCollectionUni (rst, &uvs);
        old_rst = rst;
       }
    continue;
    }

    // don't take dust  ?
    //if( c->flg & c_f_dust )
        //      continue;

    if( attrlin.Flags & CSTR_STR_SCALED )
        {
        if( cc.env )
            cc.env->scale=line_scale;
        cc.col <<= line_scale;
        cc.row <<= line_scale;
        cc.r_col <<=line_scale;
        cc.r_row <<=line_scale;
        cc.w <<= line_scale;
        cc.h <<= line_scale;
        }
   if( (rst=CSTR_NewRaster (lino ,cc.col, cc.row,cc.w))!=0 )
        {
        p2_CopyAttr2CSTR(&attr,&cc);
        CSTR_SetAttr (rst, &attr);
        cell2UniVers(&uvs, c);
        CSTR_StoreCollectionUni (rst, &uvs);
        if( c->env /*&& c->env->scale<2*/ )
          CSTR_StoreComp (rst, (uchar*)((uchar*)c->env+c->env->lines),1,c->env->scale);
        old_rst = rst;
        }
   }
return;
}
/////////////
int p2_checkLeoCase(void)
{
 cell*    c;
 version  save_vers[2];
 int i,j;

 // для цифрового алфавита не удаляем версии!
 if( line_alphabet != ALPHA_DIGITAL_TRUE  )
 {
   for (c = cell_f()->nextl; c != cell_l(); c = c -> nextl)
   {
    if( !( c->flg & c_f_let ) )
        continue;

        if(c->recsource != c_rs_LEO )
                continue;

		// без близнецов не удалять!
	    if( !p2_twin(c->vers[0].let) )
           continue;

		// цифры не отсекаем
		if( line_alphabet == ALPHA_DIGITAL &&
			c->vers[0].let >= '0' && c->vers[0].let <= '9'
          )
		  continue;

		save_vers[0] = c->vers[0];
		save_vers[1] = c->vers[1];

        // correct case
        levcut(c,1);

		// все отрезали? восстановим - page158
		// Nick 13.06.2001
		for(i=0;i<c->nvers;i++)
		{
			if( c->vers[i].let == save_vers[0].let ||
				c->vers[i].let == save_vers[1].let
			  )
			  break;
        }

		if( i >= c->nvers)
		{
			for(j=REC_MAX_VERS-1;j>=2;j--)
				c->vers[j]=c->vers[j-2];
			c->vers[0] = save_vers[0];
			c->vers[1] = save_vers[1];

			if( c->flg & c_f_bad )
			{
				c->flg &= ~c_f_bad;
				c->flg |= c_f_let;
			}

			c->nvers+=2;
		}
   }
 }

 {  // Nick 19/01/2001

  p2_checkUpperLower();
 }

return 1;
}
/////////////
///////////////////////////////////
// проверка букв на приклеивание хвостиков
// рваное е может распознаться как с !
// Кусок от i - как l,I,r
static const char basicAccent[]="AaEeOoUucIlr";
static const char basicEngAccent[]="Ilr";
static const char lithvanianAccent[] ="AaCcEeIilSsUuZz";
static const char latvianAccent[] ="AaCcEeGgKkLlNnIiSsUuZz";
static const char estonianAccent[] ="AacOoSsUuZz";
static const char turkishAccent[] ="AaCcGgIl\xfdOorSsUu"; // 0xfd=i_sans_accent
/////////////
static void p2_TestAccent()
{
 cell *c;
 const char *accents;
 version vers[VERS_IN_CELL];
 int savNum;
 uint16_t savFlg;

 if( language == LANG_LITHUANIAN )
	 accents = lithvanianAccent;
 else if( language == LANG_LATVIAN )
	 accents = latvianAccent;
 else if( language == LANG_ESTONIAN )
	 accents = estonianAccent;
 else if( language == LANG_TURKISH )
	 accents = turkishAccent;
 else if( language!=LANG_ENGLISH )
         accents=basicAccent;
 else
         accents=basicEngAccent;

 for(c=cell_f()->nextl; c != NULL; c=c->nextl)
 {
   if(c->flg & c_f_bad)
           continue;
   if( c->nvers <= 0 || c->vers[0].prob == 0)
           continue;

   // i_bottom_accent часто без точки!
   if( !(language == LANG_LITHUANIAN && c->vers[0].let == i_bottom_accent ) &&
       !strchr(accents,c->vers[0].let)
	 )
           continue;

   memcpy(vers,c->vers,VERS_IN_CELL*sizeof(version));
   savNum = c->nvers;
   savFlg = c->flg;


   estletter(c,NULL);

   if(
	   ( c->nvers <= 0 || c->vers[0].prob < vers[0].prob -20 ) //c->vers[0].let  != 'j' )
     )
   {
	   c->nvers=savNum;
	   c->flg = savFlg;
	   memcpy(c->vers,vers,VERS_IN_CELL*sizeof(version));
   }
 }

}
///////////////////
// from crit_vers in old version (Version)
// int16_t crit_vers(Version v,cell * c,const s_glue * gl); // difrv
static int32_t CritVers(cell * BC,s_glue * GL,uchar let,uchar prob)
{
void r_criteria(cell *c, const s_glue * gl);         // difrv
cell cc=*BC;
 if( language==LANG_RUSSIAN )
 {
  cc.nvers = 1;
  cc.vers[0].let =let;
  cc.vers[0].prob=prob;
  cc.vers[1].let=cc.vers[1].prob=0;
  r_criteria(&cc,GL);
 }
 else if( language==LANG_ENGLISH )
 {
    stick_center_study(&cc,NULL,1);
 }
 return MIN(40,prob - cc.vers[0].prob);
}
/////////////////////
#define POROG_GOOD_LEO 220
// распознать cell - по FON (+LEO)
int16_t estletter(cell * BC,s_glue * GL)
{
RecVersions vers;
int32_t       i;
c_comp *comp=BC->env;
uchar *lp;
int16_t *lt;
int nClust=0;

  vers.lnAltCnt = 0;

#ifdef _USE_PASS2_

#ifdef _USE_GLUE_BOX_
  // not empty list ?
  if( GL &&
	  ( GL->complist[0] == NULL || GL->complist[1] == NULL )
    )
	GL = NULL;


  if( GL )
      nClust = recogGlueRaster( GL, &vers );

  else
#endif

  {
	if( !comp )
	  goto endvers;

    lp = (uchar *)(comp+1);
    lt = (int16_t*)lp;
//   w = comp->w
//   h = comp->h

	nClust=p2_RecogCompLp(*lt,lp,comp->w,comp->h,
				 BC->col,BC->row, &vers);
  }
#endif

 // discrim some let
  for(i=0;i<vers.lnAltCnt;i++)
  {
    if( // Макры для лиг 10.09.2000 E.P.
		liga_i == vers.Alt[i].Code ||
		language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(vers.Alt[i].Code==i_sans_accent||vers.Alt[i].Code==II_dot_accent)
		||
		liga_exm == vers.Alt[i].Code ||
		strchr("|!1li",vers.Alt[i].Code)
	  )

    {
     vers.Alt[i].Prob = (uchar)MAX(1,vers.Alt[i].Prob - CritVers(BC,GL,vers.Alt[i].Code,vers.Alt[i].Prob));
    }
  }

  p2_leo_sort_vers_prob(&vers);

  if( vers.lnAltCnt > 0 &&
	  vers.Alt[0].Method != REC_METHOD_FON &&
      vers.Alt[0].Prob  < POROG_GOOD_LEO )
          vers.lnAltCnt=0;

  if( vers.lnAltCnt > VERS_IN_CELL -1 )
          vers.lnAltCnt = VERS_IN_CELL -1 ;

endvers:
  BC->nvers=(int16_t)vers.lnAltCnt;

  if(vers.lnAltCnt==0)
  {
   BC->vers[0].let =176;
   BC->vers[0].prob=0;
   BC->vers[1].let  = 0;
   BC->vers[1].prob = 0;
  }

  else
  {
   for(i=0;i<vers.lnAltCnt;i++)
        {
         BC->vers[i].let =vers.Alt[i].Code;
         BC->vers[i].prob=vers.Alt[i].Prob;
        }

   BC->vers[vers.lnAltCnt].let  = 0;
   BC->vers[vers.lnAltCnt].prob = 0;

   if( vers.Alt[0].Method==REC_METHOD_FON )
   {
    BC->recsource=c_rs_bitcmp;
    BC->history  =c_rs_bitcmp;
    BC->clink    =BC->vers[0].prob;
    BC->nClust   =nClust;
   }
   else  // LEO
   {
    BC->recsource=c_rs_LEO;
    BC->history  =c_rs_LEO;
   }
  }

  return vers.Alt[0].Prob;
}
////////////////////////
// распознать с разрезанием/склейкой кусок - от first до last,
// результат поместить в lineFon
// p2glob - language & s.o.
int32_t p2_RecogCutGlu( CSTR_rast  first,CSTR_rast last,
								  CSTR_line  lineFon,P2GLOBALS *p2glob)
{
void proc_bI(int32_t pass);
void proc_ii(void);
void make_all_cuts( void);
void make_all_glues(void);

 // set global variables
 p2_SetP2Globals(p2glob);
// очистим результаты предыдущего распознавания

  // delete cell_f() -> next & s.o.
 clear_cells();

 // пустая строка ?
 if( p2_Cstr2Cell( (CSTR_line)NULL, first, last, FALSE, CSTR_f_space) <= 0 )
 { // удалим старое содержимое lineFon
  first=CSTR_GetFirstRaster(lineFon);
  for(last=CSTR_GetNext(first);last;last=CSTR_GetNext(first))
          CSTR_DelRaster(last);
  return 0;
 }

 if( language!=LANG_RUSSIAN )
 {
  cell *c;
  uchar sv = cuts_point_methode;
  cuts_point_methode = 0 ;       // leman def cut points

  for(c=cell_f()->next; c->next != NULL; c=c->next)
   if(c->flg & (c_f_bad|c_f_let)) set_bad_cell(c); // kill

  make_all_cuts();
  make_all_glues();

  cuts_point_methode = sv;

 // в результате разрезания/склейки
 // к буквам могут быть приклеены куски
  p2_TestAccent();
 }
 else

 { // Paul
  void cuts_glues(void);
  void proc_Ukr(void);
  extern uchar langUkr;

  cuts_glues();

  if( language == LANG_RUSSIAN )
  {
   if( !langUkr )
    proc_bI(0);//paste cutted '|'
   else if( langUkr )
    proc_Ukr();//UKRAINIAN "iI & .."

    proc_ii();//paste '©'
    proc_bI(1);//glue all 'л'
  }

 }

  // удалим старое содержимое lineFon
  first=CSTR_GetFirstRaster(lineFon);
  for(last=CSTR_GetNext(first);last;last=CSTR_GetNext(first))
          CSTR_DelRaster(last);

  // распознанное ->  в lineFon
  p2_CellsToCSTR( lineFon );

  // delete cell_f() ->
  clear_cells();
  p2_GetP2Globals(p2glob);
  return 1;
}
///////////////////////

int32_t p2_setBasLines(CSTR_line lineIn)
{  // from Vlad version
 extern int16_t     bs_got;
 extern Bool    line_readyBL;
 CSTR_rast_attr attr;
 CSTR_rast      rst;
 int16_t            i,minr=0, row;
 CSTR_attr      lineAttr;

 if(!CSTR_GetLineAttr ( lineIn, &lineAttr))
          return -1;

 minr=32000;
 if( lineAttr.Flags & CSTR_STR_ReadyBL )
    {
    line_readyBL=TRUE;
	nIncline=(int16_t)lineAttr.incline; // Nick 30.10.2000
    if( lineAttr.tab_number )
        {
        for(minr=32000,rst=CSTR_GetNext(CSTR_GetFirstRaster(lineIn));rst;rst=CSTR_GetNext(rst))
            {
            CSTR_GetAttr(rst,&attr);
            if( attr.flg&(CSTR_f_let|CSTR_f_punct|CSTR_f_bad) )
                {
                row = attr.row-(int16_t)((int32_t)nIncline*attr.col/2048);
                if( minr>row )
                    minr=row;
                }
            }
        }
    }
 minrow=(int16_t)lineAttr.l_row;
 if( minr==32000 )
    minr=minrow;

 bbs1  =lineAttr.bs1;//+minrow-minr;
 bbs2  =lineAttr.bs2;//+minrow-minr;
 bbs3  =lineAttr.bs3;//+minrow-minr;
 bbs4  =lineAttr.bs4;//+minrow-minr;

 bbsm =lineAttr.bsm;

/* not need !!!!
 if( line_readyBL==TRUE&& lineAttr.tab_number )
     {
     bbs1  =lineAttr.bs1+minrow-minr;
     bbs2  =lineAttr.bs2+minrow-minr;
     bbs3  =lineAttr.bs3+minrow-minr;
     bbs4  =lineAttr.bs4+minrow-minr;
	 bbsm  =lineAttr.bsm+minrow-minr; // Nick 17.10.2000
     }

// bbsm =lineAttr.bsm;

 minrow = minr;
*/

 Nb1  =lineAttr.Nb1;
 Nb2  =lineAttr.Nb2;
 Nb3  =lineAttr.Nb3;
 Nb4  =lineAttr.Nb4;
 Nbt  =lineAttr.Nbt;
 Ps   =lineAttr.Ps;


 bs_got = 1;   // set for faragev's sticks_in_letter()

    bsdust_ps = Ps;
    i = bbs2 - (bbs3-bbs2)/2;
    bsdust_upper = MIN(i,bbs1) - 2 + minrow;
    if (language != LANG_ENGLISH)
    bsdust_upper -= (MAX(2,(bbs3-bbs2)/7));
    bsdust_lower = bbs4 + minrow;

 nIncline=(int16_t)lineAttr.incline;

 {
  extern int16_t current_fragment;

  current_fragment=(int16_t)lineAttr.fragment;
//  lineout_fragment();
 }
return 0;
}
//////////////////////
Bool p2_NoStopSnapLEO(void)
{
	if( !hSnapLEO|| LDPUMA_SkipEx(hSnapLEO,TRUE,FALSE,1))
	 return TRUE;
	return FALSE;
}

///////////////
void p2_SetP2Alphabet(int lang,char *alBet)
{
	memcpy(alBet,fon_alphabet_language[lang==LANG_DIG?2:lang==LANG_RUSSIAN?1:0],256);
    if( line_tabcell )
        alBet[liga_exm]=0;

}
////////////////////
uchar p2_GetCodePage(int lang)
{
	return CodePages[lang];
}
/////////////
void p2_DecodeCode(char *pCode,int let)
{
  strcpy(pCode , decode_ASCII_to_[let]);
}
//////////////////
int32_t p2_GetP2Globals(P2GLOBALS *p2globals)
{
      p2globals->language = language;
	  p2globals->multy_language = multy_language;
	  p2globals->langUkr = langUkr;
	  p2globals->langSer = langSer;

	  p2globals->line_number = line_number;
	  p2globals->line_alphabet = line_alphabet;
      p2globals->line_scale = line_scale;
	  p2globals->line_tabcell = line_tabcell;

	  p2globals->nIncline = nIncline;

	  p2globals->cuts_point_methode = cuts_point_methode;
      p2globals->langBul = langBul;  // Nick 13.09.2000


	  return 1;
}
////////////
int32_t p2_SetP2Globals(P2GLOBALS *p2globals)
{
      language = p2globals->language ;
	  multy_language = p2globals->multy_language ;
	  langUkr = p2globals->langUkr;
	  langSer = p2globals->langSer ;

	  line_number = p2globals->line_number;
	  line_alphabet = p2globals->line_alphabet;
      line_scale = p2globals->line_scale;
      line_tabcell = p2globals->line_tabcell;

	  nIncline = p2globals->nIncline;

	  cuts_point_methode = p2globals->cuts_point_methode;
      langBul = p2globals->langBul;  // Nick 13.09.2000


	  return 1;
}
////////////////////////////

static int32_t cmp_prob( const void *a, const void *b)
{
return (int32_t)(((RecAlt *)b)->Prob) - (int32_t)(((RecAlt *)a)->Prob) ;
}
///////////////////////
int32_t p2_leo_sort_vers_prob(RecVersions *v)
{
int i,n0,n1;
uchar c0,c1;
RecAlt a0,a1;
stdQsort(v->Alt,v->lnAltCnt,sizeof(RecAlt),cmp_prob);
if( v->lnAltCnt>1 && v->Alt[0].Prob==v->Alt[1].Prob )
        {
        c0 = v->Alt[0].Code;
        c1 = v->Alt[1].Code;
    a0 = v->Alt[0];
        a1 = v->Alt[1];

        for(n1=n0=i=0;i<v->lnAltCnt; i++)
                {
                n0 += (v->Alt[i].Code==c0);
                n1 += (v->Alt[i].Code==c1);
                }
        if( n1>n0 )
                {
        //a0.Code=c1;
                //a1.Code=c0 ;
        v->Alt[0]=a1;
                v->Alt[1]=a0;

                }
        }
return v->lnAltCnt;
}
////////////////////////
void p2_TextWord(CSTR_rast  c,CSTR_rast stop, char *intxt,Bool ansi)
{
        CSTR_rast_attr attr;
        UniVersions     vers;
        char *txt=intxt;

        for(*txt='\0'; c && c!=stop; c=CSTR_GetNextRaster (c,CSTR_f_all))
    {
    CSTR_GetAttr(c, &attr);
    if( !(attr.flg & (CSTR_f_let|CSTR_f_punct|CSTR_f_bad|CSTR_f_space)) )
        continue;
    CSTR_GetCollectionUni(c, &vers);
    if( !vers.lnAltCnt )
                strcat(txt,"~");
    else
                strcat(txt,vers.Alt[0].Code);
    }

        for(txt=intxt;*txt;txt++)
                        *txt=stdAnsiToAscii(*txt);
}
//////////////////
void p2_FillTxt(CSTR_line cc,char *intxt,Bool ansi)
{
    CSTR_rast first=CSTR_GetFirstRaster(cc);
    CSTR_rast last=CSTR_GetLastRaster(cc);
    if(!first) return;
    p2_TextWord(CSTR_GetNext(first),last,intxt,ansi);
}
//////////////////////////////
Bool32 p2_Line2Raster(c_comp *comp, RecRaster *rec)
{
#ifdef _USE_PASS2_
uchar *lp;
int16_t *lt;

  if( !comp )
	  return FALSE;

   lp = (uchar *)(comp+1);
   lt = (int16_t*)lp;
//   w = comp->w
//   h = comp->h

  return p2_Comp2Raster(*lt,lp,comp->w,comp->h,rec);
#else
    return FALSE;
#endif
}
////////////////////
Bool32 p2_LEOSetupField(char *letInfo,int32_t nFont,FontInfo *fontinfo)
{
#ifdef _USE_LEO_
  LeoFieldSetup   fs={0};
  uchar   *aa=(uchar *)letInfo;
  uchar   asciiName;

    fs.nStyle = LS_PRINT;
    for(;*aa;aa++)
        {
                // get only unknown for FONT
          asciiName=stdAnsiToAscii(*aa);
          if( asciiName && fontinfo->numFont[asciiName]<nFont)
                  fs.AlphaTable[*aa]=1;
        }

    return LEOSetupField(&fs);
#else
    return FALSE;
#endif
}
///////////////////
Bool32 p2_LEOSetupPage(void *info)
{
#ifdef _USE_LEO_
	LeoPageSetup ps={0};

    ps.nIdPage = -1;
	return LEOSetupPage(&ps);
#else
    return FALSE;
#endif
}
///////////
Bool32 p2_LEORecog(RecObject *ro)
{
#ifdef _USE_LEO_
	return LEORecogPrintChar(ro);
#else
    return FALSE;
#endif
}
///////////////
void	p2_init		(void)
{
#ifdef _USE_PASS2_
	P2_SetRSTR( p2_RecogCutGlu,
                p2_SetP2Alphabet,p2_GetCodePage,p2_DecodeCode,
                p2_NoStopSnapLEO,
				snap_monitor_ori, snap_activity, snap_show_text,
                _spell,
				p2_LEOSetupPage,
				p2_LEOSetupField,
				p2_LEORecog
				);
#endif
}
////////////////
// проверить сомнительные большие-маленькие на втором проходе
/////////////
static const uchar non_twin[]=" ЂЎЃҐ…";
static const uchar lat_twins[]="cCoOpPsSvVwWxXzZ";

static Bool32 p2_twin(uchar ch)
{
 if( language==LANG_RUSSIAN )
 {
  if(memchr(non_twin,ch,sizeof non_twin))
	 return FALSE;
  if( ch >= 128 && ch < 192 || ch >= 224 && ch < 240 )
	 return TRUE;
 }

 if( memchr(lat_twins,ch,sizeof lat_twins))
   return TRUE;
 return FALSE;
}
////////////
// return 0 - нет близнеца
// return 1 - есть слабый близнец
// return 2 - есть равный близнец
// return 3 - есть сильный близнец
static int32_t IsTwinCluster(int nClus, uchar name, uchar *tName)
{
  uchar twinName=0;
  ClustInfo clustinfo;
  ClustInfo twininfo;
  int i;
  int porogW,porogH;
  int ret = 0;

  *tName = 0;

  if( nClus <= 0 )
     return 0;

  if( !p2_twin(name) )
     return 0;

  if(  is_upper(name) )
      twinName=to_lower(name);
  else
      twinName=to_upper(name);

  *tName = twinName;

  memset( &clustinfo,0, sizeof(ClustInfo));
  FONGetClustInfo(&clustinfo, nClus);

  porogW = (clustinfo.mw)/8;
  porogH = (clustinfo.mh)/8;

  // looking best with name & size
  for(i=1; ;i++)
  {
    memset( &twininfo,0, sizeof(ClustInfo));
    twininfo.let = name;
    i  =  FONGetClustInfo(&twininfo, i);

    if( i <= 0 )  // no more with name
     break;

    if( abs(clustinfo.mw-twininfo.mw) <= porogW &&
        abs(clustinfo.mh-twininfo.mh) <= porogH
      )
      clustinfo.weight=MAX(clustinfo.weight, twininfo.weight);
  }



  for(i=1; ;i++)
  {
    memset( &twininfo,0, sizeof(ClustInfo));
    twininfo.let = twinName;
    i  =  FONGetClustInfo(&twininfo, i);

    if( i <= 0 )
     break;

    if( abs(clustinfo.mw-twininfo.mw) <= porogW &&
        abs(clustinfo.mh-twininfo.mh) <= porogH
      )
    {
      if ( twininfo.weight > clustinfo.weight+1 )
          return 3;

      if ( twininfo.weight >= clustinfo.weight - 1 )
          ret = 2;
      else
          if( ret < 1 ) ret = 1;
    }

  }

  return ret;
}
/////////////
#define v_bs1 1   // на первой базовой
#define v_bs2 2   // на второй
#define v_bs3 4
#define v_bs4 8
////////////////
#define v_capital 2
#define v_small   1
//
typedef struct tagParamRecogSize
{
  int sizeMax;
  int sizeMin;
  int sizeNum;
  int num;
  int basMax;
  int basMin;
  int twin;
  int height;
}   ParamRecogSize;


#define MAXHEI 128

typedef struct tagPOIS
{
	int poi;
	int plato;
	int progib;
} POIS;
////////////////
//////////////////////
static void InitParamSize(ParamRecogSize *param)
{
  memset(param,0,sizeof(ParamRecogSize));

  param->sizeMin = MAXHEI-1;
  param->basMin  = 0x7fff;
}
///////////////
static int AddParamSize(ParamRecogSize *param, cell *c, uchar ch,
						uint32_t isBase, Bool32 isSize)
{
  if( isBase )
  {
   param->num++;
   param->basMax  = MAX(param->basMax, c->row);
   param->basMin  = MIN(param->basMin, c->row);
  }

  if( (isBase && (ch & v_bs3)) || isSize )
  {
   param->sizeMax = MAX(param->sizeMax, c->h);
   param->sizeMin = MIN(param->sizeMin, c->h);
   param->height += c->h;
   param->sizeNum++;
  }

  if( p2_twin(c->vers[0].let) )
     param->twin++;

  return param->num;
}
///////////////
static Bool32 IsBigLetter(uchar name)
{
	uchar ch;

	if( let_lincomp[name] == v_capital )
		return TRUE;

	ch = let_lindef[name];
	if( (ch & v_bs1) && (ch&v_bs3) )
		return TRUE;
	return FALSE;
}
///////////////
static Bool32 IsBigTailLetter(uchar name)
{
	if( let_linpos[name] == 0x12 )
		return TRUE;

	return FALSE;
}
///////////////
static Bool32 IsSmallLetter(uchar name)
{
	uchar ch;

	if( let_lincomp[name] == v_small )
		return TRUE;

	ch = let_lindef[name];
	if( (ch & v_bs2) && (ch&v_bs3) )
		return TRUE;
	return FALSE;
}
///////////////
static int CheckCluHeights(int heiUp, int heiDn, int basUp, int basDn)
{
 cell *  c, *prev;

 uchar ch,chTwin;

 // name - for upper, twin for lower
 uchar name, twinName;


 for (c = cell_f()->nextl; c != cell_l(); c = c -> nextl)
 {
    if( !( c->flg & c_f_let ) )
        continue;

	name = c->vers[0].let;
    ch = let_lindef[name];

	if( !p2_twin(name) )
	{
		continue;
	}

	if(  is_upper(name) )
	{
      twinName=to_lower(name);
	  chTwin = let_lindef[twinName];
	}
    else
	{
      twinName=name;
	  chTwin = ch;
      name = to_upper(name);
      ch   = let_lindef[name];
	}

	// большая с хвостом ?
	if( IsBigTailLetter(name) )
	{
		// по размерам
     if( heiUp > 0 && heiDn > 0 &&  heiUp > heiDn )
	 {
		 if(  c->h  < heiUp -1 )
		 {
			 c->vers[0].let = twinName;
			 continue;
		 }
	 }
	}

	// неизвестны размеры ?
    if( !IsBigLetter(name) || !IsSmallLetter(twinName)
      )
    {

	 // по соседу - для й Й
	 if	(!(ch & v_bs1) || !(chTwin & v_bs2) )
	 {
		 if( language != LANG_RUSSIAN || name != 137 )
		       continue;

		 prev = c->prevl;

		 if( prev && (prev->flg & c_f_let) &&
			 (c->col - prev->col -prev->w)*4 < c->w
		   )
		 {
			 // после маленькой - маленькая
			 if ( is_lower(prev->vers[0].let) )
				 c->vers[0].let = twinName;
			 // после большой - любая: пример АйТи
//			 else if( is_upper(prev->vers[0].let) )
//				 c->vers[0].let = name;
		 }

		 continue;
	 }

    // только по положению
	 if( basUp >= 0 && basDn >= 0 &&  basDn >= basUp )
	 {
			if( (c->row-basUp) * 2 <= (basDn-c->row) )
			{
			 c->vers[0].let = name;
			 continue;
			}

			if( (c->row-basUp) >= 2 * (basDn-c->row) )
			{
			 c->vers[0].let = twinName;
			 continue;
			}

	 }

	 continue;
	}


    // по размерам
    if( heiUp > 0 && heiDn > 0 &&  heiUp > heiDn )
	{
		 if( (heiUp - c->h)*2 <= (c->h - heiDn) )
		 {
			 c->vers[0].let = name;
			 continue;
		 }

		 if( (heiUp - c->h) >= 2*(c->h - heiDn) )
		 {
			 c->vers[0].let = twinName;
			 continue;
		 }

	}

	// по положению
	if( (ch & v_bs1) && (chTwin & v_bs2) &&
		basUp >= 0 && basDn >= 0 &&  basDn >= basUp )
	{
		    if( (c->row-basUp) * 2 <= (basDn-c->row) )
			{
			 c->vers[0].let = name;
			 continue;
			}

			if( (c->row-basUp) >= 2 * (basDn-c->row) )
			{
			 c->vers[0].let = twinName;
			 continue;
			}
	}

  }  // end for

  return 1;
}
/////////////////////

static int CheckCluSosed(int basUp, int basDn)
{
 cell *  c;

 uchar ch,chTwin;

 int prevHei1=-1;
 int prevHei2=-1;
 int prevB1=-1;
 int prevB2=-1;


 // name - for upper, twin for lower
 uchar name, twinName;

 // looking for first non-twin
 for (c = cell_f()->nextl; c != cell_l(); c = c -> nextl)
 {
    if( !( c->flg & c_f_let ) )
        continue;

	name = c->vers[0].let;
    ch = let_lindef[name];

	if( p2_twin(name) )
		continue;

    if( ch & v_bs1 )
		prevB1 = c->row;
	 else if( ch & v_bs2 )
		prevB2 = c->row;

	 if(IsBigLetter(name))
		prevHei1 = c->h;
	 else if(IsSmallLetter(name))
		prevHei2 = c->h;

	 if( prevHei1 > 0 || prevHei2 > 0 ||
		prevB1 > 0   || prevB2 > 0 )
		break;
 }

 // test twins
 for (c = cell_f()->nextl; c != cell_l(); c = c -> nextl)
 {
    if( !( c->flg & c_f_let ) )
        continue;

	name = c->vers[0].let;
    ch = let_lindef[name];

	if( !p2_twin(name) )
	{
     if( ch & v_bs1 )
		prevB1 = c->row;
	 else if( ch & v_bs2 )
		prevB2 = c->row;

	 if(IsBigLetter(name))
		prevHei1 = c->h;
	 else if(IsSmallLetter(name))
		prevHei2 = c->h;

	 continue;
	}

	if( basUp > 0 && basDn > 0 )
	{
		prevB1 = basUp;
		prevB2 = basDn;
	}

	if(  is_upper(name) )
	{
      twinName=to_lower(name);
	  chTwin = let_lindef[twinName];
	}
    else
	{
      twinName=name;
	  chTwin = ch;
      name = to_upper(name);
      ch   = let_lindef[name];
	}

	// известны размеры ?
    if( (basUp < 0 || basDn < 0 ) &&
		( prevHei1 > 0 || prevHei2 > 0 ) &&
		IsBigLetter(name) &&
		IsSmallLetter(twinName )
      )
    {
		if( prevHei1 < 0 )
			  prevHei1 = (prevHei2*4)/3;
		if( prevHei2 < 0 )
			prevHei2 = (prevHei1*3)/4;

		if( (prevHei1 - c->h)*2 <= c->h - prevHei2 )
		{
			if( ch & v_bs1 )
			  prevB1 = c->row;
			prevHei1 = c->h;
			c->vers[0].let = name;
			continue;
		}

		else if( prevHei1 - c->h >= (c->h - prevHei2)*2 )
		{
			if( chTwin & v_bs2 )
			 prevB2 = c->row;
			prevHei2 = c->h;
			c->vers[0].let = twinName;
            continue;
        }
	}

    //  по положению

	 if	( (ch & v_bs1) && (chTwin & v_bs2) &&
		  ( prevB1 > 0 || prevB2 > 0 )
		)
	 {
		 if( prevB1 <0 )
			 prevB1 = prevB2 - c->h/4;
		 if( prevB2 <0 )
			 prevB2 = prevB1 + c->h/4;

		 if( c->row - prevB1 < prevB2 - c->row  )
			 c->vers[0].let = name;
		 else if (  c->row -prevB1 > prevB2 - c->row )
			 c->vers[0].let = twinName;
	 }

	name = c->vers[0].let;
    ch = let_lindef[name];

    if( ch & v_bs1 )
		prevB1 = c->row;
	else if( ch & v_bs2 )
		prevB2 = c->row;

	if(IsBigLetter(name))
		prevHei1 = c->h;
	else if(IsSmallLetter(name))
		prevHei2 = c->h;

  }  // end for

  return 1;
}
/////////////////////

////////////////
static int Progib(int *hhh,int GreyLev,POIS *pois, int threshPlato,
				  int threshMin)
{
 int  i;
 int  lefth=-1;
 int  prommin = -1;
 uchar spusk = 0;
 int  numPoi;
 int  dimPlato;

 for(numPoi=0,dimPlato=0,lefth=-1,i=1;i<GreyLev;i++)
  {

	if(hhh[i] >= hhh[i-1])
	  {
	   if(spusk == 0 )
	           continue;
	   spusk = 0;                 /*(i-1) - local minimum point */
	   if(lefth == -1 )
	        continue;

       if( prommin == -1 || hhh[i-1] <= hhh[prommin] )
	     {
		   int j,k;
		   // count the same
		   for(j=i, k=1;j<GreyLev;j++,k++)
		     if( hhh[j] != hhh[i-1] ) break;

		   if( prommin == -1 ||
			   hhh[i-1] < hhh[prommin] ||
		       k >= dimPlato
             )
		   {
		     prommin = i-1;
			 dimPlato = k;
		   }

           i=MAX(i,j-1);
		   continue;
		 }
	  }
	else
	  {
		if(spusk ) continue;
		spusk = 0xFF;                /* local maximum */
		if(lefth == -1 )
		{
		   if( hhh[i-1] > 0 )
		     lefth = i-1;
		   continue;
		}

	    if( prommin!=-1 && hhh[i-1] > hhh[prommin] &&
			hhh[prommin] < threshMin
		  )
	    {
		 // not too near ?
         if( i-1 - lefth > threshPlato )
         {  // add point
#ifdef _TESTPROM_
			 // same point ?
            if( numPoi > 0 && prommin+(dimPlato/2) == pois[numPoi-1].poi )
				numPoi--;
#endif
		    pois[numPoi].poi = prommin+(dimPlato/2);
			pois[numPoi].progib = MIN(hhh[i-1],hhh[lefth])-hhh[prommin];
			pois[numPoi++].plato = dimPlato;

#ifndef _TESTPROM_
            lefth = i-1;
			prommin = -1;
			dimPlato = 0;
			continue;
#endif
         }
		}
		  /* right higher then left */
		if(hhh[i-1] >= hhh[lefth])
		{
			lefth=i-1;     /* global maximum left */
			prommin=-1;
			dimPlato = 0;
		}
	}   // end else
  }
		 // study last point
 if( prommin != -1  && lefth != -1 && hhh[GreyLev-1] > hhh[prommin] &&
	 hhh[prommin] < threshMin
   )
  {
    if( GreyLev - 1 - lefth > threshPlato )
	{
#ifdef _TESTPROM_
		 // same point ?
      if( numPoi > 0 && prommin+(dimPlato/2) == pois[numPoi-1].poi )
				numPoi--;
#endif
	  pois[numPoi].poi = prommin + (dimPlato/2);
	  pois[numPoi].progib = MIN(hhh[GreyLev-1],hhh[lefth])-hhh[prommin];
	  pois[numPoi++].plato = dimPlato;
	}
  }

 return(numPoi);
}
/*************************/

static int CheckSizes(int *heiUp, int *heiDn,
					  ParamRecogSize *parUp,
					  ParamRecogSize *parDn,
					  int *bufHei, int minSize
					  )
{
	int i;
	int best = -1;
	int medium;
	POIS pois[MAXHEI];
	int *buf;
	int start,end;
	int numPoi;

    // короткий тест - если есть подозрение
	if( *heiUp > 0 && &heiDn > 0)
	{
     medium = (*heiUp+*heiDn)/2;

	 for (i=parDn->sizeMax+1, best=-1; i < parUp->sizeMin;i++)
	 {
		if( best == -1 || bufHei[i] < bufHei[best] ||
			bufHei[i] == bufHei[best] && abs(i-medium) < abs(best-medium)
		  )
			best = i;
	 }

	 if( best >= 0 )
		return best;
	}

    //
	start = MIN(parDn->sizeMin, parUp->sizeMin);
	end   = MAX(parDn->sizeMax, parUp->sizeMax);
	buf = bufHei + start;

	numPoi = Progib(buf, end-start+1, pois, 1, minSize);
	for(i=0;i<numPoi;i++)
		pois[i].poi += start;

	for(i=0, best=-1;i<numPoi;i++)
	{
       if( pois[i].poi > parDn->sizeMin &&
		   pois[i].poi < parUp->sizeMax &&
		   (
		    best == -1 ||
		    pois[i].progib > pois[best].progib ||
		    pois[i].progib == pois[best].progib &&
		    pois[i].plato > pois[best].plato
		   )
		  )
		  best = i;
	}

	if( best < 0 )
	{
		*heiUp = *heiDn = 0;
		return 0;
	}

	best = pois[best].poi;
	for( i = best-1; i > parDn->sizeMin;i--)
	{
		if( bufHei[i] > bufHei[best] )
		  break;
	}
	*heiDn = i;

	for( i = best+1; i < parDn->sizeMax;i++)
	{
		if( bufHei[i] > bufHei[best] )
		  break;
	}
	*heiUp = i;

	return best;
}
////////////////////

static int CheckBases(int *basUp, int *basDn,
					  ParamRecogSize *parUp,
					  ParamRecogSize *parDn,
					  int *bufHei, int minbase
					  )
{
	int i;
	int best = -1;
	int medium;
	POIS pois[MAXHEI];
	int *buf;
	int start,end;
	int numPoi;

	if(minbase==0x7fff)
        return 0;

	// короткий тест - если есть подозрение
	if( *basUp > 0 && *basDn > 0 )
	{
     medium = (*basUp+*basDn)/2;
     medium -= minbase;

	 for (i=parUp->basMax+1-minbase, best=-1; i < parDn->basMin-minbase;i++)
	 {
		if( best == -1 || bufHei[i] < bufHei[best] ||
			bufHei[i] == bufHei[best] && abs(i-medium) < abs(best-medium)
		  )
			best = i;
	 }

	 if( best >= 0 )
		return best+minbase;
	}

    //
	start = MIN(parDn->basMin, parUp->basMin)-minbase;
	end   = MAX(parDn->basMax, parUp->basMax)-minbase;

	buf = bufHei + start;

	numPoi = Progib(buf, end-start+1, pois, 1, 1);
	for(i=0;i<numPoi;i++)
		pois[i].poi += start+minbase;

	for(i=0, best=-1;i<numPoi;i++)
	{
       if( pois[i].poi >= parUp->basMin &&
		   pois[i].poi <= parDn->basMax &&
		   (
		    best == -1 ||
		    pois[i].progib > pois[best].progib ||
		    pois[i].progib == pois[best].progib &&
		    pois[i].plato > pois[best].plato
		   )
		  )
		  best = i;
	}

	if( best < 0 )
	{
		//*basUp = *basDn = -1;
		return 0;
	}

	best = pois[best].poi;
	for( i = best-1-minbase; i > parDn->basMin-minbase;i--)
	{
		if( bufHei[i] > bufHei[best-minbase] )
		  break;
	}
	*basUp = i+minbase;

	for( i = best+1-minbase; i < parDn->basMax-minbase;i++)
	{
		if( bufHei[i] > bufHei[best-minbase] )
		  break;
	}
	*basDn = i+minbase;

	return best+minbase;
}
////////////////////
static int UpDownByHeights(int heiUp, int heiDn)
{
 cell *c;

 uchar ch;
 uchar name;
 int numUp,numDn;

 numUp=0;
 numDn=0;

 if( heiUp <= 0 || heiDn <= 0 || heiUp <= heiDn )
	 return 0;

 for (c = cell_f()->nextl; c != cell_l(); c = c -> nextl)
 {
    if( !( c->flg & c_f_let ) )
        continue;

	name = c->vers[0].let;

	if( p2_twin(name) )
	{

     if( !IsBigLetter((uchar)(is_upper(name)?name:to_upper(name))) ||
		 !IsSmallLetter((uchar)(is_lower(name)?name:(uchar)to_lower(name)))
		)
		 continue;

		    // по размерам
	 if( (heiUp - c->h)*2 <= (c->h - heiDn) )
		 {
			 if(  is_lower(name) )
			    name = to_upper(name);
		 }

	  else if( (heiUp - c->h) >= 2*(c->h - heiDn) )
		 {
			 if(  is_upper(name) )
			    name = to_lower(name);
		 }
	   else
			 continue;  // don't know - don't use for statistic
	 }


	 ch = let_lindef[name];

	 if( ch & v_bs1 )
		 numUp++;

	 if( ch & v_bs2 )
		 numDn++;

  }  // end for

  if( numUp > numDn )
	  return 1;
  else if( numDn > numUp )
	  return 2;

  return 0;
}
/////////////////////
static int AntiUpBase(int basDn)
{
 cell *c;

 uchar ch;
 uchar name;
 int numUp,numDn;
 int numTwin;

 numUp=0;
 numDn=0;
 numTwin = 0;

 for (c = cell_f()->nextl; c != cell_l(); c = c -> nextl)
 {
    if( !( c->flg & c_f_let ) )
        continue;

	name = c->vers[0].let;

	if( c->row < basDn )
		continue;

	if( p2_twin(name) )
	{
		numTwin++;
		continue;
	}

	ch = let_lindef[name];

	if( ch & v_bs1 )
		 numUp++;

	if( ch & v_bs2 )
		 numDn++;

  }  // end for

 // if (numUp > 0 ) ????
  if( numUp > 0 &&
	  numUp >= numDn + numTwin
	 )
	  return 1;

  return 0;
}
/////////////////////
//
// параметры базовых линий - Nb1, Nb2, Nb3, Nb4
// Nbi < 0 - ненадежная линия
// Nbi == 127, 128 - сомнительная линия (получена по гистограмме)
// Nbi 127 - eng, 128 - rus
// Nbi = 99 - искусственная линия, только под #ifdef UFA
// Иначе - чем больше Nb1, тем надежнее.
// Не предусмотрено, а если в строке действительно 127,128 символов?
//
//
static int p2_checkUpperLower(void)
{
 cell *  c;

 uchar ch;
 int i;

 ParamRecogSize upS,downS;
 ParamRecogSize niceUp,niceDn;
 ParamRecogSize allUp,allDn;

 int numTwins;
 int notConfirmed;

 int porog;
 int numAntiCluster;  // number of clusters - not accord recog (base lines)
 Bool16 IsNice,IsConfirmed;
 uchar name,twinName;
 int bufHeight[MAXHEI];
 int bufBase[MAXHEI];
 int minBase;
 int heiUp, heiDn;
 int basUp, basDn;

 int bound;

 uchar sav_lang=language;

 // на втором проходе языки могут смешиваться!
 if( language == LANG_ENGLISH && multy_language )
     language = LANG_RUSSIAN;

 memset(bufHeight,0,MAXHEI*sizeof(int));
 memset(bufBase,0,MAXHEI*sizeof(int));

 InitParamSize(&upS);
 InitParamSize(&downS);

 InitParamSize(&niceUp);
 InitParamSize(&niceDn);

 InitParamSize(&allUp);
 InitParamSize(&allDn);

 numTwins = 0;
 numAntiCluster = 0;
 notConfirmed   = 0;

 for (c = cell_f()->nextl,minBase=0x7fff; c != cell_l(); c = c -> nextl)
 {
    if( !( c->flg & c_f_let ) )
        continue;
	minBase = MIN(minBase,c->row);
 }

 for (c = cell_f()->nextl; c != cell_l(); c = c -> nextl)
 {
    if( !( c->flg & c_f_let ) )
        continue;

	if( c->h >= MAXHEI || c->h <= 0 )
		continue;

	if( p2_twin(c->vers[0].let) )
		numTwins++;

    name = c->vers[0].let;

	bufHeight[c->h]++;
	if( c->row-minBase >= 0 && c->h-minBase < MAXHEI )
        bufBase[c->row-minBase]++;

    IsNice = FALSE;
	IsConfirmed = FALSE;

    if(  (c->flg & c_f_confirmed) || (c->recsource == c_rs_bitcmp) )
    {
		IsConfirmed = TRUE;

        if(  (i=IsTwinCluster(c->nClust,c->vers[0].let,&twinName)) <= 1 )
        {
             IsNice = TRUE;
        }

        else if ( i == 3 ) // есть противоречащий кластер
        {
           numAntiCluster++;
           name = twinName;   // renamed !
        }
    }
	else // if( c->vers[0].prob < 180 )
	{
		notConfirmed++;
//		continue;
	}

    ch = let_lindef[name];

    if( (ch & v_bs1) || IsBigLetter(name) )
	{
      AddParamSize(&allUp,c,ch,ch & v_bs1,IsBigLetter(name));

      if( IsConfirmed )
         AddParamSize(&upS, c, ch,ch & v_bs1,IsBigLetter(name));

      if( IsNice )
         AddParamSize(&niceUp, c, ch,ch & v_bs1,IsBigLetter(name));
    }

    if( (ch & v_bs2) || IsSmallLetter(name) )
	{
      AddParamSize(&allDn,c,ch,ch & v_bs2,IsSmallLetter(name));

      if( IsConfirmed )
        AddParamSize(&downS, c, ch,ch & v_bs2,IsSmallLetter(name));

      if( IsNice )
         AddParamSize(&niceDn, c, ch,ch & v_bs2,IsSmallLetter(name));
    }

  }

 // нечего корректировать
  if( numTwins <= 0 )
  {
     language = sav_lang;
	 return 0;
  }

  // to avoid problems in Checking
  niceUp.basMin=MIN(niceUp.basMin,minBase+MAXHEI-1);
  niceDn.basMin=MIN(niceDn.basMin,minBase+MAXHEI-1);
  allUp.basMin=MIN(allUp.basMin,minBase+MAXHEI-1);
  allDn.basMin=MIN(allDn.basMin,minBase+MAXHEI-1);
  upS.basMin=MIN(upS.basMin,minBase+MAXHEI-1);
  downS.basMin=MIN(downS.basMin,minBase+MAXHEI-1);

  heiUp = heiDn = 0;
  basUp = basDn = -1;

  porog = 1;

  if( niceUp.sizeNum > 0 )
  {
	  heiUp = (niceUp.height + (niceUp.sizeNum>>1))/niceUp.sizeNum;
	  porog = MAX(porog,heiUp/6);
  }
  else if( upS.sizeNum > 0 )
  {
	  heiUp = (upS.height + (upS.sizeNum>>1))/upS.sizeNum;
	  porog = MAX(porog,heiUp/6);
  }

  if( niceDn.sizeNum > 0 )
  {
	  heiDn = (niceDn.height + (niceDn.sizeNum>>1))/niceDn.sizeNum;
	  porog = MAX(porog,heiDn/4);
  }
  else if( downS.sizeNum > 0 )
  {
	  heiDn = (downS.height + (downS.sizeNum>>1))/downS.sizeNum;
	  porog = MAX(porog,heiDn/4);
//	  heiDn = downS.sizeMax;
  }


  if( notConfirmed   <= 0 &&
	  numAntiCluster <= 0 &&
      upS.num > 0 &&
      downS.num > 0 &&
      downS.basMin >= upS.basMax + porog &&
	  heiUp > heiDn + porog
    )
  {
    language = sav_lang;
	return 0;  // all OK
  }



  if( niceUp.num > 0 )
	  basUp = niceUp.basMax;
  else if( upS.num > 0 )
	  basUp = upS.basMax;

  if( niceDn.num > 0 )
	  basDn = niceDn.basMin;
  else if( downS.num > 0 )
	  basDn = downS.basMin;

  //  не противоречат?
  //  сначала смотрим по очень хорошим Nick 06.08.2001
  if( heiDn > 0 && heiUp > 0 && heiDn + 1 >= heiUp )
  {
   if( niceUp.sizeNum > 0  && niceUp.num > niceUp.twin +1 &&
	   niceDn.num <= 0
	 )
   {
	  heiUp = (niceUp.height + (niceUp.sizeNum>>1))/niceUp.sizeNum;
	  porog = MAX(porog,heiUp/6);
	  heiDn = 0;
	  basDn = -1;
   }

   else if( niceDn.sizeNum > 0 && niceDn.sizeNum > 0  && niceDn.num > niceDn.twin +1 &&
	   niceUp.num <= 0
	 )
   {
	  heiDn = (niceDn.height + (niceDn.sizeNum>>1))/niceDn.sizeNum;
	  porog = MAX(porog,heiDn/4);
	  heiUp = 0;
	  basUp = -1;
   }
  }

  // проверяем по очень хорошим, или слегка сомнительным, или по всем
  if( heiUp > 0 && heiDn > 0)
  {
	  // Nick 27.06.2002 - if all OK
	  if( niceUp.sizeNum > 5 && niceDn.sizeNum > 5 &&
			(heiUp - heiDn)*3 > heiDn
		)
		bound = (heiUp+heiDn)/2;
	  else
         bound = CheckSizes(&heiUp,&heiDn,
	           niceUp.sizeNum>0?&niceUp:upS.sizeNum>0?&upS:&allUp,
			   niceDn.sizeNum>0?&niceDn:downS.sizeNum>0?&downS:&allDn, bufHeight, 1 );
  }

	// противоречие, нет прогиба
  if( heiUp == 0 && heiDn == 0 )
  {
	  {
	   int tmpBuf[MAXHEI];
	   memcpy(tmpBuf,bufHeight,MAXHEI*sizeof(int));
	   // сгладим
	   for(i=1;i<MAXHEI-1;i++)
	   {
		  bufHeight[i] +=tmpBuf[i-1]+tmpBuf[i+1];
		  bufHeight[i] /= 2;
	   }
	  }

	  // перепроверим по всем
       if( allDn.sizeNum > allUp.sizeNum +1 )
		   allUp.sizeMax = MAXHEI -1;
	   else if( allDn.sizeNum +1 < allUp.sizeNum  )
		   allDn.sizeMin = 1;
	   else  // везде
	   {
		   allUp.sizeMax = MAXHEI -1;
		   allDn.sizeMin = 1;
	   }

	   bound = CheckSizes(&heiUp,&heiDn, &allUp,&allDn, bufHeight,2);
  }


  // не нашли прогиб - смотрим по очень хорошим
  if( heiDn <= 0 && heiUp <= 0)
  {
   if( niceUp.sizeNum > 0  && niceUp.num > niceUp.twin +1 &&
	   niceDn.num <= 0
	 )
   {
	  heiUp = (niceUp.height + (niceUp.sizeNum>>1))/niceUp.sizeNum;
	  porog = MAX(porog,heiUp/6);
   }

   else if( niceDn.sizeNum > 0 && niceDn.sizeNum > 0  && niceDn.num > niceDn.twin +1 &&
	   niceUp.num <= 0
	 )
   {
	  heiDn = (niceDn.height + (niceDn.sizeNum>>1))/niceDn.sizeNum;
	  porog = MAX(porog,heiDn/4);
   }
  }

  if( heiDn <= 0 )
  {
	  if( heiUp > 0 )
		  heiDn = (heiUp*3)/4;
  }
  if( heiUp <= 0 )
  {
	  if( heiDn > 0 )
		  heiUp = (heiDn*4)/3;
  }

  if( porog == 1 )
  {
	  int j=0;
	  for(i=1;i<MAXHEI-1;i++)
		  if( bufHeight[i] > bufHeight[j])
			  j = i;

      porog = MAX(porog,j/5);
  }

  // не противоречат?
  // проверяем по очень хорошим, или слегка сомнительным, или по всем
  if( basUp >= 0 && basDn >= 0)
  {
   bound = CheckBases(&basUp,&basDn,
	           niceUp.num>0?&niceUp:upS.num>0?&upS:&allUp,
			   niceDn.num>0?&niceDn:downS.num>0?&downS:&allDn, bufBase, minBase );
   if( bound == 0)
   {
	   // попробуем учесть близнецов
	   i = UpDownByHeights(heiUp, heiDn);
	   if( i == 1 )
		   basDn = -1;
	   else if( i == 2)
		   basUp = -1;

	   if( i == 0 ) // не получилось ничего
	   {
	    if( ( niceUp.num > niceUp.twin || upS.num > upS.twin ||
			  niceUp.num >= niceUp.twin && niceUp.num > niceDn.num ) &&
		      niceDn.num <= niceDn.twin && downS.num <= downS.twin
		   )
		  basDn = -1;
	    else
	      basUp = -1;
	   }
   }
  }

  // не вышло по приличным - попробуем по всем
  if( basUp < 0 && basDn < 0 )
	   bound = CheckBases(&basUp,&basDn, &allUp,&allDn,bufBase, minBase);

  // слишком близко ?
  if( basDn >0 && basDn < basUp + porog )
	  basUp = basDn = -1;

  // не нашли прогиб - смотрим по очень хорошим
  if( basDn < 0 && basUp <= 0)
  {
   if( niceUp.sizeNum > 0  && niceUp.num > niceUp.twin +1 &&
	   niceDn.num <= 0
	 )
	 basUp = niceUp.basMax;
   else if( niceDn.sizeNum > 0 && niceDn.sizeNum > 0  && niceDn.num > niceDn.twin +1 &&
	   niceUp.num <= 0
	 )
	 basDn = niceDn.basMin;
  }

  if( basUp < 0 )
  {
	  if( basDn >= 0 )
		  basUp = basDn -(3*porog/2);
  }
  if( basDn < 0 )
  {
	  if( basUp >= 0 )
		  basDn = basUp + (3*porog/2);
  }

  // проверим базы - вдруг у нас многобазовость !
  // много несоответсвий
  if( basDn >= 0 && AntiUpBase(basDn) )
	  return -1;

  	  //  не вышло найти размеры?
  if( heiUp == 0 && heiDn == 0 )
  {
	  // хоть проверим уровень с соседями
	   CheckCluSosed(basUp,basDn);
       language = sav_lang;
	   return 0;
  }


  CheckCluHeights(heiUp, heiDn, basUp, basDn);

  language = sav_lang;
  return 1;
}
/////////////

// склеить и распознавать на втором проходе
static const uchar mask[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

static void StoreOneInterval( uchar *raster, int b, int e)
{
int i;

	for(i=b;i<e;i++)
	{
		raster[ i>>3 ] |=  mask[ i&7 ];
	}

	return;
}
//////////////////////////
// from make_raster, online_comp
static int addRaster(c_comp *w, uchar *raster,
					 int bytesx,
					 int startx, int starty
					 )
{
 lnhead * lp;
 interval *ip;
 uchar *pp;
 int lpool_lth;
 int wid = bytesx<<3;

 w = (c_comp *)((puchar)w + w->lines);
 lpool_lth=*(pint16_t)w;
 w=(c_comp *)((pint16_t)w+1);

 lpool_lth -= sizeof(int16_t);

 lp = (lnhead *)w;
 while ( lp->lth )
 {
   pp = raster + (starty+lp->row) * bytesx;

   ip = (interval *) (lp+1);
   for( ; ip!= NULL && ip->l > 0 ; ip++, pp += bytesx)
   {
	 StoreOneInterval( pp,
		   MAX(0, startx + ip->e - ip->l),
		   MIN(wid , startx + ip->e)
		   );

   }

   lp = (lnhead *)(ip+1);
 }

 return 1;
}
//////////////////////
static int recogGlueRaster( const s_glue * gl,
						    RecVersions *vers )
{
c_comp		*cp;
int			right,bottom,left,upper,wb;
RecRaster   recRast={0};
FonSpecInfo specinfo;
int		i;

    left=upper=30000;
	right=bottom=0;

	i=0;
    while((cp = gl->complist[i++]) != NULL)
	{
		left  = MIN(left  ,cp->left);
		upper = MIN(upper ,cp->upper);
		right = MAX(right ,cp->left  + cp->w);
		bottom= MAX(bottom,cp->upper + cp->h);
	}

	recRast.lnPixWidth  = right - left;
	recRast.lnPixHeight = bottom - upper;
	recRast.lnRasterBufSize=REC_MAX_RASTER_SIZE;

	wb = REC_GW_WORD8(recRast.lnPixWidth);

	if( recRast.lnPixWidth  <= 0 ||
		recRast.lnPixHeight <= 0 ||
		wb*recRast.lnPixHeight > REC_MAX_RASTER_SIZE
	)
			return 0;


	memset( recRast.Raster, 0 , wb*recRast.lnPixHeight);


	// make common raster
    i=0;
    while((cp = gl->complist[i++]) != NULL)
	{
		addRaster(cp, recRast.Raster, wb,
				  cp->left - left,
			      cp->upper - upper
				  );
    }

	memset(&specinfo,0,sizeof(specinfo));
	specinfo.palkiLeo = 1;

	i=p2_recog(&recRast,vers,&specinfo,0);

	return specinfo.nClust;
}
//////////////////////
