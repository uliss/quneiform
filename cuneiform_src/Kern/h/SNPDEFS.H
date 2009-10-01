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
#include "ctmacros.h"
#include "recdefs.h"

#define SNP_SECTION  "Snp section"  // APPLICATION.ini std section

/////////////////////////////////////////////////////////////
// SnpTreeNode structure - used to organize snap tree
// to control execution on application level:
//

#if defined( __cplusplus ) && !defined( DPUMA_SNAP)
   class SnpTreeNode
#else
   typedef struct tagSnpTreeNode
#endif
   {  // !!! NOTE: You should place it in static dll's data segment
      // !!! NOTE: Don't use SnpTreeNode fields directly
      // (from dll code to be snapped) - use proper
      // Snp...( node, ... ) functions instead

   ////////////////////////////////////////////////
   // Public Attributes
   CppPublic
      uint32_t         Status;
         // zero - not active node
         #define STN_DRAW           0x00000001L  // some drawing permitted by user
         #define STN_STOP           0x00000002L  // stopping permitted by user
         #define STN_FROZENCHILDS   0x00010000L  // node iterated and freezes childs

		 #define NOT_ACTIVE(x) (((x) & (STN_DRAW | STN_STOP | STN_FROZENCHILDS)) == 0)

 		 #define STN_EXPANDED     	0x00000004L  // TreeView node is expanded
		 #define STN_SELECTED		0x00000008L	 // TreeView node is selected

     char           Name[64];

      uint32_t         UserLocal;
         // for local usage

      uint32_t         hTreeCtrl;
         // handle of proper TreeCtrl item, don't touch!

   /////////////////////////////////////////////////
   // Node Linkage
#if defined( __cplusplus ) && !defined( DPUMA_SNAP)
   private:
      SnpTreeNode*            Parent;
      SnpTreeNode*            Next;
      SnpTreeNode*            Prev;
      SnpTreeNode*            FirstChild;

   friend
                        // SnpTreeNode construction and connection;
      void SnpAddNode(  // should be called from dll's XxxRegisterSnpTree();
            SnpTreeNode *  node, // on module's static data segment
            const char  *  name, // user readable
            SnpTreeNode *  parent
                     );

   public:
      SnpTreeNode* GetParent()    { return Parent; };
      SnpTreeNode* GetNext()      { return Next;   };
      SnpTreeNode* GetPrev()      { return Prev;   };
      SnpTreeNode* GetFirstBaby(){ return FirstChild;};
#else
      struct tagSnpTreeNode*  Parent;
      struct tagSnpTreeNode*  Next;
      struct tagSnpTreeNode*  Prev;
      struct tagSnpTreeNode*  FirstChild;
#endif

   ////////////////////////////////////////////////
   // Iterability

   CppPrivate
      uint32_t         IterTotal;
         // set in dll when node is used inside loop
         // (if count of iteration is known);
         // does not control execution and nodes activation
         // used simply to bring information for user

      uint32_t         IterCur;
         // used when node is located inside loop
         // should be set (in dll) to 0 at start of loop
         // and incremented at start of each iteration

      uint32_t         IterParent;
         // if nonzero - node not activated;
         // increased by iterated parent node
         // to temporary disable activation
   CppPublic
      uint32_t         IterStop;
         // normally set by user to activate
         // node when IterStop == IterCur
         // first iteration correspond to 1 (but not 0)



#if defined( __cplusplus ) && !defined( DPUMA_SNAP)
      public:
         uint32_t GetIterTotal() { return IterTotal; };
         uint32_t GetIterCur()   { return IterCur; };
         uint32_t GetIterParent(){ return IterParent; };
         uint32_t GetIterStop()  { return IterStop; };
   friend        void   __SnpIterParent( SnpTreeNode* node, Bool activate );
   friend        void    SnpStartLoop( SnpTreeNode* node, uint32_t iter_total );
   friend        void    SnpLoopNext( SnpTreeNode* node );
   friend        Bool16  SnpSkip( SnpTreeNode* node );
#endif

#if defined( __cplusplus ) && !defined( DPUMA_SNAP)
   }; // SnpTreeNode class
