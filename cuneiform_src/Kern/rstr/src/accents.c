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
/*	                Recognition of accents	                     */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nt_types.h"
#include "struct.h"
#include "func.h"
#include "ligas.h"
#include "lang.h"
#include "linutil.h"
#include "status.h"

#include "compat_defs.h"
#include "minmax.h"

extern BYTE language;
extern INT pitchsize;
extern BYTE db_status;

cell *accadr1;
cell *accadr2;

static B_LINES bl;
static INT H;

static INT acc_lr(cell *,cell *,PBYTE);
static INT acc_roof(cell *,PBYTE);
static INT acc_weak_roof(cell *cc,PBYTE r);	// 31.05.2001 E.P.
static INT acc_2dot(cell *,cell *,PBYTE,BYTE);
static INT acc_cir(cell *,PBYTE);
static INT acc_tild(cell *,PBYTE);
static INT acc_macron(cell *,PBYTE);	// 17.07.2001 E.P.
static INT acc_dot(cell *c,cell *cc);

int16_t NumIntersect2(c_comp *cmp, int mHei);

// 04.09.2000 E.P.
static INT acc_double_right(cell *c,cell *cc,PBYTE r);
static INT acc_semicircle(cell *cc,PBYTE r);

//static void debug_print_raster(char *text, PBYTE r); 19.07.2001 E.P.

