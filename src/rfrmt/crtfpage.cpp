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
#include "crtfchar.h"
#include "crtfsector.h"
#include "crtfword.h"
#include "crtffragment.h"
#include "crtfpage.h"
#include "crtffunc.h"
#include "crtfstring.h"
#include "creatertf.h"
#include "frmtpict.h"
#include "rfrmtoptions.h"
#include "formatoptions.h"

// common module
#include "common/debug.h"
// ced module
#include "ced/ced.h"
#include "ced/cedcolumn.h"
#include "ced/cedframe.h"
#include "ced/cedline.h"
#include "ced/cedpage.h"
#include "ced/cedparagraph.h"
#include "ced/cedsection.h"
// cpage module
#include "cpage/cpage.h"

#include "minmax.h"
#include "charsets.h"

extern uint32_t CountPict;
extern uint32_t CountTable;
uchar Frmt_CharSet = CHARSET_RUSSIAN;

namespace CIF
{

CRtfPage::CRtfPage(const std::string& imageName) :
    image_name_(imageName), ced_page_(NULL), bad_column_(false) {
    Count.RtfSectors = 0;
    Count.RtfTextFragments = 0;
    Count.RtfFrameTextFragments = 0;
    Count.RtfPictureFragments = 0;
    Count.RtfTableFragments = 0;
    Count.RtfStrings = 0;
    Count.RtfWords = 0;
    Count.RtfChars = 0;
    SetRect(&m_rect, 32000, 32000, 0, 0);
}

CRtfPage::~CRtfPage() {
    clearSectors();
    clearFragments();
}

void CRtfPage::addFragment(CRtfFragment * frag) {
    frag->setParent(this);
    m_arFragments.push_back(frag);
}

void CRtfPage::clearFragments() {
    for (FragmentList::iterator it = m_arFragments.begin(), e = m_arFragments.end(); it != e; ++it)
        delete *it;

    m_arFragments.clear();
}

void CRtfPage::clearSectors() {
    for (SectorList::iterator it = m_arSectors.begin(), e = m_arSectors.end(); it != e; ++it)
        delete *it;

    m_arSectors.clear();
}

Size CRtfPage::pageSize() const {
    return page_size_;
}

void CRtfPage::setFontMonospace(const std::string& name) {
    font_monospace_ = name;
}

void CRtfPage::setFontSans(const std::string& name) {
    font_sans_ = name;
}

void CRtfPage::setFontSerif(const std::string& name) {
    font_serif_ = name;
}

void CRtfPage::setFormatOptions(const FormatOptions& opts) {
    font_sans_ = opts.sansSerifName();
    font_serif_ = opts.serifName();
    font_monospace_ = opts.monospaceName();
    language_ = opts.language();
    unrecognized_char_ = (char) opts.unrecognizedChar();
}

void CRtfPage::setImageName(const std::string& name) {
    image_name_ = name;
}

void CRtfPage::setUnrecognizedChar(char ch) {
    unrecognized_char_ = ch;
}

void CRtfPage::setFragmentsInColumn(const CRtfFragment * cur_frag) {
    assert(cur_frag);

    for (size_t i = 0; i < m_arFragments.size(); i++) {
        CRtfFragment * frag = m_arFragments[i];

        if (frag->type() == cur_frag->type() && frag->rect() == cur_frag->rect())
            frag->setInColumn(true);
    }
}
void CRtfPage::initCedPage() {
    ced_page_ = new CEDPage;
    ced_page_->setImageName(image_name_);
    ced_page_->setUnrecognizedChar(unrecognized_char_);
    ced_page_->setLanguage(language_);
    ced_page_->setPageSize(page_size_);
    ced_page_->setPageBorder(Rect(Point(MargT, MargL), Point(MargB, MargR)));
    ced_page_->setPageNumber(1);

    // setting page info
    PAGEINFO PageInfo;
    Handle hCPAGE = CPAGE_GetHandlePage(CPAGE_GetCurrentPage());
    GetPageInfo(hCPAGE, &PageInfo);
    ced_page_->setTurn(PageInfo.Incline2048);
    ced_page_->setImageSize(Size(PageInfo.Width, PageInfo.Height));
    ced_page_->setImageDpi(Size(PageInfo.DPIX, PageInfo.DPIY));
}

//********* Чтение internal.vit
//*********  Частичное запольнения Page.
//*********  В Page записываютя:
//*********    текстовые фрагменты с содержимым
//*********    -- фреймы  ( врезки ),
//*********    -- обычные ( кандидаты на колонки )
//---------------------------------------
//*********    без содержимого
//*********    -- таблицы
//*********    -- картинки

void CRtfPage::readInternalFile(FILE *in) {
    int16_t tmp;
    rewind(in);
    fread(&tmp, 2, 1, in);
    m_wDpi = tmp;

    fread(&tmp, 2, 1, in);
    Count.RtfTextFragments = tmp;
    fread(&tmp, 2, 1, in);
    fread(&tmp, 2, 1, in);
    fread(&tmp, 2, 1, in);
    fread(&tmp, 2, 1, in);
    fread(&tmp, 2, 1, in);
    Count.RtfFrameTextFragments = 0;

    for (int nc = 0; nc < Count.RtfTextFragments; ++nc) {
        CRtfFragment * frag = CRtfFragment::read(in);
        addFragment(frag);
    }
}

void CRtfPage::addTables() {
    ::Rect16 RectPict;

    Count.RtfTableFragments = (uint16_t) CountTable;
    for (int i = 0; i < (int) CountTable; i++) {
        CRtfFragment * frag = new CRtfFragment;
        addFragment(frag);
        // commented by uliss
        //GetTableRect(i, &RectPict, (uint32_t*) &frag->m_wUserNumber);
        RtfAssignRect_CRect_Rect16(&frag->m_rect, &RectPict);
        frag->setType(FT_TABLE);
        if (RfrmtOptions::useFramesAndColumns()) {
            frag->m_wUserNumberForFormattedMode = frag->m_wUserNumber;
            frag->m_wUserNumber = i;
        }
    }
}

void CRtfPage::addPictures() {
    Count.RtfPictureFragments = (uint16_t) CountPict;

    for (int i = 0; i < (int) CountPict; i++) {
        CRtfFragment * frag = new CRtfFragment;
        addFragment(frag);
        Rect RectPict = GetPictRect(i, (uint32_t*) &frag->m_wUserNumber);
        frag->m_rect.top = RectPict.top();
        frag->m_rect.bottom = RectPict.bottom();
        frag->m_rect.left = RectPict.left();
        frag->m_rect.right = RectPict.right();
        frag->setType(FT_PICTURE);

        if (RfrmtOptions::useFramesAndColumns()) {
            frag->m_wUserNumberForFormattedMode = frag->m_wUserNumber;
            frag->m_wUserNumber = i;
        }
    }
}
//~ поиск межсекторных H_lines & межколонных V-Lines
void CRtfPage::AddLines() {
    CRtfSector* pRtfSector;
    CRtfSector* pRtfNextSector;
    RECT Rect;
    int16_t CountSectors = m_arSectors.size();

    for (int i = 0; i < CountSectors; i++) {
        if (i == 0) {
            pRtfSector = m_arSectors[i];
            pRtfSector->SectorInfo.PaperW = page_size_.width();
            Rect.left = 0;
            Rect.right = page_size_.width();
            Rect.top = 0;
            Rect.bottom = MAX(0, pRtfSector->m_rect.top);

            if (CheckLines(&Rect, FALSE, &pRtfSector->SectorInfo)) {
                pRtfSector = *m_arSectors.insert(m_arSectors.begin() + i, new CRtfSector());
                pRtfSector->m_bFlagLine = TRUE;
                pRtfSector->SectorInfo.PaperW = page_size_.width();
                CountSectors++;
                RtfAssignRect_CRect_CRect(&pRtfSector->m_rect, &Rect);
                RtfAssignRect_CRect_CRect(&pRtfSector->m_rectReal, &Rect);
            }
        } else
        //      if( i < CountSectors )
        {
            pRtfSector = m_arSectors[i - 1];
            pRtfSector->SectorInfo.PaperW = page_size_.width();
            pRtfNextSector = m_arSectors[i];
            Rect.left = 0;
            Rect.right = page_size_.width();
            Rect.top = pRtfSector->m_rect.bottom;
            Rect.bottom = pRtfNextSector->m_rect.top;

            if (CheckLines(&Rect, FALSE, &pRtfSector->SectorInfo)) {
                pRtfSector = *m_arSectors.insert(m_arSectors.begin() + i, new CRtfSector());
                pRtfSector->m_bFlagLine = TRUE;
                pRtfSector->SectorInfo.PaperW = page_size_.width();
                CountSectors++;
                RtfAssignRect_CRect_CRect(&pRtfSector->m_rect, &Rect);
                RtfAssignRect_CRect_CRect(&pRtfSector->m_rectReal, &Rect);
            }
        }
    }
}

Rect CRtfPage::bRect() const {
    return Rect(Point(m_rect.left, m_rect.top), Point(m_rect.right, m_rect.bottom));
}

void CRtfPage::sortByUserNumber() {
    uint32_t mas[500];
    int CountFragments = Count.RtfFrameTextFragments + Count.RtfTextFragments
            + Count.RtfTableFragments + Count.RtfPictureFragments;

    int indexMinUserNumber = 0;
    for (int i = 0; i < CountFragments; i++) {
        int MinUserNumber = 32000;
        bool FlagChange = false;
        CRtfFragment* frag = NULL;

        for (int j = 0; j < CountFragments; j++) {
            frag = m_arFragments[j];

            if (frag->m_wUserNumber < MinUserNumber) {
                MinUserNumber = frag->m_wUserNumber;
                indexMinUserNumber = j;
                FlagChange = true;
            }
        }

        if (FlagChange) {
            frag = m_arFragments[indexMinUserNumber];
            frag->m_wUserNumber = 32000;
            mas[indexMinUserNumber] = i + 1;
        }
    }

    for (int i = 0; i < CountFragments; i++) {
        m_arFragments.at(i)->m_wUserNumber = mas[i];
    }
}

//Режим работы: USE_FRAME_AND_COLUMN
//Разбиение страницы на сектора
//Секторы на горизантальные колонки
//Горизантальные колонки на вертикальные колонки
//Определение терминальности вертикальных колонок
Bool CRtfPage::FindPageTree(FILE *fpFileNameIn) {
    return PageTree(fpFileNameIn, this);
}

void CRtfPage::calcPageSize() {
    if (RfrmtOptions::useNone()) {// Фрагменты отписываются по пользовательским номерам
        calcPageSizeNone();
    } else if (RfrmtOptions::useFrames() || bad_column_) {// Все фрагменты фреймы
        calcPageSizeFrames();
    } else {// Фрагменты отписываются после изучения структуры страницы
        calcPageSizeCommon();
    }
}

void CRtfPage::calcPageSizeCommon() {
    int LeftPos = 32000, TopPos = 32000, RightPos = -32000, BottomPos = -32000;

    for (FragmentList::iterator it = m_arFragments.begin(); it != m_arFragments.end(); ++it) {
        LeftPos = MIN(LeftPos, (*it)->m_rect.left);
        TopPos = MIN(TopPos, (*it)->m_rect.top);
        RightPos = MAX(RightPos, (*it)->m_rect.right);
        BottomPos = MAX(BottomPos, (*it)->m_rect.bottom);
    }

    SetPaperSize(LeftPos, RightPos, TopPos, BottomPos, page_size_, &MargL, &MargR, &MargT, &MargB);
    InitMargL = MargL;
    InitMargR = MargR;
    InitMargT = MargT;
    InitMargB = MargB;
}

void CRtfPage::calcPageSizeFrames() {
    int LeftPos = 32000, TopPos = 32000, RightPos = -32000, BottomPos = -32000;
    CRtfSector * sector = new CRtfSector;
    m_arSectors.push_back(sector);

    for (FragmentList::iterator frag = m_arFragments.begin(); frag != m_arFragments.end(); ++frag) {
        LeftPos = MIN(LeftPos, (int16_t)(*frag)->m_rect.left);
        TopPos = MIN(TopPos, (int16_t)(*frag)->m_rect.top);
        RightPos = MAX(RightPos, (int16_t)(*frag)->m_rect.right);
        BottomPos = MAX(BottomPos, (int16_t)(*frag)->m_rect.bottom);
    }

    sector->m_rectReal.left = sector->m_rect.left = LeftPos;
    sector->m_rectReal.right = sector->m_rect.right = RightPos;
    sector->m_rectReal.top = sector->m_rect.top = TopPos;
    sector->m_rectReal.bottom = sector->m_rect.bottom = BottomPos;
    SetPaperSize(LeftPos, RightPos, TopPos, BottomPos, page_size_, &MargL, &MargR, &MargT, &MargB);
    InitMargL = MargL;
    InitMargR = MargR;
    InitMargT = MargT;
    InitMargB = MargB;
}

void CRtfPage::calcPageSizeNone() {
    MargL = DefMargL;
    MargR = DefMargR;
    MargT = DefMargT;
    MargB = DefMargB;

    const int width = std::max((int) DefaultWidthPage, static_cast<int> (maxFragmentWidth() + MargL
            + MargR));

    page_size_.setWidth(width);
    page_size_.setHeight(DefaultHeightPage);
}

int CRtfPage::maxFragmentWidth() const {
    int width = 0;
    for (FragmentList::const_iterator it = m_arFragments.begin(); it != m_arFragments.end(); it++) {
        width = std::max(width, (*it)->rect().width());
    }
    return width;
}

//////////////////////////////////////////////////////////////////////////////////////////
//           Вычисления ширены и высоты листа                                           //
//////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::SetPaperSize(int32_t LeftPos, int32_t RightPos, int32_t TopPos, int32_t BottomPos,
        Size& size, int32_t* MargL, int32_t* MargR, int32_t* MargT, int32_t* MargB) {
    int i, j;
    int32_t MargL2 = DefMargL / 2, MargL10 = DefMargL / 10, MargL0 = 0;
    int32_t MargR2 = DefMargR / 2, MargR10 = DefMargR / 10, MargR0 = 0;
    int32_t MargT2 = DefMargT / 2, MargT10 = DefMargT / 10, MargT0 = 0;
    int32_t MargB2 = DefMargB / 2, MargB10 = DefMargB / 10, MargB0 = 0;
    int32_t MasMargL[] = { DefMargL, MargL2, MargL10, MargL0 };
    int32_t MasMargR[] = { DefMargR, MargR2, MargR10, MargR0 };
    int32_t MasMargT[] = { DefMargT, MargT2, MargT10, MargT0 };
    int32_t MasMargB[] = { DefMargB, MargB2, MargB10, MargB0 };
    int32_t WidthPage[] = { 11906, 20163, 23812, 23812 }; // A4,Legal,A3,A2
    int32_t HeightPage[] = { 16838, 12242, 16838, 33676 }; // A4,Legal,A3,A2

    for (j = 0; j < 3; j++) {
        for (i = 0; i < 4; i++) {
            if (((RightPos + MasMargL[i] + MasMargR[i]) <= WidthPage[j]) && ((BottomPos
                    + MasMargT[i] + MasMargB[i]) <= HeightPage[j])) {
                *MargL = MasMargL[i];
                *MargR = MasMargR[i];

                if ((BottomPos + 2 * (MasMargT[i] + MasMargB[i])) >= HeightPage[j]) {
                    *MargT = MasMargT[3];
                    *MargB = MasMargB[3];
                } else {
                    *MargT = MasMargT[i];
                    *MargB = MasMargB[i];
                }

                size.setWidth(HeightPage[j]);
                size.setHeight(WidthPage[j]);
                return;
            }
        }

        for (i = 0; i < 4; i++) {
            if (((RightPos + MasMargL[i] + MasMargR[i]) <= HeightPage[j]) && ((BottomPos
                    + MasMargT[i] + MasMargB[i]) <= WidthPage[j])) {
                *MargL = MasMargL[i];
                *MargR = MasMargR[i];

                if ((BottomPos + 2 * (MasMargT[i] + MasMargB[i])) >= WidthPage[j]) {
                    *MargT = MasMargT[3];
                    *MargB = MasMargB[3];
                } else {
                    *MargT = MasMargT[i];
                    *MargB = MasMargB[i];
                }

                size.setWidth(HeightPage[j]);
                size.setHeight(WidthPage[j]);
                return;
            }
        }
    }

    *MargL = MargL2;
    *MargR = MargR2;
    *MargT = MargT2;
    *MargB = MargB2;
    size.setWidth(HeightPage[3]);
    size.setHeight(WidthPage[3]);
}

