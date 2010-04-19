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

#include <cstring>
#include "crtffragment.h"
#include "crtfchar.h"
#include "crtfstring.h"
#include "crtfsector.h"
#include "crtfpage.h"
#include "rtfword.h"
#include "creatertf.h"
#include "crtffunc.h"
#include "frmtpict.h"

#include "ced/ced.h"
#include "cstr/cstrdefs.h"
#include "minmax.h"

extern Bool32 FlagLineTransfer;

void Rtf_CED_CreateChar(CIF::Rect* slayout, CIF::Letter* letter, CIF::CRtfChar* pRtfChar);
Handle Rtf_CED_CreateParagraph(int16_t FirstIndent, int16_t LeftIndent, int16_t RightIndent,
        int16_t IntervalBefore, RtfSectorInfo *SectorInfo, int AlignParagraph, int shad,
        int LenthStringInTwips, int LengthFragmInTwips);

namespace CIF
{

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
        pRtfChar = pRtfWord->charAt(0);

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
            m_wprev_font_size = pRtfWord->realFontSize();

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
                pRtfChar = pRtfWord->firstChar();
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
                        pRtfWord->realFontSize() * 2);
            }

#endif
        }

#endif
        //--- Цикл по словам
        CountWords = pRtfString->m_wWordsCount;

        for (int nw = 0; nw < CountWords; nw++) {
            pRtfWord = (CRtfWord*) pRtfString->m_arWords[nw];
            pRtfChar = pRtfWord->firstChar();
            Put("{");
            tmp_font_name = get_font_name(pRtfWord->fontNumber());

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

            if (pRtfWord->fontNumber() & TG_EDW_UNDERLINE) {
                Put("\\ul");
#ifdef EdWrite
                EDFontAttribs = EDFontAttribs | TG_EDW_UNDERLINE;
#endif
            }

            if (!(FlagMode & NOBOLD) && (pRtfWord->fontNumber() & TG_EDW_BOLD)) {
                Put("\\b");
#ifdef EdWrite
                EDFontAttribs = EDFontAttribs | TG_EDW_BOLD;
#endif
            }

            if (!(FlagMode & NOCURSIV) && (pRtfWord->fontNumber() & TG_EDW_ITALIC)) {
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
                PutCom("\\fs", pRtfWord->realFontSize() * 2, 1);

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

            for (int nz = 0, total = pRtfWord->charCount(); nz < total; nz++) {
                pRtfChar = pRtfWord->charAt(nz);
#ifdef EdWrite

                if (!pRtfWord->m_wcs)
                    pRtfChar->first().setProbability(0);

                if (nw == 0 && nz == 0 && pRtfChar->m_bFlg_cup_drop)
#ifdef CHEREDOV
                    EDFontPointSize = (int)((pRtfChar->font_size_ - 1) * 2);

#else

                    if ((FlagMode & NOSIZE) && !(FlagMode & USE_FRAME))
                        EDFontPointSize = DefFontSize;

                    else
                        EDFontPointSize = pRtfChar->fontSize() * 2;

#endif
                else
#ifdef CHEREDOV
                EDFontPointSize = (int)((pRtfWord->m_wRealFontPointSize - 1) * 2);

#else

                if ((FlagMode & NOSIZE) && !(FlagMode & USE_FRAME))
                    EDFontPointSize = DefFontSize;

                else
                    EDFontPointSize = pRtfWord->realFontSize() * 2;

#endif
#endif
                flag_end_word_with_hiphen = 0;

                if (nw == (total - 1) && nz == (total - 1) && pRtfChar->first().getChar() == '-')
                    flag_end_word_with_hiphen = 1;

                if (pRtfChar->first().getChar()) {
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
                                        (int) tmp_font_name, EDFontAttribs, pRtfChar->language(),
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
                                            pRtfString->m_bLineTransfer, pRtfWord->realFontSize()
                                                    * 2);

#endif
                            }

                            else {
                                Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                                CED_CreateChar(hString, slayout, Letter, EDFontPointSize,
                                        (int) tmp_font_name, EDFontAttribs, pRtfChar->language(),
                                        -1, -1);
                            }
                        }

#endif

                        if (nw == 0 && nz == 0 && pRtfChar->m_bFlg_cup_drop)
                            WriteCupDrop(pRtfChar, tmp_font_name);

                        else
                            PutChar(pRtfChar->first().getChar());
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
                                        (int) tmp_font_name, EDFontAttribs, pRtfChar->language(),
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
                                            pRtfString->m_bLineTransfer, pRtfWord->realFontSize()
                                                    * 2);

#endif
                            }

                            else {
                                Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                                CED_CreateChar(hString, slayout, Letter, EDFontPointSize,
                                        (int) tmp_font_name, EDFontAttribs, pRtfChar->language(),
                                        -1, -1);
                            }
                        }

