/*
 Copyright (c) 1993-2008, Cognitive Technologies
 All rights reserved.

 Разрешается повторное распространение и использование как в виде исходного кода,
 так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

 * При повторном распространении исходного кода должны оставаться указанное
 выше уведомление об авторском праве, этот список условий и последующий
 отказ от гарантий.
 * При повторном распространении двоичного кода в документации и/или в
 других материалах, поставляемых при распространении, должны сохраняться
 указанная выше информация об авторском праве, этот список условий и
 последующий отказ от гарантий.
 * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
 быть использованы в качестве средства поддержки и/или продвижения
 продуктов, основанных на этом ПО, без предварительного письменного
 разрешения.

 ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
 ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
 ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
 ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
 МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
 НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
 ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
 НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
 СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
 ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
 НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
 ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 * Neither the name of the Cognitive Technologies nor the names of its
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//Filename ced_struct_implem.cpp
//Created 7.12.98 by Bozhenov Artem,(c) CT inc.
#include <cstring>
#include <cstdlib>
#include <iostream>

#include "ced_struct.h"
#include "cedint.h"
#include "cedline.h"
//step of expansion of font table
#define FONTS_STEPPING 5
//step of expansion of picture table
#define PICS_STEPPING  3
CEDPage::CEDPage()
{
    sizeOfImage.cx = sizeOfImage.cy = 0;
    dpi.cx = dpi.cy = 0;
    turn = 0;
    imageName = 0;
    pageNumber = 0;
    pageSizeInTwips.cx = pageSizeInTwips.cy = 0;
    pageBordersInTwips.top = pageBordersInTwips.bottom
                             = pageBordersInTwips.left = pageBordersInTwips.right = 0;
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
    recogLang = LANGUAGE_RUS_ENG;
}

CEDPage::~CEDPage()
{
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

CEDSection * CEDPage::InsertSection()
{
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
/*
 CEDSection * CEDPage::DeleteSection(Bool32 _deleteSubItems)
 {
 if (!curSect)
 return 0;
 if (curSect->prev)
 {
 (curSect->prev)->next=curSect->next;
 (curSect->next)->prev=curSect->prev;
 if (!_deleteSubItems)
 {
 for(CEDParagraph* para = curSect->paragraphs;para;para=para->next)
 para->internalNumber--;
 }
 else
 {

 for(CEDSection * sect=curSect->next;sect;sect=sect->next)
 sect->internalNumber--;
 */

CEDSection * CEDPage::SetCurSection(CEDSection* _sect)
{
    return curSect = _sect;
}

CEDSection * CEDPage::SetCurSection(int _number)
{
    CEDSection* sect;

    for (sect = sections; sect && sect->internalNumber != _number; sect
            = sect->next)
        ;

    curSect = sect;
    return sect;
}

CEDSection * CEDPage::GetCurSection()
{
    return curSect;
}

int CEDPage::GetNumOfCurSection()
{
    return curSect->internalNumber;
}

CEDSection * CEDPage::NextSection()
{
    return curSect->next;
}

CEDSection * CEDPage::PrevSection()
{
    return curSect->prev;
}

CEDSection * CEDPage::GetSection(int _num)
{
    CEDSection* ss;

    for (ss = sections; ss && ss->internalNumber != _num; ss = ss->next)
        ;

    return ss;
}
CEDParagraph * CEDPage::GetParagraph(int _num)
{
    CEDSection * qq = sections;

    while (qq && !qq->paragraphs)
        qq = qq->next;

    CEDParagraph* ss;

    for (ss = qq ? qq->paragraphs : 0; ss && ss->internalNumber != _num; ss
            = ss->next)
        ;

    return ss;
}
CEDLine * CEDPage::GetLine(int _num)
{
    CEDParagraph *qq = GetParagraph(0);

    while (qq && !qq->lines)
        qq = qq->next;

    CEDLine* ss;

    for (ss = qq ? qq->lines : 0; ss && ss->internalNumber != _num; ss
            = ss->next)
        ;

    return ss;
}
CEDChar * CEDPage::GetChar(int _num)
{
    CEDLine *qq = GetLine(0);

    while (qq && !qq->chars)
        qq = qq->next;

    int num = 0;
    CEDChar* ss;

    for (ss = qq ? qq->chars : 0; ss && num != _num; ss = ss->next)
        num++;

    return ss;
}

