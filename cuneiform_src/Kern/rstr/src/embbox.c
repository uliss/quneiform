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
  ЫЫ   All rights reserved. This program is proprietary and       ЫЫ
  ЫЫ     a trade secret of Cognitive Technology Corporation.      ЫЫ
  ЫЫ                                                              ЫЫ
  ЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫ
*/

/*********************************************************************/
/*                                                                   */
/*                           module EMBBOX                           */
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
#include "ligas.h"
#include "linutil.h"
#include "lang.h"
#include "loadtree.h"

#include "tuner.h"
#ifdef UFA
  BOOL test_alphabet_elem(BYTE let);
#endif

double koeK=1.;                            //AK 30.04.97 It's need?
long   allK=0,klaK=0;                      //AK -"-


struct res_recog RR;
extern CHAR *tableBOX;
extern PBYTE fontBOX/*,auto_pool*/;    /* BOX table for font  */
PBYTE  omniBOX;    /* BOX save table for omni  */

INT best_answer_BOX;
static void store_bests(INT);
static void store_all_bests(INT);
static indBOX *wiptr, *indp;
static BYTE curlet;

CHAR forbidBOX[]={"&#@!-."};
CHAR *db_m1[]={"3x3 tab","3x7 tbl", "5x11 t","7x13 march","16x16 cell"};

static PBYTE full_list[512];
static PBYTE font_full_list[512];
static PBYTE omni_full_list[512];

struct list_BOX
 {
  BYTE let;
  BYTE flag;
  WORD res;
 };
typedef struct list_BOX LST;
void store_new_bests(WORD ibcos, CHAR pl, CHAR ff,CHAR n_rsn);


INT  isKlasterFull( INT typl )
{
   if( ((LST *)(full_list[typl]))->let )     // yes such letter in table
      return  1;

   return  0;
}/*isWideLetter*/

static void store_many(WORD ibcos, CHAR let, CHAR ff, INT typl)
{
     if(language==LANG_RUSSIAN) return;
  switch (let)
   {
    case 'x': case 'c': case 'v':
    case 's': case 'w':
     if (typl & 1) store_new_bests(ibcos,let,ff,-1);               // small
     if (typl & 2) store_new_bests(ibcos,(CHAR)(let +'A' -'a'),ff,-1);     // capital
     break;

    case 'X': case 'C': case 'V':
    case 'S': case 'W':
     if (typl & 1) store_new_bests(ibcos,(CHAR)(let +'a' -'A'),ff,-1);     // small
     if (typl & 2) store_new_bests(ibcos,let,ff,-1);               // capital
     break;

    case 'l': case 'i':
     if (typl & 1) store_new_bests(ibcos,'i',ff,-1);       // small
     if (typl & 2) store_new_bests(ibcos,'l',ff,-1);       // capital
     break;

    case 'o': case 'O': case '0':
     if (typl & 1) store_new_bests(ibcos,'o',ff,-1);       // small
     if (typl & 2)
      {
        store_new_bests(ibcos,'O',ff,-1);  // capital
        store_new_bests(ibcos,'0',ff,-1);
      }
     break;

    case 'p': case 'P':
     if (typl & (4+8)) store_new_bests(ibcos,'p',ff,-1);   // small + low
     if (typl & 2) store_new_bests(ibcos,'P',ff,-1);       // capital
     break;

    case 'z':
	case 'Z':
     if (typl & (1+8)) store_new_bests(ibcos,'z',ff,-1);   // small + low
     if (typl & 2) store_new_bests(ibcos,'Z',ff,-1);       // capital
     break;

    case c_bottom_accent :
	case CC_bottom_accent :

		// Конфликтные коды 17.07.2001 E.P.
		if (is_baltic_language(language))
			break;

     if (typl & 4) store_new_bests(ibcos,(CHAR)c_bottom_accent,ff,-1);
     if (typl & (2+8)) store_new_bests(ibcos,(CHAR)CC_bottom_accent,ff,-1);
     break;
   }
}


