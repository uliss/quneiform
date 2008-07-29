/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Ðàçðåøàåòñÿ ïîâòîðíîå ðàñïðîñòðàíåíèå è èñïîëüçîâàíèå êàê â âèäå èñõîäíîãî êîäà,
òàê è â äâîè÷íîé ôîðìå, ñ èçìåíåíèÿìè èëè áåç, ïðè ñîáëþäåíèè ñëåäóþùèõ óñëîâèé:

      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè èñõîäíîãî êîäà äîëæíû îñòàâàòüñÿ óêàçàííîå
        âûøå óâåäîìëåíèå îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è ïîñëåäóþùèé
        îòêàç îò ãàðàíòèé.
      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè äâîè÷íîãî êîäà â äîêóìåíòàöèè è/èëè â
        äðóãèõ ìàòåðèàëàõ, ïîñòàâëÿåìûõ ïðè ðàñïðîñòðàíåíèè, äîëæíû ñîõðàíÿòüñÿ
        óêàçàííàÿ âûøå èíôîðìàöèÿ îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è
        ïîñëåäóþùèé îòêàç îò ãàðàíòèé.
      * Íè íàçâàíèå Cognitive Technologies, íè èìåíà åå ñîòðóäíèêîâ íå ìîãóò
        áûòü èñïîëüçîâàíû â êà÷åñòâå ñðåäñòâà ïîääåðæêè è/èëè ïðîäâèæåíèÿ
        ïðîäóêòîâ, îñíîâàííûõ íà ýòîì ÏÎ, áåç ïðåäâàðèòåëüíîãî ïèñüìåííîãî
        ðàçðåøåíèÿ.

