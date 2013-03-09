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

#ifndef RSTUFF_STRUCT_H
#define RSTUFF_STRUCT_H

#include "cttypes.h"
#include "common/rect.h"
#include "common/dib.h"
#include "cpage/cpagedefs.h"

struct CCOM_cont;

struct RSPreProcessImage {
    puchar * pgpRecogDIB;
    Bool32 gbAutoRotate;
    Bool32 gbDotMatrix;
    Bool32 gbFax100;
    uint32_t gnLanguage;
    uint32_t gnTables;
    CPageHandle hCPAGE;
    Handle hDebugCancelSearchPictures;
    Handle hDebugCancelComponent;
    Handle hDebugCancelTurn;
    Handle hDebugCancelSearchLines;
    Handle hDebugCancelVerifyLines;
    Handle hDebugCancelSearchDotLines;
    Handle hDebugCancelRemoveLines;
    Handle hDebugCancelSearchTables;
    Handle hDebugCancelAutoTemplate;
    Handle hDebugEnableSearchSegment;
    const char ** pglpRecogName;
    CCOM_cont ** phCCOM;
    cf::BitmapInfoHeader * pinfo;
    Handle* phLinesCCOM;
    void * phCLINE;
    PBool32 pgneed_clean_line;
    int * pgnNumberTables;
    uint32_t gnPictures;
    Bool32* pgrc_line;
};

typedef RSPreProcessImage * PRSPreProcessImage;

struct RSCBProgressPoints {
    void * pSetUpdate;
};

typedef RSCBProgressPoints * PRSCBProgressPoints;

#endif // RSTUFF_STRUCT_H
