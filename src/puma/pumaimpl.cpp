/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include "pumaimpl.h"
#include "pumadef.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstring>
#include <iomanip>

#include "common/cifconfig.h"
#include "common/debug.h"
#include "common/helper.h"
#include "common/tostring.h"
#include "common/language.h"
#include "ligas.h"      // 12.06.2002 E.P.
#include "ccom/ccom.h"
#include "ced/ced.h"
#include "ced/cedpage.h"
#include "cfio/cfio.h"
#include "cimage/cticontrol.h"
#include "cline/cline.h"
#include "cstr/cstr.h"
#include "cpage/cpage.h"
#include "dpuma.h"
#include "exc/exc.h"
#include "export/exporterfactory.h"
#include "rblock/rblock.h"
#include "rcorrkegl/rcorrkegl.h"
#include "rfrmt/formatter.h"
#include "rimage/criimage.h"
#include "rline/rline.h"
#include "rmarker/rmarker.h"
#include "rpic/rpic.h"
#include "rpstr/rpstr.h"
#include "rselstr/rselstr.h"
#include "rshelllines/rsl.h"
#include "rstr/rstr.h"
#include "rstuff/rstuff.h"
#include "rverline/rverline.h"

// 1. Отладочная информаци
static Handle hDebugCancelRemoveLines = NULL;
static Handle hDebugCancelSearchLines = NULL;
static Handle hDebugCancelAutoTemplate = NULL;
static Handle hDebugCancelSearchTables = NULL;
static Handle hDebugCancelVerifyLines = NULL;
static Handle hDebugCancelSearchDotLines = NULL;
static Handle hDebugCancelComponent = NULL;
static Handle hDebugCancelExtractBlocks = NULL;
static Handle hDebugCancelStringsPass2 = NULL;
static Handle hDebugCancelSearchPictures = NULL;
static Handle hDebugEnableSearchSegment = NULL;
static Handle hDebugStrings = NULL;
static Handle hDebugSVLines = NULL;
static Handle hDebugSVLinesStep = NULL;
static Handle hDebugSVLinesData = NULL;
static Handle hDebugLayoutFromFile = NULL;
static Handle hDebugCancelTurn = NULL;

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
            if (attr.language == LANGUAGE_RUSSIAN && uv.lnAltCnt
                    && uv.Alt[0].Prob > 100 && !strchr("0123456789/%",
                    uv.Alt[0].Code[0]))
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

static char global_buf[64000]; // OLEG fot Consistent
static int32_t global_buf_len = 0; // OLEG fot Consistent
static uint32_t g_flgUpdate = 0;

FixedBuffer<unsigned char, PumaImpl::MainBufferSize> PumaImpl::main_buffer_;
FixedBuffer<unsigned char, PumaImpl::WorkBufferSize> PumaImpl::work_buffer_;

bool IsUpdate(uint32_t flg) {
    return (g_flgUpdate & flg) > 0;
}

void SetUpdate(uint32_t flgAdd, uint32_t flgRemove) {
    g_flgUpdate = (g_flgUpdate | flgAdd) & ~flgRemove;
}

PumaImpl::PumaImpl() :
    rect_template_(Point(-1, -1), Point(-1, -1)),
            layout_filename_("layout.bin"), input_dib_(NULL), recog_dib_(NULL),
            tables_num_(0), ccom_(NULL), cpage_(NULL), lines_ccom_(NULL),
            cline_(NULL), ed_page_(NULL), rc_line_(TRUE), kill_vsl_components_(
                    TRUE), need_clean_line_(FALSE), recog_name_(NULL),
            special_project_(SPEC_PRJ_NO) {
    modulesInit();
}

PumaImpl::~PumaImpl() {
    modulesDone();
}

void PumaImpl::append(const std::string& filename, format_t format) const {
    makeExporter(format)->appendTo(filename);
}