/* См. ligas.h 31.08.2000 E.P.
#define ACC_LEFT			1
#define ACC_RIGHT			2
#define ACC_LR				(ACC_LEFT|ACC_RIGHT)
#define ACC_ROOF			4
#define ACC_TILD			8
#define ACC_2DOT           16
#define ACC_CIR			   32
#define ACC_DOT			   64
#define ACC_UNKNOWN		  127
#define ACC_SUPERUP		  128
#define ACC_ROOF_INV	  256
#define ACC_DOUBLE_RIGHT  512	// HUNGAR 30.08.2000 E.P. Над O, U
#define ACC_SEMICIRCLE	 1024	// ROMAN 31.08.2000 E.P. Полукруг над А.
#define ACC_WEAK_ROOF	 2048	// ROMAN 31.05.2001 E.P. Слабый акцент над 'i'
#define ACC_MACRON		 4096	// LATVIAN 05.07.2001 E.P. Черта сверху над AEIU
*/
//**************************************************************************
void pass_accent(void)
{
cell *c;
BYTE  let;
for(c=cell_f()->nextl;c!=cell_l();c=c->nextl)
  if( c->nvers>0 )
  {
  let = c->vers[0].let;
  if ( (language==LANG_FRENCH || language==LANG_ITALIAN) &&
				      memchr("AEIOUaeiou",let,10) ||
       language==LANG_SPANISH  && memchr("AEINOUaeniou",let,12) ||
       language==LANG_GERMAN   && memchr("AOUaou",let,6) ||
       language==LANG_CROATIAN && memchr("SZCszc",let,6) ||
       language==LANG_DANISH   && memchr("Aa",let,2) ||
       language==LANG_DUTCH    && memchr("EIOeio",let,6) ||
       language==LANG_POLISH   && memchr("SZCNOszcno",let,10) ||
       language==LANG_SWEDISH  && memchr("AEOaeo",let,6) ||
	   language==LANG_CZECH    && memchr(// 04.09.2000 E.P.
			"AaCcDdEeIiNnOoRrSsTtUuYyZz",let,26)
			||
       language==LANG_ROMAN    && memchr("AaIi",let,4) ||
       language==LANG_HUNGAR   && memchr("AaEeIiOoUu",let,10) ||
	   language==LANG_SLOVENIAN&& memchr("CcSsZz",let,6) ||

		// Как верхние, так и нижние акценты 10.07.2001 E.P.
		language==LANG_LATVIAN    && memchr("AaCcEeGgIiKkLlNnSsUuZz",let,22) ||
		language==LANG_LITHUANIAN && memchr("AaCcEeIiSsUuZz",let,14) ||
		language==LANG_ESTONIAN   && memchr("AaOoSsUuZz",let,10) ||
		// 30.05.2002 E.P.
		language==LANG_TURKISH   && memchr("AaCcGgIiOoSsUu",let,14) ||
	   0
	   )
    accent(c);
   }
return;
}
//**************************************************************************
INT bottom_accent(cell *c)
{
 INT i,n,chr;
 version *v1,*v2,vers[VERS_IN_CELL];
 INT was_l = 0;

 if (!find_bottom_accent(c))
    return 0;

 if( language == LANG_LATVIAN )
 {
  for (v1=c->vers,v2=vers,i=0; i<c->nvers; i++,v1++)
	  if( v1->let == 'l' )
	  {
		  was_l = 1;
		  break;
	  }
 }

 for (v1=c->vers,v2=vers,i=n=0; i<c->nvers; i++,v1++)
    {
    chr=v1->let;
    v2->let=0;
    switch(language)
        {
        case (LANG_FRENCH):
            switch (chr)
                {
                case 'C': v2->let = CC_bottom_accent;break;
                case 'c': v2->let = c_bottom_accent;break;
                }

			break;

        case (LANG_POLISH):
            switch (chr)
                {
                case 'A': v2->let = AA_bottom_accent_latin;break;
                case 'a': v2->let = a_bottom_accent_latin;break;
                case 'E': v2->let = EE_bottom_accent_latin;break;
                case 'e': v2->let = e_bottom_accent_latin;break;
                }

			break;

		// 04.09.2000 E.P.
        case LANG_ROMAN:
            switch (chr)
                {
                case 'S': v2->let = SS_bottom_accent_latin;break;
                case 's': v2->let = s_bottom_accent_latin;break;
                case 'T': v2->let = TT_bottom_accent;break;
                case 't': v2->let = t_bottom_accent;break;
                }

	        break;

		// 10.07.2001 E.P.
        case (LANG_LATVIAN):
            switch (chr)
                {
                case 'G': v2->let = GG_bottom_accent;break;
                case 'K': v2->let = KK_bottom_accent;break;
                case 'k': v2->let = k_bottom_accent; break;
                case 'L': v2->let = LL_bottom_accent;break;
                case 'l': v2->let = l_bottom_accent; break;
                case '1': if( !was_l )
							  v2->let = l_bottom_accent;
					      break; // Nick 31.08.01
                case 'N': v2->let = NN_bottom_accent;break;
                case 'n': v2->let = n_bottom_accent; break;
                }

			break;

		// 10.07.2001 E.P.
        case (LANG_LITHUANIAN):
            switch (chr)
                {
                case 'A': v2->let = AA_bottom_accent;break;
                case 'a': v2->let = a_bottom_accent; break;
                case 'E': v2->let = EE_bottom_accent;break;
                case 'e': v2->let = e_bottom_accent; break;
                case 'I': v2->let = II_bottom_accent;break;
                case 'i': v2->let = i_bottom_accent; break;
                case 'U': v2->let = UU_bottom_accent;break;
                case 'u': v2->let = u_bottom_accent; break;
                }

			break;

        case (LANG_TURKISH):
            switch (chr)
                {
                case 'C': v2->let = CC_bottom_accent;break;
                case 'c': v2->let = c_bottom_accent;break;
                case 'S': v2->let = SS_bottom_accent_turkish;break;
                case 's': v2->let = s_bottom_accent_turkish;break;
                }

			break;

        }

    if ( v2->let )
        {
        v2->prob=MIN(254,v1->prob+84);
        v2++;
        n++;
        }

	// Прибалтийские Iil _bottom_accent. Продолжить просмотр версий.
	else if ( (language==LANG_LATVIAN || language==LANG_LITHUANIAN)	)
			continue;

	// Турецкая c_bottom_accent после отрезания дает
	// несколько версий с оценкой 254.
	// 18.06.2002 E.P.
	else if ( language==LANG_TURKISH )
		{
		if ( (i+1 < c->nvers) && (v1+1)->prob >= 240 )
			continue;
		}
    else
        break;
    }

 if (!n)
  return 0;

 c->nvers=n;
 v2->let=0;
 c->accent_leader = 1; // Nick 31.08.2001
 c->bottom_accent = 1;
 memcpy(c->vers,vers,VERS_IN_CELL*sizeof(version));
 return 1;
}
//**************************************************************************
INT accent(cell *c)
 {
 INT acc_type,i,n;
 version *v1,*v2,vers[VERS_IN_CELL];
 BYTE str[15];

 if (!(acc_type=type_acc(c,TRUE)))
    return bottom_accent(c);     // Try to find bottom accent. 17.10.97 E.P.

 for (v1=c->vers,v2=vers,i=n=0; i<c->nvers; i++,v1++)
{
  if (
	(language==LANG_FRENCH||language==LANG_ITALIAN) && memchr("AEIOUaeiou",v1->let,10) &&
	acc_type!=ACC_CIR && acc_type!=ACC_TILD &&
	(v1->let!='A' && v1->let!='a' || acc_type!=ACC_RIGHT ||
					 acc_type!=ACC_2DOT) &&
	(v1->let!='I' && v1->let!='i' || !(acc_type&ACC_LR)) &&
	(v1->let!='O' && v1->let!='o' || acc_type==ACC_ROOF) &&
	(v1->let!='U' && v1->let!='u' || acc_type!=ACC_RIGHT) ||
      language==LANG_GERMAN && memchr("AOUaou",v1->let,6) &&
	acc_type==ACC_2DOT)
	goto _ok;

  if ( language==LANG_PORTUGUESE && (
    memchr("AEOUIaeoui",v1->let,10)&&acc_type==ACC_RIGHT ||
    memchr("AEOaeo",v1->let,6)&&acc_type==ACC_LEFT ||
    memchr("AEOaeo",v1->let,6)&&acc_type==ACC_ROOF ||
    memchr("AOao",v1->let,4)&&acc_type==ACC_TILD ||
    memchr("EIei",v1->let,4)&&acc_type==ACC_2DOT)
	)
	goto _ok;

  if (language==LANG_ITALIAN && memchr("o",v1->let,1) &&
      acc_type==ACC_LEFT ||
	language==LANG_RUSSIAN && !langUkr && !langSer && memchr("Ґ…",v1->let,2) &&
			acc_type==ACC_2DOT
	)
	goto _ok;

	if (language==LANG_CROATIAN && memchr("cC",v1->let,2) &&
			acc_type==ACC_RIGHT ||
  language==LANG_CROATIAN && (memchr("SsZzCc",v1->let,6)) &&
      acc_type==ACC_ROOF_INV
	  )
	goto _ok;

  if (language==LANG_DANISH && (memchr("Aa",v1->let,2)) &&
      acc_type==ACC_CIR
	  )
	goto _ok;

  if (language==LANG_DUTCH && (memchr("Ee",v1->let,2)) &&
      (acc_type==ACC_LEFT||acc_type==ACC_RIGHT||
       acc_type==ACC_ROOF||acc_type==ACC_2DOT) ||
  language==LANG_DUTCH && (memchr("Ii",v1->let,2)) &&
      (acc_type==ACC_2DOT) ||
  language==LANG_DUTCH && (memchr("Oo",v1->let,2)) &&
      (acc_type==ACC_LEFT||acc_type==ACC_RIGHT||acc_type==ACC_ROOF)
	  )
	goto _ok;

  if (language==LANG_POLISH && memchr("SZCNOszcno",v1->let,10) &&
			acc_type==ACC_RIGHT ||
		language==LANG_POLISH && memchr("Zz",v1->let,2) &&
            acc_type==ACC_DOT ||

  language==LANG_SWEDISH && memchr("AEOaeo",v1->let,6) &&
	((v1->let=='A' || v1->let=='a') &&
	 (acc_type==ACC_CIR || acc_type==ACC_2DOT) ||
	 (v1->let=='E' || v1->let=='e') && acc_type&ACC_RIGHT ||
	 (v1->let=='O' || v1->let=='o') && acc_type==ACC_2DOT)
	 )
	goto _ok;

  if (language==LANG_SPANISH && memchr("AEINOUaeinou",v1->let,12) &&
	(v1->let!='N' && v1->let!='n' && acc_type&ACC_RIGHT ||
	 (v1->let=='U' || v1->let=='u' && acc_type==ACC_2DOT) ||
	 (v1->let=='N' || v1->let=='n') && acc_type==ACC_TILD)
	 )
	goto _ok;

  // 04.09.2000 E.P.
  if (language==LANG_CZECH && (
	memchr("AaEeIiOoUuYytd",v1->let,14) && acc_type==ACC_RIGHT ||
	memchr("CcDdEeNnRrSsTtZz",v1->let,16) && acc_type==ACC_ROOF_INV ||
	memchr("Uu",v1->let,2) && acc_type==ACC_CIR
	) ||

  language==LANG_ROMAN && (
		memchr("Aa",v1->let,2) && acc_type==ACC_SEMICIRCLE ||
		memchr("AaIi",v1->let,4) && acc_type==ACC_ROOF ||
		v1->let=='i' && acc_type==ACC_WEAK_ROOF
	) ||

  language==LANG_HUNGAR && (
		memchr("AaEeIiOoUu",v1->let,10) && acc_type==ACC_RIGHT ||
		memchr("OoUu",v1->let,4) &&
			(acc_type==ACC_2DOT || acc_type==ACC_DOUBLE_RIGHT)
	) ||

  language==LANG_SLOVENIAN && (
	memchr("CcSsZz",v1->let,6) && acc_type==ACC_ROOF_INV
	)
	)
	goto _ok;

  // Балтийские акценты. 10.07.2001 E.P.
  if (
   language==LANG_LATVIAN    && (
	memchr("AaEeIiUu",v1->let,8)&& acc_type==ACC_MACRON    ||
	memchr("CcSsZz",v1->let,6)  && acc_type==ACC_ROOF_INV  ||
	v1->let=='g' && acc_type	// Все что ловится
	) ||

  language==LANG_LITHUANIAN && (
	memchr("CcSsZz",v1->let,6)  && acc_type==ACC_ROOF_INV	||
	(v1->let=='E' || v1->let=='e')  && acc_type==ACC_DOT	||
	(v1->let=='U' || v1->let=='u')  && acc_type==ACC_MACRON
	) ||

  language==LANG_ESTONIAN   && (
	memchr("AaOoUu",v1->let,6)  && acc_type==ACC_2DOT		||
	(v1->let=='O' || v1->let=='o')  && acc_type==ACC_TILD	||
	memchr("SsZz",v1->let,4)    && acc_type==ACC_ROOF_INV
	)
	)
	goto _ok;

  // Турецкие акценты 30.05.2002 E.P.
  if(language == LANG_TURKISH &&
	memchr("AaIiUu",v1->let,6) && acc_type == ACC_ROOF ||
	memchr("Gg",v1->let,2) && acc_type == ACC_SEMICIRCLE ||
	memchr("OoUu",v1->let,4) && acc_type == ACC_2DOT ||
	(v1->let=='I'||v1->let==i_sans_accent) && acc_type == ACC_DOT ||
	0)
	goto _ok;

	continue;

_ok:
   {
   if (n>VERS_IN_CELL-((acc_type==ACC_LR)?3:2))
    break;

   v2->let=0;			// 16.08.2001 E.P.
   switch (v1->let)
    {
    case 'c':
     switch (acc_type)
      {
      case ACC_RIGHT :		// CROAT,POLISH 04.09.2000 E.P.
		  v2->let = c_right_accent; break;

      case ACC_ROOF_INV :	// CROAT,CZECH 04.09.2000 E.P.
						    // А также балтийские языки. 10.07.2001 E.P.
		  v2->let = c_inv_roof;		break;

      default: continue;
      }
     break;
    case 'C':
     switch (acc_type)
      {
      case ACC_RIGHT :		// CROAT,POLISH 04.09.2000 E.P.
		  v2->let = c_right_accent; break;

      case ACC_ROOF_INV :	// CROAT,CZECH 04.09.2000 E.P.
						    // А также балтийские языки. 10.07.2001 E.P.
		  v2->let = CC_inv_roof;	break;

      default: continue;
      }
     break;
    case 's':
     switch (acc_type)
      {
      case ACC_RIGHT :		// POLISH
		  v2->let = s_right_accent; break;

      case ACC_ROOF_INV :	// CROAT,CZECH 04.09.2000 E.P.
						    // А также балтийские языки. 10.07.2001 E.P.
		  v2->let=s_inv_roof;		break;

      default: continue;
      }
     break;

    case 'S':
     switch (acc_type)
      {
      case ACC_RIGHT :		// POLISH
		  v2->let = SS_right_accent;	break;

      case ACC_ROOF_INV :	// CROAT,CZECH 04.09.2000 E.P.
						    // А также балтийские языки. 10.07.2001 E.P.
		  v2->let=SS_inv_roof;			break;

      default: continue;
      }
     break;

    case 'z':
     switch (acc_type)
      {

      case ACC_RIGHT :	// POLISH
		  v2->let = z_right_accent;			break;

      case ACC_DOT :	// POLISH
		  v2->let = z_dot_accent;			break;

      case ACC_ROOF_INV :     // CROAT, CZECH 04.09.2000 E.P.
						    // А также балтийские языки. 10.07.2001 E.P.
		  v2->let=z_inv_roof;				break;

      default:  continue;
      }
     break;

    case 'Z':
     switch (acc_type)
      {
      case ACC_RIGHT :	// POLISH
		  v2->let = ZZ_right_accent;		break;

      case ACC_DOT :	// POLISH
		  v2->let = ZZ_dot_accent;			break;

      case ACC_ROOF_INV :     // CROAT, CZECH 04.09.2000 E.P.
						    // А также балтийские языки. 10.07.2001 E.P.
		  v2->let = ZZ_inv_roof;		break;

      default:  continue;
      }
     break;

    case 'A':
     switch (acc_type)
      {
      case ACC_LEFT :         v2->let=AA_left_accent;        break;
      case ACC_RIGHT:         v2->let=AA_right_accent;       break;
      case ACC_ROOF :         v2->let=AA_roof_accent;        break;
      case ACC_2DOT :         v2->let=AA_2dot_accent;        break;
      case ACC_CIR  :         v2->let=AA_circle_accent;      break;
      case ACC_TILD :         v2->let=AA_tild_accent;        break;

      case ACC_LR   :
            if (language==LANG_FRENCH||language==LANG_ITALIAN)
              {
              v2->let=AA_left_accent;
              break;
              }
			if (language==LANG_SPANISH)
              {
              v2->let=AA_right_accent;
              break;
              }
            continue;

	  // ROMAN 04.09.2000 E.P.
	  case ACC_SEMICIRCLE:	  v2->let=AA_semicircle;		break;

	  // LATVIAN 10.07.2001 E.P.
	  case ACC_MACRON:		  v2->let=AA_macron;			break;

      default: continue;
      }
     break;
    case 'E':
     switch (acc_type)
      {
      case ACC_LEFT :        v2->let=EE_left_accent;    break;
      case ACC_RIGHT:        v2->let=EE_right_accent;   break;
      case ACC_ROOF :        v2->let=EE_roof_accent;    break;
      case ACC_ROOF_INV :	 v2->let=EE_inv_roof;		break; // CZECH 04.09.2000 E.P.
      case ACC_2DOT :        v2->let=EE_2dot_accent;    break;
      case ACC_LR   :
          if (language==LANG_FRENCH||language==LANG_ITALIAN)
			       {
			       v2->let=EE_right_accent;
			       v2->prob=MIN(254,v1->prob+84);
			       v2++; n++;
			       v2->let=EE_left_accent;  break;
			       }
          if (language==LANG_SWEDISH ||  language==LANG_SPANISH ||
              language==LANG_DUTCH)
             {
             v2->let=EE_right_accent;
             break;
             }
          continue;

	  // LATVIAN 10.07.2001 E.P.
	  case ACC_MACRON:		  v2->let=EE_macron;		break;

	  // LITHUANIAN 10.07.2001 E.P.
      case ACC_DOT :		  v2->let=EE_dot_accent;    break;

      default:                                          continue;
      }
     break;
    case 'I':
     switch (acc_type)
      {
      case ACC_LEFT :     v2->let=II_left_accent;       break;
      case ACC_RIGHT:     v2->let=II_right_accent;      break;
      case ACC_ROOF :     v2->let=II_roof_accent;		break;
      // Turkish 03.06.2002 E.P.
	  case ACC_DOT :	  v2->let=II_dot_accent;		break;
      case ACC_2DOT :     v2->let=II_2dot_accent;		break;
      case ACC_LR   :     v2->let=II_right_accent;		break;
	  // LATVIAN 10.07.2001 E.P.
	  case ACC_MACRON:	  v2->let=II_macron;			break;

      default:    continue;
      }
     break;
    case 'N':
     switch (acc_type)
      {
      case ACC_RIGHT :    v2->let=NN_right_accent;  break; // POLISH
      case ACC_TILD :     v2->let=NN_tild_accent;   break;
      case ACC_ROOF_INV : v2->let=NN_inv_roof;      break; // CZECH 04.09.2000 E.P.
      default:    continue;
      }
     break;
    case 'O':
     switch (acc_type)
      {
      case ACC_LEFT :         v2->let=OO_left_accent;   break;
      case ACC_RIGHT:         v2->let=OO_right_accent;  break;
      case ACC_ROOF :         v2->let=OO_roof_accent;   break;
      case ACC_2DOT :         v2->let=OO_2dot_accent;   break;
      case ACC_LR   :         v2->let=OO_right_accent;  break;
      case ACC_TILD :         v2->let=OO_tild_accent;   break;
      case ACC_DOUBLE_RIGHT:  v2->let=OO_double_right;  break; // HUNGAR 04.09.2000 E.P.
      default:                                          continue;
      }
     break;
    case 'U':
     switch (acc_type)
      {
      case ACC_LEFT :         v2->let=UU_left_accent;   break;
      case ACC_RIGHT:         v2->let=UU_right_accent;  break;
      case ACC_ROOF :         v2->let=UU_roof_accent;   break;
      case ACC_2DOT :         v2->let=UU_2dot_accent;   break;

      case ACC_LR   :
		  if (language==LANG_FRENCH||language==LANG_ITALIAN)
		      {v2->let=UU_left_accent; break;}
		  if (language==LANG_SPANISH)
		      {v2->let=UU_right_accent; break;}
		  continue;

	  // 04.09.2000 E.P.
      case ACC_CIR:
		  v2->let=UU_circle_accent;  break;	// CZECH

      case ACC_DOUBLE_RIGHT:
		  v2->let=UU_double_right;	 break;	// HUNGAR

	  // LATVIAN,LITHUANIAN 10.07.2001 E.P.
	  case ACC_MACRON:		  v2->let=UU_macron;			break;

      default:	continue;
      }
     break;
    case 'a':
     switch (acc_type)
      {
      case ACC_LEFT :     v2->let=a_left_accent;        break;
      case ACC_RIGHT:     v2->let=a_right_accent;       break;
      case ACC_ROOF :     v2->let=a_roof_accent;        break;
      case ACC_2DOT :     v2->let=a_2dot_accent;        break;
      case ACC_TILD:

			// Тильда от версии 'o'. 16.08.2001 E.P.
		    if(language !=LANG_ESTONIAN)
				v2->let=a_tild_accent;
			break;

      case ACC_CIR  :     v2->let=a_circle_accent;      break;
      case ACC_LR   :
          if (language==LANG_FRENCH||language==LANG_ITALIAN)
             {
             v2->let=a_left_accent;
             break;
             }
          if (language==LANG_SPANISH)
             {
             v2->let=a_right_accent;
             break;
             }
          continue;

	  // ROMAN 04.09.2000 E.P.
	  case ACC_SEMICIRCLE:	v2->let=a_semicircle;		break;

	  // LATVIAN 10.07.2001 E.P.
	  case ACC_MACRON:		v2->let=a_macron;			break;

      default: continue;
      }
     break;
    case 'e':
     switch (acc_type)
      {
      case ACC_LEFT :     v2->let=e_left_accent;  break;
      case ACC_RIGHT:     v2->let=e_right_accent; break;
      case ACC_ROOF :     v2->let=e_roof_accent;  break;
      case ACC_ROOF_INV : v2->let=e_inv_roof;	  break; // CZECH 04.09.2000 E.P.
      case ACC_2DOT :     v2->let=e_2dot_accent;  break;
      case ACC_LR   :
        if (language==LANG_FRENCH||language==LANG_ITALIAN)
			       {
			       v2->let=e_right_accent;
			       v2->prob=MIN(254,v1->prob+84);
			       v2++; n++;
			       v2->let=e_left_accent;  break;
			       }
         if (language==LANG_SWEDISH || language==LANG_SPANISH)
             {
             v2->let=e_right_accent;
             break;
             }
         continue;

	  // LATVIAN 10.07.2001 E.P.
	  case ACC_MACRON:		v2->let=e_macron;		break;

	  // LITHUANIAN 10.07.2001 E.P.
      case ACC_DOT :		v2->let=e_dot_accent;   break;

      default:                                      continue;
      }
     break;
    case (BYTE)'Ґ':
     switch (acc_type)
      {
      case ACC_2DOT :
		  if( language==LANG_RUSSIAN && !langUkr && !langSer  && !langBul) // 31.08.2000 E.P.
                {
                v2->let = r_e_2dot;
                break;
                }
      default:                                          continue;
      }
     break;
    case (BYTE)'…':
     switch (acc_type)
      {
      case ACC_2DOT :
		  if( language==LANG_RUSSIAN && !langUkr && !langSer && !langBul ) // 31.08.2000 E.P.
                {
                v2->let = r_EE_2dot;
                break;
                }
      default:                                          continue;
      }
     break;

    case 'g':
	  // Над LATVIAN_g все что ловится. 10.07.2001 E.P.
	  if (language==LANG_LATVIAN)
         {
         v2->let=LATVIAN_g;
		 break;
         }
	  if (language==LANG_TURKISH)
         {
         v2->let = g_semicircle;
		 break;
         }
		continue;

      break;

	case 'G':
	  if (language==LANG_TURKISH)
         {
         v2->let = GG_semicircle;
		 break;
         }
		continue;

      break;

    case 'i':
     switch (acc_type)
      {
      case ACC_LEFT :     v2->let=i_left_accent;        break;
      case ACC_RIGHT:     v2->let=i_right_accent;       break;
      case ACC_ROOF :     v2->let=i_roof_accent;        break;
      case ACC_2DOT :     v2->let=i_2dot_accent;        break;
      case ACC_LR   :     v2->let=i_right_accent;		break;

	  // ROMAN 31.05.2001
	  // TURKISH 30.05.2002 E.P.
	  case ACC_WEAK_ROOF:
		if (n<VERS_IN_CELL-2)
			{
			// Сохранить версию 'i'
			v2->let = 'i';
			v2->prob=MAX(10,v1->prob-10);
			v2++;
			n++;

			v2->let=i_roof_accent;
			v2->prob=MAX(10,v1->prob-12);
			v2++;
			n++;
			continue;
			}

	  // LATVIAN 10.07.2001 E.P.
	  case ACC_MACRON:		v2->let=i_macron;			break;

      default:                                          continue;
      }
     break;
    case 'n':
     switch (acc_type)
      {
      case ACC_RIGHT :	  v2->let= n_right_accent;  break;   // POLISH
      case ACC_TILD :     v2->let=n_tild_accent;    break;
      case ACC_ROOF_INV : v2->let=n_inv_roof;       break;   // CZECH 04.09.2000 E.P.
      default:   continue;
      }
     break;
    case 'o':
     switch (acc_type)
      {
      case ACC_LEFT :         v2->let=o_left_accent;    break;
      case ACC_RIGHT:         v2->let=o_right_accent;   break;
      case ACC_ROOF :         v2->let=o_roof_accent;    break;
      case ACC_2DOT :         v2->let=o_2dot_accent;    break;
      case ACC_LR   :         v2->let=o_right_accent;   break;
      case ACC_TILD :         v2->let=o_tild_accent;    break;
      case ACC_DOUBLE_RIGHT:  v2->let=o_double_right;   break; // HUNGAR 04.09.2000 E.P.
      default:                                          continue;
      }
     break;

    case 'u':
     switch (acc_type)
      {
      case ACC_LEFT :         v2->let=u_left_accent;    break;
      case ACC_RIGHT:         v2->let=u_right_accent;   break;
      case ACC_ROOF :         v2->let=u_roof_accent;    break;
      case ACC_2DOT :         v2->let=u_2dot_accent;    break;

      case ACC_LR   :
		  if (language==LANG_FRENCH||language==LANG_ITALIAN)
		      {v2->let=u_left_accent; break;}
		  if (language==LANG_SPANISH)
		      {v2->let=u_right_accent; break;}
		  continue;

	  // 04.09.2000 E.P.
      case ACC_CIR:
		  v2->let=u_circle_accent;  break;	// CZECH

      case ACC_DOUBLE_RIGHT:
		  v2->let=u_double_right;   break;	// HUNGAR

	  // LATVIAN,ESTONIAN 10.07.2001 E.P.
	  case ACC_MACRON:		v2->let=u_macron;			break;

      default:   continue;
      }
     break;

	// CZECH 04.09.2000 E.P.
    case 'D':
     switch (acc_type)
      {
      case ACC_ROOF_INV :      v2->let=DD_inv_roof;    break;
      default: continue;
      }
     break;

    case 'd':
     switch (acc_type)
      {
	  case ACC_RIGHT:		// 11.09.2000 E.P.
      case ACC_ROOF_INV :      v2->let=d_inv_roof;    break;
      default: continue;
      }
     break;

	// CZECH 04.09.2000 E.P.
    case 'R':
     switch (acc_type)
      {
      case ACC_ROOF_INV :      v2->let=RR_inv_roof;    break;
      default: continue;
      }
     break;

    case 'r':
     switch (acc_type)
      {
      case ACC_ROOF_INV :      v2->let=r_inv_roof;    break;
      default: continue;
      }
     break;

	// CZECH 04.09.2000 E.P.
    case 'T':
     switch (acc_type)
      {
      case ACC_ROOF_INV :      v2->let=TT_inv_roof;    break;
      default: continue;
      }
     break;

    case 't':
     switch (acc_type)
      {
	  case ACC_RIGHT:		// 11.09.2000 E.P.
      case ACC_ROOF_INV :      v2->let=t_inv_roof;    break;
      default: continue;
      }
     break;

	// CZECH 04.09.2000 E.P.
    case 'Y':
     switch (acc_type)
      {
      case ACC_RIGHT :		   v2->let=YY_right_accent;    break;
      default: continue;
      }
     break;

    case 'y':
     switch (acc_type)
      {
      case ACC_RIGHT:		   v2->let=y_right_accent;    break;
      default: continue;
      }
     break;

    }

   if ( v2->let )	// 16.08.2001 E.P.
	   {
	   BYTE add_prob = 84;	// 27.06.2002 E.P.

	   // Для разваленной турецкой g_semicircle вредно добавлять
	   // слишком много. 27.06.2002 E.P.
	   if(language==LANG_TURKISH &&
			v2->let==g_semicircle &&
			v1->prob < 60
		 )
		 	add_prob = 10;

		v2->prob=MIN(254,v1->prob+add_prob);
		v2++;
		n++;
	   }

   }
  }

 if (!n)
  return 0;
 c->accent_leader=1;
 c->nvers=n;
 v2->let=0;
 memcpy(c->vers,vers,VERS_IN_CELL*sizeof(version));
 if (db_status)
  snap_newcell(c);
 if (snap_activity('d'))
  {
  switch (acc_type)
   {
   case ACC_DOT  :         sprintf(str,"dot accent");           break; //23.10.97
   case ACC_LEFT :         sprintf(str,"left accent");          break;
   case ACC_RIGHT:         sprintf(str,"right accent");         break;
   case ACC_ROOF :         sprintf(str,"roof accent");          break;
   case ACC_ROOF_INV :     sprintf(str,"roof invert accent");   break;
   case ACC_2DOT :         sprintf(str,"2 dot accent");         break;
   case ACC_LR   :         sprintf(str,"left or right accent"); break;
   case ACC_CIR  :         sprintf(str,"circle accent");        break;
   case ACC_TILD :         sprintf(str,"tilda accent");         break;
   case ACC_DOUBLE_RIGHT : sprintf(str,"double right accent");  break; // 31.08.2000 E.P.
   case ACC_WEAK_ROOF :    sprintf(str,"weak roof accent");     break; // 31.08.2000 E.P.
   case ACC_SEMICIRCLE :   sprintf(str,"semicircle accent");    break; // 19.07.2001 E.P.
   case ACC_MACRON:		   sprintf(str,"macron accent");		break; // 19.07.2001 E.P.
   }
  snap_show_text(str);
  snap_monitor();
  }
 return 1;
 }
