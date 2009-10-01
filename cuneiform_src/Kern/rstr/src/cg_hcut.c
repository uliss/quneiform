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

#include <string.h>
#include <stdlib.h>

#include "struct.h"
#include "func.h"
#include "cut_glue.h"
#include "fondefs.h"
#include "fon.h"
#include "ligas.h"
#include "status.h"
#include "linutil.h"

#include "compat_defs.h"
#include "minmax.h"

int16_t gbCol1,gbCol2;

typedef struct tagRecogStat
{
  int16_t nbig,ndust,weight;
} RecogStat;

static B_LINES bl;
static Bool fb1;
static Bool fb2;
static Bool fb3;
static Bool fb4;
static int16_t bl_lim=3;

static int16_t dirt_frag(cell **B, cell **E, cell *first, cell *last, Bool stop_first);
static void bl_cut(cell *B, cell *E, int16_t cut);
static Bool find_clust(uchar let);
static Bool clip_cell(int16_t j, cell *c, int16_t b1, int16_t b2, int16_t b3, int16_t b4, int16_t st_inc);
static void save_frag(cell *B, cell *E, RecogStat *rs, cell **sv_frag, int16_t *st_inc);
static void replace_frag(cell *B, cell *E, RecogStat *rs, cell *sv_frag);
static  int16_t create_cells(cell *whither, raster *r, cell *celist[], int16_t st_inc);
static cell *hide(cell *c, cell **clink);
static void restore(cell *clink, cell *wherever);
static void del_hided(cell *clink);
static Bool capital(uchar let);

void  base_lines_cut()
{
  cell *BI,*EI,*B0=cell_f()->nextl,*E0=cell_l();
  int16_t cut=0;
  get_b_lines(NULL,&bl);
  fb1=bl.n1>0 && bl.n1<255;
  fb2=bl.n2>0 && bl.n2<255;
  fb3=bl.n3>0 && bl.n3<255;
  fb4=bl.n4>0 && bl.n4<255;

  if (fict(B0) || !(cut=dirt_frag(&B0,&E0,B0,E0->prevl,FALSE)))
    return;

  BI=B0;  EI=E0=E0->nextl;
  if (E0->nextl)  E0=E0->nextl;
  while(dirt_frag(&BI,&EI,cell_f()->nextl,cell_l()->prevl,TRUE))
  {
    cell *R;
    if (fict(BI->prevl)) BI=cell_f()->next;
    else                 BI=BI->prevl;
    if (fict(EI->nextl)) { R=cell_l();   EI=R->prev;  }
    else                 { EI=EI->nextl; R=EI->nextl; }
    bl_cut(BI,EI,cut);
    BI=R;  EI=E0;
  }
}
/*
static Bool dirt_frag(cell **B,cell **E)
{
  cell *c=cell_f()->next;
  gbCol1=gbCol2=0;
  glsnap('o',c,"Cut by bases; input r_col for begin and end");
  if (gbCol1==0 && gbCol2==0 || gbCol1>gbCol2)
    return FALSE;

  *B=*E=NULL;
  for ( ; c->next; c=c->next)
  {
    if (c->r_col==gbCol1)   *B=c;
    if (c->r_col==gbCol2) { *E=c;  break; }
  }

  return (B && E);
}
*/

int32_t testDirt(CSTR_rast *beg, CSTR_rast *end)
{
  CSTR_rast rst=*beg,first=rst,last=CSTR_GetNext(*end);
  CSTR_rast capb=0,cape=0;
  int16_t dirtup=0,dirtdown=0,ncap=0;

  get_b_lines(NULL,&bl);
  fb1=bl.n1>0 && bl.n1<255;
  fb2=bl.n2>0 && bl.n2<255;
  fb3=bl.n3>0 && bl.n3<255;
  fb4=bl.n4>0 && bl.n4<255;

  *beg=*end=0;

  for ( ; rst != last; rst=CSTR_GetNext(rst))
  {
    CSTR_rast_attr attr;
    UniVersions     uni;
    CSTR_GetAttr(rst,&attr);

    if (fb1 && attr.row+bl_lim < bl.b1)      { dirtup++;   goto mark; }
    if (fb4 && attr.row+attr.h-bl_lim > bl.b4) { dirtdown++; goto mark; }

    CSTR_GetCollectionUni(rst,&uni);

    if (uni.Alt[0].Prob<trs2)
    {
      uchar let=uni.Alt[0].Code[0],letpos = let_linpos[let];
      int16_t bot=attr.row+attr.h;
      Bool d2=attr.row<bl.b2-bl_lim,d3=bot>bl.b3+bl_lim;

      if (uni.lnAltCnt==0)
      {
        if (d2)  { dirtup++;   goto mark; }
        if (d3)  { dirtdown++; goto mark; }
      }
      if (rst != first && capital(let))
      {
        ncap++;
        if (!capb)  capb=rst;
        cape=rst;
      }
      switch(letpos & 0x0F)
      {
      case 1: case 4:
        if (fb3 && d3)  { dirtdown++; goto mark; };
        break;
      case 2: case 5: case 6: case 7:
        if (fb4 && bot>bl.b4-bl_lim)  { dirtdown++; goto mark; };
        break;
      }
      if (!strchr("3568Ѓ‚…‡Ќљњ",let))
      {
        letpos >>= 4;
        switch(letpos)
        {
        case 1:
          if (fb1 && abs(attr.row-bl.b1)>bl_lim)  { dirtup++;   goto mark; }
          break;
        case 2: case 10:
          if (fb2 && d2)  { dirtup++;   goto mark; }
          break;
        case 3:  case 4:  case 5:
          if (fb1 && attr.row<bl.b1+bl_lim)  { dirtup++;   goto mark; }
          break;
        }
      }
    }
    continue;

mark:
    if (!(*beg))  *beg=rst;
    *end=rst;
  }

  if (dirtup==0 && ncap>1)  dirtup += ncap;
  if (!(*beg))   { *beg=capb;  *end=cape; }
  if (dirtup > 2*dirtdown)  return  1;
  if (dirtdown > 2*dirtup)  return -1;
  return 0;
}

