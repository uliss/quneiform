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

#ifndef PPS_MAC
#pragma message(__FILE__)
#endif

#ifndef __LNSDEFS_H
#define __LNSDEFS_H

   #ifndef __GLOBUS_H
      #include "globus.h"
   #endif

   #include "linedefs.h"

typedef struct tagLnsFrag
{
   Int32  flags;  // misc info
#define LF_LINE   0x0001   // fragment decided in lns.dll to be line                              

#define LF_STICK_LEFT   0x0010 // there is stick at left side - may be letter
#define LF_STICK_RIGHT  0x0020 // there is stick at right side - may be letter
#define LF_STICK_TOP    0x0040 // there is stick at top side - may be letter
#define LF_STICK_BOTTOM 0x0080 // there is stick at top side - may be letter

#define LF_TEXT     0x0002 // fragment seems to be part of text (can be combined with LF_LINE)


   Int32  mass;   // count of filtered black pixels
   Rect16 rc;     // real image rect
   Point16 A,B;   // start and end - if ~LF_LINE - may be not linear, so...
   double Xc     ;// center of mass in real coords
   double Yc     ;
   double Phi    ;// angle of own coords
   double aveW   ;// average width   
   double mainMxx;// squared sum in own axes
   double mainMyy;// squared sum in own axes

//   double relMxx ;// squared sum by X axes	
//   double relMxy ;// squared sum by Y axes		
//   double relMyy ;// mixed sum	

   Int16 frag_handle;   // internal usage - link to own data; 0xffff - undefined
   char  reserved[22];
} LnsFrag;

typedef struct tagAdjacentLst
{
	Int16          ltiNext;       // list of adjacent lines (left to right,
	Int16          ltiPrev;       // top to bottom), -1 == None
	Int16          ltiFirst;
	Int16          ltiLast;
}  AdjacentLst;

typedef struct tagLnsCorner
{
   Int16          ltiStart;      // LTI index of perpendicular lines
   Int16          ltiEnd;        // joined with proper end of this line
}  LnsCorner;


typedef struct tagLineInfo
{
   //========= Ãåîìåòðè÷åñêîå ïîëîæåíèå =============================
   Point16       A;             // start
   Point16       B;             // end
   Point16       Ar;            // start at rotated coords
   Point16       Br;            // end at rotated coords
   Point16       Anew;          // start for sweep: if set LI_NOTWHOLE (real cords)
   Point16       Bnew;          // end for sweep: if set LI_NOTWHOLE (real cords)

   //========= Ñïåöèàëüíûå õàðàêòåðèñòèêè ==========================
	Word32         Flags;         // common use info
//âñå ôëàãè âûíåñåíû â LineDefs.h
/*
      #define LI_NOISE      0x00000001   // is a noise line
      #define LI_ATTACHED   0x00000002   // Line  confidently
                                         // attached to form element
      #define LI_SMARTSWEEP 0x00000004   // must smartly dissolve it from image
      #define LI_CRUDESWEEP 0x00000008   //
      #define LI_SWEEP (LI_SMARTSWEEP | LI_CRUDESWEEP)
      #define LI_DOT        0x00000010   //
      #define LI_COVERED    0x00000020   // always with LI_NOISE
*/
	/*****  Âûñòàâëÿþòñÿ âåðèôèêàòîðîì ëèíèé.  *****/
/*		// Ëèáî òîëüêî îäèí èç ñëåäóþùèõ, ëèáî íè îäíîãî. Åñëè íè îäíîãî, òî
		// ëèáî ëèíèÿ ñîìíèòåëüíà, ëèáî íå èññäåäîâàëàñü.
	#define LI_IsTrue       0x00000040 
			// ïðàâèëüíî âûäåëåííàÿ ëèíèÿ
	#define LI_IsFalse      0x00000080
			// íå ëèíèÿ
	#define LI_NOTWHOLE     0x00000100
			// Êðàåâûå çàõâàòû. Ðåàëüíàÿ ëèíèÿ êîðî÷å - îò Anew äî Bnew.
	#define LI_COMPLEX      0x00000200
			// Ñîñòàâíàÿ ëèíèÿ : íåñêîëüêî ðåàëüíûõ ëèíèé, èíîãäà åùå è îäíà
			// èëè íåñêîëüêî êîìïîíåíò ñâÿçíîñòè. Èíôîðìàöèÿ î âõîäÿùèõ â åå
			// ñîñòàâ ðåàëüíûõ ëèíèÿõ è êîìïîíåíòàõ îòïèñûâàåòñÿ â ñîâåðøåííî
			// íåçàâèñèìóþ ñòðóêòóðó.
	#define LI_Pointed      0x00001000
	#define LI_Doubt        0x00002000
	#define LI_Used         0x00004000  // Èñïîëüçîâàíà êàê ïîä÷åðêèâàíèå 
 #define LI_FRMT_Used    0x00008000  // Formatter used
*/
	/*****  Âûñòàâëÿþòñÿ âûäåëèòåëåì òàáëèö.  *****/
/*
	#define LI_IsAtTable    0x00000400
	#define LI_IsNotAtTable 0x00000800
*/
	/**********************************************/
   Int16          SegCnt;        // ÷èñëî ñåãìåíòîâ â ñîñòàâíîé ëèíèè, 0-íåèíèöèàëèçèðîâàíî
   Word8          Quality;       // 0-255; 255 == good line
   Word8          Thickness;     // ñðåäíÿÿ òîëùèíà ëèíèè/øòðèõà

   //========= Ãðóïïîâûå õàðàêòåðèñòèêè ============================
   AdjacentLst    Adj;           // ïîòåíöèàëüíîå ðàñøèðåíèå íà óðîâíå
   LnsCorner      Corner;        // ñòûêîâêà óãëîâ ñ ïåðïåíäèêóëÿðíûìè
   Rect32        XXContext;     // ñîãëàñîâàíèå ñ ïàðàëëåëüíûìè ëèíèÿìè
   Rect32        XYContext;     // ñîãëàñîâàíèå ñ ïåðïåíäèêóëÿðíûìè ëèíèÿìè

   //========= Ïðèâÿçêà ê âûäåëèòåëþ (äëÿ ïîñëåäóþùåãî ñíÿòèÿ,...) =====
   Int32          ExtrDllHnd;    // âíóòðåííèé íîìåð ëèíèè îò âûäåëèòåëÿ
   Word16         Extractor;     // áèáëèîòåêà, âûäåëèâøàÿ ëèíèþ
      #define LI_UNKNOWN   0
      #define LI_LNSDLL    1
      #define LI_DOTDLL    2
      #define LI_UNION     3      // union of some set of dot & some set of solid lines

   //========= Îïðåäåëåíèÿ, ñïåöèôè÷íûå äëÿ ïóíêòèðîâ ===============
   Int16          DotAveLen;     // Ñðåäíÿÿ äëèíà øòðèõà
   Int16          DotAveGap;     // Ñðåäíèé èíòåðâàë ì/øòðèõàìè

   //================================================================
   Int16          IndCover;      // valid only for LI_COVERED-lines
   Word32         pFrmLineAtom;  // link to form description
   Word8          __buf[32];      // Çàðåçåðâèðîâàíî, çàïîëíåíî 0
   Int32          TmpUsage;      // ïîëå äëÿ âðåìåííîãî èñïîëüçîâàíèÿ

}  LineInfo;


