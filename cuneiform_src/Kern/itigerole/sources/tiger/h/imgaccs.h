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

#ifndef __IMGACCS_H
#define __IMGACCS_H

#ifndef __GLOBUS_H
#include "globus.h"
#endif

#pragma pack(4)

//
// Palette entry descriptor:

typedef struct   tagImgRGB
{
	Word8 bRed;
	Word8 bGreen;
	Word8 bBlue;
   Word8 breserved;
} TImgRGB;

_SETTYPES(TImgRGB)
//
// Export/import image descriptor.
typedef struct tagImgInfo
{
    Word16  wImageHeight;            // in pixels
    Word16  wImageWidth;             // in pixels
    Word16  wImageByteWidth;         // in bytes
    Word16  wImageDisplacement;      // in pixels

    Word16  wResolutionX;            // in dpi
    Word16  wResolutionY;            // in dpi
    Word8  bFotoMetrics;             // 1, if white is highest number,
                                     // 0 -if white is lowest number.
    Word8  bCompression;             // compression of delivered strips
#define CM_UNCOMPRESSED    0x0010
#define CM_GROUP4          0x0020
#define CM_JPEG            0x0040
#define CM_2XX             0x0080


    Word16  wAddX;                   //  "real"  coordinates of
    Word16  wAddY;                   //   upper-left corner (in pixels).

    Word8   bSamplesPerPixel;
    Word8   bBitsPerSample;
    Word8   bPlanes;
    Word8   LinesOrder;              // 0 - as DDB, 1 as DIB.
#define LO_FIRSTFIRST 0
#define LO_FIRSTLAST  1
    Int16   PaletteSize;
    TImgRGB* Palette;

    Word8    ByteAlign;
#define      BA_COMPACT     1
#define      BA_TWOBYTE     2
#define      BA_FOURBYTE    3

    Word8   spare[15];   // reserved for future.
    Err16   error;
    Int16   step;  // step of sender/receiver conversation.
#define CS_QUEST   0x0000   // struct has no information;
                            // it should be filled.
                            //(should be set by receiver when it calls
                            // SenderOpenCallback first time).

#define CS_OFFER   0x0001   // struct bears information about
                            // available image and possible variations
                            // of image representation.
                            // (should be set by sender, filling
                            // ImageInfo first time.)

#define CS_BID     0x0002   // struct bears a request of receiver to sender.

#define CS_CONFIRM 0x0003   // sender confirms image characteristics
                            // (final step of conversation)/
//
// Available:
    Word32    avFlags;
// Line numbering
#define AV_LO_FIRSTFIRST      0x0001
#define AV_LO_FIRSTLAST       0x0002
// Photometric
#define AV_PH_WHITEMIN        0x0004
#define AV_PH_WHITEMAX        0x0008
// Compression
#define AV_CM_UNCOMPRESSED    0x0010
#define AV_CM_GROUP4          0x0020
#define AV_CM_JPEG            0x0040
#define AV_CM_2XX             0x0080

// Image transformation
#define AV_IT_SCALING         0x0100
#define AV_IT_GREY            0x0200
#define AV_IT_BW              0x0400
#define AV_IT_COLOR           0x0800

// Image presentation
#define AV_IP_PLANES          0x1000
#define AV_IP_SAMPLES         0x2000
#define AV_IP_PALETTE         0x4000


//
//  Byte Width Alignment
#define   AV_BA_COMPACT       0x10000l
#define   AV_BA_TWOBYTE       0x20000l
#define   AV_BA_FOURBYTE      0x40000l
} TImgInfo;

_SETTYPES(TImgInfo)

enum EImageType
{ ITE_FIRST =0,
  ITE_BW, ITE_Gr16, ITE_Gr256,
  ITE_RGB, ITE_Planes3,
  ITE_Palette2, ITE_Palette16, ITE_Palette256,
  ITE_Unregistered,
  ITE_LAST
};

#ifdef __cplusplus
inline   EImageType GetImageType (TImgInfo *info)
{
  switch(info ->PaletteSize) // palette image
     {
      case 2:    return ITE_Palette2;
      case 16:   return ITE_Palette16;
      case 256:  return ITE_Palette256;
      default :  return ITE_Unregistered;
      case 0:    break;
     }
  // no palette presents
  if (info ->bPlanes >1)
         return (info ->bPlanes ==3)? ITE_Planes3 :ITE_Unregistered;
  if (info ->bPlanes ==0)     return ITE_Unregistered;

  // one plane images
  if (info ->bSamplesPerPixel >1)
         return (info ->bSamplesPerPixel ==3)? ITE_RGB :ITE_Unregistered;
  if (info ->bSamplesPerPixel ==0)     return ITE_Unregistered;

  // one sample per pixel images
  switch (info ->bBitsPerSample)
      {
       case 1:                   return ITE_BW;
       case 4:                   return ITE_Gr16;
       case 8:                   return ITE_Gr256;
      }
 return ITE_Unregistered;
};
#endif


