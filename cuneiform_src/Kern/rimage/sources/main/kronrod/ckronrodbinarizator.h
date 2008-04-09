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

// CDezaBinarizator.h: interface for the CDezaBinarizator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CKRONROD_BINARIZATOR_H_)
#define _CKRONROD_BINARIZATOR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

# if defined (_DEBUG)
# pragma message( __FILE__" : Binarizator by Kronrod")
# endif // (_DEBUG)

#include <windows.h>
#include <stdio.h>
#include "stdlib.h"
#include "globus.h"
#include "CRIMemory.h"

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
	#define _fmemset(a,b,c) memset(a,b,c)
	#define _fmemcpy(a,b,c) memcpy(a,b,c)
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
	PWord8               BufZone; // [BUFSZ];
	PWord8               aBufPass;
	Handle               hBufPass;
	Word8                szTempTiffName[256];
	Int8                  szScanFullName[256];
	Int8                  szScanDriverName[32];
	Int8                  Main_Dir_Name [128];    // H:\CUNEI  (see zB SCANTAB.C)
	PInt8                 MainDir;
	Word8                szValidNames[256];
    Word32                MaxNI;
    Word32                MaxNJ;
//	Word32               Itek;                   /////////////////// OTLADKA ONLY

private:
	Int32				Ln_hist;
	Int32				Korob_Files_Address;

	Word32				I_to;		//current Line "TO"
	Word32				Itek;       //current Line "FROM"
	Word32				NI;
	Word32				NJ;
	Word32				NI_1;
	Word32				NJ_1;
	Word32				NJ_2;
	Word32				NJx3;
	Word32				N_Bytes_FROM;
	Word32				N_Bytes_per_ONE_MEM;    //= 64000;	// it was 0xC000: 48 Kb
	Word32				N_Bytes_in_MBIT_Line;		// 2550 -> 319
	Word32				N_Bytes_in_all_MBIT;		// 200 * 319 = 63 800

	Word32				N_Lines_per_1_MEM;	// 64000 / 2550 = 25 (25.098)
	Word32				N_Lines_per_4_MEMs;	// 4 * 25 = 100
	Word32				N_Lines_per_8_MEMs;	// 8 * 25 = 200
	Word32				N_of_TEK_MEM;		// 0 - 7;
	Word32				N_Lines_in_TEK_MEM;
	Word32				N_tek_Line_in_MBIT;

	Word8				IER;
	Word8				Flag_ERR_Hist_L;
	Word8				Flag_ERR_Hist_R;
	Word8				mk_Key;		// -Key		MK KEY for OTLADKA PRINTS
	Word8				Flag_3x3;	// +3x3		enable any work 3*3;
	Word8				Flag_5work;	// +5work	Work with 5x, 5y;
	Word8				Flag_d8P;	// +d8P		Porog +- Diapazon_8 (for Speed)
	Word8				Flag_GLUE;	// +Glue 	make Korob (right + 2);
	Word8				Flag_Hor;	// +Hor		Hor. SEDLO;
	Word8				Flag_Lapl;	// +Laplas	Laplas 3*3 Contour
	Word8				Flag_MAX;	// +MAX		for new POROG QUANT 1x3 (use D_16)
	Word8				Flag_musor;	// +musor1x5	DELETE small Korobs
	Word8				Flag_0musor;	// +0musor	DELETE Korobs more 8*8 with F=0
	Word8				Flag_PLAN;	// +Plan	"Plan" 3*3 Contour
	Word8				Flag_T;		// -T		SHAVE (not used now);
	Word8				Flag_Vert;	// +Vert        Vert. SEDLO;
	Word8				Flag_VERB;	// +Verblud	Porog with GORB;	// OLD WERB
	Word8				Flag_WrKor;	// +WrKor	Write 2 Korob Files
	Word8				Flag_Xverb;	// +Xverblud	for TIMES ROMAN
	Word8				Flag_Z;		// -Z		RAZREZ (not used now);
	Word8				Flag_Draw;
	Word8				Flag_see;	 // for OTLADKA only
	Word8				Flag_STAT;	 // for OTLADKA: Statistic
	Word8				Flag_FactPR;
	Word8				Flag_grHist; // for OTLADKA only
	Word8				Flag_OTL;	 // can be used for any non-destructive OTLADKA  M.K.
	Word8				Flag_ERR_DISK;
	Word8				Flag_Korob_OK;
	Word8				Flag_Mem_123;
	Word8				Flag_OPEN;
	Word8				Porog_12;
	Word8				Porog_34;
	Word8				Porog_08;
	Word8				Porog_88;  // for OTLADKA
	Word8				Diapazon_8;
	Word8				Porog_38;
	Word8				Porog_58;
	Word8				Porog_78;
	Word8				Diapazon_16;
	//----------------------------------------------------------------------//
	Int8 				mkText[256];					//ARRAYs
	Int32				Hist_tek[256];
	Int32				Hist_glad[256];
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
	PWord8				pMem08[8];	// ARRAY for 8 MEMs			//POINTERs
	PWord8				pMBIT;
	PWord8				pMBIT_tek;
	PWord8				pMREF;		// for KOROBS REFERENCES
	PWord8				pMem_TEK;
	PWord8				pMem_kuda;
	PWord8				*ppMem;	//Array of FAR Pointers
	//----------------------------------------------------------------------
								// KOROB VARIABLES
	Word32				Korob_i1;		// ==Itek !!!
	Word32				Korob_i2;		// include this Line
	Word32				Korob_j1;
	Word32				Korob_nj;
	Word32				Korob_Factor;
	Int32				Korob_x;
	Int32				Korob_y;
	Word8				Korob_porog;
	//----------------------------------------------------------------------
	Word32				N_Korob;					// KOROBS STATISTIC
	Int32				LS_Korobs;
	Int32				LSH_Korobs;
	Int32				LSL_Korobs;
	Int32				LNgrey16;
	Int32				LNgreyD8;
	Int32				LNgrey08;
	//----------------------------------------------------------------------*/
	Word32				Hex_35[16];

