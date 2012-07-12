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

#ifndef WIN32
#include "minmax.h"
#include "filestuff.h"
#else
#include "windows.h"
#endif

#include "cttypes.h" /* Most type definitions are here. */
#include <errno.h>
#include <time.h>

#ifndef WIN32

typedef void* HINSTANCE;
typedef void* HMODULE;
typedef void* HWND;
typedef void* HGLOBAL;
typedef void* WNDPROC;

typedef int HDC;
typedef int HFILE;

extern int HFILE_ERROR;

typedef struct tagRECT
{
        int32_t left;
        int32_t right;
        int32_t top;
        int32_t bottom;
        bool operator==(const tagRECT& r) const {
            return left == r.left && right == r.right && top == r.top && bottom == r.bottom;
        }
} RECT;

typedef RECT* LPRECT;
typedef uint32_t COLORREF;

typedef struct
{
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

typedef int HKEY;

#ifndef WPARAM
#define WPARAM int
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

/*
 * A bunch of windows DLL initialisation values. I don't know the
 * real values of these, so I just put in random values. They are not
 * used anyway, so no harm.
 */

#ifndef WM_DESTROY
#define WM_DESTROY 96
#endif

#ifndef WM_SIZE
#define WM_SIZE 10
#endif

#ifndef WM_PAINT
#define WM_PAINT 73
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

#ifndef MAX_COMPUTERNAME_LENGTH
#define MAX_COMPUTERNAME_LENGTH 100
#endif

#ifndef MB_SYSTEMMODAL
#define MB_SYSTEMMODAL 773
#endif

#ifndef ERROR_ALREADY_EXISTS
#define ERROR_ALREADY_EXISTS EEXIST
#endif

#endif

#endif
