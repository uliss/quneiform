/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

пЮГПЕЬЮЕРЯЪ ОНБРНПМНЕ ПЮЯОПНЯРПЮМЕМХЕ Х ХЯОНКЭГНБЮМХЕ ЙЮЙ Б БХДЕ ХЯУНДМНЦН ЙНДЮ,
РЮЙ Х Б ДБНХВМНИ ТНПЛЕ, Я ХГЛЕМЕМХЪЛХ ХКХ АЕГ, ОПХ ЯНАКЧДЕМХХ ЯКЕДСЧЫХУ СЯКНБХИ:

      * оПХ ОНБРНПМНЛ ПЮЯОПНЯРПЮМЕМХХ ХЯУНДМНЦН ЙНДЮ ДНКФМШ НЯРЮБЮРЭЯЪ СЙЮГЮММНЕ
        БШЬЕ СБЕДНЛКЕМХЕ НА ЮБРНПЯЙНЛ ОПЮБЕ, ЩРНР ЯОХЯНЙ СЯКНБХИ Х ОНЯКЕДСЧЫХИ
        НРЙЮГ НР ЦЮПЮМРХИ.
      * оПХ ОНБРНПМНЛ ПЮЯОПНЯРПЮМЕМХХ ДБНХВМНЦН ЙНДЮ Б ДНЙСЛЕМРЮЖХХ Х/ХКХ Б
        ДПСЦХУ ЛЮРЕПХЮКЮУ, ОНЯРЮБКЪЕЛШУ ОПХ ПЮЯОПНЯРПЮМЕМХХ, ДНКФМШ ЯНУПЮМЪРЭЯЪ
        СЙЮГЮММЮЪ БШЬЕ ХМТНПЛЮЖХЪ НА ЮБРНПЯЙНЛ ОПЮБЕ, ЩРНР ЯОХЯНЙ СЯКНБХИ Х
        ОНЯКЕДСЧЫХИ НРЙЮГ НР ЦЮПЮМРХИ.
      * мХ МЮГБЮМХЕ Cognitive Technologies, МХ ХЛЕМЮ ЕЕ ЯНРПСДМХЙНБ МЕ ЛНЦСР
        АШРЭ ХЯОНКЭГНБЮМШ Б ЙЮВЕЯРБЕ ЯПЕДЯРБЮ ОНДДЕПФЙХ Х/ХКХ ОПНДБХФЕМХЪ
        ОПНДСЙРНБ, НЯМНБЮММШУ МЮ ЩРНЛ он, АЕГ ОПЕДБЮПХРЕКЭМНЦН ОХЯЭЛЕММНЦН
        ПЮГПЕЬЕМХЪ.

щрю опнцпюллю опеднярюбкемю бкюдекэжюлх юбрнпяйху опюб х/хкх дпсцхлх кхжюлх "йюй
нмю еярэ" аег йюйнцн-кхан бхдю цюпюмрхи, бшпюфеммшу ъбмн хкх ондпюгслебюелшу,
бйкчвюъ цюпюмрхх йнллепвеяйни жеммнярх х опхцндмнярх дкъ йнмйпермни жекх, мн ме
нцпюмхвхбюъяэ хлх. мх бкюдекеж юбрнпяйху опюб х мх ндмн дпсцне кхжн, йнрнпне
лнфер хглемърэ х/хкх онбрнпмн пюяопнярпюмърэ опнцпюллс, мх б йнел яксвюе ме
мея╗р нрберярбеммнярх, бйкчвюъ кчаше наыхе, яксвюимше, яоежхюкэмше хкх
онякеднбюбьхе сашрйх, ябъгюммше я хяонкэгнбюмхел хкх онмеяеммше бякедярбхе
мебнглнфмнярх хяонкэгнбюмхъ опнцпюллш (бйкчвюъ онрепх дюммшу, хкх дюммше,
ярюбьхе мецндмшлх, хкх сашрйх х/хкх онрепх днунднб, онмеяеммше хг-гю деиярбхи
рперэху кхж х/хкх нрйюгю опнцпюллш пюанрюрэ янблеярмн я дпсцхлх опнцпюллюлх,
мн ме нцпюмхвхбюъяэ щрхлх яксвюълх), мн ме нцпюмхвхбюъяэ хлх, дюфе еякх рюйни
бкюдекеж хкх дпсцне кхжн ашкх хгбеыемш н бнглнфмнярх рюйху сашрйнб х онрепэ.

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
/*	STICK.H		10.01.1994					*/
/*----------------------------------------------------------------------*/
// 02.11.1992	3,350	old OLEG Version
// 21.01.1993   Begin of MK Work:
//		OLD: T_config, Y_config;
//		NEW: T_hor, T_2, T_3, T_skok;
// 08.02.1993	signums->base_3
// 17.02.1993	T_skok => T_skok_L, T_skok_R
// 01.06.1993	SIGNUMS: lll_nose_1  (FLAT VERT part of NOSE >= 3)
// 01.06.1993	SIGNUMS: inc_v16;
// 05.08.1993	add for up_dot;
// 26.08.1993	to FARA 7.3
// 26.08.1993	DELETE  up_dot_NAVIS;
// 26.08.1993	to FARA 7.4
// 15.11.1993	add .num to struct INC_BASE;
// 19.11.1993	VERS-085;
// 10.12.1993	OLD  uchar inc_v16 => char inc_num;
//		VERS-087;
// 15.12.1993	xxx_OxBA => xxx_0xBA (LETTER => DIGIT);
// 16.12.1993	VERS-088;
// 03.01.1994	begin work wist base_2mk, base_3mk, base_4mk;
// 06.01.1994	l,r->max_flag;
// 07.01.1994	s->full_width;  s->left_mode, s->right_mode;
// 09.01.1994	s->t_TOP (t^ or t~);
// 10.01.1993	VERS-089;
/*----------------------------------------------------------------------*/
/*
  шшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшш
  шш                                                              шш
  шш     Copyright (C) 1990 Cognitive Technology Corporation.     шш
  шш	 All rights reserved. This program is proprietary and     шш
  шш     a trade secret of Cognitive Technology Corporation.      шш
  шш                                                              шш
  шшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшш
*/

#ifndef D_STICK_TYPES
#define D_STICK_TYPES
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
	int16_t     height,		// height of c_comp
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
//////		dis_OxBA,	// diskrim ╨ for his head
		dis_0xBA,	// diskrim ╨ for his head
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
	uchar    num_lines;      /* Number of with none single interval 	*/
	int16_t	incline;	/* Normal tg(stick)*2048 + line_incline */
	} STICK_SIGNUMS;	// auxiliary information for
				// stick diskrimination
/*----------------------------------------------------------------------*/
typedef struct
        {
	int16_t tg;             /* tg = tangens*2048  */
	int16_t num;	    /* 15.11.1993 MK NEW  */
	int16_t inc[128];       /* table of inc       */
	} INC_BASE;         /* base tables of inc */
/*----------------------------------------------------------------------*/
#define ADD_PROB(c) (c->nvers ? (c->vers[0].prob - 4) : 254 )

#endif
/*----------------------------------------------------------------------*/
