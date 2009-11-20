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

#include <cstring>
#include <cstdio>

#include "crtffragment.h"
#include "crtfword.h"
#include "crtfsector.h"
#include "crtfstring.h"
#include "creatertf.h"
#include "rtfedwrite.h"
#include "frmtpict.h"
#include "recdefs.h"
#include "ced/ced.h"
#include "cstr/cstrdefs.h"
#include "crtfpage.h"
#include "common/cifconfig.h"
#include "common/debug.h"

using namespace CIF;

extern Bool32 FlagLineTransfer;

void WriteCupDrop(CRtfChar* pRtfChar, int16_t font) {
    Put("{\\pard\\fs6\\par}");
    Put("{\\pard\\plain\\slmult0\\keepn\\widctlpar\\pvpara\\dropcapli3\\dropcapt1\\cgrid{");
    PutCom("\\fs", pRtfChar->fontPointSize * 2, 1);
    PutCom("\\f", font, 1);
    PutCom("\\dn", 9, 1);
    PutCom("\\lang", 1024, 1);
    PutChar(pRtfChar->versions[0].char_);
    Put("\\par}}");
}

CRtfFragment::CRtfFragment() :
    m_rect(Point(32000, 32000), Point()) {
    m_CountLeftEqual = 0;
    m_CountRightEqual = 0;
    m_CountLeftRightEqual = 0;
    m_CountCentreEqual = 0;

    m_bOutPutType = FOT_FRAME;
    m_LeftOffsetFragmentFromVerticalColumn = 0;
    m_RightOffsetFragmentFromVerticalColumn = 0;
    m_WidthVerticalColumn = 0;
    m_bFlagObjectInColumn = 0;
    m_wOffsetFromPrevTextFragment = 0;
    m_bFlagUsed = 0;
    m_Attr = 0;
    m_FlagCarry = 0;
    m_Flag = 0;
    pRtfParent = 0;
}

CRtfFragment::~CRtfFragment() {
    for (VectorString::iterator it = strings.begin(), end = strings.end(); it != end; ++it)
        delete *it;
}

CRtfString* CRtfFragment::GetNextString() {
    strings.push_back(new CRtfString());
    return strings.back();
}