//////////////////////////////////////////////////////////////////////////////////////////
//            Моя коррекция кегля с учетом размера строки и заданного фонта             //
//////////////////////////////////////////////////////////////////////////////////////////
void CRtfPage::CorrectKegl(void) {
    CRtfFragment* pRtfFragment;
    CRtfString* pRtfString;
    CRtfWord* pRtfWord;
    CRtfChar *pRtfChar, *pLastChar, *pFirstChar;
    uint16_t CountTextFragments;
    uint16_t CountStrings;
    uint16_t CountWords;
    uint16_t CountChars;
    int16_t LenghtStr, Real_Size_Kegl;
    char tmp_str[MAX_BUFFER_SIZE];
    CountTextFragments = Count.RtfTextFragments + Count.RtfFrameTextFragments;

    for (int i = 0; i < CountTextFragments; i++) {
        pRtfFragment = m_arFragments[i];
        CountStrings = pRtfFragment->stringCount();

        for (int ns = 0; ns < CountStrings; ns++) {
            pRtfString = pRtfFragment->stringAt(ns);
            CountWords = pRtfString->wordCount();
            //Считаем длину получившейся строки
            int len = 0;

            for (int w = 0; w < CountWords; w++)
                len += pRtfString->wordAt(w)->charCount() + 1;

            //Выделяем буфер под неё
            char* TmpString = new char[len + 1];
            TmpString[0] = 0;

            //Формируем полную строку
            for (int nw = 0; nw < CountWords; nw++) {
                int nz;
                pRtfWord = pRtfString->wordAt(nw);
                CountChars = pRtfWord->charCount();

                for (nz = 0; nz < CountChars; nz++) {
                    pRtfChar = pRtfWord->charAt(nz);
                    tmp_str[nz] = pRtfChar->first().getChar();

                    if (!nz)
                        pRtfChar->setFontSize(MIN(pRtfChar->fontSize(), MaxFontSize));
                }

                if (nw < CountWords - 1)
                    tmp_str[nz++] = ' ';

                tmp_str[nz] = 0;
                strcat(TmpString, tmp_str);
            }

            pRtfWord = pRtfString->firstWord();
            pFirstChar = pRtfWord->firstChar();
            pRtfWord = pRtfString->lastWord();
            CountChars = pRtfWord->charCount();
            pLastChar = pRtfWord->lastChar();
            LenghtStr = (int16_t) (pLastChar->idealRect().right() - pFirstChar->idealRect().left());
            // adjust kegl to the text line real width (Microsoft function)
            Real_Size_Kegl = GetRealSizeKegl(TmpString, LenghtStr, pFirstChar->fontSize(),
                    pFirstChar->font());
            AddNewKegl(pLastChar->fontSize(), Real_Size_Kegl);
            delete[] TmpString;
        }
    }
}

