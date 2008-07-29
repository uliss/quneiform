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

#ifndef __RECDEFS_H
#define __RECDEFS_H


#include "globus.h"

#pragma pack(1)

#define  RD_UNRECOG_CHAR   '~'
/////////////////////////////////////////////////////////
// RecRaster - rectangular peace of black/white image
// Black pixel is 1
// Alignment is 8 bytes ( 64 bits )

#define  REC_MAX_RASTER_SIZE   4*1024  // 256*128
#define  REC_MAX_RASTER_SIZE_EX  256*128
#define  REC_MAX_VERS          16

#define  REC_GW_WORD8(a)  ( ( (a+63)/64 )*8)
#define  REC_GW_WORD16(a) ( ( (a+63)/64 )*4)
#define  REC_GW_WORD32(a) ( ( (a+63)/64 )*2)
#define  REC_GW_WORD64(a) ( ( (a+63)/64 )*1)

#define  RRBWr( recRasterRef ) ((( (recRasterRef) .lnPixWidth + 63) >> 6) << 3)
#define  RRBWp( recRasterPtr ) ((( (recRasterPtr)->lnPixWidth + 63) >> 6) << 3)


typedef struct tagRecRaster
   {
      Int32    lnPixWidth;
      Int32    lnPixHeight;
      Int32    lnRasterBufSize;				// maximum of raster length
      Word8    Raster[REC_MAX_RASTER_SIZE];	// image
   }  RecRaster;

typedef struct tagRecRasterEx
   {
      Int32    lnPixWidth;
      Int32    lnPixHeight;
      Int32    lnRasterBufSize;				// maximum of raster length
      Int32    lnLevelGray;
      Word8    Raster[REC_MAX_RASTER_SIZE_EX];	// image
   }  RecRasterEx;
/////////////////////////////////////////////////////////
// RecBmp - .............................................
// Black pixel is 1

typedef struct tagRecBmp
   {
      Rect16         Region;
      Word8  *   pBits;
      Int32          nOffsetX;   //this is offset of Region.left-bit
                                 //relatively 0-bit 0-byte of pBits
      Int32          nByteWidth;
      Int32          nResolutionX;
      Int32          nResolutionY;
      char           reserved[16];  // zero filled reserve
   }  RecBmp;


typedef struct tagRecAlt
   {
      Word8    Code;
      Word8    CodeExt;        // for unicode
      Word8    Method;         // recognize method
      Word8    Prob;
      Word16   Info;
   }  RecAlt;
typedef struct tagUniAlt
   {
      Word8    Code[4];
      Word8    Liga;        // for unicode
      Word8    Method;         // recognize method
      Word8    Prob;
      Word8    Charset;
      Word16   Info;
   }  UniAlt;

typedef struct tagClustAlt
   {
      Int16    nClust;         // cluster index; zero - structure is not initiated
      Word8    nDist;          // distance
      Word8    nReserved;
   }  ClustAlt;

typedef struct tagRecVersions
	{
	Int32  lnAltCnt;           // count of alternates
	Int32  lnAltMax;		   // maximum of alternates
	RecAlt Alt[REC_MAX_VERS];  // alternates array
	} RecVersions;
typedef struct tagUniVersions
	{
	Int32  lnAltCnt;           // count of alternates
	Int32  lnAltMax;		   // maximum of alternates
	UniAlt Alt[REC_MAX_VERS];  // alternates array
	} UniVersions;

typedef struct tagRecVector {
Point16 beg,end;
Int32 incline;   // 2048 * tangens
#define INCLINE_UNKNOWN	(-0x41414141)	  ///	      _______
Int32  len;  // -1 - It isn't vector;  metric Eucleede û xý + yý
Int32 Mx,My;
Int32 Cx,Cy;  // debug purposes
Int32 area;
Word8 Mn; // normalized moment
Word8 reserve[3];
#define MAX_VECT	64
#define NORMA_FACTOR	12
}  RecVector;

typedef struct tagRecData
	{
	Word32    lwStatus;

	RecRaster recRaster;       // basic raster. Can be not changed
	Word16    v3x5[16];        // normalizeted image 3x5
	Word32    lwCompCnt;       // number of components, 0 - not counted
	Rect16    rect;
  RecVector    vSticks[MAX_VECT];
  Int16     lwSticksCnt;
	} RecData;

typedef struct tagRecObject
	{
	RecData     recData;
	RecVersions recResults;
	Word32      lwStatusMethods; // indicator of used methods


	} RecObject;

// Styles of images
#define LS_HAND      1   // handprinted letters
#define LS_INDEX     2   // post-indexes
#define LS_PRINT     4   // printed lettres

#ifndef PPS_MAC
	#pragma pack()
#else
	#pragma align
#endif


#endif   // __RECDEFS_H



