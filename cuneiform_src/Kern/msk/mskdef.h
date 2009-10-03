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

typedef struct
{
	unsigned short rst[16];
	unsigned short tm16abc;
	unsigned short let;
	unsigned short tm18abc;
	unsigned short tm19abc;
	int prev;
	int next;
}MSK_abc;

typedef struct
{
	unsigned short let;
	int vol;
	int start;
}MSK_abc_ind;

/////////////////////

#define NONE    -1
#define	FALSE	0
#define	TRUE	1


#define pc_16_32 16
#if pc_16_32 == 16
	#define i_16(istr,i)     ( (istr) & (ist0[i]) )
#else
	#define i_16(istr,i)  (uint16_t) ( ( ( istr & ist0[i] ) & 0xffffffff)==0?0:1 )
#endif

#define ij_16(is,i,p) ( is=((p)? ((is)|ist0[i]) : ((is)&ist1[i])) )


#if pc_16_32 == 16
	unsigned short ist0[16]={
                       0x0080,0x0040,0x0020,0x0010,
                       0x0008,0x0004,0x0002,0x0001,
                       0x8000,0x4000,0x2000,0x1000,
                       0x0800,0x0400,0x0200,0x0100
                      };

	unsigned short ist1[16]={
                       0xff7f,0xffbf,0xffdf,0xffef,
                       0xfff7,0xfffb,0xfffd,0xfffe,
                       0x7fff,0xbfff,0xdfff,0xefff,
                       0xf7ff,0xfbff,0xfdff,0xfeff
                      };
#else
	unsigned int ist0[32]={
                       0x00800000,0x00400000,0x00200000,0x00100000,
                       0x00080000,0x00040000,0x00020000,0x00010000,
                       0x80000000,0x40000000,0x20000000,0x10000000,
                       0x08000000,0x04000000,0x02000000,0x01000000,
                       0x00000080,0x00000040,0x00000020,0x00000010,
                       0x00000008,0x00000004,0x00000002,0x00000001,
                       0x00008000,0x00004000,0x00002000,0x00001000,
                       0x00000800,0x00000400,0x00000200,0x00000100
                      };

	unsigned int ist1[32]={
                       0xff7f0000,0xffbf0000,0xffdf0000,0xffef0000,
                       0xfff70000,0xfffb0000,0xfffd0000,0xfffe0000,
                       0x7fff0000,0xbfff0000,0xdfff0000,0xefff0000,
                       0xf7ff0000,0xfbff0000,0xfdff0000,0xfeff0000,

                       0x0000ff7f,0x0000ffbf,0x0000ffdf,0x0000ffef,
                       0x0000fff7,0x0000fffb,0x0000fffd,0x0000fffe,
                       0x00007fff,0x0000bfff,0x0000dfff,0x0000efff,
                       0x0000f7ff,0x0000fbff,0x0000fdff,0x0000feff
                      };
#endif

#define MAX_LIN 16
#define MAX_WID 16
#define MAX_NM  8
#define MAX_NM1 1
#define  LIMI  60


#define MAX_ABC1 41
char abn1[MAX_ABC1] =   {
     '1', '1', '7', '7', '7', '7', '7', '7', '7', '7', '2', '2',
	 '2', '3', '3', '3', '3', '3', '3', '4', '4', '5', '5', '5',
     '5', '9', '9', '9', '6', '6', '6', '3', '2', '9', '6', '2',
     '0', '0', '8', '8', '8' };


