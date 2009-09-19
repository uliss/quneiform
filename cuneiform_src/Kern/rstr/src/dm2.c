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


/*********************************************************************/
/*                                                                   */
/*                           module DM2                              */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nt_types.h"
#include "struct.h"
#include "cuthdr.h"
#include "dmconst.h"
#include "func.h"
#include "status.h"
#include "ligas.h"
#include "lang.h"
#include "linutil.h"

#include "tuner.h"

#include "compat_defs.h"

static char txg1[]={"LOCOMP ERR"};
static char txg2[]={"NO CELL got"};
static char txg3[]={"        p=(%d,%d)"};
static char txg4[]={"        GL OK"};
static char txg5[]={"        BOX OK"};
static char txg6[]={"n,a,m,e,l"};
// bad letters are allowed to eat good letters:
//    if the eaten letter is in list
//    if the result is registered
//    if the result is better than 190
//    if the result is better than ( 2*good + bad)/3
// different letters and results are allowed in (bad+good) and in (good+bad)

/*
	0,1 pos reserved for liga_i,liga_exm
	10,11 for UKR_I,II
	12,13 for turkish palka
*/
static BYTE sticks_left_to_bad[]  =  {"  /l1IJ)}]  11"};


static BYTE letters_left_to_bad[] =  {" nrvtcC(u<>мњЇ|б‘Ј"}; // 0th pos for sticks

static char ltmp0[] = "kDPbhЏЇ "; /* stick */
static char ltmp1[] = "m"; /* n */
static char ltmp2[] = "nm"; /* r */
static char ltmp3[] = "w"; /* v */
static char ltmp4[] = "u"; /* t */
static char ltmp5[] = "ao"; /* c */
static char ltmp6[] = "O6"; /* C */
static char ltmp7[] = "oO6"; /* ( */
static char ltmp8[] = "w"; /* u */
static char ltmp9[] = "od®"; /* < */
static char ltmp10[] = "xXе"; /* > */
static char ltmp11[] = "л"; /* м */
static char ltmp12[] = "›"; /* › */
static char ltmp13[] = "ий"; /* Ї */
static char ltmp14[] = "­ЌЁ€ЇЏЄЉ¬Њ"; /* | */
static char ltmp15[] = "®"; /* б */
static char ltmp16[] = "Ћ"; /* ‘ */
static char ltmp17[] = "Ї"; /* Ј */

static char *results_left_to_bad[] = {
        ltmp0, ltmp1, ltmp2, ltmp3, ltmp4, ltmp5, ltmp6, ltmp7, ltmp8, ltmp9,
        ltmp10, ltmp11, ltmp12, ltmp13, ltmp14, ltmp15, ltmp16, ltmp17
};

/*
	0 pos reserved for liga_i
	6-7 pos for turkish II_dot_accent, i_sans_accent 21.05.2002 E.P.
*/
static BYTE sticks_right_to_bad[] =  {" /l1I]11"};  // 1st pos reserved for liga_i
static BYTE letters_right_to_bad[] = {" nvt)u><бж–®ЋЄч"}; // 0th pos for sticks

static char rtmp0[] = "d­ЁЇЏ";
static char rtmp1[] = "m"; /* n */
static char rtmp2[] = "w"; /* v */
static char rtmp3[] = "u"; /* t */
static char rtmp4[] = "o"; /* ) */
static char rtmp5[] = "w"; /* u */
static char rtmp6[] = "bop6D®"; /* < */
static char rtmp7[] = "kXxЄ¦"; /* > */
static char rtmp8[] = "Є¦"; /* б */
static char rtmp9[] = "й"; /* ж */
static char rtmp10[] = "™"; /* – */
static char rtmp11[] = "о"; /* ® */
static char rtmp12[] = "ћ"; /* Ћ */
static char rtmp13[] = "¦"; /* Є */
static char rtmp14[] = "и"; /* ч */

static char *results_right_to_bad[] = {
        rtmp0, rtmp1, rtmp2, rtmp3, rtmp4, rtmp5, rtmp6, rtmp7, rtmp8, rtmp9,
        rtmp10, rtmp11, rtmp12, rtmp13, rtmp14,
};

static char *ltoi;
static INT lton;
static INT glue_pass;
static char percgot;       // percent was got as result of a glue
extern char db_pass;

void collect_pat(cell *BC, s_glue *GL, cell *EC);
cell * finpat(cell *BC, s_glue *GL, INT var, BYTE flag,BYTE pen);

extern BYTE db_status;	//snap presence
//extern BYTE accent_tab[];

static void glue_let_dust();
static void glue_let_bad();
static void glue_dust_dust();
#ifdef UFA
static void glue_all_dusts();
#endif
static INT contain(cell *,cell *);
static cut_pos cposd;
// russian glue-heuristics
static void glue_III();
static Bool config_III(cell *BC,cell *EC, cell *ECN);
static Bool config_brace_and_K(cell *BC,cell *EC);
static Bool config_CapRusGe_and_bad(cell *BC,cell *EC);

static INT is_bad_turkish_glue(BYTE c1, BYTE c2, BYTE c3); // 27.06.2002 E.P.

