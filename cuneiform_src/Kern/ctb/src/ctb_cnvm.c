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

//************************************************************************//
//* CTB_CNVM.C :       convert memory functions **************************//
//************************************************************************//

#include "ctb.h"

static void conv_bits_to_bytes8(Word8 *bits,   Word8 *bytes, Int16 l);
static void conv_bytes_to_bits8(Word8 *bytes,  Word8 *bits, Int16 l);
static void conv_bits_to_bytes4(Word8 *bits,   Word8 *bytes, Int16 l);
static void conv_bytes_to_bits4(Word8 *bytes,  Word8 *bits, Int16 l);
static void conv_bits_to_bytes2(Word8 *bits,   Word8 *bytes, Int16 l);
static void conv_bits_to_bytes1(Word8 *bits,   Word8 *bytes, Int16 l);
static void conv_bytes_to_bits2(Word8 *bytes,  Word8 *bits, Int16 l);
static void conv_bytes_to_bits1(Word8 *bytes,  Word8 *bits, Int16 l);
static void conv_bits_to_bytes4plane(Word8 *bits,  Word8 *bytes, Int16 l);
static void conv_bytes_to_bits4plane(Word8 *bytes, Word8 *bits, Int16 l);
static void conv_bits_to_bytes2plane(Word8 *bits,  Word8 *bytes, Int16 l);
static void conv_bits_to_bytes1plane(Word8 *bits,  Word8 *bytes, Int16 l);
static void conv_bytes_to_bits2plane(Word8 *bytes, Word8 *bits, Int16 l);
static void conv_bytes_to_bits1plane(Word8 *bytes, Word8 *bits, Int16 l);

static void conv_bits2_to_bits2plane_one(Word8 *bout,Word8 *text,
                       Word8 msk1,Word8 msk2,Word8 msk3,Word8 msk4,Int16 len);
static void conv_bits4_to_bits4plane_one(Word8 *bout,Word8 *text,
                                   Word8 mask1,Word8 mask2, Int16 len);
static Bool32 conv_bits_to_bytes( Int16 colors,Word8 *bin,Word8 *text, Int16 len);
static Bool32 conv_bytes_to_bits( Int16 colors, Word8 *text,Word8 *bin, Int16 len);
static void   conv_bits4_to_bits4plane(Word8 *text,Word8 *bin,Int16 len,Int16 halftone8);
static void   conv_bits2_to_bits2plane(Word8 *text,Word8 *bin,Int16 len);

// can be EXPORT
Bool32  conv_bits_to_bytes( Int16 colors,Word8 *bin,Word8 *text, Int16 len)
{
switch( colors )
	{
	case 2   : conv_bits_to_bytes8(bin,text,len);      return 1;
	case 4   : conv_bits_to_bytes4(bin,text,len);      return 1;
	case 16  : conv_bits_to_bytes2(bin,text,len);      return 1;
    case 256 : conv_bits_to_bytes1(bin,text,len);      return 1;
	case -4  : conv_bits_to_bytes4plane(bin,text,len); return 1;
	case -16 : conv_bits_to_bytes2plane(bin,text,len); return 1;
    case -256: conv_bits_to_bytes1plane(bin,text,len); return 1;
	}

return 0;
}
// can be EXPORT
Bool32  conv_bytes_to_bits( Int16 colors, Word8 *text,Word8 *bin, Int16 len)
{
switch( colors )
	{
	case 2   : conv_bytes_to_bits8(text,bin,len);      return 1;
	case 4   : conv_bytes_to_bits4(text,bin,len);      return 1;
	case 16  : conv_bytes_to_bits2(text,bin,len);      return 1;
    case 256 : conv_bytes_to_bits1(text,bin,len);      return 1;
	case -4  : conv_bytes_to_bits4plane(text,bin,len); return 1;
	case -16 : conv_bytes_to_bits2plane(text,bin,len); return 1;
    case -256: conv_bytes_to_bits1plane(text,bin,len); return 1;
	}

return 0;
}

