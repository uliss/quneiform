/*
Copyright (c) 2008, Jussi Pakkanen

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
#include "filestuff.h"
#include "cttypes.h" /* Most type definitions are here. */
#include "minmax.h"

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef HANDLE
#define HANDLE void*
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef APIENTRY /* Not currently used in Linux. */
#define APIENTRY
#endif
/*
#ifndef WORD
#define WORD short int
#endif
*/
/*typedef unsigned short int              WORD;*/


typedef int (* _CRT_ALLOC_HOOK) (int, void *, int, int, long, const char *, int);


#ifndef HINSTANCE
#define HINSTANCE void*
#endif

#ifndef HMODULE
#define HMODULE void*
#endif

#ifndef HWND
#define HWND unsigned int
#endif

#define _ASSERT assert

#ifndef LPSTR
#define LPSTR char*
#endif

#ifndef LPCSTR
#define LPCSTR const char*
#endif

#ifndef LPCWSTR
#define LPCWSTR const char*
#endif

#ifndef HDC
#define HDC int
#endif

#ifndef HGDIOBJ
#define HGDIOBJ int
#endif

#ifndef HUGE
#define HUGE
#endif

#ifndef UINT
#define UINT unsigned int
#endif

#ifndef INT
#define INT int
#endif

#ifndef LONG
#define LONG long
#endif

#ifndef ULONG
#define ULONG unsigned long
#endif

#ifndef DECLARE_HANDLE
#define DECLARE_HANDLE(a)
#endif

#ifndef WINAPI
#define WINAPI
#endif

extern int HFILE_ERROR;
typedef int HFILE;

typedef struct tagRECT {
    LONG left;
    LONG right;
    LONG top;
    LONG bottom;
} RECT;

typedef RECT* LPRECT;
typedef DWORD COLORREF;
typedef DWORD *LPCOLORREF;

#ifndef HGLOBAL
#define HGLOBAL void*
#endif

#ifndef HMETAFILE
#define HMETAFILE void*
#endif

typedef struct {
     char * lpszClassName;
     HINSTANCE hInstance;
     void * lpfnWndProc;
     unsigned int style;
     int hbrBackground;
     int hCursor;
     int hIcon;
     char * lpszMenuName;
     char *cbClsExtra;
     char *cbWndExtra;
} WNDCLASS;

typedef struct tagBITMAPINFOHEADER{
  DWORD  biSize; 
  LONG   biWidth; 
  LONG   biHeight; 
  int   biPlanes;    /* Was WORD */
  int   biBitCount;  /* Was WORD */
  DWORD  biCompression; 
  DWORD  biSizeImage; 
  LONG   biXPelsPerMeter; 
  LONG   biYPelsPerMeter; 
  DWORD  biClrUsed; 
  DWORD  biClrImportant; 
} BITMAPINFOHEADER, *PBITMAPINFOHEADER; 

typedef struct tagBITMAPCOREHEADER {
  DWORD   bcSize; 
  WORD    bcWidth; 
  WORD    bcHeight; 
  WORD    bcPlanes; 
  WORD    bcBitCount; 
} BITMAPCOREHEADER, *PBITMAPCOREHEADER; 

struct _finddata_t {
    unsigned    attrib;
    time_t    time_create; /*-1forFATfilesystems*/
    time_t    time_access; /*-1forFATfilesystems */
    time_t    time_write;
    unsigned int  size;
    char    name[260];
};

typedef struct tagRGBQUAD {
  BYTE    rgbBlue; 
  BYTE    rgbGreen; 
  BYTE    rgbRed; 
  BYTE    rgbReserved; 
} RGBQUAD; 

typedef struct tagBITMAPINFO { 
  BITMAPINFOHEADER bmiHeader; 
  RGBQUAD          bmiColors[1]; 
} BITMAPINFO, *PBITMAPINFO; 

typedef struct tagRGBTRIPLE { 
  BYTE rgbtBlue; 
  BYTE rgbtGreen; 
  BYTE rgbtRed; 
} RGBTRIPLE; 