void make_all_glues()
 {
 cell *BC;
 uint16_t i;

 sticks_left_to_bad[0] = liga_i;	// 10.09.2000 E.P.
 sticks_left_to_bad[1] = liga_exm;	// 10.09.2000 E.P.

  // Турецкие палки
 if(is_turkish_language(language))
	{
	sticks_left_to_bad[12]=II_dot_accent;
	sticks_left_to_bad[13]=i_sans_accent;

	sticks_right_to_bad[6] = II_dot_accent;
	sticks_right_to_bad[7] = i_sans_accent;

	results_left_to_bad[9][2] = 'd';
	results_left_to_bad[15][0] = 0;
	}
 else
	{
	sticks_left_to_bad[12]='1';
	sticks_left_to_bad[13]='1';

	sticks_right_to_bad[6] = '1';
	sticks_right_to_bad[7] = '1';

	results_left_to_bad[9][2] = '®';
	results_left_to_bad[15][0] = '®';
	}

  if( langUkr ){
     sticks_left_to_bad[10] = UKR_I;
     sticks_left_to_bad[11] = UKR_II;
     *((BYTE*)&results_left_to_bad[0][7]) = (BYTE)'Љ';
  }

 snap_newpass('b');
 percgot = 0;
 Z=&string;
 BC=cell_f();
 while (1)
 {
  BC=BC->next;
  if (BC->flg & c_f_fict)
   break;
 if(BC->flg & (c_f_solid|c_f_space)) BC->cg_flag |= c_cg_noglue; // Vald
 else          BC->cg_flag &= ~c_cg_noglue;
 }
 BC=cell_f();
 while (1)
 {
  cell *DOT;
  BC=BC->nextl;
  if (BC->flg & c_f_fict)
   break;
  if(BC->cg_flag & c_cg_noglue) continue; // Vald
  if( BC->accent_leader )
    continue; // Oleg : 06/10/97 17:51 : can't glued fictive images
  if ((BC->nvers > 0) && (BC->vers[0].prob > 220))
  { char c;
    DOT = NULL;
    c = BC->vers[0].let;
    if ((c == 'i') || (c == 'j'))
      DOT = dot_ij (BC);
    if (c == 'n')
      DOT = dot_ri (BC);
    if (DOT)
      DOT->cg_flag |= c_cg_noglue;
  }
 }
 number();
 glue_pass = 0;
 glue_let_dust();
 glue_pass = 1;
 glue_let_dust();
 glue_let_bad();
 glue_pass = 0;
 glue_dust_dust();
#ifdef UFA
 glue_all_dusts();
#endif
 if( language==LANG_RUSSIAN )
  glue_III();
 if (percgot)    // a % was got at glue (bad to dust)
   perc();
 BC=cell_f();
 while (1)
  {
  BC=BC->nextl;
  if (BC->flg & c_f_fict)
   break;
  i= BC->vers[0].prob;
  if (i > 70)
    if( !(BC->flg & (c_f_solid|c_f_space)) ) BC->flg = c_f_let;
  }
 if (snap_activity('b'))
  {
  snap_show_text("GL end");
  snap_monitor();
  }
 }

void collect_pat(cell *BC, s_glue *GL, cell *EC)
 {
 //cell *WC;
 INT  n, fl;
 if (db_status)
   snap_newcell(BC);
 fl = 0;
 EC=EC;
 GL_to_SBOX(GL);     // GL --> SBOX
 SBOX_to_static();          // SBOX --> globals
 static_to_cell(BC);         // globals --> cell
/*
 for (n=0; n < GL->maxnc; n++)
  {
  WC=GL->maxlist[n];
  if ((WC != BC) && (WC != EC))
     del_cell(WC);
  }
 if (EC)
     del_cell(EC);
     */
    compose_cell (GL->maxnc, GL->maxlist, BC);
 //BC->cg_flag |= c_cg_noenv;
 n = GL->ncell = GL->maxnc;
 glsnap(db_pass,BC,"cell env corrected");
 }