// 256 colors //
void conv_bits_to_bytes1(Word8 *bin,Word8 *text, Int16 len)
{
memcpy( text,bin, (Word16)len);
return;
}
void conv_bytes_to_bits1(Word8 *text,Word8 *bin, Int16 len)
{
memcpy( text,bin, (Word16)len);
return;
}

// 16 colors - EGA //
void conv_bits_to_bytes2(Word8 *bin,Word8 *text, Int16 len)
{
Int16 i;
Word8 c,*p;
memset( text,0x0, (Word16)len<<1);
for(p=text,i=0;i<len;i++)
	{
	c = bin[i];
	if( c&0xF0 ) *p= (c&0xF0)>>4; p++;
	if( c&0x0F ) *p= (c&0x0F);    p++;
	}

return;
}

void conv_bytes_to_bits2(Word8 *text,Word8 *bin, Int16 len)
{
Word8 *p=bin,c;
Int16 i;

for(i=0;i<len;i++,text+=2)
	{
	c  = text[0];
	c <<=4; c|= text[1];
	*p++=c;
	}

return;
}

// -256 colors - planar VGA //
#define SHIFT_SAVE1_1(mask )	if( c&mask )	*p  = 0x01;  p++;
#define SHIFT_SAVE1_2(mask )	if( c&mask )	*p |= 0x02;  p++;
#define SHIFT_SAVE1_3(mask )	if( c&mask )	*p |= 0x04;  p++;
#define SHIFT_SAVE1_4(mask )	if( c&mask )	*p |= 0x08;  p++;
#define SHIFT_SAVE1_5(mask )	if( c&mask )	*p |= 0x10;  p++;
#define SHIFT_SAVE1_6(mask )	if( c&mask )	*p |= 0x20;  p++;
#define SHIFT_SAVE1_7(mask )	if( c&mask )	*p |= 0x40;  p++;
#define SHIFT_SAVE1_8(mask )	if( c&mask )	*p |= 0x80;  p++;
void conv_bits_to_bytes1plane(Word8 *bin,Word8 *text,Int16 len)
{
Int16 i,len8=len/8,len0;
Word8 c,*p;

memset( text,0x0, (Word16)len);
len0 = len8;
for(p=text,i=0;i<len0;i++)
	{
    c = bin[i];
	SHIFT_SAVE1_1(0x80);
	SHIFT_SAVE1_1(0x40);
	SHIFT_SAVE1_1(0x20);
	SHIFT_SAVE1_1(0x10);
	SHIFT_SAVE1_1(0x08);
	SHIFT_SAVE1_1(0x04);
	SHIFT_SAVE1_1(0x02);
	SHIFT_SAVE1_1(0x01);
	}
len0 += len8;
for(p=text;i<len0;i++)
	{
	c = bin[i];
	SHIFT_SAVE1_2(0x80);
	SHIFT_SAVE1_2(0x40);
	SHIFT_SAVE1_2(0x20);
	SHIFT_SAVE1_2(0x10);
	SHIFT_SAVE1_2(0x08);
	SHIFT_SAVE1_2(0x04);
	SHIFT_SAVE1_2(0x02);
	SHIFT_SAVE1_2(0x01);
	}
len0 += len8;
for(p=text;i<len0;i++)
	{
	c = bin[i];
	SHIFT_SAVE1_3(0x80);
	SHIFT_SAVE1_3(0x40);
	SHIFT_SAVE1_3(0x20);
	SHIFT_SAVE1_3(0x10);
	SHIFT_SAVE1_3(0x08);
	SHIFT_SAVE1_3(0x04);
	SHIFT_SAVE1_3(0x02);
	SHIFT_SAVE1_3(0x01);
	}
len0 += len8;
for(p=text;i<len0;i++)
	{
	c = bin[i];
	SHIFT_SAVE1_4(0x80);
	SHIFT_SAVE1_4(0x40);
	SHIFT_SAVE1_4(0x20);
	SHIFT_SAVE1_4(0x10);
	SHIFT_SAVE1_4(0x08);
	SHIFT_SAVE1_4(0x04);
	SHIFT_SAVE1_4(0x02);
	SHIFT_SAVE1_4(0x01);
	}
len0 += len8;
for(p=text;i<len0;i++)
	{
    c = bin[i];
	SHIFT_SAVE1_5(0x80);
	SHIFT_SAVE1_5(0x40);
	SHIFT_SAVE1_5(0x20);
	SHIFT_SAVE1_5(0x10);
	SHIFT_SAVE1_5(0x08);
	SHIFT_SAVE1_5(0x04);
	SHIFT_SAVE1_5(0x02);
	SHIFT_SAVE1_5(0x01);
	}
len0 += len8;
for(p=text;i<len0;i++)
	{
	c = bin[i];
	SHIFT_SAVE1_6(0x80);
	SHIFT_SAVE1_6(0x40);
	SHIFT_SAVE1_6(0x20);
	SHIFT_SAVE1_6(0x10);
	SHIFT_SAVE1_6(0x08);
	SHIFT_SAVE1_6(0x04);
	SHIFT_SAVE1_6(0x02);
	SHIFT_SAVE1_6(0x01);
	}
len0 += len8;
for(p=text;i<len0;i++)
	{
	c = bin[i];
	SHIFT_SAVE1_7(0x80);
	SHIFT_SAVE1_7(0x40);
	SHIFT_SAVE1_7(0x20);
	SHIFT_SAVE1_7(0x10);
	SHIFT_SAVE1_7(0x08);
	SHIFT_SAVE1_7(0x04);
	SHIFT_SAVE1_7(0x02);
	SHIFT_SAVE1_7(0x01);
	}
len0 += len8;
for(p=text;i<len0;i++)
	{
	c = bin[i];
	SHIFT_SAVE1_8(0x80);
	SHIFT_SAVE1_8(0x40);
	SHIFT_SAVE1_8(0x20);
	SHIFT_SAVE1_8(0x10);
	SHIFT_SAVE1_8(0x08);
	SHIFT_SAVE1_8(0x04);
	SHIFT_SAVE1_8(0x02);
	SHIFT_SAVE1_8(0x01);
	}

return;
}

