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
#include "string.h"
#include "stdlib.h"

#include "minmax.h"

struct  shift_inform   /* work struct for function find_opt_shift */
	{
  int16_t max,
	    imax,
	    opt,
	    wide,
	    over;
	};

extern  uchar GL_hist [2*LIMIT_HEIGHT];  // array for histogramm
extern	center_interval GL_cent [LIMIT_HEIGHT];	// center of intervals
extern  uchar GL_left1 [LIMIT_HEIGHT], // auxiliary left and
	     GL_right1[LIMIT_HEIGHT];	//    right abris-arrays
static int16_t inc_periods[LIMIT_HEIGHT];
int inc_num_EEM ;
uchar inc_char_EEM;
/////////////////////// Functions prototypes //////////////////////////
int16_t make_center_line_dif (center_interval center[], int16_t nc,
      uchar left[], uchar right[],
      int16_t  dy, int16_t dx, INC_BASE *angles[], int16_t num_angles,
      int16_t  tab_angle[],
			Bool16 comp_wide, Bool16 sig_T, Bool16 sig_f, Bool16 sig_r,
      int16_t  *wide, int16_t hooks[], int16_t *inc_v, uchar enable_correct);

int16_t centers_len_to_hist(center_interval center[],int16_t nc,int16_t dy,int16_t dx,
        uchar hist[]);
static int16_t max_center_hist_new (uchar fun[], int16_t n,
    center_interval center[], int16_t nc, int16_t tab_angle[],
    int16_t typ)  ;
static int16_t calc_LENs_LIMITED (uchar fun[], int16_t n, int16_t sum) ;
static void make_tab_angles (const INC_BASE *angle, int16_t hei, int16_t ang[]);
static void make_hist (center_interval center[],int16_t nc, uchar hist[], int16_t ang[],
          int16_t dx,int16_t len, int16_t typ);
static void make_hist_centers_LIMITED (center_interval center[], int16_t nc,
    uchar hist[], int16_t ang[], int16_t dx, int16_t len_limit) ;
static int16_t width_of_hist(uchar hist[],int16_t len);
 int16_t overlay_interval(center_interval center[],int16_t nc, int16_t col,
          int16_t typ,  int16_t tab_angle[]);
void filtr_short(uchar fun[],int16_t n,int16_t lev);
static int16_t enable_shift(uchar left[],uchar right[],int16_t h,int16_t w,int16_t inc[]);
int16_t abris_convexity(uchar fun[],int16_t n,int16_t w);
static int16_t num_of_short_int(center_interval center[],int16_t nc,int16_t lim);
static int16_t find_opt_shift(INC_BASE *angles[],int16_t num_angles,
        int16_t dy, int16_t dx, center_interval center[], int16_t nc,
        int16_t wid,Bool16 sig_T,Bool16 sig_f,
        int16_t tab_angle[],
        uchar hist[], struct shift_inform *res);
Bool16 bad_overlay(int16_t over,int16_t width,int16_t dy,int16_t wide,Bool16 c_f);
static int16_t make_result (int16_t n, int16_t opt, int16_t res[]);
static void compress_centers(center_interval center[],int16_t nc,int16_t ang[],int16_t n,
          center_interval cent[],int16_t hooks[]);
static int16_t correct_result (center_interval cent[], int16_t inc[], int16_t dy);
int16_t find_minimum(uchar fun[],int16_t n,uchar *_imin);
static int16_t calc_inc_periods (int16_t inc[], int16_t dy, int16_t inc_periods[]);
int16_t max_center_hist (uchar fun[], int16_t n,
    center_interval center[], int16_t nc, int16_t tab_angle[], int16_t typ);
static int16_t correct_result_BACK (center_interval cent[], int16_t inc[], int16_t dy);
static int16_t abris_inc_line (uchar fun[], int16_t n, int16_t inc[], Bool16 sig_left) ;
static int16_t correct_result_MK (center_interval cent[], int16_t inc[], int16_t dy) ;
////////////////////////////////////////////////////////////////////////