static void glue_let_dust()
 {
//
//	This procedure organizes the process of decision making.
//	Namely it is responsible to make post recognition.
//	ON ENTRY: start_cell is the address of the beginning of
//		  string of components.
//	ON EXIT : better recognized string.
//
 s_glue GL={0};
 cell *BC,*LC,*WC,*WS,*CC,*DC;
 INT bdiff; char dflag;
 INT i, nsc, ngc, n, p1, p1l, p2;
 SVERS svv1;
 char  defl, ww[40];
 BYTE c1, c1l, c2, cp;
 cell bc;
 MN *mn;
 INT  inc=0;

 snap_newpass('b');

 BC=cell_f();
 while (1)
 {
  BC=BC->nextl;
  if (!BC) break;
  if (!BC->next) break;  	       // dummy cell at end reached
  defl=0; // dust is NOT defis;
  if (BC->h<<1 < BC->w) continue;      // too extended to be glued
  if( BC->accent_leader )
    continue; // Oleg : 06/10/97 17:51 : can't glued fictive images
  if (!tsimple(BC)) continue; // not simple ( "glued" earlier)
  c1l = c1 = BC->vers[0].let;
  p1l = p1 = BC->vers[0].prob;
    if ((p1 > 140) &&
      ( memchr("ij?!",c1,4) || c1==invers_exm || c1==invers_qm ||
       (accent_tab[c1] /*&& language != LANG_RUSSIAN*/))
     )   // russian don't have accents
     continue;

  if (decidust(BC))
    p1=discrid(BC,MONdust);
  if (p1 > TRSG1 && !(language == LANG_RUSSIAN && memchr("Јѓч",c1,3)))
   {   // allow glue with good 'Ё' 'Ј' 'u'
   BC->flg &= ~c_f_bad;
   BC->flg |= c_f_let;
   continue;
   }
  if (db_status)
    snap_newcell(BC);
  nsc=crepat(BC,&GL,3,0);              // close and far cells to list
  if (nsc < 2) continue;
  glsnap('b',BC,"B-->D");
  memcpy(&bc,BC,sizeof(bc));
  bdiff=BC->bdiff;
  dflag = BC->difflg & 0xf0;
  LC=WS=WC=BC->prevl;
  mn=glue(GL.complist, glue_pass);
  inc = erection_compose_inc(GL.ncell, GL.celist);
  if (mn == NULL) { glsnap('b',BC,"extrcomp err"); goto bcforw; }
  for (n=0; n < nsc; n++)
  {
    DC=GL.celist[n];
    if (DC->flg & c_f_dust)
      if (is_defis(DC))
        defl=1;
    GL.kitlist[n]=del_save_cell(DC);
  }
  WC=NULL;                             // new cell containing old pattern
  ngc=0;
  while(/*1*/ngc<MAX_CELLS_IN_LIST)
  {
    GL.nclist[ngc++]=CC=create_cell(mn,LC,(char)bdiff,dflag);
    inter_diff(CC);
    if (!CC->env)
      { glsnap('b',WS,txg1);  goto  delrest; }  // NOT to recog garbage
    if (if_dust(CC) & 7)              //  posed OR sized as dust
    {
      if (CC->flg & (c_f_let|c_f_bad))  // brought as letter - redo list
      {  CC->flg=c_f_dust;
         CC->prevl->nextl=CC->nextl;
         CC->nextl->prevl=CC->prevl;
         err_pnlet(CC);  // AL 940318 make err links
      }
    }
    else  // posed and sized as letter
      if (CC->flg & c_f_dust)   // brought as dust
      { cell *cc1;
        (CC->next)->prev=CC->prev; (cc1=CC->prev)->next=CC->next; // DELETE
        set_bad_cell(CC);
        CC->flg = c_f_bad;
        insert_cell(CC,cc1);
      }
    if (contain(CC,&bc)) WC=CC;
    if (!mn->mnnext)
      break;
    mn=mn->mnnext ;
  }
  if ((!WC) || (WC->flg & c_f_dust))
    goto not_contains;
  if (ngc == nsc)                      // same number of comps
  {
    glsnap ('b',WC,"not glued");
    GL.arg   = GAtigr+GAallest;
    GL.flarg = GFcut;
    cposd.flag=8;
    i=estcomp('b',WC,&svv1,TRSG2,0,0,&GL,&cposd,&cposd,0,0,0);
#ifdef UFA
    if( (i>=TRSG2-20) && (i > p1) && (GL.var & 15))
#else
    if ((i>=TRSG2) && (i > p1) && (GL.var & 15))
#endif
    {
      glsnap('b',WC,"box ok");
      if (GL.var & 15)
        collect_pat(WC,&GL,NULL);
      if (WC->nextl->cg_flag & c_cg_cutl) WC->cg_flag |= c_cg_cutr;
      if (WC->prevl->cg_flag & c_cg_cutr) WC->cg_flag |= c_cg_cutl;
      goto bcforw;
    }
    else goto rejct1;
  }
  // glued - check compound cell
  if (!(WC->flg & (c_f_let|c_f_bad)))  // glued pattern doesn't contain
     {  not_contains:  glsnap('b',WS,txg2);  goto  delrest; }
  set_erection(WC, inc);
  GL.arg   = GAtigr+GAallest;
  GL.flarg = GFcut;
  cposd.flag=8;
  i=0;
  if (WC && (WC->flg & (c_f_bad | c_f_let)))
    i=estcomp('b',WC,&svv1,TRSG2,0,0,&GL,&cposd,&cposd,0,0,0);
  if  (i>=TRSG2)
  { INT var_w;
    var_w = GL.var & 15;
    p2 = p1;
    cp=WC->vers[0].let;
    if (cp == c1l)    // same letter got -- must be better
      p2 = p1l;       // than non_discriminated by dust
    if (defl)         // dust was defis  -- must be seriously better
      { p2=p1+16; if (p2 > 254) p2=254; }
    if ((i > p2) ||
        ((i==p2) && (var_w > 0) && (var_w < 8))   // boxed - can be equal
       )
    {
      glok:
      glsnap('b',WC,"glued");
      cp=WC->vers[0].let;
      if ((cp==liga_uperc) || (cp==liga_lperc))
	 percgot =1;
      if (GL.var)
      {
        if (GL.var == 8)
          finpat(WC, &GL, 8, 0, 240);
        else
          collect_pat(WC,&GL,NULL);
      }
      if (WC->nextl->cg_flag & c_cg_cutl) WC->cg_flag |= c_cg_cutr;
      if (WC->prevl->cg_flag & c_cg_cutr) WC->cg_flag |= c_cg_cutl;
      goto bcforw;   // AL 940318
    }
    if (cp != c1l)       // different letter, dust was not a defis,
      if ((defl==0) && (i > (p1-TRSM1)))   /* not too worse  */
    {
      c2 = WC->vers[0].let;
      if (c1==c2) goto glok;
    }
  }

rejct1:
  if (db_status)
    { sprintf(ww,txg3,i,p1); glsnap('b',WC,ww); }
delrest:;
  for (n=0; n < ngc; n++)
    del_cell(GL.nclist[n]);
  for (n=0; n < nsc; n++)
    rest_cell(GL.kitlist[n],WS);
bcforw:
  BC=WS->nextl;
  }
 if (snap_activity('b'))
  {
  snap_show_text("B+D end; L+B start");
  snap_monitor();
  }
 }


static char txt11[]="B-->B";
static char txt12[]="B-->L";
static char txt21[]="B<--B";
static char txt22[]="L<--B";

static lll=0;
INT glue_to_o (BYTE c2, BYTE c3, cell *BC, cell *EC)
{
  if (
       ( ( c2=='(' ) || ( c2=='<')  )  &&
       ( ( c3==')' ) || ( c3=='>')  )
     )
     return 1;
  if (memchr("cCб‘",c2,4) &&
      !is_russian_baltic_conflict(c2)&&	// 17.07.2001 E.P.
	  !is_russian_turkish_conflict(c2)	// 21.05.2002 E.P.
	 )
    if ((c3==')') || (c3=='>'))
      if ((BC->col+BC->w+3) > EC->col)
        if ( (BC->h+2) >= EC->h )
          return 1;
  return 0;
}

BYTE broken_ii; // broken 'Ї','­','Ё'

