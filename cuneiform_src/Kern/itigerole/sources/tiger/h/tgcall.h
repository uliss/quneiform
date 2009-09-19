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

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                                                                          //
//                    T I G E R   O C R   L I B R A R Y                     //
//                                                                          //
//                                                                          //
//            File       :  TGCALL.H                                        //
//            Contains   :  External TIGER OCR library definitions          //
//            Written by :  Alexander Talalai, Yuri Panchul                 //
//            Compiler   :  Microsoft Visual C v.4.2                        //
//            Copyright (C) 1990 CTL.                                       //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
# ifndef TGCALL_H_INCLUDED
# define TGCALL_H_INCLUDED
//////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif
//////////////////////////////////////////////////////////////////////////////
#include "dlltypes.h"
//////////////////////////////////////////////////////////////////////////////
#pragma pack(push,1)
//////////////////////////////////////////////////////////////////////////////
# define MAX_FILENAME_LENGTH              160
//////////////////////////////////////////////////////////////////////////////
//                    Calls to Recognition Engine                           //
//////////////////////////////////////////////////////////////////////////////
 #if defined ( FILE_TIGERAPI )
        #define TIGERLOC(typ) TIGERAPI(typ)
 #else
        #define TIGERLOC(typ) TIGERFUN(typ)
 #endif
//////////////////////////////////////////////////////////////////////////////
//                   proc call function macro                               //
//////////////////////////////////////////////////////////////////////////////
 #define     TIGER_LOAD_FN                "__Tiger_Load"
 #define     TIGER_BCR_LOAD_FN            "__BCR_Load"
 #define     TIGER_CARDS_INIT_FN          "Cards_Init"
 #define     TIGER_RESET_FN               "Tiger_Reset"
 #define     TIGER_SET_REC_OPT_FN         "Tiger_SetRecognitionOptions"
 #define     TIGER_RECOG_FN               "Tiger_Recognition"
 #define     TIGER_PAGE_AN_FN             "Tiger_PageAnalysis"
 #define     TIGER_FIN_RECOG_FN           "Tiger_FinalRecognition"
 #define     TIGER_STATUS_FN              "Tiger_Status"
 #define     TIGER_ERR_MESSAGE_FN         "Tiger_ErrorMessage"
 #define     TIGER_ED_2_TEXT_FN           "Tiger_EdToText"
 #define     TIGER_EXT_ED_2_TEXT_FN       "Tiger_ExtEdToText"
 #define     TIGER_PROGRESS_LIST_FN       "Tiger_ProgressList"
 #define     TIGER_LAY_GET_P_INCL_L_FN    "TigerLayout_GetPageIncline"
 #define     TIGER_LAY_UPD_BL_FN          "TigerLayout_UpdateBlocks"
 #define     TIGER_LAY_H_SP_BL_FN         "TigerLayout_HorzSplitBlocks"
 #define     TIGER_LAY_V_SP_BL_FN         "TigerLayout_VertSplitBlocks"
 #define     TIGER_LAY_EXT_RECT_BL_FN     "TigerLayout_ExtractRectBlock"
 #define     TIGER_LAY_EXT_PIC_FN         "TigerLayout_ExtractPicture"
 #define     TIGER_LAY_GLUE_BL_FN         "TigerLayout_GlueBlocks"
 #define     TIGER_LAY_DEL_BL_FN          "TigerLayout_DeleteBlock"
 #define     TIGER_LAY_ENGL_BL_FN         "TigerLayout_EnglishBlock"
 #define     TIGER_LAY_BACKUP_FN          "TigerLayout_Backup"
 #define     TIGER_LAY_UNDO_FN            "TigerLayout_Undo"
 #define     TIGER_LAY_REDO_FN            "TigerLayout_Redo"
 #define     TIGER_SET_LANG_FN            "Tiger_SetLanguage"
 #define     TIGER_OPEN_ED_FILE_FN        "Tiger_OpenEdFile"
 #define     TIGER_GET_ED_PAGE_FN         "Tiger_GetEdPage"
 #define     TIGER_GET_ED_FRAG_FN         "Tiger_GetEdFragment"
 #define     TIGER_GET_ED_STR_FN          "Tiger_GetEdString"
 #define     TIGER_GET_ED_WORD_FN         "Tiger_GetEdWord"
 #define     TIGER_GET_ED_CHAR_FN         "Tiger_GetEdCharacter"
 #define     TIGER_CLOSE_ED_FILE_FN       "Tiger_CloseEdFile"
 #define     TIGER_SET_ED_SAV_RTF_OPT_FN  "Tiger_SetEdSaveRtfOptions"
 #define     TIGER_LAY_UPD_SHEETS_FN      "TigerLayout_UpdateSheets"
 #define     TIGER_RW_OPTIONS_FN          "Tiger_RW_Options"
 #define     TIGER_GET_LANG_FN            "Tiger_GetLanguage"
 #define     TIGER_ENUM_LANG_FN           "Tiger_EnumLanguage"
 #define     TIGER_SPELL_EXIST_FN         "Tiger_SpellerExist"
 #define     TIGER_RUN_WITH_DIB_FN        "TigerRunWithDIB"
