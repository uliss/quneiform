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

// Windows Template Library - WTL version 8.0
// Copyright (C) Microsoft Corporation. All rights reserved.
//
// This file is a part of the Windows Template Library.
// The use and distribution terms for this software are covered by the
// Microsoft Permissive License (Ms-PL) which can be found in the file
// Ms-PL.txt at the root of this distribution.

#ifndef __ATLRES_H__
#define __ATLRES_H__

#pragma once

#if defined(_WIN32_WCE) && !defined(__ATLRESCE_H__)
	#error Use atlresCE.h instead of atlres.h for Windows CE
#endif


#ifdef RC_INVOKED
#ifndef _INC_WINDOWS

  #define _INC_WINDOWS

  #ifndef _WIN32_WCE
    #define VS_VERSION_INFO     1

    #ifdef APSTUDIO_INVOKED
      #define APSTUDIO_HIDDEN_SYMBOLS // Ignore following symbols
    #endif // APSTUDIO_INVOKED

    #ifndef WINVER
      #define WINVER 0x0400   // default to Windows Version 4.0
    #endif // !WINVER

    #include <winresrc.h>

    // operation messages sent to DLGINIT
    #define LB_ADDSTRING    (WM_USER+1)
    #define CB_ADDSTRING    (WM_USER+3)
  #endif // !_WIN32_WCE

  #ifdef APSTUDIO_INVOKED
    #undef APSTUDIO_HIDDEN_SYMBOLS
  #endif // APSTUDIO_INVOKED

  #ifdef IDC_STATIC
    #undef IDC_STATIC
  #endif // IDC_STATIC
  #define IDC_STATIC      (-1)

#endif // !_INC_WINDOWS
#endif // RC_INVOKED

#ifdef APSTUDIO_INVOKED
  #define APSTUDIO_HIDDEN_SYMBOLS
#endif // APSTUDIO_INVOKED

///////////////////////////////////////////////////////////////////////////////
// ATL resource types

#ifndef RC_INVOKED
  #define RT_DLGINIT  MAKEINTRESOURCE(240)
  #define RT_TOOLBAR  MAKEINTRESOURCE(241)
#endif // RC_INVOKED

///////////////////////////////////////////////////////////////////////////////

#ifdef APSTUDIO_INVOKED
  #undef APSTUDIO_HIDDEN_SYMBOLS
#endif // APSTUDIO_INVOKED

///////////////////////////////////////////////////////////////////////////////
// Standard window components

#define ID_SEPARATOR                    0       // special separator value
#define ID_DEFAULT_PANE                 0       // default status bar pane

#ifndef RC_INVOKED  // code only
// standard control bars (IDW = window ID)
  #define ATL_IDW_TOOLBAR               0xE800  // main Toolbar for window
  #define ATL_IDW_STATUS_BAR            0xE801  // Status bar window
  #define ATL_IDW_COMMAND_BAR           0xE802  // Command bar window

// parts of a frame window
  #define ATL_IDW_CLIENT                0xE900
  #define ATL_IDW_PANE_FIRST            0xE900  // first pane (256 max)
  #define ATL_IDW_PANE_LAST             0xE9FF
  #define ATL_IDW_HSCROLL_FIRST         0xEA00  // first Horz scrollbar (16 max)
  #define ATL_IDW_VSCROLL_FIRST         0xEA10  // first Vert scrollbar (16 max)

  #define ATL_IDW_SIZE_BOX              0xEA20  // size box for splitters
  #define ATL_IDW_PANE_SAVE             0xEA21  // to shift ATL_IDW_PANE_FIRST

// bands for a rebar
  #define ATL_IDW_BAND_FIRST            0xEB00
  #define ATL_IDW_BAND_LAST             0xEBFF
#endif // !RC_INVOKED

///////////////////////////////////////////////////////////////////////////////
// Standard Commands

// File commands
#define ID_FILE_NEW                     0xE100
#define ID_FILE_OPEN                    0xE101
#define ID_FILE_CLOSE                   0xE102
#define ID_FILE_SAVE                    0xE103
#define ID_FILE_SAVE_AS                 0xE104
#define ID_FILE_PAGE_SETUP              0xE105
#define ID_FILE_PRINT_SETUP             0xE106
#define ID_FILE_PRINT                   0xE107
#define ID_FILE_PRINT_DIRECT            0xE108
#define ID_FILE_PRINT_PREVIEW           0xE109
#define ID_FILE_UPDATE                  0xE10A
#define ID_FILE_SAVE_COPY_AS            0xE10B
#define ID_FILE_SEND_MAIL               0xE10C

#define ID_FILE_MRU_FIRST               0xE110
#define ID_FILE_MRU_FILE1               0xE110          // range - 16 max
#define ID_FILE_MRU_FILE2               0xE111
#define ID_FILE_MRU_FILE3               0xE112
#define ID_FILE_MRU_FILE4               0xE113
#define ID_FILE_MRU_FILE5               0xE114
#define ID_FILE_MRU_FILE6               0xE115
#define ID_FILE_MRU_FILE7               0xE116
#define ID_FILE_MRU_FILE8               0xE117
#define ID_FILE_MRU_FILE9               0xE118
#define ID_FILE_MRU_FILE10              0xE119
#define ID_FILE_MRU_FILE11              0xE11A
#define ID_FILE_MRU_FILE12              0xE11B
#define ID_FILE_MRU_FILE13              0xE11C
#define ID_FILE_MRU_FILE14              0xE11D
#define ID_FILE_MRU_FILE15              0xE11E
#define ID_FILE_MRU_FILE16              0xE11F
#define ID_FILE_MRU_LAST                0xE11F