static void glue_let_bad()
 {
 INT   i, j, dist, ndist, wdg, bdiff, trsuccess;
 char dflag, gtofl;
 uint16_t  p1, p2, pt;
 BYTE  *tx1, *tx2, **rp1, *rp2, *rps, c2, c3, cw, flb;
 BYTE  rus_iee;
 cell  *B1, *B2, *BP, *BD, *BC, *EC, *WS;
 void  *k1, *k2, *k3;
 c_comp *S[8];
 char sgfl,fgd, flgBOX, ww[40];
 MN  *mn;
 cell w;
 SVERS svv1, sav1, sav2;
 s_glue GL={0};
 INT inc=0;     cell *C[8];

 lll++;
 flb=0;               // 1st pass:  B+B  only
passfb:
 BC=cell_f();
 while (1)
  {
  BC=BC->nextl;
  if (!BC) break;
  EC=BC->nextl;
  BP=BC->prevl;
  if (!EC) break;                      // BC = dummy cell at end
  if (!EC->next) break;                // EC = dummy cell at end
  if( BC->accent_leader )
    continue; // Oleg : 06/10/97 17:51 : can't glued fictive images
  fgd=0;  		               // no dust inside
  rus_iee = 0;                          // flag to promote 'л'
  broken_ii = 0;                        // two sticks flag
   if (!tsimple(BC)) continue;          // not simple ( "glued" earlier)
   if (!tsimple(EC)) continue;          // not simple ( "glued" earlier)
   if(BC->cg_flag & c_cg_noglue ||
      EC->cg_flag & c_cg_noglue ) continue; // Valdemar 12-09-95 06:47pm
   gtofl = 0;
  if (BC->nvers && EC->nvers)  // recovering after stick cut
  {

    c2=BC->vers[0].let;
    c3=EC->vers[0].let;
    p1=p2=pt=0;   // allow any glue estimate
    gtofl = 0;
    if (glue_to_o(c2,c3,BC,EC))
    {
      glsnap ('b',BC,"(+) --> bad");
      gtofl = 1;
      goto asif2bad;
    }
 no_gtofl:
    gtofl = 0;
    BC=BP->nextl;
    EC=BC->nextl;
    if( !EC ) break; //. Oleg : 23mar00
    if(language==LANG_RUSSIAN)
    {/* Valdemar patch some special cases */
    if( ((c2=='>' && c3=='>') || (c2=='<' && c3=='<')) &&
       BC->vers[0].prob > 200 && EC->vers[0].prob > 200 )
       continue; // stay till punctuation
     if(memchr("мњљ",c2,3) &&
        !is_russian_baltic_conflict(c2) &&	// 17.07.2001 E.P.
		!is_russian_turkish_conflict(c2)&&	// 21.05.2002 E.P.
		 (memchr("/1!()ш",c3,6) || c3==liga_exm) && // 10.09.2000 E.P.
        flb == 2 && abs(BC->h - EC->h) < 4 &&
             abs(BC->row - EC->row) < 4
	) /*to paste л */
     { rus_iee = is_lower(c2) ? 1 : 2;   goto asif2bad; }

       if(memchr("мњљ",c2,3) &&
          !is_russian_baltic_conflict(c2) &&	// 17.07.2001 E.P.
		  !is_russian_turkish_conflict(c2)&&	// 21.05.2002 E.P.
		   c3=='|' && BC->vers[0].prob < 150
		 )
         goto asif2bad; /* bad 'м' try to paste л */
    }
    if (memchr("lI1/J)!",c2,7) ||         // stick character ?
		c2==liga_i ||
		   language==LANG_TURKISH &&  // 30.05.2002 E.P.
			(c2==i_sans_accent||c2==II_dot_accent)
			||
		c2==liga_exm
		)
      if (BC->cg_flag & c_cg_cutr)        // cut at right side
        if ((c3=='c') || (c3=='(') || (c3=='<') || (c3==')'))
          { glsnap ('b',BC,"I+<-->k,b");
            goto asif2bad;
          }
    if (memchr("lI1/J)!",c3,7) ||         // stick character ?
		c3==liga_i ||
		   language==LANG_TURKISH &&  // 30.05.2002 E.P.
			(c3==i_sans_accent||c3==II_dot_accent)
			||
		c3==liga_exm
		)
      if (EC->cg_flag & c_cg_cutl)        // cut at left  side
        if ((c2=='c') || (c2=='(') || (c2=='<') || (c2==(BYTE)'ѓ'))
          { glsnap ('b',BC,"<+I-->d");
            goto asif2bad;
          }
  }
  if (flb == 2)
	  continue; // 3nd pass - glue 'л'
  if (flb)
	  goto backg;   // 2nd pass - good to bad
  if (!(BC->flg & c_f_bad) || (!(EC->flg & c_f_bad)))
	  continue;

  glsnap('b',BC,"B+B");
  p1 = BC->vers[0].prob;
  p2 = EC->vers[0].prob;
  pt= (p2 > p1)? p2:p1;

asif2bad:
  tx1=txg6;
  ltoi=NULL;
  c2=1;      // dummy character, not to coincide with any pattern
  trsuccess=70;
  B1=BC; B2=EC;
  BD=BC->next;
   fgd = 0;  // AL 940311
  if (BD!=EC) fgd=1;                   // dust inside
  bdiff=BC->bdiff; dflag = BC->difflg & 0xf0;
  goto trygl;

				       // bad-to-err
backg:
  trsuccess=140;
  BD=BC->next;
   fgd = 0;  // AL 940311
    if (BD!=EC) fgd=1;                   // dust inside
	if (!fgd) // Is it dust between comps
		{
		if(BD->flg & c_f_fict)
			continue; // Oleg 26-07-99
		}
  // if ((BC->flg & c_f_bad) || (BC->cg_flag & c_cg_rqdot))
  if (BC->flg & c_f_bad )   // 93.08.19 no more RQDOT
  {
    B1=BC; B2=EC; tx1=txt11; tx2=txt12;
    rp1=results_right_to_bad;
    rp2=letters_right_to_bad;
    rps=sticks_right_to_bad;
    rps[0] = liga_i;
    goto trgl2;
  }
  // if ((EC->flg & c_f_bad) || (EC->cg_flag & c_cg_rqdot))
  if (EC->flg & c_f_bad )   // 93.08.19 no more RQDOT
  {
    B1=EC; B2=BC; tx1=txt21; tx2=txt22;
    rp1=results_left_to_bad;
    rp2=letters_left_to_bad;
    rps=sticks_left_to_bad;
    rps[0] = liga_i;
	rps[1] = liga_exm; // 10.09.2000 E.P.
    goto trgl2;
  }
  continue;                            // both good and not to be glued
trgl2:
  p1 = B1->vers[0].prob;
  p2 = B2->vers[0].prob;
  c2 = B2->vers[0].let;
  if (db_status)
    snap_newcell(B1);
  ltoi=NULL;
  /**** 93.08.19 no more RQDOT
  if (B2->cg_flag & c_cg_rqdot)
   {
     glsnap('b',BC,tx1);
     pt = p2 = 190; goto trygl;
   }
  ***********  RQDOT *********/
  if (p2 < 140)
   { glsnap('b',BC,tx1); pt=p2; }
  else
  {
    ltoi=strchr(rps,c2);   // is letter a proper stick?
    if (ltoi)
		{ltoi=rp1[0]; goto pairgot;}

    ltoi=strchr(rp2,c2);  // is letter in the list ?
    if (!ltoi ||
        is_russian_baltic_conflict(c2)||// 17.07.2001 E.P.
		is_russian_turkish_conflict(c2)	// 21.05.2002 E.P.
	   )
    {
       glsnap('b',BC,"unregistered pair");
       continue;
    }
    lton=ltoi-(char*)rp2;
    ltoi=rp1[lton];
  pairgot:
    glsnap('b',BC,tx2);
    pt=((p2<<1)+p1)/3;
  }
				       // Check width of compound letter
trygl:
  i=BC->r_col+BC->w;
  j=EC->r_col+EC->w;
  wdg = j - BC->r_col;
  dist=EC->r_col-i; ndist = 3;
  if (fgd) // Is it dust between comps
  {
    if(BD->flg & c_f_space) continue; // Valdemar 12-09-95 07:08pm
    dist -= BD->w;
    ndist = 6;
  }
  /* Valdemar 12.11.93
   There is none-serific л that lay so far */
  if(language == LANG_RUSSIAN ){
   if(BC->stick_inc == NO_INCLINE)   // inc don't counted
       stick_center_study(BC,NULL,1);
    if(EC->stick_inc == NO_INCLINE)
       stick_center_study(EC,NULL,1);
   if(memchr("мњљ",BC->vers[0].let,3) &&
      !is_russian_baltic_conflict(BC->vers[0].let) &&	// 17.07.2001 E.P.
	  !is_russian_turkish_conflict(BC->vers[0].let)		// 21.05.2002 E.P.
	 )
	   ndist += BC->h/10 + 1;
   else if(BC->stick_inc && EC->stick_inc )
   {  ndist += BC->h/6; broken_ii=1; ltoi=NULL;}
  }      // end if lang RUSSIAN
  if (dist > ndist)
  {  glsnap('b',B1,"too far");  continue; }
  if (wdg > 2*B1->h)
  { glsnap('b',B1,"too wide"); continue;  }

  glsnap('b',B2,"try glue");
				       // Glue only 2 components
  WS=BC->prevl;
  S[0]=B1->env;
  S[1]=B2->env;
  S[2]=NULL;
  C[0]=BC;
  C[1]=EC;
  inc = erection_compose_inc(2, C);
  if (fgd)
   { S[2]=BD->env; S[3]=NULL; }
				       // Try to glue and if success
				       //     try to recognize
  mn=glue(S,2);        // allow distant glue at top and bottom
  if (mn == NULL) { glsnap('b',BC,"extrcomp err"); continue; }

  if (mn->mnnext || broken_ii)   // glued
   {
   flgBOX=1;                            // try to recognize by BOX
				       //     compound component
   memcpy(&w,BC,sizeof(w));
   // perform "crepat" actions: collect cells and envelopes
   if( fgd ){
   GL.celist[0]=B1;
   GL.celist[1]=B2;
   GL.celist[2]=BD;
   GL.celist[3]=NULL;
   GL.maxlist[0]=B1;
   GL.maxlist[1]=B2;
   GL.maxlist[2]=BD;
   GL.maxlist[3]=NULL;
   GL.maxnc = 3;
   GL.ncell=3;
   GL.var = 0; GL.fres = 0;
   GL.arg=0;
   //GL.flarg=GFfinal+GFcut;
   GL.flarg=GFcut;
   GL.complist[0]=B1->env;
   GL.complist[1]=B2->env;
   GL.complist[2]=BD->env;
   GL.complist[3]=NULL;
   inc = erection_compose_inc(3, GL.celist);
   }
   else { // no dust inside
   GL.celist[0]=B1;
   GL.celist[1]=B2;
   GL.celist[2]=NULL;
   GL.maxlist[0]=B1;
   GL.maxlist[1]=B2;
   GL.maxlist[2]=NULL;
   GL.maxnc = 2;
   GL.ncell=2;
   GL.var = 0; GL.fres = 0;
   GL.arg=0;
   //GL.flarg=GFfinal+GFcut;
   GL.flarg=GFcut;
   GL.complist[0]=B1->env;
   GL.complist[1]=B2->env;
   GL.complist[2]=NULL;
   inc = erection_compose_inc(2, GL.celist);
   }
   i=0;
   if ( crecell(B1,&GL,3) < 0)  // make_broken_raster rejects cells
     goto fingb;
   SBOX_to_static();
   SBOX_to_GL(&GL);
   sgfl = BC->cg_flag;
   BC->cg_flag |= c_cg_noenv;  // no envelope provided
   set_bad_cell(BC);
   dmBOX(BC,&GL);
   BC->cg_flag = sgfl;
   i= BC->vers[0].prob;
   }
  else
   {
   flgBOX=0;
   bdiff=B1->bdiff; dflag = BC->difflg;
   save_vers(B1,&sav1);
   save_vers(B2,&sav2);
   k1=del_save_cell(B1);
   k2=del_save_cell(B2);
   if (fgd) k3=del_save_cell(BD);
   BC=create_cell(mn,WS,(char)bdiff, dflag);
   inter_diff(BC);
   if (!BC->env)
    {
    glsnap('b',WS,txg1);
    goto  restc;                       // NOT to recog dust
    }
   set_erection(BC, inc);
   GL.arg = GAtigr + GAown;
   //GL.flarg = GFfinal + GFcut;
   GL.flarg = GFcut;
   cposd.flag=8;
   i=0;
   if (BC && (BC->flg & (c_f_bad | c_f_let)))
     i=estcomp('b',BC,&svv1,140,0,0,&GL,&cposd,&cposd,0,0,0);
   }
fingb:
  /* cursiv 'л' confuse with '¬' let's promote him */
  if( language == LANG_RUSSIAN && rus_iee)
    if( rus_iee == 1 )
      promote(1,BC,'л',60);
   else if(rus_iee == 2)
	      promote(1,BC,'›',60);

  if (gtofl)
  { BYTE vers_c;
    vers_c = BC->vers[0].let;
    if (! ( memchr("oO0®Ћ",vers_c,5) &&
			!is_russian_turkish_conflict(BC->vers[0].let)	// 21.05.2002 E.P.
		)
	   )
    {
      if (db_status)
	glsnap('b',BC,"oO0 expected");
      goto restc;
    }
  }
  if ((i<pt) || (i<trsuccess))               // 50 - bad symbol penalty
  {
    if (db_status)
      { sprintf(ww,txg3,i,pt); glsnap('b',BC,ww); }
    goto restc;
  }
  if (!ltoi)                           // not a  restricted result
    goto accel;
  cw = BC->vers[0].let;

  if (strchr(ltoi,cw) &&
      !is_russian_baltic_conflict(cw) &&	// 17.07.2001 E.P.
	  !is_russian_turkish_conflict(cw)&&	// 21.05.2002 E.P.
	  (
		// Сомнительные приклейки i_sans_accent 27.06.2002 E.P.
		!is_bad_turkish_glue(c2,c3,cw) ||
			( i > 220 && (i+i) > (p1+p2) )
	  )
	 )
    goto accel;
  if (i > p2 ||
      (c2=='|' && i > p1)
     )                          // more than biggest
    goto accel;
  if ((cw=='m') && (c2=='r'))
  {
    if ((i > 220) && ((i+i) > (p1+p2)))
      goto accel;
  }
  if( language==LANG_RUSSIAN && BC->vers[0].prob>230 &&
      c2==(BYTE)'Ї' && c3==(BYTE)'ш' && cw==(BYTE)'х' )
      goto accel; // Oleg : 30-03-1995 : near cursive rus n+cursive rus ge
  if ( ((BC->recsource & c_rs_ev) == 0) ||
       ( i < 220) ||
       ( MAX(p1,p2) > 170 ) ||
       ( (i+i) <= (p1+p2))
     )
     goto unrel_rs;
unrel_rs:
  glsnap('b',BC,"unreliable or unknown result");

restc:                                 // restore pre-glue cells
  if (flgBOX)
  {
   memcpy(BC,&w,sizeof(w));
   glsnap('b',BC,"err BOX, restored");
  }
  else
  {
   glsnap('b',BC,"to restore cells");
   del_cell(BC);
     // AL 940311 change order of crete_cell's: k1 [k3] k2
     BC=rest_cell(k1,WS);
     i=0;
   if (BC && (BC->flg & (c_f_bad | c_f_let)))
   {
     i=estcomp('b',BC,&svv1,140,0,0,&GL,&cposd,&cposd,0,0,0);
        if ((BC->flg & c_f_bad) && sav1.nvers)
        rest_vers(BC,&sav1);
     }
     if (fgd) rest_cell(k3,WS);
     BC=rest_cell(k2,WS);
   i=0;
   if (BC && (BC->flg & (c_f_bad | c_f_let)))
   {
     i=estcomp('b',BC,&svv1,140,0,0,&GL,&cposd,&cposd,0,0,0);
     if ((BC->flg & c_f_bad) && sav2.nvers)
      rest_vers(BC,&sav2);
   }
   BC = BP->nextl;
   EC = BC->nextl;
   glsnap('b',BC,"cells restored");
  }
  if (gtofl)
    goto no_gtofl;  // (+)--> o failed; make registered
  continue;
accel:                                 // accept new cells
  if (BC->nextl->cg_flag & c_cg_cutl) BC->cg_flag |= c_cg_cutr;
  if (BC->prevl->cg_flag & c_cg_cutr) BC->cg_flag |= c_cg_cutl;
  if (flgBOX)
   { glsnap('b',BC,txg5);
     collect_pat(BC,&GL,NULL);
   }
  else
  {
     if (GL.var == 8)
        finpat(BC, &GL, 8, 0, 32);
     glsnap('b',BC,txg4);
  }
  continue;
  }
  // russian have a special pass to paste 'л'
 if((flb && language!=LANG_RUSSIAN) ||
    (flb==2 && language==LANG_RUSSIAN) ) return;
 flb++;                                // glue backward two bad cells
 goto passfb;
 }

