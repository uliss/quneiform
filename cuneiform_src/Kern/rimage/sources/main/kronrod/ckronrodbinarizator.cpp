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

//
//////////////////////////////////////////////////////////////////////
#include "ckronrodbinarizator.h"

#include <stdlib.h>
#include <string.h>
#include "compat_defs.h"

#define far

//#include <stdio.h>
//#include "globus.h"
//#include "CRIMemory.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
//
CKronrodBinarizator::CKronrodBinarizator()
{
	N_Bytes_per_ONE_MEM = 64000;
	Flag_OPEN = FALSE;
	hMem08[0] = NULL;
	hMem08[1] = NULL;
	hMem08[2] = NULL;
	hMem08[3] = NULL;
	hMem08[4] = NULL;
	hMem08[5] = NULL;
	hMem08[6] = NULL;
	hMem08[7] = NULL;
	hMBIT = NULL;
	hMREF = NULL;
	hPPMEM = NULL;
	Virginity = TRUE;
	//////////////////////////////////////////////////////////////////
	//    0       1       2       3      4        5       6       7
	//  - - -   - X -   X X X   X X X   X - X   X X X   - X X   X X X
	//  - X -   X X -   -   X   - - X   X - X   X - -   X - -   - - X
	//  X - X   - X -   X X -   - X X   X X X   X X X   X X X   - X -
	//  X - X   - X -   X - -   - - X   - - X   - - X   X - X   X - -
	//  - X -   X X X   X X X   X X X   - - X   X X X   X X X   X - -
	//
	//    8       9       10      11      12      13      14     15
	//  X X X   X X X   - X -   X X -   X X X   X X -   X X X   X X X
	//  X - X   X - X   X - X   X - X   X - -   X - X   X - -   X - -
	//  - X -   X X X   X X X   X X -   X - -   X - X   X X X   X X X
	//  X - X   - - X   X - X   X - X   X - -   X - X   X - -   X - -
	//  X X X   X X -   X - X   X X X   X X X   X X -   X X X   X - -
	Hex_35[0]  = 002552;
	Hex_35[1]  = 026227;
	Hex_35[2]  = 071647;
	Hex_35[3]  = 071317;
	Hex_35[4]  = 055711;
	Hex_35[5]  = 074717;
	Hex_35[6]  = 034757;
	Hex_35[7]  = 071244;
	Hex_35[8]  = 075257;
	Hex_35[9]  = 075716;
	Hex_35[10] = 025755;
	Hex_35[11] = 065657;
	Hex_35[12] = 074447;
	Hex_35[13] = 065556;
	Hex_35[14] = 074747;
	Hex_35[15] = 074744;

    MaxNI = MAX_NI;
    MaxNJ = MAX_NJ;
}

CKronrodBinarizator::~CKronrodBinarizator()
{
}

void CKronrodBinarizator::grey_open(uint32_t H, uint32_t N)
{
	uint32_t k;
	/*......................................................................*/
	grey_open_init_flags ();
	/*......................................................................*/
	NI = H;
	NI_1 = NI - 1;
	NJ = N;
	NJ_1 = NJ - 1;
	NJ_2 = NJ_1 - 1;
	NJx3 = NJ*3;

	if (Flag_OTL)
	PR_BEG
		"GREY:  NI=%d  NJ=%d"
		"\nNota Bene:"
		"\nthere is +OTLADKA in file GREY.INI",
		NI, NJ);
	PR_END

	if (NI>MaxNI)
	{
		pr_ERR_1((pchar)("GREY: TOO MANY LINES (%d)"),NI);
		return;
	}

	if (NJ>MaxNJ)
	{
		pr_ERR_1((pchar)("GREY: TOO int32_t LINE (%d)"), NJ);
		return;
	}

	grey_open_init_MEM ();

	Flag_OPEN = TRUE;

	IER = FALSE;
	Flag_ERR_Hist_L = Flag_ERR_Hist_R = FALSE;

	Korobs_init ();

	for (k=0; k<256; k++)
		Hist_tek [k] = 0;

	Ln_hist = 0;

	if (Flag_WrKor)
		Korob_Files_Open ();
	/*......................................................................*/
	//NEVER_MORE
	#ifdef G_DEBUG
	PR_BEG	"MK Message:\nGREY OPEN  %d*%d %04X %d (%d) %d",
		NI, NJ,
		N_Bytes_per_ONE_MEM, N_Lines_per_1_MEM,
		N_Bytes_FROM, 1235);
	PR_END
	#endif

}

void CKronrodBinarizator::grey_open_init_flags()
{
	// 02.11.1992
	uint32_t	k, kk/*, n*/;
	//uchar	b1, b2, bb, cc;
	char 	Full_Name_GREY_INI [33];
	char 	Name_GREY_INI[]="\\grey.ini";	// 10 chars, include 00
	//char 	buf[888];

	for (k=0; k<22; k++)
	    if ((Full_Name_GREY_INI[k]=Main_Dir_Name[k])==0)  break;

	for (kk=0; kk<10; kk++)
	    Full_Name_GREY_INI [k++] = Name_GREY_INI [kk];

			//DEFAULT:
	mk_Key = FALSE;
///////////////////////	Flag_Cont = 1;		// before 02.09.1992
///////////////////////	Flag_MMM  = TRUE;	// before 02.09.1992
	Flag_3x3  = TRUE;
	Flag_5work= FALSE;		// NEW from 02.11.1992
	Flag_GLUE = TRUE;
	Flag_d8P  = TRUE;
	Flag_Lapl = FALSE;		// before 02.09.1992 = TRUE;
	Flag_MAX  = FALSE;		// NEW from 13.09.1992
	Flag_0musor=FALSE;		// NEW from 27.10.1992
	Flag_musor= TRUE;		// NEW from 14.10.1992
	Flag_PLAN = TRUE;
	Flag_Hor  = Flag_Vert = FALSE;	// before 02.09.1992 = TRUE;
	Flag_Xverb= TRUE;		// from 27.10.1992: for TIMES ROMAN
	Flag_VERB = TRUE;		// from 04.09.1992: Porog with GORB
	Flag_WrKor= FALSE;
	Flag_T	  = Flag_Z    = FALSE;
	Flag_Draw = Flag_see  = FALSE;
	Flag_STAT = FALSE;		// Statistic
	Flag_FactPR = Flag_grHist = FALSE;
	Flag_OTL  = FALSE;
	return;

}

void CKronrodBinarizator::grey_open_init_MEM()
{
	uint32_t	k;
	//......................................................................
	N_Bytes_FROM = (NJ + 15) / 16 * 2;
	N_Lines_per_1_MEM  = N_Bytes_per_ONE_MEM / N_Bytes_FROM / 8;	// 940301 AL
	N_Lines_per_4_MEMs = N_Lines_per_1_MEM * 4;	//100
	N_Lines_per_8_MEMs = N_Lines_per_1_MEM * 8;	//200

	if (N_Lines_per_1_MEM==0)
		pr_ERR ((pchar)("GREY: NLpOM==0"));
	if (N_Bytes_FROM==0)
		pr_ERR ((pchar)("GREY: NBF==0"));

	N_Bytes_in_MBIT_Line = (NJ + 7) / 8;	// 2550 -> 319 => 63800
	N_Bytes_in_all_MBIT = N_Bytes_in_MBIT_Line * N_Lines_per_8_MEMs;
	//////PR_BEG	"N.B.MB.L=%d N.B.MBIT=%do",
	//////		N_Bytes_in_MBIT_Line, N_Bytes_in_all_MBIT/10);	PR_END

	memory_allocation ();

	//////	for (k=0; k<MAX_NI; k++)  ppMem [k] = NULL;	// OLD
	for (k=0; k<NI; k++)
		ppMem[k] = NULL;		// NEW

	memset (pMBIT, 0, N_Bytes_in_all_MBIT);	// ZERO to MBIT
	pMBIT_tek = pMBIT;
	N_tek_Line_in_MBIT = 0;

	N_of_TEK_MEM = 0;
	//////	Flag_Mem_AB = 0;
	pMem_TEK = pMem_kuda = pMem08 [0];	// first of 8 MEMs
	N_Lines_in_TEK_MEM = 0;
	/////////////	N_Lines_ACC = 0;
	//////	I_mem_A = I_mem_B = 0;

	Itek = 0;	// for  grey_from  (really it is "I_from")
	I_to = 0;	// for  grey_to
	//////	I_beg = I_end = 0;
}

uint32_t CKronrodBinarizator::grey_to(puchar pKogo)
{
	uint32_t	n_acc, n_from;	// really it is N_lines_will_be_ready;

	if (Flag_OPEN==FALSE)
	{
		pr_ERR_2 ((pchar)("GREY TO:\nNOT OPEN %d,%d"), I_to, Itek);
		return 0;
	}

	if (I_to==NI)
	{ pr_ERR_2 ((pchar)("GREY TO:\nTOO MANY LINES %d,%d"), I_to,Itek);
					return 0; }
	memcpy (pMem_kuda, pKogo, NJ);

	if ((Flag_OTL) && (I_to<=100) && (I_to%10==0))
	PR_BEG "GREY_TO  I=%d\nJ: 00, 40, 80, C0\nB: %02X, %02X, %02X, %02X",
	I_to, pKogo [0], pKogo [0x40], pKogo [0x80], pKogo [0xC0]);	PR_END

	grey_to_memory_monitor ();

	if (ppMem[I_to]==NULL)  { pr_ERR ((pchar)("GREY TO: NULL"));  return 0; }

	I_to++;
	// здддддддддддддддддддддддддддддддддддд©
	// Ё	READY or NOT  definition:	Ё
	// Ё	(for 8 * 25 = 200 Lines)	Ё
	// ЁItek I_to  r o/n f   rd old/new fromЁ
	// Ё  0, 200)  8 -/8 2  200   0/200  50	Ё
	// Ё 50, 250)  2 2/6 2   50  50/150  50	Ё
	// Ё100, 300)  2 2/6 2   50  50/150  50	Ё
	// Ё ..................................	Ё
	// Ё900, 975)  2 2/6 2   50  50/150  50	Ё
	// Ё925,1000)  2 2/6 6   50  50/150 150	Ё
	// юдддддддддддддддддддддддддддддддддддды
	n_acc = I_to - Itek;
	if (I_to==NI) { n_from = n_acc;  goto m1; }	// LAST PORTION !!!

	if (Itek==0)
	{					// FIRST PORTION ?
		if (n_acc < N_Lines_per_8_MEMs)  return 0; 	// NO READY
		n_from = 2*N_Lines_per_1_MEM;    goto m1;	// FIRST PORTION !!!
	}
				// MIDDLE PORTION ?
	if (n_acc < (uint16_t) (6 * N_Lines_per_1_MEM))  return 0;	// NO READY
	n_from = 2 * N_Lines_per_1_MEM;			// MIDDLE PORTION !!!

m1:
	grey_to_monitor_MBIT (n_from);
	return	n_from;
}

