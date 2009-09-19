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

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "config.h"

#include "cfcompat.h"

#ifndef WIN32

/* Minimal implementations of win32-functionality.
 * Eventually these should be rewritten in standard POSIX.
 *
 * At the end of the file are some helper functions, which should be used
 * on Windows too.
 */

#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <dlfcn.h>
#include <stdarg.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <malloc/malloc.h>
#define malloc_usable_size(a) malloc_size(a)

#elif defined(__FreeBSD__)
#include <malloc_np.h>

#else
#include <malloc.h>
#endif

#include "compat_defs.h"

CFCOMPAT_FUNC(int) HFILE_ERROR;

int LoadString(HINSTANCE hInstance, uint uID, char* lpBuffer, int nBufferMax) {
	return 0;
}

int CreateDirectory(const char *dir, void *dummy) {
	if (!mkdir(dir, 0755))
		return TRUE;
	else
		return FALSE;
}

uint32_t GetTempPath(uint32_t nBufferLength, char* lpBuffer) {
	strcpy(lpBuffer, "/tmp");
	return strlen(lpBuffer);
}

int RemoveDirectory(const char *d) {
	return rmdir(d);
}

void* GlobalAlloc(uint uFlags, int dwBytes) {
	if (uFlags & GMEM_ZEROINIT)
		return calloc(dwBytes, 1);
	return malloc(dwBytes);
}

HGLOBAL GlobalFree(void *f) {
	free(f);
	return NULL;
}

void* GlobalReAlloc(void* hMem, int dwBytes, uint uFlags) {
	return realloc(hMem, dwBytes); // Should init to zero on uFlags & GMEM_ZEROINIT.
}

int GetTempFileName(const char * lpPathName, const char * lpPrefixString, uint uUnique,
		char* lpTempFileName) {
	return -1;
}

int GetLastError() {
	return errno;
}

uint32_t GetModuleFileName(HMODULE hModule, char* lpFilename, uint32_t nSize) {
	lpFilename[0] = '.'; /* Currently all modules must be in the directory pumatest was run in. */
	lpFilename[1] = '\0';
	return 1;
}

Bool CloseHandle(HANDLE hObject) {
	return FALSE;
}

HANDLE CreateFile(const char * lpFileName, uint32_t dwDesiredAccess,
		uint32_t dwShareMode, void* lpSecurityAttributes,
		uint32_t dwCreationDisposition, uint32_t dwFlagsAndAttributes,
		HANDLE hTemplateFile) {
	return 0;
}

HWND FindWindow(const char * lpClassName, const char * lpWindowName) {
	return 0;
}

uint RegisterWindowMessage(const char * lpString) {
	return 0;
}

int _findclose(long handle);
long _findfirst(const char *filespec, struct _finddata_t *fileinfo);
int _findnext(long handle, struct _finddata_t *fileinfo);
long _tell(int handle) {
	return lseek(handle, 0, SEEK_CUR);
}

Bool GetComputerName(char* lpBuffer, long unsigned int *lpnSize) {
	strncpy(lpBuffer, "CompName", *lpnSize);
	*lpnSize = strlen(lpBuffer);
	return TRUE;
}

int32_t RegOpenKeyEx(HKEY hKey, const char * lpSubKey, uint32_t ulOptions,
		REGSAM samDesired, PHKEY phkResult) {
	return 0;
}

int32_t RegQueryValueEx(HKEY hKey, const char * lpValueName, LPDWORD lpReserved,
		LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData) {
	return 0;
}

Bool GetClientRect(HWND hWnd, LPRECT lpRect) {
	return 0;
}

Bool WritePrivateProfileString(const char * lpAppName, const char * lpKeyName,
		const char * lpString, const char * lpFileName) {
	return 0;
}

uint32_t GetPrivateProfileString(const char * lpAppName, const char * lpKeyName,
		const char * lpDefault, char* lpReturnedString, uint32_t nSize,
		const char * lpFileName) {
	return 0;
}

