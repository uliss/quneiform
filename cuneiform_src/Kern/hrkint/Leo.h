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

#ifndef __LEO_H
   #define __LEO_H

   #ifndef __GLOBUS_H
   #include "globus.h"
   #endif

#ifndef __LEODEFS_H
   #include "leodefs.h"
#endif

#ifndef __RECDEFS_H
   #include "recdefs.h"
#endif

#ifndef __MEMFUNC_H
   #include "memfunc.h"
#endif

#ifndef __SNPDEFS_H
   #include "snpdefs.h"
#endif

   #ifdef __LEO__
      #define LEO_FUNC  FUN_EXPO
   #else
      #define LEO_FUNC  FUN_IMPO
   #endif

   #ifdef __cplusplus
   extern "C" {
   #endif
		#ifdef PPS_MAC
			#pragma export on
		#endif

   LEO_FUNC(Bool32)  LEOInit( MemFunc* mem );
   LEO_FUNC(Bool32)  LEOInitPass2( void );
      // Start Initialization; TRUE - OK
      // for mem==NULL need used internal memory functions from DLL
      // New call - after close library without LEODone

   LEO_FUNC(Bool32)  LEOSetupPage(LeoPageSetup* ps);
   LEO_FUNC(Bool32)  LEOSetupField(LeoFieldSetup* fs);

   LEO_FUNC(Bool32)  LEORecogPrintChar(RecObject* obj  );
   LEO_FUNC(Bool32)  LEORecogChar(RecObject* obj  );
   LEO_FUNC(Bool32)  LEORecogInd(RecObject* obj  );

   LEO_FUNC(Bool32)  LEORecogCharDebug(RecObject* obj  ); 
//
   LEO_FUNC(Bool32)  LEORecogChar_expert(RecObject*  object);
   LEO_FUNC(Bool32)  LEORecogCharPRN_expert(RecObject*  object);
// container for Recrasters
   LEO_FUNC(Bool32)  LEORecogId_expert(Int32 id_page,RecVersions *exp);
   LEO_FUNC(Bool32)  LEORecogId_Char(Int32 id_page, RecVersions *resin, RecVersions *resout);
   LEO_FUNC(Bool32)  LEOValidId_Char(Int32 id_page, RecVersions *resin, RecVersions *resout);
   LEO_FUNC(Bool32)  LEORecogRestore_Char(RecVersions *resin, RecVersions *resout);
   LEO_FUNC(Word8)   LEOValidRestore_Char( RecVersions *resin,RecVersions *resout);
   LEO_FUNC(Bool32)  LEOSetValid(Int16 id_rast,Word8 code, Word8 valid,Word8 control);
   LEO_FUNC(Bool32)  LEODelFinal(Int16 id_rast);
   LEO_FUNC(Int16)   LEO_ContStore(RecRaster *r,Word8 let, Word8 nLns,Rect16 *rect,Word8 IsPrint,
		Word8	Prob, Word8 Valid, RecVersions *Res); 
   LEO_FUNC(Int32)   LEO_GetGlobalIncline(void);
   LEO_FUNC(Bool32)  LEOPushAlphabetType(Word8 alpha_type, Word8 isPrint);
   LEO_FUNC(Bool32)  LEO_ContRestoreObject( Int16       idr, RecObject *object, 
                                            Word8 *alpha, Word8 *isPrint);
//                                     id_page can be >0
// to be deleted after implementation of LEOSetupField(), 28.02.97 20:50, VP
   LEO_FUNC(Bool32) LEOSetAlphabet( char*     char_tbl_put_to   );
   LEO_FUNC(void)   LEOSetFont( Int32 typ_of_font);
                                   
   LEO_FUNC(void)   LEOCloseCont(void); 
   LEO_FUNC(Int32)  LEOOpenCont(void);
   LEO_FUNC(void)   LEODone(void);
   LEO_FUNC(void)   LEODonePass2(void);
   LEO_FUNC(Bool32) LEOFonRerecogCTB(char *CTBname);
   LEO_FUNC(Bool32) LEO_GetStoringMode(void);
   LEO_FUNC(void)   LEO_SetStoringMode(Bool32 Mode);

   LEO_FUNC(Bool32)  LEORecogHndMethod( RecObject*  object , int Method, int gra_type);
   LEO_FUNC(Bool32)  LEORecogPrnMethod( RecObject*  object , int Method, int gra_type);

    // Stop function. Call after LEOInit()
   LEO_FUNC(char *) LEOGetMetName(Int32 method, char *met_name); 
   LEO_FUNC(Int16)  LEOGetErr(void);
      // Err16.h defined codes or one of the next
         #define ER_LEO_NO_ERROR       0
         #define ER_LEO_NULL_OBJECT    1
         #define ER_LEO_SMALL_OBJECT   2
         #define ER_LEO_LARGE_OBJECT   3
         #define ER_LEO_NOINIT_R35     4
         #define ER_LEO_NOINIT_NET     5
         #define ER_LEO_NOINIT_TRE     6
         #define ER_LEO_NOINIT_IND     7
         #define ER_LEO_NOINIT_EVN     8
         #define ER_LEO_NOINIT_NCU     9
         #define ER_LEO_OPEN           10


   LEO_FUNC(Int32)  LEOGetCPU(void);
   LEO_FUNC(void)   LEOSetPlatform(Int32 Cpu);
   LEO_FUNC(void)   LEOSortVersProb(RecVersions *v);

   LEO_FUNC( void   )      LEORegisterSnpTree(
            SnpTreeNode* parent,         // parent Snp Node, may be NULL
            __SnpToolBox* p_snp_tools     // tools complect, may be NULL
                                 );
   LEO_FUNC( void   )      LEOIndRegisterSnpTree(
            SnpTreeNode* parent,         // parent Snp Node, may be NULL
            __SnpToolBox* p_snp_tools     // tools complect, may be NULL
                                 );
      // Registartion SubTree for Debug  (SNP)

   
   LEO_FUNC(void)    LEOSnpFullRecog(RecRaster *rec);
   
// sizes container
   LEO_FUNC(void)   LEO_ClearStat(void);
   LEO_FUNC(Int32)  LEO_AddStat(Int32 name,Int32 width,Int32 height,Int32 valid);
   LEO_FUNC(Int32)  LEO_AddStatLetter(Int32 name,Int32 width,Int32 height,Int32 valid);
   LEO_FUNC(Int32)  LEO_GetCommonStat(Int32 *sizes,Int32 *ocenka);
   LEO_FUNC(Int32)  LEO_GetLetStat(Int32 name,Int32 *sizes,Int32 *ocenka);


   LEO_FUNC(Int16) LEO_GetNoCutPoint(Word8 *RASTER,  Int16 WB, Word8 NWIDTH, Word8 NLENGTH);
   LEO_FUNC(Int32)   LEO_Str2FldNo(Int32  str_no);
   // convert string number to field number. -1 if bound error 
   LEO_FUNC(Bool32) LEO_StoreCollection(RecVersions *ver);
   // store ANSI(Windows) collection
   LEO_FUNC(void)  LEOFreeAlphabets(void);
		#ifdef PPS_MAC
         #pragma export off
		#endif

   #ifdef __cplusplus
            }
   #endif

#endif