Bool CRtfFragment::FWriteText(RtfSectorInfo *SectorInfo, Bool OutPutType) {
    CRtfWord* pRtfWord;
    CRtfString* pRtfString;
    CRtfChar* pRtfChar;
    int16_t flag_end_word_with_hiphen;
    int16_t tmp_font_name;
    Bool boPrevNega, boNega;

#ifdef EdWrite
    int EDFontAttribs, EDFontPointSize;
    Handle hParagraph = NULL;
    Handle hString = NULL;
    CIF::Rect slayout;
    letterEx Letter[REC_MAX_VERS];
    int shading = -1;
#endif

    InitFragment(SectorInfo);

    //--- Цикл по строкам
    boPrevNega = false; //NEGA_STR
    for (uint ns = 0; ns < strings.size(); ns++) {
        pRtfString = strings[ns];
        pRtfWord = pRtfString->words[0];
        pRtfChar = pRtfWord->chars[0];
        if (pRtfChar->flag_cup_drop == TRUE) //заносим буквицы во frame
        {
            if ((FlagMode & USE_FRAME) || OutPutType)
                pRtfChar->flag_cup_drop = FALSE;
            else
                pRtfString->flag_begin_paragraph = TRUE;
        }

        // чтобы не смешивать в одном абзаце негатив. и позитив. строки, при смене
        // цвета стартуем новый абзац
        boNega = pRtfString->flags & CSTR_STR_NEGATIVE; //NEGA_STR

        if (boNega != boPrevNega)
            pRtfString->flag_begin_paragraph = true;

        if (pRtfString->flag_begin_paragraph == TRUE) {
            if (ns) {
                if (OutPutType)
                    Put("}");
                PutCom("\\fs", m_wprev_font_size * 2, 1);
                Put("\\par");
            }
            if (pRtfString->alignment == RTF_TP_TYPE_LINE) {
                pRtfString->alignment = RTF_TP_LEFT_AND_RIGHT_ALLIGN;
                m_fi = -pRtfString->first_indent;
            }
            else
                m_fi = pRtfString->first_indent;

            m_wvid_parag = pRtfString->alignment;
            m_li = pRtfString->left_indent;
            m_ri = pRtfString->right_indent;
            m_sb = pRtfString->space_before;
#ifdef EdWrite
            if (!RtfWriteMode && OutPutType)
                m_sb = 0;
#endif
            m_wprev_font_size = pRtfWord->real_font_point_size;

            if (FlagMode & USE_FRAME_AND_COLUMN) {
                if (SectorInfo->FlagOneString == TRUE) {
                    m_li = 0;
                    m_fi = MAX(0, m_rect.left() - SectorInfo->MargL);
                    m_ri = 0;
                }
            }
            NewParagraph(OutPutType);
#ifdef EdWrite
            if (!RtfWriteMode) {
                pRtfWord = pRtfString->words[0];
                pRtfChar = pRtfWord->chars[0];

                int colWidth = 0;

                if (pRtfParent && !(FlagMode & USE_NONE)) {
                    CRtfSector* curSect = pRtfParent->m_arSectors[pRtfParent->m_nCurSectorNumber];

                    //Если пишем с форматированием и однострочная колонка
                    if (FlagMode & USE_FRAME_AND_COLUMN && curSect->SectorInfo.FlagOneString
                            == TRUE)
                        colWidth = SectorInfo->PaperW - (SectorInfo->MargL + SectorInfo->MargR);
                    //Если пишем в колонку

                    else if (SectorInfo->hColumn == SectorInfo->hObject) {
                        colWidth
                                = curSect->m_arWidthTerminalColumns[curSect->m_VTerminalColumnNumber
                                        - 1];
                    }

                }
                //Если ничего не помогло
                if (!colWidth)
                    colWidth = m_rectReal.right() - m_rectReal.left();

                if (!pRtfChar->flag_cup_drop)
                    hParagraph = Rtf_CED_CreateParagraph(m_fi, m_li, m_ri, m_sb, SectorInfo,
                            m_wvid_parag, pRtfString->flags, pRtfString->length_string_in_twips,
                            colWidth); //NEGA_STR

            }
#endif
        }

#ifdef EdWrite
        if (!RtfWriteMode && !pRtfChar->flag_cup_drop) {

#ifdef CHEREDOV
            hString = CED_CreateLine(hParagraph,pRtfString->line_transfer,(int)((pRtfWord->real_font_point_size-1)*2));
#else
            if ((FlagMode & NOSIZE) && !(FlagMode & USE_FRAME)) {
                hString = CED_CreateLine(hParagraph, pRtfString->line_transfer, DefFontSize); //line is text line
            }
            else {
                hString = CED_CreateLine(hParagraph, pRtfString->line_transfer,
                        (int) (pRtfWord->real_font_point_size * 2));
            }
#endif
        }
#endif

        //--- Цикл по словам
        size_t CountWords = pRtfString->words.size();
        for (uint nw = 0; nw < CountWords; nw++) {
            pRtfWord = pRtfString->words[nw];
            pRtfChar = pRtfWord->chars[0];
            Put("{");

            tmp_font_name = get_font_name(pRtfWord->font_number);
            if (m_wprev_font_name != tmp_font_name) {
                switch (tmp_font_name) {
                case 0:
                    PutCom("\\f", 0, 0);
                    break;
                case 1:
                    PutCom("\\f", 1, 0);
                    break;
                case 2:
                    PutCom("\\f", 2, 0);
                    break;
                case 3:
                    PutCom("\\f", 3, 0);
                    break;

                default:
                    PutCom("\\f", 1, 0);
                    break;
                }
            }

#ifdef EdWrite
            EDFontAttribs = 0;
#endif

            if (pRtfWord->font_number & TG_EDW_UNDERLINE) {
                Put("\\ul");
#ifdef EdWrite
                EDFontAttribs = EDFontAttribs | TG_EDW_UNDERLINE;
#endif
            }
            if (!(FlagMode & NOBOLD) && (pRtfWord->font_number & TG_EDW_BOLD)) {
                Put("\\b");
#ifdef EdWrite
                EDFontAttribs = EDFontAttribs | TG_EDW_BOLD;
#endif
            }
            if (!(FlagMode & NOCURSIV) && (pRtfWord->font_number & TG_EDW_ITALIC)) {
                Put("\\i");
#ifdef EdWrite
                EDFontAttribs = EDFontAttribs | TG_EDW_ITALIC;
#endif
            }
            if (m_wprev_lang != 1024)
                PutCom("\\lang", 1024, 0);
            if ((FlagMode & NOSIZE) && !(FlagMode & USE_FRAME))
                PutCom("\\fs", DefFontSize, 1);
            else
                PutCom("\\fs", pRtfWord->real_font_point_size * 2, 1);
            flag_end_word_with_hiphen = 0;
            pRtfWord->getCoordinatesAndProbability();
            Put("{");
            PutCom("\\wcl", pRtfWord->m_wcl, 0);
            PutCom("\\wcr", pRtfWord->m_wcr, 0);
            PutCom("\\wct", pRtfWord->m_wct, 0);
            PutCom("\\wcb", pRtfWord->m_wcb, 0);
            PutCom("\\wcp", pRtfWord->m_wcp, 0);
            if (!pRtfWord->m_wcs)
                Put("\\wcs");
            Put("{");
            //--- Цикл по буквам

            size_t CountChars = pRtfWord->chars.size();
            for (uint nz = 0; nz < CountChars; nz++) {
                pRtfChar = (CRtfChar*) pRtfWord->chars[nz];
#ifdef EdWrite
                if (!pRtfWord->m_wcs)
                    pRtfChar->versions[0].probability_ = 0;

                if (nw == 0 && nz == 0 && pRtfChar->flag_cup_drop)
#ifdef CHEREDOV
                    EDFontPointSize = (int)((pRtfChar->fontPointSize-1)*2);
#else
                    if ((FlagMode & NOSIZE) && !(FlagMode & USE_FRAME))
                        EDFontPointSize = DefFontSize;
                    else
                        EDFontPointSize = (int) (pRtfChar->fontPointSize * 2);
#endif
                else
#ifdef CHEREDOV
                EDFontPointSize = (int)((pRtfWord->real_font_point_size-1)*2);
#else
                if ((FlagMode & NOSIZE) && !(FlagMode & USE_FRAME))
                    EDFontPointSize = DefFontSize;
                else
                    EDFontPointSize = (int) (pRtfWord->real_font_point_size * 2);
#endif

#endif
                flag_end_word_with_hiphen = 0;
                if (nw == (CountWords - 1) && nz == (CountChars - 1) && pRtfChar->versions[0].char_
                        == '-')
                    flag_end_word_with_hiphen = 1;

                if (pRtfChar->versions[0].char_) {
                    if (pRtfString->line_transfer == TRUE) {
#ifdef EdWrite
                        if (!RtfWriteMode) {
                            if (nw == 0 && nz == 0 && pRtfChar->flag_cup_drop) {
                                slayout.rleft() = 0;
                                slayout.rright() = 0;
                                slayout.rtop() = 0;
                                slayout.rbottom() = 0;
                                EDBOX playout__ = { 0, 0, 0, 0 };
                                Handle hObject__ = CED_CreateFrame(SectorInfo->hEDSector,
                                        SectorInfo->hColumn, playout__, 0x22, -1, -1, -1);
                                CED_SetFrameFlag(hObject__, ED_DROPCAP);
                                Size interval__;
                                if (pRtfString->flags & CSTR_STR_NEGATIVE) //NEGA_STR
                                    shading = 10000;
                                Handle hParagraph__ = CED_CreateParagraph(SectorInfo->hEDSector,
                                        hObject__, TP_LEFT_ALLIGN, slayout, 0, -1, interval__,
                                        playout__, -1, shading, -1, -1, FALSE);

                                Handle hString__ = CED_CreateLine(hParagraph__, FALSE, 6);
                                Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                                CED_CreateChar(hString__, slayout, Letter, EDFontPointSize,
                                        (int) tmp_font_name, EDFontAttribs, pRtfChar->language_,
                                        Color::undefined, Color::undefined);

                                hParagraph = Rtf_CED_CreateParagraph(m_fi, m_li, m_ri, m_sb,
                                        SectorInfo, m_wvid_parag, pRtfString->flags,
                                        pRtfString->length_string_in_twips, m_rectReal.right()
                                                - m_rectReal.left()); //NEGA_STR
#ifdef CHEREDOV
                                hString = CED_CreateLine(hParagraph,pRtfString->line_transfer,(int)((pRtfWord->real_font_point_size-1)*2));
#else
                                if ((FlagMode & NOSIZE) && !(FlagMode & USE_FRAME))
                                    hString = CED_CreateLine(hParagraph, pRtfString->line_transfer,
                                            DefFontSize);
                                else
                                    hString = CED_CreateLine(hParagraph, pRtfString->line_transfer,
                                            (int) (pRtfWord->real_font_point_size * 2));
#endif

                            }
                            else {
                                Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                                CED_CreateChar(hString, slayout, Letter, EDFontPointSize,
                                        (int) tmp_font_name, EDFontAttribs, pRtfChar->language_,
                                        Color::undefined, Color::undefined);
                            }
                        }
#endif
                        if (nw == 0 && nz == 0 && pRtfChar->flag_cup_drop)
                            WriteCupDrop(pRtfChar, tmp_font_name);
                        else
                            PutChar(pRtfChar->versions[0].char_);
                    }
                    else if (!((m_wvid_parag == RTF_TP_LEFT_AND_RIGHT_ALLIGN || m_wvid_parag
                            == RTF_TP_LEFT_ALLIGN) && flag_end_word_with_hiphen
                            && pRtfChar->flag_spell_nocarrying)) {
#ifdef EdWrite
                        if (!RtfWriteMode) {
                            if (nw == 0 && nz == 0 && pRtfChar->flag_cup_drop) {
                                slayout.rleft() = 0;
                                slayout.rright() = 0;
                                slayout.rtop() = 0;
                                slayout.rbottom() = 0;
                                EDBOX playout__ = { 0, 0, 0, 0 };
                                Handle hObject__ = CED_CreateFrame(SectorInfo->hEDSector,
                                        SectorInfo->hColumn, playout__, 0x22, -1, -1, -1);
                                CED_SetFrameFlag(hObject__, ED_DROPCAP);
                                Size interval__;
                                Handle hParagraph__ = CED_CreateParagraph(SectorInfo->hEDSector,
                                        hObject__, TP_LEFT_ALLIGN, slayout, 0, -1, interval__,
                                        playout__, -1, -1, -1, -1, FALSE);
                                Handle hString__ = CED_CreateLine(hParagraph__, FALSE, 6);
                                Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                                CED_CreateChar(hString__, slayout, Letter, EDFontPointSize,
                                        (int) tmp_font_name, EDFontAttribs, pRtfChar->language_,
                                        Color::undefined, Color::undefined);

                                hParagraph = Rtf_CED_CreateParagraph(m_fi, m_li, m_ri, m_sb,
                                        SectorInfo, m_wvid_parag, pRtfString->flags,
                                        pRtfString->length_string_in_twips, m_rectReal.right()
                                                - m_rectReal.left()); //NEGA_STR
#ifdef CHEREDOV
                                hString = CED_CreateLine(hParagraph,pRtfString->line_transfer,(int)((pRtfWord->real_font_point_size-1)*2));
#else
                                if ((FlagMode & NOSIZE) && !(FlagMode & USE_FRAME))
                                    hString = CED_CreateLine(hParagraph, pRtfString->line_transfer,
                                            DefFontSize);
                                else
                                    hString = CED_CreateLine(hParagraph, pRtfString->line_transfer,
                                            (int) (pRtfWord->real_font_point_size * 2));
#endif

                            }
                            else {
                                Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                                CED_CreateChar(hString, slayout, Letter, EDFontPointSize,
                                        (int) tmp_font_name, EDFontAttribs, pRtfChar->language_,
                                        Color::undefined, Color::undefined);
                            }
                        }
#endif
                        if (nw == 0 && nz == 0 && pRtfChar->flag_cup_drop)
                            WriteCupDrop(pRtfChar, tmp_font_name);
                        else
                            PutChar(pRtfChar->versions[0].char_);
                    }
                    else {
#ifdef EdWrite
                        if (!RtfWriteMode) {
                            EDFontAttribs = EDFontAttribs | 0x02;
                            Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                            CED_CreateChar(hString, slayout, Letter, EDFontPointSize,
                                    (int) tmp_font_name, EDFontAttribs, pRtfChar->language_,
                                    Color::undefined, Color::undefined);
                        }
#endif
                    }
                }
            }
            //--- Конец цикла по буквам
            if (nw < CountWords - 1) {
#ifdef EdWrite
                if (!RtfWriteMode && !pRtfChar->flag_cup_drop) {
                    Rtf_CED_CreateChar(&slayout, Letter, NULL);
#ifdef CHEREDOV
                    CED_CreateChar(hString, slayout, Letter, (int)((pRtfWord->real_font_point_size-1)*2), (int)tmp_font_name,
                            EDFontAttribs, -1, -1, -1);
#else

                    if ((FlagMode & NOSIZE) && !(FlagMode & USE_FRAME))
                        CED_CreateChar(hString, slayout, Letter, DefFontSize, (int) tmp_font_name,
                                EDFontAttribs, -1, Color::undefined, Color::undefined);
                    else
                        CED_CreateChar(hString, slayout, Letter,
                                (int) (pRtfWord->real_font_point_size * 2), (int) tmp_font_name,
                                EDFontAttribs, -1, Color::undefined, Color::undefined);
#endif
                }
#endif
                if (!pRtfChar->flag_cup_drop)
                    PutChar(' '); //InterWord Space
            }
            else if ((ns < strings.size() - 1) && (nw == CountWords - 1) && (m_wvid_parag
                    == RTF_TP_LEFT_AND_RIGHT_ALLIGN || m_wvid_parag == RTF_TP_LEFT_ALLIGN)
                    && !flag_end_word_with_hiphen) {
#ifdef EdWrite
                if (!RtfWriteMode) {
                    Rtf_CED_CreateChar(&slayout, Letter, NULL);
#ifdef CHEREDOV
                    CED_CreateChar(hString, slayout, Letter, (int)((pRtfWord->real_font_point_size-1)*2), (int)tmp_font_name,
                            EDFontAttribs, -1, -1, -1);
#else
                    if ((FlagMode & NOSIZE) && !(FlagMode & USE_FRAME))
                        CED_CreateChar(hString, slayout, Letter, DefFontSize, (int) tmp_font_name,
                                EDFontAttribs, -1, Color::undefined, Color::undefined);
                    else
                        CED_CreateChar(hString, slayout, Letter,
                                (int) (pRtfWord->real_font_point_size * 2), (int) tmp_font_name,
                                EDFontAttribs, -1, Color::undefined, Color::undefined);
#endif
                }
#endif

                PutChar(' '); //InterWord Space
            }

            Put("}");
            Put("}");
            Put("}");
        }
        //--- Конец цикла по словам
        if (pRtfString->line_transfer == TRUE)
            Put("\\line ");

        boPrevNega = boNega; //NEGA_STR
    }
    //--- Конец цикла по строкам
    if (OutPutType)
        Put("}");
    PutCom("\\fs", m_wprev_font_size * 2, 1);
    Put("\\par");
    return TRUE;
}

