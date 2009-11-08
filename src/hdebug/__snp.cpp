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

#include <cstdio>
#include <stdarg.h>
#include <ctime>

#define __SNAP_C
#include "dpuma.h"
#include "compat_defs.h"

using namespace CIF;

HINSTANCE hDPuma = NULL;

static FNDPUMA_IsActive IsActive = NULL;
static FNDPUMA_Skip Skip = NULL;
static FNDPUMA_StartLoop StartLoop = NULL;
static FNDPUMA_LoopNext LoopNext = NULL;
static FNDPUMA_MessageBoxOk MessageBoxOk = NULL;
static FNDPUMA_WaitUserInput WaitUserInput = NULL;
static FNDPUMA_DrawLine DrawLine = NULL;
static FNDPUMA_DrawRect DrawRect = NULL;
static FNDPUMA_DrawRaster DrawRaster = NULL;
static FNDPUMA_RasterText RasterText = NULL;
static FNDPUMA_GetDIBptr GetDIBptr = NULL;
static FNDPUMA_DrawString DrawString = NULL;
static FNDPUMA_DeleteStrings DeleteStrings = NULL;
static FNDPUMA_SendWindow SendWindow = NULL;
static FNDPUMA_SendMainWnd SendMainWnd = NULL;
static FNDPUMA_CSTR_Monitor cstr_Monitor = NULL;
static FNDPUMA_CSTR_GetPosition cstr_GetPosition = NULL;
static FNDPUMA_CSTR_SetPosition cstr_SetPosition = NULL;
static FNDPUMA_DestroyRasterWnd DestroyRasterWnd = NULL;
static FNDPUMA_CSTR_GetLength cstr_GetLength = NULL;
static FNDPUMA_ShowCutPoint ShowCutPoint = NULL;
static FNDPUMA_GetRasterPixel GetRasterPixel = NULL;
static FNDPUMA_CSTR_Update cstr_Update = NULL;
static FNDPUMA_SkipEx SkipEx = NULL;
static FNDPUMA_GetFileName fGetFileName = NULL;
static FNDPUMA_SaveFile fSaveFile = NULL;
static FNDPUMA_GetWindowHandle fGetWindowHandle = NULL;
static FNDPUMA_GetPrevSkipOwner fGetPrevSkipOwner = NULL;
static FNDPUMA_FOpen fFOpen = NULL;
static FNDPUMA_FClose fFClose = NULL;
static FNDPUMA_FPuts fFPuts = NULL;

static Handle hWriteFile = NULL;

Bool32 LDPUMA_IsActive(void) {
    return IsActive ? IsActive() : FALSE;
}

void LDPUMA_DrawRect(Handle wnd, Rect16* rc, int32_t skew, uint32_t rgb_color, int16_t pen_width,
        uint32_t key) {
    if (DrawRect)
        DrawRect(wnd, rc, skew, rgb_color, (int16_t) pen_width, key);
}

void LDPUMA_DrawLine(Handle wnd, Point16* start, Point16* end, int32_t skew, uint32_t rgb_color,
        int16_t pen_width, uint32_t key) {
    if (DrawLine)
        DrawLine(wnd, start, end, skew, rgb_color, pen_width, key);
}

void LDPUMA_Console(const char * message, ...) {
    // console output only in debug mode
#ifndef NDEBUG
    va_list marker;
    va_start(marker, message);
    vfprintf(stderr, message, marker);
    va_end(marker);
#endif
}

void LDPUMA_ConsoleN(const char * message, ...) {
#ifndef NDEBUG
    va_list marker;
    va_start( marker, message);
    vfprintf(stderr, message, marker);
    va_end(marker);
    LDPUMA_Console("\n");
#endif
}

void LDPUMA_MessageBoxOk(const char * message, ...) {
    if (MessageBoxOk) {
        va_list marker;
        va_start( marker, message);
        fputs("MessageBox: ", stderr);
        vfprintf(stderr, message, marker);
        va_end(marker);
    }
}

