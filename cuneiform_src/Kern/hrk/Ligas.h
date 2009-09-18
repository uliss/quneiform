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

#ifndef __LIGAS__
#define __LIGAS__

//#include "face_lng.h"
#include "cttypes.h"	// uchar 01.09.2000 E.P.
#include "lang_def.h"	// 12.09.2000 E.P.

#define ligas_beg       0xb1
#define liga_fi         0xb1
#define liga_fl         0xb2
#define liga_ff         0xb3
#define liga_ffi        0xb4
#define liga_rt         0xb5
#define liga_ri         0xb6
#define liga_uperc      0xb7  // upper half percent
#define liga_lperc      0xb8  // lower half percent
#define liga_ffl        0xb9
#define liga_i_usual    0xba  // i with dot
#define liga_j          0xbb  // j with dot
#define liga_exm_usual  0xbc  // ! with dot
#define liga_qm         0xbd  // ? with dot
#define liga_end_lat    0xbd
#define liga_inv_exm_usual    0xbe    // ­ with dot
#define liga_inv_qm     0xbf    // Ё with dot
#define ligas_end       0xbf

// Макры для кодов лигатур 31.08.2000 E.P.
// При добавлении новой макры нужно добавить в
// space_ligas() в SPACE.C, а также
// correct_let_tables() в ACC_TABS.C и PROPTAB.C
#define liga_i			((uchar)(is_cen_language(language)?liga_i_latin:\
						(is_baltic_language(language)?liga_i_baltic:liga_i_usual)))
#define liga_exm		((uchar)(is_cen_language(language)?liga_exm_latin:liga_exm_usual))
#define liga_inv_exm	((uchar)(is_cen_language(language)?liga_inv_exm_latin:liga_inv_exm_usual))
#define right_quocket	((uchar)(is_cen_language(language)?right_quocket_latin:\
						(is_baltic_language(language)?right_quocket_baltic:right_quocket_usual)))

#define liga_CC			((uchar)(is_cen_language(language)?liga_CC_latin:\
						(is_baltic_language(language)?liga_CC_baltic:\
						(is_turkish_language(language)?liga_CC_turkish:liga_CC_usual))))

#define liga_CR			((uchar)(is_cen_language(language)?liga_CR_latin:\
						(is_baltic_language(language)?liga_CR_baltic:\
						(is_turkish_language(language)?liga_CR_turkish:liga_CR_usual))))

#define liga_bull		((uchar)(is_cen_language(language)?liga_bull_latin:\
						(is_baltic_language(language)?liga_bull_baltic:\
						(is_turkish_language(language)?liga_bull_turkish:liga_bull_usual))))

#define low_quotes		(language==LANG_RUSSIAN?low_quotes_rus:low_quotes_usual)

#define liga_TM			((uchar)(is_cen_language(language)?liga_TM_latin:\
						(is_baltic_language(language)?liga_TM_baltic:\
						(is_turkish_language(language)?liga_TM_turkish:liga_TM_usual))))

#define AA_bottom_accent ((uchar)(is_baltic_language(language)?AA_bottom_accent_baltic:AA_bottom_accent_latin))
#define a_bottom_accent  ((uchar)(is_baltic_language(language)?a_bottom_accent_baltic:a_bottom_accent_latin))
#define EE_bottom_accent ((uchar)(is_baltic_language(language)?EE_bottom_accent_baltic:EE_bottom_accent_latin))
#define e_bottom_accent  ((uchar)(is_baltic_language(language)?e_bottom_accent_baltic:e_bottom_accent_latin))

#define SS_inv_roof		 ((uchar)(is_baltic_language(language)?SS_inv_roof_baltic:SS_inv_roof_latin))
#define s_inv_roof		 ((uchar)(is_baltic_language(language)?s_inv_roof_baltic:s_inv_roof_latin))

#define SS_bottom_accent ((uchar)(is_turkish_language(language)?SS_bottom_accent_turkish:SS_bottom_accent_latin))
#define s_bottom_accent	 ((uchar)(is_turkish_language(language)?s_bottom_accent_turkish:s_bottom_accent_latin))

