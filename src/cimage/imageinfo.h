/*
 * imageinfo.h
 *
 *  Created on: 10.10.2009
 *      Author: uliss
 */

#ifndef IMAGEINFO_H_
#define IMAGEINFO_H_

#include "cttypes.h"

typedef struct {
	uint16_t wImageHeight;
	uint16_t wImageWidth;
	uint16_t wImageByteWidth;
	uint16_t wImageDisplacement;
	uint16_t wResolutionX;
	uint16_t wResolutionY;
	uchar bFotoMetrics;
	uchar bUnused;
	uint16_t wAddX;
	uint16_t wAddY;
} CIMAGE_ImageInfo, *PCIMAGE_ImageInfo, **PPCIMAGE_ImageInfo;

typedef Bool16 (*PCIMAGE_Callback_ImageOpen)(PCIMAGE_ImageInfo);
typedef uint16_t (*PCIMAGE_Callback_ImageRead)(pchar, uint16_t);
typedef Bool16 (*PCIMAGE_Callback_ImageClose)(void);

typedef struct {
	PCIMAGE_Callback_ImageOpen CIMAGE_ImageOpen;
	PCIMAGE_Callback_ImageRead CIMAGE_ImageRead;
	PCIMAGE_Callback_ImageClose CIMAGE_ImageClose;
} CIMAGEIMAGECALLBACK, *PCIMAGEIMAGECALLBACK;

//#else
//#include "puma/puma.h"
//typedef PUMA_ImageInfo CIMAGE_ImageInfo, *PCIMAGE_ImageInfo, **PPCIMAGE_ImageInfo;
//typedef PUMA_CallBack_ImageOpen PCIMAGE_Callback_ImageOpen;
//typedef PUMA_Callback_ImageRead PCIMAGE_Callback_ImageRead;
//typedef PUMA_Callback_ImageClose PCIMAGE_Callback_ImageClose;
//typedef PUMAIMAGECALLBACK CIMAGEIMAGECALLBACK, * PCIMAGEIMAGECALLBACK;
//#endif

typedef struct CIMAGEInfoDataInGet {
	uint32_t dwX;
	uint32_t dwY;
	uint32_t dwWidth;
	uint32_t dwHeight;
	uint16_t wByteWidth;
	uchar Reserved;
	uchar MaskFlag;
} CIMAGE_InfoDataInGet, *PCIMAGE_InfoDataInGet;

typedef struct CIMAGEInfoDataOutGet {
	uint32_t dwWidth;
	uint32_t dwHeight;
	uint16_t wByteWidth;
	uint16_t byBit;
	uint32_t wBlackBit;
	puchar lpData;
} CIMAGE_InfoDataOutGet, *PCIMAGE_InfoDataOutGet, **PPCIMAGE_InfoDataOutGet;

typedef struct CIMAGEInfoDataInReplace {
	uint16_t byBit;
	uint32_t dwX;
	uint32_t dwY;
	uint32_t dwWidth;
	uint32_t dwHeight;
	uint16_t wByteWidth;
	puchar lpData;
	uchar Reserved;
	uint16_t wReserved;
	uchar MaskFlag;
} CIMAGE_InfoDataInReplace, *PCIMAGE_InfoDataInReplace;

typedef struct tagCIMAGERECT {
	int32_t dwX;
	int32_t dwY;
	uint32_t dwWidth;
	uint32_t dwHeight;
} CIMAGE_Rect, *PCIMAGE_Rect, **PPCIMAGE_Rect;

typedef struct tagCIMAGEBITMAPINFOHEADER {
	uint32_t biSize;
	int32_t biWidth;
	int32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t biXPelsPerMeter;
	int32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} CIMAGEBITMAPINFOHEADER, *PCIMAGEBITMAPINFOHEADER;

typedef struct tagCIMAGERGBQUAD {
	uchar rgbBlue;
	uchar rgbGreen;
	uchar rgbRed;
	uchar rgbReserved;
} CIMAGERGBQUAD, *PCIMAGERGBQUAD;

#endif