//////////////////////////////////////////////////////////////////////////////
//  Load and reset TIGER.DLL and TGCALL.DLL                                 //
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          Tiger_Load            (HANDLE hApplicationInstance,
												 char* lpTigerPathName);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          BCR_Load              (HANDLE hApplicationInstance,
												 char* lpTigerPathName);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          Tiger_Unload          (void);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          Tiger_Reset           (void);
//////////////////////////////////////////////////////////////////////////////
//  Recognition and page analysis                                           //
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          Tiger_SetRecognitionOptions (uint16_t wOptions);
//////////////////////////////////////////////////////////////////////////////
# define TSRO_SPELLER            1
# define TSRO_FORCE_ONE_COLUMN   2
# define TSRO_NOFORMAT           4
# define TSRO_FAX100             8
# define TSRO_DOTMATRIX          16
# define TSRO_BCR                32
# define TSRO_RECOGLANG          64
# define TSRO_CUT_LEFT_EDGE      128
# define TSRO_CUT_RIGHT_EDGE     256
# define TSRO_PICTURES_NO        512
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          Tiger_Recognition      (char* lpEdFileName);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          Tiger_PageAnalysis     (void);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          Tiger_FinalRecognition (char* lpEdFileName);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(uint16_t)            Tiger_Status           (void);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(char*)           Tiger_ErrorMessage     (uint16_t wError);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          Tiger_SetLanguage      (uint16_t wLanguage);
//////////////////////////////////////////////////////////////////////////////
//   Convertion of ED-file to text file                                     //
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          Tiger_EdToText        (char* lpEdFileName,
												 char* lpTextFileName,
												 uint16_t wOptions,
												 uint16_t wBadChar);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          Tiger_ExtEdToText     (char* lpEdFileName,
												 char* lpTextFileName,
												 uint16_t wOptions,
												 uint16_t wCodePage,
												 uint16_t wBadChar);
