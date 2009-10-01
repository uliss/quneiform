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

// CDezaBinarizator.h: interface for the CDezaBinarizator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CKRONROD_BINARIZATOR_H_)
#define _CKRONROD_BINARIZATOR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*#include <windows.h>*/
#include <stdio.h>
#include "stdlib.h"
#include "globus.h"
#include "crimemory.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
#define KRONROD_ALLOC(a)          RIMAGEAlloc(a)
#define KRONROD_FREE(h)           RIMAGEFree(h)
#define KRONROD_LOCK(h)           RIMAGELock(h)
#define KRONROD_UNLOC(h)          RIMAGEUnlock(h)
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifdef WIN32
	#define EXPORT(a) __declspec(dllexport) a
#else
	#define EXPORT(a)  a _export
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
class CRIBinarizator;
///////////////////////////////////////////////////////////////////////////////////////////////////
//
class CKronrodBinarizator
{
	//
#define BUFSZ                0x4000
#define	I_see                430
#define	J_see                40
#define	mkA0                 0xA0
#define	par_Q                0x80
#define	MAX_NI	             8192//4096
#define	MAX_NJ	             8192//4096
#define	MKPR	             wsprintf (mkText,	// "qwer=%d", qwer);   MMM;
#define	MMM	                 MessageBox (GetFocus(), (char *)mkText, 0, MB_SYSTEMMODAL)
#define	MMM_NP	             MessageBox (GetFocus(), (char *)mkText, 0, 0)
	//
#define KRONROD_EXPORT(a)    a            //EXPORT
#define	KEY						if (mk_Key)
#define	KEY_ERR_1				if (mk_Key)  \
									pr_ERR_1
#define	PR_BEG					{ \
									char buf_MKPR [199]; \
									wsprintf (buf_MKPR,
#define	PR_END						MessageBox (GetFocus(), buf_MKPR, 0, MB_SYSTEMMODAL);\
								}
	//
private:
	Handle               hAppWnd;
	puchar               BufZone; // [BUFSZ];
	puchar               aBufPass;
	Handle               hBufPass;
	uchar                szTempTiffName[256];
	char                  szScanFullName[256];
	char                  szScanDriverName[32];
	char                  Main_Dir_Name [128];    // H:\CUNEI  (see zB SCANTAB.C)
	pchar                 MainDir;
	uchar                szValidNames[256];
    uint32_t                MaxNI;
    uint32_t                MaxNJ;
//	uint32_t               Itek;                   /////////////////// OTLADKA ONLY

private:
	int32_t				Ln_hist;
	int32_t				Korob_Files_Address;

	uint32_t				I_to;		//current Line "TO"
	uint32_t				Itek;       //current Line "FROM"
	uint32_t				NI;
	uint32_t				NJ;
	uint32_t				NI_1;
	uint32_t				NJ_1;
	uint32_t				NJ_2;
	uint32_t				NJx3;
	uint32_t				N_Bytes_FROM;
	uint32_t				N_Bytes_per_ONE_MEM;    //= 64000;	// it was 0xC000: 48 Kb
	uint32_t				N_Bytes_in_MBIT_Line;		// 2550 -> 319
	uint32_t				N_Bytes_in_all_MBIT;		// 200 * 319 = 63 800

	uint32_t				N_Lines_per_1_MEM;	// 64000 / 2550 = 25 (25.098)
	uint32_t				N_Lines_per_4_MEMs;	// 4 * 25 = 100
	uint32_t				N_Lines_per_8_MEMs;	// 8 * 25 = 200
	uint32_t				N_of_TEK_MEM;		// 0 - 7;
	uint32_t				N_Lines_in_TEK_MEM;
	uint32_t				N_tek_Line_in_MBIT;