int16_t make_center_line_dif (center_interval center[], int16_t nc,   // 22.11.1993
      uchar left[], uchar right[],
      int16_t  dy, int16_t dx, INC_BASE *angles[], int16_t num_angles,
      int16_t  tab_angle[],
			Bool16 comp_wide, Bool16 sig_T, Bool16 sig_f, Bool16 sig_r,
      int16_t  *wide, int16_t hooks[], int16_t *inc_v, uchar enable_correct)
{
struct shift_inform tmp;
int16_t imax=-1;
int16_t opt, wd, ov, wid, en, optmax, i, dop_opt, dop_opt_BBB_1, dop_opt_BBB_2;
int16_t brace=0, ny;
int16_t k, sum, len_hist_LENs, mk_len_limit;

//////mkm1 = dx;
//////mkm2 = dy;

	sig_r = sig_r;	// 17.11.1993: NOT USED NOW (for NO WARNING);
/*......................................................................*/
					// CALCULATIONS for LEN's:
//////////wid = centers_len_to_hist (center, nc, dy, dx, hist_BBB);
len_hist_LENs = centers_len_to_hist (center, nc, dy, dx, GL_hist);
//wid = max_center_hist(hist,wid,center,nc,tab_angle,0)>>1; // calc average width
//wid = max_center_hist_new(hist_BBB,wid,center,nc,tab_angle,0)>>1;//19.02.1993
wid = max_center_hist_new (GL_hist,len_hist_LENs,center,nc,tab_angle,0) >> 1;
					// NB: tab_angle UNDEFINED now;
//////mkm3 = wid;
/*......................................................................*/
	mk_len_limit = 0;		// 15.10.1993 work with TOO BIG WIDE
//////	if (wid*2<dy)  goto m_OK;	// see k11/14 'F' 21*14 wid=11
	if (wid*2+1<dy)  goto m_OK;	// 19.11.1993 k11/31 'F' 21*11 wid=10
//////	for (k=wid*2/3,sum=0; k>=0; k++)  sum += GL_hist [k];  // ############
	for (k=wid*2/3,sum=0; k>=0; k--)  sum += GL_hist [k];  // N small LENs
	if (sum*3<nc)  goto  m_OK;

	mk_len_limit = wid*2/3;		// used here (LENs) and for CENTERs
	wid = calc_LENs_LIMITED (GL_hist, mk_len_limit, sum) >> 1;

m_OK:	//dis_LIMIT_EEM = mk_len_limit ? 100 : 0;	// 18.11.1993
/*......................................................................*/
if ( wid*5>4*dx && dx*3>dy*2 && comp_wide  ||	// OLD CONDITION
     wid*4>dy*3)				// 21.10.1993 SECOND CONDITION
	return(4); /* no stick : wide c_comp */

make_tab_angles (angles[0], dy, tab_angle);	// NB: This is FIRST FORM!
/*......................................................................*/
					// CALCULATION for CENTER's:
if (mk_len_limit==0)
	make_hist (center,nc, GL_hist, tab_angle, dx, wid,(int16_t) ( wid>4&&sig_T==0));
else	make_hist_centers_LIMITED (center, nc, GL_hist, tab_angle,
				dx, mk_len_limit);
/* calc hist  for angle=0 */	// NB: really - FOR BEGIN ANGLE !!!

//////opt=max_center_hist(hist,dx<<1,center,nc,tab_angle,(wid<<1)>dy);//OLEG
//////if (!fax1x2)			// MK PROBA 15.01.1993
//////	opt=max_center_hist(hist,dx<<1,center,nc,tab_angle,(wid<<1)>dy);
//////   else				// PROBA 19.02.1993: in ANY CASE:
					// BEFORE 14.10.1993:
//////	opt=max_center_hist_new(hist_BBB,dx<<1,center,nc,tab_angle,(wid<<1)>dy);
					// 14.10.1993: (k11/14, h14/1):
	opt=max_center_hist_new(GL_hist,(int16_t) (dx<<1),center,nc,tab_angle, 0); // ???

optmax = GL_hist [opt>>1];		// find maximum in hist
//////dop_opt = MAX( hist_BBB[(opt>>1)-1],hist_BBB[(opt>>1)+1]);  MAC.ERROR
dop_opt_BBB_1 = (opt<=2)      ? 0 : GL_hist[(opt>>1)-1];	// 09.04.1993
dop_opt_BBB_2 = (opt>=4*dx-2) ? 0 : GL_hist[(opt>>1)+1];
dop_opt = MAX (dop_opt_BBB_1, dop_opt_BBB_2);
/*......................................................................*/
wd = width_of_hist (GL_hist,(int16_t)( dx<<1 ));
ov = overlay_interval (center, nc, (int16_t) (opt>>2), (int16_t) (opt%4==0), tab_angle) ;

			// here was DEBUG_GRAPH

ny = dy - 2;
memcpy (&GL_left1[0], &left[1], ny);
/*
{
int i;
for(i=0;i<ny;i++)
    {
    GL_left1[i]=left[1+i];
    }
}
*/
memcpy (&GL_right1[0],&right[1],ny);   /* save for filtration */

if( dy>20 )
	{  /* this manipulations for brace recognize only */
	filtr_short (GL_left1, ny, 1);
	filtr_short (GL_right1,ny, 1);
    //filtr_short (left, ny, 1);
	//filtr_short (right,ny, 1);
	}
en = enable_shift (GL_left1, GL_right1, ny, dx, &tab_angle[1]);
/*......................................................................*/
							// 16.11.1993:
if (en==3  &&			// LEFT or RIGHT vert Line
    angles[0]->num!=0)	{	// and a priory NONZERO INC
  memset (tab_angle, 0, dy*sizeof(int16_t));  // ZEROES to tab_angle and
	if (mk_len_limit==0)		// repeat CALCULATION for CENTER's;
		make_hist (center,nc, GL_hist, tab_angle, dx, wid,
							(int16_t) (wid>4&&sig_T==0));
	else	make_hist_centers_LIMITED (center, nc, GL_hist, tab_angle,dx, mk_len_limit);
	opt=max_center_hist_new (GL_hist, (int16_t) (dx<<1), center, nc, tab_angle, 0);
	optmax = GL_hist [opt>>1];	// find maximum in hist
	dop_opt_BBB_1 = (opt<=2)      ? 0 : GL_hist[(opt>>1)-1];
	dop_opt_BBB_2 = (opt>=4*dx-2) ? 0 : GL_hist[(opt>>1)+1];
  dop_opt = MAX (dop_opt_BBB_1, dop_opt_BBB_2);
	wd = width_of_hist (GL_hist,(int16_t)( dx<<1) );
	ov = overlay_interval (center, nc, (int16_t) (opt>>2),(int16_t) ( opt%4==0), tab_angle);
	}
/*......................................................................*/
if( en )  /* if en==0 - not similar braces or vert line */
	{
	if( (en==1 || en==2) && nc<dy+5 ) /* !!!???!!! */
		{		/* almost one-line c_comp		*/
		if( en==1 )	/* similar '('				*/
			for (i=0; i<ny; i++)	/* can be convexity	*/
			    GL_hist[i] = dx-1-GL_right1[i];   /* axes symm */
		else		/* similar ')'				*/
			for (i=0; i<ny; i++)	/* can be concave	*/
			    GL_hist [i] = dx-1-GL_left1[i];   /* axes symm */
		/* checking center curve : convexity or concave		*/
		if( abris_convexity (GL_hist, ny, dx) )
				brace=en;	/* OK -->> save result	*/
		}
	}


					// it was below, inside;
					// it was "SHURIK" Error;
					// move it here 24.03.1993
	tmp.wide = (num_of_short_int(center,nc,1)*3>dy);


if ((brace==0 || wid*3<=wd) &&
     en!=3 &&				// NO VERT (Left or Right)
     ((optmax+dop_opt)*8<dy*5 || angles[0]->num!=0)) {	// 17.11.1993
		/* 1. non exist left(right) vert line or arc		*/
		/* 2. narrow width of c_comp or  wide width of hist	*/
		/*    AND hist hav'nt pik				*/
	tmp.max    = optmax ;
	tmp.imax   = imax;
	tmp.over   = ov;
	tmp.opt    = opt;        /* first angle = 0         */

	imax = find_opt_shift (angles, num_angles, dy, dx, center, nc, wid,
			       sig_T, sig_f, tab_angle, GL_hist, &tmp);
	ov     = tmp.over;
	opt    = tmp.opt;
	optmax = tmp.max;        /* optimal angle           */
	}

i = bad_overlay (ov, wid, dy, tmp.wide, sig_f);

			// here was DEBUG_GRAPH

if ( brace )
	return(brace );  /* no stick : recognize ')' or '(' */
			 /*            ret cod =  2  or  1  */
if ( i )
	return (comp_wide ? 4 : 3);
	/* no stick : overlay base incline and c_comp is small */
	/*            ret_code = 4 (for wide c_comp) or  3     */


make_result (dy, opt, tab_angle);	/* step tab_angle = 4 */

compress_centers (center, nc, tab_angle, dy, GL_cent, hooks );

/**************************************	BEFORE 22.11.1993 (see below);
if (imax!=-1)		// correct base incline if exist inc
	correct_result (GL_cent, tab_angle, dy);
	****************************************/

			// here was DEBUG_GRAPH

*wide = wid;
if( dy>20 && dx>12 && wid<dx/2)
	{
    filtr_short (left, ny, 1);
	//filtr_short (right,ny, 1);
	}
/*****************************************************	BEFORE 16.11.1993
if (imax<0)
	imax = 0;
*inc_v = angles[ imax ]->tg-angles[0]->tg;
*******************************************************/

			// 16.11.1993 Begin use information about inc
			// without information about nIncline (f_a):
if (imax<0)					// 17.11.1993 RESTORE
	imax = 0;
if (en==3)
	inc_num_EEM = *inc_v = 0;		// 17.11.1993
else	{
	inc_num_EEM = angles[imax]->num;	// 17.11.1993
	*inc_v = angles[imax]->tg;		// 16.11.1993
	if (inc_char_EEM==' ')	{	// m.b. L ( _ ) R;
	    if (inc_num_EEM>0)  inc_char_EEM = '/';	// FORW INC;
	    if (inc_num_EEM<0)  inc_char_EEM = '\\';	// BACK INC;
	    }
	}

if( enable_correct )
  {
//////if (inc_num_EEM)	// 22.11.1993 move it here with new condition;
  if (inc_num_EEM>0)        // 10.12.1993: FORW/BACK INC:
    correct_result (GL_cent, tab_angle, dy);
  if (inc_num_EEM<0)
    correct_result_BACK (GL_cent, tab_angle, dy);
  }

return (0);	/* normal return */
}


