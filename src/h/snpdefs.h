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

/////////////////////////////////////////////////////////////
// Snp definitions
/////////////////////////////////////////////////////////////
// NOTE: to add new snp function - make 4 steps, for first step
// search 'TODO: (1/4)'comment below and follow instructions
//////////////////////////////////////////////////////////////

#ifndef __SNPDEFS_H
#define __SNPDEFS_H

#define NO_SNP // JussiP: I had to disable all of SNP because it caused compile errors.
#include "cttypes.h"
#include "recdefs.h"

#define SNP_SECTION  "Snp section"  // APPLICATION.ini std section
/////////////////////////////////////////////////////////////
// SnpTreeNode structure - used to organize snap tree
// to control execution on application level:
//

class SnpTreeNode   // !!! NOTE: You should place it in static dll's data segment
{
        // !!! NOTE: Don't use SnpTreeNode fields directly
        // (from dll code to be snapped) - use proper
        // Snp...( node, ... ) functions instead

        // Public Attributes
    public:
        uint32_t Status;
        // zero - not active node
#define STN_DRAW           0x00000001L  // some drawing permitted by user
#define STN_STOP           0x00000002L  // stopping permitted by user
#define STN_FROZENCHILDS   0x00010000L  // node iterated and freezes childs
#define NOT_ACTIVE(x) (((x) & (STN_DRAW | STN_STOP | STN_FROZENCHILDS)) == 0)

#define STN_EXPANDED        0x00000004L  // TreeView node is expanded
#define STN_SELECTED        0x00000008L  // TreeView node is selected
        char Name[64];

        uint32_t UserLocal;
        // for local usage

        uint32_t hTreeCtrl;
        // handle of proper TreeCtrl item, don't touch!
    private:
        SnpTreeNode* Parent;
        SnpTreeNode* Next;
        SnpTreeNode* Prev;
        SnpTreeNode* FirstChild;

        friend
        // SnpTreeNode construction and connection;
        void SnpAddNode( // should be called from dll's XxxRegisterSnpTree();
            SnpTreeNode * node, // on module's static data segment
            const char * name, // user readable
            SnpTreeNode * parent);

    public:
        SnpTreeNode* GetParent() {
            return Parent;
        }

        SnpTreeNode* GetNext() {
            return Next;
        }

        SnpTreeNode* GetPrev() {
            return Prev;
        }

        SnpTreeNode* GetFirstBaby() {
            return FirstChild;
        }
        // Iterability
    private:
        uint32_t IterTotal;
        // set in dll when node is used inside loop
        // (if count of iteration is known);
        // does not control execution and nodes activation
        // used simply to bring information for user

        uint32_t IterCur;
        // used when node is located inside loop
        // should be set (in dll) to 0 at start of loop
        // and incremented at start of each iteration

        uint32_t IterParent;
        // if nonzero - node not activated;
        // increased by iterated parent node
        // to temporary disable activation
    public:
        uint32_t IterStop;
        // normally set by user to activate
        // node when IterStop == IterCur
        // first iteration correspond to 1 (but not 0)

    public:
        uint32_t GetIterTotal() {
            return IterTotal;
        }

        uint32_t GetIterCur() {
            return IterCur;
        }

        uint32_t GetIterParent() {
            return IterParent;
        }

        uint32_t GetIterStop() {
            return IterStop;
        }

        friend void __SnpIterParent(SnpTreeNode* node, Bool activate);
        friend void SnpStartLoop(SnpTreeNode* node, uint32_t iter_total);
        friend void SnpLoopNext(SnpTreeNode* node);
        friend Bool16 SnpSkip(SnpTreeNode* node);
};

// User implemented handlers of events, called from application:
typedef
void (*FTOnMouseDown)( // called when mouse clicked in zone
    CIF::Point16* mouse_pt,
    // in image real coords
    Rect16* zone_rect,
    // in image real coords
    uint32_t users_zone_handle,
    // one of the SetZoneOn() parameters
    uint32_t apps_zone_handle
// SetZoneOn() returned value
);