#else
   } SnpTreeNode;
#endif

//////////////////////////////////////////////////////////////
// User implemented handlers of events, called from application:
//

   typedef
   void (*FTOnMouseDown)( // called when mouse clicked in zone
      Point16* mouse_pt,
         // in image real coords
      Rect16* zone_rect,
         // in image real coords
      uint32_t   users_zone_handle,
         // one of the SetZoneOn() parameters
      uint32_t   apps_zone_handle
         // SetZoneOn() returned value
                        );

///////////////////////////////////////////////////////////////////
// Prototypes of callbacks, implemented on application level
// and called from dlls:
//

   typedef
   int  (*FTLog)(                // makes string and loggs it out
      const char* format_string, // sprintf() like
      ...                        // other arguments
                );
      #define SNP_MAXMESSAGE   1024
      // Note: result string length should be less SNP_MAXMESSAGE

   typedef
   void (*FTStatusLine)(   // sprintfs and displays status line
      char* status_line_message,
      ...
                       );
      // Note: result string length should be less SNP_MAXMESSAGE

   typedef
   void (*FTDrawRect)( // adds rect to be drawn
      Rect16* rc,
         // in image ideal coords
      int32_t  skew,
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
      uint32_t key
                     );

   typedef
   void (*FTDrawLine)( // adds line to be drawn
      Point16* start, Point16* end,
         // in image ideal coords
      int32_t  skew,
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
      uint32_t key
                     );

   typedef
   void (*FTDrawFocusRect)( // immediately shows/hides XOR focus rect
      Rect16* rc
         // in image real coords
                          );

   typedef
   uint32_t (*FTSetZoneOn)(   // setup zone expected mouse input:
      Rect16*  zone_rect,
         // in image real coords
      uint32_t   rgb_color,
         // zone color, use "wrgb.h" if "windows.h" invisible
      char*    status_line_comment,
         // to be shown in status line when mouse moves in; may be NULL
      uint32_t   users_zone_handle,
         // user defined handle to be passed back by on_mouse_down(..user_zone_handle...)
      FTOnMouseDown  on_mouse_down
         // user's handler of "mouse down" event; may be NULL
                        );
      // returns zone handle to be passed to SetMouseZoneOff()

   typedef
   void (*FTSetZoneOff)( // turns zone off
      uint32_t zone_handle
         // result of SetZoneOn
                       );

   typedef void (*FTUpdateViews)(void); // redraws views if necessary

   typedef void   (*FTMessBoxOk)( char * message );
   typedef Bool16 (*FTMessBoxYesNo)( char * message ); // Yes or No ?
   typedef Bool16 (*FTGetUserString)( // runs dialog to accept single string
      char * static_text,
      char * result_string,         // in - on start, out - result
      uint32_t result_string_length   // max length
                                    );
      // returns FALSE if user cancel
      // in this case *result_long is not changed

   typedef Bool16 (*FTGetUserLong)( // runs dialog to accept single int32_t
      char * static_text,
      int32_t * result_long
                                 );
      // returns FALSE if user cancel;
      // in this case *result_long is not changed

   typedef Bool16 (*FTGetUserRect)( // starts mouse rect input
      Rect16* rect
         // rect to be returned in image real coords
                                  );

   typedef Bool16 (*FTGetUserPoint)( // starts mouse point input
      Point16* rect
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
#else
#if !defined(_WINUSER_) && !defined(WINUSERAPI)
		#define VK_LBUTTON        0x01
		#define VK_RBUTTON        0x02
		#define VK_CANCEL         0x03
		#define VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

		#define VK_BACK           0x08
		#define VK_TAB            0x09

		#define VK_CLEAR          0x0C
		#define VK_RETURN         0x0D

		#define VK_SHIFT          0x10
		#define VK_CONTROL        0x11
		#define VK_MENU           0x12
		#define VK_PAUSE          0x13
		#define VK_CAPITAL        0x14


		#define VK_ESCAPE         0x1B

		#define VK_SPACE          0x20
		#define VK_PRIOR          0x21
		#define VK_NEXT           0x22
		#define VK_END            0x23
		#define VK_HOME           0x24
		#define VK_LEFT           0x25
		#define VK_UP             0x26
		#define VK_RIGHT          0x27
		#define VK_DOWN           0x28
		#define VK_SELECT         0x29
		#define VK_PRINT          0x2A
		#define VK_EXECUTE        0x2B
		#define VK_SNAPSHOT       0x2C
		#define VK_INSERT         0x2D
		#define VK_DELETE         0x2E
		#define VK_HELP           0x2F

		/* VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
		/* VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */

		#define VK_LWIN           0x5B
		#define VK_RWIN           0x5C
		#define VK_APPS           0x5D

		#define VK_NUMPAD0        0x60
		#define VK_NUMPAD1        0x61
		#define VK_NUMPAD2        0x62
		#define VK_NUMPAD3        0x63
		#define VK_NUMPAD4        0x64
		#define VK_NUMPAD5        0x65
		#define VK_NUMPAD6        0x66
		#define VK_NUMPAD7        0x67
		#define VK_NUMPAD8        0x68
		#define VK_NUMPAD9        0x69
		#define VK_MULTIPLY       0x6A
		#define VK_ADD            0x6B
		#define VK_SEPARATOR      0x6C
		#define VK_SUBTRACT       0x6D
		#define VK_DECIMAL        0x6E
		#define VK_DIVIDE         0x6F
		#define VK_F1             0x70
		#define VK_F2             0x71
		#define VK_F3             0x72
		#define VK_F4             0x73
		#define VK_F5             0x74
		#define VK_F6             0x75
		#define VK_F7             0x76
		#define VK_F8             0x77
		#define VK_F9             0x78
		#define VK_F10            0x79
		#define VK_F11            0x7A
		#define VK_F12            0x7B
		#define VK_F13            0x7C
		#define VK_F14            0x7D
		#define VK_F15            0x7E
		#define VK_F16            0x7F
		#define VK_F17            0x80
		#define VK_F18            0x81
		#define VK_F19            0x82
		#define VK_F20            0x83
		#define VK_F21            0x84
		#define VK_F22            0x85
		#define VK_F23            0x86
		#define VK_F24            0x87

		#define VK_NUMLOCK        0x90
		#define VK_SCROLL         0x91

		/*
		 * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
		 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
		 * No other API or message will distinguish left and right keys in this way.
		 */
		#define VK_LSHIFT         0xA0
		#define VK_RSHIFT         0xA1
		#define VK_LCONTROL       0xA2
		#define VK_RCONTROL       0xA3
		#define VK_LMENU          0xA4
		#define VK_RMENU          0xA5

		#if(WINVER >= 0x0400)
		#define VK_PROCESSKEY     0xE5
		#endif /* WINVER >= 0x0400 */

		#define VK_ATTN           0xF6
		#define VK_CRSEL          0xF7
		#define VK_EXSEL          0xF8
		#define VK_EREOF          0xF9
		#define VK_PLAY           0xFA
		#define VK_ZOOM           0xFB
		#define VK_NONAME         0xFC
		#define VK_PA1            0xFD
		#define VK_OEM_CLEAR      0xFE
#endif

      #define  SNP_RUN        VK_F5  // all snp nodes desactivated
      #define  SNP_TRACE      VK_F9  // go to active next node
      #define  SNP_STEPIN     VK_F11  // activates childs
      #define  SNP_STEPNEXT   VK_F10  // desactivate cur, activate sibling
//      #define  SNP_STEPOUT    5  // desactivate cur, activate parent
//      #define  SNP_NEXTITER   6  // increase IterCnt once
//      #define  SNP_DEBUG      7  // fall to debugger

      // special codes to be treated specifically in some nodes
      #define  SNP_F1         VK_F1    // recommended for help for node (about other keys)
      #define  SNP_F2         VK_F2
      #define  SNP_F3         VK_F3
      #define  SNP_F4         VK_F4
      #define  SNP_F5         VK_F5
      #define  SNP_F6         VK_F6
      #define  SNP_F7         VK_F7
      #define  SNP_F8         VK_F8
      #define  SNP_F9         VK_F9
      #define  SNP_F10        VK_F10
      #define  SNP_LEFT       VK_LEFT
      #define  SNP_RIGHT      VK_RIGHT
      #define  SNP_UP         VK_UP
      #define  SNP_DOWN       VK_DOWN
      #define  SNP_HOME       VK_HOME
      #define  SNP_END        VK_END
      #define  SNP_GRAY_ENTER VK_RETURN
#endif
   typedef void   (*FTNotifyAppl)(   // notifies about node activation
      SnpTreeNode* cur_node          // activated SnpTreeNode
                                    );


   typedef void   (*FTDrawRaster)(   // set specified raster
      RecRaster* raster              // to be drawn in special window
                                    );

   typedef void   (*FTRasterText)(   // char string to raster window
      char *Text                     //
                                    );

   typedef void   (*FTRasterHeader)( // header string with marked position
      char *Text, int32_t mark         // to raster window
                                    );

   typedef void   (*FTZoomToRect)(   //zooms tiff window to given rectangle
     Rect16 *rc_in_logic             // in real coords
                                 );


//typedef void   (*FT..........)(   // COPY ME!
 //   ...........................    //
 //                                 );

// TODO: (1/4) insert new function prototype here ^^^ (before this comment)
// Next (2/4) step - see 'TODO: (2/4)' below
///////////////////////////////////////////////////////////////////////////

   typedef void   (*FTStub      )(   // stub for future functions
      void                           //
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


   typedef struct    tag__SnpToolBox
   {
      FTLog             fLog;           // sprintfs and loggs out
      FTStatusLine      fStatusLine;    // sprintfs and set to status line
      FTDrawRect        fDrawRect;      // adds rect to be drawn
      FTHideRects       fHideRects;     // hides rects by key
      FTDrawLine        fDrawLine;      // adds line to be drawn
      FTHideLines       fHideLines;     // hides lines by key
      FTDrawFocusRect   fDrawFocusRect; // immediately shows/hides XOR focus rect
      FTSetZoneOn       fSetZoneOn;     // setup zone expected mouse input:
      FTSetZoneOff      fSetZoneOff;    // turns zone off
      FTUpdateViews     fUpdateViews;   // redraws views
      FTMessBoxOk       fMessBoxOk;     // invokes OK MessageBox
      FTMessBoxYesNo    fMessBoxYesNo;  // invokes Yes/No MessageBox
      FTGetUserString   fGetUserString; // runs dialog to accept zt string
      FTGetUserLong     fGetUserLong;   // runs dialog to accept single int32_t
      FTGetUserRect     fGetUserRect;   // starts mouse rect input
      FTGetUserPoint    fGetUserPoint;  // starts mouse point input
      FTWaitUserInput   fWaitUserInput; // starts input loop
      FTNotifyAppl      fNotifyAppl;    // notifies appl. about node activation
      FTDrawRaster      fDrawRaster;    // draws specified raster
      FTRasterText      fRasterText;    // char string to raster window
      FTRasterHeader    fRasterHeader;  // header string with marked position
      FTZoomToRect      fZoomToRect;    // zooms tiff window to given rectangle
                                        // in real coords

// FT..........      f..........;    // COPY ME!..............
// TODO: (2/4) insert new functions pointers here ^^^ (before this comment)
// and decrease fStubs[] length
// Next step (3/4) you should made in SNPTOOLS.H
      FTStub            fStubs[14];
   }  __SnpToolBox;   // internal usage!

#endif
