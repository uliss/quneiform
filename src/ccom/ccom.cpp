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

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "ccom.h"
#include "ccom_fun.h"
#include "resource.h"

// static dates
static CCOM_cont top, tail;
static int32_t num_containers = 0;
static int32_t user_number = 0xFFFF + 1;
static Bool32 ccom_init = FALSE;
static Bool32 ccom_show_kill_mode = FALSE;
uint16_t wHeightRC = 0;
uint16_t wLowRC = CCOM_ERR_NO;

static const char * CCOM_DAT = "ccom.dat";

static Bool32 (*my_MakeLP)(RecRaster *rRaster, uchar *lp, int16_t *lp_size, int16_t *numcomp) = NULL;

/////////////////////
// common functions
/////////////////////
Bool32 CCOM_Init(uint16_t wHeightCode)
{
    if (ccom_init)
        return TRUE;

    wHeightRC = wHeightCode;
    wLowRC = CCOM_ERR_NO;
    // inti error code
    top.next = &tail;
    top.prev = NULL;
    tail.prev = &top;
    tail.next = NULL;
    top.first = top.last = tail.first = tail.last = NULL;
    // init list of containers
    num_containers = 0;
    user_number = 0xFFFF + 1;
    ccom_init = TRUE;
    return TRUE;
}

CCOM_cont * CCOM_CreateContainer(void)
{
    CCOM_cont *new_cont, *prev = tail.prev, *next = &tail;
    new_cont = static_cast<CCOM_cont*> (calloc(1, sizeof(CCOM_cont)));

    if (!new_cont) {
        wLowRC = CCOM_ERR_NOMEMORY;
        return NULL;
    }

    prev->next = new_cont;
    next->prev = new_cont;
    new_cont->prev = prev;
    new_cont->next = next;
    new_cont->first = NULL;
    new_cont->last = new_cont->first;
    num_containers++;
    new_cont->nall = 0;

    for (int i = 0; i < 8; i++)
        new_cont->nsmall[i] = 0;

    new_cont->kill_dust_mode = FALSE;
    new_cont->language = -1;
    return new_cont;
}

Bool32 CCOM_GetContainerVolume(CCOM_handle hcont)
{
    CCOM_cont *cont = (CCOM_cont*) hcont;
    return cont->nall;
}

Bool32 CCOM_DeleteContainer(CCOM_cont * hcont)
{
    CCOM_cont *curr, *cont = hcont, *prev, *next;
    CCOM_comp * next_comp, *current;

    for (curr = top.next; curr != &tail && curr != cont; curr = curr->next)
        ;

    if (curr != cont) { // not found
        wLowRC = CCOM_ERR_VALUE;
        return FALSE;
    }

    for (current = curr->first; current != NULL; current = next_comp) {
        next_comp = current->next_comp;
        CCOM_Delete(hcont, current);
    }

    prev = curr->prev;
    next = curr->next;
    prev->next = next;
    next->prev = prev;
    free(cont);
    num_containers--;
    return TRUE;
}

void CCOM_DeleteAll(void)
{
    CCOM_cont * i, *inext;

    for (i = top.next; i && i != &tail; i = inext) {
        inext = i->next;
        CCOM_DeleteContainer(i);
    }

    wLowRC = CCOM_ERR_NO;
    wHeightRC = 0;
    top.next = &tail;
    top.prev = NULL;
    tail.prev = &top;
    tail.next = NULL;
    top.first = top.last = tail.first = tail.last = NULL;
    // init list of containers
    num_containers = 0;
    user_number = 0xFFFF + 1;
}

void CCOM_Done(void)
{
    CCOM_DeleteAll();
    ccom_init = FALSE;
}

uint32_t CCOM_GetReturnCode(void)
{
    if (wLowRC == CCOM_ERR_NO)
        return 0;

    return (wHeightRC << 16) | (wLowRC - CCOM_ERR_MIN);
}

char* CCOM_GetReturnString(uint32_t dwError)
{
    uint16_t rc = (uint16_t) ((dwError & 0xFFFF) + CCOM_ERR_MIN);
    static char szBuffer[512];

    if (dwError >> 16 != wHeightRC)
        wLowRC = CCOM_ERR_NOTIMPLEMENT;

    if (rc > 0 && rc <= CCOM_ERR_MAX - CCOM_ERR_MIN)
        strcpy((char *) szBuffer, CCOM_error_name[rc]);

    else
        return NULL;

    return szBuffer;
}

///////////////////////////////
// operation with components
///////////////////////////////

static Bool32 ccom_to_many_comps(CCOM_handle hcont)
{
    CCOM_cont * cont = (CCOM_cont *) hcont;
    return !cont->kill_dust_mode && cont->nsmall[1] > (cont->nall - cont->nsmall[1]) * 3
           && cont->nall > 3000;
}