#define ZZ_inv_roof		 ((uchar)(is_baltic_language(language)?ZZ_inv_roof_baltic:ZZ_inv_roof_latin))
#define z_inv_roof		 ((uchar)(is_baltic_language(language)?z_inv_roof_baltic:z_inv_roof_latin))


// Лиги для cp1250 30.08.2000 E.P.
#define liga_i_latin		  0xa0 // 0xba
#define liga_exm_latin        0x90 // 0xbc  // ! with dot
#define liga_inv_exm_latin    0    // 0xbe выкинуть
#define right_quocket_latin   0xa2 // 0xd8  // >>
#define liga_CC_latin         0xa9 // 0xdd  // Copyright in Circle
#define liga_CR_latin         0xae // 0xde  // ® (R) in Circle
#define liga_bull_latin		  0xff // 0xfe bullet
#define liga_TM_latin		  0xa8 // 0xd0

// Following codes should agree with
// leftlig_cen[] and rightlig_cen[] in SPACES.C
#define ligas_beg_latin 0x80
#define liga_ff_latin   0x80
#define liga_ffl_latin  0x81
#define ligas_end_latin 0x81

// Новые языки 30.08.2000 E.P.
#define is_cen_language(language) (\
(language)==LANG_CROATIAN||(language)==LANG_POLISH ||\
(language)==LANG_CZECH||(language)==LANG_ROMAN ||\
(language)==LANG_HUNGAR||(language)==LANG_SLOVENIAN\
)

#define is_ligas_lat(let )  ((let)>=ligas_beg && (let)<=ligas_end)

// Лиги для cp1250 30.08.2000 E.P.
#define is_ligas_latin(let)  (\
(let)==liga_fi|| (let)==liga_fl|| (let)==liga_ff_latin|| (let)==liga_ffi|| \
(let)==liga_rt|| (let)==liga_ri|| (let)==liga_uperc|| (let)==liga_lperc|| \
(let)==liga_ffl_latin || (let)==liga_i_latin|| (let)==liga_j|| \
(let)==liga_exm_latin|| \
(let)==liga_qm || (let)==right_quocket_latin || \
(let)==liga_CC_latin || (let)==liga_CR_latin || \
(let)==liga_bull_latin\
)

// Лиги для cp1257 10.07.2001 E.P.
#define is_ligas_baltic(let)  (\
(let)==liga_fi|| (let)==liga_fl|| (let)==liga_ff|| (let)==liga_ffi|| \
(let)==liga_rt|| (let)==liga_ri|| (let)==liga_uperc|| (let)==liga_lperc|| \
(let)==liga_ffl|| (let)==liga_i_baltic|| (let)==liga_j|| \
(let)==liga_exm_baltic|| \
(let)==liga_qm || (let)==right_quocket_baltic || \
(let)==liga_CC_baltic || (let)==liga_CR_baltic || \
(let)==liga_bull_baltic\
)

// Лиги для cp1254 20.05.2002 E.P.
#define is_ligas_turkish(let)  (\
(let)==liga_fi|| (let)==liga_fl|| (let)==liga_ff|| (let)==liga_ffi|| \
(let)==liga_rt|| (let)==liga_ri|| (let)==liga_uperc|| (let)==liga_lperc|| \
(let)==liga_ffl|| (let)==liga_i|| (let)==liga_j|| \
(let)==liga_exm_baltic|| \
(let)==liga_qm || (let)==right_quocket || \
(let)==liga_CC_turkish ||(let)==liga_CR_turkish || \
(let)==liga_TM_turkish ||(let)==liga_bull_turkish\
)

#define is_liga(let) (\
is_cen_language(language)?is_ligas_latin(let):\
(is_baltic_language(language)?is_ligas_baltic(let):\
(is_turkish_language(language)?is_ligas_turkish(let):\
is_ligas_lat(let))))

#define bad_char        0xb0

#define ED_ASCII        0
#define ED_WIN          1
#define ED_MAC          2
#define ED_WIN_FRENCH   3

#ifdef __MAC__
	#define ED_VALUE ED_MAC