//////////////////////////////////////////////////////////////////////////////
//      wOptions values                                                     //
//////////////////////////////////////////////////////////////////////////////
# define TIGER_TOEDNATIVE           0x01
# define TIGER_TOASCII              0x02
# define TIGER_TOSMARTASCII         0x04
# define TIGER_TORTF                0x08
# define TIGER_TOANSI               0x10
# define TIGER_TOSMARTANSI          0x20
# define TIGER_APPEND               0x80
# define TIGER_TOTABLETXT           0x0100
# define TIGER_TOTABLECSV           0x0200
# define TIGER_TOTABLEDBF           0x0400
# define TIGER_TOTABLEODBC          0x0800
# define TIGER_TOTABLEWKS           0x1000
# define TIGER_TOHTML               0x2000
//////////////////////////////////////////////////////////////////////////////
//       wCodePage options                                                  //
//////////////////////////////////////////////////////////////////////////////
# define TIGER_CODE_UNKNOWN         0x0000
# define TIGER_CODE_ASCII           0x0001
# define TIGER_CODE_ANSI            0x0002
# define TIGER_CODE_KOI8            0x0004
# define TIGER_CODE_ISO             0x0008
//////////////////////////////////////////////////////////////////////////////
//       Progress List                                                      //
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(uint16_t) Tiger_ProgressList (LPWORD lpwList, uint16_t wMode);
//////////////////////////////////////////////////////////////////////////////
//       Manual layout server layout functions                              //
//////////////////////////////////////////////////////////////////////////////
struct  Tiger_LayoutBlock
{
	uint16_t  Number;
	INT16 xLeft;
	INT16 yTop;
	INT16 xRight;
	INT16 yBottom;
	INT16 Type;
};
//////////////////////////////////////////////////////////////////////////////
struct Tiger_LayoutStripHandle
{
	INT16   top;
	INT16   left;
	INT16   n_strip;
	INT16   handle;
};
//////////////////////////////////////////////////////////////////////////////
struct Tiger_LayoutStrip
{
	INT16   len;
	INT16   col;
};
//////////////////////////////////////////////////////////////////////////////
#ifndef BLOCK_TEXT
#define BLOCK_TEXT                  1
#define BLOCK_PICTURE               5
#endif
//////////////////////////////////////////////////////////////////////////////
typedef struct Tiger_LayoutBlock         TIGER_LAYOUTBLOCK;
typedef struct Tiger_LayoutStripHandle   TIGER_LAYOUTSTRIPHANDLE;
typedef struct Tiger_LayoutStrip         TIGER_LAYOUTSTRIP;
typedef TIGER_LAYOUTBLOCK TIGERFAR *     LPTIGER_LAYOUTBLOCK;
typedef LPTIGER_LAYOUTBLOCK TIGERFAR *   LPLPTIGER_LAYOUTBLOCK;
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)   TigerLayout_GetPageIncline (LPINT16 lpnNumerator,
											   LPINT16 lpnDenominator);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)   TigerLayout_UpdateBlocks   (LPINT16 lpnBlocks,
											   LPLPTIGER_LAYOUTBLOCK lplpBlocks);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)   TigerLayout_HorzSplitBlocks(POINT Begin,
											   POINT End,
											   LPBOOL16 lpbSuccess);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)   TigerLayout_VertSplitBlocks(POINT Begin,
											   POINT End,
											   LPBOOL16 lpbSuccess);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)   TigerLayout_ExtractRectBlock(RECT r,
											   LPBOOL16 lpbSuccess);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)   TigerLayout_GlueBlocks     (uint16_t iBlock1,
											   uint16_t iBlock2,
											   LPBOOL16 lpbSuccess);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)   TigerLayout_DeleteBlock    (uint16_t iBlock,
											   LPBOOL16 lpbSuccess);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)   TigerLayout_EnglishBlock   (uint16_t iBlock,
											   LPBOOL16 lpbSuccess);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)   TigerLayout_ExtractPicture (RECT r,
											   LPBOOL16 lpbSuccess);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)   Layout_UpdatePictures      (char* lpPictures,
											   uint16_t wLength);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)   TigerLayout_UpdateSheets   (uint16_t FuncNumber,
											   LPINT16 * lplpSheets);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)   Tiger_RW_Options           (uint16_t FuncNumber,
											   char* lpData);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)   TigerLayout_Backup         (LPBOOL16 lpbBackupStackOverflow);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)   TigerLayout_Undo           (LPBOOL16 lpbUndoBufferEmpty);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)   TigerLayout_Redo           (LPBOOL16 lpbCantRedo);