void CRtfFragment::InitFragment(RtfSectorInfo* SectorInfo) {
    CRtfWord* pRtfWord;
    CRtfString* pRtfString;
    int PenaltyForCheredov = 0;

    pRtfString = strings[0];
    pRtfWord = pRtfString->words[0];

    if (strings.size() == 1 && SectorInfo->CountFragments > 1) {
        if (pRtfWord->real_font_point_size >= 14)
            PenaltyForCheredov = 4;
        else if (pRtfWord->real_font_point_size < 14 && pRtfWord->real_font_point_size > 10)
            PenaltyForCheredov = 2;
        else if (pRtfWord->real_font_point_size < 10 && pRtfWord->real_font_point_size > 5)
            PenaltyForCheredov = 1;
    }

    for (int nw = 0, count = pRtfString->words.size(); nw < count; nw++) {
        pRtfWord = pRtfString->words[nw];
        if (pRtfWord->real_font_point_size > 5)
            pRtfWord->real_font_point_size -= PenaltyForCheredov;
    }

    m_wprev_font_name = get_font_name(pRtfWord->font_number);
    m_wprev_Underline = pRtfWord->font_number & TG_EDW_UNDERLINE;
    m_wprev_Bold = pRtfWord->font_number & TG_EDW_BOLD;
    m_wprev_Italic = pRtfWord->font_number & TG_EDW_ITALIC;
    m_wprev_lang = 1024;
    m_wprev_font_size = pRtfWord->real_font_point_size;

    SetFragmentAlignment(SectorInfo);
}

Bool CRtfFragment::FWriteTable(int16_t NumberCurrentFragment, RtfSectorInfo *SectorInfo,
        Bool OutPutType) {
    // CString  TableString;
    //  uint32_t   CountTableElem;
    //  uint32_t   Tindex;
    //  char     Tsym;

    //  WriteTable((uint32_t)NumberCurrentFragment, SectorInfo/*, &TableString */, OutPutType);
    /*  if(RtfWriteMode)
     {
     CountTableElem = TableString.GetLength();
     for( Tindex=0;  Tindex<CountTableElem; Tindex++ )
     {
     Tsym=(char)TableString.GetAt(Tindex);
     if(Tsym)
     PutC(Tsym);
     }
     }*/
    return TRUE;
}

Bool CRtfFragment::FWritePicture(int16_t NumberCurrentFragment, RtfSectorInfo *SectorInfo,
        Bool OutPutType) {
    WritePict((uint32_t) NumberCurrentFragment, SectorInfo, OutPutType);
    return TRUE;
}

void CRtfFragment::NewParagraph(Bool OutPutType) {
    if (OutPutType) {
        switch (m_wvid_parag) {
        case RTF_TP_LEFT_ALLIGN:
            Put("\\ql");
            break;
        case RTF_TP_RIGHT_ALLIGN:
            Put("\\qr");
            break;
        case RTF_TP_LEFT_AND_RIGHT_ALLIGN:
            Put("\\qj");
            break;
        case RTF_TP_CENTER:
            if (FlagMode & USE_NONE)
                Put("\\ql");
            else
                Put("\\qc");
            break;
        case RTF_TP_ONE:
            Put("\\ql");
            break;
        }
        PutCom("\\li", m_li, 0);
        PutCom("\\ri", m_ri, 0);
        if (m_fi < 0)
            PutCom("\\fi-", abs(m_fi), 0);
        else
            PutCom("\\fi", m_fi, 0);
        PutCom("\\sb", m_sb, 0);
        PutCom("\\sa", 0, 0);
        PutCom("\\sl", 0, 0);

        Put("{");
    }
    Put("\\pard");
    Put("\\plain");

    switch (m_wprev_font_name) {
    case 0:
        PutCom("\\f", 0, 0);
        break;
    case 1:
        PutCom("\\f", 1, 0);
        break;
    case 2:
        PutCom("\\f", 2, 0);
        break;
    case 3:
        PutCom("\\f", 3, 0);
        break;
    default:
        PutCom("\\f", 1, 0);
        break;
    }

    PutCom("\\lang", m_wprev_lang, 0);
    if ((FlagMode & NOSIZE) && !(FlagMode & USE_FRAME))
        PutCom("\\fs", DefFontSize, 1);
    else
        PutCom("\\fs", m_wprev_font_size * 2, 1);

    if (OutPutType)
        return;

    switch (m_wvid_parag) {
    case RTF_TP_LEFT_ALLIGN:
        Put("\\ql");
        break;
    case RTF_TP_RIGHT_ALLIGN:
        Put("\\qr");
        break;
    case RTF_TP_LEFT_AND_RIGHT_ALLIGN:
        Put("\\qj");
        break;
    case RTF_TP_CENTER:
        if (FlagMode & USE_NONE)
            Put("\\ql");
        else
            Put("\\qc");
        break;
    case RTF_TP_ONE:
        Put("\\ql");
        break;
    }
    PutCom("\\li", m_li, 0);
    PutCom("\\ri", m_ri, 0);
    if (m_fi < 0)
        PutCom("\\fi-", abs(m_fi), 0);
    else
        PutCom("\\fi", m_fi, 0);
    PutCom("\\sb", m_sb, 0);
    PutCom("\\sa", 0, 0);
    PutCom("\\sl", 0, 0);
}

void CRtfFragment::SetFragmentAlignment(RtfSectorInfo* SectorInfo) {
    if (ProcessingUseNoneMode() != FALSE)
        return;

    Init(SectorInfo);

    if (ProcessingOverLayedFragment(SectorInfo) == FALSE) {
        if (DeterminationOfMixedFragment(SectorInfo) == FALSE) {
            if (DeterminationOfLeftRightJustification(0, strings.size()) == FALSE) {
                if (DeterminationOfListType(0, strings.size()) == FALSE) {
                    if (DeterminationOfLeftJustification(0, strings.size(), 0) == FALSE) {
                        if (DeterminationOfCentreJustification(0, strings.size()) == FALSE) {
                            if (DeterminationOfRightJustification(0, strings.size()) == FALSE) {
                                DeterminationOfLeftJustification(0, strings.size(), 1);
                            }
                        }
                    }
                }
            }
        }
        Done();
    }
}

Bool CRtfFragment::ProcessingUseNoneMode(void) {
    if (!(FlagMode & USE_NONE))
        return FALSE;

    for (int ns = 0; ns < strings.size(); ns++) {
        CRtfString *pRtfString = strings[ns];

        if (!ns)
            pRtfString->flag_begin_paragraph = TRUE;
        else
            pRtfString->flag_begin_paragraph = FALSE;

        if (ns == strings.size() - 1)
            pRtfString->line_transfer = FALSE;
        else
            pRtfString->line_transfer = TRUE;

        pRtfString->alignment = RTF_TP_LEFT_ALLIGN;
        pRtfString->left_indent = pRtfString->first_indent = pRtfString->right_indent = 0;
    }

    return TRUE;
}

