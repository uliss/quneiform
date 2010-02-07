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

#include "sys_prog.h"

using namespace CIF;

char NameFuncErr[100], Buff[60];
short NumErr;

void heapstat(char *mess)
{
#ifdef _MSC_VER
    int status = _heapchk(), n;

    switch (status) {
        case _HEAPOK: // heap is fine
        case _HEAPEMPTY: // empty heap
        case _HEAPEND: // end of heap
            return;
    }

    fprintf(stderr, "\n%s", mess);

    switch (status) {
        case _HEAPBADPTR: // bad pointer to heap
            n = 1;
            break;
        case _HEAPBADBEGIN: // bad start of heap
            n = 2;
            break;
        case _HEAPBADNODE: // bad node in heap
            n = 3;
            break;
        default: // ЌҐа бЇ®§­ ­­ п ®иЁЎЄ 
            n = -1;
    }

    Error(n, "Heapstat");
#endif
}

//===
#if defined (WIN_MOD) || defined (DEBUG_MEM)
char * PASC malloc_m(unsigned long size)
{
#ifdef WIN_MOD
#ifndef WIN32
    Handle h;
#ifdef DEBUG_MEM
    char *p, *err = "malloc_m";

    if (!(h = GlobalAlloc(GMEM_MOVEABLE, (long)size)))
        return NULL;

    if ((p = GlobalLock(h)) == NULL)
        Error(2, err);

    return p;
#else

    if (!(h = GlobalAlloc(GHND, (long)size)))
        return NULL;

    return GlobalLock(h);
#endif
#else
#ifdef DEBUG_MEM
    char *p, *err = "malloc_m";

    if (!size)
        Error(1, err);

    heapstat("malloc_m bef");

    if ((p = (char*)malloc(size)) == NULL)
        Error(2, err);

    heapstat("malloc_m aft");
    return p;
#else
    return (char*)malloc(size);
#endif
#endif
#else
#ifdef DEBUG_MEM
    char *p, *err = "malloc_m";

    if (!size)
        Error(1, err);

    if (size > 0xFFFF)
        Error(3, err);

    heapstat("malloc_m bef");

    if ((p = (char*)malloc((size_t)size)) == NULL)
        Error(2, err);

    heapstat("malloc_m aft");
    return p;
#else
    return malloc((size_t)size);
#endif
#endif
}

//=================
char * PASC halloc_m(long n, uint size)
{
    char *err = "halloc_m";
#ifdef WIN_MOD
#ifndef WIN32
    Handle h;

    if (!(h = GlobalAlloc(GHND, n * size))) return NULL;

    return GlobalLock(h);
#else
#ifdef DEBUG_MEM
    char * p;

    if (!size)Error(1, err);

    heapstat("halloc_m bef");
    p = (char*)malloc(n * size);
    heapstat("halloc_m aft");
    return p;
#else
    return (char*) malloc((int32_t)n * size);
#endif
#endif
#else
    return (char*)malloc(n*size);
#endif
}
//=================
void PASC free_m(void *ptr)
{
#ifdef WIN_MOD

    if (ptr) {
#ifndef WIN32
        Handle h, h1;
#ifdef DEBUG_MEM

        while ( GlobalUnlock( h = LOWORD(GlobalHandle(SELECTOROF(ptr))) ) );

#else
        GlobalUnlock( h = LOWORD(GlobalHandle(SELECTOROF(ptr))) );
#endif

        if ( h1 = (Handle) GlobalFree(h) )
            Error(1, "free_m");

#else
#ifdef DEBUG_MEM

        if (!ptr)
            Error(2, "free_m");

        heapstat("free_m bef");
        free(ptr);
        heapstat("free_m aft");
#else
        free(ptr);
#endif
#endif
    }

#else
#ifdef DEBUG_MEM

    if (!ptr)
        Error(2, "free_m");

    heapstat("free_m bef");
    free(ptr);
    heapstat("free_m aft");
#else
    free(ptr);
#endif
#endif
}
//=================
void PASC hfree_m(void *ptr)
{
#ifdef WIN_MOD
#ifndef WIN32

    if (ptr) {
        Handle h, h1;
#ifdef DEBUG_MEM

        while ( GlobalUnlock( h = LOWORD(GlobalHandle(SELECTOROF(ptr))) ) );

#else
        GlobalUnlock( h = LOWORD(GlobalHandle(SELECTOROF(ptr))) );
#endif

        if (h1 = (Handle) GlobalFree(h))
            Error(1, "hfree_m");
    }

#else
#ifdef DEBUG_MEM

    if (ptr == NULL)
        Error(2, "hfree_m");

    heapstat("hfree_m bef");
    free(ptr);
    heapstat("hfree_m aft");
#else
    free(ptr);
#endif
#endif
#else //WIN_MOD
    free(ptr);
#endif //WIN_MOD
}
#endif /* defined (WIN_MOD) || defined (DEBUG_MEM) */