static void glue_dust_dust()
 {
 INT  i,j, bdiff;
 char dflag;
 cell *BC, *EC, *WS;
 void *k1, *k2;
 c_comp *S[8];
 cell   *C[8]; INT inc=0;
 char  flgBOX;
 MN  *mn;
 cell w;
 SVERS svv1, sav1, sav2;
 s_glue GL;

 BC=cell_f();
 while (1)
  {
  BC=BC->next;
  if (!BC) break;
  EC=BC->next;
  if (!EC) break;                      // BC = dummy cell at end
  if (!EC->next) break;                // EC = dummy cell at end
  if (!(BC->flg & c_f_dust)) continue;
  if (!(EC->flg & c_f_dust)) continue;
  if( BC->accent_leader )
    continue; // Oleg : 06/10/97 17:51 : can't glued fictive images
  i=MAX(EC->r_col+EC->w,BC->r_col+BC->w)-BC->r_col;
  j=MAX(EC->r_row+EC->h,BC->r_row+BC->h);
  j-=MIN(EC->r_row,BC->r_row);
  if (i < 10) continue;
  if (j < 10) continue;
  if (db_status)
    snap_newcell(BC);
  glsnap('b',BC,"2 dusts");
				       // Check width of compound letter
  i=EC->r_col+EC->w;
  if (i < (j=BC->r_col+BC->w)) i=j;
  i-=BC->r_col;
  if (i > (BC->h <<1)) continue;
  glsnap('b',BC,"glue dusts");
				       // Glue only 2 components
  WS=BC;
  while (1)
   {
   WS=WS->prev;
   if (WS->flg & (c_f_let|c_f_bad)) break;
   if (WS->flg & c_f_fict) break;
   }

  S[0]=BC->env;
  S[1]=EC->env;
  S[2]=NULL;
  C[0]=BC;
  C[1]=EC;
  inc = erection_compose_inc(2, C);
				       // Try to glue and if success
  mn=glue(S,0);        //   try to recognize
  if (mn == NULL) { glsnap('b',BC,"extrcomp err"); continue; }
  if (mn->mnnext)                      // not glued
   {
   flgBOX=1;
                       // try to recognize by BOX compound
   memcpy(&w,BC,sizeof(w));            //     component
				       // Count extended envelope for 2 dusts
  // dusts MUST be GLUED, not COLLECTED  (changing cell type "dust" to "bad")
   goto restc;
   }
  else
   {
   flgBOX=0;
   bdiff=BC->bdiff; dflag = BC->difflg & 0xf0;
   save_vers(BC,&sav1);
   save_vers(EC,&sav2);
   k1=del_save_cell(BC);
   k2=del_save_cell(EC);
   BC=create_cell(mn,WS,(char)bdiff,dflag);
   inter_diff(BC);
   if ((!BC->env) || (BC->flg & c_f_dust))
    {
    glsnap('b',WS,"NOT a letter cell");
    goto  restc;
    }
   set_erection( BC, inc );
   GL.arg = GAtigr + GAown;
   // GL.flarg = GFfinal + GFcut;
   GL.flarg = GFcut;
   cposd.flag=8;
   i=0;
   if (BC && (BC->flg & (c_f_bad | c_f_let)))
     i=estcomp('b',BC,&svv1,140,0,0,&GL,&cposd,&cposd,0,0,0);
   }
// fingb:
  if (i<140)                           // 50 - bad symbol penalty
   {
restc:
   if (flgBOX)
    {
    memcpy(BC,&w,sizeof(w));
    glsnap('b',BC,"err BOX, restored");
    }
   else
    {
    glsnap('b',BC,"err; cell restored");
    del_cell(BC);
    BC=rest_cell(k2,WS);
    i=0;
    if (BC && (BC->flg & (c_f_bad | c_f_let)))
    {
      i=estcomp('b',BC,&svv1,140,0,0,&GL,&cposd,&cposd,0,0,0);
      if ((BC->flg & c_f_bad) && sav2.nvers)
        rest_vers(BC,&sav2);
    }
    BC=rest_cell(k1,WS);
    i=0;
    if (BC && (BC->flg & (c_f_bad | c_f_let)))
    {
      i=estcomp('b',BC,&svv1,140,0,0,&GL,&cposd,&cposd,0,0,0);
      if ((BC->flg & c_f_bad) && sav1.nvers)
        rest_vers(BC,&sav1);
    }
   }
   continue;
   }
  if (flgBOX)
   {
   glsnap('b',BC,txg5);
   if (GL.var & 15)
      collect_pat(BC,&GL,NULL);
   }
  else
   glsnap('b',BC,txg4);
  continue;
  }
 }