//**************************************************************************
static void mirror(PBYTE rinp,INT h, INT wb )
{
INT i;
PBYTE pi, po;
BYTE  str[100];
for(po=rinp+wb*(h-1),pi=rinp,i=0;i<h/2;i++, pi+=wb, po-=wb)
  {
  memcpy(str,po,wb);
  memcpy(po,pi,wb);
  memcpy(pi,str,wb);
  }
return;
}
//**************************************************************************
INT type_acc(cell *c,Bool enable_mark_satellit)
 {
 cell *cc;
 INT i=0,e,r,d,fld, ret;
 PBYTE raster;
 BYTE let;
 BYTE acc_base[128];INT acc_base_num;

 accadr1=accadr2=NULL;
 get_b_lines(c,&bl);
 H=bl.ps;
 if( language==LANG_POLISH )
  {
  acc_base_num = 10;
  memcpy(acc_base,"SZCNOszcno",acc_base_num);
  }
 else if( language==LANG_CROATIAN )
  {
  acc_base_num = 6;
  memcpy(acc_base,"SZCszc",acc_base_num);
  }
 else if( language==LANG_RUSSIAN && !langUkr && !langSer )
  {
  acc_base_num = 2;
  memcpy(acc_base,"Ґ…",acc_base_num);
  }
 else if( language==LANG_DANISH )
  {
  acc_base_num = 2;
  memcpy(acc_base,"Aa",acc_base_num);
  }
 else if( language==LANG_DUTCH )
  {
  acc_base_num = 6;
  memcpy(acc_base,"EIOeio",acc_base_num);
  }
 else if( language==LANG_PORTUGUESE )
  {
  acc_base_num = 10;
  memcpy(acc_base,"AEIOUaeiou",acc_base_num);
  }

 // 04.09.2000 E.P.
 else if( language==LANG_CZECH )
  {
  acc_base_num = 26;
  memcpy(acc_base,"AaCcDdEeIiNnOoRrSsTtUuYyZz",acc_base_num);
  }
 else if( language==LANG_ROMAN )
  {
  acc_base_num = 4;
  memcpy(acc_base,"AaIi",acc_base_num);
  }
 else if( language==LANG_HUNGAR )
  {
  acc_base_num = 10;
  memcpy(acc_base,"AaEeIiOoUu",acc_base_num);
  }

 else if( language==LANG_SLOVENIAN )
  {
  acc_base_num = 6;
  memcpy(acc_base,"CcSsZz",acc_base_num);
  }

 // Только верхние акценты
 else if( language==LANG_LATVIAN )
  {
  acc_base_num = 15;
  memcpy(acc_base,"AaCcEegIiSsUuZz",acc_base_num);
  }
 else if( language==LANG_LITHUANIAN )
  {
  acc_base_num = 12;
  memcpy(acc_base,"CcEeIiSsUuZz",acc_base_num);
  }
 else if( language==LANG_ESTONIAN )
  {
  acc_base_num = 10;
  memcpy(acc_base,"AaOoSsUuZz",acc_base_num);
  }
 else if( language==LANG_TURKISH )
  {
  acc_base_num = 10;
  memcpy(acc_base,"AaGgIiOoUu",acc_base_num);
  }
 else
  {   // fara letters
  acc_base_num = 12;
  memcpy(acc_base,"AEINOUaeinou",acc_base_num);
  }

 // Пропустить версии без акцентов
 for (i=0; i < c->nvers &&
		!memchr(acc_base,let_sans_acc[c->vers[i].let],acc_base_num);
	  i++) ;

 // Просмотр мусора
 let=let_sans_acc[c->vers[i].let];
 e=c->col+c->w;

 // ACC_ROOF_INF над d,t в Чешском похож на апостроф. 04.09.2000 E.P.
 if (language==LANG_CZECH && memchr("dt",let,2))
	 e += MAX(2,c->w/8);

 for (fld=0,cc=c->prevl->next;
	  cc->col<=e && cc!=cell_l();
	  cc=cc->next
	 )
  // Первая проверка
  if ( (cc->flg&(c_f_dust|c_f_punct) && cc->env) && // dust
      8*cc->h<5*H &&                   // not too big
      (cc->row+cc->h-1<=c->row ||      // upper position
       c->vers[i].let=='A' && cc->row-1<=c->row) &&
      (cc->row+4>=MIN(bl.b0,bl.b1) ||  // not too upper
       cc->row+cc->h>=c->row-bl.ps/4 ||
       pitchsize && cc->row+cc->h+3>=bl.b1) &&
      cc->h+cc->w>=bl.ps/5)            // not too small
   {

   raster=save_raster(cc);

   if (language==LANG_FRENCH || language==LANG_ITALIAN ||

	   is_cen_language(language)	|| // 04.09.2000 E.P.
	   is_baltic_language(language) || // 10.07.2001 E.P.

       language==LANG_SPANISH && !memchr("Nn",let,2) ||
       language==LANG_PORTUGUESE && memchr("AEOUIaeoui",let,10) ||
       language==LANG_SWEDISH && memchr("Ee",let,2) ||

		// Балтийский ACC_INV_ROOF и LATVIAN_g 10.07.2001 E.P.
		language==LANG_LATVIAN    && memchr("CcgSsZz",let,7) ||
		language==LANG_LITHUANIAN && memchr("CcSsZz",let,6)  ||
		language==LANG_ESTONIAN   && memchr("SsZz",let,4) ||

		// Только верхние акценты
		language==LANG_TURKISH && memchr("AaGgIiOoUu",let,10) ||
	   0
	  )
    {
	 // Вторая проверка
    if (cc->row+cc->h-((let=='i')?0:1)<=c->row &&
		cc->h+cc->w>=8 && (cc->h+cc->w>=H/2 || 3*cc->h<=2*cc->w) &&
		(let!='i' || cc->h+cc->w>=3*c->w && cc->w>=4) &&
		(d=cc->col+cc->w/2-(c->col+c->w/2))<=(2*H)/5 && d>=-H/4 &&
		-d<cc->w && 2*d<=3*MAX(cc->h,cc->w) &&
		(cc->col+cc->w>c->col+c->w/2 ||
		(cc->cg_flag&c_cg_cutacc)!=c_cg_cutacc)
	   )
     {

	 // ACC_DOUBLE_RIGHT 04.09.2000 E.P.
	 if (language==LANG_HUNGAR && memchr("OoUu",let,4) &&
		 acc_double_right(c,cc,raster)
		)
		{
		ret = ACC_DOUBLE_RIGHT;
		goto non_zero_ret;
		}

	 // В румынском есть ACC_ROOF и ACC_SEMICIRCLE,
	 // но нет ACC_ROOF_INV. 04.09.2000 E.P.
	if ( language==LANG_ROMAN )
		{
		// ACC_ROOF
		if (memchr("AOIi",let,4) && acc_roof(cc,raster))
			{
			ret= ACC_ROOF;
			goto non_zero_ret;
			}

		// Слабый acc_roof над румынской 'i'
		if (let=='i' && acc_weak_roof(cc,raster))
			{
			ret= ACC_WEAK_ROOF;
			goto non_zero_ret;
			}

		// ACC_SEMICIRCLE
		if (memchr("Aa",let,2))
			{
			mirror(raster,cc->h,(INT)((cc->w+7)/8) );
			if( acc_semicircle(cc,raster) )
				{
				ret= ACC_SEMICIRCLE;
				goto non_zero_ret;
				}
			mirror(raster,cc->h,(INT)((cc->w+7)/8) );
			}
		}

	if ( language==LANG_TURKISH )
		{
		// ACC_ROOF
		if (memchr("AaIiUu",let,6) && acc_roof(cc,raster))
			{
			ret= ACC_ROOF;
			goto non_zero_ret;
			}

		// Слабый acc_roof над турецкой 'i'
		if (let=='i' && acc_weak_roof(cc,raster))
			{
			ret= ACC_WEAK_ROOF;
			goto non_zero_ret;
			}

		// ACC_SEMICIRCLE
		if (memchr("Gg",let,2))
			{
			mirror(raster,cc->h,(INT)((cc->w+7)/8) );
			if( acc_semicircle(cc,raster) )
				{
				ret= ACC_SEMICIRCLE;
				goto non_zero_ret;
				}
			mirror(raster,cc->h,(INT)((cc->w+7)/8) );
			}
		}

	 // ACC_ROOF_INV
     if ( language==LANG_CROATIAN ||
		  language==LANG_CZECH    ||
		  language==LANG_HUNGAR   ||
		  language==LANG_SLOVENIAN||
		  is_baltic_language(language) &&	// 18.07.2001
			memchr("CcSsZz",let,6)
		)
      {
      mirror(raster,cc->h,(INT)((cc->w+7)/8) );
      if( acc_roof(cc,raster) )
		{
		ret= ACC_ROOF_INV;
		goto non_zero_ret;
		}
      mirror(raster,cc->h,(INT)((cc->w+7)/8) );
      }

	 // ACC_RIGHT, ACC_LEFT
     if (!(let=='i' && (cc->cg_flag&c_cg_cutacc)==c_cg_cutacc) &&
		 (let!='i' || cc->col<c->col+c->w) &&

		 !((language==LANG_FRENCH||language==LANG_ITALIAN) &&
			memchr("IOio",let,4) &&	c->vers[i+1].let!='a'
		  ) &&

		 !is_baltic_language(language) &&	// 14.08.2001 E.P.

		  (r=acc_lr(c,cc,raster))!=0
	    )
		{
		ret= r;
		goto non_zero_ret;
		}

	 /*
	 В cp1250 есть правый акцент и акцент справа от dt,
	 но нет левого акцента. 04.09.2000 E.P.
	 Здесь ловится все что похоже на ACC_LEFT,ACC_RIGHT:
	 */
	 if (is_cen_language(language) && ( r = acc_lr(c,cc,raster) ) !=0 )
		{
	    // Акцент над d,t в Чешском похож на апостроф. 04.09.2000 E.P.
		if (language==LANG_CZECH && memchr("dt",let,2))
			ret = ACC_ROOF_INV;
		else
			ret = ACC_RIGHT;

		goto non_zero_ret;
		}

	 // Над LATVIAN_g ловится все что похоже на ACC_LEFT,ACC_RIGHT
	 if (
		 (language==LANG_LATVIAN ||
		 // И над турецкой g_semicircle тоже
		  language==LANG_TURKISH)	// 30.05.2002 E.P.
		 && let=='g' && ( r = acc_lr(c,cc,raster) ) !=0 )
		{
		ret = r;
		goto non_zero_ret;
		}

     if( language==LANG_DUTCH )
      {
      if( memchr("EOeo",let,4) && acc_lr(c,cc,raster)==ACC_RIGHT )
        {ret= ACC_RIGHT;goto non_zero_ret;}
      if( memchr("EOeo",let,4) && acc_lr(c,cc,raster)==ACC_LEFT )
        {ret=ACC_LEFT;goto non_zero_ret;}
      if( memchr("EOeo",let,4) && acc_roof(cc,raster) )
        {ret=ACC_ROOF;goto non_zero_ret;}
      if( memchr("EIei",let,4) && acc_2dot(c,cc,raster,let) )
        {ret= ACC_2DOT;goto non_zero_ret;}
      }

    if( language==LANG_PORTUGUESE )
      {
      if( memchr("AEIOUaeiou",let,10) && acc_lr(c,cc,raster)==ACC_RIGHT )
        {ret= ACC_RIGHT;goto non_zero_ret;}
      if( memchr("AEIOaeio",let,8) && acc_lr(c,cc,raster)==ACC_LEFT )
        {ret= ACC_LEFT;goto non_zero_ret;}
      if( memchr("AEOUaeou",let,8) && acc_roof(cc,raster) )
        {ret= ACC_ROOF;goto non_zero_ret;}
      if( memchr("Ee",let,2) && acc_2dot(c,cc,raster,let) )
        {ret= ACC_2DOT;goto non_zero_ret;}
      if( memchr("AOao",let,4) &&
       2*cc->w>=c->w && 3*cc->h<=2*cc->w && 5*cc->h<=2*bl.ps &&
       (d=cc->col+cc->w/2-(c->col+c->w/2))<=MAX(5,cc->w/2) &&
       d>=-MAX(3,cc->w/3) &&
       acc_tild(cc,raster) )
        {ret= ACC_TILD;goto non_zero_ret;}
      }

     if (language==LANG_FRENCH && acc_roof(cc,raster))
		{
		ret= ACC_ROOF;
		goto non_zero_ret;
		}

     }
    else
     fld=1;
    }

   // ACC_2DOT
   if ( (
	    language==LANG_FRENCH && memchr("EIUeiu",let,6) ||
		language==LANG_GERMAN &&
			(memchr("AOUaou",let,6) ||
			 memchr("aou",c->vers[1].let,3)) ||
		language==LANG_SWEDISH &&
			(memchr("AOao",let,4) ||
			 memchr("ao",c->vers[1].let,2)) ||
		language==LANG_SPANISH && memchr("Uu",let,2) ||
		language==LANG_RUSSIAN &&
				!langUkr && !langSer &&
				!langBul && // 04.09.2000 E.P.
			memchr("…Ґ",let,2) ||

		language==LANG_HUNGAR && // 04.09.2000 E.P.
			memchr("OoUu",let,4) ||

		language==LANG_ESTONIAN && memchr("AaOoUu",let,6) ||	// 10.07.2001 E.P.

		// Турецкие умляуты по аналогии с немецкими 30.05.2002 E.P.
		language==LANG_TURKISH &&
			(memchr("OUou",let,4) ||
			 memchr("ou",c->vers[1].let,2)) ||
		0
		) && acc_2dot(c,cc,raster,let)
	   )
    {
	ret= ACC_2DOT;
	goto non_zero_ret;
	}

   // ACC_DOT
   if ( (
	   language == LANG_POLISH		&& (let=='Z'||let=='z') ||
	   language == LANG_LATVIAN		&& (let=='g')			||	// 16.08.2001 E.P.
	   language == LANG_LITHUANIAN	&& (let=='E'||let=='e') ||	// 10.07.2001 E.P.
	   language == LANG_TURKISH	&&
			(let=='I' || let==i_sans_accent) ||	// 30.05.2002 E.P.
	   0
	   ) && acc_dot(c,cc)
	  )
		{
		ret= ACC_DOT;
		goto non_zero_ret;
		}

   // ACC_CIR
   if (
	   (
	   (language==LANG_SWEDISH||language==LANG_DANISH) &&
			(memchr("Aa",let,2) || c->vers[1].let=='a') ||
		language==LANG_CZECH && memchr("Uu",let,2) // 04.09.2000 E.P.
	   ) &&
		MIN(cc->h,cc->w)>=bl.ps/4 &&
		abs(cc->h-cc->w)<=MAX(2,bl.ps/7)+
		(((cc->cg_flag&c_cg_cutacc)==c_cg_cutacc)?1:0) &&
		(d=cc->col+cc->w/2-(c->col+c->w/2))<=(2*H)/5 && d>=-H/4 &&
		acc_cir(cc,raster)
	  )
		{
		ret= ACC_CIR;
		goto non_zero_ret;
		}

   // ACC_TILD
   if (
	   ( language==LANG_SPANISH &&
			(memchr("Nn",let,2) || c->vers[1].let=='n')  ||
		 language==LANG_ESTONIAN &&
			(let=='O'||let=='o' ||
				(let=='a' && c->vers[1].let=='o') // 16.08.2001 E.P.
			)
	   ) &&
       2*cc->w>=c->w &&
	   (language!=LANG_ESTONIAN ? 3*cc->h<=2*cc->w:4*cc->h < 3*cc->w) &&
	   (language!=LANG_ESTONIAN ? 5*cc->h<=2*bl.ps : 7*cc->h < 3*bl.ps ) &&
       (d=cc->col+cc->w/2-(c->col+c->w/2))<=MAX(5,cc->w/2) &&
       d>=-MAX(3,cc->w/3) &&
       acc_tild(cc,raster)
	  )
		{
		ret= ACC_TILD;
		goto non_zero_ret;
		}

	// ACC_MACRON. 18.07.2001 E.P.
	if ( language==LANG_LATVIAN &&
			(memchr("AaEeIiUu",let,8) || memchr("aeIiUu",c->vers[1].let,6) )  ||
		language==LANG_LITHUANIAN &&
			(let=='U'||let=='u' || c->vers[1].let=='U' || c->vers[1].let=='u')
	  )
		{
		INT wlet=1;	// Обычные буквы: aEe

		// Узкие буквы
		if (strchr("Ii",let))
			wlet = 0;
		// Широкие буквы
		else if (memchr("AUu",let,8))
			wlet = 2;

		// Достаточно ли широкий акцент
		if ( wlet==0 &&   cc->w>=  c->w  ||	// Для узких букв   Ii
			 wlet==1 && 2*cc->w>=  c->w  ||	// Для обычных букв aEe
			 wlet==2 && 5*cc->w>=2*c->w		// Для широких букв AUu
		   )
			{
			// Горизонтальное смещение центра акцента от центра буквы
			d=cc->col+cc->w/2-(c->col+c->w/2);

			if ( 5*cc->h<=2*bl.ps &&	// Не слишком высокий
				 d<=MAX(5,cc->w/2)&&	// Не слишком справа от буквы
				 d>=-MAX(3,cc->w/3) &&		// Не слишком слева от буквы
				 c->col + (3*c->w)/2 > cc->col+cc->w/2 && // Над буквой Nick 20.08.01
				 c->col  - (c->w)/2 < cc->col+cc->w/2     // Над буквой
			   )
				{
				if(acc_macron(cc,raster))
					{
					ret= ACC_MACRON;
					goto non_zero_ret;
					}
				 }
		   }
		}
   }

 // "Второй проход"
 if (fld)
 {
  d=0;
  for (cc=c->prevl->next; cc->col<=e; cc=cc->next)
  {
  d=0;
   if (cc->flg&(c_f_dust|c_f_punct) &&                 // dust
       8*cc->h<5*H &&                                  // not too high
       cc->row+cc->h-((let=='i')?0:1)<=c->row &&       // upper position
       (cc->row+4>=MIN(bl.b0,bl.b1) ||                 // not too upper
	cc->row+cc->h>=c->row-bl.ps/4 ||
	pitchsize && cc->row+cc->h+3>=bl.b1) &&
       cc->h+cc->w>=bl.ps/4)                           // not too small
    {
    if ((let!='i' || 2*(cc->h+cc->w)>=5*c->w && cc->w>=4 ||
		cc->h>=cc->w && cc->h>=4 || 3*cc->h<=2*cc->w || cc->w>=H/3) &&
		cc->h+cc->w>=H/3 &&
		(d=cc->col+cc->w/2-(c->col+c->w/2))<=(2*H)/5 && d>=-H/4 &&
		-d<cc->w && 2*d<=3*MAX(cc->h,cc->w) &&
		(cc->col+cc->w>c->col+c->w/2 ||
		(cc->cg_flag&c_cg_cutacc)!=c_cg_cutacc)
	   )
     {
     raster=save_raster(cc);
     if (!(let=='i' && (cc->cg_flag&c_cg_cutacc)==c_cg_cutacc) &&
		(let!='i' || cc->col<c->col+c->w) &&
	    !((language==LANG_FRENCH||language==LANG_ITALIAN) &&
				memchr("IOio",let,4) &&
				c->vers[i+1].let!='a') &&
		(r=acc_lr(c,cc,raster))!=0
		)
		{
		ret= r;
		goto non_zero_ret;
		}

	 // ACC_ROOF
     if (language==LANG_FRENCH ||
		 language==LANG_ROMAN  ||	// 12.09.2000 E.P.
		 language==LANG_TURKISH // 30.05.2002 E.P.
		 )
		 {
		 if ( acc_roof(cc,raster)  )
			{ret= ACC_ROOF;goto non_zero_ret;}

		 // 31.05.2001 E.P.
		 if ( (language==LANG_ROMAN ||
			 language==LANG_TURKISH // 30.05.2002 E.P.
			 )&&
			 c->vers[0].let=='i' &&
			 acc_weak_roof(cc,raster)
			)
			{ret= ACC_WEAK_ROOF;goto non_zero_ret;}
		 }
     }
    }
   }
  }
 return 0;

 non_zero_ret :;
 if( enable_mark_satellit )
    {
    for (e=c->col+c->w,cc=c->prevl->next; cc->col<=e; cc=cc->next)
        if (cc->flg&(c_f_dust|c_f_punct) )
            cc->flg_new|=c_fn_acccap;
    }
    return ret;
 }
