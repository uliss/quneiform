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

unsigned char * PumaImpl::main_buffer_ = 0;
unsigned char * PumaImpl::work_buffer_ = 0;

static int32_t s_ConsoleLine = 0;

PumaImpl::PumaImpl() {
	initMainBuffer();
	initWorkBuffer();
	if (!ModulesInit(ghStorage))
		throw PumaException("Puma init failed");
}

PumaImpl::~PumaImpl() {
	modulesDone();
	freeMainBuffer();
	freeWorkBuffer();
}

void PumaImpl::analyze() {
	layout();
}

void PumaImpl::close() {
	DBG("Puma close")
	CLINE_Reset();
	ClearAll();
	// clean
	CIMAGE_Reset();
	CPAGE_DeleteAll();
	RIMAGE_Reset();
	hCPAGE = NULL;
	gpRecogDIB = gpInputDIB = NULL;
}

void PumaImpl::freeMainBuffer() {
	if (!main_buffer_)
		return;
	delete[] main_buffer_;
	main_buffer_ = NULL;
}

void PumaImpl::freeWorkBuffer() {
	if (!work_buffer_)
		return;
	delete[] work_buffer_;
	work_buffer_ = NULL;
}

void PumaImpl::initMainBuffer() {
	if (main_buffer_) {
		DBG("PumaImpl: main buffer is not empty")
		delete[] main_buffer_;
	}
	main_buffer_ = new unsigned char[MainBufferSize];
}

void PumaImpl::initWorkBuffer() {
	if (work_buffer_) {
		DBG ("PumaImpl: work buffer is not empty")
		delete[] work_buffer_;
	}
	work_buffer_ = new uchar[WorkBufferSize];
}


static Bool32 rblockProgressStep(uint32_t perc) {
	return ProgressStep(2, NULL, perc);
}
static void rblockProgressFinish(void) {
	ProgressStep(2, NULL, 100);
}
/////////////////////////////////////////////////////////////////////////
// Allex
// добавлены для обратной связи из RStuff
Bool32 DPumaSkipComponent(void) {
	return LDPUMA_Skip(hDebugCancelComponent);
}
Bool32 DPumaSkipTurn(void) {
	return LDPUMA_Skip(hDebugCancelTurn);
}

void PumaImpl::layout() {
	Bool32 rc = TRUE;
	int nBlock = 0;
	int32_t Nominator = 0; // числитель
	int32_t Denominator = 0; // знаменатель определяемого угла поворота

	RSCBProgressPoints CBforRS;
	RSPreProcessImage DataforRS;

	RMCBProgressPoints CBforRM;
	RMPreProcessImage DataforRM;

	PAGEINFO PInfo = { 0 };

	ClearAll();

	void* MemBuf = CIF::PumaImpl::mainBuffer();
	size_t size_buf = CIF::PumaImpl::MainBufferSize;
	void* MemWork = CIF::PumaImpl::workBuffer();
	int size_work = CIF::PumaImpl::WorkBufferSize;

	rc = BinariseImage();

	if (rc) {

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
			rc = RSTUFF_RSNormalise(&DataforRS, MemBuf, size_buf, MemWork,
					size_work);

			if (!rc) {
				SetReturnCode_puma(RSTUFF_GetReturnCode());
				rc = FALSE;
			}
		}
	}

	// Gleb 02.11.2000
	// Далее - разметка. Вынесена в RMARKER.DLL
	if (rc) {
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
			rc = RMARKER_PageMarkup(&DataforRM, MemBuf, size_buf, MemWork,
					size_work);

			if (!rc) {
				SetReturnCode_puma(RMARKER_GetReturnCode());
			} else
				hCPAGE = DataforRM.hCPAGE; //Paul 25-01-2001

		}
	}

	//
	// Запустим отладчик для редактирования Layout
	//

	if (rc && !LDPUMA_Skip(hDebugHandLayout)) {
		// Покажем довернутое изображение
		Handle hRotateDIB = NULL;
		Point32 p = { 0 };

		if (PUMA_XGetRotateDIB(&hRotateDIB, &p) == FALSE) {
			LDPUMA_Console(
					"ПРЕДУПРЕЖДЕНИЕ: <%s>.\
	Для показа в Layout будет использовано не повернутое изображение.\n",
					PUMA_GetReturnString(PUMA_GetReturnCode()));
			PAGEINFO PInfo = { 0 };
			GetPageInfo(hCPAGE, &PInfo);
			CIMAGE_ReadDIB((puchar) PInfo.szImageName, (Handle*) &hRotateDIB,
					TRUE);
		}
		LDPUMA_HandLayout(hRotateDIB, 0, &p);
		CIMAGE_DeleteImage((puchar) PUMA_IMAGE_ROTATE);
	}
	//
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

	if (rc)
		SetUpdate(FLG_UPDATE_NO, FLG_UPDATE_CPAGE);

	if (!rc)
		throw PumaException("PUma layout failed");
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

