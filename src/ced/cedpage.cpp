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

//step of expansion of font table
#define FONTS_STEPPING 5
//step of expansion of picture table
#define PICS_STEPPING  3

namespace CIF
{

CEDPage::CEDPage() :
    turn_(0), page_number_(0), language_(LANGUAGE_RUS_ENG), unrecognized_char_(0), resize_to_fit_(
            false) {
    sections = 0;
    curSect = 0;
    section_num_ = 0;
    fontsUsed = 0; //number of fonts used in table
    fontsCreated = 0; //number of fonts created in table
    fontTable = 0; //pointer to font table
    picsUsed = 0; //number of picture used in table
    picsCreated = 0; //number of pictures created in table
    picsTable = 0; //pointer to picture table
}

CEDPage::~CEDPage() {
    CEDChar * ch1, *ch;
    ch1 = ch = GetChar(0);

    while (ch1) {
        ch = ch1->next;

        if (ch1->alternatives)
            delete[] ch1->alternatives;

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

    //delete font table
    int i;

    for (i = 0; i < fontsUsed; i++)
        free(fontTable[i].fontName);

    delete[] fontTable;

    //delete picture table
    for (i = 0; i < picsUsed; i++)
        free(picsTable[i].data);

    delete[] picsTable;
}

void CEDPage::clearSections() {
    delete sections;
    curSect = NULL;
    section_num_ = 0;
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

bool CEDPage::isResizeToFit() const {
    return resize_to_fit_;
}

language_t CEDPage::language() const {
    return language_;
}

Rect CEDPage::pageBorder() const {
    return page_borders_in_twips_;
}

int CEDPage::pageNumber() const {
    return page_number_;
}

Size CEDPage::pageSize() const {
    return page_size_in_twips_;
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
    page_borders_in_twips_ = borders;
}

void CEDPage::setPageNumber(int number) {
    page_number_ = number;
}

void CEDPage::setPageSize(const Size& size) {
    page_size_in_twips_ = size;
}

void CEDPage::setResizeToFit(bool value) {
    resize_to_fit_ = value;
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

int CEDPage::GetNumOfCurSection() {
    return curSect->internalNumber;
}

CEDSection * CEDPage::NextSection() {
    return curSect->next;
}

CEDSection * CEDPage::PrevSection() {
    return curSect->prev;
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

    while (qq && !qq->chars)
        qq = qq->next();

    int num = 0;
    CEDChar* ss;

    for (ss = qq ? qq->chars : 0; ss && num != _num; ss = ss->next)
        num++;

    return ss;
}

Bool32 CEDPage::GoToNextSection() {
    if (curSect && curSect->next) {
        curSect = curSect->next;
        return TRUE;
    }

    else
        return FALSE;
}

Bool32 CEDPage::GoToNextParagraph(Bool32 NonFictiveOnly) {
    if (curSect && curSect->curPara && curSect->curPara->next) {
        CEDParagraph * para = curSect->curPara;
        CEDSection* sect = curSect;

        do {
            if (curSect->curPara->next->parentNumber == curSect->curPara->parentNumber)
                curSect->curPara = curSect->curPara->next;

            else {
                curSect = curSect->next;
                curSect->curPara = curSect->paragraphs;
            }
        } while (NonFictiveOnly && (curSect->curPara->type & FICTIVE) && curSect->curPara->next);

        if (NonFictiveOnly && (curSect->curPara->type & FICTIVE)) {
            curSect = sect;
            curSect->curPara = para;
            return FALSE;
        }

        else
            return TRUE;
    }

    else
        return FALSE;
}

Bool32 CEDPage::GoToNextLine() {
    CEDLine * aa;

    if (curSect && curSect->curPara && curSect->curPara->curLine && (aa
            = curSect->curPara->curLine->next())) {
        CEDParagraph *qq = GetParagraph(curSect->curPara->curLine->next()->parentNumber());
        CEDSection * ss = section(qq->parentNumber);
        curSect = ss;
        curSect->curPara = qq;
        curSect->curPara->curLine = aa;
        return TRUE;
    }

    else
        return FALSE;
}

Bool32 CEDPage::GoToNextChar() {
    CEDChar * ww;

    if (curSect && curSect->curPara && curSect->curPara->curLine
            && curSect->curPara->curLine->currentChar() && (ww
            = curSect->curPara->curLine->currentChar()->next)) {
        CEDLine * aa = GetLine(curSect->curPara->curLine->currentChar()->next->parentNumber());
        CEDParagraph *qq = GetParagraph(aa->parentNumber());
        CEDSection * ss = section(qq->parentNumber);
        curSect = ss;
        curSect->curPara = qq;
        curSect->curPara->curLine = aa;
        curSect->curPara->curLine->setCurrentChar(ww);
        return TRUE;
    }

    else
        return FALSE;
}

int CEDPage::sectionCount() const {
    return section_num_;
}

int CEDPage::GetNumberOfParagraphs() {
    if (!GetParagraph(0))
        return 0;

    int i = 0;

    for (CEDParagraph *qq = GetParagraph(0); qq->next; qq = qq->next)
        i++;

    return i + 1;
}
int CEDPage::GetNumberOfLines() {
    if (!GetLine(0))
        return 0;

    int i = 0;

    for (CEDLine *qq = GetLine(0); qq->next(); qq = qq->next())
        i++;

    return i + 1;
}
int CEDPage::GetNumberOfChars() {
    if (!GetChar(0))
        return 0;

    int i = 0;

    for (CEDChar *qq = GetChar(0); qq->next; qq = qq->next)
        i++;

    return i + 1;
}

Bool32 CEDPage::CreateFont(uchar fontNumber, uchar fontPitchAndFamily, uchar fontCharset,
        char* fontName) {
    if (fontsUsed >= fontsCreated) {
        fontEntry* tmp;
        tmp = new fontEntry[fontsCreated + FONTS_STEPPING];

        if (!tmp)
            return FALSE;

        if (fontTable) {
            memcpy(tmp, fontTable, sizeof(fontEntry) * fontsCreated);
            delete[] fontTable;
        }

        fontsCreated += FONTS_STEPPING;
        fontTable = tmp;
    }

    fontTable[fontsUsed].fontNumber = fontNumber;
    fontTable[fontsUsed].fontPitchAndFamily = fontPitchAndFamily;
    fontTable[fontsUsed].fontCharset = fontCharset;

    if (!fontName)
        return FALSE;

    fontTable[fontsUsed].fontName = strdup(fontName);

    if (!fontTable[fontsUsed].fontName)
        return FALSE;

    fontsUsed++;
    return TRUE;
}

Bool32 CEDPage::GetFont(int number, uchar* fontNumber, uchar* fontPitchAndFamily,
        uchar* fontCharset, char** fontName) {
    if (number >= fontsUsed)
        return FALSE;

    if (fontNumber)
        *fontNumber = fontTable[number].fontNumber;

    if (fontPitchAndFamily)
        *fontPitchAndFamily = fontTable[number].fontPitchAndFamily;

    if (fontCharset)
        *fontCharset = fontTable[number].fontCharset;

    if (fontName)
        *fontName = fontTable[number].fontName;

    return TRUE;
}

int CEDPage::GetFontByNum(uchar fontNumber) {
    for (int i = 0; i < fontsUsed; i++)
        if (fontTable[i].fontNumber == fontNumber)
            return i;

    return 0;
}

Bool32 CEDPage::CreatePicture(int pictNumber, const CIF::Size& pictSize, EDSIZE pictGoal,
        int pictAlign, int type, void * data, int len) {
    if (picsUsed >= picsCreated) {
        pictEntry* tmp;
        tmp = new pictEntry[picsCreated + PICS_STEPPING];

        if (!tmp)
            return FALSE;

        if (picsTable) {
            memcpy(tmp, picsTable, sizeof(pictEntry) * picsCreated);
            delete[] picsTable;
        }

        picsCreated += PICS_STEPPING;
        picsTable = tmp;
    }

    picsTable[picsUsed].pictNumber = pictNumber;
    picsTable[picsUsed].pictSize = pictSize;
    picsTable[picsUsed].pictGoal = pictGoal;
    picsTable[picsUsed].type = type;
    picsTable[picsUsed].pictAlign = pictAlign;
    picsTable[picsUsed].len = len;
    picsTable[picsUsed].data = malloc(len);

    if (!picsTable[picsUsed].data)
        return FALSE;

    memcpy(picsTable[picsUsed].data, data, len);
    picsUsed++;
    return TRUE;
}

}

