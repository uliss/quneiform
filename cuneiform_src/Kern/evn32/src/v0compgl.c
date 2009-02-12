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


#include "cttypes.h"
#include "struct.h"
//#include "func.h"
#include "v1comp.h"

//      Common variables


//      Internal functions
static void make_seg_line (Word8* raster, Int16 width);

//      Internal variables
static BWSS *bp,*bpe;
static Word8 extrcomp_flag;
#define SEG_DELIM -0x7000

BWSS *locomp_seglist(Word8* raster, BWSS *bwsp, BWSS *bwe, Int16 height, Int16 width)
{
 bwsp->b = 0; bwsp->w = SEG_DELIM; bwsp++;
 bp = bwsp;  bpe = bwe;extrcomp_flag = 0;
 while (height--)  { make_seg_line (raster,width); raster+=width; }
 return bp;
}

BWSS *extrcomp_seglist(Word8* raster, BWSS *bwsp, BWSS *bwe, Int16 width)
{
 bp = bwsp;  bpe = bwe; extrcomp_flag = 1;
 make_seg_line(raster,width);
 return bp;
}

#define nextw goto after_white
#define nextb goto after_black
#define commw goto comm_after_white
#define commb goto comm_after_black
#define setp(b,w) *((int*)p)=(w<<16)|b;
#define setpo(b,w) p->b=b;p->w=w;
static void make_seg_line(Word8* raster, Int16 width)
{
 BWSS *p = bp;
 Word8 b;

 p->w = p->b = 0;
//      After black bit actions

after_black:
 while (width--)
 {
   if(p >= bpe) return; // bound control
   switch (b = *(raster++))
   {
    case 0x00:  p->w=8;                                 nextw;  //w8
    case 0x01:  (p++)->w=7; p->b=1;                     nextb;  //w7b1
    case 0x02:  (p++)->w=6; setp(1,1)                   nextw;  //w6b1w1
    case 0x03:  (p++)->w=6; p->b=2;                     nextb;  //w6b2
    case 0x04:  (p++)->w=5; setp(1,2);                  nextw;  //w5b1w2
    case 0x06:  (p++)->w=5; setp(2,1);             nextw;  //w5b2w1
    case 0x07:  (p++)->w=5; p->b=3;                     nextb;  //w5b3
    case 0x08:  (p++)->w=4; setp(1,3);             nextw;  //w4b1w3
    case 0x0c:  (p++)->w=4; setp(2,2);             nextw;  //w4b2w2
    case 0x0e:  (p++)->w=4; setp(3,1);             nextw;  //w4b3w1
    case 0x0f:  (p++)->w=4; p->b=4;                     nextb;  //w4b4

    case 0x05:
    case 0x09:
    case 0x0a:                  //w4...
    case 0x0b:
    case 0x0d:
                p->w=4; commw;


    case 0x10:  (p++)->w=3; setp(1,4);             nextw;  //w3b1w4
    case 0x18:  (p++)->w=3; setp(2,3);             nextw;  //w3b2w3
    case 0x1c:  (p++)->w=3; setp(3,2);             nextw;  //w3b3w2
    case 0x1e:  (p++)->w=3; setp(4,1);             nextw;  //w3b4w1
    case 0x1f:  (p++)->w=3; p->b=5;                     nextb;  //w3b5

    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:                  //w3b1...
    case 0x17:
    case 0x19:
    case 0x1a:
    case 0x1b:
    case 0x1d:
                (p++)->w=3; p->b=1; commb;


    case 0x20:  (p++)->w=2; setp(1,5);             nextw;  //w2b1w5

    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
    case 0x25:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x29:          //w2b1w1...
    case 0x2a:
    case 0x2b:
    case 0x2c:
    case 0x2d:
    case 0x2e:

    case 0x2f:
                (p++)->w=2; setp(1,1); commw;

    case 0x30:  (p++)->w=2; setp(2,4);             nextw;  //w2b2w4
    case 0x38:  (p++)->w=2; setp(3,3);             nextw;  //w2b3w3
    case 0x3c:  (p++)->w=2; setp(4,2);             nextw;  //w2b4w2
    case 0x3e:  (p++)->w=2; setp(5,1);             nextw;  //w2b5w1
    case 0x3f:  (p++)->w=2; p->b=6;                     nextb;  //w2b6

    case 0x31:
    case 0x32:
    case 0x33:
    case 0x34:
    case 0x35:
    case 0x36:
    case 0x37:          //w2b2...
    case 0x39:
    case 0x3a:
    case 0x3b:
    case 0x3d:
                (p++)->w=2; p->b=2; commb;

    case 0x40:  (p++)->w=1; setp(1,6);             nextw;  //w1b1w6

    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x46:          //w1b1w2...
    case 0x47:
    case 0x48:
    case 0x49:
    case 0x4a:
    case 0x4b:
    case 0x4c:
    case 0x4d:
    case 0x4e:
    case 0x4f:
                (p++)->w=1; setp(1,2); commw;

    case 0x50:
    case 0x51:
    case 0x52:
    case 0x53:
    case 0x54:
    case 0x55:
    case 0x56:
    case 0x57:          //w1b1w1b1...
    case 0x58:
    case 0x59:
    case 0x5a:
    case 0x5b:
    case 0x5c:
    case 0x5d:
    case 0x5e:
    case 0x5f:
                (p++)->w=1; p->b=1; (p++)->w=1; p->b=1; commb;

    case 0x60:  (p++)->w=1; setp(2,5);             nextw;  //w1b2w5

    case 0x61:
    case 0x62:
    case 0x63:
    case 0x64:
    case 0x65:
    case 0x66:
    case 0x67:
    case 0x68:
    case 0x69:          //w1b2w1...
    case 0x6a:
    case 0x6b:
    case 0x6c:
    case 0x6d:
    case 0x6e:
    case 0x6f:
                (p++)->w=1; setp(2,1); commw;

    case 0x70:  (p++)->w=1; setp(3,4);             nextw;  //w1b3w4
    case 0x78:  (p++)->w=1; setp(4,3);             nextw;  //w1b4w3
    case 0x7c:  (p++)->w=1; setp(5,2);             nextw;  //w1b5w2
    case 0x7e:  (p++)->w=1; setp(6,1);             nextw;  //w1b6w1
    case 0x7f:  (p++)->w=1; p->b=7;                     nextb;  //w1b7

    case 0x71:
    case 0x72:
    case 0x73:
    case 0x74:
    case 0x75:
    case 0x76:          //w1b3...
    case 0x77:
    case 0x79:
    case 0x7a:
    case 0x7b:
    case 0x7d:
                (p++)->w=1; p->b=3; commb;

    case 0x80:  p->b+=1; p->w=7;                        nextw;  //b1w7
    case 0x81:  p->b+=1; (p++)->w=6; p->b=1;            nextb;  //b1w6b1
    case 0x83:  p->b+=1; (p++)->w=5; p->b=2;            nextb;  //b1w5b2
    case 0x87:  p->b+=1; (p++)->w=4; p->b=3;            nextb;  //b1w4b3
    case 0x8f:  p->b+=1; (p++)->w=3; p->b=4;            nextb;  //b1w3b4

    case 0x82:
    case 0x84:
    case 0x85:
    case 0x86:
    case 0x88:
    case 0x89:
    case 0x8a:          //b1w3...
    case 0x8b:
    case 0x8c:
    case 0x8d:
    case 0x8e:
                p->b+=1; p->w=3; commw;

    case 0x9f:  p->b+=1; (p++)->w=2; p->b=5;            nextb;  //b1w2b5

    case 0x90:
    case 0x91:
    case 0x92:
    case 0x93:
    case 0x94:
    case 0x95:
    case 0x96:          //b1w2b1...
    case 0x97:
    case 0x98:
    case 0x99:
    case 0x9a:
    case 0x9b:
    case 0x9c:
    case 0x9d:
    case 0x9e:
                p->b+=1; (p++)->w=2; p->b=1; commb;

    case 0xa0:
    case 0xa1:
    case 0xa2:
    case 0xa3:
    case 0xa4:
    case 0xa5:
    case 0xa6:
    case 0xa7:
    case 0xa8:          //b1w1b1w1...
    case 0xa9:
    case 0xaa:
    case 0xab:
    case 0xac:
    case 0xad:
    case 0xae:
    case 0xaf:
                p->b+=1; (p++)->w=1; setp(1,1); commw;

    case 0xbf:  p->b+=1; (p++)->w=1; setp(6,0);    nextb;  //b1w1b6

    case 0xb0:
    case 0xb1:
    case 0xb2:
    case 0xb3:
    case 0xb4:
    case 0xb5:
    case 0xb6:
    case 0xb7:          //b1w1b2...
    case 0xb8:
    case 0xb9:
    case 0xba:
    case 0xbb:
    case 0xbc:
    case 0xbd:
    case 0xbe:
                p->b+=1; (p++)->w=1; p->b=2; commb;

    case 0xc0:  p->b+=2; p->w=6;                        nextw;  //b2w6
    case 0xc1:  p->b+=2; (p++)->w=5; setp(1,0);    nextb;  //b2w5b1
    case 0xc3:  p->b+=2; (p++)->w=4; setp(2,0);    nextb;  //b2w4b2
    case 0xc7:  p->b+=2; (p++)->w=3; setp(3,0);    nextb;  //b2w3b3
    case 0xcf:  p->b+=2; (p++)->w=2; setp(4,0);    nextb;  //b2w2b4

    case 0xc2:
    case 0xc4:
    case 0xc5:
    case 0xc6:
    case 0xc8:
    case 0xc9:          //b2w2...
    case 0xca:
    case 0xcb:
    case 0xcc:
    case 0xcd:
    case 0xce:
                p->b+=2; p->w=2; commw;

    case 0xdf:  p->b+=2; (p++)->w=1; setp(5,0);    nextb;  //b2w1b5

    case 0xd0:
    case 0xd1:
    case 0xd2:
    case 0xd3:
    case 0xd4:
    case 0xd5:
    case 0xd6:
    case 0xd7:
    case 0xd8:          // b2w1b1...
    case 0xd9:
    case 0xda:
    case 0xdb:
    case 0xdc:
    case 0xdd:
    case 0xde:
                p->b+=2; (p++)->w=1; p->b=1; commb;

    case 0xe0:  p->b+=3; p->w=5;                        nextw;  //b3w5
    case 0xe1:  p->b+=3; (p++)->w=4; setp(1,0);    nextb;  //b3w4b1
    case 0xe3:  p->b+=3; (p++)->w=3; setp(2,0);    nextb;  //b3w3b2
    case 0xe7:  p->b+=3; (p++)->w=2; setp(3,0);    nextb;  //b3w2b3
    case 0xef:  p->b+=3; (p++)->w=1; setp(4,0);    nextb;  //b3w1b4

    case 0xe2:
    case 0xe4:
    case 0xe5:
    case 0xe6:
    case 0xe8:          //b3w1...
    case 0xe9:
    case 0xea:
    case 0xeb:
    case 0xec:
    case 0xed:
    case 0xee:
                p->b+=3; p->w=1; commw;

    case 0xf0:  p->b+=4; p->w=4;                        nextw;  //b4w4
    case 0xf1:  p->b+=4; (p++)->w=3; setp(1,0);    nextb;  //b4w3b1
    case 0xf3:  p->b+=4; (p++)->w=2; setp(2,0);    nextb;  //b4w2b2
    case 0xf7:  p->b+=4; (p++)->w=1; setp(3,0);    nextb;  //b4w1b3
    case 0xf8:  p->b+=5; p->w=3;                        nextw;  //b5w3
    case 0xf9:  p->b+=5; (p++)->w=2; setp(1,0);    nextb;  //b5w2b1
    case 0xfb:  p->b+=5; (p++)->w=1; setp(2,0);    nextb;  //b5w1b2
    case 0xfc:  p->b+=6; p->w=2;                        nextw;  //b6w2
    case 0xfd:  p->b+=6; (p++)->w=1; setp(1,0);    nextb;  //b6w1b1
    case 0xfe:  p->b+=7; p->w=1;                        nextw;  //b7w1
    case 0xff:  p->b+=8;                                nextb;  //b8

    case 0xf2:
    case 0xf4:
    case 0xf5:          //b4...
    case 0xf6:
    case 0xfa:
                p->b+=4; commb;

   }    // end of switch bracket
  }
 p->w = SEG_DELIM; bp = ++p; return;

//      After white bit actions

after_white:
 while (width--)
  {
   if(p >= bpe) return; // bound control
   switch (b = *(raster++))
   {
    case 0x00:  p->w+=8;                                nextw;  //w8
    case 0x01:  (p++)->w+=7; p->b=1;                    nextb;  //w7b1
    case 0x02:  (p++)->w+=6; setp(1,1);            nextw;  //w6b1w1
    case 0x03:  (p++)->w+=6; p->b=2;                    nextb;  //w6b2
    case 0x04:  (p++)->w+=5; setp(1,2);            nextw;  //w5b1w2
    case 0x06:  (p++)->w+=5; setp(2,1);            nextw;  //w5b2w1
    case 0x07:  (p++)->w+=5; p->b=3;                    nextb;  //w5b3
    case 0x08:  (p++)->w+=4; setp(1,3);            nextw;  //w4b1w3
    case 0x0c:  (p++)->w+=4; setp(2,2);            nextw;  //w4b2w2
    case 0x0e:  (p++)->w+=4; setp(3,1);            nextw;  //w4b3w1
    case 0x0f:  (p++)->w+=4; p->b=4;                    nextb;  //w4b4

    case 0x05:
    case 0x09:
    case 0x0a:                  //w4...
    case 0x0b:
    case 0x0d:
                p->w+=4; commw;


    case 0x10:  (p++)->w+=3; setp(1,4);            nextw;  //w3b1w4
    case 0x18:  (p++)->w+=3; setp(2,3);            nextw;  //w3b2w3
    case 0x1c:  (p++)->w+=3; setp(3,2);            nextw;  //w3b3w2
    case 0x1e:  (p++)->w+=3; setp(4,1);            nextw;  //w3b4w1
    case 0x1f:  (p++)->w+=3; p->b=5;                    nextb;  //w3b5

    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:                  //w3b1...
    case 0x17:
    case 0x19:
    case 0x1a:
    case 0x1b:
    case 0x1d:
                (p++)->w+=3; p->b=1; commb;


    case 0x20:  (p++)->w+=2; setp(1,5);            nextw;  //w2b1w5

    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
    case 0x25:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x29:          //w2b1w1...
    case 0x2a:
    case 0x2b:
    case 0x2c:
    case 0x2d:
    case 0x2e:
    case 0x2f:
                (p++)->w+=2; setp(1,1); commw;

    case 0x30:  (p++)->w+=2; setp(2,4);            nextw;  //w2b2w4
    case 0x38:  (p++)->w+=2; setp(3,3);            nextw;  //w2b3w3
    case 0x3c:  (p++)->w+=2; setp(4,2);            nextw;  //w2b4w2
    case 0x3e:  (p++)->w+=2; setp(5,1);            nextw;  //w2b5w1
    case 0x3f:  (p++)->w+=2; p->b=6;                    nextb;  //w2b6

    case 0x31:
    case 0x32:
    case 0x33:
    case 0x34:
    case 0x35:
    case 0x36:
    case 0x37:          //w2b2...
    case 0x39:
    case 0x3a:
    case 0x3b:
    case 0x3d:
                (p++)->w+=2; p->b=2; commb;

    case 0x40:  (p++)->w+=1; setp(1,6);            nextw;  //w1b1w6

    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x46:          //w1b1w2...
    case 0x47:
    case 0x48:
    case 0x49:
    case 0x4a:
    case 0x4b:
    case 0x4c:
    case 0x4d:
    case 0x4e:
    case 0x4f:
                (p++)->w+=1; setp(1,2); commw;

    case 0x50:
    case 0x51:
    case 0x52:
    case 0x53:
    case 0x54:
    case 0x55:
    case 0x56:
    case 0x57:          //w1b1w1b1...
    case 0x58:
    case 0x59:
    case 0x5a:
    case 0x5b:
    case 0x5c:
    case 0x5d:
    case 0x5e:
    case 0x5f:
                (p++)->w+=1; p->b=1; (p++)->w=1; p->b=1; commb;

    case 0x60:  (p++)->w+=1; setp(2,5);            nextw;  //w1b2w5

    case 0x61:
    case 0x62:
    case 0x63:
    case 0x64:
    case 0x65:
    case 0x66:
    case 0x67:
    case 0x68:
    case 0x69:          //w1b2w1...
    case 0x6a:
    case 0x6b:
    case 0x6c:
    case 0x6d:
    case 0x6e:
    case 0x6f:
                (p++)->w+=1; setp(2,1); commw;

    case 0x70:  (p++)->w+=1; setp(3,4);            nextw;  //w1b3w4
    case 0x78:  (p++)->w+=1; setp(4,3);            nextw;  //w1b4w3
    case 0x7c:  (p++)->w+=1; setp(5,2);            nextw;  //w1b5w2
    case 0x7e:  (p++)->w+=1; setp(6,1);            nextw;  //w1b6w1
    case 0x7f:  (p++)->w+=1; p->b=7;                    nextb;  //w1b7

    case 0x71:
    case 0x72:
    case 0x73:
    case 0x74:
    case 0x75:
    case 0x76:          //w1b3...
    case 0x77:
    case 0x79:
    case 0x7a:
    case 0x7b:
    case 0x7d:
                (p++)->w+=1; p->b=3; commb;

    case 0x80:  (++p)->b=1; p->w=7;                     nextw;  //b1w7
    case 0x81:  (++p)->b=1; (p++)->w=6; p->b=1;         nextb;  //b1w6b1
    case 0x83:  (++p)->b=1; (p++)->w=5; p->b=2;         nextb;  //b1w5b2
    case 0x87:  (++p)->b=1; (p++)->w=4; p->b=3;         nextb;  //b1w4b3
    case 0x8f:  (++p)->b=1; (p++)->w=3; p->b=4;         nextb;  //b1w3b4

    case 0x82:
    case 0x84:
    case 0x85:
    case 0x86:
    case 0x88:
    case 0x89:
    case 0x8a:          //b1w3...
    case 0x8b:
    case 0x8c:
    case 0x8d:
    case 0x8e:
                (++p)->b=1; p->w=3; commw;

    case 0x9f:  (++p)->b=1; (p++)->w=2; p->b=5;         nextb;  //b1w2b5

    case 0x90:
    case 0x91:
    case 0x92:
    case 0x93:
    case 0x94:
    case 0x95:
    case 0x96:          //b1w2b1...
    case 0x97:
    case 0x98:
    case 0x99:
    case 0x9a:
    case 0x9b:
    case 0x9c:
    case 0x9d:
    case 0x9e:
                (++p)->b=1; (p++)->w=2; p->b=1; commb;

    case 0xa0:
    case 0xa1:
    case 0xa2:
    case 0xa3:
    case 0xa4:
    case 0xa5:
    case 0xa6:
    case 0xa7:
    case 0xa8:          //b1w1b1w1...
    case 0xa9:
    case 0xaa:
    case 0xab:
    case 0xac:
    case 0xad:
    case 0xae:
    case 0xaf:
                (++p)->b=1; (p++)->w=1; setp(1,1); commw;

    case 0xbf:  (++p)->b=1; (p++)->w=1; setp(6,0); nextb;  //b1w1b6

    case 0xb0:
    case 0xb1:
    case 0xb2:
    case 0xb3:
    case 0xb4:
    case 0xb5:
    case 0xb6:
    case 0xb7:          //b1w1b2...
    case 0xb8:
    case 0xb9:
    case 0xba:
    case 0xbb:
    case 0xbc:
    case 0xbd:
    case 0xbe:
                (++p)->b=1; (p++)->w=1; p->b=2; commb;

    case 0xc0:  (++p)->b=2; p->w=6;                     nextw;  //b2w6
    case 0xc1:  (++p)->b=2; (p++)->w=5; setp(1,0); nextb;  //b2w5b1
    case 0xc3:  (++p)->b=2; (p++)->w=4; setp(2,0); nextb;  //b2w4b2
    case 0xc7:  (++p)->b=2; (p++)->w=3; setp(3,0); nextb;  //b2w3b3
    case 0xcf:  (++p)->b=2; (p++)->w=2; setp(4,0); nextb;  //b2w2b4

    case 0xc2:
    case 0xc4:
    case 0xc5:
    case 0xc6:
    case 0xc8:
    case 0xc9:          //b2w2...
    case 0xca:
    case 0xcb:
    case 0xcc:
    case 0xcd:
    case 0xce:
                (++p)->b=2; p->w=2; commw;

    case 0xdf:  (++p)->b=2; (p++)->w=1; setp(5,0); nextb;  //b2w1b5

    case 0xd0:
    case 0xd1:
    case 0xd2:
    case 0xd3:
    case 0xd4:
    case 0xd5:
    case 0xd6:
    case 0xd7:
    case 0xd8:          // b2w1b1...
    case 0xd9:
    case 0xda:
    case 0xdb:
    case 0xdc:
    case 0xdd:
    case 0xde:
                (++p)->b=2; (p++)->w=1; p->b=1; commb;

    case 0xe0:  (++p)->b=3; p->w=5;                     nextw;  //b3w5
    case 0xe1:  (++p)->b=3; (p++)->w=4; setp(1,0); nextb;  //b3w4b1
    case 0xe3:  (++p)->b=3; (p++)->w=3; setp(2,0); nextb;  //b3w3b2
    case 0xe7:  (++p)->b=3; (p++)->w=2; setp(3,0); nextb;  //b3w2b3
    case 0xef:  (++p)->b=3; (p++)->w=1; setp(4,0); nextb;  //b3w1b4

    case 0xe2:
    case 0xe4:
    case 0xe5:
    case 0xe6:
    case 0xe8:          //b3w1...
    case 0xe9:
    case 0xea:
    case 0xeb:
    case 0xec:
    case 0xed:
    case 0xee:
                (++p)->b=3; p->w=1; commw;

    case 0xf0:  (++p)->b=4; p->w=4;                     nextw;  //b4w4
    case 0xf1:  (++p)->b=4; (p++)->w=3; setp(1,0); nextb;  //b4w3b1
    case 0xf3:  (++p)->b=4; (p++)->w=2; setp(2,0); nextb;  //b4w2b2
    case 0xf7:  (++p)->b=4; (p++)->w=1; setp(3,0); nextb;  //b4w1b3
    case 0xf8:  (++p)->b=5; p->w=3;                     nextw;  //b5w3
    case 0xf9:  (++p)->b=5; (p++)->w=2; setp(1,0); nextb;  //b5w2b1
    case 0xfb:  (++p)->b=5; (p++)->w=1; setp(2,0); nextb;  //b5w1b2
    case 0xfc:  (++p)->b=6; p->w=2;                     nextw;  //b6w2
    case 0xfd:  (++p)->b=6; (p++)->w=1; setp(1,0); nextb;  //b6w1b1
    case 0xfe:  (++p)->b=7; p->w=1;                     nextw;  //b7w1
    case 0xff:  (++p)->b=8;                             nextb;  //b8

    case 0xf2:
    case 0xf4:
    case 0xf5:          //b4...
    case 0xf6:
    case 0xfa:
                (++p)->b=4; commb;

   }   // end of switch bracket
  }
 p->w = SEG_DELIM; bp = ++p; return;

//      Common actions with half of the byte

//      After black pixel

comm_after_black:
 if(p >= bpe) return; // bound control
 switch (b & 0x0f)
  {
   case 0x00:   p->w=4;                                 nextw;  //w4
   case 0x01:   (p++)->w=3; p->b=1;                     nextb;  //w3b1
   case 0x02:   (p++)->w=2; setp(1,1);             nextw;  //w2b1w1
   case 0x03:   (p++)->w=2; p->b=2;                     nextb;  //w2b2
   case 0x04:   (p++)->w=1; setp(1,2);             nextw;  //w1b1w2
   case 0x05:   (p++)->w=1; p->b=1; (p++)->w=1; p->b=1; nextb;  //w1b1w1b1
   case 0x06:   (p++)->w=1; setp(2,1);             nextw;  //w1b2w1
   case 0x07:   (p++)->w=1; p->b=3;                     nextb;  //w1b3
   case 0x08:   p->b+=1; p->w=3;                        nextw;  //b1w3
   case 0x09:   p->b+=1; (p++)->w=2; p->b=1;            nextb;  //b1w2b1
   case 0x0a:   p->b+=1; (p++)->w=1; setp(1,1);    nextw;  //b1w1b1w1
   case 0x0b:   p->b+=1; (p++)->w=1; p->b=2;            nextb;  //b1w1b2
   case 0x0c:   p->b+=2; p->w=2;                        nextw;  //b2w2
   case 0x0d:   p->b+=2; (p++)->w=1; p->b=1;            nextb;  //b2w1b1
   case 0x0e:   p->b+=3; p->w=1;                        nextw;  //b3w1
   case 0x0f:   p->b+=4;                                nextb;  //b4
  }


//      After white pixel

comm_after_white:
 if(p >= bpe) return; // bound control
 switch (b & 0x0f)
  {
   case 0x00:   p->w+=4;                                nextw;  //w4
   case 0x01:   (p++)->w+=3; p->b=1;                    nextb;  //w3b1
   case 0x02:   (p++)->w+=2; setp(1,1);            nextw;  //w2b1w1
   case 0x03:   (p++)->w+=2; p->b=2;                    nextb;  //w2b2
   case 0x04:   (p++)->w+=1; setp(1,2);            nextw;  //w1b1w2
   case 0x05:   (p++)->w+=1; p->b=1;(p++)->w=1; p->b=1; nextb;  //w1b1w1b1
   case 0x06:   (p++)->w+=1; setp(2,1);            nextw;  //w1b2w1
   case 0x07:   (p++)->w+=1; p->b=3;                    nextb;  //w1b3
   case 0x08:   (++p)->b=1; p->w=3;                     nextw;  //b1w3
   case 0x09:   (++p)->b=1; (p++)->w=2; p->b=1;         nextb;  //b1w2b1
   case 0x0a:   (++p)->b=1; (p++)->w=1; setp(1,1); nextw;  //b1w1b1w1
   case 0x0b:   (++p)->b=1; (p++)->w=1; p->b=2;         nextb;  //b1w1b2
   case 0x0c:   (++p)->b=2; p->w=2;                     nextw;  //b2w2
   case 0x0d:   (++p)->b=2; (p++)->w=1; p->b=1;         nextb;  //b2w1b1
   case 0x0e:   (++p)->b=3; p->w=1;                     nextw;  //b3w1
   case 0x0f:   (++p)->b=4;                             nextb;  //b4
  }

}

void invert_tiff (Word8* p, Word16 lth)
{
 while (lth--) {*p = ~*p; p++;}
}