static int16_t dirt_frag(cell **B, cell **E, cell *first, cell *last, Bool stop_first)
{
  cell *c=*B,*end=*E;
  cell *capb=NULL,*cape=NULL;
  int16_t dirtup=0,dirtdown=0,ncap=0;
/*
  gbCol1=gbCol2=0;
  glsnap('o',c,"Cut by bases; input r_col for begin and end");
  if (gbCol1 != 0 && gbCol2 != 0 && abs(gbCol1)<=abs(gbCol2))
    return gbCol1/abs(gbCol1);
*/
  *B=*E=NULL;
  for ( ; c != end && !fict(c); c=c->nextl)
  {
    if (c==last && 4*c->w < c->h)  continue;

    if (fb1 && c->row+bl_lim < bl.b1)      { dirtup++;   goto mark; }
    if (fb4 && c->row+c->h-bl_lim > bl.b4) { dirtdown++; goto mark; }

    if (c->vers[0].prob<trs2)
    {
      uchar let=c->vers[0].let;
      uchar letpos = let_linpos[let];
      int16_t bot=c->row+c->h;
      Bool d2 = (fb2) ? c->row<bl.b2-bl_lim : TRUE;
      Bool d3 = (fb3) ? bot>bl.b3+bl_lim : TRUE;

      if ((c->pr_vers.prob>=220 ||
				strchr("аж–й™",c->pr_vers.let) &&
                !is_russian_baltic_conflict(c->pr_vers.let)&&// 17.07.2001 E.P.
				!is_russian_turkish_conflict(c->pr_vers.let) // 21.05.2002 E.P.
                        ) && !find_clust(c->pr_vers.let)
		  )
        continue;

      if (!c->nvers)
      {
        uchar let=c->pr_vers.let;
//        if (c->pr_vers.prob>=220)  continue;
        if (d2)
        {
          if (!d3)
          {
            if (c->pr_vers.prob>100 && strchr("3568EHЃ‚…‡Ќљњ",let))
              continue;
            if (strchr("EHЃ‚…Ќљњ",let) || 5*c->w < 4*c->h)
            {
              STICK *st;
              if (c->n_baton==255)
                sticks_in_letter(c,0,&st);
              if (c->n_baton==1 && abs(c->save_baton[0].l-c->h)<=bl_lim ||
                  c->n_baton==2 && abs(c->save_baton[1].l-c->h)<=bl_lim)
              continue;
            }
          }
          dirtup++;
          goto mark;
        }
        if (d3)
        {
          if (!d2)
            if (
				// 16.07.2001 E.P. конфликт с a_bottom_accent_baltic 0xe0
				!is_baltic_language(language) &&
				let=='а' &&	// 0xe0
				(c->pr_vers.prob>100 || abs(c->save_baton[0].l-c->h)<=bl_lim)
			   )
              continue;

          dirtdown++;
          goto mark;
        }
      }

      if ((c != first || stop_first) && capital(let))
      {
        ncap++;
        if (!capb)  capb=c;
        cape=c;
      }
      if (let!='а' ||
			// 16.07.2001 E.P. конфликт с a_bottom_accent_baltic 0xe0
		    is_baltic_language(language)
		  )
        switch(letpos & 0x0F)
        {
        case 1: case 4:
          if (fb3 && d3)  { dirtdown++; goto mark; };
          break;
        case 2: case 5: case 6: case 7:
          if (fb4 && bot>bl.b4-bl_lim)  { dirtdown++; goto mark; };
          break;
        }
      letpos >>= 4;
      if (!strchr("3568Ѓ‚…‡Ќљњ",let))
        switch(letpos)
        {
        case 1:
          if (fb1 && abs(c->row-bl.b1)>bl_lim)  { dirtup++;   goto mark; }
          break;
        case 2: case 10:
          if (fb2 && d2)  { dirtup++;   goto mark; }
          break;
        case 3:  case 4:  case 5:
          if (fb1 && c->row<bl.b1+bl_lim)  { dirtup++;   goto mark; }
          break;
        }
/*
      if (d2 || d3)
      {
        ClustInfo cli;
        int32_t nc=FONGetNumCluster(c->r_clink);
	      FONGetClustInfo(&cli,nc);
        if (cli.prob>=trs2)
        {
          if (d2) dirtup++;
          if (d3) dirtdown++;
          goto mark;
        }
      }
*/
    }
    if (stop_first && *B)  break;
    continue;

mark:
    if (!(*B))  *B=c;
    *E=c;
  }

  if (dirtup==0 && ncap>1)  dirtup += ncap;
  if (!(*B))   { *B=capb;  *E=cape; }
/*
  if (*B && !fict((*B)->prevl))  *B=(*B)->prevl;
  if (*E && !fict((*E)->nextl))  *E=(*E)->nextl;
*/
  if (dirtup > 2*dirtdown)  return  1;
  if (dirtdown > 2*dirtup)  return -1;
  return 0;
}

