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

#include <cstring>
#include <cstdlib>

#include "compat_defs.h"
#include "minmax.h"
#include "ced_struct.h"
#include "cedint.h"
#include "cedline.h"
#include "cedsection.h"
#include "cedparagraph.h"
#include "cedpage.h"
#include "resource.h"
#include "cfio/cfio.h"

using namespace CIF;

//First of all, we define all .dib as metafiles since word does not know about .dib
static CEDPage * mainPage;
static CEDLine * curEdLine;
static edBox refBox;
static int font, kegl, lang;
static int foregroundColor, backgroundColor, fontNum;
static char * verInfo;

static void ExtDataProc(uchar* _ptr, uint32_t lth);
void NewFormattedSDD(const sheet_disk_descr* pt);
void NewFormattedFDD(const fragm_disk_descr* pt);
void NewFormattedTR(const text_ref* pt);
void NewFormattedLB(const line_beg* pt);
void NewFormattedL(const letter* pt, const uint32_t alternatives);
void NewFormattedBMR(const bit_map_ref * pt);
void NewFormattedFK(const font_kegl * pt);
void NewFormattedE(const edExtention* pt, const void* ptExt);
void NewFormattedENew(const edExtentionNew* pt, const void* ptExt);
void NewFormattedLang(const EdTagLanguage* pt);

void CED_DeleteTree(CIF::CEDPage * pg) {
    delete pg;
}

void RepairStructure();

CIF::CEDPage * CED_FormattedLoad(char * file, Bool32 readFromFile, uint32_t bufLen) {
    CED_SetRawDataProc(ExtDataProc);

    if (CED_IsEdFile(file, readFromFile, bufLen) == 96) {
        return 0;
        //      return Formattedload_96(char * file,Bool32 readFromFile, uint32_t bufLen);
    }

    else if (CED_IsEdFile(file, readFromFile, bufLen) != 2000)
        return 0;

    CED_SheetDiskDescr = NewFormattedSDD;
    CED_TextRef = NewFormattedTR;
    //  CED_FragmDiskDescr=FormattedFDD;
    //  CED_FragmDisk=NewFormattedFD;
    CED_LineBeg = NewFormattedLB;
    CED_Letter = NewFormattedL;
    CED_BitmapRef = NewFormattedBMR;
    CED_FontKegl = NewFormattedFK;
    CED_Extention = NewFormattedE;
    CED_ExtentionNew = NewFormattedENew;
    CED_EdTagLanguage = NewFormattedLang;
    mainPage = new (CEDPage);
    refBox.x = refBox.y = refBox.h = refBox.w = 0;
    font = kegl = lang = -1;
    foregroundColor = backgroundColor = fontNum = -1;
    verInfo = 0;
    CED_ReadED(file, readFromFile, bufLen);

    if (verInfo && strcmp(verInfo, "CuneiForm2000 file format") == 0) {
        RepairStructure();
        delete[] verInfo;
    }

    else
        return 0;

    return mainPage;
}

//Put non-recognized codes to the corresponding field of extData
void ExtDataProc(uchar* /*_ptr*/, uint32_t /*lth*/) {
}

void NewFormattedSDD(const sheet_disk_descr* pt) {
    mainPage->setImageDpi(CIF::Size(pt->resolution, pt->resolution));
    mainPage->setPageNumber(pt->sheet_numb);
    mainPage->setTurn(pt->incline);
    mainPage->NumberOfParagraphs = pt->quant_fragm;
    mainPage->setTurn(pt->incline);
}

