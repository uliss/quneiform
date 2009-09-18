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


/****************************************************************
*  This is Extention code of ED format.
*
*   Piter 01.08.96
*****************************************************************/
#ifndef __EDEXT__
#define __EDEXT__

#include "ctp.h"

#define SS_EXTENTION	0x1C

#pragma pack(push,1)
struct edExtention
{
	uchar/*unsigned char   */  	code;   /* always SS_EXTENTION <0x1C> */
	uint16_t/*unsigned short int*/      Ecode;  /* New extention code */
	uint16_t/*unsigned short int*/      length; /* Length in bytes */
};
struct edExtentionNew
{
	uchar/*unsigned char   */  	code;   /* always SS_EXTENTION <0x1C> */
	uint16_t/*unsigned short int*/      Ecode;  /* New extention code */
	uint32_t/*unsigned short int*/      length; /* Length in bytes */
};
/* This structure is header all structs included in extention ED */
typedef struct edExtention EDEXT;

/* DESCRIPTIONS EXTENTION CODE */

/* 0x0000 - 0x00FF special code */
#define ITS_EDEXT_SPECIAL(a)   (a>=0x0000 && a<0x0100)
/* 0x0100 - 0x01FF table description */
#define ITS_EDEXT_TABLE(a)     (a>=0x0100 && a<0x0200)
/* 0x0200 - 0x02FF picture description */
#define ITS_EDEXT_PICTURE(a)   (a>=0x0200 && a<0x0300)
/* 0x0300 - 0xEFFF Your code please...*/

/* 0xF000 - 0xFFFF temporary code for debugging ( ! Not used in releas version !)*/

#define EDEXT_VERSION		0x0000

#define EDEXT_TABLE_START 	0x0100 // edTable struct
#define EDEXT_TABLE_VER         0x0101 // array horiz. coord of vert lines (x0,x1,...)
#define EDEXT_TABLE_HOR         0x0102 // array vert. coord of horiz. lines (y0,y1,...)
#define EDEXT_TABLE_TAB         0x0103 // array ID of items
#define EDEXT_TABLE_VERLINE     0x0104 // array not delete vert lines (x00,y00,x01,y01,...)

#define EDEXT_CTP	 	0x0200 // filename of CTP file
#define EDEXT_PICTURE           0x0201 // struct of <edPicture>
#define	EDEXT_TIFF_DESC	0x202	//Описатель исходного изображения

#define EDEXT_FONTS		0x300
#define EDEXT_BORDERS	0x301
#define EDEXT_SECTION	0x302
#define EDEXT_FRAME		0x303
#define EDEXT_PARAGRAPH 0x304
#define EDEXT_TABLE_ROW 0x305
//SS_EXTENTION может быть 16 и 32 битно длинным - здесь 32
#define EDEXT_PICS		(0x306|0x8000)
#define EDEXT_TABLE_CELL 0x307
#define EDEXT_CHAR		0x308
typedef struct sTable
{
 EDEXT 	  head;
 int32_t      Sh;   	// count horizontal lines
 int32_t      Sv;           // count vertical lines
 int32_t      angle;        // angle of table 1/1024 rad
 int32_t      VerCount;     // count not delete of vert lines
 int32_t      ShiftX;       // shift tables in image
 int32_t      ShiftY;       //
} edTable;

#define EDEXT_SIZE_FILENAME 256
#pragma pack(pop)
#endif