void CRtfPage::ChangeKegl(void) {
    CRtfFragment* pRtfFragment;
    CRtfString* pRtfString;
    CRtfWord* pRtfWord;
    uint16_t CountTextFragments;
    uint16_t CountStrings;
    uint16_t CountWords;
    CountTextFragments = Count.RtfTextFragments + Count.RtfFrameTextFragments;

    // по частоте встречаемости выбираем преобразование из реал. в идеал. кегль
    for (int i = 0; i < CountTextFragments; i++) {
        pRtfFragment = m_arFragments[i];
        CountStrings = pRtfFragment->stringCount();

        for (int ns = 0; ns < CountStrings; ns++) {
            pRtfString = pRtfFragment->stringAt(ns);
            CountWords = pRtfString->wordCount();

            for (int nw = 0; nw < CountWords; nw++) {
                pRtfWord = pRtfString->wordAt(nw);

                if (CountStrings == 1)
                    pRtfWord->setRealFontSize(GetMinKegl(pRtfWord->idealFontSize()));
                else
                    pRtfWord->setRealFontSize(GetNewKegl(pRtfWord->idealFontSize()));
            }
        }
    }
}

void CRtfPage::AddNewKegl(int16_t OldKegl, int16_t NewKegl) {
    KEGL tmp;

    for (std::vector<KEGL>::iterator kegl = arKegls.begin(); kegl < arKegls.end(); kegl++) {
        if (OldKegl == kegl->OldKegl && kegl->NewKegl == NewKegl) {
            kegl->Count++;
            return;
        }
    }

    tmp.OldKegl = OldKegl;
    tmp.NewKegl = NewKegl;
    tmp.Count = 1;
    arKegls.push_back(tmp);
}