void NewFormattedE(const edExtention* pt, const void* ptExt) {
    switch (pt->Ecode) {
    case EDEXT_VERSION:
        verInfo = new char[pt->length - sizeof(edExtention)];
        memcpy(verInfo, ptExt, pt->length - sizeof(edExtention));
        break;
    case EDEXT_FONTS: {
        fontDiscr *fond = (fontDiscr *) ptExt;

        while ((char*) fond - (char*) ptExt < signed(pt->length - sizeof(edExtention))) {
            mainPage->CreateFont(fond->fontNumber, fond->fontPitchAndFamily, fond->fontCharset,
                    (char*) fond + sizeof(fontDiscr));
            fond = (fontDiscr *) ((char*) fond + fond->size);
        }

        break;
    }
    case EDEXT_BORDERS: {
        pageDescr* pd = (pageDescr*) ptExt;
        mainPage->pageSizeInTwips.cx = pd->paperw;
        mainPage->pageSizeInTwips.cy = pd->paperh;
        mainPage->pageBordersInTwips.top = pd->margt;
        mainPage->pageBordersInTwips.left = pd->margl;
        mainPage->pageBordersInTwips.bottom = pd->margb;
        mainPage->pageBordersInTwips.right = pd->margr;
        mainPage->resizeToFit = pd->resizeToFit;

        //for backward compatibility
        if (unsigned((&(pd->recogLang)) - ((uchar*) pd)) < pt->length - sizeof(edExtention))
            mainPage->recogLang = pd->recogLang;

        break;
    }
    case EDEXT_TIFF_DESC: {
        originalImageDesc* fond = (originalImageDesc*) ptExt;
        mainPage->setImageSize(CIF::Size(fond->width, fond->height));
        mainPage->setPageNumber(fond->pageNum);
        mainPage->setTurn(fond->inclune);
        mainPage->setImageDpi(CIF::Size(fond->resolutionX, fond->resolutionY));
        mainPage->unrecogChar = fond->unrecogSymbol;
        mainPage->imageName = strdup((char*) ptExt + sizeof(originalImageDesc));
        break;
    }
        /*  case EDEXT_PICS:
         {
         pictDescr *picd=(pictDescr *)ptExt;
         while((char*)picd-(char*)ptExt<pt->length-sizeof(edExtention))
         {
         mainPage->CreatePicture(picd->pictNumber, picd->pictSize, picd->pictGoal, picd->pictAlign, picd->type, (char*)picd+sizeof(pictDescr),picd->len);
         picd=(pictDescr *)(char*)picd+picd->size;
         }
         break;
         }
         */case EDEXT_SECTION: {
        CEDSection * sect = mainPage->InsertSection();
        sectParams1* sp = (sectParams1*) ptExt;
        sect->borders.bottom = sp->bottomMargin;
        sect->borders.top = sp->topMargin;
        sect->borders.left = sp->leftMargin;
        sect->borders.right = sp->rightMargin;
        sect->numberOfColumns = 0;//  sp->columns       ;
        sect->colInterval = sp->colInterval;
        sect->numSnakeCols = sp->numSnakeCols;
        sect->colInfo = new EDCOL[sect->numSnakeCols];
        char * c = (char*) ptExt + sizeof(sectParams1);
        int i;

        for (i = 0; i < sect->numSnakeCols; i++) {
            sect->colInfo[i].width = *((int32_t*) c);
            c += 4;
        }

        for (i = 0; i < sect->numSnakeCols; i++) {
            sect->colInfo[i].space = *((int32_t*) c);
            c += 4;
        }

        sectParams2* sp2 = (sectParams2*) c;
        sect->footerY = sp2->footerY;
        sect->headerY = sp2->headerY;
        sect->width = sp2->width;
        sect->height = sp2->height;
        sect->orientation = sp2->orientation;
        sect->sectionBreak = sp2->sectionBreak;

        //for backward compatibility
        if (unsigned((&(sp2->lineBetCol)) - ((uchar*) sp)) < pt->length - sizeof(edExtention))
            sect->lineBetCol = sp2->lineBetCol;

        break;
    }
    case EDEXT_PARAGRAPH: {
        paraParams * pard = (paraParams *) ptExt;
        CEDParagraph* hPara = mainPage->GetCurSection()->GetCurParagraph();
        hPara->color = (signed short) pard->color;
        hPara->interval.cx = pard->spaceBefore;
        hPara->interval.cy = pard->spaceAfter;
        hPara->alignment = pard->alignment;
        hPara->indent.rtop() = pard->firstIndent;
        hPara->indent.rleft() = pard->leftIndent;
        hPara->indent.rright() = pard->rightIndent;
        hPara->keep = pard->keep;
        hPara->shading = (signed short) pard->shading;
        hPara->spaceBetweenLines = pard->spaceBetweenLines;
        hPara->spcBtwLnsMult = pard->spcBtwLnsMult;
        hPara->userNumber = (signed short) pard->userNum;

        //for backward compatibility
        if (unsigned((&(pard->topBrdrType)) - ((uchar*) pard)) < pt->length - sizeof(edExtention))
            hPara->topBrdrType = pard->topBrdrType;

        if (unsigned((uchar*) (&(pard->topBrdrWidth)) - ((uchar*) pard)) < pt->length
                - sizeof(edExtention))
            hPara->topBrdrWidth = pard->topBrdrWidth;

        if (unsigned((&(pard->bottomBrdrType)) - ((uchar*) pard)) < pt->length
                - sizeof(edExtention))
            hPara->bottomBrdrType = pard->bottomBrdrType;

        if (unsigned((uchar*) (&(pard->bottomBrdrWidth)) - ((uchar*) pard)) < pt->length
                - sizeof(edExtention))
            hPara->bottomBrdrWidth = pard->bottomBrdrWidth;

        if (unsigned((&(pard->leftBrdrType)) - ((uchar*) pard)) < pt->length - sizeof(edExtention))
            hPara->leftBrdrType = pard->leftBrdrType;

        if (unsigned((uchar*) (&(pard->leftBrdrWidth)) - ((uchar*) pard)) < pt->length
                - sizeof(edExtention))
            hPara->leftBrdrWidth = pard->leftBrdrWidth;

        if (unsigned((&(pard->rightBrdrType)) - ((uchar*) pard)) < pt->length - sizeof(edExtention))
            hPara->rightBrdrType = pard->rightBrdrType;

        if (unsigned((uchar*) (&(pard->rightBrdrWidth)) - ((uchar*) pard)) < pt->length
                - sizeof(edExtention))
            hPara->rightBrdrWidth = pard->rightBrdrWidth;

        break;
    }
    case EDEXT_TABLE_ROW: {
        CEDParagraph *hPara = mainPage->GetCurSection()->GetCurParagraph();
        edRowDescr *rd = (edRowDescr*) (hPara)->descriptor;
        rowParam* rp = (rowParam*) ptExt;
        rd->topBrdrType = rp->topBrdrType;
        rd->topBrdrWidth = rp->topBrdrWidth;
        rd->bottomBrdrType = rp->bottomBrdrType;
        rd->bottomBrdrWidth = rp->bottomBrdrWidth;
        rd->leftBrdrType = rp->leftBrdrType;
        rd->leftBrdrWidth = rp->leftBrdrWidth;
        rd->rightBrdrType = rp->rightBrdrType;
        rd->rightBrdrWidth = rp->rightBrdrWidth;
        rd->gaph = rp->gaph;
        rd->header = rp->header;
        rd->header = rp->left;
        rd->position = rp->position;
        rd->rowHeight = rp->rowHeight;
        rd->left = rp->left;
        break;
    }
    case EDEXT_TABLE_CELL: {
        CEDParagraph *hPara = mainPage->GetCurSection()->GetCurParagraph();
        cellParam *cp = (cellParam *) ptExt;
        edCellDescr *cd = (edCellDescr*) (hPara)->descriptor;
        cd->topBrdrType = cp->topBrdrType;
        cd->topBrdrWidth = cp->topBrdrWidth;
        cd->bottomBrdrType = cp->bottomBrdrType;
        cd->bottomBrdrWidth = cp->bottomBrdrWidth;
        cd->leftBrdrType = cp->leftBrdrType;
        cd->leftBrdrWidth = cp->leftBrdrWidth;
        cd->rightBrdrType = cp->rightBrdrType;
        cd->rightBrdrWidth = cp->rightBrdrWidth;
        cd->cellX = cp->cellX;
        cd->color = (signed short) cp->color;
        cd->shading = (signed short) cp->shading;
        cd->merging = cp->merging;
        cd->vertTextAlign = cp->vertTextAlign;

        //for backward compatibility
        if (unsigned((&(cp->flag)) - ((uchar*) cp)) < pt->length - sizeof(edExtention))
            cd->flag = cp->flag;

        break;
    }
    case EDEXT_FRAME: {
        CEDParagraph *hPara = mainPage->GetCurSection()->GetCurParagraph();
        edFrameDescr *fd = (edFrameDescr*) (hPara)->descriptor;
        frameParam* fp = (frameParam*) ptExt;
        fd->rec.x = fp->posx;
        fd->rec.y = fp->posy;
        fd->rec.w = fp->absw;
        fd->rec.h = fp->absh;
        fd->borderSpace = fp->borderSpace;
        fd->dxfrtextx = fp->dxfrtextx;
        fd->dxfrtexty = fp->dxfrtexty;
        fd->position = fp->position;

        //for backward compatiility
        if (unsigned((&(fp->flag)) - ((uchar*) fp)) < pt->length - sizeof(edExtention))
            fd->flag = fp->flag;

        break;
    }
    case EDEXT_CHAR: {
        charParams* chp = (charParams*) ptExt;
        fontNum = chp->fontNumber;
        foregroundColor = chp->foregroundColor;

        //for backward compatibility
        if (unsigned((uchar*) (&(chp->backgroundColor)) - ((uchar*) chp)) < pt->length
                - sizeof(edExtention))
            backgroundColor = chp->backgroundColor;

        else
            backgroundColor = -1;

        break;
    }
    }
}
void NewFormattedENew(const edExtentionNew* pt, const void* ptExt) {
    switch (pt->Ecode) {
    case EDEXT_PICS: {
        pictDescr *picd = (pictDescr *) ptExt;

        while ((char*) picd - (char*) ptExt < signed(pt->length - sizeof(edExtentionNew))) {
            mainPage->CreatePicture(picd->pictNumber, picd->pictSize, picd->pictGoal,
                    picd->pictAlign, picd->type, (char*) picd + sizeof(pictDescr), picd->len);
            picd = (pictDescr *) ((char*) picd + picd->size);
        }

        break;
    }
    }
}
void NewFormattedTR(const text_ref* pt) {
    switch (pt->type) {
    case SSR_FRAG_TYPE: {
        CEDParagraph * para = mainPage->GetCurSection()->InsertParagraph();

        switch (pt->object) {
        case TP_MCOL_BEG:
            para->type = COLUMN_BEGIN;
            para->descriptor = malloc(sizeof(EDCOLDESCR));
            break;
        case TP_NEW_COL:
            para->type = COLUMN_BEGIN;
            para->descriptor = malloc(sizeof(EDCOLDESCR));
            break;
        case TP_MCOL_END:
            para->type = LAST_IN_COLUMN;
            break;
        case TP_NEW_TAB_BEG:
            para->type = TAB_BEGIN;
            para->descriptor = malloc(sizeof(EDTABDESCR));
            break;
        case TP_NEW_ROW_BEG:
            para->type = TAB_ROW_BEGIN;
            para->descriptor = malloc(sizeof(EDROWDESCR));
            break;
        case TP_NEW_CELL_BEG:
            para->type = TAB_CELL_BEGIN;
            para->descriptor = malloc(sizeof(EDCELLDESCR));
            break;
        case TP_NEW_TAB_END:
            para->type = TAB_END;
            break;
        case TP_FRAME_BEG:
            para->type = FRAME_BEGIN;
            para->descriptor = malloc(sizeof(EDFRAMEDESCR));
            break;
        case TP_FRAME_END:
            para->type = FRAME_END;
            break;
        }

        break;
    }
    case SSR_LINE_FN: {
        CEDParagraph *hPara = mainPage->GetParagraph(pt->object);
        curEdLine = hPara->InsertLine();
        break;
    }
    }
}