static INT contain(cell *new, cell *old)
//
//	This procedure check whether new cell contains old one.
//
 {

 if (new->r_col > old->r_col)
  return 0;
 if (new->r_col+new->w < old->r_col+old->w)
  return 0;
 if (new->r_row > old->r_row)
  return 0;
 if (new->r_row+new->h < old->r_row+old->h)
  return 0;
 return 1;
 }

void glsnap(char I, cell *C, char *txt)
 {

 if (!db_status) return;
 snap_newcell(C);
 if (snap_activity(I))
  {
  snap_show_text(txt);
  snap_monitor();
  }
 }
void dummy_snap_place()
{ return; }


#ifdef UFA
extern BYTE pitch;
static void glue_all_dusts()
 {
s_glue GLG;
B_LINES bl;
INT f_u,f_d,f_r,f_l;
cell *BC,*EC,*C;
INT ngl=0,row,col,w,h,ngreat;
INT rrow,rcol;

      if( db_status && snap_activity('b') )
       {
       snap_show_text("UFA : begin many dusts");
       snap_monitor();
       }

BC=cell_f();

while (1)
 {
 EC = BC;
 while( (EC=EC->next)->next && !(EC->flg&(c_f_let|c_f_fict))
        && !((EC->flg&c_f_bad)&&EC->nvers>0) );
  if( (BC->flg&c_f_fict) && (EC->flg&c_f_fict) ) break;
  if( BC->next!=EC )
    {
    get_b_lines(BC,&bl);
    f_u = bl.b1;
    f_d = bl.b3;
    f_l = MAX(BC->next->col,(BC->flg&c_f_fict)?0:BC->col+BC->w+2);
    f_r = MIN(EC->prev->col+EC->prev->w,((EC->flg&c_f_fict)?10000:EC->col)-2);
    if(f_r-f_l<pitch )
    {
    row = f_d; col = f_r;
    h   = f_u; w   = f_l;
    memset(&GLG,0,sizeof(GLG));
    for(ngreat=1,ngl=0,C=BC;C!=EC->next;C=C->next)
      {
      if( !(C->flg&(c_f_dust|c_f_bad)) )  continue;
      if( C->row+C->h<f_u || C->row>f_d || C->col+C->w<f_l || C->col>f_r )
        continue;
      if( C->row<row )     {row = C->row;     rrow = C->r_row;}
      if( C->row+C->h>h )   h   = C->row+C->h;
      if( C->col<col )     {col = C->col;     rcol = C->r_col;}
      if( C->col+C->w>w )   w   = C->col+C->w;
      GLG.celist[ngl]=C;
      GLG.maxlist[ngl]=C;
      GLG.complist[ngl]=C->env;
      if( C->env->scale>ngreat ) ngreat=C->env->scale;
      ngl++;
      if( ngl==MAX_CELLS_IN_LIST-1 )  break;
      }
    h -= row;
    w -= col;
    ngreat = 1<<ngreat;
    if( ngl>1 && w*ngreat>pitch/2 && h>((bl.b3-bl.b2)*3)/4 )
       {
       GLG.row   = row; GLG.col  = col;
       GLG.width = w; GLG.height = h;
       GLG.ncell = ngl;
       GLG.maxnc = ngl;
       GLG.complist[ngl]=0;
       GLG.celist[ngl]  =0;
       GLG.maxlist[ngl] =0;

       GLG.arg = GABOXR;
       GLG.flarg = GFcut;
       cposd.flag=8;

       C = new_cell();
       if ( crecell(C,&GLG,3) >= 0)  /* make_broken_raster rejects cells */
         {
         C->col = col;
         C->row = row;
         C->r_col = rcol;
         C->r_row = rrow;
         C->w   = w;
         C->h   = h;
         C->flg = c_f_bad;

         insert_cell(C,BC);
         set_bad_cell(C);
         compose_cell_save(2,GLG.celist,C);
         dmBOX(C,&GLG);
         if( C->vers[0].prob>TRSG2-20 )
          C->flg = c_f_let;
         else
          C->flg = c_f_bad;

        C->cg_flag = 8|c_cg_comp;  /* no envelope provided */
        compose_cell(ngl,GLG.celist,C);
        EC=C;
        } /* end of crecell */
       else
        free_cell(C);
       } /* end of ngl>1 */
    } /* end of many dusts */
    } /* end of normal width */
  if( EC->next==NULL )  break;  /* EC is last letter */
  BC = EC;
  } /* end of loop */

if( db_status && snap_activity('b') )
 {
 snap_show_text("UFA : end many dusts");
 snap_monitor();
 }

return;
} /* end glue_all_dusts */

