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

#include "stick.h"
#include "inc_tab.h"
#include <string.h>
#include <stdlib.h>
#include "recdefs.h"
#include "lang_def.h"

#include "minmax.h"

#define MAX_ADD_DIS  30		/* max discrim for adding 'є','1'	*/
static uchar tab_t[]=					// dis_t
	{
	40,  /* 0.  no flags                                  */
	20,  /* 1.  no beam : exist only one flags>1          */
	50,  /* 2.  no beam : exist only one flags=1          */
	4,   /* 3.  beam : left flag>right                    */
	30,  /* 4.  exist long pimple and beam                */
	2,   /* 5.  short beam and wide stick                 */
	2,   /* 6.  meandr and inc                            */
	40,  /* 7.  meandr and no inc                         */
	24,  /* 8.  for any superfluous flags                 */
	10,  /* 9.  exist upper serif                         */
	40,  /* 10. left downer flag                          */
	40,  /* 11. for any long central flag                 */
	4,   /* 12. for any near central flag                 */
	16,  /* 13. for beam , belong down half of hei        */
	14,  /* 14. head : similar to 'f' (18.11.1993: EVEN!) */
	60,  /* 15. for sitution ] for left abris             */
	70,  /* 16. without inc similar    f                  */
	120, /* 17. no beam : left flag==1 right = 0          */
	100, /* 18. for height beam : lower second b.l.       */
	10,  /* 19. similar    'f'                            */
	MD,  /* 20. MIN discrim for thin stick  (width<3)     */
	100, /* 21. too thick beam                            */
	40,  /* 22. beam-flags not overlay , thin stick       */
	20,  /* 23. upper rignt long flag for wide stick      */
	100, /* 24. no beam : right upper flag belong 0-zone  */
	40,  /* 25. narrow stick + right meandr               */
	20,  /* 26. meandr and no inc  by narrow stick        */
	40,  /* 27. similar 'l'                               */
	40,  /* 28. thick near flag for bold c_comp             */
	100,	// 29. MK: too long hook: "t."==>'t'; fax12/32 "document."
	16,	// 30. MK: about similar 'f'
	8,	// 31. MK: about similar 'f'
	44	// 32. MK: about some CUT Cases (first was 60)
	};


static uchar tab_l[]=					// dis_l
	{
	2,	// 0. right upper flag > left or no left
	8,	// 1. -- // -- + long right flag
	6, 	// 2. too different flags
	BR,	// 3. for configurations : '[' , ']'
	2, 	// 4. for z-conf and not inc
	40,	// 5. for s-conf
	LF,	// 6. for any long center flag
	NF,	// 7. for any near center flag
	10,	// 8. for any point of curve
	DB,	// 9. for any beam
	70,	// 10. for 2 serifs
	48,	// 11. for 1 half-serif
	SI,	// 12. similar    'є' : exist neck
	80,	// 13. similar    'L'
	BP,	// 14. bad proportions
	ST,	// 15. similar    't'
	14,	// 16. for DIS_BEAM_MK
	8,	// 17. SPEC CASE: NECK=1 wid>=6
	SI,	// 18. thin LEG exist
	6	// 19. about thin '(' or ')'	09.06.1993
	};
static uchar tab_slash[] =
	{
	20, /* 0. for long central flag       */
	0,  /* 1. for near central flag       */
	40  /* 2. central beam                */
	};
static uchar tab_I[]=				// dis_I
	{
	20,	// 0. long halfserif
	8, 	// 1. too different flaghs
	30,	// 2. two long and two small serifs
	LF,	// 3. for every point long center flag
	NF,	// 4. for any near center flag
	60,	// 5. for any hook
	DB,	// 6. FOR CENTRAL BEAM
	60,	// 7. two zero and two long serifs
	70,	// 8. similar    1 , f
	MD,	// 9. for  up left,down left, down right
	SI,	// 10. similar    є : exist neck
	70,	// 11. low c_comp
	BP,	// 12. bad proportions
	ST,	// 13. similar    't'
	BR,	// 14. similar    '(',')'
	SI,	// 15. thin LEG exist
	14,	// 16. for DIS_BEAM_MK
	6	// 17. about thin '(' or ')'	09.06.1993
	};

static uchar tab_1[]=					// dis_1
	{
	60,  /* 0. no left upper flag : zones 0, 1        */
	2,   /* 1. for right upper flag : zone  0         */
	8,   /* 2. bad downer serif for inc               */
	4,   /* 3. bad downer serif for no inc            */
	6,   /* 4. too different flags                    */
	20,  /* 5. no serifs                              */
	LF,  /* 6. for any long center flag               */
	NF,  /* 7. for any near center flag               */
	BR,  /* 8. for configurations [,]                 */
	30,  /* 9. for hard meandr                        */
	40,  /* 10. similar    'f'                        */
	20,  /* 11. nose + upper near flag                */
	MD,  /* 12. for  up left,down left, down right    */
	SI,  /* 13. similar    є                          */
	40,  /* 14. similar    'l'                        */
	70,  /* 15. for 2 serifs                          */
	BR,  /* 16. similar    '(',')'                    */
	20,  /* 17. short nose                            */
	8,   /* 18. solid stick - small dis               */
	MD,  /* 19. cutting nose                          */
	BP   /* 20. bad proportions                       */
	};

static uchar tab_circle_brace[]=
  {
  70, /* 0. up or down serif                 */
  10, /* 1. '(' similar    't'               */
  40, /* 2. '(' similar    '1'               */
  20, /* 3. no conc in top and bootom angles */
  60, /* 4. no flags : solid stick           */
  40, /* 5. for cutting brace                */
  28  /* 6. bad budgles                      */
  };

static uchar tab_sq_brace[]=
  {
  44,  /* 0. no flag : В  MK: 44 from 29.09.1993 (old 4)  */
  8,   /* 1. bad paar flags : ВД  for ], ДВ for [         */
  20,  /* 2. for any long center flag                     */
  8,   /* 3. for any near center flag                     */
  26,  /* 4. for up and down concaves : similar  '(',')'  */
  88,  /* 5. MK ADD 29.09.1993 for 't' => '['   */
  222, /* 6. MK ADD 30.09.1993 for TERRIBLE case    */
  80    /* 7/ pen for cutting images */
  };

/*----------------------------------------------------------------------*/
						// GLOBAL VARIABLES :
uchar GL_hist [2*LIMIT_HEIGHT];			/* array for histogramm	*/

center_interval GL_cent [LIMIT_HEIGHT];		/* center of intervals	*/

center_interval GL_center [LIMIT_CENTER];	// center of c_comp-intervals

	uchar GL_left1 [LIMIT_HEIGHT],	/* auxiliary left and		*/
	     GL_right1[LIMIT_HEIGHT];	/*    right abris-arrays	*/

static int16_t  GL_hooks[4];		/* array of "hooks"		*/
static int16_t  GL_hist_int[LIMIT_HEIGHT];	/* number of intervals in any row */

	uchar GL_left0 [LIMIT_HEIGHT],	/* left and right abris-arrays	*/
	     GL_right0[LIMIT_HEIGHT];	// NO STATIC from 19.01.1993

static int16_t GL_tab_angle [LIMIT_HEIGHT];		/* optimal center inc line */
static int16_t num_lines;
int16_t nIncline=0;
extern uchar language;

////////////////////Functions prototypes/////////////////////////////
uchar lnhead_stick_center_study(lnhead *lin,int16_t dy,int16_t dx,
        STICK_CHARS *res_left_chars,STICK_CHARS *res_right_chars,
        STICK_SIGNUMS *res_signums);
static int16_t lnhead_to_centers(lnhead *lin, int16_t wid,
        center_interval center[],uchar left[], uchar right[]);
static int16_t num_of_lines(center_interval center[],int16_t nc,int16_t dy,int16_t hist[]);
static int16_t first_tg(INC_BASE *tab_inc[], int16_t num_inc, int16_t tg2048 );
static int16_t abris_expansion (uchar left[], uchar right[],
          int16_t dy, int16_t dx, int16_t tab_angle[]);
static int16_t dis_slash (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s, int16_t typ_add);
static int16_t dis_I (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s);
static int16_t dis_1 (STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s, int16_t typ_add);
static int16_t dis_l (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s)  ;
static int16_t dis_l_stroked (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s)  ;
static int16_t dis_t (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s, int16_t sign_f);
static int16_t dis_circle_brace(STICK_CHARS *l,STICK_CHARS *r, STICK_SIGNUMS *s, int16_t typ);
static int16_t dis_r_sq_brace(STICK_CHARS *l,STICK_CHARS *r, STICK_SIGNUMS *s);
static int16_t dis_l_sq_brace(STICK_CHARS *l,STICK_CHARS *r, STICK_SIGNUMS *s);
static int16_t find_neck (STICK_CHARS *l, STICK_CHARS *r,int16_t lim_long);
static int16_t find_beam (STICK_CHARS *l, STICK_CHARS *r,int16_t lim_long);

extern int16_t set_stick_char (uchar left[], uchar right[], int16_t hooks[],
       int16_t dy, int16_t dx, int16_t opt, int16_t wide, int16_t corr_mode,
       int16_t skip_ul, int16_t skip_dl, int16_t skip_ur, int16_t skip_dr,
       int16_t inc_num,
		   STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s,
       int16_t *l_mode, int16_t *r_mode);
extern int16_t make_center_line_dif (center_interval center[], int16_t nc,
       uchar left[], uchar right[],
       int16_t  dy, int16_t dx, INC_BASE *angles[], int16_t num_angles,
       int16_t  tab_angle[],
       Bool16 comp_wide, Bool16 sig_T, Bool16 sig_f, Bool16 sig_r,
       int16_t  *wide, int16_t hooks[], int16_t *inc_v, uchar enable_correct);
extern Bool32  digit_mode, dif_adding_mode ;
//////////////////////////////////////////////////////////////////////////


uchar lnhead_stick_center_study(lnhead *lin,int16_t dy,int16_t dx,
        STICK_CHARS *res_left_chars,STICK_CHARS *res_right_chars,
        STICK_SIGNUMS *res_signums)
{
int16_t wide,opt,left_mode,right_mode,inc_v, nc, num_angles,f_a;
uchar ret_br=0;
STICK_CHARS left_chars,right_chars;
STICK_SIGNUMS signums;

  memset(GL_left0, 0xFF, dy );
  memset(GL_right0,0xFF, dy );

	if( (nc = lnhead_to_centers(lin,dx,GL_center,GL_left0,GL_right0))==0 )
    return(0);  /* too many Int16ervals */

if ( num_of_lines (GL_center, nc, dy, GL_hist_int) )
	return(0); /* non stick:>1lines*/

num_angles =   sizeof(stick_inc)/sizeof(stick_inc[0]);
f_a=first_tg(stick_inc, num_angles,nIncline );
num_angles=MIN( (dx*2048/dy)>800?LIMIT_OF_ANGLES+4:LIMIT_OF_ANGLES,
           (int16_t)(sizeof(stick_inc)/sizeof(stick_inc[0])-f_a-1));

if( (ret_br=(uchar)make_center_line_dif(GL_center,(int16_t)(nc-(GL_center[nc-1].len==1)),
		     GL_left0, GL_right0,dy, dx,
		     &stick_inc[f_a], num_angles, GL_tab_angle,
         0, 0, 1, 0, &wide, GL_hooks, &inc_v, 1)) ) // with correct
    {
    if( ret_br )
        {
        opt=0;
        set_stick_char (GL_left0, GL_right0, GL_hooks, dy, dx, opt, wide,
		        (int16_t)(opt - MIN ( GL_tab_angle[0], GL_tab_angle[dy-1] )),
		        0, 0, 0, 0,  0,		// NB: LAST ZERO PAR - inc_num (?????)
		        &left_chars, &right_chars, &signums,
		        &left_mode, &right_mode);

        return (ret_br+1); // silmular to (,)
        }
    else
    	return(0); /* abnormal set of ceneters :  */
		           /* not exist center-line       */
    }

opt = ((MAX( GL_tab_angle[0], GL_tab_angle[dy-1] ))>>1)<<1;
abris_expansion (GL_left0, GL_right0, dy, dx, GL_tab_angle);

set_stick_char (GL_left0, GL_right0, GL_hooks, dy, dx, opt, wide,
		(int16_t)(opt - MIN ( GL_tab_angle[0], GL_tab_angle[dy-1] )),
		0, 0, 0, 0,  0,		// NB: LAST ZERO PAR - inc_num (?????)
		&left_chars, &right_chars, &signums,
		&left_mode, &right_mode);
signums.incline = inc_v;
signums.inc = (inc_v>256);
*res_signums    = signums;
*res_left_chars = left_chars;
*res_right_chars = right_chars;
return 1;
}