static Bool find_clust(uchar let)
{
#ifdef _USE_FON_
  ClustInfo clustinfo;
  clustinfo.let=let;
  return FONGetClustInfo( &clustinfo,1) > 0;
#else
  return FALSE;
#endif
}

static void bl_cut(cell *B, cell *E, int16_t cut)
{
  cell *f=B,*LC=B->prev,*RC=E->next;
  RecogStat rs;
  cell *sv_frag=NULL;
  int16_t st_inc;          //баҐ¤­Ё© ­ Є«®­
  Bool repair=TRUE;

  //save current state
  save_frag(B,E,&rs,&sv_frag,&st_inc);

  //make cut by base lines
  while (f != RC)
  {
    cell *c=f;
    f=f->next;
    c->cg_flag &= ~c_cg_cutdone;  //c_cg_cutdone flag prevents cell to cut in smart_cut
    if (let_or_bad(c) && c->vers[0].prob>=trs2)
    {
      c->flg &= ~c_f_bad;  c->flg |= c_f_let;
    }
    else
    {
      int16_t b1 = (fb1 && cut>0) ? bl.b1-c->row : 0;
      int16_t b2 = (fb2 && cut>0) ? bl.b2-c->row : 0;
      int16_t b3 = (fb3 && cut<0) ? bl.b3-c->row : 0;
      int16_t b4 = (fb4 && cut<0) ? bl.b4-c->row : 0;

      if (let_or_bad(c))
      {
        c->flg &= ~c_f_let;  c->flg |= c_f_bad;
      }

      if (b1>0 || b2>0 || b3<c->h || b4<c->h)
      {
        glsnap('o',c,"try clip");
        repair &= clip_cell(0,c,b1,b2,b3,b4,st_inc);
      }
    }
  }

  if (!repair)
  {
    if (LC->next != RC && LC != RC->prev)
    {
      process_word(LC->next,RC->prev);
      replace_frag(LC->next,RC->prev,&rs,sv_frag);
    }
  }
  else
    del_hided(sv_frag);
}