/*----------------------------------------------------------------------*/
/* make histogramm of length.             */
/* in  : center[0:nc-1]-array of centers  */
/* out : hist[] - histogramm of length    */
/* return : size of histogram             */
int16_t centers_len_to_hist(center_interval center[],int16_t nc,int16_t dy,int16_t dx,
        uchar hist[])
{
int16_t i;
center_interval *p_center=&center[0],*p_end=&center[nc];	// OLEG OLD
//////center_interval *p_center = (!fax1x2)  ?  &center[0]  :  &center[1];
//////center_interval *p_end    = (!fax1x2)  ?  &center[nc] :  &center[nc-1];
		//////////////////////////// MK PROBA 14.01.1993 ///////////

memset(hist,0,2*dx);
for(;p_center!=p_end;p_center++)
	if( p_center->row<dy )
		hist[ p_center->len ] ++;
for(i=dx;i>=0 && !hist[i] ;i--);
return(++i);
}

static int16_t max_center_hist_new (uchar fun[], int16_t n,  // 15.10.1993
    center_interval center[], int16_t nc, int16_t tab_angle[],
    int16_t typ)  {
//////    int16_t wid)  {   // NEW PARAMETR 20.01.1993 (DELETED)
////int16_t i, im=-1, maxim=-1, ov=-1, over, io, ic;  // OLEG: maxim=-1;
int16_t i, im=-1, maxim=0,  ov=-1, over, io, ic;  // MK:   maxim=0;
uchar  ff;
/*......................................................................*/
if( typ )  {				// FOR WIDE LETTERS (UNUSED NOW ???)
for (i=0; i<n;)  {			// NB: this part - for CENTERs ONLY !!!
        io=i;
	ff=fun[i]; i++;

	while ( i<n && fun[i]==ff )  i++;  /* fun[i] = const */
	if( ff>maxim )  {
//////	    if (......)  {	// MK VARIANT:
		ic = i+io-1;
		over = 	overlay_interval(center,nc,(int16_t) (ic>>2),(int16_t) (ic%4==0),tab_angle);
		if( over>=ov )  {	/* num of over can be increase */
			maxim = ff;
			im=ic;
			ov=over;
			}
		}
//////	    }	// MK VARIANT (SECOND IF)
	}
}
/*......................................................................*/
else	{     /* typ==0 : overlay chars are non interessant */

//////uchar  *b_fun=&fun[0], *e_fun=&fun[n], *p_fun, *p_old;
uchar  fold, ftek, fnext;
uchar  porog_2 = (nc - 1) >> 1;  // for 18 porog_2=8;
//////uchar  porog_4 = (nc - 2) >> 2;  // for 18 porog_2=4;

//////for (p_fun=b_fun; p_fun<e_fun;)  {  // IT WAS POIInt16ER-CICLE...

	ftek = 0;	// for ftek => fold
	for (i=0; i<n; i++)  {
	    fold = ftek;
	    ftek = fun [i];
	    if ((fold + ftek) >= porog_2)  {	// m.b see 4 Cases: ODD/EVEN i/wid ???
		fnext = (i==n-1) ? 0 : fun[i+1];	// BBB
//////		if (fold>ftek)  return (i + i - 2);	// for fold
//////		if (fold<ftek)  return (i + i);		// for ftek
//////		return (i + i - 1);			// fold / ftek
		if (fold<ftek)  {
		    if (ftek<fnext)  return (i+i+2);	// 4,5,6: NEXT
			       else  return (i+i);	// 4,5,3: TEK
		    }
		else  {
		    if (fold>fnext)  return (i+i-2);	// 5,3,4: OLD
			       else  return (i+i);	// 5,3,6: TEK !!!
		    }
		}
	    if ( ftek > maxim )  { im = i + i;  maxim = ftek; }
	    }
}
/*......................................................................*/
return (im);
}

static int16_t calc_LENs_LIMITED (uchar fun[], int16_t n, int16_t sum)  {  // 15.10.1993

					// see max_center_hist_new (Part 2);

int16_t i, im=-1, maxim=0;  // MK:   maxim=0;
uchar  fold, ftek, fnext;
///uchar	porog_2 = (nc - 1) >> 1;	// for 18 porog_2=8;
uchar  porog_2 = (sum - 1) >> 1; // for 18 porog_2=8;
/*......................................................................*/
	ftek = 0;	// for ftek => fold
	for (i=0; i<=n; i++)  {
	    fold = ftek;
	    ftek = fun [i];
	    if ((fold + ftek) >= porog_2)  {	// m.b see 4 Cases: ODD/EVEN i/wid ???
		fnext = (i==n-1) ? 0 : fun[i+1];	// BBB
		if (fold<ftek)  {
		    if (ftek<fnext)  return (i+i+2);	// 4,5,6: NEXT
			       else  return (i+i);	// 4,5,3: TEK
		    }
		else  {
		    if (fold>fnext)  return (i+i-2);	// 5,3,4: OLD
			       else  return (i+i);	// 5,3,6: TEK !!!
		    }
		}
	    if ( ftek > maxim )  { im = i + i;  maxim = ftek; }
	    }
/*......................................................................*/
return (im);
}