static Bool32 ccom_small_dust(CCOM_comp * comp, CCOM_handle hcont)
{
    CCOM_cont * cont = (CCOM_cont *) hcont;
    int32_t sz = (comp->w > comp->h ? comp->w : comp->h);

    if (cont->nsmall[2] > (cont->nall - cont->nsmall[1] - cont->nsmall[2]) * 3 && cont->nall
            - cont->nsmall[1] > 3000)
        return sz < 3;

    return sz < 2;
}

CCOM_comp * CCOM_New(CCOM_handle hcont, int32_t upper, int32_t left, int32_t w, int32_t h)
{
    CCOM_comp * new_comp;
    CCOM_cont * cont = hcont;

    if (w < 1 || h < 1 || !cont) {
        wLowRC = CCOM_ERR_VALUE;
        return NULL;
    }

    new_comp = static_cast<CCOM_comp*> (calloc(1, sizeof(CCOM_comp)));

    if (!new_comp) {
        wLowRC = CCOM_ERR_NOMEMORY;
        return NULL;
    }

    memset(new_comp, 0, sizeof(CCOM_comp));
    new_comp->next_comp = NULL;
    new_comp->upper = (int16_t) upper;
    new_comp->left = (int16_t) left;
    new_comp->w = (int16_t) w;
    new_comp->h = (int16_t) h;
    new_comp->rw = (uchar) ((w + 7) / 8);

    if (!cont->first) {
        cont->first = new_comp;
        cont->last = new_comp;
    }

    else {
        cont->last->next_comp = new_comp;
        cont->last = new_comp;
    }

    {
        int32_t sz = (new_comp->w > new_comp->h) ? new_comp->w : new_comp->h;
        CCOM_comp * comp;
        cont->nall++;

        if (sz < 8)
            cont->nsmall[sz]++;

        if (ccom_to_many_comps(hcont)) {
            for (comp = cont->first; comp && comp != new_comp; comp = comp->next_comp) {
                if (ccom_small_dust(comp, hcont))
                    CCOM_Kill(comp);
            }

            cont->kill_dust_mode = TRUE;
        }
    }

    if (cont->kill_dust_mode && ccom_small_dust(new_comp, hcont)) {
        CCOM_Kill(new_comp);
    }

    return new_comp;
}

Bool32 CCOM_StoreLarge(CCOM_comp * comp, int16_t numcomp, int32_t size_lrep, uchar *lines,
                       int16_t nl, uchar free_beg, uchar free_end, RecVersions * vers, CCOM_USER_BLOCK * ub)
{
    CCOM_lnhead * ln;
    uchar comptype;

    if (!comp) {
        wLowRC = CCOM_ERR_NULL;
        return FALSE;
    }

    if (size_lrep && lines > 0 && lines) {
        comp->size_linerep = (int16_t) size_lrep;
        comp->linerep = static_cast<uchar*> (calloc(1, size_lrep + 2));

        if (!comp->linerep || size_lrep == -1) // OLEG & ANTON : very large components
            return FALSE;

        memcpy(comp->linerep, lines, size_lrep);
        *(comp->linerep + size_lrep) = 0; // LAST zero
        *(comp->linerep + size_lrep + 1) = 0;
        comp->size_linerep += 2;

        if (numcomp < 2 && (nl == 0 || free_beg == 0 || free_end == 0)) {
            ln = (CCOM_lnhead*) ((uchar*) lines + 2);
            free_beg = free_end = 0;
            nl = 0;
            for (int size = 2; size < size_lrep && ln->lth; nl++, ln = (CCOM_lnhead *) ((uchar*) ln
                                                                                  + ln->lth)) {
                if (ln->flg & LNHEAD_FREE_BEGIN)
                    free_beg++;

                if (ln->flg & LNHEAD_FREE_END)
                    free_end++;

                size += ln->lth;
            }
        }

        if (numcomp) {
            comp->numcomp = numcomp;
            comp->nl = nl; // can be 0 !!!
            comp->begs = free_beg;
            comp->ends = free_end;
        }

        else {
            comp->nl = nl;
            comp->begs = free_beg;
            comp->ends = free_end;
        }
    }

    comptype = 0;

    if (comp->type == 0) {
        if (comp->h < CCOM_DUST_SIZE)
            comptype = CCOM_CH_DUST;

        else
            comptype = CCOM_CH_LETTER;
    }

    if (vers) {
        if (!comp->vers && vers->lnAltCnt) {
            comp->vers = static_cast<RecVersions*> (calloc(1, sizeof(RecVersions)));

            if (!comp->vers)
                return FALSE;
        }

        if (comp->vers)
            memcpy(comp->vers, vers, sizeof(RecVersions));

        if (vers->lnAltCnt == 1)
            comptype |= CCOM_CH_PERFECT;
    }

    comp->type |= comptype;

    while (ub) {
        CCOM_SetUserBlock(comp, ub);
        ub = ub->next_block;
    }

    return TRUE;
}