void CKronrodBinarizator::grey_to_memory_monitor()
{
	//see GREY.C: grey_read_monitor
	/*......................................................................*/
	ppMem [I_to] = pMem_kuda;
	pMem_kuda += NJ;
	N_Lines_in_TEK_MEM++;
	if ((I_to<NI_1) && (N_Lines_in_TEK_MEM<N_Lines_per_1_MEM))  return;
	/*......................................................................*/
	hist_add (pMem_TEK, N_Lines_in_TEK_MEM);	// LAST PORTION ???
	/*......................................................................*/
	N_of_TEK_MEM = (N_of_TEK_MEM + 1) & 7;	// NB: for 8 MEMS !!!!!!!

	pMem_kuda = pMem_TEK = pMem08 [N_of_TEK_MEM];
	/*......................................................................*/
	N_Lines_in_TEK_MEM = 0;
	/*......................................................................*/
}

void CKronrodBinarizator::grey_to_monitor_MBIT(uint32_t n_from)
{
	// 22.09.1992
	uint32_t	n1, n2;

	if (N_tek_Line_in_MBIT==0)  return;
	//......................................................................
	n1 = N_tek_Line_in_MBIT * N_Bytes_in_MBIT_Line;
	n2 = N_Bytes_in_all_MBIT - n1;

	memcpy (pMBIT, pMBIT + n1, n2);	// SHIFT to begin
	memset (pMBIT + n2, 0, n1);		// ZERO to end
	if (Flag_Draw)	memset (pMBIT + n2,  0x24,  16); // * * * * * *
	pMBIT_tek = pMBIT;
	N_tek_Line_in_MBIT = 0;

	if (Flag_grHist) grey_quant_KOROB_graph_Hist ();
}

void CKronrodBinarizator::grey_from(puchar pKuda)
{
	if (Flag_OPEN==FALSE)
		return;	////////////	really ERROR
	if (Itek>=I_to)
		return;	////////////	really ERROR
	//......................................................................//
	// Nota Bene: NO WORK on UPPER and DOWN Lines !!!
	if ((Itek!=0) && (Itek!=NI_1))
		Korobs ();
	//......................................................................//
	// NEW QUANT
	memcpy (pKuda, pMBIT_tek, N_Bytes_in_MBIT_Line); // WRITE RESULT
	pMBIT_tek += N_Bytes_in_MBIT_Line;
	N_tek_Line_in_MBIT++;
	//......................................................................//
	if (++Itek==NI)
	{
		memory_free ();
		Flag_OPEN = FALSE;
	}
}

uint32_t CKronrodBinarizator::grey_close()
{
	if (Flag_STAT)
		Korob_statistic_print ();

	if (Flag_STAT)
	PR_BEG
		"MIN-MAX: %02X-%02X \n"
		"P78=%02X P34=%02X P12=%02X P38=%02X \n "
		"D8=%02X",
		Porog_08, Porog_88,
		Porog_78, Porog_34, Porog_12, Porog_38,
		Diapazon_8);
	PR_END

	memory_free ();
	Flag_OPEN = FALSE;

	//////	if (Flag_Write)  fclose (mkFile_TEMP_IMG);

	if (Flag_WrKor)  Korob_Files_Close ();

	return 0;
}

void CKronrodBinarizator::grey_quant_KOROB()
{
	if (Flag_Draw)
		grey_quant_KOROB_draw_RAMKA ();
	//////	if (Flag_grHist) grey_quant_KOROB_graph_Hist ();
	if (Flag_FactPR)
		grey_quant_KOROB_PR_Factor ();

	// SIMPLE VARIANT:
	if (Flag_3x3==FALSE)
	{
		grey_quant_KOROB_1x1 ();
		return;
	}

	// OLD VARIANT:
	if (Flag_VERB==FALSE)
	{
		grey_quant_KOROB_3x3 ();
		return;
	}

	// NEW VARIANT:
	if (Korob_Factor<0xA0)
		grey_quant_KOROB_3x3_VERB ();	// NEW
	else
		if (Flag_5work)
			grey_quant_KOROB_1x5work (); // LL-L-C-R-RR
		else
			if ((Korob_porog<Porog_34) || (Flag_MAX==FALSE))
				grey_quant_KOROB_1x1 ();	// SIMPLE !
			else
				grey_quant_KOROB_1x3_MAX ();	// for new +MAX Variant
}

void CKronrodBinarizator::grey_quant_KOROB_1x1()
{
	uint32_t	i, i1, i2;					// SIMPLE VARIANT
	uint32_t	j, j1, nj, j7;
	int32_t	porog_tek;
	uchar	b, mask_left, mask;
	puchar	pkuda_beg, pkuda;
	puchar	pkogo;

	i1 = Korob_i1;
	i2 = Korob_i2;
	j1 = Korob_j1;
	nj = Korob_nj;
	porog_tek = Korob_porog;

	j7 = j1 & 7;		//  0,  1,  2,  3,  4,  5,  6,  7
	mask_left = 0x80;
	mask_left >>= j7;	// 80, 40, 20, 10, 08, 04, 02, 01
	pkuda_beg = pMBIT_tek + (j1/8);

	for (i=i1; i<=i2; i++)
	{
		pkuda = pkuda_beg;
		pkuda_beg += N_Bytes_in_MBIT_Line;
		b = *pkuda;			// READ FIRST uchar
		mask = mask_left;
		/*......................................................................*/
		pkogo = ppMem [i] + j1;

		for (j=0; j<nj; j++)
		{
			if ((int16_t)*pkogo++>=porog_tek)
				b |= mask;
			if (mask>>=1)
				continue;
			*pkuda++ = b;		// WRITE OLD uchar
			b = *pkuda;		// READ NEW uchar
			mask = 0x80;		// NEW MASK
		}	// FOR J

		*pkuda++ = b;		// WRITE LAST uchar
		/*......................................................................*/
	}		// FOR I
}

void CKronrodBinarizator::grey_quant_KOROB_1x3_MAX()
{
	uint32_t	i, i1, i2;					// SPECIAL VARIANT
	uint32_t	j, j1, nj, j7;				// Vers-33: begin;
	uint32_t	nj_8;					// Vers-34: NO for 8 El. L/R
	int32_t	porog_tek, porog_max, delta;
	int32_t	nL, nC, nR;
	uchar	b, mask_left, mask;
	puchar	pkuda_beg, pkuda;
	puchar	pkogo;

	i1 = Korob_i1;
	i2 = Korob_i2;
	j1 = Korob_j1;
	nj = Korob_nj;
	nj_8 = nj - 8;
	porog_tek = Korob_porog;
	porog_max = porog_tek + Diapazon_16;

	j7 = j1 & 7;		//  0,  1,  2,  3,  4,  5,  6,  7
	mask_left = 0x80;
	mask_left >>= j7;	// 80, 40, 20, 10, 08, 04, 02, 01
	pkuda_beg = pMBIT_tek + (j1/8);

	for (i=i1; i<=i2; i++)
	{
	    pkuda = pkuda_beg;
		pkuda_beg += N_Bytes_in_MBIT_Line;
	    b = *pkuda;			// READ FIRST uchar
	    mask = mask_left;
		//......................................................................
	    pkogo = ppMem [i] + j1;
	    nC = 0;		// next Left (zB sic)
	    nR = *pkogo++;	// next Centre
	    for (j=0; j<nj; j++)
		{
			nL = nC;
			nC = nR;
			nR = (int16_t) *pkogo++;
			if (nC<porog_tek)
				goto m2;	// WHITE

			if (j<8)
				goto m1;		// BLACK (for 8 LEFT)
			if (j>=nj_8)
				goto m1;		// BLACK (for 8 RIGHT)

			if (nC>=porog_max)
				goto m1;	// BLACK
			delta = (nL>nR) ? (nL - nC) : (nR - nC);
			if (delta<=0)
				goto  m1;	// BLACK
			if (delta > (int16_t) Diapazon_16)
				delta = Diapazon_16;
			if ((nC - delta) < porog_tek)
				goto m2;		// WHITE !!!
m1:
			b |= mask;			// BLACK
m2:
			if (mask>>=1)
				continue;
			*pkuda++ = b;		// WRITE OLD uchar
			b = *pkuda;		// READ NEW uchar
			mask = 0x80;		// NEW MASK
		}	// FOR J
	    *pkuda++ = b;		// WRITE LAST uchar
	//......................................................................
	}		// FOR I
}

void CKronrodBinarizator::grey_quant_KOROB_1x5work()
{
	// 03.11.1992
	uint32_t	i, i1, i2;					// SPECIAL VARIANT
	uint32_t	j, j1, j7;					// from 03.11.1992
	uint32_t	j_beg, j_end;			// NB: spec work for 2 left & 2 right
	int32_t	porog_tek;
	int32_t	nLL, nL, nC, nR, nRR, nCmod;
	uchar	b, mask_left, mask;
	puchar	pkuda_beg, pkuda;
	puchar	pkogo;

	if (Korob_nj<5)
		return;	// BBB
	i1 = Korob_i1;					// LL L C R RR
	i2 = Korob_i2;					//  0 1 2 3 4
	j1 = Korob_j1;
	j_beg = j1 + 2;
	j_end = j1 + Korob_nj - 3;
	porog_tek = Korob_porog;
	//////	porog_max = porog_tek + Diapazon_16;

	j7 = j1 & 7;		//  0,  1,  2,  3,  4,  5,  6,  7
	mask_left = 0x80;
	mask_left >>= j7;	// 80, 40, 20, 10, 08, 04, 02, 01
	pkuda_beg = pMBIT_tek + (j1/8);

	for (i=i1; i<=i2; i++)
	{
	    pkuda = pkuda_beg;
		pkuda_beg += N_Bytes_in_MBIT_Line;
	    b = *pkuda;			// READ FIRST uchar
	    mask = mask_left;
		//......................................................................
	    pkogo = ppMem [i] + j1;
	    nL = *pkogo++;	// next LL
	    nC = *pkogo++;	// next L
	    nRR= *pkogo++;	// next R
	    nR = *pkogo++;	// next C

			// FIRST ELEMENT:
	    if (nL>=porog_tek)
			b |= mask;	// BLACK
	    if ((mask>>=1)==0)
		{
			*pkuda++ = b;
			b = *pkuda;
			mask = 0x80;
		}

			// SECOND ELEMENT:
	    if (nC>=porog_tek)  b |= mask;	// BLACK
	    if ((mask>>=1)==0)
		{ *pkuda++ = b;  b = *pkuda;  mask = 0x80; }

	    for (j=j_beg; j<=j_end; j++)
		{
			nLL= nL;
			nL = nC;
			nC = nR;
			nR = nRR;
			nRR= (int16_t) *pkogo++;

			if (nC<(int16_t)Porog_38)  goto m2;	// WHITE

			nCmod = nC - Diapazon_8;
			if ((nLL<=nCmod) && (nRR>=nCmod))  goto m1;	// BLACK

			nCmod = nC + Diapazon_8;
			if ((nLL>=nCmod) && (nRR>=nCmod))  goto m2;	// WHITE

			if (nC<porog_tek) goto m2;	// WHITE

m1:
			b |= mask;			// BLACK
m2:
			if (mask>>=1)
				continue;
			*pkuda++ = b;		// WRITE OLD uchar
			b = *pkuda;		// READ NEW uchar
			mask = 0x80;		// NEW MASK
		}	// FOR J

			// LAST BUT ONE ELEMENT:
	    if (nR>=porog_tek)  b |= mask;	// BLACK
	    if ((mask>>=1)==0)
		{
			*pkuda++ = b;
			b = *pkuda;
			mask = 0x80;
		}

			// LAST ELEMENT:
	    if (nRR>=porog_tek)  b |= mask;	// BLACK
	    if ((mask>>=1)==0)
		{
			*pkuda++ = b;
			b = *pkuda;
			mask = 0x80;
		}

	    *pkuda++ = b;		// WRITE LAST uchar
	//......................................................................
	}		// FOR I
}