///////////////////////////////////////////////////////////////////
// Prototypes of callbacks, implemented on application level
// and called from dlls:
typedef
int (*FTLog)( // makes string and loggs it out
    const char* format_string, // sprintf() like
    ... // other arguments
);
#define SNP_MAXMESSAGE   1024
// Note: result string length should be less SNP_MAXMESSAGE

typedef
void (*FTStatusLine)( // sprintfs and displays status line
    char* status_line_message, ...);
// Note: result string length should be less SNP_MAXMESSAGE

typedef
void (*FTDrawRect)( // adds rect to be drawn
    Rect16* rc,
    // in image ideal coords
    int32_t skew,
    // ideal-to-real coords skew (Tangens/1024)
    uint32_t rgb_color,
    // rect color, use "wrgb.h" if windows.h invisible
    int32_t pen_width,
    // positive - absolute pen width in screen pixels
    // if zero - fills all rect
    // negative means final pen width == zoom*100/(-width),
    // f.e. -100 means one real image pixel width
    uint32_t key
// some unique nonzero number to be used later on call HideRects;
    // usially address of proper SnpTreeNode casted to uint32_t
);

typedef
void (*FTHideRects)( // hides rects with specified key, 0-hides all
    uint32_t key);

typedef
void (*FTDrawLine)( // adds line to be drawn
    CIF::Point16* start, CIF::Point16* end,
    // in image ideal coords
    int32_t skew,
    // ideal-to-real coords skew (Tangens/1024)
    uint32_t rgb_color,
    // rect color, use "wrgb.h" if windows.h invisible
    int16_t pen_width,
    // positive - absolute pen width in screen pixels
    // if zero - no drawing
    // negative means: final pen width == zoom*100/(-width),
    // f.e. -100 means one real image pixel width
    uint32_t key
// some unique nonzero number to be used later on call HideLines
    // usially address of proper SnpTreeNode casted to uint32_t
);

typedef
void (*FTHideLines)( // hides lines with specified key, 0-hides all
    uint32_t key);

typedef
void (*FTDrawFocusRect)( // immediately shows/hides XOR focus rect
    Rect16* rc
// in image real coords
);

typedef uint32_t (*FTSetZoneOn)( // setup zone expected mouse input:
    Rect16* zone_rect,
    // in image real coords
    uint32_t rgb_color,
    // zone color, use "wrgb.h" if "windows.h" invisible
    char* status_line_comment,
    // to be shown in status line when mouse moves in; may be NULL
    uint32_t users_zone_handle,
    // user defined handle to be passed back by on_mouse_down(..user_zone_handle...)
    FTOnMouseDown on_mouse_down
// user's handler of "mouse down" event; may be NULL
);
// returns zone handle to be passed to SetMouseZoneOff()

typedef
void (*FTSetZoneOff)( // turns zone off
    uint32_t zone_handle
// result of SetZoneOn
);

typedef void (*FTUpdateViews)(void); // redraws views if necessary

typedef void (*FTMessBoxOk)(char * message);
typedef Bool16 (*FTMessBoxYesNo)(char * message); // Yes or No ?
typedef Bool16 (*FTGetUserString)( // runs dialog to accept single string
    char * static_text, char * result_string, // in - on start, out - result
    uint32_t result_string_length // max length
);
// returns FALSE if user cancel
// in this case *result_long is not changed

typedef Bool16 (*FTGetUserLong)( // runs dialog to accept single int32_t
    char * static_text, int32_t * result_long);
// returns FALSE if user cancel;
// in this case *result_long is not changed

typedef Bool16 (*FTGetUserRect)( // starts mouse rect input
    Rect16* rect
// rect to be returned in image real coords
);

typedef Bool16 (*FTGetUserPoint)( // starts mouse point input
    CIF::Point16* rect
// point to be returned in image real coords
);

