#include "puma.h"
#include "module.h"

#include "mpuma.h"
#include "cpage.h"
#include "cline.h"
#include "criimage.h"
#include "ctiimage.h"

#include <iostream>
#include <cstring>

using namespace std;

namespace CIF {

string Puma::module_tmp_path_(".");
string Puma::module_path_(".");

Puma::Puma() :
	buffer_(NULL), work_mem_(NULL), dib_(NULL) {
	buffer_ = new unsigned char[BufferSize];
	work_mem_ = new unsigned char[WorkMemSize];
	modules_init();
}

Puma::~Puma() {
	close();
	modules_done();

	if (buffer_)
		delete[] buffer_;

	if (work_mem_)
		delete[] work_mem_;
}

void Puma::close() {
	CLINE_Reset();

	PAGEINFO PInfo = { 0 };
	//
	// Сохраним последенне состояние и очистим контейнер
	//
	if (ed_page_) {
		CED_DeletePage(ed_page_);
		ed_page_ = NULL;
	}

	if (page_)
		GetPageInfo(page_, &PInfo);

	CSTR_DeleteAll();
	CPAGE_DeleteAll();
	page_ = CreateEmptyPage();

	strcpy((char*) PInfo.szImageName, PUMA_IMAGE_USER);
	PInfo.Incline2048 = 0;
	PInfo.Angle = 0;
	PInfo.Images = IMAGE_USER;
	SetPageInfo(page_, PInfo);

	CCOM_DeleteAll();
	ccom_ = NULL;

	CIMAGE_DeleteImage((uchar*) PUMA_IMAGE_BINARIZE);
	CIMAGE_DeleteImage((uchar*) PUMA_IMAGE_DELLINE);
	//  Повернутое изображение ( PUMA_IMAGE_ROTATE) удалять нельзя, как и исходное,
	//  поскольку оно отображается в интерфейсе. Его нужно удалять
	//  либо при получении нового довернутого изображения, либо при
	//  закрытии файла
	CIMAGE_DeleteImage((unsigned char*) PUMA_IMAGE_TURN);

	// clean
	CIMAGE_Reset();
	CPAGE_DeleteAll();
	RIMAGE_Reset();
	page_ = NULL;

	recog_dib_ = NULL;
	input_dib_ = NULL;
}

void Puma::modules_done() {
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
#endif

#ifdef _USE_RMSEGMENT_
	RMSEGMENT_Done();
#endif
	CIF::CFIO::CFIO_Done();
}

void Puma::modules_init() {
	try {
		// CONTEINERS
		if (!CLINE_Init(PUMA_MODULE_CLINE, NULL))
			throw ModuleInitException("CLINE", CLINE_GetReturnCode());

		if (!CFIO::CFIO_Init(PUMA_MODULE_CFIO, NULL))
			throw ModuleInitException("CFIO", CFIO::CFIO_GetReturnCode());

		if (!CIMAGE_Init(PUMA_MODULE_CIMAGE, NULL))
			throw ModuleInitException("CIMAGE", CIMAGE_GetReturnCode());

		//  нужна инициализация контейнера CCOM перед
		//  вызовом поиска компонент
		if (!CCOM_Init(PUMA_MODULE_CCOM, NULL))
			throw ModuleInitException("CCOM", CCOM_GetReturnCode());

		if (!CPAGE_Init(PUMA_MODULE_CPAGE, NULL))
			throw ModuleInitException("CPAGE", CPAGE_GetReturnCode());

		if (!CSTR_Init(PUMA_MODULE_CSTR, NULL))
			throw ModuleInitException("CSTR", CSTR_GetReturnCode());

		// RECOGNITIONS
		// инициализация библиотеки поиска компонент
		if (!REXC_Init(PUMA_MODULE_REXC, NULL))
			throw ModuleInitException("REXC", REXC_GetReturnCode());

		if (!RLINE_Init(PUMA_MODULE_RLINE, NULL))
			throw ModuleInitException("RLINE", RLINE_GetReturnCode());

		if (!RRECCOM_Init(PUMA_MODULE_RRECCOM, NULL))
			throw ModuleInitException("RRECOM", RRECCOM_GetReturnCode());

		RRECCOM_SetImportData(RRECCOM_OcrPath, module_path().c_str());

		if (!RSL_Init(PUMA_MODULE_RSL, NULL))
			throw ModuleInitException("RSL", RSL_GetReturnCode());

		if (!RSTUFF_Init(PUMA_MODULE_RSTUFF, NULL))
			throw ModuleInitException("RSTUFF", RSTUFF_GetReturnCode());

		if (!RMARKER_Init(PUMA_MODULE_RBLOCK, NULL))
			throw ModuleInitException("RMARKER", RMARKER_GetReturnCode());

		if (!RBLOCK_Init(PUMA_MODULE_RBLOCK, NULL))
			throw ModuleInitException("RBLOCK", RBLOCK_GetReturnCode());

		if (!RSELSTR_Init(PUMA_MODULE_RBLOCK, NULL))
			throw ModuleInitException("RSELSTR", RSELSTR_GetReturnCode());

		RSTR_SetImportData(RSTR_OcrPath, module_path().c_str());
		RSTR_SetImportData(RSTR_pchar_temp_dir, module_tmp_path().c_str());

		if (!RSTR_Init(PUMA_MODULE_RSTR, NULL))
			throw ModuleInitException("RSTR", RSTR_GetReturnCode());

		if (!RFRMT_Init(PUMA_MODULE_RFRMT, NULL))
			throw ModuleInitException("RFRMT", RFRMT_GetReturnCode());

		if (!RIMAGE_Init(PUMA_MODULE_RIMAGE, NULL))
			throw ModuleInitException("RIMAGE", RIMAGE_GetReturnCode());

		// Инициализируем виртуальные функции
		if (!RPSTR_Init(PUMA_MODULE_RPSTR, NULL))
			throw ModuleInitException("RPSTR", RPSTR_GetReturnCode());

		if (!RPIC_Init(PUMA_MODULE_RPIC, NULL))
			throw ModuleInitException("RPIC", RPIC_GetReturnCode());

		if (!CED_Init(PUMA_MODULE_CED, NULL))
			throw ModuleInitException("CED", CED_GetReturnCode());

		if (!ROUT_Init(PUMA_MODULE_ROUT, NULL))
			throw ModuleInitException("ROUT", ROUT_GetReturnCode());
		else if (!ROUT_LoadRec6List("rec6all.dat"))
			throw ModuleInitException("ROUT", ROUT_GetReturnCode());

#ifdef _USE_RVERLINE_
		if(!RVERLINE_Init(PUMA_MODULE_RVERLINE,NULL))
		throw ModuleInitException("RVERLINE", RVERLINE_GetReturnCode());
#endif

#ifdef _USE_RMSEGMENT_
		if(!RMSEGMENT_Init(PUMA_MODULE_RMSEGMENT,NULL))
		throw ModuleInitException("RMSEGMENT", RMSEGMENT_GetReturnCode());
#endif

		if (!RCORRKEGL_Init(PUMA_MODULE_RCORRKEGL, NULL))
			throw ModuleInitException("RCORRKEGL", RCORRKEGL_GetReturnCode());

	} catch (ModuleInitException& e) {
		cerr << "Init modules failed: " << e.what() << "; error code: "
				<< e.return_code() << endl;
		//ModulesDone(NULL);
	}
}

string Puma::module_path() {
	return module_path_;
}

string Puma::module_tmp_path() {
	return module_tmp_path_;
}

void Puma::open(char * dib, const string& filename) {
	close();

	if (!dib)
		throw PumaException("Puma::open: invalid dib pointer given");
	else {
		//            gpInputDIB = dib;
		dib_ = dib;
	}

	//
	// Запишем изображение
	//
	if (!CIMAGE_WriteDIB((uchar*) PUMA_IMAGE_USER, dib_, 1)) {
		cerr << "return code: " << CIMAGE_GetReturnCode() << endl;
		throw PumaException("Puma::open: can't write dib");
	}

	CIMAGEBITMAPINFOHEADER info;
//	if (!filename.empty())
//		LDPUMA_SetFileName(NULL, filename.c_str());

	if (!CIMAGE_GetImageInfo((uchar*) PUMA_IMAGE_USER, &info)) {
		cerr << "return code: " << CIMAGE_GetReturnCode() << endl;
		throw PumaException("Puma::open: can't get image info");
	} else {
		//            gRectTemplate.left = 0;
		//            gRectTemplate.right = info.biWidth;
		//            gRectTemplate.top = 0;
		//            gRectTemplate.bottom = info.biHeight;
	}

	if (!filename.empty()) {
		input_filename_.assign(filename);

		size_t dot_pos = filename.rfind('.');
		if (dot_pos != string::npos)
			layout_filename_.assign(filename, 0, dot_pos);
		else
			layout_filename_.assign(filename);

		layout_filename_ += ".bin";
	}

	page_ = CreateEmptyPage();
}
}
