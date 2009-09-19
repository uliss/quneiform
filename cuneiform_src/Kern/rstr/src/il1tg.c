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
/*         Special cases program                                    */
/*                                                                  */
/********************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "struct.h"
#include "status.h"
#include "cuthdr.h"
#include "func.h"
#include "il1peak.h"
#include "lang.h"
#include "ligas.h"
#include "minmax.h"

#define N_HIST 12                      // number of histograms
#define MAX_NEW_FUNC 9
#define MAX_VERS     16
#define BON 4
#define YES 1
#define NO 0

#define ACCENT_NOT_FOUND    0
#define ACCENT_IS_FOUND     1

#define A_ACCENT            0x01
#define O_ACCENT            0x02

struct statist             // structure for statistics
 {
 uchar top;                 // upper bound of black segments
 uchar bot;                 // lower bound of black segments
 uchar sum;                 // number of black pixels
 uchar res;                 // reserve byte (for speed)
 };


extern uchar fax1x2;
extern int16_t line_number;

int16_t    n_peak[NFUNC];
puchar  peak_vect[NFUNC];

static int16_t total_num_let;
static struct statist *hor;
static struct statist *vert;
static uchar *hist[N_HIST]; // histograms:
			   //   0 - black vertical
			   //   1 - black filled vertical
			   //   2 - white vertical
               //   3 - left vertical ( left abris )
			   //   4 - right vertical
			   //   5 - black horisontal
			   //   6 - black filled horisontal
			   //   7 - white horisontal
			   //   8 - bottom horisontal
			   //   9 - top horisontal
               //  10 - left abris ( as in ABRIS.C )
               //  11 - right abris ( as in ABRIS.C )

static uchar *funcs[MAX_NEW_FUNC];
static int16_t  nfunc=1;
static char recog_res[10];
static char *R;
static int16_t  dx,dy;
static cell * ec_c;
static int16_t  oaprop;
//static int16_t  DOprop;
static int16_t  TYprop;
static int16_t  ecprop;
static int16_t bon_a,mon_c;
static int16_t  gmax_y, gmin_y;
static uchar maxprob;
static char text_string  [256];
static char text_string1 [256];
struct nose_struct
{
    int16_t peak;
    int16_t weight;
    int16_t y;
};

version    oa_src_vers[MAX_VERS];
uchar       oa_src_nvers;
int16_t        oa_accent_existing_flag;

static void oacell                  (cell *);
static void eccell                  (cell *);
static void NHcell                  (cell *);
static void NHdiff                  (cell *);
static void TYcell                  (cell *);
static void GCcell                  (cell *);
static void set_mem                 (void);
static int16_t  check_two_case          (cell *, pchar);
static void comptorast              (puchar, cell *);
static void make_hor_vert           (cell *);
static void make_hist               (void);
static void make_func               (void);
static int16_t  line_width              (puchar, int16_t, int16_t);
static uint16_t integral                (puchar, int16_t, int16_t, int16_t);
static int16_t  gmax                    (puchar, int16_t, int16_t);
static int16_t  gmin                    (puchar, int16_t, int16_t);
static int16_t  vertsym                 (int16_t);
static int16_t  centrsym                (int16_t);
static Bool increase                (puchar, int16_t, int16_t);
static Bool decrease                (puchar, int16_t, int16_t);
static int16_t  difference              (puchar, int16_t, int16_t);
static int16_t  valley                  (puchar, int16_t, int16_t, int16_t);
static Bool fill                    (puchar, int16_t, int16_t, int16_t);
static void oarecog                 (cell *);
static void TYrecog                 (void);
//static void put_two_case            (cell *, pchar);
static void put_oa_case             (cell *);
static void put_ec_case             (cell *);
static void make_hor_vert_ec        (void);
static void make_hist_ec            (void);
static void make_func_ec            (void);
static Bool not_AvanGard_a          (void);
static Bool middle_long_sharp_peak  (int16_t,   int16_t, int16_t);
static Bool pure_concave            (puchar, int16_t, int16_t, int16_t);
static Bool hole_in_func            (puchar, int16_t, int16_t, int16_t);
// static Bool hill_in_func            (puchar, int16_t, int16_t, int16_t);
static Bool flat_func               (puchar, int16_t, int16_t, int16_t);
static void ecrecog                 (void);
static uchar filled_head             (void);
#define FH_Y_FILLED                 1
#define FH_N_FILLED                 2
#define FH_NO_IDEA                  0
static void cell_versions_to_text   (cell *);
static Bool not_letter              (cell *);

static int16_t  oa_accent_removing      ( cell *srC );
static void oa_accent_restoring     ( cell *resC );


void il1 ()
{
    cell *C;

    Z=&string;

    set_mem ();
    snap_newpass ('c');
    C = cell_f ();
    total_num_let = 0;
loop:;
    C = C -> next;
    if (C -> next != NULL)
    {
    if (not_letter(C)) goto loop;
        total_num_let++;
        oacell  (C);
        eccell  (C);
        NHcell  (C);
        TYcell  (C);
        GCcell  (C);
   if( language == LANG_RUSSIAN && langUkr ){
      int16_t  i;
      for(i=0;i<C->nvers;i++)
        if( C->vers[i].let == liga_i )
          C->vers[i].let = 'i';
   }
   if( language == LANG_RUSSIAN && langSer ){
      int16_t  i;
      for(i=0;i<C->nvers;i++){
        if( C->vers[i].let == liga_j )
          C->vers[i].let = 'j';
        if( C->vers[i].let == 'j' )
          C->vers[i].let = SERB_j;
        if( C->vers[i].let == 'J' )
          C->vers[i].let = SERB_J;
      }
   }
        goto    loop;
    }
}

static void oacell (cell *C)
{
int16_t was_a = (let_sans_acc[C->vers[0].let] == 'a')?1:0;
	int16_t res_o = 0;

    oa_accent_existing_flag = ACCENT_NOT_FOUND;
    if ( language != LANG_ENGLISH )
        oa_accent_existing_flag = oa_accent_removing( C );
    if (check_two_case (C, "oa") && tsimple(C))
    {
        snap_newcell (C);
        if (snap_activity ('c'))
        {
            snap_show_text ("Before");
            snap_monitor   ();
	    cell_versions_to_text (C);
        }

        dx = C -> w;
        dy = C -> h;
        make_hor_vert (C);
        make_hist     ();
        make_func     ();
	oarecog       (C);
	put_oa_case   (C);

        if (snap_activity ('c'))
        {
            sprintf         (text_string1, "\nAfter: nprop = %d", oaprop);
            strcat          (text_string, text_string1);
            snap_show_text  (text_string);
            snap_monitor    ();
        }
    }
    else
    {
	  if ( oa_accent_existing_flag == ACCENT_IS_FOUND )
	  oa_accent_restoring( C );
    }

	// Распознавание эстонской тильды после замены версии a->o.
	res_o = (let_sans_acc[C->vers[0].let] == 'o')? 1:0;

	if(language == LANG_ESTONIAN && was_a && res_o)
		{
		C->vers[0].let = 'o';
		C->accent_leader = 0;
		sort_vers(C);
		accent(C);
        if (C->accent_leader && snap_activity ('c'))
        {
            sprintf         (text_string1, "\nAccent");
            strcat          (text_string, text_string1);
            snap_show_text  (text_string);
            snap_monitor    ();
        }
		}
}

static int16_t oa_accent_removing( cell *srC )
{
int16_t        new_nvers, i, ret_code;
uchar       flag;
version    subst_vers[MAX_VERS];
version    *wpv;

        ret_code = ACCENT_NOT_FOUND;
        flag = 0;
        wpv = subst_vers;
        new_nvers = srC->nvers;
        for ( i = 0; i < srC->nvers; i++ )
        {
            switch  ( srC->vers[i].let )
            {
                case    a_left_accent :
					// Конфликтный код 07.09.2000 E.P.
					if (is_cen_language(language) ||
						is_baltic_language(language) // 17.07.2001 E.P.
					   )
						goto default_case;

                case    a_right_accent:
                case    a_roof_accent :
					// Конфликтные коды 07.09.2000 E.P.
					if (is_baltic_language(language)) // 17.07.2001 E.P.
						goto default_case;

                case    a_2dot_accent :
                {
                    if  ( (flag & A_ACCENT) == 0 )
                    {
                        wpv->let = 'a';
                        wpv->prob = srC->vers[i].prob;
                        wpv++;
                        flag |= A_ACCENT;
                    }
                    else
                        new_nvers --;
                    break;
                }
                case    o_left_accent :
					// Конфликтный код 07.09.2000 E.P.
					if (is_cen_language(language) ||
						is_baltic_language(language)
					   )
						goto default_case;

                case    o_right_accent:
                case    o_roof_accent :
                case    o_2dot_accent :
                {
                    if  ( (flag & O_ACCENT) == 0 )
                    {
                        wpv->let = 'o';
                        wpv->prob = srC->vers[i].prob;
                        wpv++;
                        flag |= O_ACCENT;
                    }
                    else
                        new_nvers --;
                    break;
                }
                default:
default_case:
                        wpv->let = srC->vers[i].let;
                        wpv->prob = srC->vers[i].prob;
                        wpv++;
            }
        }
        if ( flag == (A_ACCENT + O_ACCENT) )
        {
            oa_src_nvers = (uchar)srC->nvers;
            srC->nvers = new_nvers;
            memcpy( oa_src_vers, srC->vers, sizeof( srC->vers ) );
            memset( wpv, 0, sizeof( version ) );   // end of versions list
            memcpy( srC->vers, subst_vers, sizeof( subst_vers ) );
            ret_code = ACCENT_IS_FOUND;
        }
        return  ret_code;
}


static void eccell (cell *C)
 {
    if (check_two_case (C, "ec") && tsimple(C))
    {
        snap_newcell (C);
        if (snap_activity ('c'))
        {
            snap_show_text ("Before:");
            snap_monitor   ();
	    cell_versions_to_text (C);
        }
        ec_c = C;
        dx = C -> w;
        dy = C -> h;
        comptorast       (R, C);
        make_hor_vert_ec ();
        make_hist_ec     ();
        make_func_ec     ();
        ecrecog();
        put_ec_case     (C);
        if (snap_activity('c'))
        {
            sprintf         (text_string1, "\nAfter: nprop = %d", ecprop);
            strcat          (text_string, text_string1);
            snap_show_text  (text_string);
            snap_monitor    ();
        }
    }
 }

static char vrot[] = {0, 0, 0, 0, 1, 2, 2, 3, 3};
static char vrot_small[] =
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 3, 3};
//  0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17
static void NHcell (cell *C)
{
    int16_t     i, rot;
    int16_t     col,row,h,w;
    extern  servBOX SBOX;
    s_glue  GL;
    uchar    sv [sizeof ( C-> nvers) + sizeof (C -> vers)];
    cell    c, *p, *pl, *n, *nl;

    if (check_two_case (C, "NH") && tsimple(C))
    {
        snap_newcell (C);
        if (snap_activity ('c'))
        {
            snap_show_text ("Before");
            snap_monitor ();
	    cell_versions_to_text (C);
        }
        col = C -> col;
        row = C -> row;
        h   = C -> h;  dy = h;
        w   = C -> w;  dx = w;
        GL.celist   [0] = C;
        GL.celist   [1] = NULL;
        GL.complist [0] = C -> env;
        GL.complist [1] = NULL;
        GL.ncell        = 1;
        make_broken_raster (&GL, 1);
        if ( h <= 21 )
          rot = MIN ( vrot_small [SBOX.dens / 2], 8 );
        else
          rot = MIN ( vrot [SBOX.dens / 4], 17 );  // the treshold of the density
        if (!rot)
          {
             NHdiff( C );              // differ N and H by usual way
             goto NHRet;
          }
        // the density is large enough - use sheving
        memcpy (&c, C, sizeof (c));
 // !!!!! boldshave deletes the cell and creates it;
 // next/prev/nextl/prevl may be redefined;
	for (i = 0; i < rot; i++)
          boldshave (C, 2);           // shaving (2d method) with recognition
        // AL 940324
        GL.celist   [0] = C;
        GL.celist   [1] = NULL;
        GL.complist [0] = C -> env;
        GL.complist [1] = NULL;
        GL.ncell        = 1;
        make_broken_raster (&GL, 1);
        dmiBOX (C, &GL, 0);
        memcpy (sv, &(C -> nvers), sizeof (C -> nvers) + sizeof (C -> vers));
        p=C->prev; pl = C->prevl; n=C->next; nl=C->nextl;
        memcpy (C, &c, sizeof (c));
        C->prev=p; C->prevl=pl; C->next=n; C->nextl=nl;
        if (C->flg & (c_f_bad | c_f_let))
          adjust_links ( C );
        memcpy (&(C -> nvers), sv, sizeof (C -> nvers) + sizeof(C -> vers));
NHRet:
        if (snap_activity ('c'))
        {
            snap_show_text (text_string);
            snap_monitor   ();
        }
    }
}

#define MON_NH         -10

static void NHdiff( cell * C )
{

int16_t i1, i2, i3, i4, pnlH, pnlN, midw;

   pnlH = 0;
   pnlN = 0;
   make_hor_vert( C );
   make_hist();
   make_func();

   midw = ( ( C -> w ) >> 1 );       // middle of the top/bottom box side
   i1 =  midw - 1;
   i2 = i1 - 1;
   i3 = midw + 1;
   i4 = i3 + 1;

// test top abris ( hist[9] ) against monotoneus decreasing at left near middle
   if ( ( hist[9][i2] < hist[9][i1] ) && ( hist[9][i1] < hist[9][midw] ) )
     pnlH += MON_NH;         // decreasing top abris votes contra 'H'
   if ( abs( hist[9][i1] - hist[9][i4] ) <= 2 )
     pnlN += MON_NH;         // flat top abris votes contra 'N'

// test bottom abris ( hist[8] ) against monotoneus decreasing
//  at right near middle
   if ( ( hist[8][midw] < hist[8][i3] ) && ( hist[8][i3] < hist[8][i4] ) )
     pnlH += MON_NH;         // decreasing bottom abris votes contra 'H'
   if ( abs( hist[8][i1] - hist[8][i4] ) <= 2 )
     pnlN += MON_NH;         // flat bottom abris votes contra 'N'

   if ( pnlH )
     cell_bonus_let ( C, 'H', pnlH );
   if ( pnlN )
     cell_bonus_let ( C, 'N', pnlN );
   sort_vers( C );
}

#define PNL_G_EMPTY_MID 16
static void GCcell( cell * C )
{
extern  servBOX SBOX;
s_glue  GL;
uint16_t wup, wbot;
char wc;
int16_t pnl, wd, op;

    pnl = 0;  op = 0;
    if (check_two_case (C, "GC") && tsimple(C))
    {
        snap_newcell (C);
        if (snap_activity ('c'))
        {
            snap_show_text ("Before");
            snap_monitor   ();
            cell_versions_to_text (C);
        }

     // prepare GL-structure to call the function "make_broken_raster"
        GL.celist   [0] = C;
        GL.celist   [1] = NULL;
        GL.complist [0] = C -> env;
        GL.complist [1] = NULL;
        GL.ncell        = 1;
        make_broken_raster (&GL, 1);

        if ( abs (SBOX.matrBOX.vect[4] - SBOX.matrBOX.vect[10]) < SBOX.dens )
         {
          if (  // check the fillness of the middle of the box
               SBOX.matrBOX.vect[4] + SBOX.matrBOX.vect[7] +
               SBOX.matrBOX.vect[8] + SBOX.matrBOX.vect[10] < SBOX.dens )
           {
             pnl += PNL_G_EMPTY_MID;  // empty middle of the
                                      // box votes contra 'G'
             wc = 'G';
             op = 3;
             goto CGR_ApplayPenalty;
           }
          else     goto CGR_Ret;    // no decision - nothing to change
         }
        wbot = SBOX.matrBOX.vect[10] + SBOX.matrBOX.vect[11] +
               SBOX.matrBOX.vect[13] + SBOX.matrBOX.vect[14];
        wup  = SBOX.matrBOX.vect[1] + SBOX.matrBOX.vect[2] +
               SBOX.matrBOX.vect[4] + SBOX.matrBOX.vect[5];
        pnl  = wbot - wup;
        wd   = pnl;
        pnl /= SBOX.dens;
        if ( pnl > 0 )     // Havy bottom votes contra "C"
         {
          wc = 'C';
          op = 1;
         }
        if ( pnl <= 0 )   // Simmetrical case votes contra "G"
         {
          pnl = -pnl;
          wc = 'G';
          op = 2;
         }
CGR_ApplayPenalty:
        pnl &= 0xfffe;                   // penalty has to be even
        if ( pnl > 250 ) pnl = 250;      // "normalize" penalty
        cell_bonus_let ( C, wc, (int16_t)(-pnl) );  // apply penalty
        sort_vers( C );
CGR_Ret:
      if (snap_activity('c'))
        {
            sprintf         (text_string1, "\nAfter: delta = %d op = %d", wd, op);
            strcat          (text_string, text_string1);
            snap_show_text  (text_string);
            snap_monitor    ();
        }
    }
}

#define MON_T 12
#define MON_Y 22
#define BON_TY 6
static void TYcell (cell *C)
{
int16_t mon_T = 0;
int16_t mon_Y = 0;
    if (check_two_case (C, "TY") && tsimple(C))
    {
        snap_newcell (C);
        if (snap_activity ('c'))
        {
            snap_show_text ("Before");
            snap_monitor   ();
	    cell_versions_to_text (C);
        }

        dx = C -> w;
        dy = C -> h;
        make_hor_vert   (C);
        make_hist       ();
        make_func       ();
        TYrecog         ();           // generate TYprop

//  Decision making
        if ( TYprop & 1 )           // flat upper abris
         { mon_T += BON_TY; mon_Y -= MON_Y; }
        mon_T -= ( ((TYprop & 2)>>1) + ((TYprop & 4)>>2) ) * MON_T;
        mon_Y -= ( ((TYprop & 8)>>3) + ((TYprop & 16)>>4) ) * MON_Y;
        if ( mon_T )
          cell_bonus_let( C, 'T', mon_T );
        if ( mon_Y )
          cell_bonus_let( C, 'Y', mon_Y );
        sort_vers( C );

        if (snap_activity ('c'))
        {
            sprintf         (text_string1, "\nAfter: nprop = %d", TYprop);
            strcat          (text_string, text_string1);
            snap_show_text  (text_string);
            snap_monitor    ();
        }
    }
}

static void TYrecog ()
//
//	This procedure recognizes given raster with assumption
//  that it represents T or Y.
//
{
 int16_t from, to, shift, jmp;
 Bool rc;


   TYprop  = 0; from = 1; to = dx - 1; jmp = 1;

// test the top abris ( hist[9] ) against flatness

    if ( dx < 5 )  goto TY_noshift;
    if ( dy < 20 ) goto TY_noshift;
    jmp   = MAX( dy / 20, 1 );   // set admissible jump of top abris
    shift = dx >> 2;             // set shift from any side 1/4 of width
    from  = shift;
    to    = dx - shift;
TY_noshift:
    rc = hole_in_func( hist[9], from, to, jmp );
    if ( ! rc )
      TYprop |= 1;

// test left abris ( hist [3] ) from uper 1/4 to middle of the height
// against monotonious increasing
    from = dy >> 1;
    to   = dy - (dy >> 2);
    rc = increase( hist[3], from, to );
    if ( rc )
      TYprop |= 2;

// test right abris ( hist [4] ) from uper 1/4 to middle of the height
// against monotonious increasing
    from = dy >> 1;
    to   = dy - (dy >> 2);
    rc = increase( hist[4], from, to );
    if ( rc )
      TYprop |= 4;

// test left abris ( hist [3] ) from bottom 1/4 to uper 1/4 of the height
// against flatness
    from = dy >> 2;
    to   = dy - (dy >> 2);
    rc = flat_func( hist[3], from, to, jmp );
    if ( rc )
      TYprop |= 8;

// test right abris ( hist [4] ) from bottom 1/4 to uper 1/4 of the height
// against flatness
    from = dy >> 2;
    to   = dy - (dy >> 2);
    rc = flat_func( hist[4], from, to, jmp );
    if ( rc )
      TYprop |= 16;

}

static void set_mem ()
{
    int16_t   i;
    pchar c;

    R    = t_raster ();
    hor  = (struct statist *) il1_pool_ptr ();
    vert = hor + MAX_HOR_IL1;
    c    = (pchar) (vert + MAX_VERT_IL1);
    for (i = 0; i < N_HIST; i++, c += MAX_HOR_IL1)          hist      [i] = c;
    for (i = 0; i < MAX_NEW_FUNC; i++, c += MAX_HOR_IL1)    funcs     [i] = c;
    for (i = 0; i < NFUNC; i++, c += MAX_HOR_IL1 / 4)       peak_vect [i] = c;
}

static int16_t check_two_case (cell *C, pchar c)
{
    int16_t     i, n;
    version *v;
#define TWO_CASE_THRESH 140

    maxprob = 0;
    if (!(C -> flg & (c_f_let | c_f_bad)))      return 0;
    if (!C -> nvers)                            return 0;
    v = C->vers;
    n = 0;
    for (i = 0; i < C -> nvers; i++, v++)
    if ((v -> prob > TWO_CASE_THRESH) && strchr (c, v -> let))
    {
        n++;
        if (v -> prob > maxprob)    maxprob=v->prob;
    }
    if (n == 2)                                 return 1;
    else                                        return 0;
}

#include "il1tgh.c"

static Bool flat_func (puchar func, int16_t from, int16_t to, int16_t jump)
{
    int16_t i;

    if ( to - from == 1 )
      if ( abs ( func[from] - func[to] ) > jump )        return FALSE;
      else                                              return TRUE;
    if ( to - from == 2 )
     {
       if ( abs ( func[from] - func[to] ) > jump )        return FALSE;
       if ( abs ( func[from] - func[from+1] ) > jump )    return FALSE;
       if ( abs ( func[from+1] - func[to] ) > jump )      return FALSE;
       return TRUE;
     }
    for ( i = from; i < to - 1; i++ )
     {
       if ( abs ( func[i] - func[i+1] ) > jump )     return FALSE;
       if ( abs ( func[i] - func[i+2] ) > jump )     return FALSE;
       if ( abs ( func[i+1] - func[i+2] ) > jump )   return FALSE;
     }
    return TRUE;
}

static int16_t difference (puchar func, int16_t from, int16_t to)
{
    int16_t m1,m2;

    m1=gmin(func,from,to);
    m2=gmax(func,from,to);

    return m2 - m1;
}

static int16_t valley (puchar func, int16_t from, int16_t to, int16_t percent)
//
//	This procedure checks difference between minimum and maximum
//	of function in given region.
//
{
    int16_t lw1, s;

    lw1 = line_width (func, from, to);
    s   = integral (func, from, to, lw1);
    if (s < (to + 1 - from) * percent / 100)    return 0;
                                                return 1;
}

static Bool fill (puchar func, int16_t from, int16_t to, int16_t percent)
//
//	This procedure checks whether given region is close to constant.
//
{
    int16_t n1, n2, s;

    n1 = gmax (func, from, to);
    n2 = gmin (func, from, to);
    s  = integral (func, from, to, n2);
    if (s >= (to + 1 - from) * (n1 - n2) * percent / 100)   return TRUE;
    else                                                    return FALSE;
}

static void oarecog(cell *C)
//
//	This procedure recognizes given raster with assumption
//	that it represents o and a (Avant-Garde).
//
{
    int16_t d;
    strcpy (recog_res, "");
    memset (peak_vect [0], 0, NFUNC * MAX_HOR_IL1 / 4);
    memset (n_peak, 0, sizeof (n_peak));
    oaprop  = 0;

    if (!((C->font|C->font_new)&c_fp_it) && not_AvanGard_a())
     {strcat(recog_res,"a"); goto fin;}

    if (vertsym (1))
    {
        strcat (recog_res, "o");
        oaprop = 1;
        goto fin;
    }
    d = 1;
    if ( fax1x2 ) d = 0;
    if (difference (hist [4], (int16_t)(dy * 1 / 10), (int16_t)(dy * 9/10)) <= d)
    {
        strcat (recog_res, "a");
        oaprop = 2;
        goto fin;
    }

/*---------	Additional check for italic 'a'		-----------*/
    d=2;    if (dy >= 32) d=3;
	    else if (dy <=16) d=1;
    if (hist[4][dy-d-1] - hist[4][d] >=2 && increase (hist[4],d,(int16_t)(dy-d-1)))
    {
	strcat (recog_res, "a");
	oaprop=9;
	if ((C->cg_flag & c_cg_cutr) == 0)
		if (hist[4][dy-d] >= hist[4][dy-d+1])		goto fin;
    }