void CRtfFragment::Init(RtfSectorInfo* SectorInfo) {
    int LeftDif, RightDif, CentreDif;
    CRtfString *pRtfStringPrev, *pRtfString;
    CRtfWord *pRtfWord;
    CRtfChar *pRtfCharFirst, *pRtfCharLast;
    int32_t CountChars = 0;
    int32_t LengthChars = 0;
    int ns;

    m_l_fragment = 32000;
    m_r_fragment = 0;

    //  I. Поиск:       Левой(m_l_fragment) и правой(m_r_fragment) границ фрагмента
    //  II.Вычисление:  m_max_dist, котороя используется при поиске начала абзаца
    for (ns = 0; ns < strings.size(); ns++) {
        pRtfString = strings[ns];

        if (!ns) {
            pRtfString->space_before = SectorInfo->VerticalOffsetFragmentInColumn;
            pRtfString->flag_begin_paragraph = TRUE;
        }
        else
            pRtfString->space_before = 0;

        pRtfWord = pRtfString->words[0];
        pRtfCharFirst = pRtfWord->chars[0];
        pRtfString->first_char = pRtfCharFirst->versions[0].char_;

        pRtfWord = pRtfString->words.back();
        pRtfCharLast = pRtfWord->chars.back();
        pRtfString->last_char = pRtfCharLast->versions[0].char_;

        pRtfString->left_border = pRtfCharFirst->ideal_rect_.left();
        pRtfString->right_border = pRtfCharLast->ideal_rect_.right();

        CalculationLengthAndCount(pRtfString, &CountChars, &LengthChars);
        m_l_fragment = MIN(m_l_fragment, (int16_t)pRtfCharFirst->ideal_rect_.left());
        m_r_fragment = MAX(m_r_fragment, (int16_t)pRtfCharLast->ideal_rect_.right());

        if (pRtfCharLast->versions[0].char_ == '-' && pRtfCharLast->flag_spell_nocarrying)
            pRtfString->flag_carry = TRUE;
    }

    if (CountChars)
        m_max_dist = (int16_t) (LengthChars / CountChars);
    else
        m_max_dist = 10;

    // Вычисляется отступ(left_indent, right_indent) строки от краев фрагмента и центр строки
    for (ns = 0; ns < strings.size(); ns++) {
        pRtfString = strings[ns];
        pRtfString->left_indent = (int16_t) (pRtfString->left_border - m_l_fragment);
        pRtfString->right_indent = (int16_t) (m_r_fragment - pRtfString->right_border);
        pRtfString->centre = (int16_t) (pRtfString->left_border + pRtfString->right_border) / 2;
    }

    // Присваиваются признаки равенства концов и середины соседних строк
    for (ns = 1; ns < strings.size(); ns++) {
        pRtfStringPrev = strings[ns - 1];
        pRtfString = strings[ns];

        LeftDif = pRtfString->left_indent - pRtfStringPrev->left_indent;
        RightDif = pRtfString->right_indent - pRtfStringPrev->right_indent;
        CentreDif = pRtfString->centre - pRtfStringPrev->centre;

        if (abs(LeftDif) <= m_max_dist) {
            pRtfString->left_border_equal = TRUE;
            m_CountLeftEqual++;
            if (ns == 1) {
                pRtfStringPrev->left_border_equal = TRUE;
                m_CountLeftEqual++;
            }
        }

        if (abs(RightDif) <= m_max_dist) {
            pRtfString->right_border_equal = TRUE;
            m_CountRightEqual++;
            if (pRtfString->left_border_equal == TRUE)
                m_CountLeftRightEqual++;

            if (ns == 1) {
                pRtfStringPrev->right_border_equal = TRUE;
                m_CountRightEqual++;
                m_CountLeftRightEqual++;
            }
        }

        if (((abs(CentreDif) < m_max_dist) && ((LeftDif <= 0 && RightDif <= 0) || (LeftDif > 0
                && RightDif > 0)) && (abs(LeftDif) > m_max_dist / 2) && (abs(RightDif) > m_max_dist
                / 2)) || ((abs(CentreDif) < 2 * m_max_dist) && (abs(RightDif - LeftDif) < 3
                * m_max_dist) && (abs(LeftDif) > 5 * m_max_dist)
                && (abs(RightDif) > 5 * m_max_dist))) {
            pRtfString->centre_equal = TRUE;
            m_CountCentreEqual++;

            if (ns == 1) {
                pRtfStringPrev->centre_equal = TRUE;
                m_CountCentreEqual++;
            }
        }

    }

    PrintTheResult("\n ================== Init ================== \n");
}

// если колонки остались несепарабельными, то они все будут отписаны как frames
Bool CRtfFragment::ProcessingOverLayedFragment(RtfSectorInfo* SectorInfo) {
    CRtfString *pRtfStringPrev;
    CRtfString *pRtfStringNext;
    CRtfString *pRtfString;

    if (!(SectorInfo->FlagOverLayed))
        return FALSE;

    for (uint ns = 0; ns < strings.size(); ns++) {
        pRtfString = strings[ns];
        pRtfString->alignment = RTF_TP_LEFT_AND_RIGHT_ALLIGN;
        pRtfString->flag_begin_paragraph = FALSE;
        pRtfString->right_indent = 0;
    }

    for (uint ns = 0; ns < strings.size(); ns++) {
        pRtfString = strings[ns];

        if (ns == 0) {
            if (pRtfString->left_indent > m_max_dist / 2)
                pRtfString->first_indent = (uint16_t) (m_max_dist * Twips);
            else
                pRtfString->first_indent = 0;

            pRtfString->left_indent = 0;
            pRtfString->flag_begin_paragraph = TRUE;
            continue;
        }

        pRtfStringPrev = strings[ns - 1];

        if (ns == strings.size() - 1) {
            if ((pRtfString->left_indent - pRtfStringPrev->left_indent) > (m_max_dist / 2)) {
                pRtfString->left_indent = 0;
                pRtfString->first_indent = (uint16_t) (m_max_dist * Twips);
                pRtfString->flag_begin_paragraph = TRUE;
            }
        }
        else {
            pRtfStringNext = strings[ns + 1];
            if (((pRtfString->left_indent - pRtfStringPrev->left_indent) > (m_max_dist / 2))
                    && ((pRtfString->left_indent - pRtfStringNext->left_indent) > (m_max_dist / 2))) {
                pRtfString->left_indent = 0;
                pRtfString->first_indent = (uint16_t) (m_max_dist * Twips);
                pRtfString->flag_begin_paragraph = TRUE;
            }
        }
    }

    return TRUE;
}

Bool CRtfFragment::DeterminationOfLeftRightJustification(int beg, int end) {
    if (!CheckLeftRightJustification(beg, end))
        return FALSE;

    SetParagraphAlignment(beg, end, RTF_TP_LEFT_AND_RIGHT_ALLIGN);
    SetFlagBeginParagraphForLeftRightJustification(beg, end);
    CorrectIndents(beg, end);

    PrintTheResult(
            "\n ================== DeterminationOfLeftRightJustification ================== \n");

    return TRUE;
}

Bool CRtfFragment::CheckLeftRightJustification(int beg, int end) {
    CRtfString *pRtfString;
    int Count = 0;
    uint16_t CountLeftRightEqual = 0;
    uint16_t CountCentreEqual = 0;
    uint16_t CountLeftEqual = 0;

    GetCountEqual(beg, end, &CountCentreEqual, RTF_TP_CENTER);
    GetCountEqual(beg, end, &CountLeftEqual, RTF_TP_LEFT_ALLIGN);

    if (CountCentreEqual == (end - beg))
        return FALSE;

    GetCountEqual(beg, end, &CountLeftRightEqual, RTF_TP_LEFT_AND_RIGHT_ALLIGN);
    m_FlagCarry = GetFlagCarry(beg, end);
    m_FlagLeft = GetFlagLeft(beg, end);
    m_FlagStrongLeft = GetFlagStrongLeft(beg, end);
    m_FlagRight = GetFlagRight(beg, end);
    m_FlagBigSpace = GetFlagBigSpace(beg, end);

    if (m_FlagStrongLeft == TRUE)
        return FALSE;

    if (m_FlagCarry == FALSE && end - beg <= 2 && CountLeftRightEqual == 0)
        return FALSE;

    if (m_FlagCarry == FALSE && m_FlagBigSpace == FALSE && (CountLeftRightEqual < (end - beg) / 3))
        return FALSE;

    if (m_FlagRight == TRUE && end - beg <= 4)
        return FALSE;

    if ((CountLeftEqual >= (end - beg - 1)) && m_FlagBigSpace == FALSE) {
        if (((end - beg) < 5) && (CountLeftEqual > CountLeftRightEqual))
            return FALSE;

        if (CountLeftRightEqual <= (end - beg) / 2)
            return FALSE;
    }

    for (int ns = beg + 1; ns < end; ns++) {
        pRtfString = strings[ns];

        if (pRtfString->left_border_equal && pRtfString->right_border_equal)
            continue;

        if (CheckStringForLeftRightJustification(ns))
            Count++;
    }

    if (m_FlagLeft && ((Count + CountLeftRightEqual) < 4 * (end - beg - 1) / 5))
        return FALSE;

    if (m_FlagCarry) {
        if ((Count + CountLeftRightEqual) < (end - beg - 1) / 3)
            return FALSE;
    }
    else {
        if ((Count + CountLeftRightEqual) < 4 * (end - beg - 1) / 5)
            return FALSE;

        GetCountEqual(beg, end, &CountCentreEqual, RTF_TP_CENTER);
        if ((Count + CountLeftRightEqual) < CountCentreEqual)
            return FALSE;
    }
    return TRUE;
}