	uchar				IER;
	uchar				Flag_ERR_Hist_L;
	uchar				Flag_ERR_Hist_R;
	uchar				mk_Key;		// -Key		MK KEY for OTLADKA PRINTS
	uchar				Flag_3x3;	// +3x3		enable any work 3*3;
	uchar				Flag_5work;	// +5work	Work with 5x, 5y;
	uchar				Flag_d8P;	// +d8P		Porog +- Diapazon_8 (for Speed)
	uchar				Flag_GLUE;	// +Glue 	make Korob (right + 2);
	uchar				Flag_Hor;	// +Hor		Hor. SEDLO;
	uchar				Flag_Lapl;	// +Laplas	Laplas 3*3 Contour
	uchar				Flag_MAX;	// +MAX		for new POROG QUANT 1x3 (use D_16)
	uchar				Flag_musor;	// +musor1x5	DELETE small Korobs
	uchar				Flag_0musor;	// +0musor	DELETE Korobs more 8*8 with F=0
	uchar				Flag_PLAN;	// +Plan	"Plan" 3*3 Contour
	uchar				Flag_T;		// -T		SHAVE (not used now);
	uchar				Flag_Vert;	// +Vert        Vert. SEDLO;
	uchar				Flag_VERB;	// +Verblud	Porog with GORB;	// OLD WERB
	uchar				Flag_WrKor;	// +WrKor	Write 2 Korob Files
	uchar				Flag_Xverb;	// +Xverblud	for TIMES ROMAN
	uchar				Flag_Z;		// -Z		RAZREZ (not used now);
	uchar				Flag_Draw;
	uchar				Flag_see;	 // for OTLADKA only
	uchar				Flag_STAT;	 // for OTLADKA: Statistic
	uchar				Flag_FactPR;
	uchar				Flag_grHist; // for OTLADKA only
	uchar				Flag_OTL;	 // can be used for any non-destructive OTLADKA  M.K.
	uchar				Flag_ERR_DISK;
	uchar				Flag_Korob_OK;
	uchar				Flag_Mem_123;
	uchar				Flag_OPEN;
	uchar				Porog_12;
	uchar				Porog_34;
	uchar				Porog_08;
	uchar				Porog_88;  // for OTLADKA
	uchar				Diapazon_8;
	uchar				Porog_38;
	uchar				Porog_58;
	uchar				Porog_78;
	uchar				Diapazon_16;
	//----------------------------------------------------------------------//
	char 				mkText[256];					//ARRAYs
	int32_t				Hist_tek[256];
	int32_t				Hist_glad[256];
	//----------------------------------------------------------------------
	FILE				*mkFile_GREY_INI;					// FILES
	FILE				*mkFile_KOROBS_HEADS;
	FILE				*mkFile_KOROBS_BODIES;
	//----------------------------------------------------------------------
									//HANDLEs
	Handle				hMem08[8]; //	// ARRAY for 8 MEMs
	Handle				hMBIT;
	Handle				hMREF;
	Handle				hPPMEM;
	//----------------------------------------------------------------------
	puchar				pMem08[8];	// ARRAY for 8 MEMs			//POINTERs
	puchar				pMBIT;
	puchar				pMBIT_tek;
	puchar				pMREF;		// for KOROBS REFERENCES
	puchar				pMem_TEK;
	puchar				pMem_kuda;
	puchar				*ppMem;	//Array of FAR Pointers
	//----------------------------------------------------------------------
								// KOROB VARIABLES
	uint32_t				Korob_i1;		// ==Itek !!!
	uint32_t				Korob_i2;		// include this Line
	uint32_t				Korob_j1;
	uint32_t				Korob_nj;
	uint32_t				Korob_Factor;
	int32_t				Korob_x;
	int32_t				Korob_y;
	uchar				Korob_porog;
	//----------------------------------------------------------------------
	uint32_t				N_Korob;					// KOROBS STATISTIC
	int32_t				LS_Korobs;
	int32_t				LSH_Korobs;
	int32_t				LSL_Korobs;
	int32_t				LNgrey16;
	int32_t				LNgreyD8;
	int32_t				LNgrey08;
	//----------------------------------------------------------------------*/
	uint32_t				Hex_35[16];

private:
	Bool32 Virginity;
	uint32_t buf_H[8];
	uint32_t buf_B[2550];
public:
	/*
	KRONROD_EXPORT(void)   grey_open (uint32_t H, uint32_t N);
	KRONROD_EXPORT(uint32_t) grey_to   (puchar pKogo);        //returns n_lines_ready
	KRONROD_EXPORT(void)   grey_from (puchar pKuda);
	KRONROD_EXPORT(uint32_t) grey_close (void);      //returns n_lines_ready
*/
public:
	uint32_t	grey_close(void);
	void	grey_from(puchar pKuda);
	uint32_t	grey_to(puchar pKogo);
	void	grey_open(uint32_t H, uint32_t N);
	CKronrodBinarizator();
	~CKronrodBinarizator();
protected:
	void mk_hist_WORD(uint32_t * Hist_WORD, puchar p, uint32_t n);
	uint32_t mk_1235(void);
	void pr_MES_3(pchar  text, int32_t n1, int32_t n2, int32_t n3);
	void pr_MES_2(pchar  text, int32_t n1, int32_t n2);
	void pr_MES_1(pchar  text, int32_t n1);
	void pr_MES(pchar  text);
	void pr_ERR_3(pchar  text, int32_t n1, int32_t n2, int32_t n3);
	void pr_ERR_2(pchar  text, int32_t n1, int32_t n2);
	void pr_ERR_1(pchar  text, int32_t n1);
	void pr_ERR(pchar  text);
	void memory_free(void);
	void memory_alloc_from_body(void);
	void memory_allocation(void);
	void Korob_statistic_print(void);
	void Korob_statistic(void);
	void Korob_see(void);
	void Korob_Files_Close(void);
	void Korob_Files_Write(void);
	void Korob_Files_Open(void);
	void Korob_calc_porog_x_y(void);
	void Korob_calc_TdP(void);
	void Korob_calc_Factor(void);
	void Korob_calc_BOX(uint32_t j);
	void Korob_add(uint32_t j);
	void Korobs_CLEAR_and_GLUE(void);
	void Korobs(void);
	void Korobs_init(void);
	void hist_MIN_MAX(void);
	void hist_glad_8_121(void);
	void hist_add(puchar p, uint32_t N_Lines);
	void grey_PR_BYTE(uint32_t ibeg, uint32_t jbeg, uint32_t value);
	void grey_quant_KOROB_PR_Factor(void);
	void grey_quant_KOROB_graph_Hist(void);
	void grey_quant_KOROB_draw_RAMKA(void);
	void grey_quant_KOROB_3x3_VERB(void);
	void grey_quant_KOROB_3x3(void);
	void grey_quant_KOROB_1x5work(void);
	void grey_quant_KOROB_1x3_MAX(void);
	void grey_quant_KOROB_1x1(void);
	void grey_quant_KOROB(void);
	void grey_to_monitor_MBIT(uint32_t n_from);
	void grey_to_memory_monitor(void);
	void grey_open_init_MEM(void);
	void grey_open_init_flags(void);
};
///////////////////////////////////////////////////////////////////////////////////////////////////
//
typedef CKronrodBinarizator *PCKronrodBinarizator;
///////////////////////////////////////////////////
//
#endif // !defined(_CKRONROD_BINARIZATOR_H_)