/*---------------*/
    if (valley (funcs [2], (int16_t)(dy * 6 / 100), (int16_t)(dy * 94 / 100), 100))
    {
        strcat (recog_res, "o");
        oaprop = 3;
        goto fin;
    }
    if (!fill (funcs [2], (int16_t)(dy * 6 / 100), (int16_t)(dy * 3 / 10), 45))
    // funcs[2] - right abris (the distances from right side to first black point)
    // hist[4] - other right abris (the distances from left side to last black point)
    {
	if (!increase (hist [4], (int16_t)(dy * 1 / 10), (int16_t)(dy * 9 / 10)))
	{
	     strcat (recog_res, "o");
	     oaprop = 4;
	     goto fin;
	}
    }
    if (centrsym (1))
    {
        strcat (recog_res, "o");
        oaprop = 5;
        goto fin;
    }
/*  this check is wrong or useless - L.D. 09/28/92
     (except I9/20/17 and I7/19/7);
    if (difference (funcs [3], dy * 1 / 10, dy * 9 / 10) > 2)
    {
        strcat (recog_res, "a");
        oaprop = 6;
     printf("%3d |%3d   \n",line_number+1, total_num_let);
        goto fin;
    }
*/
fin:;
    bon_a = 0;
    if (strlen (recog_res) > 0 && !strchr (recog_res, 'a'))
    {
        int16_t lw, i1, i2;

        lw = line_width (hist [0], (int16_t)(dy * 2 / 10), (int16_t)(dy * 8 / 10));
        i1 = integral (hist [5], 0, lw, 0);
        i2 = integral (hist [5], (int16_t)(dx - 1 - lw), (int16_t)(dx - 1), 0);
        if (i1 && i2 * 10 / i1 > 12)
        {
            strcat (recog_res, "a");
	    bon_a = 6;
        }

	else
	{
	 i1 = 1; i2 = dy - 2;
	 if ( dy < 14 ) { i1 = 0; i2 = dy - 1; }  // small point size
	 if (gmax (hist [4], i1, i2) - gmin (hist [4], i1, i2) <= 1)
	  {
            strcat (recog_res, "a");
	    bon_a = 8;
	  }
	}
    }
}