//
// Import callbacks prototypes:

typedef Bool  (*TImpImgOpen) (TImgInfo * lpImageInfo);
typedef int32_t (*TImpImgRead) (Word8* lpImage, int32_t wMaxSize);
typedef Int16 (*TImpImgGetBlock)(Word8* lpBuff, Int16 fstLine, Int16 nLines);
typedef Bool  (*TImpImgClose)(void);

//
// Import callbacks complect:
typedef struct tagImgImport
{
 TImpImgOpen       Open;
 TImpImgRead       Read;  // may be NULL
 TImpImgGetBlock    Get;  // may be NULL
 TImpImgClose     Close;
}TImgImport;
_SETTYPES(TImgImport)
//
// Export callbacks   prototypes:

typedef Bool  (*TExpImgOpen)    (TImgInfo * lpImageInfo);
typedef int32_t (*TExpImgWrite)   (Word8* lpImage, int32_t wMaxSize);
typedef Int16 (*TExpImgSetBlock)(Word8* lpBuff, Int16 fstLine, Int16 nLines);
typedef Bool  (*TExpImgClose)   (void);

//
// Export callbacl complects:

typedef struct tagImgExport
{
      TExpImgOpen       Open;
      TExpImgWrite     Write;  // may be NULL
      TExpImgSetBlock    Set;  // may be NULL
      TExpImgClose     Close;
} TImgExport;


_SETTYPES(TImgExport)


typedef struct tagZoneFunc
 {
  Bool16 (* Get) (Rect16 * zone);
  Bool16 (* Set) (CRect16 * zone);
  Bool16 (* Enable) (Bool16  mode);

 } TZoneFunc; _SETTYPES(TZoneFunc);
//
// Obsolete image export/import style. (For Tiger compatibilyty.)

typedef struct tagTiger_ImageInfo
{
    Word16  wImageHeight;           /* Height of the image in lines       */
    Word16  wImageWidth;            /* Width of the image in pixels       */
    Word16  wImageByteWidth;        /* Size of the image line in bytes    */
    Word16  wImageDisplacement;     /* Displacement of the first pixel of */
                                    /*   image line in pixels, usually 0  */
    Word16  wResolutionX;           /* Resolution by X-axe */
    Word16  wResolutionY;           /* Resolution by Y-axe */
    Word8   bFotoMetrics;             /* White pixel */
    Word8   bUnused;                  /* not used; for alignment purpose only */

    Word16  wAddX;
    Word16  wAddY;
} Tiger_ImageInfo;

#pragma pack()


typedef Tiger_ImageInfo TIGER_IMAGEINFO;
typedef Tiger_ImageInfo* LPTIGER_IMAGEINFO;

#if defined( _MSC_VER ) && ( _MSC_VER == 800 ) /* MSVC 1.5  */
  typedef Bool16 (_far _pascal _export *TImageOpen     )(Tiger_ImageInfo* lpImageInfo);
  typedef Int16  (_far _pascal _export *TImageRead     )(Word8* lpImage, Word16 wMaxSize);
  typedef Int16  (_far _pascal _export *TImageGetBlock )(Word8* lpBuff, Int16 fstLine, Int16 nLines);
  typedef Bool16 (_far _pascal _export *TImageClose    )(void);
#else
	#ifdef __cplusplus
	extern "C"  {
	#endif
		typedef Int16  (*TImageGetBlock )(Word8* lpBuff, Int16 fstLine, Int16 nLines);
		typedef Bool16 (*TImageOpen     )(Tiger_ImageInfo* lpImageInfo);
		typedef Int16  (*TImageRead     )(Word8* lpImage, Word16 wMaxSize);
		typedef Bool16 (*TImageClose    )(void);
	#ifdef __cplusplus
	}
	#endif
#endif

typedef  struct tagImageAccess
   {
      TImageOpen           f_op;
      TImageRead           f_re;  // may be NULL
      TImageGetBlock       f_gb;  // may be NULL
      TImageClose          f_cl;
   }  TImageAccess;

   // backward names compatibility
typedef TImageOpen   BWImgOpenFunc ;
typedef TImageRead   BWImgReadFunc ;
typedef TImageClose  BWImgCloseFunc;
typedef TImageAccess TigerImageFunc;


#endif
