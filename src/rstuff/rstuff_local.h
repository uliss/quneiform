/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#ifndef RSTUFF_LOCAL_H
#define RSTUFF_LOCAL_H

#include "globus.h"
#include "rstuff_struct.h"

#ifdef __RSTUFF__
#define RSTUFF_FUNC  FUN_EXPO__
#else
#define RSTUFF_FUNC  FUN_IMPO__
#endif

Bool32 RSTUFF_Init(uint16_t wHeightCode, Handle hStorage);
Bool32 RSTUFF_Done();
Bool32 RSTUFF_SetImportData(uint32_t dwType, void * pData);

typedef enum {
    RSTUFF_FN_RSBinarise = 1,
    RSTUFF_FN_RSNormalise,
    RSTUFF_FN_RSLayout,
    RSTUFF_FN_RSSetSpecPrj
} RSTUFF_EXPORT_ENTRIES;

typedef enum {
    RSTUFF_FN_SetProgresspoints = 128,
    RSTUFF_FN_SetProgressStart,
    RSTUFF_FN_SetProgressStep,
    RSTUFF_FN_SetProgressFinish,
    RSTUFF_FN_SetInitPRGTIME,
    RSTUFF_FN_SetDPumaSkipComponent
} RSTUFF_IMPORT_ENTRIES;

/*  Описание функций  */
Bool32 RSTUFF_RSBinarise(void);
Bool32 RSTUFF_RSNormalise(PRSPreProcessImage, void* vBuff, int Size, void* vWork, int SizeWork);
Bool32 RSTUFF_RSNormVerify(PRSPreProcessImage);
Bool32 RSTUFF_RSNormRemoveLines(PRSPreProcessImage);
Bool32 RSTUFF_RSLayout(PRSPreProcessImage);

#endif // RSTUFF_LOCAL_H