static Bool not_AvanGard_a()
 {
 int16_t m;

 m=gmin(hist[3],(int16_t)(dy/3),(int16_t)(dy*2/3));
 if (gmax(hist[3],0,(int16_t)(dy/3))-m>=2 && gmax(hist[3],(int16_t)(dy*2/3),(int16_t)(dy-1))-m>=1)
  return TRUE;
 return FALSE;
 }

/***********
static void put_two_case(cell *C, pchar pair)
{
    int16_t     i, m, n;
    version *v, *v1;
    version vers [16];
    pchar   c;

    n = strlen (recog_res);
    if (!n) goto end;             // no ideas - left old versions
    if (n == 2) goto end;
    v = vers;
    c = recog_res;
    for (i = 0; i < n; i ++, v ++, c ++)
    {
        v -> let  = *c;
        v -> prob = maxprob;
    }
    v1 = C -> vers;
    for (i = 0; i < C -> nvers; i ++, v1 ++)
    {
        if (strchr (pair, v1 -> let))               continue;
        v -> let  = v1 -> let;
        v -> prob = v1 -> prob;
        if ( (v1 -> let == 'Q') && strchr (pair, 'O'))
         {
          if (strchr (recog_res, 'D'))
            {
              // if 'O' is worse than 'D', then 'Q' has to be made less too
              v -> prob -= ( DOprop << 2 ) ;
              if ( v -> prob <= 0 )   v -> prob = 2;
            }
         }
        if ( (v1 -> let == '0') && strchr (pair, 'O'))
        {
            if (strchr (recog_res, 'O'))    v -> prob = maxprob;
            else                                    continue;
        }
        v++;
    }
    v -> let   = 0;                         // end flag for versions in cell
    m          = v - vers;
    m          = MIN (m, 8);
    C -> nvers = m;
    memcpy (C -> vers, vers, MIN (m + 1, 8) * sizeof (*v));
    C -> flg   = c_f_let;
    sort_vers (C);
end:;
}
**************/