#endif

Bool config_III(cell *BC,cell *EC, cell *ECN)
{
return(  (EC->flg&c_f_bad) &&
       (ECN->flg&c_f_let) &&
       BC->broken_II && (BC->flg&c_f_let) && BC->nvers>0 &&
       (BC->vers[0].let==(BYTE)'Ї' ||
       BC->vers[1].let==(BYTE)'Ї'&& BC->vers[1].prob==BC->vers[0].prob ||
       BC->vers[2].let==(BYTE)'Ї'&& BC->vers[2].prob==BC->vers[0].prob) );

}

Bool config_CapRusGe_and_bad(cell *BC,cell *EC)
{
cell *BP,*BPP;
BP  = ( BC!=cell_f())?BC->prev:NULL;
if( !BP )	return FALSE;
BPP = ( BP!=cell_f())?BP->prev:NULL;
if( !BPP )	return FALSE;

return ( !(BP->flg&c_f_fict) && !(BC->flg&c_f_fict) && !(EC->flg&c_f_fict) &&
         (BP->nvers>0 && memchr("л®г ҐЁн",BP->vers[0].let,7) &&
			!is_russian_baltic_conflict(BP->vers[0].let)&&	// 17.07.2001 E.P.
			!is_russian_turkish_conflict(BP->vers[0].let)	// 21.05.2002 E.P.
		 ||
         BP->nvers>0 && BP->vers[0].let=='|' &&
           BPP!=NULL && BPP->nvers>0 && BPP->vers[0].let==(BYTE)'м' &&
			!is_russian_baltic_conflict(BPP->vers[0].let) &&// 17.07.2001 E.P.
			!is_russian_turkish_conflict(BPP->vers[0].let)	// 21.05.2002 E.P.
		 )&&
         BC->nvers>0 && BC->vers[0].let==(BYTE)'ѓ' && BC->vers[0].prob<220 &&
         EC->nvers<1 );
}