static void store_new_bests(WORD ibcos, CHAR pl, CHAR ff,CHAR n_rsn)
//
//	This procedure stores best NBEST answers in rec_recog.
//
 {
 int i;			// do not change this
 if (ibcos <= RR.ibest_cosinus[NBBOX-1])  return;
 i=NBBOX;
  while(--i)
   {
   if (RR.ibest_cosinus[i-1] < ibcos)
    {
    RR.ibest_cosinus[i]=RR.ibest_cosinus[i-1];
    RR.best_lets[i]=RR.best_lets[i-1];
    RR.best_font[i]=RR.best_font[i-1];
    RR.n_rsn[i]    =RR.n_rsn[i-1];
    }
   else
    {
    RR.ibest_cosinus[i]=ibcos;
    RR.best_lets[i]=pl;
    RR.best_font[i]=ff;
    RR.n_rsn[i]    =n_rsn;
    return;
    }
   }
  RR.ibest_cosinus[0]=ibcos;
  RR.best_lets[0]=pl;
  RR.best_font[0]=ff;
  RR.n_rsn[0]    =n_rsn;
 }

static CHAR rbfo, rbfc, rbfv, rbfw, rbfx, rbfs, rbfp, rbfz, rbil;

static INT letagain(CHAR curleta,INT fl)
{
   INT fl1, fl2, fl4;
   fl1 = fl;
   fl2 = fl*2;
   fl4 = fl*4;
   wiptr=NULL;
   if(language==LANG_RUSSIAN) return 0;
   switch (curleta)
    {
     case 'c':
      rbfc |= fl1;
      goto cC;
     case 'C':
      rbfc |= fl2;
      cC:
      if (!(rbfc & 1)) { rbfc |= 1; curlet = 'c'; goto comrl; }
      if (!(rbfc & 2)) { rbfc |= 2; curlet = 'C'; goto comrl; }
      break;
     case 'i':
      rbil |= fl1;
      goto il;
     case 'l':
      rbil |= fl2;
      il:
      if (!(rbil & 1)) { rbil |= 1; curlet = 'i'; goto comrl; }
      if (!(rbil & 2)) { rbil |= 2; curlet = 'l'; goto comrl; }
      break;
     case 'o':
      rbfo |= fl1; goto oO0;
     case 'O':
      rbfo |= fl2; goto oO0;
     case '0':
      rbfo |= fl4;
      oO0:
      if (!(rbfo & 1)) { rbfo |= 1; curlet = 'o'; goto comrl; }
      if (!(rbfo & 2)) { rbfo |= 2; curlet = 'O'; goto comrl; }
      if (!(rbfo & 4)) { rbfo |= 4; curlet = '0'; goto comrl; }
     break;
     case 'p':
      rbfp |= fl1;
      goto pP;
     case 'P':
      rbfp |= fl2;
      pP:
      if (!(rbfp & 1)) { rbfp |= 1; curlet = 'p'; goto comrl; }
      if (!(rbfp & 2)) { rbfp |= 2; curlet = 'P'; goto comrl; }
      break;
     case 's':
      rbfs |= fl1;
      goto sS;
     case 'S':
      rbfs |= fl2;
      sS:
      if (!(rbfs & 1)) { rbfs |= 1; curlet = 's'; goto comrl; }
      if (!(rbfs & 2)) { rbfs |= 2; curlet = 'S'; goto comrl; }
      break;
     case 'v':
      rbfv |= fl1;
      goto vV;
     case 'V':
      rbfv |= fl2;
      vV:
      if (!(rbfv & 1)) { rbfv |= 1; curlet = 'v'; goto comrl; }
      if (!(rbfv & 2)) { rbfv |= 2; curlet = 'V'; goto comrl; }
      break;
     case 'w':
      rbfw |= fl1;
      goto wW;
     case 'W':
      rbfw |= fl2;
      wW:
      if (!(rbfw & 1)) { rbfw |= 1; curlet = 'w'; goto comrl; }
      if (!(rbfw & 2)) { rbfw |= 2; curlet = 'W'; goto comrl; }
      break;
     case 'x':
      rbfx |= fl1;
      goto xX;
     case 'X':
      rbfx |= fl2;
      xX:
      if (!(rbfx & 1)) { rbfx |= 1; curlet = 'x'; goto comrl; }
      if (!(rbfx & 2)) { rbfx |= 2; curlet = 'X'; goto comrl; }
      break;
     case 'z':
      rbfz |= fl1;
      goto zZ;
     case 'Z':
      rbfz |= fl2;
      zZ:
      if (!(rbfz & 1)) { rbfz |= 1; curlet = 'z'; goto comrl; }
      if (!(rbfz & 2)) { rbfz |= 2; curlet = 'Z'; goto comrl; }
      break;
     case c_bottom_accent:

		// Конфликтный код 17.07.2001 E.P.
		if (is_baltic_language(language))
			return 0;

      rbfc |= fl1;
      goto c_bottom;

     case CC_bottom_accent:
		// Конфликтный код 17.07.2001 E.P.
		if (is_baltic_language(language))
			return 0;

      rbfc |= fl2;
      c_bottom:
      if (!(rbfc & 1)) { rbfc |= 1; curlet = c_bottom_accent; goto comrl; }
      if (!(rbfc & 2)) { rbfc |= 2; curlet = CC_bottom_accent; goto comrl; }
      break;

     default: return 0;
      comrl:  wiptr=&indp[curlet];
    }
 return 1;
}