static void make_hor_vert_ec ()
{
    int16_t  i, x, y, k;
    uchar *c;
    uchar b;
    struct statist *h;
    int16_t  bnd;

    for (x = 0, h = hor; x < ((dx + 7) / 8) * 8; x ++, h ++)    h -> sum = 0;
    for (x = 0, h = vert; x < dy; x ++, h ++)                   h -> sum = 0;
    c = R;
                        // projection on x
    for (y = dy - 1; y >= dy * 33 / 100; y --)
    {
        for (x = 0, h = hor; x < (dx + 7) / 8; x ++, c ++)
        {
            for (i = 0, b = 0x80; i < 8; i ++, b >>= 1, h ++)
            {
                if ((*c & b) != 0)
                {
                    if (h -> sum == 0)      h -> top = h -> bot = (uchar)y;
                    else if (h -> bot > y)  h -> bot            = (uchar)y;
                    h -> sum ++;
                }
            }
        }
    }
                        // projection on y
    c   = R;
    bnd = dx * 55 / 100;
    for (y = 0, h = vert + dy - 1; y < dy; y ++, h --)
    {
        for (x = 0, k = 0; x < (dx + 7) / 8; x ++, c ++)
        {
            if (x * 8 > bnd)                    continue;
            for (i = 0, b = 0x80; i < 8; i ++, b >>= 1, k ++)
            {
                if (k > bnd)                    continue;
                if ((*c & b) != 0)
                {
                    if (h -> sum == 0)  h -> top = h -> bot = (uchar)k;
                    else                h -> top            = (uchar)k;
                    h -> sum ++;
                }
            }
        }
    }
}