#endif

                        if (nw == 0 && nz == 0 && pRtfChar->m_bFlg_cup_drop)
                            WriteCupDrop(pRtfChar, tmp_font_name);

                        else
                            PutChar(pRtfChar->first().getChar());
                    }

                    else {
#ifdef EdWrite

                        if (!RtfWriteMode) {
                            EDFontAttribs = EDFontAttribs | 0x02;
                            Rtf_CED_CreateChar(&slayout, Letter, pRtfChar);
                            CED_CreateChar(hString, slayout, Letter, EDFontPointSize,
                                    (int) tmp_font_name, EDFontAttribs, pRtfChar->language(), -1,
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
                        CED_CreateChar(hString, slayout, Letter, pRtfWord->realFontSize() * 2,
                                (int) tmp_font_name, EDFontAttribs, LANGUAGE_UNKNOWN, -1, -1);

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
                        CED_CreateChar(hString, slayout, Letter, pRtfWord->realFontSize() * 2,
                                (int) tmp_font_name, EDFontAttribs, LANGUAGE_UNKNOWN, -1, -1);

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

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 InitFragment                                                   //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfFragment::InitFragment(RtfSectorInfo* SectorInfo) {
    CRtfWord* pRtfWord;
    CRtfString* pRtfString;
    int PenaltyForCheredov = 0;
    int CountWords;
    pRtfString = (CRtfString*) m_arStrings[0];
    pRtfWord = (CRtfWord*) pRtfString->m_arWords[0];
    CountWords = pRtfString->m_wWordsCount;

    if (m_wStringsCount == 1 && SectorInfo->CountFragments > 1) {
        if (pRtfWord->realFontSize() >= 14)
            PenaltyForCheredov = 4;

        else if (pRtfWord->realFontSize() < 14 && pRtfWord->realFontSize() > 10)
            PenaltyForCheredov = 2;

        else if (pRtfWord->realFontSize() < 10 && pRtfWord->realFontSize() > 5)
            PenaltyForCheredov = 1;
    }

    for (int nw = 0; nw < CountWords; nw++) {
        pRtfWord = pRtfString->m_arWords[nw];

        if (pRtfWord->realFontSize() > 5) {
            pRtfWord->setRealFontSize(pRtfWord->realFontSize() - PenaltyForCheredov);
        }
    }

    m_wprev_font_name = get_font_name(pRtfWord->fontNumber());
    m_wprev_Underline = pRtfWord->fontNumber() & TG_EDW_UNDERLINE;
    m_wprev_Bold = pRtfWord->fontNumber() & TG_EDW_BOLD;
    m_wprev_Italic = pRtfWord->fontNumber() & TG_EDW_ITALIC;
    m_wprev_lang = 1024;
    m_wprev_font_size = pRtfWord->realFontSize();
    SetFragmentAlignment(SectorInfo);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FWriteTable                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FWritePicture                                                  //
////////////////////////////////////////////////////////////////////////////////////////////////////
Bool CRtfFragment::FWritePicture(int16_t NumberCurrentFragment, RtfSectorInfo *SectorInfo,
        Bool OutPutType) {
    //  CString  PictString;
    //  uint32_t   Pindex;
    //  uint32_t  CountPictElem;
    //  char     Psym;
    WritePict((uint32_t) NumberCurrentFragment, SectorInfo,/* &PictString,*/
    OutPutType);
    /*  if(RtfWriteMode)
     {
     CountPictElem = PictString.GetLength();
     for( Pindex=0;  Pindex<CountPictElem; Pindex++ )
     {
     Psym=(char)PictString.GetAt(Pindex);
     if(Psym)
     PutC(Psym);
     }
     }
     */
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 new_paragraph                                                  //
////////////////////////////////////////////////////////////////////////////////////////////////////
void CRtfFragment::new_paragraph(Bool OutPutType) {
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

/*------------------------------------------------------------------------------------------------------------
 RTF_TP_LEFT_ALLIGN                   0   -  выравниваение по левому краю
 RTF_TP_RIGHT_ALLIGN                  1   -  выравнивание по правому краю
 RTF_TP_LEFT_ALLIGN&TP_RIGHT_ALLIGN   2   -  выравнивание по ширине
 RTF_TP_CENTER                        3   -  выравнивание по центру
 RTF_TP_ONE                           4   -  каждая строка состоит из одного слова, выравнивание левому краю
 RTF_TP_TYPE_LINE                     5   -  список
 ---------------------------------------------------------------------------------------------------------------*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 SetFragmentAlignment
void CRtfFragment::SetFragmentAlignment(RtfSectorInfo* SectorInfo) {
    if (ProcessingUseNoneMode() == FALSE) {
        Init(SectorInfo);

        if (ProcessingOverLayedFragment(SectorInfo) == FALSE) {
            if (DeterminationOfMixedFragment(SectorInfo) == FALSE) {
                if (DeterminationOfLeftRightJustification(0, m_wStringsCount) == FALSE) {
                    if (DeterminationOfListType(0, m_wStringsCount) == FALSE) {
                        if (DeterminationOfLeftJustification(0, m_wStringsCount, 0) == FALSE) {
                            if (DeterminationOfCentreJustification(0, m_wStringsCount) == FALSE) {
                                if (DeterminationOfRightJustification(0, m_wStringsCount) == FALSE) {
                                    DeterminationOfLeftJustification(0, m_wStringsCount, 1);
                                }
                            }
                        }
                    }
                }
            }

            Done();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     ProcessingUseNoneMode

Bool CRtfFragment::ProcessingUseNoneMode(void) {
    CRtfString *pRtfString;
    int ns;

    if (!(FlagMode & USE_NONE))
        return FALSE;

    for (ns = 0; ns < m_wStringsCount; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];

        if (!ns)
            pRtfString->m_wFlagBeginParagraph = TRUE;

        else
            pRtfString->m_wFlagBeginParagraph = FALSE;

        if (ns == m_wStringsCount - 1)
            pRtfString->m_bLineTransfer = FALSE;

        else
            pRtfString->m_bLineTransfer = TRUE;

        pRtfString->m_wAlignment = RTF_TP_LEFT_ALLIGN;
        pRtfString->m_wLeftIndent = pRtfString->m_wFirstIndent = pRtfString->m_wRightIndent = 0;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            Init

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
    for (ns = 0; ns < m_wStringsCount; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];

        if (!ns) {
            pRtfString->m_wSpaceBefore = SectorInfo->VerticalOffsetFragmentInColumn;
            pRtfString->m_wFlagBeginParagraph = TRUE;
        }

        else
            pRtfString->m_wSpaceBefore = 0;

        pRtfWord = (CRtfWord*) pRtfString->m_arWords[0];
        pRtfCharFirst = pRtfWord->firstChar();
        pRtfString->m_FirstChar = pRtfCharFirst->first().getChar();
        pRtfWord = (CRtfWord*) pRtfString->m_arWords[pRtfString->m_wWordsCount - 1];
        pRtfCharLast = pRtfWord->lastChar();
        pRtfString->m_LastChar = pRtfCharLast->first().getChar();
        pRtfString->m_LeftBorder = pRtfCharFirst->idealRect().left();
        pRtfString->m_RightBorder = pRtfCharLast->idealRect().right();
        CalculationLengthAndCount(pRtfString, &CountChars, &LengthChars);
        m_l_fragment = MIN(m_l_fragment, (int16_t)pRtfCharFirst->idealRect().left());
        m_r_fragment = MAX(m_r_fragment, (int16_t)pRtfCharLast->idealRect().right());

        if (pRtfCharLast->first().isHyphen() && pRtfCharLast->m_bFlg_spell_nocarrying)
            pRtfString->m_FlagCarry = TRUE;
    }

    if (CountChars)
        m_max_dist = (int16_t) (LengthChars / CountChars);

    else
        m_max_dist = 10;

    // Вычисляется отступ(m_wLeftIndent, m_wRightIndent) строки от краев фрагмента и центр строки
    for (ns = 0; ns < m_wStringsCount; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];
        pRtfString->m_wLeftIndent = (int16_t) (pRtfString->m_LeftBorder - m_l_fragment);
        pRtfString->m_wRightIndent = (int16_t) (m_r_fragment - pRtfString->m_RightBorder);
        pRtfString->m_wCentre = (int16_t) (pRtfString->m_LeftBorder + pRtfString->m_RightBorder)
                / 2;
    }

    // Присваиваются признаки равенства концов и середины соседних строк
    for (ns = 1; ns < m_wStringsCount; ns++) {
        pRtfStringPrev = (CRtfString*) m_arStrings[ns - 1];
        pRtfString = (CRtfString*) m_arStrings[ns];
        LeftDif = pRtfString->m_wLeftIndent - pRtfStringPrev->m_wLeftIndent;
        RightDif = pRtfString->m_wRightIndent - pRtfStringPrev->m_wRightIndent;
        CentreDif = pRtfString->m_wCentre - pRtfStringPrev->m_wCentre;

        if (abs(LeftDif) <= m_max_dist) {
            pRtfString->m_wLeftBorderEqual = TRUE;
            m_CountLeftEqual++;

            if (ns == 1) {
                pRtfStringPrev->m_wLeftBorderEqual = TRUE;
                m_CountLeftEqual++;
            }
        }

        if (abs(RightDif) <= m_max_dist) {
            pRtfString->m_wRightBorderEqual = TRUE;
            m_CountRightEqual++;

            if (pRtfString->m_wLeftBorderEqual == TRUE)
                m_CountLeftRightEqual++;

            if (ns == 1) {
                pRtfStringPrev->m_wRightBorderEqual = TRUE;
                m_CountRightEqual++;
                m_CountLeftRightEqual++;
            }
        }

        if (((abs(CentreDif) < m_max_dist) && ((LeftDif <= 0 && RightDif <= 0) || (LeftDif > 0
                && RightDif > 0)) && (abs(LeftDif) > m_max_dist / 2) && (abs(RightDif) > m_max_dist
                / 2)) || ((abs(CentreDif) < 2 * m_max_dist) && (abs(RightDif - LeftDif) < 3
                * m_max_dist) && (abs(LeftDif) > 5 * m_max_dist)
                && (abs(RightDif) > 5 * m_max_dist))) {
            pRtfString->m_wCentreEqual = TRUE;
            m_CountCentreEqual++;

            if (ns == 1) {
                pRtfStringPrev->m_wCentreEqual = TRUE;
                m_CountCentreEqual++;
            }
        }
    }

    PrintTheResult("\n ================== Init ================== \n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 ProcessingOverLayedFragment

// если колонки остались несепарабельными, то они все будут отписаны как frames

Bool CRtfFragment::ProcessingOverLayedFragment(RtfSectorInfo* SectorInfo) {
    CRtfString *pRtfStringPrev;
    CRtfString *pRtfStringNext;
    CRtfString *pRtfString;

    if (!(SectorInfo->FlagOverLayed))
        return FALSE;

    int ns(0);

    for (ns = 0; ns < m_wStringsCount; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];
        pRtfString->m_wAlignment = RTF_TP_LEFT_AND_RIGHT_ALLIGN;
        pRtfString->m_wFlagBeginParagraph = FALSE;
        pRtfString->m_wRightIndent = 0;
    }

    for (ns = 0; ns < m_wStringsCount; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];

        if (ns == 0) {
            if (pRtfString->m_wLeftIndent > m_max_dist / 2)
                pRtfString->m_wFirstIndent = (uint16_t) (m_max_dist * Twips);

            else
                pRtfString->m_wFirstIndent = 0;

            pRtfString->m_wLeftIndent = 0;
            pRtfString->m_wFlagBeginParagraph = TRUE;
            continue;
        }

        pRtfStringPrev = (CRtfString*) m_arStrings[ns - 1];

        if (ns == m_wStringsCount - 1) {
            if ((pRtfString->m_wLeftIndent - pRtfStringPrev->m_wLeftIndent) > (m_max_dist / 2)) {
                pRtfString->m_wLeftIndent = 0;
                pRtfString->m_wFirstIndent = (uint16_t) (m_max_dist * Twips);
                pRtfString->m_wFlagBeginParagraph = TRUE;
            }
        }

        else {
            pRtfStringNext = (CRtfString*) m_arStrings[ns + 1];

            if (((pRtfString->m_wLeftIndent - pRtfStringPrev->m_wLeftIndent) > (m_max_dist / 2))
                    && ((pRtfString->m_wLeftIndent - pRtfStringNext->m_wLeftIndent) > (m_max_dist
                            / 2))) {
                pRtfString->m_wLeftIndent = 0;
                pRtfString->m_wFirstIndent = (uint16_t) (m_max_dist * Twips);
                pRtfString->m_wFlagBeginParagraph = TRUE;
            }
        }
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 DeterminationOfLeftRightJustification

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

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CheckLeftRightJustification

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
        pRtfString = (CRtfString*) m_arStrings[ns];

        if (pRtfString->m_wLeftBorderEqual && pRtfString->m_wRightBorderEqual)
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

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 GetCountEqual

void CRtfFragment::GetCountEqual(int beg, int end, uint16_t* Count, int AlignType) {
    CRtfString *pRtfString;

    for (int i = beg; i < end; i++) {
        pRtfString = (CRtfString*) m_arStrings[i];

        switch (AlignType) {
        case RTF_TP_LEFT_ALLIGN:

            if (pRtfString->m_wLeftBorderEqual == TRUE)
                (*Count)++;

            break;
        case RTF_TP_RIGHT_ALLIGN:

            if (pRtfString->m_wRightBorderEqual == TRUE)
                (*Count)++;

            break;
        case RTF_TP_LEFT_AND_RIGHT_ALLIGN:

            if ((pRtfString->m_wLeftBorderEqual == TRUE) && (pRtfString->m_wRightBorderEqual
                    == TRUE))
                (*Count)++;

            break;
        case RTF_TP_CENTER:

            if (pRtfString->m_wCentreEqual == TRUE)
                (*Count)++;

            break;
        default:
            break;
        }
    }

    if (*Count == 1)
        (*Count)++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                       CheckStringForLeftRightJustification

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

    pRtfString = (CRtfString*) m_arStrings[ns];

    if ((pRtfString->m_wLeftIndent - LeftDif) > m_max_dist && ns < m_wStringsCount - 1) {
        pRtfStringNext = (CRtfString*) m_arStrings[ns + 1];

        if (((pRtfString->m_wLeftIndent - LeftDif) < (RightFragm - LeftFragm) / 2)
                && ((pRtfString->m_wRightIndent - RightDif) < m_max_dist)
                && ((pRtfStringNext->m_wLeftIndent - LeftDif) < m_max_dist))
            return TRUE;
    }

    if ((pRtfString->m_wLeftIndent - LeftDif) < m_max_dist && ns > 1) {
        pRtfStringPrev = (CRtfString*) m_arStrings[ns - 1];

        if ((pRtfStringPrev->m_wRightIndent - RightDif) < m_max_dist)
            return TRUE;
    }

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 SetFlagBeginParagraphForLeftRightJustification

void CRtfFragment::SetFlagBeginParagraphForLeftRightJustification(int beg, int end) {
    CRtfString *pRtfStringPrev;
    CRtfString *pRtfString;

    for (int ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];

        if (ns == beg) {
            pRtfString->m_wFlagBeginParagraph = TRUE;
            continue;
        }

        pRtfStringPrev = (CRtfString*) m_arStrings[ns - 1];

        if (((pRtfString->m_wLeftIndent > 2 * m_max_dist) && (abs(pRtfString->m_wLeftIndent
                - pRtfStringPrev->m_wLeftIndent) > m_max_dist)) || (pRtfStringPrev->m_wRightIndent
                > 10 * m_max_dist) || ((pRtfStringPrev->m_wRightIndent > 5 * m_max_dist)
                && (pRtfStringPrev->m_LastChar == ';' || pRtfStringPrev->m_LastChar == '.'))
                || (CheckNumber(pRtfString->m_FirstChar) && (pRtfStringPrev->m_LastChar == ';'
                        || pRtfStringPrev->m_LastChar == '.')) || ((pRtfString->m_wLeftIndent > 3
                * m_max_dist / 2) && (pRtfStringPrev->m_LastChar == '.' || pRtfString->m_FirstChar
                == TIRE)) || (pRtfStringPrev->m_LastChar == '.' && (pRtfString->m_FirstChar == '-'
                || pRtfString->m_FirstChar == TIRE)) || (pRtfStringPrev->m_LastChar == '?'
                && (pRtfString->m_FirstChar == '-' || pRtfString->m_FirstChar == TIRE))
                || (pRtfStringPrev->m_LastChar == ':' && (pRtfString->m_FirstChar == '-'
                        || pRtfString->m_FirstChar == TIRE)) || (pRtfStringPrev->m_wRightIndent > 2
                * m_max_dist && pRtfString->m_FirstChar == TIRE))
            pRtfString->m_wFlagBeginParagraph = TRUE;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CheckNumber

Bool CRtfFragment::CheckNumber(uchar FirstChar) {
    const char* result = NULL;
    const char* MasNumber = "0123456789";
    result = strchr(MasNumber, FirstChar);

    if (result == NULL)
        return FALSE;

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CorrectIndents

void CRtfFragment::CorrectIndents(int beg, int end) {
    CRtfString* pRtfString;
    int16_t MinLeftIndent;
    int16_t MinRightIndent;
    int i(0);

    for (i = beg; i < end; i++) {
        pRtfString = (CRtfString*) m_arStrings[i];

        if (i == beg) {
            MinLeftIndent = pRtfString->m_wLeftIndent;
            MinRightIndent = pRtfString->m_wRightIndent;
            continue;
        }

        if (pRtfString->m_wFlagBeginParagraph == TRUE) {
            MinLeftIndent = MIN(pRtfString->m_wLeftIndent, MinLeftIndent);
            MinRightIndent = MIN(pRtfString->m_wRightIndent, MinRightIndent);
        }
    }

    for (i = beg; i < end; i++) {
        pRtfString = (CRtfString*) m_arStrings[i];

        if (pRtfString->m_wFlagBeginParagraph == TRUE) {
            if (abs(pRtfString->m_wLeftIndent - MinLeftIndent) < m_max_dist)
                pRtfString->m_wLeftIndent = MinLeftIndent;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 DeterminationOfLeftJustification

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

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CheckLeftJustification

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

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 SetFlagBeginParagraphForLeftJustification

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
        pRtfString = (CRtfString*) m_arStrings[ns];

        if (pRtfString->m_LastChar == '.')
            Count++;
    }

    if (Count > (4 * (end - beg) / 5))
        FlagStringParagraph = TRUE;

    if (Count >= ((end - beg) / 3))
        FlagStringParagraphSoft = TRUE;

    for (ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];

        if (ns == beg) {
            pRtfString->m_wFlagBeginParagraph = TRUE;
            continue;
        }

        pRtfStringPrev = (CRtfString*) m_arStrings[ns - 1];

        if (((pRtfString->m_wLeftIndent - LeftDif) > 2 * m_max_dist)
                || ((pRtfStringPrev->m_wRightIndent - RightDif) > (RightFragm - LeftFragm) / 3)
                || ((pRtfString->m_wLeftIndent > m_max_dist) && (pRtfString->m_FirstChar == '—'
                        || pRtfString->m_FirstChar == TIRE)) || (CheckNumber(
                pRtfString->m_FirstChar) && (pRtfStringPrev->m_LastChar == ';'
                || pRtfStringPrev->m_LastChar == '.')) || (pRtfStringPrev->m_LastChar == '.'
                && FlagStringParagraphSoft == TRUE && (pRtfStringPrev->m_wRightIndent - RightDif)
                > 5 * m_max_dist) || (pRtfStringPrev->m_LastChar == '.' && FlagStringParagraph
                == TRUE)) {
            pRtfStringPrev->m_bLineTransfer = FALSE;
            pRtfString->m_wFlagBeginParagraph = TRUE;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 DeterminationOfCentreJustification

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
    pRtfString = (CRtfString*) m_arStrings[beg];
    pRtfString->m_wFlagBeginParagraph = TRUE;
    PrintTheResult("\n ================== DeterminationOfCentreJustification ================== \n");
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 DeterminationOfRightJustification

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
        pRtfString = (CRtfString*) m_arStrings[ns];

        if (ns == beg) {
            pRtfString->m_wFlagBeginParagraph = TRUE;
            pRtfString->m_bLineTransfer = TRUE;
            continue;
        }

        pRtfString->m_bLineTransfer = TRUE;
        pRtfStringPrev = (CRtfString*) m_arStrings[ns - 1];

        if (pRtfStringPrev->m_LastChar == '.') {
            pRtfString->m_wFlagBeginParagraph = TRUE;
            pRtfStringPrev->m_bLineTransfer = FALSE;
        }
    }

    PrintTheResult("\n ================== DeterminationOfRightJustification ================== \n");
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 DeterminationOfListType

Bool CRtfFragment::DeterminationOfListType(int beg, int end) {
    CRtfString *pRtfString;
    uchar FlagListParagraph = 0;
    int32_t MinLeft, MaxLeft, MaxRight;
    int32_t CountMinLeft = 0, CountMaxLeft = 0, CountMaxRight = 0;
    uint16_t CountCentreEqual = 0;
    GetCountEqual(beg, end, &CountCentreEqual, RTF_TP_CENTER);

    if (CountCentreEqual == (end - beg))
        return FALSE;

    pRtfString = (CRtfString*) m_arStrings[0];
    MinLeft = MaxLeft = pRtfString->m_LeftBorder;
    MaxRight = pRtfString->m_RightBorder;
    int ns(0);

    //поиск тела списка
    for (ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];
        MinLeft = MIN(MinLeft, pRtfString->m_LeftBorder);
        MaxLeft = MAX(MaxLeft, pRtfString->m_LeftBorder);
        MaxRight = MAX(MaxRight, pRtfString->m_RightBorder);
    }

    if ((MaxLeft - MinLeft) > (MaxRight - MaxLeft) / 2)
        return FALSE;

    if ((MaxLeft - MinLeft) < m_max_dist)
        return FALSE;

    for (ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];

        if ((abs(MinLeft - pRtfString->m_LeftBorder) > 5 * m_max_dist) && (abs(MaxLeft
                - pRtfString->m_LeftBorder) > 5 * m_max_dist))
            return FALSE;

        if (abs(MinLeft - pRtfString->m_LeftBorder) < m_max_dist)
            CountMinLeft++;

        if (abs(MaxLeft - pRtfString->m_LeftBorder) < m_max_dist)
            CountMaxLeft++;

        if (abs(MaxRight - pRtfString->m_RightBorder) < m_max_dist)
            CountMaxRight++;
    }

    if ((CountMinLeft > CountMaxLeft) || ((CountMinLeft + CountMaxLeft) < 4 * (end - beg) / 5)
            || (CountMaxRight < (end - beg) / 2))
        return FALSE;

    SetParagraphAlignment(beg, end, RTF_TP_TYPE_LINE);

    for (ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];

        if ((ns == beg) || abs(MinLeft - pRtfString->m_LeftBorder) < m_max_dist)
            pRtfString->m_wFlagBeginParagraph = TRUE;
    }

    PrintTheResult("\n ================== DeterminationOfListType ================== \n");
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 DeterminationOfMixedFragment

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

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            ReInit

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
        pRtfString = (CRtfString*) m_arStrings[ns];
        pRtfString->m_wLeftBorderEqual = FALSE;
        pRtfString->m_wRightBorderEqual = FALSE;
        pRtfString->m_wCentreEqual = FALSE;
    }

    //  I. Поиск:       Левой(m_l_fragmentLocal) и правой(m_r_fragmentLocal) границ фрагмента
    for (ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];

        if (ns == beg) {
            if (!ns)
                pRtfString->m_wSpaceBefore = SectorInfo->VerticalOffsetFragmentInColumn;

            else {
                pRtfStringPrev = (CRtfString*) m_arStrings[ns - 1];
                pRtfWord = (CRtfWord*) pRtfStringPrev->m_arWords[0];
                pRtfCharFirst = pRtfWord->firstChar();
                top = pRtfCharFirst->idealRect().bottom();
                pRtfWord = (CRtfWord*) pRtfString->m_arWords[0];
                pRtfCharFirst = pRtfWord->firstChar();
                bottom = pRtfCharFirst->idealRect().top();
                pRtfString->m_wSpaceBefore = (uint16_t) (bottom - top);
            }

            pRtfString->m_wFlagBeginParagraph = TRUE;
        }

        else
            pRtfString->m_wSpaceBefore = 0;

        pRtfWord = (CRtfWord*) pRtfString->m_arWords[0];
        pRtfCharFirst = pRtfWord->firstChar();
        pRtfString->m_FirstChar = pRtfCharFirst->first().getChar();
        pRtfWord = (CRtfWord*) pRtfString->m_arWords[pRtfString->m_wWordsCount - 1];
        pRtfCharLast = pRtfWord->lastChar();
        pRtfString->m_LastChar = pRtfCharLast->first().getChar();
        pRtfString->m_LeftBorder = pRtfCharFirst->idealRect().left();
        pRtfString->m_RightBorder = pRtfCharLast->idealRect().right();
        m_l_fragmentLocal = MIN(m_l_fragmentLocal, (int16_t)pRtfCharFirst->idealRect().left());
        m_r_fragmentLocal = MAX(m_r_fragmentLocal, (int16_t)pRtfCharLast->idealRect().right());
    }

    // Присваиваются признаки равенства концов и середины соседних строк
    for (ns = beg + 1; ns < end; ns++) {
        pRtfStringPrev = (CRtfString*) m_arStrings[ns - 1];
        pRtfString = (CRtfString*) m_arStrings[ns];
        LeftDif = pRtfString->m_wLeftIndent - pRtfStringPrev->m_wLeftIndent;
        RightDif = pRtfString->m_wRightIndent - pRtfStringPrev->m_wRightIndent;
        CentreDif = pRtfString->m_wCentre - pRtfStringPrev->m_wCentre;

        if (abs(LeftDif) <= m_max_dist) {
            pRtfString->m_wLeftBorderEqual = TRUE;
            m_CountLeftEqual++;

            if (ns == beg) {
                pRtfStringPrev->m_wLeftBorderEqual = TRUE;
                m_CountLeftEqual++;
            }
        }

        if (abs(RightDif) <= m_max_dist) {
            pRtfString->m_wRightBorderEqual = TRUE;
            m_CountRightEqual++;

            if (pRtfString->m_wLeftBorderEqual == TRUE)
                m_CountLeftRightEqual++;

            if (ns == beg) {
                pRtfStringPrev->m_wRightBorderEqual = TRUE;
                m_CountRightEqual++;
                m_CountLeftRightEqual++;
            }
        }

        if ((abs(CentreDif) < m_max_dist) && ((LeftDif <= 0 && RightDif <= 0) || (LeftDif > 0
                && RightDif > 0)) && (abs(LeftDif) > m_max_dist / 2) && (abs(RightDif) > m_max_dist
                / 2)) {
            pRtfString->m_wCentreEqual = TRUE;
            m_CountCentreEqual++;

            if (ns == beg) {
                pRtfStringPrev->m_wCentreEqual = TRUE;
                m_CountCentreEqual++;
            }
        }
    }

    PrintTheResult("\n ================== ReInit ================== \n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 GetNextFragmentBegEnd

void CRtfFragment::GetNextFragmentBegEnd(int32_t* beg, int32_t* end, Bool* Flag) {
    CRtfString *pRtfString;
    int i;
    *end = *beg + 1;

    for (i = *end; i < m_wStringsCount; i++) {
        pRtfString = (CRtfString*) m_arStrings[i];

        if (pRtfString->m_Attr == TRUE) {
            *end = i;
            break;
        }
    }

    if ((*end >= (int32_t) m_wStringsCount) || (i >= (int32_t) m_wStringsCount)) {
        *end = (int32_t) m_wStringsCount;
        *Flag = FALSE;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 Done

void CRtfFragment::Done(void) {
    CheckOnceAgainImportancesFlagBeginParagraph();
    SetFirstLeftAndRightIndentOfParagraph();
    DefineLineTransfer();
}

//-------------------------------------------------------------------------------------------------
//---------------------------------  HELPER FUNCTIONS  --------------------------------------------
//-------------------------------------------------------------------------------------------------
void CRtfFragment::CalculationLengthAndCount(CRtfString* pRtfString, int32_t* CountChars,
        int32_t* LengthChars) {
    CRtfWord *pRtfWord;
    CRtfChar *pRtfChar;
    uint16_t CountWords;
    CountWords = pRtfString->m_wWordsCount;

    for (int i = 0; i < CountWords; i++) {
        pRtfWord = (CRtfWord*) pRtfString->m_arWords[i];
        int WCountChars = pRtfWord->charCount();

        for (int j = 0; j < WCountChars; j++) {
            pRtfChar = pRtfWord->charAt(j);
            (*LengthChars) += MAX(0, pRtfChar->idealRect().width());
            (*CountChars)++;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                 CheckOnceAgainImportancesFlagBeginParagraph

void CRtfFragment::CheckOnceAgainImportancesFlagBeginParagraph() {
    CRtfString *pRtfString;
    CRtfString *pRtfStringPrev;
    CRtfWord *pRtfWordPrev;
    CRtfWord *pRtfWord;
    CRtfChar *pRtfChar;
    uint16_t CountWords;
    uint16_t CountChars;
    int ns(0);

    for (ns = 1; ns < m_wStringsCount; ns++) {
        pRtfStringPrev = (CRtfString*) m_arStrings[ns - 1];
        pRtfString = (CRtfString*) m_arStrings[ns];

        // если выр. другая, то необходимо начать новый параграф
        if (pRtfString->m_wAlignment != pRtfStringPrev->m_wAlignment) {
            pRtfStringPrev->m_bLineTransfer = FALSE;
            pRtfString->m_wFlagBeginParagraph = TRUE;
        }
    }

    for (ns = 1; ns < m_wStringsCount; ns++) {
        pRtfStringPrev = (CRtfString*) m_arStrings[ns - 1];
        pRtfString = (CRtfString*) m_arStrings[ns];
        pRtfWordPrev = (CRtfWord*) pRtfStringPrev->m_arWords[0];
        pRtfWord = (CRtfWord*) pRtfString->m_arWords[0];

        if (pRtfString->m_wAlignment != RTF_TP_CENTER && abs(pRtfWord->realFontSize()
                - pRtfWordPrev->realFontSize()) > 1) {
            pRtfStringPrev->m_bLineTransfer = FALSE;
            pRtfString->m_wFlagBeginParagraph = TRUE;
        }
    }

    for (ns = 1; ns < m_wStringsCount; ns++) {
        pRtfStringPrev = (CRtfString*) m_arStrings[ns - 1];
        pRtfString = (CRtfString*) m_arStrings[ns];

        if (pRtfString->m_wFlagBeginParagraph == TRUE) {
            CountWords = pRtfStringPrev->m_wWordsCount;
            pRtfWord = (CRtfWord*) pRtfStringPrev->m_arWords[CountWords - 1];
            pRtfChar = pRtfWord->lastChar();

            if (pRtfChar->first().isHyphen() && pRtfChar->m_bFlg_spell_nocarrying) {
                if (pRtfString->m_wAlignment == pRtfStringPrev->m_wAlignment)
                    pRtfString->m_wFlagBeginParagraph = FALSE;

                else if (pRtfStringPrev->m_wAlignment == RTF_TP_LEFT_AND_RIGHT_ALLIGN
                        && pRtfString->m_wAlignment == RTF_TP_LEFT_ALLIGN) {
                    pRtfString->m_wAlignment = RTF_TP_LEFT_AND_RIGHT_ALLIGN;
                    pRtfStringPrev->m_bLineTransfer = FALSE;
                    pRtfString->m_wFlagBeginParagraph = FALSE;
                }
            }
        }
    }

    PrintTheResult(
            "\n ================== CheckOnceAgainImportancesFlagBeginParagraph ================== \n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                          SetFirstLeftAndRightIndentOfParagraph

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

    for (ns = 0; ns < m_wStringsCount; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];
        pRtfString->m_LengthStringInTwips = pRtfString->GetRealStringSize();
        pRtfString->m_wLeftIndent = (int16_t) ((int16_t) (pRtfString->m_wLeftIndent * Twips)
                + m_LeftOffsetFragmentFromVerticalColumn);
        pRtfString->m_wRightIndent = (int16_t) ((int16_t) (pRtfString->m_wRightIndent * Twips)
                + m_RightOffsetFragmentFromVerticalColumn);
        pRtfString->m_wRightIndent
                = MIN(pRtfString->m_wRightIndent,
                        m_WidthVerticalColumn - (pRtfString->m_LengthStringInTwips + pRtfString->m_wLeftIndent + pRtfString->m_wRightIndent));
    }

    for (ns = 0; ns < m_wStringsCount; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];

        if (pRtfString->m_wFlagBeginParagraph == TRUE) {
            if (pRtfString->m_wAlignment == RTF_TP_LEFT_ALLIGN) {
                Dif = MAX(0, m_WidthVerticalColumn - pRtfString->m_LengthStringInTwips);
                MinLeftIndent = pRtfString->m_wLeftIndent;

                for (i = ns + 1; i < m_wStringsCount; i++) {
                    pRtfStringNext = (CRtfString*) m_arStrings[i];

                    if (pRtfStringNext->m_wFlagBeginParagraph == TRUE
                            || pRtfStringNext->m_wAlignment != RTF_TP_LEFT_ALLIGN)
                        break;

                    MinLeftIndent = MIN(pRtfStringNext->m_wLeftIndent, MinLeftIndent);
                }

                if (m_WidthVerticalColumn > pRtfString->m_LengthStringInTwips) {
                    pRtfString->m_wFirstIndent = pRtfString->m_wLeftIndent - MinLeftIndent;

                    if (pRtfString->m_wFirstIndent < (twp_dist / 3))
                        pRtfString->m_wFirstIndent = 0;

                    pRtfString->m_wLeftIndent = MIN(Dif, MinLeftIndent);
                }

                else {
                    pRtfString->m_wLeftIndent = 0;
                    pRtfString->m_wFirstIndent = 0;
                }

                pRtfString->m_wRightIndent = 0;

                if (pRtfString->m_wLeftIndent < (twp_dist / 2))
                    pRtfString->m_wLeftIndent = 0;

                if (i == (ns + 1)) {
                    pRtfString->m_wFirstIndent = pRtfString->m_wLeftIndent;
                    pRtfString->m_wLeftIndent = 0;
                }

                continue;
            }

            if (pRtfString->m_wAlignment == RTF_TP_RIGHT_ALLIGN) {
                pRtfString->m_wLeftIndent = 0;
                pRtfString->m_wFirstIndent = 0;
                pRtfString->m_wRightIndent = 0;
                continue;
            }

            if (pRtfString->m_wAlignment == RTF_TP_TYPE_LINE) {
                if (ns + 1 < m_wStringsCount) {
                    pRtfStringNext = (CRtfString*) m_arStrings[ns + 1];

                    if (pRtfStringNext->m_wFlagBeginParagraph == FALSE
                            && pRtfStringNext->m_wAlignment == RTF_TP_TYPE_LINE) {
                        pRtfString->m_wFirstIndent = pRtfStringNext->m_wLeftIndent
                                - pRtfString->m_wLeftIndent;
                        pRtfString->m_wLeftIndent = pRtfStringNext->m_wLeftIndent;
                    }

                    else
                        pRtfString->m_wFirstIndent = 0;
                }

                else
                    pRtfString->m_wFirstIndent = 0;

                continue;
            }

            if (pRtfString->m_wAlignment == RTF_TP_LEFT_AND_RIGHT_ALLIGN) {
                MinLeftIndent = pRtfString->m_wLeftIndent;
                MinRightIndent = pRtfString->m_wRightIndent;

                for (i = ns + 1; i < m_wStringsCount; i++) {
                    pRtfStringNext = (CRtfString*) m_arStrings[i];

                    if (pRtfStringNext->m_wFlagBeginParagraph == TRUE
                            || pRtfStringNext->m_wAlignment != RTF_TP_LEFT_AND_RIGHT_ALLIGN)
                        break;

                    MinLeftIndent = MIN(pRtfStringNext->m_wLeftIndent, MinLeftIndent);
                    MinRightIndent = MIN(pRtfStringNext->m_wRightIndent, MinRightIndent);
                }

                if (MinLeftIndent < (twp_dist / 3))
                    MinLeftIndent = 0;

                pRtfString->m_wFirstIndent = pRtfString->m_wLeftIndent - MinLeftIndent;

                if (pRtfString->m_wFirstIndent < (twp_dist / 3))
                    pRtfString->m_wFirstIndent = 0;

                if (MinLeftIndent < twp_dist)
                    pRtfString->m_wLeftIndent = 0;

                else
                    pRtfString->m_wLeftIndent = MinLeftIndent;

                if (MinRightIndent < twp_dist)
                    pRtfString->m_wRightIndent = 0;

                if (i == (ns + 1)) {
                    if (MinLeftIndent > ((2 * twp_dist) / 3))
                        pRtfString->m_wLeftIndent = MinLeftIndent;

                    pRtfString->m_wFirstIndent = pRtfString->m_wLeftIndent;
                    pRtfString->m_wLeftIndent = 0;
                    pRtfString->m_wRightIndent = 0;
                }

                continue;
            }

            if (pRtfString->m_wAlignment == RTF_TP_CENTER) {
                MinLeftIndent = pRtfString->m_wLeftIndent;
                MinRightIndent = pRtfString->m_wRightIndent;

                for (i = ns; i < m_wStringsCount; i++) {
                    pRtfStringNext = (CRtfString*) m_arStrings[i];
                    MinLeftIndent = MIN(pRtfStringNext->m_wLeftIndent, MinLeftIndent);
                    MinRightIndent = MIN(pRtfStringNext->m_wRightIndent, MinRightIndent);

                    if (pRtfString->m_wAlignment != RTF_TP_CENTER)
                        break;
                }

                if (abs(MinLeftIndent - MinRightIndent) < (2 * m_max_dist))
                    MinLeftIndent = MinRightIndent = 0;

                pRtfString->m_wFirstIndent = 0;
                pRtfString->m_wLeftIndent = MinLeftIndent;
                pRtfString->m_wRightIndent = MinRightIndent;
            }
        }
    }

    PrintTheResult(
            "\n ================== SetFirstLeftAndRightIndentOfParagraph ================== \n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     DefineLineTransfer

void CRtfFragment::DefineLineTransfer() {
    CRtfString *pRtfString;
    int Count;

    for (int ns = 0; ns < m_wStringsCount; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];

        if (pRtfString->m_wFlagBeginParagraph == TRUE && (FlagLineTransfer
                || pRtfString->m_wAlignment == RTF_TP_CENTER)) {
            Count = GetCountLine(ns);
            SetLineTransfer(ns, ns + Count);
        }
    }

    PrintTheResult("\n ================== DefineLineTransfer ================== \n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     GetFlagLeft

Bool CRtfFragment::GetFlagLeft(int beg, int end) {
    CRtfString* pRtfString;
    int Count = 0;
    uint16_t CountLeftEqual = 0;
    Bool PriznakLeft = FALSE;

    for (int ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];

        if (pRtfString->m_LastChar == ',') {
            Count++;

            if (pRtfString->m_wRightIndent > (pRtfString->m_RightBorder - pRtfString->m_LeftBorder)
                    / 4)
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

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     GetFlagStrongLeft

Bool CRtfFragment::GetFlagStrongLeft(int beg, int end) {
    int Count = 0;
    CRtfString *pRtfString;

    for (int ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];

        if (pRtfString->m_LastChar == '.' || pRtfString->m_LastChar == ',')
            Count++;
    }

    if (Count == end - beg)
        return TRUE;

    if ((end - beg < 5) && Count > 2)
        return TRUE;

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     GetFlagRight

Bool CRtfFragment::GetFlagRight(int beg, int end) {
    CRtfString *pRtfString;
    int ns(0);

    for (ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];

        if (pRtfString->m_wRightIndent > m_max_dist)
            return FALSE;
    }

    for (ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];

        if (pRtfString->m_wLeftIndent > ((m_r_fragment - m_l_fragment) / 2))
            return TRUE;
    }

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     GetFlagCarry

Bool CRtfFragment::GetFlagCarry(int beg, int end) {
    CRtfString *pRtfString;

    for (int ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];

        if (pRtfString->m_FlagCarry == TRUE)
            return TRUE;
    }

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     GetFlagBigSpace

Bool CRtfFragment::GetFlagBigSpace(int beg, int end) {
    CRtfString* pRtfString;
    CRtfWord* pRtfWordPrev, *pRtfWordCur;
    CRtfChar* pRtfCharPrev, *pRtfCharCur;
    int FlagBigSpase = 0;
    int CountCharInPrevWord;

    for (int ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];

        for (int i = 1; i < pRtfString->m_wWordsCount; i++) {
            pRtfWordPrev = (CRtfWord*) pRtfString->m_arWords[i - 1];
            pRtfWordCur = (CRtfWord*) pRtfString->m_arWords[i];
            pRtfCharPrev = pRtfWordPrev->lastChar();
            pRtfCharCur = pRtfWordCur->firstChar();

            if ((pRtfCharCur->idealRect().left() - pRtfCharPrev->idealRect().right()) > 2
                    * m_max_dist)
                FlagBigSpase = 1;
        }
    }

    return FlagBigSpase;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   SetParagraphAlignment

void CRtfFragment::SetParagraphAlignment(int beg, int end, int AlignType) {
    CRtfString *pRtfString;
    int i;
    beg = MAX(0, beg);

    for (i = beg; i < end; i++) {
        pRtfString = (CRtfString*) m_arStrings[i];
        pRtfString->m_wAlignment = AlignType;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     GetCountLine

int CRtfFragment::GetCountLine(int beg) {
    CRtfString *pRtfString;
    int Count = 0;

    for (int ns = beg + 1; ns < m_wStringsCount; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];

        if (pRtfString->m_wFlagBeginParagraph == TRUE)
            break;

        else
            Count++;
    }

    return Count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     SetLineTransfer

void CRtfFragment::SetLineTransfer(int beg, int end) {
    CRtfString *pRtfString;

    for (int ns = beg; ns < end; ns++) {
        pRtfString = (CRtfString*) m_arStrings[ns];
        pRtfString->m_bLineTransfer = TRUE;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   PrintTheResult

void CRtfFragment::PrintTheResult(const char* header_str) {
    /*  CRtfString  *pRtfString;
     CRtfWord    *pRtfWord;
     CRtfChar    *pRtfCharFirst;
     CString     str;
     char* str;
     uint16_t        CountWord,CountChar;

     if( FlagDebugAlign )
     {
     LDPUMA_Console(header_str);
     for(int ns=0; ns < m_wStringsCount; ns++ )
     {
     str.Empty();
     pRtfString = (CRtfString*)m_arStrings[ns];
     CountWord  = pRtfString->m_wWordsCount;
     for(int i1=0; i1<CountWord; i1++)
     {
     pRtfWord  = pRtfString->m_arWords[i1];
     CountChar = pRtfWord->m_wCharsCount;
     for(int i2=0; i2<CountChar; i2++)
     {
     if(!i2)
     str+=' ';
     pRtfCharFirst = (CRtfChar*)pRtfWord->m_arChars[i2];
     str+=pRtfCharFirst->m_chrVersions[0].m_bChar;
     }
     }
     str+='\n';
     LDPUMA_Console(str);
     str.Format("N#=%4d , Alignment= %d, BeginParagraph= %d,FI=%4d, LI=%4d, RI=%4d, LBorderEqual=%d, RBorderEqual=%d, CentreEqual=%d, dist=%3d \n",
     ns, pRtfString->m_wAlignment, pRtfString->m_wFlagBeginParagraph,
     pRtfString->m_wFirstIndent,pRtfString->m_wLeftIndent,   pRtfString->m_wRightIndent,
     pRtfString->m_wLeftBorderEqual, pRtfString->m_wRightBorderEqual, pRtfString->m_wCentreEqual,
     m_max_dist);
     LDPUMA_Console(str);
     }
     }
     */}

}