#define SAVE_TEXT1(ind) c1<<=1; c2<<=1; c3<<=1; c4<<=1; c5<<=1; c6<<=1; c7<<=1; c8<<=1; \
        if( text[ind]&0x01 ) c1 |= 1;    \
        if( text[ind]&0x02 ) c2 |= 1;    \
        if( text[ind]&0x04 ) c3 |= 1;    \
        if( text[ind]&0x08 ) c4 |= 1;    \
        if( text[ind]&0x10 ) c5 |= 1;    \
        if( text[ind]&0x20 ) c6 |= 1;    \
        if( text[ind]&0x40 ) c7 |= 1;    \
        if( text[ind]&0x80 ) c8 |= 1;

void conv_bytes_to_bits1plane(Word8 *text,Word8 *bin,Int16 len)
{
Int16   len8 = len/8;
Word8 *p1=bin+0*len8, *p2=bin+1*len8, *p3=bin+2*len8,  *p4=bin+3*len8;
Word8 *p5=bin+4*len8, *p6=bin+5*len8, *p7=bin+6*len8,  *p8=bin+7*len8;
Word8  c1, c2, c3, c4, c5, c6, c7, c8;
int i;


for(i=0;i<len8;i++,text+=8)
	{
    if( i==250 )
        i+=0;
	c1 = ( (text[0]&0x01)!=0 );
	c2 = ( (text[0]&0x02)!=0 );
	c3 = ( (text[0]&0x04)!=0 );
	c4 = ( (text[0]&0x08)!=0 );
    c5 = ( (text[0]&0x10)!=0 );
	c6 = ( (text[0]&0x20)!=0 );
	c7 = ( (text[0]&0x40)!=0 );
	c8 = ( (text[0]&0x80)!=0 );
	SAVE_TEXT1(1);
	SAVE_TEXT1(2);
	SAVE_TEXT1(3);
	SAVE_TEXT1(4);
	SAVE_TEXT1(5);
	SAVE_TEXT1(6);
	SAVE_TEXT1(7);
	*p1++=c1;
	*p2++=c2;
	*p3++=c3;
	*p4++=c4;
	*p5++=c5;
	*p6++=c6;
	*p7++=c7;
	*p8++=c8;
    }

return;
}

