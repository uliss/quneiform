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

    #ifndef __MPUMA_H__
    #define __MPUMA_H__

    /*#include <crtdbg.h>*/

    #include "resource.h"
    #include "puma.h"
    #include "dpuma.h"
    #include "pumadef.h"
    #include "mymem.h"
    #include "ccom.h"
    #include "ced.h"
    #include "cfio.h"
    #include "cpage.h"
    #include "criimage.h"
    #include "cstr.h"
    #include "ctiimage.h"
    #include "cline.h"
    #include "exc.h"
    #include "rblock.h"
    #include "rline.h"
    #include "rfrmt.h"
    #include "rout.h"
    #include "rpic.h"
    #include "rpstr.h"
    #include "rstr.h"
    #include "rstuff.h"
    #include "rverline.h"
    #include "rmarker.h"
    #include "rselstr.h"
    #include "mpumatime.h"
    #include "rsl.h"
    #include "rreccom.h"
    #include "rcorrkegl.h"
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    #define BYTE char

    // predefined
    #ifdef __PUMA_CPP__
    	#define EXTERN
    	#define VAL(a)		= a
    	#define VALM(a)		= {a}
    	#define VAL2(a,b)	= { a,b }
    	#define VAL4(a,b,c,d)	= { a,b,c,d }
    #else
    	#define EXTERN		extern
    	#define VAL(a)
    	#define VALM(a)
    	#define VAL2(a,b)
    	#define VAL4(a,b,c,d)
    #endif

    #define NOSAVEBACKUP	//не сохранять предысторию ручной фрагментации - Paul 29-12-2000


    // global variables
    // Исходные данные, передаваемые пользователем
    EXTERN Word32 gnLanguage			VAL(7);
    EXTERN Bool32 gbSpeller				VAL(TRUE);
    EXTERN Bool32 gbOneColumn			VAL(FALSE);
    EXTERN Bool32 gbFax100				VAL(FALSE);
    EXTERN Bool32 gbDotMatrix			VAL(FALSE);
    EXTERN Bool32 gbBold				VAL(TRUE);
    EXTERN Bool32 gbItalic				VAL(TRUE);
    EXTERN Bool32 gbSize				VAL(TRUE);
    EXTERN Bool32 gbFormat				VAL(TRUE); // устарела см. gnFormat
    EXTERN Word32 gnFormat				VAL(1);
    EXTERN Bool32 gnPreserveLineBreaks  VAL(FALSE);

    EXTERN Word8  gnUnrecogChar			VAL('~');

    EXTERN Word32 gnPictures			VAL(1);
    EXTERN Word32 gnTables				VAL(1);

    EXTERN const char * gpUserDictName		VAL("");
    EXTERN const char * gpSerifName			VAL("Times New Roman");
    EXTERN const char * gpSansSerifName		VAL("Arial");
    EXTERN const char * gpCourierName			VAL("Courier New");

    EXTERN char	  szInputFileName[260];
    EXTERN PWord8 gpInputDIB			VAL(NULL);
    EXTERN PWord8 gpRecogDIB			VAL(NULL);
    EXTERN Bool32 gbAutoRotate			VAL(FALSE);
    EXTERN Point32 gPageSize			VAL2(209,295); // формат А4 в миллиметрах
    EXTERN Rect32 gRectTemplate			VAL4(-1,-1,-1,-1);

    // Данные образуемые в результате работы.
    EXTERN char szFormatStorageName[]	VAL("%sstorage.tmp");
    EXTERN char szFormatTbl1[]			VAL("%stbl1%s.dat");

    EXTERN int32_t gnNumberTables			VAL(0);

    EXTERN Handle       hCCOM			      VAL(NULL);
    EXTERN Handle       hCPAGE				  VAL(NULL);
    EXTERN CLINE_handle hCLINE                VAL(0);
    EXTERN Handle       hLinesCCOM            VAL(NULL); // компоненты с линиями
    EXTERN Handle       ghEdPage			  VAL(NULL);

    // 1. Отладочная информаци
    EXTERN Handle hDebugRoot		          VAL(NULL);
    EXTERN Handle hDebugCancelStrings         VAL(NULL);
    EXTERN Handle hDebugCancelRecognition     VAL(NULL);
    EXTERN Handle hDebugCancelRemoveLines	  VAL(NULL);
    EXTERN Handle hDebugCancelSearchLines	  VAL(NULL);
    EXTERN Handle hDebugCancelOrtoMove        VAL(NULL);
    EXTERN Handle hDebugCancelAutoTemplate    VAL(NULL);
    EXTERN Handle hDebugCancelSearchTables	  VAL(NULL);
    EXTERN Handle hDebugCancelVerifyLines	  VAL(NULL);
    EXTERN Handle hDebugCancelSearchDotLines  VAL(NULL);
    EXTERN Handle hDebugCancelFormatted		  VAL(NULL);
    EXTERN Handle hDebugCancelBinarize		  VAL(NULL);
    EXTERN Handle hDebugCancelComponent		  VAL(NULL);
    EXTERN Handle hDebugCancelComponentSecond VAL(NULL);
    EXTERN Handle hDebugCancelExtractBlocks   VAL(NULL);
    EXTERN Handle hDebugCancelStringsPass2	  VAL(NULL);
    EXTERN Handle hDebugCancelStringsColor    VAL(NULL);
    EXTERN Handle hDebugCancelPostRecognition VAL(NULL);
    EXTERN Handle hDebugCancelPostSpeller     VAL(NULL);
    EXTERN Handle hDebugCancelSearchPictures  VAL(NULL);
    EXTERN Handle hDebugCancelLinePass3       VAL(NULL);
    EXTERN Handle hDebugCancelSearchNegatives VAL(NULL);
	EXTERN Handle hDebugEnableSearchSegment   VAL(NULL);
    EXTERN Handle hDebugCancelVertCells       VAL(NULL);
    EXTERN Handle hDebugEnableSaveJtl         VAL(NULL);
    EXTERN Handle hDebugEnableSaveCstr1       VAL(NULL);
    EXTERN Handle hDebugEnableSaveCstr2       VAL(NULL);
    EXTERN Handle hDebugEnableSaveCstr3       VAL(NULL);
    EXTERN Handle hDebugEnableSaveCstr4       VAL(NULL);
    EXTERN Handle hDebugCPAGEStorage	      VAL(NULL);
    // 2.
    EXTERN Handle hDebugRootStuff		      VAL(NULL);
    EXTERN Handle hDebugBinarize		      VAL(NULL);
    EXTERN Handle hDebugAutoTemplate          VAL(NULL);
    EXTERN Handle hDebugLines			      VAL(NULL);
    EXTERN Handle hDebugVerLines		      VAL(NULL);
    EXTERN Handle hDebugRSL		              VAL(NULL);

    // 3
    EXTERN Handle hDebugRootLayout		      VAL(NULL);
    EXTERN Handle hDebugLayout			      VAL(NULL);
    EXTERN Handle hDebugPictures		      VAL(NULL);
	EXTERN Handle hDebugSegment 		      VAL(NULL);
    EXTERN Handle hDebugNeg     		      VAL(NULL);
    EXTERN Handle hDebugFon     		      VAL(NULL);
    EXTERN Handle hDebugTables			      VAL(NULL);
    EXTERN Handle hDebugStrings			      VAL(NULL);
    EXTERN Handle hDebugPostLines		      VAL(NULL);
    EXTERN Handle hDebugSVLines			      VAL(NULL);
    EXTERN Handle hDebugSVLinesStep		      VAL(NULL);
    EXTERN Handle hDebugSVLinesData		      VAL(NULL);
    // 4
    EXTERN Handle hDebugRootRecognition	      VAL(NULL);
    EXTERN Handle hDebugRecognition		      VAL(NULL);
    EXTERN Handle hDebugSpell			      VAL(NULL);
    // 5
    EXTERN Handle hDebugRootFormatting	      VAL(NULL);
    EXTERN Handle hDebugConverters		      VAL(NULL);
    EXTERN Handle hDebugCeglCorr		      VAL(NULL);


    EXTERN Handle hDebugMainDebug             VAL(NULL);
    EXTERN Handle hDebugMainTimeControl       VAL(NULL);

    EXTERN Handle hDebugLayoutFromFile	      VAL(NULL);
    EXTERN Handle hDebugLayoutToFile	      VAL(NULL);
    EXTERN Handle hDebugHandLayout		      VAL(NULL);

    EXTERN Handle hDebugCancelPropertyConsole                       VAL(NULL);
    EXTERN Handle hDebugCancelConsoleOutputText                     VAL(NULL);
    EXTERN Handle					hDebugCancelConsoleKeglOutput	VAL(NULL);
    EXTERN Handle					hDebugCancelRanalFrm			VAL(NULL);
    EXTERN Handle					hDebugPrintBlocksCPAGE			VAL(NULL);
    EXTERN Handle					hDebugCancelFictive				VAL(NULL);
    EXTERN Handle					hDebugCancelTurn				VAL(NULL);
    EXTERN Handle					hDebugEnablePrintFormatted		VAL(NULL);

    EXTERN char						szLayoutFileName[256]			VAL("Layout.bin");
    EXTERN unsigned					nDebugReturnCode				VAL(0);
    EXTERN unsigned					nDebugAllocMemory				VAL(0);

    EXTERN FNPUMA_ProgressStart		fnProgressStart					VAL(NULL);
    EXTERN FNPUMA_ProgressFinish	fnProgressFinish				VAL(NULL);
    EXTERN FNPUMA_ProgressStep		fnProgressStep					VAL(NULL);

    EXTERN Word32					g_prgStep						VAL(0);
    //Allex  при разделении бинаризации и обработки сырь
    //имя картинки 'lpRecogName' сделано глабольной переменной
    EXTERN const char *					glpRecogName					VAL(NULL);
    EXTERN Bool32                   grc_line                        VAL(TRUE);
    EXTERN Bool32                   gneed_clean_line                VAL(FALSE);
    EXTERN Bool32                   gKillVSLComponents              VAL(TRUE);
    EXTERN Word8  gnSpecialProject			VAL(0);

    //allex
    #ifdef _DEBUG