Bool32 CCOM_Copy(CCOM_comp* to, CCOM_comp* from)
{
    to->scale = from->scale;

    if (from->scale & CCOM_LONGLINES) {
        int32_t size = from->size_linerep;

        if (size == -1) {
            CCOM_USER_BLOCK ublock;
            ublock.code = CCOM_UB_SIZELINEREP;
            ublock.data = (uchar*) & size;
            ublock.size = 4;
            CCOM_GetUserBlock(from, &ublock);
            size = *(int32_t*) ublock.data;
        }

        return CCOM_StoreLarge(to, from->numcomp, size, from->linerep, from->nl, from->begs,
                               from->ends, from->vers, from->user_block);
    }

    else
        return CCOM_Store(to, from->numcomp, from->size_linerep, from->linerep, from->nl,
                          from->begs, from->ends, from->vers, from->user_block);
}

Bool32 CCOM_Store(CCOM_comp * comp, int16_t numcomp, int32_t size_lrep, uchar *lines, int16_t nl,
                  uchar free_beg, uchar free_end, RecVersions *vers, CCOM_USER_BLOCK *ub)
{
    CCOM_lnhead* ln;
    uchar comptype;

    if (!comp) {
        wLowRC = CCOM_ERR_NULL;
        return FALSE;
    }

    if (size_lrep && lines > 0 && lines) {
        comp->size_linerep = (int16_t) size_lrep;
        comp->linerep = static_cast<uchar*> (calloc(1, size_lrep + 2));

        if (!comp->linerep || size_lrep == -1) // OLEG & ANTON : very large components
            return FALSE;

        memcpy(comp->linerep, lines, size_lrep);
        *(comp->linerep + size_lrep) = 0; // LAST zero
        *(comp->linerep + size_lrep + 1) = 0;
        comp->size_linerep += 2;

        if (numcomp < 2 && (nl == 0 || free_beg == 0 || free_end == 0)) {
            ln = (CCOM_lnhead*) ((uchar*) lines + 2);
            free_beg = free_end = 0;
            nl = 0;
            for (int size = 2; size < size_lrep && ln->lth; nl++, ln = (CCOM_lnhead *) ((uchar*) ln
                                                                                  + ln->lth)) {
                if (ln->flg & LNHEAD_FREE_BEGIN)
                    free_beg++;

                if (ln->flg & LNHEAD_FREE_END)
                    free_end++;

                size += ln->lth;
            }
        }

        if (numcomp) {
            comp->numcomp = numcomp;
            comp->nl = nl; // can be 0 !!!
            comp->begs = free_beg;
            comp->ends = free_end;
        }

        else {
            comp->nl = nl;
            comp->begs = free_beg;
            comp->ends = free_end;
        }
    }

    comptype = 0;

    if (comp->type == 0) {
        if (comp->h < CCOM_DUST_SIZE)
            comptype = CCOM_CH_DUST;

        else
            comptype = CCOM_CH_LETTER;
    }

    if (vers) {
        if (!comp->vers && vers->lnAltCnt) {
            comp->vers = static_cast<RecVersions*> (calloc(1, sizeof(RecVersions)));

            if (!comp->vers)
                return FALSE;
        }

        if (comp->vers)
            memcpy(comp->vers, vers, sizeof(RecVersions));

        if (vers->lnAltCnt == 1)
            comptype |= CCOM_CH_PERFECT;
    }

    comp->type |= comptype;

    while (ub) {
        CCOM_SetUserBlock(comp, ub);
        ub = ub->next_block;
    }

    return TRUE;
}

// delete without change list of comps
static CCOM_comp * CCOM_DeleteComp(CCOM_handle hcont, CCOM_comp * comp)
{
    CCOM_comp * next = comp->next_comp;
    CCOM_cont * cont = (CCOM_cont *) hcont;
    CCOM_USER_BLOCK * ub, *ubc;
    {
        int32_t sz = (comp->w > comp->h) ? comp->w : comp->h;
        cont->nall--;

        if (sz < 8)
            cont->nsmall[sz]--;
    }
    free(comp->linerep);
    free(comp->vers);

    ub = comp->user_block;

    while (ub) {
        ubc = ub->next_block;
        free(ub->data);
        free(ub);
        ub = ubc;
    }

    free(comp);
    return next;
}

Bool32 CCOM_Delete(CCOM_handle hcont, CCOM_comp * comp)
{
    CCOM_comp * curr;
    CCOM_cont * cont = hcont;

    if (!comp || !cont) {
        wLowRC = CCOM_ERR_NULL;
        return FALSE;
    }

    if (cont->first != comp) {
        for (curr = cont->first; curr != NULL; curr = curr->next_comp) {
            if (curr->next_comp == comp)
                break;
        }

        if (curr == NULL) {
            wLowRC = CCOM_ERR_NONTEXIST;
            return FALSE;
        }

        curr->next_comp = comp->next_comp;

        if (!comp->next_comp)
            cont->last = curr;
    }

    else {
        if (cont->first->next_comp)
            cont->first = cont->first->next_comp;

        else
            cont->first = NULL;

        curr = cont->first;
    }

    CCOM_DeleteComp(hcont, comp);
    return TRUE;
}

