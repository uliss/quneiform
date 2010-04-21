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

// ============================================================================
// Written by Shahverdiev  Alik
// This file cration date: 17.08.99
//
// CreateRtf.cpp
//
// ============================================================================
#include <cstdio>

#include "cfcompat.h"
#include "creatertf.h"
#include "globus.h"
#include "sys_prog.h"
#include "cpage/cpage.h"
#include "lst3_win.h"
#include "cfio/cfio.h"
#include "ced/ced.h"
#include "ced/cedpage.h"
#include "ced/cedline.h"
#include "ced/cedparagraph.h"
#include "edfile.h"
#include "dpuma.h"
#include "rtfedwrite.h"
#include "lns/lnsdefs.h"
#include "linedefs.h"
#include "normdefs.h"
#include "cstr/cstrdefs.h"
#include "cline/cline.h"

#include "crtfstruct.h"
#include "crtfchar.h"
#include "crtffragment.h"
#include "crtfpage.h"
#include "crtfsector.h"
#include "crtfstring.h"
#include "crtfhorizontalcolumn.h"
#include "crtfverticalcolumn.h"
#include "crtfword.h"
#include "crtffunc.h"
#include "crtfstruct.h"

#include "common/size.h"
#include "minmax.h"
#include "common/rect.h"
#include "common/debug.h"

extern Bool FullRtf(FILE *fpFileNameIn, const char *FileNameOut, Handle* hEdTree);
extern Bool PageTree(FILE *fpFileNameIn, CIF::CRtfPage* RtfPage, const char *FileNameOut);
extern Bool WriteTable(uint32_t IndexTable, RtfSectorInfo* SectorInfo, Bool OutPutMode);
extern Bool WritePict(uint32_t IndexPict, RtfSectorInfo* SectorInfo, Bool OutPutTypeFrame);
void GetTableRect(uint32_t NumberTable, Rect16* RectTable, uint32_t* UserNumber);
uchar GetPictRect(uint32_t NumberPict, Rect16* RectPict, uint32_t* UserNumber);
extern void RtfAssignRect_CRect_Rect16(RECT *s1, Rect16 *s2);
extern void RtfCalcRectSizeInTwips(RECT *s1, float Twips);
extern void RtfUnionRect_CRect_CRect(RECT *s1, RECT *s2);
extern void RtfAssignRect_CRect_CRect(RECT *s1, RECT *s2);

int16_t CreateEmptyRtfFile(void);

Bool ReadInternalFileRelease(FILE *fpFileNameIn, CIF::CRtfPage* RtfPage);
Handle Rtf_CED_CreateParagraph(int16_t FirstIndent, int16_t LeftIndent, int16_t RightIndent,
        int16_t IntervalBefore, RtfSectorInfo *SectorInfo, int AlignParagraph, int shad,
        int LenthStringInTwips, int LengthFragmInTwips);
void WriteCupDrop(CIF::CRtfChar* pRtfChar, int16_t font);
void Cleaning_LI_FRMT_Used_Flag(void);

float Twips;

extern char lpMyNameSerif[PATH_MAX];
extern char lpMyNameNonSerif[PATH_MAX];
extern char lpMyNameMono[PATH_MAX];
extern char WriteRtfImageName[PATH_MAX];
extern char RtfFileName[PATH_MAX];
extern uint32_t CountPict;
extern uint32_t CountTable;
extern uint32_t RtfWriteMode;
extern CIF::Point TemplateOffset;