uint GetPrivateProfileInt(const char * lpAppName, const char * lpKeyName, int16_t nDefault,
		const char * lpFileName) {
	return 0;
}

int WideCharToMultiByte(uint CodePage, uint32_t dwFlags,
		const wchar_t *lpWideCharStr, int cchWideChar, char* lpMultiByteStr,
		int cbMultiByte, const char * lpDefaultChar, pBool lpUsedDefaultChar) {
	return 0;
}
Bool ShowWindow(HWND hWnd, int nCmdShow) {
	return 0;
}

long _filelength(int fd) {
	struct stat foo;
	if (fstat(fd, &foo) != 0) {
		return 0;
	}
	return foo.st_size;
}

long filelength(int fd) {
	return _filelength(fd);
}

long _msize(void *memblock) {
	return malloc_usable_size(memblock);
}

/* All uses in Cuneiform just check if the file exists. Ignoring all other
 * cases.
 */
int _access(const char *filename, int mode) {
	struct stat foo;
	assert(mode == 0);
	return stat(filename, &foo);
}

Bool SetWindowText(HWND hWnd, const char * lpString) {
	return 0;
}

int ReleaseDC(HWND hWnd, HDC hDC) {
	return 0;
}

Bool IsIconic(HWND hWnd) {
	return 0;
}

HDC GetDC(HWND hWnd) {
	return 0;
}

Bool EndPaint(HWND hWnd, ...) {
	return 1;
}

HDC BeginPaint(HWND hwnd, ...) {
	return 0;
}

LRESULT SendMessage(HWND hWnd, uint Msg, WPARAM wParam, LPARAM lParam) {
	return 0;
}

#ifndef __CYGWIN__
void strlwr(char *foo) {
	// FIXME: this is probably actually used somewhere.
}
#endif

HWND CreateWindow(const char * lpClassName, const char * lpWindowName, uint32_t dwStyle,
		int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
		HINSTANCE hInstance, pvoid lpParam) {
	return (HWND) 55;
}

HGDIOBJ SelectObject(HDC hdc, HGDIOBJ hgdiobj) {
	return 0;
}

char* lstrcat(char* lpString1, char* lpString2) {
	return strcat(lpString1, lpString2);
}

int lstrlen(const char * lpString) {
	return strlen(lpString);
}

int lstrcmp(const char * lpString1, const char * lpString2) {
	return strcmp(lpString1, lpString2);
}

char* lstrcpy(char* lpString1, const char * lpString2) {
	return strcpy(lpString1, lpString2);
}

int wsprintf(char* lpOut, const char * lpFmt, ...) {
	char buffer[256];
	int ret;
	va_list args;
	va_start (args, lpFmt);
	ret = vsprintf(lpOut, lpFmt, args);
	if (ret < 0)
		perror(buffer);
	va_end(args);
	return ret;
}

int lstrcmpi(const char * lpString1, const char * lpString2) {
	return strcasecmp(lpString1, lpString2);
}

Bool DeleteObject(HGDIOBJ hObject) {
	return 0;
}

HWND GetFocus() {
	return NULL;
}

int MessageBox(HWND hWnd, const char * lpText, const char * lpCaption, uint uType) {
	fprintf(stderr, "MessageBox %s: %s\n", lpCaption, lpText);
	return 0;
}

int WINAPI GlobalSize(HGLOBAL hMem) {
	return 0;
}

pvoid GlobalLock(HGLOBAL hMem) {
	return NULL;
}

Bool GlobalUnlock(HGLOBAL hMem) {
	return 0;
}

Bool IsBadWritePtr(pvoid lp, int ucb) {
	return 0;
}

void OutputDebugString(const char * lpOutputString) {

}

Bool SetRect(LPRECT lprc, int xLeft, int yTop, int xRight, int yBottom) {
	lprc->left = xLeft;
	lprc->right = xRight;
	lprc->top = yTop;
	lprc->bottom = yBottom;
	return TRUE;
}