static void make_tab_angles (const INC_BASE *angle, int16_t hei, int16_t ang[])  {
							// 09.12.1993
							// case BACK NAKLON;
			// here was DEBUG_GRAPH
const int16_t *p_angle=&(angle->inc[0]);    // KOGO;
int16_t *p_ang=&ang[hei],*p_beg=&ang[0];  // KUDA
int16_t disp_for_BACK_NAKLON=0;

	if ( angle->tg!=0 )  {
				// here was DEBUG_GRAPH
	    if (angle->tg<0)				// 09.12.1993
		disp_for_BACK_NAKLON = - angle->inc[hei-1];

	    /************************** BEFORE 09.12.1993 (CAN NOT BE H>64)
	    if ( hei>128 )  {
int16_t j;
		for (j=0; p_ang>p_beg; j++)  {
		    if ( j==128 )  j=0;
		    *(--p_ang) = *(p_angle+j);
		    }
		}
	    else
		for( ; p_ang>p_beg; )
			* --p_ang = * p_angle++ ;
	    *************************************/

	    for ( ; p_ang>p_beg; )			// 09.12.1993:
		* --p_ang = (* p_angle++) + disp_for_BACK_NAKLON;
	    }
	else
      memset (ang, 0, hei<<1);

//////return ( MAX (ang[0], ang[hei-1]) );  // 09.12.1993 DELETE IT
            // ("int16_t" => "void")
						// as UNUSED and UNKNOWN !!!
}

static void make_hist (center_interval center[],int16_t nc, uchar hist[], int16_t ang[],
          int16_t dx,int16_t len, int16_t typ)
{
int16_t k;
center_interval *p_center=&center[0], *p_end=&center[nc];	// OLEG
//////center_interval *p_center = (!fax1x2)  ?  &center[0]  :  &center[1];
//////center_interval *p_end    = (!fax1x2)  ?  &center[nc] :  &center[nc-1];
		//////////////////////////// MK PROBA 14.01.1993 ///////////

memset (hist, 0, 2*dx);

if( typ )
for (; p_center!=p_end; p_center++)  /* for typ=1 need intervals,near to len */
	{
  if ( abs(p_center->len - len) > 2 )  continue;  /* ??? */

	k = p_center->col - (ang [p_center->row] << 1);
	if (k>=0)
		hist[k]++;	/* may by : turn of center < 0 */
    else if( k==-1 )
        hist[0]++;
	}
else
for (; p_center!=p_end; p_center++)
	{
	k = p_center->col - (ang [p_center->row] << 1);
	if (k>=0)
		hist[k]++;	/* may by : turn of center < 0 */
    else if( k==-1 )
        hist[0]++;
	}

return ;
}

static void make_hist_centers_LIMITED (center_interval center[], int16_t nc,
    uchar hist[], int16_t ang[], int16_t dx, int16_t len_limit)  {
							// 15.10.1993
int16_t k;              // see make_hist ();
center_interval *p_center=&center[0], *p_end=&center[nc];	// OLEG

memset(hist,0,2*dx);

for (; p_center!=p_end; p_center++)  {
	if (p_center->len > len_limit)  continue; /* ??? */

	k = p_center->col - (ang [p_center->row] << 1);
	if (k>=0)
		hist[k]++;	/* may by : turn of center < 0 */
	}
return;
}

static int16_t width_of_hist(uchar hist[],int16_t len)
{
int16_t num;
uchar *p_hist=&hist[0],*p_end=&hist[len];

for(num=0;p_hist!=p_end;p_hist++)
	if( *p_hist )
		num++;

return((num+1)>>1);
}

/*static*/ int16_t overlay_interval(center_interval center[],int16_t nc, int16_t col,
          int16_t typ,  int16_t tab_angle[])
{
int16_t num,c;
center_interval *p_center=&center[0],*p_end=&center[nc];

if( typ )
	{      /*  half byte : one cell */
	for(num=0;p_center!=p_end;p_center++)
		{
		c = (col+tab_angle[ p_center->row ])<<1;
    if( abs(p_center->col - c)<= p_center->len )
			num+=2 ;     /* overlay       */
		}
	}
else
	{      /*  ceil byte : two cell  */
	for(num=0;p_center!=p_end;p_center++)
		{
		c = ((col+tab_angle[ p_center->row ])<<1)+1;
    if( abs(p_center->col - c) < p_center->len )
			num+=2 ;   /* center of column */
    else if( abs(p_center->col - c) == p_center->len )
			num+=1 ;   /* limit of column  */
		}
	}

return(num>>1);
}

void filtr_short(uchar fun[],int16_t n,int16_t lev)
{
uchar i;
lev++;
for(i=1;i<n-1;i++)
  if( fun[i-1]==fun[i+1] && abs(fun[i]-fun[i-1])<lev )
		fun[i] = fun[i+1];
return;
}

static int16_t enable_shift(uchar left[],uchar right[],int16_t h,int16_t w,
      int16_t inc[])
{
if( abris_convexity(left,h,w) )
	{ inc_char_EEM = '(';  return(1); }	/* left arc         */
if( abris_convexity(right,h,w) )
	{ inc_char_EEM = ')';  return(2); }	/* right arc        */

if( abris_inc_line(left,h,inc,0) )
	{ inc_char_EEM = 'L';  return(3); }	/* left vert line   */
if( abris_inc_line(right,h,inc,1) )
	{ inc_char_EEM = 'R';  return(3); }	/* right vert line  */

inc_char_EEM = ' ';
return ( 0 );		/* no lines, no arcs */
}

