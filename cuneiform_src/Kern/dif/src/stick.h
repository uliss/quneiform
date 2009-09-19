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

/*----------------------------------------------------------------------*/
#include "difdefs.h"
#ifndef D_STICK_TYPES
#define D_STICK_TYPES

#define LIMIT_HEIGHT     256
#define LIMIT_WIDTH      64                /* N.B. LIMIT_WIDTH*4 < 256 !!! */
#define LIMIT_CENTER     LIMIT_HEIGHT*3
#define LIMIT_OF_ANGLES  8
/*----------------------------------------------------------------------*/
typedef struct { int16_t row,  /* row in c_comp */
		     col,  /* column * 2  */
		     len;  /* length      */
		} center_interval;
/*----------------------------------------------------------------------*/
typedef struct						// l->xxx, r->xxx;
	{
  uchar
		mount[5],   /* max flag in any zone. 0 - no flag	*/
		conc[5],    /* max concave in any zone. 0 - no concave	*/
		m_pos[5],   /* position of max flag in zone. 0-if no flag */
		c_pos[5],   /* --//-- for concaves			*/
		mb_pos[5],  /* first position flag.   0 if no flag	*/
		me_pos[5],  /* last position  flag. . 0 if no flag	*/
		cb_pos[5],  /* first position conc.   0 if no conc	*/
		ce_pos[5],  /* last position  conc.   0 if no conc	*/
		m_meandr,   /* sum of near pimples (long=1)		*/
		c_meandr,   /* sum of near concaves (long=1)		*/
		up_serif,   /* sign of serif :				*/
		down_serif, /* 0 - not exist, 1 - small, 2 - long	*/
		up_hook,    /* sign of hook  :				*/
		down_hook,  /* 0 - no, 1 - yes				*/
		num_flags,  /* num of flags				*/
		num_long_flags,  /* num of long flags			*/
		num_concs,  /* num of concaves				*/
		max_flag,   /* 06.01.1994				*/
		main_ear;   /* NOT USED in this version			*/
	}  STICK_CHARS;     /* left or right characteristics of stick	*/
/*----------------------------------------------------------------------*/
typedef struct							// s->xxx;
	{
  int16_t     height,   // height of c_comp
		width ,		// width  of c_comp
		base_2,		// second base line
		base_3,		// third  base line
		base_2mk,	// MK EDITION of SECOND,
		base_3mk,	// THIRD,
		base_4mk;	// and FOURTH base lines	03.01.1994
  uchar
		l_f_symptom,	// special signums for letter 'f'
		r_f_symptom,	// l:0,1,2,3 ; r : 0,1,2
		t_TOP,		// t^ or t~;
		dis_nose_1,	// diskrim 1 for his flat nose
		typ_nose_1,	// type 1-nose : 0(flat),1(long)
		lll_nose_1,	// MK: FLAT VERT part of NOSE >= 3
		dis_slash,	// diskrim letter '/'
		dis_left_brace, dis_right_brace,	// dis '(',')'
//////		dis_OxBA,	// diskrim є for his head
		dis_0xBA,	// diskrim є for his head
		neck,		// sign neck overlay base line 2
		T_config,	// similar 'T' (for Y-diskrim)
//////		Y_config,	// similar 'Y' (for T-diskrim)
////////////		T_hor,       // MK: about hor. difference
		T_2,		// MK: num of 2 intervals
		T_3,		// MK: num of 3 intervals
//////		T_skok,		// MK: about max skok of abris
		T_skok_L,	// MK: about max skok of abris
		T_skok_R,	// MK: about max skok of abris
		stick_width,	// optimal wide of stick
		left_mode,
		right_mode,
		full_width,	// l->max_flag + s->stick_width + r->max_flag;
		inc,		// 1-inc,0-no inc
//////		inc_v16,	// inc_v/16 03.06.1993 (see inc)  => inc_num;
		cut_l,cut_r,	// cutting signums
		dis_up_dot,	// diskrim for ij-dot
//////		up_dot_NAVIS,	// Priznak NAVISANIE (NO DOT)
		up_dot_H,	// Hight of DOT	(0 - NO DOT !)
    down_nose_1, // special for 1 : nose belong central zone
    up_dot_W; // Width of DOT
  char
		inc_num,	// 10.12.1993 (from inc_num_EEM)
		up_dot_dCOL;	// Displacement of DOT (use col)
  uchar    num_lines;      /* Number of with none single interval  */
  int16_t incline;  /* Normal tg(stick)*2048 + line_incline */
	} STICK_SIGNUMS;	// auxiliary information for
				// stick diskrimination
