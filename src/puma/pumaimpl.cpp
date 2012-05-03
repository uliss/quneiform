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
#include "common/memorybuffer.h"
#include "common/language.h"
#include "ligas.h"      // 12.06.2002 E.P.
#include "ccom/ccom.h"
#include "ced/ced.h"
#include "ced/cedpage.h"
#include "cimage/cticontrol.h"
#include "cimage/ctiimage.h"
#include "cline/cline.h"
#include "cstr/cstr.h"
#include "cpage/cpage.h"
#include "dpuma.h"
#include "exc/exc.h"
#include "export/exporterfactory.h"
#include "markup/pagemarker.h"
#include "rblock/rblock.h"
#include "rcorrkegl/rcorrkegl.h"
#include "rfrmt/formatter.h"
#include "rimage/criimage.h"
#include "rline/rline.h"
#include "rpic/rpic.h"
#include "rpstr/rpstr.h"
#include "rselstr/rselstr.h"
#include "rshelllines/rsl.h"
#include "rstr/rstr.h"
#include "rstuff/rstuff.h"
#include "rstuff/rstuff_struct.h"
#include "rverline/rverline.h"

#define PUMA_ERROR Debug() << "[PUMA] ERROR "
#define PUMA_ERROR_FUNC PUMA_ERROR << BOOST_CURRENT_FUNCTION << ' '
#define PUMA_DEBUG Debug() << "[PUMA] "
#define PUMA_DEBUG_FUNC PUMA_DEBUG << BOOST_CURRENT_FUNCTION << ' '

// 1. Отладочная информаци
static Handle hDebugCancelRemoveLines = NULL;
static Handle hDebugCancelSearchLines = NULL;
static Handle hDebugCancelAutoTemplate = NULL;
static Handle hDebugCancelSearchTables = NULL;
static Handle hDebugCancelVerifyLines = NULL;
static Handle hDebugCancelSearchDotLines = NULL;
static Handle hDebugCancelComponent = NULL;
static Handle hDebugCancelStringsPass2 = NULL;
static Handle hDebugCancelSearchPictures = NULL;
static Handle hDebugEnableSearchSegment = NULL;
static Handle hDebugStrings = NULL;
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