#define CHEREDOVON

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FullRtf                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool FullRtf(FILE *fpFileNameIn, const char* FileNameOut, Handle* hEdTree) {
    CIF::CRtfPage RtfPage;

    if (RtfWriteMode) // is activated ONLY in debug mode (нажать ??? в LDPUMA)
        Cleaning_LI_FRMT_Used_Flag(); //обнуление флажков, что линии между колонок

    if (FlagMode & USE_FRAME_AND_COLUMN) {
        if (!RtfPage.FindPageTree(fpFileNameIn, FileNameOut))
            return FALSE;

        RtfPage.SetTwips();
    }

    else {
        if (!RtfPage.ReadInternalFile(fpFileNameIn))
            return FALSE;

        RtfPage.SetTwips();
        RtfPage.CorrectKegl();
        RtfPage.ChangeKegl();
    }

    //  RtfPage.AddTables();
    RtfPage.AddPictures();

    if (FlagMode & USE_NONE)
        RtfPage.SortUserNumber();//в ручном layout user can establish own order of the fragments

#ifndef EdWrite
    RtfPage.Write(FileNameOut);
#else

    if (RtfPage.Write(FileNameOut) && !RtfWriteMode)
        RtfPage.Rtf_CED_WriteFormattedEd(FileNameOut, hEdTree);

#endif
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 UTILS                                                          //
////////////////////////////////////////////////////////////////////////////////////////////////////
int16_t GetRealSizeKegl( /*CString**/const char* str, int16_t width, int16_t FontPointSize,
        int16_t FontNumber) {
    char* sz;
    uint16_t PenaltyKeglForString = 0;
    int16_t strHeight;
    float koef = 1.;
    int len = strlen(str);

    if (FontPointSize > 30) //~ в больших кеглях большая ошибка в Microsoft function
        //      *str +="     ";
        len += 5;

    else if (FontPointSize > 20)
        len++;

    //      *str +=" ";

    if (!(FlagMode & NOBOLD) && ((char) FontNumber & TG_EDW_BOLD))
        koef = float(4. / 5.);

    sz = new char[len + 1];
    //  if( str->GetLength() < MAX_BUFFER_SIZE )
    strcpy(sz, str);
    //  else
    //      return FALSE;
    //Заполняем хвост строки пробелами
    memset(sz + strlen(str), ' ', len - strlen(str));
    sz[len] = 0;

    if (FontPointSize > ChangedKeglSize) {
        int Count = FontPointSize - ChangedKeglSize;

        for (int i = 0; i < Count; i++) {
            int16_t RealSize = GetRealSize(sz, strlen(sz), FontPointSize, FontNumber, &strHeight);

            if (RealSize > (int16_t) (width * Twips * koef)) { //криретий окончания итераций- чтобы все символы влезли в строку по ширине
                PenaltyKeglForString++;
                FontPointSize--;
            }

            else
                break;
        }
    }

    delete[] sz;
    return FontPointSize;
}


int16_t GetRealSize(const char* str, int16_t len, int16_t FontSize, int16_t FontNumber,
        int16_t* strHeight) {
    HFONT testFont;
    CIF::Size size;
    int n_Weight = 600, fn;
    uchar bItalic;
    //  TEXTMETRIC  tm;

    if (!(FlagMode & NOBOLD) && ((char) FontNumber & TG_EDW_BOLD))
        n_Weight = 800;

    if (FlagMode & NOSIZE)
        FontSize = DefFontSize / 2;

    if (!(FlagMode & NOCURSIV) && ((char) FontNumber & TG_EDW_ITALIC))
        bItalic = TRUE;

    else
        bItalic = FALSE;

    fn = get_font_name(FontNumber);
    //  TODO
    *strHeight = size.height();
    return size.width(); //in twips
}

int16_t CreateEmptyRtfFile(void) {
    const char *TitleRtf = "\\rtf1\\ansi \\deff0\\deflang1024";
    char Eol[3], Nname[260];
    int16_t i;
    FONT_COD FontCod[9] = { "Arial Cyr", "fswiss", // NonSerif
            "Times New Roman Cyr", "froman", // Serif
            "Courier Cyr", "fmodern" // Fixed_Pitch
            };
    uint16_t cr = 13/*0x0d*/, lf = 10/*0x0a*/;

    if (RtfWriteMode) {
        Eol[0] = (char) cr;
        Eol[1] = (char) lf;
        Eol[2] = 0;

        if ((out = fopen((char*) RtfFileName, "w")) == NULL)
            return -6;

        PutChar(0);
        Put("{");
        Put(TitleRtf);
        Put(" ");
        Put(Eol);
        //--WRITE Font Table--
        Put("{\\fonttbl");
        Put(Eol);

        for (i = 0; i < 3; ++i) {
            Put("{");
            PutCom("\\f", i, 0);
            Put("\\");
            Put(FontCod[i].family);
            PutCom("\\fcharset", 0, 0);
            Put(" ");
            Put(FontCod[i].name);
            Put(";}");
            Put(Eol);
        }

        Put("}");
        Put(Eol);
        Put("{\\info");
        Put("{\\title WinTiger}");
        Put("}");
        Put(Eol);
        //WRITE IMAGE INFORMATION
        Put("{\\*\\imagename \"");
        strcpy(Nname, (char*) WriteRtfImageName);
        Put(Nname);
        Put("\" \\imagepage");
        // strcpy(Nname,WriteRtfPageNumber);
        Put("1");
        Put(";}");
        Put("}");
        PutChar(1);
        fclose(out);
    }

    return TRUE;
}

Handle Rtf_CED_CreateParagraph(int16_t FirstIndent, int16_t LeftIndent, int16_t RightIndent,
        int16_t IntervalBefore, RtfSectorInfo *SectorInfo, int AlignParagraph, int m_Flag,
        int LengthStringInTwips, int LengthFragmInTwips) {
    CIF::Rect indent;
    EDBOX playout;
    EDSIZE interval;
    int align;
    int shad = -1;
    indent.rleft() = LeftIndent;
    indent.rright() = RightIndent;
    indent.rtop() = FirstIndent;
    indent.rbottom() = 0;
    interval.cx = IntervalBefore;
    interval.cy = 0;
    playout.x = -1;
    playout.w = -1;
    playout.y = -1;
    playout.h = -1;
    align = AlignParagraph;

    if (align == RTF_TP_ONE)
        align = RTF_TP_LEFT_ALLIGN;

    switch (align) {
    case RTF_TP_LEFT_ALLIGN:
        align = TP_LEFT_ALLIGN;
        break;
    case RTF_TP_RIGHT_ALLIGN:
        align = TP_RIGHT_ALLIGN;
        break;
    case RTF_TP_LEFT_AND_RIGHT_ALLIGN:
        align = TP_LEFT_ALLIGN | TP_RIGHT_ALLIGN;
        break;
    case RTF_TP_CENTER:
        align = TP_CENTER;
        break;
    }

    if (m_Flag & CSTR_STR_NEGATIVE) {//nega
        align = TP_CENTER;
        shad = 10000;

        //#ifdef VMK002
        if ((m_Flag & CSTR_STR_UPDOWN) || (m_Flag & CSTR_STR_DOWNUP)) {
            //          int addIndent =  SectorInfo->PaperW - SectorInfo->MargL - SectorInfo->MargR -
            //                           LenthStringInTwips - indent.left - indent.right;
            int addIndent = LengthFragmInTwips - LengthStringInTwips;

            if (addIndent > 0) {
                addIndent = (int) (.9 * addIndent);
                indent.rleft() += addIndent / 2;
                indent.rright() += addIndent / 2;
                indent.rtop() = 0;
            }
        }

        //#endif
    }

    return CED_CreateParagraph(SectorInfo->hEDSector, SectorInfo->hObject, align, indent,
            SectorInfo->userNum, -1, interval, playout, -1, shad, -1, -1, FALSE);
}

void Rtf_CED_CreateChar(CIF::Rect * slayout, CIF::Letter* Letter, CIF::CRtfChar* pRtfChar) {
    if (RtfWriteMode)
        return;

    if (pRtfChar) {
        *slayout = pRtfChar->realRect().translate(TemplateOffset);
//        slayout->rleft() = pRtfChar->m_Realrect.left + TemplateOffset.x();
//        slayout->rright() = pRtfChar->m_Realrect.right + TemplateOffset.x();
//        slayout->rtop() = pRtfChar->m_Realrect.top + TemplateOffset.y();
//        slayout->rbottom() = pRtfChar->m_Realrect.bottom + TemplateOffset.y();

        int i = 0;
        for (; i < pRtfChar->versionCount(); i++) {
            Letter[i].setChar(pRtfChar->versionAt(i).getChar());
            Letter[i].setProbability(pRtfChar->versionAt(i).probability() | 1);
        }

        Letter[i - 1].setProbability(Letter[i - 1].probability() & 0xFE);
    }

    else {
        slayout->rleft() = -1;
        slayout->rright() = -1;
        slayout->rtop() = -1;
        slayout->rbottom() = -1;
        Letter->setChar(' ');
        Letter->setProbability(0);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CheckVerLines                                                  //
////////////////////////////////////////////////////////////////////////////////////////////////////
#define LMin  500

Bool CheckLines(RECT* Rect, Bool FlagVer, RtfSectorInfo *SectorInfo) {
    Handle hPage;
    int32_t VCentre, HCentre;

    if (FlagVer == TRUE && Rect->bottom - Rect->top < LMin / 2)
        return FALSE;

    hPage = CPAGE_GetHandlePage(CPAGE_GetCurrentPage());
    CLINE_handle hCLINE = CLINE_GetMainContainer();
    HCentre = Rect->left + (Rect->right - Rect->left) / 2;
    VCentre = Rect->top + (Rect->bottom - Rect->top) / 2;
    CLINE_handle hline;
    CPDLine cpdata;
    DLine data;
    hline = CLINE_GetFirstLine(hCLINE);

    if (!hline)
        return FALSE;

    while (hline) {
        cpdata = CLINE_GetLineData(hline);

        if ((!cpdata) || (FlagVer && (cpdata->Dir == LD_Horiz)) || ((!FlagVer) && (cpdata->Dir
                != LD_Horiz)))
            hline = CLINE_GetNextLine(hline);

        else {
            if ((cpdata->Flags & LI_IsTrue) && !(cpdata->Flags & LI_IsAtTable) && !(cpdata->Flags
                    & LI_Used) && !(cpdata->Flags & LI_FRMT_Used)) {
                if (FlagVer) {
                    if (cpdata->Line.Beg_X * Twips > Rect->left && cpdata->Line.End_X * Twips
                            > Rect->left && cpdata->Line.Beg_X * Twips < Rect->right
                            && cpdata->Line.End_X * Twips < Rect->right && VCentre
                            > cpdata->Line.Beg_Y * Twips && VCentre < cpdata->Line.End_Y * Twips
                            && abs(cpdata->Line.Beg_Y - cpdata->Line.End_Y) * Twips > (Rect->bottom
                                    - Rect->top) / 2)
                        return TRUE;
                }

                else {//Hor
                    if (((cpdata->Line.Beg_Y * Twips > Rect->top && cpdata->Line.Beg_Y * Twips
                            < Rect->bottom) || (cpdata->Line.End_Y * Twips > Rect->top
                            && cpdata->Line.End_Y * Twips < Rect->bottom)) && abs(
                            cpdata->Line.Beg_X - cpdata->Line.End_X) * Twips > SectorInfo->PaperW
                            / 2) {
                        if (cpdata->Line.Beg_Y * Twips > Rect->top && cpdata->Line.Beg_Y * Twips
                                < Rect->bottom) {
                            Rect->top = (int32_t) (cpdata->Line.Beg_Y * Twips);
                            Rect->bottom = (int32_t) (cpdata->Line.Beg_Y * Twips + 10);
                        }

                        else {
                            Rect->top = (int32_t) (cpdata->Line.End_Y * Twips);
                            Rect->bottom = (int32_t) (cpdata->Line.End_Y * Twips + 10);
                        }

                        CLINE_CopyData(&data, cpdata, sizeof(DLine));
                        data.Flags |= LI_FRMT_Used;
                        CLINE_SetLineData(hline, (CPDLine) (&data));
                        return TRUE;
                    }
                }
            }

            hline = CLINE_GetNextLine(hline);
        }
    }

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                         Cleaning_LI_FRMT_Used_Flag                                             //
////////////////////////////////////////////////////////////////////////////////////////////////////
void Cleaning_LI_FRMT_Used_Flag(void) {
    Handle hPage;
    hPage = CPAGE_GetHandlePage(CPAGE_GetCurrentPage());
    CLINE_handle hCLINE = CLINE_GetMainContainer();
    CLINE_handle hline;
    CPDLine cpdata;
    DLine data;
    hline = CLINE_GetFirstLine(hCLINE);

    if (!hline)
        return;

    while (hline) {
        cpdata = CLINE_GetLineData(hline);

        if (!cpdata)
            hline = CLINE_GetNextLine(hline);

        else {
            if (cpdata->Flags & LI_FRMT_Used) {
                CLINE_CopyData(&data, cpdata, sizeof(DLine));
                data.Flags &= 0xffff7fff;
                CLINE_SetLineData(hline, (CPDLine) (&data));
            }

            hline = CLINE_GetNextLine(hline);
        }
    }
}