uchar   stick_recog(uchar let, STICK_CHARS *l, STICK_CHARS *r,
                STICK_SIGNUMS *s)
{
   int16_t dis=0;
   uchar ret;
switch( let )
  {
  case (uchar)'l':
    dis=dis_l(l,r, s);
    break;
  case (uchar)'t':
    dis=dis_t(l,r, s, 0/*sign_f*/);
    break;
  case (uchar)'1':
    dis=dis_1(l,r, s,(uchar)dif_adding_mode);
    break;
  case (uchar)'|':
    dis=dis_I(l,r, s);
    break;
  case (uchar)'/':
    dis=dis_slash(l,r, s,(uchar)dif_adding_mode);
    break;
  case (uchar)'(':
    dis=dis_circle_brace(l,r, s,1);
    break;
  case (uchar)')':
    dis=dis_circle_brace(l,r, s,0);
    break;
  case (uchar)']':
    dis_r_sq_brace(l,r, s);
    break;
  case (uchar)'[':
    dis_l_sq_brace(l,r, s);
    break;
  default :
    break;
  }
if( dis>255 )
   ret=0;
else
   ret = 255-dis;
return ret;
}

int32_t lnhead_stick_get_incline(lnhead *lin,int16_t dy,int16_t dx)
{
int16_t wide,opt,left_mode,right_mode,inc_v, nc, num_angles,f_a;
STICK_CHARS left_chars,right_chars;
STICK_SIGNUMS signums;

  memset(GL_left0, 0xFF, dy );
  memset(GL_right0,0xFF, dy );

	if( (nc = lnhead_to_centers(lin,dx,GL_center,GL_left0,GL_right0))==0 )
    return(-1);  /* too many Int16ervals */

if ( num_of_lines (GL_center, nc, dy, GL_hist_int) )
  return(-1); /* non stick:>1lines*/

num_angles =   sizeof(stick_inc)/sizeof(stick_inc[0]);
f_a=first_tg(stick_inc, num_angles,nIncline );
num_angles=MIN((dx*2048/dy)>800?LIMIT_OF_ANGLES+4:LIMIT_OF_ANGLES,
           (int16_t)(sizeof(stick_inc)/sizeof(stick_inc[0])-f_a-1));

if( make_center_line_dif(GL_center,(int16_t)(nc-(GL_center[nc-1].len==1)),
		     GL_left0, GL_right0,dy, dx,
		     &stick_inc[f_a], num_angles, GL_tab_angle,
         0, 0, 1, 0, &wide, GL_hooks, &inc_v, 1) ) // with correct
  return(-1); /* abnormal set of ceneters : silmular to (,) or */
		   /* not exist center-line                         */

opt = ((MAX( GL_tab_angle[0], GL_tab_angle[dy-1] ))>>1)<<1;
abris_expansion (GL_left0, GL_right0, dy, dx, GL_tab_angle);

set_stick_char (GL_left0, GL_right0, GL_hooks, dy, dx, opt, wide,
		(int16_t)(opt - MIN ( GL_tab_angle[0], GL_tab_angle[dy-1] )),
		0, 0, 0, 0,  0,		// NB: LAST ZERO PAR - inc_num (?????)
		&left_chars, &right_chars, &signums,
		&left_mode, &right_mode);

return signums.incline;
}


static int16_t lnhead_to_centers(lnhead *lin, int16_t wid,
        center_interval center[],uchar left[], uchar right[])
{
 int16_t ll,ind,n;
 lnhead   *line;
 interval *inter;
 uchar l,r,h;
 center_interval *p_center=&center[0];

for (n=0,line=lin; (ll=line->lth)>0; line=(lnhead *)((uchar*)line+ll))
	{
	h=(uchar)line->h;
	n += h;
	if( n>=LIMIT_CENTER )
		return( 0 );
  for( ind=line->row,inter=(interval *)((uchar*)line+sizeof(lnhead));
      h ;ind++,h--,inter++)     /* one line    */
		{
    int16_t inter_e, inter_l;
		  if( ind>=LIMIT_HEIGHT )
			  return 0;
    inter_e = inter->e, inter_l = inter->l;
    r = wid     - inter_e;            /* one Int16erval */
    l = inter_e - inter_l;

		if( right[ind]>r )  right[ind] = r;
		if( left[ind]>l  )  left[ind]  = l;

    p_center->col   = l + inter_e - 1 ;  /* center*2 */
		p_center->row   = ind;
    p_center->len   = inter_l;
		p_center++;
		}
	}

return(n);
}

static int16_t num_of_lines(center_interval center[],int16_t nc,int16_t dy,int16_t hist[])
{
int16_t n,*r,*e,ret;
center_interval *p_center,*p_end=&center[nc];

memset(hist,0,dy*sizeof(hist[0]));
for(p_center=&center[0];p_center<p_end;p_center++)
	hist[ p_center->row ] ++;     /* make hist of numbers of */
              /* Int16ervals in any row    */
if( nc>dy  )
{     /* too many Int16ervals */
for(n=0,r=&hist[1],e=&hist[dy-2];r<e;r++)
	if(*r>1)
		n++;
num_lines = n*20; /* OLEG:03.03.94 */
dy -= 2;
ret = (n<<2>(dy-2)*3);      /* !!!???!!! */
if( ret==0 )
  {    /* normal number of Int16ervals */
  int16_t i,j;
  for(i=1;i<dy&&hist[i]==1;i++); /* one Int16erval in row     */
  for(j=i;i<dy&&hist[i]>=2;i++); /* more 1 Int16ervals        */
	if( (i-j)<<1>dy )
		ret=1;                  /* too many central 2-row */
	}
dy += 2;
}
else ret = 0; /* nc<=dy */
return(ret);  /* 0 - OK, 1 - bad c_comp (>1 line) */
}

static int16_t first_tg(INC_BASE *tab_inc[], int16_t num_inc, int16_t tg2048 )
{
int16_t i;
if( abs(tg2048)>32 )      /* nonzero incline      */
	{
	for(i=0; i<num_inc && tg2048>tab_inc[i]->tg;i++);
	if( i ) i--; /* for i>0 tab_inc[i-1]->tg <tg2048<= tab_inc[i]->tg */
		     /* for i=0 tg2048< tab_inc[0]->tg                    */
	}
else
	{
	for(i=0;i<num_inc;i++)
		if( tab_inc[i]->tg==0 )
			break;
	if( i==num_inc )
		i=0;  /* not exist table for 0 inc */
	}
return(i);
}

static int16_t abris_expansion (uchar left[], uchar right[],
          int16_t dy, int16_t dx, int16_t tab_angle[])  {
							// 09.12.1993
int16_t i, opt;
int16_t k, max_negat_left=0, max_negat_right=0; // 09.12.1993

opt = MAX (tab_angle[0], tab_angle[dy-1]);  // NB: NO VERY GOOD !!!
for (i=0; i<dy; i++) {	/* dilate (step=4) and shift (inc = tab_angle) */
	if ( left[i]!=0xFF )  {
		/******************************	BEFORE 09.12.1993
		left[i] <<= 2;
		left[i]  = opt + left[i] - tab_angle[i];
		****************************************/
		k = opt + (left [i] << 2) - tab_angle [i];	// 09.12.1993
//////		if (k<0)  k = 0;	// NEGAT CASE (It is possible !!!!!!)
		if (k<0)  if (max_negat_left>k)  max_negat_left = k;
		left [i] = (uchar)k;
		}
        else
		left[i] = (uchar)opt;  /* empty string */

	if ( right[i]!=0xFF )  {
		/******************************	BEFORE 09.12.1993
		right[i] = (dx - 1 - right[i]) << 2;
		right[i] = opt + right[i] - tab_angle[i];
		****************************************/
		k = opt + ((dx - 1 - right[i]) << 2) - tab_angle [i];
//////		if (k<0)  k = 0;	// NEGAT CASE (It is possible !!!!!!)
		if (k<0)  if (max_negat_right>k)  max_negat_right = k;
		right [i] = (uchar)k;
		}
	else
		right[i] = (uchar)opt; /* empty string  */
	}
/*......................................................................*/
if (max_negat_left)					// 09.12.1993
  for (i=0; i<dy; i++)  left [i] -= (uchar) max_negat_left;

if (max_negat_right)
  for (i=0; i<dy; i++)  right [i] -= (uchar) max_negat_right;

return(1);
}

