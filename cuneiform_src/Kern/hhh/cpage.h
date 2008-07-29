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

#ifndef __CPAGE_H
#define __CPAGE_H

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
#ifdef __CPAGE__
  #define CPAGE_FUNC  FUN_EXPO
#else
  #define CPAGE_FUNC  FUN_IMPO
#endif

#ifdef __cplusplus
	extern "C" {
#endif

#pragma pack (push,8)

#define CPAGE_MAXNAME 260

#ifndef _Not_Cpagetyps  // Almi 30.08.00
#include "cpagetyps.h"
#endif

CPAGE_FUNC(Bool32) CPAGE_Init(Word16 wHeightCode,Handle hStorage);
CPAGE_FUNC(Bool32) CPAGE_Done();
CPAGE_FUNC(Word32) CPAGE_GetReturnCode();
CPAGE_FUNC(Int8 *) CPAGE_GetReturnString(Word32 dwError);
CPAGE_FUNC(Bool32) CPAGE_GetExportData(Word32 dwType, void * pData);
CPAGE_FUNC(Bool32) CPAGE_SetImportData(Word32 dwType, void * pData);
/////////////////////////////////////////////////////////////

typedef Word32 (*CPAGE_ExtConvert)(Word32 dwContext,
								   Word32 TypeIn ,void * lpDataIn,Word32 SizeIn,
								   Word32 TypeOut,void * LpDataOut, Word32 SizeOut);
typedef struct {
	Word32				dwContext;		// Контекст конвертора
	CPAGE_ExtConvert	fnConvertor;	// Функция конвертировани
}	CPAGE_CONVERTOR;

enum CPAGE_EXPORT_ENTRIES
{
		CPAGE_FNCPAGE_CreatePage = 1,
		CPAGE_FNCPAGE_DeletePage,
		CPAGE_FNCPAGE_SavePage,
		CPAGE_FNCPAGE_RestorePage,
		CPAGE_FNCPAGE_GetHandlePage,
		CPAGE_FNCPAGE_GetPageType,
		CPAGE_FNCPAGE_SetPageData,
		CPAGE_FNCPAGE_GetPageData,
		CPAGE_FNCPAGE_BackUp,
		CPAGE_FNCPAGE_Undo,
		CPAGE_FNCPAGE_Redo,
		CPAGE_FNCPAGE_GetCountPage,
		CPAGE_FNCPAGE_GetCountBlock,
		CPAGE_FNCPAGE_CreateBlock,
		CPAGE_FNCPAGE_GetHandleBlock,
		CPAGE_FNCPAGE_GetBlockType,
		CPAGE_FNCPAGE_GetBlockUserNum,
		CPAGE_FNCPAGE_SetBlockUserNum,
		CPAGE_FNCPAGE_GetBlockFlags,
		CPAGE_FNCPAGE_SetBlockFlags,
		CPAGE_FNCPAGE_SetBlockData,
		CPAGE_FNCPAGE_GetBlockData,
		CPAGE_FNCPAGE_DeleteBlock,
		CPAGE_FNCPAGE_SetConvertorPages,
		CPAGE_FNCPAGE_SetConvertorBlocks,
		CPAGE_FNCPAGE_GetUserPageType,
		CPAGE_FNCPAGE_GetUserBlockType,
		CPAGE_FNCPAGE_GetBuckUpCount,
		CPAGE_FNCPAGE_GetBuckUpHandle,
		CPAGE_FNCPAGE_GetPageFirst,
		CPAGE_FNCPAGE_GetPageNext,
		CPAGE_FNCPAGE_GetBlockFirst,
		CPAGE_FNCPAGE_GetBlockNext,
		CPAGE_FNCPAGE_DeleteAll,
		CPAGE_FNCPAGE_GetCurrentPage,
		CPAGE_FNCPAGE_SetCurrentPage,
		CPAGE_FNCPAGE_GetNumberPage,
		CPAGE_FNCPAGE_TableCreate,
		CPAGE_FNCPAGE_TableGetFirst,
		CPAGE_FNCPAGE_TableGetNext,
		CPAGE_FNCPAGE_GeTableGetNumberCells,
		CPAGE_FNCPAGE_PhTableGetNumberCells,
		CPAGE_FNCPAGE_TableGetNumberRow	,
		CPAGE_FNCPAGE_TableGetNumberColumn,
		CPAGE_FNCPAGE_GeTableGetSizeCell,
		CPAGE_FNCPAGE_PhTableGetSizeCell,
		CPAGE_FNCPAGE_TableGetNumberBlock	,
		CPAGE_FNCPAGE_GeTableGetPhysical	,
		CPAGE_FNCPAGE_PhTableGetNumberGeometry	,
		CPAGE_FNCPAGE_PhTableGetGeometry		,
		CPAGE_FNCPAGE_PhTableSetNumberBlock	,
		CPAGE_FNCPAGE_UpdateBlocks,
		CPAGE_FNCPAGE_TableIsPhysicCell,
		CPAGE_FNCPAGE_TableSize,
		CPAGE_FNCPAGE_HL_TableExtract,
		CPAGE_FNCPAGE_TableGetSkew,
		CPAGE_FNCPAGE_PictureGetFirst,
		CPAGE_FNCPAGE_PictureGetNext,
		CPAGE_FNCPAGE_PictureGetPlace,
		CPAGE_FNCPAGE_PictureGetMask,
		CPAGE_FNCPAGE_GetBlockInterNum,
		CPAGE_FNCPAGE_SetBlockInterNum,
		CPAGE_FNCPAGE_GetBlockDataPtr,
		CPAGE_FNCPAGE_GetBuckUpCurPos,
		CPAGE_FNCPAGE_GetInternalType,
		CPAGE_FNCPAGE_GetNameInternalType,

		CPAGE_FNCPAGE_ExTableCreate,
		CPAGE_FNCPAGE_ExTableDelete,
		CPAGE_FNCPAGE_ExTableGetFirst,
		CPAGE_FNCPAGE_ExTableGetNext,
		CPAGE_FNCPAGE_ExGeTableGetNumberCells,
		CPAGE_FNCPAGE_ExPhTableGetNumberCells,
		CPAGE_FNCPAGE_ExTableGetNumberRow,
		CPAGE_FNCPAGE_ExTableGetNumberColumn,
		CPAGE_FNCPAGE_ExGeTableGetSizeCell,
		CPAGE_FNCPAGE_ExPhTableGetSizeCell	,
		CPAGE_FNCPAGE_ExTableGetNumberBlock,
		CPAGE_FNCPAGE_ExGeTableGetPhysical	,
		CPAGE_FNCPAGE_ExPhTableGetNumberGeometry,
		CPAGE_FNCPAGE_ExPhTableGetGeometry,
		CPAGE_FNCPAGE_ExPhTableSetNumberBlock,
		CPAGE_FNCPAGE_ExTableIsPhysicCell,
		CPAGE_FNCPAGE_ExTableSize,
		CPAGE_FNCPAGE_ExTableGetSkew,
		CPAGE_FNCPAGE_GetTableFlag,
		CPAGE_FNCPAGE_GetTableCellFlag,
		CPAGE_FNCPAGE_SetTableFlag,
		CPAGE_FNCPAGE_SetTableCellFlag,
        CPAGE_FNCPAGE_ClearBackUp,  //Paul 19-01-2001
	    CPAGE_FNCPAGE_PhTableGetRect,
	    CPAGE_FNCPAGE_PhTableGetBoundType
} ;


#define DEC_FUN(a,b,c) typedef a (*FN##b)c; CPAGE_FUNC(a) b c

DEC_FUN(Handle,  CPAGE_CreatePage,(Word32 Type, void * lpData, Word32 Size));
DEC_FUN(void,    CPAGE_DeletePage,(Handle hPage));
DEC_FUN(Word32,  CPAGE_GetCountPage,());
DEC_FUN(Handle,  CPAGE_GetHandlePage,(Word32 number));
DEC_FUN(Word32,  CPAGE_GetCurrentPage,( ));
DEC_FUN(Bool32,  CPAGE_SetCurrentPage,(Word32 page));
DEC_FUN(Word32,  CPAGE_GetNumberPage,( Handle hPage ));

DEC_FUN(Bool32,  CPAGE_SavePage,(Handle page,Int8 * lpName));
DEC_FUN(Handle,  CPAGE_RestorePage,(Bool32 remove,Int8 * lpName));


DEC_FUN(Word32,  CPAGE_GetPageType,(Handle hPage));
DEC_FUN(Bool32,  CPAGE_SetPageData,(Handle page ,Word32 Type, void * lpData, Word32 Size));
DEC_FUN(Word32,  CPAGE_GetPageData,(Handle page ,Word32 Type, void * lpData, Word32 Size));

DEC_FUN(void,    CPAGE_ClearBackUp,(Handle page));   //Paul 19-01-2001
DEC_FUN(Handle,  CPAGE_BackUp,(Handle page));
DEC_FUN(Bool32,  CPAGE_Undo,(Handle page,Handle backup));
DEC_FUN(Bool32,  CPAGE_Redo,(Handle page,Handle backup));

DEC_FUN(Handle,   CPAGE_CreateBlock,(Handle page, Word32 Type, Word32 UserNum , Word32 Flags,void * lpData, Word32 Size));
DEC_FUN(void  ,  CPAGE_DeleteBlock,(Handle page,Handle block));

DEC_FUN(Word32,  CPAGE_GetCountBlock,(Handle page));
DEC_FUN(Handle,  CPAGE_GetHandleBlock,(Handle page, Word32 number));

DEC_FUN(Word32,  CPAGE_GetBlockType,(Handle page,Handle block));
DEC_FUN(Word32,  CPAGE_GetBlockUserNum,(Handle page,Handle block));
DEC_FUN(void  ,  CPAGE_SetBlockUserNum,(Handle page,Handle block,Word32 user));

DEC_FUN(Word32,  CPAGE_GetBlockFlags,(Handle page,Handle block));
DEC_FUN(void  ,  CPAGE_SetBlockFlags,(Handle page,Handle block,Word32 flags));
//Paul 09-08-99
#define CPAGE_BLOCK_USER       0x1  //выделен пользователем
#define CPAGE_BLOCK_UNCERTAIN  0x2  //грязь или нет ?
//
DEC_FUN(Bool32,  CPAGE_SetBlockData,(Handle page,Handle block,Word32 Type, void * lpData, Word32 Size));
DEC_FUN(Word32,  CPAGE_GetBlockData,(Handle page,Handle block,Word32 Type, void * lpData, Word32 Size));

DEC_FUN(CPAGE_CONVERTOR,  CPAGE_SetConvertorPages,(CPAGE_CONVERTOR func));
DEC_FUN(CPAGE_CONVERTOR,  CPAGE_SetConvertorBlocks,(Handle page,CPAGE_CONVERTOR func));

DEC_FUN(Word32,  CPAGE_GetUserPageType,());
DEC_FUN(Word32,  CPAGE_GetUserBlockType,());
DEC_FUN(Word32, CPAGE_GetBuckUpCount,(Handle page));
DEC_FUN(Handle, CPAGE_GetBuckUpHandle,(Handle page,Word32 number));
DEC_FUN(Word32, CPAGE_GetBuckUpCurPos,(Handle page));

DEC_FUN(Handle,  CPAGE_GetPageFirst,(Word32 type));
DEC_FUN(Handle,  CPAGE_GetPageNext,(Handle page,Word32 type));
DEC_FUN(Handle,  CPAGE_GetBlockFirst,(Handle page, Word32 type));
DEC_FUN(Handle,  CPAGE_GetBlockNext,(Handle page,Handle block, Word32 type));
DEC_FUN(Bool32,  CPAGE_DeleteAll,());

DEC_FUN(Bool32,  CPAGE_UpdateBlocks,( Handle hPage, Word32 type ));

DEC_FUN(Handle,  CPAGE_PictureGetFirst,		(Handle hPage));
DEC_FUN(Handle,  CPAGE_PictureGetNext,		(Handle hPage,Handle hPicture));
DEC_FUN(Bool32,  CPAGE_PictureGetPlace,		(Handle hPage,Handle hPicture,Int32 Skew2048, Point32 * lpLr,Point32 * lpWh));
DEC_FUN(Bool32,  CPAGE_PictureGetMask,			(Handle hPage,Handle hPicture,Int32 Skew2048,Char8 * lpData,Word32 * lpSize));

DEC_FUN(Word32,  CPAGE_GetBlockInterNum,	(Handle page,Handle block));
DEC_FUN(void,    CPAGE_SetBlockInterNum,	(Handle page,Handle block,Word32 inter));
DEC_FUN(Bool32,  CPAGE_GetBlockDataPtr,		(Handle page,Handle block,Word32 Type, void ** lpData));

DEC_FUN(Word32,  CPAGE_GetInternalType,		(char * name));
DEC_FUN(char *,  CPAGE_GetNameInternalType,	(Word32 type));

/*
DEC_FUN();
DEC_FUN();
DEC_FUN();
*/

// Виртуальные функции:
// Они доступны только через непосредственный вызов функций
// CPAGE_GetExportData и CPAGE_SetImportData
// HL означает ручной фрагментатор
#define VRT_FUN(a,b,c) typedef a (*FN##b)c;
//
// Битовые флаги.
//
#define CPAGE_TABLE_DEFAULT	0x0
#define CPAGE_TABLE_LINE	0x1
#define CPAGE_TABLE_TEXT	0x2

VRT_FUN(Bool32,  CPAGE_HL_TableExtract,(  Handle hPAGE, Word32 type, Rect32 rect ));

#undef DEC_FUN


#pragma pack (pop)

#ifdef __cplusplus
            }
#endif

#endif
