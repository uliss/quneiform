/*
 * pumaimpl.cpp
 *
 *  Created on: 22.09.2009
 *      Author: uliss
 */

#include "pumaimpl.h"
#include "puma.h"
#include "mpuma.h"
#include "pumadef.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstring>

#include "helper.h"
#include "specprj.h"
#include "ligas.h"		// 12.06.2002 E.P.
#include "cimage/cticontrol.h"

char global_buf[64000]; // OLEG fot Consistent
int32_t global_buf_len = 0; // OLEG fot Consistent

static double portion_of_rus_letters(CSTR_line lin_ruseng) {
	if (!lin_ruseng)
		return 0;
	CSTR_rast_attr attr;
	CSTR_rast rast = CSTR_GetFirstRaster(lin_ruseng);
	UniVersions uv;
	int nRus = 0, nAll = 0;
	for (rast = CSTR_GetNext(rast); rast; rast = CSTR_GetNext(rast)) {
		CSTR_GetAttr(rast, &attr);
		CSTR_GetCollectionUni(rast, &uv);
		if (attr.flg & (CSTR_f_let)) {
			if (attr.language == LANG_RUSSIAN && uv.lnAltCnt && uv.Alt[0].Prob
					> 100 && !strchr("0123456789/%", uv.Alt[0].Code[0]))
				nRus++;
			nAll++;
		}
	}
	if (!nAll)
		return 0;

	return (double) nRus / (double) nAll;
}

using namespace std;

#ifndef NDEBUG
#define DBG(msg) cerr << msg << endl;
#else
#define DBG(msg) ;
#endif

static Handle ghStorage = NULL;