void embBOX(servBOX *SBOX, SVERS *tvers, BOOL erection)
//
//	This procedure checks r_raster against compressed images from
//  table BOX of letters given by tgrhyp.
//	ON ENTRY: tgrhyp - sequence of hypotheses that finishes with 0.
//        tableBOX
//	ON EXIT : ans - sequence of best choices that finishes with ltr=0.
//
{
INT i, hyp,  fll,cnt;
version *hypa;
WORD probest;
BYTE ocurlet;
WORD vect[16];
elmBOX  *matr,  *wmatr;
PBYTE klist;

t_answer *ans_ptr;
Z=&string;

memset(&RR,0,sizeof(RR));
indp = (indBOX *) tableBOX;
matr = (elmBOX *) (tableBOX+1024);
for(i=0;i<15;i++)
    vect[i] = (((LONG)SBOX->matrBOX.vect[i])<<15)/SBOX->matrBOX.isq_rt;

fll=0;                                // all hypotesa not in BOX tab
best_answer_BOX=0;

for (hyp=0, hypa=(version*)&tvers->vers, ans_ptr=SBOX->best_BOX;	//AK 04.03.97 ? for address
            hyp < tvers->nvers; hyp++, hypa++, ans_ptr++)
    {
    ocurlet=curlet=hypa->let;
    ans_ptr->ltr=ocurlet;
    ans_ptr->iprob=327;                        //  0.1
    wiptr=&indp[ocurlet];

    if ( (!wiptr->ltr) ||                  // no such letter in table
           (strchr(forbidBOX,ocurlet)) )     // forbidden in BOX
        continue;

    fll=1;                   // some letter in BOX table
    rbil=rbfo=rbfc=rbfv=rbfw=rbfx=rbfs=rbfp=rbfz=0;//TMP
relet:           // repeat for "similar" letter[s]
    wmatr= matr+wiptr->numel;
    klist = full_list[wiptr->ltr];
    klist += sizeof(LST);
    cnt=0;
    if( erection&erect_rot )
        {
        while( (wmatr = *(elmBOX **)klist) != NULL )
            {
            probest=(WORD)((*scalar)(wmatr->vect,vect)>>15);
            if (probest > (WORD) (ans_ptr->iprob))
               {
               if (probest > (WORD) best_answer_BOX) best_answer_BOX=probest;
               ans_ptr->fnt=wmatr->fnt;
               ans_ptr->iprob=probest;
               ans_ptr->n_rsn=(CHAR)cnt;
               }
            cnt++;
            klist += sizeof(PBYTE *);
            }// while end  (all versions of one letter)
        }
    else
        {
        while( (wmatr = *(elmBOX **)klist) != NULL )
            {
            if( !(wmatr->bnd&0x200) )
                {
                probest=(WORD)((*scalar)(wmatr->vect,vect)>>15);
                if (probest > (WORD) (ans_ptr->iprob))
                    {
                    if (probest > (WORD) best_answer_BOX) 
                        best_answer_BOX=probest;
                    ans_ptr->fnt=wmatr->fnt;
                    ans_ptr->iprob=probest;
                    ans_ptr->n_rsn=(CHAR)cnt;
                    }                
                }
            cnt++;
            klist += sizeof(PBYTE *);
            }// while end  (all versions of one letter)
        }  

    if (letagain(ocurlet,1))  // TRY to use "similar" letter, mark current as used
        if (wiptr)
            goto relet;
    }   // for all versions in cell
   
ans_ptr->ltr=0;                       // end of list
if (!fll)                             // all not in table - make .99 to all
    {
    for (i=0,ans_ptr=SBOX->best_BOX; (i < NBBOX) && ans_ptr->ltr; i++,ans_ptr++)
        ans_ptr->iprob=32767;      //  1.00
    best_answer_BOX = 32767; //  give life to all killed versions
    }

return;
}