void CKronrodBinarizator::grey_quant_KOROB_3x3()
{
	uint32_t	i, i1, i2;
	uint32_t	j, j1, nj;
	int32_t	n, x, y, porog_tek, porog_min, porog_max;
	int32_t	nUL, nU, nUR;
	int32_t	nL,  nC, nR;
	int32_t	nDL, nD, nDR;
	int32_t	k_plan;
	uchar	b, mask_left, mask;
	uchar	here_Laplas, here_Plan, here_Hor, here_Vert;
	int32_t	L_kp_D8;
	puchar	pkuda_beg, pkuda;
	puchar	pU, pC, pD;		// Up, Centre, Down

	i1 = Korob_i1;
	i2 = Korob_i2;
	j1 = Korob_j1;
	nj = Korob_nj;
	porog_tek = Korob_porog;
	porog_min = porog_tek - Diapazon_8;
	if (Korob_Factor<0x90)
		porog_max = porog_tek + Diapazon_8;
	else
		porog_max = porog_tek + 2*Diapazon_8;

	x = Korob_x;
	y = Korob_y;

	if ((Flag_Lapl) && ((x+y)!=0))
		here_Laplas = TRUE;
	else
		here_Laplas = FALSE;

	here_Plan = FALSE;
	if (Flag_PLAN)
	{
	    k_plan = Korob_Factor - mkA0;
	    if (k_plan>0x30)  k_plan = 0x30;	////// PARAMETR 0x30 //////
	    if (k_plan>0)  here_Plan = TRUE;
	    L_kp_D8 = k_plan * Diapazon_8;
    }

	here_Hor = FALSE;
	if ((Flag_Hor) && (Korob_Factor>=par_Q))  here_Hor = TRUE;

	here_Vert = FALSE;
	if ((Flag_Vert) && (Korob_Factor>=par_Q))  here_Vert = TRUE;

	mask_left = 0x80;	mask_left >>= (j1 & 7);
	pkuda_beg = pMBIT_tek + (j1/8);

	for (i=i1; i<=i2; i++)
	{
	    pkuda = pkuda_beg;
		pkuda_beg += N_Bytes_in_MBIT_Line;
	    b = *pkuda;			// READ FIRST uchar
	    mask = mask_left;
		//......................................................................
		pU = ppMem [i - 1] + j1 - 1;	// UP
		pC = ppMem [i]     + j1 - 1;	// CENTRE
		pD = ppMem [i + 1] + j1 - 1;	// DOWN
		if (ppMem[i-1]==NULL)
		{
			PR_BEG
				"pU: i=%d %lX, %lX", i, ppMem[i-1], pU);
			PR_END
			return;
		}
		if (ppMem[i+1]==NULL)
		{
			PR_BEG
				"pD: i=%d %lX, %lX\n%d-%d %d,%d",
				i, ppMem[i+1], pD,
				i1,i2, j1,nj);
			PR_END
			return;
		}
		nU = *pU++;    nUR = *pU++;
		nC = *pC++;    nR  = *pC++;
		nD = *pD++;    nDR = *pD++;

	    for (j=0; j<nj; j++)
		{
			nUL = nU;	nU = nUR;	nUR = *pU++;
			nL  = nC;   n = nC = nR;	nR  = *pC++;
			nDL = nD;	nD = nDR;	nDR = *pD++;
			//......................................................................

			if (Flag_d8P)
			{			// for SPEED (29.08.92)
				if (n<porog_min)  goto  m_mask;
				if (n>=porog_max) { b |= mask;  goto  m_mask; }
		    }
			//......................................................................

			if (here_Laplas)
			{					// LAPLAS:
				int32_t	lapl;
				lapl =  -  nUL  -  nU  -  nUR
						-  nL   + 8*nC -  nR
						-  nDL  -  nD  -  nDR;

				if (lapl<0)
				{
					if (x)
						n+= lapl * x / 64;
				}	/* TO WHITE */
				else
				{
					if (y)
						n+= lapl * y / 64;
				}	/* TO BLACK */
			}	// END OF LAPLAS IF
			/*......................................................................*/
			if (here_Plan)
			{					// PLAN:
				int32_t	sred, plan, plan_2;

				sred = (nUL + nU + nUR + nL + nC + nR + nDL + nD + nDR) / 9;
				plan = abs (sred-nUL) + abs (sred- nU) + abs (sred- nUR) +
					   abs (sred-nL)  + abs (sred-nC)  + abs (sred- nR)  +
					   abs (sred-nDL) + abs (sred- nD) + abs (sred- nDR);

				if (plan>181)  plan = 181;	// 181*181 = 32 761 < 32K

				//////	plan_2 = (plan/4)*(plan/4)/16;	// old before 27.08.1992
				plan_2 = plan * plan;		// new from 27.08.1992

				/******************************************************************
				//////		// for stdj1.tif: DIAPAZON_8 = 0C; (B0-D0) => (00-10)
				//////		// HEX:       20 / 18 * 0C = 10;  (/ 10 = 1)
				//////		// DEC:       32 / 24 * 12 = 16;  (/ 16 = 1)
				//////	n -= plan_2 * k_plan / 24 * Diapazon_8 / 16;  // OVERFLOW ?
				*****************************************************************/

				/* for stdj1.tif: DIAPAZON_8 = 0C; (A0-D0) => (0-6)	*/
				/* HEX: / 4 * 30 / 18 * 0C = 6;				*/
				/* DEC: / 4 * 48 / 24 * 12 = 6;				*/
				//////	n -= plan_2 / 4 * k_plan / 24 * Diapazon_8 / 16;  /* OVER ?	*/

				n -= L_kp_D8 * plan_2 / (4*4*16*4*24*16 * 3);	//////	NEW  //////

			}	// END OF PLAN IF
			/*......................................................................*/
			if (here_Hor && ((Itek-i1)*4>i2))
			{		// Hor SEDLO:
				int32_t	sedlo;				/* for BLACK THICK Text only	*/
					/* for UPPER 1/4 of KOROB only	*/

				if (( nU>nC) && ( nD>nC) &&		/* Centre Vert	*/
					(nUL>nL) && (nDL>nL) &&		/* Left   Vert	*/
					(nUR>nR) && (nDR>nR))  		/* Right  Vert	*/
					{
						sedlo = (nUL + nU + nUR) - (nL + nC + nR)*2 + (nDL + nD + nDR);
						n -= sedlo*1;	/* OVERFLOW ??? */
					}

			}	// END OF HOR IF
			/*......................................................................*/
			if (here_Vert)
			{					// Vert SEDLO:
				int32_t	sedlo;				/* for BLACK THICK Text only	*/

				if (( nL>nC) && ( nR>nC) && 		/* Centre Hor	*/
					(nUL>nU) && (nUR>nU) &&		/* Up     Hor	*/
					(nDL>nD) && (nDR>nD))		/* Down   Hor	*/
			    {
					sedlo = (nUL + nL + nDL) - (nU + nC + nD)*2 + (nUR + nR + nDR);
					n -= sedlo*1;	/* OVERFLOW ??? */
				}

			}	// END OF VERT IF
			/*......................................................................*/
			if (n>=porog_tek)
				b |= mask;
m_mask:
			if (mask>>=1)
				continue;

			*pkuda++ = b;		// WRITE OLD uchar
			b = *pkuda;		// READ NEW uchar
			mask = 0x80;		// NEW MASK
		}	// FOR J
		*pkuda++ = b;		// WRITE LAST uchar
		/*......................................................................*/
	}		// FOR I
}

void CKronrodBinarizator::grey_quant_KOROB_3x3_VERB()
{
	uint32_t	i, i1, i2;				// LAPLAS to BLACK ONLY
	uint32_t	j, j1, nj;
	int32_t	n, y, porog_tek, porog_min;
	int32_t	nUL, nU, nUR;
	int32_t	nL,  nC, nR;
	int32_t	nDL, nD, nDR;
	int32_t	lapl;
	uchar	b, mask_left, mask;
	puchar	pkuda_beg, pkuda;
	puchar	pU, pC, pD;		// Up, Centre, Down

	i1 = Korob_i1;
	i2 = Korob_i2;
	j1 = Korob_j1;
	nj = Korob_nj;
	porog_tek = Korob_porog;
	porog_min = porog_tek - Diapazon_8;

	y = Korob_y;

	mask_left = 0x80;
	mask_left >>= (j1 & 7);
	pkuda_beg = pMBIT_tek + (j1/8);

	for (i=i1; i<=i2; i++)
	{
	    pkuda = pkuda_beg;
		pkuda_beg += N_Bytes_in_MBIT_Line;
	    b = *pkuda;			// READ FIRST uchar
	    mask = mask_left;
		//......................................................................
		pU = ppMem [i - 1] + j1 - 1;	// UP
		pC = ppMem [i]     + j1 - 1;	// CENTRE
		pD = ppMem [i + 1] + j1 - 1;	// DOWN

		nU = *pU++;
		nUR = *pU++;
		nC = *pC++;
		nR  = *pC++;
		nD = *pD++;
		nDR = *pD++;

		for (j=0; j<nj; j++)
		{
			nUL = nU;
			nU = nUR;
			nUR = *pU++;
			nL  = nC;
			n = nC = nR;
			nR  = *pC++;
			nDL = nD;
			nD = nDR;
			nDR = *pD++;
			//......................................................................
			if (Flag_d8P)
			{			// for SPEED (29.08.92)
				if (n<porog_min)
					goto  m_mask;
				if (n>=porog_tek)
				{
					b |= mask;
					goto  m_mask;
				}
			}
			//......................................................................
			lapl =  -  nUL  -  nU  -  nUR			// LAPLAS Y ONLY
					-  nL   + 8*nC -  nR
					-  nDL  -  nD  -  nDR;

			if (lapl>0)
				n+= lapl * y / 64;		/* TO BLACK ONLY !!! */
			//......................................................................
			if (n>=porog_tek)
				b |= mask;
m_mask:
			if (mask>>=1)
				continue;

			*pkuda++ = b;		// WRITE OLD uchar
			b = *pkuda;		// READ NEW uchar
			mask = 0x80;		// NEW MASK
		}	// FOR J
		*pkuda++ = b;		// WRITE LAST uchar
		//.....................................................................
	}		// FOR I
}