static Bool clip_cell(int16_t j, cell *c, int16_t b1, int16_t b2, int16_t b3, int16_t b4, int16_t st_inc)
{
//®ваҐ§ Ґв ®в c ўбҐ, зв® ўле®¤Ёв §  b1,b4 (®вбзҐв ®в c->row); b2,b3 - ¤®Ї®«­ЁвҐ«м­лҐ а §аҐ§л,
// Ґб«Ё ­г¦­®
//  int16_t j;
  int16_t crow=c->row;  //b1,b2,b3,b4 refer to crow
  Bool repair=TRUE;

//  for (j=0; j<2 && c; j++)
  if (j<2)
  {
    Bool cut_made=FALSE;

    if (j==0)
    {
      if (b1<=2 || b1>=c->h)  b1=0;
      if (b4>=c->h-2)  b4=0;
      cut_made = b1>0 || b4>0;
    }
    else
    {
      if (b2<=2 || b2>=c->h)   b2=0;
      if (b3<=0 || b3>c->h-2)  b3=0;
      cut_made = b2>0 || b3>0;
    }

    if (cut_made)
    {
      raster r;                   //Їа®¬Ґ¦гв®з­л© а бва
      cell *celist[MAX_SECT+1];
      int16_t wbyte=(c->w+7)>>3,i,nall,size=((c->w+7)>>3)*c->h;
      int16_t nbig=0;

      if (size>sizeof(r.pict))
        return FALSE;
      memcpy(&(r.pict),(uchar*)save_raster(c),size);
      r.w=c->w; r.h=c->h; r.top=c->r_row; r.left=c->r_col;

      if (j==0)
      {
        if (b1>0)  memset(r.pict,0,b1*wbyte);
        if (b4>0)  memset(&(r.pict[(b4+1)*wbyte]),0,(c->h-b4-1)*wbyte);
      }
      else
      {
        if (b2>0)  memset(&(r.pict[b2*wbyte]),0,wbyte);
        if (b3>0)  memset(&(r.pict[(b3+1)*wbyte]),0,wbyte);
      }

      if (!(nall=create_cells(c,&r,celist,st_inc)))
        return FALSE;

      for (i=0; i<nall; i++)
      {
        cell *t=celist[i];
        t->flg_new |= CSTR_fn_bl_cut;
        t->cg_flag=c->cg_flag;
        if (t->r_col != c->r_col)           t->cg_flag &= ~c_cg_cutl;
        if (t->r_col+t->w != c->r_col+c->w) t->cg_flag &= ~c_cg_cutr;
        if (dust(t))
        {
//          if (t->row+t->h==crow+b2 || t->row==crow+b3+2)
          continue;
        }
        nbig++;
        if (bad(t))
        {
          full_recog(t,NULL,trs2,trs2);
          if (!let(t))  repair &= clip_cell((int16_t)(j+1),t,0,(int16_t)(crow+b2-t->row),(int16_t)(crow+b3-t->row),0,st_inc);
        }
      }
      del_cell(c);
      c=NULL;
      return repair && nbig != 0;
    }
    else
      return clip_cell((int16_t)(j+1),c,0,(int16_t)(crow+b2-c->row),(int16_t)(crow+b3-c->row),0,st_inc);
  }
  return let(c) != 0;
}

static int16_t create_cells(cell *whither, raster *r, cell *celist[], int16_t st_inc)
{
  int16_t i;
  MN  *mn=c_locomp(r->pict,(int16_t)((r->w+7)>>3),r->h,r->top,r->left);
  for (i=0; i<MAX_SECT && mn; i++,mn=mn->mnnext)
  {
    cell *c=create_my_cell(mn,whither,0,0);
    inter_diff(c);
    set_erection(c, st_inc);
    celist[i]=c;
  }
  return i;
}

static void save_frag(cell *B, cell *E, RecogStat *rs, cell **sv_frag, int16_t *st_inc)
{
  cell *celist[MAX_SECT];
  int16_t n=0;
  E=E->next;
  B->complist=NULL;
  rs->weight=256;  rs->nbig=rs->ndust=0;
  for ( ; B != E; B=B->next)
  {
    cell *c=new_cell();
    *c=*B;
    c->complist=(c_comp *)(*sv_frag);
    *sv_frag=c;
    if (let_or_bad(B))
    {
      rs->nbig++;
      rs->weight = MIN(rs->weight,B->vers[0].prob);
    }
    else
      if (fb2 && fb3 && c->row > bl.b2 && c->row+c->h < bl.b3)
        rs->ndust++;
    if (n<MAX_SECT)  celist[n++]=B;
  }
  *st_inc = erection_compose_inc(n, celist);
}

static void replace_frag(cell *B, cell *E, RecogStat *rs, cell *sv_frag)
{
  cell *c;
  int16_t weight=256,nbig=0,ndust=0;
//  B_LINES bs=bl;

  E=E->next;
  for (c=B; c != E; c=c->next)
    if (let_or_bad(c))
    {
      nbig++;
      weight = MIN(weight,c->vers[0].prob);
    }
    else
      if (fb2 && fb3 && c->row > bl.b2 && c->row+c->h < bl.b3)
        ndust++;

  if (nbig < rs->nbig && ndust > rs->ndust ||  //some big cells's cut to dust
      weight <= rs->weight)
  {
    for (c=B; c != E; c=c->next)  c=del_cell(c);
    restore(sv_frag,c);
  }
  else
    del_hided(sv_frag);
}

static cell *hide(cell *c, cell **clink)
{
    cell *cp=c->prev;
    c->complist=(c_comp *)(*clink);
    del_retain_cell(c);
    *clink=c;
    return cp;
}

static void restore(cell *clink, cell *wherever)
{
  while (clink)
  {
    insert_cell(clink,wherever);
    clink=(cell *)clink->complist;
  }
}

static void del_hided(cell *clink)
{
  while (clink)
  {
    free_cell(clink);
    clink=(cell *)clink->complist;
  }
}

static Bool capital(uchar let)
{
    return (let>='0' && let<='9' || let>='A' && let<='Z' || let>='А' && let<='Я');
}