uint32_t LDPUMA_WaitUserInput(Handle cur_node, Handle wnd) {
    uint32_t rc = 0;
    if (WaitUserInput)
        rc = WaitUserInput(cur_node, wnd);
    return rc;
}

void LDPUMA_StartLoop(Handle node, uint32_t iter_total) {
    if (StartLoop)
        StartLoop(node, iter_total);
}

void LDPUMA_LoopNext(Handle node) {
    if (LoopNext)
        LoopNext(node);
}

Bool16 LDPUMA_Skip(Handle node) {
    return Skip ? Skip(node) : TRUE;
}

void LDPUMA_DrawRaster(DPUMA_RecRaster * raster) {
    if (DrawRaster)
        DrawRaster((DPUMA_RecRaster *) raster);
}

void LDPUMA_RasterText(const char * lpText) {
    if (RasterText)
        RasterText(lpText);
}

void * LDPUMA_GetDIBptr(Handle wnd) {
    if (GetDIBptr)
        return GetDIBptr(wnd);
    return NULL;
}

void LDPUMA_DrawString(Handle wnd, Point16* start, const char * string, int32_t align,
        uint32_t rgb_color, int16_t size, uint32_t key) {
    if (DrawString)
        DrawString(wnd, start, string, align, rgb_color, size, key);
}

void LDPUMA_DeleteStrings(Handle wnd, uint32_t key) {
    if (DeleteStrings)
        DeleteStrings(wnd, key);
}

uint32_t LDPUMA_SendWindow(Handle wnd, uint32_t message, uint32_t wParam, uint32_t lParam) {
    uint32_t rc = 0;
    if (SendWindow)
        rc = SendWindow(wnd, message, wParam, lParam);
    return rc;
}

uint32_t LDPUMA_SendMainWnd(uint32_t message, uint32_t wParam, uint32_t lParam) {
    uint32_t rc = 0;
    if (SendMainWnd)
        rc = SendMainWnd(message, wParam, lParam);
    return rc;
}

uint32_t LDPUMA_CSTR_Monitor(Handle owner, uint32_t cstr_line, uint32_t pos,
        DPUMA_Callback_WindowProc lpproc) {
    uint32_t rc = 0;
    if (cstr_Monitor)
        rc = cstr_Monitor(owner, cstr_line, pos, lpproc);
    return rc;
}

uint32_t LDPUMA_CSTR_GetPosition(uint32_t * cstr_raster) {
    uint32_t rc = 0;
    if (cstr_GetPosition)
        cstr_GetPosition(cstr_raster);

    return rc;
}

void LDPUMA_CSTR_SetPosition(uint32_t pos) {
    if (cstr_SetPosition)
        cstr_SetPosition(pos);
}

void LDPUMA_DestroyRasterWnd() {
    if (DestroyRasterWnd)
        DestroyRasterWnd();
}

uint32_t LDPUMA_CSTR_GetLength() {
    uint32_t rc = 0;
    if (cstr_GetLength)
        rc = cstr_GetLength();
    return rc;
}

void LDPUMA_ShowCutPoint(uint32_t number, Point * lppoint) {
    if (ShowCutPoint)
        ShowCutPoint(number, lppoint);
}

Point LDPUMA_GetRasterPixel(Handle wnd, Point point) {
    Point rc(-1, -1);
    if (GetRasterPixel)
        rc = GetRasterPixel(wnd, point);
    return rc;
}

void LDPUMA_CSTR_Update() {
    if (cstr_Update)
        cstr_Update();
}

Bool32 LDPUMA_SkipEx(Handle owner, Bool32 bIter, Bool32 bParent, int32_t nSign) {
    Bool32 rc = TRUE;
    if (SkipEx)
        rc = SkipEx(owner, bIter, bParent, nSign);
    return rc;
}

