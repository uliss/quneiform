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
#include "common/log.h"
#include "ligas.h"      // 12.06.2002 E.P.
#include "ccom/ccom.h"
#include "ced/ced.h"
#include "ced/cedpage.h"
#include "cimage/cticontrol.h"
#include "cimage/ctiimage.h"
#include "cline/cline.h"
#include "cstr/cstr.h"
#include "cpage/cpage.h"
#include "cpage/pagestorage.h"
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
#include "resource.h"
#include "rstuff/rstuff.h"
#include "rstuff/rstuff_struct.h"
#include "rverline/rverline.h"
#include "common/modules.h"
#include "puma_debug.h"

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

namespace cf {

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
    recog_name_(NULL)
{
    modulesInit();
}

PumaImpl::~PumaImpl() {
    modulesDone();
}

void PumaImpl::addLayoutBlock(const LayoutBlock &block)
{
    switch(block.type()) {
    case LayoutBlock::TEXT:
        addTextBlock(block.rect());
        break;
    case LayoutBlock::IMAGE:
        addImageBlock(block.rect());
        break;
    default:
        PUMA_WARNING_FUNC() << "unknown block type:" << block.type();
    }
}

void PumaImpl::addImageBlock(const Rect& rect)
{
    PUMA_TRACE_FUNC() << rect;

    if(!cpage_)
        return;

    POLY_ poly;
    poly.com.setRect(rect);
    poly.com.setFlag(CPAGE_BLOCK_USER);
    uint count = CPAGE_GetCountBlock(cpage_);
    CPAGE_CreateBlock(cpage_, TYPE_IMAGE, ++count, 0, &poly, sizeof(poly));

    unsetUpdateFlag(FLG_UPDATE_CPAGE);
}

void PumaImpl::addTableBlock(const Rect& block)
{
    PUMA_TRACE_FUNC() << block;

    if(!cpage_)
        return;

    POLY_ poly;
    poly.com.setRect(block);
    poly.com.setFlag(CPAGE_BLOCK_USER);
    uint count = CPAGE_GetCountBlock(cpage_);
    CPAGE_CreateBlock(cpage_, TYPE_TABLE, ++count, 0, &poly, sizeof(poly));

    unsetUpdateFlag(FLG_UPDATE_CPAGE);
}

void PumaImpl::addTextBlock(const Rect& block)
{
    PUMA_TRACE_FUNC() << block;

    if(!cpage_)
        return;

    POLY_ poly;
    poly.com.setRect(block);
    poly.com.setFlag(CPAGE_BLOCK_USER);
    uint count = CPAGE_GetCountBlock(cpage_);
    CPAGE_CreateBlock(cpage_, TYPE_TEXT, ++count, 0, &poly, sizeof(poly));

    unsetUpdateFlag(FLG_UPDATE_CPAGE);
}

LayoutBlockList PumaImpl::textBlocks() const
{
    LayoutBlockList res;
    if(!cpage_)
        return res;

    Handle block = CPAGE_GetBlockFirst(cpage_, TYPE_TEXT);

    while (block) {
        POLY_ poly;
        CPAGE_GetBlockData(block, TYPE_TEXT, &poly, sizeof(poly));
        res.push_back(LayoutBlock(poly.rect(), LayoutBlock::TEXT));
        block = CPAGE_GetBlockNext(cpage_, block, TYPE_TEXT);
    }

    return res;
}

LayoutBlockList PumaImpl::imageBlocks() const
{
    LayoutBlockList res;
    if(!cpage_)
        return res;

    Handle block = CPAGE_GetBlockFirst(cpage_, TYPE_IMAGE);

    while (block) {
        POLY_ poly;
        CPAGE_GetBlockData(block, TYPE_IMAGE, &poly, sizeof(poly));
        res.push_back(LayoutBlock(poly.rect(), LayoutBlock::IMAGE));
        block = CPAGE_GetBlockNext(cpage_, block, TYPE_IMAGE);
    }

    return res;
}

LayoutBlockList PumaImpl::tableBlocks() const
{
    LayoutBlockList res;
    if(!cpage_)
        return res;

    Handle block = CPAGE_GetBlockFirst(cpage_, TYPE_TABLE);

    while (block) {
        POLY_ poly;
        CPAGE_GetBlockData(block, TYPE_TABLE, &poly, sizeof(poly));
        res.push_back(LayoutBlock(poly.rect(), LayoutBlock::IMAGE));
        block = CPAGE_GetBlockNext(cpage_, block, TYPE_TABLE);
    }

    return res;
}

void PumaImpl::binarizeImage()
{
    clearAll();
    recog_name_ = PUMA_IMAGE_USER;

    uint bit_count = CImage::instance().image(PUMA_IMAGE_USER)->biBitCount;

    if (bit_count > 1) {
        if (!RIMAGE_Binarise(PUMA_IMAGE_USER, PUMA_IMAGE_BINARIZE, binarize_options_))
            throw PumaException("RIMAGE_Binarise failed");

        if (!CIMAGE_ReadDIB(PUMA_IMAGE_BINARIZE, &input_dib_))
            throw PumaException("CIMAGE_ReadDIB failed");

        PAGEINFO info;
        CPAGE_GetPageInfo(cpage_, &info);
        info.addFlag(IMAGE_BINARIZE);
        CPAGE_SetPageInfo(cpage_, info);
        recog_name_ = PUMA_IMAGE_BINARIZE;

        CImage::instance().enableReadMask(PUMA_IMAGE_BINARIZE);
        CImage::instance().applyReadMask(PUMA_IMAGE_BINARIZE);

        if(Config::doDump())
            CIMAGE_Dump(PUMA_IMAGE_BINARIZE, PUMA_IMAGE_BINARIZE ".bmp");
    }

    if(recognize_options_.hasTurn())
        turn(recognize_options_.turnAngle());

    applyReadMask();
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
    PUMA_TRACE_FUNC();

    CLINE_Reset();
    clearAll();
    // clean
    CPAGE_DeleteAll();
    RIMAGE_Reset();
    cpage_ = NULL;
    input_dib_ = NULL;
}

void PumaImpl::debugPrintCpage() const
{
    PUMA_DEBUG_FUNC() << "Container CPAGE has: \n name : size";
    Handle block = CPAGE_GetBlockFirst(cpage_, 0);

    while (block) {
        cfDebug() << CPAGE_GetNameInternalType(CPAGE_GetBlockType(block))
                << " : "
                << CPAGE_GetBlockData(block, CPAGE_GetBlockType(block), NULL, 0);
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

void PumaImpl::prepare()
{
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
}

void PumaImpl::layout()
{
    PUMA_TRACE_FUNC();
    prepare();
    markup();
}

void PumaImpl::loadLayoutFromFile(const std::string& fname) {
    cpage_ = CPAGE_RestorePage(TRUE, fname.c_str());

    if(cpage_ == NULL)
        throw PumaException() << "CPAGE_RestorePage failed from'" << fname << "'";

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

    if(Config::instance().debug())
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

        if (!RSTR_Init(PUMA_MODULE_RSTR, NULL)) {
            std::ostringstream buf;
            buf << "RSTR_Init failed: " << RSTR_GetReturnString(RSTR_GetReturnCode()) << "\n";
            throw PumaException(buf.str());
        }

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

        if (!RCORRKEGL_Init(PUMA_MODULE_RCORRKEGL, NULL))
            throw PumaException("CORRKEGL_Init failed.");
    }
    catch (PumaException& e) {
        modulesDone();
        throw;
    }
    catch(ModuleInitException& e) {
        modulesDone();

        std::string prefix;

        switch(e.module()) {
            case cf::MODULE_MSK:
            prefix = "MSK: ";
            break;
            default:
            prefix = "Unknown module: ";
            break;
        }

        throw PumaException(prefix + e.what());
    }
}

void PumaImpl::open(ImagePtr img)
{
    PUMA_TRACE_FUNC();

    if (!img || !img->data())
        throw PumaException("[PumaImpl::open] invalid image given");

    preOpenInitialize();

    input_filename_ = img->fileName();
    input_dib_ = (BitmapPtr) img->data();

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

    if (RSTR_NeedPass2())
        recognizePass2();
    else
        Debug() << "RSTR said that second pass is not needed.\n";
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
        return;
    }

    if (!RIMAGE_Turn(recog_name_, PUMA_IMAGE_TURN, a))
        throw PumaException("RIMAGE_Turn failed");

    CImage::instance().enableReadMask(PUMA_IMAGE_USER);

    if (!CIMAGE_ReadDIB(PUMA_IMAGE_TURN, &input_dib_))
        throw PumaException("CIMAGE_ReadDIB failed");

    PAGEINFO page_info;
    if (!CPAGE_GetPageInfo(cpage_, &page_info))
        throw PumaException("CPAGE_GetPageData failed");
    page_info.addFlag(IMAGE_TURN);
    page_info.setTurnAngle(angle);
    CPAGE_SetPageInfo(cpage_, page_info);
    recog_name_ = PUMA_IMAGE_TURN;

    if(Config::doDump())
        CIMAGE_Dump(PUMA_IMAGE_TURN, PUMA_IMAGE_TURN ".bmp");
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
            if ((int) CPAGE_GetBlockInterNum(hBlock) == line_attr.fragment) {
                nFragment = line_attr.fragment;
                break;
            }

            hBlock = CPAGE_GetBlockNext(cpage_, hBlock, 0);
        }
    }

    os << "Fragment" << std::setw(2) << line_attr.fragment << " Line" << std::setw(3)
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