void CRtfFragment::GetCountEqual(int beg, int end, uint16_t* Count, int AlignType) {
    CRtfString *pRtfString;

    for (int i = beg; i < end; i++) {
        pRtfString = strings[i];

        switch (AlignType) {
        case RTF_TP_LEFT_ALLIGN:
            if (pRtfString->left_border_equal == TRUE)
                (*Count)++;
            break;
        case RTF_TP_RIGHT_ALLIGN:
            if (pRtfString->right_border_equal == TRUE)
                (*Count)++;
            break;
        case RTF_TP_LEFT_AND_RIGHT_ALLIGN:
            if ((pRtfString->left_border_equal == TRUE) && (pRtfString->right_border_equal == TRUE))
                (*Count)++;
            break;
        case RTF_TP_CENTER:
            if (pRtfString->centre_equal == TRUE)
                (*Count)++;
            break;

        default:
            break;
        }
    }

    if (*Count == 1)
        (*Count)++;
}

Bool CRtfFragment::CheckStringForLeftRightJustification(int ns) {
    CRtfString *pRtfString;
    CRtfString *pRtfStringPrev;
    CRtfString *pRtfStringNext;
    int16_t LeftFragm, RightFragm;
    int16_t LeftDif, RightDif;

    if (m_Attr) {
        LeftFragm = m_l_fragmentLocal;
        RightFragm = m_r_fragmentLocal;
        LeftDif = m_l_fragmentLocal - m_l_fragment;
        RightDif = m_r_fragment - m_r_fragmentLocal;
    }
    else {
        LeftFragm = m_l_fragment;
        RightFragm = m_r_fragment;
        LeftDif = 0;
        RightDif = 0;
    }

    pRtfString = strings[ns];

    if ((pRtfString->left_indent - LeftDif) > m_max_dist && ns < strings.size() - 1) {
        pRtfStringNext = strings[ns + 1];
        if (((pRtfString->left_indent - LeftDif) < (RightFragm - LeftFragm) / 2)
                && ((pRtfString->right_indent - RightDif) < m_max_dist)
                && ((pRtfStringNext->left_indent - LeftDif) < m_max_dist))
            return TRUE;
    }

    if ((pRtfString->left_indent - LeftDif) < m_max_dist && ns > 1) {
        pRtfStringPrev = strings[ns - 1];
        if ((pRtfStringPrev->right_indent - RightDif) < m_max_dist)
            return TRUE;
    }

    return FALSE;
}

void CRtfFragment::SetFlagBeginParagraphForLeftRightJustification(int beg, int end) {
    CRtfString *pRtfStringPrev;
    CRtfString *pRtfString;

    for (int ns = beg; ns < end; ns++) {
        pRtfString = strings[ns];
        if (ns == beg) {
            pRtfString->flag_begin_paragraph = TRUE;
            continue;
        }

        pRtfStringPrev = strings[ns - 1];

        if (((pRtfString->left_indent > 2 * m_max_dist) && (abs(pRtfString->left_indent
                - pRtfStringPrev->left_indent) > m_max_dist)) || (pRtfStringPrev->right_indent > 10
                * m_max_dist) || ((pRtfStringPrev->right_indent > 5 * m_max_dist)
                && (pRtfStringPrev->last_char == ';' || pRtfStringPrev->last_char == '.'))
                || (CheckNumber(pRtfString->first_char) && (pRtfStringPrev->last_char == ';'
                        || pRtfStringPrev->last_char == '.')) || ((pRtfString->left_indent > 3
                * m_max_dist / 2) && (pRtfStringPrev->last_char == '.' || pRtfString->first_char
                == TIRE)) || (pRtfStringPrev->last_char == '.' && (pRtfString->first_char == '-'
                || pRtfString->first_char == TIRE)) || (pRtfStringPrev->last_char == '?'
                && (pRtfString->first_char == '-' || pRtfString->first_char == TIRE))
                || (pRtfStringPrev->last_char == ':' && (pRtfString->first_char == '-'
                        || pRtfString->first_char == TIRE)) || (pRtfStringPrev->right_indent > 2
                * m_max_dist && pRtfString->first_char == TIRE))
            pRtfString->flag_begin_paragraph = TRUE;
    }
}

Bool CRtfFragment::CheckNumber(uchar FirstChar) {
    const char* result = NULL;
    const char* MasNumber = "0123456789";

    result = strchr(MasNumber, FirstChar);
    if (result == NULL)
        return FALSE;

    return TRUE;
}

void CRtfFragment::CorrectIndents(int beg, int end) {
    CRtfString* pRtfString;
    int16_t MinLeftIndent;
    int16_t MinRightIndent;

    int i(0);
    for (i = beg; i < end; i++) {
        pRtfString = strings[i];
        if (i == beg) {
            MinLeftIndent = pRtfString->left_indent;
            MinRightIndent = pRtfString->right_indent;
            continue;
        }
        if (pRtfString->flag_begin_paragraph == TRUE) {
            MinLeftIndent = MIN(pRtfString->left_indent, MinLeftIndent);
            MinRightIndent = MIN(pRtfString->right_indent, MinRightIndent);
        }
    }

    for (i = beg; i < end; i++) {
        pRtfString = strings[i];
        if (pRtfString->flag_begin_paragraph == TRUE) {
            if (abs(pRtfString->left_indent - MinLeftIndent) < m_max_dist)
                pRtfString->left_indent = MinLeftIndent;
        }
    }

}

Bool CRtfFragment::DeterminationOfLeftJustification(int beg, int end, Bool direct) {

    if (!direct && !CheckLeftJustification(beg, end))
        return FALSE;

    SetParagraphAlignment(beg, end, RTF_TP_LEFT_ALLIGN);

    if (GetFlagCarry(beg, end) == FALSE || GetFlagLeft(beg, end))
        SetLineTransfer(beg, end - 1);

    SetFlagBeginParagraphForLeftJustification(beg, end);
    CorrectIndents(beg, end);

    PrintTheResult("\n ================== DeterminationOfLeftJustification ================== \n");

    return TRUE;

}

Bool CRtfFragment::CheckLeftJustification(int beg, int end) {
    uint16_t CountLeftEqual = 0;
    uint16_t CountRightEqual = 0;
    uint16_t CountLeftRightEqual = 0;
    uint16_t CountCentreEqual = 0;

    GetCountEqual(beg, end, &CountLeftEqual, RTF_TP_LEFT_ALLIGN);
    GetCountEqual(beg, end, &CountRightEqual, RTF_TP_RIGHT_ALLIGN);
    GetCountEqual(beg, end, &CountLeftRightEqual, RTF_TP_LEFT_AND_RIGHT_ALLIGN);
    GetCountEqual(beg, end, &CountCentreEqual, RTF_TP_CENTER);

    if ((CountLeftEqual < (end - beg) / 2 || CountLeftEqual < CountRightEqual || CountLeftEqual
            < CountLeftRightEqual || CountLeftEqual < CountCentreEqual) && (CountRightEqual
            + CountLeftRightEqual + CountCentreEqual > 0))
        return FALSE;

    return TRUE;
}

void CRtfFragment::SetFlagBeginParagraphForLeftJustification(int beg, int end) {
    CRtfString *pRtfStringPrev;
    CRtfString *pRtfString;
    uchar FlagStringParagraph = FALSE;
    uchar FlagStringParagraphSoft = FALSE;
    uint16_t Count = 0;
    int16_t LeftFragm, RightFragm;
    int16_t LeftDif, RightDif;

    if (m_Attr) {
        LeftFragm = m_l_fragmentLocal;
        RightFragm = m_r_fragmentLocal;
        LeftDif = m_l_fragmentLocal - m_l_fragment;
        RightDif = m_r_fragment - m_r_fragmentLocal;
    }
    else {
        LeftFragm = m_l_fragment;
        RightFragm = m_r_fragment;
        LeftDif = 0;
        RightDif = 0;
    }

    int ns(0);
    for (ns = beg; ns < end; ns++) {
        pRtfString = strings[ns];
        if (pRtfString->last_char == '.')
            Count++;
    }

    if (Count > (4 * (end - beg) / 5))
        FlagStringParagraph = TRUE;

    if (Count >= ((end - beg) / 3))
        FlagStringParagraphSoft = TRUE;

    for (ns = beg; ns < end; ns++) {
        pRtfString = strings[ns];
        if (ns == beg) {
            pRtfString->flag_begin_paragraph = TRUE;
            continue;
        }

        pRtfStringPrev = strings[ns - 1];

        if (((pRtfString->left_indent - LeftDif) > 2 * m_max_dist)
                || ((pRtfStringPrev->right_indent - RightDif) > (RightFragm - LeftFragm) / 3)
                || ((pRtfString->left_indent > m_max_dist) && (pRtfString->first_char == '—'
                        || pRtfString->first_char == TIRE)) || (CheckNumber(pRtfString->first_char)
                && (pRtfStringPrev->last_char == ';' || pRtfStringPrev->last_char == '.'))
                || (pRtfStringPrev->last_char == '.' && FlagStringParagraphSoft == TRUE
                        && (pRtfStringPrev->right_indent - RightDif) > 5 * m_max_dist)
                || (pRtfStringPrev->last_char == '.' && FlagStringParagraph == TRUE)) {
            pRtfStringPrev->line_transfer = FALSE;
            pRtfString->flag_begin_paragraph = TRUE;
        }
    }
}