// -16 colors - planar EGA //
#define SHIFT_SAVE2_1(mask )	if( c&mask )	*p  = 0x1;  p++;
#define SHIFT_SAVE2_2(mask )	if( c&mask )	*p |= 0x2;  p++;
#define SHIFT_SAVE2_3(mask )	if( c&mask )	*p |= 0x4;  p++;
#define SHIFT_SAVE2_4(mask )	if( c&mask )	*p |= 0x8;  p++;
void conv_bits_to_bytes2plane(Word8 *bin,Word8 *text,Int16 len)
{
Int16 i,len4=len>>2,len2=len>>1,len3=len2+len4;
Word8 c,*p;

memset( text,0x0, (Word16)len<<1);
for(p=text,i=0;i<len4;i++)
	{
    c = bin[i];
	SHIFT_SAVE2_1(0x80);
	SHIFT_SAVE2_1(0x40);
	SHIFT_SAVE2_1(0x20);
	SHIFT_SAVE2_1(0x10);
	SHIFT_SAVE2_1(0x08);
	SHIFT_SAVE2_1(0x04);
	SHIFT_SAVE2_1(0x02);
	SHIFT_SAVE2_1(0x01);
	}
for(p=text;i<len2;i++)
	{
	c = bin[i];
	SHIFT_SAVE2_2(0x80);
	SHIFT_SAVE2_2(0x40);
	SHIFT_SAVE2_2(0x20);
	SHIFT_SAVE2_2(0x10);
	SHIFT_SAVE2_2(0x08);
	SHIFT_SAVE2_2(0x04);
	SHIFT_SAVE2_2(0x02);
	SHIFT_SAVE2_2(0x01);
	}
for(p=text;i<len3;i++)
	{
	c = bin[i];
	SHIFT_SAVE2_3(0x80);
	SHIFT_SAVE2_3(0x40);
	SHIFT_SAVE2_3(0x20);
	SHIFT_SAVE2_3(0x10);
	SHIFT_SAVE2_3(0x08);
	SHIFT_SAVE2_3(0x04);
	SHIFT_SAVE2_3(0x02);
	SHIFT_SAVE2_3(0x01);
	}
for(p=text;i<len;i++)
	{
	c = bin[i];
	SHIFT_SAVE2_4(0x80);
	SHIFT_SAVE2_4(0x40);
	SHIFT_SAVE2_4(0x20);
	SHIFT_SAVE2_4(0x10);
	SHIFT_SAVE2_4(0x08);
	SHIFT_SAVE2_4(0x04);
	SHIFT_SAVE2_4(0x02);
	SHIFT_SAVE2_4(0x01);
	}

return;
}

#define SAVE_TEXT2(ind) c1<<=1; c2<<=1; c3<<=1; c4<<=1; \
												if( text[ind]&0x1 ) c1 |= 1;    \
												if( text[ind]&0x2 ) c2 |= 1;    \
												if( text[ind]&0x4 ) c3 |= 1;    \
												if( text[ind]&0x8 ) c4 |= 1;
void conv_bytes_to_bits2plane(Word8 *text,Word8 *bin,Int16 len)
{
Word8 *p1=bin, *p2=bin+(len>>2), *p3=bin+(len>>1),  *p4=bin+len-(len>>2);
Word8  c1, c2, c3, c4, i;

len >>= 2;
for(i=0;i<len;i++,text+=8)
	{
	c1 = ( (text[0]&0x1)!=0 );
	c2 = ( (text[0]&0x2)!=0 );
	c3 = ( (text[0]&0x4)!=0 );
	c4 = ( (text[0]&0x8)!=0 );
	SAVE_TEXT2(1);
	SAVE_TEXT2(2);
	SAVE_TEXT2(3);
	SAVE_TEXT2(4);
	SAVE_TEXT2(5);
	SAVE_TEXT2(6);
	SAVE_TEXT2(7);
	*p1++=c1;
	*p2++=c2;
	*p3++=c3;
	*p4++=c4;
	}

return;
}