///////////////////////
// exclude components
///////////////////////
Bool32 CCOM_SetKilledAccess(Bool32 ShowEnable)
{
    ccom_show_kill_mode = ShowEnable;
    return TRUE;
}

static Bool32 ccom_filtrate(CCOM_comp * curr)
{
    if (!ccom_show_kill_mode)
        return ((curr->large & CCOM_LR_KILLED) == 0);

    else
        return TRUE;
}

CCOM_comp * CCOM_GetNext(CCOM_comp * comp, Bool32(*filtrate)(int32_t upper, int32_t left,
                                                             int32_t w, int32_t h))
{
    CCOM_comp * curr;

    if (!comp) {
        wLowRC = CCOM_ERR_NULL;
        return NULL;
    }

    if (((uchar*) filtrate) == NULL) {
        //return comp->next_comp;
        for (curr = comp->next_comp; curr != NULL; curr = curr->next_comp) {
            if (ccom_filtrate(curr))
                return curr;
        }
    }

    else {
        for (curr = comp->next_comp; curr != NULL; curr = curr->next_comp) {
            if (filtrate(curr->upper, curr->left, curr->w, curr->h) && ccom_filtrate(curr))
                return curr;
        }
    }

    return NULL;
}

CCOM_comp * CCOM_GetFirst(CCOM_handle hcont, Bool32(*filtrate)(int32_t upper, int32_t left,
                                                               int32_t w, int32_t h))
{
    CCOM_cont * cont = (CCOM_cont*) hcont;
    CCOM_comp * curr;

    if (!cont) {
        wLowRC = CCOM_ERR_NULL;
        return NULL;
    }

    if (((uchar*) filtrate) == NULL) {
        //return cont->first;
        for (curr = cont->first; curr != NULL; curr = curr->next_comp) {
            if (ccom_filtrate(curr))
                return curr;
        }
    }

    else {
        for (curr = cont->first; curr != NULL; curr = curr->next_comp) {
            if (filtrate(curr->upper, curr->left, curr->w, curr->h) && ccom_filtrate(curr))
                return curr;
        }
    }

    return NULL;
}

////////////////////////////
// exclude representations
////////////////////////////
int16_t CCOM_GetLine(CCOM_comp * comp, CCOM_linerep *lrep)
{
    int16_t size;

    if (!comp || !lrep) {
        wLowRC = CCOM_ERR_NULL;
        return 0;
    }

    if (!comp->size_linerep) {
        wLowRC = CCOM_ERR_BADDATA;
        return 0;
    }

    size = comp->size_linerep;

    if (!size) {
        wLowRC = CCOM_ERR_NULL;
        return 0;
    }

    memcpy(lrep, &comp->linerep[0], size);
    return size;
}

Bool32 CCOM_GetRaster(CCOM_comp * comp, RecRaster *rec)
{
    int16_t w, h;
    uchar * lp;
    int16_t * lt;

    if (!comp || comp == (CCOM_comp *) 0xcdcdcdcd || !rec) {
        wLowRC = CCOM_ERR_NULL;
        return FALSE;
    }

    if (!comp->size_linerep) {
        wLowRC = CCOM_ERR_BADDATA;
        return FALSE;
    }

    if (comp->scale & CCOM_LONGLINES) {
        wLowRC = CCOM_ERR_LARGECOMP;
        return FALSE;
    }

    rec->lnPixWidth = comp->w;
    rec->lnPixHeight = comp->h;
    rec->lnRasterBufSize = REC_MAX_RASTER_SIZE;

    if (comp->scale) {
        rec->lnPixWidth = (rec->lnPixWidth + (1 << comp->scale) - 1) >> comp->scale;
        rec->lnPixHeight = (rec->lnPixHeight + (1 << comp->scale) - 1) >> comp->scale;
    }

    memset(rec->Raster, 0, REC_GW_WORD8(rec->lnPixWidth) * rec->lnPixHeight);
    rec->lnRasterBufSize = REC_MAX_RASTER_SIZE;

    if (comp->numcomp > 1) {
        Bool32 ret;
        lp = &comp->linerep[0];
        lt = (int16_t*) lp;
        w = comp->w;
        h = comp->h;

        if (comp->scale) {
            w = (w + (1 << comp->scale) - 1) >> comp->scale;
            h = (h + (1 << comp->scale) - 1) >> comp->scale;
        }

        do { // comps cycle
            ret = Linerep2Raster((CCOM_lnhead*) & lp[2], (int16_t) ((*lt) - 2), w, h, 0, 0, rec,
                                 TRUE);
            lp += *lt;
            lt = (int16_t*) lp;
        }
        while (ret && ((*lt) > 0)); // Nick - was lt );

        return ret;
    }

    w = comp->w;
    h = comp->h;

    if (comp->scale) {
        w = (w + (1 << comp->scale) - 1) >> comp->scale;
        h = (h + (1 << comp->scale) - 1) >> comp->scale;
    }

    lp = &comp->linerep[0];
    lt = (int16_t*) lp;
    return Linerep2Raster((CCOM_lnhead*) &comp->linerep[2], (int16_t) ((*lt) - 2), w, h, 0, 0, rec,
                          TRUE);
}