//®бў®Ў®¦¤Ґ­ЁҐ б ЇаҐ¤ў аЁв. Їа®ўҐаЄ®© ­  NULL
void free_c(void *ptr)
{
    if (ptr)
#ifdef WIN_MOD
#ifndef WIN32
    {   Handle h, h1;
#ifdef DEBUG_MEM

        while ( GlobalUnlock( h = LOWORD(GlobalHandle(SELECTOROF(ptr))) ) );

#else
        GlobalUnlock( h = LOWORD(GlobalHandle(SELECTOROF(ptr))) );
#endif

        if ( h1 = (Handle) GlobalFree(h) )
            Error(1, "free_c");
    }
#else
    {   free(ptr);
    }
#endif
#else
    {
        free(ptr);
    }
#endif
}

//===
void * realloc_m(void *ptr_old, uint size_old, uint size_new)
{
    void *ptr_new;

    if (!size_old)
#ifdef DEBUG_MEM
        Error(1, "realloc_m");

#else
        /* ’ Є ваҐЎгҐв бв ­¤ ав realloc'  */
        return malloc(size_new);
#endif

    if (size_new <= size_old)
        return ptr_old;

    if (ptr_new = malloc(size_new)) {
        //memcpy_m(ptr_new,ptr_old,size_old);
        memmove(ptr_new, ptr_old, size_old);
        free(ptr_old);
    }

    return ptr_new;
}

//== ‘ЋЋЃ™…Ќ€џ ЋЃ Ћ�€ЃЉЂ• ==
void Error(int num, const char *str)
{
    char str1[60];
    WAR(num, str);
#ifdef WIN_MOD
    wsprintf(str1, "\nERR=%d %s", num, str);
    //MessageBox((HWND) 0,str1,(char*) "ERROR",MB_ICONHAND|MB_OK);
#ifndef DLL_MOD
    FatalExit(num);
#endif
#endif
}

//=================
void ERRO(int num, char *str)
{
    char str1[60];
    WAR(num, str);
#ifdef WIN_MOD
    wsprintf(str1, "\nERR=%d %s", num, str);
    MessageBox((HWND) 0, str1, (char*) "ERROR", MB_ICONHAND | MB_OK);
#ifndef DLL_MOD
    FatalExit(num);
#endif
#else
    fprintf(stderr, "\nERR=%d %s", num, str);
    exit(num);
#endif
}

extern char NameFuncErr[100];
extern short NumErr;

void WAR(int num, const char *str)
{
    strcpy(NameFuncErr, str);
    NumErr = num;
}

//===
void PutMess(int num, char *str)
{
#ifndef WIN_MOD
    fprintf(stderr, "\n%s %d", str, num);
#else
    char str1[160];
    wsprintf(str1, "\n%s %d", str, num);
    MessageBox((HWND) 0, str1, (char*) "MESS", MB_ICONHAND | MB_OK);
#endif
}

//------  Common functions for mrk.dll, dot.dll, ndx.dll --------
#include "globus.h"
#include "memfunc.h"

#ifdef CT_SKEW
#include "skew1024.h"
#endif

void ProjectRect1024(Rect16 *r, int32_t Skew1024)
{
    int xa, ya,
#ifndef CT_SKEW
    xc, yc,
#endif
    dx, dy;

    if (Skew1024) {
        xa = (r->right + r->left) >> 1;
        ya = (r->top + r->bottom) >> 1;
#ifndef CT_SKEW
        xc = xa + (int) (((int32_t) ya * Skew1024) / 1024);
        yc = ya - (int) (((int32_t) xa * Skew1024) / 1024);
        dx = xc - xa;
        dy = yc - ya;
#else
        Point16 pt(xa, ya);
        pt.deskew(-Skew1024);
        dx = pt.x() - xa;
        dy = pt.y() - ya;
#endif
        r->left += (int16_t) dx;
        r->right += (int16_t) dx;
        r->bottom += (int16_t) dy;
        r->top += (int16_t) dy;
    }
}

void ProjectPoint1024(Point16 *r, int32_t Skew1024)
{
#ifndef CT_SKEW
    int16_t xa, ya;
    xa = r->x;
    ya = r->y;
    r->x = xa + (int16_t) (((int32_t) ya * Skew1024) / 1024);
    r->y = ya - (int16_t) (((int32_t) xa * Skew1024) / 1024);
#else
    r->deskew(-Skew1024);
#endif
}