int16_t dis_I (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s) {	// 17.01.1994

int16_t	dis=0, t, lmu, rmu, lmd, rmd, num_l, num_n, num_z,z;
int16_t	sl=l->num_flags, sr=r->num_flags;
int16_t	wid=s->stick_width, inc=s->inc, dy=s->height;
int16_t	max_u, max_d;
int16_t	d_L, d_R;	// 24.02.1993

	dis = 0;		// 18.11.1993 (0 or 100);
   if( s->incline>256 )
         return 126;
//////	dis += mk_dis_for_liga_exm;	// 06.01.1994 (0 or 2);	MOVE TO END;

lmu = l->mount[0];  rmu = r->mount[0];	max_u = MAX (lmu, rmu);
lmd = l->mount[4];  rmd = r->mount[4];	max_d = MAX (lmd, rmd);
/*......................................................................*/
if( lmu+lmd==0 && rmu+rmd>0 && l->conc[0]>1 && l->conc[4]>1 )
    dis += 50; // similar (
if( lmu+lmd>0 && rmu+rmd==0 && r->conc[0]>1 && r->conc[4]>1 )
    dis += 50; // similar (
if( lmu+rmu+lmd+rmd != 0 )  {
if ( max_u > (MAX(max_d,3)<<1) )		// MK PROBA 18.02.1993
	dis += 222;							// 222
if ( max_u > (MAX(max_d,2)<<1) && max_d<2 )		// MK PROBA 18.02.1993
	dis += 222;							// 222

DIS_HALF_SERIF (l,r,0,1,tab_I[0]);	/* test upper serif  */		// 20
DIS_HALF_SERIF (l,r,4,1,tab_I[0]);	/* test downer serif */		// 20

if( l->up_serif && r->up_serif )
	DIS_DIFFERENT_SERIFS(l,r,0,4,wid,tab_I[1]);			// *8
//////if( l->down_serif && r->down_serif )
if( l->down_serif && (r->down_serif || r->down_hook))	// 28.06.1993 about 'L'
	DIS_DIFFERENT_SERIFS(l,r,4,4,wid,tab_I[1]);			// *8

/* -------------------- STUDY ANGLES ----------------------- */

t = 3;				/* study long flags */
num_l = (lmu>t)+(rmu>t)+(lmd>t)+(rmd>t);
num_n = (lmu<=1)+(rmu<=1)+(lmd<=1)+(rmd<=1);
z = num_l>2 ? 2 : 1;
num_z = (lmu<z)+(rmu<z)+(lmd<z)+(rmd<z); /* num empty angles */
if( num_l==2 && num_n==2 )
	dis += tab_I[2];	/* two half-serifs only	*/		// 30
t = lmu+rmu+rmd+lmd;
if( num_z==2 && t>2 )		/* two empty half-serif */
	dis += t>4 ? tab_I[7]<<1:tab_I[7];				// 60
if( rmu>1 && lmu>rmu*2 && lmd>rmu*2 && rmd>rmu*2 )
   dis += 70;
/******************************************************	BEFORE 26.10.1993
if( num_z==3 &&			// three empty half-serif
    (lmu>0 && s->dis_nose_1==0 ||	// exist 1-nose
     rmu>0 && s->r_f_symptom && s->l_f_symptom ))
//////	dis += tab_I[8];	// similar 'f','1'			// 70
	dis += 50;		// PROBA 22.10.1993 EVENING;		// 50
	*********************************************************************/

if (num_z==3)  {		// three empty half-serif  26.10.1993 Edition:
    if (rmu>0 && s->r_f_symptom && s->l_f_symptom)
	dis += tab_I[8];	// similar 'f'				// 70
    if (lmu>0 && s->dis_nose_1==0)	// exist 1-nose, similar '1':
	dis += (lmu==1) ? 50 :		// d13/5(6) "CRITIC"		// 50
	       (lmu==2) ? 60 :		// MK Int16ERPOLATION;		// 60
			  70;		// c3/21 "3.1x"			// 70
    }

if( num_z==3 && lmd>1 )		/* three empty half-serif */
	dis += tab_I[8];						// 70

if( num_z==1 && num_l==3  && rmu==1 )  /* UL,DL,DR flags */
	dis += tab_I[8];	/* similar 1,l */			// 70

if( s->width<9 && lmu>max_d && rmu==1 )  /* image is too thin, UL,DL,DR flags */
	dis += 20;	/* similar 1,l */			// 70
if( s->width<9 && lmu>0 && max_d>0 && rmu==0 )  /* image is too thin, UL,DL,DR flags */
	dis += 20;	/* similar 1,l */			// 70


t=1;				/* study near flags */
num_l = (lmu>t)+(rmu>t)+(lmd>t)+(rmd>t);
if( num_l==3 && lmu>0 && s->typ_nose_1==1 )
	dis += tab_I[8];	/* left flag similar 1-nose  */		// 70
if( num_z==1 && rmu<t && r->up_serif==0 )
	dis += tab_I[8];	/* similar 1 */				// 70
if( num_l==3 && rmu==1 && lmd+lmu+rmd>8 )
	dis += tab_I[8];	/* similar 1 */				// 70
if (s->neck==3)						// 16.12.1993
	dis += 100;							// 100
else if (s->neck==2 && num_l<3)
	dis += tab_I[10];	/* simular є */				// 20
DIS_BRACES_CIRCLE(l,r,wid,tab_I[14]);	/* braces (,) */		// 40
}
/*......................................................................*/
else	{	/* all angles empty */
	if (s->neck==3)					// 16.12.1993
		dis += 100;						// 100
	else if (s->neck==2)
		dis += tab_I[10];	/* similar є */			// 40
	else if( s->neck==1 )
		FOUR_CONC(l,r,tab_I[10]);				// 40
	/*************************************	DELETE IT 20.10.1993
	if (!inc  &&  (s->cut_l || s->cut_r)) // 18.10.1993 CUT, NO INC&Serifs
//////	    { dis = 140; goto m_ex; }	// g14/6 "Greatest", first 't'	// 140!
	    { dis = 90;	 goto m_ex; }	// g14/6 "Greatest", first 't'	// 90!
//////	    { dis = 76;  goto m_ex; }	// g14/6 "Greatest", first 't'	// 76!
	    *************************************************************/
	}
/*......................................................................*/
/* N.B.	in group macros (CENTER_FLAG) used ->c_meandr.	*/
/*	If c_meandr==0 and exist near central flag,then */
/*	not discrim. For narrow sticks I wont check	*/
/*	near flags . -->> I increase signums c_meandr	*/
/*	for left and right chars of stick		*/

if ( wid<3 )  { l->c_meandr++;  r->c_meandr++; }	// MODIFY c_meandr !!!

/*********************************************	OLD OLEG VARIANT
DIS_CENTER_FLAGS(l,r,1,wid,inc,tab_I[3], tab_I[4]);
DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_I[3], tab_I[4]);
DIS_CENTER_FLAGS(l,r,3,wid,inc,tab_I[3], tab_I[4]);
	********************************************************/

				// This Part - as in dis_l !!!
				// ABOUT ONE PIXEL		// 24.02.1993
	d_L = d_R = tab_I[4];	// DEFAULT			// =12
	/*********************************************	before 27.07.1993
	if (s->neck<2)  {					// as in dis_l
	    if ( (!l->num_long_flags) && !s->cut_l &&
		 ( (wid>=4 && dy>18) ||				// 23.02 17:30
		   (wid==3 && !l->num_flags) ) )	d_L = 0;
	    if ( (!r->num_long_flags) && !s->cut_r &&
		 ( (wid>=4 && dy>18) ||				// 23.02 17:30
		   (wid==3 && !r->num_flags) ) )	d_R = 0;
	    }
	    *************************************************************/
	if (s->neck<2)  {
//////	    if ( (!l->num_long_flags) && !s->cut_l &&	// OLD
	    if ( !s->cut_l &&
//////		 (dy>24)  ||				// 27.07.1993 PROBA
		 (dy>24 && !s->neck)  ||			// 29.07.1993 PROBA
		 (!l->num_long_flags) &&
		  ((wid>=4 && dy>18) || (wid==3 && !l->num_flags)))  d_L = 0;
//////	    if ( (!r->num_long_flags) && !s->cut_r &&	// OLD
	    if ( !s->cut_r &&
//////		 (dy>24)  ||				// 27.07.1993 PROBA
		 (dy>24 && !s->neck)  ||			// 29.07.1993 PROBA
		 (!r->num_long_flags) &&
		  ((wid>=4 && dy>18) || (wid==3 && !r->num_flags)))  d_R = 0;
	    }

//////DIS_CENTER_FLAGS(l,r,1,wid,inc,tab_I[3], ddd);		// 60,12(0)
DIS_CENTER_FLAG(l,1,wid,inc,tab_I[3], d_L);			// 60,12(0)
DIS_CENTER_FLAG(r,1,wid,inc,tab_I[3], d_R);			// 60,12(0)
//////DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_I[3], ddd);		// 60,12(0)
DIS_CENTER_FLAG(l,2,wid,inc,tab_I[3], d_L);			// 60,12(0)
DIS_CENTER_FLAG(r,2,wid,inc,tab_I[3], d_R);			// 60,12(0)
//////DIS_CENTER_FLAGS(l,r,3,wid,inc,tab_I[3], ddd);		// 60,12(0)
DIS_CENTER_FLAG(l,3,wid,inc,tab_I[3], d_L);			// 60,12(0)
DIS_CENTER_FLAG(r,3,wid,inc,tab_I[3], d_R);			// 60,12(0)

//////DIS_CENTER_CONCS(l,r,2,inc,tab_I[3], 0);	//////tab_I[4]);
DIS_CENTER_CONCS(l,r,2,inc,tab_I[3],tab_I[4]);	// RESTORE (SEPULENIE 28.01.93)
								// 60,12

if (d_L || d_R)  {	// 02.02.1993	PROBA (ABOUT GLISTA)	//  OR ??????
CALC_LENGTH_BEAM_FLAGS (l,r,wid,s, t);
DIS_BEAM (l,r,1,t,wid==1 ? tab_I[6]<<1 : tab_I[6]);		// 140,70
DIS_BEAM (l,r,2,t,wid==1 ? tab_I[6]<<1 : tab_I[6]);	// 18.02.1993  MK PROBA
DIS_BEAM_MK (l,r,2, t, wid==1 ? tab_I[16]<<1 : tab_I[16]);  	// 28,14  25.02.1993: MK
DIS_BEAM (l,r,3,t,wid==1 ? tab_I[6]<<1 : tab_I[6]);		// 140,70
if (s->width<=4  &&  !inc  &&	// add 09.06.1993 about added thin '(' or ')'
    (l->mount[2] && r->conc[2] && 		// '(' LEFT
     (s->dis_left_brace==0) &&
     (l->m_meandr==1) && (r->c_meandr==1)  ||
     l->conc[2] && r->mount[2] && 		// ')' RIGHT
     (s->dis_right_brace==0) &&
     (l->c_meandr==1) && (r->m_meandr==1)))
	dis += tab_I[17];					// 6
}
else if (l->conc[0]  && r->conc[0]  &&	// 19.11.1993 GLISTA  similar 't'
	 l->mount[1] && r->mount[1] &&	// g14/32(33) "fastest" first 't'
	 wid < 5  &&			// g14/34(37) "Kit"
	 abs (l->mb_pos[1] - s->base_2) < 2  &&
	 abs (r->mb_pos[1] - s->base_2) < 2)
		dis += 20;					// 20

if ( wid<3 )  { l->c_meandr--;  r->c_meandr--; }	// RESTORE c_meandr !!!

if( (r->mount[3] && r->mount[4]) && l->mount[4]==0 )
	dis += ( r->down_hook + l->up_hook )*tab_I[5];		// *60
	/* left up or right down hooks */

//////if( s->base_2<2 && s->base_2>-1 )	DELETE 28.01.1993  MK
//////	dis += tab_I[11] ;	/* short stick ??? */		// 70

if( wid*2 > s->height )
	dis += tab_I[12];					// 80

if( l->mount[1]>0 && r->mount[1]>0 && r->mount[4]>0 && sl==1 && sr==2 )
	dis += tab_I[13];	/* similar 't' */		// 80

			// 12.02.1993	thin LEG exist:
if ( l->conc[4] && r->conc[4] &&	// j1/16 "CURTIS": 'R'=>"II"
     (l->ce_pos[4] - l->cb_pos[4] + r->ce_pos[4] - r->cb_pos[4]) >= (dy>>1) )
	dis += tab_I[15];					// 20

			// 19.11.1993	I,l  similar 'E' ("THEATRE")
if (wid*2+1>=dy &&	// k11/31 "PERFORMERCE", last 'E' 21*11, wid=10
    r->conc[1] && r->conc[3] &&
    (r->conc[1] | r->conc[3]) > 1)	// all cases, exclude (1,1)
	dis += 100;						// 100
/*......................................................................*/
//////m_ex:
					// 17.01.1994 MOVE IT TO END:
//	if (dis==0)  dis += mk_dis_for_liga_exm;	// (0 or 2);

	return(dis);
}
int16_t dis_slash (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s, int16_t typ_add)
{
int16_t dis=0;            // 13.12.1993
////int16_t wid = s->stick_width; NOT USED NOW
int16_t inc=s->incline;

if( s->incline<300/*64*/ )
  dis += 100;
if( typ_add )
    {
    if( s->stick_width>s->width/2 )
        dis+=100;
    }
/*dis += s->dis_slash;*/  /* primary discrim  */

DIS_CENTER_FLAG(l,0,128,inc,tab_slash[0],tab_slash[1]);		// 20, 0
DIS_CENTER_FLAG(r,0,128,inc,tab_slash[0]>>2,tab_slash[1]>>2); /* inc! */

DIS_CENTER_FLAGS(l,r,1,128,inc,tab_slash[0],tab_slash[1]);	// 20, 0
DIS_CENTER_FLAGS(l,r,2,128,inc,tab_slash[0],tab_slash[1]);	// 20, 0
DIS_CENTER_FLAGS(l,r,3,128,inc,tab_slash[0],tab_slash[1]);	// 20, 0

DIS_CENTER_FLAG(r,4,128,inc,tab_slash[0],tab_slash[1]);		// 20, 0
DIS_CENTER_FLAG(l,4,128,inc,tab_slash[0]>>2,tab_slash[1]>>2); /* inc! */

/******************************************************************
if ( s->cut_l | s->cut_r  )		// OLD OLEG;
	{
	DIS_BEAM(l,r,1,2,wid==1 ? tab_slash[2]<<1 : tab_slash[2]) ;	// 40
	DIS_BEAM(l,r,3,2,wid==1 ? tab_slash[2]<<1 : tab_slash[2]) ;	// 40
	}
	***********************************************************/
/*
if ( s->cut_l | s->cut_r  )  {		// 13.12.1993 PROBA for j11/36 "/1"
	DIS_BEAM (l, r, 1, 2, tab_slash[2]) ;				// 40
	DIS_BEAM (l, r, 3, 2, tab_slash[2]) ;				// 40
	}
else
 {   // 18.11.1993 for j8/18(20) "of Defense", first 'f'
	if (l->mount[2] && r->mount[2]>=2 &&
	    abs (r->m_pos[2] - s->base_2) < 3)
		dis += tab_slash[2];					// 40
	// NOTA BENE: zones 1,3 and other beam's lens - to future...
	}
*/
return(dis);
}