void NewFormattedLB(const line_beg* pt) {
    curEdLine->defChrFontHeight = pt->height;
    curEdLine->setHardBreak(!pt->base_line);
}

void NewFormattedBMR(const bit_map_ref * pt) {
    refBox.h = pt->height;
    refBox.w = pt->width;
    refBox.x = pt->col;
    refBox.y = pt->row;
}

void NewFormattedFK(const font_kegl * pt) {
    font = pt->new_font;
    kegl = pt->new_kegl;
}

void NewFormattedLang(const EdTagLanguage* pt) {
    lang = pt->language;
}
void NewFormattedL(const letter* pt, const uint32_t alternatives) {
    if (!curEdLine)
        return;

    CEDLine *lin = curEdLine;
    /*  if(!mainPage->GetCurSection())
     mainPage->InsertSection()->CreateColumn();//In case of wrong 'ed', such that symbols are before the definition of fragment
     CEDLine * lin=mainPage->GetCurSection()->GetCurParagraph()->GetCurLine();
     if (!lin)
     lin=mainPage->GetCurSection()->GetCurParagraph()->InsertLine();//In case of wrong 'ed', such that symbols are before the definition of line
     */
    CEDChar *chr = lin->InsertChar();
    letterEx * lpData = new letterEx[alternatives];
    memcpy(lpData, (void*) pt, alternatives * sizeof(letterEx));
    chr->alternatives = /*(letter*)*/lpData;
    chr->numOfAltern = alternatives;
    chr->setBoundingRect(refBox);
    chr->setFontHeight(kegl);
    chr->fontAttribs = font;
    chr->fontNum = fontNum;
    chr->setFontLanguage(static_cast<language_t> (lang));
    chr->backgroundColor = backgroundColor;
    chr->foregroundColor = foregroundColor;
}