//////////////////////////////////////////////////////////////////////////////
# define IdealXY(x,y,Num,Den) { x -= (((int32_t)y)*Num)/Den; y += (((int32_t)x)*Num)/Den; }
# define RealXY(x,y,Num,Den)  { y -= (((int32_t)y)*Num)/Den; x += (((int32_t)y)*Num)/Den; }
//////////////////////////////////////////////////////////////////////////////
//   ED-file access functions                                               //
//////////////////////////////////////////////////////////////////////////////
struct ED_Rect
{
	INT16 xLeft;
	INT16 yTop;
	INT16 xRight;
	INT16 yBottom;
};
//////////////////////////////////////////////////////////////////////////////
typedef struct ED_Rect       ED_RECT;
typedef ED_RECT   TIGERFAR * LPED_RECT;
typedef LPED_RECT TIGERFAR * LPLPED_RECT;
//////////////////////////////////////////////////////////////////////////////
struct ED_Page
{
    uint16_t    wFragmentCount;      // Number of fragments in the page
    ED_RECT rFrame;              // Rectangle around the page
    INT16   nInclineNumerator;   // Numerator of the page incline
    uint16_t    wInclineDenominator; // Denominator of the page incline
    uint16_t    wDpi;                // Resolution of the source image in dpi
};
//////////////////////////////////////////////////////////////////////////////
typedef struct ED_Page ED_PAGE;
typedef ED_PAGE   TIGERFAR * LPED_PAGE;
typedef LPED_PAGE TIGERFAR * LPLPED_PAGE;
//////////////////////////////////////////////////////////////////////////////
struct ED_Fragment
{
    uint16_t    wFragmentNumber;       // Fragment number
    ED_RECT rFrame;                // Rectangle around the fragment
    uint16_t    wStringCount;          // Number of strings in the fragment
    uint16_t    wType;                 // Fragment type
    uint16_t    wBaseDistanceFromLeft; // Base distance from left side
    uint16_t    wCenterFragmentNumber; // Number of the fragment accordingly to
                                   // which current is centered
};
//////////////////////////////////////////////////////////////////////////////
typedef struct ED_Fragment       ED_FRAGMENT;
typedef ED_FRAGMENT   TIGERFAR * LPED_FRAGMENT;
typedef LPED_FRAGMENT TIGERFAR * LPLPED_FRAGMENT;
//////////////////////////////////////////////////////////////////////////////
//      wType bits                                                          //
//////////////////////////////////////////////////////////////////////////////
# define TG_EDF_LEFT_ALIGNMENT   1
# define TG_EDF_RIGHT_ALIGNMENT  2
# define TG_EDF_CENTERED         4
# define TG_EDF_JUSTIFIED        (TG_EDF_LEFT_ALIGNMENT|TG_EDF_RIGHT_ALIGNMENT)
//////////////////////////////////////////////////////////////////////////////
struct ED_String
{
    uint16_t    wStringNumber; // String number
    ED_RECT rFrame;        // Frame of the string
    uint16_t    wWordCount;    // Number of words within string
    uint16_t    wType;         // Type of the string
    INT16   nIndent;       // Indent of the string
                           //  (relative to the fragment's base)
    uint16_t    wBulletIndent; // Additional indent for bullet
    INT16   nBaseLine;     // Y-coordinate for base line of the string
};
//////////////////////////////////////////////////////////////////////////////
typedef struct ED_String  ED_STRING;
typedef ED_STRING   TIGERFAR * LPED_STRING;
typedef LPED_STRING TIGERFAR * LPLPED_STRING;
//////////////////////////////////////////////////////////////////////////////
//      wType bits                                                          //
//////////////////////////////////////////////////////////////////////////////
# define TG_EDS_PARAGRAPH  1
# define TG_EDS_BULLET     2
# define TG_EDS_DROP_CAPS  4
//////////////////////////////////////////////////////////////////////////////
struct ED_Word
{
    uint16_t    wWordNumber;      // Word number
    ED_RECT rFrame;           // Frame of the word
    uint16_t    wCharacterCount;  // Number of characters in the word
    uint16_t    wFont;            // Font characteristics
    uint16_t    wPointSize;       // Point size
};
//////////////////////////////////////////////////////////////////////////////
typedef struct ED_Word  ED_WORD;
typedef ED_WORD   TIGERFAR * LPED_WORD;
typedef LPED_WORD TIGERFAR * LPLPED_WORD;
//////////////////////////////////////////////////////////////////////////////
//      wFont bits                                                          //
//////////////////////////////////////////////////////////////////////////////
# define TG_EDW_SERIF     0x1
# define TG_EDW_ITALIC    0x2
# define TG_EDW_BOLD      0x4
# define TG_EDW_UNDERLINE 0x8
# define TG_EDW_FIX_PITCH 0x10
# define TG_EDW_DROP_CAPS 0x20
//////////////////////////////////////////////////////////////////////////////
struct ED_Character
{
    uint16_t    wCharNumber;  // Character number
    ED_RECT rFrame;       // Character frame
    uchar    bCharacter;   // Character itself
    uchar    bProbability; // Character recognition quality
};
//////////////////////////////////////////////////////////////////////////////
typedef struct ED_Character       ED_CHARACTER;
typedef ED_CHARACTER   TIGERFAR * LPED_CHARACTER;
typedef LPED_CHARACTER TIGERFAR * LPLPED_CHARACTER;
//////////////////////////////////////////////////////////////////////////////
# define TG_EDC_BAD_SYMBOL    176  // Unrecognized character
# define TG_EDC_BULLET_SYMBOL 254  // Bullet
# define TG_EDC_BIG_SUSPECT   70   // Pay attention if probability less
                                   // than this value
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          Tiger_OpenEdFile      (char* lpFilename);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          Tiger_GetEdPage       (LPED_PAGE lpEdPage);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          Tiger_GetEdFragment   (uint16_t wFragmentID,
												 LPED_FRAGMENT lpEdFragment);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          Tiger_GetEdString     (uint16_t wStringID,
												 LPED_STRING lpEdString);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          Tiger_GetEdWord       (uint16_t wWordID,
												 LPED_WORD lpEdWord);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          Tiger_GetEdCharacter  (uint16_t wCharacterID,
												 LPED_CHARACTER lpEdCharacter);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          Tiger_CloseEdFile     (void);