// 4 colors - CGA //
void conv_bits_to_bytes4(Word8 *bin,Word8 *text,Int16 len)
{
Word16 i;
Word8 c,*p;
memset( text,0x0, (Word16)len<<2);
for(p=text,i=0;i<len;i++)
	{
	c = bin[i];
	if( c&0xC0 ) *p= (c&0xC0)>>6; p++;
	if( c&0x30 ) *p= (c&0x30)>>4; p++;
	if( c&0x0C ) *p= (c&0x0C)>>2; p++;
	if( c&0x03 ) *p= (c&0x03);    p++;
	}

return;
}

void conv_bytes_to_bits4(Word8 *text,Word8 *bin,Int16 len)
{
Word8 *p=bin,c;
Int16 i;

for(i=0;i<len;i++,text+=4)
	{
	c = text[0];
	c <<= 2; c |= text[1];
	c <<= 2; c |= text[2];
	c <<= 2; c |= text[3];
	*p++=c;
	}

return;
}

// -4 colors - planar CGA //
#define SHIFT_SAVE4_1(mask )	if( c&mask )	*p=0x1;  p++;
#define SHIFT_SAVE4_2(mask )	if( c&mask )	*p|=0x2; p++;
void conv_bits_to_bytes4plane(Word8 *bin,Word8 *text,Int16 len)
{
Int16 i,len2=len>>1;
Word8 c,*p;

memset( text,0x0, (Word16)len<<2);
for(p=text,i=0;i<len2;i++)
	{
  c = bin[i];
	SHIFT_SAVE4_1(0x80);
	SHIFT_SAVE4_1(0x40);
	SHIFT_SAVE4_1(0x20);
	SHIFT_SAVE4_1(0x10);
	SHIFT_SAVE4_1(0x08);
	SHIFT_SAVE4_1(0x04);
	SHIFT_SAVE4_1(0x02);
	SHIFT_SAVE4_1(0x01);
	}
for(p=text;i<len;i++)
	{
	c = bin[i];
	SHIFT_SAVE4_2(0x80);
	SHIFT_SAVE4_2(0x40);
	SHIFT_SAVE4_2(0x20);
	SHIFT_SAVE4_2(0x10);
	SHIFT_SAVE4_2(0x08);
	SHIFT_SAVE4_2(0x04);
	SHIFT_SAVE4_2(0x02);
	SHIFT_SAVE4_2(0x01);
	}

return;
}

#define SAVE_TEXT4(ind) c1<<=1; c2<<=1;              \
												if( text[ind]&0x1 ) c1 |= 1; \
												if( text[ind]&0x2 ) c2 |= 1;
void conv_bytes_to_bits4plane(Word8 *text,Word8 *bin, Int16 len)
{
Word8 *p1=bin, *p2=bin+(len>>1);
Word8  c1, c2, i;

len>>=1;
for(i=0;i<len;i++,text+=8)
	{
	c1 = ( (text[0]&0x1)!=0 );
	c2 = ( (text[0]&0x2)!=0 );
	SAVE_TEXT4(1);
	SAVE_TEXT4(2);
	SAVE_TEXT4(3);
	SAVE_TEXT4(4);
	SAVE_TEXT4(5);
	SAVE_TEXT4(6);
	SAVE_TEXT4(7);
	*p1++=c1;
	*p2++=c2;
	}

return;
}