BOOL pidx_skip(INT h, INT w,BYTE t_let);
void embBOXF(servBOX *SBOX, INT typl, BOOL erection)
//
//	This procedure checks r_raster against compressed images from
//  table BOX of letters given by tgrhyp.
//	ON ENTRY: tgrhyp - sequence of hypotheses that finishes with 0.
//        tableBOX
//	ON EXIT : ans - sequence of best choices that finishes with ltr=0.
//
{
INT i, flp, flprop;
BYTE curr_font;
WORD curr_cosinus;
PWORD vectp;
DWORD norm;
PBYTE list;
CHAR cnt,n_rsn;
elmBOX *wmatr;
t_answer *ans_ptr;
WORD vect[16];
WORD  nAll;

Z=&string;

memset(&RR,0,sizeof(RR));
vectp=SBOX->matrBOX.vect;
norm=SBOX->matrBOX.isq_rt;
for(i=0;i<15;i++)
    vect[i] = (WORD)((((LONG)vectp[i])<<15)/norm);
list = full_list[typl];
nAll = 0;


while ((curlet = ((LST *)list)->let) != 0)
    {
    flp = ((LST *)list)->flag;	list += sizeof(LST);
    flprop = proplet(curlet);
    if( !pidx_skip( SBOX->rast_h, SBOX->rast_w,curlet) )    flprop = 0;
    curr_cosinus = 0; cnt=0;
    if( erection & erect_rot )
        {
        while((wmatr = *(elmBOX **)list) != NULL)
            {
            if (flprop)
                {
                nAll++;
                i = (INT)((*scalar)(wmatr->vect,vect)>>15);

                if ((WORD)i > (WORD)curr_cosinus)
                    {
                    if ((WORD)i == 32767)
                        i=32766;
                    if ((WORD)i > 32767) 
                        i=32767;
                    *(PBYTE)(&curr_font)=wmatr->fnt;
                    *(PWORD)(&curr_cosinus)=i;
                    n_rsn=cnt;
                    }
                }
            cnt++;
            list += sizeof(PBYTE *);
            }  // while all letter's list entries
        }
    else
        {
        while((wmatr = *(elmBOX **)list) != NULL)
            {
            if (flprop && !(wmatr->bnd&0x200) ) 
                {
                nAll++;
                i = (INT)((*scalar)(wmatr->vect,vect)>>15);

                if ((WORD)i > (WORD)curr_cosinus)
                    {
                    if ((WORD)i == 32767)i=32766;
                    if ((WORD)i > 32767) i=32767;
                    *(PBYTE)(&curr_font)=wmatr->fnt;
                    *(PWORD)(&curr_cosinus)=i;
                    n_rsn=cnt;
                    }
                }
            cnt++;
            list += sizeof(PBYTE *);
            }  // while all letter's list entries
    }
        if (flprop)
            {
            if (flp)  // pairs used
                store_many(curr_cosinus,curlet,curr_font,typl);
            else
                store_new_bests(curr_cosinus,curlet,curr_font,n_rsn);
            }
    list += sizeof(PBYTE *);
    }   // while all list elements

// RR_answers to user's answer field
best_answer_BOX = RR.ibest_cosinus[0];
for (i=0,ans_ptr=SBOX->best_BOX; (i < NBBOX) && RR.best_lets[i]; i++,ans_ptr++)
    {
    ans_ptr->fnt=RR.best_font[i];
    ans_ptr->ltr=RR.best_lets[i];
    ans_ptr->iprob=RR.ibest_cosinus[i];
    ans_ptr->n_rsn=RR.n_rsn[i];
    }
ans_ptr->ltr=0;
allK += nAll;
}

//----------------- Sorting events by BOX -------------------------------
void wcomp_to_box(PWORD vector);