ÝÒÀ ÏÐÎÃÐÀÌÌÀ ÏÐÅÄÎÑÒÀÂËÅÍÀ ÂËÀÄÅËÜÖÀÌÈ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È/ÈËÈ ÄÐÓÃÈÌÈ ËÈÖÀÌÈ "ÊÀÊ
ÎÍÀ ÅÑÒÜ" ÁÅÇ ÊÀÊÎÃÎ-ËÈÁÎ ÂÈÄÀ ÃÀÐÀÍÒÈÉ, ÂÛÐÀÆÅÍÍÛÕ ßÂÍÎ ÈËÈ ÏÎÄÐÀÇÓÌÅÂÀÅÌÛÕ,
ÂÊËÞ×Àß ÃÀÐÀÍÒÈÈ ÊÎÌÌÅÐ×ÅÑÊÎÉ ÖÅÍÍÎÑÒÈ È ÏÐÈÃÎÄÍÎÑÒÈ ÄËß ÊÎÍÊÐÅÒÍÎÉ ÖÅËÈ, ÍÎ ÍÅ
ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ. ÍÈ ÂËÀÄÅËÅÖ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È ÍÈ ÎÄÍÎ ÄÐÓÃÎÅ ËÈÖÎ, ÊÎÒÎÐÎÅ
ÌÎÆÅÒ ÈÇÌÅÍßÒÜ È/ÈËÈ ÏÎÂÒÎÐÍÎ ÐÀÑÏÐÎÑÒÐÀÍßÒÜ ÏÐÎÃÐÀÌÌÓ, ÍÈ Â ÊÎÅÌ ÑËÓ×ÀÅ ÍÅ
ÍÅÑ¨Ò ÎÒÂÅÒÑÒÂÅÍÍÎÑÒÈ, ÂÊËÞ×Àß ËÞÁÛÅ ÎÁÙÈÅ, ÑËÓ×ÀÉÍÛÅ, ÑÏÅÖÈÀËÜÍÛÅ ÈËÈ
ÏÎÑËÅÄÎÂÀÂØÈÅ ÓÁÛÒÊÈ, ÑÂßÇÀÍÍÛÅ Ñ ÈÑÏÎËÜÇÎÂÀÍÈÅÌ ÈËÈ ÏÎÍÅÑÅÍÍÛÅ ÂÑËÅÄÑÒÂÈÅ
ÍÅÂÎÇÌÎÆÍÎÑÒÈ ÈÑÏÎËÜÇÎÂÀÍÈß ÏÐÎÃÐÀÌÌÛ (ÂÊËÞ×Àß ÏÎÒÅÐÈ ÄÀÍÍÛÕ, ÈËÈ ÄÀÍÍÛÅ,
ÑÒÀÂØÈÅ ÍÅÃÎÄÍÛÌÈ, ÈËÈ ÓÁÛÒÊÈ È/ÈËÈ ÏÎÒÅÐÈ ÄÎÕÎÄÎÂ, ÏÎÍÅÑÅÍÍÛÅ ÈÇ-ÇÀ ÄÅÉÑÒÂÈÉ
ÒÐÅÒÜÈÕ ËÈÖ È/ÈËÈ ÎÒÊÀÇÀ ÏÐÎÃÐÀÌÌÛ ÐÀÁÎÒÀÒÜ ÑÎÂÌÅÑÒÍÎ Ñ ÄÐÓÃÈÌÈ ÏÐÎÃÐÀÌÌÀÌÈ,
ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÝÒÈÌÈ ÑËÓ×ÀßÌÈ), ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ, ÄÀÆÅ ÅÑËÈ ÒÀÊÎÉ
ÂËÀÄÅËÅÖ ÈËÈ ÄÐÓÃÎÅ ËÈÖÎ ÁÛËÈ ÈÇÂÅÙÅÍÛ Î ÂÎÇÌÎÆÍÎÑÒÈ ÒÀÊÈÕ ÓÁÛÒÊÎÂ È ÏÎÒÅÐÜ.

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
/*	STIC-TB.H	19.11.1993	TABLES (#include to STIC_BIG.C)	*/
/*----------------------------------------------------------------------*/
// 	begin it  24.03.1993	(from STIC-TAB.H):
/*----------------------------------------------------------------------*/
//		OLD HISTORY (about BIG only):
// 05.01.1993	begin of STIC-TAB.H:	MACROS and TABLES
// 07.01.1993	tab_F ????????????????????????????????????????????????????
// 11.01.1993	tab_T[6]; tab_F[4];
// 20.01.1993	tab_Y[4], tab_T[7];
// 22.01.1993	tab_T[...], tab_F[5]
// 23.01.1993	tab_T[9] EXTRAPOLATION ASSIMETRY BAD PROPORTION
// 01.02.1993	tab_F[5] 80=>100;  DIS_BEAM_MK;
// 17.02.1993	tab_T[10]:  T_skok => T_skok_LR = (T_skok_L, T_skok_R);
// 25.02.1993	tab_I[16]; DIS_BEAM_MK (&& !inc)
// 03.03.1993	tab_F[4]: 80=>60 (PROBA)
// 24.03.1993	divide STIC-TAB.H to STIC-MAC.H, STIC-TAB.H, & STIC-TB.H
// .............
// 09.06.1993	tab_I[17] about thin '(' or ')'
// 29.09.1993	tab_sq_brace [5];
// 30.09.1993	tab_sq_brace [6];
// 12.10.1993	tab_L [7];  tab_F [1,2,3] = 30,60,90 (PROISVOLNO)
// 18.10.1993	tab_F [6];
// 19.10.1993	to FARA-078
// 26.10.1993	to FARA-082
// 18.11.1993	tab_T [10];
// 19.11.1993	VERS-085;
/*----------------------------------------------------------------------*/
/*  ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ */
/*  ÛÛ                                                              ÛÛ */
/*  ÛÛ     Copyright (C) 1992 Cognitive Technology Corporation.     ÛÛ */
/*  ÛÛ	   All rights reserved. This program is proprietary and     ÛÛ */
/*  ÛÛ     a trade secret of Cognitive Technology Corporation.      ÛÛ */
/*  ÛÛ                                                              ÛÛ */
/*  ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ */
/*----------------------------------------------------------------------*/
/* °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°° */
/* °°°                                                          °°° */
/* °°° tables for letter º,i,!,[,],1,I,l,L,T,Y,J,/, f,r,t,(,)   °°° */
/* °°°                                                          °°° */
/* °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°° */

static BYTE tab_F[]=	// MK new Discrim dis_F, use l_f_symptom 11.01.1993
	{
	0,	// 0. l_f_symptom=0: NO DISCRIM
	30,	// 1. =1
	60,	// 2. =2
	90,	// 3. =3
//////	BP,	// 4. (80) BAD PROP: "fi"=>"Fi"
	60,	// 4. (60) BAD PROP: "fi"=>"Fi"		// 03.03.1993 PROBA
//////	BP	// 5. (80) BAD PROP: for 'E' => 'F' (R.DOWN HOOK)
	100,	// 5. .............. for 'E' => 'F' (R.DOWN HOOK)  // 01.02.93
	40	// 6. for GLUE
	};			// (and for other cases)