#define LINE_HAS_EXT( li )                         \
   ( (li).Adj.ltiFirst != (li).Adj.ltiLast) &&     \
   ( (li).Adj.ltiFirst != -1              ) &&     \
   ( (li).Adj.ltiLast  != -1              )

#define LINE_HOR_LEN( li )  ( (li).B.x - (li).A.x + 1 )
#define LINE_VER_LEN( li )  ( (li).B.y - (li).A.y + 1 )
#define LINE_LEN( li )     ( maxi( LINE_HOR_LEN(li), LINE_VER_LEN(li) ) )

//#define LI_START_JOINED    0x0001   // left or top
//#define LI_END_JOINED      0x0002   // right bottom


typedef struct tagLnsInfoArray
{
	LineInfo*      Lns;            // first line pointer
   Int32          Cnt;             // total lines count
//   Int32          SumLen;         // sum lines len (excluding noise)
//removed   Int32          NoiseCnt;       // count of noise lines
//   Int32          ExtCnt;         // count of extended lines
   char reserved[12];
}  LnsInfoArray;

typedef struct tagLCSetup
{                                    // for 300 dpi:
   Int32       NeglectedDelta;       // 5
   Int32       MaxCornerGap;         // 20
   Int32       MinExtLineFilling;    // 50 (in PERCENTS)
   Int32       MinHLen;              //
   Int32       MinVLen;              //
   // for all fields 0 - use default;
}  LCSetup;

#define LTI_DEF_VLEN       60         // default min length of line to be noise
#define LTI_DEF_HLEN       100

typedef struct tagLinesTotalInfo
{
   LnsInfoArray      Hor;
   LnsInfoArray      Ver;
   Int32             Skew1024;
   Point16          ImgSize;
   Point16          ImgResolution;
   LCSetup           LCS;
   Word8             __buf[32];
}  LinesTotalInfo;

#ifdef __cplusplus
inline void ltiGetNotNoise( LinesTotalInfo* plti,  // get 
                            int& hcnt, int& vcnt   // put
                          )
{
   int i;
   hcnt = plti->Hor.Cnt;
   for (i=0; i < plti->Hor.Cnt; i++)
   {  if (plti->Hor.Lns[i].Flags & LI_NOISE)
         hcnt--;
   }

   vcnt = plti->Ver.Cnt;
   for ( i=0; i < plti->Ver.Cnt; i++)
   {  if (plti->Ver.Lns[i].Flags & LI_NOISE)
         vcnt--;
   }
}
#endif
#endif