// 2 colors - Monochrom //
#define SHIFT_SAVE8(mask )	if( c&mask )	*p=0x1; p++;
void conv_bits_to_bytes8(Word8 *bin,Word8 *text, Int16 len)
{
Int16 i;
Word8 c,*p;
memset( text,0x0, len*8);
for(p=text,i=0;i<len;i++)
	{
	c = bin[i];
	SHIFT_SAVE8(0x80);
	SHIFT_SAVE8(0x40);
	SHIFT_SAVE8(0x20);
	SHIFT_SAVE8(0x10);
	SHIFT_SAVE8(0x08);
	SHIFT_SAVE8(0x04);
	SHIFT_SAVE8(0x02);
	SHIFT_SAVE8(0x01);
	}

return;
}

#define SAVE_TEXT8(ind) c<<=1; if( text[ind]==0x1 )		c |= 1;
void conv_bytes_to_bits8(Word8 *text,Word8 *bin, Int16 len)
{
Word8 *p=bin,c;
Int16 i;

for(i=0;i<len;i++,text+=8)
	{
	c = ( text[0]==0x1 );
	SAVE_TEXT8(1);
	SAVE_TEXT8(2);
	SAVE_TEXT8(3);
	SAVE_TEXT8(4);
	SAVE_TEXT8(5);
	SAVE_TEXT8(6);
	SAVE_TEXT8(7);
	*p++=c;
	}

return;
}
// can be EXPORT
void conv_bits4_to_bits4plane(Word8 *txt,Word8 *bin,Int16 len, Int16 halftone8)
{

{
Word8 *p1=bin, *p2=bin+(len>>2), *p3=bin+(len>>1),  *p4=bin+len-(len>>2);

len >>= 2;
if( !halftone8 )
  conv_bits4_to_bits4plane_one(p1,txt,0x10,0x01,len);
conv_bits4_to_bits4plane_one(p2,txt,0x20,0x02,len);
conv_bits4_to_bits4plane_one(p3,txt,0x40,0x04,len);
conv_bits4_to_bits4plane_one(p4,txt,0x80,0x08,len);
}

return;
}


// can be EXPORT
void conv_bits2_to_bits2plane(Word8 *text,Word8 *bin,Int16 len)
{
Word8 *p1=bin, *p2=bin+(len>>1);

len >>= 1;
conv_bits2_to_bits2plane_one(p1,text,0x40,0x10,0x04,0x01,len);
conv_bits2_to_bits2plane_one(p2,text,0x80,0x20,0x08,0x02,len);

return;
}

void conv_bits4_to_bits4plane_one(Word8 *bout,Word8 *text,
                                   Word8 mask1,Word8 mask2, Int16 len)
{
Int16  i;
Word8 c;
for(i=0;i<len;i++,text+=4)
	{
	c = ( (text[0]&mask1)!=0 );
	c  <<= 1;
	if( text[0]&mask2 )	c |= 1;
	c  <<= 1;
	if( text[1]&mask1 )	c |= 1;
  c  <<= 1;
	if( text[1]&mask2 )	c |= 1;
  c  <<= 1;
	if( text[2]&mask1 )	c |= 1;
  c  <<= 1;
	if( text[2]&mask2 )	c |= 1;
  c  <<= 1;
	if( text[3]&mask1 )	c |= 1;
  c  <<= 1;
	if( text[3]&mask2 )	c |= 1;
	*bout++ = c;
	}
return ;
}

void conv_bits2_to_bits2plane_one(Word8 *bout,Word8 *text,
                                   Word8 msk1,Word8 msk2,Word8 msk3,Word8 msk4,
                                   Int16 len)
{
Int16  i;
Word8 c;
for(i=0;i<len;i++,text+=2)
	{
	c = ( (text[0]&msk1)!=0 );
	c  <<= 1;
	if( text[0]&msk2 )	c |= 1;
	c  <<= 1;
	if( text[0]&msk3 )	c |= 1;
  c  <<= 1;
	if( text[0]&msk4 )	c |= 1;
  c  <<= 1;
	if( text[1]&msk1 )	c |= 1;
  c  <<= 1;
	if( text[1]&msk2 )	c |= 1;
  c  <<= 1;
	if( text[1]&msk3 )	c |= 1;
  c  <<= 1;
	if( text[1]&msk4 )	c |= 1;
	*bout++ = c;
	}
return ;
}