#else
    #ifdef WIN_FRENCH
	#define ED_VALUE ED_WIN_FRENCH
    #else
	#define ED_VALUE ED_WIN
    #endif
#endif

#ifndef __KERNEL__
        extern  uchar     fEdCode; // Change code letter in module LINUTIL.C
        extern  uchar     bEdCode[];
#else
        uchar   fEdCode = 0;  //ASCII
        uchar   bEdCode[4]={//  ASCII, WIN, MAC, FRN
                                0xFE,0x95,0xA5,0xB0 // BULLET
                              };
#endif

#define liga_bull_usual       bEdCode[fEdCode]  // ю BULLET
#define liga_bull_usual       bEdCode[fEdCode]  // ю BULLET

#define left_quocket		  0xd7    // 0xab  <<
#define right_quocket_usual   0xd8    // 0xad  >>
#define low_quotes_usual      0x84	  // 31.05.2001 E.P.
#define low_quotes_rus		  0xd5    //  Pit  0xaf    // ,,

/***** foreign languages symbols ******************/

#define AA_left_accent      0xC0
#define a_left_accent       0xE0
#define AA_right_accent     0xC1
#define a_right_accent      0xE1
#define AA_roof_accent      0xC2
#define a_roof_accent       0xE2
#define AA_tild_accent      0xC3
#define a_tild_accent       0xE3
#define AA_2dot_accent      0xC4
#define a_2dot_accent       0xE4
#define AA_circle_accent    0xC5
#define a_circle_accent     0xE5
#define EE_left_accent      0xC8
#define e_left_accent       0xE8
#define EE_right_accent     0xC9
#define e_right_accent      0xE9
#define EE_roof_accent      0xCA
#define e_roof_accent       0xEA
#define EE_2dot_accent      0xCB
#define e_2dot_accent       0xEB
#define II_left_accent      0xCC
#define i_left_accent       0xEC
#define II_right_accent     0xCD
#define i_right_accent      0xED
#define II_roof_accent      0xCE
#define i_roof_accent       0xEE
#define II_2dot_accent      0xCF
#define i_2dot_accent       0xEF
#define NN_tild_accent      0xD1
#define n_tild_accent       0xF1
#define OO_left_accent      0xD2
#define o_left_accent       0xF2
#define OO_right_accent     0xD3
#define o_right_accent      0xF3
#define OO_roof_accent      0xD4
#define o_roof_accent       0xF4
#define OO_tild_accent      0xD5
#define o_tild_accent       0xF5
#define OO_2dot_accent      0xD6
#define o_2dot_accent       0xF6
#define UU_left_accent      0xD9
#define u_left_accent       0xF9
#define UU_right_accent     0xDA
#define u_right_accent      0xFA
#define UU_roof_accent      0xDB
#define u_roof_accent       0xFB
#define UU_2dot_accent      0xDC
#define u_2dot_accent       0xFC
#define AE_cap_deaf_sound   0xC6
#define ae_deaf_sound       0xE6
#define OE_cap_deaf_sound   0xAA
#define oe_deaf_sound       0xAC
#define ss_deaf_sound       0xDF
#define CC_bottom_accent    0xC7
#define c_bottom_accent     0xE7
#define invers_exm          0xA1
#define invers_qm           0xA0
#define OO_crossed          0xD8
#define o_crossed           0xF8

/****** accents ************/
/*
Таблица acc_tab в ACC_TABS.C теперь типа uint16_t, для добавления
новых типов акцентов. См. также ACCENTS.C. 30.08.2000 E.P.
*/
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

// Russian cursiv symbols

#define r_cu_d  0xf0    // cursiv d tail up
#define r_cu_g  0xf1    // cursiv d tail down
#define r_cu_m  0xf5    // cursiv t
#define r_cu_u  0xf7    // cursiv ee
#define r_cu_z  0xf8    // cursiv g
#define r_cu_a  0xfd    // a

#define r_EE_2dot  0xC8    // russian E ..  = 200
#define r_e_2dot   0xC0    // russian e ..  = 192

