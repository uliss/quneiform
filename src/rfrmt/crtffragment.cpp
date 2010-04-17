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

#include "crtffragment.h"
#include "crtfchar.h"
#include "crtfstring.h"
#include "crtfsector.h"
#include "crtfpage.h"
#include "rtfword.h"
#include "creatertf.h"
#include "crtffunc.h"

#include "ced/ced.h"
#include "cstr/cstrdefs.h"
#include "minmax.h"

void Rtf_CED_CreateChar(CIF::Rect* slayout, CIF::Letter* letter, CRtfChar* pRtfChar);
Handle Rtf_CED_CreateParagraph(int16_t FirstIndent, int16_t LeftIndent, int16_t RightIndent,
        int16_t IntervalBefore, RtfSectorInfo *SectorInfo, int AlignParagraph, int shad,
        int LenthStringInTwips, int LengthFragmInTwips);

CRtfFragment::CRtfFragment() {
    m_wStringsCount = 0;
    m_CountLeftEqual = 0;
    m_CountRightEqual = 0;
    m_CountLeftRightEqual = 0;
    m_CountCentreEqual = 0;
    SetRect(&m_rect, 32000, 32000, 0, 0);
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
    CRtfString* cString;
    m_wStringsCount = m_arStrings.size();

    for (int i = 0; i < m_wStringsCount; i++) {
        cString = m_arStrings[i];
        delete cString;
    }
}