int16_t CRtfPage::GetNewKegl(int16_t OldKegl) {
    int16_t FlagChange = 0, Count = 0, tmpKegl, FlagPenalty = 1;

    for (std::vector<KEGL>::iterator kegl = arKegls.begin(); kegl < arKegls.end(); kegl++) {
        if (OldKegl == kegl->OldKegl) {
            if (OldKegl <= 14) {
                if (Count < kegl->Count) {
                    FlagChange = 1;
                    Count = kegl->Count;
                    tmpKegl = kegl->NewKegl;
                }
            } else {
                FlagChange = 1;
                tmpKegl = MIN(kegl->NewKegl, OldKegl);
            }
        }
    }

    if (FlagChange) {
        for (std::vector<KEGL>::iterator __kegl = arKegls.begin(); __kegl < arKegls.end(); __kegl++) {
            if (OldKegl == __kegl->OldKegl) {
                if ((__kegl->Count > (Count / 4)) && __kegl->NewKegl < tmpKegl) {
                    tmpKegl = __kegl->NewKegl;
                    FlagPenalty = 0;
                } else if (FlagPenalty && (__kegl->Count > (Count / 10)) && __kegl->NewKegl
                        < tmpKegl) {
                    tmpKegl--;
                    tmpKegl = MAX(tmpKegl, 4);
                }
            }
        }

        OldKegl = tmpKegl;
    }

    OldKegl = MAX(4, OldKegl);
    return OldKegl;
}