Bool32 CEDPage::GoToNextSection()
{
    if (curSect && curSect->next) {
        curSect = curSect->next;
        return TRUE;
    }

    else
        return FALSE;
}

Bool32 CEDPage::GoToNextParagraph(Bool32 NonFictiveOnly)
{
    if (curSect && curSect->curPara && curSect->curPara->next) {
        CEDParagraph * para = curSect->curPara;
        CEDSection* sect = curSect;

        do {
            if (curSect->curPara->next->parentNumber
                    == curSect->curPara->parentNumber)
                curSect->curPara = curSect->curPara->next;

            else {
                curSect = curSect->next;
                curSect->curPara = curSect->paragraphs;
            }
        }
        while (NonFictiveOnly && (curSect->curPara->type & FICTIVE)
                && curSect->curPara->next);

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

Bool32 CEDPage::GoToNextLine()
{
    CEDLine * aa;

    if (curSect && curSect->curPara && curSect->curPara->curLine && (aa
                                                                     = curSect->curPara->curLine->next)) {
        CEDParagraph *qq = GetParagraph(
                               curSect->curPara->curLine->next->parentNumber);
        CEDSection * ss = GetSection(qq->parentNumber);
        curSect = ss;
        curSect->curPara = qq;
        curSect->curPara->curLine = aa;
        return TRUE;
    }

    else
        return FALSE;
}

Bool32 CEDPage::GoToNextChar()
{
    CEDChar * ww;

    if (curSect && curSect->curPara && curSect->curPara->curLine
            && curSect->curPara->curLine->curChar && (ww
                                                      = curSect->curPara->curLine->curChar->next)) {
        CEDLine * aa = GetLine(
                           curSect->curPara->curLine->curChar->next->parentNumber);
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

int CEDPage::GetNumberOfSections()
{
    return NumberOfSections;
}
int CEDPage::GetNumberOfParagraphs()
{
    if (!GetParagraph(0))
        return 0;

    int i = 0;

    for (CEDParagraph *qq = GetParagraph(0); qq->next; qq = qq->next)
        i++;

    return i + 1;
}
int CEDPage::GetNumberOfLines()
{
    if (!GetLine(0))
        return 0;

    int i = 0;

    for (CEDLine *qq = GetLine(0); qq->next; qq = qq->next)
        i++;

    return i + 1;
}
int CEDPage::GetNumberOfChars()
{
    if (!GetChar(0))
        return 0;

    int i = 0;

    for (CEDChar *qq = GetChar(0); qq->next; qq = qq->next)
        i++;

    return i + 1;
}

Bool32 CEDPage::CreateFont(uchar fontNumber, uchar fontPitchAndFamily,
                           uchar fontCharset, char* fontName)
{
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

Bool32 CEDPage::GetFont(int number, uchar* fontNumber,
                        uchar* fontPitchAndFamily, uchar* fontCharset, char** fontName)
{
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

int CEDPage::GetFontByNum(uchar fontNumber)
{
    for (int i = 0; i < fontsUsed; i++)
        if (fontTable[i].fontNumber == fontNumber)
            return i;

    return 0;
}

Bool32 CEDPage::CreatePicture(int pictNumber, const CIF::Size& pictSize, EDSIZE pictGoal,
                              int pictAlign, int type, void * data, int len)
{
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

/////////////////////////////////////////////////////////////////////
CEDSection::CEDSection()
{
    numberOfColumns/*=numberOfFrames*/ = 0;
    //  pColSize=0;
    borders.top = borders.bottom = borders.right = borders.left = 0;
    sectionBreak = 0;
    width = 0;
    height = 0;
    orientation = 0;
    headerY = 0;
    footerY = 0;
    numSnakeCols = 0;
    lineBetCol = 0;
    colInfo = 0;
    extData = 0;
    extDataLen = 0;
    paragraphs = 0;
    curPara = 0;
    internalNumber = 0;
    prev = next = 0;
    /*framesEnd=framesBeg=framesCur=*/
    columnsBeg = columnsEnd = columnsCur = 0;
}

CEDSection::~CEDSection()
{
    /*  curPara=paragraphs;
     CEDParagraph * qq;
     while (curPara)
     {
     qq=curPara->next;
     delete curPara;
     curPara=qq;
     }
     */}

CEDParagraph * CEDSection::CreateColumn()
{
    if (!columnsBeg) {
        //      if (framesEnd)
        //          SetCurParagraph(framesEnd);
        //start column
        CEDParagraph * para = InsertParagraph();
        columnsBeg = para;
        para->type = COLUMN_BEGIN;
        EDCOLDESCR* colinf = (EDCOLDESCR *) malloc(sizeof(EDCOLDESCR));
        para->descriptor = colinf;
        //finish all columns
        CEDParagraph * para1 = InsertParagraph();
        columnsEnd = para1;
        para1->type = LAST_IN_COLUMN;
        ((EDCOLDESCR*) (para->descriptor))->next = para1;
        SetCurParagraph(columnsBeg);
        columnsCur = columnsBeg;
    }

    else {
        //start column
        SetCurParagraph(columnsEnd);
        CEDParagraph * para = InsertParagraph(FALSE);
        para->type = COLUMN_BEGIN;
        EDCOLDESCR* colinf = (EDCOLDESCR *) malloc(sizeof(EDCOLDESCR));
        para->descriptor = colinf;
        ((EDCOLDESCR*) (columnsCur->descriptor))->next = para;
        ((EDCOLDESCR*) (para->descriptor))->next = columnsEnd;
        columnsCur = para;
    }

    numberOfColumns++;
    return curPara;
}

CEDParagraph * CEDSection::GetColumn(int _num)
{
    if (_num > numberOfColumns)
        return 0;

    CEDParagraph *para = columnsBeg;

    for (int i = 0; i < _num; i++)
        para = (CEDParagraph*) ((EDCOLDESCR*) para->descriptor)->next;

    return para;
}

/*CEDParagraph * CEDSection::GetFrame( int _num)
 {
 if (_num>numberOfFrames)
 return 0;
 CEDParagraph *para=framesBeg;
 for (int i=0;i<_num;i++)
 para=(CEDParagraph*)((EDFRAMEDESCR*)para->descriptor)->next;
 return para;
 }

 */
CEDParagraph * CEDSection::CreateFrame(CEDParagraph* hObject, edBox rect,
                                       char position, int borderSpace, int dxfrtextx, int dxfrtexty)
{
    if (hObject->type != COLUMN_BEGIN) {
#ifdef _DEBUG
        std::cerr << "CED error: Attempt of frame creation outside of table";
#endif
        return 0;
    }

    EDCOLDESCR *colde = (EDCOLDESCR*) (hObject->descriptor);
#ifdef _DEBUG

    if (colde == 0) {
        std::cerr << "CED error: Attempt of frame creation in ordinary paragraph\n(not in column)";
        return 0;
    }

#endif

    //if this column is not not last one - go to column's last paragraph
    if (colde->next)
        SetCurParagraph((CEDParagraph *) (colde->next)->prev);

    //otherwise do the same
    else {
        if (hObject->type == COLUMN_BEGIN)
            SetCurParagraph(columnsEnd->prev);

        else
            return 0;
    }

    //      if (columnsEnd)
    //          SetCurParagraph(columnsEnd);
    //start frame
    CEDParagraph * para = InsertParagraph();
    para->type = FRAME_BEGIN;
    edFrameDescr* framinf = (EDFRAMEDESCR *) malloc(sizeof(EDFRAMEDESCR));
    framinf->rec.x = rect.x;
    framinf->rec.y = rect.y;
    framinf->rec.w = rect.w;
    framinf->rec.h = rect.h;
    framinf->position = position;
    framinf->borderSpace = borderSpace;
    framinf->dxfrtextx = dxfrtextx;
    framinf->dxfrtexty = dxfrtexty;
    framinf->flag = 0;
    para->descriptor = framinf;
    //finish frame
    CEDParagraph * para1 = InsertParagraph();
    //      framesEnd=para1;
    para1->type = FRAME_END;
    ((EDFRAMEDESCR *) (para->descriptor))->last = para1;
    SetCurParagraph(para/*framesBeg*/);
    //      framesCur=framesBeg;
    /*  if(!framesBeg)
     {
     framesBeg=para;
     }
     else
     {
     ((EDFRAMEDESCR*)(framesCur->descriptor))->nextFrame=para;
     }
     numberOfFrames++;*/
    return curPara;
}

CEDParagraph * CEDSection::CreateTable(CEDParagraph * hObject)
{
    if (hObject->type == TAB_BEGIN || hObject->type == TAB_CELL_BEGIN) {
#ifdef _DEBUG
        std::cerr << "CED error: Attempt of table creation in a table";
#endif
        return 0;
    }

    EDCOLDESCR *colde = (EDCOLDESCR*) (hObject->descriptor);

    if (colde == 0) {
#ifdef _DEBUG
        std::cerr << "CED error: Attempt of table creation in ordinary paragraph\n(not in column or frame)";
#endif
        return 0;
    }

    if (colde->next)
        SetCurParagraph((CEDParagraph *) (colde->next));

    else {
        //      switch(hObject->type)
        //      {
        /*      case FRAME_BEGIN:
         SetCurParagraph(framesEnd);
         break;
         *///       case COLUMN_BEGIN:
        //          SetCurParagraph(columnsEnd);
        //          break;
        //      default:
        return 0;
        //      }
    }

    CEDParagraph *para = InsertParagraph(FALSE);
    CEDParagraph *para1 = InsertParagraph();

    if (!para || !para1)
        return 0;

    para->type = TAB_BEGIN;
    para1->type = TAB_END;
    EDTABDESCR * td = (EDTABDESCR*) malloc(sizeof(EDTABDESCR));
    para->descriptor = (void*) td;
    td->next = para1;
    td->last = para1;
    td->cur = para;
    td->numOfRows = 0;
    td->table = 0;
    //  td->size=sz;
    //  td->linesX=(int*)malloc(sizeof(int)*(sz.cx+1));
    //  td->linesY=(int*)malloc(sizeof(int)*(sz.cy+1));
    //  memcpy(td->linesX,cx,(sz.cx+1)*sizeof(int));
    //  memcpy(td->linesY,cy,(sz.cy+1)*sizeof(int));
    //  td->table=(int*)malloc(sizeof(int)*sz.cx*sz.cy);
    //  memcpy(td->table,table,sz.cy*sz.cx*sizeof(int));
    //  td->horShow=(int*)malloc(sizeof(int)*(sz.cy+1));
    //  memcpy(td->horShow,bHorShow,(sz.cy+1)*sizeof(int));
    //  td->verShow=(int*)malloc(sizeof(int)*(sz.cx+1));
    //  memcpy(td->verShow,bHorShow,(sz.cx+1)*sizeof(int));
    SetCurParagraph(para);
    return curPara;
}
CEDParagraph * CEDSection::CreateTableRow(CEDParagraph * hTable, int left,
                                          int rowHeight, int leftBrdrType, int leftBrdrWidth, int rightBrdrType,
                                          int rightBrdrWidth, int topBrdrType, int topBrdrWidth,
                                          int bottomBrdrType, int bottomBrdrWidth, int gaph, int position,
                                          Bool32 header)
{
    if (hTable->type != TAB_BEGIN) {
#ifdef _DEBUG
        std::cerr << "CED error: Attempt to create table row outside of table";
#endif
        return 0;
    }

#ifdef _DEBUG

    if (!hTable->descriptor) {
        std::cerr << "CED error: Attempt to create table row in ordinary paragraph\n(not in table)";
        return 0;
    }

#endif
    //insert paragraph-header
    SetCurParagraph(((EDTABDESCR *) hTable->descriptor)->last);
    CEDParagraph * para = InsertParagraph(FALSE);

    if (!para)
        return 0;

    //number of rows in table+1
    ((EDTABDESCR *) (hTable->descriptor))->numOfRows++;
    //old current: next is the one just inserted
    CEDParagraph * para1 = ((EDTABDESCR *) hTable->descriptor)->cur;

    if (para1->type == TAB_BEGIN)
        ((EDTABDESCR *) para1->descriptor)->next = para;

    else
        ((EDROWDESCR *) para1->descriptor)->last = para;

    //next paragraph is the one just insterted
    ((EDTABDESCR *) hTable->descriptor)->cur = para;
    //create description
    edRowDescr * td = (EDROWDESCR*) malloc(sizeof(EDROWDESCR));
    //fill it in
    //next line is last paragraph in table
    td->last = para->next;
    //first cell
    td->next = para;
    //current cell --- to the beginning of line
    td->cur = para;
    td->numOfCells = 0;
    //put received data
    td->left = left;
    td->rowHeight = rowHeight;
    td->leftBrdrType = leftBrdrType;
    td->leftBrdrWidth = leftBrdrWidth;
    td->rightBrdrType = rightBrdrType;
    td->rightBrdrWidth = rightBrdrWidth;
    td->topBrdrType = topBrdrType;
    td->topBrdrWidth = topBrdrWidth;
    td->bottomBrdrType = bottomBrdrType;
    td->bottomBrdrWidth = bottomBrdrWidth;
    td->gaph = gaph;
    td->position = position;
    td->header = header;
    //put type and description of paragraph
    para->descriptor = (void*) td;
    para->type = TAB_ROW_BEGIN;
    return para;
}
/*
 CEDParagraph * CEDSection::ReCreateTable(CEDParagraph * hTable,EDSIZE sz, int * cx,int * cy, int * table,
 Bool32 * bHorShow,Bool32 * bVerShow)
 {

 EDTABDESCR * td=(EDTABDESCR *)hTable->descriptor;
 free(td->linesX);
 free(td->linesY);
 free(td->table);
 free(td->horShow);
 free(td->verShow);
 td->size=sz;
 td->linesX=(int*)malloc(sizeof(int)*(sz.cx+1));
 td->linesY=(int*)malloc(sizeof(int)*(sz.cy+1));
 memcpy(td->linesX,cx,(sz.cx+1)*sizeof(int));
 memcpy(td->linesY,cy,(sz.cy+1)*sizeof(int));
 td->table=(int*)malloc(sizeof(int)*sz.cx*sz.cy);
 memcpy(td->table,table,sz.cy*sz.cx*sizeof(int));
 td->horShow=(int*)malloc(sizeof(int)*(sz.cy+1));
 memcpy(td->horShow,bHorShow,(sz.cy+1)*sizeof(int));
 td->verShow=(int*)malloc(sizeof(int)*(sz.cx+1));
 memcpy(td->verShow,bHorShow,(sz.cx+1)*sizeof(int));
 SetCurParagraph(hTable);
 return curPara;
 }
 */

CEDParagraph * CEDSection::CreateCell(CEDParagraph* hRow, int cellX,
                                      int merging, int vertTextAlign, int leftBrdrType, int leftBrdrWidth,
                                      int rightBrdrType, int rightBrdrWidth, int topBrdrType,
                                      int topBrdrWidth, int bottomBrdrType, int bottomBrdrWidth,
                                      EDBOX layout, int shading, int color)
{
    if (hRow->type != TAB_ROW_BEGIN) {
#ifdef _DEBUG
        std::cerr << "CED error: Attempt to create cell of table outside of table's row";
#endif
        return 0;
    }

    CEDParagraph * para1 = SetCurParagraph(
                               ((EDTABDESCR *) hRow->descriptor)->last);
    CEDParagraph * para = InsertParagraph(FALSE);
    EDCELLDESCR * td = (EDCELLDESCR*) malloc(sizeof(EDCELLDESCR));
    //next cell is end of table/new line
    td->next = para1;
    para->descriptor = (void*) td;
    para->type = TAB_CELL_BEGIN;
    //number of cells++
    ((EDROWDESCR *) (hRow->descriptor))->numOfCells++;
    //previous cell:next=new
    para1 = ((EDTABDESCR *) hRow->descriptor)->cur;
    ((EDROWDESCR *) para1->descriptor)->next = para;
    //current cell in row is new one
    ((EDROWDESCR *) hRow->descriptor)->cur = para;
    td->cellX = cellX;
    td->merging = merging;
    td->vertTextAlign = vertTextAlign;
    td->leftBrdrType = leftBrdrType;
    td->leftBrdrWidth = leftBrdrWidth;
    td->rightBrdrType = rightBrdrType;
    td->rightBrdrWidth = rightBrdrWidth;
    td->topBrdrType = topBrdrType;
    td->topBrdrWidth = topBrdrWidth;
    td->bottomBrdrType = bottomBrdrType;
    td->bottomBrdrWidth = bottomBrdrWidth;
    memcpy(&(td->layout), &layout, sizeof(EDBOX));
    td->shading = shading;
    td->color = color;
    td->flag = 0;
    return para;
}

CEDParagraph * CEDSection::CreateParagraph(CEDParagraph * hObject, int align,
                                           EDRECT indent, int UserNum, int FlagBorder, EDSIZE interval,
                                           edBox layout, int color, int shading, int spaceBetweenLines,
                                           char spcBtwLnsMult, char keep)
{
    if (hObject->type != TAB_CELL_BEGIN && hObject->type != FRAME_BEGIN
            && hObject->type != COLUMN_BEGIN) {
#ifdef _DEBUG
        std::cerr << "CED error: Attempt to create paragraph in table's row or in table or in ordinary paragraph\n(not in column or frame or table's cell";
#endif
        return 0;
    }

    EDCOLDESCR *colde = (EDCOLDESCR*) (hObject->descriptor);
#ifdef _DEBUG

    if (!colde) {
        std::cerr << "CED error: Attempt to create paragraph in ordinary paragraph\n(not in column or in frame or in table's cell)";
        return 0;
    }

#endif

    if (colde->next)
        SetCurParagraph((CEDParagraph *) (colde->next));

    else {
        //      switch(hObject->type)
        //      {
        /*
         case FRAME_BEGIN:
         SetCurParagraph(framesEnd);
         break;
         */
        //      case COLUMN_BEGIN:
        //          SetCurParagraph(columnsEnd);
        //          break;
        //      default:
        return 0;
        //      }
    }

    CEDParagraph *para = InsertParagraph(FALSE);
    para->alignment = align;
    para->indent = indent;
    para->userNumber = UserNum;
    para->border = FlagBorder;
    para->color = color;
    para->shading = shading;
    para->spaceBetweenLines = spaceBetweenLines;
    para->spcBtwLnsMult = spcBtwLnsMult;
    para->keep = keep;
    memcpy(&(para->interval), &interval, sizeof(interval));
    memcpy(&(para->layout), &layout, sizeof(layout));
    return para;
}

CEDParagraph * CEDSection::InsertParagraph(Bool32 AfterCurrent)
{
    if (!AfterCurrent)
        curPara = curPara->prev;

    CEDParagraph * para = new CEDParagraph;
    para->parentNumber = internalNumber;

    if (curPara) {
        para->next = curPara->next;

        if (para->next)
            (para->next)->prev = para;

        curPara->next = para;
        para->prev = curPara;
        para->internalNumber = curPara->internalNumber + 1;

        for (CEDParagraph * para1 = para->next; para1; para1 = para1->next)
            para1->internalNumber++;
    }

    else {
        paragraphs = para;
        //      if(internalNumber!=0)
        //      {
        CEDSection *ww = prev;

        while (ww && !ww->paragraphs)
            ww = ww->prev;

        if (ww) {
            CEDParagraph *qq = ww->paragraphs;

            while (qq->next)
                qq = qq->next;

            qq->next = para;
            para->prev = qq;
            para->internalNumber = qq->internalNumber + 1;
        }

        ww = next;

        while (ww && !ww->paragraphs)
            ww = ww->next;

        if (ww) {
            CEDParagraph *qq = ww->paragraphs;
            qq->prev = para;
            para->next = qq;

            while (qq) {
                qq->internalNumber++;
                qq = qq->next;
            }
        }

        //      }
    }

    curPara = para;
    return para;
}

CEDParagraph * CEDSection::SetCurParagraph(CEDParagraph* _para)
{
    curPara = _para;
    return _para;
}

CEDParagraph * CEDSection::SetCurParagraph(int _number)
{
    int i = 0;

    if (paragraphs)
        i = paragraphs->internalNumber;

    CEDParagraph* para;

    for (para = paragraphs; para && para->internalNumber - i != _number; para
            = para->next)
        ;

    curPara = para;
    return para;
}

CEDParagraph * CEDSection::GetCurParagraph()
{
    return curPara;
}

int CEDSection::GetNumOfCurParagraph()
{
    return curPara->internalNumber;
}

CEDParagraph * CEDSection::NextParagraph(Bool32 _goThroughSect)
{
    if (_goThroughSect)
        return curPara->next;

    if (curPara->next && curPara->next->parentNumber == curPara->parentNumber)
        return curPara->next;

    else
        return 0;
}

CEDParagraph * CEDSection::PrevParagraph(Bool32 _goThroughSect)
{
    if (_goThroughSect)
        return curPara->prev;

    if (curPara->prev && curPara->prev->parentNumber == curPara->parentNumber)
        return curPara->prev;

    else
        return 0;
}