void CKronrodBinarizator::grey_quant_KOROB_draw_RAMKA()
{
	uint32_t	i, i1, i2;
	uint32_t	j, j1, nj, j2;
	uchar	mask, mask_L, mask_R;
	puchar	pU, pD, pL, pR;		// Up, Down, Left, Right

	i1 = Korob_i1;
	i2 = Korob_i2;
	j1 = Korob_j1;
	nj = Korob_nj;
	j2 = j1 + nj - 1;
	//......................................................................
	pU = pMBIT_tek + (j1/8);
	pD = pU + (i2 - i1) * N_Bytes_in_MBIT_Line;
	mask = 0x80;	mask >>= (j1 & 7);

	for (j=j1; j<=j2; j++)
	{
	    if ((j&1)==0)
		{
			*pU |= mask;
			*pD |= mask;
		}

		if (mask>>=1)
			continue;
	    pU++;
	    pD++;
	    mask = 0x80;
	}
	//......................................................................
	pL = pMBIT_tek + (j1/8);
	mask_L = 0x80;
	mask_L >>= (j1 & 7);
	pR = pMBIT_tek + (j2/8);
	mask_R = 0x80;
	mask_R >>= (j2 & 7);

	for (i=i1; i<=i2; i++)
	{
	    if ((i&1)==0)
		{
			*pL |= mask_L;
			*pR |= mask_R;
		}
	    pL += N_Bytes_in_MBIT_Line;
	    pR += N_Bytes_in_MBIT_Line;
	}
	//......................................................................
}

void CKronrodBinarizator::grey_quant_KOROB_graph_Hist()
{
	uint32_t	i = Itek + N_Lines_per_1_MEM - 8;

	grey_PR_BYTE (i, 16, (uchar)((Itek >> 8) & 0xFF));	// HIGH
	grey_PR_BYTE (i, 24, (uchar)(Itek & 0xFF));	// LOW
	grey_PR_BYTE (i, 36, Porog_08);	// min
	grey_PR_BYTE (i, 48, Porog_88);	// MAX
}

void CKronrodBinarizator::grey_quant_KOROB_PR_Factor()
{
	uint32_t	ibeg, jbeg;

	ibeg = Korob_i2 + 2;
	jbeg = Korob_j1;	// for PRINT DOWN;

	if ((Korob_nj<12) && ((Korob_i2-Korob_i1)<12))	// for PRINT RIGHT:
	{
		ibeg = Korob_i1;
		jbeg = Korob_j1 + Korob_nj + 1;
	}
	if (((ibeg+5)>=NI) || ((jbeg+7)>=NJ))
		return;	// OUT !!!!!!!!!!
	if ((N_tek_Line_in_MBIT + ibeg - Itek + 5) >= N_Lines_per_8_MEMs)
		return;
	//......................................................................
	grey_PR_BYTE (ibeg, jbeg, Korob_Factor);
}

void CKronrodBinarizator::grey_PR_BYTE(uint32_t ibeg, uint32_t jbeg, uint32_t value)
{
	uint32_t	i, j, k;
	uint32_t	cifra, scale;
	uchar	mask;
	puchar	p;
	//......................................................................
	cifra = value / 16;
	for (k=0; k<2; k++)
	{
	    scale = Hex_35 [cifra];

		for (i=0; i<5; i++)
		{
			p = pMBIT_tek +
				(ibeg + i - Itek)*N_Bytes_in_MBIT_Line +
				(jbeg/8);

			mask = 0x80;
			mask >>= (jbeg & 7);

			for (j=0; j<3; j++)
			{
				if ((int16_t) (scale<<=1) < 0)
					*p |= mask;
				if ((mask>>=1) == 0)
				{
					mask = 0x80;
					p++;
				}
		    } // FOR J
		} // FOR I
	    cifra = value & 0x0F;    jbeg += 4;
	} // FOR K
}

void CKronrodBinarizator::hist_add(puchar p, uint32_t N_Lines)
{
	uint32_t	k, n;
	uint32_t	Hist_WORD [256];

	n = N_Lines * NJ;

	for (k=0; k<256; k++)  Hist_WORD [k] = 0;
			// calculate TIME for bold36: 12 sec + x*40 times:
			// measure time; Example: (60-12)/39 = 1.230;
	for (k=0; k<n; k++)  Hist_WORD[*p++]++;		// 60: 1.230 (TC)
//////	mk_hist_WORD (Hist_WORD, p, n);			// 50: 0.974 (MKASM)

	for (k=0; k<256; k++)  Hist_tek [k] += Hist_WORD [k];
/*......................................................................*/
	Ln_hist += n;

	hist_glad_8_121 ();

	hist_MIN_MAX ();
}

void CKronrodBinarizator::hist_glad_8_121()
{
	int32_t	k, n;
	int32_t	LnL, LnC, LnR;

	for (k=0; k<256; k++)
		Hist_glad [k] = Hist_tek [k];

	for (n=0; n<8; n++)
	{	/* 8 times Swertka with Binom 121 */
	    LnC = Hist_glad [0];
	    LnR = Hist_glad [1];
	    for (k=1; k<=254; k++)
		{
			LnL = LnC;		/* 0, 1, 2, ..., 252, 253 */
			LnC = LnR;		/* 1, 2, 3, ..., 253, 254 */
			LnR = Hist_glad [k+1];	/* 2, 3, 4, ..., 254, 255 */
			Hist_glad [k] = (LnL + (LnC + LnC) + LnR) / 4;
		}
    }
}

void CKronrodBinarizator::hist_MIN_MAX()
{
	/*зддддддддддддддддддддддддддддддддддддддддддддддддддддд©*/
	/*Ё	It works with Hist_glad;			Ё*/
	/*Ё maxR - right max: move from FF to left side, find	Ё*/
	/*Ё	maximun and then test it in next 32 positions;	Ё*/
	/*Ё maxL - next max: move from (KmaxR - 32) to left	Ё*/
	/*Ё	a) until more then  (LmaxR / 2);  before 22.09	Ё*/
	/*Ё	.........more then  (LmaxR * 3/4);  from 22.09	Ё*/
	/*Ё	b) then find MONOTON.MINIMUM or INCREASE * 2;	Ё*/
	/*Ё	c) then find more then  MONO_MIN * 2;		Ё*/
	/*Ё	d) and then find max using the last algoritm,	Ё*/
	/*Ё	   using const 16 (instead old 32) from 27.10.92Ё*/
	/*Ё.....................................................Ё*/
	/*Ё	THE FIRST PROJECT (not used, not tested) -	Ё*/
	/*Ё                       see MK-GREY.ABC.		Ё*/
	/*юддддддддддддддддддддддддддддддддддддддддддддддддддддды*/
	int32_t	k;
	int32_t	KmaxL, KmaxR;
	int32_t	Ln, LmaxL, LmaxR, Lporog_34;	////// Lporog_2, Lporog_4;
	int32_t	L_incr_2, Lporog_MM, Lporog_MMx2;
	/*......................................................................*/
	/*зддддддддддддддддддддддддддддддддддддддддддддддддддддд©*/
	/*Ё maxR - right max: move from FF to left side, find	Ё*/
	/*Ё	maximun and then test it in next 32 positions;	Ё*/
	/*юддддддддддддддддддддддддддддддддддддддддддддддддддддды*/
	LmaxR = 0;
	for (k=255; k>=0; k--)
	{
	    Ln = Hist_glad [k];
	    if (Ln>=LmaxR)
		{
			KmaxR = k;
			LmaxR = Ln;
		}
	    if (k < (KmaxR - 32) )
			goto  m1;	/* Right MAX is found	*/
	}

	KEY
		if (Flag_ERR_Hist_R==FALSE)
	    PR_BEG
			"GREY hist_MIN_MAX:  unknown Right Max (%02X: %ld)",
			KmaxR, LmaxR);
		PR_END

	Flag_ERR_Hist_R = TRUE;	// for NO MORE THIS PRINT
	goto  m_err_OK;	// for 00-FF
	//......................................................................*/
m1:
	Lporog_34 = LmaxR * 3 / 4;	//  from 22.09.1992
	//////	Lporog_2 = LmaxR / 2;		// before 22.09.1992
	/******	Lporog_4 = LmaxR / 4;******/	// very old variant
	/*......................................................................*/
	/*зддддддддддддддддддддддддддддддддддддддддддддддддддддд©*/
	/*Ё maxL - next max: move from (KmaxR - 16) to left	Ё*/
	/*Ё	a) until more then  (LmaxR / 2);  before 22.09	Ё*/
	/*Ё	.........more then  (LmaxR * 3/4);  from 22.09	Ё*/
	/*Ё	b) then find MONOTON.MINIMUM;			Ё*/
	/*Ё	c) then find more then  MONO_MIN * 2;		Ё*/
	/*Ё	d) and then find max using the last algoritm;	Ё*/
	/*юддддддддддддддддддддддддддддддддддддддддддддддддддддды*/

	/*..............................................................*/
	/* a) until more then  (LmaxR * 3/4);	*/
	for (; k>=0; k--)
		if (Hist_glad [k] < Lporog_34)
			break;

	if (k<=0)
		goto  m_err;
	/*..............................................................*/
	/* b) then find MONOTON.MINIMUM;	*/
	/* b) then find MONOTON.MINIMUM or INCREASE * 2;*/
	Ln = Hist_glad [k];
	L_incr_2 = Ln*2;
	for (; k>=0; k--)
	{
	    Lporog_MM = Ln;
	    Ln = Hist_glad [k];
	    if (Ln>Lporog_MM)
			break;
	    if (Ln>L_incr_2)
			break;
	}
	if (k<=0)
		goto  m_err;
	Lporog_MMx2 = Lporog_MM + Lporog_MM;
	/*..............................................................*/
	/* c) then find more then MONO_MIN * 2;	*/
	for (; k>=0; k--)
		if (Hist_glad [k] > Lporog_MMx2)
			break;
	if (k<=0)
		goto  m_err;

	/*..............................................................*/
	/* d) find max using the last algoritm;	*/
	/*    using const 16 (instead old 32)	*/
	LmaxL = 0;	KmaxL = k;	/* BBB */
	for (; k>=0; k--)
	{
	    Ln = Hist_glad [k];
	    if (Ln>=LmaxL)
		{
			KmaxL = k;
			LmaxL = Ln;
		}
		//////	    if (k < (KmaxL - 32) )  goto  m_ex;	/* NEXT MAX is found	*/
	    if (k < (KmaxL - 16) )
			goto  m_ex;	/* NEXT MAX is found	*/
	}
	goto  m_ex;	// SPEWCIAL CASE  (KmaxL < 32) !!!
	/*......................................................................*/
m_err:
	if (Itek==0)
		goto  m_err_OK;	//????????????????????????????????
	if (Flag_ERR_Hist_L)
		goto  m_err_OK;

	KEY
		pr_ERR_1 ((pchar)("GREY: unknown Left Max (nkR=%02X)"), KmaxR);

	Flag_ERR_Hist_L = TRUE;

m_err_OK:
	KmaxL = 0;
	KmaxR = 0xFF;	/* BBB	*/
	/*......................................................................*/
m_ex:
	Porog_12 = (uchar) ((KmaxL + KmaxR) / 2);		  // 4/8
	Porog_34 = (uchar) ((KmaxL + KmaxR + KmaxR + KmaxR) / 4);  // 6/8
	Porog_08 = (uchar) KmaxL;
	Porog_88 = (uchar) KmaxR;
	Diapazon_8 = (uchar) ((KmaxR - KmaxL) / 8);
	Porog_38 = Porog_12 - Diapazon_8;
	Porog_58 = Porog_12 + Diapazon_8;
	Porog_78 = Porog_34 + Diapazon_8;
	Diapazon_16 = (uchar) (Diapazon_8 >> 1);	// for +MAX 1x3 work
	/*......................................................................*/
}