Bool config_brace_and_K(cell *BC,cell *EC)
{
return( BC->nvers>0 && BC->vers[0].let==(BYTE)'>' &&
        EC->nvers>0 && EC->vers[0].let==(BYTE)'Є' &&
        EC->col - BC->col - BC->w<3 );
}
s_glue GLG;
cell *C;

void glue_III()
 {
cell *BC,*EC;
INT ngl=0,row,col,w,h, inc=0;
INT rrow,rcol;
BYTE res[30];

      if( db_status && snap_activity('b') )
       {
       snap_show_text("Glue : russian III");
       snap_monitor();
       }

BC=cell_f();

while (1)
 {
 EC = BC->next;
 res[0]='\0';
 if( config_III(BC,EC,EC->next) || config_III(EC,BC,BC->prev) )
  strcat(res,"и");

 if( config_CapRusGe_and_bad(BC,EC) )
	{
	 if (is_turkish_language(language)) // 21.05.2002 E.P.
		strcat(res,"‰");
	 else
		strcat(res,"©‰");
	}

 if( config_brace_and_K(BC,EC) )
  strcat(res,"¦†");
 if( res[0] )
    {
    memset(&GLG,0,sizeof(GLG));
    row  = BC->row;   col=BC->col;
    rrow = BC->r_row; rcol = BC->r_col;
    h   = BC->row+BC->h; w=BC->col+BC->w;
    for(ngl=0,C=BC;C!=EC->next;C=C->next)
      {
      if( C->row<row )     {row = C->row;     rrow = C->r_row;}
      if( C->row+C->h>h )   h   = C->row+C->h;
      if( C->col<col )     {col = C->col;     rcol = C->r_col;}
      if( C->col+C->w>w )   w   = C->col+C->w;
      GLG.celist[ngl]=C;
      GLG.maxlist[ngl]=C;
      GLG.complist[ngl]=C->env;
      ngl++;
      if( ngl==MAX_CELLS_IN_LIST-1 )  break;
      }
    h -= row;
    w -= col;

    GLG.row   = row; GLG.col  = col;
    GLG.width = w; GLG.height = h;
    GLG.ncell = (BYTE)ngl;
    GLG.maxnc = (BYTE)ngl;
    GLG.complist[ngl]=0;
    GLG.celist[ngl]  =0;
    GLG.maxlist[ngl] =0;
    GLG.arg = GABOXR;
    GLG.flarg = GFcut;
    cposd.flag=8;

    C = new_cell();
    if ( crecell(C,&GLG,3) >= 0)  /* make_broken_raster rejects cells */
      {
      C->col = col;
      C->row = row;
      C->r_col = rcol;
      C->r_row = rrow;
      C->w   = w;
      C->h   = h;
      C->flg = c_f_bad;

      insert_cell(C,BC);
      set_bad_cell(C);
      inc = erection_compose_inc(GLG.ncell, GLG.celist);
      set_erection(C,inc);
      compose_cell_save(2,GLG.celist,C);
      dmBOX(C,&GLG);
      if( C->vers[0].prob>TRSG2-20 && memchr(res,C->vers[0].let,strlen(res)) &&
		  !is_russian_baltic_conflict(C->vers[0].let) &&// 17.07.2001 E.P.
		  !is_russian_turkish_conflict(C->vers[0].let)	// 21.05.2002 E.P.
		)
        {
        C->flg = c_f_let;
        C->cg_flag = 8|c_cg_comp;  /* no envelope provided */
        compose_cell(2,GLG.celist,C);
        EC=C;
        } /* save good glue */
      else
        del_cell(C);
      } /* end of crecell */
      else
        free_cell(C);

      }

  if( EC->next==NULL )  break;  /* EC is last letter */
  BC = EC;
 } /* end of loop */

if( db_status && snap_activity('b') )
 {
 snap_show_text("Glue : russian III");
 snap_newcell(cell_l()->prevl);
 snap_monitor();
 }

return;
} /* end glue_III */

// 27.06.2002 E.P.
INT is_bad_turkish_glue(BYTE c1, BYTE c2, BYTE c3)
{
/*
	Определение неудачной турецкой склейки. 27.06.2002 E.P.

	с1 - левая половина
	с2 - правая половина
	с3 - результат склейки

*/
	if (!is_turkish_language(language))
		return FALSE;

	if (
		c2==i_sans_accent && c3=='h'
	   )
		return TRUE;

	return FALSE;
}