private:
	Bool32 Virginity;
	Word32 buf_H[8];
	Word32 buf_B[2550];
public:
	/*
	KRONROD_EXPORT(void)   grey_open (Word32 H, Word32 N);
	KRONROD_EXPORT(Word32) grey_to   (PWord8 pKogo);        //returns n_lines_ready
	KRONROD_EXPORT(void)   grey_from (PWord8 pKuda);
	KRONROD_EXPORT(Word32) grey_close (void);      //returns n_lines_ready
*/
public:
	Word32	grey_close(void);
	void	grey_from(PWord8 pKuda);
	Word32	grey_to(PWord8 pKogo);
	void	grey_open(Word32 H, Word32 N);
	CKronrodBinarizator();
	~CKronrodBinarizator();
protected:
	void mk_hist_WORD(PWord32 Hist_WORD, PWord8 p, Word32 n);
	Word32 mk_1235(void);
	void pr_MES_3(PInt8  text, Int32 n1, Int32 n2, Int32 n3);
	void pr_MES_2(PInt8  text, Int32 n1, Int32 n2);
	void pr_MES_1(PInt8  text, Int32 n1);
	void pr_MES(PInt8  text);
	void pr_ERR_3(PInt8  text, Int32 n1, Int32 n2, Int32 n3);
	void pr_ERR_2(PInt8  text, Int32 n1, Int32 n2);
	void pr_ERR_1(PInt8  text, Int32 n1);
	void pr_ERR(PInt8  text);
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
	void Korob_calc_BOX(Word32 j);
	void Korob_add(Word32 j);
	void Korobs_CLEAR_and_GLUE(void);
	void Korobs(void);
	void Korobs_init(void);
	void hist_MIN_MAX(void);
	void hist_glad_8_121(void);
	void hist_add(PWord8 p, Word32 N_Lines);
	void grey_PR_BYTE(Word32 ibeg, Word32 jbeg, Word32 value);
	void grey_quant_KOROB_PR_Factor(void);
	void grey_quant_KOROB_graph_Hist(void);
	void grey_quant_KOROB_draw_RAMKA(void);
	void grey_quant_KOROB_3x3_VERB(void);
	void grey_quant_KOROB_3x3(void);
	void grey_quant_KOROB_1x5work(void);
	void grey_quant_KOROB_1x3_MAX(void);
	void grey_quant_KOROB_1x1(void);
	void grey_quant_KOROB(void);
	void grey_to_monitor_MBIT(Word32 n_from);
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