void PumaImpl::open(char * dib) {
	DBG("Puma open")
	assert(dib);
	preOpenInitialize();
	gpInputDIB = (unsigned char*) dib;
	// write image
	if (!CIMAGE_WriteDIB((puchar) PUMA_IMAGE_USER, dib, 1))
		throw PumaException("PumaImpl::open can't write DIB");

	if (!postOpenInitialize("none.txt"))
		throw PumaException("Puma post open init failed");
}

bool PumaImpl::preOpenInitialize() {
	bool rc = true;
	// Удалим предыдущие окна отладки.
	Handle hRemWnd = LDPUMA_GetWindowHandle(NAME_IMAGE_DELLINE);
	if (hRemWnd)
		LDPUMA_DestroyWindow(hRemWnd);
	hRemWnd = LDPUMA_GetWindowHandle(NAME_IMAGE_BINARIZE);
	if (hRemWnd)
		LDPUMA_DestroyWindow(hRemWnd);
	hRemWnd = LDPUMA_GetWindowHandle(NAME_IMAGE_INPUT);
	if (hRemWnd)
		LDPUMA_DestroyWindow(hRemWnd);
	hRemWnd = LDPUMA_GetWindowHandle(PUMA_IMAGE_TURN);
	if (hRemWnd)
		LDPUMA_DestroyWindow(hRemWnd);
	hRemWnd = LDPUMA_GetWindowHandle(NAME_IMAGE_ORTOMOVE);
	if (hRemWnd)
		LDPUMA_DestroyWindow(hRemWnd);

	PumaImpl::close();
	ResetPRGTIME();
	if (LDPUMA_Skip(hDebugRoot)) {
		if (s_ConsoleLine)
			LDPUMA_ConsoleClear(s_ConsoleLine);
		s_ConsoleLine = LDPUMA_ConsoleGetCurLine();
	} else {

	}
	SetUpdate(FLG_UPDATE, FLG_UPDATE_NO);
	SetReturnCode_puma(IDS_ERR_NO);
	return rc;
}

bool PumaImpl::postOpenInitialize(const char * lpFileName) {
	bool rc = true;
	CIMAGEBITMAPINFOHEADER info;
	if (lpFileName)
		LDPUMA_SetFileName(NULL, lpFileName);
	if (!CIMAGE_GetImageInfo((puchar) PUMA_IMAGE_USER, &info)) {
		SetReturnCode_puma(CIMAGE_GetReturnCode());
		rc = false;
	} else {
		gRectTemplate.left = 0;
		gRectTemplate.right = info.biWidth;
		gRectTemplate.top = 0;
		gRectTemplate.bottom = info.biHeight;
	}
	if (lpFileName) {
		szInputFileName = lpFileName;
		strcpy(szLayoutFileName, lpFileName);
		char * s = strrchr(szLayoutFileName, '.');
		if (s)
			*s = 0;
		strcat(szLayoutFileName, ".bin");
	} else
		szInputFileName = "";
	hCPAGE = CreateEmptyPage();
	return rc;
}

void PumaImpl::recognize() {
	// Проверим: выделены ли фрагменты.
	if (!CPAGE_GetCountBlock(hCPAGE) || IsUpdate(FLG_UPDATE_CPAGE))
		layout();
	if (!Recognize())
		throw PumaException("Puma recognition failed");

	DBG("Puma recognize")
}

void PumaImpl::save(const std::string& filename, int Format) const {
#ifndef NDEBUG
	cerr << "Puma save to: " << filename << endl;
#endif

	if (!PUMA_XSave(filename, static_cast<puma_format_t> (Format),
			PUMA_CODE_UTF8))
		throw PumaException("Puma save failed");
}

unsigned char * PumaImpl::mainBuffer() {
	assert(main_buffer_);
	return main_buffer_;
}

unsigned char * PumaImpl::workBuffer() {
	assert(work_buffer_);
	return work_buffer_;
}

}