void sort_events_box (PWORD list, INT nl)
{
 PWORD lend, lcur, lcp;
 PWORD ncur, ncp;
 WORD ev_marks[100], probest;
 WORD vector[15], norm;
 BYTE ocurlet;
 elmBOX  *matr,  *wmatr;

 memset (vector,0,sizeof(vector));
 wcomp_to_box(vector);
 if ((norm = long_sqrt((*scalar)(vector, vector)))==0)  norm = 1;

 ncur = ev_marks;	lcur = list;	lend = list+nl;
 indp = (indBOX *) tableBOX;    matr = (elmBOX *) (tableBOX+1024);
 do
  {
     *ncur = 327;	ocurlet = *(PBYTE)lcur;
     wiptr=&indp[ocurlet];
     if ((!wiptr->ltr) ||                 // no such letter in table
      (strchr(forbidBOX,ocurlet)))      // forbidden in BOX
	 continue;
     //wmatr=(elmBOX *)((PBYTE)matr+wiptr->numel);
     wmatr=matr+wiptr->numel;      
     while(1)
      {
       probest=(*scalarf)(wmatr->vect, vector, norm);
       if (probest > *ncur)
	{
	 if (probest > 32767)     probest=32767;
	 *ncur = probest;
	}
       probest = wmatr->list;
       if (!probest) break;
       //wmatr=(elmBOX *)((PBYTE)matr+probest);
       wmatr=matr+wiptr->numel;      
      }

    ncp = ncur; lcp = lcur;
    while (ncp != ev_marks)
     {
      if (*ncp <= *(ncp-1)) break;
      if (*(((PBYTE)lcp)+1) < *(((PBYTE)lcp)-1)) break;
      probest = *ncp; *ncp = *(ncp-1); ncp--; *ncp = probest;
      probest = *lcp; *lcp = *(lcp-1); lcp--; *lcp = probest;
     }

  } while (ncur++, ++lcur != lend);

}

//--------------------- Load BOX tables ----------------------------------

static void straight_BOX(PBYTE free)
{
 indBOX *o;
 elmBOX *m;
 o=(indBOX  *)tableBOX;
 m=(elmBOX  *)(tableBOX+1024);
 //while (++o != (indBOX *) m)    o->numel *= sizeof (elmBOX);
 //while (free -(PBYTE)m > 0) { m->list *= sizeof(elmBOX); m++; }
}

static PBYTE list_BOX(PBYTE free, INT typl)
{
 INT flp;
 BYTE tlw;
 register unsigned ocurlet;
 WORD list;
 indBOX *owiptr;
 elmBOX *matr, *wmatr;

 Z=&string;

 full_list[typl] = free;

   if( typl >= 32 ){
      BYTE  bnd=0;
      if( typl >= 256 )
        bnd = 1;
      indp = owiptr = (indBOX  *)tableBOX;
      matr = (elmBOX  *)(tableBOX+1024);
      rbil=rbfo=rbfc=rbfv=rbfw=rbfx=rbfs=rbfp=rbfz=0;//TMP
      tlw = typl & 0x0ff;
      owiptr = &owiptr[tlw];
      if( owiptr->ltr ){
         curlet = ocurlet = owiptr->ltr;
         tlw = 0;
         //wmatr=(elmBOX *)((PBYTE)matr+owiptr->numel);
         wmatr=matr+owiptr->numel;
         ((LST *)free)->let = curlet;
         ((LST *)free)->flag = 0;
         free += sizeof (LST);
         while(1){
            if( ( (wmatr->bnd/256) & 0x01 ) == bnd ){
               tlw++;
               *(PBYTE *)free = (PBYTE) wmatr;   free += sizeof(PBYTE *);
            }
            list = wmatr->list;
            if (!list) break;
            //wmatr=(elmBOX *)((PBYTE)matr+list);
            wmatr=(elmBOX *)matr+list;
         }
         if( tlw ){
            *((PBYTE *)free) = NULL; free += sizeof (PBYTE *);
         }
         else{
            free -= sizeof (LST);
         }
      }/*if( owiptr->ltr )*/
      ((LST *)free)->let = 0;  free += sizeof (LST);

      return free;
   }/*if( typl >= 32 )*/

second:
 indp=owiptr=(indBOX  *)tableBOX;
 matr=(elmBOX  *)(tableBOX+1024);
 rbil=rbfo=rbfc=rbfv=rbfw=rbfx=rbfs=rbfp=rbfz=0;
 while (++owiptr != (indBOX *)matr)
  {
  if (!(owiptr->ltr)) continue;
  ocurlet = owiptr->ltr;
  if (typl & 256)
   { if ( memchr("1Iil",ocurlet,4) || 
			ocurlet==liga_i ||
			language == LANG_TURKISH &&  // 30.05.2002 E.P.
				(ocurlet==i_sans_accent||ocurlet==II_dot_accent) 

		) continue;}
  else
   { if (!memchr("1Iil",ocurlet,4) && 
		ocurlet!=liga_i &&
		!(language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(ocurlet==i_sans_accent||ocurlet==II_dot_accent) 
		 )
		) continue;}
  curlet = ocurlet;
  if ((tlw = typl & 0x0f) != 0)
    if ((let_lincomp[ocurlet] & tlw) == 0)   // size incompatible to letter
      continue;
  //wmatr=(elmBOX *)((PBYTE)matr+owiptr->numel);
  wmatr=matr+owiptr->numel;
  if ((flp=letagain(curlet,0))==0)
   {
     ((LST *)free)->let = curlet;
     ((LST *)free)->flag = 0;
     free += sizeof (LST);
     goto lwrk; // not a "paired" letter
   }
  if (wiptr==NULL) continue;            // all versions used

  ((LST *)free)->let = ocurlet;
  ((LST *)free)->flag = 1;
  free += sizeof(LST);

rwrk:;
  //wmatr=(elmBOX *)((PBYTE)matr+wiptr->numel);
  wmatr=matr+wiptr->numel;          
lwrk:;
  while(1)
   {
   if( ( (wmatr->bnd/256) & 0x01 ) == 0 ){
      *(PBYTE *)free = (PBYTE) wmatr;   free += sizeof(PBYTE *);
   }
   list = wmatr->list;
   if (!list) break;
   //wmatr=(elmBOX *)((PBYTE)matr+list);
   wmatr=(elmBOX *)matr+list;   
   }

  if (flp == 0) goto lwrs;  // not a paired letter
  if  (letagain(curlet,1) && wiptr) goto rwrk;
lwrs:;
  *((PBYTE *)free) = NULL; free += sizeof (PBYTE *);
  }
 if ((typl & 256) == 0)
  {
   full_list[16+typl] = free;
   typl |= 256;
   goto second;
  }
 ((LST *)free)->let = 0;  free += sizeof (LST);

 return free;
}