Bool CRtfFragment::DeterminationOfCentreJustification(int beg, int end) {
    CRtfString *pRtfString;

    uint16_t CountLeftEqual = 0;
    uint16_t CountRightEqual = 0;
    uint16_t CountLeftRightEqual = 0;
    uint16_t CountCentreEqual = 0;

    GetCountEqual(beg, end, &CountLeftEqual, RTF_TP_LEFT_ALLIGN);
    GetCountEqual(beg, end, &CountRightEqual, RTF_TP_RIGHT_ALLIGN);
    GetCountEqual(beg, end, &CountLeftRightEqual, RTF_TP_LEFT_AND_RIGHT_ALLIGN);
    GetCountEqual(beg, end, &CountCentreEqual, RTF_TP_CENTER);

    if ((CountCentreEqual < (end - beg) / 2 || CountCentreEqual < CountRightEqual
            || CountCentreEqual < CountLeftRightEqual || CountCentreEqual < CountLeftEqual)
            && (CountRightEqual + CountLeftRightEqual + CountLeftEqual > 0))
        return FALSE;

    SetParagraphAlignment(beg, end, RTF_TP_CENTER);
    SetLineTransfer(beg, end);

    pRtfString = strings[beg];
    pRtfString->flag_begin_paragraph = TRUE;

    PrintTheResult("\n ================== DeterminationOfCentreJustification ================== \n");

    return TRUE;
}

Bool CRtfFragment::DeterminationOfRightJustification(int beg, int end) {
    CRtfString *pRtfStringPrev;
    CRtfString *pRtfString;

    uint16_t CountLeftEqual = 0;
    uint16_t CountRightEqual = 0;
    uint16_t CountLeftRightEqual = 0;
    uint16_t CountCentreEqual = 0;

    m_FlagCarry = GetFlagCarry(beg, end);
    if (m_FlagCarry && m_FlagRight == FALSE)
        return FALSE;

    GetCountEqual(beg, end, &CountLeftEqual, RTF_TP_LEFT_ALLIGN);
    GetCountEqual(beg, end, &CountRightEqual, RTF_TP_RIGHT_ALLIGN);
    GetCountEqual(beg, end, &CountLeftRightEqual, RTF_TP_LEFT_AND_RIGHT_ALLIGN);
    GetCountEqual(beg, end, &CountCentreEqual, RTF_TP_CENTER);

    if (CountRightEqual < (end - beg) / 2)
        return FALSE;

    if ((CountRightEqual < CountCentreEqual || CountRightEqual < CountLeftRightEqual
            || CountRightEqual < CountLeftEqual) && (CountCentreEqual + CountLeftRightEqual
            + CountLeftEqual > 0))
        return FALSE;

    SetParagraphAlignment(beg, end, RTF_TP_RIGHT_ALLIGN);

    for (int ns = beg; ns < end; ns++) {
        pRtfString = strings[ns];
        if (ns == beg) {
            pRtfString->flag_begin_paragraph = TRUE;
            pRtfString->line_transfer = TRUE;
            continue;
        }

        pRtfString->line_transfer = TRUE;

        pRtfStringPrev = strings[ns - 1];
        if (pRtfStringPrev->last_char == '.') {
            pRtfString->flag_begin_paragraph = TRUE;
            pRtfStringPrev->line_transfer = FALSE;
        }
    }

    PrintTheResult("\n ================== DeterminationOfRightJustification ================== \n");

    return TRUE;
}

Bool CRtfFragment::DeterminationOfListType(int beg, int end) {
    CRtfString *pRtfString;
    uchar FlagListParagraph = 0;
    int32_t MinLeft, MaxLeft, MaxRight;
    int32_t CountMinLeft = 0, CountMaxLeft = 0, CountMaxRight = 0;
    uint16_t CountCentreEqual = 0;

    GetCountEqual(beg, end, &CountCentreEqual, RTF_TP_CENTER);

    if (CountCentreEqual == (end - beg))
        return FALSE;

    pRtfString = strings[0];
    MinLeft = MaxLeft = pRtfString->left_border;
    MaxRight = pRtfString->right_border;

    int ns(0);
    //поиск тела списка
    for (ns = beg; ns < end; ns++) {
        pRtfString = strings[ns];
        MinLeft = MIN(MinLeft, pRtfString->left_border);
        MaxLeft = MAX(MaxLeft, pRtfString->left_border);
        MaxRight = MAX(MaxRight, pRtfString->right_border);
    }

    if ((MaxLeft - MinLeft) > (MaxRight - MaxLeft) / 2)
        return FALSE;

    if ((MaxLeft - MinLeft) < m_max_dist)
        return FALSE;

    for (ns = beg; ns < end; ns++) {
        pRtfString = strings[ns];

        if ((abs(MinLeft - pRtfString->left_border) > 5 * m_max_dist) && (abs(MaxLeft
                - pRtfString->left_border) > 5 * m_max_dist))
            return FALSE;

        if (abs(MinLeft - pRtfString->left_border) < m_max_dist)
            CountMinLeft++;

        if (abs(MaxLeft - pRtfString->left_border) < m_max_dist)
            CountMaxLeft++;

        if (abs(MaxRight - pRtfString->right_border) < m_max_dist)
            CountMaxRight++;
    }

    if ((CountMinLeft > CountMaxLeft) || ((CountMinLeft + CountMaxLeft) < 4 * (end - beg) / 5)
            || (CountMaxRight < (end - beg) / 2))
        return FALSE;

    SetParagraphAlignment(beg, end, RTF_TP_TYPE_LINE);

    for (ns = beg; ns < end; ns++) {
        pRtfString = strings[ns];

        if ((ns == beg) || abs(MinLeft - pRtfString->left_border) < m_max_dist)
            pRtfString->flag_begin_paragraph = TRUE;
    }

    PrintTheResult("\n ================== DeterminationOfListType ================== \n");

    return TRUE;
}

Bool CRtfFragment::DeterminationOfMixedFragment(RtfSectorInfo* SectorInfo) {
    int32_t beg = 0, end;
    Bool Flag = TRUE;

    if (m_Attr == FALSE)
        return FALSE;

    while (Flag) {
        GetNextFragmentBegEnd(&beg, &end, &Flag);
        ReInit(SectorInfo, beg, end);
        if (DeterminationOfLeftRightJustification(beg, end) == FALSE) {
            if (DeterminationOfListType(beg, end) == FALSE) {
                if (DeterminationOfLeftJustification(beg, end, 0) == FALSE) {
                    if (DeterminationOfCentreJustification(beg, end) == FALSE) {
                        if (DeterminationOfRightJustification(beg, end) == FALSE) {
                            DeterminationOfLeftJustification(beg, end, 1);
                        }
                    }
                }
            }
        }
        beg = end;
    }

    PrintTheResult("\n ================== DeterminationOfMixedFragment ================== \n");
    return TRUE;
}