int16_t abris_convexity(uchar fun[],int16_t n,int16_t w)
{
uchar i,ff,fo,imin,num,minim,eq;

minim =(uchar) find_minimum( fun, n, &imin );

if( w>0 )
	{
	i=n>>2;
	if( (imin<i || imin>(n-i)) ) /* max of fun belong [n/4, 3n/4] */
		return(0) ;
	}
else if( imin==0 || imin==n-1 )
	return(0);

for(eq=num=0,fo=fun[0],i=1;i<imin;i++) /* fun must decrease */
	{
	ff=fun[i];
	if( ff>fo )
		num++;
        if( ff==fo )
		eq++;
	fo=ff;
	}


for(fo=fun[imin],i=imin+1;i<n;i++)      /* fun must increase */
	{
	ff=fun[i];
	if( ff<fo )
		num++;
	if( ff==fo )
		eq++;
	fo=ff;
	}
if( w>0 )
	{
	w>>=1;
	if( !w ) w=2;
	if( (fun[0]-minim + fun[n-1]-minim)<=18*w/10 )//2 )
		return(0);         /* flat curve */
	}

if( w>0 )
	return( num==0 && eq*4<n*3 );
else
	return (num<3) ;
}

static int16_t num_of_short_int(center_interval center[],int16_t nc,int16_t lim)
{
int16_t n;
center_interval *p_center=&center[0],*p_end=&center[nc];

for(n=0;p_center!=p_end;p_center++)
	if( p_center->len <= lim )
		n++;
return(n);
}

static int16_t find_opt_shift(INC_BASE *angles[],int16_t num_angles,
        int16_t dy, int16_t dx, center_interval center[], int16_t nc,
        int16_t wid,Bool16 sig_T,Bool16 sig_f,
        int16_t tab_angle[],
        uchar hist[], struct shift_inform *res)
{

int16_t i, maxim=res->max, imax, over, ovmax, op, optmax, curr_max, pr;
int16_t start_opt=res->opt, start_over=res->over, start_max=res->max;

for (pr=1,curr_max=start_max,ovmax=start_over,imax=-1,i=0; i<num_angles; i++)
	{
	make_tab_angles (angles[i+1], dy, tab_angle);
	make_hist (center, nc, hist, tab_angle, dx, wid,(int16_t) ( wid>4 && !sig_T ));
	op = max_center_hist (hist, (int16_t) (dx<<1), center, nc, tab_angle, 0);
	curr_max = hist[op>>1];
	if ( pr && curr_max!=start_max ) pr=0;
	over = overlay_interval (center, nc,(int16_t) ( op>>2),(int16_t) ( op%4==0), tab_angle);
	if( ( maxim<curr_max ||				  /* decrease max  */
	    (maxim==curr_max && imax==-1 && i>0 && pr) )
	    && !bad_overlay(over,wid,dy,res->wide,sig_f) )/* overlay is OK */
						   /* if max[0]=max[1]=... */
		{                                  /* get second maximum   */
		maxim = curr_max;
		imax=i;
		optmax=op;
		ovmax = over;
		}

			// here was DEBUG_GRAPH
//////#if defined(DEBUG_GRAPH)
//////#if  KADR==1
//////.......................
//////#endif
//////#endif
	}

if( imax!=-1 && start_over-ovmax<=((dy>8?dy>>3:1)+1) &&
    start_max<maxim )
	{
	/*  set optimal inc if last hist no equ opt hist */
	/*      and number of overlay-points is increase */
	if( i!=imax )
		{
        int         loc_inc = (dx*2048)/dy, i;
        // oleg : 10-02-1998 : try original incline near artifical res
        if( wid<3 && angles[ imax+1 ]->tg>850 &&
            abs(loc_inc-angles[ imax+1 ]->tg)>64 )
            {
            INC_BASE    loc_ang={0};
            for(i=0;i<128;i++)
                loc_ang.inc[i]=(i*dx)/dy;
            loc_ang.tg = loc_inc;
            make_tab_angles(&loc_ang, dy,tab_angle);
            }
        else
		    make_tab_angles(angles[imax+1], dy,tab_angle);
		make_hist(center,nc, hist, tab_angle,dx,wid,(int16_t) (wid>4&&!sig_T));
		}
	}
else
	{    /* restore 0 inc */
	make_tab_angles(angles[0], dy,tab_angle);
	make_hist(center,nc, hist, tab_angle,dx,wid,(int16_t) (wid>4&&!sig_T));
	maxim  = start_max;
	ovmax  = start_over;
	optmax = start_opt;
	imax   = -1;
	}

res->max  = maxim;
res->imax = (imax==-1) ? imax : (imax + 1);
res->over = ovmax;
res->opt  = optmax;	/* save result */

return ( res->imax );
}

/*static*/ Bool16 bad_overlay(int16_t over,int16_t width,int16_t dy,int16_t wide,Bool16 c_f)
{
Bool16 ret;
if( width<3 )
	ret = ( over<(wide ? dy>>2 : (dy<30?6*(dy>>3):5*(dy>>3)/*dy/3*/) ) );
else
	ret = ( over<( !c_f ? dy - (dy>>2) : (dy<30?dy*2/3:dy*5/8)  ) );
return(ret);
}

static int16_t make_result (int16_t n, int16_t opt, int16_t res[])
{
int16_t *p_res=&res[0],*e_res=&res[n];

for(; p_res<e_res; p_res++)
	*p_res = ((*p_res)<<2) + opt;

return ( opt%4==0 );	/* return 1 if cut between two cells, else return 0 */
}

static void compress_centers(center_interval center[],int16_t nc,int16_t ang[],int16_t n,
          center_interval cent[],int16_t hooks[])
{
int16_t r,D,L,en_cp;
center_interval *p_center=&center[0], *p_end=&center[nc];

memset(cent,0,n*sizeof(center_interval));
memset(hooks,0,4*sizeof(hooks[0]));
for(;p_center!=p_end;p_center++)
	{
	en_cp=0;
	r = p_center->row ;
  D = abs(p_center->col-(ang[ r ]>>1)) ;
  L = MAX(p_center->len,2);

	if( D<=L )
		en_cp=1;
	else
		{
		if ( D<L+2 && (cent[r].len==0 || cent[r].len<p_center->len) )
			en_cp=1; /* cent[r] is free or more length */
		else
		    {   /* hook-intervals */
        int16_t ind;
		    ind = (p_center->col>ang[r]>>1)?2:0; /* left or right */
		    if( r>(n>>1) )
			ind++;  /* down */
		    hooks[ind]++;
		    }
		}
	if( en_cp )
		{ /* save interval */
		center_interval *buf=&cent[r];
		buf -> row = p_center -> row;
		buf -> col = p_center -> col;
		buf -> len = p_center -> len;
		}
	}

return;
}