int16_t CRtfPage::GetMinKegl(int16_t OldKegl) {
    int16_t FlagChange = 0, tmpKegl = 72;

    for (std::vector<KEGL>::iterator kegl = arKegls.begin(); kegl < arKegls.end(); kegl++) {
        if (OldKegl == kegl->OldKegl) {
            FlagChange = 1;
            tmpKegl = MIN(kegl->NewKegl, tmpKegl);
        }
    }

    if (FlagChange)
        OldKegl = tmpKegl;

    return OldKegl;
}

CEDPage * CRtfPage::Write() {
    if (RfrmtOptions::useNone()) { // Фрагменты отписываются по пользовательским номерам
        writeUsingNone();
    } else if (RfrmtOptions::useFrames() || bad_column_) { // Все фрагменты фреймы
        writeUsingFrames();
    } else {
        writeUsingFramesAndColumns(); // Фрагменты отписываются после изучения структуры страницы
    }

    return ced_page_;
}

void CRtfPage::writeUsingNone() {
    calcPageSize();
    initCedPage();
    writeFonts();

    size_t CountSectors = Count.RtfFrameTextFragments + Count.RtfTextFragments
            + Count.RtfTableFragments + Count.RtfPictureFragments;

    for (size_t i = 0; i < CountSectors; i++) {
        m_nCurSectorNumber = i;
        int FragmentType = FT_TEXT;
        int InGroupNumber = i;
        int NumberCurrentFragment = getFlagAndNumberFragment(&FragmentType, &InGroupNumber);
        CRtfSector * sector = new CRtfSector;
        sector->SectorInfo.userNum = NumberCurrentFragment;
        sector->m_VTerminalColumnNumber = 1;
        m_arSectors.push_back(sector);
        writeSectorsHeader(i);
        CRtfFragment * frag = m_arFragments[NumberCurrentFragment];

        if (FragmentType == FT_TABLE) {
            //pRtfFragment->FWriteTable(InGroupNumber, &pRtfSector->SectorInfo, FOT_SINGLE);
        } else if (FragmentType == FT_PICTURE) {
            frag->FWritePicture(InGroupNumber, &sector->SectorInfo, FOT_SINGLE);
        } else {
            frag->setParent(this);
            frag->writeText(&sector->SectorInfo);
        }
    }
}