const char * LDPUMA_GetFileName(Handle wnd) {
    const char * rc = "";
    if (fGetFileName)
        rc = fGetFileName(wnd);
    return rc;
}

Bool32 LDPUMA_SaveFile(void * lpDIB, char * lpFileName, uint32_t nFormat) {
    Bool32 rc = FALSE;
    if (fSaveFile)
        rc = fSaveFile(lpDIB, lpFileName, nFormat);
    return rc;
}

Handle LDPUMA_GetWindowHandle(const char * name) {
    Handle rc = NULL;
    if (fGetWindowHandle)
        rc = fGetWindowHandle(name);
    return rc;
}

Handle LDPUMA_GetPrevSkipOwner() {
    Handle rc = NULL;
    if (fGetPrevSkipOwner)
        rc = fGetPrevSkipOwner();
    return rc;
}

Handle LDPUMA_FOpen(const char * lpName, const char * lpMode) {
    Handle hFile = NULL;

    if (fFOpen && !LDPUMA_Skip(hWriteFile))
        hFile = fFOpen(lpName, lpMode);
    return hFile;
}

void LDPUMA_FClose(Handle hFile) {
    if (fFClose && hFile)
        fFClose(hFile);
}

int32_t LDPUMA_FPuts(Handle hFile, const char * lpString) {
    int32_t rc = 0;
    if (fFPuts && hFile)
        rc = fFPuts(hFile, lpString);
    return rc;
}

#ifdef DPUMA_SNAP
#include "snptools.h"

void SnpSetTools(__SnpToolBox* tools)
{
}

void SnpDrawFocusRect(Rect16* rc) {
}

Bool16 SnpGetUserRect(Rect16* rect) {
    return TRUE;
}
Bool16 SnpGetUserPoint(Point16* pnt) {
    return TRUE;
}

uint32_t SnpWaitUserInput(SnpTreeNode* cur_node) {
    return LDPUMA_WaitUserInput(cur_node ? *(Handle *) cur_node : NULL, NULL);
}

void SnpAddNode(SnpTreeNode * node, const char * name, SnpTreeNode * parent) {
}

void SnpStartLoop(SnpTreeNode* node, uint32_t iter_total) {
    LDPUMA_StartLoop(node ? *(Handle *) node : NULL, iter_total);
}

void SnpLoopNext(SnpTreeNode* node) {
    LDPUMA_LoopNext(node ? *(Handle *) node : NULL);
}

Bool16 SnpSkip(SnpTreeNode* node) {
    return LDPUMA_Skip(node ? *(Handle *) node : NULL);
}

void SnpDrawRaster(RecRaster * raster) {
    LDPUMA_DrawRaster((DPUMA_RecRaster *) raster);
}

void SnpRasterText(char * lpText) {
    LDPUMA_RasterText(lpText);
}

void SnpZoomToRect(Rect16 * lpRect) {
}

void SnpRasterHeader(char * lpText, uint32_t num) {
}

void SnpHideLines(Handle key) {
}

void SnpUpdateViews(void) {
}

int SnpLog(const char * message, ...) {
    va_list marker;
    va_start( marker, message);
    vfprintf(stderr, message, marker);
    va_end(marker);
}

Bool SnpIsActive(void) {
    return LDPUMA_IsActive();
}

uint32_t SnpSetZoneOn(Rect16* zone_rect, uint32_t rgb_color, char* status_line_comment,
        uint32_t users_zone_handle, FTOnMouseDown on_mouse_down) {
    return 0;
}
void SnpSetZoneOff(uint32_t zone_handle) {
    ;
}

void SnpNotifyAppl(SnpTreeNode* cur_node) {
    ;
}
// tree manipulation
void __SnpIterParent(SnpTreeNode* node, Bool activate) {
    ;
}
#else
int SnpLog(const char * /*message*/, ...) {
    return 0;
}
#endif