// Edit commands
#define ID_EDIT_CLEAR                   0xE120
#define ID_EDIT_CLEAR_ALL               0xE121
#define ID_EDIT_COPY                    0xE122
#define ID_EDIT_CUT                     0xE123
#define ID_EDIT_FIND                    0xE124
#define ID_EDIT_PASTE                   0xE125
#define ID_EDIT_PASTE_LINK              0xE126
#define ID_EDIT_PASTE_SPECIAL           0xE127
#define ID_EDIT_REPEAT                  0xE128
#define ID_EDIT_REPLACE                 0xE129
#define ID_EDIT_SELECT_ALL              0xE12A
#define ID_EDIT_UNDO                    0xE12B
#define ID_EDIT_REDO                    0xE12C

// Window commands
#define ID_WINDOW_NEW                   0xE130
#define ID_WINDOW_ARRANGE               0xE131
#define ID_WINDOW_CASCADE               0xE132
#define ID_WINDOW_TILE_HORZ             0xE133
#define ID_WINDOW_TILE_VERT             0xE134
#define ID_WINDOW_SPLIT                 0xE135
#ifndef RC_INVOKED      // code only
  #define ATL_IDM_WINDOW_FIRST          0xE130
  #define ATL_IDM_WINDOW_LAST           0xE13F
  #define ATL_IDM_FIRST_MDICHILD        0xFF00  // window list starts here
  #define ATL_IDM_LAST_MDICHILD         0xFFFD
#endif // !RC_INVOKED
// TabView
#define ID_WINDOW_TABFIRST              0xFF00	// = ATL_IDM_FIRST_MDICHILD
#define ID_WINDOW_TABLAST               0xFFFD
#define ID_WINDOW_SHOWTABLIST           0xFFFE

// Help and App commands
#define ID_APP_ABOUT                    0xE140
#define ID_APP_EXIT                     0xE141
#define ID_HELP_INDEX                   0xE142
#define ID_HELP_FINDER                  0xE143
#define ID_HELP_USING                   0xE144
#define ID_CONTEXT_HELP                 0xE145      // shift-F1
// special commands for processing help
#define ID_HELP                         0xE146      // first attempt for F1
#define ID_DEFAULT_HELP                 0xE147      // last attempt

// Misc
#define ID_NEXT_PANE                    0xE150
#define ID_PREV_PANE                    0xE151
#define ID_PANE_CLOSE                   0xE152

// Format
#define ID_FORMAT_FONT                  0xE160

// Scroll
#define ID_SCROLL_UP                    0xE170
#define ID_SCROLL_DOWN                  0xE171
#define ID_SCROLL_PAGE_UP               0xE172
#define ID_SCROLL_PAGE_DOWN             0xE173
#define ID_SCROLL_TOP                   0xE174
#define ID_SCROLL_BOTTOM                0xE175
#define ID_SCROLL_LEFT                  0xE176
#define ID_SCROLL_RIGHT                 0xE177
#define ID_SCROLL_PAGE_LEFT             0xE178
#define ID_SCROLL_PAGE_RIGHT            0xE179
#define ID_SCROLL_ALL_LEFT              0xE17A
#define ID_SCROLL_ALL_RIGHT             0xE17B

// OLE commands
#define ID_OLE_INSERT_NEW               0xE200
#define ID_OLE_EDIT_LINKS               0xE201
#define ID_OLE_EDIT_CONVERT             0xE202
#define ID_OLE_EDIT_CHANGE_ICON         0xE203
#define ID_OLE_EDIT_PROPERTIES          0xE204
#define ID_OLE_VERB_FIRST               0xE210     // range - 16 max
#ifndef RC_INVOKED      // code only
  #define ID_OLE_VERB_LAST              0xE21F
#endif // !RC_INVOKED

// View commands (same number used as IDW used for toolbar and status bar)
#define ID_VIEW_TOOLBAR                 0xE800
#define ID_VIEW_STATUS_BAR              0xE801
#define ID_VIEW_REFRESH                 0xE803

///////////////////////////////////////////////////////////////////////////////
// Standard control IDs

#ifdef IDC_STATIC
  #undef IDC_STATIC
#endif // IDC_STATIC
#define IDC_STATIC              (-1)     // all static controls

///////////////////////////////////////////////////////////////////////////////
// Standard string error/warnings

// idle status bar message
#define ATL_IDS_IDLEMESSAGE             0xE001

#ifndef RC_INVOKED      // code only
  #define ATL_IDS_SCFIRST               0xEF00
#endif // !RC_INVOKED

#define ATL_IDS_SCSIZE                  0xEF00
#define ATL_IDS_SCMOVE                  0xEF01
#define ATL_IDS_SCMINIMIZE              0xEF02
#define ATL_IDS_SCMAXIMIZE              0xEF03
#define ATL_IDS_SCNEXTWINDOW            0xEF04
#define ATL_IDS_SCPREVWINDOW            0xEF05
#define ATL_IDS_SCCLOSE                 0xEF06
#define ATL_IDS_SCRESTORE               0xEF12
#define ATL_IDS_SCTASKLIST              0xEF13

#define ATL_IDS_MDICHILD                0xEF1F
#define ATL_IDS_MRU_FILE                0xEFDA

///////////////////////////////////////////////////////////////////////////////
// Misc. control IDs

// Property Sheet control id's (determined with Spy++)
#define ID_APPLY_NOW                    0x3021
#define ID_WIZBACK                      0x3023
#define ID_WIZNEXT                      0x3024
#define ID_WIZFINISH                    0x3025
#define ATL_IDC_TAB_CONTROL             0x3020

#endif // __ATLRES_H__
