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
/*	STIC-FRT.H	03.12.1993	TABLES (#include to STIC_FRT.C)	*/
/*----------------------------------------------------------------------*/
// 22.11.1993	Exclude STIC-FRT.H from STIC-TAB.H; (see STIC_FRT.C);
// 03.12.1993	VERS-086
/*----------------------------------------------------------------------*/
/* °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°° */
/* °°°								°°° */
/* °°°	tables for letters  f,r,t;				°°° */
/* °°°								°°° */
/* °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°° */

static uchar tab_f[]=					// dis_f
	{
	40,	// 0.  beam belong upper zone
	20,	// 1.  no beam : exist only one flags>1
	40,	// 2.  no beam : exist only one flags=1
	8,	// 3.  short beam : left flag=right flag=1
	2,	// 4.  meandr and inc
	4,	// 5.  meandr and no inc
	10,	// 6.  left upper pimple
	40,	// 7.  left upper long flags
	40,	// 8.  no right upper flags
	4,	// 9.  short right upper flags
	60,	// 10. similar    't','l'
	70,	// 11. similar 'F'
	80,	// 12. stick have false beam
	100,	// 13. similar [
//////	100,	// 14. too  low beam (b.l. too heigh )
	60,	// 14. too  low beam (b.l. too heigh )		09.03.1993
	BP,	// 15. MK: 80 for BAD PROPORTIONS (GLISTA)
//////	222	// 16. MK: UP-LEFT SERIF
	};

static uchar plus_f[]=    /* part of dis-value * 16 for bonus */
	{
	8,  /* 0.  exist left upper hole               */
	4   /* 1.  left_sig_f=right_sign_f=1           */
	};

static uchar  tab_r[]=					// dis_r
	{
	20,	// 0. beam : left flag > right		// OLD 8
	2,	// 1. upper right flag = 1
	70,	// 2. upper right flag = 0
	10,	// 3. exist left center flag
	10,	// 4. exist right center flag
	6,	// 5. for any superfluous left or right flag
	4,	// 6. too different down flags
	10,	// 7. long downer half serif
	60,	// 8. near right upper flag belong zone 1
	80,	// 9. too small upper flags
	20,	// 10. long downer flag, left downer not
	10,	// 11. two short right upper flags
	40,	// 12. too short upper flags
	100,	// 13. simmetrial upper zone and meandr
	50,	// 14. similar 'I'
	40,	// 15. left part of 'm' similar 'r'	??? NOT USED ???
	80,	// 16. MK too long LEFT.UP  24.03.1993
	40	// 17. MK: UP FLAGS 2:1	    25.10.1993
	};

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
//////	15,  /* 14. head : similar to 'f'                     */
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
//////	BP,  // 29. MK.80: too long hook: "t."==>'t'; fax12/32 "document."
	100,	// 29. MK: too long hook: "t."==>'t'; fax12/32 "document."
	16,	// 30. MK: about similar 'f'
	8,	// 31. MK: about similar 'f'
	44	// 32. MK: about some CUT Cases (first was 60)
	};

/*----------------------------------------------------------------------*/