static void make_hist_ec ()
{
    int16_t i;

    for (i = 0; i < N_HIST; i ++)
    memset (hist [i], 0, MAX_HOR_IL1);

    /* all histogramms on horisontal scanes are built assuming */
    /*  that box is cut at the left by 1/2 of width            */
    for (i = 0; i < dy; i ++)
    {
        hist [0][i] = vert [i]. sum;
        hist [1][i] = vert [i]. top - vert [i]. bot + 1;
        hist [2][i] = vert [i]. top - vert [i]. bot + 1 - vert [i]. sum;
        hist [3][i] = dx * 55 / 100 - vert [i]. bot;  /* left abris */
        hist [4][i] = vert [i]. top + 1;        /* right abris */
    }

    /* all histogramms on vertical scanes are built assuming */
    /*  that box is cut at the bottom by 1/3 of height       */
    for (i = 0; i < dx; i ++)
    {
        /* hist[5] - the number of black pixels in the vertical scanes */
        hist [5][i] = hor [i]. sum;

        /* hist[6] - the distances between highest and lowest   */
        /*             black pixels in vertical scanes          */
        hist [6][i] = hor [i]. top - hor [i]. bot + 1;

        /* hist[7] - the number of white pixels between highest and */
        /*           lowest black pixels in vertical scanes         */
        hist [7][i] = hor [i]. top - hor [i]. bot + 1 - hor[i]. sum;

        /* hist[8] - bottom abris */
        hist [8][i] = dy - hor [i]. bot;

        /* hist[9] - top abris */
        hist [9][i] = hor [i]. top + 1;
    }
}