Bool32 CCOM_GetExtRaster(CCOM_comp * comp, RecRaster *rec)
{
    int16_t w, h;
    uchar * lp;
    int16_t * lt;

    if (!comp || !rec) {
        wLowRC = CCOM_ERR_NULL;
        return FALSE;
    }

    if (!comp->scale)
        return CCOM_GetRaster(comp, rec);

    if (!comp->size_linerep) {
        wLowRC = CCOM_ERR_BADDATA;
        return FALSE;
    }

    if (comp->scale & CCOM_LONGLINES) {
        wLowRC = CCOM_ERR_LARGECOMP;
        return FALSE;
    }

    rec->lnPixWidth = comp->w;
    rec->lnPixHeight = comp->h;
    rec->lnRasterBufSize = REC_MAX_RASTER_SIZE;
    memset(rec->Raster, 0, REC_GW_WORD8(rec->lnPixWidth) * rec->lnPixHeight);

    if (comp->numcomp > 1)
        return FALSE;

    w = comp->w;
    h = comp->h;
    lp = &comp->linerep[0];
    lt = (int16_t*) lp;
    return Linerep2ExtRaster((CCOM_lnhead*) &comp->linerep[2], (int16_t) ((*lt) - 2), w, h, 0, 0,
                             rec);
}

Bool32 CCOM_GetScaleRaster(CCOM_comp * comp, RecRaster *rec, int32_t scale)
{
    int16_t w, h;
    uchar * lp;
    int16_t * lt;

    if (!comp || !rec) {
        wLowRC = CCOM_ERR_NULL;
        return FALSE;
    }

    if (!comp->size_linerep) {
        wLowRC = CCOM_ERR_BADDATA;
        return FALSE;
    }

    if (comp->scale & CCOM_LONGLINES) {
        wLowRC = CCOM_ERR_LARGECOMP;
        return FALSE;
    }

    rec->lnPixWidth = comp->w;
    rec->lnPixHeight = comp->h;
    rec->lnPixWidth = (rec->lnPixWidth + (1 << scale) - 1) >> scale;
    rec->lnPixHeight = (rec->lnPixHeight + (1 << scale) - 1) >> scale;
    rec->lnRasterBufSize = REC_MAX_RASTER_SIZE;
    memset(rec->Raster, 0, REC_GW_WORD8(rec->lnPixWidth) * rec->lnPixHeight);

    if (comp->numcomp > 1)
        return FALSE;

    w = comp->w;
    h = comp->h;
    lp = &comp->linerep[0];
    lt = (int16_t*) lp;
    return Linerep2ScaleRaster((CCOM_lnhead*) &comp->linerep[2], (int16_t) ((*lt) - 2), w, h, 0, 0,
                               rec, scale);
}

Bool32 CCOM_AddLPToRaster(CCOM_comp * comp, RecRaster *rec)
{
    int16_t w, h;
    uchar * lp;
    int16_t * lt;

    if (!comp || !rec || !rec->lnPixWidth || !rec->lnPixHeight) {
        wLowRC = CCOM_ERR_NULL;
        return FALSE;
    }

    if (!comp->size_linerep || !comp->linerep) {
        wLowRC = CCOM_ERR_BADDATA;
        return FALSE;
    }

    if (comp->scale & CCOM_LONGLINES) {
        wLowRC = CCOM_ERR_LARGECOMP;
        return FALSE;
    }

    if (comp->numcomp > 1) {
        Bool32 ret;
        lp = &comp->linerep[0];
        lt = (int16_t*) lp;
        w = comp->w;
        h = comp->h;

        if (comp->scale) {
            w = (w + (1 << comp->scale) - 1) >> comp->scale;
            h = (h + (1 << comp->scale) - 1) >> comp->scale;
        }

        do { // comps cycle
            ret = Linerep2Raster((CCOM_lnhead*) & lp[2], (int16_t) ((*lt) - 2), w, h, 0, 0, rec,
                                 TRUE);
            lp += *lt;
            lt = (int16_t*) lp;
        }
        while (ret && *lt);

        return ret;
    }

    w = comp->w;
    h = comp->h;

    if (comp->scale) {
        w = (w + (1 << comp->scale) - 1) >> comp->scale;
        h = (h + (1 << comp->scale) - 1) >> comp->scale;
    }

    lp = &comp->linerep[0];
    lt = (int16_t*) lp;
    return Linerep2Raster((CCOM_lnhead*) &comp->linerep[2], (int16_t) ((*lt) - 2), w, h, 0, 0, rec,
                          TRUE);
}