extern int32_t   dif_typ_of_font;
int16_t dis_1 (STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s, int16_t typ_add)
{							// 17.01.1994
int16_t   dis_sI=0;
int16_t	dis=0, t, lmu, rmu, lmd, rmd, dy=s->height;
int16_t	wid=s->stick_width, inc=s->inc;
int16_t	tt = (dy>30) ? 2 : 1;	// 25.02.1993	// CAUTION: USED TWICE ######
//////int16_t	d_L, d_R;	// 04.06.1993
if( typ_add && s->incline>256 )
    return 200;
if( !digit_mode )
{
if( s->incline>1000 )
	return 200;
else if( s->incline>300 && s->incline<500 )
    dis+=20;
else if( s->incline>500 && s->incline<700 )
    dis+=80;
else if( s->incline>700 && s->incline<1000 )
    dis+=120;
}
else
{
if( s->incline>1000 )
	return 200;
else if( s->incline>500 && s->incline<700 )
    dis+=60;
else if( s->incline>700 && s->incline<1000 )
    dis+=80;
}
lmu = MAX(l->mount[0],l->mount[1]);  rmu = MAX(r->mount[0],r->mount[1]);
lmd = l->mount[4];  rmd = r->mount[4];

/*......................................................................*/
if( lmu+rmu+lmd+rmd != 0 )  {
if( lmu>2 && rmu>2 && lmd>2 && rmd>2 )
    {
    dis_sI = dis_I(l,r, s);
    if( dis_sI>10 )
        dis_sI -= 10;
    if( !typ_add && dis_sI)
        dis += 10;
    }
if( lmu==1 && dy>20 )
	dis += tab_1[17];		/* too small nose */		// 20
t = wid>6 ? 1 : 0;
if( lmu<=t )				/* no left upper flag */
	dis += tab_1[0];						// 60
#ifndef RUS_ENG_LANG
//if(  language != LANG_RUSSIAN || language == LANG_RUSSIAN && lmd<1 && rmd<1 )
#else
//if( multy_language )
#endif
if( !( dif_typ_of_font&LEO_FONT_TW ) )
if( lmd<1 && rmd<1 ||
    rmu>2 && lmd<2 && rmd<2 ||
    rmu>8 && lmd<4 && rmd<4 ||
    abs(rmu-lmu)<2 && rmd+1<rmu && lmd+1<lmu ) // for russian only
{
if( rmu>1 && !inc || rmu>2 && inc)	/* exist right upper flag */
	{	/* different nonlinear (dis=f(rmu)) discrim */
	if( rmu<3 )
		dis += tab_1[1]*(rmu-1);				// *2
	else
        {
        if( digit_mode )
            {
            if( rmu>6 && rmu>(rmd+lmd)/2  ||
                rmu>4 && lmd==0 && rmd==0 && abs(rmu-lmu)<2 ||
                rmu>4 && lmd==0 && rmd==0 && lmu>4 )
                dis += tab_1[1]*6*(rmu-1);
            }
        else
            {
            if( rmu<6 )
		        dis += tab_1[1]*12*(rmu-1);				// *2
	        else
		        dis += tab_1[1]*20*(rmu-1);				// *2
            }
        }
	}
}
if( lmu*2<rmu && lmd*2<rmu && rmd<rmu )
    dis += tab_1[1]*20*(rmu-1);
if( lmd>=lmu*2 && lmd>5 && l->mb_pos[4]<s->height/2 )
    dis += 60;
DIS_HALF_SERIF(l,r,4,0,tab_1[2]);	/* test downer serif */		// 8

if( l->down_serif && r->down_serif )
	DIS_DIFFERENT_SERIFS(l,r,4,4,wid,tab_1[4]);			// 6

if( inc && ( r->down_hook || r->mount[3]>1 ))
	dis += tab_1[14];		/* similar inc 'l' */		// 40
/*..................................................*/
if( typ_add==0 && !(s->typ_nose_1&&l->mount[0]>wid) )	/* not add */
{
int16_t ser = l->up_serif + r->up_serif + l->down_serif + r->down_serif ;

if(0)
//if(  language != LANG_RUSSIAN )
{
if ( lmu==1 && rmu==1 && lmd==1 && rmd==1 && ser>5 ||	// study angle:
/////lmu>1  && rmu>1  && lmd>1  && rmd>1  && ser>4 )	// flags+serifs-information
     lmu>tt && rmu>tt && lmd>tt && rmd>tt && ser>4 )	// 25.02.1993
		dis += tab_1[15];					// 70
}

DIS_BRACES_CIRCLE(l,r,wid,tab_1[16]);	// braces (,)			// 40
if( !(lmd>wid && rmd>wid) )
if ( (r->conc[0]>1 && r->conc[4]>1) ||	// OLD OLEG
     (r->conc[0]>wid) )			// 01.06.1993 for ')' after INC
	dis += tab_1[16];		// this also ')'-config		// 40
if( (r->conc[0] || r->conc[1]) && r->mount[4]>2 && l->mount[4]>2 )
	{	/* pitch є */
	int16_t tt=find_neck(l,r,1);
/******	if( tt<2 ) ******/
	if( tt>=0  &&  tt<2 )		// 31.05.1993 (WAS ERROR if FFFF)
		dis += tab_1[13];	// similar    є			// 20
	}
}
/*..................................................*/
if( l->mount[4]==0 && r->mount[4]==0 && r->mount[0]==0 &&
    s->typ_nose_1==0 )		/* no down serif */
		dis += tab_1[19];	// cutting nose			// 4

if ( (s->typ_nose_1==0)  &&		// 02.06.1993 PROBA:
     wid > 5  &&
     lmu > 3  &&
     lmd > 2  &&			// add 17:30 (for #4 ???)
     (s->lll_nose_1  ||  dy>24 && l->m_pos[0]<3) )
//////		dis += 88;	// PROBA-88				// 88
		dis += 20;	// PROBA-20				// 20

/******************************************************************
if (r->conc[0]>=MAX(wid,3))	// 09.06.1993  for CUT. 'h' to "ll"
	dis += 80;		// PROBA-80				// 80
	***************************************************************/

if (s->cut_r &&		// 09.06.1993 PROBA for CUT. "The" (stdj10);
    (r->mount[0] || r->m_meandr==0) &&
    !r->down_serif &&
////l->mount[0]>MAX(wid*2-3,5))	// first
    l->mount[0]>MAX(wid,5))	// second
//////	dis += 80;		// PROBA-80				// 80
//////	dis += 160;		// PROBA-160				// 160
	dis += (rmu+2)*80;	// PROBA-rmu"*80			// *80
}
/*.......................................................................*/
else	{ /* no serifs */
	if( l->mount[1]+l->mount[2]+l->mount[3]+
	    r->mount[1]+r->mount[2]+r->mount[3] < 2 )
		dis += tab_1[12]<<1;	// small dis  for solid stick	// 8
	else
		dis += tab_1[18];	// exist central flags		// 8
	if( s->neck>1 )
		dis += tab_1[13];					// 20
	else if( s->neck==1 )
		FOUR_CONC(l,r,tab_1[13]);				// *20?
	/*************************************	DELETE IT 20.10.1993
	if (!inc  &&  (s->cut_l || s->cut_r)) // 18.10.1993 CUT, NO INC&Serifs
//////	    { dis = 140; goto m_ex; }	// g14/6 "Greatest", first 't'	// 140!
	    { dis = 90;  goto m_ex; }	// g14/6 "Greatest", first 't'	// 90!
//////	    { dis = 76;  goto m_ex; }	// g14/6 "Greatest", first 't'	// 76!
	    *************************************************************/
	}
/*......................................................................*/
///if (l->mount[1]>2 && l->mb_pos[1]>5)	// 21.10.1993 for TOO DOWN NOSE
if (l->mount[1]>2  &&  			// 22.10.1993 for TOO DOWN NOSE
    l->mb_pos[1] >			// 22.10.1993 vers-080
////(s->cut_l ? 3 : 4))			// ok55/4(5) "22021"  (exotic Nose)
////(s->cut_l ? 3 : 5))			// j8/13(15) "105/18" (exotic Nose)
    (s->cut_l ? 3 : (inc ? 5 : 4)))	// fax25/12(17) "Windows": "nd" => "m1"
    {
    if( !(l->conc[0]>1) )
	    dis += tab_1[6];					// 60
    }
if( digit_mode )
    {
    int16_t s_dis=dis;
    dis=0;
    DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_1[6],tab_1[7]);		// 60,12
    DIS_CENTER_CONCS(l,r,2,inc,tab_1[6],tab_1[7]);			// 60,12
    dis>>=3;
    dis = s_dis+dis;
    }
else
    {
    DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_1[6],tab_1[7]);		// 60,12
    DIS_CENTER_CONCS(l,r,2,inc,20/*tab_1[6]*/,tab_1[7]);			// 60,12
    }

#ifndef UFA
DIS_BRACES(l,r,tab_1[8]); /* braces [,] */    // 40
#endif

//////if( lmu==1 )	// OLD OLEG
//////if ( (lmu==1) && (dy<22) ) 	// MK 19.01.1922:  stdh25/15 (???)
//////if ( lmu==1 )		// OLD OLEG RESTORE OTLADKA 02.02.1992
if ( (lmu==1) && (dy<22) ) 	// 28.07.1993	SNOWA PROBA ##############
	{
	t = find_beam(l,r,1);
	if( t>=0 && t<3 && r->mount[t]>=l->mount[t] )
		dis += tab_1[10]*(r->mount[t]-r->mount[t]+1);	// *40
		/* similar 'f' */
	}

if( s->dis_nose_1 )
    {
    if( !digit_mode )
	    dis += s->dis_nose_1;
    else
        {
        if( typ_add )
            {
            if( s->width > s->stick_width*2 )
                dis += s->dis_nose_1;
            else
                dis += s->dis_nose_1>>1;
            }
        else
            dis += s->dis_nose_1>>2;
        }
    }
	/* primary discrim */

if( wid>4 && l->mount[0]==1 && l->mount[1]==lmu )
	dis += r->conc[0]*tab_1[11];					// 20

if( l->up_serif>1 && lmu<=lmd && lmu>1 && rmu<2 && rmd>1 && lmd>1 &&
    typ_add )
	dis += tab_1[12];	// min DIS for l-config			// 4

if( s->neck>1 )
	dis += tab_1[13];	// similar 'є'				// 20

if( wid*2 > s->height )
	dis += tab_1[20];	// too wide stick : bad proportions	// 80
/*......................................................................*/
if (rmd<2 && (s->cut_l || s->cut_r))	// 21.10.1993 CUT, NO R.M.D
	dis += 46;			// 254 => 218			// 46
/*......................................................................*/
if (rmd>=2 && lmu>lmd+wid+rmd)		// 17.01.1994 too long nose
	dis += 60;			// for der Laterne		// 60
					// h15/6, i15/28 {'l} => {1}
if( lmu>2*MAX(lmd,rmd) && l->me_pos[0]<4 && lmu>8 && lmu==l->mount[0])
    dis += 60;
/*......................................................................*/
//////m_ex:
dis = MAX(dis,dis_sI);
if(0)
//if( is_digital_string() )
//if( language==LANG_RUSSIAN )
  {
  if( dis>100 )     dis=40;
  else if( dis>60 ) dis=20;
  else if( dis>20 ) dis=4;
  else              dis=0;
  }

return(dis);
}

int16_t	find_neck(STICK_CHARS *l, STICK_CHARS *r,int16_t lim_long)
{
int16_t i,num,minim,m,rr,ll;

for(num=-1,minim=m=i=0;i<3;i++)
	{
	rr=r->conc[i];
	ll=l->conc[i];
	m = ll + rr;
	if( rr>=lim_long && ll>=lim_long &&
	    minim>=m ) /* two flags>lim_long, find last minimum */
		{
		minim = m;
		num = i;
		}
	}

return( num );
}

int16_t	find_beam (STICK_CHARS *l, STICK_CHARS *r,int16_t lim_long)
{
int16_t i,num,maxim,m,rr,ll;

for(num=-1,maxim=m=i=0;i<3;i++)
	{
	rr=r->mount[i];
	ll=l->mount[i];
	m = ll + rr;
	if( rr>=lim_long && ll>=lim_long &&
	    maxim<=m ) /* two flags>=lim_long, find last maximum */
		{
		maxim = m;
		num = i;
		}
	}

return( num );
}

int16_t dis_circle_brace(STICK_CHARS *l,STICK_CHARS *r, STICK_SIGNUMS *s, int16_t typ)
{
int16_t dis=0;
int16_t inc = s->inc;
int16_t sl=l->num_flags,sr=r->num_flags,wid=s->stick_width;
int16_t   lu, ld, ru, rd;

if( l->up_serif && r->up_serif &&
    ( !inc || l->down_serif + r->down_serif>2 && !inc)  )
  dis += tab_circle_brace[0];   /* up serif */
if( l->down_serif && r->down_serif &&
    ( !inc || l->down_serif + r->down_serif>2 && !inc) )
  dis += tab_circle_brace[0];   /* down serif */

if( r->mount[0] && r->mount[4] )    /* '('    */
  {
  if( r->up_serif==0 && r->down_serif==2 )
    dis += tab_circle_brace[1]; /* similar 't' */
  }

if( sl==1 && sr==0 && (l->mount[0] || l->mount[1]) )  /* ')' */
  dis += tab_circle_brace[2];   /* similar '1' */
ru=MAX(r->mount[0],r->mount[1]);
rd=MAX(r->mount[3],r->mount[4]);
lu=MAX(l->mount[0],l->mount[1]);
ld=MAX(l->mount[3],l->mount[4]);
if( typ )
{ // '('
    if( !r->mount[0] && !r->mount[1] && r->mount[3]<2 && r->mount[4]<2 )
        dis += tab_circle_brace[2];   /* similar '1' */
    if( !r->mount[3] && !r->mount[4] && r->mount[0]<2 && r->mount[1]<2 )
        dis += tab_circle_brace[2];   /* similar '1' */
    if( l->conc[1]==0 && l->conc[3]==0 &&
        l->conc[2]>(s->stick_width>2?2:1) )
        dis += tab_circle_brace[6]; /* bad bugles in 1,3 zones  */
	if( !inc && r->mount[0]>2 && r->mount[4]>2 &&
		abs(r->mount[0] - r->mount[4])>MIN(r->mount[0],r->mount[4]) )
		dis += tab_circle_brace[6]; /* differnets mounts  */
    if( s->width>6 &&
        l->conc[0]<2 && l->conc[1]<2 &&
        r->mount[0]<2 && r->mount[1]<2 )
        dis += tab_circle_brace[2]; /* no objects in 1,2 zones  */
    if( s->stick_width>=s->width/2 &&
        r->mount[0]<s->width/2 &&
        r->mount[4]<s->width/2 )
        dis += tab_circle_brace[2]; /* no objects in 1,2 zones  */
    if( inc==0 && abs(ru-rd)>=s->stick_width )
        dis += 40;
    }
else
{ // ')'
    if( !l->mount[0] && !l->mount[1] && l->mount[3]<2 && l->mount[4]<2 )
        dis += tab_circle_brace[2];   /* similar '1' */
    if( !l->mount[3] && !l->mount[4] && l->mount[0]<2 && l->mount[1]<2 )
        dis += tab_circle_brace[2];   /* similar '1' */
    if( r->conc[1]==0 && r->conc[3]==0 &&
        r->conc[2]>(s->stick_width>2?2:1) )
        dis += tab_circle_brace[6]; /* bad bugles in 1,3 zones  */
	if( !inc && l->mount[0]>2 && l->mount[4]>2 &&
		abs(l->mount[0] - l->mount[4])>MIN(l->mount[0],l->mount[4]) )
		dis += tab_circle_brace[6]; /* differnets mounts  */
    if( s->width>6 &&
        r->conc[0]<2 && r->conc[1]<2 &&
        l->mount[0]<2 && l->mount[1]<2 )
        dis += tab_circle_brace[2]; /* no objects in 1,2 zones  */
    if( s->stick_width>=s->width/2 &&
        l->mount[0]<s->width/2 &&
        l->mount[4]<s->width/2 )
        dis += tab_circle_brace[2]; /* no objects in 1,2 zones  */
    if( inc==0 && abs(lu-ld)>=s->stick_width )
        dis += 40;
    }
//if( (!lu || !ld || !ru || !rd) && lu+ld+ru+rd>s->stick_width )
  //  dis += 40;
if( abs(r->mount[4]-l->mount[4])<2 && abs(r->mount[0]-l->mount[0])<2 )
    dis += tab_circle_brace[2];   /* similar '1' */



if( typ ) /* left */
  {
  if( l->conc[0]==0 && l->conc[4]==0 && l->mount[2]==0 )
    dis += tab_circle_brace[3];
  if( l->conc[4]<2 && r->mount[0]<3 )
    dis += tab_circle_brace[3];
    /* no conc in top and bootom left angles     */
  if( sl==0 && sr==0 && r->conc[2]==0 )
    dis += tab_circle_brace[4]; /* solid stick */
  if( s->cut_r )
    dis  += tab_circle_brace[5];  /* ( : not cut right !  O=(+)*/
  if( s->cut_l )
    dis  += tab_circle_brace[5];    // PROBA 24.06.1993
  if( wid<7 && (r->mount[0]==0 && r->mount[1]>1 ||
          r->mount[4]==0 && r->mount[3]>1)  )
    dis += tab_circle_brace[6]; /* bad bugles in 1,3 zones  */
  if( r->mount[0]>wid && r->mount[4]<1 || r->mount[4]>wid && r->mount[0]<1 )
    dis += 40;
  if( l->mount[0]>1 && l->mount[4]>1 || r->conc[0]>1 && r->conc[4]>1 )
    dis += 80;
  dis += s->dis_left_brace;   /* primary dis  */
  }
else    /* right */
  {
  if( r->conc[0]==0 && r->conc[4]==0 && r->mount[2]==0 )
    dis += tab_circle_brace[3];
    /* no conc in top and bootom right angles */
  if( r->conc[4]<2 && l->mount[0]<3 )
    dis += tab_circle_brace[3];
  if( sl==0 && sr==0 && l->conc[2]==0 )
    dis += tab_circle_brace[4]; /* solid stick */
  if( s->cut_l )
    dis  += tab_circle_brace[5];  /* ) : not cut left !  O=|+)*/
  if( s->cut_r )
    dis  += tab_circle_brace[5];    // PROBA 24.06.1993
  if( wid<7 && (l->mount[0]==0 && l->mount[1]>1 ||
          l->mount[4]==0 && l->mount[3]>1)  )
    dis += tab_circle_brace[6]; /* bad bugles in 1,3 zones  */
  if( l->mount[0]>wid && l->mount[4]<1 || l->mount[4]>wid && l->mount[0]<1 )
    dis += 40;
  if( r->mount[0]>1 && r->mount[4]>1 || l->conc[0]>1 && l->conc[4]>1 )
    dis += 80;
  dis += s->dis_right_brace;    /* primary dis */
  }

return(dis);
}

