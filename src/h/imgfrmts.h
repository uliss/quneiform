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


#ifndef __IMGFRMTS_H
#define __IMGFRMTS_H

//
//  General descriptor for available image disk formats.
struct TFormatDescriptor
{
 int16_t              id;
 char   extentions[32];
 char	      name[32];
 int16_t           flags;

#define   FD_READAVAIL			0x0001
#define   FD_WRITEAVAIL			0x0002

#define   FD_GREYAVAIL			0x0004
#define   FD_BWAVAIL          0x0008
#define   FD_COLORAVAIL			0x0010

#define   FD_MULTIPAGE			0x0020

#define   FD_BWDEF				0x0040  // Default formats for image types
#define   FD_GREYDEF			0x0080
#define   FD_COLORDEF			0x0100

#define   FD_FIRSTLASTONLY		0x0200  // Format supports DIB line sequence only
#define   FD_FORCEBWNEGATIVE	0x0400  // Format supports negative fotometric only B&W
#define   FD_FORCEGRAYNEGATIVE	0x0800  // Format supports negative fotometric only B&W

 uint32_t	iWLEntry;				  // Reserved for internal use

}; _SETTYPES( TFormatDescriptor )

typedef struct tagIFS_FORMATLISTSTRUCT{
   char * lpstrFilter;
	PTFormatDescriptor ifsFDL;
}IFS_FORMATLISTSTRUCT;

_SETTYPES( IFS_FORMATLISTSTRUCT );

//
// File Types constants (Ids in TFormatDescriptor)

#define	IAPI_TIFF_UNCOMPRESSED				0
#define	IAPI_BMP_UNCOMPRESSED				1
#define	IAPI_PCX					2
#define	IAPI_TARGA					3
#define	IAPI_GIF					4
#define	IAPI_WPG					5
#define	IAPI_WMF					6
#define	IAPI_TIFF_HUFFMAN 				7
#define	IAPI_TIFF_G3_FAX				8
#define	IAPI_TIFF_LZW					9
#define	IAPI_TIFF_G4_FAX				10
#define	IAPI_DCX					11
#define	IAPI_BMP_COMPRESSED				12
#define	IAPI_JPEG		         		13
#define	IAPI_EPS		         		14
#define	IAPI_PICT		         		15
#define	IAPI_TIFF_PACK         				16
#define	IAPI_TIFF_2D	         			17
#define	IAPI_CALS	         			18
#define	IAPI_LASER_DATA	   				19
#define	IAPI_XBM	         			20
#define	IAPI_MACPAINT	         			21
#define	IAPI_GX2	         			22
#define	IAPI_KOFAX	         			23
#define	IAPI_IOCA	         			24
#define	IAPI_ICON	         			25
#define	IAPI_IFF_ILBM	        			26
#define	IAPI_CLIP	        			27
#define	IAPI_IMG	        			28
#define	IAPI_BROOK_TROUT   				29
#define	IAPI_MSP   					30
#define	IAPI_CUT   					31
#define	IAPI_TARGA16 					32
#define	IAPI_CCITT_G3					33
#define	IAPI_CCITT_G4					34
#define	IAPI_XPM					35
#define	IAPI_XWD					36
#define	IAPI_RAST					37
#define	IAPI_ASCII					38
#define	IAPI_PHOTOCD 					39
#define	IAPI_TIFF_JPEG					40
#define	IAPI_PHOTOSHOP					41
#define	IAPI_IMNET					42
#define	IAPI_PCL_1					43
#define	IAPI_PCL_5					44

#define	IAPI_CTL2XX					45   // Attention !!! If You will upgrade
							     // AccuSoft You MUST check this value
							     // It can't be smaller than MAXFILTERS

#define  _2XX_WINSTRING   "CT Native (.2xx)\0*.2xx\0"
#define  _2XX_CAPTION     "CT Native (.2xx)"
#define  _2XX_EXTENSION   "*.2xx"


#endif //__IMGFRMTS_H