Bool32 CCOM_AddCompToRaster(CCOM_comp * comp, int16_t relleft, int16_t relupper, RecRaster *rec)
{
    int16_t w, h, left = relleft, upper = relupper;
    uchar * lp;
    int16_t * lt;

    if (!comp || !rec || !rec->lnPixWidth || !rec->lnPixHeight) {
        wLowRC = CCOM_ERR_NULL;
        return FALSE;
    }

    if (!comp->size_linerep) {
        wLowRC = CCOM_ERR_BADDATA;
        return FALSE;
    }

    if (comp->scale & CCOM_LONGLINES) {
        wLowRC = CCOM_ERR_LARGECOMP;
        return FALSE;
    }

    if (comp->numcomp > 1) {
        Bool32 ret;
        lp = &comp->linerep[0];
        lt = (int16_t*) lp;
        w = comp->w;
        h = comp->h;

        if (comp->scale) {
            w = (w + (1 << comp->scale) - 1) >> comp->scale;
            // i.e., not a [w/2^scale], but [( w+((2^scale)-1) )/2^scale] instead.
            // as far both width and height are not supposed to be 0.
            // (hence --- even the very small lines are still drawn (past tense of draw))
            h = (h + (1 << comp->scale) - 1) >> comp->scale;
            // beginning of component can be 0.
            // (i.e., pre-image of 0 with respect to this contraction is not only 0,
            // but also the whole interval [0..((2^scale)-1))]
            left >>= comp->scale;
            upper >>= comp->scale;
        }

        do { // comps cycle
            ret = Linerep2Raster((CCOM_lnhead*) & lp[2], (int16_t) ((*lt) - 2), w, h, left, upper,
                                 rec, FALSE);
            lp += *lt;
            lt = (int16_t*) lp;
        }
        while (ret && *lt);

        return ret;
    }

    w = comp->w;
    h = comp->h;

    if (comp->scale) {
        w = (w + (1 << comp->scale) - 1) >> comp->scale;
        h = (h + (1 << comp->scale) - 1) >> comp->scale;
        // exactly here we do ">>=comp->scale":
        left >>= comp->scale;
        upper >>= comp->scale;
    }

    lp = &comp->linerep[0];
    lt = (int16_t*) lp;
    return Linerep2Raster((CCOM_lnhead*) &comp->linerep[2], (int16_t) ((*lt) - 2), w, h, left,
                          upper, rec, FALSE);
}

Bool32 CCOM_GetCollection(CCOM_comp * comp, RecVersions *vers)
{
    if (!comp || !vers) {
        wLowRC = CCOM_ERR_NULL;
        return FALSE;
    }

    vers->lnAltMax = REC_MAX_VERS;

    if (comp->vers)
        memcpy(vers, comp->vers, sizeof(RecVersions));

    else {
        vers->lnAltCnt = 0;
    }

    return TRUE;
}

Bool32 CCOM_SetUserBlock(CCOM_comp * comp, CCOM_USER_BLOCK * ublock)
{
    CCOM_USER_BLOCK * ub, *rub = 0;

    if (!comp || !ublock) {
        wLowRC = CCOM_ERR_NULL;
        return FALSE;
    }

    if (ublock->size && ublock->data) {
        if (comp->user_block) {
            ub = comp->user_block;

            while (ub) {
                rub = ub;
                ub = ub->next_block;
            }
        }

        ub = comp->user_block;

        if (ub)
            while (ub) {
                if (ub->code == ublock->code) {
                    if (ub->data && ub->size)
                        free(ub->data);

                    ub->data = static_cast<uchar*> (calloc(1, ublock->size));

                    if (!ub->data)
                        return FALSE;

                    ub->size = ublock->size;
                    memcpy(ub->data, ublock->data, ublock->size);
                    return TRUE;
                }

                ub = ub->next_block;
            }

        ub = static_cast<CCOM_USER_BLOCK*> (calloc(1, sizeof(CCOM_USER_BLOCK)));

        if (!ub) {
            wLowRC = CCOM_ERR_NOMEMORY;
            return FALSE;
        }

        ub->data = static_cast<uchar*> (calloc(1, ublock->size));

        if (!ub->data) {
            wLowRC = CCOM_ERR_NOMEMORY;
            return FALSE;
        }

        ub->size = ublock->size;
        ub->code = ublock->code;
        memcpy(ub->data, ublock->data, ublock->size);

        if (!comp->user_block)
            comp->user_block = ub;

        else
            rub->next_block = ub;
    }

    return TRUE;
}