typedef uint32_t (*FTWaitUserInput)( // starts input loop
    SnpTreeNode* cur_node
// current SnpTreeNode, used to correct following navigation
);
#ifndef DPUMA_SNAP
// returned value notifies about next point of stopping;
// snap tree is updated from application
#define  SNP_RUN        1  // all snp nodes desactivated
#define  SNP_TRACE      2  // go to active next node
#define  SNP_STEPIN     3  // activates childs
#define  SNP_STEPNEXT   4  // desactivate cur, activate sibling
#define  SNP_STEPOUT    5  // desactivate cur, activate parent
#define  SNP_NEXTITER   6  // increase IterCnt once
#define  SNP_DEBUG      7  // fall to debugger
// special codes to be treated specifically in some nodes
#define  SNP_F1         11    // recommended for help for node (about other keys)
#define  SNP_F2         12
#define  SNP_F3         13
#define  SNP_F4         14
#define  SNP_F5         15
#define  SNP_F6         16
#define  SNP_F7         17
#define  SNP_F8         18
#define  SNP_F9         19
#define  SNP_F10        20
#define  SNP_LEFT       21
#define  SNP_RIGHT      22
#define  SNP_UP         23
#define  SNP_DOWN       24
#define  SNP_HOME       25
#define  SNP_END        26
#define  SNP_GRAY_ENTER 27
#endif
typedef void (*FTNotifyAppl)( // notifies about node activation
    SnpTreeNode* cur_node // activated SnpTreeNode
);

typedef void (*FTDrawRaster)( // set specified raster
    RecRaster* raster // to be drawn in special window
);

typedef void (*FTRasterText)( // char string to raster window
    char *Text //
);

typedef void (*FTRasterHeader)( // header string with marked position
    char *Text, int32_t mark // to raster window
);

typedef void (*FTZoomToRect)( //zooms tiff window to given rectangle
    Rect16 *rc_in_logic // in real coords
);

//typedef void   (*FT..........)(   // COPY ME!
//   ...........................    //
//                                 );

// TODO: (1/4) insert new function prototype here ^^^ (before this comment)
// Next (2/4) step - see 'TODO: (2/4)' below
///////////////////////////////////////////////////////////////////////////

typedef void (*FTStub)( // stub for future functions
    void //
);

//////////////////////////////////////////////////////////////
// SnpToolBox internal structure - complect of callbacks to
// be supplied from application to dlls:
//
// Note:
// If some tool is not implemented, it's stub should be
// addressed; no NULL members assumed when structure is used
// inside dll
//
// If snap mode is Off - NULL pointer  should be passed to dll
// instead of SnpToolBox structure address.
//


typedef struct tag__SnpToolBox {
    FTLog fLog; // sprintfs and loggs out
    FTStatusLine fStatusLine; // sprintfs and set to status line
    FTDrawRect fDrawRect; // adds rect to be drawn
    FTHideRects fHideRects; // hides rects by key
    FTDrawLine fDrawLine; // adds line to be drawn
    FTHideLines fHideLines; // hides lines by key
    FTDrawFocusRect fDrawFocusRect; // immediately shows/hides XOR focus rect
    FTSetZoneOn fSetZoneOn; // setup zone expected mouse input:
    FTSetZoneOff fSetZoneOff; // turns zone off
    FTUpdateViews fUpdateViews; // redraws views
    FTMessBoxOk fMessBoxOk; // invokes OK MessageBox
    FTMessBoxYesNo fMessBoxYesNo; // invokes Yes/No MessageBox
    FTGetUserString fGetUserString; // runs dialog to accept zt string
    FTGetUserLong fGetUserLong; // runs dialog to accept single int32_t
    FTGetUserRect fGetUserRect; // starts mouse rect input
    FTGetUserPoint fGetUserPoint; // starts mouse point input
    FTWaitUserInput fWaitUserInput; // starts input loop
    FTNotifyAppl fNotifyAppl; // notifies appl. about node activation
    FTDrawRaster fDrawRaster; // draws specified raster
    FTRasterText fRasterText; // char string to raster window
    FTRasterHeader fRasterHeader; // header string with marked position
    FTZoomToRect fZoomToRect; // zooms tiff window to given rectangle
    // in real coords

    // FT..........      f..........;    // COPY ME!..............
    // TODO: (2/4) insert new functions pointers here ^^^ (before this comment)
    // and decrease fStubs[] length
    // Next step (3/4) you should made in SNPTOOLS.H
    FTStub fStubs[14];
} __SnpToolBox; // internal usage!

#endif
