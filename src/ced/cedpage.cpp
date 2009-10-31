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

#include "cedpage.h"
#include "cedsection.h"
#include "cedparagraph.h"
#include "cedline.h"
#include "cedchar.h"
#include "cedint.h"
#include "resource.h"

#include <cstring>

//step of expansion of font table
#define FONTS_STEPPING 5
//step of expansion of picture table
#define PICS_STEPPING  3
const int MAX_RTF_COLORS = 200;

namespace CIF {

CEDPage::CEDPage() {
    sizeOfImage.cx = sizeOfImage.cy = 0;
    dpi.cx = dpi.cy = 0;
    turn = 0;
    imageName = 0;
    pageNumber = 0;
    pageSizeInTwips.cx = pageSizeInTwips.cy = 0;
    pageBordersInTwips.top = pageBordersInTwips.bottom = pageBordersInTwips.left
            = pageBordersInTwips.right = 0;
    extData = 0;
    extDataLen = 0;
    sections = 0;
    curSect = 0;
    unrecogChar = 0;
    NumberOfSections = 0;
    fontsUsed = 0; //number of fonts used in table
    fontsCreated = 0; //number of fonts created in table
    fontTable = 0; //pointer to font table
    picsUsed = 0; //number of picture used in table
    picsCreated = 0; //number of pictures created in table
    picsTable = 0; //pointer to picture table
    resizeToFit = 0;
    recogLang = LANG_RUSENG;
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
        li = li1->next;
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
    se1 = se = GetSection(0);
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

    if (imageName)
        free(imageName);
}

CEDSection * CEDPage::InsertSection() {
    NumberOfSections++;
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

CEDSection * CEDPage::SetCurSection(CEDSection* _sect) {
    return curSect = _sect;
}

CEDSection * CEDPage::SetCurSection(int _number) {
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

CEDSection * CEDPage::GetSection(int _num) {
    CEDSection* ss;
    for (ss = sections; ss && ss->internalNumber != _num; ss = ss->next)
        ;
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
    for (ss = qq ? qq->lines : 0; ss && ss->internalNumber != _num; ss = ss->next)
        ;
    return ss;
}
CEDChar * CEDPage::GetChar(int _num) {
    CEDLine *qq = GetLine(0);
    while (qq && !qq->chars)
        qq = qq->next;
    int num = 0;
    CEDChar* ss;
    for (ss = qq ? qq->chars : 0; ss && num != _num; ss = ss->next)
        num++;
    return ss;
}

Bool CEDPage::GoToNextSection() {
    if (curSect && curSect->next) {
        curSect = curSect->next;
        return TRUE;
    }
    else
        return FALSE;
}

Bool CEDPage::GoToNextParagraph(Bool NonFictiveOnly) {
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
        }
        while (NonFictiveOnly && (curSect->curPara->type & FICTIVE) && curSect->curPara->next);
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

Bool CEDPage::GoToNextLine() {
    CEDLine * aa;
    if (curSect && curSect->curPara && curSect->curPara->curLine && (aa
            = curSect->curPara->curLine->next)) {
        CEDParagraph *qq = GetParagraph(curSect->curPara->curLine->next->parentNumber);
        CEDSection * ss = GetSection(qq->parentNumber);
        curSect = ss;
        curSect->curPara = qq;
        curSect->curPara->curLine = aa;
        return TRUE;
    }
    else
        return FALSE;
}

Bool CEDPage::GoToNextChar() {
    CEDChar * ww;
    if (curSect && curSect->curPara && curSect->curPara->curLine
            && curSect->curPara->curLine->curChar
            && (ww = curSect->curPara->curLine->curChar->next)) {
        CEDLine * aa = GetLine(curSect->curPara->curLine->curChar->next->parentNumber);
        CEDParagraph *qq = GetParagraph(aa->parentNumber);
        CEDSection * ss = GetSection(qq->parentNumber);
        curSect = ss;
        curSect->curPara = qq;
        curSect->curPara->curLine = aa;
        curSect->curPara->curLine->curChar = ww;
        return TRUE;
    }
    else
        return FALSE;
}

int CEDPage::GetNumberOfSections() {
    return NumberOfSections;
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
    for (CEDLine *qq = GetLine(0); qq->next; qq = qq->next)
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

Bool CEDPage::CreateFont(uchar fontNumber, uchar fontPitchAndFamily, uchar fontCharset,
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

Bool CEDPage::GetFont(int number, uchar* fontNumber, uchar* fontPitchAndFamily, uchar* fontCharset,
        char** fontName) {
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

Bool CEDPage::CreatePicture(int pictNumber, EDSIZE pictSize, EDSIZE pictGoal, int pictAlign,
        int type, void * data, int len) {
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

Bool32 CEDPage::FormattedWriteRtf(const char * fileName) {
    Bool ret;
    StrRtfOut *rtf = NULL;
    StrRtfColor *color = NULL;
    int j;
    CEDSection* sect;

    // allocate space for the Strrtf info structure
    rtf = new StrRtfOut;

    //open new rtf
    rtf->hFile = fopen(fileName, "wb");
    if (!rtf->hFile) {
        SetReturnCode_ced(0);
        return FALSE;
    }

    // initialize global variables
    rtf->RtfInHdrFtr = 0;
    rtf->RtfInTable = FALSE;
    rtf->page = this;

    rtf->PrevChar.fontHeight = 24;
    rtf->PrevChar.fontNum = -1;
    rtf->PrevChar.foregroundColor = -1;
    rtf->PrevChar.backgroundColor = -1;

    rtf->table = new int[rtf->page->fontsUsed];
    memset(rtf->table, -1, sizeof(int) * rtf->page->fontsUsed);

    // allocate color table
    if (NULL == (color = rtf->color = new StrRtfColor[MAX_RTF_COLORS])) {
        return 0;
    }
    memset(color, 0, sizeof(struct StrRtfColor) * MAX_RTF_COLORS); // initialize with zeros

    // write the rtf header
    if (!BeginRtfGroup(rtf))
        goto WRITE_END;
    if (!WriteRtfControl(rtf, "rtf", PARAM_INT, 1))
        goto WRITE_END;
    if (!WriteRtfControl(rtf, "ansi", PARAM_NONE, 0))
        goto WRITE_END;
    if (!WriteRtfControl(rtf, "deff", PARAM_INT, 0))
        goto WRITE_END;
    if (!WriteRtfControl(rtf, "deflang", PARAM_INT, 1024))
        goto WRITE_END;
    //fill in font table with default values
    for (int i = 0; i < rtf->page->fontsUsed; i++)
        rtf->table[i] = rtf->page->fontTable[i].fontNumber;
    rtf->maxFntNum = -1;
    if (!WriteRtfFont(rtf, TRUE))
        goto WRITE_END; //write the font table
    if (!WriteRtfColor(rtf))
        goto WRITE_END;
    //write the color table
    //    if (!WriteRtfStylesheet(w,rtf)) goto WRITE_END; //write the color table
    if (!WriteRtfMargin(rtf))
        goto WRITE_END;
    //write default margin and paper information

    sect = GetSection(0);
    rtf->PrevPfmt = DEFAULT_PFMT;

    // write the text lines
    for (; sect > 0; sect = sect->next) {
        if (!WriteRtfSection(rtf, sect))
            goto WRITE_END; //write section properties
        //  int sectNum=0;
        for (int colNum = 0; colNum < sect->numberOfColumns; colNum++) {
            CEDParagraph* col = sect->GetColumn(colNum);

            Bool firstParaGraph = TRUE;//process first element in a row - for correct paragraph convertation
            CEDParagraph* para = col->GetFirstObject();
            while (para) {
                if (para->type & FICTIVE) {
                    if (para->type == TAB_BEGIN) {
                        CEDParagraph * prevRow = 0;
                        for (int tblRowNum = 0; tblRowNum
                                < ((EDTABDESCR*) (para)->descriptor)->numOfRows; tblRowNum++) {
                            CEDParagraph * tblRow = para->GetRow(tblRowNum);
                            WriteRtfRow(rtf, tblRow, prevRow);
                            prevRow = tblRow;
                            for (int tblCellNum = 0; tblCellNum
                                    < ((EDROWDESCR*) (tblRow)->descriptor)->numOfCells; tblCellNum++) {
                                CEDParagraph* hPara = tblRow->GetCell(tblCellNum);
                                CEDParagraph* para1 = hPara->GetFirstObject();
                                rtf->reset = TRUE;
                                if (!WriteRtfControl(rtf, "pard", PARAM_NONE, 0))
                                    goto WRITE_END;
                                while (para1) {
                                    if (!WriteRtfPara(rtf, para1, para1->GetNextObject()
                                            ? TRUE
                                            : FALSE))
                                        goto WRITE_END;
                                    para1 = para1->GetNextObject();
                                }
                                if (!WriteRtfControl(rtf, "cell", PARAM_NONE, 0))
                                    goto WRITE_END;
                            }
                            if (!WriteRtfControl(rtf, "row", PARAM_NONE, 0))
                                goto WRITE_END;
                            // write para break
                        }
                        rtf->RtfInTable = FALSE;
                        para = para->GetNextObject();
                        rtf->reset = TRUE;
                        if (!WriteRtfControl(rtf, "pard", PARAM_NONE, 0))
                            goto WRITE_END;
                    }
                    else if (para->type == FRAME_BEGIN) {
                        rtf->RtfInFrame = TRUE;
                        if (!WriteRtfControl(rtf, "pard", PARAM_NONE, 0))
                            goto WRITE_END;
                        rtf->reset = TRUE;
                        rtf->frm = para;
                        para = rtf->frm->GetFirstObject();
                        //if there is only one picture in a frame --- dont write frame's width and height
                        rtf->wrtFrmSz = TRUE;
                        //if there is only one paragraph in a frame and this paragraph is real.
                        if (para && (!para->GetNextObject()) && (!(para->type & FICTIVE))) {
                            CEDLine * l = para->lines;
                            //if there is only one line in a paragraph
                            if (l && ((!l->next) || l->next->parentNumber != l->parentNumber)) {
                                CEDChar * c = l->chars;
                                //the same for symbol
                                if (c && ((!c->next) || c->next->parentNumber != c->parentNumber)
                                        && CED_IsPicture(c))
                                    rtf->wrtFrmSz = FALSE;
                            }
                        }
                        //if there is only one picture in a frame --- dont write frame's width and height
                        //if there is only one paragraph in a frame and this paragraph is a table
                        if (para && (!para->GetNextObject()) && (para->type == TAB_BEGIN))
                            rtf->wrtFrmSz = FALSE;
                    }
                }
                else {
                    int shading = para->shading;
                    //if we found painted paragraph, which is first one in a non-first line
                    //then we fill in fake paragraph in order not to paint column break in word
                    if (colNum != 0 && firstParaGraph && rtf->PrevPfmt && shading
                            != rtf->PrevPfmt->shading) {
                        if (!WriteRtfControl(rtf, "pard", PARAM_NONE, 0))
                            goto WRITE_END;
                        if (!WriteRtfControl(rtf, "fs", PARAM_INT, 2))
                            goto WRITE_END;
                        rtf->PrevChar.fontHeight = 2;
                        if (!WriteRtfControl(rtf, "par", PARAM_NONE, 0))
                            goto WRITE_END;
                        rtf->reset = TRUE;
                    }
                    if (!WriteRtfPara(rtf, para, TRUE))
                        goto WRITE_END;
                    para = para->GetNextObject();
                }
                if (!para && rtf->RtfInFrame) {
                    para = rtf->frm->GetNextObject();
                    rtf->RtfInFrame = FALSE;
                    rtf->reset = TRUE;
                    if (!WriteRtfControl(rtf, "pard", PARAM_NONE, 0))
                        goto WRITE_END;
                }
                firstParaGraph = FALSE;
            }
            if (colNum != sect->numberOfColumns - 1)
                if (!WriteRtfControl(rtf, "column", PARAM_NONE, 0))
                    goto WRITE_END;
        }
        // write section break if any
        if (!WriteRtfControl(rtf, "pard", PARAM_NONE, 0))
            goto WRITE_END; // end any open table
        if (sect->next)
            if (!WriteRtfControl(rtf, "sect", PARAM_NONE, 0))
                goto WRITE_END;
        // end the previous section

        continue;
    }

    // terminate table in not terminated properly
    if (rtf->RtfInTable) {
        if (!WriteRtfControl(rtf, "sstblend", PARAM_NONE, 0))
            goto WRITE_END; // instructs the reader to ignore the following cell/row symbols
        if (!WriteRtfControl(rtf, "cell", PARAM_NONE, 0))
            goto WRITE_END;
        // write para break
        if (!WriteRtfControl(rtf, "row", PARAM_NONE, 0))
            goto WRITE_END;
        // write para break
    }

    // end all open groups
    for (j = rtf->GroupLevel; j > 0; j--)
        if (!EndRtfGroup(rtf))
            goto WRITE_END;

    ret = TRUE;
    SetReturnCode_ced(IDS_ERR_NO);
    goto FINAL;
    WRITE_END: SetReturnCode_ced(1);
    ret = FALSE;
    FINAL:
    // flush text buffer
    FlushRtfLine(rtf); // flush the rtf line to the output

    fclose(static_cast<FILE*> (rtf->hFile));

    if (rtf->table)
        delete[] rtf->table; // free rtf control area
    if (rtf)
        delete rtf; // free rtf control area
    if (color)
        delete[] color; // free the rtf color table

    return ret;
}

}