static void make_func_ec ()
{
    int16_t i;

    for (i = 0; i < MAX_NEW_FUNC; i ++)
    memset (funcs [i], 0, MAX_HOR_IL1);
    nfunc = 1;
    for (i = 0; i < dy; i ++)
    funcs [nfunc][i] = hist [0][i];
    nfunc ++;
    for (i = 0; i < dx; i ++)
    funcs [nfunc][i] = dy - hist [8][i];
    nfunc ++;
    for (i = 0; i < dy; i ++)
    funcs [nfunc][i] = hist [4][i];
    nfunc ++;
    nfunc = MIN (nfunc, MAX_NEW_FUNC);
}

static Bool middle_long_sharp_peak (int16_t nf, int16_t from, int16_t to)
{
    int16_t i, i1;

    extremum (funcs [nf], nf, from, to,
                line_width (funcs [nf], from, to), YES, YES);
    i1 = 0;
    for (i = 0; i < n_peak [nf]; i ++)
    if ((peak_vect [nf][i] & MIDDLE_PEAK) &&
        (peak_vect [nf][i] & LONG_PEAK) &&
        (peak_vect [nf][i] & SHARP_PEAK))
    i1 ++;
    if (i1)                                     return TRUE;
                                                return FALSE;
}

static Bool pure_concave (puchar f, int16_t from, int16_t to, int16_t jmp)
{
    int16_t i, j, fl;

    fl = 0;
    for (i = from; i < to; i ++)
    {
	if ( ( f [i+1] - f [i] ) > jmp )                   break;
    }
    for (j = to; j > from; j --)
    {
    if (  f[j - 1] - f[j] > jmp )
      fl += 1;

     if ( j >= 2 )
      if ( f[j-2] - f[j] > jmp )
        fl += 1;

     if ( fl )  break;
    }
    if (j <= i)                                 return TRUE;
                                                return FALSE;
}

