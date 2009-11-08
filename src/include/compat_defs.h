/*
 Copyright (c) 2008, 2009 Jussi Pakkanen

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

/**
 * \file
 * Typedefs for the various win32-only variable types.
 * Only defines those types that are currently undefined. Thus this
 * can be included after other headers without redefinition errors.
 *
 * Most of these are also in c_types.h and/or cttypes.h. I added them
 * here so as to make changes minimal and isolated. Eventually this
 * header should be removed altogether.
 */

#ifndef compat_typedefs_h_
#define compat_typedefs_h_

#include <time.h>
#include "minmax.h"
#include "rect.h"
#include "filestuff.h"
#include "cttypes.h" /* Most type definitions are here. */
#include <errno.h>

typedef int (* _CRT_ALLOC_HOOK)(int, void *, int, int, long, const char *, int);

#ifndef DECLARE_HANDLE
#define DECLARE_HANDLE(a)
#endif

extern int HFILE_ERROR;

#ifdef WIN32
#include <windows.h>
#else

#define GetGValue(rgb) ((unsigned char) (((uint16_t) (rgb)) >> 8))
#define GetBValue(rgb) ((unsigned char) ((rgb) >> 16))
#define GetRValue(rgb) ((unsigned char) (rgb))

typedef void *HMETAFILE;
typedef void *HMODULE;
typedef void *HGLOBAL;
typedef void *HINSTANCE;
typedef void *HICON;
typedef void *HWND;
typedef int HDC;
typedef int HGDIOBJ;
typedef int HFILE;
typedef int HKEY;
typedef int HFONT;
typedef int *PHKEY;
typedef int REGSAM;
typedef uint32_t COLORREF;
typedef uint32_t *LPCOLORREF;

#ifndef APIENTRY /* Not currently used in Linux. */
#define APIENTRY
#endif

#ifndef WINAPI
#define WINAPI
#endif

#pragma pack(push, 2)

