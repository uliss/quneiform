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
#include "common/cifconfig.h"
#include "common/debug.h"
#include "specprj.h"
#include "ligas.h"		// 12.06.2002 E.P.
#include "cimage/cticontrol.h"

#include "dpuma.h"
#include "ccom/ccom.h"
#include "ced/ced.h"
#include "cfio/cfio.h"
#include "cpage/cpage.h"
#include "criimage.h"
#include "cstr/cstr.h"
#include "cimage/ctiimage.h"
#include "cline.h"
#include "rblock.h"
#include "rline.h"
#include "rfrmt/rfrmt.h"
#include "rout/rout.h"
#include "rout/exporterfactory.h"
#include "rpic.h"
#include "rpstr/rpstr.h"
#include "rstr/rstr.h"
#include "rstuff/rstuff.h"
#include "rverline.h"
#include "rmarker/rmarker.h"
#include "rselstr.h"
#include "rshelllines/rsl.h"
#include "rreccom/rreccom.h"
#include "rcorrkegl/rcorrkegl.h"

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

PumaImpl::PumaImpl() :
    do_spell_correction_(true), fax100_(false), language_(LANG_RUSENG), input_dib_(NULL),
            recog_dib_(NULL), ccom_(NULL), cpage_(NULL), lines_ccom_(NULL), cline_(NULL), ed_page_(
                    NULL), special_project_(0), special_global_buf_len_(0), kill_vsl_components_(
                    true) {
    modulesInit();
}

PumaImpl::~PumaImpl() {
    modulesDone();
}

void PumaImpl::addUserDictionary_(const std::string& name) {
    if (name.empty())
        return;
    user_dict_.insert(name);
}

void PumaImpl::addUserDictionary(const std::string& name) {
    const char DictionarySeparator = ',';
    if (name.find(DictionarySeparator) == string::npos)
        addUserDictionary_(name);
    else {
        size_t pos = 0, last_comma = 0;
        for (size_t i = 0; i < name.size(); i++) {
            if (name[i] == DictionarySeparator) {
                addUserDictionary_(name.substr(pos, i - pos));
                pos = i + 1;
                last_comma = i;
            }
        }
        if (last_comma)
            addUserDictionary_(name.substr(last_comma + 1));
    }
}

void PumaImpl::binarizeImage() {
    // Бинаризуем изображение
    recog_dib_ = input_dib_;
    recog_name_ = PUMA_IMAGE_USER;

    if (!CIMAGE_GetImageInfo(PUMA_IMAGE_USER, &info_))
        throw PumaException("CIMAGE_GetImageInfo failed");

    if (Config::instance().debug())
        Debug() << "The image depth is " << info_.biBitCount << " at this point.\n";

    if (info_.biBitCount <= 1)
        return;

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
    CLINE_Reset();
    clearAll();
    // clean
    CIMAGE_Reset();
    CPAGE_DeleteAll();
    RIMAGE_Reset();

    cpage_ = NULL;
    recog_dib_ = input_dib_ = NULL;
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
    rfrmt_->setLanguage(language_);
    rfrmt_->setFormatOptions(format_options_);

    if (ed_page_) {
        CED_DeletePage(ed_page_);
        ed_page_ = NULL;
    }

    rfrmt_->format(input_filename_, &ed_page_);

    if (Config::instance().debugDump()) {
        rfrmt_->setFormatOptions(format_options_);
        rfrmt_->saveRtf(input_filename_ + "_tmp_.rtf");
        std::string fname = input_filename_ + "_tmp_.fed";
        save(fname.c_str(), PUMA_TOEDNATIVE);
    }
}

void PumaImpl::layout() {
    clearAll();
    binarizeImage();

    layoutRsl();
    layoutRStuff();
    layoutRMarker();

    if (Config::instance().debugHigh()) {
        Debug() << "Container CPAGE contains: \n name : size\n";
        Handle block = CPAGE_GetBlockFirst(cpage_, 0);
        while (block) {
            Debug() << CPAGE_GetNameInternalType(CPAGE_GetBlockType(cpage_, block)) << " : "
                    << CPAGE_GetBlockData(cpage_, block, CPAGE_GetBlockType(cpage_, block), NULL, 0)
                    << "\n";
            block = CPAGE_GetBlockNext(cpage_, block, 0);
        }
    }

    SetUpdate(FLG_UPDATE_NO, FLG_UPDATE_CPAGE);
}

void PumaImpl::layoutRsl() {
    rsl_->setCCom(&ccom_);
    rsl_->setCLine(&cline_);
    rsl_->setCPage(cpage_);
    rsl_->setTableMode(layout_options_.tableMode());
    rsl_->verifyNormalization();
}