void CKronrodBinarizator::Korobs_init()
{
	// 11.09.1992
	uint32_t	k;
	int32_t * 	KR = (int32_t *) pMREF;	// KOROBS REFERENCES

	for (k=0; k<NJx3; k++)
		KR[k] = 9999;	// OTL: "ERROR" INDICATION
	N_Korob = 0;
	LS_Korobs = LSH_Korobs = LSL_Korobs = 0L;
	LNgrey16 = LNgreyD8 = LNgrey08 = 0L;
	// J = 0		// J = 1		// J = (NJ - 1)
	KR[0] = 9999;	KR[3] = 0;		KR [NJx3 - 3] = 1;		// LAST
	KR[1] = 1;		KR[4] = NJ_1;	KR [NJx3 - 2] = 9999;	// NEXT
	KR[2] = 9999;	KR[5] = 0;		KR [NJx3 - 1] = 9999;	// DOWN
}

void CKronrodBinarizator::Korobs()
{
	uint32_t	i2, j1, nj, j2m; /*i1,*/ 	// здддддддддддддддддддддддддддддддддддд©
	uint32_t	j;			// Ё Current line contain a number of [)Ё
	uint32_t	a, aaa;	// LAST for BASE// Ё intervals. Used NON-FULL reference	Ё
	uint32_t	b, bbb;	// BASE		// Ё structure, 3 int16_t's per each busy	Ё
	uint32_t	c, ccc;	// LEFT		// Ё elemelt: [+0],[+1]	- references to	Ё
	uint32_t	d, ddd;	// RIGHT	// Ё LAST & NEXT; [+2] - 'DOWN': if 0,	Ё
	uint32_t	e, eee;	// NEXT		// Ё this interval is free, else it is 	Ё
	uint32_t	f, fff;	// NEXT for NEXT// Ё the number of last busy line.	Ё
	uint32_t	b_old;			// Ё    New KOROB included to it. The	Ё
	int32_t	ai2, ei2;		// Ё simplest case - if KOROB is inter 	Ё
	puchar	pC;			// Ё free interval. But it can be lefts	Ё

	#define KRType        int16_t
	int16_t *	KR = (int16_t*)pMREF;// Ё from left boundary or rights from 	Ё
	// (KOROBS REFERENCES)	// Ё right boundary of free interval...	Ё
	// юдддддддддддддддддддддддддддддддддддды
	/*......................................................................*/
	Korobs_CLEAR_and_GLUE ();
	/*......................................................................*/
	pC = ppMem [Itek];
	/*......................................................................*/
	// KR:        (a)  b   c   d   e  (f)
	// [0] LAST:  <-   a . . . . . b   e
	// [1] NEXT:   b   e . . . . . f   ->
	// [2] DOWN:  ***  0 (free). .*** ***
	// . . . . . . . . . . . . . . . . . .
	b = 1;

m1:
	if (b==NJ_1)
		return;
	bbb = b + b + b;
	//////	if ((Flag_OTL) && (b>=640))  return;	// ****** OTLADKA *******
	if (KR [bbb + 2])
	{
		b = KR [bbb + 1];
		goto  m1;
	}	// TO NEXT
	e = KR [bbb + 1];
	for (j=b; j<e; j++)
	{		// for free INTERVAL:
	//////	    if (pC[j]>=Porog_38)  goto m2;
	    if (pC[j]>=Porog_38)
		{
			Korob_add (j);
			if (Flag_Korob_OK)
				goto m2;
		}
	}
	b = e;		// to NEXT INTERVAL
	goto  m1;
	/*......................................................................*/
m2:							// KOROB IS FOUND
	//////	Korob_add (j);	if (IER)  return;
	if (IER)
		return;	///////////// ?????????? //////////////
	/*i1 = Korob_i1;*/	i2 = Korob_i2;
	j1 = Korob_j1;
	nj = Korob_nj;
	j2m = j1 + nj;
	c = j1;		ccc = c + c + c;
	d = j2m;	ddd = d + d + d;
	eee = e + e + e;
	N_Korob++;
	if (Flag_see)
		Korob_see ();	// OTLADKA
	/*......................................................................*/
						// REFERENCE for LEFT:
	if (c<=b)
		goto m_Left_spec;	// (for SPECIAL CASE)
					// NORMAL CASE: GLUE  b & c:
	KR [bbb + 1] = (KRType)c;	// BASE: NEXT
	KR [ccc + 0] = (KRType)b;	// LEFT: LAST (NB: NEXT - see below)
	KR [ccc + 2] = (KRType)i2;	// LEFT: DOWN (including)
	goto  m_right;
/*......................................................................*/
m_Left_spec:			// REFERENCE for LEFT: SPECIAL CASE

	//	╨			иммммммммммммммм╩
	//	╨z			╨a		╨b
	//ддддддвдддддддддддддддддддддддвдддддддддддддддвддддддддддддддддддддддддд
	//	╨	Ёc.5	     ???╨c.3	Ёc.1	╨c.0'	Ёd
	//	╨	Ёддддддддддддддд╨ддддддддддддддд╨дддддддЁ
	//	╨	Ё		╨	Ё	╨	Ё
	//	╨	Ё		хммммммммммммммм╪	Ё
	//		Ё		Ё	Ё	Ё	Ё
	//		Ёc.6	     ???Ёc.4	Ёc.2	Ёc.0"	Ё
	//		юддддддддддддддддддддддддддддддддддддддды

	b_old = b;		// NOTA BENE: used for next value of  c

mLs1:
	bbb = b + b + b;	// NOTA BENE:  b  can be modified !!!
/*......................................................................*/
	if ((int16_t) i2 > KR [bbb + 2])
		KR [bbb + 2] = (KRType)i2;  // for all cases !

	if (c==b)
		goto  mLs_ex;	// case c.0

	a = KR [bbb + 0];	aaa = a + a + a;
	ai2 = KR [aaa + 2];	// DOWN of a-PoluInterval [a;b)
	if (c>a)
	{			// case c.1 or c.2:
	    if ((int16_t)i2<=ai2)
			goto mLs_ex;	// case c.1;
				// case c.2:
	    KR [aaa + 1] = (KRType)c;	//NEXT	//GLUE  a & c
	    KR [ccc + 0] = (KRType)a;	//LAST
	    KR [ccc + 1] = (KRType)b;	//NEXT	//GLUE  c & b
	    KR [bbb + 0] = (KRType)c;	//LAST
	    KR [ccc + 2] = (KRType)i2;	//DOWN
	    goto  mLs_ex;
	}
	// case  c.3,5  or  c.4,6:
	if ((int16_t)i2<=ai2)
	{
		b = a;
		goto  mLs1;
	}	// case  c.3,5;
	// case  c.4,6:
	KR [aaa + 2] = (KRType)i2;	//DOWN for POLUINTERVAL [a;b) !!!
	b = a;			// for repeat
	goto  mLs1;		// REPEAT !!!!!!
	/*......................................................................*/
mLs_ex:
	c = b_old;	// NOTA BENE:  'c' is used below...
	/*......................................................................*/
m_right:					// REFERENCE for RIGHT:
	ccc = c + c + c;
	if (d>=e)
		goto m_right_spec;	// (for SPECIAL CASE)
					// NORMAL CASE: GLUE c/d, d/e:
	KR [ccc + 1] = (KRType)d;	// LEFT:  NEXT	// GLUE  c & d
	KR [ddd + 0] = (KRType)c;	// RIGHT: LAST
	KR [ddd + 1] = (KRType)e;	// RIGHT: NEXT	// GLUE  d & e
	KR [ddd + 2] = (KRType)0;	// RIGHT: DOWN (free)
	KR [eee + 0] = (KRType)d;	// NEXT:  LAST
	b = d;		// to NEXT INTERVAL
	goto  m1;
/*......................................................................*/
m_right_spec:			// REFERENCE for RIGHT: SPECIAL CASE:

	//			иммммммммммммммммммммммммммммммм╩
	//			╨e				╨f
	//ддддддддддддддддддддддвдддддддддддддддддддддддддддддддвддддддддд
	//	Ёc		╨		Ёd.1		╨d.3	Ёd.5
	//	Ёддддддддддддддд╨ддддддддддддддддддддддддддддддд╨дддддддЁ
	//	Ё		╨		Ё		╨	Ё
	//	Ё		хммммммммммммммммммммммммммммммм╪	Ё
	//	Ё		Ё		Ё		Ё	Ё
	//	Ё		Ёd.0		Ёd.2		Ёd.4	Ёd.6
	//	юддддддддддддддддддддддддддддддддддддддддддддддддддддддды

			// GLUE  c & e	// sic! (for some cases...)
	KR [ccc + 1] = (KRType)e;	// NEXT
	KR [eee + 0] = (KRType)c;	// LAST

mrs1:
	if (d==e)
	{
		b = e;
		goto  m1;
	}	// case d.0: IGNORE d
	//////	ddd = d + d + d;	// sic!
	eee = e + e + e;	// sic!
	ei2 = KR [eee + 2];	// DOWN of e-Interval
	f = KR [eee + 1];
	fff = f + f + f;	// NJ_1 - ??????
	if (d<f)
	{
		// case d.1 or d.2:
	    if ((int32_t)i2<=ei2)
		{
			b = f;
			goto m1;
		}	// case d.1;
		// case d.2:
	    KR [ccc + 1] = (KRType)d;	//NEXT	//GLUE  c & d
	    KR [ddd + 0] = (KRType)c;	//LAST
	    KR [ddd + 1] = (KRType)f;	//NEXT	//GLUE  d & f
	    KR [ddd + 2] = (KRType)ei2;	//DOWN from 'e' !!!
	    KR [fff + 0] = (KRType)d;	//LAST
	    b = f;
	    goto m1;
	}
	// case  d.3,5  or  d.4,6:
	if ((int16_t)i2<=ei2)
	{
		e = f;
		goto  mrs1;
	}	// case  d.3,5;
	// case  d.4,6:
	KR [ccc + 1] = (KRType)f;	//NEXT	//GLUE  c & f
	KR [fff + 0] = (KRType)c;	//LAST
	e = f;			// for repeat
	goto  mrs1;		// REPEAT !!!!!!
	/*......................................................................*/
#undef KRType
}

void CKronrodBinarizator::Korobs_CLEAR_and_GLUE()
{
	uint32_t	b, bbb;	// BASE
	uint32_t	c, ccc;	// NEXT for BASE
	uint32_t	d;	// NEXT for NEXT
	int32_t	bi2;
	int32_t *  KR = (int32_t *) pMREF;	// KOROBS REFERENCES

	b = 1;
m1:
	bbb = b + b + b;
	bi2 = KR [bbb + 2];

	if ((bi2!=0) && (bi2 < (int16_t) Itek))
		KR[bbb+2] = bi2 = 0;  // CLEAR

	c = KR [bbb + 1];

	if (c==NJ_1)
		return;

	ccc = c + c + c;

	if (bi2 != KR [ccc + 2])
	{
		b = c;
		goto  m1;
	}
					//(both DOWN were NOT EQUAL)
	d = KR [ccc + 1];		// both DOWN are EQUAL: GLUE:
	KR [bbb + 1] = d;	// NEXT for THIS
	KR [d+d+d+0] = b;	// LAST for NEXT
	b = d;
	if (b!=NJ_1)
		goto  m1;	// BBB;
}