namespace CIF {

FixedBuffer<unsigned char, PumaImpl::MainBufferSize> PumaImpl::main_buffer_;
FixedBuffer<unsigned char, PumaImpl::WorkBufferSize> PumaImpl::work_buffer_;

PumaImpl::PumaImpl() {
	modulesInit();
}

PumaImpl::~PumaImpl() {
	modulesDone();
	//	CTIControl * ci = new CTIControl;
	//	cimage_.reset(ci);
}

void PumaImpl::binarizeImage() {
	// Бинаризуем изображение
	gpRecogDIB = gpInputDIB;
	glpRecogName = PUMA_IMAGE_USER;

	//Allex
	//CIMAGEBITMAPINFOHEADER info;
	if (!CIMAGE_GetImageInfo((puchar) PUMA_IMAGE_USER, &info))
		throw PumaException("CIMAGE_GetImageInfo failed");

	LDPUMA_Console("The image depth is %d at this point.\n", info.biBitCount);

	if (info.biBitCount > 1) {
		//RIMAGE_BINARISE_KRONROD
		if (!RIMAGE_Binarise((puchar) PUMA_IMAGE_USER,
				(puchar) PUMA_IMAGE_BINARIZE, 4, 0))
			throw PumaException("RIMAGE_Binarise failed");

		if (!CIMAGE_ReadDIB((puchar) PUMA_IMAGE_BINARIZE,
				(Handle*) &gpRecogDIB, TRUE))
			throw PumaException("CIMAGE_ReadDIB failed");

		PAGEINFO info;
		GetPageInfo(hCPAGE, &info);
		info.Images |= IMAGE_BINARIZE;
		SetPageInfo(hCPAGE, info);

		glpRecogName = PUMA_IMAGE_BINARIZE;
	}
}

void PumaImpl::clearAll() {
	// Сохраним последенне состояние и очистим контейнер
	if (ghEdPage) {
		CED_DeletePage(ghEdPage);
		ghEdPage = NULL;
	}

	PAGEINFO PInfo;
	memset(&PInfo, 0, sizeof(PInfo));

	if (hCPAGE)
		GetPageInfo(hCPAGE, &PInfo);

	CSTR_DeleteAll();
	CPAGE_DeleteAll();
	hCPAGE = CreateEmptyPage();

	strcpy((char*) PInfo.szImageName, PUMA_IMAGE_USER);
	PInfo.Incline2048 = 0;
	PInfo.Angle = 0;
	PInfo.Images = IMAGE_USER;
	SetPageInfo(hCPAGE, PInfo);

	CCOM_DeleteAll();
	hCCOM = NULL;
	CIMAGE_DeleteImage((puchar) PUMA_IMAGE_BINARIZE);
	CIMAGE_DeleteImage((puchar) PUMA_IMAGE_DELLINE);
	//  Повернутое изображение ( PUMA_IMAGE_ROTATE) удалять нельзя, как и исходное,
	//  поскольку оно отображается в интерфейсе. Его нужно удалять
	//  либо при получении нового довернутого изображения, либо при
	//  закрытии файла
	CIMAGE_DeleteImage((puchar) PUMA_IMAGE_TURN);
}

void PumaImpl::close() {
	DBG("Puma close")
	CLINE_Reset();
	clearAll();
	// clean
	CIMAGE_Reset();
	CPAGE_DeleteAll();
	RIMAGE_Reset();
	hCPAGE = NULL;

	gpRecogDIB = gpInputDIB = NULL;
}

void PumaImpl::extractComponents() {
	if (!LDPUMA_Skip(hDebugCancelComponent)) {
		LDPUMA_Console("Пропущен этап выделения компонент.\n");
		return;
	}

	PAGEINFO info;
	if (!GetPageInfo(hCPAGE, &info))
		throw PumaException("GetPageInfo failed");

	ExcControl exc;
	memset(&exc, 0, sizeof(exc));

	CCOM_DeleteContainer((CCOM_handle) hCCOM);
	hCCOM = NULL;

	if (!REXC_SetImportData(REXC_ProgressStep, (void*) rexcProgressStep))
		throw PumaException("REXC_SetImportData failed");

	// будет распознавания эвентами
	exc.Control = Ex_ExtraComp | Ex_Picture;

	//Andrey: orientation is obtained from new library RNORM
	if (gnPictures)
		exc.Control |= Ex_PictureLarge;

	uchar w8 = (uchar) gbDotMatrix;
	REXC_SetImportData(REXC_Word8_Matrix, &w8);

	w8 = (uchar) gbFax100;
	REXC_SetImportData(REXC_Word8_Fax1x2, &w8);

	CIMAGEIMAGECALLBACK clbk;
	if (!CIMAGE_GetCallbackImage((uchar*) info.szImageName, &clbk))
		throw PumaException("CIMAGE_GetCallbackImage failed");

	if (!REXCExtracomp3CB(
			exc, // поиск компонент by 3CallBacks
			(TImageOpen) clbk.CIMAGE_ImageOpen,
			(TImageClose) clbk.CIMAGE_ImageClose,
			(TImageRead) clbk.CIMAGE_ImageRead))
		throw PumaException("REXCExtracomp3CB failed");

	hCCOM = (Handle) REXCGetContainer();
	if (!hCCOM)
		throw PumaException("REXCGetContainer failed");

	hCCOM = (Handle) REXCGetContainer();
	if (!hCCOM)
		throw PumaException("REXCGetContainer failed");

	SetUpdate(FLG_UPDATE_NO, FLG_UPDATE_CCOM);
}

void PumaImpl::extractStrings() {
	if (LDPUMA_Skip(hDebugStrings)) {
		if (!RSELSTR_ExtractTextStrings(hCCOM, hCPAGE))
			throw PumaException("RSELSTR_ExtractTextStrings failed");
	} else {
		if (!RBLOCK_ExtractTextStrings(hCCOM, hCPAGE))
			throw PumaException("RBLOCK_ExtractTextStrings failed");
	}
}

void PumaImpl::formatResult() {
	SetOptionsToFRMT();
	if (ghEdPage) {
		CED_DeletePage(ghEdPage);
		ghEdPage = NULL;
	}

	if (!RFRMT_Formatter(szInputFileName.c_str(), &ghEdPage))
		throw PumaException("RFRMT_Formatter failed");

	if (!LDPUMA_Skip(hDebugEnablePrintFormatted)) {
		std::string fname(szInputFileName + "_tmp_.rtf");
		SetOptionsToFRMT();
		RFRMT_SaveRtf(fname.c_str(), 8);
		fname = szInputFileName + "_tmp_.fed";
		save(fname.c_str(), PUMA_TOEDNATIVE);
	}
}

// Allex
// добавлены для обратной связи из RStuff
Bool32 DPumaSkipComponent(void) {
	return LDPUMA_Skip(hDebugCancelComponent);
}
Bool32 DPumaSkipTurn(void) {
	return LDPUMA_Skip(hDebugCancelTurn);
}

void PumaImpl::layout() {
	clearAll();
	binarizeImage();

	RSCBProgressPoints CBforRS;
	RSPreProcessImage DataforRS;

	RMCBProgressPoints CBforRM;
	RMPreProcessImage DataforRM;

	void* MemBuf = CIF::PumaImpl::mainBuffer();
	size_t size_buf = CIF::PumaImpl::MainBufferSize;
	void* MemWork = CIF::PumaImpl::workBuffer();
	int size_work = CIF::PumaImpl::WorkBufferSize;

#define SET_CB(a,b)   a.p##b = (void*)b
	SET_CB(CBforRS, ProgressStart);
	CBforRS.pProgressStep = (void*) ProgressStepLayout;
	CBforRS.pProgressStepLines = (void*) ProgressStepLines;
	CBforRS.pProgressStepTables = (void*) ProgressStepTables;
	SET_CB(CBforRS, ProgressFinish);
	SET_CB(CBforRS, InitPRGTIME);
	SET_CB(CBforRS, StorePRGTIME);
	SET_CB(CBforRS, RestorePRGTIME);
	SET_CB(CBforRS, DonePRGTIME);
	SET_CB(CBforRS, rexcProgressStep);
	SET_CB(CBforRS, DPumaSkipComponent);
	SET_CB(CBforRS, DPumaSkipTurn);
	CBforRS.pSetReturnCode = (void*) SetReturnCode_puma;
	SET_CB(CBforRS, GetModulePath);
	SET_CB(CBforRS, SetUpdate);

	SET_CB(CBforRM, ProgressStart);
	CBforRM.pProgressStepAutoLayout = (void*) ProgressStepAutoLayout;
	CBforRM.pProgressStepSearchTables = (void*) ProgressStepSearchTables;
	SET_CB(CBforRM, ProgressFinish);
	SET_CB(CBforRM, InitPRGTIME);
	SET_CB(CBforRM, StorePRGTIME);
	SET_CB(CBforRM, RestorePRGTIME);
	SET_CB(CBforRM, DonePRGTIME);
	SET_CB(CBforRM, rexcProgressStep);
	SET_CB(CBforRM, DPumaSkipComponent);
	SET_CB(CBforRM, DPumaSkipTurn);
	CBforRM.pSetReturnCode = (void*) SetReturnCode_puma;
	SET_CB(CBforRM, GetModulePath);
	SET_CB(CBforRM, SetUpdate);
#undef SET_CB

	DataforRS.gbAutoRotate = gbAutoRotate;
	DataforRS.pgpRecogDIB = &gpRecogDIB;
	DataforRS.pinfo = &info;
	DataforRS.hCPAGE = hCPAGE;
	DataforRS.phCCOM = &hCCOM;
	DataforRS.phCLINE = &hCLINE;
	DataforRS.phLinesCCOM = &hLinesCCOM;
	DataforRS.gnPictures = gnPictures;
	DataforRS.gnLanguage = gnLanguage;
	DataforRS.gbDotMatrix = gbDotMatrix;
	DataforRS.gbFax100 = gbFax100;
	DataforRS.pglpRecogName = &glpRecogName;
	DataforRS.pgrc_line = &grc_line;
	DataforRS.gnTables = gnTables;
	DataforRS.pgnNumberTables = &gnNumberTables;
	DataforRS.pgneed_clean_line = &gneed_clean_line;
	DataforRS.gRectTemplate = gRectTemplate;
	DataforRS.fnXSetTemplate = PUMA_XSetTemplate;
	DataforRS.fnXGetTemplate = PUMA_XGetTemplate;
	DataforRS.hDebugCancelSearchPictures = hDebugCancelSearchPictures;
	DataforRS.hDebugCancelComponent = hDebugCancelComponent;
	DataforRS.hDebugCancelTurn = hDebugCancelTurn;
	DataforRS.hDebugCancelAutoTemplate = hDebugCancelAutoTemplate;
	DataforRS.hDebugCancelSearchLines = hDebugCancelSearchLines;
	DataforRS.hDebugCancelVerifyLines = hDebugCancelVerifyLines;
	DataforRS.hDebugCancelSearchDotLines = hDebugCancelSearchDotLines;
	DataforRS.hDebugCancelRemoveLines = hDebugCancelRemoveLines;
	DataforRS.hDebugCancelSearchTables = hDebugCancelSearchTables;
	DataforRS.szLayoutFileName = szLayoutFileName;
	DataforRS.hDebugEnableSearchSegment = hDebugEnableSearchSegment;

	// калбэки
	if (RSTUFF_SetImportData(RSTUFF_FN_SetProgresspoints, &CBforRS)) {
		///нормализуем - обработка, поиск картинок, поиск линий
		if (!RSTUFF_RSNormalise(&DataforRS, MemBuf, size_buf, MemWork,
				size_work))
			throw PumaException("RSTUFF_RSNormalise failed");
	}

	// Gleb 02.11.2000
	// Далее - разметка. Вынесена в RMARKER.DLL
	DataforRM.gbAutoRotate = gbAutoRotate;
	DataforRM.pgpRecogDIB = &gpRecogDIB;
	DataforRM.gbOneColumn = gbOneColumn;
	DataforRM.gKillVSLComponents = gKillVSLComponents;
	DataforRM.pinfo = &info;
	DataforRM.hCPAGE = hCPAGE;
	DataforRM.hCCOM = hCCOM;
	DataforRM.hCLINE = hCLINE;
	DataforRM.phLinesCCOM = &hLinesCCOM;
	DataforRM.gnPictures = gnPictures;
	DataforRM.gnLanguage = gnLanguage;
	DataforRM.gbDotMatrix = gbDotMatrix;
	DataforRM.gbFax100 = gbFax100;
	DataforRM.pglpRecogName = &glpRecogName;
	DataforRM.pgrc_line = &grc_line;
	DataforRM.gnTables = gnTables;
	DataforRM.pgnNumberTables = &gnNumberTables;
	DataforRM.pgneed_clean_line = &gneed_clean_line;
	DataforRM.hDebugCancelSearchPictures = hDebugCancelSearchPictures;
	DataforRM.hDebugCancelComponent = hDebugCancelComponent;
	DataforRM.hDebugCancelTurn = hDebugCancelTurn;
	DataforRM.hDebugCancelSearchLines = hDebugCancelSearchLines;
	DataforRM.hDebugCancelVerifyLines = hDebugCancelVerifyLines;
	DataforRM.hDebugCancelSearchDotLines = hDebugCancelSearchDotLines;
	DataforRM.hDebugCancelRemoveLines = hDebugCancelRemoveLines;
	DataforRM.hDebugCancelSearchTables = hDebugCancelSearchTables;
	DataforRM.hDebugLayoutFromFile = hDebugLayoutFromFile;
	DataforRM.hDebugCancelExtractBlocks = hDebugCancelExtractBlocks;
	DataforRM.hDebugHandLayout = hDebugHandLayout;
	DataforRM.hDebugPrintBlocksCPAGE = hDebugPrintBlocksCPAGE;
	DataforRM.hDebugSVLines = hDebugSVLines;
	DataforRM.hDebugSVLinesStep = hDebugSVLinesStep;
	DataforRM.hDebugSVLinesData = hDebugSVLinesData;
	DataforRM.szLayoutFileName = szLayoutFileName;
	DataforRM.hDebugEnableSearchSegment = hDebugEnableSearchSegment;

	if (RMARKER_SetImportData(0, &CBforRM)) {
		if (!RMARKER_PageMarkup(&DataforRM, MemBuf, size_buf, MemWork,
				size_work))
			throw PumaException("RMARKER_PageMarkup failed");

		hCPAGE = DataforRM.hCPAGE; //Paul 25-01-2001
	}

	if (!LDPUMA_Skip(hDebugPrintBlocksCPAGE)) {
		LDPUMA_Console("Контейнер CPAGE содержит: \n имя : размер\n");
		Handle block = CPAGE_GetBlockFirst(hCPAGE, 0);
		while (block) {
			LDPUMA_Console("%s : %i\n", CPAGE_GetNameInternalType(
					CPAGE_GetBlockType(hCPAGE, block)), CPAGE_GetBlockData(
					hCPAGE, block, CPAGE_GetBlockType(hCPAGE, block), NULL, 0));
			block = CPAGE_GetBlockNext(hCPAGE, block, 0);
		}
	}

	SetUpdate(FLG_UPDATE_NO, FLG_UPDATE_CPAGE);
}

void PumaImpl::loadLayoutFromFile(const std::string& fname) {
	hCPAGE = CPAGE_RestorePage(TRUE, fname.c_str());
	if (hCPAGE == NULL) {
		ostringstream os;
		os << "CPAGE_RestorePage failed from '" << fname << "'";
		throw PumaException(os.str());
	}
	CPAGE_SetCurrentPage(CPAGE_GetNumberPage(hCPAGE));
}

void PumaImpl::modulesDone() {
	ROUT_Done();
	CED_Done();
	RCORRKEGL_Done();
	RPIC_Done();
	RIMAGE_Done();
	RFRMT_Done();
	RSL_Done();
	REXC_Done();
	RLINE_Done();
	RMARKER_Done();
	RBLOCK_Done();
	RSELSTR_Done();
	RSTR_Done();
	CSTR_Done();
	CCOM_Done();
	CPAGE_Done();
	CIMAGE_Done();
	CLINE_Done();
	RPSTR_Done();
	RSTUFF_Done();
	RRECCOM_Done();

#ifdef _USE_RVERLINE_
	RVERLINE_Done();
#endif //_USE_RVERLINE_
#ifdef _USE_RMSEGMENT_
	RMSEGMENT_Done();
#endif //_USE_RMSEGMENT_
	CFIO::CFIO_Done();
}

void PumaImpl::modulesInit() {
	try {
		// CONTEINERS
		if (!CLINE_Init(PUMA_MODULE_CLINE, NULL))
			throw PumaException("CLINE_Init failed.");
		if (!CFIO::CFIO_Init(PUMA_MODULE_CFIO, NULL))
			throw PumaException("CFIO_Init failed.");
		if (!CIMAGE_Init(PUMA_MODULE_CIMAGE, NULL))
			throw PumaException("CIMAGE_Init failed.");
		//  нужна инициализация контейнера CCOM перед
		//  вызовом поиска компонент
		if (!CCOM_Init(PUMA_MODULE_CCOM, NULL))
			throw PumaException("CCOM_Init failed.");
		if (!CPAGE_Init(PUMA_MODULE_CPAGE, ghStorage))
			throw PumaException("CPAGE_Init failed.");
		if (!CSTR_Init(PUMA_MODULE_CSTR, ghStorage))
			throw PumaException("CSTR_Init failed.");

		// RECOGNITIONS
		if (!REXC_Init(PUMA_MODULE_REXC, NULL)) // инициализация библиотеки поиска компонент
			throw PumaException("REXC_Init failed.");

		//	REXC_SetImportData(REXC_OcrPath, GetModulePath());
		if (!RLINE_Init(PUMA_MODULE_RLINE, ghStorage))
			throw PumaException("RLINE_Init failed.");

		if (!RRECCOM_Init(PUMA_MODULE_RRECCOM, ghStorage))
			throw PumaException("RRECCOM_Init failed.");

		RRECCOM_SetImportData(RRECCOM_OcrPath, GetModulePath());

		if (!RSL_Init(PUMA_MODULE_RSL, ghStorage))
			throw PumaException("RSL_Init failed.");

		if (!RSTUFF_Init(PUMA_MODULE_RSTUFF, ghStorage))
			throw PumaException("RSTUFF_Init failed.");

		if (!RMARKER_Init(PUMA_MODULE_RBLOCK, ghStorage))
			throw PumaException("RMARKER_Init failed.");

		if (!RBLOCK_Init(PUMA_MODULE_RBLOCK, ghStorage))
			throw PumaException("RBLOCK_Init failed.");

		if (!RSELSTR_Init(PUMA_MODULE_RBLOCK, ghStorage))
			throw PumaException("RSELSTR_Init failed.");

		RSTR_SetImportData(RSTR_OcrPath, GetModulePath());
		RSTR_SetImportData(RSTR_pchar_temp_dir, GetModuleTempPath());

		if (!RSTR_Init(PUMA_MODULE_RSTR, ghStorage))
			throw PumaException("RSTR_Init failed.");
		if (!RFRMT_Init(PUMA_MODULE_RFRMT, ghStorage))
			throw PumaException("RFRMT_Init failed.");
		if (!RIMAGE_Init(PUMA_MODULE_RIMAGE, ghStorage))
			throw PumaException("RIMAGE_Init failed.");

		// Инициализируем виртуальные функции
		if (!RPSTR_Init(PUMA_MODULE_RPSTR, ghStorage))
			throw PumaException("RPSTR_Init failed.");
		if (!RPIC_Init(PUMA_MODULE_RPIC, ghStorage))
			throw PumaException("RPIC_Init failed.");
		if (!CED_Init(PUMA_MODULE_CED, ghStorage))
			throw PumaException("CED_Init failed.");
		if (!ROUT_Init(PUMA_MODULE_ROUT, ghStorage))
			throw PumaException("ROUT_Init failed.");
		if (!ROUT_LoadRec6List("rec6all.dat"))
			throw PumaException("ROUT_LoadRec6List failed.");

#ifdef _USE_RVERLINE_
		if(!RVERLINE_Init(PUMA_MODULE_RVERLINE,ghStorage))
		throw PumaException("RVERLINE_Init failed.");
#endif
#ifdef _USE_RMSEGMENT_
		if(!RMSEGMENT_Init(PUMA_MODULE_RMSEGMENT,ghStorage))
		throw PumaException("RMSEGMENT_Init failed.");
#endif
		if (!RCORRKEGL_Init(PUMA_MODULE_RCORRKEGL, ghStorage))
			throw PumaException("CORRKEGL_Init failed.");
	} catch (PumaException& e) {
		modulesDone();
		throw e;
	}
}

void PumaImpl::open(char * dib) {
	DBG("Puma open")
	assert(dib);
	preOpenInitialize();
	gpInputDIB = (unsigned char*) dib;
	// write image
	if (!CIMAGE_WriteDIB((puchar) PUMA_IMAGE_USER, dib, 1))
		throw PumaException("PumaImpl::open can't write DIB");

	postOpenInitialize();
	hCPAGE = CreateEmptyPage();
}

void PumaImpl::normalize() {
	//Нормализуем вертикальные строки
	RPSTR_NormalizeVertStr();
}

void PumaImpl::pass1() {
	if (!LDPUMA_Skip(hDebugEnableSaveCstr1))
		savePass1(replaceFileExt(szInputFileName, "_1.cst"));

	recognizePass1();
}

void PumaImpl::pass2() {
	if (!LDPUMA_Skip(hDebugEnableSaveCstr2)) {
		std::string CstrFileName = CIF::replaceFileExt(szInputFileName,
				"_2.cst");
		CSTR_SaveCont(CstrFileName.c_str());
	}

	///////////////////////////////
	// OLEG : 01-05-18 : for GiP //
	///////////////////////////////
	if (SPEC_PRJ_GIP == gnSpecialProject && gnLanguage == LANG_RUSENG) {
		int i, n;
		double s;
		CSTR_line lin_ruseng;

		n = CSTR_GetMaxNumber();
		for (s = 0.0, i = 1; i <= n; i++) {
			lin_ruseng = CSTR_GetLineHandle(i, 1);
			s += portion_of_rus_letters(lin_ruseng);
		}
		if (n)
			s /= (double) n;
		if (n && s < 0.4) {

			for (i = 0; i <= n; i++) {
				for (int j = 1; j < 10; j++) {
					lin_ruseng = CSTR_GetLineHandle(i, j);
					if (lin_ruseng)
						CSTR_DeleteLine(lin_ruseng);
				}
			}
			gnLanguage = LANG_ENGLISH;
			recognizeSetup(gnLanguage);
			recognizePass1();
		}
	}

	if (RSTR_NeedPass2())
		recognizePass2();
	else
		LDPUMA_Console("RSTR считает, что второй проход не нужен.\n");
}

void PumaImpl::spellCorrection() {
	if (!LDPUMA_Skip(hDebugEnableSaveCstr3)) {
		std::string CstrFileName = CIF::replaceFileExt(szInputFileName,
				"_3.cst");
		CSTR_SaveCont(CstrFileName.c_str());
	}

	// Дораспознаем по словарю
	CSTR_SortFragm(1);
	RPSTR_CollectCapDrops(1);
	if (LDPUMA_Skip(hDebugCancelPostSpeller) && gbSpeller) {
		if (!RPSTR_CorrectSpell(1))
			throw PumaException("RPSTR_CorrectSpell failed");
	}
}

void PumaImpl::preOpenInitialize() {
	PumaImpl::close();
	SetUpdate(FLG_UPDATE, FLG_UPDATE_NO);
}

void PumaImpl::printResult() {
	// Печать результатов в консоль
	int count = CSTR_GetMaxNumber();
	for (int i = 1; i <= count; i++)
		PrintResult(i, CSTR_GetLineHandle(i, CSTR_LINVERS_MAINOUT), hCPAGE);
}

void PumaImpl::postOpenInitialize() {
	LDPUMA_SetFileName(NULL, "none.txt");
	szInputFileName = "none.bin";

	CIMAGEBITMAPINFOHEADER info;
	if (!CIMAGE_GetImageInfo((puchar) PUMA_IMAGE_USER, &info))
		throw PumaException("CIMAGE_GetImageInfo failed");

	setTemplate(Rect(Point(0, 0), info.biWidth, info.biHeight));
}

void PumaImpl::recognize() {
	// Проверим: выделены ли фрагменты.
	if (!CPAGE_GetCountBlock(hCPAGE) || IsUpdate(FLG_UPDATE_CPAGE))
		layout();

	DBG("Puma recognize")

	CSTR_DeleteAll();

	if (hCPAGE)
		CPAGE_UpdateBlocks(hCPAGE, TYPE_CPAGE_TABLE);

	// Сохраним описание Layout в файл.
	if (!LDPUMA_Skip(hDebugLayoutToFile))
		saveLayoutToFile(szLayoutFileName);

	// Прочитаем описание Layout из файла.
	if (!LDPUMA_Skip(hDebugLayoutFromFile))
		loadLayoutFromFile(szLayoutFileName);

	if (IsUpdate(FLG_UPDATE_CCOM))
		extractComponents();

	// Получим описатель страницы
	hCPAGE = CPAGE_GetHandlePage(CPAGE_GetCurrentPage());

	// Выделим строки
	if (!LDPUMA_Skip(hDebugCancelStrings)) {
		LDPUMA_Console("Пропущен этап выделения строк.\n");
		return;
	}

	extractStrings();

	if (!LDPUMA_Skip(hDebugCancelRecognition)) {
		LDPUMA_Console("Пропущен этап распознавания.\n");
		return;
	}

	// распознаем строки
	CSTR_SortFragm(0);
	recognizeSetup(gnLanguage);

	CSTR_SortFragm(0);
	CSTR_line ln;
	CSTR_attr attr;
	int32_t nf = CSTR_GetMaxFragment(0);
	Handle hBlock = CPAGE_GetBlockFirst(hCPAGE, TYPE_TEXT);
	if (hBlock) {
		AutoBuffer<int, InitZero> flagfrag(nf);

		for (int i = 0; hBlock && i < nf; i++) {
			flagfrag[i] = CPAGE_GetBlockFlags(hCPAGE, hBlock);
			hBlock = CPAGE_GetBlockNext(hCPAGE, hBlock, TYPE_TEXT);
		}
		for (int i = 1; i <= nf; i++) {
			ln = CSTR_FirstLineFragm(i, 0);
			if (ln) {
				CSTR_GetLineAttr(ln, &attr);
				if (flagfrag[attr.fragment - 1] & CPAGE_BLOCK_USER) {
					attr.Flags |= CSTR_STR_HandFragment;
					CSTR_SetLineAttr(ln, &attr);
				}
				do {
					ln = CSTR_NextLineFragm(ln);
					if (ln) {
						CSTR_GetLineAttr(ln, &attr);
						if (flagfrag[attr.fragment - 1] & CPAGE_BLOCK_USER) {
							attr.Flags |= CSTR_STR_HandFragment;
							CSTR_SetLineAttr(ln, &attr);
						}
					}
				} while (ln);
			}
		}
	}

	pass1();
	pass2();

	spellCorrection();
	recognizeCorrection();

	printResult();

	// OLEG fot Consistent
	if (SPEC_PRJ_CONS == gnSpecialProject) {
		char * buf = &global_buf[0], buf_str[1024];
		char * pb = buf;
		global_buf_len = 0;
		CSTR_line buf_line;
		int count = CSTR_GetMaxNumber();

		for (int i = 1; i <= count; i++) {
			buf_line = CSTR_GetLineHandle(i, 1);
			CSTR_LineToTxt_Coord(buf_line, buf_str, 1023);
			strcpy(pb, buf_str);
			int len = strlen(pb);
			pb += len + 1;
			global_buf_len += len + 1;
		}
		*pb = 0;
		global_buf_len++;

		// OLEG fot Consistent
	}

	normalize();

	if (!LDPUMA_Skip(hDebugCancelFormatted)) {
		LDPUMA_Console("Пропущен этап форматирования.\n");
		return;
	}

	// Отформатируем результат
	formatResult();
}

void PumaImpl::recognizeCorrection() {
	// Скорректируем результат распознавани
	CSTR_SortFragm(1);
	if (LDPUMA_Skip(hDebugCancelPostRecognition)) {
		if (!RCORRKEGL_SetImportData(RCORRKEGL_Bool32_Fax100, &gbFax100)
				|| !RCORRKEGL_CorrectKegl(1))
			throw PumaException("PumaImpl::recognizeCorrection() failed");
	}

	CSTR_SortFragm(1);
	RPSTR_CorrectIncline(1);

	if (!LDPUMA_Skip(hDebugEnableSaveCstr4)) {
		std::string CstrFileName = CIF::replaceFileExt(szInputFileName,
				"_4.cst");
		CSTR_SaveCont(CstrFileName.c_str());
	}
}

void PumaImpl::recognizePass1() {
	// распознавание строк
	for (int i = 1, count = CSTR_GetMaxNumber(); i <= count; i++) {
		CSTR_line lin_out = CSTR_NewLine(i, CSTR_LINVERS_MAINOUT, -1); // OLEG
		if (!lin_out)
			throw PumaException("CSTR_NewLine failed");

		CSTR_line lin_in = CSTR_GetLineHandle(i, CSTR_LINVERS_MAIN);
		if (!lin_in)
			throw PumaException("CSTR_GetLineHandle failed");

		// Recognition
		if (!RSTR_Recog(lin_in, lin_out))
			throw PumaException("RSTR_Recog failed");

#ifdef _USE_REF_    // Nick 23.05.2001
		{
			int numFind = 0;
			REF_findEtaz(lin_in, lin_out, &numFind);
			// есть формулы ?
			if( numFind > 0 ) {
				// новое число строк
				count = CSTR_GetMaxNumber();
				CSTR_DeleteLine( lin_out );
				// изменить номер текущей строки
				RSTR_ChangeLineNumber(-1);
				// перераспознать строку
				i--;
			}
		}
#endif
	}

#ifdef _USE_REF_    // Nick 23.05.2001
	// попробуем найти куски формул
	REF_unionEtaz();
#endif

	if (!RSTR_EndPage(hCPAGE))
		throw PumaException("RSTR_EndPage failed");
}

void PumaImpl::recognizePass2() {
	if (!LDPUMA_Skip(hDebugCancelStringsPass2)) {
		LDPUMA_Console("Пропущен этап второго прохода распознавания.\n");
		return;
	}

	// рапознавание строк
	uchar w8 = 1;
	RSTR_SetImportData(RSTR_Word8_P2_active, &w8);

	for (int i = 1, count = CSTR_GetMaxNumber(); i <= count; i++) {
		CSTR_line lin_out = CSTR_GetLineHandle(i, CSTR_LINVERS_MAINOUT);
		if (!lin_out)
			continue;

		CSTR_line lin_in = CSTR_GetLineHandle(i, CSTR_LINVERS_MAIN);
		if (!lin_in)
			continue;

		// Recognition
		if (!RSTR_Recog(lin_in, lin_out))
			throw PumaException("RSTR_Recog failed");
	}

	if (!RSTR_EndPage(hCPAGE))
		throw PumaException("RSTR_EndPage failed");
}

void PumaImpl::recognizeSetup(int language) {
	// рапознавание строк
	PAGEINFO info;
	GetPageInfo(hCPAGE, &info);
	RSTR_Options opt;
	opt.pageSkew2048 = info.Incline2048;//0
	int32_t nResolutionY = info.DPIY;//300;

	opt.language = language;
	global_buf_len = 0; // OLEG fot Consistent
	if (!RSTR_NewPage(nResolutionY, hCPAGE))
		throw PumaException("RSTR_NewPage failed");

	if (!RSTR_SetOptions(&opt))
		throw PumaException("RSTR_SetOptions failed");

	// Настройка параметров
	uchar w8 = (uchar) gnLanguage;
	RSTR_SetImportData(RSTR_Word8_Language, &w8);

	uint16_t w16 = (uint16_t) info.DPIY;//300;
	RSTR_SetImportData(RSTR_Word16_Resolution, &w16);

	w8 = (uchar) gbFax100;
	RSTR_SetImportData(RSTR_Word8_Fax1x2, &w8);

	w8 = (uchar) gbDotMatrix;
	RSTR_SetImportData(RSTR_Word8_Matrix, &w8);

	w8 = 0;
	RSTR_SetImportData(RSTR_Word8_P2_active, &w8);

	w8 = 1;
	if (!LDPUMA_Skip(hDebugCancelStringsPass2))
		RSTR_SetImportData(RSTR_Word8_P2_disable, &w8);

	w8 = (uchar) gbSpeller;
	RSTR_SetImportData(RSTR_Word8_Spell_check, &w8);

	RSTR_SetImportData(RSTR_pchar_user_dict_name, gpUserDictName);

	// Передать язык в словарный контроль. 12.06.2002 E.P.
	// причем всегда 08.02.2008 DVP
	{
		uchar w8 = (uchar) language;
		RPSTR_SetImportData(RPSTR_FNIMP_LANGUAGE, &w8);
		RCORRKEGL_SetImportData(RCORRKEGL_FNIMP_LANGUAGE, &w8);
	}
}

void PumaImpl::rotate(void * dib, Point32 * p) {
	// Определим угол поворота страницы
	PAGEINFO PInfo;

	assert(p);
	assert(dib);

	if (!CPAGE_GetPageData(hCPAGE, PT_PAGEINFO, (void*) &PInfo, sizeof(PInfo)))
		throw PumaException("CPAGE_GetPageData failed");

	CIMAGEBITMAPINFOHEADER info;
	if (PInfo.BitPerPixel > 1) {
		if (!CIMAGE_GetImageInfo((uchar*) PUMA_IMAGE_BINARIZE, &info))
			throw PumaException("CIMAGE_GetImageInfo failed");

		if (PInfo.Incline2048 > 0) {
			p->x = (int32_t) info.biWidth * PInfo.Incline2048 / 2048
					* PInfo.Incline2048 / 2048;
			p->y = (int32_t)(info.biWidth) * PInfo.Incline2048 / 2048;
		} else {
			p->x = -(int32_t) info.biHeight * PInfo.Incline2048 / 2048
					+ (int32_t) info.biWidth * PInfo.Incline2048 / 2048
							* PInfo.Incline2048 / 2048;
			p->y = 0;
		}
	}

	// Создадим довернутое изображение
	GetPageInfo(hCPAGE, &PInfo);

	CIMAGE_DeleteImage((puchar) PUMA_IMAGE_ROTATE);

	CIMAGE_EnableMask((puchar) PUMA_IMAGE_USER, (puchar) "r", false);
	if (!RIMAGE_Rotate((puchar) PUMA_IMAGE_USER, (puchar) PUMA_IMAGE_ROTATE,
			PInfo.Incline2048, 2048, 0))
		throw PumaException("RIMAGE_Rotate failed");

	if (!CIMAGE_ReadDIB((puchar) PUMA_IMAGE_ROTATE, (void**) dib, true))
		throw PumaException("CIMAGE_ReadDIB failed");

	CIMAGE_EnableMask((puchar) PUMA_IMAGE_USER, (puchar) "r", true);
	PInfo.Images |= IMAGE_ROTATE;
	SetPageInfo(hCPAGE, PInfo);
}

void PumaImpl::rout(const std::string& filename, int Format) const {
	char szName[260];
	strcpy(szName, filename.c_str());
	char * str = strrchr(szName, '.');
	if (str)
		*(str) = '\0';

	if (!ROUT_SetImportData(ROUT_BOOL_PreserveLineBreaks,
			(void*) gnPreserveLineBreaks) || !ROUT_SetImportData(
			ROUT_PCHAR_PageName, szName) || !ROUT_SetImportData(
			ROUT_HANDLE_PageHandle, ghEdPage) || !ROUT_SetImportData(
			ROUT_LONG_Format, (void*) Format) || !ROUT_SetImportData(
			ROUT_LONG_Code, (void*) PUMA_CODE_UTF8) || !ROUT_SetImportData(
			ROUT_PCHAR_BAD_CHAR, &gnUnrecogChar))
		throw PumaException("ROUT_SetImportData failed");

	// Количество объектов
	long countObjects = ROUT_CountObjects();
	if (countObjects == -1)
		return;

	// Цикл по объектам на странице
	for (long objIndex = 1; objIndex <= countObjects; objIndex++) {
		std::string path(filename);

		if (countObjects != 1) {
			path = ROUT_GetDefaultObjectName(objIndex);
			if (!path.empty())
				throw PumaException("ROUT_GetDefaultObjectName failed");
		}

		if (!ROUT_SaveObject(objIndex, path.c_str(), FALSE))
			throw PumaException("ROUT_SaveObject failed");
	}
}

void PumaImpl::rout(void * dest, size_t size, int format) const {
	if (!ROUT_SetImportData(ROUT_BOOL_PreserveLineBreaks,
			(void*) gnPreserveLineBreaks) || !ROUT_SetImportData(
			ROUT_HANDLE_PageHandle, ghEdPage) || !ROUT_SetImportData(
			ROUT_LONG_Format, (void*) format) || !ROUT_SetImportData(
			ROUT_LONG_Code, (void*) PUMA_CODE_UTF8) || !ROUT_SetImportData(
			ROUT_PCHAR_BAD_CHAR, &gnUnrecogChar))
		throw PumaException("ROUT_SetImportData failed");

	// Количество объектов
	long countObjects = ROUT_CountObjects();
	if (countObjects == -1)
		return;

	// Просмотрим размер памяти
	long nSize = 0;
	// Цикл по объектам на странице
	for (long objIndex = 1; objIndex <= countObjects; objIndex++) {
		long nCurSize = ROUT_GetObjectSize(objIndex);
		nSize += nCurSize;
		if (nSize <= (long) size) {
			if (!ROUT_GetObject(objIndex, (uchar*) dest + (nSize - nCurSize),
					&nCurSize))
				throw PumaException("ROUT_GetObject failed");
		}
	}
}

void PumaImpl::save(const std::string& filename, int Format) const {
	if (!ghEdPage)
		throw PumaException("Puma save failed");

#ifndef NDEBUG
	cerr << "Puma save to: " << filename << endl;
#endif

	if (!LDPUMA_Skip(hDebugCancelFormatted))
		return;

	switch (Format) {
	case PUMA_DEBUG_TOTEXT:
		saveToText(filename);
		break;
	case PUMA_TORTF:
		if (!CED_WriteFormattedRtf(filename.c_str(), ghEdPage))
			throw PumaException("Save to RTF failed");
		break;
	case PUMA_TOEDNATIVE:
		if (!CED_WriteFormattedEd(filename.c_str(), ghEdPage))
			throw PumaException("Save to native format failed");
		break;
	case PUMA_TOTEXT:
	case PUMA_TOSMARTTEXT:
	case PUMA_TOTABLETXT:
	case PUMA_TOTABLEDBF:
	case PUMA_TOHTML:
	case PUMA_TOHOCR:
		rout(filename, Format);
		break;
	default: {
		ostringstream os;
		os << "Unknown output format: " << Format;
		throw PumaException(os.str());
	}
	}
}

void PumaImpl::save(void * dest, size_t size, int format) const {
	if (LDPUMA_Skip(hDebugCancelFormatted)) {
		switch (format) {
		case PUMA_TOTEXT:
		case PUMA_TOSMARTTEXT:
		case PUMA_TOTABLETXT:
		case PUMA_TOTABLEDBF:
		case PUMA_TOHTML:
			rout(dest, size, format);
		default: {
			ostringstream os;
			os << "Unknown output format: " << format;
			throw PumaException(os.str());
		}
		}
	}
}

void PumaImpl::saveLayoutToFile(const std::string& fname) {
	CPAGE_ClearBackUp(hCPAGE);
	if (!CPAGE_SavePage(hCPAGE, fname.c_str())) {
		ostringstream os;
		os << "CPAGE_SavePage to '" << fname << "' failed.";
		throw PumaException(os.str());
	}
}

void PumaImpl::savePass1(const std::string& fname) {
	CSTR_SaveCont(fname.c_str());
}

void PumaImpl::saveToText(ostream& os) const {
	for (int i = 1, count = CSTR_GetMaxNumber(); i <= count; i++) {
		CSTR_line lin_out = CSTR_GetLineHandle(i, 1); // OLEG
		if (!lin_out)
			throw PumaException("CSTR_GetLineHandle failed");

		char txt[500];
		if (CSTR_LineToTxt(lin_out, txt))
			os << txt << "\n";
	}
}

void PumaImpl::saveToText(const std::string& filename) const {
	ofstream of(filename.c_str());
	if (!of)
		return;
	saveToText(of);
}

void PumaImpl::setTemplate(const Rect& r) {
	gRectTemplate.left = r.left();
	gRectTemplate.right = r.right();
	gRectTemplate.top = r.top();
	gRectTemplate.bottom = r.bottom();
}

unsigned char * PumaImpl::mainBuffer() {
	return main_buffer_.begin();
}

unsigned char * PumaImpl::workBuffer() {
	return work_buffer_.begin();
}

}