//**************************************************************************
static INT acc_lr(cell *c,cell *cc,PBYTE r)
 {
 INT l,r1,r2,r3,b,s1,s2,s3,s4,s13,s24,i,j,d;
 char *sCanHaveLRAccents = NULL;     // 21.08.1997 E.P. for Polish
 INT nCanHaveLRAccents = 0;
 BYTE let;

 if (cc->env->nl>2)
  return 0;
 l=(cc->w+7)/8;
 if (c->vers[0].let=='i' && 3*cc->h<=5*cc->w &&
     !(!(r[0]&128) && r[l*(cc->h-1)]&128 &&
       r[(cc->w-1)/8]&(128>>((cc->w-1)%8)) &&
       !(r[l*(cc->h-1)+(cc->w-1)/8]&(128>>((cc->w-1)%8)))))
  {
  for (s1=i=0; i<cc->h; i++)
   for (j=0; j<cc->w; j++)
    if (r[l*i+j/8]&(128>>(j%8)))
     s1++;
  if (3*s1>2*cc->h*cc->w)
   return 0;
  }
 accadr1=cc;
 for (r1=cc->h-1; r1>=0 && !(r[l*r1]&0x80); r1--) ;
 j=cc->w/2;
 b=0x80>>(j%8);
 j=j/8;
 for (r2=cc->h-1; r2>=0 && !(r[l*r2+j]&b); r2--) ;
 j=cc->w-1;
 b=0x80>>(j%8);
 j=j/8;
 for (r3=cc->h-1; r2>=0 && !(r[l*r3+j]&b); r3--) ;
 if (r1>cc->h/2 && r3>cc->h/2 && 2*r2<r1+r3-1)
  return 0;
 for (s1=i=0; i<cc->h/2; i++)
  for (j=0; j<cc->w/2; j++)
   if (r[l*i+j/8]&(128>>(j%8)))
    s1++;
 for (s3=0,i=(cc->h+1)/2; i<cc->h; i++)
  for (j=(cc->w+1)/2; j<cc->w; j++)
   if (r[l*i+j/8]&(128>>(j%8)))
    s3++;
 for (s2=i=0; i<cc->h/2; i++)
  for (j=(cc->w+1)/2; j<cc->w; j++)
   if (r[l*i+j/8]&(128>>(j%8)))
    s2++;
 for (s4=0,i=(cc->h+1)/2; i<cc->h; i++)
  for (j=0; j<cc->w/2; j++)
   if (r[l*i+j/8]&(128>>(j%8)))
    s4++;

 if ( language == LANG_POLISH )
    {
    sCanHaveLRAccents = "SCZNOsczno";
    nCanHaveLRAccents = 10;
    }
 else if ( language == LANG_CROATIAN )
    {
    sCanHaveLRAccents = "Cc";
    nCanHaveLRAccents = 2;
    }
 // 04.09.2000 E.P.
 else if ( language == LANG_CZECH )
    {
    sCanHaveLRAccents = "AaEeIiOoUuYydt";	// Сюда же dt с апострофом
    nCanHaveLRAccents = 14;
    }
 else if ( language == LANG_HUNGAR )
    {
    sCanHaveLRAccents = "AaEeIiOoUu";
    nCanHaveLRAccents = 10;
    }
 else if ( language == LANG_LATVIAN )
    {
    sCanHaveLRAccents = "g";	// LATVIAN_g
    nCanHaveLRAccents = 1;
    }
 else if (is_baltic_language(language))
    {
    sCanHaveLRAccents = "";
    nCanHaveLRAccents = 0;
    }
 else
    {
    sCanHaveLRAccents = "AEIOUaeiou";
    nCanHaveLRAccents = 10;
    }

 for (i=0; i<c->nvers && memchr(sCanHaveLRAccents,
        c->vers[i].let,nCanHaveLRAccents)==NULL; i++)
           ;
 if( i==c->nvers )
	 i=0;

 s13=s1+s3;
 s24=s2+s4;

 let=c->vers[i].let;	// 16.07.2001 E.P.

 if (s13+s24>=6 || let!='i')
  {
  d=((cc->cg_flag&c_cg_cutacc)==c_cg_cutacc)?3:1;
  if ((memchr("AEUaeu",let,6) || c->vers[i+1].let=='a') &&
              (language==LANG_FRENCH||language==LANG_ITALIAN))
   if (let!='i' && 2*(s13-s24)>=s13-d ||
       s13-s24>=MAX(3,s13/4) && (3*cc->h<2*cc->w || 3*cc->w<2*cc->h) ||
       4*s13>=11*s24 || 3*(s13-s24)>=s13 && s4<=1 && 6*s4<=s24)
    return ACC_LEFT;
  if (let!='i' && 2*(s24-s13)>=s24-d ||
      s24-s13>=MAX(3,s24/4) && (3*cc->h<2*cc->w || 3*cc->w<2*cc->h) ||
      4*s24>=11*s13 || 3*(s24-s13)>=s24 && s3<=1 && 6*s3<=s13)
   return ACC_RIGHT;

  if (cc->w<=cc->h/2 &&
      (let!='i' || cc->h>=c->h/3 && 3*cc->w<=cc->h))
   {
   if ((memchr("AEUaeu",let,6) || c->vers[i+1].let=='a') &&
               (language==LANG_FRENCH||language==LANG_ITALIAN))
    return ACC_LR;
   else
    if (s24>=s13)
        {
        // Avoid mistake with Zz dot accent. 23.10.97.
        if (!(
				(language==LANG_POLISH && memchr("Zz",let,2) ||

				 // And with Ee dot accent. 16.07.2001 E.P.
			     language==LANG_LITHUANIAN && memchr("Ee",let,2) ||

				 // And with II dot accent. 31.05.2002 E.P.
			     language==LANG_TURKISH && let=='I'

				 ) &&
				  s24==s13 && MIN(s2,s4)<=MIN(s1,s3)
			  )
		   )
            return ACC_RIGHT;
        }

   }
   // Add chance for letters that can have ONLY right accent. 10.10.97 E.P.
   if ( language==LANG_POLISH && memchr("SCNOscno",let,8) )
     if ( s24>=s13 &&          // Right direction
          cc->h>=c->h/3        // Not too small
        )
        return ACC_RIGHT;

   // Add chance for latvian 'g'. 17.07.2001 E.P.
   if ( language==LANG_LATVIAN && let=='g' &&
          cc->h >= c->h/4 &&        // Not too small
		  cc->w >= c->w/4
	  )
        return ACC_RIGHT;

  }
 accadr1=NULL;
 return 0;
 }