Bool PtInRect(const RECT *lprc, POINT pt) {
	if (pt.x >= lprc->left && pt.x < lprc->right && pt.y >= lprc->top && pt.y
			< lprc->bottom)
		return TRUE;
	return FALSE;
}

/* This is only called from creatertf.cpp. It does not use lprcDst at all so
 * we do not need to calculate it.
 */

Bool IntersectRect(LPRECT lprcDst, const RECT *lprcSrc1, const RECT *lprcSrc2) {
	if (lprcSrc1->left > lprcSrc2->right || lprcSrc1->right < lprcSrc2->left
			|| lprcSrc1->top > lprcSrc2->bottom || lprcSrc1->bottom
			< lprcSrc2->top)
		return FALSE;
	return TRUE;
}

Bool UnionRect(LPRECT lprcDst, const RECT *lprcSrc1, const RECT *lprcSrc2) {
	if (lprcSrc1->left - lprcSrc1->right == 0 || lprcSrc1->top
			- lprcSrc1->bottom == 0) {
		lprcDst->left = lprcSrc2->left;
		lprcDst->right = lprcSrc2->right;
		lprcDst->top = lprcSrc2->top;
		lprcDst->bottom = lprcSrc2->bottom;
		return TRUE;
	}
	if (lprcSrc2->left - lprcSrc2->right == 0 || lprcSrc2->top
			- lprcSrc2->bottom == 0) {
		lprcDst->left = lprcSrc1->left;
		lprcDst->right = lprcSrc1->right;
		lprcDst->top = lprcSrc1->top;
		lprcDst->bottom = lprcSrc1->bottom;
		return TRUE;
	}

	lprcDst->left = lprcSrc1->left < lprcSrc2->left ? lprcSrc1->left
			: lprcSrc2->left;
	lprcDst->right = lprcSrc1->right > lprcSrc2->right ? lprcSrc1->right
			: lprcSrc2->right;
	lprcDst->top = lprcSrc1->top < lprcSrc2->top ? lprcSrc1->top
			: lprcSrc2->top;
	lprcDst->bottom = lprcSrc1->bottom > lprcSrc2->bottom ? lprcSrc1->bottom
			: lprcSrc2->bottom;

	return 0;
}

HWND GetActiveWindow() {
	return NULL;
}

HFONT CreateFont(int nHeight, int nWidth, int nEscapement, int nOrientation,
		int fnWeight, uint32_t fdwItalic, uint32_t fdwUnderline,
		uint32_t fdwStrikeOut, uint32_t fdwCharSet,
		uint32_t fdwOutputPrecision, uint32_t fdwClipPrecision,
		uint32_t fdwQuality, uint32_t fdwPitchAndFamily, const char * lpszFace) {
	return 0;
}

Bool GetTextExtentPoint32(HDC hdc, const char * lpString, int c, LPSIZE lpSize) {
	return 0;
}

Bool EnumWindows(WNDENUMPROC lpEnumFunc, LPARAM lParam) {
	return 0;
}

int GetWindowText(HWND hWnd, char* lpString, int nMaxCount) {
	return 0;
}

HMODULE LoadLibrary(const char * lpFileName) {
	return dlopen(lpFileName, RTLD_LAZY);
}

Bool FreeLibrary(HMODULE hModule) {
	return dlclose(hModule);
}

void* GetProcAddress(HMODULE hModule, const char * lpProcName) {
	return dlsym(hModule, lpProcName);
}

HGDIOBJ GetStockObject(int fnObject) {
	return -1;
}

Bool IsWindowVisible(HWND hWnd) {
	return 0;
}

LRESULT DefWindowProc(HWND hWnd, uint Msg, WPARAM wParam, LPARAM lParam) {
	return 0;
}

int32_t GetWindowLong(HWND hWnd, int nIndex) {
	return 0;
}