#ifdef WIN_FRENCH
// ЇҐаҐЄалў ойЁҐбп бЁ¬ў®«л ­ ЎЁа Ґ¬лҐ ­  Є« ўЁ вгаҐ
// ў аҐ¦Ё¬Ґ а бЇ®§­ ў ­Ёп да ­жг§бЄ®Ј® п§лЄ 
	#define KEY_D5   0x91
	#define KEY_D7   0x92
	#define KEY_D8   0x93
#endif

#define liga_TM_usual  0xd0    // TM конфликт с Croatian DD_stroked 31.05.2001
#define liga_CC_usual  0xdd    // Copyright in Circle
#define liga_CR_usual  0xde    // TM (R) in Circle

/***** ukrainian symbols ! intersect foreign languages symbols **************/
#define  UKR_I   0xC6
#define  UKR_i   0xDA
#define  UKR_II  0xC5
#define  UKR_ii  0xC2
#define  UKR_E   0xCC
#define  UKR_e   0xC9
#define  UKR_G   0xC7
#define  UKR_g   0xD6

//       LIGATURA ASCII      ALT ANSI
/***** serbian symbols ! intersect foreign languages symbols **************/
#define  SERB_HH  0xD3 //211 M У Ox80 128 Ђ
#define  SERB_hh  0xD4 //212 m Ф Ox90 144 ђ
#define  SERB_L   0xCD //205 W Н Ox8A 138 Љ
#define  SERB_l   0xC4 //196 w Д Ox9A 154 љ
#define  SERB_N   0xCB //203 T Л Ox8C 140 Њ
#define  SERB_n   0xC2 //194 t В Ox9C 156 њ
#define  SERB_H   0xD2 //210 I Т Ox8E 142 Ћ
#define  SERB_h   0xD1 //209 i С Ox9E 158 ћ
#define  SERB_U   0xCA //202 B К Ox8F 143 Џ
#define  SERB_u   0xC1 //193 b Б Ox9F 159 џ
#define  SERB_J   0xD6 //        OxA3 163 Ј
#define  SERB_j   0xD9 //        OxBC 188 ј


/****** polish characters ********************************************/
#define AA_bottom_accent_latin    0xA5 //165
#define a_bottom_accent_latin     0xB9 //185
#define EE_bottom_accent_latin    0xCA //202
#define e_bottom_accent_latin     0xEA //234
#define SS_right_accent     0x8C //140
#define s_right_accent      0x9C //156
#define ZZ_right_accent     0x8F //143
#define z_right_accent      0x9F //159
#define ZZ_dot_accent       0xAF //175
#define z_dot_accent        0xBF //191
#define CC_right_accent     0xC6 //198
#define c_right_accent      0xE6 //230
#define NN_right_accent     0xD1 //209
#define n_right_accent      0xF1 //241
#define OO_right_accent     0xD3 //211
#define o_right_accent      0xF3 //243
/***** CROATIAN **************/
#define  DD_stroked  0xD0      //  208
#define  d_stroked   0xF0      //  240
#define  SS_inv_roof_latin 0x8A      //  138
#define  s_inv_roof_latin  0x9A      //  154
#define  ZZ_inv_roof_latin 0x8E      //  142
#define  z_inv_roof_latin  0x9E      //  158
#define  CC_inv_roof 0xC8      //  200
#define  c_inv_roof  0xE8      //  232

#define  POLISH_SS    SS_right_accent
#define  POLISH_s     s_right_accent
#define  POLISH_ZZR   ZZ_right_accent
#define  POLISH_zr    z_right_accent
#define  POLISH_LL    0xA3 //163   L stroked
#define  POLISH_l     0xB3 //179
#define  POLISH_AA    AA_bottom_accent_latin
#define  POLISH_a     a_bottom_accent_latin
#define  POLISH_ZZD   ZZ_dot_accent
#define  POLISH_zd    z_dot_accent
#define  POLISH_CC    CC_right_accent
#define  POLISH_c     c_right_accent
#define  POLISH_EE    EE_bottom_accent_latin
#define  POLISH_e     e_bottom_accent_latin
#define  POLISH_NN    NN_right_accent
#define  POLISH_n     n_right_accent
#define  POLISH_OO    OO_right_accent
#define  POLISH_o     o_right_accent