static Bool hole_in_func (puchar f, int16_t from, int16_t to, int16_t jmp)
{
    int16_t  min;
    min = gmin ( f, from, to );
    if ( ( f[to] - min > jmp ) && ( f[from] - min > jmp ) )
      return TRUE;
    else
      return FALSE;
}

/*
static Bool hill_in_func (puchar f, int16_t from, int16_t to, int16_t jmp)
{
    int16_t  max;
    max = gmax ( f, from, to );
    if ( ( max - f[to] > jmp ) && ( max - f[from]  > jmp ) )
      return TRUE;
    else
      return FALSE;
}
*/

#define PNL_C   80
static void ecrecog ()
{
    int16_t n1, n2, n3, jump;

    strcpy (recog_res, "");
    memset (peak_vect [0], 0, NFUNC * MAX_HOR_IL1 / 4);
    memset (n_peak, 0, sizeof (n_peak));

    ecprop  = 0;
    mon_c = 0;

    if (middle_long_sharp_peak (1, 0, (int16_t)(dy - 1)))
    {
        strcat (recog_res, "e");
        ecprop = 1;
        goto fin;
    }

    jump = MAX ( dy / 10, 1 ) - 1;
    if (pure_concave (funcs [3], (int16_t)(dy * 2 / 10), (int16_t)(dy * 8 / 10), jump))
    {
      for ( n3 = 0, n2 = 0, n1 = (dx>>1); n1 < dx-2; n1++ )
      {
          if ( hist[7][n1] > 0 )
          {
            n2 ++;
            if ( n3 == 0 )
                n3 = n1;
          }
      }
      switch( filled_head() )
      {
        case    FH_N_FILLED:
        {
            if ( ( n2 > 1 ) && ( abs( funcs[2][n3] - (dy>>1) ) <= 2 ) )
            {
                strcat (recog_res, "ce");
                mon_c -= 20;
                ecprop = 20;
                goto fin;
            }
            strcat (recog_res, "c");
            ecprop = 2;
            goto fin;
        }
        case    FH_Y_FILLED:
        {
                strcat (recog_res, "e");
                ecprop = 22;
                goto fin;
        }
        case    FH_NO_IDEA:
        {
            strcat (recog_res, "ce");
            ecprop = 222;
            goto fin;
        }
      }
    }

    n1 = gmax (funcs [3], (int16_t)(dy * 33 / 100), (int16_t)(dy * 67 / 100));
    n2 = line_width (funcs [3], (int16_t)(dy * 2 / 10), (int16_t)(dy * 8 / 10));
    /** funcs[3] - right abris of the left half of rastre, i.e.
                   the distanse from last right black point to
                   the bound, staying at (dx/2 + 1) from left side. **/
    /** funcs[2] - bottom abris of the top 3/4 rastre **/
    /** funcs[1] - total amount of the black points in the left half of rastre **/
    if (n1 == n2 )
    {
        n1 = gmin( funcs [2], (int16_t)(dx-dx/4), (int16_t)(dx-1) );
        if ( ( n2 < dx/2 ) && ( n1 > (dy/2) ) &&
             ( funcs [1][n1] < gmax( funcs[1], (int16_t)(dy * 33/100), (int16_t)(dy * 67/100) ) ) )
        {
            strcat (recog_res, "c");
            ecprop = 333;
     /* printf("'c': %3d |%3d   \n",line_number+1, total_num_let);*/
        }
        else
        {
            strcat (recog_res, "e");
            ecprop = 33;
     /* printf("'e': %3d |%3d   \n",line_number+1, total_num_let);    */
        }
        goto fin;
    }
    if (n1 - n2 < 0)
    {
        strcat (recog_res, "e");
        ecprop = 3;
  /*   printf("'c': %3d |%3d   \n",line_number+1, total_num_let); */
        goto fin;
    }

    if (n1 == n2 + 1)
    {
        if (n2 > 10)
        {
            strcat (recog_res, "c");
            ecprop = 4;
        }
        else
        {
          if (n2 >= 5)
            strcat (recog_res, "ce");
          mon_c -= ( PNL_C * (jump + 1) ) / n2;
          ecprop = 44;
        }
        goto fin;
    }

    strcat (recog_res, "e");
    ecprop = 5;
    goto fin;
fin:;
}

#define TRESH_MATR 100
static uchar filled_head()
{
extern  servBOX SBOX;
s_glue  GL;
uint16_t wup, wbot;

     // prepare GL-structure to call the function "make_broken_raster"
        GL.celist   [0] = ec_c;
        GL.celist   [1] = NULL;
        GL.complist [0] = ec_c -> env;
        GL.complist [1] = NULL;
        GL.ncell        = 1;
        make_broken_raster (&GL, 1);

        if ( ( SBOX.matrBOX.vect[4] < SBOX.matrBOX.vect[0] ) ||
             ( SBOX.matrBOX.vect[4] < SBOX.matrBOX.vect[2] ) )
         return FH_N_FILLED;

        if ( SBOX.matrBOX.vect[7] > SBOX.matrBOX.vect[10] )
         return FH_Y_FILLED;


          wup  = SBOX.matrBOX.vect[3] + SBOX.matrBOX.vect[4];
          wbot = SBOX.matrBOX.vect[9] + SBOX.matrBOX.vect[10];
          if ( wup - wbot > SBOX.dens )
           {
             if ( SBOX.dens <= 21 )
              return FH_Y_FILLED;
             {
               uchar i;
               for ( wbot = 32657, i = 0; i < WBOX * HBOX; i++ )
                {
                  if ( wbot > SBOX.matrBOX.vect[i] )
                    wbot = SBOX.matrBOX.vect[i];
                }
               if ( wbot )  return FH_Y_FILLED;
               else         return FH_NO_IDEA;
             }
           }

        return FH_N_FILLED;

}

