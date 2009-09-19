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

/**********  Заголовок  *******************************************************/
/*  Автор      :  Александр Михайлов                                          */
/*  комментарии                                                               */
/*  и далнейшая                                                               */
/*  правка     :  Алексей Коноплев                                            */
/*  Редакция   :  08.06.00                                                    */
/*  Файл       :  'RSTUFF.H'                                                */
/*  Содержание :  Интерфейс библиотеки                                        */
/*  Назначение :                                                              */
/*----------------------------------------------------------------------------*/
//Almi 16.06.00 //Last edit ........
#ifndef __RSTUFF_H
#define __RSTUFF_H

#ifndef __GLOBUS_H
 #include "globus.h"
#endif

#ifndef __PUMA_H
 #include "puma.h"
#endif

#ifdef __RSTUFF__
  #define RSTUFF_FUNC  FUN_EXPO
#else
  #define RSTUFF_FUNC  FUN_IMPO
#endif

#ifdef __cplusplus
        extern "C" {
#endif

#pragma pack (push,8)
/*
#ifndef Handle
        #define Handle void *
#endif
*/
///////////////////////////////////////////////////////////////////////////////////////////
//
typedef struct tagRSPreProcessImage
{
	puchar	*pgpRecogDIB;
	Bool32	gbAutoRotate;
	Bool32  gbDotMatrix;
	Bool32  gbFax100;
	uint32_t  gnLanguage;
	uint32_t  gnTables;
	Handle	hCPAGE;
	Handle  hDebugCancelSearchPictures;
	Handle  hDebugCancelComponent;
	Handle  hDebugCancelTurn;
	Handle  hDebugCancelSearchLines;
	Handle  hDebugCancelVerifyLines;
	Handle  hDebugCancelSearchDotLines;
	Handle  hDebugCancelRemoveLines;
	Handle  hDebugCancelSearchTables;
	Handle  hDebugCancelAutoTemplate;
	Handle  hDebugEnableSearchSegment;
	const char ** pglpRecogName;
	Handle* phCCOM;
	void *  pinfo;
	Handle* phLinesCCOM;
	void *  phCLINE;
	PBool32 pgneed_clean_line;
	int32_t *  pgnNumberTables;
	uint32_t  gnPictures;
	Bool32* pgrc_line;
	Rect32  gRectTemplate;
	FNPUMA_XSetTemplate fnXSetTemplate;
	FNPUMA_XGetTemplate fnXGetTemplate;
	char *szLayoutFileName;
} RSPreProcessImage, *PRSPreProcessImage;
///////////////////////////////////////////////////////////////////////////////////////////
//
typedef struct tagRSCBProgressPoints
{
	void *	pProgressStart;
	void *	pProgressStep;
	void *	pProgressStepLines;
	void *	pProgressStepTables;
	void *	pProgressFinish;
	void *  pInitPRGTIME;
	void *  pStorePRGTIME;
	void *  pRestorePRGTIME;
	void *  pDonePRGTIME;
	void *  prexcProgressStep;
	void *  pDPumaSkipComponent;
	void *  pDPumaSkipTurn;
	void *  pSetReturnCode;
	void *  pGetModulePath;
	void *  pSetUpdate;
} RSCBProgressPoints, *PRSCBProgressPoints;
///////////////////////////////////////////////////////////////////////////////////////////
RSTUFF_FUNC(Bool32) RSTUFF_Init(uint16_t wHeightCode,Handle hStorage);
RSTUFF_FUNC(Bool32) RSTUFF_Done();
RSTUFF_FUNC(uint32_t) RSTUFF_GetReturnCode();
RSTUFF_FUNC(char *) RSTUFF_GetReturnString(uint32_t dwError);
RSTUFF_FUNC(Bool32) RSTUFF_GetExportData(uint32_t dwType, void * pData);
RSTUFF_FUNC(Bool32) RSTUFF_SetImportData(uint32_t dwType, void * pData);
/////////////////////////////////////////////////////////////

typedef enum
{
                RSTUFF_FN_RSBinarise = 1,
                RSTUFF_FN_RSNormalise,
                RSTUFF_FN_RSLayout,
				RSTUFF_FN_RSSetSpecPrj
} RSTUFF_EXPORT_ENTRIES;
//////////////////////////////////////////////////////////////////////////////////////////////
//typedef void   (*PRSTUFFCBPRogressStart)(void);
//typedef Bool32 (*PRSTUFFCBPRogressStep)(uint32_t);
//typedef void   (*PRSTUFFCBPRogressFinish)(void);
//////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
		RSTUFF_FN_SetProgresspoints = 128,
		RSTUFF_FN_SetProgressStart,
		RSTUFF_FN_SetProgressStep,
		RSTUFF_FN_SetProgressFinish,
		RSTUFF_FN_SetInitPRGTIME,
		RSTUFF_FN_SetDPumaSkipComponent
} RSTUFF_IMPORT_ENTRIES;
////////////////////////////////////////////////////////////////////////////////////////////////
/*  Описание функций  */
#define DEC_FUN(a,b,c) typedef a (*FNRSTUFF##b)c; RSTUFF_FUNC(a) RSTUFF_##b c
DEC_FUN(Bool32, RSBinarise,          (void));
DEC_FUN(Bool32, RSNormalise,         (PRSPreProcessImage,void* vBuff,int Size,void* vWork,int SizeWork));
DEC_FUN(Bool32, RSNormVerify,        (PRSPreProcessImage));
DEC_FUN(Bool32, RSNormRemoveLines,   (PRSPreProcessImage));
DEC_FUN(Bool32, RSLayout,            (PRSPreProcessImage));
DEC_FUN(Bool32, RSSetSpecPrj,          (uchar NoSpecPrj));
#undef DEC_FUN


#pragma pack (pop)

#ifdef __cplusplus
            }
#endif

#endif //
