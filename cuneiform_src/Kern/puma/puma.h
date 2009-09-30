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

#include <string>

#include "globus.h"
#include "cttypes.h"
#include "singleton.h"
#include "pumaimpl.h"

#ifdef __PUMA__
#define PUMA_FUNC  FUN_EXPO__
#else
#define PUMA_FUNC  FUN_IMPO__
#endif

namespace CIF {
class PumaImpl;
typedef Singleton<PumaImpl, CreateUsingStatic> Puma;
}

struct PUMA_ImageInfo {
	uint16_t wImageHeight;
	uint16_t wImageWidth;
	uint16_t wImageByteWidth;
	uint16_t wImageDisplacement;
	uint16_t wResolutionX;
	uint16_t wResolutionY;
	uchar bFotoMetrics;
	uchar bUnused;
	uint16_t wAddX;
	uint16_t wAddY;
};

typedef Bool16 (*PUMA_Callback_ImageOpen)(PUMA_ImageInfo *);
typedef uint16_t (*PUMA_Callback_ImageRead)(pchar, uint16_t);
typedef Bool16 (*PUMA_Callback_ImageClose)(void);

struct PUMAIMAGECALLBACK {
	PUMA_Callback_ImageOpen CIMAGE_ImageOpen;
	PUMA_Callback_ImageRead CIMAGE_ImageRead;
	PUMA_Callback_ImageClose CIMAGE_ImageClose;
};

enum PUMA_EXPORT_ENTRIES {
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
};

bool LPUMA_Load(char * lpPath);
void LPUMA_Unload(void);

uint32_t LPUMA_GetLanguage(void);
void LPUMA_SetLanguage(uint32_t lang);
bool LPUMA_GetSpeller(void);
void LPUMA_SetSpeller(bool nNewValue);
bool LPUMA_GetOneColumn(void);
void LPUMA_SetOneColumn(bool nNewValue);
bool LPUMA_GetFax100(void);
void LPUMA_SetFax100(bool nNewValue);
bool LPUMA_GetDotMatrix(void);
void LPUMA_SetDotMatrix(bool nNewValue);
char * LPUMA_GetUserDictName(void);
void LPUMA_SetUserDictName(char * lpszNewValue);
bool LPUMA_GetBold(void);
void LPUMA_SetBold(bool nNewValue);
bool LPUMA_GetItalic(void);
void LPUMA_SetItalic(bool nNewValue);
bool LPUMA_GetSize(void);
void LPUMA_SetSize(bool nNewValue);
bool LPUMA_GetFormat(void); // old
void LPUMA_SetFormat(bool nNewValue);// old
char * LPUMA_GetSerifName(void);
void LPUMA_SetSerifName(char * lpszNewValue);
char * LPUMA_GetSansSerifName(void);
void LPUMA_SetSansSerifName(char * lpszNewValue);
char * LPUMA_GetCourierName(void);
void LPUMA_SetCourierName(char * lpszNewValue);

void LPUMA_SetPictures(uint32_t nNewValue);
uint32_t LPUMA_GetPictures(void);
void LPUMA_SetTables(uint32_t nNewValue);
uint32_t LPUMA_GetTables(void);

char * LPUMA_GetVersion(void);

uint32_t LPUMA_GetFormatMode(void); // old
void LPUMA_SetFormatMode(uint32_t nNewValue);// old

void LPUMA_SetUnrecogChar(uchar nChar);
uchar LPUMA_GetUnrecogChar(void);

void LPUMA_SetAutoRotate(bool b);
bool LPUMA_GetAutoRotate(void);

void LPUMA_SetPageSize(uint32_t width, uint32_t height);

void LPUMA_SetCurrentEdPage(Handle hEdPage);
Handle LPUMA_GetCurrentEdPage(void);

void LPUMA_SetPreserveLineBreaks(bool b);
bool LPUMA_GetPreserveLineBreaks(void);

uint32_t LCED_DeletePage(Handle hEdPage);
#define DEC_FUN(a,b,c) typedef a (*FNPUMA_##b)c;

// Module functions
DEC_FUN(int, GetReturnCode,())
DEC_FUN(char *, GetReturnString,(uint32_t))
DEC_FUN(bool, SetImportData,(uint32_t, void *))

PUMA_FUNC int PUMA_GetReturnCode();
PUMA_FUNC char * PUMA_GetReturnString(int Error);
PUMA_FUNC bool PUMA_SetImportData(uint32_t, void *);

struct PUMAENTRY {
	FNPUMA_GetReturnCode fnGetReturnCode;
	FNPUMA_GetReturnString fnGetReturnString;
	FNPUMA_SetImportData fnSetImportData;
};

typedef PUMAENTRY * LPPUMAENTRY;
// Enum
DEC_FUN(int32_t, EnumLanguages,(int32_t))
DEC_FUN(int32_t, EnumFormats,(int32_t))
DEC_FUN(int32_t, EnumCodes,(int32_t, int32_t))
DEC_FUN(int32_t, EnumFormatMode,(int32_t))
DEC_FUN(int32_t, EnumTable,(int32_t))
DEC_FUN(int32_t, EnumPicture,(int32_t))

PUMA_FUNC int32_t PUMA_EnumLanguages(int32_t nPrev);
PUMA_FUNC int32_t PUMA_EnumFormats(int32_t nPrev);
PUMA_FUNC int32_t PUMA_EnumCodes(int32_t format, int32_t nPrev);
PUMA_FUNC int32_t PUMA_EnumFormatMode(int32_t nPrev);
PUMA_FUNC int32_t PUMA_EnumTable(int32_t nPrev);
PUMA_FUNC int32_t PUMA_EnumPicture(int32_t nPrev);

// misc
DEC_FUN(bool, XGetRotateDIB,(void **, Point32*))
DEC_FUN(void, RenameImageName,(const std::string&))
DEC_FUN(bool, XSetTemplate,(Rect32))
DEC_FUN(bool, XGetTemplate,(Rect32*))
DEC_FUN(void, GetSpecialBuffer,(char*,int32_t*))
DEC_FUN(bool, SetSpecialProject,(uchar))

PUMA_FUNC bool PUMA_XGetRotateDIB(void ** lpDIB, Point32 * p);
PUMA_FUNC void PUMA_RenameImageName(const std::string& name);
PUMA_FUNC bool PUMA_XSetTemplate(Rect32 rect);
PUMA_FUNC bool PUMA_XGetTemplate(Rect32 *pRect);
PUMA_FUNC void PUMA_GetSpecialBuffer(char * szResult, int32_t *nResultLength);
PUMA_FUNC bool PUMA_SetSpecialProject(uchar nSpecPrj);

#undef DEC_FUN

#define PUMA_FORMAT_NONE			  0x0040
#define PUMA_FORMAT_ALL				  0x0001
#define PUMA_FORMAT_ONLY_FRAME		0x0002

#define PUMA_TABLE_NONE			0
#define PUMA_TABLE_DEFAULT			1
#define PUMA_TABLE_ONLY_LINE		2
#define PUMA_TABLE_ONLY_TEXT		3
#define PUMA_TABLE_LINE_TEXT		4

#define PUMA_PICTURE_NONE			0
#define PUMA_PICTURE_ALL			1

#endif