/***** CROATIAN **************/
#define  CROAT_D      DD_stroked
#define  CROAT_d      d_stroked
#define  CROAT_SR     SS_inv_roof_latin
#define  CROAT_sr     s_inv_roof_latin
#define  CROAT_CA     CC_right_accent
#define  CROAT_ca     c_right_accent
#define  CROAT_ZR     ZZ_inv_roof_latin
#define  CROAT_zr     z_inv_roof_latin
#define  CROAT_CR     CC_inv_roof
#define  CROAT_cr     c_inv_roof
#endif

#define SS_NEG_HALF_SPACE 0x1e
#define SS_POS_HALF_SPACE 0x1f
#define SS_SPACE          0x20

// Новые буквы для CZECH, ROMAN, HUNGAR 30.08.2000 E.P.
// См. также функцию fromcp1250_to_cp852() в ACC_TABS.C
#define AA_semicircle		0xC3
#define a_semicircle		0xE3
#define DD_inv_roof			0xCF
#define d_inv_roof			0xEF
#define EE_inv_roof			0xCC
#define e_inv_roof			0xEC
#define NN_inv_roof			0xD2
#define n_inv_roof			0xF2
#define OO_double_right		0xD5
#define o_double_right		0xF5
#define RR_inv_roof			0xD8
#define r_inv_roof			0xF8
#define SS_bottom_accent_latin    0xAA	// См. макрос SS_bottom_accent 18.05.2002 E.P.
#define s_bottom_accent_latin     0xBA	// См. макрос s_bottom_accent  18.05.2002 E.P.
#define TT_inv_roof			0x8D
#define t_inv_roof			0x9D
#define TT_bottom_accent    0xDE
#define t_bottom_accent     0xFE
#define UU_double_right		0xDB
#define u_double_right		0xFB
#define UU_circle_accent    0xD9
#define u_circle_accent     0xF9
#define YY_right_accent     0xDD
#define y_right_accent      0xFD

// ********************************************************************************
// Балтийские языки. 05.07.2001 E.P.

#define is_baltic_language(language) (\
(language)==LANG_LATVIAN||\
(language)==LANG_LITHUANIAN||\
(language)==LANG_ESTONIAN\
)

// Лиги для балтийских языков. 05.07.2001 E.P.
#define low_quotes_baltic	 0x84	// = low_quotes_usual
#define liga_exm_baltic		 0xbc	// = liga_exm_usual
#define liga_i_baltic		 0xa0	// = liga_i_latin
#define right_quocket_baltic 0xa2	// = right_quocket_latin
#define liga_TM_baltic		 0xa8	// = liga_TM_latin
#define liga_CC_baltic		 0xa9	// = liga_CC_latin
#define liga_CR_baltic		 0xae	// = liga_CR_latin
#define liga_bull_baltic	 0xff	// = liga_bull_latin

// Following codes should agree with
// leftlig_baltic[] and rightlig_baltic[] in SPACE.C
#define ligas_beg_baltic	0xb1	// except for 0xba
#define ligas_end_baltic	0xbd	// except for 0xba

// Буквы балтийских языков, cp1257. 06.07.2001 E.P.
/*
#define AA_2dot_accent      0xc4	   196 = западно-европейская cp1252
#define a_2dot_accent       0xe4	   228 = западно-европейская cp1252
*/
#define AA_macron			0xc2	// 194 Макрон есть черта сверху над буквой
#define a_macron			0xe2	// 226
#define AA_bottom_accent_baltic 0xc0// 192
#define a_bottom_accent_baltic	0xe0// 224
/*
#define CC_inv_roof			0xc8	   200 = центрально-европейская cp1250
#define c_inv_roof			0xe8	   232 = центрально-европейская cp1250
*/
#define EE_macron			0xc7	// 199 Макрон есть черта сверху над буквой
#define e_macron			0xe7	// 231
#define EE_bottom_accent_baltic 0xc6// 198
#define e_bottom_accent_baltic	0xe6// 230
#define EE_dot_accent		0xcb	// 203 Точка сверху, как над польской буквой ZZ_dot_accent
#define e_dot_accent		0xeb	// 235 Точка сверху, как над польской буквой z_dot_accent