void RepairStructure() {
    CEDParagraph * tabBeg = 0, *rowBeg = 0, *cellBeg = 0, *colBeg = 0, *frmBeg = 0;
    CEDParagraph *hPara = mainPage->GetParagraph(0);

    while (hPara) {
        switch (hPara->type) {
        case TAB_BEGIN: {
            //Memorized beginning of table
            tabBeg = hPara;
            EDTABDESCR *td = (EDTABDESCR*) (tabBeg->descriptor);
            td->numOfRows = 0;
            td->cur = hPara;
            td->next = hPara;
            td->table = 0;
            break;
        }
        case TAB_ROW_BEGIN: {
            //If it is not a first line - refer to the current one
            if (rowBeg) {
                EDROWDESCR *rd = (EDROWDESCR*) (rowBeg->descriptor);
                rd->last = hPara;
            }

            //In a table of lines+1
            if (tabBeg) {
                EDTABDESCR *td = (EDTABDESCR*) (tabBeg->descriptor);
                td->numOfRows++;

                //If it is a first line
                if (!rowBeg)
                    td->next = hPara;

                td->cur = hPara;
            }

            rowBeg = hPara;
            EDROWDESCR *rd = (EDROWDESCR*) (rowBeg->descriptor);
            rd->numOfCells = 0;
            rd->next = hPara;
            rd->cur = hPara;
            cellBeg = 0;
            break;
        }
        case TAB_CELL_BEGIN: {
            if (cellBeg) {
                EDCELLDESCR *cd = (EDCELLDESCR*) (cellBeg->descriptor);
                cd->next = hPara;
            }

            if (rowBeg) {
                EDROWDESCR *rd = (EDROWDESCR*) (rowBeg->descriptor);
                rd->numOfCells++;

                if (!cellBeg)
                    rd->next = hPara;

                rd->cur = hPara;
            }

            cellBeg = hPara;
            break;
        }
        case TAB_END: {
            if (rowBeg) {
                EDROWDESCR *rd = (EDROWDESCR*) (rowBeg->descriptor);
                rd->last = hPara;
            }

            if (tabBeg) {
                EDTABDESCR *td = (EDTABDESCR*) (tabBeg->descriptor);
                td->last = hPara;
            }

            if (cellBeg) {
                EDCELLDESCR *cd = (EDCELLDESCR*) (cellBeg->descriptor);
                cd->next = hPara;
            }

            cellBeg = tabBeg = rowBeg = 0;
            break;
        }
        case FRAME_BEGIN: {
            frmBeg = hPara;
            break;
        }
        case FRAME_END: {
            EDFRAMEDESCR *fd = (EDFRAMEDESCR*) (frmBeg->descriptor);
            fd->last = hPara;
            frmBeg = 0;
            break;
        }
        case COLUMN_BEGIN: {
            CEDSection * sec = mainPage->GetSection(hPara->parentNumber);

            if (colBeg) {
                EDCOLDESCR *cd = (EDCOLDESCR*) (colBeg->descriptor);
                cd->next = hPara;
            }

            else
                sec->columnsBeg = hPara;

            colBeg = hPara;
            sec->numberOfColumns++;
            sec->columnsCur = hPara;
            break;
        }
        case LAST_IN_COLUMN: {
            if (colBeg) {
                EDCOLDESCR *cd = (EDCOLDESCR*) (colBeg->descriptor);
                cd->next = hPara;
            }

            colBeg = 0;
            mainPage->GetSection(hPara->parentNumber)->columnsEnd = hPara;
            break;
        }
        }

        hPara = hPara->next;
    }
}