void PumaImpl::binarizeImage() {
    recog_dib_ = input_dib_;
    recog_name_ = PUMA_IMAGE_USER;

    getImageInfo(PUMA_IMAGE_USER);

    if (Config::instance().debug())
        Debug() << "The image depth is " << info_.biBitCount
                << " at this point.\n";

    if (info_.biBitCount > 1) {
        //RIMAGE_BINARISE_KRONROD
        if (!RIMAGE_Binarise(PUMA_IMAGE_USER, PUMA_IMAGE_BINARIZE, 4, 0))
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
        delete ed_page_;
        ed_page_ = NULL;
    }

    PAGEINFO PInfo;
    memset(&PInfo, 0, sizeof(PInfo));

    if (cpage_)
        GetPageInfo(cpage_, &PInfo);

    CSTR_DeleteAll();
    CPAGE_DeleteAll();
    cpage_ = CreateEmptyPage();
    strcpy(PInfo.szImageName, PUMA_IMAGE_USER);
    PInfo.Incline2048 = 0;
    PInfo.Angle = 0;
    PInfo.Images = IMAGE_USER;
    SetPageInfo(cpage_, PInfo);
    CCOM_DeleteAll();
    ccom_ = NULL;

    //  Повернутое изображение ( PUMA_IMAGE_ROTATE) удалять нельзя, как и исходное,
    //  поскольку оно отображается в интерфейсе. Его нужно удалять
    //  либо при получении нового довернутого изображения, либо при
    //  закрытии файла
    CIMAGE_DeleteImage(PUMA_IMAGE_BINARIZE);
    CIMAGE_DeleteImage(PUMA_IMAGE_DELLINE);
    CIMAGE_DeleteImage(PUMA_IMAGE_TURN);
}

void PumaImpl::clearFormat() {
    formatter_.reset();
}