static int16_t correct_result (center_interval cent[], int16_t inc[], int16_t dy)  {
							// 15.01.1994
						// NOTA BENE: see also
						// corr_result_BACK for INC<0;
/* step of incline = 4 */
int16_t i, j, n1=dy>>2, n2=dy-(dy>>2), d, num_periods, pp;
int16_t len_tek, len_old;

static  int16_t OTL_key_VARIANT=2;  // 0 RET, 1 OLD, or 2 NEW MK;

						// 15.01.1994 NEW EDITION:
	if (OTL_key_VARIANT==0)  return (0);	////// OTLADKA # 0 //////
	if (OTL_key_VARIANT==2)  {		////// OTLADKA # 2//////
	    correct_result_MK (cent, inc, dy);
	    return (1);
	    }
/*......................................................................*/
for (i=n1; i<n2; i++)
	if ((cent[i].col<<1)!=inc[i])
		break;	/* find point, different from incline */

if (i==n2)  return(0);	/* center and optimal incline coincide */

num_periods = calc_inc_periods (inc, dy, inc_periods);

for (j=0,i=dy-1; cent[i].len==0 &&
		 j<inc_periods[num_periods-1]; j++,i--);	// CONTINUE !
/* skip free centers in last period */
/*......................................................................*/
for (pp=num_periods-1, i=dy-1, d=j;			// DOWNER PARTS
     i>=0;
     i-=inc_periods[pp], d=0, pp--)  {
	for (j=d;
	     j<inc_periods[pp] &&
//////	     intermediate_center_d(&cent[i-j],inc[i]);	// OLD EDITION
/////////////return( c->len!=0 && inc - (c->col<<1) >= 4  );	// BODY !!!
	     cent[i-j].len!=0 &&			// MK EDITION
	     (inc[i] - (cent[i-j].col<<1)) >= 4;
	     j++);	// CONTINUE

m1:	if (j==0)  goto m1_ex;			// 27.10.1993 PROBA (BIG SKOK):
	len_tek = cent[i-j].len;
//////	len_old = cent[i-j-1].len;		// MAC.ERROR 14.01.1994
	len_old = cent[i-j+1].len;		// 14.01.1994 stda10/18 'l'
  if (abs(len_tek - len_old)>1)  {  // There is BIG SKOK !!!
		j--;
		goto  m1;
		}

m1_ex:	for (j--; j>=0; j--)  {		/* modify   */
		inc [i-j] -= 4;		/* step = 4 */
		}
	}
/*......................................................................*/
for (pp=num_periods-1, i=dy-1-inc_periods[num_periods-1]+1;	// UPPER PARTS
     i>=0;
     i-=inc_periods[--pp])  {
					// HERE WAS MAC.ERR. 10.05.1993:
//////	if( inc[i]<inc[i-1] )             /* next group is'n changed */
	/**************************************	BEFORE 26.10.1993
	if ( (i==0) || (inc[i]<inc[i-1]) )	// next group is'n changed
	    for (j=0; j<inc_periods[pp] &&
		 intermediate_center_u(&cent[i+j],inc[i]); j++);    // CONTINUE
		 ********************************************************/
					// 26.10.1993 NEW EDITION,
					// because old used undefined j (???):
	if ((i!=0) && (inc[i]>=inc[i-1]))  continue;  // next group was changed

	for (j=0;
	     j<inc_periods[pp] &&
//////	     intermediate_center_u(&cent[i+j],inc[i]);	// OLD EDITION;
/////////////return( c->len!=0 && (c->col<<1) - inc >= 4  );    // BODY !!!
	     cent[i+j].len!=0 &&			// MK EDITION
	     ((cent[i+j].col<<1) - inc[i]) >= 4;
//////	     ((cent[i+j].col<<1) - inc[i]) == 4;	// PROBA 26.10.1993
	     j++);    // CONTINUE

m2:	if (j==0 || i+j==dy)  goto m2_ex;	// 27.10.1993 PROBA (BIG SKOK):
	len_tek = cent[i+j].len;
	len_old = cent[i+j-1].len;
  if (abs(len_tek - len_old)>1)  {  // There is BIG SKOK !!!
		j--;
		goto  m2;
		}

m2_ex:	for (j--; j>=0; j--)  {
		inc [i+j] += 4;		/* step = 4 */
		}
	}
/*......................................................................*/
if ( cent[dy-1].len==0 && inc[dy-1]!=inc[dy-2] )
	inc[dy-1] = inc[dy-2];

if ( cent[0].len==0 && inc[0]!=inc[1] )
	inc[0] = inc[1];

	/* interval in fisrt and last lines and incline no */
	/* overlay AND shift last group of incline points  */

return(1);
}

int16_t find_minimum(uchar fun[],int16_t n,uchar *_imin)
{
uchar i, imin, minim, io, ff;

for(imin=0,minim=fun[0],i=1;i<n;)
	{
	io=i;
	ff=fun[i++];

	while( i<n && fun[i]==ff )i++;  /* fun[i] = const */

	if( minim>ff )
		{
		minim = ff;       /* imin - center of interval [io,i] : */
		imin=(i+io)>>1;   /*        fun[k]=const for io<=k<=i   */
		}

	}

*_imin = imin  ;

return(minim);
}

static int16_t calc_inc_periods (int16_t inc[], int16_t dy, int16_t inc_periods[])
{
int16_t *p_inc=&inc[0], *p_end=&inc[dy], *p_inc_per= &inc_periods[0],
    iold=inc[0], i, k, k_old;

for (k=k_old=0; p_inc!=p_end; k++,p_inc++)
	if( (i=*p_inc)!=iold )
		{       /* new period */
		*p_inc_per++ = k-k_old;
		iold = i;
		k_old = k;
		}

if( k!=k_old )
*p_inc_per++ = k-k_old; /* last period */
*p_inc_per = 0;
return ( p_inc_per - inc_periods );
}

