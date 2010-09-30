/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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

#include <cstdlib>
#include <cstring>
#include <memory>
#include "componentrecognizer.h"
#include "alphabets/alphabetfactory.h"
#include "common/language.h"
#include "evn/evn.h"
#include "ccom/ccom.h"
#include "struct.h"

namespace cf
{

static void getExtComp(CCOM_comp* pcomp, CCOM_comp* ec)
{
    memset(ec, 0, sizeof(CCOM_comp));

    ec->h = pcomp->h;
    ec->w = pcomp->w;
    ec->rw = pcomp->rw;
    ec->nl = pcomp->nl;
    ec->begs = pcomp->begs;
    ec->ends = pcomp->ends;
    ec->scale = pcomp->scale;
}

const int DEFAULT_SCALE = 5;

ComponentRecognizer::ComponentRecognizer() :
    max_comp_height_(0), max_comp_width_(0), min_comp_height_(0), min_comp_width_(0), max_scale_(
            DEFAULT_SCALE)
{
    memset(alphabet_, 0, sizeof(alphabet_));
}

ComponentRecognizer::~ComponentRecognizer()
{
}

void ComponentRecognizer::alphabetInit(language_t language)
{
    if (!AlphabetFactory::instance().isLanguageData(language))
        throw Exception(
                "[ComponentRecognizer::alphabetInit] No alphabet tables found for language: "
                        + Language(language).isoName(), language);

    AlphabetPtr alph = AlphabetFactory::instance().make(language);
    assert(alph->size() <= sizeof(alphabet_));
    alph->exportToTable(alphabet_);

    EVNSetAlphabet(alphabet_);
}

void ComponentRecognizer::loadAlphabetTables(language_t language)
{
    AlphabetFactory::AlphabetTables alphabet_tbl = AlphabetFactory::instance().alphabetTables(
            language);

    if (!EVNInitLanguage(alphabet_tbl.first.c_str(), alphabet_tbl.second.c_str(), language))
        throw Exception("[ComponentRecognizer::loadAlphabetTables] EVNInitLanguage failed");
}

uint ComponentRecognizer::maxComponentHeight() const
{
    return (max_comp_height_ > 0) ? max_comp_height_ : RASTER_MAX_HEIGHT;
}

uint ComponentRecognizer::maxComponentWidth() const
{
    return (max_comp_width_ > 0) ? max_comp_width_ : RASTER_MAX_WIDTH;
}

uint ComponentRecognizer::maxScale() const
{
    return (max_scale_ > 0) ? max_scale_ : DEFAULT_SCALE;
}

void ComponentRecognizer::recognize(CCOM_cont * ccom, language_t language)
{
    alphabetInit(language);
    loadAlphabetTables(language);
    recognizeComponents(ccom);
}

void ComponentRecognizer::recognizeComponent(CCOM_comp* pcomp)
{
    if(!pcomp)
        throw Exception("[ComponentRecognizer::recognizeComponent] Invalid component pointer given");

    unsigned char evn_res[17] = "";
    int32_t nvers = 0;
    CCOM_comp comp = *pcomp;
    CCOM_comp ec;

    comp.scale = pcomp->scale;
    comp.w = pcomp->w;
    comp.rw = pcomp->rw;
    comp.h = pcomp->h;

    if (comp.scale < 3 && (comp.w >> comp.scale) < maxComponentWidth() && (comp.h >> comp.scale)
            < maxComponentHeight()) {
        if (comp.scale) {
            comp.w >>= comp.scale;
            comp.h >>= comp.scale;
            comp.rw = (comp.w + 7) / 8;
        }

        getExtComp(&comp, &ec);

        nvers = (int16_t) EVNRecog_lp(&ec, comp.linerep + sizeof(int16_t), comp.size_linerep
                - sizeof(int16_t), evn_res);
        pcomp->type = ec.type;
        pcomp->cs = ec.cs;
    }

    if (nvers) {
        if (!pcomp->vers) {
            pcomp->vers = static_cast<RecVersions*> (calloc(1, sizeof(RecVersions)));
            assert(pcomp->vers);
        }

        if (pcomp->cs == 255)
            nvers >>= 1;

        int32_t vers_beg = pcomp->vers->lnAltCnt;

        if (nvers + pcomp->vers->lnAltCnt > REC_MAX_VERS)
            nvers = REC_MAX_VERS - pcomp->vers->lnAltCnt;

        pcomp->vers->lnAltCnt += nvers;

        if (pcomp->cs == 255) {
            for (int i = 0; i < nvers; i++) {
                pcomp->vers->Alt[vers_beg + i].Code = evn_res[2 * i];
                pcomp->vers->Alt[vers_beg + i].Prob = evn_res[2 * i + 1];
                pcomp->vers->Alt[vers_beg + i].Method = REC_METHOD_NCU;
            } // network collection
        }
        else {
            for (int i = 0; i < nvers; i++) {
                pcomp->vers->Alt[vers_beg + i].Code = evn_res[i];
                pcomp->vers->Alt[vers_beg + i].Prob = 255;
                pcomp->vers->Alt[vers_beg + i].Method = REC_METHOD_EVN;
            } // event collection
        }
    }
}

void ComponentRecognizer::recognizeComponents(CCOM_cont * ccom)
{
    CCOM_comp* pcomp = CCOM_GetFirst(ccom, NULL);

    while (pcomp) {
        recognizeComponent(pcomp);
        pcomp = CCOM_GetNext(pcomp, NULL);
    }
}

void ComponentRecognizer::setMaxComponentHeight(uint height)
{
    max_comp_height_ = height;
}

void ComponentRecognizer::setMaxComponentWidth(uint width)
{
    max_comp_width_ = width;
}

void ComponentRecognizer::setMinComponentHeight(uint height)
{
    min_comp_height_ = height;
}

void ComponentRecognizer::setMinComponentWidth(uint width)
{
    min_comp_width_ = width;
}

void ComponentRecognizer::setMaxScale(uint scale)
{
    max_scale_ = scale;
}

}
