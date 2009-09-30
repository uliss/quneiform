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
#include "specprj.h"
#include "ligas.h"		// 12.06.2002 E.P.
#include "helper.h"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cassert>
#include <cstring>

using namespace std;

#ifndef NDEBUG
#define DBG(msg) cerr << msg << endl;
#else
#define DBG(msg) ;
#endif

static Handle ghStorage = NULL;

char global_buf[64000]; // OLEG fot Consistent
int32_t global_buf_len = 0; // OLEG fot Consistent

namespace CIF {

static Bool32 rblockProgressStep(uint32_t perc) {
	//	return ProgressStep(2, NULL, perc);
}
static void rblockProgressFinish(void) {
	//	ProgressStep(2, NULL, 100);
}
static Bool32 rselstrProgressStep(uint32_t perc) {
	//	return ProgressStep(2, NULL, perc);
}
static void rselstrProgressFinish(void) {
	//	ProgressStep(2, NULL, 100);
}

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

// Allex
// добавлены для обратной связи из RStuff
Bool32 DPumaSkipComponent(void) {
	return LDPUMA_Skip(hDebugCancelComponent);
}
Bool32 DPumaSkipTurn(void) {
	return LDPUMA_Skip(hDebugCancelTurn);
}

std::string PumaImpl::layout_filename_("layout.bin");

PumaImpl::PumaImpl() {
	modulesInit();
}

PumaImpl::~PumaImpl() {
	modulesDone();
}

void PumaImpl::analyze() {
	layout();
}

void PumaImpl::binarizeImage() {
	gpRecogDIB = gpInputDIB;
	glpRecogName = PUMA_IMAGE_USER;

	if (!CIMAGE_GetImageInfo((puchar) PUMA_IMAGE_USER, &info))
		throw PumaException("PumaImpl: Can't get image info");

#ifndef NDEBUG
	cerr << "The image depth is " << info.biBitCount << " at this point."
			<< endl;
#endif

	if (info.biBitCount > 1) {
		//RIMAGE_BINARISE_KRONROD
		if (!RIMAGE_Binarise((puchar) PUMA_IMAGE_USER,
				(puchar) PUMA_IMAGE_BINARIZE, 4, 0))
			throw PumaException("PumaImpl: Can't binarize image");

		if (!CIMAGE_ReadDIB((puchar) PUMA_IMAGE_BINARIZE,
				(Handle*) &gpRecogDIB, TRUE))
			throw PumaException("PumaImpl: Can't read DIB");

		PageInfo info;
		GetPageInfo(hCPAGE, &info);
		info.Images |= PageInfo::IMAGE_BINARIZE;
		SetPageInfo(hCPAGE, info);
		glpRecogName = PUMA_IMAGE_BINARIZE;
	}
}

void PumaImpl::clearAll() {
	//
	// Сохраним последенне состояние и очистим контейнер
	//
	if (ghEdPage) {
		CED_DeletePage(ghEdPage);
		ghEdPage = NULL;
	}

	PAGEINFO PInfo;
	if (hCPAGE)
		GetPageInfo(hCPAGE, &PInfo);

	CSTR_DeleteAll();
	CPAGE_DeleteAll();
	hCPAGE = CreateEmptyPage();

	strcpy(PInfo.szImageName, PUMA_IMAGE_USER);
	PInfo.Incline2048 = 0;
	PInfo.Angle = 0;
	PInfo.Images = PageInfo::IMAGE_USER;
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
	PAGEINFO info;
	if (!GetPageInfo(hCPAGE, &info))
		throw PumaException("PumaImpl: GetPageInfo failed");
	if (!ExtractComponents(FALSE, NULL, (puchar) info.szImageName))
		throw PumaException("PumaImpl: ExtractComponents failed");
}

void PumaImpl::extractStrings() {
	makeStrings();
	// recognize strings
	CSTR_SortFragm(0);
	recognizeSetup();

	if (LDPUMA_Skip(hDebugCancelRecognition))
		recognizeStrings();
	else
		LDPUMA_Console("Пропущен этап распознавания.\n");
}

void PumaImpl::format() {
	if (!LDPUMA_Skip(hDebugCancelFormatted)) {
		LDPUMA_Console("Пропущен этап форматирования.\n");
		return;
	}

	SetOptionsToFRMT();

	if (ghEdPage) {
		CED_DeletePage(ghEdPage);
		ghEdPage = NULL;
	}

	if (!RFRMT_Formatter(szInputFileName.c_str(), &ghEdPage))
		throw PumaException("PumaImpl: RFRMT_Formatter failed");

	if (!LDPUMA_Skip(hDebugEnablePrintFormatted)) {
		std::string fname(szInputFileName + "_tmp_.rtf");
		SetOptionsToFRMT();
		RFRMT_SaveRtf(fname.c_str(), 8);
		fname = szInputFileName + "_tmp_.fed";
		save(fname.c_str(), PUMA_TOEDNATIVE);
	}
}

void PumaImpl::layout() {
	clearAll();
	binarizeImage();
	normalize();
	pageMarkup();

#ifndef NDEBUG
	fprintf(stderr, "Container CPAGE has:\n");
	fprintf(stderr, " %-20s : %s\n", "name", "size");
	Handle block = CPAGE_GetBlockFirst(hCPAGE, 0);
	while (block) {
		fprintf(stderr, " %-20s : %i\n", CPAGE_GetNameInternalType(
				CPAGE_GetBlockType(hCPAGE, block)), CPAGE_GetBlockData(hCPAGE,
				block, CPAGE_GetBlockType(hCPAGE, block), NULL, 0));
		block = CPAGE_GetBlockNext(hCPAGE, block, 0);
	}
#endif

	SetUpdate(FLG_UPDATE_NO, FLG_UPDATE_CPAGE);
}

void PumaImpl::makeStrings() {
	RBLOCK_SetImportData(RBLOCK_FNRBLOCK_ProgressStep,
			(void*) rblockProgressStep);
	RBLOCK_SetImportData(RBLOCK_FNRBLOCK_ProgressFinish,
			(void*) rblockProgressFinish);

	if (!RBLOCK_ExtractTextStrings(hCCOM, hCPAGE))
		throw PumaException("PumaImpl: extract strings failed");
}

void PumaImpl::modulesDone() {
	ROUT_Done();
	CED_Done();
	if (!LDPUMA_Skip(hDebugEnableSaveJtl)) {
		My_SJTL_close();
	}
	My_SJTL_Done();
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
	CIF::CFIO::CFIO_Done();
}

void PumaImpl::modulesInit() {
	try {
		// CONTEINERS
		if (!CLINE_Init(PUMA_MODULE_CLINE, NULL))
			throw PumaException("CLINE_Init failed");
		if (!CIF::CFIO::CFIO_Init(PUMA_MODULE_CFIO, NULL))
			throw PumaException("CFIO_Init failed");
		if (!CIMAGE_Init(PUMA_MODULE_CIMAGE, NULL))
			throw PumaException("CIMAGE_Init failed");
		//  нужна инициализация контейнера CCOM перед
		//  вызовом поиска компонент
		if (!CCOM_Init(PUMA_MODULE_CCOM, NULL))
			throw PumaException("CCOM_Init failed");
		if (!CPAGE_Init(PUMA_MODULE_CPAGE, ghStorage))
			throw PumaException("CPAGE_Init failed");
		if (!CSTR_Init(PUMA_MODULE_CSTR, ghStorage))
			throw PumaException("CSTR_Init failed");

		// RECOGNITIONS
		// инициализация библиотеки поиска компонент
		if (!REXC_Init(PUMA_MODULE_REXC, NULL))
			throw PumaException("REXC_Init failed");
		if (!RLINE_Init(PUMA_MODULE_RLINE, ghStorage))
			throw PumaException("RLINE_Init failed");
		if (!RRECCOM_Init(PUMA_MODULE_RRECCOM, ghStorage))
			throw PumaException("RRECCOM_Init failed");
		RRECCOM_SetImportData(RRECCOM_OcrPath, GetModulePath());
		if (!RSL_Init(PUMA_MODULE_RSL, ghStorage))
			throw PumaException("RSL_Init failed");
		if (!RSTUFF_Init(PUMA_MODULE_RSTUFF, ghStorage))
			throw PumaException("RSTUFF_Init failed");
		if (!RMARKER_Init(PUMA_MODULE_RBLOCK, ghStorage))
			throw PumaException("RMARKER_Init failed");
		if (!RBLOCK_Init(PUMA_MODULE_RBLOCK, ghStorage))
			throw PumaException("RBLOCK_Init failed");
		if (!RSELSTR_Init(PUMA_MODULE_RBLOCK, ghStorage))
			throw PumaException("RSELSTR_Init failed");
		RSTR_SetImportData(RSTR_OcrPath, GetModulePath());
		RSTR_SetImportData(RSTR_pchar_temp_dir, GetModuleTempPath());
		if (!RSTR_Init(PUMA_MODULE_RSTR, ghStorage))
			throw PumaException("RSTR_Init failed");
		if (!RFRMT_Init(PUMA_MODULE_RFRMT, ghStorage))
			throw PumaException("RFRMT_Init failed");
		if (!RIMAGE_Init(PUMA_MODULE_RIMAGE, ghStorage))
			throw PumaException("RIMAGE_Init failed");

		// Инициализируем виртуальные функции
		if (!RPSTR_Init(PUMA_MODULE_RPSTR, ghStorage))
			throw PumaException("RPSTR_Init failed");
		if (!RPIC_Init(PUMA_MODULE_RPIC, ghStorage))
			throw PumaException("RPIC_Init failed");
		if (!CED_Init(PUMA_MODULE_CED, ghStorage))
			throw PumaException("CED_Init failed");
		if (!ROUT_Init(PUMA_MODULE_ROUT, ghStorage))
			throw PumaException("CSTR_Init failed");
		if (!ROUT_LoadRec6List("rec6all.dat"))
			throw PumaException("ROUT_LoadRec6List failed");

#ifdef _USE_RVERLINE_
		if(!RVERLINE_Init(PUMA_MODULE_RVERLINE, ghStorage))
		throw PumaException("RVERLINE_Init failed");
#endif

#ifdef _USE_RMSEGMENT_
		if(!RMSEGMENT_Init(PUMA_MODULE_RMSEGMENT, ghStorage))
		throw PumaException("RMSEGMENT_Init failed");
#endif

		if (!RCORRKEGL_Init(PUMA_MODULE_RCORRKEGL, ghStorage))
			throw PumaException("RCORRKEGL_Init failed");

		My_SJTL_Init();
		if (!LDPUMA_Skip(hDebugEnableSaveJtl)) {
			My_SJTL_open("one.frm", "one.jtl");
		}
	} catch (PumaException& e) {
		modulesDone();
		throw PumaException(string("PumaImpl::moduleInit failed") + e.what());
	}
}

void PumaImpl::normalize() {
	RSCBProgressPoints CBforRS;
	RSPreProcessImage DataforRS;

#define SET_CB(a,b)   a.p##b = (void*)b
	CBforRS.pProgressStep = (void*) ProgressStepLayout;
	CBforRS.pProgressStepLines = (void*) ProgressStepLines;
	CBforRS.pProgressStepTables = (void*) ProgressStepTables;
	SET_CB(CBforRS, InitPRGTIME);
	SET_CB(CBforRS, StorePRGTIME);
	SET_CB(CBforRS, RestorePRGTIME);
	SET_CB(CBforRS, DonePRGTIME);
	SET_CB(CBforRS, rexcProgressStep);
	SET_CB(CBforRS, DPumaSkipComponent);
	SET_CB(CBforRS, DPumaSkipTurn);
	//	CBforRS.pSetReturnCode = (void*) SetReturnCode_puma;
	SET_CB(CBforRS, GetModulePath);
	SET_CB(CBforRS, SetUpdate);
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
	DataforRS.szLayoutFileName = layout_filename_;
	DataforRS.hDebugEnableSearchSegment = hDebugEnableSearchSegment;

	// калбэки
	if (RSTUFF_SetImportData(RSTUFF_FN_SetProgresspoints, &CBforRS)) {
		///нормализуем - обработка, поиск картинок, поиск линий
		if (!RSTUFF_RSNormalise(&DataforRS, main_buffer_.get(),
				main_buffer_.size(), work_buffer_.get(), work_buffer_.size()))
			throw PumaException("PumaImpl: normalize failed.");
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
}

void PumaImpl::pageMarkup() {
	RMCBProgressPoints CBforRM;
	RMPreProcessImage DataforRM;

#define SET_CB(a,b)   a.p##b = (void*)b
	//		SET_CB(CBforRM, ProgressStart);
	CBforRM.pProgressStepAutoLayout = (void*) ProgressStepAutoLayout;
	CBforRM.pProgressStepSearchTables = (void*) ProgressStepSearchTables;
	//		SET_CB(CBforRM, ProgressFinish);
	SET_CB(CBforRM, InitPRGTIME);
	SET_CB(CBforRM, StorePRGTIME);
	SET_CB(CBforRM, RestorePRGTIME);
	SET_CB(CBforRM, DonePRGTIME);
	SET_CB(CBforRM, rexcProgressStep);
	SET_CB(CBforRM, DPumaSkipComponent);
	SET_CB(CBforRM, DPumaSkipTurn);
	//	CBforRM.pSetReturnCode = (void*) SetReturnCode_puma;
	SET_CB(CBforRM, GetModulePath);
	SET_CB(CBforRM, SetUpdate);
#undef SET_CB

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
	DataforRM.szLayoutFileName = layout_filename_;
	DataforRM.hDebugEnableSearchSegment = hDebugEnableSearchSegment;

	if (RMARKER_SetImportData(0, &CBforRM)) {
		if (!RMARKER_PageMarkup(&DataforRM, main_buffer_.get(),
				main_buffer_.size(), work_buffer_.get(), work_buffer_.size()))
			throw PumaException("PumaImpl: RMARKER_PageMarkup failed");

		hCPAGE = DataforRM.hCPAGE;
	}
}

void PumaImpl::preOpenInitialize() {
	PumaImpl::close();
	SetUpdate(FLG_UPDATE, FLG_UPDATE_NO);
}

void PumaImpl::postOpenInitialize() {
	CIMAGEBITMAPINFOHEADER info;
	if (!CIMAGE_GetImageInfo((puchar) PUMA_IMAGE_USER, &info))
		throw PumaException("PumaImpl: CIMAGE_GetImageInfo failed.");

	gRectTemplate.left = 0;
	gRectTemplate.right = info.biWidth;
	gRectTemplate.top = 0;
	gRectTemplate.bottom = info.biHeight;

	szInputFileName = "none.txt";
	layout_filename_ = "none.bin";

	hCPAGE = CreateEmptyPage();
}

void PumaImpl::readLayoutFromFile(const std::string& fname) {
	hCPAGE = CPAGE_RestorePage(TRUE, fname.c_str());

	if (hCPAGE == NULL)
		throw PumaException("PumaImpl: CPAGE_RestorePage failed: " + fname);

	CPAGE_SetCurrentPage(CPAGE_GetNumberPage(hCPAGE));
	LDPUMA_Console("Layout восстановлен из файла '%s'\n", fname.c_str());
}

void PumaImpl::recognize() {
	// Проверим: выделены ли фрагменты.
	if (!CPAGE_GetCountBlock(hCPAGE) || IsUpdate(FLG_UPDATE_CPAGE))
		layout();

	CSTR_DeleteAll();

	if (hCPAGE)
		CPAGE_UpdateBlocks(hCPAGE, TYPE_CPAGE_TABLE);

	if (!LDPUMA_Skip(hDebugLayoutToFile))
		saveLayoutToFile(layout_filename_);

	if (!LDPUMA_Skip(hDebugLayoutFromFile))
		readLayoutFromFile(layout_filename_);

	if (IsUpdate(FLG_UPDATE_CCOM)) {
		if (LDPUMA_Skip(hDebugCancelComponent))
			extractComponents();
		else
			LDPUMA_Console("Пропущен этап выделения компонент.\n");
	}

	// get page descriptor
	hCPAGE = CPAGE_GetHandlePage(CPAGE_GetCurrentPage());

	// Выделим строки
	if (LDPUMA_Skip(hDebugCancelStrings))
		extractStrings();
	else
		LDPUMA_Console("Пропущен этап выделения строк.\n");
}

void PumaImpl::recognizeSetup() {
	// рапознавание строк
	PAGEINFO info;
	GetPageInfo(hCPAGE, &info);

	RSTR_Options opt;
	opt.pageSkew2048 = info.Incline2048;//0
	int32_t nResolutionY = info.DPIY;//300;

	opt.language = gnLanguage;
	global_buf_len = 0; // OLEG fot Consistent
	if (!RSTR_NewPage(nResolutionY, hCPAGE))
		throw PumaException("PumaImpl: RSTR_NewPage failed");

	if (!RSTR_SetOptions(&opt))
		throw PumaException("PumaImpl: RSTR_SetOptions failed");

	// setting parameters
	uchar w8 = gnLanguage;
	RSTR_SetImportData(RSTR_Word8_Language, &w8);

	short w16 = info.DPIY;//300;
	RSTR_SetImportData(RSTR_Word16_Resolution, &w16);

	w8 = gbFax100;
	RSTR_SetImportData(RSTR_Word8_Fax1x2, &w8);

	w8 = gbDotMatrix;
	RSTR_SetImportData(RSTR_Word8_Matrix, &w8);

	w8 = 0;
	RSTR_SetImportData(RSTR_Word8_P2_active, &w8);

	w8 = 1;
	if (!LDPUMA_Skip(hDebugCancelStringsPass2))
		RSTR_SetImportData(RSTR_Word8_P2_disable, &w8);

	w8 = gbSpeller;
	RSTR_SetImportData(RSTR_Word8_Spell_check, &w8);

	RSTR_SetImportData(RSTR_pchar_user_dict_name, gpUserDictName);

	// Передать язык в словарный контроль. 12.06.2002 E.P.
	// причем всегда 08.02.2008 DVP
	{
		uchar w8 = gnLanguage;
		RPSTR_SetImportData(RPSTR_FNIMP_LANGUAGE, &w8);
		RCORRKEGL_SetImportData(RCORRKEGL_FNIMP_LANGUAGE, &w8);
	}
}

void PumaImpl::recognizeStrings() {
	CSTR_SortFragm(0);
	CSTR_line ln;
	CSTR_attr attr;
	Handle hBlock = CPAGE_GetBlockFirst(hCPAGE, TYPE_TEXT);
	if (hBlock) {
		int nf = CSTR_GetMaxFragment(0);
		AutoArrayZ<int> flagfrag(nf);

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

	recognizeStringsPass1();

	// OLEG : 01-05-18 : for GiP
	if (SPEC_PRJ_GIP == gnSpecialProject && gnLanguage == LANG_RUSENG)
		specialProject();

	recognizeStringsPass2();

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
			throw PumaException("PumaImpl: RPSTR_CorrectSpell failed");
	}
	// Скорректируем результат распознавани
	CSTR_SortFragm(1);
	if (LDPUMA_Skip(hDebugCancelPostRecognition)) {
		if (!RCORRKEGL_SetImportData(RCORRKEGL_Bool32_Fax100, &gbFax100)
				|| !RCORRKEGL_CorrectKegl(1))
			throw PumaException("PumaImpl: kecl correction failed");
	}
	CSTR_SortFragm(1);
	RPSTR_CorrectIncline(1);

	if (!LDPUMA_Skip(hDebugEnableSaveJtl) && !szInputFileName.empty()) {
		if (My_SJTL_save(szInputFileName.c_str()) == -1) {
			My_SJTL_open("one.frm", "one.jtl");
			My_SJTL_save(szInputFileName.c_str());
		}
	}

	if (!LDPUMA_Skip(hDebugEnableSaveCstr4)) {
		std::string CstrFileName = CIF::replaceFileExt(szInputFileName,
				"_4.cst");
		CSTR_SaveCont(CstrFileName.c_str());
	}

	// Печать результатов в консоль
	int count = CSTR_GetMaxNumber();
	if (LDPUMA_Skip(hDebugCancelConsoleOutputText) && LDPUMA_IsActive()) {
		for (int i = 1; i <= count; i++) {
			PrintResult(i, CSTR_GetLineHandle(i, CSTR_LINVERS_MAINOUT), hCPAGE);
		}
	}

	if (SPEC_PRJ_CONS == gnSpecialProject) {
		char buf_str[1024];
		char * pb = &global_buf[0];
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
	}

	//Нормализуем вертикальные строки
	// normalize vertical strings
	RPSTR_NormalizeVertStr();

	format();
}

void PumaImpl::recognizeStringsPass1() {
	if (!LDPUMA_Skip(hDebugEnableSaveCstr1)) {
		std::string CstrFileName = CIF::replaceFileExt(szInputFileName,
				"_1.cst");
		CSTR_SaveCont(CstrFileName.c_str());
	}

	// распознавание строк
	int count = CSTR_GetMaxNumber();
	LDPUMA_StartLoop(hDebugRecognition, count);

	for (int i = 1; i <= count; i++) {
		LDPUMA_LoopNext(hDebugRecognition);
		if (!LDPUMA_Skip(hDebugRecognition)) {
			LDPUMA_Console("Остановились перед %i строкой.\n", i);
			LDPUMA_Console("Нажмите любую клавишу...\n");
			LDPUMA_WaitUserInput(hDebugRecognition, NULL);
		}

		CSTR_line lin_out = CSTR_NewLine(i, CSTR_LINVERS_MAINOUT, -1); // OLEG
		if (!lin_out)
			throw PumaException("PumaImpl: CSTR_NewLine faild.");

		CSTR_line lin_in = CSTR_GetLineHandle(i, CSTR_LINVERS_MAIN);
		if (!lin_in)
			throw PumaException("PumaImpl: CSTR_GetLineHandle faild.");

		// Recognition
		if (!RSTR_Recog(lin_in, lin_out))
			throw PumaException("PumaImpl: RSTR_Recog faild.");

#ifdef _USE_REF_
		{
			int numFind = 0;
			REF_findEtaz(lin_in, lin_out, &numFind);
			// есть формулы ?
			// formulas found?
			if(numFind > 0) {
				// новое число строк
				// new count of strings
				count = CSTR_GetMaxNumber();
				CSTR_DeleteLine(lin_out);

				// изменить номер текущей строки
				// change current string number
				RSTR_ChangeLineNumber(-1);
				// перераспознать строку
				// recognize again
				i--;
			}
		}
#endif
	}

#ifdef _USE_REF_
	// trying to find piece of formulas
	REF_unionEtaz();
#endif

	if (!RSTR_EndPage(hCPAGE))
		throw PumaException("PumaImpl: RSTR_EndPage failed");
}

void PumaImpl::recognizeStringsPass2() {
	if (!LDPUMA_Skip(hDebugEnableSaveCstr2)) {
		std::string CstrFileName = CIF::replaceFileExt(szInputFileName,
				"_2.cst");
		CSTR_SaveCont(CstrFileName.c_str());
	}

	if (!RSTR_NeedPass2()) {
		LDPUMA_Console("RSTR thinks that second pass don't needed.\n");
		return;
	}

	if (!LDPUMA_Skip(hDebugCancelStringsPass2)) {
		LDPUMA_Console("Skipped second pass of recognition\n");
		return;
	}

	uchar w8 = 1;
	RSTR_SetImportData(RSTR_Word8_P2_active, &w8);

	int count = CSTR_GetMaxNumber();

	LDPUMA_StartLoop(hDebugRecognition, count);

	for (int i = 1; i <= count; i++) {
		LDPUMA_LoopNext(hDebugRecognition);
		if (!LDPUMA_Skip(hDebugRecognition)) {
			LDPUMA_Console("Остановились перед %i строкой.\n", i);
			LDPUMA_Console("Нажмите любую клавишу...\n");
			LDPUMA_WaitUserInput(hDebugRecognition, NULL);
		}

		CSTR_line lin_out = CSTR_GetLineHandle(i, CSTR_LINVERS_MAINOUT);
		if (!lin_out)
			continue;

		CSTR_line lin_in = CSTR_GetLineHandle(i, CSTR_LINVERS_MAIN);
		if (!lin_in)
			continue;

		// Recognition
		if (!RSTR_Recog(lin_in, lin_out))
			throw PumaException("PumaImpl: RSTR_Recog failed");
	}

	if (!RSTR_EndPage(hCPAGE))
		throw PumaException("PumaImpl: RSTR_EndPage failed");
}

void PumaImpl::save(const std::string& filename, int Format) const {
#ifndef NDEBUG
	cerr << "Puma save to: " << filename << endl;
#endif

	if (NULL == ghEdPage)
		throw PumaException("PumaImpl: save failed");

	if (LDPUMA_Skip(hDebugCancelFormatted)) {
		switch (Format) {
		case PUMA_DEBUG_TOTEXT:
			saveToText(filename);
			break;
		case PUMA_TORTF:
			if (!CED_WriteFormattedRtf(filename.c_str(), ghEdPage))
				throw PumaException("PumaImpl: Save to RTF failed");
			break;
		case PUMA_TOEDNATIVE:
			if (!CED_WriteFormattedEd(filename.c_str(), ghEdPage))
				throw PumaException("PumaImpl: Save to EDNATIVE failed");
		case PUMA_TOTEXT:
		case PUMA_TOSMARTTEXT:
		case PUMA_TOTABLETXT:
		case PUMA_TOTABLEDBF:
		case PUMA_TOHTML:
		case PUMA_TOHOCR:
			if (!ConverROUT(filename.c_str(),
					static_cast<puma_format_t> (Format), PUMA_CODE_UTF8, false))
				throw PumaException("PumaImpl: Save to HOCR failed");
			break;
		default:
			throw PumaException("Pumaimpl::save unknown format");
		}
	}
}

void PumaImpl::saveLayoutToFile(const std::string& fname) {
#ifdef NOSAVEBACKUP
	CPAGE_ClearBackUp(hCPAGE);
#endif
	if (!CPAGE_SavePage(hCPAGE, fname.c_str()))
		throw PumaException("CPAGE_SavePage failed" + fname);

	LDPUMA_Console("Layout saved in file '%s'\n", fname.c_str());
}

void PumaImpl::saveToText(const std::string& filename) const {
	ofstream of(filename.c_str());
	if (!of)
		throw PumaException("PumaImpl::saveToText() can't open file: "
				+ filename);

	for (int i = 1, count = CSTR_GetMaxNumber(); i <= count; i++) {
		CSTR_line lin_out = CSTR_GetLineHandle(i, 1);
		if (!lin_out)
			throw PumaException("PumaImpl::saveToText() failed");

		char txt[500];
		if (CSTR_LineToTxt(lin_out, txt))
			of << txt << endl;
		else
			throw PumaException("PumaImpl::saveToText() CSTR_LineToTxt failed");
	}
}

void PumaImpl::specialProject() {
	int n = CSTR_GetMaxNumber();
	double s = 0.0;
	CSTR_line lin_ruseng;
	for (int i = 1; i <= n; i++) {
		lin_ruseng = CSTR_GetLineHandle(i, 1);
		s += portion_of_rus_letters(lin_ruseng);
	}
	if (n)
		s /= (double) n;

	// TODO Hard-coded coefficient
	if (n && s < 0.4) {
		for (int i = 0; i <= n; i++) {
			// TODO Hard-coded constant 10
			for (int j = 1; j < 10; j++) {
				lin_ruseng = CSTR_GetLineHandle(i, j);
				if (lin_ruseng)
					CSTR_DeleteLine(lin_ruseng);
			}
		}

		gnLanguage = LANG_ENGLISH;
		recognizeSetup();
		recognizeStringsPass1();
	}
}

}