Bool32 CCOM_GetUserBlock(CCOM_comp * comp, CCOM_USER_BLOCK * ublock)
{
    CCOM_USER_BLOCK * ub;

    if (!comp || !ublock) {
        wLowRC = CCOM_ERR_NULL;
        return FALSE;
    }

    if (ublock->code) {
        ub = comp->user_block;

        if (ub)
            while (ub) {
                if (ub->code == ublock->code) {
                    ublock->data = ub->data;
                    ublock->size = ub->size;
                    return TRUE;
                }

                ub = ub->next_block;
            }
    }

    return FALSE;
}

Bool32 CCOM_MakeLP(RecRaster *rRaster, uchar *lp, int16_t *lp_size, int16_t *numcomp)
{
    if (!my_MakeLP)
        return FALSE;

    return my_MakeLP(rRaster, lp, lp_size, numcomp);
}

int32_t CCOM_NewUserCode(void)
{
    user_number++;
    return user_number;
}

void ccom_new_save(void)
{
    FILE *fp = fopen(CCOM_DAT, "wb");
    fclose(fp);
}

Bool32 ccom_save_comp(CCOM_comp *cur)
{
    FILE *fp = fopen(CCOM_DAT, "wb+");
    CCOM_USER_BLOCK *ub = cur->user_block;
    uint32_t zub = { 0 };

    if (!fp)
        return FALSE;

    if (fwrite(cur, sizeof(CCOM_comp), 1, fp) != 1) {
        fclose(fp);
        return FALSE;
    }


    if (fwrite(cur->linerep, cur->size_linerep, 1, fp) != 1) {
        fclose(fp);
        return FALSE;
    }

    while (ub) {
        if (fwrite(ub, 8, 1, fp) != 1) {
            fclose(fp);
            return FALSE;
        }

        if (fwrite(ub->data, ub->size, 1, fp) != 1) {
            fclose(fp);
            return FALSE;
        }

        ub = ub->next_block;
    }

    if (fwrite(&zub, 4, 1, fp) != 1) {
        fclose(fp);
        return FALSE;
    }

    fclose(fp);
    return TRUE;
}

Bool32 CCOM_Backup(CCOM_handle hcont)
{
    CCOM_cont *curr, *cont = (CCOM_cont*) hcont;
    CCOM_comp *current;

    for (curr = top.next; curr != &tail && curr != cont; curr = curr->next)
        ;

    if (curr != cont) { // not found
        wLowRC = CCOM_ERR_VALUE;
        return FALSE;
    }

    ccom_new_save();

    for (current = curr->first; current != NULL; current = current->next_comp) {
        if (!ccom_save_comp(current)) {
            wLowRC = CCOM_ERR_FILEACCESS;
            return FALSE;
        }
    }

    return TRUE;
}

CCOM_handle CCOM_Restore(void)
{

    CCOM_comp cur, *real_comp;
    CCOM_USER_BLOCK *ub;
    int32_t zub;
    CCOM_handle hnd = CCOM_CreateContainer();

    if (hnd == NULL)
        return NULL;

    FILE * fp = fopen(CCOM_DAT, "rb");

    if (!fp) {
        wLowRC = CCOM_ERR_FILEACCESS;
        CCOM_DeleteContainer(hnd);
        return NULL;
    }

    while (1) {
        if (fread(&cur, sizeof(CCOM_comp), 1, fp) != 1)
            break; // EOF

        cur.linerep = static_cast<uchar*> (calloc(1, cur.size_linerep));

        if (!cur.linerep) {
            wLowRC = CCOM_ERR_NOMEMORY;
            return (CCOM_handle) NULL;
        }

        if (fread(cur.linerep, cur.size_linerep, 1, fp) != 1) {
            wLowRC = CCOM_ERR_FILEACCESS;
            return (CCOM_handle) NULL;
        }

        real_comp = CCOM_New(hnd, cur.upper, cur.left, cur.w, cur.h);
        CCOM_Store(real_comp, cur.numcomp, cur.size_linerep, cur.linerep, cur.nl, cur.begs,
                   cur.ends, cur.vers, NULL);

        while (1) {
            if (fread(&zub, 4, 1, fp) != 1) {
                wLowRC = CCOM_ERR_FILEACCESS;
                return NULL;
            }

            if (zub == 0)
                break;

            ub = static_cast<CCOM_USER_BLOCK*> (calloc(1, sizeof(CCOM_USER_BLOCK)));

            if (ub == NULL) {
                wLowRC = CCOM_ERR_NOMEMORY;
                return NULL;
            }

            ub->code = zub;

            if (fread(&ub->size, 4, 1, fp) != 1) {
                wLowRC = CCOM_ERR_FILEACCESS;
                free(ub);
                return NULL;
            }

            ub->data = static_cast<uchar*> (calloc(1, ub->size));

            if (ub->data == NULL) {
                wLowRC = CCOM_ERR_NOMEMORY;
                free(ub);
                return NULL;
            }

            if (fread(ub->data, ub->size, 1, fp) != 1) {
                wLowRC = CCOM_ERR_FILEACCESS;
                free(ub->data);
                free(ub);
                return NULL;
            }

            CCOM_SetUserBlock(real_comp, ub);
        }
    }

    fclose(fp);
    return hnd;
}