void CRtfFragment::ReInit(RtfSectorInfo* SectorInfo, int beg, int end) {
    int LeftDif, RightDif, CentreDif, top, bottom;
    CRtfString *pRtfStringPrev, *pRtfString;
    CRtfWord *pRtfWord;
    CRtfChar *pRtfCharFirst, *pRtfCharLast;
    int ns(0);

    m_l_fragmentLocal = 32000;
    m_r_fragmentLocal = 0;

    m_CountLeftEqual = 0;
    m_CountRightEqual = 0;
    m_CountLeftRightEqual = 0;
    m_CountCentreEqual = 0;
    for (ns = beg; ns < end; ns++) {
        pRtfString = strings[ns];
        pRtfString->left_border_equal = FALSE;
        pRtfString->right_border_equal = FALSE;
        pRtfString->centre_equal = FALSE;
    }
    //  I. Поиск:       Левой(m_l_fragmentLocal) и правой(m_r_fragmentLocal) границ фрагмента
    for (ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) strings[ns];

        if (ns == beg) {
            if (!ns)
                pRtfString->space_before = SectorInfo->VerticalOffsetFragmentInColumn;
            else {
                pRtfStringPrev = strings[ns - 1];

                pRtfWord = pRtfStringPrev->words[0];
                pRtfCharFirst = pRtfWord->chars[0];
                top = pRtfCharFirst->ideal_rect_.bottom();

                pRtfWord = pRtfString->words[0];
                pRtfCharFirst = pRtfWord->chars[0];
                bottom = pRtfCharFirst->ideal_rect_.top();
                pRtfString->space_before = (uint16_t) (bottom - top);
            }
            pRtfString->flag_begin_paragraph = TRUE;
        }
        else
            pRtfString->space_before = 0;

        pRtfWord = pRtfString->words[0];
        pRtfCharFirst = pRtfWord->chars[0];
        pRtfString->first_char = pRtfCharFirst->versions[0].char_;

        pRtfWord = pRtfString->words.back();
        pRtfCharLast = pRtfWord->chars.back();
        pRtfString->last_char = pRtfCharLast->versions[0].char_;

        pRtfString->left_border = pRtfCharFirst->ideal_rect_.left();
        pRtfString->right_border = pRtfCharLast->ideal_rect_.right();

        m_l_fragmentLocal = MIN(m_l_fragmentLocal, (int16_t)pRtfCharFirst->ideal_rect_.left());
        m_r_fragmentLocal = MAX(m_r_fragmentLocal, (int16_t)pRtfCharLast->ideal_rect_.right());

    }

    // Присваиваются признаки равенства концов и середины соседних строк
    for (ns = beg + 1; ns < end; ns++) {
        pRtfStringPrev = strings[ns - 1];
        pRtfString = strings[ns];

        LeftDif = pRtfString->left_indent - pRtfStringPrev->left_indent;
        RightDif = pRtfString->right_indent - pRtfStringPrev->right_indent;
        CentreDif = pRtfString->centre - pRtfStringPrev->centre;

        if (abs(LeftDif) <= m_max_dist) {
            pRtfString->left_border_equal = TRUE;
            m_CountLeftEqual++;
            if (ns == beg) {
                pRtfStringPrev->left_border_equal = TRUE;
                m_CountLeftEqual++;
            }
        }

        if (abs(RightDif) <= m_max_dist) {
            pRtfString->right_border_equal = TRUE;
            m_CountRightEqual++;
            if (pRtfString->left_border_equal == TRUE)
                m_CountLeftRightEqual++;

            if (ns == beg) {
                pRtfStringPrev->right_border_equal = TRUE;
                m_CountRightEqual++;
                m_CountLeftRightEqual++;
            }
        }

        if ((abs(CentreDif) < m_max_dist) && ((LeftDif <= 0 && RightDif <= 0) || (LeftDif > 0
                && RightDif > 0)) && (abs(LeftDif) > m_max_dist / 2) && (abs(RightDif) > m_max_dist
                / 2)) {
            pRtfString->centre_equal = TRUE;
            m_CountCentreEqual++;

            if (ns == beg) {
                pRtfStringPrev->centre_equal = TRUE;
                m_CountCentreEqual++;
            }
        }
    }

    PrintTheResult("\n ================== ReInit ================== \n");
}

void CRtfFragment::GetNextFragmentBegEnd(int32_t* beg, int32_t* end, Bool* Flag) {
    CRtfString *pRtfString;
    int i;

    *end = *beg + 1;
    for (i = *end; i < strings.size(); i++) {
        pRtfString = strings[i];
        if (pRtfString->attr == TRUE) {
            *end = i;
            break;
        }
    }

    if ((*end >= (int32_t) strings.size()) || (i >= (int32_t) strings.size())) {
        *end = (int32_t) strings.size();
        *Flag = FALSE;
    }
}

void CRtfFragment::Done(void) {
    CheckOnceAgainImportancesFlagBeginParagraph();
    SetFirstLeftAndRightIndentOfParagraph();
    DefineLineTransfer();
}

void CRtfFragment::CalculationLengthAndCount(CRtfString* pRtfString, int32_t* CountChars,
        int32_t* LengthChars) {
    CRtfWord *pRtfWord;
    CRtfChar *pRtfChar;
    uint16_t CountWords;
    uint16_t WCountChars;

    CountWords = pRtfString->words.size();
    for (int i = 0; i < CountWords; i++) {
        pRtfWord = (CRtfWord*) pRtfString->words[i];
        WCountChars = pRtfWord->chars.size();
        for (int j = 0; j < WCountChars; j++) {
            pRtfChar = (CRtfChar*) pRtfWord->chars[j];
            (*LengthChars) += MAX(0, pRtfChar->ideal_rect_.right() - pRtfChar->ideal_rect_.left());
            (*CountChars)++;
        }
    }

}

void CRtfFragment::CheckOnceAgainImportancesFlagBeginParagraph() {
    CRtfString *pRtfString;
    CRtfString *pRtfStringPrev;
    CRtfWord *pRtfWordPrev;
    CRtfWord *pRtfWord;
    CRtfChar *pRtfChar;
    uint16_t CountWords;
    uint16_t CountChars;

    int ns(0);
    for (ns = 1; ns < strings.size(); ns++) {
        pRtfStringPrev = strings[ns - 1];
        pRtfString = strings[ns];

        // если выр. другая, то необходимо начать новый параграф
        if (pRtfString->alignment != pRtfStringPrev->alignment) {
            pRtfStringPrev->line_transfer = FALSE;
            pRtfString->flag_begin_paragraph = TRUE;
        }
    }

    for (ns = 1; ns < strings.size(); ns++) {
        pRtfStringPrev = strings[ns - 1];
        pRtfString = strings[ns];

        pRtfWordPrev = pRtfStringPrev->words[0];
        pRtfWord = pRtfString->words[0];

        if (pRtfString->alignment != RTF_TP_CENTER && abs(pRtfWord->real_font_point_size
                - pRtfWordPrev->real_font_point_size) > 1) {
            pRtfStringPrev->line_transfer = FALSE;
            pRtfString->flag_begin_paragraph = TRUE;
        }
    }

    for (ns = 1; ns < strings.size(); ns++) {
        pRtfStringPrev = strings[ns - 1];
        pRtfString = strings[ns];

        if (pRtfString->flag_begin_paragraph == TRUE) {
            CountWords = pRtfStringPrev->words.size();
            pRtfWord = pRtfStringPrev->words[CountWords - 1];
            CountChars = pRtfWord->chars.size();
            pRtfChar = pRtfWord->chars[CountChars - 1];
            if (pRtfChar->versions[0].char_ == '-' && pRtfChar->flag_spell_nocarrying) {
                if (pRtfString->alignment == pRtfStringPrev->alignment)
                    pRtfString->flag_begin_paragraph = FALSE;
                else if (pRtfStringPrev->alignment == RTF_TP_LEFT_AND_RIGHT_ALLIGN
                        && pRtfString->alignment == RTF_TP_LEFT_ALLIGN) {
                    pRtfString->alignment = RTF_TP_LEFT_AND_RIGHT_ALLIGN;
                    pRtfStringPrev->line_transfer = FALSE;
                    pRtfString->flag_begin_paragraph = FALSE;
                }
            }
        }
    }

    PrintTheResult(
            "\n ================== CheckOnceAgainImportancesFlagBeginParagraph ================== \n");
}

