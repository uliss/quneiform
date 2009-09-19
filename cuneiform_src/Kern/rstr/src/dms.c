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

/*
  ЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫ
  ЫЫ                                                              ЫЫ
  ЫЫ     Copyright (C) 1990 Cognitive Technology Corporation.     ЫЫ
  ЫЫ	 All rights reserved. This program is proprietary and     ЫЫ
  ЫЫ     a trade secret of Cognitive Technology Corporation.      ЫЫ
  ЫЫ                                                              ЫЫ
  ЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫ
*/

/*********************************************************************/
/*                                                                   */
/*                           module DMs (subr)                       */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>
#include <string.h>
#include "nt_types.h"
#include "struct.h"
#include "cuthdr.h"
#include "dmconst.h"
#include "func.h"

extern servBOX SBOX;
extern INT best_answer_BOX;

void min3(v_val *a, BYTE b, v_val *c)
 {
Z = &string;
// insert value 'b' in proper place of vector 'a'; vector 'c' -- target
 if (b < 70) b = 0;
 if (b <= a->v3)
  { c->v3=b; c->v2=a->v3; c->v1=a->v2; return; }
 c->v3=a->v3;
 if (b <= a->v2)
  { c->v2=b; c->v1=a->v2; return; }
 c->v2=a->v2;
 if (b <= a->v1)
  { c->v1 = b; return; }
 c->v1 = a->v1;
 }

INT compare_vect(v_val *a, v_val *b)
 {
 INT x;
 INT a1;
 INT a2, a3;
 INT b1;
 INT b2, b3;

 x = (a3 = a->v3)-(b3=b->v3);
 if (x)
   return x;
 x = (a2 = a->v2) - (b2=b->v2);
 if (x)
   return x;
 x = (a1 = a->v1) - (b1=b->v1);
   return x;
 /************* 93.06.15 attempt to ignore weak 2nd component's difference
  {
   y = MAX(a2, b2);
   if (y > 170)      // one of vectors -- not too bad
     return x;
   if (abs(x) > 70)  // solid deficit
     return x;
   // both vectors weak in 2nd component
   y = (a1=a->v1)-(b1=b->v1);
   if (MAX(a1,b1) < 170)  // both unreliable
     return x;
   if (abs(y) > 50)  // reliable deficit of 1st component  50=220-170
     return y;
   return x;
 }
 93.06.15 *******************/
 //// return  (a->v1 - b->v1);
 }

#pragma warning ( disable : 4047 )				   //AK 11.03.97

void save_vers(cell *C, SVERS *S)
 {
 INT i, nn;
 version *s, *d;
 S->nvers  = C->nvers;
 S->source = C->recsource;
 S->pos = C->cpos;
 S->flg = (char)C->flg;
 S->broken_II = C->broken_II;
 S->cg_flag_fine  = C->cg_flag_fine;
 S->accent_leader = C->accent_leader;
 S->bottom_accent = C->bottom_accent;
 S->history       = C->history; // Nick
 S->clink         = C->clink;   // Nick
 S->clu_attr      = C->clu_attr;   // Nick
 S->nClust        = C->nClust;   // Nick
 nn=S->nvers;
 if (nn==0)  nn = 1;
 for (i=0, s=&(C->vers), d=&(S->vers); i<nn; i++, d++, s++) //AK 04.03.97 ? for address
 {
   d->let = s->let;
   d->prob = s->prob;
 }
 S->pr_vers=C->pr_vers;
 if (S->nvers <= 0) return;
 d = &(S->vers[S->nvers]);
 d->let = d->prob = 0;
 }

void rest_vers(cell *C, SVERS *S)
{
 INT i, nn;
 version *s, *d;
 C->nvers = S->nvers;
 C->flg   = S->flg;
 C->cpos  = S->pos;
 C->recsource = S->source;
 C->broken_II = S->broken_II;
 C->cg_flag_fine  = S->cg_flag_fine;
 C->accent_leader = S->accent_leader;
 C->bottom_accent = S->bottom_accent;
 C->history       = S->history; // Nick
 C->clink         = S->clink;   // Nick
 C->clu_attr      = S->clu_attr;   // Nick
 C->nClust        = S->nClust;   // Nick

 nn=S->nvers;
 if (nn==0) nn=1;
 for (i=0, d=&(C->vers), s=&(S->vers); i<nn; i++, d++, s++)  //AK 04.03.97 ? for address
 {
  d->let = s->let;
  d->prob = s->prob;
 }
 C->pr_vers=S->pr_vers;
 if (C->nvers == 0) return;
 d = &(C->vers[C->nvers]);
 d->let = d->prob = 0;
 adjust_links(C);
}