static void cell_versions_to_text (cell *C)
{
    pchar t;
    int16_t   i;

    t  = text_string;
    *t = 0;
    if (C -> flg & c_f_let)
    {
        for (i = 0; i < C -> nvers; i ++)
        {
            t += sprintf (t, "%c %d ", C -> vers [i]. let, C -> vers [i]. prob);
            if (t - text_string > sizeof (text_string) - 10) break;
        }
        *t = 0;
    }
}

static Bool not_letter(cell * C)
{
  extern int16_t sMALL_SIZE;
     B_LINES bl;
     int16_t h;
     if (C->h <= sMALL_SIZE)  //NB 3.4.95
          return TRUE;
     get_b_lines (C, &bl);
     if (bl.n3 != -1) 				return FALSE;
     h= (bl.b3-bl.b2) >> 1;
     if (h >= bl.b3 - C->row)			return TRUE;
     if (h >= C->row + C->h - bl.b2)		return TRUE;
     return FALSE;
}


static void put_oa_case (cell *C)
{
#define MON_SYM     -30
#define MON_R_ABR   -40
#define MON_VAL     -40
#define MON_FILL    -40
#define MON_ITAL    -10
#define MON_        -10

  switch ( oaprop )
   {
    case   1:   cell_bonus_let ( C, 'a', MON_SYM );   break;
    case   2:   cell_bonus_let ( C, 'o', MON_R_ABR ); break;
    case   3:   cell_bonus_let ( C, 'a', MON_VAL );   break;
    case   4:   cell_bonus_let ( C, 'a', MON_FILL );  break;
    case   5:   cell_bonus_let ( C, 'a', MON_SYM );   break;
    case   6:   cell_bonus_let ( C, 'o', MON_ );      break;
    case   9:   cell_bonus_let ( C, 'o', MON_ITAL );  break;
   }

  if ( bon_a ) cell_bonus_let ( C, 'a', bon_a );
  oaprop += bon_a * 10 ;
  if ( oa_accent_existing_flag == ACCENT_IS_FOUND )
    oa_accent_restoring( C );
  sort_vers ( C );
}

static void put_ec_case (cell *C)
{
#define MON_MID_PEAK        -40
#define MON_NOT_FILLED_HEAD -56
#define MON_LARGE_LEFT_SIDE -36
#define MON_FILLED_HEAD     -40
#define MON_IMITATION       -2
#define MON_NOT_C           -80
#define MON_NOT_E           -20

  switch ( ecprop )
   {
    case   1  :   cell_bonus_let ( C, 'c', MON_MID_PEAK );        break;
    case   2  :   cell_bonus_let ( C, 'e', MON_NOT_FILLED_HEAD ); break;
    case   20 :   cell_bonus_let ( C, 'c', mon_c );               break;
    case   22 :   cell_bonus_let ( C, 'c', MON_FILLED_HEAD );     break;
    case   222:   cell_bonus_let ( C, 'c', MON_IMITATION );       break;                                      break;
    case   3  :   cell_bonus_let ( C, 'c', MON_LARGE_LEFT_SIDE ); break;
    case   33 :   cell_bonus_let ( C, 'c', MON_LARGE_LEFT_SIDE ); break;
    case   333:   cell_bonus_let ( C, 'e', MON_NOT_E );           break;
    case   4  :   cell_bonus_let ( C, 'e', MON_NOT_E );           break;
    case   44 :   cell_bonus_let ( C, 'c', mon_c );               break;
    case   5  :   cell_bonus_let ( C, 'c', MON_NOT_C );           break;
   }

  sort_vers ( C );
}

static void oa_accent_restoring( cell *resC )
{
int16_t i, j;
int16_t recogAccent_o=0;	// 14.08.2001 E.P.

version    *srcv, *resv;
    for ( resv = resC->vers, i = 0; i < resC->nvers; i++, resv++ )
    {
        for ( srcv = oa_src_vers, j = 0; j < oa_src_nvers; j++, srcv++ )
        {
            if ( resv->let == 'o' )
            {
              switch ( srcv->let )
				{
                case    o_left_accent :
					// Конфликтный код 07.09.2000 E.P.
					if (is_cen_language(language) ||
						is_baltic_language(language)
					   )
						continue;

                case    o_right_accent:
                case    o_roof_accent :
                case    o_2dot_accent :
					srcv->prob = resv->prob;
					break;
				}

              continue;
            }
            if ( resv->let == 'a' )
            {
              switch ( srcv->let )
              {
                case    a_left_accent :

					// Конфликтный код 07.09.2000 E.P.
					if (is_cen_language(language) ||
						is_baltic_language(language) // 17.07.2001 E.P.
						)
						continue;

                case    a_right_accent:
                case    a_roof_accent :
					// Конфликтные коды 07.09.2000 E.P.
					if (is_baltic_language(language)) // 17.07.2001 E.P.
						continue;

                case    a_2dot_accent :
                    {
					srcv->prob = resv->prob;
					}
              }
              continue;
            }
            if ( resv->let == srcv->let )
            {
                    srcv->prob = resv->prob;
                    break;
            }
        }
    }
    memcpy( resC->vers, oa_src_vers, sizeof( oa_src_vers ) );

}

/****************************************************
static print_hist(int16_t dx, int16_t dy)
{
 int16_t start, end, i;
 for (; dy; dy--)
  {
   start = dx - hist[3][dy-1];
   end   = hist[4][dy-1]-1;
   for (i=0; i<start; i++) printf(".");
   for (i=start; i<=end; i++) printf("*");
   for (i=end; i<=dx; i++) printf(".");
   printf("\n");
  }
}
***************************************************/

