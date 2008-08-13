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

#ifndef __PUMA_H
#define __PUMA_H

#ifndef __GLOBUS_H
#include "globus.h"
#endif
#include "cttypext.h"
/*
#ifndef __RECDEFS_H
   #include "recdefs.h"
#endif
   #include "memfunc.h"
*/
#ifdef __PUMA__
  #define PUMA_FUNC  FUN_EXPO
#else
  #define PUMA_FUNC  FUN_IMPO
#endif

#ifdef __cplusplus
	extern "C" {
#endif

#define PUMA_MAXNAME 260

typedef void (*FNPUMA_ProgressStart)( void );
typedef void (*FNPUMA_ProgressFinish)( void );
typedef Bool32 (*FNPUMA_ProgressStep)( Word32 step,const char* name, Word32 percent );
typedef struct
{
	Word16    wImageHeight;
	Word16    wImageWidth;
	Word16    wImageByteWidth;
	Word16    wImageDisplacement;
	Word16    wResolutionX;
	Word16    wResolutionY;
	Word8     bFotoMetrics;
	Word8     bUnused;
	Word16    wAddX;
	Word16    wAddY;
}
PUMA_ImageInfo;

typedef Bool16 (*PUMA_Callback_ImageOpen)(PUMA_ImageInfo *);
typedef Word16   (*PUMA_Callback_ImageRead)(PInt8 , Word16 );
typedef Bool16 (*PUMA_Callback_ImageClose)(void);

typedef struct
{
	PUMA_Callback_ImageOpen   CIMAGE_ImageOpen;
	PUMA_Callback_ImageRead   CIMAGE_ImageRead;
	PUMA_Callback_ImageClose  CIMAGE_ImageClose;
}
PUMAIMAGECALLBACK;

typedef Bool32 (*FNInit)(Word16 wHeightCode,Handle hStorage);
typedef Bool32 (*FNDone)();
typedef Word32 (*FNGetReturnCode)();
typedef char * (*FNGetReturnString)(Word32 dwError);
typedef Bool32 (*FNGetExportData)(Word32 dwType, void * pData);
typedef Bool32 (*FNSetImportData)(Word32 dwType, void * pData);

typedef struct
{
	FNInit				fnInit;
	FNDone				fnDone;
	FNGetReturnCode		fnGetReturnCode;
	FNGetReturnString	fnGetReturnString;
	FNGetExportData		fnGetExportData;
	FNSetImportData		fnSetImportData;
} PUMAENTRY, * LPPUMAENTRY;

enum PUMA_EXPORT_ENTRIES
{
		PUMA_FNPUMA_XOpen = 1,
		PUMA_FNPUMA_XClose,
		PUMA_FNPUMA_XPageAnalysis,
		PUMA_FNPUMA_XFinalRecognition,
		PUMA_FNPUMA_XSave,
		PUMA_FNPUMA_EnumLanguages,
		PUMA_FNPUMA_EnumFormats,
		PUMA_FNPUMA_EnumCodes,
		PUMA_Word32_Language,
		PUMA_Bool32_Speller,
		PUMA_Bool32_OneColumn,
		PUMA_Bool32_Fax100,
		PUMA_Bool32_DotMatrix,
		PUMA_pchar_UserDictName,
		PUMA_Bool32_Bold,
		PUMA_Bool32_Italic,
		PUMA_Bool32_Size,
		PUMA_Bool32_Format,
		PUMA_pchar_SerifName,
		PUMA_pchar_SansSerifName,
		PUMA_pchar_CourierName,
		PUMA_Word32_Pictures,
		PUMA_Word32_Tables,
		PUMA_pchar_Version,
		PUMA_Word32_Format,
		PUMA_FNPUMA_EnumFormatMode,
		PUMA_FNPUMA_EnumTable,
		PUMA_FNPUMA_EnumPicture,
		PUMA_Word8_Format,
		PUMA_FNPUMA_XGetRotateDIB,
		PUMA_FNPUMA_ProgressStart,
		PUMA_FNPUMA_ProgressFinish,
		PUMA_FNPUMA_ProgressStep,
		PUMA_Bool32_AutoRotate,
		PUMA_Point32_PageSize,
		PUMA_FNPUMA_RenameImageName,
		PUMA_FNPUMA_XSetTemplate,
		PUMA_Handle_CurrentEdPage,
		PUMA_FNPUMA_Save,
		PUMA_Bool32_PreserveLineBreaks,
		PUMA_FNPUMA_XOpenClbk,
		PUMA_LPPUMAENTRY_CED,
		PUMA_LPPUMAENTRY_ROUT,
		PUMA_FNPUMA_SaveToMemory,
        PUMA_FNPUMA_GetSpecialBuffer,
        PUMA_FNPUMA_SetSpecialProject,
		PUMA_FNPUMA_XGetTemplate
} ;

Bool32	LPUMA_Load( char * lpPath );
void	LPUMA_Unload( void );

Word32	LPUMA_GetLanguage( void );
void	LPUMA_SetLanguage(Word32 lang);
Bool32 	LPUMA_GetSpeller( void );
void 	LPUMA_SetSpeller(Bool32 nNewValue);
Bool32 	LPUMA_GetOneColumn( void );
void 	LPUMA_SetOneColumn(Bool32 nNewValue);
Bool32 	LPUMA_GetFax100( void );
void 	LPUMA_SetFax100(Bool32 nNewValue);
Bool32 	LPUMA_GetDotMatrix( void );
void 	LPUMA_SetDotMatrix(Bool32 nNewValue);
char * 	LPUMA_GetUserDictName( void );
void 	LPUMA_SetUserDictName(char * lpszNewValue);
Bool32 	LPUMA_GetBold( void );
void 	LPUMA_SetBold(Bool32 nNewValue);
Bool32 	LPUMA_GetItalic( void );
void 	LPUMA_SetItalic(Bool32 nNewValue);
Bool32 	LPUMA_GetSize( void );
void 	LPUMA_SetSize(Bool32 nNewValue);
Bool32 	LPUMA_GetFormat( void );      // old
void 	LPUMA_SetFormat(Bool32 nNewValue);// old
char * 	LPUMA_GetSerifName( void );
void 	LPUMA_SetSerifName(char * lpszNewValue);
char * 	LPUMA_GetSansSerifName( void );
void 	LPUMA_SetSansSerifName(char * lpszNewValue);
char * 	LPUMA_GetCourierName( void );
void 	LPUMA_SetCourierName(char * lpszNewValue);

void	LPUMA_SetPictures(Word32 nNewValue);
Word32	LPUMA_GetPictures( void );
void	LPUMA_SetTables(Word32 nNewValue);
Word32	LPUMA_GetTables( void );

char * 	LPUMA_GetVersion( void );

Word32 	LPUMA_GetFormatMode( void );      // old
void 	LPUMA_SetFormatMode(Word32 nNewValue);// old

void	LPUMA_SetUnrecogChar(Word8 nChar);
Word8	LPUMA_GetUnrecogChar( void );

Bool32  LPUMA_SetProgressFunction(FNPUMA_ProgressStart fnStart,FNPUMA_ProgressStep fnStep,FNPUMA_ProgressFinish fnFinish);

void    LPUMA_SetAutoRotate(Bool32 b);
Bool32  LPUMA_GetAutoRotate( void );

void    LPUMA_SetPageSize(Word32 width,Word32 height);

void    LPUMA_SetCurrentEdPage(Handle hEdPage);
Handle  LPUMA_GetCurrentEdPage( void );

void    LPUMA_SetPreserveLineBreaks( Bool32 b );
Bool32  LPUMA_GetPreserveLineBreaks( void );

Word32  LCED_DeletePage(Handle hEdPage);

#define DEC_FUN(a,b,c) typedef a (*FN##b)c; PUMA_FUNC(a) b c; a L##b c

DEC_FUN(Bool32, PUMA_Init,(Word16 wHeightCode,Handle hStorage));
DEC_FUN(Bool32, PUMA_Done,());
DEC_FUN(Word32, PUMA_GetReturnCode,());
DEC_FUN(char *, PUMA_GetReturnString,(Word32 dwError));
DEC_FUN(Bool32, PUMA_GetExportData,(Word32 dwType, void * pData));
DEC_FUN(Bool32, PUMA_SetImportData,(Word32 dwType, void * pData));

DEC_FUN(Bool32, PUMA_XOpen,(void * pDIB,const char * lpFileName));
DEC_FUN(Bool32, PUMA_XClose,( void ));
DEC_FUN(Bool32, PUMA_XPageAnalysis,( void ));
DEC_FUN(Bool32, PUMA_XFinalRecognition,( void ));
DEC_FUN(Bool32, PUMA_XSave,(const char * lpOutFileName, Int32 lnFormat, Int32 lnCode ));
DEC_FUN(Int32,  PUMA_EnumLanguages,(Int32 nPrev ));
DEC_FUN(Int32,  PUMA_EnumFormats,(Int32 nPrev ));
DEC_FUN(Int32,  PUMA_EnumCodes,(Int32 format, Int32 nPrev ));
DEC_FUN(Int32,  PUMA_EnumFormatMode,(Int32 nPrev ));
DEC_FUN(Int32,  PUMA_EnumTable,(Int32 nPrev ));
DEC_FUN(Int32,  PUMA_EnumPicture,(Int32 nPrev ));
DEC_FUN(Bool32,	PUMA_XGetRotateDIB,(void ** lpDIB, Point32 * p));
DEC_FUN(void ,	PUMA_RenameImageName,(char * name));
DEC_FUN(Bool32, PUMA_XSetTemplate,(Rect32 rect));
DEC_FUN(Bool32, PUMA_XGetTemplate,(Rect32 *pRect));
DEC_FUN(Bool32, PUMA_Save,(Handle hEdPage, const char * lpOutFileName, Int32 lnFormat, Int32 lnCode, Bool32 bAppend ));
DEC_FUN(Bool32, PUMA_XOpenClbk,(PUMAIMAGECALLBACK CallBack,const char * lpFileName));
DEC_FUN(Word32, PUMA_SaveToMemory,(Handle hEdPage, Int32 lnFormat, Int32 lnCode, char * lpMem, Word32 size ));
DEC_FUN(void ,	PUMA_GetSpecialBuffer,(char * szResult,Int32 *nResultLength));
DEC_FUN(Bool32,	PUMA_SetSpecialProject,(Word8 nSpecPrj));

#undef DEC_FUN
// Languages codes
	#define PUMA_LANG_ENGLISH     0
	#define PUMA_LANG_GERMAN      1
	#define PUMA_LANG_FRENCH      2
	#define PUMA_LANG_RUSSIAN     3
	#define PUMA_LANG_SWEDISH     4
	#define PUMA_LANG_SPANISH     5
	#define PUMA_LANG_ITALIAN     6
	#define PUMA_LANG_RUSENG      7
	#define PUMA_LANG_UKRAINIAN   8
	#define PUMA_LANG_SERBIAN     9
	#define PUMA_LANG_CROATIAN   10
	#define PUMA_LANG_POLISH     11
	#define PUMA_LANG_DANISH     12
	#define PUMA_LANG_PORTUGUESE 13
	#define PUMA_LANG_DUTCH      14
	#define PUMA_LANG_DIG        15
    #define PUMA_LANG_UZBEK      16
    #define PUMA_LANG_KAZ        17
    #define PUMA_LANG_KAZ_ENG    18
    #define PUMA_LANG_CZECH      19	// 01.09.2000 E.P.
    #define PUMA_LANG_ROMAN      20
    #define PUMA_LANG_HUNGAR     21
    #define PUMA_LANG_BULGAR     22
    #define PUMA_LANG_SLOVENIAN  23	// 25.05.2001 E.P.
    #define PUMA_LANG_LATVIAN	 24	// 09.07.2001 E.P.
    #define PUMA_LANG_LITHUANIAN 25
    #define PUMA_LANG_ESTONIAN	 26
    #define PUMA_LANG_TURKISH	 27

// Format codes
	# define PUMA_TOEDNATIVE      0
	# define PUMA_TOTEXT          0x02
	# define PUMA_TOSMARTTEXT     0x04
	# define PUMA_TORTF           0x08
	# define PUMA_TOTABLETXT      0x0100
	# define PUMA_TOTABLECSV      0x0200
	# define PUMA_TOTABLEDBF      0x0400
	# define PUMA_TOTABLEODBC     0x0800
	# define PUMA_TOTABLEWKS      0x1000
	# define PUMA_TOHTML          0x2000
#ifdef _DEBUG
	# define PUMA_DEBUG_TOTEXT	  0x12
#endif

//       Codes                                                  //
	# define PUMA_CODE_UNKNOWN    0x0000
	# define PUMA_CODE_ASCII      0x0001
	# define PUMA_CODE_ANSI       0x0002
	# define PUMA_CODE_KOI8       0x0004
	# define PUMA_CODE_ISO        0x0008

	# define PUMA_FORMAT_NONE			  0x0040
	# define PUMA_FORMAT_ALL				  0x0001
	# define PUMA_FORMAT_ONLY_FRAME		0x0002

	# define PUMA_TABLE_NONE			0
	# define PUMA_TABLE_DEFAULT			1
	# define PUMA_TABLE_ONLY_LINE		2
	# define PUMA_TABLE_ONLY_TEXT		3
	# define PUMA_TABLE_LINE_TEXT		4

	# define PUMA_PICTURE_NONE			0
	# define PUMA_PICTURE_ALL			1

#ifdef __cplusplus
            }
#endif

#endif