void PumaImpl::layoutRStuff() {
    RStuff::setUpdate = SetUpdate;

    RSPreProcessImage DataforRS;
    setData(DataforRS);

    ///нормализуем - обработка, поиск картинок, поиск линий
    rstuff_->setDotMatrix(layout_options_.dotMatrix());
    rstuff_->setFax100(fax100_);
    rstuff_->setCPage(cpage_);
    rstuff_->setLanguage(language_);
    rstuff_->setImageData(DataforRS);
    rstuff_->normalize();
}

void PumaImpl::layoutRMarker() {
    rmarker_->setCCom(ccom_);
    rmarker_->setCPage(cpage_);
    rmarker_->setCLine(cline_);
    rmarker_->setLanguage(language_);
    rmarker_->setOneColumn(layout_options_.oneColumn());
    rmarker_->setPicturesMode(layout_options_.pictures());
    rmarker_->setKillSVLComponents(kill_vsl_components_);
    rmarker_->pageMarkup();
    cpage_ = rmarker_->cPage();
}

LayoutOptions PumaImpl::layoutOptions() const {
    return layout_options_;
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

        //	REXC_SetImportData(REXC_OcrPath, GetModulePath());
        if (!RLINE_Init(PUMA_MODULE_RLINE, ghStorage))
            throw PumaException("RLINE_Init failed.");

        // FIXME
        RReccom::setOcrPath(modulePath());
        //        RRECCOM_SetImportData(RRECCOM_OcrPath, modulePath());

        rsl_.reset(new Rsl);
        rstuff_.reset(new RStuff);
        rmarker_.reset(new RMarker);
        rfrmt_.reset(new RFrmt);

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

void PumaImpl::pass1() {
    if (Config::instance().debugHigh())
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
            recognizeSetup();
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

void PumaImpl::printRecognizeOptions() const {
    Debug() << "############################\n" << "CuneiForm Recognize options:\n"
            << "  Language:      " << language_ << "\n" << boolalpha << "  Fax:           "
            << fax100_ << "\n" << "  Use speller:   " << do_spell_correction_ << "\n"
            << layout_options_ << format_options_;
    if (!user_dict_.empty()) {
        Debug() << "User dictionaries: ";
        for (DictContainer::iterator it = user_dict_.begin(), end = user_dict_.end(); it != end; ++it)
            Debug() << *it << " ";
        Debug() << "\n";
    }
    Debug() << "############################\n";
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
    input_filename_ = "none.bin";

    if (!CIMAGE_GetImageInfo(PUMA_IMAGE_USER, &info_))
        throw PumaException("CIMAGE_GetImageInfo failed");

    layout_options_.setRect(Rect(Point(0, 0), info_.biWidth, info_.biHeight));
}

void PumaImpl::recognize() {
    if (Config::instance().debug())
        printRecognizeOptions();

    // Проверим: выделены ли фрагменты.
    if (!CPAGE_GetCountBlock(cpage_) || IsUpdate(FLG_UPDATE_CPAGE))
        layout();

    CSTR_DeleteAll();

    if (cpage_)
        CPAGE_UpdateBlocks(cpage_, TYPE_CPAGE_TABLE);

    // Сохраним описание Layout в файл.
    if (!LDPUMA_Skip(hDebugLayoutToFile))
        saveLayoutToFile(layout_options_.layoutFilename());

    // Прочитаем описание Layout из файла.
    if (!LDPUMA_Skip(hDebugLayoutFromFile))
        loadLayoutFromFile(layout_options_.layoutFilename());

    if (IsUpdate(FLG_UPDATE_CCOM)) {
        PAGEINFO info;
        if (!GetPageInfo(cpage_, &info))
            throw PumaException("GetPageInfo failed");
        rstuff_->extractComponents(info.szImageName);
    }

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
    recognizeSetup();

    CSTR_SortFragm(0);
    CSTR_line ln;
    CSTR_attr attr;
    int nf = CSTR_GetMaxFragment(0);
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

    if (Config::instance().debugHigh())
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

void PumaImpl::recognizeSetup() {
    // рапознавание строк
    PAGEINFO info;
    GetPageInfo(cpage_, &info);
    RSTR_Options opt;
    opt.pageSkew2048 = info.Incline2048;//0
    int32_t nResolutionY = info.DPIY;//300;

    opt.language = language_;
    opt.Alphabet = 0;
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

    uchar fax100 = fax100_ ? TRUE : FALSE;
    RSTR_SetImportData(RSTR_Word8_Fax1x2, &fax100);

    uchar dm = layout_options_.dotMatrix() ? TRUE : FALSE;
    RSTR_SetImportData(RSTR_Word8_Matrix, &dm);

    w8 = 0;
    RSTR_SetImportData(RSTR_Word8_P2_active, &w8);

    w8 = 1;
    if (!LDPUMA_Skip(hDebugCancelStringsPass2))
        RSTR_SetImportData(RSTR_Word8_P2_disable, &w8);

    w8 = do_spell_correction_ ? TRUE : FALSE;
    RSTR_SetImportData(RSTR_Word8_Spell_check, &w8);

    std::ostringstream os;
    for (DictContainer::iterator it = user_dict_.begin(), end = user_dict_.end(); it != end; ++it)
        os << *it << std::ends;
    os << std::ends;
    user_dict_name_ = os.str();
    RSTR_SetImportData(RSTR_pchar_user_dict_name, user_dict_name_.c_str());

    // Передать язык в словарный контроль. 12.06.2002 E.P.
    // причем всегда 08.02.2008 DVP
    {
        uchar w8 = (uchar) language_;
        RPSTR_SetImportData(RPSTR_FNIMP_LANGUAGE, &w8);
        RCORRKEGL_SetImportData(RCORRKEGL_FNIMP_LANGUAGE, &w8);
    }
}

void PumaImpl::removeUserDictionary(const std::string& name) {
    user_dict_.erase(name);
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

void PumaImpl::rout(void * dest, size_t size, int format) const {
    char unrecog = format_options_.unrecognizedChar();
    Bool line_breaks = format_options_.preserveLineBreaks();
    if (!ROUT_SetImportData(ROUT_BOOL_PreserveLineBreaks, (void*) line_breaks)
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

    if (Config::instance().debug())
        Debug() << "Puma save to: " << filename << endl;

    ExporterFactory::instance().setPage(ed_page_);
    ExporterFactory::instance().setFormatOptions(format_options_);
    Exporter * exp = ExporterFactory::instance().make(Format);
    exp->exportTo(filename);
    delete exp;
}

void PumaImpl::saveCSTR(int pass) {
    ostringstream os;
    os << removeFileExt(input_filename_) << "_" << pass << ".cst";
    if (!CSTR_SaveCont(os.str().c_str()))
        Debug() << "Can't save container to " << os.str() << endl;
}

void PumaImpl::saveLayoutToFile(const std::string& fname) {
    CPAGE_ClearBackUp(cpage_);
    if (!CPAGE_SavePage(cpage_, fname.c_str())) {
        ostringstream os;
        os << "CPAGE_SavePage to '" << fname << "' failed.";
        throw PumaException(os.str());
    }
}

void PumaImpl::setData(RSPreProcessImage& data) {
    layout_options_.setData(data);
    data.pgpRecogDIB = (uchar**) &input_dib_;
    data.pinfo = &info_;
    data.phCCOM = &ccom_;
    data.phCLINE = &cline_;
    data.phLinesCCOM = &lines_ccom_;
    data.pglpRecogName = recog_name_.c_str();
    data.hDebugCancelSearchPictures = 0;
    data.hDebugCancelComponent = 0;
    data.hDebugCancelTurn = 0;
    data.hDebugCancelAutoTemplate = 0;
    data.hDebugCancelSearchLines = 0;
    data.hDebugCancelVerifyLines = 0;
    data.hDebugCancelSearchDotLines = 0;
    data.hDebugCancelRemoveLines = 0;
    data.hDebugCancelSearchTables = 0;
    data.hDebugEnableSearchSegment = 0;
}

void PumaImpl::setFormatOptions(const FormatOptions& opt) {
    format_options_ = opt;
}

void PumaImpl::setLayoutOptions(const LayoutOptions& opt) {
    layout_options_ = opt;
}

void PumaImpl::setOptionFax100(bool val) {
    fax100_ = val;
}

void PumaImpl::setOptionLanguage(language_t lang) {
    language_ = lang;
    SetUpdate(FLG_UPDATE_CCOM, FLG_UPDATE_NO);
}

void PumaImpl::setOptionUserDictionaryName(const char * name) {
    user_dict_name_ = name;
}

void PumaImpl::setOptionUseSpeller(bool value) {
    do_spell_correction_ = value;
}

void PumaImpl::setPageTemplate(const Rect& r) {
    Rect old_rect = layout_options_.rect();
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
            layout_options_.setRect(rect);
        }
    }
}

void PumaImpl::spellCorrection() {
    if (Config::instance().debugHigh())
        saveCSTR(3);

    // Дораспознаем по словарю
    CSTR_SortFragm(1);
    RPSTR_CollectCapDrops(1);
    if (!do_spell_correction_)
        return;

    if (!RPSTR_CorrectSpell(1))
        throw PumaException("RPSTR_CorrectSpell failed");
}

}
