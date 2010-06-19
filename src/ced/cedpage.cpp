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
#include "cedpage.h"
#include "cedchar.h"
#include "cedline.h"
#include "cedsection.h"
#include "cedparagraph.h"

namespace CIF
{

CEDPage::CEDPage() :
    turn_(0), page_number_(0), language_(LANGUAGE_RUS_ENG), unrecognized_char_(0) {
    sections = 0;
    curSect = 0;
    section_num_ = 0;
}

CEDPage::~CEDPage() {
    CEDChar * ch1, *ch;
    ch1 = ch = GetChar(0);

    while (ch1) {
        ch = ch1->next();
        delete ch1;
        ch1 = ch;
    }

    CEDLine * li1, *li;
    li1 = li = GetLine(0);

    while (li1) {
        li = li1->next();
        delete li1;
        li1 = li;
    }

    CEDParagraph * pa1, *pa;
    pa1 = pa = GetParagraph(0);

    while (pa1) {
        pa = pa1->next;

        if (pa1->descriptor) {
            if (pa1->type == TAB_BEGIN) {
                EDTABDESCR * td = (EDTABDESCR *) pa1->descriptor;

                if (td->table) {
                    delete[] td->linesX;
                    delete[] td->linesY;
                    delete[] td->table;
                }

                //              free(td->horShow);
                //              free(td->verShow);
            }

            free(pa1->descriptor);
        }

        delete pa1;
        pa1 = pa;
    }

    CEDSection * se1, *se;
    se1 = se = section(0);

    while (se1) {
        if (se1->colInfo)
            delete[] se1->colInfo;

        se = se1->next;
        delete se1;
        se1 = se;
    }

    clearPictures();
}

void CEDPage::addFont(const FontEntry& font) {

}

void CEDPage::clearPictures() {
    for (size_t i = 0; i < pictures_.size(); i++) {
        free(pictures_[i]->data);
        delete pictures_[i];
    }
    pictures_.clear();
}

void CEDPage::clearSections() {
    delete sections;
    curSect = NULL;
    section_num_ = 0;
}

PictureEntry * CEDPage::findPictureByNumber(int number) const {
    for (size_t i = 0; i < pictures_.size(); i++) {
        if (pictures_.at(i)->pictNumber == number)
            return pictures_[i];
    }

    return NULL;
}

const FontEntry& CEDPage::fontAt(size_t pos) const {
    return fonts_.at(pos);
}

size_t CEDPage::fontCount() const {
    return fonts_.size();
}

Size CEDPage::imageDpi() const {
    return image_dpi_;
}

std::string CEDPage::imageName() const {
    return image_name_;
}

Size CEDPage::imageSize() const {
    return image_size_;
}

language_t CEDPage::language() const {
    return language_;
}

Rect CEDPage::pageBorder() const {
    return page_borders_;
}

int CEDPage::pageNumber() const {
    return page_number_;
}

Size CEDPage::pageSize() const {
    return page_size_;
}

size_t CEDPage::pictureCount() const {
    return pictures_.size();
}

PictureEntry * CEDPage::pictureAt(size_t pos) const {
    return pictures_.at(pos);
}

void CEDPage::setImageDpi(const Size& dpi) {
    image_dpi_ = dpi;
}

void CEDPage::setImageName(const std::string& filename) {
    image_name_ = filename;
}

void CEDPage::setImageSize(const Size& size) {
    image_size_ = size;
}

void CEDPage::setLanguage(language_t language) {
    language_ = language;
}

void CEDPage::setPageBorder(const Rect& borders) {
    page_borders_ = borders;
}

void CEDPage::setPageNumber(int number) {
    page_number_ = number;
}

void CEDPage::setPageSize(const Size& size) {
    page_size_ = size;
}

void CEDPage::setTurn(int angle) {
    turn_ = angle;
}

void CEDPage::setUnrecognizedChar(char chr) {
    unrecognized_char_ = chr;
}

int CEDPage::turn() const {
    return turn_;
}

char CEDPage::unrecognizedChar() const {
    return unrecognized_char_;
}

CEDSection * CEDPage::InsertSection() {
    section_num_++;
    CEDSection * sect = new CEDSection;

    if (curSect) {
        sect->next = curSect->next;

        if (sect->next)
            (sect->next)->prev = sect;

        curSect->next = sect;
        sect->prev = curSect;
        sect->internalNumber = curSect->internalNumber + 1;

        for (CEDSection * sect1 = sect->next; sect1; sect1 = sect1->next)
            sect1->internalNumber++;
    }

    else
        sections = sect;

    curSect = sect;
    return sect;
}

CEDSection * CEDPage::setCurrentSection(CEDSection* _sect) {
    return curSect = _sect;
}

CEDSection * CEDPage::setCurrentSection(int _number) {
    CEDSection* sect;

    for (sect = sections; sect && sect->internalNumber != _number; sect = sect->next)
        ;

    curSect = sect;
    return sect;
}

CEDSection * CEDPage::GetCurSection() {
    return curSect;
}

CEDSection * CEDPage::NextSection() {
    return curSect->next;
}

CEDSection * CEDPage::section(int _num) {
    CEDSection* ss = sections;
    for (; ss && ss->internalNumber != _num; ss = ss->next) {
    }

    return ss;
}
CEDParagraph * CEDPage::GetParagraph(int _num) {
    CEDSection * qq = sections;

    while (qq && !qq->paragraphs)
        qq = qq->next;

    CEDParagraph* ss;

    for (ss = qq ? qq->paragraphs : 0; ss && ss->internalNumber != _num; ss = ss->next)
        ;

    return ss;
}
CEDLine * CEDPage::GetLine(int _num) {
    CEDParagraph *qq = GetParagraph(0);

    while (qq && !qq->lines)
        qq = qq->next;

    CEDLine* ss;

    for (ss = qq ? qq->lines : 0; ss && ss->internalNumber() != _num; ss = ss->next())
        ;

    return ss;
}
CEDChar * CEDPage::GetChar(int _num) {
    CEDLine *qq = GetLine(0);

    while (qq && !qq->first())
        qq = qq->next();

    int num = 0;
    CEDChar* ss;

    for (ss = qq ? qq->first() : 0; ss && num != _num; ss = ss->next())
        num++;

    return ss;
}

int CEDPage::sectionCount() const {
    return section_num_;
}

Bool32 CEDPage::GetFont(int number, uchar* fontNumber, uchar* fontPitchAndFamily,
        uchar* fontCharset, const char** fontName) {
    if (number >= fonts_.size())
        return FALSE;

    if (fontNumber)
        *fontNumber = fontAt(number).fontNumber;

    if (fontPitchAndFamily)
        *fontPitchAndFamily = fonts_[number].fontPitchAndFamily;

    if (fontCharset)
        *fontCharset = fonts_[number].fontCharset;

    if (fontName)
        *fontName = fonts_[number].fontName.c_str();

    return TRUE;
}

int CEDPage::GetFontByNum(uchar fontNumber) {
    for (size_t i = 0; i < fonts_.size(); i++)
        if (fonts_[i].fontNumber == fontNumber)
            return i;

    return 0;
}

Bool32 CEDPage::CreatePicture(int pictNumber, const CIF::Size& pictSize, EDSIZE pictGoal,
        int pictAlign, int type, void * data, int len) {
    PictureEntry * tmp = new PictureEntry;

    tmp->pictNumber = pictNumber;
    tmp->pictSize = pictSize;
    tmp->pictGoal = pictGoal;
    tmp->type = type;
    tmp->pictAlign = pictAlign;
    tmp->len = len;
    tmp->data = malloc(len);

    if (!tmp->data) {
        delete tmp;
        return FALSE;
    }

    memcpy(tmp->data, data, len);
    pictures_.push_back(tmp);
    return TRUE;
}

}