Bool RegisterClass(const WNDCLASS *lpWndClass) {
	return 0;
}

HMODULE GetModuleHandle(const char * lpModuleName) {
	return NULL;
}

HICON LoadIcon(HINSTANCE hInstance, const char * lpIconName) {
	return NULL;
}

int LoadCursor(HINSTANCE hInstance, const char * lpCursorName) {
	return 0;
}

Bool Rectangle(HDC hdc, int nLeftRect, int nTopRect, int nRightRect,
		int nBottomRect) {
	return 0;
}

/* String to uppercase. */

char* _strupr(char*s) {
	return s;
}

#else /* WIN32 */

#include<io.h>
#include<direct.h>
#include"cfcompat.h"
#include "cttypes.h"
#include <windows.h>
#include <malloc.h>

static HMODULE thismod;

CFCOMPAT_FUNC(Bool)
WINAPI DllMain(HINSTANCE hinstDLL, uint32_t fdwReason, pvoid lpvReserved) {
	thismod = (HMODULE) hinstDLL;
	return TRUE;
}

CFCOMPAT_FUNC(char*)
mkdtemp(char *tmpl) {
	static const char charset[] =
	"=#abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	static const unsigned int charset_len = sizeof(charset) - 1;

	const int len = strlen (tmpl);
	char* x_tail = tmpl + len - 6;
	LARGE_INTEGER rand_seed;
	uint64_t value = rand_seed.QuadPart ^ GetCurrentThreadId();
	unsigned int cnt = 0;

	if (len < 6)
	return NULL;
	if(memcmp(x_tail, "XXXXXX", 6))
	return NULL;

	QueryPerformanceCounter(&rand_seed);

	do {
		uint64_t val = value;
		char* x_char = x_tail;
		while(*x_char) {
			*x_char++ = charset[val % charset_len];
			val /= charset_len;
		}
		if (CreateDirectory (tmpl, NULL))
		return tmpl;
		if (ERROR_ALREADY_EXISTS != GetLastError())
		return NULL;
		value += 65537;
		++cnt;
	}while (cnt < TMP_MAX);

	return NULL;
}

#endif /* WIN32 */

/* General helper functions. */

static const char *separator = "/"; /* Yes, on Windows too. */

#ifdef WIN32

static void get_install_path(char *path) {
	const int psize = 128;
	char modulepath[128]; /* MSVC fails when psize is put in these brackets. */
	char fname[50];
	char suffix[10];

	GetModuleFileName(thismod, modulepath, psize);
	winpath_to_internal(modulepath);
	split_path(modulepath, path, fname, suffix);
}

#else

static void get_install_path(char *path) {
	strcat(path, INSTALL_DATADIR);
}

#endif

/* We try to locate data files in two locations:
 *
 * 1. At the directory pointed to by environment variable CF_DATADIR
 * 2. At a platform-dependent install location
 *
 * Caller's responsibility is to ensure *e1 and *e2 are large enough.
 *
 */

static void build_name_estimates(const char *base_name, char *env_name,
		char *prefix_name) {
	const char *env_prefix;
	const char *varname = "CF_DATADIR";
	int len = 0;

	env_name[0] = '\0';
	prefix_name[0] = '\0';

	env_prefix = getenv(varname);
	if (env_prefix)
		len = strlen(env_prefix);
	if (len > 0) {
		strcat(env_name, env_prefix);
		if (strcmp(env_prefix + len - 1, separator) != 0) {
			strcat(env_name, separator);
		}
		strcat(env_name, base_name);
	}

	get_install_path(prefix_name);
	if (strlen(prefix_name) > 0) {
		strcat(prefix_name, separator);
		strcat(prefix_name, base_name);
	}
}

CFCOMPAT_FUNC(int)
open_data_file(const char *basename, int mode) {
	char ename[1024];
	char pname[1024];
	int i;

	build_name_estimates(basename, ename, pname);
	i = open(ename, mode);
	if(i != -1)
	return i;
	return open(pname, mode);
}