//////////////////////////////////////////////////////////////////////////////
#define SIZEofBUF         (0 + 256*sizeof(int))
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)       Tiger_SetEdSaveRtfOptions(uint16_t wFrame,
												 char* lpNameSerif,
												 char* lpNameNoSerif,
												 char* lpNameMono,
												 char* lpSerif,
												 char* lpNoSerif,
												 char* lpMono);
//////////////////////////////////////////////////////////////////////////////
#define FUNC_SHEET_UPDATE            1
#define FUNC_SHEET_DELETE            2
#define FUNC_SET_TEMP_DIR            3
#define FUNC_SET_TABLE               4
#define FUNC_SET_USER_DICT           5
#define FUNC_SET_WINDOWS_DIR         6
#define FUNC_HANDSHAKING_VERS        7
#define FUNC_HANDSHAKING_PASS        8
#define FUNC_SET_MMX                 9
#define FUNC_SET_EDEXT	            10
#define FUNC_SET_MASHINE_NAME       11
#define FUNC_PROTECT_INFO           12
#define FUNC_SET_RTF_COORDINATES    13
#define FUNC_SET_IMAGENAME          14
#define FUNC_SET_PAGENUMBER         15
#define FUNC_DELETE_TEMP_FILES      16
// duplicate in file ..\USER\TGCALL.H
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(uint16_t)            Tiger_GetLanguage     (void);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(uint16_t)            Tiger_EnumLanguage    (uint16_t prev);
//////////////////////////////////////////////////////////////////////////////
TIGERLOC(BOOL16)          Tiger_RunWithDIB      (void *, void *);
//////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
//////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////
#undef TIGERLOC
#endif