//**************************************************************************
static INT acc_roof(cell *cc,PBYTE r)
 {
 INT r1,r2,r3,b,s11,s12,s21,s22,s31,s32,ss13,ss2,sp,sm,ss,i,j,l;

 if (cc->h>cc->w+2 || cc->w<=5)
  return 0;
 accadr1=cc;
 l=(cc->w+7)/8;
 for (r1=cc->h-1; r1>=0 && !(r[l*r1]&0x80); r1--) ;
 j=cc->w/2;
 b=0x80>>(j%8);
 j=j/8;
 for (r2=cc->h-1; r2>=0 && !(r[l*r2+j]&b); r2--) ;
 j=cc->w-1;
 b=0x80>>(j%8);
 j=j/8;
 for (r3=cc->h-1; r2>=0 && !(r[l*r3+j]&b); r3--) ;
 if (r1>=cc->h-2 && r3>=cc->h-2 && r2<=MIN(cc->h-2,2*(cc->h)/3) &&
     2*r2<r1+r3-2)
  return 1;
 if (r2==cc->h-1)
  {accadr1=NULL; return 0;}
 for (s11=s12=j=0; j<cc->w/3; j++)
  {
  for (i=0; i<cc->h/2; i++)
   if (r[i*l+j/8]&(128>>(j%8)))
    s11++;
  for (i=(cc->h+1)/2; i<cc->h; i++)
   if (r[i*l+j/8]&(128>>(j%8)))
    s12++;
  }
 for (s21=s22=0,j=cc->w/3; j<cc->w-cc->w/3; j++)
  {
  for (i=0; i<cc->h/2; i++)
   if (r[i*l+j/8]&(128>>(j%8)))
    s21++;
  for (i=(cc->h+1)/2; i<cc->h; i++)
   if (r[i*l+j/8]&(128>>(j%8)))
    s22++;
  }
 for (s31=s32=0,j=cc->w-cc->w/3; j<cc->w; j++)
  {
  for (i=0; i<cc->h/2; i++)
   if (r[i*l+j/8]&(128>>(j%8)))
    s31++;
  for (i=(cc->h+1)/2; i<cc->h; i++)
   if (r[i*l+j/8]&(128>>(j%8)))
    s32++;
  }
 ss13=(cc->h/2)*(cc->w/3);
 ss2=(cc->h/2)*(cc->w-2*(cc->w/3));
 sp=s12+s21+s32;
 sm=s11+s22+s31;
 ss=2*ss13+ss2;
 if ((5*s11>3*ss13 || 2*(ss13-s12)>ss13 ||
      5*s31>3*ss13 || 2*(ss13-s32)>ss13 ||
      /*5*s22>3*ss2 ||*/ 2*(ss2-s21)>ss2 || 5*sm>4*sp) &&
     (2*sm>sp || 5*sm>2*ss || 5*(ss-sp)>2*ss))
  {accadr1=NULL; return 0;}
 return 1;
 }