CFCOMPAT_FUNC(int)
data_file_exists(const char *basename) {
	char ename[1024];
	char pname[1024];

	build_name_estimates(basename, ename, pname);
	if(_access(ename, 0) == 0)
	return 0;
	return _access(pname, 0);
}

/* Split a file name in three: path, base file name, and extension.
 * All internal file names use / as path separator, even on Windows.
 */
CFCOMPAT_FUNC(void)
split_path(const char *fname, char *file_path, char *basename, char *ext) {
	int last_path = -1;
	int suff = -1;
	size_t l = strlen(fname);
	int path_end, base_start, base_end, ext_start;
	size_t i;

	file_path[0] = '\0';
	basename[0] = '\0';

	for (i = 0; i < l; i++) {
		if (fname[i] == '.')
		suff = i;
		if (fname[i] == '/')
		last_path = i;
	}

	path_end = 0;
	base_start = 0;
	if (last_path == 0) { // File in root.
		path_end = 1;
		base_start = 1;
	}
	if (last_path > 0) {
		path_end = last_path;
		base_start = last_path + 1;
	}

	if (suff > last_path) {
		ext_start = suff + 1;
		base_end = suff;
	} else {
		base_end = ext_start = l;
	}

	memcpy(file_path, fname, path_end);
	file_path[path_end] = '\0';
	memcpy(basename, fname + base_start, base_end - base_start);
	basename[base_end - base_start] = '\0';
	memcpy(ext, fname + ext_start, l - ext_start);
	ext[l - ext_start] = '\0';
}

CFCOMPAT_FUNC(void)
make_path(char *opath, const char *dir, const char *basename, const char *ext) {
	const char dirsep = '/';
	const char *dirseps = "/";
	opath[0] = '\0';

	if(dir) {
		strcat(opath, dir);
		if(opath[strlen(opath) - 1] != dirsep) {
			strcat(opath, dirseps);
		}
	}

	if(basename)
	strcat(opath, basename);

	if(ext) {
		if(ext[0] != '.')
		strcat(opath, ".");
		strcat(opath, ext);
	}
}

/**
 * Convert backslashes to slashes. No-op on UNIX.
 */
CFCOMPAT_FUNC(void)
winpath_to_internal(char *p) {
#if WIN32
	int i;
	for(i=0; p[i] != '\0'; i++) {
		if(p[i] == '\\')
		p[i] = '/';
	}
#endif
}

/* Get current working directory. */

CFCOMPAT_FUNC(unsigned int) curr_dir(unsigned int bsize, char* buf) {
#ifdef _MSC_VER
	_getcwd(buf, bsize);
#else
	getcwd(buf, bsize);
#endif
	winpath_to_internal(buf);
	return strlen(buf);
}

#if WIN32

#define BUFSIZE 100

CFCOMPAT_FUNC (FILE*)
create_temp_file(void) {
	char temppath[BUFSIZE];
	char tempfname[BUFSIZE];
	uint32_t retval;

	retval = GetTempPath(BUFSIZE, temppath);
	if(retval >= BUFSIZE || retval == 0)
	return NULL;

	if(GetTempFileName(temppath, "CF", 0, tempfname) == 0)
	return NULL;

	return fopen(tempfname, "w+bD");
}

#else

CFCOMPAT_FUNC (FILE*)
create_temp_file(void) {
	FILE *tmp_file;
	int tmp_fd;
	char* pattrn = malloc(100);
	strcpy(pattrn, "/tmp/CF.XXXXXX");

	tmp_fd = mkstemp(pattrn);

	/* unlink file immediatly, it gets unlinked when file descriptor is closed */
	unlink(pattrn);
	free(pattrn);

	if (tmp_fd == -1)
	return NULL;

	if (!(tmp_file = fdopen(tmp_fd, "w+b"))) {
		return NULL;
	}

	return tmp_file;
}
#endif