static int16_t dis_l (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s)
{
							// 17.01.1994
int16_t	dis=0, t, lmu, rmu, lmd, rmd, num_l, num_z, dy=s->height;
int16_t	sl=l->num_flags, sr=r->num_flags;
int16_t	wid=s->stick_width, inc=s->inc;
int16_t	tt;		// MK WORK 28.01.1992
int16_t	d_L, d_R;	// 23.02.1993

//	dis = dis_LIMIT_EEM;		// 18.11.1993 (0 or 100);
if( s->incline>256 )
         return 126;

tt = (dy>30) ? 2 : 1;	// used in some places... (see also BUMERANG)

lmu = l->mount[0];  rmu = r->mount[0];
lmd = l->mount[4];  rmd = r->mount[4];
/*......................................................................*/
if( lmu+rmu+lmd+rmd != 0 )  {				// WITH ANGLES:
t = wid>6 ? 1 : 0;
if ( rmu>t && lmu==0 )  {	/* no left upper flags ВДД */
	if( rmu==1 )  dis += r->conc[0] ? tab_l[0] : 0;		// 2,0
		else  dis += r->conc[0] ? tab_l[1] : tab_l[0];	// 8,2
	}

DIS_BRACES(l,r,tab_l[3]);	/* braces [,] */		// *40

//////if( r->down_hook==0 )			// OLD OLEG
if ( !r->up_hook && !r->down_hook )		// 25.02.1993	// for BUMERANG
{	/* not exist right down hook : find brace-configurations */

if (!s->dis_left_brace)			// MK 25.01.1993
	DIS_BRACES_CIRCLE(l,r,wid,tab_l[3]);	// braces (,)	// *40

if( l->mount[4]>2 && r->conc[4]>2 && r->mount[4]==0 &&	// 30.06.1993 PROBA
	(r->conc[4]>wid ||
	 l->down_hook && r->conc[4]>8))	// frac3/1 "POUR" 'U'=>'ll' (last l)
		dis += (1+(r->conc[4]))*tab_l[3];		// *40
}

if ( lmu>1 && lmd==0 && rmu==0 && rmd>1  )		// conf : ї
		dis += inc ? 0 : tab_l[4]*(lmu+rmd-2);	//	  А   // *2

if( lmu==0 && lmd>tt && rmu>tt && rmd==0  )		// conf : Ъ   // BUMERANG
		dis += tab_l[5]*(rmu+lmd-2);		//	  Щ   // *40

if( rmu>0 && r->up_serif>1 )
	DIS_DIFFERENT_SERIFS(l,r,0,2,wid,tab_l[2]);		// *6

if( !inc && l->down_serif && r->down_serif )
	DIS_DIFFERENT_SERIFS(l,r,4,2,wid,tab_l[2]);		// *6

if( l->up_serif && r->up_serif && !inc )  {
	int16_t ser = l->up_serif + r->up_serif + l->down_serif + r->down_serif;

	DIS_CURVE(l,r,4,1,tab_l[8]);				// 10

	if ( (lmu+rmu)<<1>(lmd+rmd) )  {
	    if (wid<4 && lmu==1 && rmu==1 && lmd==1 && rmd==1 && ser>5 ||
		dy<=30 && lmu>1 && rmu>1 && lmd>1 && ser>4 ||
		dy>30  && lmu>2  && rmu>2 && lmd>2 )
			dis += tab_l[10];  /* up and down serifs  */	// 70
	    }
	}

if( inc )  {  /* inc may form false pimple : hard conditions for find serifs */
	if ( rmu>2 && lmu>1 && rmd>1 && lmd>2 )
		dis +=  tab_l[10];	// up and down serifs		// 70
	}
if ( inc  &&  (lmu + wid + wid + rmd) > dy )	// 08.07.1993
		dis += 222;		// stdk11/34 'z' => 'l'		//222

					// 11.01.1994  SPEC.TYPFEACE of liga_i:
if (lmu>=3  &&  l->m_pos[0]>=6  &&	// stdf7/7**,27,39,46 and
    abs (l->m_pos[0]-s->base_2)<=2)	// stdf12/12,25***,26
		dis += 20;		// for der Laterne;

/* ----------------------- STUDY ANGLES ---------------------- */
t = 1;
num_l = (lmu>t)+(rmu>t)+(lmd>t)+(rmd>t);	// number of long flags
num_z = (lmu==0)+(rmu==0)+(lmd==0)+(rmd==0);	// number of empty angles
//////num_z = (lmu<tt)+(rmu<tt)+(lmd<tt)+(rmd<tt);	// for BUMERANG





if ( (num_z==3) &&					// 01.06.1993
     (rmu>0 && s->r_f_symptom && s->l_f_symptom  ||	// Similar 'f'  ||
     (s->lll_nose_1==0  &&				// Similar '1'
      (num_l==1 && rmd==0 && r->mount[3]==0 ||
       (lmu>1 || lmu>0&&dy<17) && s->dis_nose_1==0 && r->mount[3]==0))))
		dis += tab_l[11];	// similar 'f','1'	// 48

if ( (num_l==1) &&				// 09.07.1993, 12.10.1993
     (rmd>MAX(wid,4)) && (l->conc[4]*2<wid))	// stdg19/8(11) 'l' (3:4)
		dis += tab_l[11];	// similar 'L'		// 48

if ( num_l==3 && lmu>0 && s->typ_nose_1==1 &&
     (  (l->m_pos[0]>=MAX(wid,5)-1) ) &&
     lmu >= lmd)					// 31.05.1993
	dis += tab_l[11];	// similar '1'			// 48

if (num_l==3 && abs(s->inc_num)>1 && s->cut_r && rmu<2)	// 10.12.1993	EDIT;
	dis += 40;	// PROBA-40 for 'I' INC,CUT 		// 40

if( num_l==3 && lmu>=4 && rmu>=4 && rmd<=4 )
	dis += tab_l[11];	// LENKA SPEC CASE: 'U'=>"Ll"	// 48

if( num_l==2 && num_z==2 && r->down_serif && l->down_serif )
	dis += tab_l[11];	/* two half-serifs only */	// 48

if ( ( lmd==0 || (lmd==1 && dy>30) || (rmd<<1>dy && lmd<3) ) &&
     (l->conc[4]<1 || l->ce_pos[4]==dy-1) )	// find L-configurations
	{
	if( rmd>wid-1 && wid>4 && (lmu<<1)<rmd && rmu<rmd)	// for BUMERANG
		dis += tab_l[13];				// 80
	else if( !inc )  {
		if ( rmd > (wid*3)>>1 )	dis += tab_l[13];	// 80
		else if ( rmd>=wid )	dis += tab_l[13]>>1;	// 40
		}
	}



if ( s->neck==2 &&
     ( !(l->mount[0] && l->mount[4] && r->mount[4]) )  &&
     ( lmu<2  ||  lmu==2 && l->m_meandr>1 && dy>20 ) )		// MK EDITION
		dis += tab_l[12];				// 20

if ( s->neck==3 )				// 18.02.1993	//m.b.NECK=3
		dis += 100;			// 15.12.1993	// 100

if (dis<tab_l[10] && lmu>tt && rmu>tt && lmd>tt)  dis = tab_l[10];	// 70;

if (s->cut_r &&		// 09.06.1993 PROBA for CUT. "The" (stdj10);
    (r->mount[0] || r->m_meandr==0) &&
    !r->down_serif &&
    l->mount[0]>MAX(wid,5))	// second
	dis += (rmu+2)*80;	// PROBA rmu"*80		// *80
}
/*......................................................................*/
else    {						// EMPTY ANGLES:

	if (s->neck==3)				// 15.12.1993 EDITION:
		dis += 100;						// 100
	else if ( s->neck==2  &&  dy<27 )	// "NO ZIRAFA"
		dis += tab_l[12];					// 20
	else if ( s->neck==1 )
		FOUR_CONC(l,r,tab_l[12]);				// 20

	}
/*......................................................................*/
/* N.B. in group macros (CENTER_FLAG) used ->c_meandr.	*/
/*	If c_meandr==0 and exist near central flag,then	*/
/*	not discrim. For narrow sticks I wont check	*/
/*	near flags . -->> I increase signums c_meandr	*/
/*	for left and right chars of stick		*/
if( wid<3 )  { l->c_meandr++;  r->c_meandr++;  }	// MODIFY c_meandr !!!


				// ABOUT ONE PIXEL		// 23.02.1993
	d_L = d_R = tab_l[7];	// DEFAULT			// =12
	if (s->neck<2)  {	// see OLD VARIANT in dis_I !!!!!!!!!!!!!!
	    if ( !s->cut_l &&
		 (dy>24 && !s->neck)  ||			// 29.07.1993 PROBA
		 (!l->num_long_flags) &&
		  ((wid>=4 && dy>18) || (wid==3 && !l->num_flags)))  d_L = 0;
	    if ( !s->cut_r &&
		 (dy>24 && !s->neck)  ||			// 29.07.1993 PROBA
		 (!r->num_long_flags) &&
		  ((wid>=4 && dy>18) || (wid==3 && !r->num_flags)))  d_R = 0;
	    }

				// add 25.05.1993 for stdf12/11 "Given"
	if (s->neck && l->m_pos[0]>MAX(5,s->base_2))	// ZONE-0 TOO DOWN !!!
		DIS_CENTER_FLAG (l,0,wid,inc,tab_l[6],d_L);	// 60, 12(0)

DIS_FLAGS_L_R(1,tab_l[6]);				// 60, 12(0)/12(0)
DIS_FLAGS_L_R(2,tab_l[6]);				// 60, 12(0)/12(0)

DIS_CENTER_CONC(l,2,inc,tab_l[6],d_L);		// 23.02.1993	// 60,12(0)
DIS_CENTER_CONC(r,2,inc,tab_l[6],d_R);		// 23.02.1993	// 60,12(0)

if (d_L || d_R)  {	// 02.02.1993	PROBA (ABOUT GLISTA)	//  OR ??????
CALC_LENGTH_BEAM_FLAGS (l,r,wid,s, t);
DIS_BEAM (l,r,1,t,wid==1 ? tab_l[9]<<1 : tab_l[9]);		// 140,70
DIS_BEAM (l,r,2,t,wid==1 ? tab_l[9]<<1 : tab_l[9]);	// 01.02.1993: ZONE-2
DIS_BEAM_MK (l,r,2, t, wid==1 ? tab_l[16]<<1 : tab_l[16]);  	// 28,14  01.02.1993: MK
if( !inc )
	DIS_BEAM(l,r,3,t,wid==1 ? tab_l[9]<<1 : tab_l[9]) ;	// 140,70
if (s->width<=4  &&  !inc  &&	// add 09.06.1993 about added thin '(' or ')'
    (l->mount[2] && r->conc[2] && 		// '(' LEFT
     (s->dis_left_brace==0) &&
     (l->m_meandr==1) && (r->c_meandr==1)  ||
     l->conc[2] && r->mount[2] && 		// ')' RIGHT
     (s->dis_right_brace==0) &&
     (l->c_meandr==1) && (r->m_meandr==1)))
	dis += tab_l[19];						// 6
}
else if (l->conc[0]  && r->conc[0]  &&	// 19.11.1993 GLISTA  similar 't'
	 l->mount[1] && r->mount[1] &&	// g14/32(33) "fastest" first 't'
	 wid < 5  &&			// g14/34(37) "Kit"
	 abs (l->mb_pos[1] - s->base_2) < 2  &&
	 abs (r->mb_pos[1] - s->base_2) < 2)
		dis += 20;					// 20

if( wid<3 )  { l->c_meandr--;  r->c_meandr--;  }	// RESTORE c_meandr !!!

if( wid*2 > s->height )
	dis += tab_l[14];	/* too wide stick : bad proportions */	// 80

if( l->mount[1]>0 && r->mount[1]>0 && r->mount[4]>0 && sl==1 && sr==2 )
	dis += tab_l[15];	/* similar 't' */			// 80

if ( (s->neck==1) && (wid>=6) &&
     (
       ( l->conc[0] && r->conc[0] &&	// fax 27/7 "Capability", first 'є'
	 ( l->cb_pos[0]==r->cb_pos[0] && abs(l->cb_pos[0]-s->base_2)<4  ||
	   l->ce_pos[0]==r->ce_pos[0] && abs(l->ce_pos[0]-s->base_2)<4 ) &&
	   l->cb_pos[0]!=l->ce_pos[0] && r->cb_pos[0]!=r->ce_pos[0] )
	 ||
       ( l->conc[1] && r->conc[1] &&	// fax ....................... 'є'
	 ( l->cb_pos[1]==r->cb_pos[1] && abs(l->cb_pos[1]-s->base_2)<4  ||
	   l->ce_pos[1]==r->ce_pos[1] && abs(l->ce_pos[1]-s->base_2)<4 ) &&
	   l->cb_pos[1]!=l->ce_pos[1] && r->cb_pos[1]!=r->ce_pos[1] )
   ) )
		dis += tab_l[17];					// 6

			// 12.02.1993	thin LEG exist:
if ( l->conc[4] && r->conc[4] &&	// j1/16 "CURTIS": 'R'=>"II"
     (l->ce_pos[4] - l->cb_pos[4] + r->ce_pos[4] - r->cb_pos[4]) >= (dy>>1) )
		dis += tab_l[18];					// 20


			// 19.11.1993	l,I  similar 'E' ("THEATRE")
if (wid*2+1>=dy &&	// k11/31 "PERFORMERCE", last 'E' 21*11, wid=10
    r->conc[1] && r->conc[3] &&
    (r->conc[1] | r->conc[3]) > 1)	// all cases, exclude (1,1)
	dis += 100;						// 100
/*......................................................................*/
					// 17.01.1994 MOVE IT TO END:
	//if (dis==0)  dis += mk_dis_for_liga_exm;	// (0 or 2);

	return(dis);
}