CRtfString* CRtfFragment::GetNextString() {
    m_arStrings.push_back(new CRtfString());
    return m_arStrings.back();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FWriteText                                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool CRtfFragment::FWriteText(int16_t NumberCurrentFragment, RtfSectorInfo *SectorInfo,
        Bool OutPutType) {
    CRtfWord* pRtfWord;
    CRtfString* pRtfString;
    CRtfChar* pRtfChar;
    uint16_t CountWords;
    uint16_t CountChars;
    int16_t flag_end_word_with_hiphen;
    int16_t tmp_font_name;
    Bool boPrevNega, boNega;
#ifdef EdWrite
    int EDFontAttribs, EDFontPointSize;
    Handle hParagraph = NULL;
    Handle hString = NULL;
    CIF::Rect slayout;
    CIF::Letter Letter[REC_MAX_VERS];
    int shading = -1;
#endif
    InitFragment(SectorInfo);
    //--- Цикл по строкам
    boPrevNega = false; //NEGA_STR

    for (int ns = 0; ns < m_wStringsCount; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];
        pRtfWord = (CRtfWord*) pRtfString->m_arWords[0];
        pRtfChar = (CRtfChar*) pRtfWord->m_arChars[0];

        if (pRtfChar->m_bFlg_cup_drop == TRUE) { //заносим буквицы во frame
            if ((FlagMode & USE_FRAME) || OutPutType)
                pRtfChar->m_bFlg_cup_drop = FALSE;

            else
                pRtfString->m_wFlagBeginParagraph = TRUE;
        }

        // чтобы не смешивать в одном абзаце негатив. и позитив. строки, при смене
        // цвета стартуем новый абзац
        boNega = pRtfString->S_Flags & CSTR_STR_NEGATIVE; //NEGA_STR

        if (boNega != boPrevNega)
            pRtfString->m_wFlagBeginParagraph = true;

        if (pRtfString->m_wFlagBeginParagraph == TRUE) {
            if (ns) {
                if (OutPutType)
                    Put("}");

                PutCom("\\fs", m_wprev_font_size * 2, 1);
                Put("\\par");
            }

            if (pRtfString->m_wAlignment == RTF_TP_TYPE_LINE) {
                pRtfString->m_wAlignment = RTF_TP_LEFT_AND_RIGHT_ALLIGN;
                m_fi = -pRtfString->m_wFirstIndent;
            }

            else
                m_fi = pRtfString->m_wFirstIndent;

            m_wvid_parag = pRtfString->m_wAlignment;
            m_li = pRtfString->m_wLeftIndent;
            m_ri = pRtfString->m_wRightIndent;
            m_sb = pRtfString->m_wSpaceBefore;
#ifdef EdWrite

            if (!RtfWriteMode && OutPutType)
                m_sb = 0;

#endif
            m_wprev_font_size = pRtfWord->m_wRealFontPointSize;

            if (FlagMode & USE_FRAME_AND_COLUMN) {
                if (SectorInfo->FlagOneString == TRUE) {
                    m_li = 0;
                    m_fi = MAX(0, (int16_t)(m_rect.left - SectorInfo->MargL));
                    m_ri = 0;
                }
            }

            new_paragraph(OutPutType);
#ifdef EdWrite

            if (!RtfWriteMode) {
                pRtfWord = (CRtfWord*) pRtfString->m_arWords[0];
                pRtfChar = (CRtfChar*) pRtfWord->m_arChars[0];
                int colWidth = 0;

                if (pRtfParent && !(FlagMode & USE_NONE)) {
                    CRtfSector* curSect =
                            (CRtfSector*) pRtfParent->m_arSectors[pRtfParent->m_nCurSectorNumber];

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
                    colWidth = m_rectReal.right - m_rectReal.left;

                if (!pRtfChar->m_bFlg_cup_drop)
                    hParagraph = Rtf_CED_CreateParagraph(m_fi, m_li, m_ri, m_sb, SectorInfo,
                            m_wvid_parag, /*m_Flag*/pRtfString->S_Flags,
                            pRtfString->m_LengthStringInTwips, colWidth); //NEGA_STR
            }

#endif
        }

#ifdef EdWrite

        if (!RtfWriteMode && !pRtfChar->m_bFlg_cup_drop) {
#ifdef CHEREDOV
            hString = CED_CreateLine(hParagraph, pRtfString->m_bLineTransfer, (int)((pRtfWord->m_wRealFontPointSize - 1) * 2));
#else

            if ((FlagMode & NOSIZE) && !(FlagMode & USE_FRAME)) {
                hString = CED_CreateLine(hParagraph, pRtfString->m_bLineTransfer, DefFontSize); //line is text line
            }

            else {
                hString = CED_CreateLine(hParagraph, pRtfString->m_bLineTransfer,
                        (int) (pRtfWord->m_wRealFontPointSize * 2));
            }

#endif
        }

#endif
        //--- Цикл по словам
        CountWords = pRtfString->m_wWordsCount;

        for (int nw = 0; nw < CountWords; nw++) {
            pRtfWord = (CRtfWord*) pRtfString->m_arWords[nw];
            pRtfChar = (CRtfChar*) pRtfWord->m_arChars[0];
            Put("{");
            tmp_font_name = get_font_name(pRtfWord->m_wFontNumber);

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

            if (pRtfWord->m_wFontNumber & TG_EDW_UNDERLINE) {
                Put("\\ul");
#ifdef EdWrite
                EDFontAttribs = EDFontAttribs | TG_EDW_UNDERLINE;
#endif
            }

            if (!(FlagMode & NOBOLD) && (pRtfWord->m_wFontNumber & TG_EDW_BOLD)) {
                Put("\\b");
#ifdef EdWrite
                EDFontAttribs = EDFontAttribs | TG_EDW_BOLD;
#endif
            }

            if (!(FlagMode & NOCURSIV) && (pRtfWord->m_wFontNumber & TG_EDW_ITALIC)) {
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
                PutCom("\\fs", pRtfWord->m_wRealFontPointSize * 2, 1);

            flag_end_word_with_hiphen = 0;
            pRtfWord->get_coordinates_and_probability();
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
            CountChars = pRtfWord->m_wCharsCount;

            for (int nz = 0; nz < CountChars; nz++) {
                pRtfChar = (CRtfChar*) pRtfWord->m_arChars[nz];
#ifdef EdWrite

                if (!pRtfWord->m_wcs)
                    pRtfChar->m_chrVersions[0].m_bProbability = 0;

                if (nw == 0 && nz == 0 && pRtfChar->m_bFlg_cup_drop)
#ifdef CHEREDOV
                    EDFontPointSize = (int)((pRtfChar->m_wFontPointSize - 1) * 2);

#else

                    if ((FlagMode & NOSIZE) && !(FlagMode & USE_FRAME))
                        EDFontPointSize = DefFontSize;

                    else
                        EDFontPointSize = (int) (pRtfChar->m_wFontPointSize * 2);

#endif
                else
#ifdef CHEREDOV
                EDFontPointSize = (int)((pRtfWord->m_wRealFontPointSize - 1) * 2);

#else

                if ((FlagMode & NOSIZE) && !(FlagMode & USE_FRAME))
                    EDFontPointSize = DefFontSize;

                else
                    EDFontPointSize = (int) (pRtfWord->m_wRealFontPointSize * 2);

#endif
#endif
                flag_end_word_with_hiphen = 0;

                if (nw == (CountWords - 1) && nz == (CountChars - 1)
                        && pRtfChar->m_chrVersions[0].m_bChar == '-')
                    flag_end_word_with_hiphen = 1;

                if (pRtfChar->m_chrVersions[0].m_bChar) {
                    if (pRtfString->m_bLineTransfer == TRUE) {
#ifdef EdWrite

                        if (!RtfWriteMode) {
                            if (nw == 0 && nz == 0 && pRtfChar->m_bFlg_cup_drop) {
                                slayout = CIF::Rect();
                                EDBOX playout__ = { 0, 0, 0, 0 };
                                Handle hObject__ = CED_CreateFrame(SectorInfo->hEDSector,
                                        SectorInfo->hColumn, playout__, 0x22, -1, -1, -1);
                                CED_SetFrameFlag(hObject__, ED_DROPCAP);
                                EDSIZE interval__ = { 0, 0 };

                                //                          if(m_Flag & CSTR_STR_NEGATIVE) //nega
                                if (pRtfString->S_Flags & CSTR_STR_NEGATIVE) //NEGA_STR
                                    shading = 10000;

                                Handle hParagraph__ = CED_CreateParagraph(SectorInfo->hEDSector,
                                        hObject__, TP_LEFT_ALLIGN, slayout, 0, -1, interval__,
                                        playout__, -1, shading, -1, -1, FALSE);
                                Handle hString__ = CED_CreateLine(hParagraph__, FALSE, 6);
                                Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                                CED_CreateChar(hString__, slayout, Letter, EDFontPointSize,
                                        (int) tmp_font_name, EDFontAttribs, pRtfChar->m_blanguage,
                                        -1, -1);
                                hParagraph = Rtf_CED_CreateParagraph(m_fi, m_li, m_ri, m_sb,
                                        SectorInfo, m_wvid_parag,/*m_Flag*/pRtfString->S_Flags,
                                        pRtfString->m_LengthStringInTwips, m_rectReal.right
                                                - m_rectReal.left); //NEGA_STR
#ifdef CHEREDOV
                                hString = CED_CreateLine(hParagraph, pRtfString->m_bLineTransfer, (int)((pRtfWord->m_wRealFontPointSize - 1) * 2));
#else

                                if ((FlagMode & NOSIZE) && !(FlagMode & USE_FRAME))
                                    hString = CED_CreateLine(hParagraph,
                                            pRtfString->m_bLineTransfer, DefFontSize);

                                else
                                    hString = CED_CreateLine(hParagraph,
                                            pRtfString->m_bLineTransfer,
                                            (int) (pRtfWord->m_wRealFontPointSize * 2));

#endif
                            }

                            else {
                                Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                                CED_CreateChar(hString, slayout, Letter, EDFontPointSize,
                                        (int) tmp_font_name, EDFontAttribs, pRtfChar->m_blanguage,
                                        -1, -1);
                            }
                        }

#endif

                        if (nw == 0 && nz == 0 && pRtfChar->m_bFlg_cup_drop)
                            WriteCupDrop(pRtfChar, tmp_font_name);

                        else
                            PutChar(pRtfChar->m_chrVersions[0].m_bChar);
                    }

                    else if (!((m_wvid_parag == RTF_TP_LEFT_AND_RIGHT_ALLIGN || m_wvid_parag
                            == RTF_TP_LEFT_ALLIGN) && flag_end_word_with_hiphen
                            && pRtfChar->m_bFlg_spell_nocarrying)) {
#ifdef EdWrite

                        if (!RtfWriteMode) {
                            if (nw == 0 && nz == 0 && pRtfChar->m_bFlg_cup_drop) {
                                slayout = CIF::Rect();
                                EDBOX playout__ = { 0, 0, 0, 0 };
                                Handle hObject__ = CED_CreateFrame(SectorInfo->hEDSector,
                                        SectorInfo->hColumn, playout__, 0x22, -1, -1, -1);
                                CED_SetFrameFlag(hObject__, ED_DROPCAP);
                                EDSIZE interval__ = { 0, 0 };

                                Handle hParagraph__ = CED_CreateParagraph(SectorInfo->hEDSector,
                                        hObject__, TP_LEFT_ALLIGN, slayout, 0, -1, interval__,
                                        playout__, -1, -1, -1, -1, FALSE);
                                Handle hString__ = CED_CreateLine(hParagraph__, FALSE, 6);
                                Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                                CED_CreateChar(hString__, slayout, Letter, EDFontPointSize,
                                        (int) tmp_font_name, EDFontAttribs, pRtfChar->m_blanguage,
                                        -1, -1);
                                hParagraph = Rtf_CED_CreateParagraph(m_fi, m_li, m_ri, m_sb,
                                        SectorInfo, m_wvid_parag,/*m_Flag*/pRtfString->S_Flags,
                                        pRtfString->m_LengthStringInTwips, m_rectReal.right
                                                - m_rectReal.left); //NEGA_STR
#ifdef CHEREDOV
                                hString = CED_CreateLine(hParagraph, pRtfString->m_bLineTransfer, (int)((pRtfWord->m_wRealFontPointSize - 1) * 2));
#else

                                if ((FlagMode & NOSIZE) && !(FlagMode & USE_FRAME))
                                    hString = CED_CreateLine(hParagraph,
                                            pRtfString->m_bLineTransfer, DefFontSize);

                                else
                                    hString = CED_CreateLine(hParagraph,
                                            pRtfString->m_bLineTransfer,
                                            (int) (pRtfWord->m_wRealFontPointSize * 2));

#endif
                            }

                            else {
                                Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                                CED_CreateChar(hString, slayout, Letter, EDFontPointSize,
                                        (int) tmp_font_name, EDFontAttribs, pRtfChar->m_blanguage,
                                        -1, -1);
                            }
                        }

#endif

                        if (nw == 0 && nz == 0 && pRtfChar->m_bFlg_cup_drop)
                            WriteCupDrop(pRtfChar, tmp_font_name);

                        else
                            PutChar(pRtfChar->m_chrVersions[0].m_bChar);
                    }

                    else {
#ifdef EdWrite

                        if (!RtfWriteMode) {
                            EDFontAttribs = EDFontAttribs | 0x02;
                            Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                            CED_CreateChar(hString, slayout, Letter, EDFontPointSize,
                                    (int) tmp_font_name, EDFontAttribs, pRtfChar->m_blanguage, -1,
                                    -1);
                        }

#endif
                    }
                }
            }

            //--- Конец цикла по буквам
            if (nw < CountWords - 1) {
#ifdef EdWrite

                if (!RtfWriteMode && !pRtfChar->m_bFlg_cup_drop) {
                    Rtf_CED_CreateChar(&slayout, Letter, NULL);
#ifdef CHEREDOV
                    CED_CreateChar(hString, slayout, Letter, (int)((pRtfWord->m_wRealFontPointSize - 1)*2), (int)tmp_font_name,
                            EDFontAttribs, -1, -1, -1);
#else

                    if ((FlagMode & NOSIZE) && !(FlagMode & USE_FRAME))
                        CED_CreateChar(hString, slayout, Letter, DefFontSize, (int) tmp_font_name,
                                EDFontAttribs, LANGUAGE_UNKNOWN, -1, -1);

                    else
                        CED_CreateChar(hString, slayout, Letter,
                                (int) (pRtfWord->m_wRealFontPointSize * 2), (int) tmp_font_name,
                                EDFontAttribs, LANGUAGE_UNKNOWN, -1, -1);

#endif
                }

#endif

                if (!pRtfChar->m_bFlg_cup_drop)
                    PutChar(' '); //InterWord Space
            }

            else if ((ns < m_wStringsCount - 1) && (nw == CountWords - 1) && (m_wvid_parag
                    == RTF_TP_LEFT_AND_RIGHT_ALLIGN || m_wvid_parag == RTF_TP_LEFT_ALLIGN)
                    && !flag_end_word_with_hiphen) {
#ifdef EdWrite

                if (!RtfWriteMode) {
                    Rtf_CED_CreateChar(&slayout, Letter, NULL);
#ifdef CHEREDOV
                    CED_CreateChar(hString, slayout, Letter, (int)((pRtfWord->m_wRealFontPointSize - 1)*2), (int)tmp_font_name,
                            EDFontAttribs, -1, -1, -1);
#else

                    if ((FlagMode & NOSIZE) && !(FlagMode & USE_FRAME))
                        CED_CreateChar(hString, slayout, Letter, DefFontSize, (int) tmp_font_name,
                                EDFontAttribs, LANGUAGE_UNKNOWN, -1, -1);

                    else
                        CED_CreateChar(hString, slayout, Letter,
                                (int) (pRtfWord->m_wRealFontPointSize * 2), (int) tmp_font_name,
                                EDFontAttribs, LANGUAGE_UNKNOWN, -1, -1);

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
        if (pRtfString->m_bLineTransfer == TRUE)
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