INT signif_line_count (cell *a)
{
 lnhead *Lp1, *Lp2;
 c_comp *cp1;

 INT  nl, lc1, Lc1;

  if (a->env == NULL) return 0;
  nl = 0;
  cp1 = a->env;
  Lc1=cp1->nl;	    // number of lines in component
  Lp1=(lnhead *) ( (char *)cp1 + cp1->lines + 2); // beginning of first line
  Lp2=Lp1;
  for (lc1=0; lc1 < Lc1; lc1++)
  {
    if (Lp1->h > 1)  nl++;
    Lp2=Lp1=(lnhead *) ((char *)Lp2+Lp2->lth);
  }
 return nl;
}
/////////////////////////
INT cell_is_BOX_solid (cell  *B1)
{
 SVERS svers;
 if (!tenv(B1))
  return 0;

 SBOX.best_BOX[0].ltr=0;
 save_vers(B1,&svers);

 memset(&mBOX,0,sizeof(mBOX));

 SBOX.rast_w = B1->w; SBOX.rast_h = B1->h;
 SBOX.up_row = B1->r_row; SBOX.left_col = B1->r_col;

 comp_to_box(mBOX,B1->env,SBOX.up_row,SBOX.left_col,SBOX.rast_h,SBOX.rast_w);

 if ((SBOX.matrBOX.isq_rt = long_sqrt((*scalar)(mBOX,mBOX)))==0)
   SBOX.matrBOX.isq_rt=1;

 embBOX(&SBOX,&svers, (Bool)((B1->pos_inc&erect_rot)!=0));      // BOX estimate
 return best_answer_BOX;

}
//////////////////////////////
//#ifdef _BAL_VERSION_
// for BAL - new version 6.02.2001
void save_rast_vers(CSTR_rast C, SVERS *S)
 {
 INT i, nn;
// version *s, *d;
 UniVersions vers;
 CSTR_rast_attr attr;
 version *d;

 CSTR_GetCollectionUni(C,&vers);
 CSTR_GetAttr(C,&attr);

 S->nvers  = (Int16)vers.lnAltCnt;
 S->source = attr.recsource;
 S->pos = attr.cpos;
 S->flg = (char)attr.flg;
 S->broken_II = attr.broken_II;
 S->cg_flag_fine  = attr.cg_flag_fine;
 S->accent_leader = attr.accent_leader;

// S->bottom_accent = attr.bottom_accent; - у CSTR нет
 S->bottom_accent = 0;

 S->history       = (uchar)attr.RecogHistory; // Nick
 S->clink         = attr.clink;   // Nick
 S->clu_attr      = attr.clu_attr;   // Nick
 S->nClust        = attr.nClust;   // Nick

 nn=S->nvers;
// if (nn==0)  nn = 1;
 for (i=0, d=&(S->vers); i<nn; i++, d++ ) //AK 04.03.97 ? for address
 {
   d->let  = vers.Alt[i].Liga;
   d->prob =  vers.Alt[i].Prob;
 }

 d = &(S->vers[S->nvers]);
 d->let = d->prob = 0;

 S->pr_vers= S->vers[0];
}
////////////////
Bool32 p2_accept_Cell( cell *c,CSTR_rast_attr *rst, CCOM_comp *cmp, INT scale);
///////////////////
INT rast_is_BOX_solid (CSTR_rast B1,Int16 scale)
{
 SVERS svers;
 CCOM_comp *cmp;
 CSTR_rast_attr attr;
 c_comp *env;

 extern PBYTE kit_curr;

 cmp = CSTR_GetComp(B1);
 CSTR_GetAttr(B1,&attr);

// if (!tenv(B1))
 if( !cmp || (attr.cg_flag & CSTR_cg_noenv) )
     return 0;

 if( cmp->type == ch_great)
	 return 0;

 SBOX.best_BOX[0].ltr=0;
 save_rast_vers(B1,&svers);

 memset(&mBOX,0,sizeof(mBOX));

 SBOX.rast_w = attr.w>>scale;
 SBOX.rast_h = attr.h>>scale;
 SBOX.up_row = attr.r_row>>scale;
 SBOX.left_col = attr.r_col>>scale;

 // переведем CCOM_comp -> c_comp
 if ( !p2_accept_Cell(NULL,&attr,cmp,scale) )
	 return 0;

  // вернем - добавили при p2_accept_Cell()
 kit_curr -= sizeof(c_comp)+cmp->size_linerep+2;
 env = (c_comp *)kit_curr;
 // change to real !
 // must be real - error somewhere
 // when C_COMP<->c_comp ?
 env->upper = attr.r_row>>scale;
 env->left  = attr.r_col>>scale;

 comp_to_box(mBOX,env,
	         SBOX.up_row,SBOX.left_col,
	         SBOX.rast_h,SBOX.rast_w);


 if ((SBOX.matrBOX.isq_rt = long_sqrt((*scalar)(mBOX,mBOX)))==0)
   SBOX.matrBOX.isq_rt=1;

      // BOX estimate
 embBOX(&SBOX,&svers, (Bool)((attr.pos_inc&erect_rot)!=0));


 return best_answer_BOX;
}
//////////////////////
//#endif