namespace cf {

static char global_buf[64000]; // OLEG fot Consistent
static int32_t global_buf_len = 0; // OLEG fot Consistent

uint32_t PumaImpl::update_flags_ = 0;

bool PumaImpl::hasUpdateFlag(uint32_t flg) {
    return (update_flags_ & flg) > 0;
}

void PumaImpl::SetUpdate(uint32_t flgAdd, uint32_t flgRemove) {
    setUpdateFlag(flgAdd);
    unsetUpdateFlag(flgRemove);
}

void PumaImpl::setUpdateFlag(uint32_t flg) {
    update_flags_ |= flg;
}

void PumaImpl::unsetUpdateFlag(uint32_t flg) {
    update_flags_ &= (~flg);
}

PumaImpl::PumaImpl() :
    layout_filename_("layout.bin"),
    input_dib_(NULL),
    tables_num_(0),
    ccom_(NULL),
    cpage_(NULL),
    lines_ccom_(NULL),
    cline_(NULL),
    rc_line_(TRUE),
    need_clean_line_(FALSE),
    recog_name_(NULL),
    special_project_(SPEC_PRJ_NO)
{
    modulesInit();
}

PumaImpl::~PumaImpl() {
    modulesDone();
}

void PumaImpl::binarizeImage() {
    recog_name_ = PUMA_IMAGE_USER;

    getImageInfo(PUMA_IMAGE_USER);

    if (info_.biBitCount > 1) {
        if (!RIMAGE_Binarise(PUMA_IMAGE_USER, PUMA_IMAGE_BINARIZE, BINARIZATOR_KRONROD, 0))
            throw PumaException("RIMAGE_Binarise failed");

        if (!CIMAGE_ReadDIB(PUMA_IMAGE_BINARIZE, &input_dib_))
            throw PumaException("CIMAGE_ReadDIB failed");

        PAGEINFO info;
        GetPageInfo(cpage_, &info);
        info.Images |= IMAGE_BINARIZE;
        SetPageInfo(cpage_, info);
        recog_name_ = PUMA_IMAGE_BINARIZE;

        CImage::instance().enableReadMask(PUMA_IMAGE_BINARIZE);
        CImage::instance().applyReadMask(PUMA_IMAGE_BINARIZE);

        if(Config::doDump())
            CIMAGE_Dump(PUMA_IMAGE_BINARIZE, PUMA_IMAGE_BINARIZE ".bmp");
    }
}

void PumaImpl::clearAll() {
    // Сохраним последенне состояние и очистим контейнер
    ed_page_.reset();

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
    input_filename_.clear();

    //  Повернутое изображение ( PUMA_IMAGE_ROTATE) удалять нельзя, как и исходное,
    //  поскольку оно отображается в интерфейсе. Его нужно удалять
    //  либо при получении нового довернутого изображения, либо при
    //  закрытии файла
    CIMAGE_RemoveImage(PUMA_IMAGE_BINARIZE);
    CIMAGE_RemoveImage(PUMA_IMAGE_DELLINE);
    CIMAGE_RemoveImage(PUMA_IMAGE_TURN);
}

CEDPagePtr PumaImpl::cedPage() {
    return ed_page_;
}

void PumaImpl::close() {
    if (Config::doDebug())
        PUMA_DEBUG_FUNC << "\n";

    CLINE_Reset();
    clearAll();
    // clean
    CPAGE_DeleteAll();
    RIMAGE_Reset();
    cpage_ = NULL;
    input_dib_ = NULL;
}

void PumaImpl::debugPrintCpage() {
    Debug() << "Container CPAGE has: \n name : size\n";
    Handle block = CPAGE_GetBlockFirst(cpage_, 0);

    while (block) {
        Debug() << CPAGE_GetNameInternalType(CPAGE_GetBlockType(cpage_, block))
                << " : "
                << CPAGE_GetBlockData(cpage_, block, CPAGE_GetBlockType(cpage_, block), NULL, 0)
                << "\n";
        block = CPAGE_GetBlockNext(cpage_, block, 0);
    }
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

    unsetUpdateFlag(FLG_UPDATE_CCOM);
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
    formatter_.reset(new Formatter(format_options_));
    ed_page_ = formatter_->format(input_filename_);
}

void PumaImpl::getImageInfo(const std::string& image_name) {
    if (!CIMAGE_GetImageInfo(image_name.c_str(), &info_))
        throw PumaException("CIMAGE_GetImageInfo failed");
}

void PumaImpl::layout() {
    clearAll();
    binarizeImage();

    if(recognize_options_.hasTurn())
        turn(recognize_options_.turnAngle());

    applyReadMask();

    RSPreProcessImage DataforRS;

    DataforRS.pgpRecogDIB = (uchar**) &input_dib_;
    DataforRS.pinfo = &info_;
    DataforRS.hCPAGE = cpage_;
    DataforRS.phCCOM = &ccom_;
    DataforRS.phCLINE = &cline_;
    DataforRS.phLinesCCOM = &lines_ccom_;
    DataforRS.pglpRecogName = &recog_name_;
    DataforRS.pgrc_line = &rc_line_;
    DataforRS.pgnNumberTables = &tables_num_;
    DataforRS.pgneed_clean_line = &need_clean_line_;
    DataforRS.hDebugCancelSearchPictures = hDebugCancelSearchPictures;
    DataforRS.hDebugCancelComponent = hDebugCancelComponent;
    DataforRS.hDebugCancelTurn = hDebugCancelTurn;
    DataforRS.hDebugCancelAutoTemplate = hDebugCancelAutoTemplate;
    DataforRS.hDebugCancelSearchLines = hDebugCancelSearchLines;
    DataforRS.hDebugCancelVerifyLines = hDebugCancelVerifyLines;
    DataforRS.hDebugCancelSearchDotLines = hDebugCancelSearchDotLines;
    DataforRS.hDebugCancelRemoveLines = hDebugCancelRemoveLines;
    DataforRS.hDebugCancelSearchTables = hDebugCancelSearchTables;
    DataforRS.hDebugEnableSearchSegment = hDebugEnableSearchSegment;

    RSCBProgressPoints CBforRS;
    CBforRS.pSetUpdate = (void*) SetUpdate;

    rstuff_->setCallbacks(&CBforRS);
    rstuff_->setImageData(&DataforRS);
    rstuff_->setRecognizeOptions(recognize_options_);
    rstuff_->normalize();

    markup();
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

void PumaImpl::markup() {
    marker_->setComponentContainer(ccom_);
    marker_->setCLine(cline_);
    marker_->setCPage(cpage_);
    marker_->setLayoutFilename(layout_filename_);
    marker_->setOptions(recognize_options_);
    marker_->markupPage();
    cpage_ = marker_->cpage();

    if (Config::instance().debug())
        debugPrintCpage();

    unsetUpdateFlag(FLG_UPDATE_CPAGE);
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
#ifdef _USE_RVERLINE_
    RVERLINE_Done();
#endif //_USE_RVERLINE_
#ifdef _USE_RMSEGMENT_
    RMSEGMENT_Done();
#endif //_USE_RMSEGMENT_
}

void PumaImpl::modulesInit() {
    try {
        // CONTEINERS
        if (!CLINE_Init(PUMA_MODULE_CLINE, NULL))
            throw PumaException("CLINE_Init failed.");

        //  нужна инициализация контейнера CCOM перед
        //  вызовом поиска компонент
        if (!CCOM_Init(PUMA_MODULE_CCOM))
            throw PumaException("CCOM_Init failed.");

        if (!CPAGE_Init(PUMA_MODULE_CPAGE, NULL))
            throw PumaException("CPAGE_Init failed.");

        if (!CSTR_Init(PUMA_MODULE_CSTR, NULL))
            throw PumaException("CSTR_Init failed.");

        // инициализация библиотеки поиска компонент
        comp_extractor_.reset(new ComponentExtractor);

        if (!RLINE_Init(PUMA_MODULE_RLINE, NULL))
            throw PumaException("RLINE_Init failed.");

        if (!RSL_Init(PUMA_MODULE_RSL, NULL))
            throw PumaException("RSL_Init failed.");

        rstuff_.reset(new RStuff);
        marker_.reset(new PageMarker);

        if (!RBLOCK_Init(PUMA_MODULE_RBLOCK, NULL))
            throw PumaException("RBLOCK_Init failed.");

        if (!RSELSTR_Init(PUMA_MODULE_RBLOCK, NULL))
            throw PumaException("RSELSTR_Init failed.");

        RSTR_SetImportData(RSTR_OcrPath, modulePath());
        RSTR_SetImportData(RSTR_pchar_temp_dir, moduleTmpPath());

        if (!RSTR_Init(PUMA_MODULE_RSTR, NULL))
            throw PumaException("RSTR_Init failed.");

        if (!RIMAGE_Init(PUMA_MODULE_RIMAGE))
            throw PumaException("RIMAGE_Init failed.");

        // Инициализируем виртуальные функции
        if (!RPSTR_Init(PUMA_MODULE_RPSTR, NULL))
            throw PumaException("RPSTR_Init failed.");

        if (!RPIC_Init(PUMA_MODULE_RPIC, NULL))
            throw PumaException("RPIC_Init failed.");

#ifdef _USE_RVERLINE_

        if (!RVERLINE_Init(PUMA_MODULE_RVERLINE, NULL))
        throw PumaException("RVERLINE_Init failed.");

#endif
#ifdef _USE_RMSEGMENT_

        if (!RMSEGMENT_Init(PUMA_MODULE_RMSEGMENT, NULL))
        throw PumaException("RMSEGMENT_Init failed.");

#endif

        if (!RCORRKEGL_Init(PUMA_MODULE_RCORRKEGL, NULL))
            throw PumaException("CORRKEGL_Init failed.");
    }
    catch (PumaException& e) {
        modulesDone();
        throw;
    }
}

void PumaImpl::open(ImagePtr img) {
    if (!img || !img->data())
        throw PumaException("[PumaImpl::open] invalid image given");

    if (Config::doDebug())
        PUMA_DEBUG_FUNC << "\n";

    preOpenInitialize();

    input_filename_ = img->fileName();
    input_dib_ = (BitmapHandle) img->data();

    // write image
    if (!CIMAGE_AddImage(PUMA_IMAGE_USER, input_dib_))
        throw PumaException("[PumaImpl::open] can't write DIB");

    cpage_ = CreateEmptyPage();
    postOpenInitialize();
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

void PumaImpl::turn(int angle)
{
    CImage::instance().disableReadMask(PUMA_IMAGE_USER);

    rimage_turn_angle_t a = (rimage_turn_angle_t) 0;
    switch(angle) {
    case 90:
        a = RIMAGE_TURN_90;
        break;
    case 180:
        a = RIMAGE_TURN_180;
        break;
    case 270:
        a = RIMAGE_TURN_270;
        break;
    default:
        break;
    }

    if (!RIMAGE_Turn(PUMA_IMAGE_USER, PUMA_IMAGE_TURN, a))
        throw PumaException("RIMAGE_Turn failed", angle);

    CImage::instance().enableReadMask(PUMA_IMAGE_USER);

    if (!CIMAGE_ReadDIB(PUMA_IMAGE_TURN, &input_dib_))
        throw PumaException("CIMAGE_ReadDIB failed");

    PAGEINFO page_info;
    if (!CPAGE_GetPageData(cpage_, PT_PAGEINFO, &page_info, sizeof(page_info)))
        throw PumaException("CPAGE_GetPageData failed");
    page_info.Images |= IMAGE_TURN;
    page_info.setTurnAngle(angle);
    CPAGE_SetPageData(cpage_, PT_PAGEINFO, &page_info, sizeof(page_info));
    recog_name_ = PUMA_IMAGE_TURN;
}

void PumaImpl::preOpenInitialize() {
    PumaImpl::close();
    setUpdateFlag(FLG_UPDATE);
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
}

void PumaImpl::recognize() {
    assert(cpage_);
    // Проверим: выделены ли фрагменты.
    if (!CPAGE_GetCountBlock(cpage_) || hasUpdateFlag(FLG_UPDATE_CPAGE))
        layout();

    if (Config::instance().debug()) {
        PUMA_DEBUG_FUNC << '\n' << recognize_options_;
    }

    CSTR_DeleteAll();

    if (cpage_)
        CPAGE_UpdateBlocks(cpage_, TYPE_CPAGE_TABLE);

    if (Config::instance().debugDump())
        saveLayoutToFile(layout_filename_);

    if (Config::instance().debugDump())
        loadLayoutFromFile(layout_filename_);

    if (hasUpdateFlag(FLG_UPDATE_CCOM))
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

void PumaImpl::rotate(BitmapHandle * dib, Point * p) {
    // Определим угол поворота страницы
    PAGEINFO page_info;
    assert(p);
    assert(dib);

    if (!CPAGE_GetPageData(cpage_, PT_PAGEINFO, (void*) &page_info, sizeof(page_info)))
        throw PumaException("CPAGE_GetPageData failed");

    if (page_info.BitPerPixel > 1) {
        BitmapInfoHeader info;
        if (!CIMAGE_GetImageInfo(PUMA_IMAGE_BINARIZE, &info))
            throw PumaException("CIMAGE_GetImageInfo failed");

        if (page_info.Incline2048 > 0) {
            p->rx() = info.biWidth * page_info.Incline2048 / 2048
                    * page_info.Incline2048 / 2048;
            p->ry() = info.biWidth * page_info.Incline2048 / 2048;
        }
        else {
            p->rx() = -info.biHeight * page_info.Incline2048 / 2048
                    + info.biWidth * page_info.Incline2048 / 2048
                            * page_info.Incline2048 / 2048;
            p->ry() = 0;
        }
    }

    // Создадим довернутое изображение
    GetPageInfo(cpage_, &page_info);
    CIMAGE_RemoveImage(PUMA_IMAGE_ROTATE);

    CImage::instance().disableReadMask(PUMA_IMAGE_USER);

    if (!RIMAGE_Rotate(PUMA_IMAGE_USER, PUMA_IMAGE_ROTATE, page_info.Incline2048, 2048))
        throw PumaException("RIMAGE_Rotate failed");

    if (!CIMAGE_ReadDIB(PUMA_IMAGE_ROTATE, dib))
        throw PumaException("CIMAGE_ReadDIB failed");

    CImage::instance().enableReadMask(PUMA_IMAGE_USER);
    page_info.Images |= IMAGE_ROTATE;
    SetPageInfo(cpage_, page_info);
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
    setUpdateFlag(FLG_UPDATE);
    setUpdateFlag(FLG_UPDATE_CCOM);
    setUpdateFlag(FLG_UPDATE_CPAGE);
}

static inline bool isValidPageTemplate(const Rect& r)
{
    return r.left() >= 0 ||
            r.right() >= 0 ||
            r.bottom() >= 0 ||
            r.top() >= 0;
}

void PumaImpl::setSpecialProject(special_project_t SpecialProject) {
    special_project_ = SpecialProject;
    RSTR_SetSpecPrj(SpecialProject);
}

void PumaImpl::applyReadMask()
{
    if(!recognize_options_.hasReadRects())
        return;

    BitmapHandle image = CImage::instance().image(recog_name_);
    if(!image) {
        PUMA_ERROR_FUNC << "image not found: " << recog_name_ << "\n";
        return;
    }

    Rect full_page(0, 0, image->biWidth, image->biHeight);

    PAGEINFO page_info;
    GetPageInfo(cpage_, &page_info);
    page_info.status &= ~(PINFO_USERTEMPLATE | PINFO_AUTOTEMPLATE);

    CImage::instance().addRectToReadMask(recog_name_, full_page);

    std::vector<Rect> read_masks = recognize_options_.readRects();
    for(size_t i = 0; i < read_masks.size(); i++) {
        Rect r = read_masks.at(i).intersected(full_page).normalized();
        CImage::instance().removeRectFromReadMask(recog_name_, r);

        if(Config::instance().debugHigh())
            PUMA_DEBUG << "mask rect added: " << r << "\n";
    }

    CImage::instance().enableReadMask(recog_name_);
    CImage::instance().applyReadMask(recog_name_);

    PUMA_DEBUG_FUNC << "masks applied to: " << recog_name_ << "\n";

    if(Config::doDump())
        CIMAGE_Dump(recog_name_, "ImageAfterMasks.bmp");

    SetPageInfo(cpage_, page_info);
    setUpdateFlag(FLG_UPDATE);
}

}
