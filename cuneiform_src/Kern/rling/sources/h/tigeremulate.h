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

#ifndef __TIGER_EMULATION_H__
#define __TIGER_EMULATION_H__

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000

#ifndef PPS_MAC
   #pragma message(__FILE__)
#endif

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "nt_types.h"

#include "crldefines.h"

#define MEMSET(a,b,c)       memset(a,b,c)
#define MEMCHR(a,b,c)       memchr(a,b,c)
#define MEMCPY(a,b,c)       memcpy(a,b,c)

#define TG_STREAM  1
#define TB_STREAM  2
#define ED_STREAM  3
#define TX_STREAM  4
#define DB_STREAM  5
#define IM_STREAM  6
#define VC_STREAM  7
#define WK_STREAM  8

#define DIRSEP        '\\'

#ifndef LANG_RUSSIAN
#define LANG_RUSSIAN           3
#endif

INT    TE_open          (INT, CHAR *, INT, INT);    /* / stream, name, ... */
INT    TE_write         (INT, BYTE *, DWORD);    /* / stream, name, ... */
INT    TE_table_op      (INT, INT, INT, INT);      /* / table #, ...          */
LONG   TE_read          (INT, CHAR *, LONG);        /* / handle, addr, lth */
INT    TE_close         (INT);                      /* / handle            */
CHAR * TE_handle_fgets  ( INT handle, CHAR * s, INT len );
LONG   TE_file_length   (INT);

#define TGOPEN(a,b,c,d)        TE_open(a,b,c,d)
#define TBOPEN(a,b,c,d)        TE_table_op(a,b,c,d)
#define TGREAD(a,b,c)          TE_read(a,b,c)
#define TGWRITE(a,b,c)         TE_write(a,b,c)
#define TGCLOSE(a)             TE_close(a)
#define TGGETS(a,b,c)          TE_handle_fgets(a,b,c)
#define TGFILELTH(a)           TE_file_length(a)

#define BO_READ                O_RDONLY|O_BINARY
#define BO_READ_TEXT           O_RDONLY|O_TEXT


typedef  struct
{
   BYTE  let;
   LONG  tr1;
   LONG  tr2;
}
StructTree;
////////////////////////////////////////////////////////////////////////////////////
typedef struct elidx
{
	BYTE ltr;                  // the letter
	BYTE vnum;                 // the number of the classes
	WORD numel;                // the number of the first class
}
 indBOX;
////////////////////////////////////////////////////////////////////////////////////
typedef struct eltab
{
	BYTE ltr;                  // the letter
	BYTE fnt;                  // the font number
	WORD list;                 // next of same letter
	WORD bnd;                  // the bound of the typeface
	WORD vect[15];             // the vector of the letter
}
elmBOX;
///////////////////////////////////////////////////////////////////////////////////
struct mn_struc
{
	void *mnfirstbox; 		// address of the first box
	INT mncounter; 		// (was INT) number of living lines in the component
#define mnfree	mnfirstbox	// reference to next free main number
	INT mnupper;   		// upper bound of component
	INT mnlower; 			// lower bound of component
	INT mnboxcnt;  		// number of boxes in component
#define usual_box_count 20 	// heuristic of number of lines in a letter
#define great_box_count	200	// heuristic for number of boxes in a picture
	BYTE mnlines;                  // number of lines in the component
	BYTE mnbegs;           	// number of free line begins
	BYTE mnends;                   // number of free line ends
	BYTE mnflag;         	        // flag byte for main number
#define mnpicture 1		// component is a picture
	struct mn_struc *mnnext;	// address of next dead component
 };
typedef struct mn_struc MN;
/////////////////////////////////////////////////////////////////////////////////////
struct _Root;
typedef struct _Root ROOT;
typedef struct _Root *PROOT;
struct _Root
{
	INT   yRow;
	INT   xColumn;

	union
	{
		struct
		{
			WORD  wSegmentPtr;
			WORD  wLength;
		}
		u2;

		ROOT *pNext;
	}
	u1;

	INT   nHeight;
	INT   nWidth;
	BYTE  bType;
	BYTE  bReached;
	INT   nBlock;
	INT   nUserNum;
};
////////////////////////////////////////////////////////////////////////////////////
struct box_struct
{
	struct box_struct *boxnext; 	// chain address (zero if no next box)
	MN *		    boxmain; 	// component main number pointer
	WORD		    boxptr; 	// ptr to the empty place in the box
	INT		        boxleft; 	// left boundary for line envelope
	INT 		    boxright; 	// right boundary for line envelope
	INT 		    boxey; 	// row of line end+1 ( if line ends within
				//    box)
	INT             boxel; 	// length of the last segment (if line ends
				//    within box)
	INT	            boxex; 	// coordinate of last segment end (if line
				//    ends within box)
	BYTE 		    boxflag; 	// byte for box attributes flags
	BYTE		    boxwf; 	// working flag (for picture compress)
	WORD		    boxresw; 	// reserved word (for *4 arround)
};
typedef struct box_struct BOX;


#endif  //#ifndef __TIGER_EMULATION_H__