#ifdef _DEBUG
void PrintPara(FILE *stream, Handle para);
void CED_ShowTree(char * name, Handle hEdPage)
{
    //  CEDPage * page=(CEDPage *)hEdPage;
    Bool32 inFrm = FALSE, inTbl = FALSE;
    Handle frm;
    Handle tblRow;
    unsigned int tblRowNum = 0, tblCellNum = 0;
    FILE *stream = fopen( name, "w" );
    fprintf(stream, "Page,dpi=%i,imName=%s,\ntw.x=%i,tw.y=%i,sz.x=%i,sz.y=%i,turn=%i,char=%c\n",
            CED_GetPageDpi(hEdPage).cx, CED_GetPageImageName(hEdPage), CED_GetPageSize(hEdPage).cx,
            CED_GetPageSize(hEdPage).cy, CED_GetPageImageSize(hEdPage).cx,
            CED_GetPageImageSize(hEdPage).cy, CED_GetPageTurn(hEdPage), CED_GetPageUnrecogChar(hEdPage));
    fprintf(stream, "\nTable of pictures:\nTotal pictures:%d", CED_GetNumOfPics(hEdPage));

    for (unsigned int p = 0; p < CED_GetNumOfPics(hEdPage); p++) {
        int pictNumber, pictAlign , type, length;
        CIF::Size pictSize;
        EDSIZE pictGoal;
        CED_GetPicture(hEdPage, p, &pictNumber, pictSize, &pictGoal, &pictAlign , &type, 0, &length);
        fprintf(stream, "\npictNumber=%d, type=%d, length=%d, pictSize.x=%d, pictSize.y=%d,\npictGoal.x=%d, pictGoal.y=%d, pictAlign=%d\n",
                pictNumber, type, length, pictSize.width(), pictSize.height(), pictGoal.cx, pictGoal.cy, pictAlign);
    }

    for (unsigned int i = 0; i < CED_GetCountSection(hEdPage); i++) {
        Handle sect = CED_GetSection(hEdPage, i);
        fprintf(stream, "\nSection %i,borders:l=%i,r=%i,t=%i,b=%i\n", i,
                CED_GetSectionBorder(sect).left, CED_GetSectionBorder(sect).right,
                CED_GetSectionBorder(sect).top, CED_GetSectionBorder(sect).bottom);
        fprintf(stream, "\ncolumns:");

        for (unsigned int c = 0; c < CED_GetNumSnakeCols(sect); c++)
        fprintf(stream, "\ncolumn %i,wd=%i,sp=%i", c,
                CED_GetSnakeColumnWidth(sect, c), CED_GetSnakeColumnSpacing(sect, c));

        for (unsigned int j = 0; j < CED_GetCountColumn(sect); j++) {
            Handle col = CED_GetColumn(sect, j);

            if (!col) {
                printf("Internal error in CED_GetColumn");
                continue;
            }

            fprintf(stream, "\nNewCol");
            Handle para = CED_GetFirstObject(col);

            while (para) {
                if (!CED_IsParagraph(para)) {
                    if (CED_IsTable(para)) {
                        fprintf(stream, "\nHere goes a table!!!!!!!!!!!!!!!!!!!!!!\n");

                        for (tblRowNum = 0; tblRowNum < CED_GetCountRow(para); tblRowNum++) {
                            tblRow = CED_GetTableRow(para, tblRowNum);
                            int left, heig, lb, rb, tb, bb, gp, pos;
                            CED_GetTableRowParams(tblRow, &left, &heig, &lb, 0, &rb, 0, &tb, 0, &bb, 0, &gp, &pos, 0);
                            fprintf(stream, "\nRow: left=%d,heig=%d,lb=%d,rb=%d,tb=%d,bb=%d,gp=%d,pos=%d", left, heig, lb, rb, tb, bb, gp, pos);

                            for (tblCellNum = 0; tblCellNum < CED_GetCountCell(tblRow); tblCellNum++) {
                                Handle hPara = CED_GetCell(tblRow, tblCellNum);
                                int cx, mg, va;
                                CED_GetCellParams(hPara, &cx, &mg, &va, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
                                int flag = CED_GetCellFlag(hPara);
                                char ss[100];
                                ss[0] = 0;

                                if (flag&ED_TDIR_UP)
                                strcpy(ss, ",ED_TDIR_UP");

                                if (flag&ED_TDIR_DOWN)
                                strcpy(ss, ",ED_TDIR_DOWN");

                                fprintf(stream, "\nCellX:%d,mrg:%d,vAlign:%d%s", cx, mg, va, ss);
                                Handle para1 = CED_GetFirstObject(hPara);

                                while (para1) {
                                    PrintPara(stream, para1);
                                    para1 = CED_GetNextObject(para1);
                                }

                                fprintf(stream, "\nHere is end of cell--------------------");
                            }

                            fprintf(stream, "\nHere is end of row========================");
                        }

                        fprintf(stream, "\nHere is end of table!!!!!!!!!!!!!!!!!!!!\n");
                        para = CED_GetNextObject(para);
                    }

                    else if (CED_IsFrame(para)) {
                        inFrm = TRUE;
                        frm = para;
                        EDBOX bx = CED_GetFrameRect(para);
                        fprintf(stream, "\nHere is a frame,x=%d,y=%d,w=%d,h=%d,pos=%x,dx=%d,dy=%d!!!!!!!!!\n", bx.x, bx.y, bx.w, bx.h, CED_GetFramePosition(para), CED_GetFrameDxfrtextx(para), CED_GetFrameDxfrtexty(para));
                        para = CED_GetFirstObject(frm);
                    }
                }

                else {
                    PrintPara(stream, para);
                    para = CED_GetNextObject(para);
                }

                if (!para && inFrm) {
                    para = CED_GetNextObject(frm);
                    inFrm = FALSE;
                    fprintf(stream, "Here is end of frame!!!!!!!!!!!!!!!!!!!!\n");
                }
            }
        }
    }

    fclose( stream );
}

void PrintPara(FILE *stream, Handle para)
{
    CIF::Rect indent = (static_cast<CIF::CEDParagraph*>(para))->indent;
    fprintf(stream, "\nparagraph,alig=%i,FInd=%d,LInd=%i,RInd=%d",
            CED_GetAlignment(para), indent.top(), indent.left(), indent.right());
    fprintf(stream, ",usNum=%i,sb=%i,sa=%i\n", CED_GetUserNumber(para), CED_GetInterval(para).cx, CED_GetInterval(para).cy);

    for (unsigned l = 0; l < CED_GetCountLine(para); l++) {
        Handle line = CED_GetLine(para, l);

        for (unsigned c = 0; c < CED_GetCountChar(line); c++) {
            Handle chr = CED_GetChar(line, c);

            if (!CED_IsPicture(chr))
            fprintf(stream, "%c", CED_GetAlternatives(chr)[0].alternative);

            else
            fprintf(stream, "\\pict%d\\", CED_GetCharFontNum(chr) - ED_PICT_BASE);
        }

        fprintf(stream, "\n");
    }
}
#endif

Bool32 WriteRemark(Handle hFile, int type, int object);
Bool32 WriteExtCode(Handle hFile, int Ecode, void* object, int lenOfObj, int etraLen = 0);
Bool32 WriteFontTable(Handle hFile, CIF::CEDPage *page);
Bool32 WriteTiffDescr(Handle hFile, CIF::CEDPage* page);
Bool32 WritePictTable(Handle hFile, CIF::CEDPage* page);
Bool32 WritePara(Handle hFile, CEDParagraph* hPara);

Bool32 CED_FormattedWrite(const char * fileName, CIF::CEDPage *page) {
    int ret;
    int fn;
    int sec;
    CEDChar *tmpChr;
    Handle hFile = CFIO_OpenFreeFile(0, fileName, OSF_CREATE | OSF_BINARY | OSF_WRITE);

    if (!hFile) {
        SetReturnCode_ced(CFIO_GetReturnCode());
        return FALSE;
    }

    //Write header
    sheet_disk_descr sdd;
    sdd.code = SS_SHEET_DESCR;
    sdd.quant_fragm = 1;
    sdd.sheet_numb = page->pageNumber();
    sdd.descr_lth = sizeof(sdd) + sizeof(fragm_disk_descr);
    sdd.resolution = (uint16_t) page->imageDpi().height();
    sdd.incline = page->turn();
    sdd.version = 2000;

    if (!CFIO_WriteToFile(hFile, (pchar) &sdd, sizeof(sdd)))
        goto ED_WRITE_END;

    fragm_disk_descr fdd;
    memset((void*) &fdd, 0, sizeof(fdd));
    fdd.code = SS_FRAGMENT;

    if (!CFIO_WriteToFile(hFile, (pchar) &fdd, sizeof(fdd)))
        goto ED_WRITE_END;

    //Write text description of version
    if (!WriteExtCode(hFile, EDEXT_VERSION, (void*) "CuneiForm2000 file format", strlen(
            "CuneiForm2000 file format") + 1))
        goto ED_WRITE_END;

    //Write table of fonts
    if (!WriteFontTable(hFile, page))
        goto ED_WRITE_END;

    //Write the boundaries of the page
    pageDescr pd;
    pd.paperw = page->pageSizeInTwips.cx;
    pd.paperh = page->pageSizeInTwips.cy;
    pd.margt = page->pageBordersInTwips.top;
    pd.margl = page->pageBordersInTwips.left;
    pd.margb = page->pageBordersInTwips.bottom;
    pd.margr = page->pageBordersInTwips.right;
    pd.resizeToFit = uchar(page->resizeToFit);
    pd.recogLang = page->recogLang;

    if (!WriteExtCode(hFile, EDEXT_BORDERS, &pd, sizeof(pd)))
        goto ED_WRITE_END;

    //Write parameters of .tif
    if (!WriteTiffDescr(hFile, page))
        goto ED_WRITE_END;

    //Write all available pictures
    if (!WritePictTable(hFile, page))
        goto ED_WRITE_END;

    //Write descriptions of sections and paragraphs.
    for (sec = 0; sec < page->GetNumberOfSections(); sec++) {
        CEDSection * sect = page->GetSection(sec);
        int i;
        sectParams1 sp;
        sp.bottomMargin = sect->borders.bottom;
        sp.topMargin = sect->borders.top;
        sp.leftMargin = sect->borders.left;
        sp.rightMargin = sect->borders.right;
        sp.columns = sect->numberOfColumns;
        sp.colInterval = sect->colInterval;
        sp.numSnakeCols = sect->numSnakeCols;

        if (!WriteExtCode(hFile, EDEXT_SECTION, &sp, sizeof(sp), sizeof(sectParams2)
                + sect->numSnakeCols * 8))
            goto ED_WRITE_END;

        for (i = 0; i < sect->numSnakeCols; i++)
            if (!CFIO_WriteToFile(hFile, (pchar) &(sect->colInfo[i].width), 4))
                goto ED_WRITE_END;

        for (i = 0; i < sect->numSnakeCols; i++)
            if (!CFIO_WriteToFile(hFile, (pchar) &(sect->colInfo[i].space), 4))
                goto ED_WRITE_END;

        sectParams2 sp2;
        sp2.footerY = sect->footerY;
        sp2.headerY = sect->headerY;
        sp2.width = sect->width;
        sp2.height = sect->height;
        sp2.orientation = sect->orientation;
        sp2.sectionBreak = sect->sectionBreak;
        sp2.lineBetCol = sect->lineBetCol;

        if (!CFIO_WriteToFile(hFile, (pchar) &sp2, sizeof(sp2)))
            goto ED_WRITE_END;

        if (!WriteRemark(hFile, SSR_FRAG_TYPE, TP_MCOL_BEG))
            goto ED_WRITE_END;

        if (!WriteRemark(hFile, SSR_FRAG_PNUM, MIN(sect->numberOfColumns, 1)))
            goto ED_WRITE_END;

        //Write headers of paragraphs in rows
        Bool32 fisrtCol = TRUE;//In order for it not to write NEW_COL in the beginning og first column
        CEDParagraph* hPara = sect->paragraphs;

        while (hPara && hPara->parentNumber == sec) {
            rowParam rp;
            edRowDescr *rd;
            cellParam cp;
            edCellDescr *cd;
            edFrameDescr *fd;
            frameParam fp;

            switch (hPara->type) {
            default:

                if (!WritePara(hFile, hPara))
                    goto ED_WRITE_END;

                break;
            case TAB_BEGIN:

                if (!WriteRemark(hFile, SSR_FRAG_TYPE, TP_NEW_TAB_BEG))
                    goto ED_WRITE_END;

                break;
            case TAB_ROW_BEGIN:

                if (!WriteRemark(hFile, SSR_FRAG_TYPE, TP_NEW_ROW_BEG))
                    goto ED_WRITE_END;

                rd = (edRowDescr*) (hPara)->descriptor;
                rp.topBrdrType = rd->topBrdrType;
                rp.topBrdrWidth = rd->topBrdrWidth;
                rp.bottomBrdrType = rd->bottomBrdrType;
                rp.bottomBrdrWidth = rd->bottomBrdrWidth;
                rp.leftBrdrType = rd->leftBrdrType;
                rp.leftBrdrWidth = rd->leftBrdrWidth;
                rp.rightBrdrType = rd->rightBrdrType;
                rp.rightBrdrWidth = rd->rightBrdrWidth;
                rp.gaph = rd->gaph;
                rp.header = uchar(rd->header);
                rp.left = rd->header;
                rp.position = rd->position;
                rp.rowHeight = rd->rowHeight;
                rp.left = rd->left;

                if (!WriteExtCode(hFile, EDEXT_TABLE_ROW, &rp, sizeof(rp)))
                    goto ED_WRITE_END;

                break;
            case TAB_CELL_BEGIN:

                if (!WriteRemark(hFile, SSR_FRAG_TYPE, TP_NEW_CELL_BEG))
                    goto ED_WRITE_END;

                cd = (edCellDescr*) (hPara)->descriptor;
                cp.topBrdrType = cd->topBrdrType;
                cp.topBrdrWidth = cd->topBrdrWidth;
                cp.bottomBrdrType = cd->bottomBrdrType;
                cp.bottomBrdrWidth = cd->bottomBrdrWidth;
                cp.leftBrdrType = cd->leftBrdrType;
                cp.leftBrdrWidth = cd->leftBrdrWidth;
                cp.rightBrdrType = cd->rightBrdrType;
                cp.rightBrdrWidth = cd->rightBrdrWidth;
                cp.cellX = cd->cellX;
                cp.color = cd->color;
                cp.shading = cd->shading;
                cp.merging = cd->merging;
                cp.vertTextAlign = cd->vertTextAlign;
                cp.flag = cd->flag;
                memcpy(&(cp.layout), &(cd->layout), sizeof(edBox));

                if (!WriteExtCode(hFile, EDEXT_TABLE_CELL, &cp, sizeof(cp)))
                    goto ED_WRITE_END;

                break;
            case TAB_END:

                if (!WriteRemark(hFile, SSR_FRAG_TYPE, TP_NEW_TAB_END))
                    goto ED_WRITE_END;

                break;
            case FRAME_BEGIN:

                if (!WriteRemark(hFile, SSR_FRAG_TYPE, TP_FRAME_BEG))
                    goto ED_WRITE_END;

                fd = (edFrameDescr*) (hPara)->descriptor;
                fp.posx = fd->rec.x;
                fp.posy = fd->rec.y;
                fp.absw = fd->rec.w;
                fp.absh = fd->rec.h;
                fp.borderSpace = fd->borderSpace;
                fp.dxfrtextx = fd->dxfrtextx;
                fp.dxfrtexty = fd->dxfrtexty;
                fp.position = fd->position;
                fp.flag = fd->flag;

                if (!WriteExtCode(hFile, EDEXT_FRAME, &fp, sizeof(fp), 0))
                    goto ED_WRITE_END;

                break;
            case FRAME_END:

                if (!WriteRemark(hFile, SSR_FRAG_TYPE, TP_FRAME_END))
                    goto ED_WRITE_END;

                break;
            case COLUMN_BEGIN:

                if (!fisrtCol)
                    if (!WriteRemark(hFile, SSR_FRAG_TYPE, TP_NEW_COL))
                        goto ED_WRITE_END;

                fisrtCol = FALSE;
                break;
            case LAST_IN_COLUMN:
                break;
            }

            hPara = hPara->next;
        }

        if (!WriteRemark(hFile, SSR_FRAG_TYPE, TP_MCOL_END))
            goto ED_WRITE_END;
    }

    if (!WriteRemark(hFile, SSR_FRAG_END, 0))
        goto ED_WRITE_END;

    //Write the file itself
    tmpChr = page->GetChar(0);

    if (tmpChr) {
        font_kegl fk;
        fk.code = SS_FONT_KEGL;
        font = fk.new_font = tmpChr->fontAttribs;
        kegl = fk.new_kegl = tmpChr->fontHeight();

        if (!CFIO_WriteToFile(hFile, (pchar) &fk, sizeof(fk)))
            goto ED_WRITE_END;

        charParams chp;
        chp.fontNumber = fontNum = tmpChr->fontNum;
        chp.foregroundColor = foregroundColor = tmpChr->foregroundColor;
        chp.backgroundColor = backgroundColor = tmpChr->backgroundColor;

        if (!WriteExtCode(hFile, EDEXT_CHAR, &chp, sizeof(chp)))
            goto ED_WRITE_END;

        EdTagLanguage fl;
        fl.code = SS_LANGUAGE;
        fl.language = lang = tmpChr->fontLanguage();

        if (!CFIO_WriteToFile(hFile, (pchar) &fl, sizeof(fl)))
            goto ED_WRITE_END;
    }

    else
        font = kegl = 0;

    fn = 0;
    CEDLine *line;

    if (line = page->GetLine(0)) {
        do {
            //CFIO_WriteToFile(hFile,(char*)&lb,sizeof(lb));
            if (!WriteRemark(hFile, SSR_LINE_FN, line->parentNumber))
                goto ED_WRITE_END;

            if (line->SetCurChar(0)) {
                CEDChar *chr = line->GetCurChar();

                do {
                    line->SetCurChar(chr);
                    bit_map_ref bmr;
                    bmr.code = SS_BITMAP_REF;
                    Rect bbox = chr->boundingRect();
                    bmr.col = bbox.left();
                    bmr.row = bbox.top();
                    bmr.height = bbox.height();
                    bmr.width = bbox.width();

                    if (!CFIO_WriteToFile(hFile, (pchar) (&bmr), sizeof(bmr)))
                        goto ED_WRITE_END;

                    if (chr->fontHeight() != kegl || chr->fontAttribs != font) {
                        font_kegl fk;
                        fk.code = SS_FONT_KEGL;
                        font = fk.new_font = chr->fontAttribs;
                        kegl = fk.new_kegl = chr->fontHeight();

                        if (!CFIO_WriteToFile(hFile, (pchar) &fk, sizeof(fk)))
                            goto ED_WRITE_END;
                    }

                    if (chr->fontNum != fontNum || chr->foregroundColor != foregroundColor
                            || chr->backgroundColor != backgroundColor) {
                        charParams chp;
                        chp.fontNumber = fontNum = chr->fontNum;
                        chp.foregroundColor = foregroundColor = chr->foregroundColor;
                        chp.backgroundColor = backgroundColor = chr->backgroundColor;

                        if (!WriteExtCode(hFile, EDEXT_CHAR, &chp, sizeof(chp)))
                            goto ED_WRITE_END;
                    }

                    if (chr->fontLanguage() != lang) {
                        EdTagLanguage fl;
                        fl.code = SS_LANGUAGE;
                        fl.language = lang = chr->fontLanguage();

                        if (!CFIO_WriteToFile(hFile, (pchar) &fl, sizeof(fl)))
                            goto ED_WRITE_END;
                    }

                    if (chr->alternatives) {
                        if (!CFIO_WriteToFile(hFile, (pchar) chr->alternatives, chr->numOfAltern
                                * sizeof(letterEx)))
                            goto ED_WRITE_END;
                    }

                    else {
                        letterEx l;
                        l.alternative = ' ';
                        l.probability = 254;

                        if (!CFIO_WriteToFile(hFile, (pchar) &l, sizeof(letterEx)))
                            goto ED_WRITE_END;
                    }

                    chr = line->NextChar(FALSE);
                } while (chr);
            }

            if (line->hardBreak() || line->defChrFontHeight > 0) {
                line_beg lb;
                lb.code = SS_LINE_BEG;
                lb.base_line = !line->hardBreak();
                lb.height = line->defChrFontHeight;

                if (!CFIO_WriteToFile(hFile, (pchar) &lb, sizeof(line_beg)))
                    goto ED_WRITE_END;
            }

            line = line->next;//para->NextLine(FALSE);
        } while (line);
    }

    SetReturnCode_ced(IDS_ERR_NO);
    ret = TRUE;
    goto FINAL;
    ED_WRITE_END: SetReturnCode_ced(CFIO_GetReturnCode());
    ret = FALSE;
    FINAL: CFIO_CloseFreeFile(hFile, CSF_SAVEDISK);
    return ret;
}

Bool32 WriteFontTable(Handle hFile, CIF::CEDPage* page) {
    char* ch = 0;
    //define the sum of lengths of all names of fonts
    int len = 0;
    int q;

    if (!(page->fontsUsed))
        return TRUE;

    for (q = 0; q < page->fontsUsed; q++) {
        page->GetFont(q, 0, 0, 0, &ch);

        if (ch)
            len += strlen(ch) + 1;
    }

    if (!WriteExtCode(hFile, EDEXT_FONTS, 0, 0, len + sizeof(fontDiscr) * page->fontsUsed))
        return FALSE;

    fontDiscr fond;

    for (q = 0; q < page->fontsUsed; q++) {
        page->GetFont(q, &(fond.fontNumber), &(fond.fontPitchAndFamily), &(fond.fontCharset), &ch);
        fond.size = strlen(ch) + 1 + sizeof(fontDiscr);

        //write info about font
        if (!CFIO_WriteToFile(hFile, (pchar) &fond, sizeof(fontDiscr)))
            return FALSE;

        //write font's name
        if (!CFIO_WriteToFile(hFile, (pchar) ch, strlen(ch) + 1))
            return FALSE;
    }

    return TRUE;
}

Bool32 WriteTiffDescr(Handle hFile, CEDPage* page) {
    originalImageDesc fond;
    fond.height = page->imageSize().height();
    fond.width = page->imageSize().width();
    fond.pageNum = page->pageNumber();
    fond.inclune = page->turn();
    fond.resolutionX = (uint16_t) page->imageDpi().width();
    fond.resolutionY = (uint16_t) page->imageDpi().height();
    fond.unrecogSymbol = page->unrecogChar;

    if (!WriteExtCode(hFile, EDEXT_TIFF_DESC, &fond, sizeof(fond), strlen(page->imageName) + 1))
        return FALSE;

    if (!CFIO_WriteToFile(hFile, (pchar) page->imageName, strlen(page->imageName) + 1))
        return FALSE;

    return TRUE;
}

Bool32 WritePictTable(Handle hFile, CEDPage* page) {
    //define sum of lengths of all pictures
    int len = 0;
    int q;

    if (!(page->picsUsed))
        return TRUE;

    for (q = 0; q < page->picsUsed; q++) {
        len += page->picsTable[q].len;
    }

    if (!WriteExtCode(hFile, EDEXT_PICS, 0, 0, len + sizeof(pictDescr) * page->picsUsed))
        return FALSE;

    pictDescr picd;

    for (q = 0; q < page->picsUsed; q++) {
        picd.pictAlign = page->picsTable[q].pictAlign;
        picd.pictGoal.cx = page->picsTable[q].pictGoal.cx;
        picd.pictGoal.cy = page->picsTable[q].pictGoal.cy;
        picd.pictNumber = page->picsTable[q].pictNumber;
        picd.pictSize = page->picsTable[q].pictSize;
        picd.len = page->picsTable[q].len;
        picd.type = page->picsTable[q].type;
        picd.size = page->picsTable[q].len + sizeof(picd);

        //write picture info.
        if (!CFIO_WriteToFile(hFile, (pchar) &picd, sizeof(picd)))
            return FALSE;

        //write picture
        if (!CFIO_WriteToFile(hFile, (pchar) page->picsTable[q].data, page->picsTable[q].len))
            return FALSE;
    }

    return TRUE;
}

Bool32 WritePara(Handle hFile, CEDParagraph* hPara) {
    paraParams pard;

    if (!WriteRemark(hFile, SSR_FRAG_TYPE, hPara->alignment))
        return FALSE;

    pard.color = hPara->color;
    pard.spaceBefore = hPara->interval.cx;
    pard.spaceAfter = hPara->interval.cy;
    pard.alignment = hPara->alignment;
    pard.firstIndent = hPara->indent.top();
    pard.leftIndent = hPara->indent.left();
    pard.rightIndent = hPara->indent.right();
    pard.keep = hPara->keep;
    pard.shading = hPara->shading;
    pard.spaceBetweenLines = hPara->spaceBetweenLines;
    pard.spcBtwLnsMult = hPara->spcBtwLnsMult;
    pard.userNum = hPara->userNumber;
    pard.topBrdrType = hPara->topBrdrType;
    pard.topBrdrWidth = hPara->topBrdrWidth;
    pard.bottomBrdrType = hPara->bottomBrdrType;
    pard.bottomBrdrWidth = hPara->bottomBrdrWidth;
    pard.leftBrdrType = hPara->leftBrdrType;
    pard.leftBrdrWidth = hPara->leftBrdrWidth;
    pard.rightBrdrType = hPara->rightBrdrType;
    pard.rightBrdrWidth = hPara->rightBrdrWidth;
    pard.brdrBtw = hPara->brdrBtw;

    if (!WriteExtCode(hFile, EDEXT_PARAGRAPH, &pard, sizeof(pard)))
        return FALSE;

    return TRUE;
}

//write lenOfObj+extraLen to the field corresponding to length
Bool32 WriteExtCode(Handle hFile, int Ecode, void* object, int lenOfObj, int extraLen) {
    if (!(Ecode & 0x8000)) {
        edExtention ext;
        ext.code = SS_EXTENTION;
        ext.Ecode = Ecode;
        ext.length = lenOfObj + extraLen + sizeof(ext);

        if (!CFIO_WriteToFile(hFile, (pchar) &ext, sizeof(ext)))
            return FALSE;
    }

    else {
        edExtentionNew ext;
        ext.code = SS_EXTENTION;
        ext.Ecode = Ecode;
        ext.length = lenOfObj + extraLen + sizeof(ext);

        if (!CFIO_WriteToFile(hFile, (pchar) &ext, sizeof(ext)))
            return FALSE;
    }

    if (lenOfObj)
        if (!CFIO_WriteToFile(hFile, (pchar) object, lenOfObj))
            return FALSE;

    return TRUE;
}

Bool32 WriteRemark(Handle hFile, int type, int object) {
    text_ref tr;
    tr.code = SS_REMARK;
    tr.type = type;
    tr.object = object;

    if (!CFIO_WriteToFile(hFile, (pchar) &tr, sizeof(tr)))
        return FALSE;

    return TRUE;
}

uint32_t CED_IsEdFile(char * file, Bool32 readFromFile, uint32_t bufLen) {
    Handle PedHandle;
    uint32_t len;
    puchar start;

    if (readFromFile) {
        len = CFIO_ReadMemoryFromFile(file, &PedHandle);

        if (len == 0)
            return 0;

        start = static_cast<puchar> (CFIO_LockMemory(PedHandle));

        if (!start) {
            CFIO_UnlockMemory(PedHandle);
            CFIO_FreeMemory(PedHandle);
            return 0;
        }
    }

    else {
        start = (uchar*) file;
        len = bufLen;
    }

    Bool32 ret = 96;

    if (len < sizeof(sheet_disk_descr) + sizeof(fragm_disk_descr) + 1) {
        ret = 0;
        goto END;
    }

    if (start[0] != 0x0a) {
        ret = 0;
        goto END;
    }

    if (start[sizeof(sheet_disk_descr)] != 0x0b) {
        ret = 0;
        goto END;
    }

    if (start[sizeof(sheet_disk_descr) + sizeof(fragm_disk_descr)] >= 0x20) {
        ret = 0;
        goto END;
    }

    if (((sheet_disk_descr*) start)->version = 2000)
        ret = 2000;

    END:

    if (readFromFile) {
        CFIO_UnlockMemory(PedHandle);
        CFIO_FreeMemory(PedHandle);
    }

    return ret;
}