void CRtfFragment::SetFirstLeftAndRightIndentOfParagraph() {
    CRtfString *pRtfString;
    CRtfString *pRtfStringNext;
    int16_t MinLeftIndent;
    int16_t MinRightIndent;
    int i;
    int ns(0);
    int16_t twp_dist;
    int16_t Dif = 0;

    twp_dist = (int16_t) (3 * m_max_dist * Twips);
    for (ns = 0; ns < strings.size(); ns++) {
        pRtfString = strings[ns];
        pRtfString->length_string_in_twips = pRtfString->GetRealStringSize();
        pRtfString->left_indent = (int16_t) ((int16_t) (pRtfString->left_indent * Twips)
                + m_LeftOffsetFragmentFromVerticalColumn);
        pRtfString->right_indent = (int16_t) ((int16_t) (pRtfString->right_indent * Twips)
                + m_RightOffsetFragmentFromVerticalColumn);
        pRtfString->right_indent
                = MIN(pRtfString->right_indent,
                        m_WidthVerticalColumn - (pRtfString->length_string_in_twips + pRtfString->left_indent + pRtfString->right_indent));
    }

    for (ns = 0; ns < strings.size(); ns++) {
        pRtfString = strings[ns];
        if (pRtfString->flag_begin_paragraph == TRUE) {

            if (pRtfString->alignment == RTF_TP_LEFT_ALLIGN) {
                Dif = MAX(0,m_WidthVerticalColumn - pRtfString->length_string_in_twips);

                MinLeftIndent = pRtfString->left_indent;
                for (i = ns + 1; i < strings.size(); i++) {
                    pRtfStringNext = strings[i];
                    if (pRtfStringNext->flag_begin_paragraph == TRUE || pRtfStringNext->alignment
                            != RTF_TP_LEFT_ALLIGN)
                        break;
                    MinLeftIndent = MIN(pRtfStringNext->left_indent, MinLeftIndent);
                }

                if (m_WidthVerticalColumn > pRtfString->length_string_in_twips) {
                    pRtfString->first_indent = pRtfString->left_indent - MinLeftIndent;
                    if (pRtfString->first_indent < (twp_dist / 3))
                        pRtfString->first_indent = 0;

                    pRtfString->left_indent = MIN(Dif, MinLeftIndent);
                }
                else {
                    pRtfString->left_indent = 0;
                    pRtfString->first_indent = 0;
                }

                pRtfString->right_indent = 0;

                if (pRtfString->left_indent < (twp_dist / 2))
                    pRtfString->left_indent = 0;

                if (i == (ns + 1)) {
                    pRtfString->first_indent = pRtfString->left_indent;
                    pRtfString->left_indent = 0;
                }

                continue;
            }

            if (pRtfString->alignment == RTF_TP_RIGHT_ALLIGN) {
                pRtfString->left_indent = 0;
                pRtfString->first_indent = 0;
                pRtfString->right_indent = 0;
                continue;
            }

            if (pRtfString->alignment == RTF_TP_TYPE_LINE) {
                if (ns + 1 < strings.size()) {
                    pRtfStringNext = strings[ns + 1];
                    if (pRtfStringNext->flag_begin_paragraph == FALSE && pRtfStringNext->alignment
                            == RTF_TP_TYPE_LINE) {
                        pRtfString->first_indent = pRtfStringNext->left_indent
                                - pRtfString->left_indent;
                        pRtfString->left_indent = pRtfStringNext->left_indent;
                    }
                    else
                        pRtfString->first_indent = 0;
                }
                else
                    pRtfString->first_indent = 0;
                continue;
            }

            if (pRtfString->alignment == RTF_TP_LEFT_AND_RIGHT_ALLIGN) {
                MinLeftIndent = pRtfString->left_indent;
                MinRightIndent = pRtfString->right_indent;

                for (i = ns + 1; i < strings.size(); i++) {
                    pRtfStringNext = strings[i];
                    if (pRtfStringNext->flag_begin_paragraph == TRUE || pRtfStringNext->alignment
                            != RTF_TP_LEFT_AND_RIGHT_ALLIGN)
                        break;
                    MinLeftIndent = MIN(pRtfStringNext->left_indent, MinLeftIndent);
                    MinRightIndent = MIN(pRtfStringNext->right_indent, MinRightIndent);
                }

                if (MinLeftIndent < (twp_dist / 3))
                    MinLeftIndent = 0;

                pRtfString->first_indent = pRtfString->left_indent - MinLeftIndent;

                if (pRtfString->first_indent < (twp_dist / 3))
                    pRtfString->first_indent = 0;

                if (MinLeftIndent < twp_dist)
                    pRtfString->left_indent = 0;
                else
                    pRtfString->left_indent = MinLeftIndent;

                if (MinRightIndent < twp_dist)
                    pRtfString->right_indent = 0;

                if (i == (ns + 1)) {
                    if (MinLeftIndent > ((2 * twp_dist) / 3))
                        pRtfString->left_indent = MinLeftIndent;

                    pRtfString->first_indent = pRtfString->left_indent;
                    pRtfString->left_indent = 0;
                    pRtfString->right_indent = 0;
                }
                continue;
            }

            if (pRtfString->alignment == RTF_TP_CENTER) {
                MinLeftIndent = pRtfString->left_indent;
                MinRightIndent = pRtfString->right_indent;

                for (i = ns; i < strings.size(); i++) {
                    pRtfStringNext = strings[i];
                    MinLeftIndent = MIN(pRtfStringNext->left_indent, MinLeftIndent);
                    MinRightIndent = MIN(pRtfStringNext->right_indent, MinRightIndent);
                    if (pRtfString->alignment != RTF_TP_CENTER)
                        break;
                }

                if (abs(MinLeftIndent - MinRightIndent) < (2 * m_max_dist))
                    MinLeftIndent = MinRightIndent = 0;
                pRtfString->first_indent = 0;
                pRtfString->left_indent = MinLeftIndent;
                pRtfString->right_indent = MinRightIndent;
            }

        }
    }
    PrintTheResult(
            "\n ================== SetFirstLeftAndRightIndentOfParagraph ================== \n");
}

void CRtfFragment::DefineLineTransfer() {
    CRtfString *pRtfString;
    int Count;

    for (int ns = 0; ns < strings.size(); ns++) {
        pRtfString = strings[ns];
        if (pRtfString->flag_begin_paragraph == TRUE && (FlagLineTransfer || pRtfString->alignment
                == RTF_TP_CENTER)) {
            Count = GetCountLine(ns);
            SetLineTransfer(ns, ns + Count);
        }
    }

    PrintTheResult("\n ================== DefineLineTransfer ================== \n");
}

Bool CRtfFragment::GetFlagLeft(int beg, int end) {
    CRtfString* pRtfString;
    int Count = 0;
    uint16_t CountLeftEqual = 0;
    Bool PriznakLeft = FALSE;

    for (int ns = beg; ns < end; ns++) {
        pRtfString = strings[ns];
        if (pRtfString->last_char == ',') {
            Count++;
            if (pRtfString->right_indent > (pRtfString->right_border - pRtfString->left_border) / 4)
                PriznakLeft = TRUE;
        }
    }

    if (Count > 1 && PriznakLeft)
        return TRUE;

    GetCountEqual(beg, end, &CountLeftEqual, RTF_TP_LEFT_ALLIGN);

    if (CountLeftEqual == (end - beg))
        return TRUE;

    m_FlagBigSpace = GetFlagBigSpace(beg, end);
    m_FlagCarry = GetFlagCarry(beg, end);

    if (!m_FlagBigSpace && !m_FlagCarry)
        return TRUE;

    return FALSE;
}

Bool CRtfFragment::GetFlagStrongLeft(int beg, int end) {
    int Count = 0;
    CRtfString *pRtfString;

    for (int ns = beg; ns < end; ns++) {
        pRtfString = strings[ns];
        if (pRtfString->last_char == '.' || pRtfString->last_char == ',')
            Count++;
    }

    if (Count == end - beg)
        return TRUE;

    if ((end - beg < 5) && Count > 2)
        return TRUE;

    return FALSE;
}

Bool CRtfFragment::GetFlagRight(int beg, int end) {
    CRtfString *pRtfString;

    int ns(0);
    for (ns = beg; ns < end; ns++) {
        pRtfString = strings[ns];
        if (pRtfString->right_indent > m_max_dist)
            return FALSE;
    }

    for (ns = beg; ns < end; ns++) {
        pRtfString = strings[ns];
        if (pRtfString->left_indent > ((m_r_fragment - m_l_fragment) / 2))
            return TRUE;
    }

    return FALSE;
}

Bool CRtfFragment::GetFlagCarry(int beg, int end) {
    CRtfString *pRtfString;

    for (int ns = beg; ns < end; ns++) {
        pRtfString = strings[ns];
        if (pRtfString->flag_carry == TRUE)
            return TRUE;
    }

    return FALSE;
}

Bool CRtfFragment::GetFlagBigSpace(int beg, int end) {
    CRtfString* pRtfString;
    CRtfWord* pRtfWordPrev, *pRtfWordCur;
    CRtfChar* pRtfCharPrev, *pRtfCharCur;
    int FlagBigSpase = 0;
    int CountCharInPrevWord;

    for (int ns = beg; ns < end; ns++) {
        pRtfString = strings[ns];

        for (int i = 1; i < pRtfString->words.size(); i++) {
            pRtfWordPrev = pRtfString->words[i - 1];
            pRtfWordCur = pRtfString->words[i];

            CountCharInPrevWord = pRtfWordPrev->chars.size();

            pRtfCharPrev = pRtfWordPrev->chars[CountCharInPrevWord - 1];
            pRtfCharCur = pRtfWordCur->chars[0];
            if ((pRtfCharCur->ideal_rect_.left() - pRtfCharPrev->ideal_rect_.right()) > 2
                    * m_max_dist)
                FlagBigSpase = 1;
        }
    }

    return FlagBigSpase;
}

void CRtfFragment::SetParagraphAlignment(int beg, int end, int AlignType) {
    for (int i = MAX(0,beg); i < end; i++)
        strings.at(i)->alignment = AlignType;
}

int CRtfFragment::GetCountLine(int beg) {
    CRtfString *pRtfString;
    int Count = 0;

    for (int ns = beg + 1; ns < strings.size(); ns++) {
        pRtfString = strings[ns];
        if (pRtfString->flag_begin_paragraph == TRUE)
            break;
        else
            Count++;
    }
    return Count;
}

void CRtfFragment::SetLineTransfer(int beg, int end) {
    CRtfString *pRtfString;

    for (int ns = beg; ns < end; ns++) {
        pRtfString = strings[ns];
        pRtfString->line_transfer = TRUE;
    }
}
void CRtfFragment::PrintTheResult(const char* header_str) {
    CRtfString *pRtfString;
    CRtfWord *pRtfWord;
    CRtfChar *pRtfCharFirst;

    if (!Config::instance().debugHigh())
        return;

    Debug() << header_str;
    for (int ns = 0; ns < strings.size(); ns++) {
        pRtfString = strings[ns];
        Debug() << *pRtfString << "\n";
        fprintf(
                stderr,
                "N#=%4d , Alignment= %d, BeginParagraph= %d,FI=%4d, LI=%4d, RI=%4d, LBorderEqual=%d, RBorderEqual=%d, CentreEqual=%d, dist=%3d \n",
                ns, pRtfString->alignment, pRtfString->flag_begin_paragraph,
                pRtfString->first_indent, pRtfString->left_indent, pRtfString->right_indent,
                pRtfString->left_border_equal, pRtfString->right_border_equal,
                pRtfString->centre_equal, m_max_dist);
    }
}

