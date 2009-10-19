/*
 * pumaimpl.cpp
 *
 *  Created on: 22.09.2009
 *      Author: uliss
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstring>
#include <iomanip>

#include "pumaimpl.h"

#define __PUMA_CPP__
#include "mpuma.h"
#include "pumadef.h"

#include "helper.h"
#include "common/debug.h"
#include "specprj.h"
#include "ligas.h"		// 12.06.2002 E.P.
#include "cimage/cticontrol.h"

#include "dpuma.h"
#include "ccom/ccom.h"
#include "ced.h"
#include "cfio/cfio.h"
#include "cpage/cpage.h"
#include "criimage.h"
#include "cstr/cstr.h"
#include "cimage/ctiimage.h"
#include "cline.h"
#include "exc.h"
#include "rblock.h"
#include "rline.h"
#include "rfrmt.h"
#include "rout.h"
#include "rpic.h"
#include "rpstr/rpstr.h"
#include "rstr/rstr.h"
#include "rstuff.h"
#include "rverline.h"
#include "rmarker.h"
#include "rselstr.h"
#include "rsl.h"
#include "rreccom.h"
#include "rcorrkegl.h"

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
            if (attr.language == LANG_RUSSIAN && uv.lnAltCnt && uv.Alt[0].Prob > 100 && !strchr(
                    "0123456789/%", uv.Alt[0].Code[0]))
                nRus++;
            nAll++;
        }
    }
    if (!nAll)
        return 0;

    return (double) nRus / (double) nAll;
}

using namespace std;

static Handle ghStorage = NULL;

namespace CIF {

FixedBuffer<unsigned char, PumaImpl::MainBufferSize> PumaImpl::main_buffer_;
FixedBuffer<unsigned char, PumaImpl::WorkBufferSize> PumaImpl::work_buffer_;

PumaImpl::PumaImpl() :
    rect_template_(Point(-1, -1), Point(-1, -1)), do_spell_corretion_(true), fax100_(false),
            one_column_(false), dot_matrix_(false), auto_rotate_(false), preserve_line_breaks_(
                    false), language_(LANG_RUSENG), layout_filename_("layout.bin"), pictures_(
                    PUMA_PICTURE_ALL), tables_(PUMA_TABLE_DEFAULT), input_dib_(NULL), recog_dib_(
                    NULL), tables_num_(0), ccom_(NULL), cpage_(NULL), lines_ccom_(NULL), cline_(
                    NULL), ed_page_(NULL), special_project_(0), special_global_buf_len_(0),
            kill_vsl_components_(true), rc_line_(TRUE), need_clean_line_(FALSE) {
    format_options_.setLanguage(language_);
    modulesInit();
}

PumaImpl::~PumaImpl() {
    modulesDone();
}

void PumaImpl::binarizeImage() {
    // Бинаризуем изображение
    recog_dib_ = input_dib_;
    recog_name_ = PUMA_IMAGE_USER;

    if (!CIMAGE_GetImageInfo(PUMA_IMAGE_USER, &info_))
        throw PumaException("CIMAGE_GetImageInfo failed");

    LDPUMA_Console("The image depth is %d at this point.\n", info_.biBitCount);

    if (info_.biBitCount > 1) {
        //RIMAGE_BINARISE_KRONROD
        if (!RIMAGE_Binarise((puchar) PUMA_IMAGE_USER, (puchar) PUMA_IMAGE_BINARIZE, 4, 0))
            throw PumaException("RIMAGE_Binarise failed");

        if (!CIMAGE_ReadDIB(PUMA_IMAGE_BINARIZE, (Handle*) input_dib_, TRUE))
            throw PumaException("CIMAGE_ReadDIB failed");

        PAGEINFO info;
        GetPageInfo(cpage_, &info);
        info.Images |= IMAGE_BINARIZE;
        SetPageInfo(cpage_, info);

        recog_name_ = PUMA_IMAGE_BINARIZE;
    }
}

void PumaImpl::clearAll() {
    // Сохраним последенне состояние и очистим контейнер
    if (ed_page_) {
        CED_DeletePage(ed_page_);
        ed_page_ = NULL;
    }

    PAGEINFO PInfo;
    memset(&PInfo, 0, sizeof(PInfo));

    if (cpage_)
        GetPageInfo(cpage_, &PInfo);

    CSTR_DeleteAll();
    CPAGE_DeleteAll();
    cpage_ = CreateEmptyPage();

    strcpy((char*) PInfo.szImageName, PUMA_IMAGE_USER);
    PInfo.Incline2048 = 0;
    PInfo.Angle = 0;
    PInfo.Images = IMAGE_USER;
    SetPageInfo(cpage_, PInfo);

    CCOM_DeleteAll();
    ccom_ = NULL;
    CIMAGE_DeleteImage(PUMA_IMAGE_BINARIZE);
    CIMAGE_DeleteImage(PUMA_IMAGE_DELLINE);
    //  Повернутое изображение ( PUMA_IMAGE_ROTATE) удалять нельзя, как и исходное,
    //  поскольку оно отображается в интерфейсе. Его нужно удалять
    //  либо при получении нового довернутого изображения, либо при
    //  закрытии файла
    CIMAGE_DeleteImage(PUMA_IMAGE_TURN);
}

void PumaImpl::close() {
    Debug() << "Puma close\n";
    CLINE_Reset();
    clearAll();
    // clean
    CIMAGE_Reset();
    CPAGE_DeleteAll();
    RIMAGE_Reset();

    cpage_ = NULL;
    recog_dib_ = input_dib_ = NULL;
}

void PumaImpl::extractComponents() {
    if (!LDPUMA_Skip(hDebugCancelComponent)) {
        LDPUMA_Console("Пропущен этап выделения компонент.\n");
        return;
    }

    PAGEINFO info;
    if (!GetPageInfo(cpage_, &info))
        throw PumaException("GetPageInfo failed");

    ExcControl exc;
    memset(&exc, 0, sizeof(exc));

    CCOM_DeleteContainer((CCOM_handle) ccom_);
    ccom_ = NULL;

    // будет распознавания эвентами
    exc.Control = Ex_ExtraComp | Ex_Picture;

    //Andrey: orientation is obtained from new library RNORM
    if (pictures_ != PUMA_PICTURE_NONE)
        exc.Control |= Ex_PictureLarge;

    uchar w8 = dot_matrix_ ? TRUE : FALSE;
    REXC_SetImportData(REXC_Word8_Matrix, &w8);

    w8 = fax100_ ? TRUE : FALSE;
    REXC_SetImportData(REXC_Word8_Fax1x2, &w8);

    CIMAGEIMAGECALLBACK clbk;
    if (!CIMAGE_GetCallbackImage(info.szImageName, &clbk))
        throw PumaException("CIMAGE_GetCallbackImage failed");

    if (!REXCExtracomp3CB(
            exc, // поиск компонент by 3CallBacks
            (TImageOpen) clbk.CIMAGE_ImageOpen, (TImageClose) clbk.CIMAGE_ImageClose,
            (TImageRead) clbk.CIMAGE_ImageRead))
        throw PumaException("REXCExtracomp3CB failed");

    ccom_ = (Handle) REXCGetContainer();
    if (!ccom_)
        throw PumaException("REXCGetContainer failed");

    SetUpdate(FLG_UPDATE_NO, FLG_UPDATE_CCOM);
}

void PumaImpl::extractStrings() {
    if (LDPUMA_Skip(hDebugStrings)) {
        if (!RSELSTR_ExtractTextStrings(ccom_, cpage_))
            throw PumaException("RSELSTR_ExtractTextStrings failed");
    }
    else {
        if (!RBLOCK_ExtractTextStrings(ccom_, cpage_))
            throw PumaException("RBLOCK_ExtractTextStrings failed");
    }
}

FormatOptions PumaImpl::formatOptions() const {
    return format_options_;
}

void PumaImpl::formatResult() {
    RFRMT_SetFormatOptions(format_options_);

    if (ed_page_) {
        CED_DeletePage(ed_page_);
        ed_page_ = NULL;
    }

    if (!RFRMT_Formatter(input_filename_.c_str(), &ed_page_))
        throw PumaException("RFRMT_Formatter failed");

    if (!LDPUMA_Skip(hDebugEnablePrintFormatted)) {
        std::string fname(input_filename_ + "_tmp_.rtf");
        RFRMT_SetFormatOptions(format_options_);
        RFRMT_SaveRtf(fname.c_str(), 8);
        fname = input_filename_ + "_tmp_.fed";
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
    SET_CB(CBforRS, DPumaSkipComponent);
    SET_CB(CBforRS, DPumaSkipTurn);
    SET_CB(CBforRS, SetUpdate);
    SET_CB(CBforRM, DPumaSkipComponent);
    SET_CB(CBforRM, DPumaSkipTurn);
    SET_CB(CBforRM, SetUpdate);
#undef SET_CB

    DataforRS.gbAutoRotate = auto_rotate_;
    DataforRS.pgpRecogDIB = (uchar**) &input_dib_;
    DataforRS.pinfo = &info_;
    DataforRS.hCPAGE = cpage_;
    DataforRS.phCCOM = &ccom_;
    DataforRS.phCLINE = &cline_;
    DataforRS.phLinesCCOM = &lines_ccom_;
    DataforRS.gnPictures = pictures_;
    DataforRS.gnLanguage = language_;
    DataforRS.gbDotMatrix = dot_matrix_;
    DataforRS.gbFax100 = fax100_;
    DataforRS.pglpRecogName = recog_name_.c_str();
    DataforRS.pgrc_line = &rc_line_;
    DataforRS.gnTables = tables_;
    DataforRS.pgnNumberTables = &tables_num_;
    DataforRS.pgneed_clean_line = &need_clean_line_;
    DataforRS.gRectTemplate = rect_template_;
    DataforRS.hDebugCancelSearchPictures = hDebugCancelSearchPictures;
    DataforRS.hDebugCancelComponent = hDebugCancelComponent;
    DataforRS.hDebugCancelTurn = hDebugCancelTurn;
    DataforRS.hDebugCancelAutoTemplate = hDebugCancelAutoTemplate;
    DataforRS.hDebugCancelSearchLines = hDebugCancelSearchLines;
    DataforRS.hDebugCancelVerifyLines = hDebugCancelVerifyLines;
    DataforRS.hDebugCancelSearchDotLines = hDebugCancelSearchDotLines;
    DataforRS.hDebugCancelRemoveLines = hDebugCancelRemoveLines;
    DataforRS.hDebugCancelSearchTables = hDebugCancelSearchTables;
    DataforRS.szLayoutFileName = (char*) layout_filename_.c_str();
    DataforRS.hDebugEnableSearchSegment = hDebugEnableSearchSegment;

    // калбэки
    if (RSTUFF_SetImportData(RSTUFF_FN_SetProgresspoints, &CBforRS)) {
        ///нормализуем - обработка, поиск картинок, поиск линий
        if (!RSTUFF_RSNormalise(&DataforRS, MemBuf, size_buf, MemWork, size_work))
            throw PumaException("RSTUFF_RSNormalise failed");
    }

    // Gleb 02.11.2000
    // Далее - разметка. Вынесена в RMARKER.DLL
    DataforRM.gbAutoRotate = auto_rotate_;
    DataforRM.pgpRecogDIB = (uchar**) &recog_dib_;
    DataforRM.gbOneColumn = one_column_;
    DataforRM.gKillVSLComponents = kill_vsl_components_;
    DataforRM.pinfo = &info_;
    DataforRM.hCPAGE = cpage_;
    DataforRM.hCCOM = ccom_;
    DataforRM.hCLINE = cline_;
    DataforRM.phLinesCCOM = &lines_ccom_;
    DataforRM.gnPictures = pictures_;
    DataforRM.gnLanguage = language_;
    DataforRM.gbDotMatrix = dot_matrix_;
    DataforRM.gbFax100 = fax100_;
    DataforRM.pgrc_line = &rc_line_;
    DataforRM.gnTables = tables_;
    DataforRM.pgnNumberTables = &tables_num_;
    DataforRM.pgneed_clean_line = &need_clean_line_;
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
    DataforRM.szLayoutFileName = (char*) layout_filename_.c_str();
    DataforRM.hDebugEnableSearchSegment = hDebugEnableSearchSegment;

    if (RMARKER_SetImportData(0, &CBforRM)) {
        if (!RMARKER_PageMarkup(&DataforRM, MemBuf, size_buf, MemWork, size_work))
            throw PumaException("RMARKER_PageMarkup failed");

        cpage_ = DataforRM.hCPAGE; //Paul 25-01-2001
    }

    if (!LDPUMA_Skip(hDebugPrintBlocksCPAGE)) {
        LDPUMA_Console("Контейнер CPAGE содержит: \n имя : размер\n");
        Handle block = CPAGE_GetBlockFirst(cpage_, 0);
        while (block) {
            LDPUMA_Console("%s : %i\n",
                    CPAGE_GetNameInternalType(CPAGE_GetBlockType(cpage_, block)),
                    CPAGE_GetBlockData(cpage_, block, CPAGE_GetBlockType(cpage_, block), NULL, 0));
            block = CPAGE_GetBlockNext(cpage_, block, 0);
        }
    }

    SetUpdate(FLG_UPDATE_NO, FLG_UPDATE_CPAGE);
}

void PumaImpl::loadLayoutFromFile(const std::string& fname) {
    cpage_ = CPAGE_RestorePage(TRUE, fname.c_str());
    if (cpage_ == NULL) {
        ostringstream os;
        os << "CPAGE_RestorePage failed from '" << fname << "'";
        throw PumaException(os.str());
    }
    CPAGE_SetCurrentPage(CPAGE_GetNumberPage(cpage_));
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
        CIMAGE_Init();
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

        RRECCOM_SetImportData(RRECCOM_OcrPath, modulePath());

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

        RSTR_SetImportData(RSTR_OcrPath, modulePath());
        RSTR_SetImportData(RSTR_pchar_temp_dir, moduleTmpPath());

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
    }
    catch (PumaException& e) {
        modulesDone();
        throw e;
    }
}

void PumaImpl::open(char * dib) {
    Debug() << "Puma open\n";

    assert(dib);
    preOpenInitialize();
    input_dib_ = dib;
    // write image
    if (!CIMAGE_WriteDIB(PUMA_IMAGE_USER, dib, 1))
        throw PumaException("PumaImpl::open can't write DIB");

    postOpenInitialize();
    cpage_ = CreateEmptyPage();
}

const char * PumaImpl::modulePath() const {
    return ".";
}

const char * PumaImpl::moduleTmpPath() const {
    return ".";
}

void PumaImpl::normalize() {
    //Нормализуем вертикальные строки
    RPSTR_NormalizeVertStr();
}

Rect PumaImpl::pageTemplate() const {
    return rect_template_;
}

void PumaImpl::pass1() {
    if (!LDPUMA_Skip(hDebugEnableSaveCstr1))
        saveCSTR(1);
    recognizePass1();
}

void PumaImpl::pass2() {
    if (!LDPUMA_Skip(hDebugEnableSaveCstr2))
        saveCSTR(2);

    ///////////////////////////////
    // OLEG : 01-05-18 : for GiP //
    ///////////////////////////////
    if (SPEC_PRJ_GIP == special_project_ && language_ == LANG_RUSENG) {
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
            language_ = LANG_ENGLISH;
            recognizeSetup(language_);
            recognizePass1();
        }
    }

    if (RSTR_NeedPass2())
        recognizePass2();
    else
        LDPUMA_Console("RSTR считает, что второй проход не нужен.\n");
}

void PumaImpl::preOpenInitialize() {
    PumaImpl::close();
    SetUpdate(FLG_UPDATE, FLG_UPDATE_NO);
}

void PumaImpl::printResult(std::ostream& os) {
    int count = CSTR_GetMaxNumber();
    for (int i = 1; i <= count; i++)
        printResultLine(os, i);
}

void PumaImpl::printResultLine(std::ostream& os, size_t lineNumber) {
    CSTR_line lout = CSTR_GetLineHandle(lineNumber, CSTR_LINVERS_MAINOUT);
    CSTR_rast start = CSTR_GetFirstRaster(lout);
    CSTR_rast stop = CSTR_GetLastRaster(lout);
    CSTR_attr line_attr;

    static int nFragment = -1;

    CSTR_GetLineAttr(lout, &line_attr);

    if (line_attr.fragment != nFragment) {
        nFragment = -1;
        Handle hBlock = CPAGE_GetBlockFirst(cpage_, 0);
        while (hBlock) {
            if ((int) CPAGE_GetBlockInterNum(cpage_, hBlock) == line_attr.fragment) {
                nFragment = line_attr.fragment;
                break;
            }
            hBlock = CPAGE_GetBlockNext(cpage_, hBlock, 0);
        }
    }

    os << "Fragment" << setw(2) << line_attr.fragment << " Line" << setw(3) << lineNumber << ": <";

    if (start && stop) {
        CSTR_rast_attr attr;
        UniVersions vers;
        CSTR_rast c = CSTR_GetNextRaster(start, CSTR_f_all);
        for (; c && c != stop; c = CSTR_GetNextRaster(c, CSTR_f_all)) {
            CSTR_GetAttr(c, &attr);
            if (!(attr.flg & (CSTR_f_let | CSTR_f_punct | CSTR_f_bad | CSTR_f_space | CSTR_f_solid)))
                continue;
            if (CSTR_GetCollectionUni(c, &vers)) {
                if (vers.lnAltCnt)
                    os << (char*) vers.Alt[0].Code;
                else
                    os << '~';
            }
        }
    }

    os << ">\n";
}

void PumaImpl::postOpenInitialize() {
    LDPUMA_SetFileName(NULL, "none.txt");
    input_filename_ = "none.bin";

    if (!CIMAGE_GetImageInfo(PUMA_IMAGE_USER, &info_))
        throw PumaException("CIMAGE_GetImageInfo failed");

    rect_template_.set(Point(0, 0), info_.biWidth, info_.biHeight);
}

void PumaImpl::recognize() {
    // Проверим: выделены ли фрагменты.
    if (!CPAGE_GetCountBlock(cpage_) || IsUpdate(FLG_UPDATE_CPAGE))
        layout();

    Debug() << "Puma recognize\n";

    CSTR_DeleteAll();

    if (cpage_)
        CPAGE_UpdateBlocks(cpage_, TYPE_CPAGE_TABLE);

    // Сохраним описание Layout в файл.
    if (!LDPUMA_Skip(hDebugLayoutToFile))
        saveLayoutToFile(layout_filename_);

    // Прочитаем описание Layout из файла.
    if (!LDPUMA_Skip(hDebugLayoutFromFile))
        loadLayoutFromFile(layout_filename_);

    if (IsUpdate(FLG_UPDATE_CCOM))
        extractComponents();

    // Получим описатель страницы
    cpage_ = CPAGE_GetHandlePage(CPAGE_GetCurrentPage());

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
    recognizeSetup(language_);

    CSTR_SortFragm(0);
    CSTR_line ln;
    CSTR_attr attr;
    int32_t nf = CSTR_GetMaxFragment(0);
    Handle hBlock = CPAGE_GetBlockFirst(cpage_, TYPE_TEXT);
    if (hBlock) {
        AutoBuffer<int, InitZero> flagfrag(nf);

        for (int i = 0; hBlock && i < nf; i++) {
            flagfrag[i] = CPAGE_GetBlockFlags(cpage_, hBlock);
            hBlock = CPAGE_GetBlockNext(cpage_, hBlock, TYPE_TEXT);
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
                }
                while (ln);
            }
        }
    }

    pass1();
    pass2();

    spellCorrection();
    recognizeCorrection();

    printResult(cerr);

    // OLEG fot Consistent
    if (SPEC_PRJ_CONS == special_project_) {
        char * buf = &special_global_buf_[0];
        char buf_str[1024];
        char * pb = buf;
        special_global_buf_len_ = 0;
        CSTR_line buf_line;
        int count = CSTR_GetMaxNumber();

        for (int i = 1; i <= count; i++) {
            buf_line = CSTR_GetLineHandle(i, 1);
            CSTR_LineToTxt_Coord(buf_line, buf_str, 1023);
            strcpy(pb, buf_str);
            int len = strlen(pb);
            pb += len + 1;
            special_global_buf_len_ += len + 1;
        }
        *pb = 0;
        special_global_buf_len_++;

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
    if (!RCORRKEGL_SetImportData(RCORRKEGL_Bool32_Fax100, &fax100_) || !RCORRKEGL_CorrectKegl(1))
        throw PumaException("PumaImpl::recognizeCorrection() failed");

    CSTR_SortFragm(1);
    RPSTR_CorrectIncline(1);

    if (!LDPUMA_Skip(hDebugEnableSaveCstr4))
        saveCSTR(4);
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

    if (!RSTR_EndPage(cpage_))
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

    if (!RSTR_EndPage(cpage_))
        throw PumaException("RSTR_EndPage failed");
}

void PumaImpl::recognizeSetup(int language) {
    // рапознавание строк
    PAGEINFO info;
    GetPageInfo(cpage_, &info);
    RSTR_Options opt;
    opt.pageSkew2048 = info.Incline2048;//0
    int32_t nResolutionY = info.DPIY;//300;

    opt.language = language;
    special_global_buf_len_ = 0; // OLEG fot Consistent
    if (!RSTR_NewPage(nResolutionY, cpage_))
        throw PumaException("RSTR_NewPage failed");

    if (!RSTR_SetOptions(&opt))
        throw PumaException("RSTR_SetOptions failed");

    // Настройка параметров
    uchar w8 = (uchar) language_;
    RSTR_SetImportData(RSTR_Word8_Language, &w8);

    uint16_t w16 = (uint16_t) info.DPIY;//300;
    RSTR_SetImportData(RSTR_Word16_Resolution, &w16);

    w8 = fax100_ ? TRUE : FALSE;
    RSTR_SetImportData(RSTR_Word8_Fax1x2, &w8);

    w8 = dot_matrix_ ? TRUE : FALSE;
    RSTR_SetImportData(RSTR_Word8_Matrix, &w8);

    w8 = 0;
    RSTR_SetImportData(RSTR_Word8_P2_active, &w8);

    w8 = 1;
    if (!LDPUMA_Skip(hDebugCancelStringsPass2))
        RSTR_SetImportData(RSTR_Word8_P2_disable, &w8);

    w8 = do_spell_corretion_ ? TRUE : FALSE;
    RSTR_SetImportData(RSTR_Word8_Spell_check, &w8);

    RSTR_SetImportData(RSTR_pchar_user_dict_name, user_dict_name_.c_str());

    // Передать язык в словарный контроль. 12.06.2002 E.P.
    // причем всегда 08.02.2008 DVP
    {
        uchar w8 = (uchar) language;
        RPSTR_SetImportData(RPSTR_FNIMP_LANGUAGE, &w8);
        RCORRKEGL_SetImportData(RCORRKEGL_FNIMP_LANGUAGE, &w8);
    }
}

void PumaImpl::rotate(void * dib, Point * p) {
    // Определим угол поворота страницы
    PAGEINFO PInfo;

    assert(p);
    assert(dib);

    if (!CPAGE_GetPageData(cpage_, PT_PAGEINFO, (void*) &PInfo, sizeof(PInfo)))
        throw PumaException("CPAGE_GetPageData failed");

    BitmapInfoHeader info;
    if (PInfo.BitPerPixel > 1) {
        if (!CIMAGE_GetImageInfo(PUMA_IMAGE_BINARIZE, &info))
            throw PumaException("CIMAGE_GetImageInfo failed");

        if (PInfo.Incline2048 > 0) {
            p->rx() = info.biWidth * PInfo.Incline2048 / 2048 * PInfo.Incline2048 / 2048;
            p->ry() = info.biWidth * PInfo.Incline2048 / 2048;
        }
        else {
            p->rx() = -(int32_t) info.biHeight * PInfo.Incline2048 / 2048 + (int32_t) info.biWidth
                    * PInfo.Incline2048 / 2048 * PInfo.Incline2048 / 2048;
            p->ry() = 0;
        }
    }

    // Создадим довернутое изображение
    GetPageInfo(cpage_, &PInfo);

    CIMAGE_DeleteImage(PUMA_IMAGE_ROTATE);

    CIMAGE_EnableMask(PUMA_IMAGE_USER, "r", false);
    if (!RIMAGE_Rotate((puchar) PUMA_IMAGE_USER, (puchar) PUMA_IMAGE_ROTATE, PInfo.Incline2048,
            2048, 0))
        throw PumaException("RIMAGE_Rotate failed");

    if (!CIMAGE_ReadDIB(PUMA_IMAGE_ROTATE, (void**) dib, true))
        throw PumaException("CIMAGE_ReadDIB failed");

    CIMAGE_EnableMask(PUMA_IMAGE_USER, "r", true);
    PInfo.Images |= IMAGE_ROTATE;
    SetPageInfo(cpage_, PInfo);
}

void PumaImpl::rout(const std::string& filename, int Format) const {
    char szName[260];
    strcpy(szName, filename.c_str());
    char * str = strrchr(szName, '.');
    if (str)
        *(str) = '\0';

    char unrecog = format_options_.unrecognizedChar();
    if (!ROUT_SetImportData(ROUT_BOOL_PreserveLineBreaks, (void*) preserve_line_breaks_)
            || !ROUT_SetImportData(ROUT_PCHAR_PageName, szName) || !ROUT_SetImportData(
            ROUT_HANDLE_PageHandle, ed_page_) || !ROUT_SetImportData(ROUT_LONG_Format,
            (void*) Format) || !ROUT_SetImportData(ROUT_LONG_Code, (void*) PUMA_CODE_UTF8)
            || !ROUT_SetImportData(ROUT_PCHAR_BAD_CHAR, (void*) &unrecog))
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
    char unrecog = format_options_.unrecognizedChar();

    if (!ROUT_SetImportData(ROUT_BOOL_PreserveLineBreaks, (void*) preserve_line_breaks_)
            || !ROUT_SetImportData(ROUT_HANDLE_PageHandle, ed_page_) || !ROUT_SetImportData(
            ROUT_LONG_Format, (void*) format) || !ROUT_SetImportData(ROUT_LONG_Code,
            (void*) PUMA_CODE_UTF8) || !ROUT_SetImportData(ROUT_PCHAR_BAD_CHAR, (void*) &unrecog))
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
            if (!ROUT_GetObject(objIndex, (uchar*) dest + (nSize - nCurSize), &nCurSize))
                throw PumaException("ROUT_GetObject failed");
        }
    }
}

void PumaImpl::save(const std::string& filename, int Format) const {
    if (!ed_page_)
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
        if (!CED_WriteFormattedRtf(filename.c_str(), ed_page_))
            throw PumaException("Save to RTF failed");
        break;
    case PUMA_TOEDNATIVE:
        if (!CED_WriteFormattedEd(filename.c_str(), ed_page_))
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

void PumaImpl::saveCSTR(int pass) {
    ostringstream os;
    os << removeFileExt(input_filename_) << "_" << pass << ".cst";
    CSTR_SaveCont(os.str().c_str());
}

void PumaImpl::saveLayoutToFile(const std::string& fname) {
    CPAGE_ClearBackUp(cpage_);
    if (!CPAGE_SavePage(cpage_, fname.c_str())) {
        ostringstream os;
        os << "CPAGE_SavePage to '" << fname << "' failed.";
        throw PumaException(os.str());
    }
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

void PumaImpl::setFormatOptions(const FormatOptions& opt) {
    format_options_ = opt;
}

void PumaImpl::setOptionAutoRotate(bool val) {
    auto_rotate_ = val;
    SetUpdate(FLG_UPDATE, FLG_UPDATE_NO);
}

void PumaImpl::setOptionDotMatrix(bool val) {
    dot_matrix_ = val;
    SetUpdate(FLG_UPDATE_CCOM, FLG_UPDATE_NO);
}

void PumaImpl::setOptionFax100(bool val) {
    fax100_ = val;
    SetUpdate(FLG_UPDATE_CCOM, FLG_UPDATE_NO);
}

void PumaImpl::setOptionLanguage(language_t lang) {
    language_ = lang;
    SetUpdate(FLG_UPDATE_CCOM, FLG_UPDATE_NO);
}

void PumaImpl::setOptionOneColumn(bool val) {
    one_column_ = val;
    SetUpdate(FLG_UPDATE_CPAGE, FLG_UPDATE_NO);
}

void PumaImpl::setOptionPictures(puma_picture_t type) {
    pictures_ = type;
    SetUpdate(FLG_UPDATE_CPAGE, FLG_UPDATE_NO);
}

void PumaImpl::setOptionTable(puma_table_t mode) {
    tables_ = mode;
    SetUpdate(FLG_UPDATE_CPAGE, FLG_UPDATE_NO);
}

void PumaImpl::setOptionUserDictionaryName(const char * name) {
    user_dict_name_ = name;
}

void PumaImpl::setOptionUseSpeller(bool value) {
    do_spell_corretion_ = value;
}

void PumaImpl::setPageTemplate(const Rect& r) {
    Rect old_rect = rect_template_;
    Rect rect = r;

    BitmapInfoHeader info;
    if (CIMAGE_GetImageInfo(PUMA_IMAGE_USER, &info)) {
        CIMAGE_Rect full = { 0, 0, info.biWidth, info.biHeight };
        PAGEINFO PInfo;
        GetPageInfo(cpage_, &PInfo);
        //		PInfo.status &= ~(PINFO_USERTEMPLATE | PINFO_AUTOTEMPLATE);
        PInfo.status &= ~3;
        if (rect.left() < 0 && rect.right() < 0 && rect.bottom() < 0 && rect.top() < 0) {
            rect.set(Point(full.dwX, full.dwY), full.dwWidth, full.dwHeight);
        }

        if (old_rect == rect) {
            PInfo.X = rect.left();
            PInfo.Y = rect.top();
            SetPageInfo(cpage_, PInfo);
            return;
        }

        if (CIMAGE_AddReadCloseRects(PUMA_IMAGE_USER, 1, &full)) {
            if (rect.left() >= 0 && rect.top() >= 0 && rect.width() <= info.biWidth
                    && rect.height() <= info.biHeight) {
                CIMAGE_Rect rtmp = { rect.left(), rect.top(), rect.width(), rect.height() };
                CIMAGE_RemoveReadCloseRects(PUMA_IMAGE_USER, 1, &rtmp);
                PInfo.X = rect.left();
                PInfo.Y = rect.top();
            }
            else {
                CIMAGE_Rect rtmp = { 0, 0, info.biWidth - 1, info.biHeight - 1 };
                CIMAGE_RemoveReadCloseRects(PUMA_IMAGE_USER, 1, &rtmp);
                PInfo.X = 0;
                PInfo.Y = 0;
            }
            SetPageInfo(cpage_, PInfo);
            SetUpdate(FLG_UPDATE, FLG_UPDATE_NO);
            rect_template_ = rect;
        }
    }
}

void PumaImpl::spellCorrection() {
    if (!LDPUMA_Skip(hDebugEnableSaveCstr3))
        saveCSTR(3);

    // Дораспознаем по словарю
    CSTR_SortFragm(1);
    RPSTR_CollectCapDrops(1);
    if (!do_spell_corretion_)
        return;

    if (!RPSTR_CorrectSpell(1))
        throw PumaException("RPSTR_CorrectSpell failed");
}

unsigned char * PumaImpl::mainBuffer() {
    return main_buffer_.begin();
}

unsigned char * PumaImpl::workBuffer() {
    return work_buffer_.begin();
}

}