void CKronrodBinarizator::Korob_add(uint32_t j)
{

	Korob_calc_BOX (j);
	if (Flag_Korob_OK==FALSE)
		return;
	/*......................................................................*/
	/******	grey_Korob_add_hist ();******/		/* OLD: < 24.07.1992	*/
	Korob_calc_Factor ();			/* NEW: = 24.07.1992	*/
	/*......................................................................*/
	if ((Flag_0musor) &&		// PROBA 27.10.1992
		(Korob_Factor==0) &&	// may be musor...
		(Korob_nj>=8)  &&		// no small (see points of i)
		((Korob_i2 - Korob_i1)>=7))
	{	// no small (from 8*8)
		Flag_Korob_OK = FALSE;
		return;
	}
	/*......................................................................*/
	Korob_calc_porog_x_y ();
	//////	Korob_calc_TdP ();	/* Delta POROG Buffer	*/
	if (IER)
		return;	// ????????????????????????????

	//////	if (Flag_MMM)	grey_quant_KOROB ();
	grey_quant_KOROB ();

	//////	if (Flag_STAT)  Korob_statistic ();

	//////	Korob_add_show_Factor (N_tek);

	if (Flag_WrKor)
		Korob_Files_Write ();
}

void CKronrodBinarizator::Korob_calc_BOX(uint32_t j)
{
	uint32_t	ii, jj;					/* NB: U+ -> +2  R & L	*/
	uint32_t	i1,i2, ni, i_down;
	uint32_t	j1,j2, nj;
	uint32_t	n_0_2, n_j_empty;	/* for RIGHT +0 (U-) or +2 (U+) GLUE  */
	uchar	 FlagD, FlagL/*FlagR,*//*, Flag_DOWN_100*/;
	puchar	p;
	int32_t	count_5555 = 5555;	// BBB for OTLADKA

	i1 = i2 = Itek;
	ni = 1;
	j1 = j2 = j;
	nj = 1;
	//////	i_down = I_to;			// for 1x1
	//////	if (Flag_3x3)  i_down--;	// for 3x3
	i_down = I_to - 1;		// for 3x3
	if ((i_down - Itek) > 128)
		i_down = Itek + 128;
	n_0_2 = Flag_GLUE * 2;		// 0 or 2
	/*	Flag_DOWN_100 = FALSE;*/ // commented by D.L.3.4.94 as never used
	/*......................................................................*/
mR:
	n_j_empty = /*FlagR = */ 0;		// move RIGHT, GLUE (0) or (+2)
	jj = (j2==j1)  ?  (j2 + 1) : (j2 + 1 - n_0_2);	// FIRST of NOT

	////if (jj>=NJ_2)  { j2 = NJ_1;  goto mR2; }  // on RIGHT SIDE;  zB sic !!!
	////if (jj==NJ_1)  { j2 = NJ_1;  goto mR2; }  // on RIGHT SIDE;  zB sic !!!
	if (jj>=NJ_2)
	{
		j2 = NJ_2;
		goto mR2;
	}  // +MK 27.09.1993

	for ( ; jj<NJ_1; jj++)
	{
		for (ii=i1; ii<=i2; ii++)
		{
			if (ppMem [ii] [jj] >= Porog_38)
				goto mR1;
		}
		//////  if (jj>=NJ_2)  { j2 = NJ_1;  goto mR2; }  // on RIGHT SIDE;  zB sic !!!
		//////  if (jj==NJ_1)  { j2 = NJ_1;  goto mR2; }  // on RIGHT SIDE;  zB sic !!!
		if (jj>=NJ_2)
		{
			j2 = NJ_2;
			goto mR2;
		}  // +MK 27.09.1993
		//////	    if (++n_j_empty>2)  break;	// no RIGHT POINTS in 2 Columns
		if (++n_j_empty>n_0_2)
			break;  // no RIGHT POINTS in 2 Columns
		continue;

mR1:
		//   FlagR = TRUE;	// NO EMPTY COLUMN // commented by D.L.3.4.94 as never used
		n_j_empty = 0;
	}
	//////	j2 = j1 + nj - 1;	// OLD VARIANT ###
	j2 = jj - 1;		// NEW VARIANT !!!
	//////	nj = jj - j1;		// NEW VARIANT !!!
mR2:
	nj = j2 - j1 + 1;	// INVARIANTNO !!!

	if ((--count_5555)==0)
		goto  m_err;	//////////////////////////
	/*......................................................................*/
mD:
	for (ii=i2+1,FlagD=FALSE; ii<i_down; ii++)
	{	/* moving DOWN	*/
		p = ppMem [ii];
		for (jj=j1; jj<=j2; jj++)
		{
			if (p [jj] >= Porog_38)
				goto mD1;
		}
		break;	/* NO DOWN POINTS */
mD1:
		ni++;
		FlagD = TRUE;
	}

	i2 = i1 + ni - 1;

	if ((--count_5555)==0)
		goto  m_err;
	//////////////////////////

	if (i2 == (Itek + 127))
	{
		i2 = Itek + 99;	// 100 Lines
		/*	    Flag_DOWN_100 = TRUE;*/// commented by D.L.3.4.94 as never used
		goto  m_ex;	////////////////////////////////////////
	}
	if (FlagD)
		goto mR;	/****** REPEAT RIGHT ******/
	/* NOTA BENE: in some Cases here may be goto UCHET; MK */
	/*......................................................................*/
	n_j_empty = FlagL = 0;		// move LEFT, GLUE (0) or (+2)
	jj = (j2==j1)  ?  (j1 - 1) : (j1 - 1 + n_0_2);	// FIRST of NOT

	if (jj<=3)
	{
		j1 = 1;
		goto  mL2;
	}	// ON LEFT SIDE: zB sic !!!

	for ( ; jj>0; jj--)
	{
		// OLD: before 23.10.1992
		for (ii=i1; ii<=i2; ii++)
		{
			if (ppMem [ii] [jj] >= Porog_38)
				goto mL1;
		}
		//////	    break;	/* NO LEFT POINTS */
		//////	    if (jj==1)  break;	// no LEFT POINTS  &  LEFT SIDE !!!
		//////	    if (jj==2)  break;	// no LEFT POINTS  &  LEFT SIDE !!! ??????
		if (jj<=2)
		{
			j1 = 1;
			goto  mL2;
		}	// on LEFT SIDE: zB sic !!!
		if (++n_j_empty>n_0_2)
			break;  // no LEFT POINTS in 2 Columns
		continue;

mL1:
		FlagL = TRUE;
		n_j_empty = 0;
	}
	j1 = jj + 1;		// NEW VARIANT !!!
mL2:
	nj = j2 - j1 + 1;	// INVARIANTNO !!!

	if ((--count_5555)==0)
		goto  m_err;	//////////////////////////

	if (FlagL)
		goto mD;	/****** REPEAT DOWN ******/
	goto  m_ex;
	/*......................................................................*/
m_err:
	PR_BEG
		"GREY ERROR 5555:"
		"\nNI=%d  NJ=%d"
		"\ni1=%d i2=%d j1=%d j2=%d",
		NI, NJ, i1, i2, j1, (j1 + nj - 1));
	PR_END
	/*......................................................................*/
m_ex:
	//////	if ((Flag_musor) && (nj<=5) && (i1==i2))  {	// for Kor. 1*1
	if ((Flag_musor) && (nj<=6) && ((i2-i1)<2))
	{	// for Kor. 2*2
		Flag_Korob_OK = FALSE;
		return;
	}
	/*......................................................................*/
	Flag_Korob_OK = TRUE;
	Korob_i1 = i1;
	Korob_i2 = i2;
	Korob_j1 = j1;
	Korob_nj = nj;
}

void CKronrodBinarizator::Korob_calc_Factor()
{
	/* Calculation of Factor by Ndiff and Nblack:	*/
	/*..............................................*/
	/*	NEW: Var-2 27.07.1992:			*/
	/* Ndiff - number of BLACK Points (i,j),	*/
	/*	   for which one or 4 SOSED is WHITE,	*/
	/*	   i.e. has other density about porog;	*/
	/* Factor = (Nblack - Ndiff)*255/Nblack		*/
	/*..............................................*/
	/*	OLD: Var-1 24.07.1992:			*/
	/* Ndiff - number of points (i,j), for each	*/
	/*    or right (i,j+1), or down (i+1,j) SOSED	*/
	/*    has other density about porog;		*/
	/*..............................................*/
	/* Nblack - number of black points (>=porog);	*/
	/*	Var-1: spec.work for RIGHT & DOWN !!!	*/
	/*	Var-2: all black Border is diff !!!!!!	*/
	/* porog: HAND - par_W;  AUTO - Porog_12;	*/
	int32_t	LNblack, LNdiff, LNinter;
	uint32_t	i1,i2, j1,nj, nj_1;
	uint32_t	i, j;
	uchar	por_tek;
	puchar	pU, pC, pD;	/* Up, Centre, Down */

	i1 = Korob_i1;
	j1 = Korob_j1;
	i2 = Korob_i2;
	nj = Korob_nj;
	nj_1 = nj - 1;

	//////	por_tek = Porog_38;	// before 07.09.1992
	por_tek = Porog_12;	//   from 07.09.1992

	LNdiff = LNblack = 0;

	pC = ppMem [i1] + j1;	/* UP Line: */
	for (j=0; j<nj; j++)
	    if (pC[j]>=por_tek)
		{
			LNblack++;
			LNdiff++;
		}

	pC = ppMem [i2] + j1;	/* DOWN Line: */
	for (j=0; j<nj; j++)
	    if (pC[j]>=por_tek)
		{
			LNblack++;
			LNdiff++;
		}

	for (i=i1+1; i<i2; i++)
	{	/* NB: not including UP & DOWN	*/
	    pU = ppMem [i-1] + j1;
	    pC = ppMem [i]   + j1;
	    pD = ppMem [i+1] + j1;
	    if (pC[0]>=por_tek)
		{
			LNblack++;
			LNdiff++;
		}	/* LEFT	*/
	    if (pC[nj_1]>=por_tek)
		{
			LNblack++;
			LNdiff++;
		}   /* RIGHT	*/
	    for (j=1; j<nj_1; j++)
		{	/* NB: not including L & R !!!	*/
			if (pC[j]<por_tek)
				continue;
			LNblack++;
			if ((pU[j]<por_tek)   ||
				(pC[j-1]<por_tek) ||
				(pC[j+1]<por_tek) ||
				(pD[j]<por_tek))
			{
				LNdiff++;
				continue;
			}
		}   /* FOR J */
    }	/* FOR I */
	/*......................................................................*/
	LNinter = LNblack - LNdiff;
	//////	if (LNblack==0)  pr_ERR ("GREY: Korob EMPTY");	/* BBB	*/
	//////	if (LNblack==0)	 Korob_Factor = 0x80;	/* BBB	*/
	if (LNblack==0)
		Korob_Factor = 0;	// ???????????????????
	else
		Korob_Factor = (uint32_t)(LNinter*255L/LNblack);
	/*......................................................................*/
}

