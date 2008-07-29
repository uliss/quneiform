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

#include "lnslang.h"

	#ifndef __FLTBUF_H
	#	include "fltbuf.h"
	#endif

   #ifndef __MEMOPS_H
   #  include "memops.h"
   #endif
   #ifndef __XMACROS_H
   #  include "xmacros.h"
   #endif
TFltBuf::TFltBuf( int width_dword )
	{
		x16 = NULL;
      if ( (Int32)32 * sizeof(Int32) * (Int32)width_dword  > (Int32)0xFFF0){
			// size of buffer > 64K
			return;
		};

      x16 = (Int32 HUGE*)(malloc( (Int32)32 * (Int32)sizeof(Int32) * (Int32)width_dword  ));
		if (x16 == NULL) {
			return;
		};
		widthDWord = width_dword;
		x8 		= x16 + 16*widthDWord;
		x4 		= x8  + 8 *widthDWord;
		x2 		= x4  + 4 *widthDWord;
		cur16 	= x2  + 2 *widthDWord;
		last16 	= cur16 + widthDWord;

		nCurLine = 0;

	};

void TFltBuf::destroy( void )
	{
		if (x16!=NULL) {
         free( (void*)x16 );
			x16 = NULL;
		};
	};

void TFltBuf::addLine( void* new_line )
	{
      memmove( (void*)(x16+widthDWord*nCurLine), new_line, widthDWord*4 );
		nCurLine++;
	};

/***********************/
#define  A_B_to_AB() {  memcpy(curAB, curA, wdw<<2);  CopyOR(curAB, curB, wdw); }
/***********************/

void TFltBuf::preBuffer( void )
   {
      int wdw = widthDWord;
      Int32* curA;
      Int32* curB;
      Int32* curAB;
      int i=0;

      curA = (Int32*)x16;
      curB = (Int32*)x16 + wdw;
      curAB = (Int32*)x8;
		for( i = 0; i < 8; i++ ){
         A_B_to_AB();
         curAB += wdw;
         curA  += wdw*2;
         curB  += wdw*2;
		};

      curA = (Int32*)x8;
      curB = (Int32*)x8 + wdw;
      curAB = (Int32*)x4;
		for( i = 0; i < 4; i++ ){
         A_B_to_AB();
         curAB += wdw;
         curA  += wdw*2;
         curB  += wdw*2;
		};

      curA = (Int32*)x4;
      curB = (Int32*)x4 + wdw;
      curAB = (Int32*)x2;
		for( i = 0; i < 2; i++ ){
         A_B_to_AB();
         curAB += wdw;
         curA  += wdw*2;
         curB  += wdw*2;
		};

      curA = (Int32*)x2;
      curB = (Int32*)x2 + wdw;
      curAB = (Int32*)cur16;
      A_B_to_AB();

		nCurLine = 0;
	};

void TFltBuf::updateByLine( void* new_line )
	{
      int wdw = widthDWord;
      Int32* curA;
      Int32* curB;
      Int32* curAB;
      Int32 n_cur = nCurLine;


		swap( last16, cur16 );

      curA = (Int32*)x16+wdw*n_cur;
      memmove( (void*)curA, new_line, wdw*4 );
      curB = (n_cur & (Int32)1) ?
             (Int32*)x16+wdw*(n_cur - 1):    // odd - get previous
             (Int32*)x16+wdw*(n_cur + 1);    // even - get next
		n_cur >>= 1;
      curAB = (Int32*)x8 + wdw*n_cur;
      A_B_to_AB();

      curA = (Int32*)x8+wdw*n_cur;
      curB = (n_cur & (Int32)1) ?
             (Int32*)x8+wdw*(n_cur - 1):    // odd - get previous
             (Int32*)x8+wdw*(n_cur + 1);    // even - get next
		n_cur >>= 1;
      curAB = (Int32*)x4 + wdw*n_cur;
      A_B_to_AB();

      curA = (Int32*)x4+wdw*n_cur;
      curB = (n_cur & (Int32)1) ?
             (Int32*)x4+wdw*(n_cur - 1):    // odd - get previous
             (Int32*)x4+wdw*(n_cur + 1);    // even - get next
		n_cur >>= 1;
      curAB = (Int32*)x2 + wdw*n_cur;
      A_B_to_AB();

      curA = (Int32*)x2+wdw*n_cur;
      curB = (n_cur & (Int32)1) ?
             (Int32*)x2+wdw*(n_cur - 1):    // odd - get previous
             (Int32*)x2+wdw*(n_cur + 1);    // even - get next
		n_cur >>= 1;
		assert( n_cur == 0 );
      curAB = (Int32*)cur16;           // + wdw*n_cur;
      A_B_to_AB();

		nCurLine++;
		if (nCurLine > 15)
			nCurLine = 0;
	};