void correct_let_tables(void);
void correct_letters_pidx_table(void);

PBYTE load_BOX(PBYTE free)
{
 INT i;
 correct_let_tables(); // overload lang depending tables б¬. д ©« ACC_TABS.c
 correct_letters_pidx_table(); // overload proportinal tab
 straight_BOX(free);
 for (i=0; i<16; i++)
   free = list_BOX(free,i);
 for (i=32; i<512; i++)
   free = list_BOX(free,i);

//for  two  tables  3x5
 memcpy(omni_full_list,full_list,512*sizeof(PBYTE));
 omniBOX = tableBOX;

 return free;
}


void load_font( INT  font )
{

 if( font ){
    memcpy(full_list,font_full_list,512*sizeof(PBYTE));
    tableBOX = fontBOX;
 }
 else{
    memcpy(full_list,omni_full_list,512*sizeof(PBYTE));
    tableBOX = omniBOX;
 }

 return;
}/*load_font*/


PBYTE  preload_font_BOX( PBYTE free )
{
 INT   i;
// LONG  l;
// BYTE  workLetter;
// StructTree *tmp=(StructTree *)auto_pool;               //AK

 /*
 i = 0;
 workLetter = 1;
 while( workLetter )
   {
    workLetter = tmp[i].let;
    if( !workLetter )
      { //AK 29.04.97 sizeof(struct StructTree) = 12 (!) ?
       fontBOX = auto_pool + tmp[i].tr1;                //AK 29.04.97
       l = tmp[i].tr2 - tmp[i].tr1;                     //AK 29.04.97
       
	   break;
      }
    i++;
   }

*/
 correct_let_tables(); // overload lang depending tables б¬. д ©« ACC_TABS.c
 correct_letters_pidx_table(); // overload proportinal tab

 {
  indBOX *o;
  elmBOX *m;
  int size_m;

  size_m = sizeof(*m);                            //AK 29.04.97
  o=(indBOX  *)fontBOX;
  m=(elmBOX  *)(fontBOX+1024);
//Followed makes vissssssss!                      //AK 10.04.97
// sizeof(elmBOX) = 0 - ?        see embbox.h for def. struct elmBOX
/*
  while (++o != (indBOX *) m)
     o->numel *= size_m;      //AK 29.04.97
  while (free-(PBYTE)m > 0)
    {
     m->list *= size_m;        //AK 29.04.97
     m++;
    }
*/
 }

 tableBOX = fontBOX;

 for (i=0; i<16; i++)
   free = list_BOX(free,i);
 for (i=32; i<512; i++)
   free = list_BOX(free,i);
 memcpy(font_full_list,full_list,512*sizeof(PBYTE));

 return  free;
}/*preload_font_BOX*/