void PumaImpl::close() {
    if (Config::instance().debug())
        Debug() << "Puma::close\n";

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
    PAGEINFO info;

    if (!GetPageInfo(cpage_, &info))
        throw PumaException("GetPageInfo failed");

    ExcControl exc;
    memset(&exc, 0, sizeof(exc));
    CCOM_DeleteContainer(ccom_);
    ccom_ = NULL;
    // будет распознавания эвентами
    exc.Control = Ex_ExtraComp | Ex_Picture;

    //Andrey: orientation is obtained from new library RNORM
    if (recognize_options_.pictureSearch())
        exc.Control |= Ex_PictureLarge;

    uchar w8 = recognize_options_.dotMatrix() ? TRUE : FALSE;
    REXC_SetImportData(REXC_Word8_Matrix, &w8);
    w8 = recognize_options_.fax() ? TRUE : FALSE;
    REXC_SetImportData(REXC_Word8_Fax1x2, &w8);
    CIMAGEIMAGECALLBACK clbk;

    if (!CIMAGE_GetCallbackImage(info.szImageName, &clbk))
        throw PumaException("CIMAGE_GetCallbackImage failed");

    if (!REXCExtracomp3CB(
            exc, // поиск компонент by 3CallBacks
            (TImageOpen) clbk.CIMAGE_ImageOpen,
            (TImageClose) clbk.CIMAGE_ImageClose,
            (TImageRead) clbk.CIMAGE_ImageRead))
        throw PumaException("REXCExtracomp3CB failed");

    ccom_ = REXCGetContainer();

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

CRtfPage * PumaImpl::formatPage() {
    return formatter_.get() ? formatter_->page() : NULL;
}

void PumaImpl::formatResult() {
    formatter_.reset(new Formatter(format_options_));

    if (ed_page_) {
        delete ed_page_;
        ed_page_ = NULL;
    }

    ed_page_ = formatter_->format(input_filename_);
}

void PumaImpl::getImageInfo(const std::string& image_name) {
    if (!CIMAGE_GetImageInfo(image_name.c_str(), &info_))
        throw PumaException("CIMAGE_GetImageInfo failed");
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
    RMPreProcessImage DataforRM;

    CBforRS.pDPumaSkipComponent = (void*) DPumaSkipComponent;
    CBforRS.pDPumaSkipTurn = (void*) DPumaSkipTurn;
    CBforRS.pSetUpdate = (void*) SetUpdate;

    DataforRS.gbAutoRotate = recognize_options_.autoRotate();
    DataforRS.pgpRecogDIB = (uchar**) &input_dib_;
    DataforRS.pinfo = &info_;
    DataforRS.hCPAGE = cpage_;
    DataforRS.phCCOM = &ccom_;
    DataforRS.phCLINE = &cline_;
    DataforRS.phLinesCCOM = &lines_ccom_;
    DataforRS.gnPictures = recognize_options_.pictureSearch();
    DataforRS.gnLanguage = recognize_options_.language();
    DataforRS.gbDotMatrix = recognize_options_.dotMatrix();
    DataforRS.gbFax100 = recognize_options_.fax();
    DataforRS.pglpRecogName = &recog_name_;
    DataforRS.pgrc_line = &rc_line_;
    DataforRS.gnTables = recognize_options_.tableMode();
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

    void* MemBuf = CIF::PumaImpl::mainBuffer();
    size_t size_buf = CIF::PumaImpl::MainBufferSize;
    void* MemWork = CIF::PumaImpl::workBuffer();
    int size_work = CIF::PumaImpl::WorkBufferSize;

    // калбэки
    if (RSTUFF_SetImportData(RSTUFF_FN_SetProgresspoints, &CBforRS)) {
        ///нормализуем - обработка, поиск картинок, поиск линий
        if (!RSTUFF_RSNormalise(&DataforRS, MemBuf, size_buf, MemWork,
                size_work))
            throw PumaException("RSTUFF_RSNormalise failed");
    }

    // Gleb 02.11.2000
    // Далее - разметка. Вынесена в RMARKER.DLL
    DataforRM.gbOneColumn = recognize_options_.oneColumn();
    DataforRM.gKillVSLComponents = kill_vsl_components_;
    DataforRM.hCPAGE = cpage_;
    DataforRM.hCCOM = ccom_;
    DataforRM.hCLINE = cline_;
    DataforRM.gnPictures = recognize_options_.pictureSearch();
    DataforRM.gnLanguage = recognize_options_.language();
    DataforRM.hDebugCancelSearchPictures = hDebugCancelSearchPictures;
    DataforRM.hDebugLayoutFromFile = hDebugLayoutFromFile;
    DataforRM.hDebugCancelExtractBlocks = hDebugCancelExtractBlocks;
    DataforRM.hDebugSVLines = hDebugSVLines;
    DataforRM.hDebugSVLinesStep = hDebugSVLinesStep;
    DataforRM.hDebugSVLinesData = hDebugSVLinesData;
    DataforRM.szLayoutFileName = layout_filename_.c_str();

    if (!RMARKER_PageMarkup(&DataforRM, MemBuf, size_buf, MemWork, size_work))
        throw PumaException("RMARKER_PageMarkup failed");

    cpage_ = DataforRM.hCPAGE; //Paul 25-01-2001

    if (Config::instance().debug()) {
        Debug() << "Container CPAGE has: \n name : size\n";
        Handle block = CPAGE_GetBlockFirst(cpage_, 0);

        while (block) {
            Debug() << CPAGE_GetNameInternalType(CPAGE_GetBlockType(cpage_,
                    block)) << " : " << CPAGE_GetBlockData(cpage_, block,
                    CPAGE_GetBlockType(cpage_, block), NULL, 0) << "\n";
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

ExporterPtr PumaImpl::makeExporter(format_t format) const {
    ExporterFactory::instance().setPage(ed_page_);
    ExporterFactory::instance().setFormatOptions(format_options_);
    ExporterPtr e = ExporterFactory::instance().make(format);
    e->setOutputPictureDir(output_image_dir_);
    return e;
}

void PumaImpl::modulesDone() {
    RCORRKEGL_Done();
    RPIC_Done();
    RIMAGE_Done();
    RSL_Done();
    RLINE_Done();
    RBLOCK_Done();
    RSELSTR_Done();
    RSTR_Done();
    CSTR_Done();
    CCOM_Done();
    CPAGE_Done();
    CLINE_Done();
    RPSTR_Done();
    RSTUFF_Done();
#ifdef _USE_RVERLINE_
    RVERLINE_Done();
#endif //_USE_RVERLINE_
#ifdef _USE_RMSEGMENT_
    RMSEGMENT_Done();
#endif //_USE_RMSEGMENT_
    CFIO_Done();
}

void PumaImpl::modulesInit() {
    try {
        // CONTEINERS
        if (!CLINE_Init(PUMA_MODULE_CLINE, NULL))
            throw PumaException("CLINE_Init failed.");

        if (!CFIO_Init(PUMA_MODULE_CFIO, NULL))
            throw PumaException("CFIO_Init failed.");

        CIMAGE_Init();

        //  нужна инициализация контейнера CCOM перед
        //  вызовом поиска компонент
        if (!CCOM_Init(PUMA_MODULE_CCOM))
            throw PumaException("CCOM_Init failed.");

        if (!CPAGE_Init(PUMA_MODULE_CPAGE, ghStorage))
            throw PumaException("CPAGE_Init failed.");

        if (!CSTR_Init(PUMA_MODULE_CSTR, ghStorage))
            throw PumaException("CSTR_Init failed.");

        // инициализация библиотеки поиска компонент
        comp_extractor_.reset(new ComponentExtractor);

        if (!RLINE_Init(PUMA_MODULE_RLINE, ghStorage))
            throw PumaException("RLINE_Init failed.");

        if (!RSL_Init(PUMA_MODULE_RSL, ghStorage))
            throw PumaException("RSL_Init failed.");

        if (!RSTUFF_Init(PUMA_MODULE_RSTUFF, ghStorage))
            throw PumaException("RSTUFF_Init failed.");

        rmarker_.reset(new RMarker);

        if (!RBLOCK_Init(PUMA_MODULE_RBLOCK, ghStorage))
            throw PumaException("RBLOCK_Init failed.");

        if (!RSELSTR_Init(PUMA_MODULE_RBLOCK, ghStorage))
            throw PumaException("RSELSTR_Init failed.");

        RSTR_SetImportData(RSTR_OcrPath, modulePath());
        RSTR_SetImportData(RSTR_pchar_temp_dir, moduleTmpPath());

        if (!RSTR_Init(PUMA_MODULE_RSTR, ghStorage))
            throw PumaException("RSTR_Init failed.");

        if (!RIMAGE_Init(PUMA_MODULE_RIMAGE, ghStorage))
            throw PumaException("RIMAGE_Init failed.");

        // Инициализируем виртуальные функции
        if (!RPSTR_Init(PUMA_MODULE_RPSTR, ghStorage))
            throw PumaException("RPSTR_Init failed.");

        if (!RPIC_Init(PUMA_MODULE_RPIC, ghStorage))
            throw PumaException("RPIC_Init failed.");

#ifdef _USE_RVERLINE_

        if (!RVERLINE_Init(PUMA_MODULE_RVERLINE, ghStorage))
        throw PumaException("RVERLINE_Init failed.");

#endif
#ifdef _USE_RMSEGMENT_

        if (!RMSEGMENT_Init(PUMA_MODULE_RMSEGMENT, ghStorage))
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

void PumaImpl::open(ImagePtr img) {
    if (!img || !img->data())
        throw PumaException("[PumaImpl::open] invalid image given");

    if (Config::instance().debug())
        Debug() << "Puma open\n";
    input_filename_ = img->fileName();
    preOpenInitialize();
    input_dib_ = img->data();

    // write image
    if (!CIMAGE_WriteDIB(PUMA_IMAGE_USER, input_dib_, 1))
        throw PumaException("[PumaImpl::open] can't write DIB");

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
    if (Config::instance().debugDump())
        saveCSTR(1);

    recognizePass1();
}

void PumaImpl::pass2() {
    if (Config::instance().debugDump())
        saveCSTR(2);

    if (SPEC_PRJ_GIP == special_project_ && recognize_options_.language()
            == LANGUAGE_RUS_ENG)
        pass2special();

    if (RSTR_NeedPass2())
        recognizePass2();
    else
        Debug() << "RSTR said that second pass is not needed.\n";
}

void PumaImpl::pass2special() {
    double s = 0;
    int n = CSTR_GetMaxNumber();
    for (int i = 1; i <= n; i++) {
        CSTR_line lin_ruseng = CSTR_GetLineHandle(i, 1);
        s += portion_of_rus_letters(lin_ruseng);
    }

    if (n)
        s /= (double) n;

    if (n && s < 0.4) {
        for (int i = 0; i <= n; i++) {
            for (int j = 1; j < 10; j++) {
                CSTR_line lin_ruseng = CSTR_GetLineHandle(i, j);

                if (lin_ruseng)
                    CSTR_DeleteLine(lin_ruseng);
            }
        }

        recognize_options_.setLanguage(LANGUAGE_ENGLISH);
        recognizeSetup();
        recognizePass1();
    }
}

void PumaImpl::spellCorrection() {
    if (Config::instance().debugDump())
        saveCSTR(3);

    // Дораспознаем по словарю
    CSTR_SortFragm(1);
    RPSTR_CollectCapDrops(1);

    if (!recognize_options_.spellCorection())
        return;

    if (!RPSTR_CorrectSpell(1))
        throw PumaException("RPSTR_CorrectSpell failed");
}

void PumaImpl::preOpenInitialize() {
    PumaImpl::close();
    SetUpdate(FLG_UPDATE, FLG_UPDATE_NO);
}

void PumaImpl::preprocessImage() {
    uint32_t Angle = 0;

    // Выделим компоненты
    extractComponents();

    // Проинициализируем контейнер CPAGE
    PAGEINFO PInfo;
    GetPageInfo(cpage_, &PInfo);
    strcpy(PInfo.szImageName, recog_name_);
    PInfo.BitPerPixel = info_.biBitCount;
    PInfo.DPIX = info_.biXPelsPerMeter * 254L / 10000;
    PInfo.DPIX = PInfo.DPIX < 200 ? 200 : PInfo.DPIX;
    PInfo.DPIY = info_.biYPelsPerMeter * 254L / 10000;
    PInfo.DPIY = PInfo.DPIY < 200 ? 200 : PInfo.DPIY;
    PInfo.Height = info_.biHeight;
    PInfo.Width = info_.biWidth;
    //      PInfo.X = 0; Уже установлено
    //      PInfo.Y = 0;
    PInfo.Incline2048 = 0;
    PInfo.Page = 1;
    PInfo.Angle = Angle;
    SetPageInfo(cpage_, PInfo);
}

void PumaImpl::printResult(std::ostream& os) {
    for (int i = 1, count = CSTR_GetMaxNumber(); i <= count; i++)
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
            if ((int) CPAGE_GetBlockInterNum(cpage_, hBlock)
                    == line_attr.fragment) {
                nFragment = line_attr.fragment;
                break;
            }

            hBlock = CPAGE_GetBlockNext(cpage_, hBlock, 0);
        }
    }

    os << "Fragment" << setw(2) << line_attr.fragment << " Line" << setw(3)
            << lineNumber << ": <";

    if (start && stop) {
        CSTR_rast_attr attr;
        UniVersions vers;
        CSTR_rast c = CSTR_GetNextRaster(start, CSTR_f_all);

        for (; c && c != stop; c = CSTR_GetNextRaster(c, CSTR_f_all)) {
            CSTR_GetAttr(c, &attr);

            if (!(attr.flg & (CSTR_f_let | CSTR_f_punct | CSTR_f_bad
                    | CSTR_f_space | CSTR_f_solid)))
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
    getImageInfo(PUMA_IMAGE_USER);
    rect_template_.set(Point(), info_.biWidth, info_.biHeight);
}

void PumaImpl::recognize() {
    assert(cpage_);
    // Проверим: выделены ли фрагменты.
    if (!CPAGE_GetCountBlock(cpage_) || IsUpdate(FLG_UPDATE_CPAGE))
        layout();

    if (Config::instance().debug()) {
        Debug() << "Puma recognize\n" << recognize_options_;
    }

    CSTR_DeleteAll();

    if (cpage_)
        CPAGE_UpdateBlocks(cpage_, TYPE_CPAGE_TABLE);

    if (Config::instance().debugDump())
        saveLayoutToFile(layout_filename_);

    if (Config::instance().debugDump())
        loadLayoutFromFile(layout_filename_);

    if (IsUpdate(FLG_UPDATE_CCOM))
        extractComponents();

    // Получим описатель страницы
    cpage_ = CPAGE_GetHandlePage(CPAGE_GetCurrentPage());

    // Выделим строки
    extractStrings();

    // распознаем строки
    CSTR_SortFragm(0);
    recognizeSetup();
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
                } while (ln);
            }
        }
    }

    pass1();
    pass2();
    spellCorrection();
    recognizeCorrection();

    if (Config::instance().debug())
        printResult(cerr);

    if (SPEC_PRJ_CONS == special_project_)
        recognizeSpecial();

    normalize();
}

void PumaImpl::recognizeCorrection() {
    // Скорректируем результат распознавани
    CSTR_SortFragm(1);

    Bool32 fax100 = recognize_options_.fax();
    if (!RCORRKEGL_SetImportData(RCORRKEGL_Bool32_Fax100, &fax100)
            || !RCORRKEGL_CorrectKegl(1))
        throw PumaException("PumaImpl::recognizeCorrection() failed");

    CSTR_SortFragm(1);
    RPSTR_CorrectIncline(1);

    if (Config::instance().debugDump())
        saveCSTR(4);
}

void PumaImpl::recognizePass1() {
    // распознавание строк
    for (int i = 1, count = CSTR_GetMaxNumber(); i <= count; i++) {
        CSTR_line lin_out = CSTR_NewLine(i, CSTR_LINVERS_MAINOUT); // OLEG

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
            if ( numFind > 0 ) {
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

void PumaImpl::recognizeSetup() {
    // распознавание строк
    PAGEINFO info;
    GetPageInfo(cpage_, &info);
    RSTR_Options opt;
    opt.pageSkew2048 = info.Incline2048;//0
    int32_t nResolutionY = info.DPIY;//300;
    opt.language = recognize_options_.language();
    global_buf_len = 0; // OLEG fot Consistent

    if (!RSTR_NewPage(nResolutionY, cpage_))
        throw PumaException("RSTR_NewPage failed");

    if (!RSTR_SetOptions(&opt))
        throw PumaException("RSTR_SetOptions failed");

    // Настройка параметров
    uchar w8 = (uchar) recognize_options_.language();
    RSTR_SetImportData(RSTR_Word8_Language, &w8);
    uint16_t w16 = (uint16_t) info.DPIY;//300;
    RSTR_SetImportData(RSTR_Word16_Resolution, &w16);
    w8 = recognize_options_.fax() ? TRUE : FALSE;
    RSTR_SetImportData(RSTR_Word8_Fax1x2, &w8);
    w8 = recognize_options_.dotMatrix() ? TRUE : FALSE;
    RSTR_SetImportData(RSTR_Word8_Matrix, &w8);
    w8 = 0;
    RSTR_SetImportData(RSTR_Word8_P2_active, &w8);
    w8 = 1;

    if (!LDPUMA_Skip(hDebugCancelStringsPass2))
        RSTR_SetImportData(RSTR_Word8_P2_disable, &w8);

    w8 = recognize_options_.spellCorection() ? TRUE : FALSE;
    RSTR_SetImportData(RSTR_Word8_Spell_check, &w8);
    RSTR_SetImportData(RSTR_pchar_user_dict_name, recognize_options_.userDict().c_str());
    // Передать язык в словарный контроль. 12.06.2002 E.P.
    // причем всегда 08.02.2008 DVP
    {
        uchar w8 = (uchar) recognize_options_.language();
        RPSTR_SetImportData(RPSTR_FNIMP_LANGUAGE, &w8);
        RCORRKEGL_SetImportData(RCORRKEGL_FNIMP_LANGUAGE, &w8);
    }
}

void PumaImpl::recognizeSpecial() {
    char * buf = &global_buf[0], buf_str[1024];
    char * pb = buf;
    global_buf_len = 0;

    for (int i = 1, count = CSTR_GetMaxNumber(); i <= count; i++) {
        CSTR_line buf_line = CSTR_GetLineHandle(i, 1);
        CSTR_LineToTxt_Coord(buf_line, buf_str, 1023);
        strcpy(pb, buf_str);
        int len = strlen(pb);
        pb += len + 1;
        global_buf_len += len + 1;
    }

    *pb = 0;
    global_buf_len++;
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
            p->rx() = info.biWidth * PInfo.Incline2048 / 2048
                    * PInfo.Incline2048 / 2048;
            p->ry() = info.biWidth * PInfo.Incline2048 / 2048;
        }

        else {
            p->rx() = -(int32_t) info.biHeight * PInfo.Incline2048 / 2048
                    + (int32_t) info.biWidth * PInfo.Incline2048 / 2048
                            * PInfo.Incline2048 / 2048;
            p->ry() = 0;
        }
    }

    // Создадим довернутое изображение
    GetPageInfo(cpage_, &PInfo);
    CIMAGE_DeleteImage(PUMA_IMAGE_ROTATE);
    CIMAGE_EnableMask(PUMA_IMAGE_USER, "r", false);

    if (!RIMAGE_Rotate((puchar) PUMA_IMAGE_USER, (puchar) PUMA_IMAGE_ROTATE,
            PInfo.Incline2048, 2048, 0))
        throw PumaException("RIMAGE_Rotate failed");

    if (!CIMAGE_ReadDIB(PUMA_IMAGE_ROTATE, (void**) dib, true))
        throw PumaException("CIMAGE_ReadDIB failed");

    CIMAGE_EnableMask(PUMA_IMAGE_USER, "r", true);
    PInfo.Images |= IMAGE_ROTATE;
    SetPageInfo(cpage_, PInfo);
}

void PumaImpl::save(const std::string& filename, format_t format) const {
    makeExporter(format)->exportTo(filename);
}

void PumaImpl::save(std::ostream& os, format_t format) const {
    makeExporter(format)->exportTo(os);
}

void PumaImpl::saveCSTR(int pass) {
    ostringstream os;
    os << "cuneiform_pass_" << pass << ".cst";
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

        os << CSTR_LineToTxt(lin_out, "~") << "\n";
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

void PumaImpl::setRecognizeOptions(const RecognizeOptions& opt) {
    recognize_options_ = opt;
    SetUpdate(FLG_UPDATE, FLG_UPDATE_NO);
    SetUpdate(FLG_UPDATE_CCOM, FLG_UPDATE_NO);
    SetUpdate(FLG_UPDATE_CPAGE, FLG_UPDATE_NO);
}

void PumaImpl::setImageOutputDir(const std::string& path) {
    output_image_dir_ = path;
}

void PumaImpl::setPageTemplate(const Rect& r) {
    Rect old_rect = rect_template_;
    Rect rect = r;
    BitmapInfoHeader info;

    if (CIMAGE_GetImageInfo(PUMA_IMAGE_USER, &info)) {
        CIMAGE_Rect full = { 0, 0, info.biWidth, info.biHeight };
        PAGEINFO PInfo;
        GetPageInfo(cpage_, &PInfo);
        //      PInfo.status &= ~(PINFO_USERTEMPLATE | PINFO_AUTOTEMPLATE);
        PInfo.status &= ~3;

        if (rect.left() < 0 && rect.right() < 0 && rect.bottom() < 0
                && rect.top() < 0) {
            rect.set(Point(full.dwX, full.dwY), full.dwWidth, full.dwHeight);
        }

        if (old_rect == rect) {
            PInfo.X = rect.left();
            PInfo.Y = rect.top();
            SetPageInfo(cpage_, PInfo);
            return;
        }

        if (CIMAGE_AddReadCloseRects(PUMA_IMAGE_USER, 1, &full)) {
            if (rect.left() >= 0 && rect.top() >= 0 && rect.width()
                    <= info.biWidth && rect.height() <= info.biHeight) {
                CIMAGE_Rect rtmp = { rect.left(), rect.top(), rect.width(),
                        rect.height() };
                CIMAGE_RemoveReadCloseRects(PUMA_IMAGE_USER, 1, &rtmp);
                PInfo.X = rect.left();
                PInfo.Y = rect.top();
            }

            else {
                CIMAGE_Rect rtmp =
                        { 0, 0, info.biWidth - 1, info.biHeight - 1 };
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

void PumaImpl::setSpecialProject(special_project_t SpecialProject) {
    special_project_ = SpecialProject;
    RSTUFF_RSSetSpecPrj(SpecialProject);
    RSTR_SetSpecPrj(SpecialProject);
}

unsigned char * PumaImpl::mainBuffer() {
    return main_buffer_.begin();
}

unsigned char * PumaImpl::workBuffer() {
    return work_buffer_.begin();
}

}