//**************************************************************************
static Bool32 IsProgib(PBYTE r,int w,int h)
{
	int i,j;
	int wb=(w+7)>>3;
	uchar cc;
	int tek;
	int buffer[128],bufUp[128],bufDn[128];
	int left,right,best;

	if(w>128 )
		w = 128;

	memset(buffer,0,w*sizeof(int));
	memset(bufUp,0,w*sizeof(int));
	memset(bufDn,0,w*sizeof(int));

	for(i=0;i<h;i++,r+=wb)
	{
		for(j=0,cc=128,tek=r[0];j<w;j++,cc>>=1)
		{
			if(!cc)
			{
				cc =128;
                tek=r[j>>3];
			}

			if( tek&cc )
			{
				buffer[j]++;
				if( !bufUp[j] )
					bufUp[j]=i+1;
				bufDn[j]=i;
			}
		}
	}

	for(i=1,left=0;i<w/2;i++)
		if(buffer[i]>=buffer[left] )
			left = i;

    for(i=w-1,right=w-1;i > w/2;i--)
		if(buffer[i]>=buffer[right] )
			right = i;

    for( i = best= left+1; i<right; i++)
		if( buffer[i] < buffer[best] )
			best = i;

    if( buffer[best] *3 < MIN(buffer[left],buffer[right]) )
		return TRUE;

    if( (buffer[best]-1)*2 >  MIN( buffer[left],buffer[right]) )
		return FALSE;

	for(i=best,j=0;i>=left;i--)
	{
		if( ( bufUp[i] < bufUp[best] && bufDn[i] < bufDn[best] ) ||
			( bufUp[i] > bufUp[best] && bufDn[i] > bufDn[best] )
          )
		{
			j++;
			break;
		}
	}

	for(i=best;i<=right;i++)
	{
		if( ( bufUp[i] < bufUp[best] && 	bufDn[i] < bufDn[best] ) ||
			( bufUp[i] > bufUp[best] && bufDn[i] > bufDn[best] )
          )
		{
			j++;
			break;
		}
	}

	if( j > 0 )
        return FALSE;

	return TRUE;
}
/////////////////
static INT acc_2dot(cell *c,cell *cc,PBYTE r,BYTE let)
 {
 cell *cc1;
 INT l,d,b,j,i1,i2,r1,r2,r3,s1,s2,u;
 BYTE let1;

 if (cc->row<bl.b1-1 && cc->row+cc->h<c->row-MAX(4,cc->h)-1)
  return 0;

 if( !(language==LANG_RUSSIAN && !langUkr && !langSer) )
 {
  if (
	 (
	  2*(cc->h-(((cc->cg_flag&c_cg_cutacc)==c_cg_cutacc)?1:0))<=cc->w+1 ||
	  // Nick 29.08.01 for Estonian
	  language == LANG_ESTONIAN && 3*cc->h < 2*H &&
      (NumIntersect2(cc->env, cc->h)+1)*2 >= cc->h
     )
	  &&
     (d=cc->col+cc->w/2-(c->col+c->w/2))<=4*H/9 && d>=-H/4 &&
     (!memchr("Uu",let,2 || (cc->cg_flag&c_cg_cutacc)!=c_cg_cutacc) ||
      cc->w<3*c->w/4)
	)
  {
  l=(cc->w+7)/8;
  j=(cc->w-1)/2;
  b=128>>(j%8);
  j/=8;
  for (i1=0; !(r[i1*l+j]&b) &&
		i1 < cc->h;		// 04.09.2000 Nick
		i1++) ;
  for (i2=cc->h-1; !(r[i2*l+j]&b) &&
		i2 >=0;		// 04.09.2000 Nick
		i2--) ;

  i2=cc->h-1-i2;
  if (i1 /*&& i1>=cc->h/3*/ &&
      (i2 || (cc->cg_flag&c_cg_cutacc)==c_cg_cutacc) &&
      i1+i2+(((cc->cg_flag&c_cg_cutacc)==c_cg_cutacc)?1:0)>=
	  cc->h/2 - (language==LANG_ESTONIAN?1:0) )  // Nick 30.08.01
   {
   u=0;
   for (j=(cc->w-1)/2; j>=0 && !(r[l*(i1-1)+j/8]&(128>>(j%8))); j--) ;
   if (j>=0)
    u++;
   for (j=(cc->w-1)/2; j<cc->w && !(r[l*(i1-1)+j/8]&(128>>(j%8))); j++) ;
   if (j<cc->w)
    u++;
   if (i2)
    {
    i2=cc->h-1-i2;
    for (j=(cc->w-1)/2; j>=0 && !(r[l*(i2+1)+j/8]&(128>>(j%8))); j--) ;
    if (j>=0)
     u++;
    for (j=(cc->w-1)/2; j<cc->w && !(r[l*(i2+1)+j/8]&(128>>(j%8))); j++) ;
    if (j<cc->w)
     u++;
    }
   if (u<3 && i2 || u<2)
    return 0;

   if( language == LANG_ESTONIAN && !IsProgib(r,cc->w,cc->h) )
	   return 0;

	 accadr1=cc; return 1;
   }
  }
 }

 // now test two points
 accadr1=cc;
 if ((cc1=c->prevl)->col+cc1->w-c->col>=cc->w)
  if (cc->col>=c->col+c->w/2)
   if (cc1->row<=c->row-cc->h)
    if ((d=(cc1->col+cc1->w+cc->col)/2-(c->col+c->w/2))<=4*H/9 && d>=-H/4)
     if (!memchr("/l()J1It[]VW",(let1=cc1->vers[0].let),12) &&
			let1!=liga_i &&
		 !(language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(let1==i_sans_accent||let1==II_dot_accent)
		  )
		)
      if (let!='i')
       return 1;
 if (cc->col+cc->w/2<=c->col+c->w/2+((let=='A')?c->h:H)/4)
  if (c->prevl->flg&c_f_fict || cc->col>c->prevl->col+c->prevl->w ||
       !memchr("ij",c->prevl->vers[0].let,2) &&
			   cc->col+cc->w/2>=c->prevl->col+c->prevl->w)
   if (let!='i' ||
       cc->row+cc->h<=c->row && (cc->cg_flag&c_cg_cutacc)!=c_cg_cutacc)
    for (cc1=cc->next; cc1->col<c->col+c->w || cc1->flg&(c_f_dust|c_f_punct);
							       cc1=cc1->next)
     if (cc1->flg&(c_f_dust|c_f_punct))
      if (let!='i' || cc1->row+cc1->h<=c->row)
       if (cc1->row>=bl.b1-1 || cc1->row+cc1->h>=c->row-MAX(4,cc1->h)-2)
	if (cc1->h+cc1->w>=bl.ps/5)
	 if (c->nextl->flg&c_f_fict ||
	     cc1->col+cc1->w/2<=c->nextl->col+1 ||
	     memchr("fg",(let1=c->nextl->vers[0].let),2) ||
       let1==liga_fi || let1==liga_fl || is_liga_ff(let1) || //let1==liga_ff ||
       let1==liga_ffi || is_liga_ffl(let1) || //let1==liga_ffl ||
	     let=='i' && !memchr("ij",let1,2) ||
	     let1=='j' && cc1!=dot_ij(c->nextl))
	  if (cc->cg_flag&c_cg_cutl &&
	      (cc->cg_flag&c_cg_cutacc)!=c_cg_cutacc ||
	      (r1=abs(cc->h-cc1->h))<=(s1=MAX(2,(cc->h+cc1->h+3)/6)) &&
	      (r2=abs(cc->w-cc1->w))<=(s2=MAX(2,(cc->w+cc1->w+3)/6)) &&
	      (r3=abs(cc->row+cc->h/2-(cc1->row+cc1->h/2)))<=s1 &&
	       r1+r2+r3<=s1+s2/2+((let=='i')?0:2))
	   if ((d=(cc1->col+cc1->w+cc->col)/2-(c->col+c->w/2))<=
					     4*((let=='A')?c->h:H)/9 &&
	       (d>=-H/8 || (let!='i' || c->cg_flag&c_cg_cut) && d>-H/6 ||
		cc1->col+cc1->w/2>c->col && d>-H/5))
	    if (cc1->col+cc1->w-cc->col<=c->w || let=='i')
	     if (!(3*cc->w>2*cc->h && 3*cc1->w<=2*cc1->h &&
		   !(c->prevl->flg&c_f_fict) &&
		   c->prevl->col+c->prevl->w>cc->col &&
		   cc1->col+cc1->w/2-(c->col+c->w/2)<=H/6))
	      if (!memchr("Uu",let,2) || cc1->col+cc1->w-cc->col<3*c->w/4 ||
		  (cc->cg_flag&c_cg_cutacc)!=c_cg_cutacc &&
		  (cc1->cg_flag&c_cg_cutacc)!=c_cg_cutacc)
	       {accadr2=cc1; return 1;}
 accadr1=NULL;
 return 0;
 }
