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
/*	STIC-TAB.H	16.12.1993	TABLES (#include to STIC_DIS.C)	*/
/*----------------------------------------------------------------------*/
// 05.01.1993	begin of STIC-TAB.H:	MACROS and TABLES
// 07.01.1993	tab_F ????????????????????????????????????????????????????
// 08.01.1993	BR_i, tab_i[4];  tab_t[29];
// 11.01.1993	tab_T[6]; tab_F[4];
// 20.01.1993	tab_Y[4], tab_T[7];
// 22.01.1993	tab_T[...], tab_F[5]
// 23.01.1993	tab_T[9] EXTRAPOLATION ASSIMETRY BAD PROPORTION
// 26.01.1992	tab_t[9] 222=>124
//              DIS_CENTER_FLAGS_1, DIS_CENTER_FLAG_1
// 01.02.1993	tab_F[5] 80=>100;  DIS_BEAM_MK;  tab_l[16]
// 09.02.1993	tab_l[16]
// 10.02.1993	tab_t[16,30,31]: 70,16,8
// 12.02.1993	tab_l[17] PROBA 16=>6
// 		tab_l[18], tab_I[15]: thin LEG exist
// 16.02.1993	tab_OxBA[22]: similar 'l'
// 17.02.1993	tab_f[16]
// 17.02.1993	tab_T[10]:  T_skok => T_skok_LR = (T_skok_L, T_skok_R);
// 25.02.1993	tab_I[16]; DIS_BEAM_MK (&& !inc)
//		DIS_BRACE for dis_l (BUMERANG)
// 26.02.1993	after FARA-6.0: DIS_BRACE
//		DIS_FLAGS_L_R
// 02/03.03.1993	Experiment about DIS_BRACES
// 03.03.1993	tab_F[4]: 80=>60 (PROBA)
// 09.03.1993	tab_f[14]: 100=>60 (PROBA)
// 10.03.1993	tab_t[29]: 80=>100 (PROBA)
// 24.03.1993	divide it to STIC-MAC.H, STIC-TAB.H, & STIC-TB.H
//		tab_r[16] for TOO int32_t LEFT.UP
// 09.06.1993	tab_l[19] about thin '(' or ')'
// 18.10.1993	tab_t[32]
// 19.10.1993	to FARA-078
// 20.10.1993	tab_t[32]: 60=>44;
// 20.10.1993	to FARA-079
// 25.10.1993	tab_r[0] 8=>20;  tab_r[17];
// 26.10.1993	to FARA-082
// 18.11.1993	tab_t[14] 15 => 14 (EVEN!!!)
// 19.11.1993	VERS-085;
// 22.11.1993	Exclude STIC-FRT.H from STIC-TAB.H;	(see STIC_FRT.C);
// 03.12.1993	VERS-086
// 15.12.1993	dis_OxBA => dis_0xBA (LETTER => DIGIT);
// 16.12.1993	VERS-088;
/*----------------------------------------------------------------------*/
/*  шшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшш */
/*  шш								шш */
/*  шш	Copyright (C) 1992 Cognitive Technology Corporation.	шш */
/*  шш	All rights reserved. This program is proprietary and	шш */
/*  шш	a trade secret of Cognitive Technology Corporation.	шш */
/*  шш								шш */
/*  шшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшш */
/*----------------------------------------------------------------------*/
/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟ */
/* ╟╟╟								╟╟╟ */
/* ╟╟╟	tables for letters  ╨,i,!,1,l;				╟╟╟ */
/* ╟╟╟								╟╟╟ */
/* ╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟╟ */

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
	SI,  /* 13. similar    ╨                          */
	40,  /* 14. similar    'l'                        */
	70,  /* 15. for 2 serifs                          */
	BR,  /* 16. similar    '(',')'                    */
	20,  /* 17. short nose                            */
	8,   /* 18. solid stick - small dis               */
	MD,  /* 19. cutting nose                          */
	BP   /* 20. bad proportions                       */
	};

static uchar tab_0xBA[]=
	{
	6,   /* 0. central long left flag                */
	20,  /* 1. up serif                              */
	10,  /* 2. bad down serif                        */
	16,  /* 3. for any superfluous flag              */
	30,  /* 4. not pitch and exist long flags        */
	BR,  /* 5. for configurations [,]                */
	LF,  /* 6. for any long central flag             */
	NF,  /* 7. for any near central flag             */
	20,  /* 8. for beam in zone 3                    */
	30,  /* 9. for uncorrect number of half_sefif    */
	40,  /* 10. for true stick : no flags, no concs  */
	20,  /* 11. long head                            */
	30,  /* 12. exist right long falgs by small left */
	26,  /* 13. differnts down serif                 */
	20,  /* 14. for inc                              */
	SI,  /* 15. bad neck : meandr and short flags    */
	40,  /* 16. similar    'l'                       */
	BP,  /* 17. bad proportions                      */
	40,  /* 18. similar    'J'                       */
	60,  /* 19. similar    'L'                       */
	120, /* 20. similar    't'                       */
	50,  /* 21. similar    '1'                       */
	50	// 22. similar	'l'	MK
	};

static uchar tab_i[]=
	{
	40,  /* 0. for long central flag       */
	6,   /* 1. for near central flag       */
	BR,  /* 2. for configurations [,]    without {Ь} */
	BR_i /* 3. for configurations [,] with point {Ь} */
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
	SI,	// 12. similar    '╨' : exist neck
	80,	// 13. similar    'L'
	BP,	// 14. bad proportions
	ST,	// 15. similar    't'
	14,	// 16. for DIS_BEAM_MK
	8,	// 17. SPEC CASE: NECK=1 wid>=6
	SI,	// 18. thin LEG exist
	6	// 19. about thin '(' or ')'	09.06.1993
	};

/*----------------------------------------------------------------------*/