static int16_t dis_l_stroked (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s)
{
int16_t	dis=0, t, lmu, rmu, lmd, rmd, num_l, num_z, dy=s->height;
int16_t	sl=l->num_flags, sr=r->num_flags;
int16_t	wid=s->stick_width, inc=s->inc;
int16_t	tt;		// MK WORK 28.01.1992
int16_t	d_L, d_R;	// 23.02.1993

//	dis = dis_LIMIT_EEM;		// 18.11.1993 (0 or 100);

tt = (dy>30) ? 2 : 1;	// used in some places... (see also BUMERANG)

lmu = l->mount[0];  rmu = r->mount[0];
lmd = l->mount[4];  rmd = r->mount[4];
/*......................................................................*/
if( lmu+rmu+lmd+rmd != 0 )  {				// WITH ANGLES:
t = wid>6 ? 1 : 0;
if ( rmu>t && lmu==0 )  {	/* no left upper flags ВДД */
	if( rmu==1 )  dis += r->conc[0] ? tab_l[0] : 0;		// 2,0
		else  dis += r->conc[0] ? tab_l[1] : tab_l[0];	// 8,2
	}

DIS_BRACES(l,r,tab_l[3]);	/* braces [,] */		// *40

if ( !r->up_hook && !r->down_hook )		// 25.02.1993	// for BUMERANG
{	/* not exist right down hook : find brace-configurations */

if (!s->dis_left_brace)			// MK 25.01.1993
	DIS_BRACES_CIRCLE(l,r,wid,tab_l[3]);	// braces (,)	// *40

if( l->mount[4]>2 && r->conc[4]>2 && r->mount[4]==0 &&	// 30.06.1993 PROBA
	(r->conc[4]>wid ||
	 l->down_hook && r->conc[4]>8))	// frac3/1 "POUR" 'U'=>'ll' (last l)
		dis += (1+(r->conc[4]))*tab_l[3];		// *40
}

if ( lmu>1 && lmd==0 && rmu==0 && rmd>1  )		// conf : ї
		dis += inc ? 0 : tab_l[4]*(lmu+rmd-2);	//	  А   // *2

if( lmu==0 && lmd>tt && rmu>tt && rmd==0  )		// conf : Ъ   // BUMERANG
		dis += tab_l[5]*(rmu+lmd-2);		//	  Щ   // *40

if( rmu>0 && r->up_serif>1 )
	DIS_DIFFERENT_SERIFS(l,r,0,2,wid,tab_l[2]);		// *6

if( !inc && l->down_serif && r->down_serif )
	DIS_DIFFERENT_SERIFS(l,r,4,2,wid,tab_l[2]);		// *6

if( l->up_serif && r->up_serif && !inc )  {
	int16_t ser = l->up_serif + r->up_serif + l->down_serif + r->down_serif;

	DIS_CURVE(l,r,4,1,tab_l[8]);				// 10

	if ( (lmu+rmu)<<1>(lmd+rmd) )  {
	    if (wid<4 && lmu==1 && rmu==1 && lmd==1 && rmd==1 && ser>5 ||
		dy<=30 && lmu>1 && rmu>1 && lmd>1 && ser>4 ||
		dy>30  && lmu>2  && rmu>2 && lmd>2 )
			dis += tab_l[10];  /* up and down serifs  */	// 70
	    }
	}

if( inc )  {  /* inc may form false pimple : hard conditions for find serifs */
	if ( rmu>2 && lmu>1 && rmd>1 && lmd>2 )
		dis +=  tab_l[10];	// up and down serifs		// 70
	}
if ( inc  &&  (lmu + wid + wid + rmd) > dy )	// 08.07.1993
		dis += 222;		// stdk11/34 'z' => 'l'		//222

					// 11.01.1994  SPEC.TYPFEACE of liga_i:
if (lmu>=3  &&  l->m_pos[0]>=6  &&	// stdf7/7**,27,39,46 and
    abs (l->m_pos[0]-s->base_2)<=2)	// stdf12/12,25***,26
		dis += 20;		// for der Laterne;

/* ----------------------- STUDY ANGLES ---------------------- */
t = 1;
num_l = (lmu>t)+(rmu>t)+(lmd>t)+(rmd>t);	// number of long flags
num_z = (lmu==0)+(rmu==0)+(lmd==0)+(rmd==0);	// number of empty angles

if ( (num_z==3) &&					// 01.06.1993
     (rmu>0 && s->r_f_symptom && s->l_f_symptom  ||	// Similar 'f'  ||
     (s->lll_nose_1==0  &&				// Similar '1'
      (num_l==1 && rmd==0 && r->mount[3]==0 ||
       (lmu>1 || lmu>0&&dy<17) && s->dis_nose_1==0 && r->mount[3]==0))))
		dis += tab_l[11];	// similar 'f','1'	// 48

if ( (num_l==1) &&				// 09.07.1993, 12.10.1993
     (rmd>MAX(wid,4)) && (l->conc[4]*2<wid))	// stdg19/8(11) 'l' (3:4)
		dis += tab_l[11];	// similar 'L'		// 48

if ( num_l==3 && lmu>0 && s->typ_nose_1==1 &&	// OLD OLEG   &&
     ( /*(pitchsize==0) ||*/ (l->m_pos[0]>=MAX(wid,5)-1) ) &&	// 01.06.1993 &&
     lmu >= lmd)					// 31.05.1993
	dis += tab_l[11];	// similar '1'			// 48

if (num_l==3 && abs(s->inc_num)>1 && s->cut_r && rmu<2)	// 10.12.1993	EDIT;
	dis += 40;	// PROBA-40 for 'I' INC,CUT 		// 40

if( num_l==3 && lmu>=4 && rmu>=4 && rmd<=4 )
	dis += tab_l[11];	// LENKA SPEC CASE: 'U'=>"Ll"	// 48

if( num_l==2 && num_z==2 && r->down_serif && l->down_serif )
	dis += tab_l[11];	/* two half-serifs only */	// 48

if ( ( lmd==0 || (lmd==1 && dy>30) || (rmd<<1>dy && lmd<3) ) &&
     (l->conc[4]<1 || l->ce_pos[4]==dy-1) )	// find L-configurations
	{
	if( rmd>wid-1 && wid>4 && (lmu<<1)<rmd && rmu<rmd)	// for BUMERANG
		dis += tab_l[13];				// 80
	else if( !inc )  {
		if ( rmd > (wid*3)>>1 )	dis += tab_l[13];	// 80
		else if ( rmd>=wid )	dis += tab_l[13]>>1;	// 40
		}
	}

if ( s->neck==2 &&
     ( !(l->mount[0] && l->mount[4] && r->mount[4]) )  &&
     ( lmu<2  ||  lmu==2 && l->m_meandr>1 && dy>20 ) )		// MK EDITION
		dis += tab_l[12];				// 20

if ( s->neck==3 )				// 18.02.1993	//m.b.NECK=3
		dis += 100;			// 15.12.1993	// 100

				// MK-SOPLI 26/28.01.1992 for 'I'=>'l'
if (dis<tab_l[10] && lmu>tt && rmu>tt && lmd>tt)  dis = tab_l[10];	// 70;

if (s->cut_r &&		// 09.06.1993 PROBA for CUT. "The" (stdj10);
    (r->mount[0] || r->m_meandr==0) &&
    !r->down_serif &&
    l->mount[0]>MAX(wid,5))	// second
	dis += (rmu+2)*80;	// PROBA rmu"*80		// *80
}
/*......................................................................*/
else    {						// EMPTY ANGLES:
	if (s->neck==3)				// 15.12.1993 EDITION:
		dis += 100;						// 100
	else if ( s->neck==2  &&  dy<27 )	// "NO ZIRAFA"
		dis += tab_l[12];					// 20
	else if ( s->neck==1 )
		FOUR_CONC(l,r,tab_l[12]);				// 20
	}
/*......................................................................*/
/* N.B. in group macros (CENTER_FLAG) used ->c_meandr.	*/
/*	If c_meandr==0 and exist near central flag,then	*/
/*	not discrim. For narrow sticks I wont check	*/
/*	near flags . -->> I increase signums c_meandr	*/
/*	for left and right chars of stick		*/
if( wid<3 )  { l->c_meandr++;  r->c_meandr++;  }	// MODIFY c_meandr !!!


				// ABOUT ONE PIXEL		// 23.02.1993
	d_L = d_R = tab_l[7];	// DEFAULT			// =12
	if (s->neck<2)  {	// see OLD VARIANT in dis_I !!!!!!!!!!!!!!
	    if ( !s->cut_l &&
		 (dy>24 && !s->neck)  ||			// 29.07.1993 PROBA
		 (!l->num_long_flags) &&
		  ((wid>=4 && dy>18) || (wid==3 && !l->num_flags)))  d_L = 0;
	    if ( !s->cut_r &&
		 (dy>24 && !s->neck)  ||			// 29.07.1993 PROBA
		 (!r->num_long_flags) &&
		  ((wid>=4 && dy>18) || (wid==3 && !r->num_flags)))  d_R = 0;
	    }

//DIS_CENTER_CONC(l,2,inc,tab_l[6],d_L);    // 23.02.1993 // 60,12(0)
//DIS_CENTER_CONC(r,2,inc,tab_l[6],d_R);    // 23.02.1993 // 60,12(0)

// special polish
if( l->mount[2]<3 || r->mount[2]<3 )
  {
  int16_t rm=r->mount[0],lm=l->mount[0];
  if( rm<r->mount[1] ) rm=r->mount[1];
  if( rm<r->mount[2] ) rm=r->mount[2];
  if( lm<l->mount[1] ) lm=l->mount[1];
  if( lm<l->mount[2] ) lm=l->mount[2];

  if( rm<2 && lm<2 )
    dis += 40;  // too small pits or absent mounts
  else if( l->mount[2]<2 && r->mount[2]==0 && r->mount[1]==0 )
    dis += 30;

  if( l->conc[2]>1 && r->conc[2]>1 && wid<7 )
    dis += 20*MAX(r->conc[2],l->conc[2]);

  if( l->mount[1]+l->mount[2]+r->mount[1]+r->mount[2]<3 )
    dis +=10;

  if( l->mount[3]==0 && l->mount[2]==0 )
    dis += 30;

  if( r->mount[4] )
    {
    rm = MAX(r->mount[1],r->mount[2]);
    if( r->mount[4]==1 )
      {
      if( rm<1 )
        dis+=80;
      }
    else
      {
      if( r->mount[4]<3 && rm<2 )
        dis+=80;
      if( r->mount[4]>2 && abs(r->mount[4] - rm)>2 )
        dis+=80;
      }
    }
  if( l->mount[4] )
    {
    if( l->mount[4]==1 )
      {
      if( l->mount[2]<1 )
        dis+=80;
      }
    else
      {
      lm = MAX(l->mount[1],l->mount[2]);
      if( l->mount[4]<3 && lm<2 )
        dis+=80;
      if( l->mount[4]>2 && abs(l->mount[4] - lm)>2 )
        dis+=80;
      }
    }
  }
if( wid<3 )  { l->c_meandr--;  r->c_meandr--;  }	// RESTORE c_meandr !!!

if( wid*2 > s->height )
	dis += tab_l[14];	/* too wide stick : bad proportions */	// 80

if( l->mount[1]>0 && r->mount[1]>0 && r->mount[4]>0 && sl==1 && sr==2 )
	dis += tab_l[15];	/* similar 't' */			// 80

			// 09,10,11.02.1993	CASE NECK=1 wid>=6 (see dis_0xBA)
if ( (s->neck==1) && (wid>=6) &&
     (
       ( l->conc[0] && r->conc[0] &&	// fax 27/7 "Capability", first 'є'
	 ( l->cb_pos[0]==r->cb_pos[0] && abs(l->cb_pos[0]-s->base_2)<4  ||
	   l->ce_pos[0]==r->ce_pos[0] && abs(l->ce_pos[0]-s->base_2)<4 ) &&
	   l->cb_pos[0]!=l->ce_pos[0] && r->cb_pos[0]!=r->ce_pos[0] )
	 ||
       ( l->conc[1] && r->conc[1] &&	// fax ....................... 'є'
	 ( l->cb_pos[1]==r->cb_pos[1] && abs(l->cb_pos[1]-s->base_2)<4  ||
	   l->ce_pos[1]==r->ce_pos[1] && abs(l->ce_pos[1]-s->base_2)<4 ) &&
	   l->cb_pos[1]!=l->ce_pos[1] && r->cb_pos[1]!=r->ce_pos[1] )
   ) )
		dis += tab_l[17];					// 6

			// 12.02.1993	thin LEG exist:
if ( l->conc[4] && r->conc[4] &&	// j1/16 "CURTIS": 'R'=>"II"
     (l->ce_pos[4] - l->cb_pos[4] + r->ce_pos[4] - r->cb_pos[4]) >= (dy>>1) )
		dis += tab_l[18];					// 20

			// 19.11.1993	l,I  similar 'E' ("THEATRE")
if (wid*2+1>=dy &&	// k11/31 "PERFORMERCE", last 'E' 21*11, wid=10
    r->conc[1] && r->conc[3] &&
    (r->conc[1] | r->conc[3]) > 1)	// all cases, exclude (1,1)
	dis += 100;						// 100
/*......................................................................*/
					// 17.01.1994 MOVE IT TO END:
	//if (dis==0)  dis += mk_dis_for_liga_exm;	// (0 or 2);

	return(dis);
}