#ifdef _MSC_VER
#include<crtdbg.h>
#endif
    EXTERN _CrtMemState				g_dbgMemState;

    #endif

    // Флаги обновления контейнеров
    // перехали в mpumatime.h
    //#define FLG_UPDATE_NO		0
    //#define FLG_UPDATE			(Word32)-1
    //#define FLG_UPDATE_CCOM		0x1
    //#define FLG_UPDATE_CPAGE	0x2

    EXTERN Word32 g_flgUpdate			VAL(0);

    Bool32 IsUpdate(Word32 flg);
    void   SetUpdate(Word32 flgAdd,Word32 flgRemove);

    //typedef struct
    //	{
    //	Word32 dwBeg;   // Начало текущего диапозона
    //	Word32 dwEnd;	// Конец
    //
    //	Word32 dwStep;  // текущий номер шага
    //	char * name;	// текущее название шага
    //	} PRGTIME;

    EXTERN PRGTIME g_PrgTime;

    //////////////////////////////////////////////////////////////////
    // 07.07,2000 Allex
    // Вынес Бинаризацию наверх из PreProcessImage
    //////////////////////////////////////////////////////////Allex
    EXTERN CIMAGEBITMAPINFOHEADER info;
    //EXTERN PUMALinesBuffer        gSVLBuffer            VALM(0);
    //#define PUMA_SVL_FIRST_STEP                         0x1
    //#define PUMA_SVL_SECOND_STEP                        0x2
    //#define PUMA_SVL_THRID_STEP                         0x3
    //#define PUMAMaxNumLines                             2000
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //functions
    void	SetReturnCode_puma(Word32 rc);
    Word32	GetReturnCode_puma();
    char *	GetModulePath();
    char *	GetModuleTempPath();
    char *  GetResourceString(Word32 id);

    Bool32 InitMem(void);
    void   DoneMem(void);
    void   GiveMainBuff (void **vvBuff, int *Size);
    void   GiveWorkBuff (void **vvBuff, int *Size);

    //use SJTL.dll
    #ifdef __cplusplus
    	extern "C" {
    #endif
    void My_SJTL_Init(void);
    int My_SJTL_save(const char* filename);
    int My_SJTL_save_old(char *filename, CSTR_line lino,int nfield);
    int My_SJTL_open(const char *frmname, const char *jtlname);
    int My_SJTL_mkfrm(char *frmname);
    void My_SJTL_close(void);
    void My_SJTL_Done(void);
    #ifdef __cplusplus
    	}
    #endif


    Bool32 ModulesInit(Handle  ghStorage);
    Bool32 ModulesDone(Handle  ghStorage);
    char * GetModulesString(Word32 dwError);

    Bool32 Layout(PWord8 lpdata);
    Bool32 Recognize();

    void InitDebug();
    void DoneDebug();

    // STATIC
    static char lang_suff[][4] ={
    	"","GRM","FRN","RUS","SWE","SPA","ITA","R&E","UKR","SER","CRO",
    	"POL","DAN","POR","DUT","DIG"
    	};

    // Enum.cpp
    long	_EnumFormats(long prev);
    long	_EnumCodes(long f,long prev);
    long	_EnumLanguage(long prev);
    long	_EnumFormatMode(long prev);
    long	_EnumTable(long prev);
    long	_EnumPicture(long prev);

    // common.cpp
    void	ClearAll( void );
    Bool32  rexcProgressStep  (Word32);
    Bool32  ExtractComponents(Bool32 bIsRotate, Handle * prev_ccom , PWord8 name);
    Bool32  RemoveLines(Handle hccom,Handle hcpage,PWord8 * pDIB);
    void    SetOptionsToFRMT( void );
    Bool32  SaveToText(char * lpOutFileName,int code);