void CKronrodBinarizator::Korob_calc_TdP()
{
	int32_t	i1,i2, j1,nj, Factor;				/* OTLADKA ONLY	*/
	int32_t	i, j;
	uchar	mkMax, nn/*, dd*/;

	if (Flag_T==FALSE)
		return;
	/*......................................................................*/
	Factor  = Korob_Factor;
	if (Factor<par_Q)
		return;
	/*......................................................................*/
	i1 = Korob_i1;
	j1 = Korob_j1;
	i2 = Korob_i2;
	nj = Korob_nj;

	for (j=0; j<nj; j++)
	{
		mkMax = 0;
		for (i=i1; i<=i2; i++)
		{
			nn = ppMem [i1 + i] [j1 + j];
			if (nn>mkMax)
				mkMax = nn;		/* VERT MAXIMUM	*/
		}
		///////	    dd = (mkMax>=Porog_34)  ?  0 : (Porog_34 - mkMax)/8;  ???????????
		//	    if (mkMax>=Porog_34)                          // commented by D.L.3.4.94 as never used
		//		  dd = (uchar) (((uint16_t) (Porog_34 - mkMax) / 8));// commented by D.L.3.4.94 as never used
		//	    else  dd = 0;// commented by D.L.3.4.94 as never used
		///////	    Buf_TdP [j1 + j] = dd;
	}
}

void CKronrodBinarizator::Korob_calc_porog_x_y()
{
	int32_t	Factor;
	int32_t	porog_tek, xx, yy;

	if (Flag_Xverb)
		goto m_TIMES_ROMAN;	// from 27.10.1992
	if (Flag_VERB)
		goto m_Verblud;		// from 04.09.1992
	/*......................................................................*/
	Factor = Korob_Factor;				// OLD VARIANT:
	if (Factor<0x68)
	{
		porog_tek = Porog_38;
		goto m_dP;
	}

	if (Factor<0x98)
	{
		/* 68Ж98 (0x30): */
	    porog_tek = Porog_38 + (Factor-0x68)*2*Diapazon_8/0x30;
	    goto m_dP;
	}
	porog_tek = Porog_58;

m_dP:
	//////porog_tek += par_dP;
	if (Factor<0x40)
	{
		xx = 0;
		yy = 24;
		goto m_ex;
	}

	if (Factor<0x60)
	{
		/* 40Ж60 (0x20): */
	    xx = 0;
	    yy = 24 - (Factor - 0x40)*8/0x20;
	    goto m_ex;
	}

	if (Factor<0x80)
	{
		/* 60Ж80 (0x20): */
	    xx = (Factor - 0x60)*8/0x20;
		//////	    yy = 24 - (Factor - 0x60)*8/0x20;	// ERROR before 27.10 92 !!!
	    yy = 16 - (Factor - 0x60)*8/0x20;
	    goto m_ex;
	}

	if (Factor<0xA0)
	{
		/* 80ЖA0 (0x20): */
	    xx = 8 + (Factor - 0x80)*8/0x20;
	    yy = 8 - (Factor - 0x80)*8/0x20;
	    goto m_ex;
	}

	if (Flag_PLAN==FALSE)
	{
		/* OLD LAPLAS, WITHOUT PLAN:	*/
	    if (Factor<0xC0)
		{
			/* A0ЖC0 (0x20): */
			xx = 16 + (Factor - 0xA0)*8/0x20;
			yy = 0;
			goto m_ex;
		}
	    xx = 24;
		yy = 0;		/* C0ЖFF */
	    goto m_ex;
	}

	if (Flag_PLAN==TRUE)
	{
		/* NEW LAPLAS, WITH:	*/
	    if (Factor<0xC0)
		{
			/* A0ЖC0 (0x20): */
			xx = 16 - (Factor - 0xA0)*16/0x20;	/* DECREASE !!!	*/
			yy = 0;
			goto m_ex;
		}
	    xx = 0;
		yy = 0;		/* C0ЖFF */
	}
	goto m_ex;
	/*......................................................................*/
m_TIMES_ROMAN:
	// more soft Analog of Verblud (must be +X, +V):

	/**********************************************
	Factor = Korob_Factor;
	xx = 0;		// BBB
	if (Factor<=0x40)	yy = 24;
	else if (Factor<0xA0)	yy = (0xA0 - Factor) / 4;
	else			yy = 0;
	*************************************************/

	// NB: OLD: 40-A0:  24 -> 0
	//     NEW: 50-90:  16 -> 0
	Factor = Korob_Factor;
	xx = 0;		// BBB

	if (Factor<=0x50)
		yy = 16;
	else
		if (Factor<0x90)
			yy = (0xA0 - Factor) / 4;
		else
			yy = 0;

	// VARIANT   27.10.1002:
	// Factor    Porog   Divider
	// 00 - 70   3/8......	----
	// 70 - A0   3/8 - 6/8	0x10
	// A0 - D0   6/8......	----
	// D0 - E0   6/8 - 4/8	0x20
	// E0 - FF   4/8......	----

	if (Factor<=0x70)
		porog_tek = Porog_38;
	else
		if (Factor<0xA0)
			porog_tek = Porog_38 + (Factor - 0x70)*Diapazon_8 / 0x10;
		else
			if (Factor<=0xD0)
				porog_tek = Porog_34;
			else
				if (Factor<0xE0)
					porog_tek = Porog_34 - (Factor - 0xD0)*Diapazon_8 / 0x20;
				else
					porog_tek = Porog_12;
	goto m_ex;
	/*......................................................................*/

m_Verblud:
	//			6/8   -
	//			    /	\
	//			  /   B	  \
	//			/   A	C   - -  4/8
	//		      /   9	  D
	// 3/8	- - - - - - /   8	    E F
	//	............. 7
	//	0 1 2 3 4 5 6

	Factor = Korob_Factor;
	xx = 0;		// BBB

	if (Factor<=0x40)
		yy = 24;
	else
		if (Factor<0xA0)
			yy = (0xA0 - Factor) / 4;
		else
			yy = 0;

	/*......................................................................*/
	// VARIANT-4 22.09.1002 (????):
	// Factor    Porog   Divider
	// 00 - 60   3/8......	----
	// 60 - 90   3/8 - 6/8	0x10
	// 90 - B0   6/8 - 7/8	0x20
	// B0 - C0   7/8......	----
	// C0 - D8   7/8 - 6/8	0x18
	// D8 - E0   6/8 - 5/8	0x08
	// E0 - FF   5/8......	----
	if (Factor<=0x60)
		porog_tek = Porog_38;
	else
		if (Factor<0x90)
			porog_tek = Porog_38 + (Factor - 0x60)*Diapazon_8 / 0x10;
		else
			if (Factor<0xB0)
				porog_tek = Porog_34 + (Factor - 0x90)*Diapazon_8 / 0x20;
			else
				if (Factor<=0xC0)
					porog_tek = Porog_78;
				else
					if (Factor<0xD8)
						porog_tek = Porog_78 - (Factor - 0xC0)*Diapazon_8 / 0x18;
					else
						if (Factor<0xE0)
							porog_tek = Porog_34 - (Factor - 0xD8)*Diapazon_8 / 0x08;
						else
							porog_tek = Porog_12;
	/*......................................................................*/
m_ex:

	Korob_porog = (uchar) porog_tek;
	Korob_x = xx;
	Korob_y = yy;
/*......................................................................*/
}

void CKronrodBinarizator::Korob_Files_Open()
{
	Flag_ERR_DISK = FALSE;
	mkFile_KOROBS_HEADS = fopen ("\\CUNEI\\KOROBS\\kor-head.img", "wb");
	mkFile_KOROBS_BODIES= fopen ("\\CUNEI\\KOROBS\\kor-body.img", "wb");
	if (mkFile_KOROBS_HEADS==NULL)
		Flag_ERR_DISK = TRUE;
	if (mkFile_KOROBS_BODIES==NULL)
		Flag_ERR_DISK = TRUE;
	Korob_Files_Address = 0;
}

void CKronrodBinarizator::Korob_Files_Write()
{
	uint32_t	n, ni, i;
	//////////////////////////////////////////////////

	if (Flag_ERR_DISK)  return;

	buf_H [0] = Korob_i1;
	buf_H [1] = Korob_j1;
	buf_H [2] = ni = Korob_i2 - Korob_i1 + 1;
	buf_H [3] = Korob_nj;

	buf_H [4] = Korob_Factor + (Korob_porog << 8);
	buf_H [5] = Porog_08 + (Porog_88 << 8);
	* (uint32_t*) (buf_H + 6) = Korob_Files_Address;

	Korob_Files_Address += (uint32_t) ni * Korob_nj;


	n = fwrite (buf_H, 2, 8, mkFile_KOROBS_HEADS);
	if (n!=8)
	PR_BEG
		"ERROR Korob_Files_Write: n=%d", n);
	PR_END
	/*......................................................................*/
	for (i=Korob_i1; i<=Korob_i2; i++)
	{
	    memcpy ((LPBYTE) buf_B, ppMem [i] + Korob_j1, Korob_nj);	///
	    n = fwrite (buf_B, 1, Korob_nj, mkFile_KOROBS_BODIES);
	    if (n!=Korob_nj)
		PR_BEG
			"ERROR Korob_Files_Write: n=%d#%d",
			 n, Korob_nj);
		PR_END
	}
}

void CKronrodBinarizator::Korob_Files_Close()
{
	if (Flag_ERR_DISK)
		return;
	fclose (mkFile_KOROBS_HEADS);
	fclose (mkFile_KOROBS_BODIES);
}

void CKronrodBinarizator::Korob_see()
{
	if ((Korob_i1>I_see) || (Korob_i2<I_see)  ||
	    (Korob_j1>J_see) || (Korob_j1 + Korob_nj <= J_see))
		return;

	PR_BEG
		"see %d-%d: K-%d)  %d-%d %d-%d (%d)\n"
		"P38=%02X P12=%02X P34=%02X  D8=%02X\n"
		"F=%02X P=%02X x=%02X y=%02X",
		I_see, J_see, N_Korob, Korob_i1, Korob_i2,
		Korob_j1, (Korob_j1 + Korob_nj - 1), Korob_nj,
		Porog_38, Porog_12, Porog_34, Diapazon_8,
		Korob_Factor, Korob_porog, Korob_x, Korob_y);
	PR_END
}