int16_t	dis_t (STICK_CHARS *l, STICK_CHARS *r, STICK_SIGNUMS *s, int16_t sign_f)
{							// 10.01.1994
int16_t i, dis=0, t, lm, rm, dy=s->height, dx=s->width, t2;
int16_t	nl=l->num_long_flags, nr=r->num_long_flags;
int16_t	sl=l->num_flags, sr=r->num_flags;
int16_t	wid=s->stick_width, inc=s->inc;
int16_t	tpos;	// 18.11.1993 (old - use the same {t}, how for zone)

t = find_beam (l, r, 1);
/*......................................................................*/
if( t<0 )  {						// BAD BEAM:
	t=find_beam(l,r,0);
	if( t<0 )  {
		dis += tab_t[0];  /* no flag long >=1  */		// 40
		lm = rm = 0;	// MK 23.03.1993 about SHURIK g20/51 s->r ERR
		}
	    else  { lm = l->mount[t];  rm = r->mount[t]; }	// 23.03.1993

	if( lm+rm>1 )  {	/* first flag>=2, second flag=0 */
		if (lm)	dis += lm*tab_t[1];			// *20
		else
		  if ((s->cut_l || s->cut_r) && rm<3 &&	// 19.10.1993
		      rm>r->mount[4])			// 20.10.1993
				// 20.11.1993 fax8/19(21) "using" 'n' => "tl"
			dis += (wid>=5 ? 88 : 44);		// 88/44
		else	dis += tab_t[1];			// 20

		if( t==0 && r->mb_pos[0]<2 &&
		    !(s->base_2!=-1 &&			// 18.11.1993
		      abs(r->m_pos[0]-s->base_2) < (/*fax1x2 ? 3 :*/ 2 )) &&
		    !(r->m_pos[0]>dy/6&&r->me_pos[0]>dy/3) )
			dis += tab_t[24];				//100
				//long right flag belong 0-zone
					  /* MANY KNOTs !!! */
		}
	else	{		/* first flag=1, second flag=0  */
		if( rm )
			dis += wid>2 ? tab_t[2] : tab_t[20] ;	     // 50,4
		else
			dis += tab_t[17];			      // 120
		if( wid<3 )  {
			if( t==2 && r->mount[3] )
				dis += tab_t[20];			// 4
			if( r->m_meandr>2 ||
			    r->m_meandr==2 && r->mount[4]==0 )
				dis += tab_t[25];			// 40
			}
		if( rm && lm==0 && t>0 && l->mount[0] )
			dis += tab_t[2];				// 50
		}
	}
/*......................................................................*/
else	{						// NORMAL BEAM:
	lm = l->mount[t];   rm = r->mount[t];

				// MK PROBA 03.02.1993 about 'r'=>'t'
	if ((t==0) && (wid>=4) && (lm<=1) && (rm>=4) &&
	    (r->mount[4]<5))	// 07.01.1994 EDITION
		dis += 22;	// MK PROBA 04.02.1993			// 22

if (1/*!fax1x2*/)	{				// OTLADKA 20.01.1993
	if ( (l->me_pos[t] - l->mb_pos[t]) > dx &&	// OLD ###########
	     (r->me_pos[t] - r->mb_pos[t]) > dx )
		dis += tab_t[21]; /* bad overlay beam-flags */	      // 100
	}
if (0/*fax1x2*/)	{				// OTLADKA 20.01.1993
	if ( ((l->mb_pos[t] - r->me_pos[t]) > 1) ||	// NEW PROBA
	     ((r->mb_pos[t] - l->me_pos[t]) > 1) )
		dis += tab_t[21]; /* bad overlay beam-flags */	      // 100
	}

/* N.B. in TC-interface base lines are not known */
  if( s->base_2!=-1 )  {  /* base lines known */
    t2=2;
#ifdef RUS_ENG_LANG
if( multy_language ) t2=4;
#endif
	    if (s->base_2>MAX(s->height/6,3) &&
    (MAX(l->m_pos[t],r->m_pos[t]) < s->base_2-t2 ||
		 MAX(l->mb_pos[t],r->mb_pos[t]) > s->base_2+4))	// 19.10.1993
			dis += tab_t[18];			      // 100
	    else {/*if (inc_num_EEM>=2  &&		// 18.11.1993 similar '7'
		     t==0  &&			// j10/29 "08/77" first '7'
		     l->mb_pos[0] <= 1 &&
		     l->me_pos[0] + 2 <= r->me_pos[0]  &&
		     l->me_pos[0] + 2 <= s->base_2)
			dis += tab_t[18];			      // 100*/}
	    }

	if( lm+rm>3 )	/* flags : 1,2  2,1 2,2  ... */
		{	/* long flags */
		int16_t im,ma;
		if( lm*3>rm*4 && lm>3 )
			dis += tab_t[3];  /* left>right */		// 4

		for(i=0;i<3;i++)  {
			if( i==t )continue;
			if( l->mount[i]>=l->mount[t]) break;
			}

					// 26.08.1993	INCREASE for "ft"
		if (i==0  &&  nl==2  &&  nr==2  &&
		    t>0  &&  s->cut_l  &&
		    (!inc && l->mount[0]==lm || inc && l->mount[0]>lm)  &&
		    rm>=3  &&  r->mount[4]>=3)		// second variant
			return (-444);	// NOTA BENE:  INCREASE !!!!!!!!!!
			/************************************************/

		if ( i<3 )  {  /* exist left top long flag  */
			if ( l->mount[t]>1 )
				dis += tab_t[4];			// 30
			else if( !inc )
				dis += tab_t[4]>>2;			// 7
			}
			/* exist big flag and beam */

		for (im=ma=-1,i=0;i<3;i++)  {
			if( i==t )continue;
			if( r->mount[i]>=r->mount[t]) break;
			if( r->mount[i]>ma)
				ma = r->mount[(im=i)];
			}

		if( i<3 )  {  /* exist long right upper flag */
			dis += ( r->me_pos[i]-r->mb_pos[i])*tab_t[14]; // *15
			}

		if( s->width>4 && im==0 && ma>=r->mount[t]-1 &&
		    !(ma==1&&inc) )
			dis += tab_t[23];				// 20
		}

	else if( lm + rm==2 )  	{
		for(i=0;i<3;i++)  {
			if( i==t )continue;
			if( l->mount[i]>1) break;
			if( r->mount[i]>2) break;
			}
		if( i<3 )
			dis += tab_t[4];				// 30
			/* exist big flag and beam */
		if( wid>4 )
			dis += tab_t[5]; // short beam and wide stick	// 2
		if( s->width>2 && !(l->mount[3]==1&&l->conc[4]) )
			dis+=((sl>1)+(sr>2))*
			     (inc ? tab_t[6] :				// *2
				    (s->width<4?tab_t[26]:tab_t[7])); //*20,40
		else if( sl>1 || sr>2  )
			dis += tab_t[20];				// 4
		/* meandr and narrow beam */
		if( inc && wid<5 &&
		    (l->me_pos[t]<=r->mb_pos[t]||r->me_pos[t]<=l->mb_pos[t]))
			dis += tab_t[22];				// 40
		}
	}	// IF ELSE: NORMAL BEEM;
/*......................................................................*/
	dis += ((nr>3)+(nl>1))*tab_t[8];			       // *24
	/* too many left or right flags */
	if( l->mount[4]>1 && !inc || l->mount[4]>2 && inc )
		dis += tab_t[10] ;  /* left downer flag */		// 40
if( t<2 )
	DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_t[11],tab_t[12]);	     //40,4

if( t>0 && t<3 && dx<<2 < dy*3 )
	{  /* KNOT : find big flags of beam - part bold 'H' */
	if( l->num_flags==1 && l->me_pos[t]-l->mb_pos[t]+1>dy/3 &&
	    s->cut_l   ||
	    r->num_flags==1 && r->me_pos[t]-r->mb_pos[t]+1>dy/3 &&
	    s->cut_r   )
		dis += tab_t[28];					// 40
	}

if( l->up_serif==2 && l->down_serif==2 &&
    r->up_serif==0 && r->down_serif==0 &&  t<2 )
	dis += tab_t[15];  /* similar    ],) */				// 60

//////t = MAX( l->m_pos[t], r->m_pos[t])+1;
tpos = MAX( l->m_pos[t], r->m_pos[t])+1;	// 18.11.1993
if( tpos>dy>>1 )
	dis += tab_t[13]*(tpos-(dy>>1)); /* too low beam */	     // *16
/* ----------------- FIND f-CONFIGURATIONS ------------------- */

if (t>0 &&		// 18.11.1993 PROBA for f10/15 "of"
    r->mount[0] > (wid>2 ? 2 : 1) &&
    l->conc[0]>0 )
	dis += tab_t[14];   /* head : similar to 'f' */			// 14
		// NB: include THIN STICK, but EXCLUDE SOME OTHER CASES;


if ( s->r_f_symptom==2 )	// similar 'f'		// 10.02.1993
	if (r->up_serif==2)
		dis += tab_t[16];					// 70
	else if ( r->up_serif==1 && s->l_f_symptom )
		dis += tab_t[30];					// 16
	else	dis += tab_t[31];					// 8

if( s->l_f_symptom==2 && s->r_f_symptom==2 && sign_f &&
    r->mount[4]==0 )
	dis += tab_t[19];    /* similar 'f' */				// 10

if (l->down_hook ||			// 04/07.06.1993
    l->mount[4] >= 2 + inc + 0/*fax1x2*/)	// 07.06.1993
	dis += tab_t[16];		// similar 'f'			// 70
	// NB: I can use SERIF==2, SERIF==1, !INC in combinations with L.M.4