//**************************************************************************
static INT acc_cir(cell *c,PBYTE r)
 {
 INT l,u,v,i,j,s;

 accadr1=c;
 l=(c->w+7)/8;
 u=v=0;
 if (r[l*(c->h/2)]&(128+64))
  u++;
 if (r[l*(c->h/2)+(c->w-1)/8]&(128>>((c->w-1)%8)) ||
     r[l*(c->h/2)+(c->w-2)/8]&(128>>((c->w-2)%8)))
  {u++; v++;}
 if ((r[(c->w/2)/8]|r[l+(c->w/2)/8])&(128>>((c->w/2)%8)))
  u++;
 if ((r[l*(c->h-1)+(c->w/2)/8]|r[l*(c->h-2)+(c->w/2)/8])&
					       (128>>((c->w/2)%8)))
  {u++; v++;}
 if (u>=3 || (c->cg_flag&c_cg_cutacc)==c_cg_cutl && v==2)
  {
  if (!(r[l*(c->h/2)+(c->w/2)/8]&(128>>((c->w/2)%8))))
   return 1;
  if ((c->cg_flag&c_cg_cutacc)==c_cg_cutacc)
   for (i=c->h/2-1; i<=c->h/2+1; i++)
    for (j=c->w/2-1; j<=c->w/2+1; j++)
     if (!(r[l*i+j/8]&(128>>(j%8))))
      return 1;
  if (3*(c->h+c->w)>2*bl.ps)
   {
   for (s=i=0; i<c->h; i++)
    for (j=0; j<c->w; j++)
     if (r[l*i+j/8]&(128>>(j%8)))
      s++;
   if (3*s>2*c->h*c->w)
    return 1;
   }
  }
 accadr1=NULL;
 return 0;
 }
//**************************************************************************
static INT acc_tild(cell *c,PBYTE r)
 {
 INT j;
 INT noTild = 0;
 INT numIntersect=0;

 accadr1=c;
 if (c->h<=3 && 3*c->h<=c->w)
  return 1;
 j=c->w/3;
 if ((c->cg_flag&c_cg_cutacc)==c_cg_cutacc)
    j--;

 if ( language == LANG_ESTONIAN ||
	  language == LANG_PORTUGUESE	// 05.07.2002 E.P.
	)
	       numIntersect = NumIntersect2(c->env, c->h);

 if (r[((c->w+7)/8)*(c->h-1)+j/8]&(128>>(j%8)))
 {
      // Nick 29.08.2001
	 if ( language == LANG_ESTONIAN ||
		  language == LANG_PORTUGUESE	// 05.07.2002 E.P.
		)
	 {
		 j--;
		 if ( r[((c->w+7)/8)*(c->h-1)+j/8]&(128>>(j&7)) )
		 {
			 // dwo dots ?
           if( numIntersect*2 >= c->h )
		   {
	        accadr1=NULL;
	        return 0;
		   }

           noTild = 1;
		 }
	 }
	 // old version
	 else
	 {
	  accadr1=NULL;
	  return 0;
	 }
 }

 j = (2*(c->w))/3 ;
 if (r[j/8]&(128>>(j&7)) )
 {
	 // Nick 29.08.2001
	 if ( language == LANG_ESTONIAN ||
		  language == LANG_PORTUGUESE	// 05.07.2002 E.P.
		)
	 {
		 j++;
	     if (  (r[j/8]&(128>>(j&7)))  )
		 {
			 	 // dwo dots ?
           if( noTild || numIntersect*2 >= c->h )
		   {
	        accadr1=NULL;
	        return 0;
		   }
		 }
	 }
	 else // old standard
	 {
	  accadr1=NULL;
	  return 0;
	 }
 }

 return 1;
 }

 static INT acc_dot(cell *c,cell *cc)
 {
 extern BYTE fax1x2;

 H=bl.ps;
 accadr1 = accadr2 = NULL;

 if (cc->flg&(c_f_dust+c_f_punct) &&   // dust
      2*cc->h<c->h &&                  // not too high
      cc->row+cc->h-2<=bl.b2 &&        // upper position
      (cc->row+4>=MIN(bl.b0,bl.b1) ||  // not too upper
       cc->row>=bl.b1-bl.ps/3) &&
      cc->row+cc->h<=c->row)

     if (cc->cg_flag&c_cg_just && cc->h>=2 && cc->w>=2 ||
        !(cc->cg_flag&c_cg_just) && cc->h+cc->w >=3)

         if (8*cc->w>=c->w &&
             (abs(cc->h-cc->w)<=H/5 ||
              cc->h<cc->w && cc->w-(cc->h+((fax1x2)?2:0))<=H/4 ||
              cc->h<cc->w && 3*(c->row-cc->row)>=c->h && abs(cc->w-c->w)<=1 ||
              cc->h>=2*cc->w ) &&   // Added 23.10.97
             cc->col+3>=c->col &&
             abs(c->col+c->w/2-(cc->col+cc->w/2))<=MAX(2,c->w/4) && // abs 18.10.97 for Z
             cc->col+cc->w-6<=c->col+c->w &&
             (c->prevl->col+c->prevl->w<cc->col))
                {
                accadr1 = cc;
                return 1;
                }

 return 0;
 }
//**************************************************************************
INT find_bottom_accent(cell *c)
 {
 cell *cc;
 INT i,e,fld;
 BYTE let;
 BYTE acc_base[128];INT acc_base_num;

 accadr1=accadr2=NULL;
 get_b_lines(c,&bl);
 H=bl.ps;

 if( language==LANG_POLISH )
  {
  acc_base_num = 4;
  memcpy(acc_base,"AEae",acc_base_num);
  }
 else if( language==LANG_FRENCH )
  {
  acc_base_num = 2;
  memcpy(acc_base,"Cc",acc_base_num);
  }
 // 04.09.2000 E.P.
 else if( language==LANG_ROMAN )
  {
  acc_base_num = 4;
  memcpy(acc_base,"SsTt",acc_base_num);
  }
 // 10.07.2001 E.P.
 else if( language==LANG_LATVIAN )
  {
  acc_base_num = 7;
  memcpy(acc_base,"GKkLlNn",acc_base_num);
  }
 else if( language==LANG_LITHUANIAN )
  {
  acc_base_num = 8;
  memcpy(acc_base,"AaEeIiUu",acc_base_num);
  }
 else if( language==LANG_TURKISH )
  {
  acc_base_num = 4;
  memcpy(acc_base,"CcSs",acc_base_num);
  }
 else
    return 0;

 for (i=0;
 i < c->nvers && !memchr(acc_base,let_sans_acc[c->vers[i].let],acc_base_num);
								      i++) ;

 if( i >= c->nvers )
	 let = 0;
 else
     let=let_sans_acc[c->vers[i].let];

 for (fld=0,e=c->col+c->w,cc=c->prevl->next; cc&&cc->col<=e; cc=cc->next)
 {
  int ccx=cc->col;
  ccx=ccx;	// Отладка 05.07.2002 E.P.

  if ( (cc->flg&(c_f_dust|c_f_punct) && cc->env) && // dust
	  // not too big
      (2*cc->h<H ||
		( (language == LANG_LITHUANIAN ||
		   language == LANG_LATVIAN ||
		   language== LANG_TURKISH	// 19.06.2002 E.P.
		  ) && 3*cc->h<2*H
		)
	  ) &&
      c->row+c->h-1<=cc->row &&          // bottom position
      (cc->row<=bl.b4 ||                 // not too low
       cc->row<=c->row+c->h+bl.ps/4) &&
      cc->h+cc->w>=bl.ps/5 &&            // not too small
      cc->col+cc->w >= c->col+c->w/2     // not too left ">=" 05.07.2002 E.P.
     )
	  {
	  // Узкие румынские акценты под t, s. 16.07.2001 E.P.
	  if (language==LANG_ROMAN &&  (let=='t' || let=='s') )
		  {
		  if ( 8*cc->w < 5*c->w &&		// not too wide
			   2*cc->w >= cc->h			// proportion: not too thin
			 )
			  {
			  accadr1 = cc;
			  return 1;
			  }
		  }

	  // Узкие прибалтийские буквы и узкие акценты. 16.07.2001 E.P.
	  else if (language==LANG_LATVIAN &&
				// Узкая буква
				( (let=='l' || c->h > 3* c->w) &&
				   cc->w < 3*c->w &&		// not too wide
				   2*cc->w >= cc->h			// proportion: not too thin
				) ||
				// Узкие акценты типа запятой в центре под буквой
				strchr("GKkLNn",let) &&
				   8*cc->w < 5*c->w &&		// not too wide
				   7*cc->w > 3*cc->h		// proportion: not too thin (old 2>=1)
			  )
			  {
			  accadr1 = cc;
			  return 1;
			  }

	  else if (language==LANG_LITHUANIAN &&
				// Узкие буквы
				(let=='i' || let== 'I' || c->h > 3* c->w) &&
				   cc->w < 3*c->w &&		// not too wide
				   2*cc->w >= cc->h			// proportion: not too thin
			  )
			  {
			  accadr1 = cc;
			  return 1;
			  }

	  else if (language==LANG_TURKISH &&
				// Узкие буквы
				(let=='s') &&
				   2*cc->w < c->w &&	// not too wide
				   2*cc->w >= cc->h		// proportion: not too thin
			  )
			  {
			  accadr1 = cc;
			  return 1;
			  }

	  // Нормальные буквы с хвостиками: AaEeCcNnUu...
	  else if ( 8*cc->w < 5*c->w &&		// not too wide
			    3*cc->w >= 2*cc->h		// proportion: not too thin
			  )
			  {
			  accadr1 = cc;
			  return 1;
			  }

	  }
 }

 return 0;
 }