typedef BITMAPCOREHEADER* LPBITMAPCOREHEADER;
typedef BITMAPINFOHEADER* LPBITMAPINFOHEADER;
typedef BITMAPINFO* LPBITMAPINFO;

typedef struct tagMETAHEADER { 
  WORD  mtType; 
  WORD  mtHeaderSize; 
  WORD  mtVersion; 
  DWORD mtSize; 
  WORD  mtNoObjects; 
  DWORD mtMaxRecord; 
  WORD  mtNoParameters; 
} METAHEADER, *PMETAHEADER;

typedef struct tagPALETTEENTRY { 
  BYTE peRed; 
  BYTE peGreen; 
  BYTE peBlue; 
  BYTE peFlags; 
} PALETTEENTRY; 

typedef struct tagLOGPALETTE { 
  WORD         palVersion; 
  WORD         palNumEntries; 
  PALETTEENTRY palPalEntry[1]; 
} LOGPALETTE;

/*
typedef struct {
    int x;
} HPALETTE;
*/

typedef BOOL(*WNDENUMPROC)(HWND, int);

typedef int HPALETTE;

/*
typedef struct tagPAINTSTRUCT { 
  HDC  hdc; 
  BOOL fErase; 
  RECT rcPaint; 
  BOOL fRestore; 
  BOOL fIncUpdate; 
  unsigned char rgbReserved[32]; 
} PAINTSTRUCT, *PPAINTSTRUCT; 
*/

typedef struct tagPOINT { 
  LONG x; 
  LONG y; 
} POINT, *PPOINT;

typedef struct  tagSIZE{
    LONG cx;    
    LONG cy;
} SIZE;

typedef struct tagBITMAPFILEHEADER { 
  WORD    bfType; 
  DWORD   bfSize; 
  WORD    bfReserved1; 
  WORD    bfReserved2; 
  DWORD   bfOffBits; 
} BITMAPFILEHEADER, *PBITMAPFILEHEADER; 

typedef struct tagCOPYDATASTRUCT {
    ULONG dwData;
    DWORD cbData;
    PVOID lpData;
} COPYDATASTRUCT, *PCOPYDATASTRUCT;

typedef POINT* LPPOINT;
typedef SIZE* LPSIZE;

typedef int HKEY;

typedef int HFONT;

typedef int * PHKEY;
typedef int REGSAM;

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

#ifndef LPTSTR
#define LPTSTR char *
#endif

#ifndef CREATE_NEW
#define CREATE_NEW 2
#endif

#ifndef LPCTSTR
#define LPCTSTR const char *
#endif

#ifndef RGB
#define RGB(r,g,b) ((Word32)(((Word8)(r)|((Word16)((Word8)(g))<<8))|(((Word32)(Word8)(b))<<16)))
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

#include "split.h"
#include "winfuncs.h"
/* A bunch of windows DLL initialisation values. I don't know the
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

#ifndef PAINTSTRUCT
#define PAINTSTRUCT int
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
#define GHND 666
#endif

#ifndef GMEM_DDESHARE
#define GMEM_DDESHARE 777
#endif

#ifndef GMEM_DISCARDABLE
#define GMEM_DISCARDABLE 888
#endif

#ifndef GMEM_FIXED
#define GMEM_FIXED 999
#endif

#ifndef GMEM_LOWER
#define GMEM_LOWER 0000
#endif

#ifndef GMEM_MOVEABLE
#define GMEM_MOVEABLE 1111
#endif

#ifndef GMEM_NOCOMPACT
#define GMEM_NOCOMPACT 2222
#endif

#ifndef GMEM_NODISCARD
#define GMEM_NODISCARD 3333
#endif

#ifndef GMEM_NOT_BANKED
#define GMEM_NOT_BANKED 4444
#endif

#ifndef GMEM_NOTIFY
#define GMEM_NOTIFY 5555
#endif

#ifndef GMEM_SHARE
#define GMEM_SHARE 6666
#endif

#ifndef GMEM_ZEROINIT
#define GMEM_ZEROINIT 7777
#endif

#ifndef GPTR
#define GPTR 8888
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
#define ERROR_ALREADY_EXISTS 1000
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

#ifdef __cplusplus
    }
#endif
    
#endif