//    Bool32  HL_TableExtract( Handle hPAGE, Word32 type, Rect32 rect );
//    Bool32  HL_TableExtractEx( Handle hPAGE, Word32 perc, Rect32 rect );
    // Функции прогресс индикатора.
    // 1. Для инициализации внутреннего счетчика надо вызвать InitPRGTIME. Возвращает TRUE, если выполняется впервые
    // 2. Внутри одной функции разбиение идет всегда от 0 до 100 процентов
    // 3. Перед вызовом функции надо установить диапозон прогресса для этой функции в масштабе 0 - 100
    //		Для этого надо выполнить StorePRGTIME(Word32 beg, Word32 end);
    // 4. После возврата из этой функции надо восстановить счетчик функцией RestorePRGTIME(PRGTIME prev)
    //		Параметром этой функции должно быть возвращаемое значение функции StorePRGTIME
    // 5. В конце функции вызвать DonePRGTIME
    void    ResetPRGTIME();
    Bool32	InitPRGTIME();
    Bool32	DonePRGTIME();
    PRGTIME	StorePRGTIME(Word32 beg, Word32 end);// Устанавливает дапозон изменений, который учитывается в  ProgressStep
    void 	RestorePRGTIME(PRGTIME prev);// Устанавливает дапозон изменений, который учитывается в  ProgressStep

    void	ProgressStart();
    void	ProgressFinish();
    Bool32	ProgressStep(Word32 step,char*name,Word32 percent);
    Bool32  ProgressStepLayout(Word32 step,Word32 percent);
    Bool32  ProgressStepLines(Word32 step,Word32 percent);
    Bool32  ProgressStepTables(Word32 step,Word32 percent);
    Bool32  ProgressStepSearchTables(Word32 step,Word32 percent);
    Bool32  ProgressStepAutoLayout(Word32 step,Word32 percent);
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Allex (перенесено в pumadef.h
    //#define NAME_IMAGE_INPUT		"Исходное изображение"
    //#define NAME_IMAGE_BINARIZE    "Бинаризованное изображение"
    //#define NAME_IMAGE_DELLINE		"Изображение со снятыми линиями"
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    Bool32 PreProcessImage();
    Bool32 BinariseImage();

    Bool32 PrintResult(int num,CSTR_line lout,Handle hCPAGE);
    Bool32 ConverROUT(char * lpOutFileName, int32_t lnFormat, int32_t lnCode,Bool32 bAppend);
    Word32 ConverROUTtoMemory(Handle hEd, int32_t lnFormat, int32_t lnCode, Byte * lpMem, Word32 size);
    /*
    Bool32 ReadSVLFromPageContainer ( LinesTotalInfo * );
    Bool32 ShortVerticalLinesProcess ( Word32 );
    Bool32 SVLFilter(LinesTotalInfo *, LinesTotalInfo *);
    Bool32 SVLComponentFilter(LineInfo *);
    Bool32 IsRectIntersect(Rect16 *, Rect16 *);
    Bool32 CompIsGood (CCOM_comp *, int32_t );
    */
    //////////////////////////////////////////////////////////////////////////////////

    #endif