void PumaImpl::recognize()
{
    PUMA_TRACE_FUNC() << '\n' << recognize_options_;

    assert(cpage_);
    // Проверим: выделены ли фрагменты.
    if (!CPAGE_GetCountBlock(cpage_) || hasUpdateFlag(FLG_UPDATE_CPAGE)) {
        if(!CPAGE_GetCountBlock(cpage_))
            PUMA_WARNING_FUNC() << "empty page layout";

        PUMA_WARNING_FUNC() << "update layout";
        layout();
    }

    CSTR_DeleteAll();
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
            flagfrag[i] = CPAGE_GetBlockFlags(hBlock);
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
        printResult(std::cerr);

    normalize();
}

void PumaImpl::setBinarizeOptions(const BinarizeOptions& opt)
{
    binarize_options_ = opt;
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

void PumaImpl::rotate(BitmapPtr * dib, Point * p) {
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
    std::ostringstream os;
    os << "cuneiform_pass_" << pass << ".cst";
    CSTR_SaveCont(os.str().c_str());
}

void PumaImpl::saveLayoutToFile(const std::string& fname) {
    CPAGE_ClearBackUp(cpage_);

    if (!CPAGE_SavePage(cpage_, fname.c_str()))
        throw PumaException() << "CPAGE_SavePage to '" << fname << "' failed.";
}

void PumaImpl::saveToText(std::ostream& os) const {
    for (int i = 1, count = CSTR_GetMaxNumber(); i <= count; i++) {
        CSTR_line lin_out = CSTR_GetLineHandle(i, 1); // OLEG

        if (!lin_out)
            throw PumaException("CSTR_GetLineHandle failed");

        os << CSTR_LineToTxt(lin_out, "~") << "\n";
    }
}

void PumaImpl::saveToText(const std::string& filename) const {
    std::ofstream of(filename.c_str());

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

void PumaImpl::dumpComponents()
{
    if(!ccom_)
        return;

    CCOM_comp * pcomp = CCOM_GetFirst(ccom_, NULL);
    while(pcomp != NULL) {
//        std::cerr <<
    }
}

static inline bool isValidPageTemplate(const Rect& r)
{
    return r.left() >= 0 ||
            r.right() >= 0 ||
            r.bottom() >= 0 ||
            r.top() >= 0;
}

void PumaImpl::applyReadMask()
{
    if(!recognize_options_.hasReadRects())
        return;

    BitmapPtr image = CImage::instance().image(recog_name_);
    if(!image) {
        PUMA_ERROR_FUNC() << "image not found:" << recog_name_;
        return;
    }

    Rect full_page(0, 0, image->biWidth, image->biHeight);

    PAGEINFO page_info;
    GetPageInfo(cpage_, &page_info);
    page_info.status &= ~(PINFO_USERTEMPLATE | PINFO_AUTOTEMPLATE);

    CImage::instance().addRectToReadMask(recog_name_, full_page);

    std::vector<Rect> read_masks = recognize_options_.readRects();
    for(size_t i = 0; i < read_masks.size(); i++) {
        Rect r = read_masks.at(i).normalized().intersected(full_page);
        if(!CImage::instance().removeRectFromReadMask(recog_name_, r))
            PUMA_WARNING_FUNC() << "can't remove rectangle from mask:" << r;

        PUMA_TRACE_FUNC() << "mask rect added:" << r;
    }

    CImage::instance().enableReadMask(recog_name_);
    CImage::instance().applyReadMask(recog_name_);

    PUMA_TRACE_FUNC() << "masks applied to: " << recog_name_;

    if(Config::doDump())
        CIMAGE_Dump(recog_name_, "ImageAfterMasks.bmp");

    SetPageInfo(cpage_, page_info);
    setUpdateFlag(FLG_UPDATE);
}

cpage::BackupPage * PumaImpl::cpage()
{
    if(!cpage_)
        return NULL;

    return &cpage::PageStorage::page(cpage_);
}

const PAGEINFO * PumaImpl::pageInfo() const
{
    if(!cpage_)
        return NULL;

    return cpage::PageStorage::page(cpage_).pageInfo();
}

}