// Все фрагменты фреймы
void CRtfPage::writeUsingFrames() {
    calcPageSize();
    initCedPage();
    writeFonts();

    int InGroupNumber;

    int CountFragments = Count.RtfFrameTextFragments + Count.RtfTextFragments
            + Count.RtfTableFragments + Count.RtfPictureFragments;
    writeSectorsHeader(0);
    CRtfSector * pRtfSector = m_arSectors.at(0);
    SectorInfo * SectorInfo = &pRtfSector->SectorInfo;
    SectorInfo->Offset.set(0, 0);

    if (CountFragments) {
        EDSIZE interval;
        interval.cx = 0;
        interval.cy = 0;
        Rect playout;
        Rect indent;

        CEDParagraph * ced_par = CED_CreateParagraph(SectorInfo->hEDSector, SectorInfo->hObject,
                ALIGN_LEFT, indent, SectorInfo->userNum, -1, interval, playout, Color::null(),
                Color::null(), -1);
        CEDLine * ced_str = new CEDLine(NULL, false, 6);
        ced_par->addLine(ced_str);
    }

    for (int i = 0; i < CountFragments; i++) {
        CRtfFragment * frag = m_arFragments[i];

        if (frag->type() == FT_TABLE) {
            InGroupNumber = i - (Count.RtfFrameTextFragments + Count.RtfTextFragments);
            //            pRtfFragment->FWriteTable(InGroupNumber, SectorInfo, FOT_FRAME);
        } else if (frag->type() == FT_PICTURE) {
            InGroupNumber = i - (Count.RtfFrameTextFragments + Count.RtfTextFragments
                    + Count.RtfTableFragments);
            frag->FWritePicture(InGroupNumber, SectorInfo, FOT_FRAME);
        } else {
            Rect EdFragmRect(Point(frag->m_rect.left, frag->m_rect.top), Point(frag->m_rect.right,
                    frag->m_rect.bottom));
            // TODO uliss: hardcoded values
            SectorInfo->hObject = SectorInfo->hEDSector->createFrame(SectorInfo->hColumn,
                    EdFragmRect, 0x22, -1, 86, 43);

            SectorInfo->FlagOverLayed = FALSE;
            frag->setParent(this);
            frag->writeText(SectorInfo);
        }
    }
}
//табл. помещаются в существующие сектора как frame (м.б.изменение размера колонки)
// или создаются для них новые сектора(терминал.колонка)
void CRtfPage::ToPlacePicturesAndTables(void) {
    for (FragmentList::iterator frag_it = m_arFragments.begin(); frag_it != m_arFragments.end(); ++frag_it) {
        if ((*frag_it)->type() != FT_PICTURE && (*frag_it)->type() != FT_TABLE)
            continue;

        CRtfSector * str;
        const size_t CountSectors = m_arSectors.size();

        //страница пустая:создается новый сектор для них
        if (CountSectors == 0) {
            m_arSectors.push_back(new CRtfSector());
            str = m_arSectors.front();
            str->m_rect = (*frag_it)->m_rect;
            str->m_rectReal = (*frag_it)->m_rect;
            str->ToPlacePicturesAndTables(*frag_it);
            continue;
        }

        //картина или таблица ниже последного сектора:создается новый сектор для них
        str = m_arSectors.back();

        if ((*frag_it)->m_rect.top >= str->m_rectReal.bottom) {
            m_arSectors.push_back(new CRtfSector());
            str = m_arSectors.back();
            str->m_rect = (*frag_it)->m_rect;
            str->m_rectReal = (*frag_it)->m_rect;
            str->ToPlacePicturesAndTables(*frag_it);
            continue;
        }

        for (size_t j = 0; j < CountSectors; j++) {//картина или таблица между секторами:создается новый сектор для них
            str = m_arSectors[j];

            if ((*frag_it)->m_rect.bottom <= str->m_rectReal.top) {
                str = *m_arSectors.insert(m_arSectors.begin() + j, new CRtfSector());
                str->m_rect = (*frag_it)->m_rect;
                str->m_rectReal = (*frag_it)->m_rect;
                str->ToPlacePicturesAndTables(*frag_it);
                break;
            } else {//картина или таблица внутри сектора: добавляетс
                if (((*frag_it)->m_rect.top < str->m_rectReal.top && (*frag_it)->m_rect.bottom
                        > str->m_rectReal.top) || ((*frag_it)->m_rect.top >= str->m_rectReal.top
                        && (*frag_it)->m_rect.top < str->m_rectReal.bottom)) {
                    str->ToPlacePicturesAndTables((*frag_it));
                    break;
                }
            }
        }
    }
}

