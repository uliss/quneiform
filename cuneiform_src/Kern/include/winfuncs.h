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

#ifndef WINFUNCS_H_
#define WINFUNCS_H_
int LoadString(HINSTANCE hInstance, UINT uID, LPTSTR lpBuffer, int nBufferMax);
/*
void _makepath(char *path, const char *drive, const char *dir,
               const char *fname, const char *ext);
void _splitpath(const char *path, char *drive, char *dir,
                char *fname, char *ext);
*/
int CreateDirectory(const char *dir, void *dummy);
char *_fullpath(char *absPath, const char *relPath, int maxLength);
void CopyMemory(void *Destination, const void* Source, int length); // memcpy
DWORD GetTempPath(DWORD nBufferLength, LPTSTR lpBuffer);
int RemoveDirectory(const char *d);
void* GlobalAlloc(UINT uFlags, int dwBytes);
void GlobalFree(void *f);
void* GlobalReAlloc(void* hMem, int dwBytes, UINT uFlags);
int GetTempFileName(LPCTSTR lpPathName, LPCTSTR lpPrefixString,
                    UINT uUnique, LPTSTR lpTempFileName);
int GetLastError();

DWORD GetModuleFileName(HMODULE hModule, LPTSTR lpFilename, DWORD nSize);
BOOL CloseHandle(HANDLE hObject);
DWORD GetCurrentDirectory(DWORD nBufferLength, LPTSTR lpBuffer);
HANDLE CreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess,
DWORD dwShareMode, void* lpSecurityAttributes,
DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

HWND FindWindow(LPCTSTR lpClassName, LPCTSTR lpWindowName);
BOOL DeleteFile(LPCTSTR lpFileName);
UINT RegisterWindowMessage(LPCTSTR lpString);
void Sleep(int);
#endif