typedef struct tagBITMAPINFOHEADER
{
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
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagBITMAPCOREHEADER
{
    uint32_t bcSize;
    uint16_t bcWidth;
    uint16_t bcHeight;
    uint16_t bcPlanes;
    uint16_t bcBitCount;
} BITMAPCOREHEADER, *PBITMAPCOREHEADER;

#pragma pack(pop)

struct _finddata_t
{
    unsigned attrib;
    time_t time_create; /*-1forFATfilesystems*/
    time_t time_access; /*-1forFATfilesystems */
    time_t time_write;
    unsigned int size;
    char name[260];
};

typedef struct tagRGBQUAD
{
    uchar rgbBlue;
    uchar rgbGreen;
    uchar rgbRed;
    uchar rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO
{
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[1];
} BITMAPINFO, *PBITMAPINFO;

typedef struct tagRGBTRIPLE
{
    uchar rgbtBlue;
    uchar rgbtGreen;
    uchar rgbtRed;
} RGBTRIPLE;

typedef BITMAPCOREHEADER* LPBITMAPCOREHEADER;
typedef BITMAPINFOHEADER* LPBITMAPINFOHEADER;
typedef BITMAPINFO* LPBITMAPINFO;

typedef struct tagMETAHEADER
{
    uint16_t mtType;
    uint16_t mtHeaderSize;
    uint16_t mtVersion;
    uint32_t mtSize;
    uint16_t mtNoObjects;
    uint32_t mtMaxRecord;
    uint16_t mtNoParameters;
} METAHEADER, *PMETAHEADER;

typedef struct tagPALETTEENTRY
{
    uchar peRed;
    uchar peGreen;
    uchar peBlue;
    uchar peFlags;
} PALETTEENTRY;

typedef struct tagLOGPALETTE
{
    uint16_t palVersion;
    uint16_t palNumEntries;
    PALETTEENTRY palPalEntry[1];
} LOGPALETTE;

typedef Bool(*WNDENUMPROC)(HWND, int);

typedef int HPALETTE;

typedef struct tagPAINTSTRUCT
{
    HDC hdc;
    Bool fErase;
    CIF::Rect rcPaint;
    Bool fRestore;
    Bool fIncUpdate;
    unsigned char rgbReserved[32];
} PAINTSTRUCT, *PPAINTSTRUCT;

#pragma pack(push, 2)
typedef struct tagBITMAPFILEHEADER
{
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

#pragma pack(pop)

typedef struct tagCOPYDATASTRUCT
{
    ulong dwData;
    uint32_t cbData;
    pvoid lpData;
} COPYDATASTRUCT, *PCOPYDATASTRUCT;

#endif

#ifndef _O_BINARY
#define _O_BINARY O_BINARY
#endif

#ifndef _O_CREAT
#define _O_CREAT O_CREAT
#endif

#ifndef _O_EXCL
#define _O_EXCL O_EXCL
#endif

#ifndef _O_RDONLY
#define _O_RDONLY O_RDONLY
#endif

#ifndef _O_RDWR
#define _O_RDWR O_RDWR
#endif

#ifndef _S_IREAD
#define _S_IREAD S_IREAD
#endif

#ifndef _S_IWRITE
#define _S_IWRITE S_IWRITE
#endif

#ifndef WPARAM
#define WPARAM int
#endif

#ifndef COLORONCOLOR
#define COLORONCOLOR 31
#endif

#ifndef DIB_RGB_COLORS
#define DIB_RGB_COLORS 85
#endif

#ifndef SRCCOPY
#define SRCCOPY 482
#endif

#ifndef LPARAM
#define LPARAM int
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef O_TEXT
#define O_TEXT 0
#endif

#ifndef CREATE_NEW
#define CREATE_NEW 2
#endif

#ifndef RGB
#define RGB(r,g,b) ((uint32_t)(((uchar)(r)|((uint16_t)((uchar)(g))<<8))|(((uint32_t)(uchar)(b))<<16)))
#endif

#ifndef GENERIC_READ
#define GENERIC_READ 1
#endif

#ifndef GENERIC_WRITE
#define GENERIC_WRITE 2
#endif

#ifndef FILE_ATTRIBUTE_NORMAL
#define FILE_ATTRIBUTE_NORMAL 4
#endif

#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE NULL
#endif

#ifndef LRESULT
#define LRESULT int
#endif

#define HMENU int

/*
 * A bunch of windows DLL initialisation values. I don't know the
 * real values of these, so I just put in random values. They are not
 * used anyway, so no harm.
 */

#ifndef DLL_PROCESS_ATTACH
#define DLL_PROCESS_ATTACH 1
#endif

#ifndef DLL_PROCESS_DETACH
#define DLL_PROCESS_DETACH 2
#endif

#ifndef DLL_THREAD_ATTACH
#define DLL_THREAD_ATTACH 3
#endif

#ifndef DLL_THREAD_DETACH
#define DLL_THREAD_DETACH 4
#endif

#ifndef GWL_HINSTANCE
#define GWL_HINSTANCE 42
#endif

#ifndef WS_OVERLAPPEDWINDOW
#define WS_OVERLAPPEDWINDOW 6
#endif

#ifndef GRAY_BRUSH
#define GRAY_BRUSH 1
#endif

#ifndef NULL_BRUSH
#define NULL_BRUSH 0
#endif

#ifndef WM_DESTROY
#define WM_DESTROY 96
#endif

#ifndef WM_SIZE
#define WM_SIZE 10
#endif

#ifndef WM_PAINT
#define WM_PAINT 73
#endif

#ifndef WHITE_BRUSH
#define WHITE_BRUSH 939
#endif

#ifndef IDC_ARROW
#define IDC_ARROW 38
#endif

#ifndef IDI_ASTERISK
#define IDI_ASTERISK 939
#endif

#ifndef GHND
#define GHND 0x42
#endif

#ifndef GMEM_DDESHARE
#define GMEM_DDESHARE 0
#endif

#ifndef GMEM_DISCARDABLE
#define GMEM_DISCARDABLE 0
#endif

#ifndef GMEM_FIXED
#define GMEM_FIXED 0
#endif

#ifndef GMEM_LOWER
#define GMEM_LOWER 0
#endif

#ifndef GMEM_MOVEABLE
#define GMEM_MOVEABLE 0x02
#endif

#ifndef GMEM_NOCOMPACT
#define GMEM_NOCOMPACT 0
#endif

#ifndef GMEM_NODISCARD
#define GMEM_NODISCARD 0
#endif

#ifndef GMEM_NOT_BANKED
#define GMEM_NOT_BANKED 0
#endif

#ifndef GMEM_NOTIFY
#define GMEM_NOTIFY 0
#endif

#ifndef GMEM_SHARE
#define GMEM_SHARE 0
#endif

#ifndef GMEM_ZEROINIT
#define GMEM_ZEROINIT 0x0040
#endif

#ifndef GPTR
#define GPTR 0x40
#endif

#ifndef DRIVE_FIXED
#define DRIVE_FIXED 44
#endif

#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS 0
#endif

#ifndef HKEY_CURRENT_USER
#define HKEY_CURRENT_USER 69
#endif

#ifndef KEY_ALL_ACCESS
#define KEY_ALL_ACCESS 100
#endif

#ifndef CP_ACP
#define CP_ACP 593
#endif

#ifndef MAX_COMPUTERNAME_LENGTH
#define MAX_COMPUTERNAME_LENGTH 100
#endif

#ifndef MB_SYSTEMMODAL
#define MB_SYSTEMMODAL 773
#endif

#ifndef ERROR_ALREADY_EXISTS
#define ERROR_ALREADY_EXISTS EEXIST
#endif

#ifndef OUT_DEFAULT_PRECIS
#define OUT_DEFAULT_PRECIS 253
#endif

#ifndef DEFAULT_QUALITY
#define DEFAULT_QUALITY 49
#endif

#ifndef CLIP_DEFAULT_PRECIS
#define CLIP_DEFAULT_PRECIS 7
#endif

#ifndef WM_COPYDATA
#define WM_COPYDATA 30
#endif

#ifndef MB_ICONSTOP
#define MB_ICONSTOP 888
#endif

#include "cfcompat.h"

#endif