// Фрагменты отписываются после изучения структуры страницы
void CRtfPage::writeUsingFramesAndColumns() {
    ToPlacePicturesAndTables();
    calcPageSize();
    initCedPage();
    writeFonts();

    AddLines();

    //Считаем расстояния между секциями (кроме последней)
    for (size_t i = 0; i < m_arSectors.size() - 1; i++) {
        CRtfSector * sector = m_arSectors[i];
        CRtfSector * next_sector = m_arSectors[i + 1];
        sector->SectorInfo.InterSectorDist = freeSpaceBetweenSectors(sector, next_sector);
    }

    //Отписываем каждую секцию
    for (size_t i = 0; i < m_arSectors.size(); i++) {
        m_nCurSectorNumber = i;
        CRtfSector * sector = m_arSectors[i];
        sector->setPage(this);

        if (sector->m_bFlagLine == FALSE)
            sector->calcSector();

        writeSectorsHeader(i);
        sector->Write();
    }
}

// чтобы дать больший, чем default, вертикальный offset of the section
int CRtfPage::freeSpaceBetweenSectors(CRtfSector * first, CRtfSector * second) {
    Rect free_place(Point(first->m_rect.left, first->m_rect.bottom + 1), //
            Point(first->m_rect.right, second->m_rect.top - 1));

    if (free_place.height() <= 0)
        return 0;

    size_t CountFragments = m_arFragments.size(); //это множество фрагментов pRtfNextSector

    for (size_t i = 0; i < CountFragments; i++) {
        CRtfFragment * frag = m_arFragments[i];

        if (frag->m_rect.bottom <= free_place.top() || //
                frag->m_rect.right <= free_place.left() || //
                frag->m_rect.top >= free_place.bottom() || //
                frag->m_rect.left >= free_place.right())
            continue;

        if (frag->m_rect.top >= free_place.top() && frag->m_rect.top <= free_place.bottom())
            free_place.setBottom(frag->m_rect.top);
    }

    return std::max(0, free_place.height());
}

void CRtfPage::writeFonts() {
    ced_page_->addFont(FontEntry(0, FF_SWISS, Frmt_CharSet, font_sans_));
    ced_page_->addFont(FontEntry(1, FF_ROMAN, Frmt_CharSet, font_serif_));
    ced_page_->addFont(FontEntry(2, FF_MODERN, Frmt_CharSet, font_monospace_));
    ced_page_->addFont(FontEntry(3, FF_SWISS, Frmt_CharSet, "Arial Narrow"));
}