#define GG_bottom_accent	0xcc	// 204 Акцент внизу, посередине
#define LATVIAN_g			0xec	// 236 Акцент над g сверху, различной формы: опрокинутая
									//     запятая, рог с правым наклоном и т.д.
#define II_macron			0xce	// 206 Макрон есть черта сверху над буквой
#define i_macron			0xee	// 238
#define II_bottom_accent	0xc1	// 193
#define i_bottom_accent		0xe1	// 225

#define KK_bottom_accent	0xcd	// 205
#define k_bottom_accent		0xed	// 237

#define LL_bottom_accent	0xcf	// 207
#define l_bottom_accent		0xef	// 239

#define NN_bottom_accent	0xd2	// 210
#define n_bottom_accent		0xf2	// 242
/*
#define OO_tild_accent      0xd5	   213 = западно-европейская cp1252
#define o_tild_accent       0xf5	   245 = западно-европейская cp1252
#define OO_2dot_accent      0xd6	   214 = западно-европейская cp1252
#define o_2dot_accent       0xf6	   246 = западно-европейская cp1252
*/
#define SS_inv_roof_baltic	0xd0	// 208
#define s_inv_roof_baltic	0xf0	// 240
/*
#define UU_2dot_accent      0xdc	   220 = западно-европейская cp1252
#define u_2dot_accent       0xfc	   252 = западно-европейская cp1252
*/
#define UU_bottom_accent	0xd8	// 216
#define u_bottom_accent		0xf8	// 248
#define UU_macron			0xdb	// 219
#define u_macron			0xfb	// 251

#define ZZ_inv_roof_baltic	0xde	// 222
#define z_inv_roof_baltic	0xfe	// 254

// ********************************************************************************
// Турецкий язык. 18.05.2002 E.P.

#define is_turkish_language(language) ((language)==LANG_TURKISH)

// Лиги для турецкого языка
#define liga_TM_turkish		 0x99	// 153
#define liga_CC_turkish		 0xa9	// = liga_CC_latin	= liga_CC_baltic
#define liga_CR_turkish		 0xae	// = liga_CR_latin	= liga_CR_baltic
#define liga_bull_turkish	 0x95	// 149

// Following codes should agree with
// leftlig_turkish[] and rightlig_turkish[] in SPACE.C
#define ligas_beg_turkish	0xb1	//
#define ligas_end_turkish	0xbd	//

// Буквы турецкого языка, cp1254.
/*
#define AA_roof_accent      0xc2	194 = западно-европейская cp1252
#define a_roof_accent       0xe2	226 = западно-европейская cp1252
#define CC_bottom_accent    0xC7	199 = западно-европейская cp1252
#define c_bottom_accent     0xE7	231 = западно-европейская cp1252
#define II_roof_accent      0xCE	206 = западно-европейская cp1252
#define i_roof_accent       0xEE	238 = западно-европейская cp1252
*/
#define GG_semicircle		0xd0 //	208
#define g_semicircle		0xf0 //	240
/*
#define OO_2dot_accent      0xD6	214 = западно-европейская cp1252
#define o_2dot_accent       0xF6	246 = западно-европейская cp1252
#define UU_roof_accent      0xDB	219 = западно-европейская cp1252
#define u_roof_accent       0xFB	251 = западно-европейская cp1252
#define UU_2dot_accent      0xDC	220 = западно-европейская cp1252
#define u_2dot_accent       0xFC	252 = западно-европейская cp1252
*/
#define II_dot_accent		0xdd //	221 = западно-европейская cp1252
#define i_sans_accent		0xfd //	253 = западно-европейская cp1252

#define SS_bottom_accent_turkish    0xde // 222
#define s_bottom_accent_turkish     0xfe // 254

// ********************************************************************************