int16_t max_center_hist (uchar fun[], int16_t n,
    center_interval center[], int16_t nc, int16_t tab_angle[], int16_t typ)
{
int16_t i,im=-1,maxim=-1,ov=-1,over,io,ic;       uchar ff;
if( typ )
{
for (i=0; i<n;)
	{
        io=i;
	ff=fun[i]; i++;

	while ( i<n && fun[i]==ff ) i++;	/* fun[i] = const */
	if( ff>maxim )
		{
		ic = i+io-1;
		over = overlay_interval (center,nc,(int16_t) (ic>>2),(int16_t) (ic%4==0),tab_angle);
		if( over>=ov )      /* num of over can be increase */
			{
			maxim = ff;
			im=ic;
			ov=over;
			}
		}
	}
}
else
{	/* typ==0 : overlay chars are non interessant */

uchar *b_fun=&fun[0],*e_fun=&fun[n],*p_fun,*p_old;

for (p_fun=b_fun; p_fun<e_fun;)
	{
	p_old=p_fun;
	ff=*p_fun++;

	while ( p_fun<e_fun && *p_fun==ff ) p_fun++;	/* fun[i] = const */
	if( ff>maxim )
		{
		im = p_fun-b_fun+p_old-b_fun-1;
		maxim = ff;
		}
	}
}
return(im);
}

static int16_t correct_result_BACK (center_interval cent[], int16_t inc[], int16_t dy)
{							// 14.01.1994
					// This is MODIFY of correct_result
					// for BACK NAKLON (INC<0);
					// Begin it 10.12.1993, very formalno;
					// See comments "FORW INC", "BACK INC";
/* step of incline = 4 */
int16_t i, j, n1=dy>>2, n2=dy-(dy>>2), d, num_periods, pp;
int16_t len_tek, len_old;
static  int16_t OTL_key_RET=0;

	if (OTL_key_RET)  return (0);	////// OTLADKA //////	stic_mak@530

for (i=n1; i<n2; i++)
	if ((cent[i].col<<1)!=inc[i])
		break;	/* find point, different from incline */

if (i==n2)  return(0);	/* center and optimal incline coincide */

num_periods = calc_inc_periods (inc, dy, inc_periods);

for (j=0,i=dy-1; cent[i].len==0 &&
		 j<inc_periods[num_periods-1]; j++,i--);	// CONTINUE !
/* skip free centers in last period */
/*......................................................................*/
for (pp=num_periods-1, i=dy-1, d=j;			// DOWNER PARTS
     i>=0;
     i-=inc_periods[pp], d=0, pp--)  {
	for (j=d;
	     j<inc_periods[pp] &&
//////	     intermediate_center_d(&cent[i-j],inc[i]);	// OLD EDITION
/////////////return( c->len!=0 && inc - (c->col<<1) >= 4  );	// BODY !!!
	     cent[i-j].len!=0 &&			// MK EDITION
//////	     (inc[i] - (cent[i-j].col<<1)) >= 4;	// FORW INC
	     (inc[i] - (cent[i-j].col<<1)) <= -4;	// BACK INC
	     j++);	// CONTINUE

m1:	if (j==0)  goto m1_ex;			// 27.10.1993 PROBA (BIG SKOK):
	len_tek = cent[i-j].len;
//////	len_old = cent[i-j-1].len;		// MAC.ERROR 14.01.1994
	len_old = cent[i-j+1].len;		// 14.01.1994
	if (abs(len_tek - len_old)>1)  {	// There is BIG SKOK !!!
		j--;
		goto  m1;
		}

m1_ex:	for (j--; j>=0; j--)  {		/* modify   */
//////		inc [i-j] -= 4;		/* step = 4 */	// FORW INC
		inc [i-j] += 4;		/* step = 4 */	// BACK INC
		}
	}
/*......................................................................*/
for (pp=num_periods-1, i=dy-1-inc_periods[num_periods-1]+1;	// UPPER PARTS
     i>=0;
     i-=inc_periods[--pp])  {
					// HERE WAS MAC.ERR. 10.05.1993:
//////	if( inc[i]<inc[i-1] )             /* next group is'n changed */
	/**************************************	BEFORE 26.10.1993
	if ( (i==0) || (inc[i]<inc[i-1]) )	// next group is'n changed
	    for (j=0; j<inc_periods[pp] &&
		 intermediate_center_u(&cent[i+j],inc[i]); j++);    // CONTINUE
		 ********************************************************/
					// 26.10.1993 NEW EDITION,
					// because old used undefined j (???):
//////	if ((i!=0) && (inc[i]>=inc[i-1]))  continue;  // next group was changed
	if ((i!=0) && (inc[i]!=inc[i-1]))  continue;  // next group was changed
			// NB: such edition - for both FORW INC and BACK INC !

	for (j=0;
	     j<inc_periods[pp] &&
//////	     intermediate_center_u(&cent[i+j],inc[i]);	// OLD EDITION;
/////////////return( c->len!=0 && (c->col<<1) - inc >= 4  );    // BODY !!!
	     cent[i+j].len!=0 &&			// MK EDITION
//////	     ((cent[i+j].col<<1) - inc[i]) >= 4;	// FORW INC
	     ((cent[i+j].col<<1) - inc[i]) <= -4;	// BACK INC
	     j++);    // CONTINUE

m2:	if (j==0 || i+j==dy)  goto m2_ex;	// 27.10.1993 PROBA (BIG SKOK):
	len_tek = cent[i+j].len;
	len_old = cent[i+j-1].len;
	if (abs(len_tek - len_old)>1)  {	// There is BIG SKOK !!!
		j--;
		goto  m2;
		}

m2_ex:	for (j--; j>=0; j--)  {
//////		inc [i+j] += 4;		/* step = 4 */	// FORW INC
		inc [i+j] -= 4;		/* step = 4 */	// BACK INC
		}
	}
/*......................................................................*/
if ( cent[dy-1].len==0 && inc[dy-1]!=inc[dy-2] )
	inc[dy-1] = inc[dy-2];

if ( cent[0].len==0 && inc[0]!=inc[1] )
	inc[0] = inc[1];

	/* interval in fisrt and last lines and incline no */
	/* overlay AND shift last group of incline points  */

return(1);
}

