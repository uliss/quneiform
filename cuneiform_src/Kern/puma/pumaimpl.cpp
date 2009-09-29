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

namespace CIF {

std::string szLayoutFileName("Layout.bin");

PumaImpl::PumaImpl() {
	if (!ModulesInit(ghStorage))
		throw PumaException("Puma init failed");
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
	cerr << "The image depth is " << info.biBitCount << "at this point."
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
	CBforRS.pSetReturnCode = (void*) SetReturnCode_puma;
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
	DataforRS.szLayoutFileName = szLayoutFileName;
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
	CBforRM.pSetReturnCode = (void*) SetReturnCode_puma;
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
	DataforRM.szLayoutFileName = szLayoutFileName;
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
	szLayoutFileName = "none.bin";

	hCPAGE = CreateEmptyPage();
}

void PumaImpl::recognize() {
	// Проверим: выделены ли фрагменты.
	if (!CPAGE_GetCountBlock(hCPAGE) || IsUpdate(FLG_UPDATE_CPAGE))
		layout();
	if (!Recognize())
		throw PumaException("Puma recognition failed");
}

void PumaImpl::save(const std::string& filename, int Format) const {
#ifndef NDEBUG
	cerr << "Puma save to: " << filename << endl;
#endif

	if (!PUMA_XSave(filename, static_cast<puma_format_t> (Format),
			PUMA_CODE_UTF8))
		throw PumaException("Puma save failed");
}
}
