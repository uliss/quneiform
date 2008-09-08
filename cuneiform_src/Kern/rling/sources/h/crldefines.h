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

# ifndef __CRLING_DEFINES_H_
# define __CRLING_DEFINES_H_
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define     RLING_MAX_IMAGE_NAME           256
#define     RLING_MAX_WORD_LENGHT          32       //see Speldefs.h
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ðàçìåð ìîçãîâ äëÿ ñëîâàðÿ
//#define SizeTables			0x001f0000  // AK 23.03.98
#define SizeTables	0x00210000	//	2162688 07.06.2002 E.P.
///////////////////////////////////////////////////////////////////////////////////////////////////////
#define   RLING_ERROR_CANT_OPEN_TABLE                2
#define   RLING_ERROR_ED_ALREADY_CHEKED              3
#define   RLING_ERROR_MEMORY_ERROR                   4
#define   RLING_ERROR_WRONG_ED_FILE                  5
#define   RLING_ERROR_CANT_OPEN_USER_DICTONARY       8
#define   RLING_ERROR_TOO_MANY_USER_DICTONARY        9
#define   RLING_ERROR_MEMORY_FAULT                   13
#define   RLING_ERROR_UNSUPPORTED_LANGUAGE           33
////////////////////////////////////////////////////////////////////////////////////
#define BOX_NUM				512;
#define BOX_NUM0			2048;
#define BOXHLTH	            (sizeof(BOX))	// the box header length
#define BOXSIZE             (BOXHLTH+32*4)	// the length of box
#define BOXBOUNDARY         (BOXSIZE-6)	// the boundary control
#define     RLING_RGBQUDSSIZE(a, b)        a += ( (0x0001 << b) * sizeof(RLINGRGBQUAD) )
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
# define    RLING_EXTENDBYTES(a,b)         (((a) + (b) - 1)/(b))*(b)
# define    RLING_EXTENDBITS(a,b)          (((a) + ((b)*8) - 1)/((b)*8))*(b)
# define    RLING_DIB_EXT                  4
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
# define    RLING_INCH_TO_METER(a)         ( ( (a) / 2.54 ) * 100 )
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//# define    RLING_STRING_LENGHT(a)         strlen(a)
//# define    RLING_STRING_COPY(a,b)         strcpy(a,b)
//# define    RLING_STRING_N_COPY(a,b,c)     strncpy(a,b,c)
//# define    RLING_STRING_CAT(a,b)          strcat(a,b)
//# define    RLING_STRING_COMPARE(a,b)      strcmp(a,b)
//# define    RLING_ABS(a)                   abs(a)
//# define    RLING_MEMSET(a,b,c)            memset(a,b,c)
//# define    RLING_MEMCOPY(a,b,c)           memcpy(a,b,c)
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if !defined DLL_PROCESS_ATTACH || !defined DLL_THREAD_ATTACH || !defined DLL_THREAD_DETACH || !defined DLL_PROCESS_DETACH
#define DLL_PROCESS_ATTACH 1
#define DLL_THREAD_ATTACH  2
#define DLL_THREAD_DETACH  3
#define DLL_PROCESS_DETACH 0
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef APIENTRY
# define APIENTRY     __stdcall
#endif

#include "nt_types.h"
/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if !defined BOOL
typedef int                 BOOL;
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if !defined DWORD
typedef unsigned long       DWORD;
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if !defined LPVOID
typedef void                *LPVOID;
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
 */
# endif //__CRLING_DEFINES_H_
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file