static int16_t abris_inc_line (uchar fun[], int16_t n, int16_t inc[], Bool16 sig_left)  {
							// 27.10.1993
					// MK EDITION:
					// LEFT - Exactly 0;
					// RIGHT - ANY VALUE;
					// must be more 1/4 UP and RIGHT;
					// NB: It was error about sig_left
					//     (It was NEVER USED ???)
int16_t i;    // OLD: hist=0;
int16_t n2=(n+1)/2; // UPPER PART >= DOWN.PART (?)
int16_t k, kU=0, kD=0;
int16_t kmax, v, vmax;
#define	mk10	10
int16_t hist_U[mk10], hist_D[mk10];

	k = inc [0];	// fow NO WARNING, NOT USED NOW (All ZEROES);

	/******************************	BEFORE 27.10.1993, without RETURN;
if (sig_left)  {	// left abris		###### IT WAS ERROR ??? ######
	for( i=0; i<n; i++)
		if( (fun[i]-inc[i])>1 )
			hist++;
		***************************/

if (sig_left==0)  {	// left abris	// 27.10.1993 (LEFT or RIGHT - ???)
	for (i=0; i<n2; i++)  		// 27.10.1993
		if (fun[i]==0)  kU++;
	for (i=n2; i<n; i++)
		if (fun[i]==0)  kD++;
	if (kU*2>=n2 && kD*2>=n2)
		return (1);	// There is LEFT LINE;
	return (0);		// NOT LEFT LINE;
	}
/*......................................................................*/
/******************************************************	BEFORE 27.10.1993
else	{	// right abris : sub wide of stick for compare with inc line
        for( i=0; i<n; i++)
		if( (fun[n-1-i]-inc[i])>1 )
			hist++;
	}
return( hist<= (n>16?n>>3:2) ? 1 : 0 );
	***********************************/

  memset (hist_U, 0, mk10*sizeof(int16_t)); // 27.10.1993
  memset (hist_D, 0, mk10*sizeof(int16_t));
	for (i=0; i<n2; i++)  if (fun[i]<mk10)  hist_U[fun[i]]++;
	for (i=n2; i<n; i++)  if (fun[i]<mk10)  hist_D[fun[i]]++;
	for (k=kmax=vmax=0; k<mk10; k++)
	    if ((v=hist_U[k]+hist_D[k])>vmax)  { kmax = k;  vmax = v; }
	kU = hist_U [kmax];
	kD = hist_D [kmax];

	if (kU*2>=n2 && kD*2>=n2)	// The same, how for LEFT CASE:
		return (1);	// There is RIGHT LINE;
	return (0);		// NOT RIGHT LINE;
}

static int16_t correct_result_MK (center_interval cent[], int16_t inc[], int16_t dy)  {

int16_t i, n1=dy>>2, n2=dy-(dy>>2), num_periods;
int16_t nper, per_inc, i_up, i_down;  // UP, DOWN for PERIOD, INCLUDE;

for (i=n1; i<n2; i++)
	if ((cent[i].col<<1)!=inc[i])
		break;	/* find point, different from incline */

if (i==n2)  return(0);	/* center and optimal incline coincide */

num_periods = calc_inc_periods (inc, dy, inc_periods);

/*......................................................................*/
	i_up = dy - inc_periods [num_periods - 1];	// 15.01.1994

	for (i=dy-1; i>=i_up; i--)  if (cent[i].len)  break;
				/* skip free centers in last period */
/*......................................................................*/
						// DOWNER PARTS to RIGHT:
	i_up = dy;					// 17.01.1994
	for (nper=num_periods-1; nper>=0; nper--)  {	// PERIODS from DOWN:
	    i_down = i_up - 1;
	    if (i_up!=dy)  i = i_down;		// from DOWN PART of PERIOD;
	    i_up -= inc_periods [nper];
	    per_inc = inc [i];
	    while (i>=i_up  &&  cent [i].len!=0  &&
		   per_inc - (cent[i].col<<1) >= 4  &&
				// NEW MK CONDITION: NO BIG SKOK to DOWN:
//////		   (i>=dy-1 || cent[i].len-cent[i+1].len < 4))   // 17.01.1994
		   (i>=dy-2 || cent[i].len-cent[i+1].len < 4)) { // 17.01.1994
			i--;
			}

			// NOW  i = i.first - 1;  NB: MAY BE i=-1; !!!

			// 17.01.1994 EDITION:
			// SKIP ONE LINE and then may be SOME LINES more:
	    while (++i<=i_down)	{
		if (i>1  &&		// no 0 (sic!) or 1 (not use 1/0);
        abs(cent[i].len-cent[i-1].len)>1) // BIG SKOK to UP
			continue;		// SKIP THIS LINE;
		break;		// ELSE  begin correct from this line;
		}

			// NOW  i = i.first (corrected by MK);

	    while (i<=i_down)  {
		inc [i++] -= 4;		// modify TO RIGHT (M=4);
		}
	    }
/*......................................................................*/
						// UPPER PARTS to LEFT:
	i_up = dy;					// 17.01.1994
	for (nper=num_periods-1; nper>=0; nper--)  {	// PERIODS from DOWN:
	    i_down = i_up - 1;
	    i_up -= inc_periods [nper];
	    i = i_up;				// from UPPER PART of PERIOD;
	    if ((i!=0) && (inc[i]>=inc[i-1]))
		continue;	// next group was changed
	    per_inc = inc [i];
	    while (i<=i_down  &&  cent [i].len!=0  &&
		   (cent[i].col<<1) - per_inc >= 4)  {
			i++;
			}

			// NOW  i = i.last + 1;  NB: MAY BE i=dy; !!!

			// 17.01.1994 EDITION:
			// SKIP ONE LINE and then may be SOME LINES more:
	    while (--i>=i_up)  {
//////		if (i<dy-1  &&			// NOT LAST LINE and
		if (i<dy-2  &&			// NOT LAST LINE and
        abs(cent[i].len-cent[i+1].len)>1) // BIG SKOK to DOWN;
			continue;		// SKIP THIS LINE;
					// 16/17.01.1994 MK ADD:
		if (i>0  &&		// NOT FIRST LINE
//////		    i<3  &&		// 17.01.1994  17:04  SOPLI I WOPLI;
				// DELETE 17.01.1994, RESTORE 18.01.1994:
		    i<3  &&		// 17.01.1994  17:04  SOPLI I WOPLI;
		    (cent[i].col  - (cent[i].len<<1) !=	    // 2*LEFT(i) !=
		     cent[i-1].col- (cent[i-1].len<<1)))    // 2*LEFT(i-1);
			continue;		// SKIP THIS LINE;
		break;		// ELSE  begin correct from this line;
		}

			// NOW  i = i.last (corrected by MK);

	    while (i>=i_up)  {
		inc [i--] += 4;		// modify TO LEFT (M=4);
		}
	    }
/*......................................................................*/
if ( cent[dy-1].len==0 && inc[dy-1]!=inc[dy-2] )
	inc[dy-1] = inc[dy-2];

if ( cent[0].len==0 && inc[0]!=inc[1] )
	inc[0] = inc[1];

	/* interval in fisrt and last lines and incline no */
	/* overlay AND shift last group of incline points  */

return(1);
}