unsigned int i_abn1[MAX_ABC1][MAX_LIN]={
248, 240, 128, 0, 0, 0, 0, 0, 0, 2, 4, 124, 252, 252, 252, 252,
254, 248, 240, 480, 960, 1792, 1792, 1792, 1808, 3872, 8128, 16256, 0, 0, 0, 0,
0, 0, 0, 0, 248, 256, 7936, 7936, 7936, 7936, 7936, 7936, 7936, 7936, 7936, 7936,
0, 0, 0, 240, 0, 256, 1792, 16128, 16128, 7936, 7936, 7936, 7936, 7936, 7936, 7936,
0, 0, 0, 0, 256, 1008, 1792, 3840, 3840, 32512, 7936, 7936, 7936, 7936, 7936, 7936,
0, 0, 0, 240, 0, 256, 1792, 16128, 16128, 7936, 7936, 7936, 7936, 7936, 7936, 7936,
0, 0, 248, 0, 0, 256, 1792, 7936, 7936, 7936, 7936, 7936, 7936, 7936, 16128, 7936,
0, 0, 0, 0, 0, 252, 248, 240, 192, 896, 1792, 3840, 7936, 16128, 32512, 32512,
0, 0, 0, 0, 0, 252, 240, 240, 192, 128, 768, 1792, 3840, 7936, 32512, 32512,
0, 0, 0, 0, 0, 252, 1008, 960, 1984, 1920, 1792, 3840, 7936, 16128, 16128, 16128,
0, 0, 0, 0, 0, 24, 24, 48, 192, 896, 1792, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 28, 28, 48, 192, 128, 768, 1792, 0, 0, 0, 0,
0, 0, 0, 0, 0, 12, 12, 8, 16, 224, 0, 768, 1792, 0, 0, 0,
0, 0, 0, 3888, 384, 0, 0, 0, 0, 0, 240, 0, 768, 16128, 16128, 16128,
0, 0, 0, 32, 4064, 0, 0, 0, 0, 0, 240, 224, 768, 16128, 16128, 16128,
0, 0, 0, 248, 3968, 0, 0, 0, 0, 0, 0, 248, 768, 7936, 16128, 16128,
0, 0, 240, 128, 3840, 0, 0, 0, 0, 0, 240, 0, 768, 16128, 16128, 16128,
0, 0, 0, 0, 224, 3968, 0, 0, 0, 0, 224, 0, 768, 16128, 16128, 16128,
0, 0, 0, 248, 8064, 0, 0, 0, 0, 0, 240, 240, 816, 768, 768, 16128,
1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 252, 248, 248, 248,
32768, 0, 0, 16384, 16384, 0, 0, 0, 0, 0, 0, 0, 252, 252, 252, 252,
0, 0, 0, 0, 16128, 0, 0, 0, 0, 252, 0, 0, 0, 0, 0, 0,
0, 0, 0, 32512, 0, 0, 0, 0, 0, 0, 252, 0, 0, 0, 0, 0,
0, 0, 0, 0, 16128, 0, 0, 0, 0, 0, 0, 252, 0, 0, 0, 0,
0, 0, 0, 65280, 65280, 0, 0, 0, 0, 0, 240, 256, 256, 1792, 7936, 16128,
0, 0, 0, 0, 32769, 0, 0, 0, 0, 0, 240, 256, 256, 1792, 7936, 16128,
0, 0, 0, 0, 32769, 0, 0, 0, 0, 0, 0, 496, 256, 1792, 7936, 16128,
0, 0, 0, 0, 32769, 0, 0, 0, 0, 0, 0, 256, 496, 1920, 7936, 16128,
240, 224, 3968, 0, 0, 0, 0, 0, 0, 0, 32769, 0, 0, 0, 0, 0,
254, 252, 240, 1984, 384, 0, 0, 0, 0, 0, 32769, 0, 0, 0, 0, 0,
240, 224, 128, 0, 1792, 0, 0, 0, 0, 0, 49155, 0, 0, 0, 0, 0,
0, 0, 0, 0, 255, 0, 0, 0, 0, 0, 0, 255, 0, 0, 0, 0,
0, 0, 0, 0, 255, 255, 0, 0, 0, 0, 65280, 65280, 0, 0, 0, 0,
0, 0, 0, 0, 32769, 0, 0, 0, 0, 0, 33023, 32783, 0, 0, 0, 0,
0, 0, 0, 65280, 65280, 0, 0, 0, 0, 0, 0, 32769, 0, 0, 0, 0,
0, 0, 0, 0, 8, 1008, 1792, 7936, 65280, 61440, 49159, 49159, 0, 0, 0, 0,
0, 0, 0, 0, 0, 57345, 57345, 24576, 24576, 1, 1, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 3, 3, 3, 3, 1, 1, 0, 0, 0, 0, 0,
0, 0, 0, 32769, 0, 0, 0, 0, 0, 0, 0, 32769, 0, 0, 0, 0,
0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
0, 0, 0, 0, 32768, 32768, 0, 0, 0, 0, 32768, 0, 0, 0, 0, 0
};