static BYTE tab_I[]=				// dis_I
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
	SI,	// 10. similar    º : exist neck
	70,	// 11. low c_comp
	BP,	// 12. bad proportions
	ST,	// 13. similar    't'
	BR,	// 14. similar    '(',')'
	SI,	// 15. thin LEG exist
	14,	// 16. for DIS_BEAM_MK
	6	// 17. about thin '(' or ')'	09.06.1993
	};

static BYTE tab_J[]=				// dis_J
	{
	100, /* 0. no left downer flag                 */
	20,  /* 1. too near left downer flag           */
	80,  /* 2. short left downer pits and no serif */
	6,   /* 3. too different upper serif           */
	40,  /* 4. for any long central flag           */
	6    /* 5. for any near central flag           */
	};

static BYTE tab_L[]=				// dis_L
	{
	40, /* 0. for long central flag		*/
	6,  /* 1. for near central flag		*/
	30, /* 2. too different flags		*/
	70, /* 3. too short down right flag	*/
	LF, /* 4. for any long center flag	*/
	NF, /* 5. for any near center flag	*/
	40, /* 6. similar    º			*/
	100 /* 7. too LONG down right flag	*/
	};

static BYTE tab_T[]=				// for dis_T
	{
	40,	// 0. for long central flag
	6,	// 1. for near central flag
	10,	// 2. for any point of curve
	6,	// 3. too different flags
	60,	// 4. too small upper flags
//////	70,	// 5. hole in beam : similar 'Y'	////// OLD
//////	10,	// 5. MK ASSIMETR.			// ...20, 10;
	20,	// 5. MK ASSIMETR. if NO CUT		// ...20, 10, 20;
	10,	// 6. MK ASSIMETR. if CUT !!!
	20,	// 7. MK: for each T_2 (see...)
	110,	// 8. MAX for ASSIMETR and T_2		// ...80, 100;
	100, 	// 9. EXTRAPOL. ASSIM. BAD PROPORTION	// 222, 124, 100;
//////	100 	// 10. for T_skok_L, T_skok_R		// 100;
	60	// 10. similar '7'			// 60;
	};

static BYTE tab_Y[] =				// dis_Y
	{
	40, /* 0. for long central flag		*/
	6,  /* 1. for near central flag		*/
	80, /* 2. similar 'T'			*/
	40, /* 3. too different ears		*/
	40, /* 4. MK: for each T_3 or T_2<2	*/
	80  /* 5. MK: for T_skok		*/
	};

/*......................................................................*/

static BYTE tab_circle_brace[]=
	{
	70, /* 0. up or down serif                 */
	10, /* 1. '(' similar    't'               */
	40, /* 2. '(' similar    '1'               */
	20, /* 3. no conc in top and bootom angles */
	60, /* 4. no flags : solid stick           */
	80, /* 5. for cutting brace                */
	28  /* 6. bad budgles                      */
	};

static BYTE tab_slash[] =
	{
	20, /* 0. for long central flag       */
	0,  /* 1. for near central flag       */
	40  /* 2. central beam                */
	};

static BYTE tab_sq_brace[]=
	{
	44,  /* 0. no flag : Â 	MK: 44 from 29.09.1993 (old 4)	*/
	8,   /* 1. bad paar flags : ÂÄ  for ], ÄÂ for [         */
	20,  /* 2. for any long center flag                     */
	8,   /* 3. for any near center flag                     */
	26,  /* 4. for up and down concaves : similar  '(',')'  */
	88,  /* 5. MK ADD 29.09.1993 for 't' => '['		*/
	222  /* 6. MK ADD 30.09.1993 for TERRIBLE case		*/
	};

static BYTE tab_vert[]=
	{
	60, /* 0. for any serif               */
	40, /* 1. for long central flag       */
	6,  /* 2. for near central flag       */
	10, /* 3. bad downer zone             */
	100,/* 4. similar to slash            */
	30, /* 5. similar to 1		      */
	60  /* 6. similar to ?		      */
	};

/*----------------------------------------------------------------------*/