void CRtfPage::writeSectorsHeader(int i) {
    int CountHTerminalColumns;
    int EDCountHTerminalColumns;
    CIF::Rect border;
    EDCOL *pEDColumnFirst, *pEDColumn;

    CRtfSector * sector = m_arSectors[i];

    if (i > 0) //!!!Art
        sector->SectorInfo.Offset.ry() = m_arSectors[i - 1]->m_rectReal.bottom
                + m_arSectors[i - 1]->SectorInfo.InterSectorDist;//!!!Art
    else
        //!!!Art
        sector->SectorInfo.Offset.ry() = sector->m_rectReal.top;

    sector->SectorInfo.FlagOneString = FALSE;
    sector->SectorInfo.PaperW = page_size_.width();
    sector->SectorInfo.PaperH = page_size_.height();

    // m_bFlagLine (есть линии) => не пытаться сдвигать margL для красоты
    if (RfrmtOptions::useFramesAndColumns() && sector->m_bFlagLine == FALSE) {
        sector->GetCountAndRightBoundVTerminalColumns();
        CountHTerminalColumns = sector->m_arWidthTerminalColumns.size();

        if (CountHTerminalColumns) {
            if (sector->m_FlagOneString == FALSE)
                MargL = sector->m_arRightBoundTerminalColumns[0];
            else {
                MargL = MIN(InitMargL, sector->m_arRightBoundTerminalColumns[0]);
                sector->SectorInfo.FlagOneString = TRUE;
            }
        } else
            MargL = MAX(sector->m_rectReal.left, 0);
    }

    if (RfrmtOptions::useFramesAndColumns() && sector->m_bFlagLine == FALSE) {
        if (CountHTerminalColumns) {
            if (sector->m_FlagOneString == FALSE)
                MargR = page_size_.width()
                        - (sector->m_arRightBoundTerminalColumns[CountHTerminalColumns - 1]
                                + sector->m_arWidthTerminalColumns[CountHTerminalColumns - 1]);
            else
                MargR = MIN(InitMargR, page_size_.width()
                        - (sector->m_arRightBoundTerminalColumns[CountHTerminalColumns - 1]
                                + sector->m_arWidthTerminalColumns[CountHTerminalColumns - 1]));
        } else
            MargR = page_size_.width() - sector->m_rectReal.right;
    }

    sector->SectorInfo.Offset.rx() = MargL;
    sector->SectorInfo.MargL = MargL;
    sector->SectorInfo.MargR = MargR;

    border.setLeft(MargL);
    border.setTop(MargT);
    border.setRight(MargR);
    border.setBottom(MargB);

    if (RfrmtOptions::useFramesAndColumns())
        sector->SectorInfo.userNum = -1;
    else
        CountHTerminalColumns = 0;

    if (RfrmtOptions::useFramesAndColumns() && sector->m_bFlagLine == TRUE)
        CountHTerminalColumns = 0;

    EDCountHTerminalColumns = CountHTerminalColumns;

    if (!EDCountHTerminalColumns)
        EDCountHTerminalColumns = 1;

    pEDColumnFirst = pEDColumn = (EDCOL*) malloc(EDCountHTerminalColumns * sizeof(EDCOL));

    if (!pEDColumn)
        return;

    if (!CountHTerminalColumns) {
        pEDColumn->width = page_size_.width() - (MargL + MargR);
        pEDColumn->space = 0;
    }

    for (int j = 0; j < CountHTerminalColumns; j++) {
        if (RfrmtOptions::useFramesAndColumns() && sector->SectorInfo.FlagOneString == TRUE)
            pEDColumn->width = page_size_.width() - (MargL + MargR);
        else
            pEDColumn->width = sector->m_arWidthTerminalColumns[j];

        if (j < CountHTerminalColumns - 1) {
            pEDColumn->space = sector->m_arRightBoundTerminalColumns[j + 1]
                    - (sector->m_arRightBoundTerminalColumns[j] + pEDColumn->width);
        } else
            pEDColumn->space = 0;

        pEDColumn++;
    }

    sector->m_hEDSector = CED_CreateSection(ced_page_, border, -1, EDCountHTerminalColumns,
            pEDColumnFirst, 0, page_size_.width(), page_size_.height(), 0, -1, -1);
    sector->SectorInfo.hEDSector = sector->m_hEDSector;
    sector->SectorInfo.hEDPage = ced_page_;
    sector->SectorInfo.hFirstColumn = sector->SectorInfo.hEDSector->createColumn();
    sector->SectorInfo.hColumn = sector->SectorInfo.hFirstColumn;
    sector->SectorInfo.hObject = sector->SectorInfo.hFirstColumn;
    free(pEDColumnFirst);
}

int CRtfPage::getFlagAndNumberFragment(int * FragmentType, int * InGroupNumber) {
    *FragmentType = FT_TEXT;
    int i = *InGroupNumber;
    int CountT = Count.RtfTextFragments + Count.RtfFrameTextFragments;
    int CountTT = CountT + Count.RtfTableFragments;
    int CountTTP = CountTT + Count.RtfPictureFragments;

    for (int j = 0; j < CountTTP; j++) {
        CRtfFragment * frag = m_arFragments[j];

        if ((uint32_t) (i + 1) == frag->m_wUserNumber) {
            i = j;
            break;
        }
    }

    if (i < CountT) {
        *InGroupNumber = i;
        *FragmentType = FT_TEXT;
    } else {
        if (i < CountTT) {
            *InGroupNumber = i - CountT;
            *FragmentType = FT_TABLE;
        } else {
            *InGroupNumber = i - CountTT;
            *FragmentType = FT_PICTURE;
        }
    }

    return i;
}

}