//**************************************************************************
static INT acc_semicircle(cell *cc,PBYTE r)
{
/* ACC_SEMICIRCLE - акцент в форме полукруга над румынской Аа.

	Функция работает на перевернутом растре и распознает
	полукруг выпуклостью вверх, аналогично acc_roof

	Скопирован из acc_roof().
	04.09.2000 E.P.
*/
 INT r1,r2,r3,b,s11,s12,s21,s22,s31,s32,ss13,ss2,sp,sm,ss,i,j,l;

 if (cc->h>cc->w+2 || cc->w<=5)
  return 0;
 accadr1=cc;
 l=(cc->w+7)/8;
 for (r1=cc->h-1; r1>=0 && !(r[l*r1]&0x80); r1--) ;
 j=cc->w/2;
 b=0x80>>(j%8);
 j=j/8;
 for (r2=cc->h-1; r2>=0 && !(r[l*r2+j]&b); r2--) ;
 j=cc->w-1;
 b=0x80>>(j%8);
 j=j/8;
 for (r3=cc->h-1; r2>=0 && !(r[l*r3+j]&b); r3--) ;

 if (r1>=cc->h-2 && r3>=cc->h-2 && r2<=MIN(cc->h-2,2*(cc->h)/3) &&
     2*r2<r1+r3-2)
  return 1;

 if (r2==cc->h-1)
  {accadr1=NULL; return 0;}

 for (s11=s12=j=0; j<cc->w/3; j++)
  {
  for (i=0; i<cc->h/2; i++)
   if (r[i*l+j/8]&(128>>(j%8)))
    s11++;
  for (i=(cc->h+1)/2; i<cc->h; i++)
   if (r[i*l+j/8]&(128>>(j%8)))
    s12++;
  }
 for (s21=s22=0,j=cc->w/3; j<cc->w-cc->w/3; j++)
  {
  for (i=0; i<cc->h/2; i++)
   if (r[i*l+j/8]&(128>>(j%8)))
    s21++;
  for (i=(cc->h+1)/2; i<cc->h; i++)
   if (r[i*l+j/8]&(128>>(j%8)))
    s22++;
  }
 for (s31=s32=0,j=cc->w-cc->w/3; j<cc->w; j++)
  {
  for (i=0; i<cc->h/2; i++)
   if (r[i*l+j/8]&(128>>(j%8)))
    s31++;
  for (i=(cc->h+1)/2; i<cc->h; i++)
   if (r[i*l+j/8]&(128>>(j%8)))
    s32++;
  }
 ss13=(cc->h/2)*(cc->w/3);
 ss2=(cc->h/2)*(cc->w-2*(cc->w/3));
 sp=s12+s21+s32;
 sm=s11+s22+s31;
 ss=2*ss13+ss2;

 if (
	 (5*s11>3*ss13 || 2*(ss13-s12)>ss13 ||
      5*s31>3*ss13 || 2*(ss13-s32)>ss13 ||
      2*(ss2-s21)>ss2 || 5*sm>4*sp)
	 &&
     (2*sm>sp || 5*sm>2*ss || 5*(ss-sp)>2*ss)
	 )
  {accadr1=NULL; return 0;}

 return 1;
}
//**************************************************************************
// Nick 07.09.00
// how many intersections >= 2 - on height from 0 to mHei
// return < 0 - some error
#define MAXNUMINT 64
int16_t NumIntersect2(c_comp *cmp, int mHei)
{
 int      i,crow;
 lnhead   *line;
 int      len;
 interval *vint;
 int      numint[MAXNUMINT];

 if( !cmp || mHei <= 0 )
     return -1;

 if( mHei > MAXNUMINT )
	 mHei = MAXNUMINT;

 memset(numint,0,mHei*sizeof(int));

 line=(lnhead *)((char *)cmp+cmp->lines+sizeof(INT));

 // test all lines
 for (; (len=line->lth)>0; line=(lnhead *)((char *)line+len))
 {
  // current line
  for (vint=(interval *)((char *)line+sizeof(lnhead)),
	   crow = line->row,i=0; i < line->h; i++, crow++ )
  {
	  if( crow < 0 )
		  return -2;  // invalid c_comp
      if( crow >= mHei )
		  break;
	  numint[crow]++;

	  // vint->l,->e - now ignore
  }
 }

 for( i = crow = 0; crow < mHei; crow++ )
  if( numint[crow] > 1 ) i++;

 return i;
}
//**************************************************************************
static int16_t FindAngles(cell *c, int *lUAngle, int *rUAngle,
						int *lDAngle, int *rDAngle )
{
 c_comp   *cmp=c->env;
 int      i,crow;
 lnhead   *line;
 int      len;
 interval *vint;

 if( !cmp  )
     return -1;

 *lUAngle = *rUAngle = c->h + c->w;
 *lDAngle = *rDAngle = c->h + c->w;

 line=(lnhead *)((char *)cmp+cmp->lines+sizeof(INT));

 // test all lines
 for (; (len=line->lth)>0; line=(lnhead *)((char *)line+len))
 {
  // current line
  for (vint=(interval *)((char *)line+sizeof(lnhead)),
	   crow = line->row,i=0; i < line->h; i++, crow++, vint++ )
  {
	  if( crow < 0 )
		  return -2;  // invalid c_comp

	  *lUAngle=MIN(*lUAngle, crow + vint->e - vint->l);
      *rUAngle=MIN(*rUAngle, crow + c->w - vint->e );

	  *lDAngle=MIN(*lDAngle, c->h-1 - crow + vint->e - vint->l);
      *rDAngle=MIN(*rDAngle, c->h-1 - crow + c->w - vint->e );
  }
 }


 return 1;
}
//**************************************************************************
static int16_t Test2Cell2(cell *c1,cell *c2,
						int *lUAngle, int *rUAngle,
						int *lDAngle, int *rDAngle )
{
 int      i,j,crow;
 lnhead   *line;
 int      len;
 interval *vint;
 c_comp   *cmp1,*cmp2,*cmp;
 int      numint[MAXNUMINT];
 int      startRow,startCol;
 int      minCol,minRow;
 int      mHei;
 int      wid,hei;  // for common box

 cmp1 = c1->env;
 cmp2 = c2->env;

 if( !cmp1 || !cmp2 )
     return -1;

 minRow = MIN(c1->row,c2->row);
 hei = mHei = MAX(c1->h+c1->row-minRow,c2->h+c2->row-minRow);
 minCol = MIN(c1->col,c2->col);
 wid = MAX(c1->w+c1->col-minCol,c2->w+c2->col-minCol);

 if( mHei > MAXNUMINT )
	 mHei = MAXNUMINT;

 if(  mHei <= 0 )
     return -1;

 *lUAngle = *rUAngle = hei + wid;
 *lDAngle = *rDAngle = hei + wid;

 memset(numint,0,mHei*sizeof(int));

 for(j=0, cmp=cmp1;j<2;j++,cmp=cmp2)
 {
  line=(lnhead *)((char *)cmp+cmp->lines+sizeof(INT));

  startRow = (j==0?c1->row:c2->row)-minRow;
  startCol = (j==0?c1->col:c2->col)-minCol;

 // test all lines
  for (; (len=line->lth)>0; line=(lnhead *)((char *)line+len))
  {
  // current line
   for (vint=(interval *)((char *)line+sizeof(lnhead)),
	   crow = line->row,i=0; i < line->h; i++, crow++, vint++ )
   {
	  if( crow < 0 )
		  return -2;  // invalid c_comp

      if( crow + startRow < mHei )
	     numint[crow+startRow]++;

      *lUAngle=MIN(*lUAngle, startRow + crow + startCol + vint->e - vint->l);
      *rUAngle=MIN(*rUAngle, startRow + crow + wid - vint->e - startCol);

	  *lDAngle=MIN(*lDAngle, hei-1 - crow - startRow + startCol + vint->e - vint->l);
      *rDAngle=MIN(*rDAngle, hei-1 - crow - startRow + wid - vint->e -startCol);
   }
  }
 }

 for( i = crow = 0; crow < mHei; crow++ )
  if( numint[crow] > 1 ) i++;

 return (i*100)/mHei;
}
//**************************************************************************
static INT acc_double_right(cell *c,cell *cc,PBYTE r)
{
// ACC_DOUBLE_RIGHT - двойной правый акцент

	// Котович 04.09.2000
 int numInt;
 int leftUAngle,rightUAngle;  // up angles
 int leftDAngle,rightDAngle;  // down angles
 cell *cc1;  // neighbour of cc

 accadr1 = accadr2 = NULL;

 if( c )
 {
	if( cc->h*2 > c->h )
		return 0;

	if( cc->w > c->w )
		return 0;

	if( cc->col < c->col - (cc->w/3) )
		return 0;

	if( cc->row >= c->row )
		return 0;

	if( cc->row + cc->h > c->row +(cc->h/2) )
		return 0;
 }

  // test single component

	numInt = NumIntersect2(cc->env, cc->h);

	if( numInt*2 >= cc->h)
	{

	if( FindAngles(cc,&leftUAngle,&rightUAngle,&leftDAngle,&rightDAngle) < 0 )
		return 0;

	if( leftUAngle   >= rightUAngle*3 &&
		leftDAngle*3 <= rightDAngle
	  )
        {
           accadr1 = cc;
  	       return 1;
        }
	}

 // now test with neighbour
 // left
  cc1 =  cc -> prev;
  if( cc->col>=c->col+c->w/2 &&
	  !(cc1->flg & c_f_fict) &&
      cc1->row<=c->row-cc->h &&
	  cc1->col >= c->col - cc->w/2 &&
	  cc1->col + cc1->w < c->col+c->w &&
	  abs(cc1->h - cc->h) <= cc->h/3 &&
	  abs(cc1->row - cc->row) <= cc->h/3
	)
  {
    numInt = Test2Cell2(cc,cc1,
						&leftUAngle, &rightUAngle,
						&leftDAngle, &rightDAngle );
	if( numInt >= 50 &&
		leftUAngle   >= rightUAngle*3 &&
		leftDAngle*3 <= rightDAngle
	  )
        {
           accadr1 = cc;
		   accadr2 = cc1;
  	       return 1;
        }
  }

  // right
  cc1 =  cc -> next;
  if( cc->col < c->col+c->w/2 &&
	  !(cc1->flg & c_f_fict) &&
      cc1->row<=c->row-cc->h &&
	  cc1->col < c->col + c->w &&
	  cc1->col + cc1->w < c->col+c->w + cc->w/2 &&
	  abs(cc1->h - cc->h) <= cc->h/3 &&
	  abs(cc1->row - cc->row) <= cc->h/3
	)
  {
    numInt = Test2Cell2(cc,cc1,
						&leftUAngle, &rightUAngle,
						&leftDAngle, &rightDAngle );
	if( numInt >= 50 &&
		leftUAngle   >= rightUAngle*2 &&
		leftDAngle*2 <= rightDAngle   &&
        (leftUAngle + rightDAngle ) >= (rightUAngle+leftDAngle)*3
	  )
        {
		  if( snap_activity('e'))
		  {
			  char qq[128];
			  sprintf(qq,"found double right! numI=%d lU=%d rU=%d lD=%d rD=%d",numInt,leftUAngle,rightUAngle,
		              leftDAngle,rightDAngle);
			  snap_show_text(qq);
			  snap_monitor();
		  }

           accadr1 = cc;
		   accadr2 = cc1;
  	       return 1;
        }
  }

  return 0;
}
//**************************************************************************
static INT acc_weak_roof(cell *cc,PBYTE r)
 {
 INT r1,r2,r3,b,j,l;

 if (cc->h>cc->w+2 || cc->w<=5)
  return 0;
 accadr1=cc;
 l=(cc->w+7)/8;
 for (r1=cc->h-1; r1>=0 && !(r[l*r1]&0x80); r1--) ;
 j=cc->w/2;
 b=0x80>>(j%8);
 j=j/8;
 for (r2=cc->h-1; r2>=0 && !(r[l*r2+j]&b); r2--) ;
 j=cc->w-1;
 b=0x80>>(j%8);
 j=j/8;
 for (r3=cc->h-1; r2>=0 && !(r[l*r3+j]&b); r3--) ;
 if (r1>=cc->h-2 && r3>=cc->h-2 && r2<=MIN(cc->h-2,2*(cc->h)/3) &&
     2*r2<r1+r3-2)
  return 1;
 if (r2==cc->h-1)
  {accadr1=NULL; return 0;}

 return 1;
 }
//**************************************************************************/
//Nick 20.08.01
static Bool NotEmptyInside(PBYTE r, INT w, INT h)
{
	int i,j,sum,allSum;
	int wb = (w+7)>>3;
	uchar cc;
	PBYTE rr;

	for(i=0,allSum=0;i<h;i++,r+=wb)
	{
		for(j=0,cc=128,rr=r,sum=0;j<w;j++,cc>>=1)
		{
			if(!cc)
			{
				cc=128;
				rr++;
			}

			if( rr[0] & cc )
			{
				if( sum > 1 )
				{
					sum++;
					break;
				}

				if( sum == 0 )
					sum++;

				continue;
			}

			if( sum == 1 )
				sum++;
		}

		if( sum == 1 || sum == 2)
			allSum++;
	}

	if( allSum*5 >= h*2 )
		return TRUE;

	return FALSE;
}
////////////////
static INT acc_macron(cell *c,PBYTE r)
{
 // Изучение растра акцента типа "макрон" - черта сверху над буквой
	INT i, j;
	INT h, w, l, s, start;

	h=c->h;
	w=c->w;
	l=(w+7)/8;	// Ширина растра в байтах

	accadr1=c;

	// Высота достаточно малая, а форма достаточно вытянутая
	if (h<=3 && w > 2*h)  // Nick 28.08.01.  Old version - >= 3*h
	{
		//Nick 20.08.01
		if( NotEmptyInside( r, w, h) )
		  return 1;
	}

	// Обрить тонкие слои сверху
	for (i=0,start=0; i<c->h/2; i++)
		{
		s=0;
		for (j=0; j<w; j++)
			{
			// Черная точка
			if (r[l*i+j/8]&(128>>(j%8)))
				s++;
			}

		// Какова накопленная ширина среза
		if (s < w/2)
		{
			h--;	// Тонкий слой сверху
			start++;
		}
		else
			break;
		}

	// Обрить тонкие слои снизу
	for (i=c->h-1; i>=c->h/2; i--)	// От исходной высоты
		{
		s=0;
		for (j=0; j<w; j++)
			{
			// Черная точка
			if (r[l*i+j/8]&(128>>(j%8)))
				s++;
			}

		// Какова накопленная ширина среза
		if (s < w/2)
			h--;	// Тонкий слой снизу
		else
			break;
		}

	// Оставшаяся форма достаточно вытянутая
	if (w > 2*h)   // Nick 28.08.01
	{
		//Nick 20.08.01
        if( NotEmptyInside( r+start*l, w, h) )
		   return 1;
	}

	accadr1 = NULL;
	return 0;
}
//**************************************************************************
/*
void debug_print_raster(char *text, PBYTE r)
{
// Отладочная печать растра 19.07.2001 E.P.
char buf[500]={0};
char *p=buf;
long n;

p +=sprintf(p,"%s\n",text);

for(n=0; n< 10; n++)
	p += sprintf(p,"%x ", r[n]);

	snap_show_text(buf);
	snap_monitor();
}
*/
//**************************************************************************
