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

/* ------------------------------------------------------------------ *\
|* 03-18-93 02:42pm, Mike                                             *|
|*                                                                    *|
|*             Spelling dictionary project. Release 2.0               *|
|*                         64-unit alphabet.                          *|
|*                                                                    *|
\* ------------------------------------------------------------------ */

#ifndef __SPELLDICT_H
#define __SPELLDICT_H

#if _MSC_VER > 1000
   #pragma once
#endif // _MSC_VER > 1000

 /* ----------------------------------------------------------------- *
 *  File contents:                                                    *
 *                                                                    *
 *    The first generation of the internal structures of the static   *
 *  dictionary.                                                       *
 *                                                                    *
 * ------------------------------------------------------------------ *
 *                                                                    *
 *  List of types:                                                    *
 *                                                                    *
 *    TVertP                                                          *
 *    TVertV                                                          *
 *    TPostfics                                                       *
 *    TAccount                                                        *
 *    TAddr                                                           *
 *    TTailVar                                                        *
 *    TShiftType                                                      *
 *    TTailSet                                                        *
 *    TDictState;                                                     *
 *    TBuildInfo;                                                     *
 *                                                                    *
 * ------------------------------------------------------------------ */

/* --- Dictionary constants. ---------------------------------------- */

#define STAT_DICT_SIGN  "CTCDict"

#ifdef S_COMMENTS
  #if sizeof( STAT_DICT_SIGN ) > 8
  #error Check header size for static dictionary.
  #endif
#endif

#define ABCSIZE  66             /* size of alfabet <= 64          ??? */
#define POS_ROOT 0x07           /* mask on 3 first buits in VERTV ??? */

#define MAX_WORD_SIZE 32        /* Maximal lth of word <-> tree depth */
#define MAXLEVEL      2         /* Depth of positional subtree        */

/* --- Key type for search in the dictionary. ----------------------- */

#define KEYTYPE BYTE
#define NATURAL WORD

/* --- Positional tree. --------------------------------------------- */

typedef struct  {
    WORD    cont:1;      /*  20 bits - addr of continuation       */
    WORD  noterm:1;      /*  vert isn't terminal in tree          */
    WORD   exist:1;      /*  vertex exists                        */
    WORD  shift0:5;      /*  first 5 bits & next 8 + 8 bits       */
    WORD  shift1:8;      /*    in 21 bits relative address        */
    WORD  shift2:8;      /*                                       */
} TVertP;

typedef TVertP  * PTVertP;


/* --- Pointer tree. ------------------------------------------------ */

typedef struct  {
    WORD cont:1;         /*  if == 1 then this vertex is          */
                         /*  followed by the continuation         */
    WORD noterm:1;       /*  vert isn't terminal in tree          */
    WORD key:6;          /*  letter no.: 0--63                    */
} TVertV;

typedef TVertV  * PTVertV;


/* --- Continuations of vertex. ------------------------------------- */

typedef struct  {
    WORD cont:1;         /*  continuation exists                  */
    WORD tail:1;         /*  =1 (the cont is for tail/accnt)      */
    WORD accnt:1;        /*  =0 - tail, =1 - accnt                */
    WORD enter0:5;       /*  tail kind number - first 5 bits      */
    WORD enter1:8;       /*  tail kind number - next  8 bits      */
} TPostfics;

typedef TPostfics  * PTPostfics;

typedef struct  {
    WORD cont:1;         /*  continuation exists                  */
    WORD tail:1;         /*  =1 (the cont is for tail/accnt)      */
    WORD accnt:1;        /*  =0 - tail, =1 - accnt                */
    WORD wrdterm:1;      /*  if == 1 - vertex may be end of word  */
    WORD spare:1;        /*  spare                                */
    WORD freq :3;        /*  frequency of word occurence          */
} TAccount;

typedef TAccount  * PTAccount;

typedef struct  {
    WORD cont:1;         /*  continuation exists                  */
    WORD tail:1;         /*  = 0 (the cont is for reference)      */
    WORD lth:1;          /*  lth of shift (bytes): 0->1, 1->2/3   */
    WORD shift0:5;       /*  first 5 bits or 1-byte shift         */
    WORD lth2:1;         /*  lth of long shift: = 0 - 2 bytes     */
    WORD shift2:7;       /*  next 7 bits of shift (lth=1,lth2=0)  */
    WORD shift3:8;       /*  next 8 bits of shift (lth=1,lth2=1)  */
} TAddr;

typedef TAddr  * PTAddr;


/* --- Tail string unit structure. ---------------------------------- */

typedef struct {
    WORD      ch:6;     /* character of the tail                  */
    WORD        :1;     /* not used                               */
    WORD tailend:1;     /* flag of end of the tail                */
} TTailSet;

typedef TTailSet  * PTTailSet;


/* --- Tail rules. -------------------------------------------------- */

typedef struct {
    WORD  tablenum;         /* tail table number                      */
    WORD  maxtaillth;       /* max lth of tail in the table           */
    DWORD tailmask;         /* mask of tail set variant               */
} TTailVar;

typedef TTailVar  * PTTailVar;


typedef DWORD               TShiftType;
typedef TShiftType  * PTShiftType;


/* --- Static dictionary control structure type. -------------------- */

struct dict_state {               /* declaration for compatibility... */
    BYTE  * root;           /* pointer to dictionary tree       */
    BYTE  * tailset_root;   /* pointer to tail set allocation   */
    PTTailVar     vartable;       /* table of "tail rules"            */
    PTShiftType   table;          /* table of shifts to "tail strings"*/
    DWORD         size;           /* size of used memory              */
    WORD          abcSize;        /* size of alphabet                 */
};

typedef struct dict_state   TDictState;
typedef TDictState  * PTDictState;

/* ------ Static dictionary header mask stucture. ------------------- */

typedef struct {

    CHAR sign        [8];         /* "CTCDict"                        */
    CHAR cpuType     [8];         /* "Mac" or "Intel"                 */
    CHAR language    [8];         /* "FRA"/"RUS"/"ENG"/etc.           */
    CHAR version     [8];         /* "01.00"                          */
    CHAR treeLength  [8];         /* RADIX 10 - in bytes              */
    CHAR tailsLength [8];         /* RADIX 10 - in bytes              */
    CHAR rulesLength [8];         /* RADIX 10 - in bytes              */
    CHAR hushLength  [8];         /* RADIX 10 - in bytes              */
    CHAR abcSize     [8];         /* size of the current alphabet     */
    CHAR abcUpper   [64];         /* string of uppercase letters      */
    CHAR abcLower   [64];         /* string of lowercase letters      */

} TDictHeaderMask;

typedef TDictHeaderMask  * PTDictHeaderMask;

/* ------ Dictionary builder information stucture. ------------------ */

typedef struct {                   /* -- information about vertex --- */
    WORD key;                      /* key of vertex                   */
    WORD levNum;                   /* absolute level number           */
    WORD wordTerm;                 /* vertex is terminal in a word    */
    WORD treeTerm;                 /* vertex is terminal in the tree  */
    WORD levTerm;                  /* vertex is most right in level   */
    WORD tailsNum;                 /* number of vertex's tails        */
    WORD refLth;                   /* length of reference to brother  */
    WORD sonsNum;                  /* number of vertex's sons         */
} TBuildInfo;

typedef TBuildInfo * PTBuildInfo;

/* ------------------------------------------------------------------ */
#endif