/*----------------------------------------------------------------------*/
typedef struct
        {
  int16_t tg;             /* tg = tangens*2048  */
  int16_t num;      /* 15.11.1993 MK NEW  */
  int16_t inc[256];       /* table of inc       */
	} INC_BASE;         /* base tables of inc */
/*----------------------------------------------------------------------*/
#define ADD_PROB(c) (c->nvers ? (c->vers[0].prob - 4) : 254 )

struct ln_head
 {
 int16_t  lth; // length of one line representation
 int16_t  h;   // height of line
 int16_t  row; // relative row of line start
 uint16_t flg;  // flags of free beg and free end
#define l_fbeg		0x20
#define l_fend		0x80
#define l_cbeg          0x02
#define l_cend          0x08
 };
typedef struct ln_head lnhead;

struct int_s
 {
 uchar l;  // length of interval
 uchar e;  // end of interval coordinates
 };
typedef struct int_s interval;

#define DIS_CENTER_FLAG(U,i,width,inc,t1,t2)             \
  { int16_t t,dist=1;                                  \
	t = U->mount[i];                                 \
	if( U->mount[0]>width || U->mount[4]>width )     \
		dist++;                                  \
	if( t>dist )                                     \
		dis += (t1)*(t-1);                       \
	else if( t>((dist)-1) && !inc && U->c_meandr>0 ) \
		dis += (t2);}

#define DIS_CENTER_FLAGS(L,R,i,width,inc,t1,t2)		\
		{					\
		DIS_CENTER_FLAG(L,i,width,inc,t1,t2)	\
		DIS_CENTER_FLAG(R,i,width,inc,t1,t2)	\
		}

#define DIS_HALF_SERIF(L,R,i,d,tab)                            	     \
	{if( L->mount[i]==0 && R->mount[i]>d &&                      \
	    (L->conc[i]>0||R->mount[i]>d+1) )                        \
		dis += tab*(R->mount[i]+L->conc[i]);                 \
	else if( L->mount[i]>d && R->mount[i]==0  &&                 \
		 (R->conc[i]>0||L->mount[i]>d+1) )                   \
		dis += tab*(L->mount[i]+R->conc[i]);}

#define DIS_DIFFERENT_SERIFS(L,R,i,dist,wid,tab)      \
	{                                             \
	int16_t t,m;                                      \
	t =  abs(L->mount[i]-R->mount[i]) ;           \
	m =  MIN(L->mount[i],R->mount[i]) ;           \
	if( t>MAX(((wid)>>1),dist) || m==0 )          \
		dis += (tab)*t;                       \
	}

#define DIS_DIFFERENT_SERIFS(L,R,i,dist,wid,tab)      \
	{                                             \
	int16_t t,m;                                      \
	t =  abs(L->mount[i]-R->mount[i]) ;           \
	m =  MIN(L->mount[i],R->mount[i]) ;           \
	if( t>MAX(((wid)>>1),dist) || m==0 )          \
		dis += (tab)*t;                       \
	}

#define DIS_BRACES_CIRCLE(L,R,wid,tab)  \
	{                               \
	DIS_BRACE_CIRCLE(L,R,wid,tab)   \
	DIS_BRACE_CIRCLE(R,L,wid,tab)   \
	}

			// 10.06.1993: CENSURE MAX 4*tab:
#define DIS_BRACE_CIRCLE(L,R,wid,tab)					\
	{								\
	if( L->mount[0]==0 && L->mount[4]==0 &&				\
	    R->mount[0]*3>=wid && R->mount[4]*3>=wid    &&		\
	    L->num_concs<3                              &&		\
	    (L->mount[2]>0||L->conc[0]>0&&L->conc[4]>0) &&		\
	    R->mount[0]>0  && R->mount[4]>0  && R->m_meandr<4 )		\
		dis += (tab) * min ((R->mount[0] + R->mount[4]), 4);	\
	}