CCOM_comp *CCOM_LargeNew(CCOM_handle hcont, int32_t upper, int32_t left, int32_t w, int32_t h,
                         int32_t size)
{
    CCOM_comp *comp = CCOM_New(hcont, upper, left, w, h);

    if (!comp)
        return (CCOM_comp*) NULL;

    comp->scale = CCOM_LONGLINES;

    if (size > 32767) {
        comp->size_linerep = -1;
    }

    else
        comp->size_linerep = (int16_t) size;

    comp->linerep = static_cast<uchar*> (calloc(1, size + 4));

    if (!comp->linerep)
        return (CCOM_comp*) NULL;

    *((int16_t*) comp->linerep) = comp->size_linerep;
    comp->user_block = (CCOM_USER_BLOCK *) (comp->linerep + 2); // first line
    return comp;
}

Bool32 CCOM_LargeNewLn(CCOM_comp *comp, CCOM_lnhead **lnh)
{
    uchar *p = (uchar*) comp->user_block;

    if (!comp->user_block) {
        *lnh = (CCOM_lnhead*) NULL;
        return FALSE;
    }

    *lnh = (CCOM_lnhead*) comp->user_block;
    p += 8;
    comp->user_block = (CCOM_USER_BLOCK *) p; // +=2*4 = 8 bytes = 4 word16
    return TRUE;
}

Bool32 CCOM_LargeNewInterval(CCOM_comp *comp, int16_t e, int16_t l)
{
    uchar *p = (uchar*) comp->user_block;
    CCOM_interval16 inter = { l, e };
    memcpy(p, (uchar*) &inter, 4);
    p += 4;
    comp->user_block = (CCOM_USER_BLOCK *) p; // +=4 bytes = 2 word16
    return TRUE;
}

void CCOM_LargeClose(CCOM_comp *comp)
{
    comp->user_block = NULL;
    comp->type = CCOM_CH_GREAT;
}

Bool32 CCOM_Kill(CCOM_comp *c)
{
    c->large |= CCOM_LR_KILLED;
    return TRUE;
}

Bool32 CCOM_Reanimate(CCOM_comp *c)
{
    c->large &= ~CCOM_LR_KILLED;
    return TRUE;
}

Bool32 CCOM_ClearContatiner(CCOM_handle hcont)
{
    CCOM_cont *curr, *cont = (CCOM_cont*) hcont;
    CCOM_comp * next_comp, *current;

    for (curr = top.next; curr != &tail && curr != cont; curr = curr->next)
        ;

    if (curr != cont) { // not found
        wLowRC = CCOM_ERR_VALUE;
        return FALSE;
    }

    for (current = curr->first; current != NULL; current = next_comp) {
        next_comp = current->next_comp;
        CCOM_Delete(hcont, current);
    }

    return TRUE;
}

Bool32 CCOM_CompressContatiner(CCOM_handle hcont)
{
    CCOM_cont *curr, *cont = (CCOM_cont*) hcont;
    CCOM_comp * next_comp, *current;

    if (!cont->kill_dust_mode)
        return TRUE;

    for (curr = top.next; curr != &tail && curr != cont; curr = curr->next)
        ;

    if (curr != cont) { // not found
        wLowRC = CCOM_ERR_VALUE;
        return FALSE;
    }

    for (current = curr->first; current != NULL; current = next_comp) {
        next_comp = current->next_comp;

        if (ccom_small_dust(current, hcont))
            CCOM_DeleteComp(hcont, current);
    }

    return TRUE;
}

Bool32 CCOM_SetLanguage(CCOM_handle hcont, int16_t language)
{
    CCOM_cont *cnt = (CCOM_cont*) hcont;

    if (!hcont) {
        wLowRC = CCOM_ERR_NULL;
        return FALSE;
    }

    cnt->language = language;
    return TRUE;
}

Bool32 CCOM_GetLanguage(CCOM_handle hcont, int16_t *language)
{
    CCOM_cont *cnt = (CCOM_cont*) hcont;

    if (!hcont) {
        wLowRC = CCOM_ERR_NULL;
        return FALSE;
    }

    *language = cnt->language;
    return TRUE;
}

Bool32 CCOM_SetImportData(uint32_t dwType, void * pData)
{
    wLowRC = CCOM_ERR_NO;

    switch (dwType) {
        case CCOM_FNIMP_MAKELP:
            my_MakeLP = (Bool32(*)(RecRaster*, uchar*, int16_t*, int16_t*)) pData;
            break;
        default:
            wLowRC = CCOM_ERR_NOTIMPLEMENT;
            return FALSE;
    }

    return TRUE;
}

int CCOM_Version()
{
    return CCOM_VERSION_CODE;
}