/* ----------------- FIND l-CONFIGURATIONS ------------------- */

if( l->m_meandr==1 && l->num_flags==1 && l->mount[4] &&
    r->m_meandr==1 && r->num_flags==1 && r->mount[0] )
	dis += tab_t[27];   /* head : similar to 'f' */			// 40

/*......................................................................*/
{				// MK: TOO int32_t HOOK: for "t." ==> 't'

	if ((r->mount[4]>=lm+wid+rm) &&		// 10.03.1993
	    (r->m_pos[4]+wid+3 > dy))		// 11.03.1993 no big hook
		dis += tab_t [29];				     //80=>100
}


	if (s->neck>=2)			// 20.10.1993
		dis += 20;						// 20
/*......................................................................*/
						// 10.01.1994	BONUS:
	if (dis==0	&&	// NO DISCRIM;
	    s->t_TOP	&&	// t^
	    l->mount[0]	&&	// LMU EXIST;
	    l->conc[4]	&&	// LDC EXIST;
	    s->base_2mk>0  &&	// BASE_2 1 or 2;
	    s->base_2mk<=2)
		dis = -32;	// BONUS !!!!!!!!!!!!
/*......................................................................*/
return(dis);
}

/* letter [ */            // 22.11.1993
int16_t dis_l_sq_brace(STICK_CHARS *l,STICK_CHARS *r, STICK_SIGNUMS *s)
{
int16_t dis=0, lmu, rmu, lmd, rmd, dy=s->height;
int16_t wid=s->stick_width, inc=s->inc;

lmu = l->mount[0];  rmu = r->mount[0];
lmd = l->mount[4];  rmd = r->mount[4];

if (rmu==0 ||       // 29.09.1993
    rmu==1 && wid>4)  {
  dis += tab_sq_brace[0];   // no upper right flag  // 4 => 44
  if (r->mount[1]>=2)   // MK ADD;
    dis += tab_sq_brace[5]; // for 't' => '[' // +88 = 132
  }

else
  if (r->m_pos[0]>=2  &&  // 29/30.09.1993  for 't'=>'[' MK ADD:
      r->mb_pos[0]>0)  {  // 19.11.1993 for j2/14 '[' (0-2-4)
  dis += tab_sq_brace[0];   // FALSE U.R. FLAG  // 44
  if (r->m_pos[0]>=3)
    dis += tab_sq_brace[0]; // more FALSE;    // 44
  if (r->mb_pos[0]>=2)
    dis += tab_sq_brace[5]; // more FALSE   // 88
//////  if ((l->conc[5]) && (dy - l->cb_pos[5] >= 2)) #############
  if ((l->conc[4]) && (dy - l->cb_pos[4] >= 2)) // 19.11.1993
    dis += tab_sq_brace[5]; // more FALSE   // 88
  }

else if (rmd-rmu>=2  &&   // 29.09.1993 for stdc20/13 "bit"
//////   ((l->conc[5]) && (dy - l->cb_pos[5] >= 2)))  #############
   ((l->conc[4]) && (dy - l->cb_pos[4] >= 2)))  // 19.11.1993
    dis += tab_sq_brace[5]; // for 't' => '[' // 88

//////else  DELETE 29.09.1993
if( lmu>=rmu )
  dis += (lmu-rmu+1)*tab_sq_brace[1]; /* bad paar flags */  // *8

if ( rmd==0 )
  dis += tab_sq_brace[0];   // no downer right flag // 4 => 44

//////else  DELETE 29.09.1993
if ( lmd>=rmd )
  dis += (lmd-rmd+1)*tab_sq_brace[1]; /* bad paar flags */  // 8

DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_sq_brace[2],tab_sq_brace[3]);  // 20,8

if( l->conc[0]>1 && l->ce_pos[0]>1 )
  dis += tab_sq_brace[4];   /* similar '(' */   // 26
if( l->conc[4]>1 && l->cb_pos[4]<dy-1 )
  dis += tab_sq_brace[4];   /* similar '(' */   // 26
if (wid==1 && l->conc[0] && l->conc[4]) // 22.11.1993 f11/21 "(03)"
  dis += tab_sq_brace[4];   // similar '('      // 26

if ( l->conc[3]>1 && r->conc[3]>1 ) // Oleg : 05.17.1994 pdem0/18
  dis += tab_sq_brace[4];   // similar '!'

  if( s->cut_l )  dis  += tab_sq_brace[7];    // 24.06.1993
  if( s->cut_r )  dis  += tab_sq_brace[7];

/********************************* 19.11.1993 DELETE IT (PROBA)
if ((rmu==1 && (rmd)) ||  // 29.09.1993 for stdh6/7 'l' ("celebrate")
    (rmd==1 && (rmu)))  {
  if (wid>=3)
    dis += tab_sq_brace[0]; // WIDE STICK with THIN   // 44
  if (r->num_flags>2)
    dis += tab_sq_brace[5]; // WIDE STICK with THIN   // 88
  }
  *******************************************************/

return(dis);
}
/*----------------------------------------------------------------------*/
/* letter ] */            // 29.09.1993 MK
int16_t dis_r_sq_brace(STICK_CHARS *l,STICK_CHARS *r, STICK_SIGNUMS *s)
{
int16_t dis=0,lmu,rmu,lmd,rmd,dy=s->height;
int16_t wid=s->stick_width,inc=s->inc;

lmu = l->mount[0];  rmu = r->mount[0];
lmd = l->mount[4];  rmd = r->mount[4];

if (lmu==0)  {
  dis += tab_sq_brace[0];   // no left upper flag // 4 => 44
  if (lmd<=1)
    dis += tab_sq_brace[6]; // TERRIBLE     // 222
  }

//////else  DELETE 29.09.1993
if (rmu>=lmu)
  dis += (rmu-lmu+1)*tab_sq_brace[1]; /* bad paar flags */

if (lmd==0)  {
  dis += tab_sq_brace[0];   // no left downer flag  // 4 => 44
  if (lmu<=1)
    dis += tab_sq_brace[6]; // TERRIBLE     // 222
  }

//////else  DELETE 29.09.1993
if (rmd>=lmd)
  dis += (rmd-lmd+1)*tab_sq_brace[1]; /* bad paar flags */

DIS_CENTER_FLAGS(l,r,2,wid,inc,tab_sq_brace[2],tab_sq_brace[3]);

if( r->conc[0]>1 && r->ce_pos[0]>1 )
  dis += tab_sq_brace[4];   /* similar ')' */
if( r->conc[4]>1 && r->cb_pos[4]<dy-1 )
  dis += tab_sq_brace[4];   /* similar ')' */

dis +=  s->dis_up_dot;      /* similar 'j' */

  if( s->cut_l )  dis  += tab_sq_brace[7];    // 24.06.1993
  if( s->cut_r )  dis  += tab_sq_brace[7];

if (wid>=4  &&      // 29.09.1993 for stdb13/35 'l' ("cornmeal")
    (lmu==1 && (lmd)) ||
    (lmd==1 && (lmu)))
  dis += tab_sq_brace[0];   // WIDE STICK with THIN   // 44

if (r->up_serif==2)
  dis += tab_sq_brace[0];           // 44

if (r->down_serif==2)
  dis += tab_sq_brace[0];           // 44

if (r->num_flags>=3)
  dis += tab_sq_brace[0];           // 44

if (wid<=2  && r->num_flags>=2 && l->num_flags>=3)
  dis += tab_sq_brace[0];           // 44

return(dis);
}

/* rough propability of letter 'f' */
static uchar config_f( STICK_CHARS *l,STICK_CHARS *r)
{
uchar ret=3,lf=(l->mount[0]!=0)+(l->mount[1]!=0)+(l->mount[2]!=0),
	rf=(r->mount[0]!=0)+(r->mount[1]!=0)+(r->mount[2]!=0),
	br_l=(r->mount[0]&&r->mount[4]&&l->mount[2]);
if( rf<2 && r->m_meandr==2 && r->mount[4]==0 && r->mount[3]==0 )
	rf=2;
if( lf==1 && rf==2 || rf>1 && l->mount[4] ||
    lf<=1 && l->conc[0] && r->mount[0]  &&
	     !(l->conc[4]&&r->mount[4]) &&
	     !r->conc[2] && !br_l
    )
	ret=2;
else if( lf>1 || lf==1 && rf==1  )
	ret=1;
return(ret);
}
/*----------------------------------------------------------------------*/
/* rough propability of letter 'r' */
static uchar config_r( STICK_CHARS *l,STICK_CHARS *r)
{
uchar ret=3,lf=(l->mount[0]!=0)+(l->mount[1]!=0)+(l->mount[2]!=0),
	rf=(r->mount[0]!=0)+(r->mount[1]!=0);
if( lf==0 && rf==1 )
	ret=r->mount[0]?2:1;
else if( lf==1 && l->mount[0] && r->mount[0]>l->mount[0] )
	ret=1;
return(ret);
}
/*----------------------------------------------------------------------*/
/* rough propability of letter 't' */
static uchar config_t( STICK_CHARS *l,STICK_CHARS *r)
{
uchar ret=3,lf=(l->mount[0]!=0)+(l->mount[1]!=0)+(l->mount[2]!=0),
	rf=(r->mount[0]!=0)+(r->mount[1]!=0)+(r->mount[2]!=0);

if( lf==1 && l->m_meandr==1 && rf==1 )
	ret=2;
else if( lf==1 && rf==2 || rf==1 && lf==0 )
	ret=1;
return(ret);
}
/*----------------------------------------------------------------------*/
/* rough propability of letter '1' */
static uchar config_1( STICK_CHARS *l,STICK_CHARS *r)
{
uchar ret=3;

if( r->mount[0]==0 && r->mount[1]==0 && r->mount[2]==0 &&
    r->mount[3]==0 && r->mount[4]==0 && l->mount[2]==0 &&
    l->mount[3]==0 && l->mount[4]==0 )
    {
    if( l->mount[1]==0 && l->mount[0] )
	ret = 2;
    else if( l->mount[0]==0 && l->mount[1] )
	ret = 1;
    }
return(ret);
}
uchar similar_wide_frt1(STICK_CHARS *left_chars,STICK_CHARS *right_chars)
{
uchar ret=0;
ret |= config_f(left_chars,right_chars);
ret |= config_r(left_chars,right_chars)<<2;
ret |= config_t(left_chars,right_chars)<<4;
ret |= config_1(left_chars,right_chars)<<6;
return(ret);
}

int16_t similar_l_stroked (STICK_CHARS *l,STICK_CHARS *r,STICK_SIGNUMS *s)
{
return(dis_l_stroked(l,r,s)<MAX_ADD_DIS);
}
//////////////////////////////////////////////////////////////////////
// typ_thin_stick : study thin stick (width<=4) for EVENTS-pass
//     *lin - list of intervals, dy - height, dx - width of c_comp
//     return : propabilites letters 'f','r','t'  ( 0b00ttrrff or 0 )
//              tt,rr,ff = 01(bad),10(good),11(verybad)
//////////////////////////////////////////////////////////////////////
uint16_t typ_thin_stick(lnhead *lin,int16_t dy,int16_t dx)
{
int16_t wide,opt,left_mode,right_mode,inc_v, nc, num_angles,f_a;
uint16_t ret=0;
STICK_CHARS left_chars,right_chars;
STICK_SIGNUMS signums;

memset(GL_left0, 0xFF, dy );
memset(GL_right0,0xFF, dy );

if( (nc = lnhead_to_centers(lin,dx,GL_center,GL_left0,GL_right0))==0 )
    return(0);  // too many intervals

if ( num_of_lines (GL_center, nc, dy, GL_hist_int) )
	return(0); // non stick:>1lines

num_angles =   sizeof(stick_inc)/sizeof(stick_inc[0]);
f_a=first_tg(stick_inc, num_angles, nIncline );
num_angles=MIN(LIMIT_OF_ANGLES,
           sizeof(stick_inc)/sizeof(stick_inc[0])-f_a-1);

if( make_center_line_dif(GL_center, (int16_t)(nc-(GL_center[nc-1].len==1)),
		     GL_left0, GL_right0,dy, dx,
		     &stick_inc[f_a], num_angles, GL_tab_angle,
         0, 0, 1, 0, &wide, GL_hooks, &inc_v, 1) ) // with correct
	return(0); // abnormal set of ceneters : silmular to (,) or
		       // not exist center-line

opt = ((MAX( GL_tab_angle[0], GL_tab_angle[dy-1] ))>>1)<<1;
abris_expansion (GL_left0, GL_right0, dy, dx, GL_tab_angle);

set_stick_char (GL_left0, GL_right0, GL_hooks, dy, dx, opt, wide,
		(int16_t)(opt - MIN ( GL_tab_angle[0], GL_tab_angle[dy-1] )),
		0, 0, 0, 0,  0,
		&left_chars, &right_chars, &signums,
		&left_mode, &right_mode);

ret = similar_wide_frt1(&left_chars,&right_chars);


if (dy>16 && dx<4)
  ret |= 0x0C;  // 09.07.1993 SUPPRESS 'r'

if (language == LANG_POLISH &&
    similar_l_stroked(&left_chars,&right_chars,&signums) )
  ret |= 0x300;
return(ret);
}