void CKronrodBinarizator::Korob_statistic()
{
	uint32_t	i1,i2, j1,nj, nj_1;
	uint32_t	i, j;
	int32_t	nC, nL, nR;
	int32_t	por_tek;
	puchar	pC;
	/*......................................................................*/
	LS_Korobs += (Korob_i2 - Korob_i1 + 1) * (int32_t) Korob_nj;
	LSH_Korobs += (Korob_i2 - Korob_i1 + 1);
	LSL_Korobs += Korob_nj;
	/*......................................................................*/
	i1 = Korob_i1;
	j1 = Korob_j1;
	i2 = Korob_i2;
	nj = Korob_nj;
	nj_1 = nj - 1;

	por_tek = Korob_porog;

	PR_BEG
		"K-%d) %d-%d %d,%d\nF=%02X P=%02X",
		N_Korob+1, i1,i2, j1,nj, Korob_Factor, por_tek);
	PR_END

	for (i=i1; i<=i2; i++)
	{
	    pC = ppMem [i] + j1;
	    for (j=1; j<nj_1; j++)
		{
			/* NB: not including L & R !!!	*/
			nC = pC [j];
			nL = pC [j - 1];
			nR = pC [j + 1];
			if (nC<por_tek)
				continue;	// mulT be BLACK
			if (nL<por_tek)
				continue;	// mulT be BLACK
			if (nR<por_tek)
				continue;	// mulT be BLACK
			if ((nR-nC)>=16)
				LNgrey16++;
			if ((nR-nC) >= (int16_t) Diapazon_8)
				LNgreyD8++;
			if ((nR-nC) >= 8)
				LNgrey08++;
			if ((nR-nC)>=16)
			PR_BEG
				"%ld) %d-%d K=%d\n%02X %02X %02X\nF=%02X P=%02X",
				LNgrey16, i, j1+j, N_Korob+1, nL, nC, nR, Korob_Factor, por_tek);
			PR_END
		}   /* FOR J */
    }	/* FOR I */
}

void CKronrodBinarizator::Korob_statistic_print()
{
	uint32_t	percent;

	percent = (uint16_t) (LS_Korobs * 100L / (NI * (int32_t) NJ));
	PR_BEG
		"%d Korobs;  S=%ld (%d)\n"
		"SH=%ld SL=%ld\n"
		"Ngrey: 16)%ld  D8)%ld  08)%ld",
		N_Korob, LS_Korobs, percent,
		LSH_Korobs, LSL_Korobs,
		LNgrey16, LNgreyD8, LNgrey08);
	PR_END
}

void CKronrodBinarizator::memory_allocation()
{
	uint32_t	k;

	if (Flag_OTL)
		PR_BEG  "N.B.p.O.M=%04X", N_Bytes_per_ONE_MEM); PR_END

	for (k=0; k<8; k++)
	{	//~64 000 b (63 750) per each of 8 MEMs:
	    if (hMem08[k])
			PR_BEG "MEM.ALL ERR: hMem08 [%d] = %04X", k, hMem08 [k]); PR_END

		hMem08[k] = KRONROD_ALLOC(N_Bytes_per_ONE_MEM);   //GlobalAlloc (GHND, N_Bytes_per_ONE_MEM);

		if (hMem08[k]==0)
			PR_BEG "hMem08 [%d] = %04X", k, hMem08 [k]); PR_END
	}

	if (Flag_OTL)
		PR_BEG  "N.B.in.all.MBIT=%04X", N_Bytes_in_all_MBIT); PR_END

	if (hMBIT)
		PR_BEG "MEM.ALL ERR: hMBIT = %04X", hMBIT);	PR_END

	hMBIT = KRONROD_ALLOC(N_Bytes_in_all_MBIT); //GlobalAlloc (GHND, N_Bytes_in_all_MBIT);	//~63 800 b

	if (hMBIT==0)
		PR_BEG "hMBIT = %04X", hMBIT);	PR_END

	if (hMREF)
		PR_BEG "MEM.ALL ERR: hMREF = %04X", hMREF);	PR_END

	hMREF = KRONROD_ALLOC(NJ*6);

	if (hMREF==0)
		PR_BEG "hMREF = %04X", hMREF);	PR_END

	if (hPPMEM)
		PR_BEG "MEM.ALL ERR: hPPMEM = %04X", hPPMEM);  PR_END

	hPPMEM = KRONROD_ALLOC(NI*4);

	if (hPPMEM==0)
		PR_BEG "hPPMEM = %04X", hPPMEM);  PR_END
	/*......................................................................*/
	for (k=0; k<8; k++)
	{
	    pMem08[k] = (puchar)KRONROD_LOCK(hMem08[k]); //GlobalLock (hMem08 [k]);

		if (pMem08[k]==NULL)
			PR_BEG "pMem08[%d] = NULL", k);  PR_END
	}

	pMBIT = (puchar)KRONROD_LOCK(hMBIT); //GlobalLock (hMBIT);	// BITMAP
	if (pMBIT==NULL)  PR_BEG "pMBIT = NULL", k);  PR_END

	pMREF = (puchar)KRONROD_LOCK(hMREF); //GlobalLock (hMREF);	// KOROBS REFERENCES
	if (pMREF==NULL)  PR_BEG "pMREF = NULL", k);  PR_END

	ppMem = (uchar far * far *) KRONROD_LOCK(hPPMEM); //GlobalLock (hPPMEM);	// POINTERS to LINES
	if (ppMem==NULL)  PR_BEG "ppMem = NULL", k);  PR_END
	/*......................................................................*/
	KEY
		PR_BEG
			"hMem08: %X,%X,%X,%X,%X,%X,%X,%X",
			hMem08[0], hMem08[1], hMem08[2], hMem08[3],
			hMem08[4], hMem08[5], hMem08[6], hMem08[7]);
		PR_END

	KEY
		PR_BEG
			"pMem08: %lX,%lX,%lX,%lX\n....... %lX,%lX,%lX,%lX",
			pMem08[0], pMem08[1], pMem08[2], pMem08[3],
			pMem08[4], pMem08[5], pMem08[6], pMem08[7]);
		PR_END
}

void CKronrodBinarizator::memory_alloc_from_body()
{
	uint32_t	k;

	if (!Virginity)  return;

	PR_BEG
		"MEMORY_ALLOC_FROM_BODY");
	PR_END

	Virginity = FALSE;

	for (k=0; k<8; k++)
	{
		//~64 000 b (63 750) per each of 8 MEMs:
	    hMem08[k] = KRONROD_ALLOC(0xFFFE); //GlobalAlloc (GHND, 0xFFFE);

		if (hMem08[k]==0)
			PR_BEG
				"hMem08 [%d] = %04X",
				k, hMem08[k]);
			PR_END
	}

	hMBIT = KRONROD_ALLOC(0xFFFE); //GlobalAlloc (GHND, 0xFFFE);		//~63 800 b
	if (hMBIT==0)
		PR_BEG
			"hMBIT = %04X", hMBIT);
		PR_END

	hMREF = KRONROD_ALLOC(MaxNJ*6); //GlobalAlloc (GHND, MAX_NJ*6);		// NJ*3 int16_t
	if (hMREF==0)
		PR_BEG
			"hMREF = %04X", hMREF);
		PR_END

	hPPMEM = KRONROD_ALLOC(MaxNI*sizeof(char**)); //GlobalAlloc (GHND, MAX_NI*4);		// MAX_NI far *
	//////	hPPMEM = GlobalAlloc (GHND, NI*4);		// NI far *
	if (hPPMEM==0)
		PR_BEG
			"hPPMEM = %04X", hPPMEM);
		PR_END
	/*......................................................................*/
	for (k=0; k<8; k++)
	{
	    pMem08 [k] = (puchar)KRONROD_LOCK(hMem08[k]); //GlobalLock (hMem08 [k]);
	    if (pMem08[k]==NULL)
			PR_BEG
				"pMem08 [%d] = NULL", k);
			PR_END
	}

	pMBIT = (puchar)KRONROD_LOCK(hMBIT); //GlobalLock (hMBIT);	// BITMAP
	if (pMBIT==NULL)
		PR_BEG
			"pMBIT = NULL", k);
		PR_END

	pMREF = (puchar)KRONROD_LOCK(hMREF); //GlobalLock (hMREF);	// KOROBS REFERENCES
	if (pMREF==NULL)
		PR_BEG
			"pMREF = NULL", k);
		PR_END

	ppMem = (uchar far * far *) KRONROD_LOCK(hPPMEM); //GlobalLock (hPPMEM);	// POINTERS to LINES
	if (ppMem==NULL)
		PR_BEG
			"ppMem = NULL", k);
		PR_END
	/*......................................................................*/
	KEY
		PR_BEG
			"hMem08: %X,%X,%X,%X,%X,%X,%X,%X",
			hMem08[0], hMem08[1], hMem08[2], hMem08[3],
			hMem08[4], hMem08[5], hMem08[6], hMem08[7]);
		PR_END

	KEY
		PR_BEG
			"pMem08: %lX,%lX,%lX,%lX\n....... %lX,%lX,%lX,%lX",
			pMem08[0], pMem08[1], pMem08[2], pMem08[3],
			pMem08[4], pMem08[5], pMem08[6], pMem08[7]);
		PR_END

}

void CKronrodBinarizator::memory_free()
{
	uint32_t	k;
				//for 8 MEMs
	for (k=0; k<8; k++)
	{
		if (hMem08[k])
			KRONROD_FREE( hMem08[k] );

		hMem08[k] = 0;
	}

	if (hMBIT)
		KRONROD_FREE(hMBIT);

	hMBIT = 0;

	if (hMREF)
		KRONROD_FREE(hMREF);

	hMREF = 0;

	if (hPPMEM)
		KRONROD_FREE(hPPMEM);

	hPPMEM = 0;
}

void CKronrodBinarizator::pr_ERR(pchar text)
{
	wsprintf ((char *)(mkText), (char *)text);
	MMM;
	//////	MessageBox (GetFocus(), mkText, 0, MB_SYSTEMMODAL);
}

void CKronrodBinarizator::pr_ERR_1(pchar text, int32_t n1)
{
	//////	wsprintf (mkText, "MK GREY MESSAGE [1] %s %d", text, n1);
	wsprintf ((char *)mkText, (char *)text, n1);
	MMM;
}

void CKronrodBinarizator::pr_ERR_2(pchar text, int32_t n1, int32_t n2)
{
	//////	wsprintf (mkText, "MK GREY MESSAGE [2] %s %d,%d", text, n1, n2);
	wsprintf ((char *)mkText, (char *)text, n1, n2);
	MMM;
}

void CKronrodBinarizator::pr_ERR_3(pchar text, int32_t n1, int32_t n2, int32_t n3)
{
	wsprintf ((char *)mkText, (char *)text, n1, n2, n3);
	MMM;
}

void CKronrodBinarizator::pr_MES(pchar text)
{
	wsprintf ((char *)mkText, (char *)text);
	MMM_NP;
	//////	MessageBox (GetFocus(), mkText, 0, 0);
}

void CKronrodBinarizator::pr_MES_1(pchar text, int32_t n1)
{
	//////	wsprintf (mkText, "MK GREY MESSAGE [1] %s %d", text, n1);
	wsprintf ((char *)mkText, (char *)text, n1);
	MMM_NP;
}

void CKronrodBinarizator::pr_MES_2(pchar text, int32_t n1, int32_t n2)
{
	//////	wsprintf (mkText, "MK GREY MESSAGE [2] %s %d,%d", text, n1, n2);
	wsprintf ((char *)mkText, (char *)text, n1, n2);
	MMM_NP;
}

void CKronrodBinarizator::pr_MES_3(pchar text, int32_t n1, int32_t n2, int32_t n3)
{
	wsprintf ((char *)mkText, (char *)text, n1, n2, n3);
	MMM_NP;
}

uint32_t CKronrodBinarizator::mk_1235()
{
	return 0;
}

void CKronrodBinarizator::mk_hist_WORD(uint32_t * Hist_WORD, puchar p, uint32_t n)
{
	return;
}