#define FOUR_CONC(l,r,tab)                                      \
	{                                                       \
	if( FOUR_CONC_COND(l,r) || FOUR_CONC_COND(r,l) )        \
		dis += (tab);                                   \
	}

#define FOUR_CONC_COND(l,r)                             \
	(l->conc[0] && l->conc[4] && l->c_meandr<3  &&  \
	 l->ce_pos[0]>0 &&  r->m_meandr)                \

#define DIS_CENTER_CONCS(L,R,i,inc,t1,t2)      \
		{                              \
		DIS_CENTER_CONC(L,i,inc,t1,t2) \
		DIS_CENTER_CONC(R,i,inc,t1,t2) \
		}

#define DIS_CENTER_CONC(U,i,inc,t1,t2)              \
	{ int16_t t,m;                                  \
	m = ((U->up_serif==0)&&(U->down_serif==0)); \
	t = U->conc[i];                             \
	if( t>1 )                                   \
		dis += (t1)*(t-1);                  \
	else if( t>0 && m &&!inc && U->m_meandr>0 ) \
		dis += (t2);}

#define CALC_LENGTH_BEAM_FLAGS(l,r,width,signums,t)     \
	{                                               \
	if ( l->c_meandr && r->c_meandr )	t = 2;	\
	else	{					\
		if ( width>3 )		t=3;		\
		else if ( width==1 )	t=1;		\
		else			t=2;		\
		}                                       \
	if ( t==2 && signums->height<16 )	t=1;	\
	}

#define DIS_BEAM(L,R,i,dist,tab)                \
	{                                       \
	if( L->mount[i]+R->mount[i] > (dist) )  \
		dis += tab;                     \
	}

#define DIS_BEAM_MK(L,R,i,dist,tab)			\
	{						\
	if ( L->mount[i] && R->mount[i] && !inc &&	\
	     ((L->mount[i] + R->mount[i])==(dist)) )	\
		dis += tab;				\
	}

#define DIS_BRACE(L,R,tab)					\
	{							\
	if( L->mount[0]==0 && L->mount[4]==0 &&			\
	    L->num_long_flags==0 &&				\
	    R->mount[0]>1  && R->mount[4]>1  &&			\
	    R->m_meandr==0  &&					\
	    (L->conc[0]<3 || L->ce_pos[0]<4) )			\
		dis += (tab)*(R->mount[0] + R->mount[4] - 2);	\
	}

#define DIS_BRACES(L,R,tab)         \
	{                           \
	DIS_BRACE(L,R,tab)          \
	DIS_BRACE(R,L,tab)          \
	}

#define DIS_CURVE(L,R,i,lev,tab)         	   \
	{                                          \
	int16_t ll,rr;                                 \
	ll=L->mount[i]; rr=R->conc[i];             \
	if( ll>lev && rr>lev && (ll>0||rr>0) )     \
		dis += (ll+rr)*(tab);              \
	ll=L->conc[i]; rr=R->mount[i];             \
	if( ll>lev && rr>lev && (ll>0||rr>0))      \
		dis += (ll+rr)*(tab);              \
	}
#define DIS_FLAGS_L_R(i,t1)				\
		{					\
		DIS_CENTER_FLAG(l,i,wid,inc,t1,d_L)	\
		DIS_CENTER_FLAG(r,i,wid,inc,t1,d_R)	\
		}



#define LF 80	/* long central flag	*/
#define NF 12	/* small central flag	*/
#define MD  4	/* min dis value	*/
#define SI 20	/* similar  є		*/
#define BP 80	/* bad proportions	*/
#define ST 80	/* similar  't'		*/
#define BR 40	/* similar  braces	*/
#define BR_i 0	/* similar  braces for case letter 'i' with point {ш} */
#define DB 70	/* auxiliary beam	*/
#endif

/*----------------------------------------------------------------------*/
